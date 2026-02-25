cmake_minimum_required(VERSION 3.13)
if (APPLE)
    project(dusk LANGUAGES C CXX OBJC OBJCXX)
else ()
    project(dusk LANGUAGES C CXX)
endif ()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(extern/aurora EXCLUDE_FROM_ALL)

option(DUSK_BUILD_WARNINGS "If off, compiler warnings will be suppressed")

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unknown-pragmas -Wno-unused-variable -Wno-unused-parameter")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++ -Wno-register -fPIC")
    set(CMAKE_PREFIX_PATH /usr)
    set(CMAKE_LIBRARY_ARCHITECTURE x86_64-linux-gnu)
    set(CMAKE_LIBRARY_PATH "/usr/lib64" "/usr/lib/x86_64-linux-gnu" CACHE PATH "")
    set(CMAKE_FIND_LIBRARY_CUSTOM_LIB_SUFFIX 64)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    set(CMAKE_BUILD_RPATH "$ORIGIN")
elseif (APPLE)
	add_compile_options(-Wno-declaration-after-statement)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    set(CMAKE_BUILD_RPATH "$ORIGIN")
elseif (MSVC)
    add_compile_options(/bigobj)
    add_compile_options(/Zc:strictStrings-)
    add_compile_options(/MP)
    if (NOT DUSK_BUILD_WARNINGS)
        add_compile_options(/W0)
    endif ()
    add_compile_options(/utf-8)
endif ()
if (NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error -Wno-c++11-narrowing")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error -Wno-c++11-narrowing")
endif ()

include(files.cmake)

# TODO: version handling for res includes
set(DUSK_TP_VERSION GZ2E01)

source_group("dolzel" FILES ${DOLZEL_FILES} ${Z2AUDIOLIB_FILES} ${JSYSTEM_FILES} ${JSYSTEM_DEBUG_FILES} ${REL_FILES})
source_group("dusk" FILES ${DUSK_FILES})

# game_debug is for game code files that we know work when compiled with DEBUG=1
# Of course, if building a release build, this distinction is irrelevant
add_library(game_debug STATIC ${JSYSTEM_DEBUG_FILES} ${SSYSTEM_FILES})
target_compile_definitions(game_debug PRIVATE TARGET_PC VERSION=0 $<$<CONFIG:Debug>:DEBUG=1>)

# Make these properties PUBLIC so that the regular game target also sees them.
target_include_directories(game_debug PUBLIC include src assets/${DUSK_TP_VERSION} ${CMAKE_BINARY_DIR}/../${DUSK_TP_VERSION}/include)
target_link_libraries(game_debug PUBLIC aurora::core aurora::gx aurora::si aurora::vi aurora::pad aurora::mtx)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(game SHARED ${DOLZEL_FILES} ${Z2AUDIOLIB_FILES} ${SSYSTEM_FILES} ${JSYSTEM_FILES} ${REL_FILES} ${DUSK_FILES})
target_link_libraries(game PRIVATE game_debug)
target_compile_definitions(game PRIVATE TARGET_PC VERSION=0 NDEBUG=1 NDEBUG_DEFINED=1 DEBUG_DEFINED=0)

add_executable(dusk src/dusk/main.cpp)
target_compile_definitions(dusk PRIVATE TARGET_PC VERSION=0)
target_include_directories(dusk PRIVATE include)
target_link_libraries(dusk PRIVATE game aurora::main)
