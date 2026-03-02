#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JORServer.h"

u32 JORMessageBox(const char* message, const char* title, u32 style) {
    u32 status = 0;
    JORMContext* mctx = JORServer::getInstance()->attachMCTX(MCTX_MSG_OPEN_MESSAGE_BOX);
    mctx->openMessageBox(&status, style, message, title);
    JORServer::getInstance()->releaseMCTX(mctx);

    while (status == 0) {
        JOR_MESSAGELOOP();
    }

    return status;
}
