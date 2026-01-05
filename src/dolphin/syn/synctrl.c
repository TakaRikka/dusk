#include <dolphin/dolphin.h>
#include <dolphin/ax.h>
#include <dolphin/mix.h>
#include <dolphin/syn.h>
#include "sdk_math.h"

#include "__syn.h"

f32 __SYNn128[128] = {
    0.000000f,
    0.007813f,
    0.015625f,
    0.023438f,
    0.031250f,
    0.039063f,
    0.046875f,
    0.054688f,
    0.062500f,
    0.070313f,
    0.078125f,
    0.085938f,
    0.093750f,
    0.101563f,
    0.109375f,
    0.117188f,
    0.125000f,
    0.132813f,
    0.140625f,
    0.148438f,
    0.156250f,
    0.164063f,
    0.171875f,
    0.179688f,
    0.187500f,
    0.195313f,
    0.203125f,
    0.210938f,
    0.218750f,
    0.226563f,
    0.234375f,
    0.242188f,
    0.250000f,
    0.257813f,
    0.265625f,
    0.273438f,
    0.281250f,
    0.289063f,
    0.296875f,
    0.304688f,
    0.312500f,
    0.320313f,
    0.328125f,
    0.335938f,
    0.343750f,
    0.351563f,
    0.359375f,
    0.367188f,
    0.375000f,
    0.382813f,
    0.390625f,
    0.398438f,
    0.406250f,
    0.414063f,
    0.421875f,
    0.429688f,
    0.437500f,
    0.445313f,
    0.453125f,
    0.460938f,
    0.468750f,
    0.476563f,
    0.484375f,
    0.492188f,
    0.500000f,
    0.507813f,
    0.515625f,
    0.523438f,
    0.531250f,
    0.539063f,
    0.546875f,
    0.554688f,
    0.562500f,
    0.570313f,
    0.578125f,
    0.585938f,
    0.593750f,
    0.601563f,
    0.609375f,
    0.617188f,
    0.625000f,
    0.632813f,
    0.640625f,
    0.648438f,
    0.656250f,
    0.664063f,
    0.671875f,
    0.679688f,
    0.687500f,
    0.695313f,
    0.703125f,
    0.710938f,
    0.718750f,
    0.726563f,
    0.734375f,
    0.742188f,
    0.750000f,
    0.757813f,
    0.765625f,
    0.773438f,
    0.781250f,
    0.789063f,
    0.796875f,
    0.804688f,
    0.812500f,
    0.820313f,
    0.828125f,
    0.835938f,
    0.843750f,
    0.851563f,
    0.859375f,
    0.867188f,
    0.875000f,
    0.882813f,
    0.890625f,
    0.898438f,
    0.906250f,
    0.914063f,
    0.921875f,
    0.929688f,
    0.937500f,
    0.945313f,
    0.953125f,
    0.960938f,
    0.968750f,
    0.976563f,
    0.984375f,
    0.992188f
};

// prototypes
static void __SYNSetData(SYNSYNTH* synth, u8 midiChannel);
static void __SYNSetSustainPedal(SYNSYNTH* synth, u8 midiChannel, u8 data);
static void __SYNProgramChange(SYNSYNTH* synth, u8 midiChannel, u8 program);
static void __SYNReleaseChannelNotes(SYNSYNTH* synth, u8 midiChannel);
static void __SYNNoteOff(SYNSYNTH* synth, u8 midiChannel, u8 keyNum);
static void __SYNNoteOn(SYNSYNTH* synth, u8 midiChannel, u8 keyNum, u8 keyVel);
static void __SYNPitchWheel(SYNSYNTH* synth, u8 midiChannel, u8 lsb, u8 msb);
static void __SYNMidiIn(SYNSYNTH* synth, u8* input);

