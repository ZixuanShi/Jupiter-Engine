#!/usr/bin/env python3
import os
import subprocess
import sys

from utils import ROOT, active_preset, project_dir


def main():
    # A build preset resolves its binaryDir through the configure preset, which reads this.
    environment = {**os.environ, "JUPITER_PROJECT_DIR": str(project_dir())}
    sys.exit(subprocess.run(["cmake", "--build", "--preset", active_preset()],
                            cwd=ROOT, env=environment).returncode)


if __name__ == "__main__":
    main()
