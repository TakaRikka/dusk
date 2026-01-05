#include <dolphin/dolphin.h>
#include <dolphin/ax.h>
#include <dolphin/mix.h>
#include <dolphin/syn.h>
#include "sdk_math.h"

#include "__syn.h"

#define AX_MAX_VOICES 64

SYNVOICE __SYNVoice[64];

void __SYNClearVoiceReferences(void* p) {
    AXVPB* axvpb;
    SYNSYNTH* synth;
    SYNVOICE* voice;

    ASSERTLINE(33, p);

    axvpb = p;
    synth = (void*)axvpb->userContext;
    voice = &__SYNVoice[axvpb->index];

    ASSERTLINE(38, synth);
    ASSERTLINE(39, axvpb->index < AX_MAX_VOICES);
    MIXReleaseChannel(axvpb);

    if (voice->keyGroup) {
        synth->keyGroup[voice->midiChannel][voice->keyGroup] = 0;
    }
    if (synth->voice[voice->midiChannel][voice->keyNum] == voice) {
        synth->voice[voice->midiChannel][voice->keyNum] = 0;
    }

    voice->synth = 0;
    synth->notes--;
}

void __SYNSetVoiceToRelease(SYNVOICE* voice, u32 priority) {
    ASSERTLINE(63, voice);
    voice->veState = 3;
    voice->peState = 3;
    AXSetVoicePriority(voice->axvpb, priority);
}

void __SYNServiceVoice(int i) {
    SYNVOICE* voice;
    SYNSYNTH* synth;

    voice = &__SYNVoice[i];
    synth = voice->synth;

    if (synth != NULL) {
        if ((voice->type == 0) && (voice->axvpb->pb.state == 0)) {
            if (voice->keyGroup != 0) {
                voice->synth->keyGroup[voice->midiChannel][voice->keyGroup] = 0;
            }
            if (synth->voice[voice->midiChannel][voice->keyNum] == voice) {
                synth->voice[voice->midiChannel][voice->keyNum] = 0;
            }
            voice->veState = 4;
        }

        __SYNRunVolumeEnvelope(voice);

        if (voice->veState == 4) {
            if (voice->keyGroup != 0) {
                voice->synth->keyGroup[voice->midiChannel][voice->keyGroup] = 0;
            }
            voice->synth = NULL;
            MIXReleaseChannel(voice->axvpb);
            AXFreeVoice(voice->axvpb);
            synth->notes--;
            return;
        }

        __SYNRunLfo(voice);
        __SYNRunPitchEnvelope(voice);
        __SYNUpdateMix(voice);
        __SYNUpdateSrc(voice);
    }
}
