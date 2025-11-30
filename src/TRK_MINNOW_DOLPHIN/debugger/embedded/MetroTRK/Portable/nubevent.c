#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/nubevent.h"

TRKEventQueue gTRKEventQueue;

DSError TRKInitializeEventQueue() {
    TRKInitializeMutex(&gTRKEventQueue);
    TRKAcquireMutex(&gTRKEventQueue);
    gTRKEventQueue.count = 0;
    gTRKEventQueue.next = 0;
    gTRKEventQueue.eventID = 0x100;
    TRKReleaseMutex(&gTRKEventQueue);
    return DS_NoError;
}

BOOL TRKGetNextEvent(TRKEvent* event) {
    BOOL status = 0;
    TRKAcquireMutex(&gTRKEventQueue);
    if (0 < gTRKEventQueue.count) {
        TRK_memcpy(event, &gTRKEventQueue.events[gTRKEventQueue.next], sizeof(TRKEvent));
        gTRKEventQueue.count--;
        if (++gTRKEventQueue.next == 2) {
            gTRKEventQueue.next = 0;
        }
        status = 1;
    }
    TRKReleaseMutex(&gTRKEventQueue);
    return status;
}

DSError TRKPostEvent(TRKEvent* event) {
    DSError ret = DS_NoError;
    int nextEventID;

    TRKAcquireMutex(&gTRKEventQueue);

    if (gTRKEventQueue.count == 2) {
        ret = DS_EventQueueFull;

    } else {
        nextEventID = (gTRKEventQueue.next + gTRKEventQueue.count) % 2;
        TRK_memcpy(&gTRKEventQueue.events[nextEventID], event, sizeof(TRKEvent));
        gTRKEventQueue.events[nextEventID].eventID = gTRKEventQueue.eventID;

        if (++gTRKEventQueue.eventID < 0x100) {
            gTRKEventQueue.eventID = 0x100;
        }

        gTRKEventQueue.count++;
    }

    TRKReleaseMutex(&gTRKEventQueue);
    return ret;
}

void TRKConstructEvent(TRKEvent* event, NubEventType eventType) {
    event->eventType = eventType;
    event->eventID = 0;
    event->msgBufID = -1;
}

void TRKDestructEvent(TRKEvent* event) {
    TRKReleaseBuffer(event->msgBufID);
}
