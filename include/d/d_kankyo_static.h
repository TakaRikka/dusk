#ifndef D_KANKYO_STATIC_H
#define D_KANKYO_STATIC_H

#include "d/d_com_inf_game.h"

/**
 * This is a made-up file to help matching weak function generation.
 *
 * `dKankyo_DayProc` is supposed to be weakly generated at the end of TUs where it's called,
 * but it'll generate in every TU where the header it's defined in is included. So only include this
 * header in TUs where `dKankyo_DayProc` needs to be generated.
 */

static void dKankyo_DayProc() {
    dComIfGs_offTmpBit((u16)dSv_event_tmp_flag_c::tempBitLabels[91]);
}

#endif /* D_KANKYO_STATIC_H */
