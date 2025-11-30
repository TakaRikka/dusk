#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/serpoll.h"
#include "TRK_MINNOW_DOLPHIN/ppc/Generic/targimpl.h"
#include "trk.h"

void TRKNubMainLoop(void) {
    void* msg;
    TRKEvent event;
    BOOL isShutdownRequested;
    BOOL isNewInput;

    isShutdownRequested = FALSE;
    isNewInput = FALSE;
    while (isShutdownRequested == FALSE) {
        if (TRKGetNextEvent(&event) != FALSE) {
            isNewInput = FALSE;

            switch (event.eventType) {
            case NUBEVENT_Null:
                break;

            case NUBEVENT_Request:
                msg = TRKGetBuffer(event.msgBufID);
                TRKDispatchMessage(msg);
                break;

            case NUBEVENT_Shutdown:
                isShutdownRequested = TRUE;
                break;

            case NUBEVENT_Breakpoint:
            case NUBEVENT_Exception:
                TRKTargetInterrupt(&event);
                break;

            case NUBEVENT_Support:
                TRKTargetSupportRequest();
                break;
            }

            TRKDestructEvent(&event);
            continue;
        }

        if ((isNewInput == FALSE) || (*(u8*)gTRKInputPendingPtr != '\0')) {
            isNewInput = TRUE;
            TRKGetInput();
            continue;
        }

        if (TRKTargetStopped() == FALSE) {
            TRKTargetContinue();
        }
        isNewInput = FALSE;
    }
}
