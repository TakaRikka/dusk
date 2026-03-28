ExternalProject_Add(
    libjpeg-turbo
    URL https://github.com/libjpeg-turbo/libjpeg-turbo/archive/refs/tags/3.1.90.tar.gz
    URL_HASH SHA256=076ef1431f2803a91f07e0f92433d4dcf39bc9113226c4f46ba3d3d54f514c9d
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)

ExternalProject_Get_Property(libjpeg-turbo INSTALL_DIR)
add_library(libjpeg-turbo-lib STATIC IMPORTED GLOBAL)
set_target_properties(libjpeg-turbo-lib PROPERTIES IMPORTED_LOCATION ${INSTALL_DIR}/lib/turbojpeg-static.lib)
target_include_directories(libjpeg-turbo-lib INTERFACE ${INSTALL_DIR}/include)
