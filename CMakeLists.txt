cmake_minimum_required(VERSION 3.25)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
            "Build type options: Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif ()

# obtain revision info from git
find_package(Git)
if (GIT_FOUND)
    # make sure version information gets re-run when the current Git HEAD changes
    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} rev-parse --git-path HEAD
            OUTPUT_VARIABLE dusk_git_head_filename
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${dusk_git_head_filename}")

    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} rev-parse --symbolic-full-name HEAD
            OUTPUT_VARIABLE dusk_git_head_symbolic
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${GIT_EXECUTABLE} rev-parse --git-path ${dusk_git_head_symbolic}
            OUTPUT_VARIABLE dusk_git_head_symbolic_filename
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${dusk_git_head_symbolic_filename}")

    # defines DUSK_WC_REVISION
    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
            OUTPUT_VARIABLE DUSK_WC_REVISION
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    # defines DUSK_WC_DESCRIBE
    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} describe --tags --long --dirty --match "v*"
            OUTPUT_VARIABLE DUSK_WC_DESCRIBE
            OUTPUT_STRIP_TRAILING_WHITESPACE)

    # remove the git hash, then collapse a clean "-0" suffix only
    string(REGEX REPLACE "-[^-]+(-dirty|)$" "\\1" DUSK_WC_DESCRIBE "${DUSK_WC_DESCRIBE}")
    string(REGEX REPLACE "-0$" "" DUSK_WC_DESCRIBE "${DUSK_WC_DESCRIBE}")

    # defines DUSK_WC_BRANCH
    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
            OUTPUT_VARIABLE DUSK_WC_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    # defines DUSK_WC_DATE
    execute_process(WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND ${GIT_EXECUTABLE} log -1 --format=%ad
            OUTPUT_VARIABLE DUSK_WC_DATE
            OUTPUT_STRIP_TRAILING_WHITESPACE)
else ()
    message(STATUS "Unable to find git, commit information will not be available")
endif ()

if (DUSK_WC_DESCRIBE MATCHES "^v([0-9]+)\\.([0-9]+)\\.([0-9]+)(-([0-9]+).*)?$")
    set(DUSK_SHORT_VERSION_STRING "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}")
    if (CMAKE_MATCH_5)
        set(DUSK_VERSION_STRING "${DUSK_SHORT_VERSION_STRING}.${CMAKE_MATCH_5}")
    else ()
        set(DUSK_VERSION_STRING "${DUSK_SHORT_VERSION_STRING}.0")
    endif ()
else ()
    set(DUSK_WC_DESCRIBE "UNKNOWN-VERSION")
    set(DUSK_VERSION_STRING "0.0.0.0")
    set(DUSK_SHORT_VERSION_STRING "0.0.0")
endif ()

# Add version information to CI environment variables
if(DEFINED ENV{GITHUB_ENV})
    file(APPEND "$ENV{GITHUB_ENV}" "DUSK_VERSION=${DUSK_WC_DESCRIBE}\n")
