# The game ABI surface shared by the main build and the mod SDK (sdk/CMakeLists.txt)
include_guard(GLOBAL)

get_filename_component(_game_root "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

set(DUSK_GAME_VERSION "GZ2E01" CACHE STRING "Target game version (GZ2E01, GZ2J01, GZ2P01)")

if (DUSK_GAME_VERSION STREQUAL "GZ2E01")
    set(_game_version_val 0)
elseif (DUSK_GAME_VERSION STREQUAL "GZ2P01")
    set(_game_version_val 1)
elseif (DUSK_GAME_VERSION STREQUAL "GZ2J01")
    set(_game_version_val 2)
else ()
    message(FATAL_ERROR "Unsupported target game version: ${DUSK_GAME_VERSION}")
endif ()

# PARTIAL_DEBUG makes debug and release share one struct/vtable ABI so a mod binary loads into either
set(_game_compile_defs TARGET_PC=1 WIDESCREEN_SUPPORT=1 AVOID_UB=1 VERSION=${_game_version_val} MTX_USE_PS=1 PARTIAL_DEBUG=1)
if (ANDROID)
    list(APPEND _game_compile_defs TARGET_ANDROID=1)
endif ()

set(_game_include_dirs
        ${_game_root}/include
        ${_game_root}/src
        ${_game_root}/assets/${DUSK_GAME_VERSION}
        ${_game_root}/libs/JSystem/include
        ${_game_root}/libs
        ${_game_root}/extern/aurora/include/dolphin
        ${_game_root}/extern/aurora/include
        ${_game_root}/extern
        ${CMAKE_CURRENT_BINARY_DIR}
)

# Interface target for mods and sub-projects to inherit game headers/defines.
add_library(dusklight_game_headers INTERFACE)
target_include_directories(dusklight_game_headers INTERFACE ${_game_include_dirs})
target_compile_definitions(dusklight_game_headers INTERFACE ${_game_compile_defs})
