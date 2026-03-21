cmake_minimum_required(VERSION 3.16)
if (APPLE)
    project(dusk LANGUAGES C CXX OBJC OBJCXX)
else ()
    project(dusk LANGUAGES C CXX)
endif ()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(DAWN_USE_WAYLAND ON CACHE BOOL "Enable support for Wayland surface" FORCE)
endif ()
set(AURORA_ENABLE_DVD ON CACHE BOOL "Enable DVD API support" FORCE)
set(AURORA_ENABLE_CARD ON CACHE BOOL "Enable CARD API support" FORCE)
add_subdirectory(extern/aurora EXCLUDE_FROM_ALL)

add_subdirectory(libs/freeverb)

option(DUSK_BUILD_WARNINGS "If off, compiler warnings will be suppressed")
option(DUSK_SELECTED_OPT "If on, selected parts of the project will be compiled with optimizations on Debug, intending to make the game run at 30 FPS. Note for MSVC: you will need to remove '/RTC1' from your debug flags in CMake.")
option(DUSK_MOVIE_SUPPORT "If on, compile against libjpeg-turbo to enable THP file decoding" ON)

find_package(libjpeg-turbo)
set(DUSK_MOVIE_SUPPORT_REAL ${DUSK_MOVIE_SUPPORT})
if (DUSK_MOVIE_SUPPORT AND NOT libjpeg-turbo_FOUND)
    message(WARNING "libjpeg-turbo not found but DUSK_MOVIE_SUPPORT set, movie playback will not be available!")
    set(DUSK_MOVIE_SUPPORT_REAL OFF)
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    # -Wno-multichar: Multi-character constants ('ABCD') are implementation-defined but all compilers
    #   (CW, GCC, Clang, MSVC) encode them identically in big-endian order.
    #   For >4-char literals (which GCC/Clang truncate to int), use the MULTI_CHAR() macro.
    # -Wwrite-strings: Game code relies on implicit const char* -> char* conversions
    # -Wdeprecated-declarations: JSystem uses std::iterator, deprecated in C++17
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-multichar -Wno-write-strings")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar -Wno-write-strings -Wno-trigraphs -Wno-deprecated-declarations")
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    set(CMAKE_BUILD_RPATH "$ORIGIN")
elseif (APPLE)
    add_compile_options(-Wno-declaration-after-statement -Wno-non-pod-varargs)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    set(CMAKE_BUILD_RPATH "$ORIGIN")
elseif (MSVC)
    add_compile_options(/bigobj)
    add_compile_options(/Zc:strictStrings-)
    add_compile_options(/MP)
    if (NOT DUSK_BUILD_WARNINGS)
        add_compile_options(/W0)
    else ()
        # Disable warnings
        add_compile_options(/wd4068) # unknown pragma
        add_compile_options(/wd4291) # no matching delete operator, leaks if exception thrown
        # Only show warnings once
        add_compile_options(/wo4244) # narrowing conversion, possible data loss
    endif ()
    add_compile_options(/utf-8)
endif ()


include(FetchContent)

message(STATUS "dusk: Fetching cxxopts")
FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v3.3.1
)

FetchContent_MakeAvailable(cxxopts)

include(files.cmake)

# TODO: version handling for res includes

set(DUSK_GAME_NAME "GZ2E")
set(DUSK_GAME_VERSION "01")

set(DUSK_TP_VERSION ${DUSK_GAME_NAME}${DUSK_GAME_VERSION})

message(STATUS "dusk: Game Name: ${DUSK_GAME_NAME}")
message(STATUS "dusk: Game Version: ${DUSK_GAME_VERSION}")
message(STATUS "dusk: TP Version: ${DUSK_TP_VERSION}")

source_group("dolzel" FILES ${DOLZEL_FILES} ${Z2AUDIOLIB_FILES} ${JSYSTEM_FILES} ${JSYSTEM_DEBUG_FILES} ${REL_FILES})
source_group("dusk" FILES ${DUSK_FILES})

set(GAME_COMPILE_DEFS TARGET_PC WIDESCREEN_SUPPORT AVOID_UB=1 VERSION=0
    DUSK_TP_VERSION="${DUSK_TP_VERSION}" DUSK_GAME_NAME="${DUSK_GAME_NAME}" DUSK_GAME_VERSION="${DUSK_GAME_VERSION}")

