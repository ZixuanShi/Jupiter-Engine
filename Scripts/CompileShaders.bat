@echo off

set "SHADER_SOURCE_DIR=%JUPITER_ENGINE_DIR%Assets/Shaders"
set "SHADER_BAKED_DIR=%JUPITER_ENGINE_DIR%_Baked/Shaders"

REM Common shaders, should be packed to client project release package
if not exist "%JUPITER_ENGINE_DIR%_Baked/Shaders" mkdir "%JUPITER_ENGINE_DIR%_Baked/Shaders"

"%VULKAN_SDK%/Bin/dxc.exe" -spirv -T vs_6_0 -E main "%SHADER_SOURCE_DIR%/triangle.vs.hlsl" -Fo "%SHADER_BAKED_DIR%/triangle.vs.spv"
"%VULKAN_SDK%/Bin/dxc.exe" -spirv -T ps_6_0 -E main "%SHADER_SOURCE_DIR%/triangle.ps.hlsl" -Fo "%SHADER_BAKED_DIR%/triangle.ps.spv"

if %ERRORLEVEL% EQU 0 (
    echo Shader compilation successful!
) else (
    echo Error: Shader compilation failed!
)

pause