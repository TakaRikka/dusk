#include <dolphin/dolphin.h>
#include <dolphin/ax.h>
#include "sdk_math.h"

#include "__ax.h"

static u32 __AXSrcCycles[5] = {
    0x00000DF8,
    0x00000F78,
    0x000014B8,
    0x000019F8,
    0x000019F8
};

static u32 __AXMainMixCycles[16] = {
    0x00000000, 0x000002F8, 0x000002F8,
	0x000005BE, 0x000002F8, 0x000005F0,
	0x000005F0, 0x000008B6, 0x00000000,
	0x000004F1, 0x000004F1, 0x000009A6,
	0x000004F1, 0x000009E2, 0x000009E2,
	0x00000E97
};

static u32 __AXAuxMixCycles[32] = {
    0x00000000, 0x000002F8, 0x000002F8,
	0x000005BE, 0x00000000, 0x000004F1,
	0x000004F1, 0x000009A6, 0x000002F8,
	0x000005F0, 0x000005F0, 0x000008B6,
	0x000002F8, 0x000007E9, 0x000007E9,
	0x00000C9E, 0x00000000, 0x000002F8,
	0x000002F8, 0x000005BE, 0x00000000,
	0x000004F1, 0x000004F1, 0x000009A6,
	0x000004F1, 0x000007E9, 0x000007E9,
	0x00000AAF, 0x000004F1, 0x000009E2,
	0x000009E2, 0x00000E97
};

static AXPB __AXPB[AX_MAX_VOICES] ATTRIBUTE_ALIGN(32);
static AXPBITDBUFFER __AXITD[AX_MAX_VOICES] ATTRIBUTE_ALIGN(32);
static AXPBU __AXUpdates[AX_MAX_VOICES] ATTRIBUTE_ALIGN(32);
static AXVPB __AXVPB[AX_MAX_VOICES];

static u32 __AXMaxDspCycles;
static u32 __AXRecDspCycles;
static u32 __AXNumVoices;

u32 __AXGetNumVoices(void) {
    return __AXNumVoices;
}

