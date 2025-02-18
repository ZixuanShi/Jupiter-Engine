@echo off
setlocal EnableDelayedExpansion

:: Configuration
set "VERSION=1.1.0"
set "CONFIG_FILE=%~dp0shader_compile.conf"
set "LOG_FILE=%~dp0shader_compile.log"
set "TIMESTAMP=%DATE:~10,4%%DATE:~4,2%%DATE:~7,2%_%TIME:~0,2%%TIME:~3,2%%TIME:~6,2%"

:: Default paths (overridable via config)
set "SHADER_SOURCE_DIR=%JUPITER_ENGINE_DIR%Assets\Shaders"
set "SHADER_BAKED_DIR=%JUPITER_ENGINE_DIR%_Baked\Jupiter_Common\Shaders"
set "VULKAN_GLSLC=%VULKAN_SDK%\Bin\glslc.exe"

:: Color codes (using ANSI escapes via powershell)
set "GREEN=powershell -Command Write-Host -ForegroundColor Green"
set "RED=powershell -Command Write-Host -ForegroundColor Red"
set "YELLOW=powershell -Command Write-Host -ForegroundColor Yellow"
set "NC=powershell -Command Write-Host -NoNewline"

:: Banner
echo Shader Compiler v%VERSION%
echo =========================

:: Load configuration file if exists
if exist "%CONFIG_FILE%" (
    for /f "tokens=1,2 delims==" %%a in (%CONFIG_FILE%) do (
        set "%%a=%%b"
    )
)

:: Log function
:log
    echo [%TIMESTAMP%] %* >> "%LOG_FILE%" 2>nul
    if "%VERBOSE%"=="true" echo [%TIMESTAMP%] %*
goto :eof

:: Check prerequisites
:check_prereqs
    if not defined JUPITER_ENGINE_DIR (
        %RED% "Error: JUPITER_ENGINE_DIR environment variable not set"
        call :log "Error: JUPITER_ENGINE_DIR not set"
        exit /b 1
    )
    if not exist "%VULKAN_GLSLC%" (
        %RED% "Error: glslc.exe not found at %VULKAN_GLSLC%"
        call :log "Error: Vulkan SDK glslc not found"
        exit /b 1
    )
    if not exist "%SHADER_SOURCE_DIR%" (
        %RED% "Error: Shader source directory not found: %SHADER_SOURCE_DIR%"
        call :log "Error: Source directory missing"
        exit /b 1
    )
goto :eof

:: Create baked directory
:setup_dirs
    if not exist "%SHADER_BAKED_DIR%" (
        mkdir "%SHADER_BAKED_DIR%" 2>nul
        if errorlevel 1 (
            %RED% "Error: Failed to create baked directory: %SHADER_BAKED_DIR%"
            call :log "Error: Failed to create baked directory"
            exit /b 1
        )
        call :log "Created baked directory: %SHADER_BAKED_DIR%"
    )
goto :eof

:: Compile shader function
:compile_shader
    set "SOURCE=%1"
    set "STAGE=%2"
    set "OUTPUT=%3"
    %YELLOW% "Compiling %SOURCE% (%STAGE%)..."
    call :log "Compiling %SOURCE% to %OUTPUT% (%STAGE%)"
    "%VULKAN_GLSLC%" -fshader-stage=%STAGE% "%SOURCE%" -o "%OUTPUT%" 2>> "%LOG_FILE%"
    if !ERRORLEVEL! EQU 0 (
        %GREEN% "Success: Compiled %SOURCE%"
        call :log "Success: Compiled %SOURCE%"
    ) else (
        %RED% "Error: Failed to compile %SOURCE%"
        call :log "Error: Compilation failed for %SOURCE%"
        set "COMPILATION_FAILED=true"
    )
goto :eof

:: Main compilation process
:main
    call :log "Starting shader compilation (v%VERSION%)"

    :: Check prerequisites
    call :check_prereqs || exit /b 1
    call :setup_dirs || exit /b 1

    :: Supported shader types
    set "SHADER_TYPES=glsl hlsl"
    set "SHADER_STAGES=vertex fragment compute"

    :: Compile all shaders
    set "COMPILATION_FAILED=false"
    for %%t in (%SHADER_TYPES%) do (
        for %%s in (%SHADER_STAGES%) do (
            set "SOURCE_FILE=%SHADER_SOURCE_DIR%\sample_%%s.%%t"
            set "OUTPUT_FILE=%SHADER_BAKED_DIR%\sample_%%s_%%t.spv"
            if exist "!SOURCE_FILE!" (
                call :compile_shader "!SOURCE_FILE!" %%s "!OUTPUT_FILE!"
            ) else (
                call :log "Skipping !SOURCE_FILE! - not found"
            )
        )
    )

    :: Summary
    if "!COMPILATION_FAILED!"=="false" (
        %GREEN% "All shaders compiled successfully!"
        call :log "Compilation completed successfully"
    ) else (
        %RED% "Shader compilation completed with errors!"
        call :log "Compilation completed with errors"
        exit /b 1
    )
goto :eof

:: Parse command line arguments
:parse_args
    if /i "%~1"=="/h" goto :help
    if /i "%~1"=="/v" set "VERBOSE=true" & shift & goto :parse_args
    if /i "%~1"=="/clean" rmdir /s /q "%SHADER_BAKED_DIR%" 2>nul & shift & goto :parse_args
goto :main_entry

:help
    echo Usage: %0 [options]
    echo Options:
    echo   /h        Show this help message
    echo   /v        Enable verbose output
    echo   /clean    Clean baked shader directory before compilation
    exit /b 0

:main_entry
    :: Create log directory if needed
    mkdir "%~dp0" 2>nul

    :: Execute main function
    call :main

    :: Pause if not in CI environment
    if not defined CI pause
exit /b %ERRORLEVEL%