static void __SYNSetData(SYNSYNTH* synth, u8 midiChannel) {
    ASSERTLINE(59, synth);
    ASSERTLINE(60, midiChannel < 16);

    if (synth->rpn[midiChannel]) {
        u16 param = (synth->controller[midiChannel][0x65] << 8) + synth->controller[midiChannel][0x64];
        switch(param) {
        case 0:
            synth->pwMaxCents[midiChannel] = (synth->controller[midiChannel][0x26] + (synth->controller[midiChannel][0x6] * 100)) << 0x10;
            break;
        case 1:
            ASSERTMSGLINE(80, FALSE, "RPN 0001 not supported\n");
            break;
        case 2:
            ASSERTMSGLINE(86, FALSE, "RPN 0002 not supported\n");
            break;
        case 3:
            ASSERTMSGLINE(92, FALSE, "RPN 0003 not supported\n");
            break;
        case 4:
            ASSERTMSGLINE(98, FALSE, "RPN 0004 not supported\n");
            break;
        }
    }
}

static void __SYNSetSustainPedal(SYNSYNTH* synth, u8 midiChannel, u8 data) {
    int i;
    SYNVOICE* voice;

    ASSERTLINE(111, synth);
    ASSERTLINE(112, midiChannel < 16);
    ASSERTLINE(113, data < 128);

    // check if you're below 0x80 only to check if you're below 0x40. ok then.
    if (data < 64) {
        for (i = 0; i < 128; i++) {
            voice = synth->voice[midiChannel][i];
            if (voice && voice->hold) {
                __SYNSetVoiceToRelease(voice, synth->priorityNoteRelease);
                voice->synth->voice[voice->midiChannel][voice->keyNum] = 0;
            }
        }
    }
}

static void __SYNProgramChange(SYNSYNTH* synth, u8 midiChannel, u8 program) {
    ASSERTLINE(142, synth);
    ASSERTLINE(143, midiChannel < 16);
    ASSERTLINE(144, program < 128);

    if (midiChannel == 9) {
        synth->inst[midiChannel] = synth->percussiveInst;
    } else {
        synth->inst[midiChannel] = synth->melodicInst;
    }

    synth->inst[midiChannel] += program;
}

static void __SYNReleaseChannelNotes(SYNSYNTH* synth, u8 midiChannel) {
    int i;
    SYNVOICE* voice;

    ASSERTLINE(162, synth);
    ASSERTLINE(163, midiChannel < 16);

    for (i = 0; i < 128; i++) {
        voice = synth->voice[midiChannel][i];
        if (voice) {
            __SYNSetVoiceToRelease(voice, synth->priorityNoteRelease);
            synth->voice[midiChannel][i] = 0;
        }
    }
}

void __SYNClearAllNotes(SYNSYNTH* synth) {
    u8 i;

    ASSERTLINE(189, synth);

    for (i = 0; i < 16; i++) {
        __SYNReleaseChannelNotes(synth, i);
    }
}

void __SYNSetController(SYNSYNTH* synth, u8 midiChannel, u8 function, u8 value) {
    ASSERTLINE(201, synth);
    ASSERTLINE(202, midiChannel < 16);
    ASSERTLINE(203, function < 128);
    ASSERTLINE(204, value < 128);

    synth->controller[midiChannel][function] = value;
    switch(function) {
    case 6:
        __SYNSetData(synth, midiChannel);
        break;
    case 7:
        synth->volAttn[midiChannel] = __SYNVolumeAttenuation[value];
        break;
    case 11:
        synth->expAttn[midiChannel] = __SYNVolumeAttenuation[value];
        break;
    case 0x26:
        __SYNSetData(synth, midiChannel);
        break;
    case 0x40:
        __SYNSetSustainPedal(synth, midiChannel, value);
        break;
    case 0x5B:
        synth->auxAAttn[midiChannel] = __SYNVolumeAttenuation[value];
        break;
    case 0x5C:
        synth->auxBAttn[midiChannel] = __SYNVolumeAttenuation[value];
        break;
    case 0x5D:
        break;
    case 0x62:
    case 0x63:
        synth->rpn[midiChannel] = 0;
        break;
    case 0x64:
    case 0x65:
        synth->rpn[midiChannel] = 1;
        break;
    case 0x78:
        __SYNReleaseChannelNotes(synth, midiChannel);
        break;
    case 0x79:
        if (value == 0) {
            __SYNResetController0(synth, midiChannel);
        } else {
            __SYNResetController(synth, midiChannel);
        }
        break;
    case 0x7B:
    case 0x7C:
    case 0x7D:
    case 0x7E:
    case 0x7F:
        __SYNReleaseChannelNotes(synth, midiChannel);
        break;
    default:
        break;
    }
}

