#!/usr/bin/env python3
import shutil

from utils import ROOT


def main():
    shutil.rmtree(ROOT / "_Output", ignore_errors=True)
    shutil.rmtree(ROOT / "_ProjectFiles", ignore_errors=True)
    (ROOT / ".vscode" / "launch.json").unlink(missing_ok=True)


if __name__ == "__main__":
    main()
