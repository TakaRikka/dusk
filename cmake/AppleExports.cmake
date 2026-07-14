include_guard(GLOBAL)

get_filename_component(_dir "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)

# Apple mod linking: symgen scans and filters the game's exports, generating an exports list used for the executable
# link step, and generates a MH_EXECUTE Mach-O stub that mods can link against without having to build the whole game.
function(setup_apple_exports target)
    include("${_dir}/SymbolManifest.cmake")
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

    # Dawn is linked statically on Apple; mods reach wgpu* through the executable.
    if (TARGET dawn::webgpu_dawn)
        get_target_property(_dawn_type dawn::webgpu_dawn TYPE)
        if (_dawn_type STREQUAL "STATIC_LIBRARY")
            list(APPEND _sdk_args --sdk-lib "$<TARGET_FILE:dawn::webgpu_dawn>")
        endif ()
    endif ()

    set(_exp "${CMAKE_BINARY_DIR}/${_config_subdir}dusklight_exports.exp")
    add_custom_command(TARGET ${target} PRE_LINK
            COMMAND "${_symgen}" exports
            "@${_rsp}"
            --out "${_exp}"
            --exclude cmake_pch
            --exclude miniz
            --exclude asan_options
            --exclude src/dusk
            ${_sdk_args}
            COMMENT "Generating dusklight exports"
            VERBATIM)
    target_link_options(${target} PRIVATE -Xlinker -exported_symbols_list -Xlinker "${_exp}")

    # Generate the stub executable mods link against via -bundle_loader.
    set(_stub_args)
    if (IOS)
        set(_stub_platform "ios")
        list(APPEND _stub_args --platform ios)
    elseif (TVOS)
        set(_stub_platform "tvos")
        list(APPEND _stub_args --platform tvos)
    else ()
        set(_stub_platform "macos")
    endif ()
    if (CMAKE_OSX_DEPLOYMENT_TARGET)
        list(APPEND _stub_args --min-os "${CMAKE_OSX_DEPLOYMENT_TARGET}")
    endif ()
    if (CMAKE_OSX_ARCHITECTURES)
        set(_archs "${CMAKE_OSX_ARCHITECTURES}")
    else ()
        set(_archs "${CMAKE_SYSTEM_PROCESSOR}")
    endif ()
    set(_arch_names "")
    foreach (_arch IN LISTS _archs)
        string(TOLOWER "${_arch}" _arch)
        list(APPEND _stub_args --arch "${_arch}")
        list(APPEND _arch_names "${_arch}")
    endforeach ()
    list(JOIN _arch_names "_" _arch_name)

    set(_stub "${CMAKE_BINARY_DIR}/${_config_subdir}dusklight-stub")
    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${_symgen}" stub -f macho "${_exp}" -o "${_stub}" ${_stub_args}
            BYPRODUCTS "${_stub}"
            COMMENT "Generating dusklight link stub"
            VERBATIM)
    install(FILES "${_stub}" DESTINATION sdk RENAME "stub-${_stub_platform}-${_arch_name}")
endfunction()
