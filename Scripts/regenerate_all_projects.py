# Copyright Jupiter Technologies, Inc. All Rights Reserved.
#
# Master regeneration script — replaces regenerate_projects_files.py (Premake-based).
#
# Runs in sequence:
#   1. Clean old project files
#   2. Compile shaders
#   3. Generate CMake project files for all projects
#
# Usage:
#   python Scripts/regenerate_all_projects.py           # from engine root
#   python Scripts/regenerate_all_projects.py --ninja   # force Ninja for all projects

import os
import subprocess
import sys

def get_engine_root() -> str:
    scripts_dir = os.path.dirname(os.path.abspath(__file__))
    return os.path.dirname(scripts_dir)

def run(cmd: list[str], **kwargs) -> None:
    print(">>>", " ".join(str(c) for c in cmd))
    result = subprocess.run(cmd, **kwargs)
    if result.returncode != 0:
        sys.exit(result.returncode)

if __name__ == "__main__":
    engine_root  = get_engine_root()
    forward_args = sys.argv[1:]   # e.g. ["--ninja"]

    # 1. Clean old intermediate / project files
    run(["python", os.path.join(engine_root, "Scripts", "clean.py")],
        cwd=engine_root)

    # 2. Bake shaders
    run(["python", os.path.join(engine_root, "Scripts", "compile_shaders.py")],
        cwd=engine_root)

    # 3. Generate project files for each client project
    projects = ["Blank", "UnitTests"]
    for project in projects:
        script = os.path.join(
            engine_root, "Projects", project, "Scripts", "generate_project_files.py")
        run(["python", script] + forward_args)
