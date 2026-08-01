#!/usr/bin/env python3
import json
import subprocess
import sys

from utils import ROOT, SETUP_FILE

LAUNCH_FILE = ROOT / ".vscode" / "launch.json"
COMPILE_COMMANDS_LINK = ROOT / "compile_commands.json"
CONFIGS = ("debug", "dev", "release")


def platform_name():
    if sys.platform == "darwin":
        return "macos"
    if sys.platform.startswith("linux"):
        return "linux"
    return "windows"


def prompt_choice(label, choices, default):
    while True:
        print(f"{label}? [{'/'.join(choices)}] (default: {default}): ", end="", flush=True)
        answer = input().strip().lower() or default
        if answer in choices:
            return answer
        print(f"Invalid choice: {answer}")


def write_launch_json(preset):
    ext = ".exe" if sys.platform == "win32" else ""
    LAUNCH_FILE.parent.mkdir(parents=True, exist_ok=True)
    launch = {
        "version": "2.0.0",
        "configurations": [
            {
                "name": f"Launch ({preset})",
                "type": "lldb",
                "request": "launch",
                "program": f"${{workspaceFolder}}/_Output/{preset}/JupiterEngine{ext}",
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


def main():
    config = sys.argv[1].lower() if len(sys.argv) > 1 else prompt_choice("Configuration", CONFIGS, "dev")
    if config not in CONFIGS:
        print(f"Invalid configuration: {config} (expected one of {', '.join(CONFIGS)})")
        sys.exit(1)

    preset = f"{platform_name()}-{config}"
    SETUP_FILE.parent.mkdir(parents=True, exist_ok=True)
    SETUP_FILE.write_text(json.dumps({"config": config, "preset": preset}, indent=2) + "\n")
    write_launch_json(preset)
    print(f"Saved setup: {preset}")

    result = subprocess.run(["cmake", "--preset", preset, "-Wno-dev"], cwd=ROOT)
    if result.returncode == 0:
        link_compile_commands(preset)

    sys.exit(result.returncode)


if __name__ == "__main__":
    main()
