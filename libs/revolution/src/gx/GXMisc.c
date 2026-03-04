#include <cstddef>
#include <revolution/base/PPCArch.h>
#include <revolution/gx.h>
#include <revolution/os.h>

#include "__gx.h"

static GXDrawSyncCallback TokenCB;
static GXDrawDoneCallback DrawDoneCB;
static u8 DrawDone;
static OSThreadQueue FinishQueue;

void GXSetMisc(GXMiscToken token, u32 val) {
    switch (token) {
    case GX_MT_XF_FLUSH:
        __GXData->vNum = val;
        __GXData->vNumNot = !__GXData->vNum;
        __GXData->bpSentNot = 1;

        if (__GXData->vNum != 0) {
            __GXData->dirtyState |= 8;
        }
        break;
    case GX_MT_DL_SAVE_CONTEXT:
        ASSERTMSGLINE(235, !__GXData->inDispList, "GXSetMisc: Cannot change DL context setting while making a display list");
        __GXData->dlSaveContext = (val != 0);
        break;
    case GX_MT_ABORT_WAIT_COPYOUT:
        __GXData->abtWaitPECopy = (val != 0);
        break;
    case GX_MT_NULL:
        break;
    default:
#if DEBUG
        OSReport("GXSetMisc: bad token %d (val %d)\n", token, val);
#endif
        break;
    }
}

void GXFlush(void) {
    CHECK_GXBEGIN(284, "GXFlush");
    if (__GXData->dirtyState) {
        __GXSetDirtyState();
    }
    
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);
    GX_WRITE_U32(0);

    PPCSync();
}

void GXResetWriteGatherPipe(void) {
    while (PPCMfwpar() & 1) {
    }
    PPCMtwpar(OSUncachedToPhysical((void*)GXFIFO_ADDR));
}

static void __GXAbortWait(u32 clocks) {
    OSTime time0;
    OSTime time1;

    time0 = OSGetTime();
    do {
        time1 = OSGetTime();
    } while (time1 - time0 <= (clocks / 4));
}

static void __GXAbortWaitPECopyDone(void) {
    u32 peCnt0;
    u32 peCnt1;

    peCnt0 = __GXReadMEMCounterU32(0x28, 0x27);
    do {
        peCnt1 = peCnt0;
        __GXAbortWait(32);

        peCnt0 = __GXReadMEMCounterU32(0x28, 0x27);
    } while (peCnt0 != peCnt1);
}

void __GXAbort(void) {
    if (__GXData->abtWaitPECopy && __GXIsGPFifoReady()) {
        __GXAbortWaitPECopyDone();
    }

    __PIRegs[0x18 / 4] = 1;
    __GXAbortWait(200);
    __PIRegs[0x18 / 4] = 0;
    __GXAbortWait(20);
}

void GXAbortFrame(void) {
    __GXAbort();

    if (__GXIsGPFifoReady()) {
        __GXCleanGPFifo();
        __GXInitRevisionBits();
        __GXData->dirtyState = 0;
        GXFlush();
    }
}

void GXSetDrawSync(u16 token) {
    BOOL enabled;
    u32 reg;

    CHECK_GXBEGIN(489, "GXSetDrawSync");

    enabled = OSDisableInterrupts();
    reg = token | 0x48000000;
    GX_WRITE_RA_REG(reg);
    SC_PE_TOKEN_SET_TOKEN(443, reg, token);
    SC_PE_TOKEN_SET_RID(443, reg, 0x47);
    GX_WRITE_RA_REG(reg);
    GXFlush();
    OSRestoreInterrupts(enabled);
    __GXData->bpSentNot = 0;
}

u16 GXReadDrawSync(void) {
    u16 token = GX_GET_PE_REG(7);
    return token;
}

