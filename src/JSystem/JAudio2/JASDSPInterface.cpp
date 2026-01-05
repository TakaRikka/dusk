#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASDSPInterface.h"
#include "JSystem/JAudio2/JASCalc.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"
#include "JSystem/JAudio2/JASWaveInfo.h"
#include "JSystem/JAudio2/dspproc.h"
#include "JSystem/JAudio2/dsptask.h"
#include "JSystem/JAudio2/osdsp_task.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include <dolphin/os.h>

JASDsp::TChannel* JASDsp::CH_BUF;

JASDsp::FxBuf* JASDsp::FX_BUF;

f32 JASDsp::sDSPVolume;

u16 JASDsp::SEND_TABLE[] = {
    0x0D00,
    0x0D60,
    0x0DC8,
    0x0E28,
    0x0E88,
    0x0EE8,
    0x0CA0,
    0x0F40,
    0x0FA0,
    0x0B00,
    0x09A0,
    0x0000,
};

u32 JASWaveInfo::one = 1;

#if DEBUG
s32 JASDsp::dspMutex = 1;
#endif

void JASDsp::boot(void (*param_0)(void*)) {
    static bool data_804512F4 = true;
    if (data_804512F4) {
        DspBoot(param_0);
#if !PLATFORM_GCN
        // DsetVARAM(JKRHeap::getAltAramStartAdr());
#endif
        data_804512F4 = false;
    }
}

void JASDsp::releaseHalt(u32 param_0) {
    DSPReleaseHalt2(param_0);
}

void JASDsp::finishWork(u16 param_0) {
    DspFinishWork(param_0);
}

void JASDsp::syncFrame(u32 param_0, u32 param_1, u32 param_2) {
    DsyncFrame2(param_0, param_1, param_2);
}

void JASDsp::setDSPMixerLevel(f32 dsp_level) {
    sDSPVolume = dsp_level;
    dsp_level *= 4.0f;
    JUT_ASSERT(277, (0.0 <= dsp_level) && (dsp_level <= 8.0));
    DsetMixerLevel(dsp_level);
}

f32 JASDsp::getDSPMixerLevel() {
    return sDSPVolume;
}

JASDsp::TChannel* JASDsp::getDSPHandle(int param_0) {
    return CH_BUF + param_0;
}

JASDsp::TChannel* JASDsp::getDSPHandleNc(int param_0) {
    return (TChannel*)OSCachedToUncached(CH_BUF + param_0);
}

void JASDsp::setFilterTable(s16* param_0, s16* param_1, u32 param_2) {
    for (int i = 0; i < param_2; i++) {
        *param_0++ = *param_1++;
    }
}

void JASDsp::flushBuffer() {
    DCFlushRange(CH_BUF, sizeof(TChannel) * 64);
    DCFlushRange(FX_BUF, sizeof(FxBuf) * 4);
}

void JASDsp::invalChannelAll() {
    DCInvalidateRange(CH_BUF, sizeof(TChannel) * 64);
}

u8 const ATTRIBUTE_ALIGN(32) JASDsp::DSPADPCM_FILTER[64] = {
    0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x04, 0x00, 0x04, 0x00,
    0x10, 0x00, 0xF8, 0x00, 0x0E, 0x00, 0xFA, 0x00, 0x0C, 0x00, 0xFC, 0x00, 0x12, 0x00, 0xF6, 0x00,
    0x10, 0x68, 0xF7, 0x38, 0x12, 0xC0, 0xF7, 0x04, 0x14, 0x00, 0xF4, 0x00, 0x08, 0x00, 0xF8, 0x00,
    0x04, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x04, 0x00, 0xFC, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00,
};

