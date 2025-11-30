#include <dolphin.h>

// prototypes
DECL_WEAK int Hu_IsStub();
void DBInitInterrupts();
s32 DBQueryData();
u32 DBRead();
u32 DBWrite();
void DBOpen();
void DBClose();

DECL_WEAK int Hu_IsStub() {
    return 1;
}

void DBInitComm(int* inputFlagPtr, int* mtrCallback) {}

void DBInitInterrupts() {}

s32 DBQueryData() {
    return 0;
}

u32 DBRead(u8*, u32) {
    return 0;
}

u32 DBWrite() {
    return 0;
}

void DBOpen() {}

void DBClose() {}
