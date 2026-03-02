#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JORServer.h"

int JORShellExecute(const char* param_0, const char* param_1, const char* param_2, const char* param_3, int param_4) {
    int rt = 0;
    JORServer* instance = JORServer::getInstance();
    
    JORMContext* mctx = instance->attachMCTX(MCTX_MSG_SHELL_EXEC);
    mctx->sendShellExecuteRequest(&rt, param_0, param_1, param_2, param_3, param_4);
    instance->releaseMCTX(mctx);

    while (rt == 0) {
        JOR_MESSAGELOOP();
    }

    return rt;
}
