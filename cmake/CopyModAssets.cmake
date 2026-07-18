# Copies a mod asset directory for packaging, skipping dotfiles and dot-directories
# (.gitkeep, .DS_Store, ...). Usage: cmake -DSRC=<dir> -DDST=<dir> -P CopyModAssets.cmake
file(MAKE_DIRECTORY "${DST}")
file(GLOB_RECURSE _files RELATIVE "${SRC}" "${SRC}/*")
foreach (_file IN LISTS _files)
    if (NOT _file MATCHES "(^|/)\\.")
        get_filename_component(_dir "${_file}" DIRECTORY)
        file(COPY "${SRC}/${_file}" DESTINATION "${DST}/${_dir}")
    endif ()
endforeach ()
