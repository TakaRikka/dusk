#ifndef D_A_FR_H
#define D_A_FR_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_msg_flow.h"
#include <dolphin/types.h>
#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-unsorted
 * @class fr_class
 * @brief Frog
 *
 * @details
 *
 */
class fr_class : public fopEn_enemy_c {
public:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ u8 field_0x5b4;
    /* 0x5B5 */ cXyz field_0x5b8;
    /* 0x5C4 */ mDoExt_McaMorf* mMorf;
    /* 0x5C8 */ mDoExt_btkAnm* mBtkAnm;
    /* 0x5CC */ u8 field_0x5cc;
    /* 0x5CD */ s8 field_0x5cd;
    /* 0x5CE */ s8 field_0x5ce;
    /* 0x5CF */ u8 field_0x5cf;
    /* 0x5D0 */ s16 field_0x5d0;
    /* 0x5D2 */ s16 field_0x5d2;
    /* 0x5D4 */ s16 field_0x5d4;
    /* 0x5D6 */ u8 field_0x5d6[0x5d8 - 0x5d6];
    /* 0x5D8 */ f32 field_0x5d8;
    /* 0x5DC */ s16 field_0x5dc[4];
    /* 0x5E4 */ s16 field_0x5e4;
    /* 0x5E6 */ u8 field_0x5e6[0x5e8 - 0x5e6];
    /* 0x5E8 */ f32 field_0x5e8;
    /* 0x5EC */ s8 field_0x5ec;
    /* 0x5ED */ u8 field_0x5ed[0x5f0 - 0x5ed];
    /* 0x5F0 */ f32 field_0x5f0;
    /* 0x5F4 */ f32 field_0x5f4;
    /* 0x5F8 */ fpc_ProcID field_0x5f8;
    /* 0x5FC */ s16 field_0x5fc;
    /* 0x5FE */ u8 field_0x5fe[0x600 - 0x5fe];
    /* 0x600 */ f32 field_0x600;
    /* 0x604 */ dBgS_AcchCir mAcchCir;
    /* 0x644 */ dBgS_ObjAcch mAcch;
    /* 0x81C */ dCcD_Stts mStts;
    /* 0x858 */ dCcD_Sph mSph;
    /* 0x990 */ u8 field_0x990;
    /* 0x991 */ s8 field_0x991;
    /* 0x992 */ s16 field_0x992;
    /* 0x994 */ s16 field_0x994;
    /* 0x998 */ dMsgFlow_c mMsgFlow;
    /* 0x9E4 */ u8 field_0x9e4;
    /* 0x9E5 */ u8 field_0x9e5[0x9e8 - 0x9e5];
};

STATIC_ASSERT(sizeof(fr_class) == 0x9e8);


#endif /* D_A_FR_H */
