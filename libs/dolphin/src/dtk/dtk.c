#include <dolphin/ai.h>
#include <dolphin/dtk.h>
#include <dolphin/dvd.h>
#include <dolphin/os.h>

static DTKTrack* __DTKCurrentTrack;
static DTKTrack* __DTKPlayListHead;
static DTKTrack* __DTKPlayListTail;
static volatile u32 __DTKState;
static volatile u32 __DTKTempState;
static volatile u32 __DTKRepeatMode;
static volatile u32 __DTKPosition;
static volatile u32 __DTKInterruptFrequency;
static volatile u8 __DTKVolumeL;
static volatile u8 __DTKVolumeR;
static volatile u32 __DTKShutdownFlag;
static volatile u32 __DTKTrackEnded;
static DTKFlushCallback __DTKFlushCallback;
static int __busy_for_ais_address;

static DVDCommandBlock __block_for_run_callback;
static DVDCommandBlock __block_for_prep_callback;
static DVDCommandBlock __block_for_stream_status;
static DVDCommandBlock __block_for_ais_isr;
static DVDCommandBlock __block_for_flushtracks;
static DVDCommandBlock __block_for_repeatmode;
static DVDCommandBlock __block_for_set_state;
static DVDCommandBlock __block_for_next_track;
static DVDCommandBlock __block_for_prev_track;

static void __DTKStartAi(void) {
    AISetStreamVolLeft(__DTKVolumeL);
    AISetStreamVolRight(__DTKVolumeR);
    AIResetStreamSampleCount();
    AISetStreamTrigger(__DTKInterruptFrequency);
    AISetStreamPlayState(1);
}

static void __DTKStopAi(void) {
    AISetStreamVolLeft(0);
    AISetStreamVolRight(0);
    AISetStreamPlayState(0);
}

static void __DTKCheckUserCallback(DTKTrack* track, u32 event) {
    ASSERTLINE(84, track);
    if (track && track->callback && (track->eventMask & event)) {
        track->callback(track->eventMask & event);
    }
}

static void __DTKForward(void) {
    BOOL old = OSDisableInterrupts();
    if (__DTKCurrentTrack && __DTKCurrentTrack->next) {
        __DTKCurrentTrack = __DTKCurrentTrack->next;
    }
    OSRestoreInterrupts(old);
}

static void __DTKBackward(void) {
    BOOL old = OSDisableInterrupts();
    if (__DTKCurrentTrack && __DTKCurrentTrack->prev) {
        __DTKCurrentTrack = __DTKCurrentTrack->prev;
    }
    OSRestoreInterrupts(old);
}

static void __DTKCallbackForStreamStatus(s32 result, DVDCommandBlock* block) {
    if ((result & 0xFF) == 0) {
        __DTKTrackEnded = 1;
        __DTKPosition = 0;
    }
}

static void __DTKCallbackForRun(s32 result, DVDFileInfo* fileInfo) {
    __DTKStartAi();
    DVDStopStreamAtEndAsync(&__block_for_run_callback, 0);
    __DTKState = DTK_STATE_RUN;
    __DTKCheckUserCallback(__DTKCurrentTrack, 1);
}

static void __DTKCallbackForPreparePaused(s32 result, DVDFileInfo* fileInfo) {
    __DTKStopAi();
    DVDStopStreamAtEndAsync(&__block_for_prep_callback, 0);
    __DTKState = DTK_STATE_PAUSE;
    __DTKCheckUserCallback(__DTKCurrentTrack, 32);
}

static void __DTKPrepareCurrentTrack(void) {
    DVDPrepareStreamAsync(&__DTKCurrentTrack->dvdFileInfo, 0, 0, __DTKCallbackForRun);
}

static void __DTKPrepareCurrentTrackPaused(void) {
    DVDPrepareStreamAsync(&__DTKCurrentTrack->dvdFileInfo, 0, 0, __DTKCallbackForPreparePaused);
}

