@echo off

set "SHADER_SOURCE_DIR=%JUPITER_ENGINE_DIR%Assets/Shaders"
set "SHADER_BAKED_DIR=%JUPITER_ENGINE_DIR%_Baked/Jupiter_Common/Shaders"

if not exist "%SHADER_BAKED_DIR%" mkdir "%SHADER_BAKED_DIR%"

"%VULKAN_SDK%/Bin/glslc.exe" -fshader-stage=vertex "%SHADER_SOURCE_DIR%/sample_vert.glsl" -o "%SHADER_BAKED_DIR%/sample_vert.spv"
"%VULKAN_SDK%/Bin/glslc.exe" -fshader-stage=fragment "%SHADER_SOURCE_DIR%/sample_frag.glsl" -o "%SHADER_BAKED_DIR%/sample_frag.spv"

if %ERRORLEVEL% EQU 0 (
    echo Shader compilation successful!
) else (
    echo Error: Shader compilation failed!
)

pause