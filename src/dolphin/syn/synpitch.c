#include <dolphin/dolphin.h>
#include <dolphin/syn.h>
#include "sdk_math.h"

#include "__syn.h"

static f32 __SYNCentsTable[100] = {
    1.000000f,
    1.000578f,
    1.001156f,
    1.001734f,
    1.002313f,
    1.002892f,
    1.003472f,
    1.004052f,
    1.004632f,
    1.005212f,
    1.005793f,
    1.006374f,
    1.006956f,
    1.007537f,
    1.008120f,
    1.008702f,
    1.009285f,
    1.009868f,
    1.010451f,
    1.011035f,
    1.011619f,
    1.012204f,
    1.012789f,
    1.013374f,
    1.013959f,
    1.014545f,
    1.015132f,
    1.015718f,
    1.016305f,
    1.016892f,
    1.017480f,
    1.018068f,
    1.018656f,
    1.019244f,
    1.019833f,
    1.020423f,
    1.021012f,
    1.021602f,
    1.022192f,
    1.022783f,
    1.023374f,
    1.023965f,
    1.024557f,
    1.025149f,
    1.025741f,
    1.026334f,
    1.026927f,
    1.027520f,
    1.028114f,
    1.028708f,
    1.029302f,
    1.029897f,
    1.030492f,
    1.031087f,
    1.031683f,
    1.032279f,
    1.032876f,
    1.033472f,
    1.034070f,
    1.034667f,
    1.035265f,
    1.035863f,
    1.036462f,
    1.037060f,
    1.037660f,
    1.038259f,
    1.038859f,
    1.039459f,
    1.040060f,
    1.040661f,
    1.041262f,
    1.041864f,
    1.042466f,
    1.043068f,
    1.043671f,
    1.044274f,
    1.044877f,
    1.045481f,
    1.046085f,
    1.046689f,
    1.047294f,
    1.047899f,
    1.048505f,
    1.049111f,
    1.049717f,
    1.050323f,
    1.050930f,
    1.051537f,
    1.052145f,
    1.052753f,
    1.053361f,
    1.053970f,
    1.054579f,
    1.055188f,
    1.055798f,
    1.056408f,
    1.057018f,
    1.057629f,
    1.058240f,
    1.058851f,
};

static f32 __SYNOctavesTableUp[12] = {
    1.000000f,
    2.000000f,
    4.000000f,
    8.000000f,
    16.000000f,
    32.000000f,
    64.000000f,
    128.000000f,
    256.000000f,
    512.000000f,
    1024.000000f,
    2048.000000f,
};

static f32 __SYNSemitonesTableUp[12] = {
    1.000000f,
    1.059463f,
    1.122462f,
    1.189207f,
    1.259921f,
    1.334840f,
    1.414214f,
    1.498307f,
    1.587401f,
    1.681793f,
    1.781797f,
    1.887749f,
};