static void __DTKCallbackForPlaylist(s32 result, DVDCommandBlock* block) {
    __DTKPosition = result;
    __busy_for_ais_address = 0;

    if (__DTKTrackEnded) {
        __DTKTrackEnded = 0;
        __DTKCheckUserCallback(__DTKCurrentTrack, 16);
        __DTKState = DTK_STATE_BUSY;

        switch (__DTKRepeatMode) {
        case DTK_MODE_NOREPEAT:
            if (__DTKCurrentTrack) {
                if (__DTKCurrentTrack->next) {
                    __DTKCurrentTrack = __DTKCurrentTrack->next;
                    __DTKStopAi();
                    __DTKPrepareCurrentTrack();
                } else {
                    __DTKCurrentTrack = __DTKPlayListHead;
                    __DTKStopAi();
                    __DTKState = DTK_STATE_STOP;
                }
            }
            break;
        case DTK_MODE_ALLREPEAT:
            if (__DTKCurrentTrack) {
                if (__DTKCurrentTrack->next) {
                    __DTKCurrentTrack = __DTKCurrentTrack->next;
                    __DTKStopAi();
                    __DTKPrepareCurrentTrack();
                } else {
                    __DTKCurrentTrack = __DTKPlayListHead;
                    __DTKStopAi();
                    __DTKPrepareCurrentTrack();
                }
            }
            break;
        case DTK_MODE_REPEAT1:
            if (__DTKCurrentTrack) {
                __DTKStopAi();
                __DTKPrepareCurrentTrack();
            }
            break;
        }
    } else {
        DVDGetStreamErrorStatusAsync(&__block_for_stream_status, __DTKCallbackForStreamStatus);
    }
}

static void __DTKCallbackForAIInterrupt(u32 count) {
    AISetStreamTrigger(count + __DTKInterruptFrequency);
    if (__DTKCurrentTrack && !__busy_for_ais_address) {
        __busy_for_ais_address = 1;
        DVDGetStreamPlayAddrAsync(&__block_for_ais_isr, __DTKCallbackForPlaylist);
    }
}

static void __DTKCallbackForFlush(s32 result, DVDCommandBlock* block) {
    DTKTrack* track;

    AISetStreamPlayState(0);
    track = __DTKPlayListHead;
    while (track) {
        DVDClose(&track->dvdFileInfo);
        track = track->next;
    }

    __DTKPlayListHead = NULL;
    __DTKPlayListTail = NULL;
    __DTKCurrentTrack = NULL;
    __DTKState = DTK_STATE_STOP;

    if (__DTKFlushCallback) {
        __DTKFlushCallback();
        __DTKFlushCallback = NULL;
    }

    __DTKState = DTK_STATE_STOP;
    __DTKShutdownFlag = 0;
}

static void __DTKCallbackForStop(s32 result, DVDCommandBlock* block) {
    __DTKCheckUserCallback(__DTKCurrentTrack, 2);
    __DTKState = DTK_STATE_STOP;
}

static void __DTKCallbackForNextTrack(s32 result, DVDCommandBlock* block) {
    AISetStreamPlayState(0);
    __DTKForward();
    __DTKState = DTK_STATE_STOP;
    DTKSetState(__DTKTempState);
}

static void __DTKCallbackForPrevTrack(s32 result, DVDCommandBlock* block) {
    AISetStreamPlayState(0);
    __DTKBackward();
    __DTKState = DTK_STATE_STOP;
    DTKSetState(__DTKTempState);
}

void DTKInit(void) {
    __DTKCurrentTrack = NULL;
    __DTKPlayListHead = NULL;
    __DTKPlayListTail = NULL;
    __DTKState = DTK_STATE_STOP;
    __DTKRepeatMode = DTK_MODE_NOREPEAT;
    __DTKPosition = 0;
    __DTKInterruptFrequency = 48000;
    __DTKVolumeL = 255;
    __DTKVolumeR = 255;

    AISetStreamVolLeft(0);
    AISetStreamVolRight(0);
    AIRegisterStreamCallback(__DTKCallbackForAIInterrupt);
    AIResetStreamSampleCount();
    AISetStreamPlayState(0);
}

