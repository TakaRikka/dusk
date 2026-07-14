# Dusklight Mod SDK entry point
#
# Provides selectable mod service, game ABI, and WebGPU header surfaces without configuring the
# full game tree.
#
# Usage (from a mod project):
#   add_subdirectory(<dusk>/sdk dusk-sdk EXCLUDE_FROM_ALL)
#   add_mod(my_mod FEATURES game webgpu SOURCES ... MOD_JSON mod.json)
#
# TODO: auto-download link targets from tag

cmake_minimum_required(VERSION 3.25)

if (TARGET dusklight_mod_api)
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

# Mod API and optional feature header surfaces
include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/GameABIConfig.cmake")

if (WIN32 OR APPLE OR ANDROID)
    set(DUSK_GAME_EXE "" CACHE FILEPATH "Game binary or link stub mods link against")
    if (DUSK_GAME_EXE AND NOT EXISTS "${DUSK_GAME_EXE}")
        message(FATAL_ERROR "Mod SDK: DUSK_GAME_EXE does not exist: ${DUSK_GAME_EXE}")
    endif ()
endif ()

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/ModSDK.cmake")
