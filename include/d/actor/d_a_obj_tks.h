#ifndef D_A_OBJ_TKS_H
#define D_A_OBJ_TKS_H

#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_npc4.h"

struct daObjTks_HIOParam {
    /* 0x00 */ daNpcF_HIOParam common;
    /* 0x6C */ f32 offset_x;
    /* 0x70 */ f32 offset_y;
    /* 0x74 */ f32 offset_z;
    /* 0x78 */ f32 spring_factor;
    /* 0x7C */ f32 spring_atten;
};

class daObjTks_Param_c {
public:
    virtual ~daObjTks_Param_c() {}

    static const daObjTks_HIOParam m;
};

/**
 * @ingroup actors-objects
 * @class daObjTks_c
 * @brief Ooccoo Jr. (small)
 *
 * @details
 *
*/

class daObjTks_c : public daNpcF_c {
public:
    daObjTks_c();
    virtual ~daObjTks_c();

    int Create();
    int CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    int ctrlJoint(J3DJoint*, J3DModel*);
    static int createHeapCallBack(fopAc_ac_c*);
    static int ctrlJointCallBack(J3DJoint*, int);
    void reset();
    inline int setAction(void (daObjTks_c::*)());
    void wait();
    void talk();
    void demo();
    void warp();
    inline void lookat();

    virtual inline void setParam();
    virtual inline BOOL main();
    virtual inline BOOL ctrlBtk();
    virtual inline void setMtx();
    virtual inline void setAttnPos();
    virtual inline bool setExpressionAnm(int, bool);
    virtual inline bool setExpressionBtp(int);
    virtual inline void setExpression(int, f32);
    virtual inline void setMotionAnm(int, f32);
    virtual inline void setMotion(int, f32, int);
    virtual inline BOOL drawDbgInfo();
    virtual inline void drawOtherMdls();

    void calcSpringF(f32* param_0, f32 param_1, f32* param_2) {
        *param_2 = daObjTks_Param_c::m.spring_atten * (*param_2 + (daObjTks_Param_c::m.spring_factor * (*param_0 - param_1)));
        *param_0 += *param_2;
    }

    inline void playExpression();
    inline void playMotion();

    bool chkAction(void (daObjTks_c::*i_action)()) { return i_action == mAction; }

    void setStart() { field_0xde0 = 1; }

    void setLookMode(int i_mode) {
        if (i_mode >= 0 && i_mode < 6 && i_mode != mLookMode) {
            mLookMode = i_mode;
        }
    }

    void setExpressionTalkAfter() {
        (void)mExpression;
        setExpression(1, -1.0f);
    }

    /* 0xB48 */ Z2Creature mSound;
    /* 0xBD8 */ daNpcF_MatAnm_c* mpMatAnm;
    /* 0xBDC */ daNpcF_Lookat_c mLookat;
    /* 0xC78 */ u8 field_0xC78[0xC7C - 0xC78];
    /* 0xC7C */ dCcD_Cyl mCcCyl;
    /* 0xDB8 */ void (daObjTks_c::*mAction)();
    /* 0xDC4 */ request_of_phase_process_class mPhase;
    /* 0xDCC */ f32 field_0xdcc;
    /* 0xDD0 */ f32 field_0xdd0;
    /* 0xDD4 */ u8 field_0xDD4[0xDDC - 0xDD4];
    /* 0xDDC */ s16 mLookMode;
    /* 0xDDE */ u16 field_0xdde;
    /* 0xDE0 */ u8 field_0xde0;
    /* 0xDE1 */ u8 field_0xde1;
};

STATIC_ASSERT(sizeof(daObjTks_c) == 0xDE4);

#endif /* D_A_OBJ_TKS_H */
