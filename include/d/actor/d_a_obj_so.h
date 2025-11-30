#ifndef D_A_OBJ_SO_H
#define D_A_OBJ_SO_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class obj_so_class
 * @brief Monkey Cage (Saru Ori)
 *
 * @details
 *
 */
class obj_so_class {
public:
    /* 0x0000 */ fopAc_ac_c actor;
    /* 0x0568 */ request_of_phase_process_class mPhase;
    /* 0x0570 */ u8 field_0x570;
    /* 0x0571 */ u8 field_0x571;
    /* 0x0574 */ J3DModel* field_0x574;
    /* 0x0578 */ u8 field_0x578[0x594 - 0x578];
    /* 0x0594 */ J3DModel* field_0x594[26];
    /* 0x05FC */ s8 field_0x5fc[26];
    /* 0x0618 */ cXyz field_0x618[26];
    /* 0x0750 */ f32 field_0x750[26];
    /* 0x07B8 */ cXyz field_0x7b8[26];
    /* 0x08F0 */ cXyz field_0x8f0[26];
    /* 0x0A28 */ csXyz field_0xa28[26];
    /* 0x0AC4 */ csXyz field_0xac4[26];
    /* 0x0B60 */ s16 field_0xb60[2];
    /* 0x0B64 */ u8 field_0xb64[0xb94 - 0xb64];
    /* 0x0B94 */ s16 field_0xb94[2];
    /* 0x0B98 */ u8 field_0xb98[0xbc8 - 0xb98];
    /* 0x0BC8 */ s16 field_0xbc8[2];
    /* 0x0BCC */ u8 field_0xbcc[0xbfc - 0xbcc];
    /* 0x0BFC */ s16 field_0xbfc[2];
    /* 0x0C00 */ u8 field_0xc00[0xc30 - 0xc00];
    /* 0x0C30 */ s8 field_0xc30[26];
    /* 0x0C4C */ u32 field_0xc4c[26];
    /* 0x0CB4 */ u32 field_0xcb4[26];
    /* 0x0D1C */ Z2Creature mSound;
    /* 0x0DAC */ s16 field_0xdac;
    /* 0x0DAE */ s16 field_0xdae;
    /* 0x0DB0 */ s16 field_0xdb0;
    /* 0x0DB2 */ s16 field_0xdb2[2];
    /* 0x0DB6 */ s16 field_0xdb6;
    /* 0x0DB8 */ s16 field_0xdb8;
    /* 0x0DBA */ s16 field_0xdba;
    /* 0x0DBC */ s16 field_0xdbc;
    /* 0x0DBE */ s16 field_0xdbe;
    /* 0x0DC0 */ s16 field_0xdc0;
    /* 0x0DC2 */ s16 field_0xdc2;
    /* 0x0DC4 */ s8 field_0xdc4;
    /* 0x0DC8 */ f32 field_0xdc8;
    /* 0x0DCC */ dBgS_AcchCir mAcchCir;
    /* 0x0E0C */ dBgS_ObjAcch mObjAcch;
    /* 0x0FE4 */ u8 field_0xfe4[0xfec - 0xfe4];
    /* 0x0FEC */ Mtx mBgMtx[2];
    /* 0x104C */ dBgW* mBgW[2];
    /* 0x1054 */ s8 field_0x1054;
    /* 0x1056 */ s16 field_0x1056;
    /* 0x1058 */ dCcD_Stts mStts;
    /* 0x1094 */ dCcU_AtInfo mAtInfo;
    /* 0x10B8 */ dCcD_Cyl field_0x10b8[8];
    /* 0x1A98 */ s8 field_0x1a98[8];
    /* 0x1AA0 */ J3DModel* field_0x1aa0[8];
    /* 0x1AC0 */ cXyz field_0x1ac0[8];
    /* 0x1B20 */ s16 field_0x1b20[8];
    /* 0x1B30 */ s16 field_0x1b30[8];
    /* 0x1B40 */ s16 field_0x1b40[8];
    /* 0x1B50 */ s16 field_0x1b50[8];
    /* 0x1B60 */ s8 field_0x1b60;
    /* 0x1B64 */ Mtx mMtx;
    /* 0x1B94 */ fpc_ProcID field_0x1b94;
    /* 0x1B98 */ u32 field_0x1b98;
    /* 0x1B9C */ u32 field_0x1b9c;
    /* 0x1BA0 */ s16 mDemoMode;
    /* 0x1BA2 */ s16 field_0x1ba2;
    /* 0x1BA4 */ cXyz mDemoCamEye;
    /* 0x1BB0 */ cXyz mDemoCamCenter;
    /* 0x1BBC */ f32 mDemoCamFovy;
    /* 0x1BC0 */ f32 field_0x1bc0;
    /* 0x1BC4 */ u8 field_0x1bc4[0x1bcc - 0x1bc4];
    /* 0x1BCC */ u8 field_0x1bcc;

    bool partBreak() {
        for (int i = 0; i < 8; i++) {
            if (field_0x1a98[i] == 2) {
                return true;
            }
        }

        return false;
    }
};

STATIC_ASSERT(sizeof(obj_so_class) == 0x1bd0);

class daObj_So_HIO_c : public JORReflexible {
public:
    daObj_So_HIO_c();
    virtual ~daObj_So_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mScale;
};


#endif /* D_A_OBJ_SO_H */