void GXSetDrawDone(void) {
    u32 reg;
    BOOL enabled;

    CHECK_GXBEGIN(489, "GXSetDrawDone");
    enabled = OSDisableInterrupts();
    reg = 0x45000002;
    GX_WRITE_RA_REG(reg);
    GXFlush();
    DrawDone = GX_FALSE;
    OSRestoreInterrupts(enabled);
    __GXData->bpSentNot = GX_FALSE;
}

void GXWaitDrawDone(void) {
    BOOL enabled;

    CHECK_GXBEGIN(527, "GXWaitDrawDone");

    enabled = OSDisableInterrupts();
    while (!DrawDone) {
        OSSleepThread(&FinishQueue);
    }
    OSRestoreInterrupts(enabled);
}

void GXDrawDone(void) {
    CHECK_GXBEGIN(558, "GXDrawDone");
    GXSetDrawDone();
    GXWaitDrawDone();
}

void GXPixModeSync(void) {
    CHECK_GXBEGIN(580, "GXPixModeSync");
    GX_WRITE_RA_REG(__GXData->peCtrl);
    __GXData->bpSentNot = 0;
}

void GXTexModeSync(void) {
    u32 reg;

    CHECK_GXBEGIN(625, "GXTexModeSync");
    reg = 0x63000000;
    GX_WRITE_RA_REG(reg);
    __GXData->bpSentNot = 0;
}

#if DEBUG
void __GXBypass(u32 reg) {
    CHECK_GXBEGIN(647, "__GXBypass");
    GX_WRITE_RA_REG(reg);
    __GXData->bpSentNot = 0;
}

u16 __GXReadPEReg(u32 reg) {
    return GX_GET_PE_REG(reg);
}
#endif

void GXPokeAlphaMode(GXCompare func, u8 threshold) {
    u32 reg;

    reg = (func << 8) | threshold;
    GX_SET_PE_REG(3, reg);
}

void GXPokeAlphaRead(GXAlphaReadMode mode) {
    u32 reg;

    reg = 0;
    SC_PE_PI_CTL_SET_AFMT(672, reg, mode);
    SC_PE_PI_CTL_SET_ZFMT(672, reg, 1);
    GX_SET_PE_REG(4, reg);
}

void GXPokeAlphaUpdate(GXBool update_enable) {
    u32 reg;

    reg = GX_GET_PE_REG(1);
    SC_PE_CMODE0_SET_ALPHA_MASK(683, reg, update_enable);
    GX_SET_PE_REG(1, reg);
}

void GXPokeBlendMode(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp op) {
    u32 reg;

    reg = GX_GET_PE_REG(1);
    SC_PE_CMODE0_SET_BLEND_ENABLE(699, reg, ((type == GX_BM_BLEND) || (type == GX_BM_SUBTRACT)));
    SC_PE_CMODE0_SET_BLENDOP(700, reg, (type == GX_BM_SUBTRACT));
    SC_PE_CMODE0_SET_LOGICOP_ENABLE(702, reg, (type == GX_BM_LOGIC));
    SC_PE_CMODE0_SET_LOGICOP(703, reg, op);
    SC_PE_CMODE0_SET_SFACTOR(704, reg, src_factor);
    SC_PE_CMODE0_SET_DFACTOR(705, reg, dst_factor);
    SC_PE_CMODE0_SET_RID(706, reg, 0x41);
    GX_SET_PE_REG(1, reg);
}

void GXPokeColorUpdate(GXBool update_enable) {
    u32 reg;

    reg = GX_GET_PE_REG(1);
    SC_PE_CMODE0_SET_COLOR_MASK(717, reg, update_enable);
    GX_SET_PE_REG(1, reg);
}

void GXPokeDstAlpha(GXBool enable, u8 alpha) {
    u32 reg = 0;

    SC_PE_CMODE1_SET_CONSTANT_ALPHA(726, reg, alpha);
    SC_PE_CMODE1_SET_CONSTANT_ALPHA_ENABLE(727, reg, enable);
    GX_SET_PE_REG(2, reg);
}

