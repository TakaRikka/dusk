/**
 * targcont.c
 * Description:
 */

#include "TRK_MINNOW_DOLPHIN/Os/dolphin/targcont.h"

DSError TRKTargetContinue(void) {
    TRKTargetSetStopped(0);
    UnreserveEXI2Port();
    TRKSwapAndGo();
    ReserveEXI2Port();
    return 0;
}