u32 const ATTRIBUTE_ALIGN(32) JASDsp::DSPRES_FILTER[320] = {
    0x0C3966AD,
    0x0D46FFDF,
    0x0B396696,
    0x0E5FFFD8,
    0x0A446669,
    0x0F83FFD0,
    0x095A6626,
    0x10B4FFC8,
    0x087D65CD,
    0x11F0FFBF,
    0x07AB655E,
    0x1338FFB6,
    0x06E464D9,
    0x148CFFAC,
    0x0628643F,
    0x15EBFFA1,
    0x0577638F,
    0x1756FF96,
    0x04D162CB,
    0x18CBFF8A,
    0x043561F3,
    0x1A4CFF7E,
    0x03A46106,
    0x1BD7FF71,
    0x031C6007,
    0x1D6CFF64,
    0x029F5EF5,
    0x1F0BFF56,
    0x022A5DD0,
    0x20B3FF48,
    0x01BE5C9A,
    0x2264FF3A,
    0x015B5B53,
    0x241EFF2C,
    0x010159FC,
    0x25E0FF1E,
    0x00AE5896,
    0x27A9FF10,
    0x00635720,
    0x297AFF02,
    0x001F559D,
    0x2B50FEF4,
    0xFFE2540D,
    0x2D2CFEE8,
    0xFFAC5270,
    0x2F0DFEDB,
    0xFF7C50C7,
    0x30F3FED0,
    0xFF534F14,
    0x32DCFEC6,
    0xFF2E4D57,
    0x34C8FEBD,
    0xFF0F4B91,
    0x36B6FEB6,
    0xFEF549C2,
    0x38A5FEB0,
    0xFEDF47ED,
    0x3A95FEAC,
    0xFECE4611,
    0x3C85FEAB,
    0xFEC04430,
    0x3E74FEAC,
    0xFEB6424A,
    0x4060FEAF,
    0xFEAF4060,
    0x424AFEB6,
    0xFEAC3E74,
    0x4430FEC0,
    0xFEAB3C85,
    0x4611FECE,
    0xFEAC3A95,
    0x47EDFEDF,
    0xFEB038A5,
    0x49C2FEF5,
    0xFEB636B6,
    0x4B91FF0F,
    0xFEBD34C8,
    0x4D57FF2E,
    0xFEC632DC,
    0x4F14FF53,
    0xFED030F3,
    0x50C7FF7C,
    0xFEDB2F0D,
    0x5270FFAC,
    0xFEE82D2C,
    0x540DFFE2,
    0xFEF42B50,
    0x559D001F,
    0xFF02297A,
    0x57200063,
    0xFF1027A9,
    0x589600AE,
    0xFF1E25E0,
    0x59FC0101,
    0xFF2C241E,
    0x5B53015B,
    0xFF3A2264,
    0x5C9A01BE,
    0xFF4820B3,
    0x5DD0022A,
    0xFF561F0B,
    0x5EF5029F,
    0xFF641D6C,
    0x6007031C,
    0xFF711BD7,
    0x610603A4,
    0xFF7E1A4C,
    0x61F30435,
    0xFF8A18CB,
    0x62CB04D1,
    0xFF961756,
    0x638F0577,
    0xFFA115EB,
    0x643F0628,
    0xFFAC148C,
    0x64D906E4,
    0xFFB61338,
    0x655E07AB,
    0xFFBF11F0,
    0x65CD087D,
    0xFFC810B4,
    0x6626095A,
    0xFFD00F83,
    0x66690A44,
    0xFFD80E5F,
    0x66960B39,
    0xFFDF0D46,
    0x66AD0C39,
    0x00000C8B,
    0x18F82527,
    0x30FB3C56,
    0x471C5133,
    0x5A8262F1,
    0x6A6D70E2,
    0x76417A7C,
    0x7D897F61,
    0x7FFF7F61,
    0x7D897A7C,
    0x764170E2,
    0x6A6D62F1,
    0x5A825133,
    0x471C3C56,
    0x30FB2527,
    0x18F80C8B,
    0x0000F375,
    0xE708DAD9,
    0xCF05C3AA,
    0xB8E4AECD,
    0xA57E9D0F,
    0x95938F1E,
    0x89BF8584,
    0x8277809F,
    0x8001809F,
    0x82778584,
    0x89BF8F1E,
    0x95939D0F,
    0xA57EAECD,
    0xB8E4C3AA,
    0xCF05DAD9,
    0xE708F375,
    0x000007FF,
    0x0FFF17FF,
    0x1FFF27FF,
    0x2FFF37FF,
    0x3FFF47FF,
    0x4FFF57FF,
    0x5FFF67FF,
    0x6FFF77FF,
    0x7FFF7800,
    0x70006800,
    0x60005800,
    0x50004800,
    0x40003800,
    0x30002800,
    0x20001800,
    0x10000800,
    0x0000F801,
    0xF001E801,
    0xE001D801,
    0xD001C801,
    0xC001B801,
    0xB001A801,
    0xA0019801,
    0x90018801,
    0x80018800,
    0x90009800,
    0xA000A800,
    0xB000B800,
    0xC000C800,
    0xD000D800,
    0xE000E800,
    0xF000F800,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0x1FFF3FFF,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0x1FFFC001,
    0x00000192,
    0x032404B6,
    0x064707D9,
    0x096A0AFB,
    0x0C8B0E1B,
    0x0FAB1139,
    0x12C71455,
    0x15E1176D,
    0x18F81A82,
    0x1C0B1D93,
    0x1F19209F,
    0x222323A6,
    0x252726A7,
    0x282629A3,
    0x2B1F2C98,
    0x2E102F87,
    0x30FB326E,
    0x33DE354D,
    0x36B93824,
    0x398C3AF2,
    0x3C563DB7,
    0x3F174073,
    0x41CD4325,
    0x447A45CC,
    0x471C4869,
    0x49B34AFB,
    0x4C3F4D81,
    0x4EBF4FFB,
    0x51335268,
    0x539A54C9,
    0x55F5571D,
    0x58425964,
    0x5A825B9C,
    0x5CB35DC7,
    0x5ED75FE3,
    0x60EB61F0,
    0x62F163EE,
    0x64E865DD,
    0x66CF67BC,
    0x68A6698B,
    0x6A6D6B4A,
    0x6C236CF8,
    0x6DC96E96,
    0x6F5E7022,
    0x70E2719D,
    0x72547307,
    0x73B5745F,
    0x750475A5,
    0x764176D8,
    0x776B77FA,
    0x78847909,
    0x79897A05,
    0x7A7C7AEE,
    0x7B5C7BC5,
    0x7C297C88,
    0x7CE37D39,
    0x7D897DD5,
    0x7E1D7E5F,
    0x7E9C7ED5,
    0x7F097F37,
    0x7F617F86,
    0x7FA67FC1,
    0x7FD87FE9,
    0x7FF57FFD,
};

