cmake_minimum_required(VERSION 3.25)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
            "Build type options: Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif ()

include(cmake/DetectVersion.cmake)
detect_version()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
project(dusklight LANGUAGES C CXX VERSION ${DUSK_VERSION_STRING})
if (APPLE)
    enable_language(OBJC OBJCXX)
endif ()
# Adjust CMAKE_SYSTEM_PROCESSOR on Windows to match compiler target
include(cmake/WindowsTargetProcessor.cmake)
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

# Folder-based instead of target-based organization
# in Visual Studio and Xcode generators
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_cmake")

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
if (ENABLE_ASAN)
    if (CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC" AND
            CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        add_compile_options($<$<COMPILE_LANGUAGE:C,CXX>:/fsanitize=address>)
        add_link_options(/fsanitize=address /INCREMENTAL:NO)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "ProgramDatabase")
        foreach (_lang C CXX)
            foreach (_rtc_flag /RTC1 /RTCc /RTCs /RTCu)
                string(REPLACE "${_rtc_flag}" "" CMAKE_${_lang}_FLAGS_DEBUG "${CMAKE_${_lang}_FLAGS_DEBUG}")
            endforeach ()
        endforeach ()
    elseif (CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "GNU" AND
            CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        add_compile_options(
                $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-fsanitize=address>
                $<$<COMPILE_LANGUAGE:C,CXX,OBJC,OBJCXX>:-fno-omit-frame-pointer>
        )
        add_link_options(-fsanitize=address)
    else ()
        message(FATAL_ERROR "ENABLE_ASAN requires GNU-like or MSVC-like C/C++ compiler frontends")
    endif ()

    add_compile_definitions(NDEBUG_SANITIZER) # Avoids absl issue with SwissTable debug code
    message(STATUS "dusklight: Enabled AddressSanitizer")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(DAWN_USE_WAYLAND ON CACHE BOOL "Enable support for Wayland surface" FORCE)
endif ()
set(AURORA_ENABLE_DVD ON CACHE BOOL "Enable DVD API support" FORCE)
set(AURORA_ENABLE_CARD ON CACHE BOOL "Enable CARD API support" FORCE)
set(AURORA_ENABLE_RMLUI ON CACHE BOOL "Enable RmlUi UI support" FORCE)
add_subdirectory(extern/aurora EXCLUDE_FROM_ALL)
target_compile_definitions(aurora_mtx PRIVATE MTX_USE_PS=1)

add_subdirectory(libs/freeverb)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DUSK_GFX_DEBUG_GROUPS_DEFAULT ON)
else ()
    set(DUSK_GFX_DEBUG_GROUPS_DEFAULT OFF)
endif ()

option(DUSK_BUILD_WARNINGS "Enable compiler warnings (off by default)")
option(DUSK_SELECTED_OPT "If on, selected parts of the project will be compiled with optimizations on Debug, intending to make the game run at 30 FPS. Note for MSVC: you will need to remove '/RTC1' from your debug flags in CMake.")
option(DUSK_MOVIE_SUPPORT "If on, compile against libjpeg-turbo to enable THP file decoding" ON)
option(DUSK_ENABLE_UPDATE_CHECKER "Enable update checking support" ON)
option(DUSK_ENABLE_SENTRY_NATIVE "Enable sentry-native crash reporting support" OFF)
option(DUSK_PACKAGE_INSTALL "Install Dusklight with a Linux-native file structure" OFF)
option(DUSK_GFX_DEBUG_GROUPS "Report debug groups to the native graphics API" ${DUSK_GFX_DEBUG_GROUPS_DEFAULT})
option(DUSK_ENABLE_CODE_MODS "Enable code mods" OFF)

# Edit & Continue
if (MSVC)
    if ("${CMAKE_MSVC_DEBUG_INFORMATION_FORMAT}" STREQUAL "" AND CMAKE_BUILD_TYPE STREQUAL "Debug"
            AND CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "EditAndContinue")
    endif ()
    if (CMAKE_MSVC_DEBUG_INFORMATION_FORMAT STREQUAL "EditAndContinue")
        add_link_options("/INCREMENTAL")
    endif ()
endif ()

