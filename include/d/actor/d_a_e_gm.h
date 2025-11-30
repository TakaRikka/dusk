#ifndef D_A_E_GM_H
#define D_A_E_GM_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-enemies
 * @class daE_GM_c
 * @brief Baby Gohma / Gohma Eye
 * 
 * @details 
 * 
*/

class daE_GM_c : public fopEn_enemy_c {
public:
    typedef void (daE_GM_c::*actionFunc)();
    
    void setAction(actionFunc);
    void action();
    void egg_wait();
    void appear();
    void walk();
    void create_item();
    void pechanko_damage();
    void wait();
    void egg_damage();
    void setDeathLightEffect();
    void setEggBreakEffect();
    void damage();
    void core_damage();
    void core_delwait();
    void core_piku();
    void core_pechanko_damage();
    void hook();
    void rebound();
    void walk2();
    void dead_wait();
    void core_wait();
    void core_escape();
    BOOL chaseCheck();
    void normal_wait();
    void normal_chase();
    void mtx_set();
    void cc_set();
    int draw();
    void damage_check();
    int execute();
    int _delete();
    int CreateHeap();
    cPhs__Step create();

    void InstantKill() { field_0xa76 = 1; }
    u8 CheckDeath() { return field_0xa68; }
    void MoveStart(u8 param_1) {
        field_0xa71 = true;
        field_0xa5e = param_1;
    }
    BOOL checkAction(actionFunc action) { return mAction == action; }
    BOOL checkOldAction(actionFunc action) { return mOldAction == action; }

private:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ dBgS_AcchCir mAcchCir;
    /* 0x5F4 */ dBgS_ObjAcch mObjAcch;
    /* 0x7CC */ dCcD_Stts mStts;
    /* 0x808 */ dCcD_Sph mSph;
    /* 0x940 */ dCcU_AtInfo mAtInfo;
    /* 0x964 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x968 */ J3DModel* mpEggModel;
    /* 0x96C */ Z2CreatureEnemy mSound;
    /* 0xA10 */ cXyz field_0xa10;
    /* 0xA1C */ csXyz field_0xa1c;
    /* 0xA24 */ actionFunc mAction;
    /* 0xA30 */ actionFunc mOldAction;
    /* 0xA3C */ int mMode;
    /* 0xA40 */ f32 field_0xa40;
    /* 0xA44 */ f32 field_0xa44;
    /* 0xA48 */ f32 mColor;
    /* 0xA4C */ f32 field_0xa4c;
    /* 0xA50 */ f32 field_0xa50;
    /* 0xA54 */ f32 field_0xa54;
    /* 0xA58 */ u32 mShadowKey;
    /* 0xA5C */ s16 field_0xa5c;
    /* 0xA5E */ s16 field_0xa5e;
    /* 0xA60 */ s16 field_0xa60;
    /* 0xA62 */ s16 field_0xa62;
    /* 0xA64 */ s16 field_0xa64;
    /* 0xA66 */ s16 field_0xa66;
    /* 0xA68 */ u8 field_0xa68;
    /* 0xA69 */ u8 field_0xa69;
    /* 0xA6A */ u8 field_0xa6a;
    /* 0xA6B */ u8 field_0xa6b;
    /* 0xA6C */ u8 field_0xa6c;
    /* 0xA6D */ u8 field_0xa6d;
    /* 0xA6E */ u8 field_0xa6e;
    /* 0xA6F */ u8 mType;
    /* 0xA70 */ u8 mSwBit;
    /* 0xA71 */ bool field_0xa71;
    /* 0xA72 */ u8 field_0xa72;
    /* 0xA73 */ u8 mRandIndex;
    /* 0xA74 */ u8 field_0xa74;
    /* 0xA75 */ u8 field_0xa75;
    /* 0xA76 */ bool field_0xa76;
    /* 0xA77 */ u8 field_0xa77;
};

STATIC_ASSERT(sizeof(daE_GM_c) == 0xa78);

#endif /* D_A_E_GM_H */