void __AXServiceVPB(AXVPB* pvpb) {
    AXPB* ppbDsp;
    AXPB* ppbUser;
    u32 sync;

    ASSERTLINE(238, (pvpb->index >= 0) && (pvpb->index < AX_MAX_VOICES));

    __AXNumVoices += 1;
    ppbDsp = &__AXPB[pvpb->index];
    ppbUser = &pvpb->pb;
    sync = pvpb->sync;
    if (sync == 0) {
        ppbUser->state = ppbDsp->state;
        ppbUser->ve.currentVolume = ppbDsp->ve.currentVolume;
        ppbUser->addr.currentAddressHi = ppbDsp->addr.currentAddressHi;
        ppbUser->addr.currentAddressLo = ppbDsp->addr.currentAddressLo;
        return;
    }
    if (sync & AX_SYNC_FLAG_COPYALL) {
        // copy the whole PB struct. (size: 0xF4)
        u32* src;
        u32* dst;
        src = (void*)ppbUser;
        dst = (void*)ppbDsp;

        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src);

        if (pvpb->updateCounter != 0) {
            u32 count;
            src = (void*)&__AXUpdates[pvpb->index];
            dst = (void*)pvpb->updateData;
            for (count = pvpb->updateCounter; count; count--) {
                *(dst) = *(src); dst+=1; src+=1;
            }
        }
        return;
    }

    if (sync & AX_SYNC_FLAG_COPYSELECT) {
        ppbDsp->srcSelect = ppbUser->srcSelect;
        ppbDsp->coefSelect = ppbUser->coefSelect;
    }

    if (sync & AX_SYNC_FLAG_COPYMXRCTRL) {
        ppbDsp->mixerCtrl = ppbUser->mixerCtrl;
    }

    if (sync & AX_SYNC_FLAG_COPYSTATE) {
        ppbDsp->state = ppbUser->state;
    } else {
        ppbUser->state = ppbDsp->state;
    }

    if (sync & AX_SYNC_FLAG_COPYTYPE) {
        ppbDsp->type = ppbUser->type;
    }

    if (sync & AX_SYNC_FLAG_COPYAXPBMIX) {
        // copy AXPBMIX.
        u16* src;
        u16* dst;
        src = (void*)&ppbUser->mix;
        dst = (void*)&ppbDsp->mix;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src);
    }

    if (sync & AX_SYNC_FLAG_COPYTSHIFT) {
        ppbDsp->itd.targetShiftL = ppbUser->itd.targetShiftL;
        ppbDsp->itd.targetShiftR = ppbUser->itd.targetShiftR;
    } else if (sync & AX_SYNC_FLAG_COPYITD) {
        // copy ITD struct.
        u16* src;
        u16* dst;
        u32* dst_;
        src = (void*)&ppbUser->itd;
        dst = (void*)&ppbDsp->itd;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); src+=1;

        dst; // fixes reg alloc
        dst_ = pvpb->itdBuffer;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0; dst_+=1;
        *(dst_) = 0;
    }

    if (sync & AX_SYNC_FLAG_COPYUPDATE) {
        // copy UPDATE struct.
        u16* src;
        u16* dst;
        dst = (void*)&ppbDsp->update;
        src = (void*)&ppbUser->update;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src);

        if (pvpb->updateCounter) {
            u32* src_;
            u32* dst_;
            u32 count;

            dst_ = (void*)&__AXUpdates[pvpb->index];
            src_ = (void*)&pvpb->updateData;

            for (count = pvpb->updateCounter; count; count--) {
                *(dst_) = *(src_); dst_+=1; src_+=1;
            }
        }
    }

    if (sync & AX_SYNC_FLAG_COPYDPOP) {
        // copy DPOP struct.
        u16* src;
        u16* dst;
        dst = (void*)&ppbDsp->dpop;
        src = (void*)&ppbUser->dpop;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src);
    }

    if (sync & AX_SYNC_FLAG_SWAPVOL) {
        ppbUser->ve.currentVolume = ppbDsp->ve.currentVolume;
        ppbDsp->ve.currentDelta = ppbUser->ve.currentDelta;
    } else if (sync & AX_SYNC_FLAG_COPYVOL) {
        ppbDsp->ve.currentVolume = ppbUser->ve.currentVolume;
        ppbDsp->ve.currentDelta = ppbUser->ve.currentDelta;
    }

    if (sync & AX_SYNC_FLAG_COPYFIR) {
        // copy FIR struct.
        u16* src;
        u16* dst;
        dst = (void*)&ppbDsp->fir;
        src = (void*)&ppbUser->fir;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src);
    }

    if (sync & (AX_SYNC_FLAG_COPYLOOP | AX_SYNC_FLAG_COPYLOOPADDR | AX_SYNC_FLAG_COPYENDADDR | AX_SYNC_FLAG_COPYCURADDR)) {
        if (sync & AX_SYNC_FLAG_COPYLOOP) {
            ppbDsp->addr.loopFlag = ppbUser->addr.loopFlag;
        }
        if (sync & AX_SYNC_FLAG_COPYLOOPADDR) {
            *(u32*)&ppbDsp->addr.loopAddressHi = *(u32*)&ppbUser->addr.loopAddressHi;
        }
        if (sync & AX_SYNC_FLAG_COPYENDADDR) {
            *(u32*)&ppbDsp->addr.endAddressHi = *(u32*)&ppbUser->addr.endAddressHi;
        }
        if (sync & AX_SYNC_FLAG_COPYCURADDR) {
            *(u32*)&ppbDsp->addr.currentAddressHi = *(u32*)&ppbUser->addr.currentAddressHi;
        } else {
            *(u32*)&ppbUser->addr.currentAddressHi = *(u32*)&ppbDsp->addr.currentAddressHi;
        }
    } else if (sync & AX_SYNC_FLAG_COPYADDR) {
        // copy ADDR struct.
        u32* src;
        u32* dst;
        dst = (void*)&ppbDsp->addr;
        src = (void*)&ppbUser->addr;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); 
    } else {
        ppbUser->addr.currentAddressHi = ppbDsp->addr.currentAddressHi;
        ppbUser->addr.currentAddressLo = ppbDsp->addr.currentAddressLo;
    }

    if (sync & AX_SYNC_FLAG_COPYADPCM) {
        // copy ADPCM struct.
        u32* src;
        u32* dst;
        dst = (void*)&ppbDsp->adpcm;
        src = (void*)&ppbUser->adpcm;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
    }

    if (sync & AX_SYNC_FLAG_COPYRATIO) {
        ppbDsp->src.ratioHi = ppbUser->src.ratioHi;
        ppbDsp->src.ratioLo = ppbUser->src.ratioLo;
    } else if (sync & AX_SYNC_FLAG_COPYSRC) {
        // copy SRC struct.
        u16* src;
        u16* dst;
        dst = (void*)&ppbDsp->src;
        src = (void*)&ppbUser->src;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
    }

    if (sync & AX_SYNC_FLAG_COPYADPCMLOOP) {
        // copy ADPCMLOOP struct.
        u16* src;
        u16* dst;
        dst = (void*)&ppbDsp->adpcmLoop;
        src = (void*)&ppbUser->adpcmLoop;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src);
    }

    if (sync & 0x400000) {
        ppbDsp->lpf.a0 = ppbUser->lpf.a0;
        ppbDsp->lpf.b0 = ppbUser->lpf.b0;
        return;
    }

    if (sync & 0x200000) {
        // copy AXPBLPF struct
        u16* src;
        u16* dst;

        dst = (void*)&ppbDsp->lpf;
        src = (void*)&ppbUser->lpf;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src);
    }
}