if (DUSK_MOVIE_SUPPORT)
    find_package(libjpeg-turbo 3.0 CONFIG QUIET)
    if (libjpeg-turbo_FOUND)
        message(STATUS "dusklight: Using system libjpeg-turbo")
    else ()
        message(STATUS "dusklight: Fetching libjpeg-turbo")
        include(ExternalProject)
        set(_jpeg_install_dir ${CMAKE_BINARY_DIR}/libjpeg-turbo-install)
        if (WIN32)
            set(_jpeg_lib ${_jpeg_install_dir}/lib/turbojpeg-static.lib)
        else ()
            set(_jpeg_lib ${_jpeg_install_dir}/lib/libturbojpeg.a)
        endif ()
        set(_jpeg_cmake_args
            -DCMAKE_INSTALL_PREFIX=${_jpeg_install_dir}
            -DCMAKE_PROJECT_INCLUDE=${CMAKE_CURRENT_SOURCE_DIR}/cmake/WindowsTargetProcessor.cmake
            -DENABLE_SHARED=OFF
            -DWITH_TURBOJPEG=ON
            -DWITH_JAVA=OFF
        )
        if (CMAKE_TOOLCHAIN_FILE)
            get_filename_component(_jpeg_toolchain_file "${CMAKE_TOOLCHAIN_FILE}" ABSOLUTE BASE_DIR "${CMAKE_SOURCE_DIR}")
            list(APPEND _jpeg_cmake_args -DCMAKE_TOOLCHAIN_FILE=${_jpeg_toolchain_file})
        endif ()
        set(_jpeg_passthrough_vars
            ANDROID_ABI
            ANDROID_PLATFORM
            CMAKE_BUILD_TYPE
            CMAKE_C_COMPILER
            CMAKE_C_COMPILER_LAUNCHER
            CMAKE_MAKE_PROGRAM
            CMAKE_MSVC_RUNTIME_LIBRARY
            CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
            CMAKE_OSX_ARCHITECTURES
            CMAKE_OSX_DEPLOYMENT_TARGET
            CMAKE_OSX_SYSROOT
            DEPLOYMENT_TARGET
            ENABLE_ARC
            ENABLE_BITCODE
            PLATFORM
        )
        foreach(_var IN LISTS _jpeg_passthrough_vars)
            if (DEFINED ${_var})
                list(APPEND _jpeg_cmake_args -D${_var}=${${_var}})
            endif ()
        endforeach ()
        ExternalProject_Add(libjpeg-turbo-ext
            URL https://github.com/libjpeg-turbo/libjpeg-turbo/archive/refs/tags/3.1.0.tar.gz
            URL_HASH SHA256=35fec2e1ddfb05ecf6d93e50bc57c1e54bc81c16d611ddf6eff73fff266d8285
            CMAKE_ARGS ${_jpeg_cmake_args}
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
    # -Wdeprecated-declarations: JSystem uses std::iterator, deprecated in C++17
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-multichar")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar -Wno-trigraphs -Wno-deprecated-declarations")
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    set(CMAKE_BUILD_RPATH "$ORIGIN")
elseif (APPLE)
    add_compile_options(-Wno-declaration-after-statement -Wno-non-pod-varargs)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    set(CMAKE_BUILD_RPATH "$ORIGIN")
elseif (MSVC)
    add_compile_options(
        $<$<COMPILE_LANGUAGE:C,CXX>:/bigobj>
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
message(STATUS "dusklight: Fetching cxxopts")
FetchContent_Declare(cxxopts
    URL https://github.com/jarro2783/cxxopts/archive/refs/tags/v3.3.1.tar.gz
    URL_HASH SHA256=3bfc70542c521d4b55a46429d808178916a579b28d048bd8c727ee76c39e2072
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
)
message(STATUS "dusklight: Fetching nlohmann/json")
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
)
message(STATUS "dusklight: Fetching miniz")
FetchContent_Declare(miniz
    URL https://github.com/richgel999/miniz/releases/download/3.0.2/miniz-3.0.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    EXCLUDE_FROM_ALL
)