endif()
message(STATUS "Dusk version set to ${DUSK_WC_DESCRIBE}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
project(dusk LANGUAGES C CXX VERSION ${DUSK_VERSION_STRING})
if (APPLE AND NOT TVOS AND CMAKE_SYSTEM_NAME STREQUAL tvOS)
    # ios.toolchain.cmake hack for SDL
    set(TVOS ON)
    set(IOS OFF)
endif ()

if(APPLE AND NOT CMAKE_OSX_SYSROOT)
    # If the Xcode SDK is lagging behind system version, CMake needs this done first
    execute_process(COMMAND xcrun --sdk macosx --show-sdk-path
            OUTPUT_VARIABLE CMAKE_OSX_SYSROOT
            OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(DAWN_USE_WAYLAND ON CACHE BOOL "Enable support for Wayland surface" FORCE)
endif ()
set(AURORA_ENABLE_DVD ON CACHE BOOL "Enable DVD API support" FORCE)
set(AURORA_ENABLE_CARD ON CACHE BOOL "Enable CARD API support" FORCE)
add_subdirectory(extern/aurora EXCLUDE_FROM_ALL)

add_subdirectory(libs/freeverb)

option(DUSK_BUILD_WARNINGS "Enable compiler warnings (off by default)")
option(DUSK_SELECTED_OPT "If on, selected parts of the project will be compiled with optimizations on Debug, intending to make the game run at 30 FPS. Note for MSVC: you will need to remove '/RTC1' from your debug flags in CMake.")
option(DUSK_MOVIE_SUPPORT "If on, compile against libjpeg-turbo to enable THP file decoding" ON)

if(ANDROID)
    set(DUSK_MOVIE_SUPPORT OFF)
    set(NOD_COMPRESS_BZIP2 OFF CACHE BOOL "" FORCE)
    set(NOD_COMPRESS_LZMA OFF CACHE BOOL "" FORCE)
    set(NOD_COMPRESS_ZLIB OFF CACHE BOOL "" FORCE)
    set(NOD_COMPRESS_ZSTD OFF CACHE BOOL "" FORCE)
endif ()

option(DUSK_ENABLE_SENTRY_NATIVE "Enable sentry-native crash reporting support" OFF)
set(DUSK_SENTRY_DSN "" CACHE STRING "Sentry DSN")
set(DUSK_SENTRY_ENVIRONMENT "development" CACHE STRING "Sentry environment")

if (DUSK_MOVIE_SUPPORT)
    find_package(libjpeg-turbo QUIET)
    if (libjpeg-turbo_FOUND)
        message(STATUS "dusk: Using system libjpeg-turbo")
    else ()
        message(STATUS "dusk: Fetching libjpeg-turbo")
        include(ExternalProject)
        set(_jpeg_install_dir ${CMAKE_BINARY_DIR}/libjpeg-turbo-install)
        if (WIN32)
            set(_jpeg_lib ${_jpeg_install_dir}/lib/turbojpeg-static.lib)
        else ()
            set(_jpeg_lib ${_jpeg_install_dir}/lib/libturbojpeg.a)
        endif ()
        ExternalProject_Add(libjpeg-turbo-ext
            URL https://github.com/libjpeg-turbo/libjpeg-turbo/archive/refs/tags/3.1.0.tar.gz
            URL_HASH SHA256=35fec2e1ddfb05ecf6d93e50bc57c1e54bc81c16d611ddf6eff73fff266d8285
            CMAKE_ARGS
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DCMAKE_INSTALL_PREFIX=${_jpeg_install_dir}
                -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                -DCMAKE_C_COMPILER_LAUNCHER=${CMAKE_C_COMPILER_LAUNCHER}
                -DCMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}
                -DENABLE_SHARED=OFF
                -DWITH_TURBOJPEG=ON
                -DWITH_JAVA=OFF
            BUILD_BYPRODUCTS ${_jpeg_lib}
        )
        file(MAKE_DIRECTORY ${_jpeg_install_dir}/include)
        add_library(libjpeg-turbo::turbojpeg-static STATIC IMPORTED GLOBAL)
        set_target_properties(libjpeg-turbo::turbojpeg-static PROPERTIES
            IMPORTED_LOCATION ${_jpeg_lib}
            INTERFACE_INCLUDE_DIRECTORIES ${_jpeg_install_dir}/include
        )
        add_dependencies(libjpeg-turbo::turbojpeg-static libjpeg-turbo-ext)
    endif ()
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
    add_compile_options(
        $<$<COMPILE_LANGUAGE:C,CXX>:/bigobj>
        $<$<COMPILE_LANGUAGE:C,CXX>:/Zc:strictStrings->
        $<$<COMPILE_LANGUAGE:C,CXX>:/MP>
        $<$<COMPILE_LANGUAGE:C,CXX>:/FS>
    )

    if (NOT DUSK_BUILD_WARNINGS)
        add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/W0>)
    else ()
        # Disable warnings
        add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/wd4068>) # unknown pragma
        add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/wd4291>) # no matching delete operator, leaks if exception thrown
        # Only show warnings once
        add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/wo4244>) # narrowing conversion, possible data loss
    endif ()
    add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/utf-8>)
endif ()


include(FetchContent)

