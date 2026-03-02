#include <dolphin/dolphin.h>
#include <dolphin/seq.h>

static u8 __SEQMidiEventLength[128] = {
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02,
    0x00, 0x00, 0x02, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

static SEQSEQUENCE* __SEQSequenceList;

// prototypes
static void __SEQPushSequenceList(SEQSEQUENCE* sequence);
static void __SEQRemoveSequenceFromList(SEQSEQUENCE* sequence);
static u32 __SEQGetIntTrack(SEQTRACK* track);
static void __SEQHandleSysExEvent(SEQTRACK* track);
static void __SEQSetTicksPerFrame(SEQTRACK* track, f32 bps);
static void __SEQTempoMetaEvent(SEQTRACK* track);
static void __SEQTrackEnd(SEQTRACK* track);
static void __SEQHandleMetaEvent(SEQTRACK* track);
static void __SEQHandleSynthEvent(SYNSYNTH* synth, SEQTRACK* track);
static void __SEQRunEvent(SYNSYNTH* synth, SEQTRACK* track);
static void __SEQInitTracks(SEQSEQUENCE* sequence, u8* read, int tracks);
static void __SEQReadHeader(SEQSEQUENCE* sequence, u8* midiStream);

static void __SEQPushSequenceList(SEQSEQUENCE* sequence) {
    BOOL old;

    old = OSDisableInterrupts();
    if (__SEQSequenceList) {
        sequence->next = __SEQSequenceList;
    } else {
        sequence->next = NULL;
    }

    __SEQSequenceList = sequence;
    OSRestoreInterrupts(old);
}

static void __SEQRemoveSequenceFromList(SEQSEQUENCE* sequence) {
    BOOL old;
    SEQSEQUENCE* thisSequence;
    SEQSEQUENCE* next;

    old = OSDisableInterrupts();
    thisSequence = __SEQSequenceList;
    __SEQSequenceList = NULL;

    while(thisSequence) {
        next = thisSequence->next;
        if (thisSequence != sequence) {
            __SEQPushSequenceList(thisSequence);
        }
        thisSequence = next;
    }

    OSRestoreInterrupts(old);
}

static u32 __SEQGetIntTrack(SEQTRACK* track) {
    u32 value;

    ASSERTLINE(120, track);
    for (value = *track->current & 0x7F; *track->current & 0x80; value = (value << 7) + (*track->current & 0x7F)) {
        track->current++;
    }
    track->current++;
    return value;
}

static void __SEQHandleSysExEvent(SEQTRACK* track) {
    u32 length;

    ASSERTLINE(143, track);
    length = __SEQGetIntTrack(track);
    track->current += length;
}

static void __SEQSetTicksPerFrame(SEQTRACK* track, f32 bps) {
    SEQSEQUENCE* sequence;

    ASSERTLINE(157, track);
    sequence = track->sequence;
    track->beatsPerSec = bps;
    track->ticksPerFrame = (65536.0f * (160.0f / ((32000.0f / bps) / sequence->timeFormat)));
}

static void __SEQTempoMetaEvent(SEQTRACK* track) {
    u32 data;
    f32 beatsPerSec;

    data = *track->current;
    track->current++;

    data = (data << 8) + *track->current;
    track->current++;

    data = (data << 8) + *track->current;
    track->current++;

    beatsPerSec = 1000000 / (f32)data;
    __SEQSetTicksPerFrame(track, beatsPerSec);
}

static void __SEQTrackEnd(SEQTRACK* track) {
    SEQSEQUENCE* sequence;
    ASSERTLINE(199, track);

    sequence = track->sequence;
    sequence->tracksRunning--;
    track->state = 0;
    if (sequence->tracksRunning == 0) {
        sequence->end = 1;
    }
}

static void __SEQHandleMetaEvent(SEQTRACK* track) {
    u8 type;
    u32 length;

    ASSERTLINE(218, track);
    type = *track->current;
    track->current++;

    switch(type) {
    case 0x2F:
        __SEQTrackEnd(track);
        return;
    case 0x51:
        length = __SEQGetIntTrack(track);
        __SEQTempoMetaEvent(track);
        return;
    default:
        length = __SEQGetIntTrack(track);
        track->current += length;
        return;
    }
}

static void __SEQHandleSynthEvent(SYNSYNTH* synth, SEQTRACK* track) {
    u8 ch[3];
    u32 bytes;
    void (*callback)(void *, u8);

    bytes = __SEQMidiEventLength[track->status - 0x80];
    ch[0] = track->status;

    switch(bytes) {
    case 0:
        break;
    case 1:
        ch[1] = *track->current; track->current++;
        break;
    case 2:
        ch[1] = *track->current; track->current++;
        ch[2] = *track->current; track->current++;
        break;
    }

    if ((ch[0] & 0xF0) == 0xB0) {
        callback = ((SEQSEQUENCE*)track->sequence)->callback[ch[1]];
        if (callback) {
           callback(track, ch[1]);
        }
    }

    SYNMidiInput(synth, ch);
}

static void __SEQRunEvent(SYNSYNTH* synth, SEQTRACK* track) {
    u8 event;

    ASSERTLINE(303, synth);
    ASSERTLINE(304, track);

    event = *track->current;
    if (event >= 0x80) {
        track->status = event; track->current++;
    }

    switch(track->status) {
    case 0xF7:
    case 0xF0:
        __SEQHandleSysExEvent(track);
        break;
    case 0xFF:
        __SEQHandleMetaEvent(track);
        break;
    default:
        __SEQHandleSynthEvent(synth, track);
        break;
    }

    if (track->current >= track->end) {
        __SEQTrackEnd(track);
    }
}

static void __SEQInitTracks(SEQSEQUENCE* sequence, u8* read, int tracks) {
    int i;
    u8* p;
    u32 chunk;
    u32 bytes;
    SEQTRACK* track;

    i = 0;
    p = read;

    while (tracks) {
        while (1) {
            chunk = *(u32*)p;
            p += 4;
            bytes = *(u32*)p;
            p += 4;
            if (chunk == 'MTrk') {
                track = &sequence->track[i];
                track->sequence = sequence;
                track->start = p;
                track->end = &p[bytes];
                track->current = p;
                track->defaultTicksPerFrame = (u32)(65536.0f * (160.0f / (16000.0f / (f32)sequence->timeFormat)));
                track->state = 0;
                p += bytes;
                break;
            }
            p += bytes;
        }
        tracks--;
        i++;
    }
}

static void __SEQReadHeader(SEQSEQUENCE* sequence, u8* midiStream) {
    u8* read;
    u32 bytes;
    u32 fileType;

    read = midiStream;
    ASSERTMSGLINE(401, *(u32*)read == 'MThd', "!!!midiStream is not a valid MIDI file\n!!!");
    read += 4;

    bytes = *(u32*)read;
    read += 4;

    fileType = *(u16*)read;
    read+=2;

    sequence->nTracks = *(u16*)read;
    read+=2;

    sequence->timeFormat = *(s16*)read;
    read+=2;

    ASSERTMSGLINE(416, sequence->timeFormat >= 0, "!!!SEQ does not support SMPTE time!!!\n");
    bytes -= 6;
    read += bytes;

    switch(fileType) {
    case 0:
        sequence->nTracks = 1;
        __SEQInitTracks(sequence, read, 1);
        break;
    case 1:
        ASSERTMSGLINE(438, sequence->nTracks < 0x40, "exceeded SEQ_MAX_TRACKS, please increase SEQ_MAX_TRACKS\n");
        __SEQInitTracks(sequence, read, sequence->nTracks);
        break;
    default:
        ASSERTMSGLINE(446, 0, "!!!Invalid MIDI file type\n!!!");
        break;
    }

    sequence->tracksRunning = sequence->nTracks;
}

void SEQInit(void) {
    __SEQSequenceList = NULL;
}

void SEQQuit(void) {
    __SEQSequenceList = NULL;
}

void SEQRunAudioFrame(void) {
    SEQSEQUENCE* sequence;
    u32 i;
    SEQTRACK* track;
    u32 ticks;

    for (sequence = __SEQSequenceList; sequence; sequence = sequence->next) {
        if ((sequence->state == 1) || (sequence->state == 2)) {
            for (i = 0; i < sequence->nTracks; i++) {
                track = &sequence->track[i];
                if ((track->state == 1) || (track->state == 2)) {
                    ticks = track->ticksPerFrame;
                    if (track->delay > ticks) {
                        track->delay -= ticks;
                    } else {
                        while (ticks >= track->delay) {
                            ticks -= track->delay;
                            __SEQRunEvent(&sequence->synth, track);
                            if (track->state != 0) {
                                track->delay = __SEQGetIntTrack(track) << 0x10;
                            } else {
                                break;
                            }
                        }

                        track->delay -= ticks;
                    }
                }
            }
        }

        if (sequence->end != 0) {
            if (sequence->state == 2) {
                SEQSetState(sequence, 0);
                SEQSetState(sequence, 2);
            } else {
                SEQSetState(sequence, 0);
            }
        }
    }
}

void SEQAddSequence(SEQSEQUENCE* sequence, u8* midiStream, void* wt, u32 aramBase, u32 zeroBase, u32 priorityVoiceAlloc, u32 priorityNoteOn, u32 priorityNoteRelease) {
    int i;

    ASSERTLINE(559, sequence);
    ASSERTLINE(560, midiStream);
    ASSERTLINE(561, wt);
    ASSERTLINE(562, aramBase);
    ASSERTLINE(563, (priorityVoiceAlloc < 32) && (priorityVoiceAlloc > 0));
    ASSERTLINE(564, (priorityNoteOn < 32) && (priorityNoteOn > 0));
    ASSERTLINE(565, (priorityNoteRelease < 32) && (priorityNoteRelease > 0));

    SYNInitSynth(&sequence->synth, wt, aramBase, zeroBase, priorityVoiceAlloc, priorityNoteOn, priorityNoteRelease);
    sequence->state = 0;

    for(i = 0; i < 0x80; i++) {
        sequence->callback[i] = 0;
    }

    __SEQReadHeader(sequence, midiStream);
    __SEQPushSequenceList(sequence);
}

void SEQRemoveSequence(SEQSEQUENCE* sequence) {
    ASSERTLINE(598, sequence);
    __SEQRemoveSequenceFromList(sequence);
    SYNQuitSynth(&sequence->synth);
}

void SEQRegisterControllerCallback(SEQSEQUENCE* sequence, u8 controller, void (*callback)(void*, u8)) {
    ASSERTLINE(617, sequence);
    ASSERTLINE(618, controller < 128);
    ASSERTLINE(619, callback);
    sequence->callback[controller] = callback;
}

void SEQSetState(SEQSEQUENCE* sequence, u32 state) {
    int i;

    ASSERTLINE(632, sequence);

    switch(state) {
    case 1:
    case 2:
        if (sequence->state == 0) {
            int old;

            old = OSDisableInterrupts();
            for (i = 0; i < sequence->nTracks; i++) {
                SEQTRACK* track = &sequence->track[i];
                track->current = track->start;
                track->ticksPerFrame = track->defaultTicksPerFrame;
                track->delay = __SEQGetIntTrack(track) << 0x10;
                track->state = 1;                    
            }
            sequence->tracksRunning = sequence->nTracks;
            OSRestoreInterrupts(old);
        }
        sequence->end = 0;
        break;
    case 0:
    case 3: {
        int old;
        u8 ch[3];

        for (i = 0; i < 16; i++) {
            old = OSDisableInterrupts();
            ch[0] = (i | 0xB0);
            ch[1] = 0x7B;
            ch[2] = 0;
            SYNMidiInput(&sequence->synth, ch);
            OSRestoreInterrupts(old);
        }
        break;
    }
    }

    sequence->state = state;
}

u32 SEQGetState(SEQSEQUENCE* sequence) {
    ASSERTLINE(700, sequence);
    return sequence->state;
}

void SEQSetTempo(SEQSEQUENCE* sequence, u32 trackIndex, f32 bpm) {
    int i;

    ASSERTLINE(711, sequence);
    ASSERTLINE(712, (trackIndex < sequence->nTracks) || (trackIndex == SEQ_ALL_TRACKS));

    if (trackIndex == -1) {
        for (i = 0; i < sequence->nTracks; i++) {
            __SEQSetTicksPerFrame(&sequence->track[i], bpm / 60.0f);
        }
        return;
    }

    __SEQSetTicksPerFrame(&sequence->track[trackIndex], bpm / 60.0f);
}

f32 SEQGetTempo(SEQSEQUENCE* sequence, u32 trackIndex) {
    ASSERTLINE(733, sequence);
    ASSERTLINE(734, trackIndex < sequence->nTracks);
    return 60.0f* sequence->track[trackIndex].beatsPerSec;
}

void SEQSetVolume(SEQSEQUENCE* sequence, s32 dB) {
    ASSERTLINE(745, sequence);
    SYNSetMasterVolume(&sequence->synth, dB);
}

long SEQGetVolume(SEQSEQUENCE* sequence) {
    ASSERTLINE(756, sequence);
    SYNGetMasterVolume(&sequence->synth);
}
