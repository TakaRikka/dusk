#ifndef D_A_OBJ_YOBIKUSA_H
#define D_A_OBJ_YOBIKUSA_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjYobikusa_c
 * @brief Hawk Grass
 *
 * @details
 *
 */
class daObjYobikusa_c : public fopAc_ac_c {
public:
    enum Mode_e {
        /* 0 */ MODE_SWING_WIND,
        /* 1 */ MODE_PUSH_DOWN,
        /* 2 */ MODE_PICK_LEAF,
    };

    enum CallType_e {
        /* 0 */ CALL_TYPE_FLY,
        /* 1 */ CALL_TYPE_HORSE,
    };

    typedef void (daObjYobikusa_c::*actionFunc)();
    typedef struct actionFuncEntry {
        actionFunc initFn;
        actionFunc execFn;
    } actionFuncEntry;

    typedef struct attributes {
        /* 0x00 */ f32 field_0x00;
        /* 0x04 */ f32 field_0x04;
        /* 0x08 */ f32 field_0x08;
        /* 0x0C */ f32 field_0x0c;
        /* 0x10 */ f32 field_0x10;
        /* 0x14 */ f32 field_0x14;
        /* 0x18 */ f32 field_0x18;
        /* 0x1C */ f32 field_0x1c;
        /* 0x20 */ f32 field_0x20;
        /* 0x24 */ f32 mJointScaleStep;
        /* 0x28 */ f32 mNewLeafInitialAmplitude;
        /* 0x2C */ f32 field_0x2c;  // Unused?
        /* 0x30 */ f32 mPickLeafInitialAmplitude;
        /* 0x34 */ f32 mPickLeafAmplitudeDecay;
        /* 0x38 */ s16 field_0x38;  // Unused?
        /* 0x3A */ s16 field_0x3a;
        /* 0x3C */ s16 mNewLeafTickSpeed;
        /* 0x3E */ s16 mPickLeafTickSpeed;
    } attributes;

    void setAction(daObjYobikusa_c::Mode_e);
    void callInit();
    void callExecute();
    void initSwingWind();
    void executeSwingWind();
    void initPushDown();
    void executePushDown();
    void initPickLeaf();
    void executePickLeaf();
    void create_init();
    void initBaseMtx();
    void getJointAngle(csXyz*, int);
    f32 getJointScale(int);
    void initCcCylinder();
    void setCcCylinder();
    bool toPickLeaf();
    bool setNewLeaf();
    int execute();

    inline int createHeap();
    inline int draw();
    inline cPhs__Step create();
    inline ~daObjYobikusa_c();

    static attributes const M_attr;
    static actionFuncEntry ActionTable[3];

    const attributes* attr() const { return &M_attr; }
    int getType() { return argument & 0x7F; }
    u8 getPathID() { return fopAcM_GetParam(this); }
    bool isPlayerCorrect() { return (s8)(u8)(fopAcM_GetParam(this) >> 8) > 0; }

private:
    /* 0x568 */ J3DModel* mpActiveModel;
    /* 0x56C */ J3DModel* mpModelTypeA;
    /* 0x570 */ J3DModel* mpModelTypeB;
    /* 0x574 */ Mtx mMtx;
    /* 0x5A4 */ request_of_phase_process_class mPhase;
    /* 0x5AC */ actionFuncEntry* mActionEntry;
    /* 0x5B0 */ int mMode;
    /* 0x5B4 */ dCcD_Stts mStts;
    /* 0x5F0 */ dCcD_Cyl mCcCyl;
    /* 0x72C */ cBgS_PolyInfo mPolyInfo;
    /* 0x73C */ f32 mGroundH;
    /* 0x740 */ f32 mJointScale;
    /* 0x744 */ f32 mNewLeafAmplitude;
    /* 0x748 */ f32 mPickLeafAmplitude;
    /* 0x74C */ csXyz mLeafAngles[3];
    /* 0x760 */ char* mResName;
    /* 0x764 */ s16 field_0x764;
    /* 0x766 */ s16 field_0x766;
    /* 0x768 */ s16 mNewLeafTick;
    /* 0x76A */ s16 mPickLeafTick;
    /* 0x76C */ s16 mPlayerDeltaAngle;
};  // Size: 0x770

STATIC_ASSERT(sizeof(daObjYobikusa_c) == 0x770);

#endif /* D_A_OBJ_YOBIKUSA_H */
