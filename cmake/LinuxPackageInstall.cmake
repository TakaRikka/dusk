set(ASSET_SOURCE_DIR "${CMAKE_SOURCE_DIR}/platforms/freedesktop")

foreach(RES 16 32 48 64 128 256 512)
    install (FILES ${ASSET_SOURCE_DIR}/${RES}x${RES}/apps/dusklight.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/${RES}x${RES}/apps
        RENAME "${DUSK_APP_ID}.png")
endforeach()

configure_file(${ASSET_SOURCE_DIR}/dusklight.desktop.in ${CMAKE_BINARY_DIR}/${DUSK_APP_ID}.desktop)
install (FILES ${CMAKE_BINARY_DIR}/${DUSK_APP_ID}.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications/)

configure_file(${ASSET_SOURCE_DIR}/dusklight.metainfo.xml.in ${CMAKE_BINARY_DIR}/${DUSK_APP_ID}.metainfo.xml)
install (FILES ${CMAKE_BINARY_DIR}/${DUSK_APP_ID}.metainfo.xml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo/)
