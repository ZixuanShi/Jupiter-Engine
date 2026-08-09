# macOS ships Python 3.9, where `str | None` in an annotation is a runtime TypeError.
# This defers annotation evaluation (PEP 563) so modern syntax stays usable.
from __future__ import annotations

import json
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

# Engine-level state -- which project was configured last -- so it stays beside the engine even
# though everything the build produces lands under the project.
SETUP_FILE = ROOT / "_ProjectFiles" / "setup.json"

PROJECTS = ROOT / "Projects"

CONFIGS = ("debug", "dev", "release")
PLATFORMS = ("macos", "ios-device", "windows", "linux")

USAGE = (f"Usage: py Scripts/setup.py [{'|'.join(CONFIGS)}] [{'|'.join(PLATFORMS)}] "
         f"<project name or path>")


def host_platform() -> str:
    """Map the running OS onto the platform half of a preset name."""
    if sys.platform == "darwin":
        return "macos"
    if sys.platform.startswith("linux"):
        return "linux"
    return "windows"


def active_setup() -> dict:
    """Return what the last setup.py run recorded, or exit if there is none."""
    if not SETUP_FILE.exists():
        print("No setup found. Run: py Scripts/setup.py")
        sys.exit(1)

    return json.loads(SETUP_FILE.read_text())


def active_preset() -> str:
    """Return the preset the last setup.py run configured, or exit if there is none."""
    return active_setup()["preset"]


def project_dir() -> Path:
    """Return the active project's directory, the root of its _Output/_ProjectFiles/_Saved."""
    return ROOT / active_setup()["project"]


def binary_name() -> str:
    """Return the executable's name, which is the project directory's own."""
    return project_dir().name


def bundle_id() -> str:
    """Return the app's bundle identifier.

    Derived rather than stored, matching what Projects/<Name>/CMakeLists.txt spells for CMake.
    """
    return f"com.jupitertechnologies.{binary_name().lower()}"


def output_dir(preset) -> Path:
    """Return the directory the build writes its final artifact into."""
    return project_dir() / "_Output" / preset


def artifact_path(preset) -> Path:
    """Return the artifact to launch or install.

    macOS nests the executable inside the bundle; iOS bundles are flat and get installed
    whole, so callers there want the .app itself. Every other platform is a bare binary.    
    """
    
    root = output_dir(preset)
    name = binary_name()

    if preset.startswith("ios"):
        return root / f"{name}.app"
    if preset.startswith("macos"):
        return root / f"{name}.app" / "Contents" / "MacOS" / name

    ext = ".exe" if sys.platform == "win32" else ""
    return root / f"{name}{ext}"


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


def traces_dir() -> Path:
    """Return where pulled captures land, beside the ones a desktop run writes."""
    return project_dir() / "_Saved" / "Traces"


# GetSavedDir() on iOS, relative to the app data container devicectl addresses. Still the engine's
# name because PlatformPaths.cpp's fallback spells it; the two have to agree.
IOS_TRACES = "Library/Application Support/JupiterEngine/Traces"


def device_traces(udid) -> list | None:
    """Return the names of the .gputrace bundles sitting in the app's container.

    None when the listing itself failed -- the directory does not exist yet, or devicectl
    errored -- which callers must not mistake for an empty directory.
    """
    with tempfile.TemporaryDirectory() as directory:
        listing = Path(directory) / "files.json"
        subprocess.run(["xcrun", "devicectl", "device", "info", "files", "--device", udid,
                        "--domain-type", "appDataContainer", "--domain-identifier", bundle_id(),
                        "--subdirectory", IOS_TRACES, "--no-recurse",
                        "--json-output", str(listing)], capture_output=True)
        if not listing.exists():
            return None
        files = json.loads(listing.read_text()).get("result", {}).get("files", [])

    names = [Path(entry.get("path") or entry.get("name") or "").name for entry in files]
    return [name for name in names if name.endswith(".gputrace")]


def executable_path(preset) -> Path:
    """Return the compiled binary, which is what proves a build actually ran.

    Differs from artifact_path on iOS: CMake writes the bundle's Info.plist during
    configure, so the .app exists as a directory before anything is compiled.
    """

    if preset.startswith("ios"):
        return artifact_path(preset) / binary_name()
    return artifact_path(preset)
