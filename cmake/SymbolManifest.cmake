include_guard(GLOBAL)

get_filename_component(_SYMBOL_MANIFEST_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)

set(_SYMGEN_VERSION "1.3.1")
set(_SYMGEN_RELEASE_BASE_URL "https://github.com/encounter/symgen/releases/download/v${_SYMGEN_VERSION}")
set(SYMGEN_PATH "" CACHE FILEPATH "Path to a symgen executable; empty downloads the pinned release")
mark_as_advanced(SYMGEN_PATH)

function(symgen_host_asset out_name)
    string(TOLOWER "${CMAKE_HOST_SYSTEM_PROCESSOR}" _host_processor)
    set(_asset "")

    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        if (_host_processor MATCHES "^(arm64|aarch64)$")
            set(_asset "symgen-macos-arm64")
        elseif (_host_processor MATCHES "^(x86_64|amd64)$")
            set(_asset "symgen-macos-x86_64")
        endif ()
    elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
        if (_host_processor MATCHES "^(aarch64|arm64)$")
            set(_asset "symgen-linux-aarch64")
        elseif (_host_processor MATCHES "^(x86_64|amd64)$")
            set(_asset "symgen-linux-x86_64")
        elseif (_host_processor MATCHES "^(i[3-6]86|x86)$")
            set(_asset "symgen-linux-i686")
        endif ()
    elseif (CMAKE_HOST_WIN32)
        if (_host_processor MATCHES "^(arm64|aarch64)$")
            set(_asset "symgen-windows-arm64.exe")
        elseif (_host_processor MATCHES "^(x86_64|amd64)$")
            set(_asset "symgen-windows-x86_64.exe")
        elseif (_host_processor MATCHES "^(i[3-6]86|x86)$")
            set(_asset "symgen-windows-x86.exe")
        endif ()
    endif ()

    set(${out_name} "${_asset}" PARENT_SCOPE)
endfunction()

function(ensure_symgen required)
    if (TARGET symgen)
        return()
    endif ()

    if (SYMGEN_PATH)
        get_filename_component(_symgen "${SYMGEN_PATH}" ABSOLUTE)
        if (NOT EXISTS "${_symgen}")
            if (required)
                message(FATAL_ERROR "symgen: SYMGEN_PATH does not exist: ${_symgen}")
            endif ()
            message(STATUS "symgen: SYMGEN_PATH does not exist, symbol manifest generation "
                    "skipped (by-name hook resolution will be unavailable)")
            return()
        endif ()
    else ()
        symgen_host_asset(_asset)
        if (_asset STREQUAL "")
            if (required)
                message(FATAL_ERROR "symgen: no prebuilt binary for host "
                        "${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR} "
                        "(configure with -DDUSK_ENABLE_CODE_MODS=OFF)")
            endif ()
            message(STATUS "symgen: no prebuilt binary for host "
                    "${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR}; "
                    "symbol manifest generation skipped (by-name hook resolution will be unavailable)")
            return()
        endif ()

        set(_symgen_dir "${CMAKE_BINARY_DIR}/_deps/symgen")
        set(_symgen "${_symgen_dir}/${_asset}")
        set(_url "${_SYMGEN_RELEASE_BASE_URL}/${_asset}")
        message(STATUS "dusklight: Fetching symgen ${_SYMGEN_VERSION} (${_asset})")
        file(MAKE_DIRECTORY "${_symgen_dir}")
        file(DOWNLOAD "${_url}" "${_symgen}"
                TLS_VERIFY ON
                STATUS _download_status
                SHOW_PROGRESS)
        list(GET _download_status 0 _download_code)
        if (NOT _download_code EQUAL 0)
            list(GET _download_status 1 _download_message)
            file(REMOVE "${_symgen}")
            if (required)
                message(FATAL_ERROR "symgen: failed to download ${_url}: ${_download_message}")
            endif ()
            message(STATUS "symgen: failed to download ${_url}: ${_download_message}; "
                    "symbol manifest generation skipped (by-name hook resolution will be unavailable)")
            return()
        endif ()
        if (NOT CMAKE_HOST_WIN32)
            file(CHMOD "${_symgen}" PERMISSIONS
                    OWNER_READ OWNER_WRITE OWNER_EXECUTE
                    GROUP_READ GROUP_EXECUTE
                    WORLD_READ WORLD_EXECUTE)
        endif ()
    endif ()

    add_custom_target(symgen DEPENDS "${_symgen}")
    set(SYMGEN_EXE "${_symgen}" CACHE INTERNAL "symgen executable" FORCE)
endfunction()

function(setup_symbol_manifest target)
    ensure_symgen(TRUE)
    if (NOT TARGET symgen)
        return()
    endif ()
    add_dependencies(${target} symgen)

    if (WIN32)
        set(_input --pdb "$<TARGET_PDB_FILE:${target}>")
    else ()
        set(_input --binary "$<TARGET_FILE:${target}>")
    endif ()

    if (APPLE)
        # Room for the symbol manifest and several prepatch arenas.
        target_link_options(${target} PRIVATE "LINKER:-headerpad,0x1000")
        # ld64 may update an existing output, which breaks our symdb insertion. Remove it first.
        add_custom_command(TARGET ${target} PRE_LINK
                COMMAND "${CMAKE_COMMAND}" -E rm -f "$<TARGET_FILE:${target}>"
                VERBATIM)
    endif ()

    # The manifest is embedded into the image as a new section, located at runtime through
    # the descriptor manifest.cpp reserves. On Apple platforms this command must stay
    # attached before the ad-hoc codesign POST_BUILD command: the patch removes any existing
    # signature.
    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${SYMGEN_EXE}" manifest ${_input} --embed "$<TARGET_FILE:${target}>"
            COMMENT "Embedding symbol manifest"
            VERBATIM)
endfunction()
