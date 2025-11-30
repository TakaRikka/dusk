#ifndef D_A_OBJ_BATTA_H
#define D_A_OBJ_BATTA_H

#include "d/actor/d_a_player.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_insect.h"

/**
 * @ingroup actors-objects
 * @class daObjBATTA_c
 * @brief Insect - Grasshopper (Batta)
 *
 * @details
 *
 */
class daObjBATTA_c : public dInsect_c {
public:
    typedef void (daObjBATTA_c::*actionFunc)();

    void setAction(void (daObjBATTA_c::*)());
    void wait();
    void turn();
    void jump();
    void fly_up();
    void fly_down();
    void bin_wait();
    void bin_action();
    void hook();
    void boomerang();
    void batta_setParticle();
    void action();
    void hit_check();
    int execute();
    int _delete();
    void mtx_set();
    int create();

    inline int CreateHeap();
    inline int draw();

    bool checkAction(actionFunc i_func) {
        return mActionFunc == i_func;
    }

private:
    /* 0x590 */ Z2Creature mCreature;
    /* 0x620 */ mDoExt_McaMorfSO* mpMorf;
    /* 0x624 */ mDoExt_brkAnm* mBrk;
    /* 0x628 */ mDoExt_btkAnm* mBtk;
    /* 0x62C */ dBgS_AcchCir mAcchCir;
    /* 0x66C */ dBgS_ObjAcch mAcch;
    /* 0x844 */ dCcD_Stts mStts;
    /* 0x890 */ dCcD_Sph mSph;
    /* 0x8B8 */ daPy_boomerangMove_c mBoomerangMove;
    /* 0x9C4 */ request_of_phase_process_class mPhase;
    /* 0x9CC */ f32 mParticleScale;
    /* 0x9D0 */ actionFunc mActionFunc;
    /* 0x9DC */ actionFunc mPrevActionFunc;
    /* 0x9E8 */ int field_0x9e8;
    /* 0x9EC */ s16 field_0x9ec;
    /* 0x9EE */ u16 mActionTimer;
    /* 0x9F0 */ u8 field_0x9f0;
    /* 0x9F1 */ u8 field_0x9f1;
    /* 0x9F2 */ u8 field_0x9f2;
    /* 0x9F3 */ u8 mSex;
    /* 0x9F4 */ u8 mIsHIOOwner;
};

STATIC_ASSERT(sizeof(daObjBATTA_c) == 0x9f8);

#endif /* D_A_OBJ_BATTA_H */
