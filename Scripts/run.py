#!/usr/bin/env python3
# macOS ships Python 3.9, where `str | None` in an annotation is a runtime TypeError.
# This defers annotation evaluation (PEP 563) so modern syntax stays usable.
from __future__ import annotations

import json
import subprocess
import sys
import tempfile
from pathlib import Path

from utils import BUNDLE_ID, active_preset, artifact_path, executable_path


def run_steps(steps) -> int:
    """Run commands in order, stopping at the first failure."""
    for step in steps:
        try:
            code = subprocess.run(step).returncode
        except KeyboardInterrupt:
            print("\nDetached. The app is still running.")
            return 0
        if code != 0:
            return code
    return 0


def connected_device() -> str | None:
    """Return the identifier of the first paired, reachable device, or None.

    Reads the JSON rather than the printed table, which has no stable columns. The state to
    match is not one string: devicectl reports tunnelState as "connected" over USB and
    "available" over the network, so this rejects "unavailable" instead of listing the rest.
    """
    with tempfile.TemporaryDirectory() as directory:
        output = Path(directory) / "devices.json"
        subprocess.run(["xcrun", "devicectl", "list", "devices", "--json-output", str(output)],
                       capture_output=True)
        if not output.exists():
            return None
        devices = json.loads(output.read_text())["result"]["devices"]

    for device in devices:
        connection = device.get("connectionProperties", {})
        if (connection.get("pairingState") == "paired"
                and connection.get("tunnelState") != "unavailable"):
            return device["identifier"]
    return None


def launch_steps(preset, artifact, console) -> list | None:
    """Return the commands that install and launch the artifact.

    None means the target is not reachable; the reason has already been printed.
    """
    if preset.startswith("ios-device"):
        udid = connected_device()
        if udid is None:
            print("No available device. Check that the iPad is:")
            print("  - connected or on the same network, and unlocked")
            print("  - trusting this Mac")
            print("  - in Developer Mode (Settings > Privacy & Security > Developer Mode)")
            print("Then: xcrun devicectl list devices")
            return None
        launch = ["xcrun", "devicectl", "device", "process", "launch",
                  "--device", udid, BUNDLE_ID]
        if console:
            launch.insert(5, "--console")

        return [
            ["xcrun", "devicectl", "device", "install", "app", "--device", udid, str(artifact)],
            launch,
        ]

    # Run directly rather than via `open`, so stdout stays attached to this terminal.
    return [[str(artifact)]]


def main():
    console = "--console" in sys.argv[1:]
    unknown = [a for a in sys.argv[1:] if a != "--console"]
    if unknown:
        print(f"Unknown argument(s): {', '.join(unknown)}")
        print("Usage: py Scripts/run.py [--console]")
        sys.exit(1)

    preset = active_preset()
    artifact = artifact_path(preset)

    if not executable_path(preset).exists():
        print(f"Not built: {executable_path(preset)}")
        print("Run: py Scripts/build.py")
        sys.exit(1)

    steps = launch_steps(preset, artifact, console)
    sys.exit(1 if steps is None else run_steps(steps))


if __name__ == "__main__":
    main()
