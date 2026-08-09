#!/usr/bin/env python3
import shutil

from utils import ROOT, PROJECTS


def summarize(path) -> str:
    """Return the file count and total size of a directory tree."""
    files = [p for p in path.rglob("*") if p.is_file() or p.is_symlink()]
    total = sum(p.stat().st_size for p in files if p.is_file())
    return f"{len(files)} files, {total / (1024 * 1024):.1f} MB"


def remove(path):
    """Delete a file or directory tree, reporting what went."""
    label = path.relative_to(ROOT) if path.is_relative_to(ROOT) else path

    if path.is_dir() and not path.is_symlink():
        print(f"Removed {label} ({summarize(path)})")
        shutil.rmtree(path, ignore_errors=True)
    elif path.exists() or path.is_symlink():
        print(f"Removed {label}")
        path.unlink(missing_ok=True)
    else:
        print(f"Skipped {label} (not present)")


def main():
    remove(ROOT / "_Output")
    remove(ROOT / "_ProjectFiles")
    remove(ROOT / ".vscode" / "launch.json")

    # Every project, not just the configured one: clean has to work when there is no setup.json,
    # and a project built earlier under another preset still has its own directories.
    for project in sorted(p for p in PROJECTS.glob("*") if p.is_dir()):
        for name in ("_Output", "_ProjectFiles", "_Saved"):
            if (project / name).exists():
                remove(project / name)


if __name__ == "__main__":
    main()
