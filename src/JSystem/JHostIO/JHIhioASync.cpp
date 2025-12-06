#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JHostIO/JHIMccBuf.h"
#include "JSystem/JHostIO/JHIRMcc.h"
#include <dolphin/dolphin.h>
#include "global.h"

u32 gsEnableHostio;
u32 gsEnableInterface;
u32 gsDataToRead;

u8* gsReadBuf;
u8* gsWriteBuf;
JHICommBufReader* gsJHIrecvBuf;
JHICommBufWriter* gsJHIsendBuf;

JHIMccContext gMccContext;

BOOL JHIInit(u32 enabled) {
    gsEnableHostio = enabled;
    gsDataToRead = 0;
    gsEnableInterface = TRUE;

    if (gsEnableHostio) {
        OS_REPORT("INFO:********************************\n");
        OS_REPORT("INFO: Opening Hostio service...\n");

        if (!JHIInitMCC(&gMccContext, NULL)) {
            gsEnableInterface = FALSE;
            OS_REPORT("ERROR: Cannot Open HostIO Interface..\n");
            OS_REPORT("INFO:  *** Disable JHostIO ***\n");
        }

        gsReadBuf = new (32) u8[0xC000];
        gsWriteBuf = new (32) u8[0xC000];

        if (gsReadBuf == NULL || gsWriteBuf == NULL) {
            gsEnableInterface = FALSE;
            OS_REPORT("ERROR: Cannot alloc transport buffer..\n");
            OS_REPORT("INFO:  *** Disable JHostIO ***\n");
        }
    }

    return gsEnableInterface && gsEnableHostio;
}

static void dummyString() {
    DEAD_STRING("INFO:Quitting Hostio...");
}

u32 JHIEventLoop() {
    static u32 tNowWrite;
    static u32 tNowRead;
    static u8* tReadBuf;
    static u8* tWriteBuf;

    int var_r30;
    u32 available_size;

    if (gsEnableHostio && gsEnableInterface) {
        if (tNowRead != 0) {
            var_r30 = gsJHIsendBuf->writeBegin();
            if (var_r30 >= tNowRead) {
                gsJHIsendBuf->write(tReadBuf, tNowRead);
                gsJHIsendBuf->writeEnd();
                tNowRead = 0;
                tReadBuf = gsReadBuf;
            } else if (var_r30 != 0) {
                gsJHIsendBuf->write(tReadBuf, var_r30);
                gsJHIsendBuf->writeEnd();
                tNowRead -= var_r30;
                tReadBuf += var_r30;
            }
        } else {
            available_size = gMccContext.mp_reader->available();
            if (available_size != 0) {
                JHIReport("DEBUG: Dolphin <- PC Data Available : Size:%d\n", available_size);
                gMccContext.mp_reader->readBegin();

                u32 var_r27 = gMccContext.mp_reader->read(gsReadBuf);
                if (var_r27 != 0) {
                    gMccContext.mp_reader->readEnd();
                    tNowRead = 0;
                    tReadBuf = gsReadBuf;

                    u32 var_r28 = gsJHIsendBuf->writeBegin();
                    if (var_r28 >= var_r27) {
                        gsJHIsendBuf->write(tReadBuf, var_r27);
                        gsJHIsendBuf->writeEnd();
                    } else {
                        if (var_r28 != 0) {
                            gsJHIsendBuf->write(tReadBuf, var_r28);
                            gsJHIsendBuf->writeEnd();
                        }
                        tNowRead = var_r27 - var_r28;
                        tReadBuf = gsReadBuf + var_r28;
                    }
                }
            }
        }

        if (tNowWrite != 0) {
            var_r30 = gMccContext.mp_writer->writeBegin();
            if (var_r30 > tNowWrite) {
                gMccContext.mp_writer->write(tWriteBuf, tNowWrite);
                gMccContext.mp_writer->writeEnd();
                tNowWrite = 0;
                tWriteBuf = gsWriteBuf;
            } else if (var_r30 != 0) {
                gMccContext.mp_writer->write(tWriteBuf, var_r30);
                gMccContext.mp_writer->writeEnd();
                tNowWrite -= var_r30;
                tWriteBuf += var_r30;
            }
        } else {
            available_size =  gsJHIrecvBuf->available();
            if (available_size != 0) {
                JHIReport("DEBUG: Dolphin -> PC Data Available : Size:%d\n", available_size);

                u32 temp_r26 = available_size < 0xC000 ? available_size : 0xC000;
                gsJHIrecvBuf->readBegin();
                gsJHIrecvBuf->read(gsWriteBuf, temp_r26);
                gsJHIrecvBuf->readEnd();
                tNowWrite = 0;
                tWriteBuf = gsWriteBuf;

                var_r30 = gMccContext.mp_writer->writeBegin();
                if (var_r30 > temp_r26) {
                    gMccContext.mp_writer->write(tWriteBuf, temp_r26);
                    gMccContext.mp_writer->writeEnd();
                } else if (var_r30 != 0) {
                    gMccContext.mp_writer->write(tWriteBuf, var_r30);
                    gMccContext.mp_writer->writeEnd();
                    tNowWrite = temp_r26 - var_r30;
                    tWriteBuf = gsWriteBuf + var_r30;
                }
            }
        }

        if (!gMccContext.mp_reader->isPort() || !gMccContext.mp_writer->isPort()) {
            gsEnableInterface = 0;
        }
        (void)0;
    } else if (!gsEnableInterface && gsEnableHostio != 0 && JHIInitInterface()) {
        OS_REPORT("INFO: JHostIO: Enabled USB2EXI Interface\n");
        bool sp8 = JHINegotiateInterface(800);
        if (sp8) {
            gMccContext = JHIGetHiSpeedContext();
        } else {
            gMccContext = JHIGetLowSpeedContext();
        }

        gMccContext.mp_reader->enablePort();
        gMccContext.mp_writer->enablePort();
        gMccContext.mp_reader->init();
        gMccContext.mp_writer->init();
        gsEnableInterface = TRUE;
    }

    return 0;
}

BOOL JOR_CHECKINTERFACE() {
    return (gsEnableInterface && gsEnableHostio) != 0;
}

void JHISetBuffer(JHIContext* pCtx) {
    gsJHIrecvBuf = pCtx->mp_reader;
    gsJHIsendBuf = pCtx->mp_writer;
}
