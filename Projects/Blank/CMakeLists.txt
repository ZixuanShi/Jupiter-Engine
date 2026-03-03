# Copyright Jupiter Technologies, Inc. All Rights Reserved.

cmake_minimum_required(VERSION 3.31)
project(Blank CXX)

# Locate the engine — mirrors the JUPITER_ENGINE_ROOT env var used by premake5.lua
set(JUPITER_ENGINE_ROOT "$ENV{JUPITER_ENGINE_ROOT}")
if(NOT JUPITER_ENGINE_ROOT)
    set(JUPITER_ENGINE_ROOT "C:/Program Files/Jupiter Technologies/Jupiter-Engine")
    message(WARNING "JUPITER_ENGINE_ROOT not set. Defaulting to: ${JUPITER_ENGINE_ROOT}")
endif()

# Pull in the engine as a subdirectory (builds JupiterEngine static lib)
add_subdirectory("${JUPITER_ENGINE_ROOT}" "${JUPITER_ENGINE_ROOT}/_ProjectFiles/Engine")

# Pull in the client project helper macro
include("${JUPITER_ENGINE_ROOT}/Scripts/JupiterEngineClient.cmake")

# Create the Blank executable and wire it to JupiterEngine
jupiter_add_client(Blank)