void __AXDumpVPB(AXVPB* pvpb) {
    AXPB* ppbDsp;

    ppbDsp = &__AXPB[pvpb->index];
    if (ppbDsp->state == 1) {
        __AXDepopVoice(ppbDsp);
    }
    pvpb->pb.state = ppbDsp->state = ppbDsp->update.updNum[0] 
        = ppbDsp->update.updNum[1] = ppbDsp->update.updNum[2] 
        = ppbDsp->update.updNum[3] = ppbDsp->update.updNum[4] = 0;
    __AXPushCallbackStack(pvpb);
}

void __AXSyncPBs(u32 lessDlpfycles) {
    u32 cycles;
    u32 i;
    AXVPB* pvpb;

    __AXNumVoices = 0;
    DCInvalidateRange(__AXPB, sizeof(__AXPB));
    DCInvalidateRange(__AXITD, sizeof(__AXITD));
    cycles = (__AXGetCommandListCycles() + 0x10000) - 0x55F0 + lessDlpfycles;

    for (i = 31; i; i--) {
        for (pvpb = __AXGetStackHead(i); pvpb; pvpb = pvpb->next) {
            if (pvpb->depop != 0) {
                __AXDepopVoice(&__AXPB[pvpb->index]);
            }

            if ((pvpb->pb.state == 1) || (pvpb->updateCounter != 0)) {
                if (pvpb->pb.srcSelect != 2) {
                    cycles += __AXSrcCycles[pvpb->pb.src.ratioHi];
                }

                if (pvpb->pb.lpf.on) {
                    cycles += 555;
                }

                cycles += __AXMainMixCycles[pvpb->pb.mixerCtrl & 0xF] + __AXAuxMixCycles[(pvpb->pb.mixerCtrl >> 4) & 0x1F] + __AXAuxMixCycles[(pvpb->pb.mixerCtrl >> 9) & 0x1F] + 0x8C;
                if (__AXMaxDspCycles > cycles) {
                    __AXServiceVPB(pvpb);
                } else {
                    __AXDumpVPB(pvpb);
                }
            } else {
                __AXServiceVPB(pvpb);
            }

            pvpb->sync = 0;
            pvpb->depop = 0;
            pvpb->updateMS = pvpb->updateCounter = 0;
            pvpb->updateWrite = pvpb->updateData;
        }
    }

    __AXRecDspCycles = cycles;
    for (pvpb = __AXGetStackHead(0); pvpb; pvpb = pvpb->next) {
        if (pvpb->depop != 0) {
            __AXDepopVoice(&__AXPB[pvpb->index]);
        }
        pvpb->depop = 0;
        __AXPB[pvpb->index].state = __AXPB[pvpb->index].update.updNum[0] = __AXPB[pvpb->index].update.updNum[1] 
                                  = __AXPB[pvpb->index].update.updNum[2] = __AXPB[pvpb->index].update.updNum[3] 
                                  = __AXPB[pvpb->index].update.updNum[4] = 0;
    }

    DCFlushRange(__AXPB, sizeof(__AXPB));
    DCFlushRange(__AXITD, sizeof(__AXITD));
    DCFlushRange(__AXUpdates, sizeof(__AXUpdates));
}

AXPB* __AXGetPBs(void) {
    return __AXPB;
}

