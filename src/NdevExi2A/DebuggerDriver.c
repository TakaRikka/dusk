#include "NdevExi2A/DebuggerDriver.h"

#include "NdevExi2A/exi2.h"
#include <dolphin/exi.h>
#include <dolphin/os.h>

static s32 __DBRecvDataSize;
static u32 __DBRecvMail;

static u8 EXIInputFlag;

static MtrCallback __DBDbgCallback;
static MtrCallback __DBMtrCallback;

static u8 SendCount = 0x80;

#define IS_TRUE(x) ((x) != FALSE)
#define IS_FALSE(x) !IS_TRUE(x)
#define ROUND_UP(x, align) (((x) + (align)-1) & (-(align)))

void __DBMtrHandler(s32 param_0, OSContext* ctx) {
    EXIInputFlag = 1;
    if (__DBMtrCallback != (void*)NULL) {
        __DBMtrCallback(0, ctx);
    }
}

void __DBIntrHandler(s16 param_0, OSContext* ctx) {
    *__PIRegs = 0x1000;
    if (__DBDbgCallback != NULL) {
        __DBDbgCallback(param_0, ctx);
    }
}

void __DBCheckMailBox(void) {
    u8 sp08;
    u32 mail;
    sp08 = __DBReadUSB_CSR();
    if (!(sp08 & 0x8)) {
        __DBReadMailbox(&mail);

        if (ODEMUIsValidMail(mail)) {
            __DBRecvMail = mail;
            __DBRecvDataSize = ODEMUGetSize(mail);
            EXIInputFlag = 1;
        }
    }
}

u8 __DBReadUSB_CSR() {
    u8 result;
    __DBEXIReadReg(0x34000000, &result, 1);
    return result;
}

void DBInitComm(u8** a, MtrCallback callback) {
    BOOL interrupts = OSDisableInterrupts();

    //pEXIInputFlag = &EXIInputFlag;
    *a = &EXIInputFlag;
    __DBMtrCallback = callback;
    __DBEXIInit();

    OSRestoreInterrupts(interrupts);
}

void DBInitInterrupts(void) {
    __OSMaskInterrupts(0x18000);
    __OSMaskInterrupts(0x40);
    __DBDbgCallback = &__DBMtrHandler;
    __OSSetInterruptHandler(0x19, __DBIntrHandler);
    __OSUnmaskInterrupts(0x40);
}

u32 DBQueryData(void) {
    BOOL interrupts;
    EXIInputFlag = 0;
    if (__DBRecvDataSize == 0) {
        interrupts = OSDisableInterrupts();
        __DBCheckMailBox();
        OSRestoreInterrupts(interrupts);
    }
    return __DBRecvDataSize;
}

BOOL DBRead(u32* buffer, s32 count) {
    u32 v;
    u32 interrupts = OSDisableInterrupts();
    v = ODEMUGetPc2NngcOffset(__DBRecvMail) + 0x1000;

    __DBRead(v, buffer, ROUND_UP(count, 4));

    __DBRecvDataSize = 0;
    EXIInputFlag = 0;

    OSRestoreInterrupts(interrupts);

    return 0;
}

BOOL DBWrite(void* src, u32 size) {
    BOOL interrupts;
    u32 v;
    u32 mail;

    interrupts = OSDisableInterrupts();

    __DBWaitForSendMail();

    SendCount++;
    v = (SendCount & 0x1) ? 0x800 : 0;

    while (!__DBWrite(v, src, ROUND_UP(size, 4)))
        ;

    __DBWaitForSendMail();

    mail = ODEMUGenMailData(SendCount, size);
    while (!__DBWriteMailbox(mail))
        ;

    __DBWaitForSendMail();

    OSRestoreInterrupts(interrupts);

    return 0;
}

void __DBWaitForSendMail() {
    u8 busyFlag;
    do {
        busyFlag = __DBReadUSB_CSR();
    } while (busyFlag & 0x4);
}

void DBOpen(void) {}

void DBClose(void) {}

static u32 ODEMUGetSize(u32 mail) {
    return mail & 0x1FFF;
}

BOOL ODEMUIsValidMail(u32 mail) {
    return (mail & 0x1F000000) == 0x1F000000;
}

u32 ODEMUGetPc2NngcOffset(u32 v) {
    return ODEMUGetPage(v) & 0x1 ? 0x800 : 0;
}

u32 ODEMUGetPage(u32 v) {
    return (v & 0xFF0000) >> 16;
}

u32 ODEMUGenMailData(u32 v, u32 size) {
    return (0x1f000000 | ((v << 16) & 0xFF0000)) | (size & 0x1FFF);
}

void __DBReadMailbox(u32* out) {
    __DBEXIReadReg(0x34000200, out, 4);
}

BOOL __DBRead(u32 param_0, void* dst, u32 count) {
    return __DBEXIReadRam(((param_0 + 0xD10000) << 6) & 0x3FFFFF00, dst, count);
}

BOOL __DBWriteMailbox(u32 val) {
    return __DBEXIWriteReg(0xB4000100, &val, 0x4);
}

BOOL __DBWrite(u32 param_0, void* dst, u32 count) {
    return __DBEXIWriteRam((((param_0 + 0xD10000) << 6) & 0x3FFFFF00) | 0x80000000, dst, count);
}
