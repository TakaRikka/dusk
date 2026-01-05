#include <dolphin/dolphin.h>
#include <dolphin/os.h>
#include <dolphin/ax.h>
#include <dolphin/mix.h>
#include <dolphin/syn.h>
#include "sdk_math.h"

#include "__syn.h"

SYNSYNTH* __SYNSynthList;

// prototypes
static void __SYNAddSynthToList(SYNSYNTH* synth);
static void __SYNRemoveSynthFromList(SYNSYNTH* synth);

static void __SYNAddSynthToList(SYNSYNTH* synth) {
    BOOL old = OSDisableInterrupts();

    if (__SYNSynthList) {
        synth->next = __SYNSynthList;
    } else {
        synth->next = NULL;
    }
    __SYNSynthList = synth;
    OSRestoreInterrupts(old);
}

static void __SYNRemoveSynthFromList(SYNSYNTH* synth) {
    SYNSYNTH* tempHead;
    SYNSYNTH* tempTail;
    SYNSYNTH* tempSynth;
    BOOL old;

    old = OSDisableInterrupts();
    tempHead = NULL;
    tempTail = NULL;
    
    for (tempSynth = __SYNSynthList; tempSynth; tempSynth = tempSynth->next) {
        if (tempSynth != synth) {
            if (tempHead) {
                tempTail->next = tempSynth;
                tempTail = tempSynth;
            } else {
                tempHead = tempTail = tempSynth;
            }
        }
    }

    if (tempTail) {
        tempTail->next = NULL;
    }

    __SYNSynthList = tempHead;
    OSRestoreInterrupts(old);
}

void SYNInit(void) {
    int i;

    for (i = 0; i < 64; i++) {
        __SYNVoice[i].synth = 0;
    }
    __SYNSynthList = NULL;
}

void SYNQuit(void) {
    SYNInit();
}

void SYNRunAudioFrame(void) {
    int i;
    SYNSYNTH* synth;

    for (i = 0; i < 64; i++) {
        __SYNServiceVoice(i);
    }

    for (synth = __SYNSynthList; synth; synth = synth->next) {
        __SYNRunInputBufferEvents(synth);
    }
}

void SYNInitSynth(SYNSYNTH* synth, void* wavetable, u32 aramBase, u32 zeroBase, u32 priorityVoiceAlloc, u32 priorityNoteOn, u32 priorityNoteRelease) {
    u32* p;
    u32 midiChannel;
    u32 noteNumber;

    ASSERTLINE(158, synth);
    ASSERTLINE(159, wavetable);
    ASSERTLINE(160, aramBase);

    p = wavetable;
    synth->percussiveInst = (void*)((u32)wavetable + *(p)); p += 1;
    synth->melodicInst = (void*)((u32)wavetable + *(p)); p += 1;
    synth->region = (void*)((u32)wavetable + *(p)); p += 1;
    synth->art = (void*)((u32)wavetable + *(p)); p += 1;
    synth->sample = (void*)((u32)wavetable + *(p)); p += 1;
    synth->adpcm = (void*)((u32)wavetable + *(p)); p += 1;
    synth->aramBaseWord = (aramBase >> 1);
    synth->aramBaseByte = aramBase;
    synth->aramBaseNibble = (aramBase << 1);
    synth->zeroBaseWord = zeroBase >> 1;
    synth->zeroBaseByte = zeroBase;
    synth->zeroBaseNibble = (zeroBase << 1);
    synth->priorityVoiceAlloc = priorityVoiceAlloc;
    synth->priorityNoteOn = priorityNoteOn;
    synth->priorityNoteRelease = priorityNoteRelease;
    synth->masterVolume = 0;
    __SYNResetAllControllers(synth);
    synth->inputPosition = &synth->input[0][0];
    synth->inputCounter = 0;
    synth->notes = 0;

    for (midiChannel = 0; midiChannel < 16; midiChannel++) {
        for (noteNumber = 0; noteNumber < 16; noteNumber++) {
            synth->keyGroup[midiChannel][noteNumber] = 0;
        }
    }

    for (midiChannel = 0; midiChannel < 16; midiChannel++) {
        for (noteNumber = 0; noteNumber < 128; noteNumber++) {
            synth->voice[midiChannel][noteNumber] = 0;
        }
    }

    __SYNAddSynthToList(synth);
}

void SYNQuitSynth(SYNSYNTH* synth) {
    int i;
    BOOL old;
    SYNVOICE* voice;

    old = OSDisableInterrupts();
    if (synth->notes) {
        for (i = 0; i < 64; i++) {
            voice = &__SYNVoice[i];
            if (voice->synth == synth) {
                MIXReleaseChannel(voice->axvpb);
                AXFreeVoice(voice->axvpb);
                voice->synth = 0;
            }
        }
    }

    __SYNRemoveSynthFromList(synth);
    OSRestoreInterrupts(old);
}

void SYNMidiInput(SYNSYNTH* synth, u8* input) {
    u8* src;

    ASSERTLINE(244, synth);
    ASSERTLINE(245, input);

    src = input;
    *(synth->inputPosition) = *(src); (synth->inputPosition) += 1; (src) += 1;
    *(synth->inputPosition) = *(src); (synth->inputPosition) += 1; (src) += 1;
    *(synth->inputPosition) = *(src); (synth->inputPosition) += 1; (src) += 1;
    synth->inputCounter++;

    if (synth->inputCounter >= SYN_INPUT_BUFFER_SIZE) {
        ASSERTMSGLINE(258, FALSE, "synth input buffer exceeded, increase SYN_INPUT_BUFFER_SIZE");
    }
}

void SYNSetMasterVolume(SYNSYNTH* synth, s32 dB) {
    ASSERTLINE(267, synth);
    synth->masterVolume = (dB << 0x10);
}

s32 SYNGetMasterVolume(SYNSYNTH* synth) {
    ASSERTLINE(278, synth);
    return synth->masterVolume >> 0x10;
}

u32 SYNGetActiveNotes(SYNSYNTH* synth) {
    ASSERTLINE(289, synth);
    return synth->notes;
}
