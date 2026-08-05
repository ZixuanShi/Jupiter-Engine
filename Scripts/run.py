#!/usr/bin/env python3
# macOS ships Python 3.9, where `str | None` in an annotation is a runtime TypeError.
# This defers annotation evaluation (PEP 563) so modern syntax stays usable.
from __future__ import annotations

import json
import os
import subprocess
import sys

from utils import (BUNDLE_ID, active_preset, artifact_path, connected_device, executable_path,
                   no_device_help, pull_captures)

# What the Dev Menu's Capture button needs. Measured on macOS 26: MTLCaptureEnabled in the
# bundle's Info.plist does not work, even launched through LaunchServices -- only this does.
CAPTURE_ENV = {"MTL_CAPTURE_ENABLED": "1"}


def run_steps(steps, environment) -> int:
    """Run commands in order, stopping at the first failure."""
    for step in steps:
        try:
            code = subprocess.run(step, env={**os.environ, **environment}).returncode
        except KeyboardInterrupt:
            print("\nDetached. The app is still running.")
            return 0
        if code != 0:
            return code
    return 0


def launch_steps(udid, artifact, console, environment) -> list:
    """Return the commands that install and launch the artifact."""
    if udid is None:
        # Run directly rather than via `open`, so stdout stays attached to this terminal.
        return [[str(artifact)]]

    # devicectl launches on the device, so the environment travels as an argument rather than
    # being inherited from this process.
    launch = ["xcrun", "devicectl", "device", "process", "launch", "--device", udid,
              "--environment-variables", json.dumps(environment)]
    if console:
        launch.append("--console")
    launch.append(BUNDLE_ID)

    return [
        ["xcrun", "devicectl", "device", "install", "app", "--device", udid, str(artifact)],
        launch,
    ]


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

    udid = None
    if preset.startswith("ios-device"):
        udid = connected_device()
        if udid is None:
            no_device_help()
            sys.exit(1)

    environment = {} if "release" in preset else CAPTURE_ENV
    code = run_steps(launch_steps(udid, artifact, console, environment), environment)

    # Convenience only -- with --console this is where the session ends, so it collects what you
    # just captured. Otherwise devicectl returns at launch and this gets the previous session.
    # Either way Scripts/pull_captures.py fetches on demand, and neither re-copies.
    if udid is not None:
        pull_captures(udid)

    sys.exit(code)


if __name__ == "__main__":
    main()
