#!/usr/bin/env python3
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path

from utils import (ROOT, SETUP_FILE, PROJECTS, CONFIGS, PLATFORMS, USAGE, ANDROID_NDK_VERSION,
                   android_ndk_root, bundle_id, host_platform, artifact_path, output_dir)

LAUNCH_FILE = ROOT / ".vscode" / "launch.json"
COMPILE_COMMANDS_LINK = ROOT / "_ProjectFiles" / "compile_commands.json"
ANDROID_TEMPLATE = ROOT / "Build" / "Android" / "Template"


def write_launch_json(preset):
    """Write the debugger config pointing at the active preset's artifact.

    iOS and Android get no configuration: lldb cannot launch a device app from the host,
    so F5 would only ever fail. Those targets go through Scripts/run.py, which drives
    devicectl / adb instead.
    """
    LAUNCH_FILE.parent.mkdir(parents=True, exist_ok=True)

    if preset.startswith(("ios", "android")):
        LAUNCH_FILE.write_text(json.dumps({"version": "2.0.0", "configurations": []},
                                          indent=4) + "\n")
        print("Note: F5 is unavailable for this target. Use the Run task or py Scripts/run.py")
        return

    # A project outside the repo has no workspace-relative spelling, so it falls back to absolute.
    artifact = artifact_path(preset)
    try:
        program = f"${{workspaceFolder}}/{artifact.relative_to(ROOT).as_posix()}"
        cwd = f"${{workspaceFolder}}/{output_dir(preset).relative_to(ROOT).as_posix()}"
    except ValueError:
        program = artifact.as_posix()
        cwd = output_dir(preset).as_posix()

    launch = {
        "version": "2.0.0",
        "configurations": [
            {
                "name": f"Launch ({preset})",
                "type": "lldb",
                "request": "launch",
                "program": program,
                "cwd": cwd,
                "preLaunchTask": "Build"
            }
        ]
    }
    LAUNCH_FILE.write_text(json.dumps(launch, indent=4) + "\n")


def instantiate_android_project(project):
    """Copy Build/Android/Template over the project's gradle tree and fill in its names.

    Copied over, never wiped: CMake writes libmain.so and stage_assets.py writes assets into
    app/src/main/, and both must survive a re-run. The tree is generated -- edit the template,
    not the copy.
    """
    destination = project / "_ProjectFiles" / "android"
    shutil.copytree(ANDROID_TEMPLATE, destination, dirs_exist_ok=True)

    replacements = {
        "@JUPITER_APP_NAME@": project.name,
        "@JUPITER_APPLICATION_ID@": bundle_id(),
        "@JUPITER_SDL_JAVA_DIR@": (ROOT / "Vendor" / "SDL3" / "Android").as_posix(),
    }
    for file in (destination / "settings.gradle",
                 destination / "app" / "build.gradle",
                 destination / "app" / "src" / "main" / "AndroidManifest.xml"):
        text = file.read_text()
        for token, value in replacements.items():
            text = text.replace(token, value)
        file.write_text(text)


def relative_project(project) -> str:
    """Return how setup.json should spell a project path.

    Relative to the repo for the ones under Projects/, so the file stays portable; absolute for
    anything outside it, which pathlib rejoins unchanged.
    """
    try:
        return project.relative_to(ROOT).as_posix()
    except ValueError:
        return project.as_posix()


def link_compile_commands(preset, project):
    """Point a stable path at the active preset's compilation database.

    IntelliSense needs it to see IS_PLATFORM_* and the include paths; the fixed location
    keeps the preset name -- and now the project -- out of .vscode/settings.json.
    """
    target = project / "_ProjectFiles" / "build" / preset / "compile_commands.json"

    if COMPILE_COMMANDS_LINK.is_symlink() or COMPILE_COMMANDS_LINK.exists():
        COMPILE_COMMANDS_LINK.unlink()

    try:
        COMPILE_COMMANDS_LINK.symlink_to(target)
        return
    except OSError:
        # WinError 1314: CreateSymbolicLinkW needs Developer Mode or elevation. A hard link is
        # no use either -- CMake rewrites the database through a temp file and renames it, which
        # leaves any extra hard link pointing at the previous contents.
        pass

    try:
        shutil.copy2(target, COMPILE_COMMANDS_LINK)
        print("Copied compile_commands.json (no symlink privilege). Re-run Setup after adding "
              "or removing a source file.")
    except OSError as error:
        print(f"Could not publish compile_commands.json ({error})")


