#include <revolution/exi.h>
#include <revolution/os.h>
#include "__os.h"

static s32 Chan;
static u32 Dev;
static u32 BarnacleEnabled;
static u32 Enabled;

// prototypes
int InitializeUART(void);
int ReadUARTN(void);
int WriteUARTN(void *buf, u32 len);
void __OSEnableBarnacle(s32 chan, u32 dev);

static BOOL ProbeBarnacle(s32 chan, u32 dev, u32* revision) {
    int err;
    u32 cmd;

    if (chan != 2 && dev == 0 && !EXIAttach(chan, NULL)) {
        return FALSE;
    }

    err = !EXILock(chan, dev, NULL);
    if (!err) {
        err = !EXISelect(chan, dev, EXI_FREQ_1M);
        if (!err) {
            cmd = 0x20011300;
            err = FALSE;
            err |= !EXIImm(chan, &cmd, sizeof(cmd), EXI_WRITE, NULL);
            err |= !EXISync(chan);
            err |= !EXIImm(chan, revision, sizeof(revision), EXI_READ, NULL);
            err |= !EXISync(chan);
            err |= !EXIDeselect(chan);
        }

        EXIUnlock(chan);
    }

    if (chan != 2 && dev == 0) {
        EXIDetach(chan);
    }

    if (err) {
        return FALSE;
    }

    return (*revision != 0xFFFFFFFF) ? TRUE : FALSE;
}

void __OSEnableBarnacle(s32 chan, u32 dev) {
    u32 id;

    if (!EXIGetID(chan, dev, &id)) {
        return;
    }

    switch (id) {
    case EXI_MEMORY_CARD_59:
    case EXI_MEMORY_CARD_123:
    case EXI_MEMORY_CARD_251:
    case EXI_MEMORY_CARD_507:
    case EXI_USB_ADAPTER:
    case EXI_NPDP_GDEV:
    case EXI_MODEM:
    case 0x03010000:
    case 0x04020100:
    case EXI_ETHER:
    case 0x04020300:
    case 0x04220000:
    case EXI_RS232C:
    case EXI_MIC:
    case EXI_AD16:
    case EXI_STREAM_HANGER:
    case 0x80000004:
    case 0x80000008:
    case 0x80000010:
    case 0x80000020:
    case 0xFFFFFFFF:
        break;
    default:
        if (ProbeBarnacle(chan, dev, &id)) {
            Chan = chan;
            Dev = dev;
            Enabled = BarnacleEnabled = 0xA5FF005A;
            break;
        }
    }
}

int InitializeUART(void) {
    if (BarnacleEnabled == 0xA5FF005A) {
        return 0;
    }

    if ((OSGetConsoleType() & 0x10000000) == 0) {
        Enabled = 0;
        return 2;
    }

    Chan = 0;
    Dev = 1;
    Enabled = 0xA5FF005A;
    return 0;
}

int ReadUARTN(void) {
    return 4;
}

static int QueueLength(void) {
    u32 cmd;

    if (EXISelect(Chan, Dev, 3) == 0) {
        return -1;
    }

    cmd = 0x20010000;
    EXIImm(Chan, &cmd, sizeof(cmd), EXI_WRITE, 0);
    EXISync(Chan);
    EXIImm(Chan, &cmd, 1, EXI_READ, 0);
    EXISync(Chan);
    EXIDeselect(Chan);
    return 0x10 - (cmd >> 0x18);
}

int WriteUARTN(void *buf, u32 len) {
    u32 cmd;
    s32 xLen;
    int qLen;
    char* ptr;
    int locked;
    int error;

    if ((Enabled - 0xA5FF0000) != 0x5A) {
        return 2;
    }

    locked = EXILock(Chan, Dev, 0);
    if (locked == 0) {
        return 0;
    } else {
        ptr = (char*)buf;
    }

    while ((u32)ptr - (u32)buf < len) {
        if (*(s8*)ptr == 0xA) {
            *ptr = 0xD;
        }
        ptr++;
    }
    error = 0;
    cmd = 0xA0010000;

    while (len != 0) {
        qLen = QueueLength();
        if (qLen < 0) {
            error = 3;
            break;
        }
        
        if ((qLen >= 0xC) || (qLen >= len)) {
            if (EXISelect(Chan, Dev, EXI_FREQ_8M) == 0) {
                error = 3;
                break;
            }
            
            EXIImm(Chan, &cmd, sizeof(cmd), EXI_WRITE, 0);
            EXISync(Chan);

            while((qLen != 0) && (len != 0)) {
                if ((qLen < 4) && (qLen < len)) {
                    break;
                }

                xLen = len < 4 ? (long)len : 4;
                
                EXIImm(Chan, buf, xLen, EXI_WRITE, 0);
                (char*)buf += xLen;
                len -= xLen;
                qLen -= xLen;
                EXISync(Chan);
            }
            EXIDeselect(Chan);
        }
    }

    EXIUnlock(Chan);
    return error;
}
