#include <dolphin/si.h>
#include <dolphin/vi.h>

#include "__os.h"

#define ROUND(n, a) (((u32)(n) + (a)-1) & ~((a)-1))

#if DEBUG
const char* __SIVersion = "<< Dolphin SDK - SI\tdebug build: Apr  5 2004 03:55:31 (0x2301) >>";
#else
const char* __SIVersion = "<< Dolphin SDK - SI\trelease build: Apr  5 2004 04:14:16 (0x2301) >>";
#endif

static SIControl Si = {
    /* chan */       -1,
    /* poll */        0,
    /* inputBytes */  0,
    /* input */       NULL,
    /* callback */    NULL
};

static SIPacket Packet[4];
static OSAlarm Alarm[4];
static u32 Type[4] = { SI_ERROR_NO_RESPONSE, SI_ERROR_NO_RESPONSE, SI_ERROR_NO_RESPONSE, SI_ERROR_NO_RESPONSE };
static OSTime TypeTime[4];
static OSTime XferTime[4];
static SITypeCallback TypeCallback[4][4];
static __OSInterruptHandler RDSTHandler[4];
static BOOL InputBufferValid[4];
static u32 InputBuffer[4][2];
static volatile u32 InputBufferVcount[4];

u32 __PADFixBits;

// prototypes
static u32 CompleteTransfer();
static void SITransferNext(s32 chan);
static void SIInterruptHandler(__OSInterrupt interrupt, OSContext* context);
static int __SITransfer(s32 chan, void* output, u32 outputBytes, void* input, u32 inputBytes, SICallback callback);
static void AlarmHandler(OSAlarm* alarm, OSContext* context);
static void GetTypeCallback(s32 chan, u32 error, OSContext* context);
static int SIGetResponseRaw(s32 chan);

BOOL SIBusy(void) {
    return (Si.chan != -1) ? TRUE : FALSE;
}

BOOL SIIsChanBusy(s32 chan) {
    return Packet[chan].chan != -1 || Si.chan == chan;
}

static void SIClearTCInterrupt(void) {
    u32 reg;

    reg = __SIRegs[SI_COMCSR_IDX];
    reg |= SI_COMCSR_TCINT_MASK;
    reg &= ~SI_COMCSR_TSTART_MASK;
    __SIRegs[SI_COMCSR_IDX] = reg;
}

static u32 CompleteTransfer(void) {
    u32 sr;
    u32 i;
    u32 rLen;
    u8* input;
    u32 temp;

    sr = __SIRegs[SI_STATUS_IDX];
    SIClearTCInterrupt();

    if (Si.chan != -1) {
        XferTime[Si.chan] = __OSGetSystemTime();
        input = Si.input;
        rLen = Si.inputBytes / sizeof(u32);
        for (i = 0; i < rLen; i++) {
            *((u32*)input)++ = __SIRegs[i+0x20];
        }
        
        rLen = Si.inputBytes & 3;
        if (rLen != 0) {
            temp = __SIRegs[i + 32];
            for (i = 0; i < rLen; i++) {
                *(input++) = temp >> ((3 - i) * 8);
            }
        }

        if (__SIRegs[SI_COMCSR_IDX] & SI_COMCSR_COMERR_MASK) {
            sr >>= (3 - Si.chan) * 8;
            sr &= 0xF;
            if ((sr & 8) != 0 && (Type[Si.chan] & 0x80) == 0) {
                Type[Si.chan] = 8;
            }

            if (sr == 0) {
                sr = 4;
            }
        } else {
            TypeTime[Si.chan] = __OSGetSystemTime();
            sr = 0;
        }

        Si.chan = -1;
    }

    return sr;
}

static void SITransferNext(s32 chan) {
    int i;
    SIPacket* packet;

    for (i = 0; i < 4; i++) {
        chan++;
        chan %= 4;
        packet = &Packet[chan];

        if (packet->chan != -1) {
            if (packet->fire <= __OSGetSystemTime()) {
                if (__SITransfer(packet->chan, packet->output, packet->outputBytes, packet->input, packet->inputBytes, packet->callback) != 0) {
                    OSCancelAlarm(&Alarm[chan]);
                    packet->chan = -1;
                }
                return;
            }
        }
    }
}

#define CHAN_NONE -1

