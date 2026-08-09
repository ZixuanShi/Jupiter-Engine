file(GLOB_RECURSE JUPITER_EDITOR_MODULE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cppm")

add_executable(JupiterEditor Source/Main.cpp)
target_sources(JupiterEditor
    PUBLIC
        FILE_SET CXX_MODULES
        FILES
            ${JUPITER_EDITOR_MODULE_SOURCES}
)

jupiter_link_engine(JupiterEditor)
jupiter_apply_platform_fixups(JupiterEditor)