static f32 __SYNSemitonesTableDown[128] = {
    1.000000f,
    0.943874f,
    0.890899f,
    0.840896f,
    0.793701f,
    0.749154f,
    0.707107f,
    0.667420f,
    0.629961f,
    0.594604f,
    0.561231f,
    0.529732f,
    0.500000f,
    0.471937f,
    0.445449f,
    0.420448f,
    0.396850f,
    0.374577f,
    0.353553f,
    0.333710f,
    0.314980f,
    0.297302f,
    0.280616f,
    0.264866f,
    0.250000f,
    0.235969f,
    0.222725f,
    0.210224f,
    0.198425f,
    0.187288f,
    0.176777f,
    0.166855f,
    0.157490f,
    0.148651f,
    0.140308f,
    0.132433f,
    0.125000f,
    0.117984f,
    0.111362f,
    0.105112f,
    0.099213f,
    0.093644f,
    0.088388f,
    0.083427f,
    0.078745f,
    0.074325f,
    0.070154f,
    0.066216f,
    0.062500f,
    0.058992f,
    0.055681f,
    0.052556f,
    0.049606f,
    0.046822f,
    0.044194f,
    0.041714f,
    0.039373f,
    0.037163f,
    0.035077f,
    0.033108f,
    0.031250f,
    0.029496f,
    0.027841f,
    0.026278f,
    0.024803f,
    0.023411f,
    0.022097f,
    0.020857f,
    0.019686f,
    0.018581f,
    0.017538f,
    0.016554f,
    0.015625f,
    0.014748f,
    0.013920f,
    0.013139f,
    0.012402f,
    0.011706f,
    0.011049f,
    0.010428f,
    0.009843f,
    0.009291f,
    0.008769f,
    0.008277f,
    0.007813f,
    0.007374f,
    0.006960f,
    0.006570f,
    0.006201f,
    0.005853f,
    0.005524f,
    0.005214f,
    0.004922f,
    0.004645f,
    0.004385f,
    0.004139f,
    0.003906f,
    0.003687f,
    0.003480f,
    0.003285f,
    0.003100f,
    0.002926f,
    0.002762f,
    0.002607f,
    0.002461f,
    0.002323f,
    0.002192f,
    0.002069f,
    0.001953f,
    0.001844f,
    0.001740f,
    0.001642f,
    0.001550f,
    0.001463f,
    0.001381f,
    0.001304f,
    0.001230f,
    0.001161f,
    0.001096f,
    0.001035f,
    0.000977f,
    0.000922f,
    0.000870f,
    0.000821f,
    0.000775f,
    0.000732f,
    0.000691f,
    0.000652f,
};

f32 __SYNGetRelativePitch(SYNVOICE* voice) {
    s32 cents;

    cents = voice->cents + voice->lfoCents + voice->peCents + voice->synth->pwCents[voice->midiChannel];
    cents = cents / 65536;

    if (cents > 0) {
        s32 octaves = (cents / 1200);
        s32 semitones = (cents % 1200) / 100;
        cents = (cents % 100);
        return __SYNOctavesTableUp[octaves] * __SYNSemitonesTableUp[semitones] * __SYNCentsTable[cents];
    }

    if (cents < 0) {
        s32 semitones = cents / 100;
        cents = (cents % 100);
        if (cents != 0) {
            cents += 100;
            semitones-=1;
        }
        semitones *= -1;
        return __SYNSemitonesTableDown[semitones] * __SYNCentsTable[cents];
    }

    return 1.0f;
}

void __SYNSetupPitch(SYNVOICE* voice) {
    voice->srcRatio = (voice->sample->sampleRate / 32000.0f);
    voice->cents = (voice->keyNum - voice->region->unityNote) * 100;
    voice->cents = (voice->cents + voice->region->fineTune);
    voice->cents = (voice->cents << 0x10);
}

void __SYNSetupSrc(SYNVOICE* voice) {
    f32 srcRatio;
    u32 value;
    u16* p;

    srcRatio = voice->srcRatio* __SYNGetRelativePitch(voice);
    if (srcRatio > 4.0f) {
        value = 0x40000;
    } else {
        value = (65536.0f* srcRatio);
    }

    voice->axvpb->pb.srcSelect = 1;
    p = (void*)&voice->axvpb->pb.src;
    *(p) = (value >> 0x10); p += 1;
    *(p) = (value); p += 1;
    *(p) = 0; p += 1;
    *(p) = 0; p += 1;
    *(p) = 0; p += 1;
    *(p) = 0; p += 1;
    *(p) = 0; p += 1;

    voice->axvpb->sync &= 0xFFF7FFFF;
    voice->axvpb->sync |= 0x40001;
}

void __SYNUpdateSrc(SYNVOICE* voice) {
    u32 ratio = (65536.0f * (voice->srcRatio * __SYNGetRelativePitch(voice)));

    if (ratio > 0x40000) {
        ratio = 0x40000;
    }

    *(u32*)&voice->axvpb->pb.src.ratioHi = ratio;
    voice->axvpb->sync |= 0x80000;
}