void JASDsp::initBuffer() {
    CH_BUF = new(JASDram, 0x20) TChannel[64];
    JUT_ASSERT(354, CH_BUF);
    FX_BUF = new(JASDram, 0x20) FxBuf[4];
    JUT_ASSERT(356, FX_BUF);
    JASCalc::bzero(CH_BUF, 0x6000);
    JASCalc::bzero(FX_BUF, sizeof(FxBuf) * 4);
    for (u8 i = 0; i < 4; i++) {
        setFXLine(i, NULL, NULL);
    }
    DsetupTable(0x40, u32(CH_BUF), u32(&DSPRES_FILTER), u32(&DSPADPCM_FILTER), u32(FX_BUF));
    flushBuffer();
}

int JASDsp::setFXLine(u8 param_0, s16* buffer, JASDsp::FxlineConfig_* param_2) {
    FxBuf* puVar3 = FX_BUF + param_0;
    JASCriticalSection aJStack_20;
    puVar3->field_0x0 = 0;
    if (param_2 != NULL) {
        puVar3->field_0xa = param_2->field_0x4;
        puVar3->field_0x8 = SEND_TABLE[param_2->field_0x2];
        puVar3->field_0xe = param_2->field_0x8;
        puVar3->field_0xc = SEND_TABLE[param_2->field_0x6];
        puVar3->field_0x2 = param_2->field_0xc;
        setFilterTable((s16*)puVar3->field_0x10, param_2->field_0x10, 8);
    }
    if (buffer != NULL && param_2 != NULL) {
        u32 bufsize = param_2->field_0xc * 0xa0;
        puVar3->field_0x4 = buffer;
        JASCalc::bzero(buffer, bufsize);
        JUT_ASSERT(420, (reinterpret_cast<u32>(buffer) & 0x1f) == 0);
        JUT_ASSERT(421, (bufsize & 0x1f) == 0);
        DCFlushRange(buffer, bufsize);
    } else if (param_2 == NULL || buffer != NULL) {
        puVar3->field_0x4 = buffer;
    }

    if (puVar3->field_0x4 != NULL) {
        puVar3->field_0x0 = param_2->field_0x0;
    } else {
        puVar3->field_0x0 = 0;
    }
    DCFlushRange(puVar3, sizeof(FxBuf));
    return 1;
}