void GXPokeDither(GXBool dither) {
    u32 reg;

    reg = GX_GET_PE_REG(1);
    SC_PE_CMODE0_SET_DITHER_ENABLE(737, reg, dither);
    GX_SET_PE_REG(1, reg);
}

void GXPokeZMode(GXBool compare_enable, GXCompare func, GXBool update_enable) {
    u32 reg = 0;

    SC_PE_ZMODE_SET_ENABLE(746, reg, compare_enable);
    SC_PE_ZMODE_SET_FUNC(747, reg, func);
    SC_PE_ZMODE_SET_MASK(748, reg, update_enable);
    GX_SET_PE_REG(0, reg);
}

void GXPeekARGB(u16 x, u16 y, u32* color) {
    u32 addr = (u32)OSPhysicalToUncached(0x08000000);

    SC_PE_PI_EFB_ADDR_SET_X(771, addr, x);
    SC_PE_PI_EFB_ADDR_SET_Y(772, addr, y);
    SC_PE_PI_EFB_ADDR_SET_TYPE(773, addr,  0);
    *color = *(u32*)addr;
}

void GXPokeARGB(u16 x, u16 y, u32 color) {
    u32 addr = (u32)OSPhysicalToUncached(0x08000000);

    SC_PE_PI_EFB_ADDR_SET_X(771, addr, x);
    SC_PE_PI_EFB_ADDR_SET_Y(772, addr, y);
    SC_PE_PI_EFB_ADDR_SET_TYPE(773, addr,  0);
    *(u32*)addr = color;
}

void GXPeekZ(u16 x, u16 y, u32* z) {
    u32 addr = (u32)OSPhysicalToUncached(0x08000000);

    SC_PE_PI_EFB_ADDR_SET_X(791, addr, x);
    SC_PE_PI_EFB_ADDR_SET_Y(792, addr, y);
    SC_PE_PI_EFB_ADDR_SET_TYPE(793, addr,  1);
    *z = *(u32*)addr;
}

void GXPokeZ(u16 x, u16 y, u32 z) {
    u32 addr = (u32)OSPhysicalToUncached(0x08000000);

    SC_PE_PI_EFB_ADDR_SET_X(791, addr, x);
    SC_PE_PI_EFB_ADDR_SET_Y(792, addr, y);
    SC_PE_PI_EFB_ADDR_SET_TYPE(793, addr,  1);
    *(u32*)addr = z;
}

GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback cb) {
    GXDrawSyncCallback oldcb;
    BOOL enabled;

    oldcb = TokenCB;
    enabled = OSDisableInterrupts();
    TokenCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

static void GXTokenInterruptHandler(__OSInterrupt interrupt, OSContext* context) {
    u16 token;
    OSContext exceptionContext;
    u32 reg;

    token = GX_GET_PE_REG(7);
    if (TokenCB != NULL) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        TokenCB(token);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
    reg = GX_GET_PE_REG(5);
    SC_PE_INTRCTL_SET_INT0CLR(0, reg, 1);
    GX_SET_PE_REG(5, reg);
}

GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback cb) {
    GXDrawDoneCallback oldcb;
    BOOL enabled;

    oldcb = DrawDoneCB;
    enabled = OSDisableInterrupts();
    DrawDoneCB = cb;
    OSRestoreInterrupts(enabled);
    return oldcb;
}

static void GXFinishInterruptHandler(__OSInterrupt interrupt, OSContext* context) {
    OSContext exceptionContext;
    u32 reg = 0;

    reg = GX_GET_PE_REG(5);
    SC_PE_INTRCTL_SET_INT1CLR(0, reg, 1);
    GX_SET_PE_REG(5, reg);
    DrawDone = 1;
    if (DrawDoneCB != NULL) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        DrawDoneCB();
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
    OSWakeupThread(&FinishQueue);
}

