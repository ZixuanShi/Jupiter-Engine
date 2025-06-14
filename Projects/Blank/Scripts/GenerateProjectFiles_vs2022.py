# Copyright Jupiter Technologies, Inc. All Rights Reserved.
import os

if __name__ == "__main__":
    os.chdir(os.path.dirname(os.path.abspath(__file__)))

    jupiterRoot = os.getenv("JUPITER_ENGINE_ROOT", "C:/Program Files/Jupiter Technologies/Jupiter-Engine")
    command = "\"" + jupiterRoot + "/Tools/Premake/Bin/Premake5.exe" + "\" vs2022"

    os.system(command)
    os.system("pause")