BOOL JASDsp::changeFXLineParam(u8 param_0, u8 param_1, u32 param_2) {
    JUT_ASSERT(450, dspMutex);
    FxBuf* buf = &FX_BUF[param_0];
    switch (param_1) {
    case 0: buf->field_0x8 = param_2; break;
    case 1: buf->field_0xc = param_2; break;
    case 2: buf->field_0xa = param_2; break;
    case 3: buf->field_0xe = param_2; break;
    case 4: buf->field_0x2 = param_2; break;
    case 5: buf->field_0x10[0] = param_2; break;
    case 6: buf->field_0x10[1] = param_2; break;
    case 7: buf->field_0x10[2] = param_2; break;
    case 8: buf->field_0x10[3] = param_2; break;
    case 9: buf->field_0x10[4] = param_2; break;
    case 10: buf->field_0x10[5] = param_2; break;
    case 11: buf->field_0x10[6] = param_2; break;
    case 12: buf->field_0x10[7] = param_2; break;
    case 13: setFilterTable((s16*)buf->field_0x10, (s16*)param_2, 8); break;
    case 14: buf->field_0x0 = param_2; break;
    }
    return 1;
}

void JASDsp::TChannel::init() {
    JUT_ASSERT(489, dspMutex);
    mPauseFlag = 0;
    mIsFinished = 0;
    mForcedStop = 0;
    mIsActive = 0;
    field_0x058 = 0;
    field_0x068 = 0;
    initFilter();
}

void JASDsp::TChannel::playStart() {
    JUT_ASSERT(508, dspMutex);
    field_0x10c = 0;
    field_0x060 = 0;
    field_0x008 = 1;
    field_0x066 = 0;
    int i;
    for (i = 0; i < 4; i++) {
        field_0x078[i] = 0;
        field_0x0a8[i] = 0;
    }
    for (i = 0; i < 20; i++) {
        field_0x080[i] = 0;
    }
    mIsActive = 1;
}

void JASDsp::TChannel::playStop() {
    JUT_ASSERT(540, dspMutex);
    mIsActive = 0;
}

void JASDsp::TChannel::replyFinishRequest() {
    JUT_ASSERT(549, dspMutex);
    mIsFinished = 0;
    mIsActive = 0;
}

void JASDsp::TChannel::forceStop() {
    JUT_ASSERT(559, dspMutex);
    mForcedStop = 1;
}

bool JASDsp::TChannel::isActive() const {
    JUT_ASSERT(568, dspMutex);
    return mIsActive != 0;
}

bool JASDsp::TChannel::isFinish() const {
    JUT_ASSERT(577, dspMutex);
    return mIsFinished != 0;
}

void JASDsp::TChannel::setWaveInfo(JASWaveInfo const& param_0, u32 param_1, u32 param_2) {
    int i;
    JUT_ASSERT(610, dspMutex);
    field_0x118 = param_1;
    static const u8 COMP_BLOCKSAMPLES[8] = {0x10, 0x10, 0x01, 0x01, 0x01, 0x10, 0x10, 0x01};
    field_0x064 = COMP_BLOCKSAMPLES[param_0.field_0x00];
    static const u8 COMP_BLOCKBYTES[8] = {0x09, 0x05, 0x08, 0x10, 0x01, 0x01, 0x01, 0x01};
    field_0x100 = COMP_BLOCKBYTES[param_0.field_0x00];
    field_0x068 = 0;
    if (field_0x100 >= 4) {
        field_0x11c = param_0.field_0x18;
        field_0x102 = param_0.field_0x02;
        if (field_0x102) {
            if (param_2 == 1) {
                param_2 = param_0.field_0x10;
            }
            field_0x110 = param_0.field_0x10;
            field_0x114 = param_0.field_0x14;
            field_0x104 = param_0.field_0x1c;
            field_0x106 = param_0.field_0x1e;
        } else {
            field_0x114 = field_0x11c;
        }
        if (param_2 && field_0x114 > param_2) {
            switch (param_0.field_0x00) {
            case 0:
            case 1:
                field_0x068 = param_2;
                field_0x118 += param_2 * field_0x100 >> 4;
                field_0x110 -= param_2;
                field_0x114 -= param_2;
                break;
            case 2:
            case 3:
                field_0x068 = param_2;
                break;
            }
        }
        for (i = 0; i < 16; i++) {
            field_0x0b0[i] = 0;
        }
    }
}

