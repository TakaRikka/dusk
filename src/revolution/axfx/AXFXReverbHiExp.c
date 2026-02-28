#include <revolution/axfx.h>
#include <revolution/mem.h>
#include <cmath>

static u32 __EarlySizeTable[8][3] = {
    {157, 479, 829},   {317, 809, 1117},  {479, 941, 1487},   {641, 1259, 1949},
    {797, 1667, 2579}, {967, 1901, 2903}, {1123, 2179, 3413}, {1279, 2477, 3889}
};

static f32 __EarlyCoefTable[8][3] = {
    {0.4f, -1.0f, 0.3f}, {0.5f, -0.95f, 0.3f}, {0.6f, -0.9f, 0.3f}, {0.75f, -0.85f, 0.3f},
    {-0.9f, 0.8f, 0.3f}, {-1.0f, 0.7f, 0.3f},  {-1.0f, 0.7f, 0.3f}, {-1.0f, 0.7f, 0.3f}
};

static u32 __FilterSizeTable[7][8] = {
    {1789, 1999, 2333, 433, 149, 47, 73, 67}, {149, 293, 449, 251, 103, 47, 73, 67},
    {947, 1361, 1531, 433, 137, 47, 73, 67},  {1279, 1531, 1973, 509, 149, 47, 73, 67},
    {1531, 1847, 2297, 563, 179, 47, 73, 67}, {1823, 2357, 2693, 571, 137, 47, 73, 67},
    {1823, 2357, 2693, 571, 179, 47, 73, 67}
};

static BOOL __AllocDelayLine(AXFX_REVERBHI_EXP* reverb);
static void __FreeDelayLine(AXFX_REVERBHI_EXP* reverb);
static void __BzeroDelayLines(AXFX_REVERBHI_EXP* reverb);
static BOOL __InitParams(AXFX_REVERBHI_EXP* reverb);

f32 dummy_0() {
    return 32000.0f;
}

