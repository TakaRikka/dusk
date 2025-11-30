#ifndef D_A_NPC_TKJ2_H
#define D_A_NPC_TKJ2_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class npc_tkj2_class
 * @brief Oocca 2
 *
 * @details
 *
*/

class npc_tkj2_class {
public:
    /* 0x000 */ fopEn_enemy_c actor;
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ u8 field_0x5b4;
    /* 0x5B8 */ cXyz field_0x5b8;
    /* 0x5C4 */ s16 field_0x5c4;
    /* 0x5C8 */ f32 field_0x5c8;
    /* 0x5CC */ s16 field_0x5cc;
    /* 0x5D0 */ f32 field_0x5d0;
    /* 0x5D4 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x5D8 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x5DC */ mDoExt_btpAnm* mpBtpAnm;
    /* 0x5E0 */ f32 field_0x5e0;
    /* 0x5E4 */ f32 field_0x5e4;
    /* 0x5E8 */ s16 field_0x5e8;
    /* 0x5EA */ u8 field_0x5ea;
    /* 0x5EB */ u8 field_0x5eb;
    /* 0x5EC */ csXyz field_0x5ec;
    /* 0x5F2 */ u8 field_0x5f2;
    /* 0x5F3 */ u8 field_0x5f3;
    /* 0x5F4 */ int field_0x5f4;
    /* 0x5F8 */ s16 field_0x5f8;
    /* 0x5FC */ cXyz field_0x5fc;
    /* 0x608 */ s16 mAction;
    /* 0x60A */ s16 field_0x60a;
    /* 0x60C */ u32 mShadowKey;
    /* 0x610 */ s16 field_0x610[4];
    /* 0x618 */ s16 field_0x618;
    /* 0x61A */ s16 field_0x61a;
    /* 0x61C */ u8 field_0x61c[0x620 - 0x61c];
    /* 0x620 */ s16 field_0x620;
    /* 0x624 */ daPy_boomerangMove_c mBoomerangMove;
    /* 0x630 */ u8 field_0x630;
    /* 0x634 */ dBgS_AcchCir mAcchCir;
    /* 0x674 */ dBgS_ObjAcch mObjAcch;
    /* 0x84C */ u8 field_0x84c[0x850 - 0x84c];
    /* 0x850 */ dCcD_Stts mStts;
    /* 0x88C */ dCcD_Sph field_0x88c;
    /* 0x9C4 */ u8 field_0x9c4[0x9e8 - 0x9c4];
    /* 0x9E8 */ Z2CreatureEnemy mSound;
    /* 0xA8C */ s8 field_0xa8c;
    /* 0xA8E */ s16 field_0xa8e;
    /* 0xa90 */ s16 field_0xa90;
    /* 0xA92 */ s16 field_0xa92;
    /* 0xA94 */ s16 field_0xa94;
    /* 0xA98 */ f32 field_0xa98;
    /* 0xA9C */ u8 field_0xa9c;
};

STATIC_ASSERT(sizeof(npc_tkj2_class) == 0xaa0);

class daNpc_Tkj2_HIO_c {
public:
    daNpc_Tkj2_HIO_c();
    virtual ~daNpc_Tkj2_HIO_c() {}

    /* 天空人（アクション用）- Sky People (For Action) */
    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 basic_size;          // 基本大きさ - Basic Size
    /* 0x0C */ f32 hook_hit_size;       // フックヒット大きさ - Hook Hit Size
    /* 0x10 */ f32 boots_struggle_sp;   // ブーツ時もがきSP - Boots Struggle SP
};

#endif /* D_A_NPC_TKJ2_H */