void JASDsp::TChannel::setOscInfo(u32 param_0) {
    JUT_ASSERT(671, dspMutex);
    field_0x118 = 0;
    field_0x064 = 16;
    field_0x100 = param_0;
}

void JASDsp::TChannel::initAutoMixer() {
    JUT_ASSERT(688, dspMutex);
    if (field_0x058) {
        field_0x054 = field_0x056;
    } else {
        field_0x054 = 0;
        field_0x058 = 1;
    }
}

void JASDsp::TChannel::setAutoMixer(u16 param_0, u8 param_1, u8 param_2, u8 param_3,
                                        u8 param_4) {
    JUT_ASSERT(709, dspMutex);
    field_0x050 = (param_1 << 8) | param_2;
    field_0x052 = param_3 << 8 | param_3 << 1;
    field_0x056 = param_0;
    field_0x058 = 1;
}

void JASDsp::TChannel::setPitch(u16 param_0) {
    JUT_ASSERT(763, dspMutex);
    if (param_0 >= 0x7fff) {
        param_0 = 0x7fff;
    }
    mPitch = param_0;
}

void JASDsp::TChannel::setMixerInitVolume(u8 param_0, s16 param_1) {
    JUT_ASSERT(798, dspMutex);
    u16* tmp = field_0x010[param_0];
    tmp[2] = param_1;
    tmp[1] = param_1;
    tmp[3] = 0;
}

void JASDsp::TChannel::setMixerVolume(u8 param_0, s16 param_1) {
    u16* tmp;
    JUT_ASSERT(841, dspMutex);
    if (mForcedStop == 0) {
        tmp = field_0x010[param_0];
        tmp[1] = param_1;
        tmp[3] &= 0xff;
    }
}

void JASDsp::TChannel::setPauseFlag(u8 param_0) {
    JUT_ASSERT(863, dspMutex);
    mPauseFlag = param_0;
}

void JASDsp::TChannel::flush() {
    DCFlushRange(this, sizeof(*this));
}

void JASDsp::TChannel::initFilter() {
    u32 i;
    JUT_ASSERT(888, dspMutex);
    for (i = 0; i < 8; i++) {
        fir_filter_params[i] = 0;
    }
    fir_filter_params[0] = 0x7fff;
    for (i = 0; i < 8; i++) {
        iir_filter_params[i] = 0;
    }
    iir_filter_params[0] = 0x7fff;
    iir_filter_params[4] = 0;
}

void JASDsp::TChannel::setFilterMode(u16 param_0) {
    JUT_ASSERT(914, dspMutex);
    u8 r30 = param_0 & 0x20;
    u8 r31 = param_0 & 0x1f;
    if (r30) {
        if (r31 > 0x14) {
            r31 = 0x14;
        }
    } else {
        if (r31 > 0x18) {
            r31 = 0x18;
        }
    }
    mFilterMode = r30 + r31;
}

void JASDsp::TChannel::setIIRFilterParam(s16* param_0) {
    JUT_ASSERT(937, dspMutex);
    setFilterTable(iir_filter_params, param_0, 8);
}

void JASDsp::TChannel::setFIR8FilterParam(s16* param_0) {
    JUT_ASSERT(948, dspMutex);
    setFilterTable(fir_filter_params, param_0, 8);
}

void JASDsp::TChannel::setDistFilter(s16 param_0) {
    JUT_ASSERT(959, dspMutex);
    iir_filter_params[4] = param_0;
}

void JASDsp::TChannel::setBusConnect(u8 param_0, u8 param_1) {
    JUT_ASSERT(973, dspMutex);
    u16* tmp = field_0x010[param_0];
    static u16 const connect_table[12] = {
        0x0000, 0x0D00, 0x0D60, 0x0DC0, 0x0E20, 0x0E80,
        0x0EE0, 0x0CA0, 0x0F40, 0x0FA0, 0x0B00, 0x09A0,
    };
    u16 r30 = 0;
    r30 = connect_table[param_1];
    tmp[0] = r30;
}

u16 DSP_CreateMap2(u32 param_0) {
    u16 r30 = 0;
    JASDsp::TChannel* channel = &JASDsp::CH_BUF[param_0 << 4];
    for (int i = 0; i < 16; i++) {
        r30 <<= 1;
        if (channel->isActive()) {
            r30 |= 1;
        }
        channel++;
    }
    return r30;
}
