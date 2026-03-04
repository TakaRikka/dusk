#include <dolphin/dolphin.h>
#include <dolphin/ax.h>
#include <dolphin/syn.h>
#include "sdk_math.h"

#include "__syn.h"

static f32 __SYNLfo[64] = {
    0.000000f,
    0.098020f,
    0.195090f,
    0.290280f,
    0.382680f,
    0.471400f,
    0.555570f,
    0.634390f,
    0.707110f,
    0.773010f,
    0.831470f,
    0.881920f,
    0.923880f,
    0.956940f,
    0.980790f,
    0.995180f,
    1.000000f,
    0.995180f,
    0.980790f,
    0.956940f,
    0.923880f,
    0.881920f,
    0.831470f,
    0.773010f,
    0.707110f,
    0.634390f,
    0.555570f,
    0.471400f,
    0.382680f,
    0.290280f,
    0.195090f,
    0.098020f,
    0.000000f,
    -0.098020f,
    -0.195090f,
    -0.290280f,
    -0.382680f,
    -0.471400f,
    -0.555570f,
    -0.634390f,
    -0.707110f,
    -0.773010f,
    -0.831470f,
    -0.881920f,
    -0.923880f,
    -0.956940f,
    -0.980790f,
    -0.995180f,
    -1.000000f,
    -0.995180f,
    -0.980790f,
    -0.956940f,
    -0.923880f,
    -0.881920f,
    -0.831470f,
    -0.773010f,
    -0.707110f,
    -0.634390f,
    -0.555570f,
    -0.471400f,
    -0.382680f,
    -0.290280f,
    -0.195090f,
    -0.098020f,
};

void __SYNSetupLfo(SYNVOICE* voice) {
    ASSERTLINE(47, voice);

    voice->lfoState = voice->lfoAttn = voice->lfoCents = 0;
    voice->lfoFreq = voice->art->lfoFreq;
    voice->lfoDelay = voice->art->lfoDelay;
    voice->lfoAttn_ = voice->art->lfoAtten;
    voice->lfoCents = voice->art->lfoPitch;
    voice->lfoModAttn = voice->art->lfoMod2Atten;
    voice->lfoModCents = voice->art->lfoMod2Pitch;
}

void __SYNRunLfo(SYNVOICE* voice) {
    f32 lfoAmplitude;
    f32 lfoModWheel;

    ASSERTLINE(66, voice);

    if (voice->lfoDelay != 0) {
        voice->lfoDelay--;
    } else {
        voice->lfoState += voice->lfoFreq;
        lfoAmplitude = __SYNLfo[(voice->lfoState >> 0x10) % 64];
        lfoModWheel = __SYNn128[voice->synth->controller[voice->midiChannel][1]];
        voice->lfoAttn = (lfoAmplitude * (voice->lfoAttn_ + (voice->lfoModAttn* lfoModWheel)));
        voice->lfoCents = (lfoAmplitude * (voice->lfoCents_ + (voice->lfoModCents* lfoModWheel)));
    }
}