void DTKShutdown(void) {
    AISetStreamVolLeft(0);
    AISetStreamVolRight(0);
    AIRegisterStreamCallback(NULL);
    AIResetStreamSampleCount();
    AISetStreamPlayState(0);

    __DTKShutdownFlag = 1;
    DTKFlushTracks(NULL);
    __DTKState = DTK_STATE_STOP;

    while (__DTKShutdownFlag) {}
}

u32 DTKQueueTrack(char* fileName, DTKTrack* track, u32 eventMask, DTKCallback callback) {
    u32 startTrack;
    BOOL old;

    startTrack = 0;
    if (!DVDOpen(fileName, &track->dvdFileInfo)) {
        return 1;
    }

    old = OSDisableInterrupts();
    track->fileName = fileName;
    track->eventMask = eventMask;
    track->callback = callback;

    if (__DTKPlayListHead == NULL) {
        __DTKPlayListHead = track;
        __DTKPlayListTail = track;
        track->prev = NULL;
        track->next = NULL;
        if (__DTKState == DTK_STATE_RUN) {
            startTrack = 1;
        }
    } else {
        __DTKPlayListTail->next = track;
        track->prev = __DTKPlayListTail;
        __DTKPlayListTail = track;
        track->next = NULL;
    }

    if (__DTKCurrentTrack == NULL) {
        __DTKCurrentTrack = track;
    }

    OSRestoreInterrupts(old);
    __DTKCheckUserCallback(track, 8);

    if (startTrack != 0) {
        __DTKState = DTK_STATE_BUSY;
        __DTKPrepareCurrentTrack();
    }

    return 0;
}

u32 DTKRemoveTrack(DTKTrack* track) {
    BOOL old;

    if (track == __DTKCurrentTrack) {
        return 2;
    }

    old = OSDisableInterrupts();
    DVDClose(&track->dvdFileInfo);

    if (track == __DTKPlayListHead && track == __DTKPlayListTail) {
        __DTKPlayListHead = NULL;
        __DTKPlayListTail = NULL;
        OSRestoreInterrupts(old);
        return 0;
    }

    if (track == __DTKPlayListHead) {
        __DTKPlayListHead = track->next;
        __DTKPlayListHead->prev = NULL;
        if (__DTKRepeatMode == DTK_MODE_ALLREPEAT) {
            __DTKPlayListTail->next = __DTKPlayListHead;
        }
        OSRestoreInterrupts(old);
        return 0;
    }

    if (track == __DTKPlayListTail) {
        __DTKPlayListTail = track->prev;
        __DTKPlayListTail->next = NULL;
        if (__DTKRepeatMode == DTK_MODE_ALLREPEAT) {
            __DTKPlayListTail->next = __DTKPlayListHead;
        }
        OSRestoreInterrupts(old);
        return 0;
    }

    track->prev->next = track->next;
    track->next->prev = track->prev;
    OSRestoreInterrupts(old);
    return 0;
}

int DTKFlushTracks(DTKFlushCallback callback) {
    u32 temp;

    if (__DTKState == DTK_STATE_BUSY) {
        return 0;
    }

    temp = __DTKState;
    __DTKState = DTK_STATE_BUSY;
    __DTKFlushCallback = callback;
    if (temp == DTK_STATE_RUN) {
        DVDCancelStreamAsync(&__block_for_flushtracks, __DTKCallbackForFlush);
    } else {
        __DTKCallbackForFlush(0, 0);
    }
    return 1;
}

void DTKSetSampleRate(u32 samplerate) {
    // obsolete
}

void DTKSetInterruptFrequency(u32 samples) {
    __DTKInterruptFrequency = samples;
    AIResetStreamSampleCount();
    AISetStreamTrigger(__DTKInterruptFrequency);
}

void DTKSetRepeatMode(u32 repeat) {
    __DTKRepeatMode = repeat;
}

