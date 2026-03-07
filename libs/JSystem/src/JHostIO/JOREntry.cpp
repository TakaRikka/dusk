#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JORServer.h"
#include "JSystem/JHostIO/JOREntry.h"
#include "JSystem/JHostIO/JHIhioASync.h"

void JORInit() {
    JHIInit(TRUE);

    JHICommBufWriter* pComWriter = JKR_NEW JHICommBufWriter(0x10000, 0x10000, 4);
    JHICommBufReader* pComReader = JKR_NEW JHICommBufReader(0, 0x10000, 4);

    JHIContext ctx;
    ctx.mp_reader = JKR_NEW JHICommBufReader(0x10000, 0x10000, 4);
    ctx.mp_writer = JKR_NEW JHICommBufWriter(0, 0x10000, 4);

    JHIComPortManager<JHICmnMem>* pPortMng = JHIComPortManager<JHICmnMem>::create();
    pPortMng->getRefPort().setBuf(pComReader, pComWriter);
    JHISetBuffer(&ctx);

    JORServer* pServer = JORServer::create();
    pPortMng->addTag(pServer);
    pServer->sendReset();
}
