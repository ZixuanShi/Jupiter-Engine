#!/usr/bin/env python3
import subprocess
import sys

from utils import ROOT, active_preset


def main():
    sys.exit(subprocess.run(["cmake", "--build", "--preset", active_preset()], cwd=ROOT).returncode)


if __name__ == "__main__":
    main()
