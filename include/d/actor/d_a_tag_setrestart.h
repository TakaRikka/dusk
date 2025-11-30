#ifndef D_A_TAG_SETRESTART_H
#define D_A_TAG_SETRESTART_H

#include "f_op/f_op_actor_mng.h"

class daTagRestart_c : public fopAc_ac_c {
public:
    int Create();
    int create();
    int execute();
    int _delete();

    /* 0x568 */ s8 mPlayerRoomNo;
    /* 0x56C */ cXyz field_0x56c[4];
};

namespace daTagRestart_prm {
    inline u8 getFRoomNo(daTagRestart_c* i_tag) { return fopAcM_GetParam(i_tag); }
};

#endif /* D_A_TAG_SETRESTART_H */
