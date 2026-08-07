#!/usr/bin/env python3
"""Copy GPU captures off the connected device into _Saved/Traces."""
import shutil
import subprocess
import sys
from pathlib import Path

# Scripts/ is the import root for utils, and it is no longer this file's own directory.
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from utils import BUNDLE_ID, IOS_TRACES, TRACES, connected_device, device_traces, no_device_help


def pull_captures(udid) -> int:
    """Copy captures the device has and this machine does not into _Saved/Traces.

    Only iOS needs this. On macOS GetSavedDir() is _Saved, so a capture is already local the
    moment the engine writes it. Returns how many were copied.
    """
    TRACES.mkdir(parents=True, exist_ok=True)
    have = {path.name for path in TRACES.glob("*.gputrace")}

    staging = TRACES / ".staging"

    copied = 0
    for name in device_traces(udid) or []:   # None: no Traces dir yet, so nothing to pull
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


def main() -> None:
    if sys.argv[1:]:
        print("Usage: py Scripts/Debug/pull_captures.py")
        sys.exit(1)

    # Deliberately not read from setup.json: you are normally on the macos preset by the time
    # you want to look at a trace, and this needs only the device and the bundle id.
    udid = connected_device()
    if udid is None:
        no_device_help()
        sys.exit(1)

    copied = pull_captures(udid)
    print(f"{copied} new capture(s) in {TRACES}" if copied else f"Nothing new. {TRACES}")


if __name__ == "__main__":
    main()
