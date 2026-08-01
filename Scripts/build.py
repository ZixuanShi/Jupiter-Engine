#!/usr/bin/env python3
import json
import subprocess
import sys

from utils import ROOT, SETUP_FILE


def main():
    if not SETUP_FILE.exists():
        print("No setup found. Run: py Scripts/setup.py")
        sys.exit(1)

    preset = json.loads(SETUP_FILE.read_text())["preset"]
    sys.exit(subprocess.run(["cmake", "--build", "--preset", preset], cwd=ROOT).returncode)


if __name__ == "__main__":
    main()
