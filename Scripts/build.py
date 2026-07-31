#!/usr/bin/env python3
import json
import subprocess
import sys

from utils import ROOT, SETUP_FILE


def main():
    if not SETUP_FILE.exists():
        print("No setup found. Run: py setup.py")
        sys.exit(1)
    setup = json.loads(SETUP_FILE.read_text())
    sys.exit(subprocess.run(["cmake", "--build", "--preset", setup["preset"]], cwd=ROOT).returncode)


if __name__ == "__main__":
    main()