void __AXSetPBDefault(AXVPB* p) {
    p->pb.state = 0;
    p->pb.itd.flag = 0;
    p->sync = 0x2000A4;
    p->updateMS = p->updateCounter = 0;
    p->updateWrite = p->updateData;
    p->pb.update.updNum[0] = p->pb.update.updNum[1] = p->pb.update.updNum[2] = p->pb.update.updNum[3] = p->pb.update.updNum[4] = 0;
    p->pb.lpf.on = 0;
}

void __AXVPBInit(void) {
    u32 i;
    AXPB* ppb;
    AXPBITDBUFFER* ppbi;
    AXPBU* ppbu;
    AXVPB* pvpb;
    u32* p;

#if DEBUG
    OSReport("Initializing AXVPB code module\n");
#endif
    __AXMaxDspCycles = OS_BUS_CLOCK / 400;
    __AXRecDspCycles = 0;

#define BUFFER_MEMSET(buffer, size)    \
    {                                  \
        p = (u32*)&buffer;             \
        for (i = size; i != 0; i--) {  \
            *p = 0;                    \
            p++;                       \
        }                              \
    }

    BUFFER_MEMSET(__AXPB, 0xF40);
    BUFFER_MEMSET(__AXITD, 0x400);
    BUFFER_MEMSET(__AXVPB, 0x22C0);

    for (i = 0; i < AX_MAX_VOICES; i++) {
        ppb = &__AXPB[i];
        ppbi = &__AXITD[i];
        ppbu = &__AXUpdates[i];
        pvpb = &__AXVPB[i];

        ASSERTLINE(913, (u32)ppb ^ 0x1F);
        ASSERTLINE(914, (u32)ppbi ^ 0x1F);
        ASSERTLINE(915, (u32)ppbu ^ 0x1F);

        pvpb->index = i;
        pvpb->updateWrite = pvpb->updateData;
        pvpb->itdBuffer = ppbi;
        __AXSetPBDefault(pvpb);

        if (i == 0x3F) {
            pvpb->pb.nextHi = pvpb->pb.nextLo = ppb->nextHi = ppb->nextLo = 0;
        } else {
            pvpb->pb.nextHi = (u16)(  (u32)((char*)ppb + sizeof(AXPB)) >> 16 );
            pvpb->pb.nextLo = (u16)(  (u32)((char*)ppb + sizeof(AXPB)) );
            ppb->nextHi = (u16)(  (u32)((char*)ppb + sizeof(AXPB)) >> 16 );
            ppb->nextLo = (u16)(  (u32)((char*)ppb + sizeof(AXPB)) );
        }

        pvpb->pb.currHi = (u16)(((u32)ppb) >> 16);
        pvpb->pb.currLo = (u16)((u32)ppb);
        ppb->currHi = (u16)(((u32)ppb) >> 16);
        ppb->currLo = (u16)((u32)ppb);
        pvpb->pb.itd.bufferHi = (u16)(((u32)ppbi) >> 16);
        pvpb->pb.itd.bufferLo = (u16)((u32)ppbi);
        ppb->itd.bufferHi = (u16)(((u32)ppbi) >> 16);
        ppb->itd.bufferLo = (u16)((u32)ppbi);
        pvpb->pb.update.dataHi = (u16)(((u32)ppbu) >> 16);
        pvpb->pb.update.dataLo = (u16)((u32)ppbu);
        ppb->update.dataHi = (u16)(((u32)ppbu) >> 16);
        ppb->update.dataLo = (u16)((u32)ppbu);

        pvpb->priority = 1;
        __AXPushFreeStack(pvpb);
    }

    DCFlushRange(__AXPB, sizeof(__AXPB));
}

void __AXVPBQuit(void) {
#if DEBUG
    OSReport("Shutting down AXVPB code module\n");
#endif
}

