#ifndef D_A_TAG_CHGRESTART_H
#define D_A_TAG_CHGRESTART_H

#include "d/d_com_inf_game.h"

class daTagChgRestart_c : public fopAc_ac_c {
public:
    daTagChgRestart_c() {}
    int Create();
    int create();
    int execute();
    int draw();
    int _delete();

    /* 0x0568 */ cXyz mVertices[4];
};

STATIC_ASSERT(sizeof(daTagChgRestart_c) == 0x598);

namespace daTagChgRestart_prm {
    inline u8 getSwNo(daTagChgRestart_c* p_chgRestart) {
        return (fopAcM_GetParam(p_chgRestart) >> 8) & 0xFF;
    }
    inline u8 getPlayerNo(daTagChgRestart_c* p_chgRestart) {
        return fopAcM_GetParam(p_chgRestart);
    }
};  // namespace daTagChgRestart_prm

#endif /* D_A_TAG_CHGRESTART_H */
