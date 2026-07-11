include_guard(GLOBAL)

get_filename_component(_DUSK_WINDOWS_EXPORTS_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)

# Windows mod linking: generate the curated export surface for the game executable and the
# import library mods link against. symgen scans the built objects, filters by source, and
# writes a .def used by the main link and import library generation.
function(setup_windows_exports target)
    if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(WARNING "dusklight: Windows code-mod exports are x64-only for now; skipping")
        return()
    endif ()

    include("${_DUSK_WINDOWS_EXPORTS_CMAKE_DIR}/SymbolManifest.cmake")
    ensure_symgen(TRUE)
    set(_symgen "${SYMGEN_EXE}")
    add_dependencies(${target} symgen)

    set(_config_subdir "")
    if (CMAKE_CONFIGURATION_TYPES)
        set(_config_subdir "$<CONFIG>/")
    endif ()

    set(_rsp_lines "$<TARGET_OBJECTS:${target}>")
    foreach (_lib IN LISTS JSYSTEM_LIBRARIES)
        list(APPEND _rsp_lines "$<TARGET_FILE:${_lib}>")
    endforeach ()
    list(JOIN _rsp_lines "\n" _rsp_content)
    set(_rsp "${CMAKE_BINARY_DIR}/${_config_subdir}dusklight_exports_input.rsp")
    file(GENERATE OUTPUT "${_rsp}" CONTENT "${_rsp_content}")

    set(_sdk_args)
    foreach (_lib aurora_card aurora_core aurora_dvd aurora_gd aurora_gx aurora_mtx
            aurora_os aurora_pad aurora_si aurora_vi)
        if (TARGET ${_lib})
            list(APPEND _sdk_args --sdk-lib "$<TARGET_FILE:${_lib}>")
        endif ()
    endforeach ()

    # Generate curated exports list from the main binary
    set(_def "${CMAKE_BINARY_DIR}/${_config_subdir}dusklight_exports.def")
    add_custom_command(TARGET ${target} PRE_LINK
            # TODO: src/dusk/ is NOT excluded: inline code in game headers
            # currently call into it (e.g. dusk::frame_interp::lookup_replacement).
            COMMAND "${_symgen}" def
            --rsp "${_rsp}"
            --out "${_def}"
            --exclude cmake_pch
            --exclude miniz
            --exclude asan_options
            --max-exports 58000
            ${_sdk_args}
            COMMENT "Generating dusklight exports"
            VERBATIM)
    target_link_options(${target} PRIVATE "/DEF:${_def}")

    # Generate import library for mods to link against.
    set(_implib "${CMAKE_BINARY_DIR}/${_config_subdir}dusklight_imports.lib")
    get_filename_component(_compiler_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
    find_program(DUSK_LLVM_DLLTOOL llvm-dlltool HINTS "${_compiler_dir}")
    if (DUSK_LLVM_DLLTOOL)
        set(_implib_cmd "${DUSK_LLVM_DLLTOOL}" -d "${_def}" -D dusklight.exe -m i386:x86-64
                -l "${_implib}")
    else ()
        set(_implib_cmd "${CMAKE_AR}" /nologo "/def:${_def}" /machine:x64 /name:dusklight.exe
                "/out:${_implib}")
    endif ()
    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${_implib_cmd}
            BYPRODUCTS "${_implib}"
            COMMENT "Generating dusklight import library"
            VERBATIM)
    set(DUSK_GAME_IMPLIB "${_implib}" CACHE INTERNAL "Import library for Windows mod linking")
    set(DUSK_GAME_DEF "${_def}" CACHE INTERNAL "Curated export .def for the game executable")

    # Ship the import library as sdk/dusklight.lib in the install tree: mods may use it to
    # compile against Dusklight without having to build the whole game. (See DUSK_GAME_IMPLIB)
    install(FILES "${_implib}" DESTINATION sdk RENAME dusklight.lib)
endfunction()
