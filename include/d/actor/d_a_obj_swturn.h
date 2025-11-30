#ifndef D_A_OBJ_SWTURN_H
#define D_A_OBJ_SWTURN_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjSwTurn_c
 * @brief Arbiter's Ground Turn Switch
 *
 * @details
 *
 */
class daObjSwTurn_c : public dBgS_MoveBgActor {
public:
    typedef void (daObjSwTurn_c::*modeFunc)();

    enum Mode {
        MODE_WAIT,
        MODE_ROTATE,
    };

    enum Kind {
        KIND_0,
        KIND_1,
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    void mode_proc_call();
    void init_modeWait();
    void modeWait();
    void init_modeRotate();
    void modeRotate();
    void rotate_sub_0();
    void rotate_sub_1();
    int Draw();
    int Delete();

    u8 getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getModelType() { return fopAcM_GetParamBit(this, 8, 4); }
    u8 getCount() { return fopAcM_GetParamBit(this, 12, 8); }
    u8 getSwNo2() { return fopAcM_GetParamBit(this, 20, 8); }
    u8 getKind() { return fopAcM_GetParamBit(this, 28, 4); }
    u16 getRevCount() { return mRevCount; }
    int getRotateAngle() {
        return (mKind == 0) ? field_0x5ba + field_0x5c4 * 0x4000 :
                                       field_0x5ba + (field_0x5c4 << 0xe) / 3;
    }

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mModel;
    /* 0x5AC */ u8 mModelType;
    /* 0x5AD */ u8 mKind;
    /* 0x5AE */ u8 mMode;
    /* 0x5AF */ u8 field_0x5af;
    /* 0x5B0 */ u8 field_0x5b0[2];
    /* 0x5B4 */ f32 field_0x5b4;
    /* 0x5B8 */ s16 field_0x5b8;
    /* 0x5BA */ s16 field_0x5ba;
    /* 0x5BC */ s16 field_0x5bc;
    /* 0x5C0 */ int field_0x5c0;
    /* 0x5C4 */ s16 field_0x5c4;
    /* 0x5C6 */ u8 field_0x5c6;
    /* 0x5C8 */ int field_0x5c8;
    /* 0x5CC */ u16 field_0x5cc;
    /* 0x5CE */ u8 field_0x5ce;
    /* 0x5CF */ u8 field_0x5cf;
    /* 0x5D0 */ u8 field_0x5d0;
    /* 0x5D1 */ u8 field_0x5d1;
    /* 0x5D2 */ u16 mRevCount;
    /* 0x5D4 */ u16 field_0x5d4;
    /* 0x5D8 */ dBgW_Base::PushPullLabel mPushPullLabel;
    /* 0x5DC */ s16 field_0x5dc;
};

STATIC_ASSERT(sizeof(daObjSwTurn_c) == 0x5e0);

#endif /* D_A_OBJ_SWTURN_H */
