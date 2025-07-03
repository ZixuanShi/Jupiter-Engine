import os
import subprocess

if __name__ == "__main__":
    # Get Scripts root directory
    jupiterEngineRoot  = os.getenv("JUPITER_ENGINE_ROOT", "C:/Program Files/Jupiter Technologies/Jupiter-Engine")

    # Clean up old project files
    subprocess.run(["python", "Scripts/Clean.py"], cwd=jupiterEngineRoot)

    # Regenerate project files
    subprocess.run(["python", "Scripts/CompileShaders.py"], cwd=jupiterEngineRoot)

    # Generate projects files
    subprocess.run(["python", "Projects/Blank/Scripts/GenerateProjectFiles_vs2022.py"], cwd=jupiterEngineRoot)
    subprocess.run(["python", "Projects/UnitTests/Scripts/GenerateProjectFiles_vs2022.py"], cwd=jupiterEngineRoot)
