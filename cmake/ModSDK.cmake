# add_mod(<target> SOURCES <file>... MOD_JSON <mod.json> [RES_DIR <res>] [OVERLAY_DIR <overlay>]
#         [TEXTURES_DIR <textures>] [OUTPUT_DIR <dir>] [BUNDLE])
set(DUSK_MODS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/mods" CACHE PATH "Directory to write mod packages into")

function(_mod_lib_name out_var)
    set(_arch "${CMAKE_SYSTEM_PROCESSOR}")
    if (APPLE AND CMAKE_OSX_ARCHITECTURES)
        list(LENGTH CMAKE_OSX_ARCHITECTURES _count)
        if (_count GREATER 1)
            message(FATAL_ERROR "add_mod: universal binaries are not supported")
        endif ()
        set(_arch "${CMAKE_OSX_ARCHITECTURES}")
    endif ()
    string(TOLOWER "${CMAKE_SYSTEM_NAME}" _platform)
    string(TOLOWER "${_arch}" _arch)
    if (_arch MATCHES "^(i[3-6]86|x86)$")
        set(_arch "x86")
    endif ()
    if (WIN32)
        set(_ext ".dll")
    elseif (APPLE)
        set(_ext ".dylib")
    else ()
        set(_ext ".so")
    endif ()
    set(${out_var} "${_platform}-${_arch}${_ext}" PARENT_SCOPE)
endfunction()

function(_mod_resolve_source_path out_var path)
    if (IS_ABSOLUTE "${path}")
        set(_path "${path}")
    else ()
        set(_path "${CMAKE_CURRENT_SOURCE_DIR}/${path}")
    endif ()
    set(${out_var} "${_path}" PARENT_SCOPE)
endfunction()

function(_mod_collect_assets out_var dir)
    if (NOT IS_DIRECTORY "${dir}")
        message(FATAL_ERROR "add_mod: asset directory does not exist: ${dir}")
    endif ()

    file(GLOB_RECURSE _files CONFIGURE_DEPENDS LIST_DIRECTORIES false "${dir}/*")
    set(${out_var} ${_files} PARENT_SCOPE)
endfunction()

function(add_mod target_name)
    cmake_parse_arguments(ARG "BUNDLE" "MOD_JSON;RES_DIR;OVERLAY_DIR;TEXTURES_DIR;OUTPUT_DIR" "SOURCES" ${ARGN})
    if (NOT ARG_MOD_JSON)
        message(FATAL_ERROR "add_mod: MOD_JSON is required")
    endif ()
    _mod_resolve_source_path(_mod_json "${ARG_MOD_JSON}")
    if (NOT EXISTS "${_mod_json}")
        message(FATAL_ERROR "add_mod: MOD_JSON does not exist: ${_mod_json}")
    endif ()

    set(_has_lib FALSE)
    set(_lib_name "")
    if (ARG_SOURCES)
        set(_has_lib TRUE)
        add_library(${target_name} SHARED ${ARG_SOURCES})
        _mod_lib_name(_lib_name)
        set_target_properties(${target_name} PROPERTIES
                PREFIX ""
                C_VISIBILITY_PRESET hidden
                CXX_VISIBILITY_PRESET hidden
                VISIBILITY_INLINES_HIDDEN ON
                WINDOWS_EXPORT_ALL_SYMBOLS OFF)
        target_compile_features(${target_name} PRIVATE cxx_std_20)
        target_link_libraries(${target_name} PRIVATE dusklight_game_headers)

        if (NOT TARGET dusklight)
            # Apply global compile options for out-of-tree mod builds
            if (CMAKE_SYSTEM_NAME STREQUAL Linux)
                target_compile_options(${target_name} PRIVATE
                        -Wno-multichar -Wno-trigraphs -Wno-deprecated-declarations)
            elseif (APPLE)
                target_compile_options(${target_name} PRIVATE
                        -Wno-declaration-after-statement -Wno-non-pod-varargs)
            elseif (MSVC)
                target_compile_options(${target_name} PRIVATE
                        "$<$<COMPILE_LANGUAGE:C,CXX>:/bigobj>"
                        "$<$<COMPILE_LANGUAGE:C,CXX>:/utf-8>")
            endif ()
            # Use signed char on ARM to match the original game (and x86)
            string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" _mod_arch)
            if (_mod_arch MATCHES "^(arm|aarch64)" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
                target_compile_options(${target_name} PRIVATE -fsigned-char)
            endif ()
        endif ()

        if (APPLE)
            # Game symbols resolve against the host executable at dlopen time.
            target_link_options(${target_name} PRIVATE -undefined dynamic_lookup)
        elseif (ANDROID)
            if (TARGET dusklight)
                target_link_libraries(${target_name} PRIVATE dusklight)
            elseif (DUSK_GAME_SOLIB)
                target_link_libraries(${target_name} PRIVATE "${DUSK_GAME_SOLIB}")
            else ()
                message(FATAL_ERROR "add_mod: DUSK_GAME_SOLIB is not set (libmain.so)")
            endif ()
        elseif (UNIX)
            target_link_options(${target_name} PRIVATE -Wl,--allow-shlib-undefined)
        elseif (WIN32)
            # Link against the generated import library (game ABI surface). Function calls
            # resolve through import thunks. Data is toolchain dependent:
            # - clang-cl: lld's mingw mode auto-imports data references, fixed up at load by
            #   the mod SDK's pseudo-relocation runtime (pseudo_reloc.cpp).
            # - cl (MSVC): only DUSK_GAME_DATA-annotated data is reachable. Un-annotated
            #   references fail to link.
            if (NOT DUSK_GAME_IMPLIB)
                message(FATAL_ERROR "add_mod: DUSK_GAME_IMPLIB is not set.")
            endif ()
            target_link_libraries(${target_name} PRIVATE "${DUSK_GAME_IMPLIB}")
            set_target_properties(${target_name} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
            target_compile_definitions(${target_name} PRIVATE _ITERATOR_DEBUG_LEVEL=0)
            if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
                target_compile_options(${target_name} PRIVATE "$<$<COMPILE_LANGUAGE:C,CXX>:/clang:-mcmodel=large>")
                target_sources(${target_name} PRIVATE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../sdk/pseudo_reloc.cpp")
                # lld mingw mode rewrites /DEFAULTLIB directives to -l style and skips %LIB%, so
                # the CRT libraries and search paths are spelled out explicitly.
                target_link_options(${target_name} PRIVATE -lldmingw /nodefaultlib /INCREMENTAL:NO)
                target_link_libraries(${target_name} PRIVATE
                        msvcrt.lib msvcprt.lib vcruntime.lib ucrt.lib
                        oldnames.lib uuid.lib kernel32.lib user32.lib)
                set(_lib_dirs "$ENV{LIB}")
                if ("${_lib_dirs}" STREQUAL "")
                    message(FATAL_ERROR "add_mod: %LIB% is empty; configure from a VS dev shell")
                endif ()
                foreach (_libdir IN LISTS _lib_dirs)
                    target_link_options(${target_name} PRIVATE "/libpath:${_libdir}")
                endforeach ()
            endif ()
        endif ()
    endif ()

    set(_output_dir "${DUSK_MODS_OUTPUT_DIR}")
    if (ARG_OUTPUT_DIR)
        set(_output_dir "${ARG_OUTPUT_DIR}")
    endif ()
    set(_stage "${CMAKE_CURRENT_BINARY_DIR}/${target_name}_stage")
    set(_out "${_output_dir}/${target_name}.dusk")

    set(_zip_args "${_lib_name}" mod.json)
    set(_package_deps "${_mod_json}")
    set(_package_inputs "${_mod_json}")
    set(_extra_cmds "")
    set(_lib_copy_cmd "")
    set(_target_depend "")
    if (_has_lib)
        list(APPEND _zip_args "${_lib_name}")
        set(_lib_copy_cmd COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:${target_name}>" "${_stage}/${_lib_name}")
        set(_target_depend ${target_name})
    endif ()
    if (ARG_RES_DIR)
        _mod_resolve_source_path(_res_dir "${ARG_RES_DIR}")
        _mod_collect_assets(_res_deps "${_res_dir}")
        list(APPEND _package_deps ${_res_deps})
        list(APPEND _package_inputs "${_res_dir}" ${_res_deps})
        list(APPEND _zip_args res)
        list(APPEND _extra_cmds COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${_res_dir}" "${_stage}/res")
    endif ()
    if (ARG_OVERLAY_DIR)
        _mod_resolve_source_path(_overlay_dir "${ARG_OVERLAY_DIR}")
        _mod_collect_assets(_overlay_deps "${_overlay_dir}")
        list(APPEND _package_deps ${_overlay_deps})
        list(APPEND _package_inputs "${_overlay_dir}" ${_overlay_deps})
        list(APPEND _zip_args overlay)
        list(APPEND _extra_cmds COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${_overlay_dir}" "${_stage}/overlay")
    endif ()
    if (ARG_TEXTURES_DIR)
        _mod_resolve_source_path(_textures_dir "${ARG_TEXTURES_DIR}")
        _mod_collect_assets(_textures_deps "${_textures_dir}")
        list(APPEND _package_deps ${_textures_deps})
        list(APPEND _package_inputs "${_textures_dir}" ${_textures_deps})
        list(APPEND _zip_args textures)
        list(APPEND _extra_cmds COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${_textures_dir}" "${_stage}/textures")
    endif ()

    set(_bundle_cmds "")
    if (ARG_BUNDLE AND TARGET dusklight)
        file(READ "${_mod_json}" _mod_json_text)
        string(JSON _mod_id GET "${_mod_json_text}" id)
        set_property(GLOBAL APPEND PROPERTY DUSK_BUNDLED_MOD_TARGETS "${target_name}")
        set_property(GLOBAL APPEND PROPERTY DUSK_BUNDLED_MOD_IDS "${_mod_id}")
        set_property(GLOBAL APPEND PROPERTY DUSK_BUNDLED_MOD_STAGES "${_stage}")
        set_property(GLOBAL APPEND PROPERTY DUSK_BUNDLED_MOD_PACKAGES "${_out}")
        set_property(GLOBAL APPEND PROPERTY DUSK_BUNDLED_MOD_LIB_NAMES "${_lib_name}")
        set(_bundle_cmds
                COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bundled_mods"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_out}" "${CMAKE_BINARY_DIR}/bundled_mods/${target_name}.dusk")
    endif ()

    set(_package_target "${target_name}_package")
    set(_package_inputs_file "${CMAKE_CURRENT_BINARY_DIR}/${target_name}_package_inputs.txt")
    list(SORT _package_inputs)
    set(_package_inputs_text "")
    foreach (_package_input IN LISTS _package_inputs)
        string(APPEND _package_inputs_text "${_package_input}\n")
    endforeach ()
    file(GENERATE OUTPUT "${_package_inputs_file}" CONTENT "${_package_inputs_text}")
    add_custom_command(OUTPUT "${_out}"
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${_stage}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${_stage}" "${_output_dir}"
            ${_lib_copy_cmd}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_mod_json}" "${_stage}/mod.json"
            ${_extra_cmds}
            COMMAND ${CMAKE_COMMAND} -E chdir "${_stage}" ${CMAKE_COMMAND} -E tar cvf "${_out}" --format=zip ${_zip_args}
            ${_bundle_cmds}
            DEPENDS ${_target_depend} ${_package_deps} "${_package_inputs_file}"
            COMMENT "Packaging ${target_name} -> ${_out}"
            COMMAND_EXPAND_LISTS
            VERBATIM
    )
    add_custom_target(${_package_target} ALL DEPENDS "${_out}")
    if (TARGET dusklight_mods)
        add_dependencies(dusklight_mods ${_package_target})
    endif ()
endfunction()

# Install rules for BUNDLE mods.
# - Windows: the .dusk archives into <install>/mods (the loader extracts native libs to the
#   user cache).
# - Linux: pre-extracted stage dirs into <install>/mods so native libs dlopen in place from
#   read-only installs.
# - macOS: pre-extracted stage dirs into the installed app's Contents/Resources/mods, dylibs
#   ad-hoc signed in place, then the whole bundle re-signed.
# - iOS/tvOS: assets into <app>/mods/<id> and the dylib into Frameworks/<id>.dylib.
# - Android: nothing here; gradle packs ${CMAKE_BINARY_DIR}/bundled_mods into APK assets.
function(install_bundled_mods)
    get_property(_targets GLOBAL PROPERTY DUSK_BUNDLED_MOD_TARGETS)
    if (NOT _targets OR ANDROID)
        return ()
    endif ()
    get_property(_ids GLOBAL PROPERTY DUSK_BUNDLED_MOD_IDS)
    get_property(_stages GLOBAL PROPERTY DUSK_BUNDLED_MOD_STAGES)
    get_property(_lib_names GLOBAL PROPERTY DUSK_BUNDLED_MOD_LIB_NAMES)
    list(LENGTH _targets _count)
    math(EXPR _last "${_count} - 1")

    if (APPLE)
        get_target_property(_app_name dusklight OUTPUT_NAME)
        if (NOT _app_name)
            set(_app_name dusklight)
        endif ()
        set(_bundle_dir "${CMAKE_INSTALL_PREFIX}/${_app_name}.app")
        if (IOS OR TVOS)
            foreach (_i RANGE ${_last})
                list(GET _targets ${_i} _target)
                list(GET _ids ${_i} _id)
                list(GET _stages ${_i} _stage)
                list(GET _lib_names ${_i} _lib_name)
                install(DIRECTORY "${_stage}/" DESTINATION "${_bundle_dir}/mods/${_id}"
                        PATTERN "${_lib_name}" EXCLUDE)
                install(PROGRAMS "$<TARGET_FILE:${_target}>"
                        DESTINATION "${_bundle_dir}/Frameworks" RENAME "${_id}.dylib")
            endforeach ()
        else ()
            foreach (_i RANGE ${_last})
                list(GET _ids ${_i} _id)
                list(GET _stages ${_i} _stage)
                list(GET _lib_names ${_i} _lib_name)
                install(DIRECTORY "${_stage}/" DESTINATION "${_bundle_dir}/Contents/Resources/mods/${_id}")
                install(CODE "execute_process(COMMAND /usr/bin/codesign --force --sign - \"${_bundle_dir}/Contents/Resources/mods/${_id}/${_lib_name}\" COMMAND_ERROR_IS_FATAL ANY)")
            endforeach ()
            if (TARGET crashpad_handler)
                install(CODE "execute_process(COMMAND /usr/bin/codesign --force --sign - \"${_bundle_dir}/Contents/MacOS/$<TARGET_FILE_NAME:crashpad_handler>\" COMMAND_ERROR_IS_FATAL ANY)")
            endif ()
            install(CODE "execute_process(COMMAND /usr/bin/codesign --force --sign - --entitlements \"${DUSK_ENTITLEMENTS}\" \"${_bundle_dir}\" COMMAND_ERROR_IS_FATAL ANY)")
        endif ()
        return ()
    endif ()

    if (DUSK_PACKAGE_INSTALL)
        set(_mods_dest "${CMAKE_INSTALL_DATAROOTDIR}/dusklight/mods")
    else ()
        set(_mods_dest "${CMAKE_INSTALL_PREFIX}/mods")
    endif ()
    if (WIN32)
        foreach (_target IN LISTS _targets)
            install(FILES "${CMAKE_BINARY_DIR}/bundled_mods/${_target}.dusk" DESTINATION "${_mods_dest}")
        endforeach ()
    else ()
        foreach (_i RANGE ${_last})
            list(GET _ids ${_i} _id)
            list(GET _stages ${_i} _stage)
            install(DIRECTORY "${_stage}/" DESTINATION "${_mods_dest}/${_id}")
        endforeach ()
    endif ()
endfunction()