static void SIInterruptHandler(__OSInterrupt interrupt, OSContext* context) {
    u32 reg;
    s32 chan;
    u32 sr;
    SICallback callback;
    int i;
    u32 vcount;
    u32 x;

    reg = __SIRegs[SI_COMCSR_IDX];
    if ((reg & (SI_COMCSR_TCINT_MASK | SI_COMCSR_TCINTMSK_MASK)) == (SI_COMCSR_TCINT_MASK | SI_COMCSR_TCINTMSK_MASK)) {
        ASSERTLINE(376, Si.chan != CHAN_NONE);

        chan = Si.chan;
        sr = CompleteTransfer();
        callback = Si.callback;
        Si.callback = NULL;
        SITransferNext(chan);

        if (callback) {
            callback(chan, sr, context);
        }

        sr = __SIRegs[SI_STATUS_IDX];
        sr &= 0x0F000000 >> (chan << 3);
        __SIRegs[SI_STATUS_IDX] = sr;

        if (Type[chan] == SI_ERROR_BUSY && !SIIsChanBusy(chan)) {
            static u32 cmdTypeAndStatus;

            SITransfer(chan, &cmdTypeAndStatus, 1, &Type[chan], 3, &GetTypeCallback, OSMicrosecondsToTicks(65));
        }
    }

    if ((reg & (SI_COMCSR_RDSTINT_MASK | SI_COMCSR_RDSTINTMSK_MASK)) == (SI_COMCSR_RDSTINT_MASK | SI_COMCSR_RDSTINTMSK_MASK)) {
        vcount = 1 + VIGetCurrentLine();
        x = (Si.poll & (0x3FF << 16)) >> 16;

        for (i = 0; i < 4; i++) {
            if (SIGetResponseRaw(i)) {
                InputBufferVcount[i] = vcount;
            }
        }

        for (i = 0; i < 4; i++) {
            if ((Si.poll & (0x80000000 >> (24 + i))) != 0) {
                if (InputBufferVcount[i] == 0 || ((x >> 1) + InputBufferVcount[i]) < vcount) {
                    return;
                }
            }
        }

        for (i = 0; i < 4; i++) {
            InputBufferVcount[i] = 0;
        }

        for (i = 0; i < 4; i++) {
            if (RDSTHandler[i] != 0) {
                (*RDSTHandler[i])(interrupt, context);
            }
        }
    }
}

static BOOL SIEnablePollingInterrupt(BOOL enable) {
    BOOL enabled;
    BOOL rc;
    u32 reg;
    int i;

    enabled = OSDisableInterrupts();
    reg = __SIRegs[SI_COMCSR_IDX];
    rc = ((reg & SI_COMCSR_RDSTINTMSK_MASK) != 0) ? TRUE : FALSE;

    if (enable) {
        reg |= SI_COMCSR_RDSTINTMSK_MASK;
        
        for (i = 0; i < 4; i++) {
            InputBufferVcount[i] = 0;
        }
    } else {
        reg &= ~SI_COMCSR_RDSTINTMSK_MASK;
    }

    reg &= ~(SI_COMCSR_TCINT_MASK | SI_COMCSR_TSTART_MASK);
    __SIRegs[SI_COMCSR_IDX] = reg;

    OSRestoreInterrupts(enabled);
    return rc;
}

