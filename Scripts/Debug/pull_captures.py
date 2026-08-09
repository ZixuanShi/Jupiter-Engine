#!/usr/bin/env python3
"""Copy GPU captures off the connected device into _Saved/Traces."""
import shutil
import subprocess
import sys
from pathlib import Path

# Scripts/ is the import root for utils, and it is no longer this file's own directory.
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from utils import IOS_TRACES, bundle_id, connected_device, device_traces, no_device_help, traces_dir


def pull_captures(udid, traces) -> int:
    """Copy captures the device has and this machine does not into _Saved/Traces.

    Only iOS needs this. On macOS GetSavedDir() is _Saved, so a capture is already local the
    moment the engine writes it. Returns how many were copied.
    """
    traces.mkdir(parents=True, exist_ok=True)
    have = {path.name for path in traces.glob("*.gputrace")}

    staging = traces / ".staging"

    copied = 0
    for name in device_traces(udid) or []:   # None: no Traces dir yet, so nothing to pull
        if name in have:
            continue

        # Staged, because a .gputrace is a directory bundle and devicectl unpacks a directory
        # source *into* the destination rather than under it -- copying straight to traces/
        # scatters one trace's parts across it, and a second pull merges into the first.
        # Inside it so the renames below stay on one filesystem.
        shutil.rmtree(staging, ignore_errors=True)
        staging.mkdir(parents=True)

        print(f"Pulling {name}")
        subprocess.run(["xcrun", "devicectl", "device", "copy", "from", "--device", udid,
                        "--domain-type", "appDataContainer", "--domain-identifier", bundle_id(),
                        "--source", f"{IOS_TRACES}/{name}", "--destination", str(staging)])

        entries = list(staging.iterdir())
        if len(entries) == 1 and entries[0].name == name:
            entries[0].rename(traces / name)             # devicectl kept the bundle
        elif entries:
            (traces / name).mkdir()                      # it unpacked; rebuild the bundle
            for entry in entries:
                entry.rename(traces / name / entry.name)

        shutil.rmtree(staging, ignore_errors=True)
        copied += 1 if (traces / name).exists() else 0
    return copied


def main() -> None:
    if sys.argv[1:]:
        print("Usage: py Scripts/Debug/pull_captures.py")
        sys.exit(1)

    # The preset is deliberately not read: you are normally back on macos by the time you want to
    # look at a trace. The project is, because it owns both the bundle id and _Saved/Traces.
    udid = connected_device()
    if udid is None:
        no_device_help()
        sys.exit(1)

    traces = traces_dir()
    copied = pull_captures(udid, traces)
    print(f"{copied} new capture(s) in {traces}" if copied else f"Nothing new. {traces}")


if __name__ == "__main__":
    main()
