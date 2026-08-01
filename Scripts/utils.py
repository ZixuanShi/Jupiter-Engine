import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SETUP_FILE = ROOT / "_ProjectFiles" / "setup.json"

BINARY_NAME = "JupiterEngine"
BUNDLE_ID = "com.jupitertechnologies.engine"

CONFIGS = ("debug", "dev", "release")
PLATFORMS = ("macos", "ios-sim", "ios-device", "windows", "linux")

USAGE = f"Usage: py Scripts/setup.py [{' | '.join(CONFIGS)}] [{' | '.join(PLATFORMS)}]"


def host_platform() -> str:
    """Map the running OS onto the platform half of a preset name."""
    if sys.platform == "darwin":
        return "macos"
    if sys.platform.startswith("linux"):
        return "linux"
    return "windows"


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