# Declare all dependencies first so CMake can download them in parallel
message(STATUS "dusk: Fetching cxxopts")
FetchContent_Declare(cxxopts
    URL https://github.com/jarro2783/cxxopts/archive/refs/tags/v3.3.1.tar.gz
    URL_HASH SHA256=3bfc70542c521d4b55a46429d808178916a579b28d048bd8c727ee76c39e2072
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
message(STATUS "dusk: Fetching nlohmann/json")
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(cxxopts json)

if (DUSK_ENABLE_SENTRY_NATIVE)
    message(STATUS "dusk: Fetching sentry-native")
    set(SENTRY_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(SENTRY_BACKEND crashpad CACHE STRING "" FORCE)
    if (WIN32)
        set(SENTRY_TRANSPORT winhttp CACHE STRING "" FORCE)
    endif ()
    set(SENTRY_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(SENTRY_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SENTRY_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(sentry_native
        GIT_REPOSITORY https://github.com/getsentry/sentry-native.git
        GIT_TAG 0.13.6
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        GIT_SUBMODULES_RECURSE TRUE
    )
    if (NOT sentry_native_POPULATED)
        FetchContent_Populate(sentry_native)
        set(_dusk_skip_install_rules ${CMAKE_SKIP_INSTALL_RULES})
        set(CMAKE_SKIP_INSTALL_RULES ON)
        add_subdirectory(${sentry_native_SOURCE_DIR} ${sentry_native_BINARY_DIR} EXCLUDE_FROM_ALL)
        set(CMAKE_SKIP_INSTALL_RULES ${_dusk_skip_install_rules})
    endif ()
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(PLATFORM_NAME win32)
elseif (CMAKE_SYSTEM_NAME STREQUAL Darwin)
    if (IOS)
        set(PLATFORM_NAME ios)
    elseif (TVOS)
        set(PLATFORM_NAME tvos)
    else ()
        set(PLATFORM_NAME macos)
    endif ()
else ()
    string(TOLOWER CMAKE_SYSTEM_NAME PLATFORM_NAME)
endif ()

configure_file(${CMAKE_SOURCE_DIR}/version.h.in ${CMAKE_BINARY_DIR}/version.h)

include(files.cmake)

# TODO: version handling for res includes

set(DUSK_BUNDLE_NAME Dusk)
set(DUSK_BUNDLE_IDENTIFIER dev.decomp.dusk)
set(DUSK_COMPANY_NAME "Twilit Realm")
set(DUSK_FILE_DESCRIPTION "Dusk")
set(DUSK_PRODUCT_NAME "Dusk")
set(DUSK_COPYRIGHT "Copyright (C) Twilit Realm contributors")
set(DUSK_GAME_NAME "GZ2E")
set(DUSK_GAME_VERSION "01")
set(DUSK_TP_VERSION ${DUSK_GAME_NAME}${DUSK_GAME_VERSION})

message(STATUS "dusk: Game Version: ${DUSK_TP_VERSION}")

source_group("dolzel" FILES ${DOLZEL_FILES} ${Z2AUDIOLIB_FILES} ${JSYSTEM_FILES} ${JSYSTEM_DEBUG_FILES} ${REL_FILES})
source_group("dusk" FILES ${DUSK_FILES})

set(GAME_COMPILE_DEFS TARGET_PC WIDESCREEN_SUPPORT=1 AVOID_UB=1 VERSION=0
    DUSK_TP_VERSION="${DUSK_TP_VERSION}" DUSK_GAME_NAME="${DUSK_GAME_NAME}" DUSK_GAME_VERSION="${DUSK_GAME_VERSION}")

set(GAME_INCLUDE_DIRS
    include
    src
    assets/${DUSK_TP_VERSION}
    libs/JSystem/include
    libs
    extern/aurora/include/dolphin
    extern
    ${CMAKE_BINARY_DIR})

set(GAME_LIBS aurora::core aurora::gx aurora::gd aurora::si aurora::vi aurora::pad aurora::mtx aurora::os aurora::dvd
    aurora::card freeverb cxxopts::cxxopts absl::flat_hash_map nlohmann_json::nlohmann_json TracyClient)

list(APPEND GAME_LIBS libzstd_static)

if (DUSK_ENABLE_SENTRY_NATIVE)
    list(APPEND GAME_LIBS sentry)
    list(APPEND GAME_COMPILE_DEFS DUSK_ENABLE_SENTRY_NATIVE=1 SENTRY_BUILD_STATIC=1)
endif ()

if (DUSK_MOVIE_SUPPORT)
    if (TARGET libjpeg-turbo::turbojpeg-static)
        list(APPEND GAME_LIBS libjpeg-turbo::turbojpeg-static)
    else ()
        list(APPEND GAME_LIBS libjpeg-turbo::turbojpeg)
    endif ()
    list(APPEND GAME_COMPILE_DEFS MOVIE_SUPPORT=1)
endif ()

# Edit & Continue
if (MSVC)
    add_compile_options("/ZI")
    add_link_options("/INCREMENTAL")
endif ()

if(ANDROID)
    list(APPEND GAME_COMPILE_DEFS TARGET_ANDROID=1)
endif ()

# game_debug is for game code files that we know work when compiled with DEBUG=1
# Of course, if building a release build, this distinction is irrelevant
add_library(game_debug OBJECT ${JSYSTEM_DEBUG_FILES} ${SSYSTEM_FILES}
        src/dusk/audio/DuskAudioSystem.cpp
        src/dusk/audio/JASCriticalSection.cpp
        src/dusk/audio/DuskDsp.cpp
        src/dusk/audio/Adpcm.cpp
        src/dusk/audio/DspStub.cpp
        src/dusk/imgui/ImGuiAudio.cpp)

# game_base is for all other game code files
add_library(game_base OBJECT ${DOLZEL_FILES} ${Z2AUDIOLIB_FILES} ${JSYSTEM_FILES} ${REL_FILES} ${DUSK_FILES} ${DOLPHIN_FILES})

target_compile_definitions(game_debug PRIVATE ${GAME_COMPILE_DEFS} $<$<CONFIG:Debug>:DEBUG=1> $<$<CONFIG:Debug>:PARTIAL_DEBUG=1>)
target_compile_definitions(game_base PRIVATE ${GAME_COMPILE_DEFS} NDEBUG=1 NDEBUG_DEFINED=1 DEBUG_DEFINED=0 $<$<CONFIG:Debug>:PARTIAL_DEBUG=1>)

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

if(ANDROID)
    add_library(dusk SHARED src/dusk/main.cpp)
    set_target_properties(dusk PROPERTIES OUTPUT_NAME main)
else ()
    add_executable(dusk src/dusk/main.cpp)
endif ()

target_compile_definitions(dusk PRIVATE TARGET_PC AVOID_UB=1 VERSION=0)
target_include_directories(dusk PRIVATE include)
target_link_libraries(dusk PRIVATE game aurora::main)
if (TARGET crashpad_handler)
    add_dependencies(dusk crashpad_handler)
endif ()

if (ANDROID)
    # SDLActivity loads SDL_main via dlsym on Android. Since aurora::main is a static
    # archive, force an undefined reference so the linker keeps the SDL_main object.
    target_link_options(dusk PRIVATE "-Wl,-u,SDL_main")
endif ()

add_custom_command(TARGET dusk POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:dusk>/res"
    COMMENT "Copying resources"
)

if (WIN32)
    set(DUSK_WINDOWS_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platforms/windows)
    set(DUSK_WINDOWS_ICON_PNG ${CMAKE_CURRENT_SOURCE_DIR}/res/icon.png)
    set(DUSK_WINDOWS_ICON_ICO ${CMAKE_CURRENT_BINARY_DIR}/dusk.ico)
    set(DUSK_WINDOWS_RC ${CMAKE_CURRENT_BINARY_DIR}/dusk.rc)
    set(DUSK_WINDOWS_MANIFEST ${CMAKE_CURRENT_BINARY_DIR}/dusk.manifest)

    add_custom_command(
            OUTPUT ${DUSK_WINDOWS_ICON_ICO}
            COMMAND powershell -ExecutionPolicy Bypass -File
            ${DUSK_WINDOWS_RESOURCE_DIR}/Create-IcoFromPng.ps1
            -InputPng ${DUSK_WINDOWS_ICON_PNG}
            -OutputIco ${DUSK_WINDOWS_ICON_ICO}
            DEPENDS ${DUSK_WINDOWS_ICON_PNG} ${DUSK_WINDOWS_RESOURCE_DIR}/Create-IcoFromPng.ps1
            VERBATIM
            COMMENT "Generating Windows icon"
    )

    configure_file(${DUSK_WINDOWS_RESOURCE_DIR}/dusk.manifest.in ${DUSK_WINDOWS_MANIFEST} @ONLY)
    configure_file(${DUSK_WINDOWS_RESOURCE_DIR}/dusk.rc.in ${DUSK_WINDOWS_RC} @ONLY)

    target_sources(dusk PRIVATE ${DUSK_WINDOWS_ICON_ICO} ${DUSK_WINDOWS_RC})
    set_target_properties(dusk PROPERTIES WIN32_EXECUTABLE TRUE)

    if (MSVC)
        target_link_options(dusk PRIVATE /MANIFEST:NO)
    endif ()
endif ()

if (APPLE)
    if (IOS)
        set(DUSK_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platforms/ios)
        set(DUSK_INFO_PLIST ${DUSK_RESOURCE_DIR}/Info.plist.in)
        file(GLOB_RECURSE DUSK_RESOURCE_FILES "${DUSK_RESOURCE_DIR}/Base.lproj/*")
    endif ()
    if (IOS OR TVOS)
        target_sources(dusk PRIVATE ${DUSK_RESOURCE_FILES})
        foreach (FILE ${DUSK_RESOURCE_FILES})
            file(RELATIVE_PATH NEW_FILE "${DUSK_RESOURCE_DIR}" ${FILE})
            get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
            set_property(SOURCE ${FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/${NEW_FILE_PATH}")
        endforeach ()
        set_target_properties(
            dusk PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_BUNDLE_NAME ${DUSK_BUNDLE_NAME}
            MACOSX_BUNDLE_GUI_IDENTIFIER ${DUSK_BUNDLE_IDENTIFIER}
            MACOSX_BUNDLE_BUNDLE_VERSION ${DUSK_VERSION_STRING}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${DUSK_SHORT_VERSION_STRING}
            OUTPUT_NAME dusk
            XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES"
            XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "YES"
        )
        if (CMAKE_GENERATOR STREQUAL "Xcode")
            set_target_properties(dusk PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${DUSK_INFO_PLIST})
        elseif (DEFINED DUSK_INFO_PLIST)
            set(MACOSX_BUNDLE_EXECUTABLE_NAME dusk)
            set(MACOSX_BUNDLE_GUI_IDENTIFIER ${DUSK_BUNDLE_IDENTIFIER})
            set(MACOSX_BUNDLE_BUNDLE_NAME ${DUSK_BUNDLE_NAME})
            set(MACOSX_BUNDLE_BUNDLE_VERSION ${DUSK_VERSION_STRING})
            set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${DUSK_SHORT_VERSION_STRING})
            set(DUSK_GENERATED_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/dusk.Info.plist)
            configure_file(${DUSK_INFO_PLIST} ${DUSK_GENERATED_INFO_PLIST})
            add_custom_command(
                TARGET dusk POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DUSK_GENERATED_INFO_PLIST} $<TARGET_FILE_DIR:dusk>/Info.plist
                VERBATIM
            )
        endif ()
    endif ()
endif ()

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

# Packaging logic
function(get_target_output_name target result_var)
    get_target_property(output_name ${target} OUTPUT_NAME)
    if (NOT output_name)
        set(${result_var} "${target}" PARENT_SCOPE)
    else ()
        set(${result_var} "${output_name}" PARENT_SCOPE)
    endif ()
endfunction()
function(get_target_prefix target result_var)
    set(${result_var} "" PARENT_SCOPE)
    if (APPLE)
        # Have to recreate some bundle logic here, since CMake can't tell us
        get_target_property(is_bundle ${target} MACOSX_BUNDLE)
        if (is_bundle)
            get_target_output_name(${target} output_name)
            if (CMAKE_SYSTEM_NAME STREQUAL Darwin)
                set(${result_var} "${output_name}.app/Contents/MacOS/" PARENT_SCOPE)
            else ()
                set(${result_var} "${output_name}.app/" PARENT_SCOPE)
            endif ()
        endif ()
    endif ()
endfunction()
list(APPEND BINARY_TARGETS dusk)
set(EXTRA_TARGETS "")
if (TARGET crashpad_handler)
    list(APPEND EXTRA_TARGETS crashpad_handler)
endif ()
install(TARGETS ${BINARY_TARGETS} ${EXTRA_TARGETS} DESTINATION ${CMAKE_INSTALL_PREFIX})
aurora_install_runtime_dlls(dusk ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_INSTALL_PREFIX})
if (CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
    set(DEBUG_FILES_LIST "")
    foreach (target IN LISTS BINARY_TARGETS EXTRA_TARGETS)
        get_target_output_name(${target} output_name)
        if (WIN32)
            install(FILES $<TARGET_PDB_FILE:${target}> DESTINATION ${CMAKE_INSTALL_PREFIX} OPTIONAL)
        elseif (APPLE)
            get_target_prefix(${target} target_prefix)
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND rm -fr \"$<TARGET_FILE_NAME:${target}>.dSYM\")")
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND dsymutil \"${target_prefix}$<TARGET_FILE_NAME:${target}>\")")
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND strip -S \"${target_prefix}$<TARGET_FILE_NAME:${target}>\")")
            if (NOT target_prefix STREQUAL "")
                install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND mv \"${target_prefix}$<TARGET_FILE_NAME:${target}>.dSYM\" .)")
            endif ()
        elseif (UNIX)
            get_target_prefix(${target} target_prefix)
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND objcopy --only-keep-debug \"${target_prefix}$<TARGET_FILE_NAME:${target}>\" \"${target_prefix}$<TARGET_FILE_NAME:${target}>.dbg\")")
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND objcopy --strip-debug --add-gnu-debuglink=$<TARGET_FILE_NAME:${target}>.dbg \"${target_prefix}$<TARGET_FILE_NAME:${target}>\")")
        endif ()
        list(APPEND DEBUG_FILES_LIST "${output_name}")
    endforeach ()
    if (WIN32)
        list(TRANSFORM DEBUG_FILES_LIST APPEND ".pdb")
        list(JOIN DEBUG_FILES_LIST " " DEBUG_FILES)
        install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND 7z a -t7z \"${CMAKE_INSTALL_PREFIX}/debug.7z\" ${DEBUG_FILES})")
    elseif (APPLE)
        list(TRANSFORM DEBUG_FILES_LIST APPEND ".dSYM")
        list(JOIN DEBUG_FILES_LIST " " DEBUG_FILES)
        install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND tar acfv \"${CMAKE_INSTALL_PREFIX}/debug.tar.xz\" ${DEBUG_FILES})")
    elseif (UNIX)
        list(TRANSFORM DEBUG_FILES_LIST APPEND ".dbg")
        list(JOIN DEBUG_FILES_LIST " " DEBUG_FILES)
        install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND tar -I \"xz -9 -T0\" -cvf \"${CMAKE_INSTALL_PREFIX}/debug.tar.xz\" ${DEBUG_FILES})")
    endif ()
endif ()
foreach (target IN LISTS BINARY_TARGETS)
    get_target_prefix(${target} target_prefix)
    foreach (extra_target IN LISTS EXTRA_TARGETS)
        get_target_prefix(${extra_target} extra_prefix)
        if (NOT "${target_prefix}" STREQUAL "${extra_prefix}")
            # Copy extra target to target prefix
            install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\" COMMAND cp \"${extra_prefix}$<TARGET_FILE_NAME:${extra_target}>\" \"${target_prefix}$<TARGET_FILE_NAME:${extra_target}>\")")
        endif ()
    endforeach ()
endforeach ()
