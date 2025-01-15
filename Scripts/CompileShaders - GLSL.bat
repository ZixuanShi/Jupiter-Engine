@echo off

set "SHADER_SOURCE_DIR=%JUPITER_ENGINE_DIR%Assets/Shaders"
set "SHADER_BAKED_DIR=%JUPITER_ENGINE_DIR%_Baked/Jupiter_Common/Shaders"

if not exist "%SHADER_BAKED_DIR%" mkdir "%SHADER_BAKED_DIR%"

"%VULKAN_SDK%/Bin/glslc.exe" -fshader-stage=vertex "%SHADER_SOURCE_DIR%/sample.vs.glsl" -o "%SHADER_BAKED_DIR%/sample.vs.spv"
"%VULKAN_SDK%/Bin/glslc.exe" -fshader-stage=fragment "%SHADER_SOURCE_DIR%/sample.ps.glsl" -o "%SHADER_BAKED_DIR%/sample.ps.spv"

if %ERRORLEVEL% EQU 0 (
    echo Shader compilation successful!
) else (
    echo Error: Shader compilation failed!
)

pause