# Version detection shared by the main build and the mod SDK (sdk/CMakeLists.txt)
include_guard(GLOBAL)

get_filename_component(_DUSK_VERSION_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

set(DUSK_SENTRY_DSN "" CACHE STRING "Sentry DSN")
set(DUSK_SENTRY_ENVIRONMENT "development" CACHE STRING "Sentry environment")

set(DUSK_VERSION_OVERRIDE "" CACHE STRING "Override version string (skips git detection and format validation)")

macro(detect_version)
    if (DUSK_VERSION_OVERRIDE)
        set(DUSK_WC_DESCRIBE "${DUSK_VERSION_OVERRIDE}")
        set(DUSK_VERSION_STRING "0.0.0.0")
        set(DUSK_SHORT_VERSION_STRING "0.0.0")
        set(DUSK_VERSION_CODE "1")
        set(DUSK_WC_REVISION "")
        set(DUSK_WC_BRANCH "")
        set(DUSK_WC_DATE "")
        message(STATUS "Dusklight version overridden to ${DUSK_WC_DESCRIBE}")
    else ()
        # obtain revision info from git
        find_package(Git)
        if (GIT_FOUND)
            # make sure version information gets re-run when the current Git HEAD changes
            execute_process(WORKING_DIRECTORY ${_DUSK_VERSION_ROOT} COMMAND ${GIT_EXECUTABLE} rev-parse --git-path HEAD
                    OUTPUT_VARIABLE dusk_git_head_filename
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
            get_filename_component(dusk_git_head_filename "${dusk_git_head_filename}" ABSOLUTE BASE_DIR "${_DUSK_VERSION_ROOT}")
            set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${dusk_git_head_filename}")

            execute_process(WORKING_DIRECTORY ${_DUSK_VERSION_ROOT} COMMAND ${GIT_EXECUTABLE} rev-parse --symbolic-full-name HEAD
                    OUTPUT_VARIABLE dusk_git_head_symbolic
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
            execute_process(WORKING_DIRECTORY ${_DUSK_VERSION_ROOT}
                    COMMAND ${GIT_EXECUTABLE} rev-parse --git-path ${dusk_git_head_symbolic}
                    OUTPUT_VARIABLE dusk_git_head_symbolic_filename
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
            get_filename_component(dusk_git_head_symbolic_filename "${dusk_git_head_symbolic_filename}" ABSOLUTE BASE_DIR "${_DUSK_VERSION_ROOT}")
            set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${dusk_git_head_symbolic_filename}")

            # defines DUSK_WC_REVISION
            execute_process(WORKING_DIRECTORY ${_DUSK_VERSION_ROOT} COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
                    OUTPUT_VARIABLE DUSK_WC_REVISION
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
            # defines DUSK_WC_DESCRIBE
            execute_process(WORKING_DIRECTORY ${_DUSK_VERSION_ROOT} COMMAND ${GIT_EXECUTABLE} describe --tags --long --dirty --match "v*"
                    OUTPUT_VARIABLE DUSK_WC_DESCRIBE
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

            # remove the git hash, then collapse a clean "-0" suffix only
            string(REGEX REPLACE "-[^-]+(-dirty|)$" "\\1" DUSK_WC_DESCRIBE "${DUSK_WC_DESCRIBE}")
            string(REGEX REPLACE "-0$" "" DUSK_WC_DESCRIBE "${DUSK_WC_DESCRIBE}")

            # defines DUSK_WC_BRANCH
            execute_process(WORKING_DIRECTORY ${_DUSK_VERSION_ROOT} COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
                    OUTPUT_VARIABLE DUSK_WC_BRANCH
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
            # defines DUSK_WC_DATE
            execute_process(WORKING_DIRECTORY ${_DUSK_VERSION_ROOT} COMMAND ${GIT_EXECUTABLE} log -1 --format=%ad
                    OUTPUT_VARIABLE DUSK_WC_DATE
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
        else ()
            message(STATUS "Unable to find git, commit information will not be available")
        endif ()

        if (DUSK_WC_DESCRIBE MATCHES "^v([0-9]+)\\.([0-9]+)\\.([0-9]+)([-+].*)?$")
            set(DUSK_SHORT_VERSION_STRING "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}")
            set(_ver_major ${CMAKE_MATCH_1})
            set(_ver_minor ${CMAKE_MATCH_2})
            set(_ver_patch ${CMAKE_MATCH_3})
            set(DUSK_VERSION_TWEAK "0")
            if (DUSK_WC_DESCRIBE MATCHES "^v[0-9]+\\.[0-9]+\\.[0-9]+-([0-9]+)(-dirty)?$")
                set(DUSK_VERSION_TWEAK "${CMAKE_MATCH_1}")
            elseif (DUSK_WC_DESCRIBE MATCHES "^v[0-9]+\\.[0-9]+\\.[0-9]+-[0-9A-Za-z.-]+-([0-9]+)(-dirty)?$")
                set(DUSK_VERSION_TWEAK "${CMAKE_MATCH_1}")
            endif ()
            set(DUSK_VERSION_STRING "${DUSK_SHORT_VERSION_STRING}.${DUSK_VERSION_TWEAK}")
            if (DUSK_VERSION_TWEAK GREATER 999)
                set(_tweak 999)
            else ()
                set(_tweak ${DUSK_VERSION_TWEAK})
            endif ()
            # encoding: major*1e7 + minor*1e5 + patch*1e3 + tweak; collision-free for major<210, minor<100, patch<100, tweak<=999
            math(EXPR DUSK_VERSION_CODE
                    "${_ver_major} * 10000000 + ${_ver_minor} * 100000 + ${_ver_patch} * 1000 + ${_tweak}")
        else ()
            set(DUSK_WC_DESCRIBE "UNKNOWN-VERSION")
            set(DUSK_VERSION_STRING "0.0.0.0")
            set(DUSK_SHORT_VERSION_STRING "0.0.0")
            set(DUSK_VERSION_CODE "1")
        endif ()

    endif ()

    # Add version information to CI environment variables
    if (DEFINED ENV{GITHUB_ENV})
        file(APPEND "$ENV{GITHUB_ENV}" "DUSK_VERSION=${DUSK_WC_DESCRIBE}\n")
        file(APPEND "$ENV{GITHUB_ENV}" "DUSK_VERSION_CODE=${DUSK_VERSION_CODE}\n")
    endif ()
    message(STATUS "Dusklight version set to ${DUSK_WC_DESCRIBE}")
endmacro()

# Sets PLATFORM_NAME and configures version.h into the caller's binary dir.
macro(configure_version_header)
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

    configure_file(${_DUSK_VERSION_ROOT}/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
endmacro()