void AXSetVoiceSrcType(AXVPB* p, u32 type) {
    BOOL old;
    AXPB* ppb;

    ASSERTLINE(1020, p);
    ASSERTLINE(1021, type <= AX_SRC_TYPE_4TAP_16K);

    old = OSDisableInterrupts();
    ppb = &p->pb;
    switch(type) {
    case AX_SRC_TYPE_NONE:
        ppb->srcSelect = 2;
        break;
    case AX_SRC_TYPE_LINEAR:
        ppb->srcSelect = 1;
        break;
    case AX_SRC_TYPE_4TAP_8K:
        ppb->srcSelect = 0;
        ppb->coefSelect = 0;
        break;
    case AX_SRC_TYPE_4TAP_12K:
        ppb->srcSelect = 0;
        ppb->coefSelect = 1;
        break;
    case AX_SRC_TYPE_4TAP_16K:
        ppb->srcSelect = 0;
        ppb->coefSelect = 2;
        break;
    }

    p->sync |= AX_SYNC_FLAG_COPYSELECT;
    OSRestoreInterrupts(old);
}

void AXSetVoiceState(AXVPB* p, u16 state) {
    BOOL old;

    old = OSDisableInterrupts();
    p->pb.state = state;
    p->sync |= AX_SYNC_FLAG_COPYSTATE;
    if (state == 0) {
        p->depop = 1;
    }
    OSRestoreInterrupts(old);
}

void AXSetVoiceType(AXVPB* p, u16 type) {
    BOOL old;

    old = OSDisableInterrupts();
    p->pb.type = type;
    p->sync |= AX_SYNC_FLAG_COPYTYPE;
    OSRestoreInterrupts(old);
}

void AXSetVoiceMix(AXVPB* p, AXPBMIX* mix) {
    BOOL old;
    u16 mixerCtrl;
    u16* dst;
    u16* src;
    
    src = (u16*)mix;
    dst = (u16*)&p->pb.mix;
    mixerCtrl = 0;

    old = OSDisableInterrupts();

    if ((*dst++ = *src++)) mixerCtrl |= 0x1;
    if ((*dst++ = *src++)) mixerCtrl |= 0x9;
    if ((*dst++ = *src++)) mixerCtrl |= 0x2;
    if ((*dst++ = *src++)) mixerCtrl |= 0xA;
    if ((*dst++ = *src++)) mixerCtrl |= 0x10;
    if ((*dst++ = *src++)) mixerCtrl |= 0x50;
    if ((*dst++ = *src++)) mixerCtrl |= 0x20;
    if ((*dst++ = *src++)) mixerCtrl |= 0x60;
    if ((*dst++ = *src++)) mixerCtrl |= 0x200;
    if ((*dst++ = *src++)) mixerCtrl |= 0xA00;
    if ((*dst++ = *src++)) mixerCtrl |= 0x400;
    if ((*dst++ = *src++)) mixerCtrl |= 0xC00;
    if ((*dst++ = *src++)) mixerCtrl |= 0x1000;
    if ((*dst++ = *src++)) mixerCtrl |= 0x3000;
    if ((*dst++ = *src++)) mixerCtrl |= 0x4;
    if ((*dst++ = *src++)) mixerCtrl |= 0xC;
    if ((*dst++ = *src++)) mixerCtrl |= 0x80;
    if ((*dst++ = *src++)) mixerCtrl |= 0x180;

    p->pb.mixerCtrl = mixerCtrl;
    p->sync |= (AX_SYNC_FLAG_COPYAXPBMIX | AX_SYNC_FLAG_COPYMXRCTRL);
    OSRestoreInterrupts(old);
}

void AXSetVoiceItdOn(AXVPB* p) {
    BOOL old;

    old = OSDisableInterrupts();
    p->pb.itd.flag = 1;
    p->pb.itd.shiftL = p->pb.itd.shiftR = p->pb.itd.targetShiftL = p->pb.itd.targetShiftR = 0;
    p->sync &= ~(AX_SYNC_FLAG_COPYTSHIFT);
    p->sync |= AX_SYNC_FLAG_COPYITD;
    OSRestoreInterrupts(old);
}

void AXSetVoiceItdTarget(AXVPB* p, u16 lShift, u16 rShift) {
    BOOL old;

    old = OSDisableInterrupts();
    p->pb.itd.targetShiftL = lShift;
    p->pb.itd.targetShiftR = rShift;
    p->sync |= AX_SYNC_FLAG_COPYTSHIFT;
    OSRestoreInterrupts(old);
}

void AXSetVoiceUpdateIncrement(AXVPB* p) {
    BOOL old;

    old = OSDisableInterrupts();
    p->updateMS++;
    p->sync |= AX_SYNC_FLAG_COPYUPDATE;
    ASSERTMSGLINE(1191, p->updateMS <= 4, "PB updates cannot exceed 5ms\n");
    OSRestoreInterrupts(old);
}

