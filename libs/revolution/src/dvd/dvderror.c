#include <revolution.h>
#include <revolution/os.h>
#include <revolution/dvd.h>
#include <revolution/nand.h>

#include "os/__os.h"
#include "__dvd.h"

static BOOL ExistFlag = FALSE;
static NANDCommandBlock NandCb;
static NANDFileInfo NandInfo;
static DVDCBCallback Callback;
static u32 NextOffset;
DVDErrorInfo __ErrorInfo ATTRIBUTE_ALIGN(32);
DVDErrorInfo __FirstErrorInfo ATTRIBUTE_ALIGN(32);

void cbForNandClose(s32 result, NANDCommandBlock* block) {
    if (Callback) {
        Callback((result == 0) ? 1 : 2, NULL);
    }
}

void cbForNandWrite(s32 result, NANDCommandBlock* block) {
    if (NANDCloseAsync(&NandInfo, cbForNandClose, &NandCb) != 0) {
        if (Callback) {
            Callback(2, NULL);
        }
    }
}

void cbForNandSeek(s32 result, NANDCommandBlock* block) {
    if (result == sizeof(DVDErrorInfo) * (1 + NextOffset)) {
        if (NextOffset == 0) {
            __ErrorInfo.nextOffset = (1 + NextOffset) % 7;
        }

        DCFlushRange((void*)&__ErrorInfo, sizeof(__ErrorInfo));

        if (NANDWriteAsync(&NandInfo, (void*)&__ErrorInfo, sizeof(__ErrorInfo), cbForNandWrite, &NandCb) != 0) {
            cbForNandWrite(-1, NULL);
        }
    } else {
        if (Callback) {
            Callback(2, NULL);
        }
    }
}

void cbForNandWrite0(s32 result, NANDCommandBlock* block) {
    if (result == sizeof(__FirstErrorInfo)) {
        if (NANDSeekAsync(&NandInfo, (s32)(sizeof(DVDErrorInfo) * (1 + NextOffset)), 0, cbForNandSeek, &NandCb) != 0) {
            cbForNandSeek(-1, NULL);
        }
    } else {
        if (Callback) {
            Callback(2, NULL);
        }
    }
}

void cbForNandSeek2(s32 result, NANDCommandBlock* block) {
    if (result == sizeof(DVDErrorInfo)) {
        __FirstErrorInfo.nextOffset = (__FirstErrorInfo.nextOffset + 1) % 7;

        if (NANDWriteAsync(&NandInfo, (void*)&__FirstErrorInfo, sizeof(__FirstErrorInfo), cbForNandWrite0, &NandCb) != 0) {
            cbForNandWrite0(-1, NULL);
        }
    } else {
        if (Callback) {
            Callback(2, NULL);
        }
    }
}

void cbForNandRead(s32 result, NANDCommandBlock* block) {
    if (result == sizeof(DVDErrorInfo)) {
        NextOffset = __FirstErrorInfo.nextOffset;

        if (NANDSeekAsync(&NandInfo, sizeof(DVDErrorInfo), 0, cbForNandSeek2, &NandCb) != 0) {
            cbForNandSeek2(-1, NULL);
        }
    }  else {
        __ErrorInfo.nextOffset = 1;
        if (NANDWriteAsync(&NandInfo, (void*)&__ErrorInfo, sizeof(__ErrorInfo), cbForNandWrite, &NandCb) != 0) {
            cbForNandWrite(-1, NULL);
        }
    }
}

void cbForNandSeek0(s32 result, NANDCommandBlock* block) {
    if (result == 0) {
        NextOffset = 0;
        __ErrorInfo.nextOffset = 1;

        if (NANDWriteAsync(&NandInfo, (void*)&__FirstErrorInfo, sizeof(__FirstErrorInfo), cbForNandWrite0, &NandCb) != 0) {
            cbForNandWrite0(-1, NULL);
        }
    } else {
        if (Callback) {
            Callback(2, NULL);
        }
    }
}

void cbForNandSeek1(s32 result, NANDCommandBlock* block) {
    if (result == sizeof(DVDErrorInfo)) {
        if (NANDReadAsync(&NandInfo, (void*)&__FirstErrorInfo, sizeof(__FirstErrorInfo), cbForNandRead, &NandCb) != 0) {
            cbForNandRead(-1, NULL);
        }
    } else {
        if (NANDSeekAsync(&NandInfo, 0, 0, cbForNandSeek0, &NandCb) != 0) {
            cbForNandSeek0(-1, NULL);
        }
    }
}

void cbForNandOpen(s32 result, NANDCommandBlock* block) {
    if (result == 0) {
        if (ExistFlag) {
            if (NANDSeekAsync(&NandInfo, sizeof(DVDErrorInfo), 0, cbForNandSeek1, &NandCb) != 0) {
                cbForNandSeek1(-1, NULL);
            }
        } else {
            NextOffset = 0;
            __ErrorInfo.nextOffset = 1;
            if (NANDWriteAsync(&NandInfo, (void*)&__FirstErrorInfo, sizeof(__FirstErrorInfo), cbForNandWrite0, &NandCb) != 0) {
                cbForNandWrite0(-1, NULL);
            }
        }
    } else {
        if (Callback) {
            Callback(2, NULL);
        }
    }
}

void cbForNandCreate(s32 result, NANDCommandBlock* block) {
    if (result == 0 || result == -6) {
        if (result == -6) {
            ExistFlag = TRUE;
        }

        if (NANDPrivateOpenAsync("/shared2/test2/dvderror.dat", &NandInfo, 3, cbForNandOpen, &NandCb) != 0) {
            if (Callback) {
               Callback(2, NULL);
            }
        }
    } else {
        if (Callback) {
            Callback(2, NULL);
        }
    }
}

void cbForNandCreateDir(s32 result, NANDCommandBlock* block) {
    if (result == 0 || result == -6) {
        if (NANDPrivateCreateAsync("/shared2/test2/dvderror.dat", 0x3F, 0,  cbForNandCreate, &NandCb) != 0) {
            if (Callback) {
                Callback(2, NULL);
            }
        }
    } else {
        if (Callback) {
            Callback(2, NULL);
        }
    }
}

void cbForPrepareControlRegister(u32 intType) {
    if (intType == 1) {
        __ErrorInfo.unk_0x14 = DVDLowGetControlRegister();
    } else {
        __ErrorInfo.unk_0x14 = 0xFFFFFFFF;
    }

    if (NANDPrivateCreateDirAsync("/shared2/test2", 0x3F, 0, cbForNandCreateDir, &NandCb) != 0) {
        if (Callback) {
            Callback(2, NULL);
        }
    }
}

void cbForPrepareStatusRegister(u32 intType) {
    if (intType == 1) {
        __ErrorInfo.status = DVDLowGetStatusRegister();
    } else {
        __ErrorInfo.status = 0xFFFFFFFF;
    }

    if (DVDLowPrepareControlRegister(cbForPrepareControlRegister) == 0) {
        if (Callback) {
            Callback(2, NULL);
        }
    }
}

void __DVDStoreErrorCode(u32 error, DVDCBCallback callback) {
    __ErrorInfo.error = error;
    __ErrorInfo.dateTime = (u32)OSTicksToSeconds(OSGetTime());
    Callback = callback;
    DVDLowPrepareStatusRegister(cbForPrepareStatusRegister);
}