BOOL AXFXReverbHiExpInit(AXFX_REVERBHI_EXP* reverb) {
    u32 ch, i;
    BOOL result = TRUE;
    BOOL mask = OSDisableInterrupts();

    reverb->active = 1;

    if (reverb->preDelayTimeMax < 0.0f) {
        AXFXReverbHiExpShutdown(reverb);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    reverb->earlyMaxLength = __EarlySizeTable[8 - 1][2];
    reverb->preDelayMaxLength = (u32)(reverb->preDelayTimeMax * 32000);

    for (i = 0; i < 3; i++) {
        reverb->combMaxLength[i] = __FilterSizeTable[6][i];
    }

    for (i = 0; i < 2; i++) {
        reverb->allpassMaxLength[i] = __FilterSizeTable[6][3 + i];
    }

    for (ch = 0; ch < 3; ch++) {
        reverb->lastAllpassMaxLength[ch] = __FilterSizeTable[6][5 + ch];
    }

    result = __AllocDelayLine(reverb);
    if (result == FALSE) {
        AXFXReverbHiExpShutdown(reverb);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    __BzeroDelayLines(reverb);
    result = __InitParams(reverb);
    if (result == FALSE) {
        AXFXReverbHiExpShutdown(reverb);
        OSRestoreInterrupts(mask);
        return FALSE;
    }

    reverb->active &= ~1;
    OSRestoreInterrupts(mask);
    return TRUE;
}

void AXFXReverbHiExpShutdown(AXFX_REVERBHI_EXP* reverb) {
    BOOL mask = OSDisableInterrupts();
    reverb->active |= 1;
    __FreeDelayLine(reverb);
    OSRestoreInterrupts(mask);
}

void AXFXReverbHiExpCallback(AXFX_BUFFERUPDATE* bufferUpdate, AXFX_REVERBHI_EXP* reverb) {
    u32 ch, i;
    u32 samp;
    s32* input[3];
    f32 data;
    f32 output[3];
    f32* earlyLine;
    f32 earlyOut;
    f32* preDelayLine;
    f32 preDelayOut;
    f32 filterOut;
    f32* combLine;
    f32 combOutOne;
    f32* allpass;
    f32 outTmp;
    f32 allpassIn;
    f32 allpassCoef;
    f32 lpfOut;
    f32 lpfCoef1;
    f32 lpfCoef2;
    f32 fusedOut[3];
    f32 fusedGain;
    f32 crosstalkGain;
    f32 crosstalkL;
    f32 crosstalkR;
    f32 crosstalkS;
    s32* inBusData[3];
    s32* outBusData[3];

    if (reverb->active != 0) {
        reverb->active &= ~2;
        return;
    }

    input[0] = bufferUpdate->left;
    input[1] = bufferUpdate->right;
    input[2] = bufferUpdate->surround;

    if (reverb->busIn != NULL) {
        inBusData[0] = reverb->busIn->left;
        inBusData[1] = reverb->busIn->right;
        inBusData[2] = reverb->busIn->surround;
    }

    if (reverb->busOut != NULL) {
        outBusData[0] = reverb->busOut->left;
        outBusData[1] = reverb->busOut->right;
        outBusData[2] = reverb->busOut->surround;
    }

    lpfCoef1 = 1.0f - reverb->lpfCoef;
    lpfCoef2 = reverb->lpfCoef;
    allpassCoef = reverb->allpassCoef;
    fusedGain = reverb->fusedGain * 0.6f;
    crosstalkGain = reverb->crosstalk * 0.5f;

    for (samp = 0; samp < 96; samp++) {
        for (ch = 0; ch < 3; ch++) {
            if (reverb->busIn != NULL) {
                data = (f32)(*(input[ch]) + *(inBusData[ch]++));
            } else {
                data = (f32)(*input[ch]);
            }

            earlyLine = reverb->earlyLine[ch];
            earlyOut = earlyLine[reverb->earlyPos[0]] * reverb->earlyCoef[0] +
                       earlyLine[reverb->earlyPos[1]] * reverb->earlyCoef[1] +
                       earlyLine[reverb->earlyPos[2]] * reverb->earlyCoef[2];

#if SDK_AUG2010
            earlyLine[reverb->earlyPos[2]] = data;
#endif

            if (reverb->preDelayLength != 0) {
                preDelayLine = reverb->preDelayLine[ch];
                preDelayOut = preDelayLine[reverb->preDelayPos];
                preDelayLine[reverb->preDelayPos] = data;
            } else {
                preDelayOut = data;
            }

            filterOut = 0.0f;
            for (i = 0; i < 3; i++) {
                combLine = reverb->combLine[ch][i];
                combOutOne = combLine[reverb->combPos[i]];
                combLine[reverb->combPos[i]] = preDelayOut + (combOutOne * reverb->combCoef[i]);
                filterOut += combOutOne;
            }

            for (i = 0; i < 2; i++) {
                allpass = reverb->allpassLine[ch][i];
                outTmp = allpass[reverb->allpassPos[i]];
                allpassIn = filterOut + outTmp * allpassCoef;
                allpass[reverb->allpassPos[i]] = allpassIn;
                filterOut = outTmp - allpassIn * allpassCoef;
            }

            lpfOut = lpfCoef1 * filterOut + lpfCoef2 * reverb->lastLpfOut[ch];
            reverb->lastLpfOut[ch] = lpfOut;
            allpass = reverb->lastAllpassLine[ch];
            outTmp = allpass[reverb->lastAllpassPos[ch]];
            allpassIn = lpfOut + outTmp * allpassCoef;
            allpass[reverb->lastAllpassPos[ch]] = allpassIn;
            fusedOut[ch] = outTmp - allpassIn * allpassCoef;
            if (++reverb->lastAllpassPos[ch] >= reverb->lastAllpassLength[ch]) {
                reverb->lastAllpassPos[ch] = 0;
            }

            fusedOut[ch] *= fusedGain;
            fusedOut[ch] += earlyOut;
        }

        crosstalkL = fusedOut[1] + fusedOut[2];
        crosstalkR = fusedOut[0] + fusedOut[2];
        crosstalkS = fusedOut[0] + fusedOut[1];

        output[0] = fusedOut[0] + crosstalkL * crosstalkGain;
        output[1] = fusedOut[1] + crosstalkR * crosstalkGain;
        output[2] = fusedOut[2] + crosstalkS * crosstalkGain;

        *(input[0]++) = (s32)(output[0] * reverb->outGain);
        *(input[1]++) = (s32)(output[1] * reverb->outGain);
        *(input[2]++) = (s32)(output[2] * reverb->outGain);

        if (reverb->busOut != NULL) {
            *(outBusData[0]++) = (s32)(output[0] * reverb->sendGain);
            *(outBusData[1]++) = (s32)(output[1] * reverb->sendGain);
            *(outBusData[2]++) = (s32)(output[2] * reverb->sendGain);
        }

        for (i = 0; i < 3; i++) {
            if (++reverb->earlyPos[i] >= reverb->earlyLength) {
                reverb->earlyPos[i] = 0;
            }
        }

        if (reverb->preDelayLength != 0) {
            if (++reverb->preDelayPos >= reverb->preDelayLength) {
                reverb->preDelayPos = 0;
            }
        }

        for (i = 0; i < 3; i++) {
            if (++reverb->combPos[i] >= reverb->combLength[i]) {
                reverb->combPos[i] = 0;
            }
        }

        for (i = 0; i < 2; i++) {
            if (++reverb->allpassPos[i] >= reverb->allpassLength[i]) {
                reverb->allpassPos[i] = 0;
            }
        }
    }
}

static BOOL __AllocDelayLine(AXFX_REVERBHI_EXP* reverb) {
    u32 ch, i;

    for (ch = 0; ch < 3; ch++) {
        reverb->earlyLine[ch] = (f32*)__AXFXAlloc(sizeof(f32) * reverb->earlyMaxLength);
        if (reverb->earlyLine[ch] == NULL)
            return FALSE;

        if (reverb->preDelayMaxLength != 0) {
            reverb->preDelayLine[ch] = (f32*)__AXFXAlloc(sizeof(f32) * reverb->preDelayMaxLength);
            if (reverb->preDelayLine[ch] == NULL)
                return FALSE;
        } else {
            reverb->preDelayLine[ch] = NULL;
        }

        for (i = 0; i < 3; i++) {
            reverb->combLine[ch][i] = (f32*)__AXFXAlloc(sizeof(f32) * reverb->combMaxLength[i]);
            if (reverb->combLine[ch][i] == NULL)
                return FALSE;
        }

        for (i = 0; i < 2; i++) {
            reverb->allpassLine[ch][i] = (f32*)__AXFXAlloc(sizeof(f32) * reverb->allpassMaxLength[i]);
            if (reverb->allpassLine[ch][i] == NULL)
                return FALSE;
        }

        reverb->lastAllpassLine[ch] = (f32*)__AXFXAlloc(sizeof(f32) * reverb->lastAllpassMaxLength[ch]);
        if (reverb->lastAllpassLine[ch] == NULL)
            return FALSE;
    }

    return TRUE;
}

static void __BzeroDelayLines(AXFX_REVERBHI_EXP* reverb) {
    u32 ch, i;

    for (ch = 0; ch < 3; ch++) {
        if (reverb->earlyLine[ch] != NULL) {
            memset(reverb->earlyLine[ch], 0, sizeof(f32) * reverb->earlyMaxLength);
        }

        if (reverb->preDelayLine[ch] != NULL) {
            memset(reverb->preDelayLine[ch], 0, sizeof(f32) * reverb->preDelayMaxLength);
        }

        for (i = 0; i < 3; i++) {
            if (reverb->combLine[ch][i] != NULL) {
                memset(reverb->combLine[ch][i], 0, sizeof(f32) * reverb->combMaxLength[i]);
            }
        }

        for (i = 0; i < 2; i++) {
            if (reverb->allpassLine[ch][i] != NULL) {
                memset(reverb->allpassLine[ch][i], 0, sizeof(f32) * reverb->allpassMaxLength[i]);
            }
        }

        if (reverb->lastAllpassLine[ch] != NULL) {
            memset(reverb->lastAllpassLine[ch], 0, sizeof(f32) * reverb->lastAllpassMaxLength[ch]);
        }
    }
}

static void __FreeDelayLine(AXFX_REVERBHI_EXP* reverb) {
    u32 ch, i;

    for (ch = 0; ch < 3; ch++) {
        if (reverb->earlyLine[ch] != NULL) {
            __AXFXFree(reverb->earlyLine[ch]);
            reverb->earlyLine[ch] = NULL;
        }

        if (reverb->preDelayLine[ch] != NULL) {
            __AXFXFree(reverb->preDelayLine[ch]);
            reverb->preDelayLine[ch] = NULL;
        }

        for (i = 0; i < 3; i++) {
            if (reverb->combLine[ch][i] != NULL) {
                __AXFXFree(reverb->combLine[ch][i]);
                reverb->combLine[ch][i] = NULL;
            }
        }

        for (i = 0; i < 2; i++) {
            if (reverb->allpassLine[ch][i] != NULL) {
                __AXFXFree(reverb->allpassLine[ch][i]);
                reverb->allpassLine[ch][i] = NULL;
            }
        }

        if (reverb->lastAllpassLine[ch] != NULL) {
            __AXFXFree(reverb->lastAllpassLine[ch]);
            reverb->lastAllpassLine[ch] = NULL;
        }
    }
}

f32 dummy_1() {
    return -3.0f;
}

static BOOL __InitParams(AXFX_REVERBHI_EXP* reverb) {
    u32 ch, i;

    if (reverb->earlyMode >= 8)
        return FALSE;

    if (reverb->preDelayTime < 0.0f || reverb->preDelayTime > reverb->preDelayTimeMax)
        return FALSE;

    if (reverb->fusedMode >= 6)
        return FALSE;

    if (reverb->fusedTime < 0.0f)
        return FALSE;

    if (reverb->coloration < 0.0f || reverb->coloration > 1.0f)
        return FALSE;

    if (reverb->damping < 0.0f || reverb->damping > 1.0f)
        return FALSE;

    if (reverb->crosstalk < 0.0f || reverb->crosstalk > 1.0f)
        return FALSE;

    if (reverb->earlyGain < 0.0f || reverb->earlyGain > 1.0f)
        return FALSE;

    if (reverb->fusedGain < 0.0f || reverb->fusedGain > 1.0f)
        return FALSE;

    if (reverb->outGain < 0.0f || reverb->outGain > 1.0f)
        return FALSE;

    if (reverb->sendGain < 0.0f || reverb->sendGain > 1.0f)
        return FALSE;

    reverb->earlyLength = __EarlySizeTable[reverb->earlyMode][2];
    for (i = 0; i < 3; i++) {
        reverb->earlyPos[i] = reverb->earlyLength - __EarlySizeTable[reverb->earlyMode][i];
        reverb->earlyCoef[i] = __EarlyCoefTable[reverb->earlyMode][i] * reverb->earlyGain * 0.6f;
    }

    reverb->preDelayPos = 0;
    reverb->preDelayLength = (u32)(reverb->preDelayTime * (f32)32000);

    for (i = 0; i < 3; i++) {
        reverb->combPos[i] = 0;
        reverb->combLength[i] = __FilterSizeTable[reverb->fusedMode][i];
        reverb->combCoef[i] = pow(10.0f, (-3.0f * (f32)(reverb->combLength[i]) / (f32)(reverb->fusedTime * 32000)));
    }

    for (i = 0; i < 2; i++) {
        reverb->allpassPos[i] = 0;
        reverb->allpassLength[i] = __FilterSizeTable[reverb->fusedMode][3 + i];
    }

    for (ch = 0; ch < 3; ch++) {
        reverb->lastAllpassPos[ch] = 0;
        reverb->lastAllpassLength[ch] = __FilterSizeTable[reverb->fusedMode][5 + ch];
    }

    reverb->allpassCoef = reverb->coloration;
    reverb->lpfCoef = 1.0f - reverb->damping;
    if (reverb->lpfCoef > 0.95f)
        reverb->lpfCoef = 0.95f;

    for (ch = 0; ch < 3; ch++) {
        reverb->lastLpfOut[ch] = 0.0f;
    }

    return TRUE;
}
