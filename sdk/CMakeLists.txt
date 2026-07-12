# Dusklight Mod SDK entry point
#
# Provides game/service headers, compile definitions, version.h and WebGPU
# headers without configuring the full game tree.
#
# Usage (from a mod project):
#   add_subdirectory(<dusk>/sdk dusk-sdk EXCLUDE_FROM_ALL)
#   add_mod(my_mod SOURCES ... MOD_JSON mod.json)
#
# On Windows, pass -DDUSK_GAME_IMPLIB=<path to sdk/dusklight.lib> from the
# matching game release. TODO: auto-download from tag

cmake_minimum_required(VERSION 3.25)

if (TARGET dusklight_game_headers)
    message(FATAL_ERROR "Mod SDK already configured")
endif ()

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/WindowsTargetProcessor.cmake")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
            "Build type options: Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif ()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Version detection & version.h
include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/DetectVersion.cmake")
detect_version()
configure_version_header()

# Provides dawn::webgpu_dawn and dawn::dawncpp_headers for public gfx service headers.
include("${CMAKE_CURRENT_SOURCE_DIR}/../extern/aurora/cmake/AuroraDependencyVersions.cmake")
set(AURORA_DAWN_PROVIDER "package" CACHE STRING "How to provide Dawn for the mod SDK")
include("${CMAKE_CURRENT_SOURCE_DIR}/../extern/aurora/cmake/AuroraDawnProvider.cmake")

# Game ABI headers & compile definitions
include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/GameABIConfig.cmake")

if (WIN32)
    set(DUSK_GAME_IMPLIB "" CACHE FILEPATH "Path to the dusklight import library (sdk/dusklight.lib)")
    if (DUSK_GAME_IMPLIB AND NOT EXISTS "${DUSK_GAME_IMPLIB}")
        message(FATAL_ERROR "Mod SDK: DUSK_GAME_IMPLIB does not exist: ${DUSK_GAME_IMPLIB}")
    endif ()
endif ()

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/ModSDK.cmake")
