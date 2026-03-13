import os
import subprocess

if __name__ == "__main__":
    # Get Scripts root directory
    jupiter_engine_root  = os.getenv("JUPITER_ENGINE_ROOT", "C:/Program Files/Jupiter Technologies/Jupiter-Engine")

    # Clean up old project files
    subprocess.run(["python", "Scripts/clean.py"], cwd=jupiter_engine_root)

    # Bake assets
    subprocess.run(["python", "Scripts/compile_shaders.py"], cwd=jupiter_engine_root)

    # Generate project files
    subprocess.run(["python", "Projects/Blank/Scripts/generate_project_files_vs2022.py"], cwd=jupiter_engine_root)
    subprocess.run(["python", "Projects/UnitTests/Scripts/generate_project_files_vs2022.py"], cwd=jupiter_engine_root)
