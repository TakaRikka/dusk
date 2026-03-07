#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JORServer.h"
#include "JSystem/JHostIO/JOREntry.h"
#include "JSystem/JHostIO/JHIhioASync.h"

void JORInit() {
    JHIInit(TRUE);

    JHICommBufWriter* pComWriter = new JHICommBufWriter(0x10000, 0x10000, 4);
    JHICommBufReader* pComReader = new JHICommBufReader(0, 0x10000, 4);

    JHIContext ctx;
    ctx.mp_reader = new JHICommBufReader(0x10000, 0x10000, 4);
    ctx.mp_writer = new JHICommBufWriter(0, 0x10000, 4);

    JHIComPortManager<JHICmnMem>* pPortMng = JHIComPortManager<JHICmnMem>::create();
    pPortMng->getRefPort().setBuf(pComReader, pComWriter);
    JHISetBuffer(&ctx);

    JORServer* pServer = JORServer::create();
    pPortMng->addTag(pServer);
    pServer->sendReset();
}
