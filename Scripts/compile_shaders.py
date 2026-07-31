# Copyright Jupiter Technologies, Inc. All Rights Reserved.

import os
import subprocess

g_shader_source_dir: str = ""
g_shader_output_dir: str = ""
g_compiler_path: str = ""


def get_compiler_path() -> str:
    compiler_path = os.getenv("VULKAN_SDK")
    if not compiler_path:
        print("Error: VULKAN_SDK environment variable is not set.")
        exit(1)

    compiler_path += "/bin/glslc.exe"
    if not os.path.exists(compiler_path):
        print(f"Error: glslc not found at {compiler_path}. Please ensure Vulkan SDK is installed.")
        exit(1)

    print(f"Using glslc compiler at: {compiler_path}")
    return compiler_path


def compile_shader(file: str) -> None:
    arg = ""
    if "Vert" in file:
        arg = "-fshader-stage=vertex"
    elif "Frag" in file:
        arg = "-fshader-stage=fragment"

    path = os.path.join(g_shader_source_dir, file)
    out  = os.path.join(g_shader_output_dir, file + ".spv")
    command = [g_compiler_path, arg, path, "-o", out]

    result = subprocess.run(command, capture_output=True)
    if result.returncode != 0:
        print(f"Error compiling {file}: {result.stderr.decode()}")
    else:
        print(f"Successfully compiled {file} to {out}")


if __name__ == "__main__":
    # Get paths
    jupiterRoot = os.getenv("JUPITER_ENGINE_ROOT_LEGACY", "C:\\Users\\szx07\\repos\\Jupiter-Engine-legacy")
    g_shader_source_dir = jupiterRoot + "/Assets/Shaders"
    g_shader_output_dir = jupiterRoot + "/_Baked/Jupiter_Common/Shaders"
    g_compiler_path = get_compiler_path()

    if not os.path.exists(g_shader_output_dir):
        os.makedirs(g_shader_output_dir)

    for root, dirs, files in os.walk(g_shader_source_dir):
        for file in files:
            compile_shader(file)
