/**
 * msg.c
 * Description:
 */

#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/msg.h"
#include "TRK_MINNOW_DOLPHIN/utils/common/MWTrace.h"
#include "trk.h"

DSError TRKMessageSend(TRK_Msg* msg) {
    DSError write_err = TRKWriteUARTN(&msg->m_msg, msg->m_msgLength);
    MWTRACE(1, "MessageSend : cc_write returned %ld\n", write_err);
    return DS_NoError;
}
