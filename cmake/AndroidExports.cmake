include_guard(GLOBAL)

get_filename_component(_dir "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)

# Android mod linking: symgen scans and filters the game's exports, generating a version script used for the executable
# link step, and generates a shared object stub that mods can link against without having to build the whole game.
function(setup_android_exports target)
    include("${_dir}/SymbolManifest.cmake")
    ensure_symgen(TRUE)
    add_dependencies(${target} symgen)

    set(_rsp_lines "$<TARGET_OBJECTS:${target}>")
    foreach (_lib IN LISTS JSYSTEM_LIBRARIES)
        list(APPEND _rsp_lines "$<TARGET_FILE:${_lib}>")
    endforeach ()
    list(JOIN _rsp_lines "\n" _rsp_content)
    set(_rsp "${CMAKE_BINARY_DIR}/dusklight_exports_input.rsp")
    file(GENERATE OUTPUT "${_rsp}" CONTENT "${_rsp_content}")

    set(_sdk_args)
    foreach (_lib aurora_card aurora_core aurora_dvd aurora_gd aurora_gx aurora_mtx
            aurora_os aurora_pad aurora_si aurora_vi)
        if (TARGET ${_lib})
            list(APPEND _sdk_args --sdk-lib "$<TARGET_FILE:${_lib}>")
        endif ()
    endforeach ()
    if (TARGET dawn::webgpu_dawn)
        get_target_property(_dawn_type dawn::webgpu_dawn TYPE)
        if (_dawn_type STREQUAL "STATIC_LIBRARY")
            list(APPEND _sdk_args --sdk-lib "$<TARGET_FILE:dawn::webgpu_dawn>")
        endif ()
    endif ()

    set(_vscript "${CMAKE_BINARY_DIR}/dusklight_exports.ver")
    add_custom_command(TARGET ${target} PRE_LINK
            COMMAND "${SYMGEN_EXE}" exports
            "@${_rsp}"
            --out "${_vscript}"
            --format version-script
            --exclude cmake_pch
            --exclude miniz
            --exclude asan_options
            --exclude src/dusk
            # Resolved from the Java side; the SDL ones live in the statically-linked
            # SDL archive, outside the provenance scan.
            --extra-sym JNI_OnLoad
            --extra-sym SDL_main
            --extra-sym "Java_*"
            ${_sdk_args}
            COMMENT "Generating dusklight exports"
            VERBATIM)
    target_link_options(${target} PRIVATE "-Wl,--version-script=${_vscript}")

    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" _arch)
    set(_stub "${CMAKE_BINARY_DIR}/stub-android-${_arch}.so")
    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${SYMGEN_EXE}" stub -f elf "$<TARGET_FILE:${target}>" -o "${_stub}"
            --soname "$<TARGET_FILE_NAME:${target}>" --arch "${_arch}"
            BYPRODUCTS "${_stub}"
            COMMENT "Generating dusklight link stub"
            VERBATIM)
    install(FILES "${_stub}" DESTINATION sdk)
endfunction()