void __SYNResetController0(SYNSYNTH* synth, u8 midiChannel) {
    u8 volume;
    u8 pan;
    u8 expression;
    int i;

    ASSERTLINE(315, synth);
    ASSERTLINE(316, midiChannel < 16);

    synth->pwMaxCents[midiChannel] = 0xC80000;
    synth->pwCents[midiChannel] = 0;
    volume = synth->controller[midiChannel][7];
    pan = synth->controller[midiChannel][10];
    expression = synth->controller[midiChannel][11];

    for (i = 0; i < 128; i++) {
        synth->controller[midiChannel][i] = 0;
    }

    __SYNSetController(synth, midiChannel, 7, volume);
    __SYNSetController(synth, midiChannel, 0xA, pan);
    __SYNSetController(synth, midiChannel, 0xB, expression);
    __SYNSetController(synth, midiChannel, 0x5B, 0);
    __SYNSetController(synth, midiChannel, 0x5C, 0);
}

void __SYNResetController(SYNSYNTH* synth, u8 midiChannel) {
    int i;

    ASSERTLINE(345, synth);
    ASSERTLINE(346, midiChannel < 16);

    synth->pwMaxCents[midiChannel] = 0xC80000;
    synth->pwCents[midiChannel] = 0;

    for (i = 0; i < 128; i++) {
        synth->controller[midiChannel][i] = 0;
    }

    __SYNSetController(synth, midiChannel, 7, 0x64);
    __SYNSetController(synth, midiChannel, 0xA, 0x40);
    __SYNSetController(synth, midiChannel, 0xB, 0x7F);
    __SYNSetController(synth, midiChannel, 0x5B, 0);
    __SYNSetController(synth, midiChannel, 0x5C, 0);
}

void __SYNResetAllControllers(SYNSYNTH* synth) {
    u8 midiChannel;

    ASSERTLINE(372, synth);

    for (midiChannel = 0; midiChannel < 16; midiChannel++) {
        __SYNProgramChange(synth, midiChannel, 0);
        __SYNResetController(synth, midiChannel);
    }
}

static void __SYNNoteOff(SYNSYNTH* synth, u8 midiChannel, u8 keyNum) {
    SYNVOICE* voice;

    ASSERTLINE(389, synth);
    ASSERTLINE(390, midiChannel < 16);
    ASSERTLINE(391, keyNum < 128);

    voice = synth->voice[midiChannel][keyNum];
    if (voice) {
        if (synth->controller[midiChannel][64] > 64) {
            voice->hold = 1;
            return;
        }
        __SYNSetVoiceToRelease(voice, synth->priorityNoteRelease);
        synth->voice[midiChannel][keyNum] = 0;
    }
}

