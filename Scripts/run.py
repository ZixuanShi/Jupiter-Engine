#!/usr/bin/env python3
# macOS ships Python 3.9, where `str | None` in an annotation is a runtime TypeError.
# This defers annotation evaluation (PEP 563) so modern syntax stays usable.
from __future__ import annotations

import json
import os
import subprocess
import sys
import time

from utils import (active_preset, artifact_path, bundle_id, connected_device, executable_path,
                   no_device_help)

# What the Dev Menu's Capture button needs. Measured on macOS 26: MTLCaptureEnabled in the
# bundle's Info.plist does not work, even launched through LaunchServices -- only this does.
CAPTURE_ENV = {"MTL_CAPTURE_ENABLED": "1"}

# Fixed, not per-project: the activity lives in the template's own namespace, and only
# applicationId varies -- see Build/Android/Template.
ANDROID_ACTIVITY = "com.jupitertechnologies.jupiter.JupiterActivity"


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


def flag_name(argument) -> str:
    """Return a launch flag's name without its leading dashes, or "" if it is not a flag.

    Mirrors LaunchArgs::PreInit, so what this script calls a flag is what the engine stores:
    one or more '-' then a name.
    """
    name = argument.lstrip("-")
    return name if name and name != argument else ""


def launch_steps(udid, artifact, environment, app_args) -> list:
    """Return the commands that install and launch the artifact."""
    if udid is None:
        # Run directly rather than via `open`, so stdout stays attached to this terminal.
        return [[str(artifact), *app_args]]

    # devicectl launches on the device, so the environment travels as an argument rather than
    # being inherited from this process.
    launch = ["xcrun", "devicectl", "device", "process", "launch", "--device", udid,
              "--environment-variables", json.dumps(environment)]

    # The one launch arg this script reads rather than only forwarding: a device gives the app
    # no terminal, so devicectl has to be asked to relay its output.
    if any(flag_name(a) == "console" for a in app_args):
        launch.append("--console")

    # devicectl's root parser claims its own global short options (-v -q -t -j -l) even out of the
    # args meant for the app: "-console" holds an 'l', so it is read as --log-output and demands a
    # path. "--" ends that scan, and before the bundle id it is consumed rather than forwarded.
    launch += ["--", bundle_id(), *app_args]

    return [
        ["xcrun", "devicectl", "device", "install", "app", "--device", udid, str(artifact)],
        launch,
    ]


def android_device() -> str | None:
    """Return the serial of the first adb device in state `device`, or None."""
    result = subprocess.run(["adb", "devices"], capture_output=True, text=True)
    for line in result.stdout.splitlines()[1:]:
        parts = line.split()
        if len(parts) >= 2 and parts[1] == "device":
            return parts[0]
    return None


def android_help() -> None:
    """Print how to reach the phone; see Build/Android/README.md for the whole flow."""
    print("No adb device. Wireless debugging (the OnePlus USB driver never enumerates):")
    print("  Settings > Developer options > Wireless debugging > Pair device with pairing code")
    print("  adb pair <ip:pairing-port>     (once; pairing survives reboots)")
    print("  adb connect <ip:port>          (each session; the port changes)")
    print("Then: adb devices")


def run_android(artifact) -> int:
    """Install the APK, launch the activity, and stream its logcat until Ctrl+C.

    Launch arguments are not forwarded: `am start` extras only reach an activity that parses
    them, which SDLActivity does not. Wire getArguments() through the template when needed.
    """
    serial = android_device()
    if serial is None:
        android_help()
        return 1

    adb = ["adb", "-s", serial]
    for step in ([*adb, "install", "-r", str(artifact)],
                 [*adb, "shell", "am", "start", "-W", "-n", f"{bundle_id()}/{ANDROID_ACTIVITY}"]):
        code = subprocess.run(step).returncode
        if code != 0:
            return code

    # `am start -W` waits for the activity, but the process can lag it by a beat.
    pid = ""
    for _ in range(50):
        pid = subprocess.run([*adb, "shell", "pidof", "-s", bundle_id()],
                             capture_output=True, text=True).stdout.strip()
        if pid:
            break
        time.sleep(0.1)

    if not pid:
        print(f"{bundle_id()} started but its process is gone -- crashed on launch?")
        print("Look at: adb logcat -b crash")
        return 1

    try:
        return subprocess.run([*adb, "logcat", "--pid", pid]).returncode
    except KeyboardInterrupt:
        print("\nDetached. The app is still running.")
        return 0


def main():
    # Every argument is the app's. This script owns none of them, so what it acts on is exactly
    # what LaunchArgs stores.
    app_args = sys.argv[1:]

    preset = active_preset()
    executable = executable_path(preset)

    if not executable.exists():
        print(f"Not built: {executable}")
        print("Run: py Scripts/build.py")
        sys.exit(1)

    if preset.startswith("android"):
        sys.exit(run_android(artifact_path(preset)))

    udid = None
    if preset.startswith("ios-device"):
        udid = connected_device()
        if udid is None:
            no_device_help()
            sys.exit(1)

    environment = {} if "release" in preset else CAPTURE_ENV
    steps = launch_steps(udid, artifact_path(preset), environment, app_args)
    sys.exit(run_steps(steps, environment))


if __name__ == "__main__":
    main()
