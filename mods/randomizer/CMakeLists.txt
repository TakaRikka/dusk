cmake_minimum_required(VERSION 3.25)
project(randomizer CXX)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(DUSK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." CACHE PATH "Path to dusk source root")
    option(DUSK_MOD_USE_FULL_TREE "Use full build instead of the minimal mod SDK" OFF)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    if (DUSK_MOD_USE_FULL_TREE)
        add_subdirectory("${DUSK_DIR}" dusk EXCLUDE_FROM_ALL)
    else ()
        add_subdirectory("${DUSK_DIR}/sdk" dusk-sdk EXCLUDE_FROM_ALL)
    endif ()
endif ()

# Generator dependencies, linked statically into the mod library (no CRT crossing).
include(FetchContent)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

message(STATUS "randomizer: Fetching yaml-cpp")
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.9.0
)
message(STATUS "randomizer: Fetching base64pp")
FetchContent_Declare(
        base64pp
        GIT_REPOSITORY https://github.com/matheusgomes28/base64pp.git
        GIT_TAG v0.2.0-rc0
)

message(STATUS "randomizer: Fetching battery-embed")
FetchContent_Declare(
        battery-embed
        GIT_REPOSITORY https://github.com/batterycenter/embed.git
        GIT_TAG        fdbae3f
)

FetchContent_MakeAvailable(yaml-cpp base64pp battery-embed)

set(RANDOMIZER_GENERATOR_SOURCES
        generator/logic/area.cpp
        generator/logic/dungeon.cpp
        generator/logic/entrance.cpp
        generator/logic/entrance_shuffle.cpp
        generator/logic/fill.cpp
        generator/logic/flatten/bits.cpp
        generator/logic/flatten/flatten.cpp
        generator/logic/flatten/simplify_algebraic.cpp
        generator/logic/hints.cpp
        generator/logic/item.cpp
        generator/logic/item_pool.cpp
        generator/logic/location.cpp
        generator/logic/plandomizer.cpp
        generator/logic/requirement.cpp
        generator/logic/search.cpp
        generator/logic/spoiler_log.cpp
        generator/logic/world.cpp
        generator/randomizer.cpp
        generator/seedgen/config.cpp
        generator/seedgen/seed.cpp
        generator/seedgen/settings.cpp
        generator/utility/color.cpp
        generator/utility/common.cpp
        generator/utility/endian.cpp
        generator/utility/file.cpp
        generator/utility/log.cpp
        generator/utility/path.cpp
        generator/utility/platform.cpp
        generator/utility/random.cpp
        generator/utility/string.cpp
        generator/utility/text.cpp
        generator/utility/time.cpp
)

set(RANDOMIZER_SOURCES
        src/flags.cpp
        src/messages.cpp
        src/randomizer_context.cpp
        src/stages.cpp
        src/tools.cpp
        src/verify_item_functions.cpp
        src/paths.cpp
        src/session.cpp
)

# Embed the generator's YAML data into the mod library. Paths are relative to this
# directory and must match the RANDO_DATA_PATH-based b::embed<> identifiers in code.
file(GLOB_RECURSE RANDOMIZER_DATA RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/generator/data/*")

# Create an OBJECT library to hold all battery-embed generated files so we can add it
# to both the mod and the standalone generator
add_library(randomizer_embeds OBJECT)

b_embed(randomizer_embeds "res/shadow_crystal.bti")
foreach (RANDOMIZER_FILE IN LISTS RANDOMIZER_DATA)
    if (RANDOMIZER_FILE MATCHES "^generator/data/tests")
        continue()
    endif ()
    b_embed(randomizer_embeds "${RANDOMIZER_FILE}")
endforeach ()

add_mod(randomizer
    FEATURES game
    SOURCES  src/mod.cpp ${RANDOMIZER_GENERATOR_SOURCES} ${RANDOMIZER_SOURCES}
    MOD_JSON mod.json
    RES_DIR  res
)

target_link_libraries(randomizer PRIVATE yaml-cpp::yaml-cpp base64pp fmt::fmt randomizer_embeds)

string(LENGTH "${CMAKE_CURRENT_SOURCE_DIR}/" RANDOMIZER_SOURCE_PATH_SIZE)
target_compile_definitions(randomizer PRIVATE
        RANDO_DATA_PATH="generator/data/"
        SOURCE_PATH_SIZE=${RANDOMIZER_SOURCE_PATH_SIZE}
)

# standalone randomizer generator for testing
add_executable(randomizer_generator ${CMAKE_CURRENT_SOURCE_DIR}/generator/generator_only.cpp ${RANDOMIZER_GENERATOR_SOURCES})
target_link_libraries(randomizer_generator PRIVATE yaml-cpp::yaml-cpp base64pp fmt::fmt randomizer_embeds)
target_compile_definitions(randomizer_generator PRIVATE
        RANDO_DATA_PATH="generator/data/"
        SOURCE_PATH_SIZE=${RANDOMIZER_SOURCE_PATH_SIZE}
        RANDO_SAVE_PATH="${CMAKE_BINARY_DIR}/randomizer/"
        RANDOMIZER_ONLY)
