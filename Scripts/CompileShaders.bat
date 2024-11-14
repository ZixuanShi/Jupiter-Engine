@echo off
if not exist "%JUPITER_ENGINE_DIR%_Baked/Shaders" mkdir "%JUPITER_ENGINE_DIR%_Baked/Shaders"

"%VULKAN_SDK%/Bin/dxc.exe" -spirv -T vs_6_0 -E main "%JUPITER_ENGINE_DIR%Assets/Shaders/triangle.vs.hlsl" -Fo "%JUPITER_ENGINE_DIR%_Baked/Shaders/triangle.vs.spv"
"%VULKAN_SDK%/Bin/dxc.exe" -spirv -T ps_6_0 -E main "%JUPITER_ENGINE_DIR%Assets/Shaders/triangle.ps.hlsl" -Fo "%JUPITER_ENGINE_DIR%_Baked/Shaders/triangle.ps.spv"

if %ERRORLEVEL% EQU 0 (
    echo Shader compilation successful!
) else (
    echo Error: Shader compilation failed!
)

pause