int DTKSetState(u32 state) {
    if (__DTKState == state) {
        return 1;
    }

    if (__DTKState == DTK_STATE_BUSY) {
        return 0;
    }

    switch (state) {
    case DTK_STATE_STOP:
        if (__DTKCurrentTrack) {
            __DTKState = DTK_STATE_BUSY;
            AISetStreamVolLeft(0);
            AISetStreamVolRight(0);
            AISetStreamPlayState(0);
            DVDCancelStreamAsync(&__block_for_set_state, __DTKCallbackForStop);
        }
        break;
    case DTK_STATE_RUN:
        if (__DTKState == DTK_STATE_PAUSE) {
            __DTKStartAi();
            __DTKState = DTK_STATE_RUN;
            if (__DTKCurrentTrack) {
                __DTKCheckUserCallback(__DTKCurrentTrack, 1);
            }
        } else if (__DTKCurrentTrack) {
            __DTKState = DTK_STATE_BUSY;
            __DTKPrepareCurrentTrack();
        } else {
            __DTKState = DTK_STATE_RUN;
        }
        __DTKTrackEnded = 0;
        break;
    case DTK_STATE_PREPARE:
        if (__DTKState == DTK_STATE_STOP) {
            if (__DTKCurrentTrack) {
                __DTKState = DTK_STATE_BUSY;
                __DTKPrepareCurrentTrackPaused();
            }
            __DTKTrackEnded = 0;
        }
        break;
    case DTK_STATE_PAUSE:
        AISetStreamPlayState(0);
        if (__DTKState == DTK_STATE_RUN) {
            __DTKState = DTK_STATE_PAUSE;
        }
        __DTKCheckUserCallback(__DTKCurrentTrack, 4);
        break;
    }

    return 1;
}

int DTKNextTrack(void) {
    if (__DTKState == DTK_STATE_BUSY) {
        return 0;
    }

    if (__DTKCurrentTrack) {
        __DTKTempState = __DTKState;
        __DTKState = DTK_STATE_BUSY;
        if (__DTKTempState == DTK_STATE_RUN) {
            AISetStreamVolLeft(0);
            AISetStreamVolRight(0);
            DVDCancelStreamAsync(&__block_for_next_track, __DTKCallbackForNextTrack);
        } else {
            __DTKForward();
            __DTKState = __DTKTempState;
        }

        return 1;
    }

    return 0;
}

int DTKPrevTrack(void) {
    if (__DTKState == DTK_STATE_BUSY) {
        return 0;
    }

    if (__DTKCurrentTrack) {
        __DTKTempState = __DTKState;
        __DTKState = DTK_STATE_BUSY;
        if (__DTKTempState == DTK_STATE_RUN) {
            AISetStreamVolLeft(0);
            AISetStreamVolRight(0);
            DVDCancelStreamAsync(&__block_for_prev_track, __DTKCallbackForPrevTrack);
        } else {
            __DTKBackward();
            __DTKState = __DTKTempState;
        }

        return 1;
    }

    return 0;
}

u32 DTKGetSampleRate(void) {
    return 1;  // obsolete
}

u32 DTKGetRepeatMode(void) {
    return __DTKRepeatMode;
}

u32 DTKGetState(void) {
    return __DTKState;
}

u32 DTKGetPosition(void) {
    return __DTKPosition;
}

u32 DTKGetInterruptFrequency(void) {
    return __DTKInterruptFrequency;
}

DTKTrack* DTKGetCurrentTrack(void) {
    return __DTKCurrentTrack;
}

void DTKSetVolume(u8 left, u8 right) {
    __DTKVolumeL = left;
    __DTKVolumeR = right;
    if (__DTKState == DTK_STATE_RUN) {
        AISetStreamVolLeft(left);
        AISetStreamVolRight(right);
    }
}

u16 DTKGetVolume(void) {
    return (__DTKVolumeL << 8) | __DTKVolumeR;
}