static void __SYNNoteOn(SYNSYNTH* synth, u8 midiChannel, u8 keyNum, u8 keyVel) {
    AXVPB* axvpb;
    SYNVOICE* voice;
    SYNVOICE* oldVoice;

    ASSERTLINE(420, synth);
    ASSERTLINE(421, midiChannel < 16);
    ASSERTLINE(422, keyNum < 128);
    ASSERTLINE(423, keyVel < 128);

    if (keyVel) {
        if (synth->voice[midiChannel][keyNum]) {
            __SYNSetVoiceToRelease(synth->voice[midiChannel][keyNum], synth->priorityNoteRelease);
            synth->voice[midiChannel][keyNum] = 0;
        }

        axvpb = AXAcquireVoice(synth->priorityVoiceAlloc, &__SYNClearVoiceReferences, (u32)synth);
        
        if (axvpb) {
            voice = &__SYNVoice[axvpb->index];
            voice->axvpb = axvpb;
            voice->synth = synth;
            voice->midiChannel = midiChannel;
            voice->keyNum = keyNum;
            voice->keyVel = keyVel;
            voice->hold = 0;

            if (__SYNGetWavetableData(voice) != 0) {
                synth->voice[midiChannel][keyNum] = voice;
                synth->notes++;
                voice->keyGroup = voice->region->keyGroup;
                if (voice->keyGroup != 0) {
                    oldVoice = synth->keyGroup[midiChannel][voice->keyGroup];
                    if (oldVoice) {
                        oldVoice->synth = 0;
                        MIXReleaseChannel(oldVoice->axvpb);
                        AXFreeVoice(oldVoice->axvpb);
                        synth->voice[midiChannel][oldVoice->keyNum] = 0;
                        synth->notes--;
                    }
                    synth->keyGroup[midiChannel][voice->keyGroup] = voice;
                }

                __SYNSetupPitch(voice);
                __SYNSetupVolume(voice);
                __SYNSetupPan(voice);
                __SYNSetupLfo(voice);
                __SYNSetupVolumeEnvelope(voice);
                __SYNSetupPitchEnvelope(voice);

                if (midiChannel == 9) {
                    MIXInitChannel(axvpb, 0, __SYNGetVoiceInput(voice), synth->auxAAttn[midiChannel] >> 0x10, synth->auxBAttn[midiChannel] >> 0x10, voice->pan, 0x7F, __SYNGetVoiceFader(voice));
                } else {
                    MIXInitChannel(axvpb, 0, __SYNGetVoiceInput(voice), synth->auxAAttn[midiChannel] >> 0x10, synth->auxBAttn[midiChannel] >> 0x10, synth->controller[midiChannel][10], 0x7F, __SYNGetVoiceFader(voice));
                }

                __SYNSetupSample(voice);
                __SYNSetupSrc(voice);
                axvpb->pb.state = 1;
                axvpb->sync = (axvpb->sync | 4);
                AXSetVoicePriority(axvpb, synth->priorityNoteOn);
                return;
            }

            voice->synth = NULL;
            MIXReleaseChannel(axvpb);
            AXFreeVoice(axvpb);
        }
    } else {
        __SYNNoteOff(synth, midiChannel, keyNum);
    }
}

static void __SYNPitchWheel(SYNSYNTH* synth, u8 midiChannel, u8 lsb, u8 msb) {
    s32 position;

    ASSERTLINE(565, synth);
    ASSERTLINE(566, midiChannel < 16);
    ASSERTLINE(567, lsb < 128);
    ASSERTLINE(568, msb < 128);

    position = lsb + (msb << 7) - 0x2000;
    synth->pwCents[midiChannel] = (synth->pwMaxCents[midiChannel] * ((f32)position / 8192.0f));
}

static void __SYNMidiIn(SYNSYNTH* synth, u8* input) {
    u8* ch;
    u8 midiFunction;
    u8 midiChannel;
    u8 _2ndByte;
    u8 _3rdByte;

    ASSERTLINE(589, synth);
    ASSERTLINE(590, input);

    ch = input;
    midiFunction = (*(ch) >> 4); ch += 0;
    midiChannel = (*(ch) & 0xF); ch += 1;
    _2ndByte = *(ch); ch += 1;

    switch(midiFunction) {
    case 8:
        __SYNNoteOff(synth, midiChannel, _2ndByte);
        return;
    case 9:
        _3rdByte = *(ch); ch += 1;
        __SYNNoteOn(synth, midiChannel, _2ndByte, _3rdByte);
        return;
    case 11:
        _3rdByte = *(ch); ch += 1;
        __SYNSetController(synth, midiChannel, _2ndByte, _3rdByte);
        return;
    case 12:
        __SYNProgramChange(synth, midiChannel, _2ndByte);
        return;
    case 14:
        _3rdByte = *(ch); ch+=1;
        __SYNPitchWheel(synth, midiChannel, _2ndByte, _3rdByte);
        return;
    }
}

void __SYNRunInputBufferEvents(SYNSYNTH* synth) {
    u8* input;

    for (input = &synth->input[0][0]; synth->inputCounter; synth->inputCounter--) {
        __SYNMidiIn(synth, input);
        input+=3;
    }

    synth->inputPosition = &synth->input[0][0];
}

u8 SYNGetMidiController(SYNSYNTH* synth, u8 midiChannel, u8 function) {
    ASSERTLINE(678, synth);
    ASSERTLINE(679, midiChannel < 16);
    ASSERTLINE(680, function < 128);
    return synth->controller[midiChannel][function];
}
