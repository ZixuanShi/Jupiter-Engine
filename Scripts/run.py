#!/usr/bin/env python3
# macOS ships Python 3.9, where `str | None` in an annotation is a runtime TypeError.
# This defers annotation evaluation (PEP 563) so modern syntax stays usable.
from __future__ import annotations

import json
import subprocess
import sys

from utils import SETUP_FILE, BUNDLE_ID, artifact_path


def run_steps(steps) -> int:
    """Run commands in order, stopping at the first failure."""
    for step in steps:
        code = subprocess.run(step).returncode
        if code != 0:
            return code
    return 0


def booted_simulator() -> str | None:
    """Return the UDID of the first booted simulator, or None."""
    result = subprocess.run(["xcrun", "simctl", "list", "devices", "booted", "-j"],
                            capture_output=True, text=True)

    for devices in json.loads(result.stdout).get("devices", {}).values():
        for device in devices:
            return device["udid"]
    return None


def connected_device() -> str | None:
    """Return the UDID of the first connected physical device, or None."""
    result = subprocess.run(["xcrun", "devicectl", "list", "devices"],
                            capture_output=True, text=True)

    for line in result.stdout.splitlines():
        if "unavailable" in line or "available" not in line:
            continue
        for token in line.split():
            if len(token) == 36 and token.count("-") == 4:
                return token
    return None


def launch_steps(preset, artifact) -> list | None:
    """Return the commands that install and launch the artifact.

    None means the target is not reachable; the reason has already been printed.
    """
    if preset.startswith("ios-sim"):
        udid = booted_simulator()
        if udid is None:
            print("No booted simulator. Boot one first, e.g.:")
            print("  xcrun simctl boot 'iPad Pro 13-inch (M4)' && open -a Simulator")
            return None
        return [
            ["xcrun", "simctl", "install", udid, str(artifact)],
            ["xcrun", "simctl", "launch", "--console-pty", udid, BUNDLE_ID],
        ]

    if preset.startswith("ios-device"):
        udid = connected_device()
        if udid is None:
            print("No available device. Check that the iPad is:")
            print("  - connected or on the same network, and unlocked")
            print("  - trusting this Mac")
            print("  - in Developer Mode (Settings > Privacy & Security > Developer Mode)")
            print("Then: xcrun devicectl list devices")
            return None
        return [
            ["xcrun", "devicectl", "device", "install", "app", "--device", udid, str(artifact)],
            ["xcrun", "devicectl", "device", "process", "launch", "--console",
             "--device", udid, BUNDLE_ID],
        ]

    # Desktop runs the executable directly rather than via `open`, so stdout stays attached
    # to this terminal instead of disappearing into the system log.
    return [[str(artifact)]]


def main():
    if not SETUP_FILE.exists():
        print("No setup found. Run: py Scripts/setup.py")
        sys.exit(1)

    preset = json.loads(SETUP_FILE.read_text())["preset"]
    artifact = artifact_path(preset)

    if not artifact.exists():
        print(f"Not built: {artifact}")
        print("Run: py Scripts/build.py")
        sys.exit(1)

    steps = launch_steps(preset, artifact)
    sys.exit(1 if steps is None else run_steps(steps))


if __name__ == "__main__":
    main()
