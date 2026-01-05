#include <dolphin/dolphin.h>
#include <dolphin/syn.h>
#include "sdk_math.h"

#include "__syn.h"

int __SYNGetWavetableData(SYNVOICE* voice) {
    u32 regionIndex;
    SYNSYNTH* synth;

    synth = voice->synth;
    regionIndex = synth->inst[voice->midiChannel]->keyRegion[voice->keyNum];
    if (regionIndex == 0xFFFF) {
        return 0;
    }
    voice->region = &synth->region[regionIndex];
    voice->art = &synth->art[voice->region->articulationIndex];
    voice->sample = &synth->sample[voice->region->sampleIndex];
    voice->adpcm = &synth->adpcm[voice->sample->adpcmIndex];
    return 1;
}
