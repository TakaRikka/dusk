include_guard(GLOBAL)

get_filename_component(_DUSK_WINDOWS_EXPORTS_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)

# Windows mod linking: generate the curated export surface for the game executable and the
# import library mods link against. symgen scans the built objects, filters by source, and
# writes a .def used by the main link and import library generation.
function(setup_windows_exports target)
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" _implib_arch)
    if (_implib_arch STREQUAL "arm64")
        set(_dlltool_machine "arm64")
        set(_lib_machine "arm64")
    elseif (_implib_arch MATCHES "^(amd64|x86_64)$")
        set(_dlltool_machine "i386:x86-64")
        set(_lib_machine "x64")
    else ()
        message(FATAL_ERROR
                "dusklight: no Windows mod linking support for ${CMAKE_SYSTEM_PROCESSOR}")
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

    set(_forward_args)
    if (TARGET dawn::webgpu_dawn)
        get_target_property(_dawn_type dawn::webgpu_dawn TYPE)
        if (_dawn_type STREQUAL "SHARED_LIBRARY")
            list(APPEND _forward_args
                    --forward-dll "$<TARGET_FILE:dawn::webgpu_dawn>"
                    --forward-sym-prefix wgpu)
        endif ()
    endif ()

    # Generate curated exports list from the main binary
    set(_def "${CMAKE_BINARY_DIR}/${_config_subdir}dusklight_exports.def")
    add_custom_command(TARGET ${target} PRE_LINK
            COMMAND "${_symgen}" def
            "@${_rsp}"
            --out "${_def}"
            --exclude cmake_pch
            --exclude miniz
            --exclude asan_options
            --exclude src/dusk
            --max-exports 58000
            ${_sdk_args}
            ${_forward_args}
            COMMENT "Generating dusklight exports"
            VERBATIM)
    target_link_options(${target} PRIVATE "/DEF:${_def}")

    # Generate import library for mods to link against.
    set(_implib "${CMAKE_BINARY_DIR}/${_config_subdir}dusklight_imports.lib")
    get_filename_component(_compiler_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
    find_program(DUSK_LLVM_DLLTOOL llvm-dlltool HINTS "${_compiler_dir}")
    if (DUSK_LLVM_DLLTOOL)
        set(_implib_cmd "${DUSK_LLVM_DLLTOOL}" -d "${_def}" -D dusklight.exe
                -m "${_dlltool_machine}" -l "${_implib}")
    else ()
        set(_implib_cmd "${CMAKE_AR}" /nologo "/def:${_def}" "/machine:${_lib_machine}"
                /name:dusklight.exe "/out:${_implib}")
    endif ()
    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${_implib_cmd}
            BYPRODUCTS "${_implib}"
            COMMENT "Generating dusklight import library"
            VERBATIM)
    set(DUSK_GAME_IMPLIB "${_implib}" CACHE INTERNAL "Import library for Windows mod linking")

    install(FILES "${_implib}" DESTINATION sdk RENAME "windows-${_implib_arch}.lib")
endfunction()
