#!/usr/bin/env python3
import json
import subprocess
import sys

from utils import ROOT, SETUP_FILE, CONFIGS, PLATFORMS, USAGE, host_platform, artifact_path

LAUNCH_FILE = ROOT / ".vscode" / "launch.json"
COMPILE_COMMANDS_LINK = ROOT / "compile_commands.json"


def write_launch_json(preset):
    """Write the debugger config pointing at the active preset's artifact."""
    program = artifact_path(preset)
    relative = program.relative_to(ROOT).as_posix()

    LAUNCH_FILE.parent.mkdir(parents=True, exist_ok=True)
    launch = {
        "version": "2.0.0",
        "configurations": [
            {
                "name": f"Launch ({preset})",
                "type": "lldb",
                "request": "launch",
                "program": f"${{workspaceFolder}}/{relative}",
                "cwd": f"${{workspaceFolder}}/_Output/{preset}",
                "preLaunchTask": "Build"
            }
        ]
    }
    LAUNCH_FILE.write_text(json.dumps(launch, indent=4) + "\n")


def link_compile_commands(preset):
    """Point a stable repo-root path at the active preset's compilation database.

    IntelliSense needs this to know about IS_PLATFORM_* / IS_CONFIG_* and the include
    paths; without it every #if IS_PLATFORM_MACOS block greys out as inactive. The link
    keeps .vscode/settings.json free of the preset name, which changes per platform.
    """
    target = ROOT / "_ProjectFiles" / "build" / preset / "compile_commands.json"

    if COMPILE_COMMANDS_LINK.is_symlink() or COMPILE_COMMANDS_LINK.exists():
        COMPILE_COMMANDS_LINK.unlink()

    try:
        COMPILE_COMMANDS_LINK.symlink_to(target)
    except OSError as error:
        # Windows needs Developer Mode or elevation for symlinks; not fatal.
        print(f"Could not link compile_commands.json ({error})")


def parse_args(argv) -> tuple:
    """Resolve argv into (config, platform), or exit with usage on bad input."""
    args = [a.lower() for a in argv]

    if not args:
        print(USAGE)
        sys.exit(1)

    config = next((a for a in args if a in CONFIGS), None)
    platform = next((a for a in args if a in PLATFORMS), None)
    unknown = [a for a in args if a not in CONFIGS and a not in PLATFORMS]

    if unknown:
        print(f"Unknown argument(s): {', '.join(unknown)}")
        print(USAGE)
        sys.exit(1)

    if config is None:
        print("Missing configuration.")
        print(USAGE)
        sys.exit(1)

    return config, platform or host_platform()


def main():
    config, platform = parse_args(sys.argv[1:])
    preset = f"{platform}-{config}"

    SETUP_FILE.parent.mkdir(parents=True, exist_ok=True)
    SETUP_FILE.write_text(
        json.dumps({"config": config, "platform": platform, "preset": preset}, indent=2) + "\n")
    write_launch_json(preset)
    print(f"Saved setup: {preset}")

    result = subprocess.run(["cmake", "--preset", preset, "-Wno-dev"], cwd=ROOT)
    if result.returncode == 0:
        link_compile_commands(preset)

    sys.exit(result.returncode)


if __name__ == "__main__":
    main()