BOOL SIRegisterPollingHandler(__OSInterruptHandler handler) {
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();
    for (i = 0; i < 4; i++) {
        if (RDSTHandler[i] == handler) {
            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    for (i = 0; i < 4; i++) {
        if (RDSTHandler[i] == 0) {
            RDSTHandler[i] = handler;
            SIEnablePollingInterrupt(TRUE);
            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }

    OSRestoreInterrupts(enabled);
    return FALSE;
}

BOOL SIUnregisterPollingHandler(__OSInterruptHandler handler) {
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();
    for (i = 0; i < 4; i++) {
        if (RDSTHandler[i] == handler) {
            RDSTHandler[i] = 0;
            
            for (i = 0; i < 4; i++) {
                if (RDSTHandler[i] != 0) {
                    break;
                }
            }

            if (i == 4) {
                SIEnablePollingInterrupt(FALSE);
            }

            OSRestoreInterrupts(enabled);
            return TRUE;
        }
    }    

    OSRestoreInterrupts(enabled);
    return FALSE;
}

void SIInit(void) {
    OSRegisterVersion(__SIVersion);

    Packet[0].chan = Packet[1].chan = Packet[2].chan = Packet[3].chan = -1;
    Si.poll = 0;
    SISetSamplingRate(0);

    do {} while(__SIRegs[SI_COMCSR_IDX] & SI_COMCSR_TSTART_MASK);

    __SIRegs[SI_COMCSR_IDX] = SI_COMCSR_TCINT_MASK;
    __OSSetInterruptHandler(0x14, SIInterruptHandler);
    __OSUnmaskInterrupts(0x800);

    SIGetType(0);
    SIGetType(1);
    SIGetType(2);
    SIGetType(3);
}

static int __SITransfer(s32 chan, void* output, u32 outputBytes, void* input, u32 inputBytes, SICallback callback) {
    BOOL enabled;
    u32 rLen;
    u32 i;
    u32 sr;
    union {
        u32 val;
        struct {
            u32 tcint : 1;
            u32 tcintmsk : 1;
            u32 comerr : 1;
            u32 rdstint : 1;
            u32 rdstintmsk : 1;
            u32 pad2 : 4;
            u32 outlngth : 7;
            u32 pad1 : 1;
            u32 inlngth : 7;
            u32 pad0 : 5;
            u32 channel : 2;
            u32 tstart : 1;
        } f;
    } comcsr;

    ASSERTMSGLINE(627, (chan >= 0) && (chan < 4), "SITransfer(): invalid channel.");
    ASSERTMSGLINE(629, (outputBytes != 0) && (outputBytes <= 128), "SITransfer(): output size is out of range (must be 1 to 128).");
    ASSERTMSGLINE(631, (inputBytes != 0) && (inputBytes <= 128), "SITransfer(): input size is out of range (must be 1 to 128).");

    enabled = OSDisableInterrupts();
    if (Si.chan != -1) {
        OSRestoreInterrupts(enabled);
        return 0;
    }

    ASSERTLINE(641, (__SIRegs[SI_COMCSR_IDX] & (SI_COMCSR_TSTART_MASK | SI_COMCSR_TCINT_MASK)) == 0);
    sr = __SIRegs[SI_STATUS_IDX];
    sr &= (0x0F000000 >> (chan* 8));
    __SIRegs[SI_STATUS_IDX] = sr;
    
    Si.chan = chan;
    Si.callback = callback;
    Si.inputBytes = inputBytes;
    Si.input = input;

    rLen = ROUND(outputBytes, 4) / 4;
    for (i = 0; i < rLen; i++) {
        __SIRegs[i + 0x20] = ((u32*)output)[i];
    }
    
    comcsr.val = __SIRegs[SI_COMCSR_IDX];
    comcsr.f.tcint = 1;
    comcsr.f.tcintmsk = callback ? 1 : 0;
    comcsr.f.outlngth = outputBytes == 0x80 ? 0 : outputBytes;
    comcsr.f.inlngth = inputBytes == 0x80 ? 0 : inputBytes;
    comcsr.f.channel = chan;
    comcsr.f.tstart = 1;

    __SIRegs[SI_COMCSR_IDX] = comcsr.val;
    OSRestoreInterrupts(enabled);
    return 1;
}

u32 SISync(void) {
    BOOL enabled;
    u32 sr;

    do {} while(__SIRegs[SI_COMCSR_IDX] & SI_COMCSR_TSTART_MASK);

    enabled = OSDisableInterrupts();
    sr = CompleteTransfer();
    SITransferNext(4);
    OSRestoreInterrupts(enabled);
    return sr;
}

u32 SIGetStatus(s32 chan) {
    BOOL enabled;
    u32 sr;
    int chanShift;

    enabled = OSDisableInterrupts();
    sr = __SIRegs[SI_STATUS_IDX];
    chanShift = (3 - chan) * 8;
    sr >>= chanShift;

    if ((sr & 8) != 0) {
        if ((Type[chan] & SI_ERROR_BUSY) == 0) {
            Type[chan] = 8;
        }
    }

    OSRestoreInterrupts(enabled);
    return sr;
}

void SISetCommand(s32 chan, u32 command) {
    ASSERTMSGLINE(752, (chan >= 0) && (chan < 4), "SISetCommand(): invalid channel.");
    __SIRegs[chan* 3] = command;
}

u32 SIGetCommand(s32 chan) {
    ASSERTMSGLINE(770, (chan >= 0) && (chan < 4), "SIGetCommand(): invalid channel.");
    return __SIRegs[chan* 3];
}

void SITransferCommands(void) {
    __SIRegs[SI_STATUS_IDX] = SI_COMCSR_TCINT_MASK;
}

u32 SISetXY(u32 x, u32 y) {
    u32 poll;
    BOOL enabled;

    ASSERTMSGLINE(803, x >= 8, "SISetXY(): x is out of range (8 <= x <= 1023).");
    ASSERTMSGLINE(804, x <= 1023, "SISetXY(): x is out of range (8 <= x <= 1023).");
    ASSERTMSGLINE(805, y <= 255, "SISetXY(): y is out of range (0 <= y <= 255).");

    poll = x << 0x10;
    poll |= y << 8;
    enabled = OSDisableInterrupts();
    Si.poll &= 0xFC0000FF;
    Si.poll |= poll;
    poll = Si.poll;
    __SIRegs[0x30 / 4] = poll;
    OSRestoreInterrupts(enabled);
    return poll;
}

u32 SIEnablePolling(u32 poll) {
    BOOL enabled;
    u32 en;

    ASSERTMSGLINE(834, (poll & 0x0FFFFFFF) == 0, "SIEnablePolling(): invalid chan bit(s).");
    if (poll == 0) {
        return Si.poll;
    }
    
    enabled = OSDisableInterrupts();
    poll = poll >> 24;
    en = poll & 0xF0;
    ASSERTLINE(865, en);
    poll &= ((en >> 4) | 0x03FFFFF0);
    poll &= 0xFC0000FF;
    
    Si.poll &= ~(en >> 4);
    Si.poll |= poll;
    poll = Si.poll;
    SITransferCommands();
    __SIRegs[0x30 / 4] = poll;
    OSRestoreInterrupts(enabled);
    return poll;
}

u32 SIDisablePolling(u32 poll) {
    BOOL enabled;

    ASSERTMSGLINE(908, (poll & 0x0FFFFFFF) == 0, "SIDisablePolling(): invalid chan bit(s).");
    if (poll == 0) {
        return Si.poll;
    }

    enabled = OSDisableInterrupts();
    poll = poll >> 24;
    poll &= 0xF0;
    ASSERTLINE(921, poll);
    poll = Si.poll & ~poll;
    __SIRegs[0x30 / 4] = poll;
    Si.poll = poll;
    OSRestoreInterrupts(enabled);
    return poll;
}

static BOOL SIGetResponseRaw(s32 chan) {
    u32 sr;

    sr = SIGetStatus(chan);
    if (sr & 0x20) {
        InputBuffer[chan][0] = __SIRegs[1 + chan * 3];
        InputBuffer[chan][1] = __SIRegs[2 + chan * 3];
        InputBufferValid[chan] = TRUE;
        return TRUE;
    }

    return FALSE;
}

BOOL SIGetResponse(s32 chan, void* data) {
    BOOL rc;
    BOOL enabled;

    ASSERTMSGLINE(971, ((chan >= 0) && (chan < 4)), "SIGetResponse(): invalid channel.");
    enabled = OSDisableInterrupts();
    SIGetResponseRaw(chan);
    rc = InputBufferValid[chan];
    InputBufferValid[chan] = FALSE;
    
    if (rc) {
        ((u32*)data)[0] = InputBuffer[chan][0];
        ((u32*)data)[1] = InputBuffer[chan][1];
    }

    OSRestoreInterrupts(enabled);
    return rc;
}

static void AlarmHandler(OSAlarm* alarm, OSContext* context) {
    s32 chan;
    SIPacket* packet;

    chan = (s32)(alarm - Alarm);
    ASSERTLINE(1002, 0 <= chan && chan < SI_MAX_CHAN);

    packet = &Packet[chan];
    if (packet->chan != -1) {
        ASSERTLINE(1006, packet->fire <= __OSGetSystemTime());

        if (__SITransfer(packet->chan, packet->output, packet->outputBytes, packet->input, packet->inputBytes, packet->callback)) {
            packet->chan = -1;
        }
    }
}

BOOL SITransfer(s32 chan, void* output, u32 outputBytes, void* input, u32 inputBytes, 
                SICallback callback, OSTime delay) {
    BOOL enabled;
    SIPacket* packet;
    OSTime now;
    OSTime fire;

    packet = &Packet[chan];
    enabled = OSDisableInterrupts();

    if (packet->chan != -1 || Si.chan == chan) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    now = __OSGetSystemTime();
    if (delay == 0) {
        fire = now;
    } else {
        fire = delay + XferTime[chan];
    }

    if (now < fire) {
        delay = fire - now;
        OSSetAlarm(&Alarm[chan], delay, AlarmHandler);
    } else if (__SITransfer(chan, output, outputBytes, input, inputBytes, callback)) {
        OSRestoreInterrupts(enabled);
        return TRUE;
    }

    packet->chan = chan;
    packet->output = output;
    packet->outputBytes = outputBytes;
    packet->input = input;
    packet->inputBytes = inputBytes;
    packet->callback = callback;
    packet->fire = fire;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

static void CallTypeAndStatusCallback(s32 chan, u32 type) {
    SITypeCallback callback;
    int i;

    for (i = 0; i < 4; i++) {
        callback = TypeCallback[chan][i];
        
        if (callback != 0) {
            TypeCallback[chan][i] = 0;
            (*callback)(chan, type);
        }
    }
}

static void GetTypeCallback(s32 chan, u32 error, OSContext* context) {
    u32 type;
    u32 chanBit;
    int fix;
    u32 id;
    
    ASSERTLINE(1137, 0 <= chan && chan < SI_MAX_CHAN);
    
    ASSERTLINE(1139, (Type[chan] & 0xff) == SI_ERROR_BUSY);
    Type[chan] &= ~SI_ERROR_BUSY;
    Type[chan] |= error;
    TypeTime[chan] = __OSGetSystemTime();

    type = Type[chan];
    chanBit = 0x80000000 >> chan;
    fix = __PADFixBits & chanBit;
    __PADFixBits &= ~chanBit;

    if ((error & 0xF) != 0 || (type & 0x18000000) != 0x08000000 || (type & 0x80000000) == 0 || (type & 0x04000000) != 0) {
        OSSetWirelessID(chan, 0);
        CallTypeAndStatusCallback(chan, Type[chan]);
    } else {
        static u32 cmdFixDevice[4];

        id = OSGetWirelessID(chan) << 8;
        
        if (fix != 0 && (id & 0x100000) != 0) {
            cmdFixDevice[chan] = 0x4E000000 | (id & 0xCFFF00) | 0x100000;
            Type[chan] = SI_ERROR_BUSY;
            SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3, &GetTypeCallback, 0);
            return;
        }

        if ((type & 0x00100000) != 0) {
            if ((id & 0xCFFF00) != (type & 0xCFFF00)) {
                if ((id & 0x100000) == 0) {
                    id = type & 0xCFFF00;
                    id |= 0x100000;
                    OSSetWirelessID(chan, id >> 8);
                }

                cmdFixDevice[chan] = 0x4E000000 | id;
                Type[chan] = SI_ERROR_BUSY;
                SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3, &GetTypeCallback, 0);
                return;
            }
        } else {
            if ((type & 0x40000000) != 0) {
                id = type & 0xCFFF00;
                id |= 0x100000;
                OSSetWirelessID(chan, id >> 8);

                cmdFixDevice[chan] = 0x4E000000 | id;
                Type[chan] = SI_ERROR_BUSY;
                SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3, &GetTypeCallback, 0);
                return;
            }

            OSSetWirelessID(chan, 0);
        }

        CallTypeAndStatusCallback(chan, Type[chan]);
    }
}

u32 SIGetType(s32 chan) {
    static u32 cmdTypeAndStatus;
    BOOL enabled;
    u32 type;
    OSTime diff;

    enabled = OSDisableInterrupts();
    ASSERTLINE(1243, 0 <= chan && chan < SI_MAX_CHAN);
    type = Type[chan];
    diff = __OSGetSystemTime() - TypeTime[chan];
    if ((Si.poll & (0x80 >> chan)) != 0) {
        if (type != 8) {
            TypeTime[chan] = __OSGetSystemTime();
            OSRestoreInterrupts(enabled);
            return type;
        }

        type = Type[chan] = SI_ERROR_BUSY;
    } else {
        if (diff <= OSMillisecondsToTicks(50) && type != 8) {
            OSRestoreInterrupts(enabled);
            return type;
        }

        if (diff <= OSMillisecondsToTicks(75)) {
            Type[chan] = SI_ERROR_BUSY;
        } else {
            type = Type[chan] = SI_ERROR_BUSY;
        }
    }

    TypeTime[chan] = __OSGetSystemTime();
    SITransfer(chan, &cmdTypeAndStatus, 1, &Type[chan], 3, &GetTypeCallback, OSMicrosecondsToTicks(65));
    OSRestoreInterrupts(enabled);
    return type;
}

u32 SIGetTypeAsync(s32 chan, SITypeCallback callback) {
    BOOL enabled;
    u32 type;
    int i;

    enabled = OSDisableInterrupts();
    type = SIGetType(chan);

    if ((Type[chan] & SI_ERROR_BUSY) != 0) {
        for (i = 0; i < SI_MAX_TYPE; i++) {
            if (TypeCallback[chan][i] == callback) {
                break;
            }

            if (TypeCallback[chan][i] == 0) {
                TypeCallback[chan][i] = callback;
                break;
            }
        }

        ASSERTLINE(1324, i < SI_MAX_TYPE);
    } else {
        (*callback)(chan, type);
    }

    OSRestoreInterrupts(enabled);
    return type;
}

u32 SIDecodeType(u32 type) {
    u32 error;

    error = type & 0xFF;
    type &= ~0xFF;

    if (error & SI_ERROR_NO_RESPONSE) {
        return SI_ERROR_NO_RESPONSE;
    }

    if (error & (SI_ERROR_UNKNOWN | SI_ERROR_COLLISION | SI_ERROR_OVER_RUN | SI_ERROR_UNDER_RUN)) {
        return SI_ERROR_UNKNOWN;
    }

    if (error != 0) {
        ASSERTLINE(1371, error == SI_ERROR_BUSY);
        return SI_ERROR_BUSY;
    }

    if ((type & SI_TYPE_MASK) == SI_TYPE_N64) {
        switch (type & 0xFFFF0000) {
        case SI_N64_MIC:
        case SI_N64_KEYBOARD:
        case SI_GBA:
        case SI_N64_MOUSE:
        case SI_N64_CONTROLLER:
            return type & 0xFFFF0000;
        default:
            return SI_ERROR_UNKNOWN;
        }
    }

    if ((type & SI_TYPE_MASK) != SI_TYPE_DOLPHIN) {
        return SI_ERROR_UNKNOWN;
    }

    switch (type & 0xFFFF0000) {
    case SI_GC_STEERING:
    case SI_GC_CONTROLLER:
        return type & 0xFFFF0000;
    }

    if ((type & 0xFFE00000) == SI_GC_KEYBOARD) {
        return SI_GC_KEYBOARD;
    }

    if ((type & SI_GC_WIRELESS) != 0 && (type & SI_WIRELESS_IR) == 0) {
        if ((type & SI_GC_WAVEBIRD) == SI_GC_WAVEBIRD) {
            return SI_GC_WAVEBIRD;
        }

        if ((type & SI_WIRELESS_STATE) == 0) {
            return SI_GC_RECEIVER;
        }
    }

    if ((type & SI_GC_CONTROLLER) == SI_GC_CONTROLLER) {
        return SI_GC_CONTROLLER;
    }

    return SI_ERROR_UNKNOWN;
}

u32 SIProbe(s32 chan) {
    return SIDecodeType(SIGetType(chan));
}

char* SIGetTypeString(u32 type) {
    switch (SIDecodeType(type)) {
    case SI_ERROR_NO_RESPONSE:
        return "No response";
    case SI_ERROR_BUSY:
        return "Busy";
    case SI_N64_CONTROLLER:
        return "N64 controller";
    case SI_N64_MIC:
        return "N64 microphone";
    case SI_N64_KEYBOARD:
        return "N64 keyboard";
    case SI_N64_MOUSE:
        return "N64 mouse";
    case SI_GBA:
        return "GameBoy Advance";
    case SI_GC_CONTROLLER:
        return "Standard controller";
    case SI_GC_RECEIVER:
        return "Wireless receiver";
    case SI_GC_WAVEBIRD:
        return "WaveBird controller";
    case SI_GC_KEYBOARD:
        return "Keyboard";
    case SI_GC_STEERING:
        return "Steering";
    case SI_ERROR_UNKNOWN:
    default:
        return "Unknown";
    }
}
