# Copyright Jupiter Technologies, Inc. All Rights Reserved.

import os
import subprocess

g_shaderSourceDir: str = ""
g_shaderOutputDir: str = ""
g_compilerPath   : str = ""


def GetCompilerPath() -> str:
    compilerPath = os.getenv("VULKAN_SDK")
    if not compilerPath:
        print("Error: VULKAN_SDK environment variable is not set.")
        exit(1)
    
    compilerPath += "/bin/glslc.exe"
    if not os.path.exists(compilerPath):
        print(f"Error: glslc not found at {compilerPath}. Please ensure Vulkan SDK is installed.")
        exit(1)
    
    print(f"Using glslc compiler at: {compilerPath}")
    return compilerPath


def CompileShader(file: str) -> None:
    arg = ""
    if "Vert" in file:
        arg = "-fshader-stage=vertex"
    elif "Frag" in file:
        arg = "-fshader-stage=fragment"

    path = os.path.join(g_shaderSourceDir, file)
    out  = os.path.join(g_shaderOutputDir, file + ".spv")
    command = [g_compilerPath, arg, path, "-o", out]

    result = subprocess.run(command, capture_output=True)
    if result.returncode != 0:
        print(f"Error compiling {file}: {result.stderr.decode()}")
    else:
        print(f"Successfully compiled {file} to {out}")


if __name__ == "__main__":
    # Get paths
    jupiterRoot     = os.getenv("JUPITER_ENGINE_ROOT", "C:/Program Files/Jupiter Technologies/Jupiter-Engine")
    g_shaderSourceDir = jupiterRoot + "/Assets/Shaders"
    g_shaderOutputDir = jupiterRoot + "/_Baked/Jupiter_Common/Shaders"
    g_compilerPath    = GetCompilerPath()
    
    if not os.path.exists(g_shaderOutputDir):
        os.makedirs(g_shaderOutputDir)

    for root, dirs, files in os.walk(g_shaderSourceDir):
        for file in files:
            CompileShader(file)
