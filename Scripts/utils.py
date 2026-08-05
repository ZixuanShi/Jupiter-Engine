# macOS ships Python 3.9, where `str | None` in an annotation is a runtime TypeError.
# This defers annotation evaluation (PEP 563) so modern syntax stays usable.
from __future__ import annotations

import json
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SETUP_FILE = ROOT / "_ProjectFiles" / "setup.json"

BINARY_NAME = "JupiterEngine"
BUNDLE_ID = "com.jupitertechnologies.engine"

CONFIGS = ("debug", "dev", "release")
PLATFORMS = ("macos", "ios-device", "windows", "linux")

USAGE = f"Usage: py Scripts/setup.py [{'|'.join(CONFIGS)}] [{'|'.join(PLATFORMS)}]"


def host_platform() -> str:
    """Map the running OS onto the platform half of a preset name."""
    if sys.platform == "darwin":
        return "macos"
    if sys.platform.startswith("linux"):
        return "linux"
    return "windows"


def active_preset() -> str:
    """Return the preset the last setup.py run configured, or exit if there is none."""
    if not SETUP_FILE.exists():
        print("No setup found. Run: py Scripts/setup.py")
        sys.exit(1)

    return json.loads(SETUP_FILE.read_text())["preset"]


def output_dir(preset) -> Path:
    """Return the directory the build writes its final artifact into."""
    return ROOT / "_Output" / preset


def artifact_path(preset) -> Path:
    """Return the artifact to launch or install.

    macOS nests the executable inside the bundle; iOS bundles are flat and get installed
    whole, so callers there want the .app itself. Every other platform is a bare binary.    
    """
    
    root = output_dir(preset)

    if preset.startswith("ios"):
        return root / f"{BINARY_NAME}.app"
    if preset.startswith("macos"):
        return root / f"{BINARY_NAME}.app" / "Contents" / "MacOS" / BINARY_NAME

    ext = ".exe" if sys.platform == "win32" else ""
    return root / f"{BINARY_NAME}{ext}"


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


def no_device_help() -> None:
    """Print why no device was found and how to check."""
    print("No available device. Check that the iPad is:")
    print("  - connected or on the same network, and unlocked")
    print("  - trusting this Mac")
    print("  - in Developer Mode (Settings > Privacy & Security > Developer Mode)")
    print("Then: xcrun devicectl list devices")


TRACES = ROOT / "_Saved" / "Traces"

# GetSavedDir() on iOS, relative to the app data container devicectl addresses.
IOS_TRACES = "Library/Application Support/JupiterEngine/Traces"


def device_traces(udid) -> list:
    """Return the names of the .gputrace bundles sitting in the app's container."""
    with tempfile.TemporaryDirectory() as directory:
        listing = Path(directory) / "files.json"
        subprocess.run(["xcrun", "devicectl", "device", "info", "files", "--device", udid,
                        "--domain-type", "appDataContainer", "--domain-identifier", BUNDLE_ID,
                        "--subdirectory", IOS_TRACES, "--no-recurse",
                        "--json-output", str(listing)], capture_output=True)
        if not listing.exists():
            return []
        files = json.loads(listing.read_text()).get("result", {}).get("files", [])

    names = [Path(entry.get("path") or entry.get("name") or "").name for entry in files]
    return [name for name in names if name.endswith(".gputrace")]


def pull_captures(udid) -> int:
    """Copy captures the device has and this machine does not into _Saved/Traces.

    Only iOS needs this. On macOS GetSavedDir() is _Saved, so a capture is already local the
    moment the engine writes it. Returns how many were copied.
    """
    TRACES.mkdir(parents=True, exist_ok=True)
    have = {path.name for path in TRACES.glob("*.gputrace")}

    staging = TRACES / ".staging"

    copied = 0
    for name in device_traces(udid):
        if name in have:
            continue

        # Staged, because a .gputrace is a directory bundle and devicectl unpacks a directory
        # source *into* the destination rather than under it -- copying straight to TRACES
        # scatters one trace's parts across it, and a second pull merges into the first.
        # Inside TRACES so the renames below stay on one filesystem.
        shutil.rmtree(staging, ignore_errors=True)
        staging.mkdir(parents=True)

        print(f"Pulling {name}")
        subprocess.run(["xcrun", "devicectl", "device", "copy", "from", "--device", udid,
                        "--domain-type", "appDataContainer", "--domain-identifier", BUNDLE_ID,
                        "--source", f"{IOS_TRACES}/{name}", "--destination", str(staging)])

        entries = list(staging.iterdir())
        if len(entries) == 1 and entries[0].name == name:
            entries[0].rename(TRACES / name)             # devicectl kept the bundle
        elif entries:
            (TRACES / name).mkdir()                      # it unpacked; rebuild the bundle
            for entry in entries:
                entry.rename(TRACES / name / entry.name)

        shutil.rmtree(staging, ignore_errors=True)
        copied += 1 if (TRACES / name).exists() else 0
    return copied


def executable_path(preset) -> Path:
    """Return the compiled binary, which is what proves a build actually ran.

    Differs from artifact_path on iOS: CMake writes the bundle's Info.plist during
    configure, so the .app exists as a directory before anything is compiled.
    """

    if preset.startswith("ios"):
        return artifact_path(preset) / BINARY_NAME
    return artifact_path(preset)