void AXSetVoiceUpdateWrite(AXVPB* p, u16 param, u16 data) {
    BOOL old;

    old = OSDisableInterrupts();
    p->updateCounter+=2;
    ASSERTMSGLINE(1205, p->updateCounter <= 128, "PB update block exceeded 128 words\n");

    *(p->updateWrite) = param; p->updateWrite+=1;
    *(p->updateWrite) = data; p->updateWrite+=1;
    p->sync |= AX_SYNC_FLAG_COPYUPDATE;
    OSRestoreInterrupts(old);
}

void AXSetVoiceDpop(AXVPB* p, AXPBDPOP* dpop) {
    BOOL old;
    u16* dst;
    u16* src;

    dst = (void*)&p->pb.dpop;
    src = (void*)dpop;

    old = OSDisableInterrupts();
    {
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
    }
    p->sync |= AX_SYNC_FLAG_COPYDPOP;
    OSRestoreInterrupts(old);
}

void AXSetVoiceVe(AXVPB* p, AXPBVE* ve) {
    BOOL old;

    old = OSDisableInterrupts();
    p->pb.ve.currentVolume = ve->currentVolume;
    p->pb.ve.currentDelta = ve->currentDelta;
    p->sync |= AX_SYNC_FLAG_COPYVOL;
    OSRestoreInterrupts(old);
}

void AXSetVoiceVeDelta(AXVPB* p, s16 delta) {
    BOOL old;

    old = OSDisableInterrupts();
    p->pb.ve.currentDelta = delta;
    p->sync |= AX_SYNC_FLAG_SWAPVOL;
    OSRestoreInterrupts(old);
}

void AXSetVoiceFir(AXVPB* p, AXPBFIR* fir) {
    BOOL old;

    old = OSDisableInterrupts();
    p->pb.fir.numCoefs = fir->numCoefs;
    p->pb.fir.coefsHi = fir->coefsHi;
    p->pb.fir.coefsLo = fir->coefsLo;
    p->sync |= AX_SYNC_FLAG_COPYFIR;
    OSRestoreInterrupts(old);
}

void AXSetVoiceAddr(AXVPB* p, AXPBADDR* addr) {
    BOOL old;
    u32* dst;
    u32* src;

    dst = (void*)&p->pb.addr;
    src = (void*)addr;

    old = OSDisableInterrupts();
    {
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src);
    }

    switch(addr->format) {
    case 0:
        ASSERTMSGLINE(1335, (addr->loopAddressLo & 0xF) > 1, "*** loop address on ADPCM frame header! ***\n");
        ASSERTMSGLINE(1340, (addr->endAddressLo & 0xF) > 1, "*** end address on ADPCM frame header! ***\n");
        ASSERTMSGLINE(1345, (addr->currentAddressLo & 0xF) > 1, "*** current address on ADPCM frame header! ***\n");
        break;
    case 10:
        dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0x08000000; dst+=1;
        *(dst) = 0; dst+=1;
        break;
    case 25:
        dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0; dst+=1;
        *(dst) = 0x01000000; dst+=1;
        *(dst) = 0; dst+=1;
        break;
    default:
        ASSERTMSGLINE(1389, 0, "unknown addr->formaqt in PB\n");
        break;
    }

    p->sync &= ~(AX_SYNC_FLAG_COPYLOOP | AX_SYNC_FLAG_COPYLOOPADDR | AX_SYNC_FLAG_COPYENDADDR | AX_SYNC_FLAG_COPYCURADDR);
    p->sync |= (AX_SYNC_FLAG_COPYADDR | AX_SYNC_FLAG_COPYADPCM);
    OSRestoreInterrupts(old);
}

void AXSetVoiceLoop(AXVPB* p, u16 loop) {
    BOOL old;

    old = OSDisableInterrupts();
    p->pb.addr.loopFlag = loop;
    p->sync |= AX_SYNC_FLAG_COPYLOOP;
    OSRestoreInterrupts(old);
}

void AXSetVoiceLoopAddr(AXVPB* p, u32 addr) {
    BOOL old;

    old = OSDisableInterrupts();
    p->pb.addr.loopAddressHi = (addr >> 0x10);
    p->pb.addr.loopAddressLo = (addr);
    p->sync |= AX_SYNC_FLAG_COPYLOOPADDR;
    OSRestoreInterrupts(old);
}

