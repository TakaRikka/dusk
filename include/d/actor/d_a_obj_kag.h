#ifndef D_A_OBJ_KAG_H
#define D_A_OBJ_KAG_H

#include "SSystem/SComponent/c_phase.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_s_gnd_chk.h"
#include "d/d_cc_d.h"
#include "d/d_insect.h"

/**
 * @ingroup actors-objects
 * @class daObjKAG_c
 * @brief Insect - Dayfly
 *
 * @details
 *
*/

class daObjKAG_c : public dInsect_c {
public:
    typedef void (daObjKAG_c::*actionFunc)();

    void setAction(actionFunc);
    bool Kag_Bgcheck(cXyz*, csXyz*, cXyz*);
    void fly();
    void wait();
    void walk();
    void bin_wait();
    void bin_action();
    void hook();
    void boomerang();
    void action();
    void checkGroundPos();
    void hit_check();
    int execute();
    int _delete();
    void setBaseMtx();
    cPhs__Step create();
    void kag_setParticle();
    inline int CreateHeap();
    inline int draw();
private:
    /* 0x590 */ request_of_phase_process_class mPhase;
    /* 0x598 */ Z2Creature mSound;
    /* 0x628 */ mDoExt_McaMorfSO* mpModelMorf;
    /* 0x62C */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x630 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x634 */ dBgS_GndChk mGndChk;
    /* 0x688 */ dCcD_Stts mStts;
    /* 0x6C4 */ dCcD_Sph mSph;
    /* 0x7FC */ daPy_boomerangMove_c mBoomerangMove;
    /* 0x808 */ cXyz field_0x808;
    /* 0x814 */ csXyz field_0x814;
    /* 0x81C */ f32 field_0x81c;
    /* 0x820 */ f32 field_0x820;
    /* 0x824 */ actionFunc mAction;
    /* 0x830 */ actionFunc mPrevAction;
    /* 0x83C */ int field_0x83c;
    /* 0x840 */ u8 field_0x840[0x844 - 0x840];
    /* 0x844 */ f32 field_0x844;
    /* 0x848 */ f32 field_0x848;
    /* 0x84C */ s16 field_0x84c;
    /* 0x84E */ s16 field_0x84e;
    /* 0x850 */ u8 field_0x850[0x852 - 0x850];
    /* 0x852 */ s16 field_0x852;
    /* 0x854 */ s16 field_0x854;
    /* 0x856 */ s16 field_0x856;
    /* 0x858 */ u8 field_0x858[0x85a - 0x858];
    /* 0x85A */ u8 field_0x85a;
    /* 0x85B */ u8 field_0x85b;
    /* 0x85C */ u8 field_0x85c;
    /* 0x85D */ u8 field_0x85d;
    /* 0x85E */ u8 field_0x85e;
    /* 0x85F */ u8 field_0x85f;
};

STATIC_ASSERT(sizeof(daObjKAG_c) == 0x860);

#endif /* D_A_OBJ_KAG_H */
