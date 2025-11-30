#ifndef D_A_OBJ_SAKUITA_H
#define D_A_OBJ_SAKUITA_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

struct daObjSakuita_Attr_c {
    /* 0x00 */ f32 mGravity;
    /* 0x04 */ f32 mCylR;
    /* 0x08 */ f32 mCylH;
    /* 0x0C */ f32 mOffset;
    /* 0x10 */ f32 mWindRate;
    /* 0x14 */ f32 mSpeedRate;
    /* 0x18 */ f32 mHangingPosition;
    /* 0x1C */ f32 mWindShake;
    /* 0x20 */ f32 mYVibration;
    /* 0x24 */ f32 mYConvergence;
    /* 0x28 */ s16 mYPeriod;
    /* 0x2C */ u8 mBoardWeight;
};

/**
 * @ingroup actors-objects
 * @class daObjSakuita_c
 * @brief Rope Plank
 *
 * @details
 *
 */
class daObjSakuita_c : public fopAc_ac_c {
public:
    void create_init();
    void initBaseMtx();
    void setBaseMtx();
    void setPlatePos();
    void calcAngle();
    ~daObjSakuita_c();

    inline int createHeap();
    inline int draw();
    inline int execute();
    inline int create();

    const daObjSakuita_Attr_c& attr() const { return M_attr; }

    static daObjSakuita_Attr_c const M_attr;

private:
    /* 0x568 */ J3DModel* mModel;
    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ Mtx mMtx;
    /* 0x5A4 */ dCcD_Stts mStts;
    /* 0x5A4 */ dCcD_Cyl mCyl;
    /* 0x71C */ cXyz field_0x71c;
    /* 0x728 */ cXyz field_0x728;
    /* 0x734 */ cXyz field_0x734;
    /* 0x740 */ f32 field_0x740;
    /* 0x744 */ csXyz field_0x744;
    /* 0x74A */ s16 field_0x74a;
    /* 0x74C */ u8 field_0x74c;
};

STATIC_ASSERT(sizeof(daObjSakuita_c) == 0x750);


#endif /* D_A_OBJ_SAKUITA_H */