void AXSetVoiceEndAddr(AXVPB* p, u32 addr) {
    BOOL old;

    old = OSDisableInterrupts();
    p->pb.addr.endAddressHi = (addr >> 0x10);
    p->pb.addr.endAddressLo = (addr);
    p->sync |= AX_SYNC_FLAG_COPYENDADDR;
    OSRestoreInterrupts(old);
}

void AXSetVoiceCurrentAddr(AXVPB* p, u32 addr) {
    BOOL old;

    old = OSDisableInterrupts();
    p->pb.addr.currentAddressHi = (addr >> 0x10);
    p->pb.addr.currentAddressLo = (addr);
    p->sync |= AX_SYNC_FLAG_COPYCURADDR;
    OSRestoreInterrupts(old);
}

void AXSetVoiceAdpcm(AXVPB* p, AXPBADPCM* adpcm) {
    BOOL old;
    u32* dst;
    u32* src;

    dst = (void*)&p->pb.adpcm;
    src = (void*)adpcm;

    old = OSDisableInterrupts();

    {
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
    }
    p->sync |= AX_SYNC_FLAG_COPYADPCM;
    OSRestoreInterrupts(old);
}

void AXSetVoiceSrc(AXVPB* p, AXPBSRC* src_) {
    BOOL old;
    u16* dst;
    u16* src;

    dst = (void*)&p->pb.src;
    src = (void*)src_;

    old = OSDisableInterrupts();
    {
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
    }
    p->sync &= ~(AX_SYNC_FLAG_COPYRATIO);
    p->sync |= AX_SYNC_FLAG_COPYSRC;
    OSRestoreInterrupts(old);
}

void AXSetVoiceSrcRatio(AXVPB* p, float ratio) {
    u32 r;
    BOOL old;

    old = OSDisableInterrupts();
    r = 65536.0f* ratio;
    if (r > 0x40000) {
        r = 0x40000;
    }
    p->pb.src.ratioHi = ((u32)r >> 0x10);
    p->pb.src.ratioLo = ((u32)r);
    p->sync |= AX_SYNC_FLAG_COPYRATIO;
    OSRestoreInterrupts(old);
}

void AXSetVoiceAdpcmLoop(AXVPB* p, AXPBADPCMLOOP* adpcmloop) {
    BOOL old;
    u16* dst;
    u16* src;

    dst = (void*)&p->pb.adpcmLoop;
    src = (void*)adpcmloop;
    old = OSDisableInterrupts();
    {
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
        *(dst) = *(src); dst+=1; src+=1;
    }
    p->sync |= AX_SYNC_FLAG_COPYADPCMLOOP;
    OSRestoreInterrupts(old);
}

void AXSetVoiceLpf(AXVPB* p, AXPBLPF* lpf) {
    BOOL old;
    u16* dst;
    u16* src;

    dst = (u16*)&p->pb.lpf;
    src = (u16*)lpf;

    old = OSDisableInterrupts();

    *dst = *src; dst++; src++;
    *dst = *src; dst++; src++;
    *dst = *src; dst++; src++;
    *dst = *src; dst++; src++;
    p->sync |= 0x200000;

    OSRestoreInterrupts(old);
}

void AXSetVoiceLpfCoefs(AXVPB* p, u16 a0, u16 b0) {
    BOOL old;
    old = OSDisableInterrupts();

    p->pb.lpf.a0 = a0;
    p->pb.lpf.b0 = b0;
    p->sync |= 0x400000;

    OSRestoreInterrupts(old);
}

#define PI 3.14159265358979323846f

void AXGetLpfCoefs(u16 freq, u16* a0, u16* b0) {
    f32 bb;
    f32 cc;

    ASSERTMSGLINE(1616, freq <= 16000, "freq is out of range");

    cc = 2.0f - cos(((PI * 2) * (f32)freq) / 32000.0f);
    bb = sqrt((cc * cc) - 1.0f) - cc;

    *b0 = 32768.0f * -bb;
    *a0 = 0x7FFF - *b0;
}

void AXSetMaxDspCycles(u32 cycles) {
    __AXMaxDspCycles = cycles;
}

u32 AXGetMaxDspCycles(void) {
    return __AXMaxDspCycles;
}

u32 AXGetDspCycles(void) {
    return __AXRecDspCycles;
}