set(_fetch_content_deps cxxopts json miniz)
if (DUSK_ENABLE_CODE_MODS)
    message(STATUS "dusklight: Fetching funchook")
    # cmake/PatchFunchook.cmake patches funchook's cmake/capstone.cmake.in to inject a
    # PATCH_COMMAND into capstone's inner ExternalProject. That PATCH_COMMAND runs
    # cmake/PatchCapstone.cmake after capstone is cloned, which removes the
    # cmake_policy(SET CMP0048 OLD) line that CMake >= 3.27 rejects.
    # This is incredibly scuffed and we should probably think of a better way to do this
    set(CAPSTONE_FIX_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/cmake/PatchCapstone.cmake")
    FetchContent_Declare(funchook
        GIT_REPOSITORY https://github.com/kubo/funchook.git
        GIT_TAG v1.1.3
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        PATCH_COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=<SOURCE_DIR> -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/PatchFunchook.cmake
        EXCLUDE_FROM_ALL
    )
    set(FUNCHOOK_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(FUNCHOOK_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    set(FUNCHOOK_INSTALL OFF CACHE BOOL "" FORCE)
    if (APPLE AND CMAKE_OSX_ARCHITECTURES)
        list(LENGTH CMAKE_OSX_ARCHITECTURES _osx_arch_count)
        if (_osx_arch_count EQUAL 1)
            list(GET CMAKE_OSX_ARCHITECTURES 0 _osx_arch)
            if (_osx_arch MATCHES "^(arm64|aarch64|ARM64)$")
                set(FUNCHOOK_CPU arm64 CACHE STRING "" FORCE)
            elseif (_osx_arch MATCHES "^(x86_64|AMD64|amd64|i[3-6]86|x86)$")
                set(FUNCHOOK_CPU x86 CACHE STRING "" FORCE)
            endif ()
        endif ()
    endif ()
    list(APPEND _fetch_content_deps funchook)
endif ()
FetchContent_MakeAvailable(${_fetch_content_deps})

if (DUSK_ENABLE_SENTRY_NATIVE)
    message(STATUS "dusklight: Fetching sentry-native")
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

# Use signed char on ARM to match the original game (and x86)
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" _arch)
if(_arch MATCHES "^(arm|aarch64)" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    add_compile_options(-fsigned-char)
endif()

configure_version_header()

include(files.cmake)

# TODO: version handling for res includes

set(DUSK_BUNDLE_NAME Dusklight)
set(DUSK_BUNDLE_IDENTIFIER dev.twilitrealm.dusk)
set(DUSK_COMPANY_NAME "Twilit Realm")
set(DUSK_FILE_DESCRIPTION "Dusklight")
set(DUSK_PRODUCT_NAME "Dusklight")
set(DUSK_COPYRIGHT "Copyright (C) Twilit Realm contributors")

source_group("dolzel" FILES ${DOLZEL_FILES} ${Z2AUDIOLIB_FILES} ${REL_FILES})
source_group("dusklight" FILES ${DUSK_FILES} ${DUSK_HTTP_BACKEND_FILES})

include(cmake/GameABIConfig.cmake)

find_package(Threads REQUIRED)
set(GAME_COMPILE_DEFS DUSK_BUILDING_GAME=1)
set(GAME_LIBS aurora::core aurora::gx aurora::gd aurora::si aurora::vi aurora::pad aurora::mtx aurora::os aurora::dvd
    aurora::card freeverb cxxopts::cxxopts absl::flat_hash_map nlohmann_json::nlohmann_json TracyClient fmt::fmt
    Threads::Threads zstd::libzstd dusklight_game_headers)
if (DUSK_ENABLE_CODE_MODS)
    list(APPEND GAME_LIBS funchook-static)
endif ()

if (DUSK_ENABLE_SENTRY_NATIVE)
    list(APPEND GAME_LIBS sentry)
    list(APPEND GAME_COMPILE_DEFS DUSK_ENABLE_SENTRY_NATIVE=1 SENTRY_BUILD_STATIC=1)
endif ()

if (WIN32)
    list(APPEND GAME_LIBS Ws2_32)
    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        list(APPEND GAME_LIBS dbghelp)
        list(APPEND GAME_COMPILE_DEFS DUSK_CRASH_DBGHELP=1)
    endif ()
endif ()

set(DUSK_HTTP_BACKEND_SOURCE src/dusk/http/no_backend.cpp)
if (DUSK_ENABLE_UPDATE_CHECKER)
    list(APPEND GAME_COMPILE_DEFS DUSK_ENABLE_UPDATE_CHECKER=1)
    if (WIN32)
        set(DUSK_HTTP_BACKEND_SOURCE src/dusk/http/winhttp.cpp)
        list(APPEND GAME_LIBS winhttp)
        list(APPEND GAME_COMPILE_DEFS DUSK_HTTP_BACKEND_WINHTTP=1)
        message(STATUS "dusklight: Enabled update checker (WinHTTP)")
    elseif (ANDROID)
        set(DUSK_HTTP_BACKEND_SOURCE src/dusk/http/android.cpp)
        list(APPEND GAME_COMPILE_DEFS DUSK_HTTP_BACKEND_ANDROID=1)
        message(STATUS "dusklight: Enabled update checker (Android)")
    elseif (APPLE)
        find_library(FOUNDATION_FRAMEWORK Foundation REQUIRED)
        set(DUSK_HTTP_BACKEND_SOURCE src/dusk/http/url_session.mm)
        set_source_files_properties(src/dusk/http/url_session.mm PROPERTIES COMPILE_FLAGS -fobjc-arc)
        list(APPEND GAME_LIBS ${FOUNDATION_FRAMEWORK})
        list(APPEND GAME_COMPILE_DEFS DUSK_HTTP_BACKEND_URLSESSION=1)
        message(STATUS "dusklight: Enabled update checker (NSURLSession)")
    elseif (CMAKE_SYSTEM_NAME STREQUAL Linux)
        find_package(CURL QUIET OPTIONAL_COMPONENTS HTTPS SSL)
        if (CURL_FOUND AND CURL_HTTPS_FOUND AND CURL_SSL_FOUND)
            set(DUSK_HTTP_BACKEND_SOURCE src/dusk/http/curl.cpp)
            list(APPEND GAME_LIBS CURL::libcurl)
            list(APPEND GAME_COMPILE_DEFS DUSK_HTTP_BACKEND_LIBCURL=1)
            message(STATUS "dusklight: Enabled update checker (libcurl)")
        else ()
            message(STATUS "dusklight: Disabled update checker (libcurl + HTTPS/SSL not found)")
        endif ()
    else ()
        message(STATUS "dusklight: Disabled update checker (unsupported platform)")
    endif ()
endif ()
list(APPEND DUSK_FILES ${DUSK_HTTP_BACKEND_SOURCE})

if (DUSK_MOVIE_SUPPORT)
    if (TARGET libjpeg-turbo::turbojpeg-static)
        list(APPEND GAME_LIBS libjpeg-turbo::turbojpeg-static)
    else ()
        list(APPEND GAME_LIBS libjpeg-turbo::turbojpeg)
    endif ()
    list(APPEND GAME_COMPILE_DEFS MOVIE_SUPPORT=1)
endif ()

set(DUSK_ENABLE_DISCORD_DEFAULT ON)
if (DEFINED DUSK_ENABLE_DISCORD_RPC AND NOT DEFINED DUSK_ENABLE_DISCORD)
    set(DUSK_ENABLE_DISCORD_DEFAULT ${DUSK_ENABLE_DISCORD_RPC})
endif ()
option(DUSK_ENABLE_DISCORD "Enable Discord Rich Presence support" ${DUSK_ENABLE_DISCORD_DEFAULT})
if (DUSK_ENABLE_DISCORD AND NOT ANDROID AND NOT IOS AND NOT TVOS)
    list(APPEND GAME_COMPILE_DEFS DUSK_DISCORD=1)
endif ()

if (DUSK_ENABLE_CODE_MODS)
    list(APPEND GAME_COMPILE_DEFS DUSK_CODE_MODS=1)
endif ()

if (DUSK_PACKAGE_INSTALL)
    include(GNUInstallDirs)
    list(APPEND GAME_COMPILE_DEFS DUSK_ASSET_DIR="${CMAKE_INSTALL_FULL_DATADIR}/dusklight/")
endif ()

if (DUSK_GFX_DEBUG_GROUPS)
    list(APPEND GAME_COMPILE_DEFS DUSK_GFX_DEBUG_GROUPS=1)
    target_compile_definitions(aurora_gx PRIVATE AURORA_GFX_DEBUG_GROUPS)
endif ()

# game_debug is for game code files that we know work when compiled with DEBUG=1
# Of course, if building a release build, this distinction is irrelevant
set(GAME_DEBUG_FILES
        ${SSYSTEM_FILES}
        src/dusk/audio/DuskAudioSystem.cpp
        src/dusk/audio/JASCriticalSection.cpp
        src/dusk/audio/DuskDsp.cpp
        src/dusk/audio/Adpcm.cpp
        src/dusk/audio/DspStub.cpp
        src/dusk/imgui/ImGuiAudio.cpp
)
set_source_files_properties(
        ${GAME_DEBUG_FILES}
        PROPERTIES
        COMPILE_DEFINITIONS "$<$<CONFIG:Debug>:DEBUG=1>"
)

# game_base is for all other game code files
set(GAME_BASE_FILES
        ${DOLZEL_FILES}
        ${Z2AUDIOLIB_FILES}
        ${REL_FILES}
        ${DUSK_FILES}
        ${DOLPHIN_FILES}
)
set_source_files_properties(
        ${GAME_BASE_FILES}
        PROPERTIES
        COMPILE_DEFINITIONS "NDEBUG=1;NDEBUG_DEFINED=1;DEBUG_DEFINED=0"
)

foreach(jsystem_lib IN LISTS JSYSTEM_LIBRARIES)
    target_compile_definitions(${jsystem_lib} PRIVATE ${GAME_COMPILE_DEFS} $<$<CONFIG:Debug>:DEBUG=1>)
    target_link_libraries(${jsystem_lib} PRIVATE ${GAME_LIBS})
    set_target_properties(${jsystem_lib} PROPERTIES FOLDER "JSystem")
endforeach()

set(JSYSTEM_LINK_LIBRARIES ${JSYSTEM_LIBRARIES})
if (CMAKE_CXX_LINK_GROUP_USING_RESCAN_SUPPORTED OR CMAKE_LINK_GROUP_USING_RESCAN_SUPPORTED)
    # GNU ld resolves static archives in a single left-to-right pass. The split
    # JSystem libraries reference each other, so they need a RESCAN group there.
    set(JSYSTEM_LINK_LIBRARIES "$<LINK_GROUP:RESCAN,${JSYSTEM_LIBRARIES}>")
endif ()

set(DUSK_FILES src/dusk/main.cpp ${GAME_BASE_FILES} ${GAME_DEBUG_FILES} ${miniz_SOURCE_DIR}/miniz.c)
if(ANDROID)
    add_library(dusklight SHARED ${DUSK_FILES})
    set_target_properties(dusklight PROPERTIES OUTPUT_NAME main)
else ()
    add_executable(dusklight ${DUSK_FILES})
endif ()
if (ENABLE_ASAN)
    target_sources(dusklight PRIVATE src/dusk/asan_options.c)
endif ()

target_compile_definitions(dusklight PRIVATE ${GAME_COMPILE_DEFS})
target_include_directories(dusklight PRIVATE ${miniz_SOURCE_DIR})
target_link_libraries(dusklight PRIVATE aurora::main ${GAME_LIBS} ${JSYSTEM_LINK_LIBRARIES})
target_precompile_headers(dusklight PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_SOURCE_DIR}/include/dusk_pch.hpp>")

if (DUSK_ENABLE_CODE_MODS)
    include(cmake/SymbolManifest.cmake)
    if (WIN32)
        # Game ABI exports & import library for mod linking
        include(cmake/WindowsExports.cmake)
        setup_windows_exports(dusklight)
    endif ()
    # Post-link symbol manifest: hookable-surface name->address map keyed to the build.
    setup_symbol_manifest(dusklight)
endif ()

# Hook reliability: guaranteed patchable entries on the game ABI surface, and no identical-code folding.
if (MSVC)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(DUSK_PATCHABLE_ENTRY_FLAG $<$<COMPILE_LANGUAGE:C,CXX>:/hotpatch>)
    endif ()
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM64")
        target_link_options(dusklight PRIVATE /FUNCTIONPADMIN:16 /OPT:NOICF)
    else ()
        target_link_options(dusklight PRIVATE /FUNCTIONPADMIN /OPT:NOICF)
    endif ()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64" OR CMAKE_OSX_ARCHITECTURES MATCHES "arm64")
        set(DUSK_PATCHABLE_ENTRY_FLAG $<$<COMPILE_LANGUAGE:C,CXX>:-fpatchable-function-entry=2,1>)
    else ()
        set(DUSK_PATCHABLE_ENTRY_FLAG $<$<COMPILE_LANGUAGE:C,CXX>:-fpatchable-function-entry=10,5>)
    endif ()
endif ()
if (DEFINED DUSK_PATCHABLE_ENTRY_FLAG)
    target_compile_options(dusklight PRIVATE ${DUSK_PATCHABLE_ENTRY_FLAG})
    foreach(jsystem_lib IN LISTS JSYSTEM_LIBRARIES)
        target_compile_options(${jsystem_lib} PRIVATE ${DUSK_PATCHABLE_ENTRY_FLAG})
    endforeach()
endif ()

if (WIN32)
    target_link_libraries(dusklight PRIVATE Psapi)
endif ()
if (APPLE)
    # Mods resolve game symbols from the executable at dlopen time.
    target_link_options(dusklight PRIVATE -Wl,-export_dynamic)
elseif (UNIX AND NOT ANDROID)
    target_link_options(dusklight PRIVATE -rdynamic)
endif ()

if (TARGET crashpad_handler)
    add_dependencies(dusklight crashpad_handler)
    add_custom_command(TARGET dusklight POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:crashpad_handler>"
            "$<TARGET_FILE_DIR:dusklight>"
        COMMENT "Copying crashpad handler"
    )
endif ()

if (ANDROID)
    # SDLActivity loads SDL_main via dlsym on Android. Since aurora::main is a static
    # archive, force an undefined reference so the linker keeps the SDL_main object.
    target_link_options(dusklight PRIVATE "-Wl,-u,SDL_main")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    target_link_options(dusklight PRIVATE "-Wl,--build-id=sha1")
    target_link_libraries(dusklight PRIVATE dl)
endif ()

if (NOT APPLE)
    add_custom_command(TARGET dusklight POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/res"
            "$<TARGET_FILE_DIR:dusklight>/res"
        COMMENT "Copying resources"
    )
endif ()

if (WIN32)
    set(DUSK_WINDOWS_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platforms/windows)
    set(DUSK_WINDOWS_ICON_PNG ${CMAKE_CURRENT_SOURCE_DIR}/res/icon.png)
    set(DUSK_WINDOWS_ICON_ICO ${CMAKE_CURRENT_BINARY_DIR}/dusklight.ico)
    set(DUSK_WINDOWS_RC ${CMAKE_CURRENT_BINARY_DIR}/dusklight.rc)
    set(DUSK_WINDOWS_MANIFEST ${CMAKE_CURRENT_BINARY_DIR}/dusklight.manifest)

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

    configure_file(${DUSK_WINDOWS_RESOURCE_DIR}/dusklight.manifest.in ${DUSK_WINDOWS_MANIFEST} @ONLY)
    configure_file(${DUSK_WINDOWS_RESOURCE_DIR}/dusklight.rc.in ${DUSK_WINDOWS_RC} @ONLY)

    target_sources(dusklight PRIVATE ${DUSK_WINDOWS_ICON_ICO} ${DUSK_WINDOWS_RC})
    set_target_properties(dusklight PROPERTIES WIN32_EXECUTABLE TRUE)

    if (MSVC)
        target_link_options(dusklight PRIVATE /MANIFEST:NO)
    endif ()
endif ()

include(cmake/ModSDK.cmake)

if (DUSK_ENABLE_CODE_MODS AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_custom_target(dusklight_mods) # Aggregate target for all in-tree mods
    add_subdirectory(mods/template_mod)
    add_subdirectory(mods/ao_mod)
    add_subdirectory(mods/shadow_mod)
endif ()

if (APPLE)
    if (IOS)
        set(DUSK_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platforms/ios)
    elseif (TVOS)
        set(DUSK_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platforms/tvos)
    else ()
        set(DUSK_RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platforms/macos)
        set(DUSK_ENTITLEMENTS ${DUSK_RESOURCE_DIR}/Dusklight.entitlements)
    endif ()
    set(DUSK_INFO_PLIST ${DUSK_RESOURCE_DIR}/Info.plist.in)
    file(GLOB_RECURSE DUSK_RESOURCE_FILES
        "${DUSK_RESOURCE_DIR}/Assets.car"
        "${DUSK_RESOURCE_DIR}/Base.lproj/*"
        "${DUSK_RESOURCE_DIR}/Dusklight.icns")
    file(GLOB_RECURSE DUSK_APP_RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/res/*")
    target_sources(dusklight PRIVATE ${DUSK_RESOURCE_FILES})
    target_sources(dusklight PRIVATE ${DUSK_APP_RESOURCE_FILES})
    foreach (FILE ${DUSK_RESOURCE_FILES})
        file(RELATIVE_PATH NEW_FILE "${DUSK_RESOURCE_DIR}" ${FILE})
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        set_property(SOURCE ${FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/${NEW_FILE_PATH}")
    endforeach ()
    foreach (FILE ${DUSK_APP_RESOURCE_FILES})
        file(RELATIVE_PATH NEW_FILE "${CMAKE_CURRENT_SOURCE_DIR}" ${FILE})
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        set_property(SOURCE ${FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/${NEW_FILE_PATH}")
    endforeach ()
    set(_apple_bundle_properties
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME ${DUSK_BUNDLE_NAME}
        MACOSX_BUNDLE_GUI_IDENTIFIER ${DUSK_BUNDLE_IDENTIFIER}
        MACOSX_BUNDLE_BUNDLE_VERSION ${DUSK_VERSION_STRING}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${DUSK_SHORT_VERSION_STRING}
        MACOSX_BUNDLE_INFO_PLIST ${DUSK_INFO_PLIST}
        OUTPUT_NAME Dusklight
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES"
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "YES"
    )
    if (DUSK_ENTITLEMENTS)
        list(APPEND _apple_bundle_properties
            XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS ${DUSK_ENTITLEMENTS})
    endif ()
    if (NOT IOS AND NOT TVOS)
        list(APPEND _apple_bundle_properties
            XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME "YES")
    endif ()
    set_target_properties(dusklight PROPERTIES ${_apple_bundle_properties})

    if (NOT IOS AND NOT TVOS AND NOT "${CMAKE_GENERATOR}" STREQUAL "Xcode")
        set(_sign_nested_commands)
        if (TARGET crashpad_handler)
            list(APPEND _sign_nested_commands
                COMMAND /usr/bin/codesign --force --sign -
                    "$<TARGET_FILE_DIR:dusklight>/crashpad_handler")
        endif ()
        add_custom_command(TARGET dusklight POST_BUILD
            ${_sign_nested_commands}
            COMMAND /usr/bin/codesign --force --sign - --entitlements
                "${DUSK_ENTITLEMENTS}" "$<TARGET_BUNDLE_DIR:dusklight>"
            COMMENT "Signing Dusklight.app with entitlements"
            VERBATIM
        )
    endif ()
endif ()

if (APPLE AND NOT IOS AND NOT TVOS)
    find_library(APPKIT_FRAMEWORK AppKit REQUIRED)
    target_sources(dusklight PRIVATE src/dusk/file_select_macos.mm)
    set_source_files_properties(src/dusk/file_select_macos.mm PROPERTIES COMPILE_FLAGS -fobjc-arc)
    target_link_libraries(dusklight PRIVATE ${APPKIT_FRAMEWORK})
endif ()

if (IOS)
    find_library(UIKIT_FRAMEWORK UIKit REQUIRED)
    find_library(UNIFORM_TYPE_IDENTIFIERS_FRAMEWORK UniformTypeIdentifiers REQUIRED)
    target_sources(dusklight PRIVATE src/dusk/ios/FileSelectDialog.m)
    set_source_files_properties(src/dusk/ios/FileSelectDialog.m PROPERTIES COMPILE_FLAGS -fobjc-arc)
    target_link_libraries(dusklight PRIVATE ${UIKIT_FRAMEWORK} ${UNIFORM_TYPE_IDENTIFIERS_FRAMEWORK})
endif ()

include(extern/aurora/cmake/AuroraCopyRuntimeDLLs.cmake)
aurora_copy_runtime_dlls(dusklight)

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
list(APPEND BINARY_TARGETS dusklight)
set(EXTRA_TARGETS "")
if (TARGET crashpad_handler)
    list(APPEND EXTRA_TARGETS crashpad_handler)
endif ()
if (DUSK_PACKAGE_INSTALL)
    install(TARGETS ${BINARY_TARGETS} ${EXTRA_TARGETS} DESTINATION ${CMAKE_INSTALL_BINDIR})
else()
    install(TARGETS ${BINARY_TARGETS} ${EXTRA_TARGETS} DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
aurora_install_runtime_dlls(dusklight ${CMAKE_INSTALL_PREFIX})
if (NOT APPLE)
    if (DUSK_PACKAGE_INSTALL)
        install(DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/dusklight)
    else()
        install(DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_INSTALL_PREFIX})
    endif()
endif ()
if (CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo AND NOT DUSK_PACKAGE_INSTALL)
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
    # This is a terrible hack to only run this on CI
    # until I turn this into a script or something
    if(DEFINED ENV{GITHUB_ENV})
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

install_bundled_mods()