void __GXPEInit(void) {
    u32 reg;
    __OSSetInterruptHandler(0x12, GXTokenInterruptHandler);
    __OSSetInterruptHandler(0x13, GXFinishInterruptHandler);
    OSInitThreadQueue(&FinishQueue);
    __OSUnmaskInterrupts(0x2000);
    __OSUnmaskInterrupts(0x1000);
    reg = GX_GET_PE_REG(5);
    SC_PE_INTRCTL_SET_INT0CLR(0, reg, 1);
    SC_PE_INTRCTL_SET_INT1CLR(0, reg, 1);
    SC_PE_INTRCTL_SET_INT0EN(0, reg, 1);
    SC_PE_INTRCTL_SET_INT1EN(0, reg, 1);
    GX_SET_PE_REG(5, reg);
}

u32 GXCompressZ16(u32 z24, GXZFmt16 zfmt) {
    u32 z16;
    u32 z24n;
    s32 exp;
    s32 shift;
#if DEBUG
#define temp exp
#else
    s32 temp;
    u8 unused[4];
#endif

    z24n = ~(z24 << 8);
    temp = __cntlzw(z24n);
    switch (zfmt) {
    case GX_ZC_LINEAR:
        z16 = (z24 >> 8) & 0xFFFF;
        break;
    case GX_ZC_NEAR:
        if (temp > 3) {
            exp = 3;
        } else {
            exp = temp;
        }
        if (exp == 3) {
            shift = 7;
        } else {
            shift = 9 - exp;
        }
        z16 = ((z24 >> shift) & 0x3FFF & ~0xFFFFC000) | (exp << 14);
        break;
    case GX_ZC_MID:
        if (temp > 7) {
            exp = 7;
        } else {
            exp = temp;
        }
        if (exp == 7) {
            shift = 4;
        } else {
            shift = 10 - exp;
        }
        z16 = ((z24 >> shift) & 0x1FFF & ~0xFFFFE000) | (exp << 13);
        break;
    case GX_ZC_FAR:
        if (temp > 12) {
            exp = 12;
        } else {
            exp = temp;
        }
        if (exp == 12) {
            shift = 0;
        } else {
            shift = 11 - exp;
        }
        z16 = ((z24 >> shift) & 0xFFF & ~0xFFFFF000) | (exp << 12);
        break;
    default:
        OSPanic(__FILE__, 1004, "GXCompressZ16: Invalid Z format\n");
        break;
    }
    return z16;
}

u32 GXDecompressZ16(u32 z16, GXZFmt16 zfmt) {
    u32 z24;
    u32 cb1;
    s32 exp;
    s32 shift;

    cb1; cb1; cb1; z16; z16; z16;  // needed to match

    switch (zfmt) {
    case GX_ZC_LINEAR:
        z24 = (z16 << 8) & 0xFFFF00;
        break;
    case GX_ZC_NEAR:
        exp = (z16 >> 14) & 3;
        if (exp == 3) {
            shift = 7;
        } else {
            shift = 9 - exp;
        }
        cb1 = -1 << (24 - exp);
        z24 = (cb1 | ((z16 & 0x3FFF) << shift)) & 0xFFFFFF;
        break;
    case GX_ZC_MID:
        exp = (z16 >> 13) & 7;
        if (exp == 7) {
            shift = 4;
        } else {
            shift = 10 - exp;
        }
        cb1 = -1 << (24 - exp);
        z24 = (cb1 | ((z16 & 0x1FFF) << shift)) & 0xFFFFFF;
        break;
    case GX_ZC_FAR:
        exp = (z16 >> 12) & 0xF;
        if (exp == 12) {
            shift = 0;
        } else {
            shift = 11 - exp;
        }
        cb1 = -1 << (24 - exp);
        z24 = (cb1 | ((z16 & 0xFFF) << shift)) & 0xFFFFFF;
        break;
    default:
        OSPanic(__FILE__, 1054, "GXDecompressZ16: Invalid Z format\n");
        break;
    }
    return z24;
}
