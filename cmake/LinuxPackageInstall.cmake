set(ASSET_SOURCE_DIR "${CMAKE_SOURCE_DIR}/platforms/freedesktop")
set(LINUX_APP_ID "dev.twilitrealm.Dusklight")

foreach(RES 16 32 48 64 128 256 512)
    install (FILES ${ASSET_SOURCE_DIR}/${RES}x${RES}/apps/${LINUX_APP_ID}.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/${RES}x${RES}/apps)
endforeach()

install (FILES ${ASSET_SOURCE_DIR}/${LINUX_APP_ID}.metainfo.xml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo/)

install (FILES ${ASSET_SOURCE_DIR}/${LINUX_APP_ID}.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications/)
