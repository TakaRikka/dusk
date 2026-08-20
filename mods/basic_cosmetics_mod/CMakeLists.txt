cmake_minimum_required(VERSION 3.25)
project(basic_cosmetics_mod CXX)

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

include(FetchContent)

message(STATUS "basic_cosmetics_mod: Fetching xxhash")
FetchContent_Declare(
        xxhash
        GIT_REPOSITORY https://github.com/Cyan4973/xxHash.git
        GIT_TAG        v0.8.3
)
FetchContent_MakeAvailable(xxhash)

set(COSMETIC_SOURCES src/color_utils.cpp src/midna_hair_color.cpp src/texture_utils.cpp src/hooks.cpp)

add_mod(basic_cosmetics_mod
    FEATURES game fmt
    SOURCES  src/mod.cpp ${COSMETIC_SOURCES}
    MOD_JSON mod.json
    RES_DIR  res
    BUNDLE
)

target_link_libraries(basic_cosmetics_mod PRIVATE xxHash::xxhash)
