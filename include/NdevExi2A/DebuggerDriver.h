#ifndef NDEVEXI2A_DEBUGGERDRIVER_H
#define NDEVEXI2A_DEBUGGERDRIVER_H

#include <dolphin/os.h>
#include "types.h"

typedef void (*MtrCallback)(s32, OSContext*);
typedef void (*IntrCallback)(s16, OSContext*);

#include <NdevExi2A/DebuggerDriver.h>

static u32 ODEMUGetSize(u32 mail);

static void __DBMtrHandler(s32 param_0, OSContext* ctx);

static void __DBIntrHandler(s16 param_0, OSContext* ctx);

u8 __DBReadUSB_CSR();

void __DBReadMailbox(u32* out);

void __DBCheckMailBox(void);

BOOL __DBWriteMailbox(u32 val);

BOOL __DBRead(u32 param_0, void* dst, u32 count);

BOOL __DBWrite(u32 param_0, void* src, u32 count);

void DBInitComm(u8** a, MtrCallback callback);

void DBInitInterrupts(void);

BOOL ODEMUIsValidMail(u32 mail);

u32 DBQueryData(void);

u32 ODEMUGetPage(u32 v);

u32 ODEMUGetPc2NngcOffset(u32 v);

BOOL DBRead(u32* buffer, s32 count);

void __DBWaitForSendMail();

u32 ODEMUGenMailData(u32 v, u32 size);

BOOL DBWrite(void* src, u32 size);

void DBOpen(void);

void DBClose(void);

#endif /* NDEVEXI2A_DEBUGGERDRIVER_H */
