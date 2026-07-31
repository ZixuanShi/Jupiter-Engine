# Copyright Jupiter Technologies, Inc. All Rights Reserved.
#
# Generates project files for the UnitTests project using CMake.
# Replaces generate_project_files_vs2022.py (Premake-based).

import os
import subprocess
import platform
import sys

def get_project_dir() -> str:
    scripts_dir = os.path.dirname(os.path.abspath(__file__))
    return os.path.dirname(scripts_dir)

def get_engine_root() -> str:
    """Returns the root of this Jupiter Engine (legacy) checkout, from the dedicated
    JUPITER_ENGINE_ROOT_LEGACY env var. This overrides JUPITER_ENGINE_ROOT (which points
    at the current Jupiter-Engine checkout) so this repo never builds against that one."""
    engine_root = os.environ.get("JUPITER_ENGINE_ROOT_LEGACY")
    if not engine_root:
        sys.exit("JUPITER_ENGINE_ROOT_LEGACY is not set. Point it at this Jupiter-Engine-legacy checkout.")
    return engine_root

def get_cmake_generator(args: list[str]) -> list[str]:
    if "--ninja" in args:
        return ["-G", "Ninja"]
    if "--vs2022" in args or platform.system() == "Windows":
        return ["-G", "Visual Studio 17 2022", "-A", "x64"]
    return ["-G", "Ninja"]

if __name__ == "__main__":
    project_dir = get_project_dir()
    build_dir   = os.path.join(project_dir, "_ProjectFiles")
    os.makedirs(build_dir, exist_ok=True)

    generator_args = get_cmake_generator(sys.argv[1:])

    engine_root = get_engine_root()
    env = os.environ.copy()
    env["JUPITER_ENGINE_ROOT"] = engine_root

    cmd = ["cmake", "-S", project_dir, "-B", build_dir] + generator_args
    print("Using JUPITER_ENGINE_ROOT:", engine_root)
    print("Running:", " ".join(cmd))
    result = subprocess.run(cmd, env=env)
    sys.exit(result.returncode)
