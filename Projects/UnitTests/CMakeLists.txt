# Copyright Jupiter Technologies, Inc. All Rights Reserved.

cmake_minimum_required(VERSION 3.31)
project(UnitTests CXX)

set(JUPITER_ENGINE_ROOT "$ENV{JUPITER_ENGINE_ROOT}")
if(NOT JUPITER_ENGINE_ROOT)
    set(JUPITER_ENGINE_ROOT "C:/Program Files/Jupiter Technologies/Jupiter-Engine")
    message(WARNING "JUPITER_ENGINE_ROOT not set. Defaulting to: ${JUPITER_ENGINE_ROOT}")
endif()

add_subdirectory("${JUPITER_ENGINE_ROOT}" "${JUPITER_ENGINE_ROOT}/_ProjectFiles/Engine")
include("${JUPITER_ENGINE_ROOT}/Scripts/JupiterEngineClient.cmake")

jupiter_add_client(UnitTests)