set(GAME_INCLUDE_DIRS
    include
    src
    assets/${DUSK_TP_VERSION}
    libs/JSystem/include
    libs
    extern/aurora/include/dolphin
    extern
    ${CMAKE_SOURCE_DIR}/build/${DUSK_TP_VERSION}/include
    build/${DUSK_TP_VERSION}/include)

set(GAME_LIBS aurora::core aurora::gx aurora::gd aurora::si aurora::vi aurora::pad aurora::mtx aurora::os aurora::dvd
    aurora::card freeverb cxxopts::cxxopts absl::flat_hash_map)

if (DUSK_MOVIE_SUPPORT_REAL)
  if (TARGET libjpeg-turbo::turbojpeg-static)
    message(STATUS "dusk: Linking against libjpeg-turbo static library")
    list(APPEND GAME_LIBS "libjpeg-turbo::turbojpeg-static")
  else ()
    message(STATUS "dusk: Linking against libjpeg-turbo shared library")
    list(APPEND GAME_LIBS "libjpeg-turbo::turbojpeg")
  endif ()
  list(APPEND GAME_COMPILE_DEFS MOVIE_SUPPORT=1)
endif ()

# game_debug is for game code files that we know work when compiled with DEBUG=1
# Of course, if building a release build, this distinction is irrelevant
add_library(game_debug OBJECT ${JSYSTEM_DEBUG_FILES} ${SSYSTEM_FILES}
        src/dusk/audio/DuskAudioSystem.cpp
        src/dusk/audio/JASCriticalSection.cpp
        src/dusk/audio/DuskDsp.hpp
        src/dusk/audio/DuskDsp.cpp
        src/dusk/audio/Adpcm.cpp
        src/dusk/audio/Adpcm.hpp
        src/dusk/audio/DspStub.cpp)

# game_base is for all other game code files
add_library(game_base OBJECT ${DOLZEL_FILES} ${Z2AUDIOLIB_FILES} ${JSYSTEM_FILES} ${REL_FILES} ${DUSK_FILES}
        ${DOLPHIN_FILES}
        src/dusk/imgui/ImGuiStubLog.cpp
        src/dusk/imgui/ImGuiAudio.cpp)

target_compile_definitions(game_debug PRIVATE ${GAME_COMPILE_DEFS} $<$<CONFIG:Debug>:DEBUG=1>)
target_compile_definitions(game_base PRIVATE ${GAME_COMPILE_DEFS} NDEBUG=1 NDEBUG_DEFINED=1 DEBUG_DEFINED=0)

# only apply PCH to game_base since not all headers are necessarily validated with DEBUG=1
target_precompile_headers(game_base PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_SOURCE_DIR}/include/dusk_pch.hpp>")

target_include_directories(game_debug PRIVATE ${GAME_INCLUDE_DIRS})
target_include_directories(game_base PRIVATE ${GAME_INCLUDE_DIRS})

# This implicitly pulls in the library include directories even though no
# linking actually takes place for object libraries
target_link_libraries(game_debug PRIVATE ${GAME_LIBS})
target_link_libraries(game_base PRIVATE ${GAME_LIBS})

# Combined game library
add_library(game STATIC
    $<TARGET_OBJECTS:game_base>
    $<TARGET_OBJECTS:game_debug>)
target_link_libraries(game PUBLIC ${GAME_LIBS})

add_executable(dusk src/dusk/main.cpp)
target_compile_definitions(dusk PRIVATE TARGET_PC AVOID_UB=1 VERSION=0)
target_include_directories(dusk PRIVATE include)
target_link_libraries(dusk PRIVATE game aurora::main freeverb)

add_custom_command(TARGET dusk POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:dusk>/res"
    COMMENT "Copying resources"
)

include(extern/aurora/cmake/AuroraCopyRuntimeDLLs.cmake)
aurora_copy_runtime_dlls(dusk)

if (DUSK_SELECTED_OPT)
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        set(_opt_flags /O2 /Ob2)
    elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        set(_opt_flags -O2)
    endif ()

    target_compile_options(xxhash PRIVATE ${_opt_flags})
    target_compile_options(aurora_gx PRIVATE ${_opt_flags})
    target_compile_options(freeverb PRIVATE ${_opt_flags})
endif ()