def resolve_project(name) -> Path:
    """Resolve a project argument to its directory, or exit if it names no project.

    A bare name is looked up under Projects/, which is where they normally live; anything with a
    separator is taken as a path, so a project can sit outside the repo.
    """
    candidate = Path(name)
    if candidate.parent == Path("."):
        candidate = PROJECTS / name
    elif not candidate.is_absolute():
        candidate = ROOT / candidate

    candidate = candidate.resolve()
    if not (candidate / "CMakeLists.txt").exists():
        print(f"No project at '{candidate}': a project needs a CMakeLists.txt")
        sys.exit(1)

    return candidate


def parse_args(argv) -> tuple:
    """Resolve argv into (config, platform, project dir), or exit with usage on bad input.

    Config and platform are matched case-insensitively; the project keeps the case it was typed
    in, because it is a path and only this filesystem is forgiving about that.
    """
    if not argv:
        print(USAGE)
        sys.exit(1)

    config = next((a.lower() for a in argv if a.lower() in CONFIGS), None)
    platform = next((a.lower() for a in argv if a.lower() in PLATFORMS), None)

    # Whatever is left is the project. Never inferred from the last run: which project you are
    # building decides the binary, the bundle id and where _Output goes, so it is said every time.
    rest = [a for a in argv if a.lower() not in CONFIGS and a.lower() not in PLATFORMS]
    if len(rest) > 1:
        print(f"Expected one project, got: {', '.join(rest)}")
        print(USAGE)
        sys.exit(1)

    if config is None:
        print("Missing configuration.")
        print(USAGE)
        sys.exit(1)

    if not rest:
        print("Missing project.")
        print(USAGE)
        sys.exit(1)

    return config, platform or host_platform(), resolve_project(rest[0])


def main():
    config, platform, project = parse_args(sys.argv[1:])
    preset = f"{platform}_{config}"

    # Configure first, and record nothing until it succeeds. Writing setup.json up front left
    # build.py and run.py pointed at a preset whose build tree the failed configure never created,
    # so the next Build reported a CMake error for a config nobody had chosen.
    #
    # The preset reads JUPITER_PROJECT_DIR to place the build tree; CMake itself reads the cache
    # variable, so a ninja-triggered reconfigure needs no environment at all.
    environment = {**os.environ, "JUPITER_PROJECT_DIR": str(project)}
    if platform == "android":
        ndk = android_ndk_root()
        if not ndk.is_dir():
            print(f"NDK not found at '{ndk}'. Install it: "
                  f'sdkmanager --install "ndk;{ANDROID_NDK_VERSION}"')
            sys.exit(1)
        environment["ANDROID_NDK_ROOT"] = str(ndk)

    result = subprocess.run(["cmake", "--preset", preset, "-Wno-dev",
                             f"-DJUPITER_PROJECT={project.as_posix()}"], cwd=ROOT, env=environment)
    if result.returncode != 0:
        sys.exit(result.returncode)

    SETUP_FILE.parent.mkdir(parents=True, exist_ok=True)
    SETUP_FILE.write_text(json.dumps({"config": config, "platform": platform, "preset": preset,
                                      "project": relative_project(project)}, indent=2) + "\n")
    write_launch_json(preset)
    link_compile_commands(preset, project)
    if platform == "android":
        instantiate_android_project(project)
    print(f"Saved setup: {preset} ({project.name})")


if __name__ == "__main__":
    main()
