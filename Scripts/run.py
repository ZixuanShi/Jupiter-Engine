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
    ext = ".exe" if sys.platform == "win32" else ""
    binary = ROOT / "_Output" / setup["preset"] / f"JupiterEngine{ext}"
    sys.exit(subprocess.run([str(binary)]).returncode)


if __name__ == "__main__":
    main()
