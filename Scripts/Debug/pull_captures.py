#!/usr/bin/env python3
"""Copy GPU captures off the connected device into _Saved/Traces."""
import sys
from pathlib import Path

# Scripts/ is the import root for utils, and it is no longer this file's own directory.
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from utils import TRACES, connected_device, no_device_help, pull_captures


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
