#ifndef D_D_ERROR_MSG_H
#define D_D_ERROR_MSG_H

#include "d/d_com_inf_game.h"

struct dShutdownErrorMsg_c {
    static bool execute();
};

struct dDvdErrorMsg_c {
    static void draw(s32);
    static u8 execute();
};

#endif /* D_D_ERROR_MSG_H */
