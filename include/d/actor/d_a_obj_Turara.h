#ifndef D_A_OBJ_TURARA_H
#define D_A_OBJ_TURARA_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"
#include "d/d_event_lib.h"
#include "d/d_bg_w.h"

/**
 * @ingroup actors-objects
 * @class daTurara_c
 * @brief Icicle
 *
 * @details
 *
 */

class daTurara_c : public dBgS_MoveBgActor, public request_of_phase_process_class, public dEvLib_callback_c {
public:
    daTurara_c() : dEvLib_callback_c(this) {}
    ~daTurara_c() {}

    void setBaseMtx();
    int CreateHeap();
    int create();
    void setFallStat();
    int Execute(Mtx**);
    void move();
    void init_modeWait();
    void modeWait();
    bool eventStart();
    void init_modeDropInit();
    void modeDropInit();
    void init_modeDrop();
    void modeDrop();
    void bgCheck();
    void init_modeDropEnd();
    void modeDropEnd();
    void init_modeDropEnd2();
    void modeDropEnd2();
    int Draw();
    int Delete();

    int getSwBit1() { return fopAcM_GetParamBit(this, 0, 8); }
    int getSwBit2() { return fopAcM_GetParamBit(this, 8, 8); }
    int getSwBit3() { return fopAcM_GetParamBit(this, 16, 8); }

    s32 getEvetID() {
        return fopAcM_GetParamBit(this, 0x18, 8);
    }

    u8 getWaitTime() { return shape_angle.z; }
    int getItemTbleNum() { return shape_angle.x >> 8 & 0xff; }
    int getState() { return shape_angle.x; }

    static const dCcD_SrcGObjInf mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;

private:
    /* 0x5b8 */ request_of_phase_process_class mPhaseReq;
    /* 0x5c0 */ J3DModel* mpModel[2];
    /* 0x5c8 */ u8 mMode;
    /* 0x5c9 */ u8 field_0x5c9;
    /* 0x5cc */ dCcD_Stts mColliderStts;
    /* 0x608 */ dCcD_Cyl mCylCollider;
    /* 0x744 */ u8 mWaitTime;
    /* 0x748 */ u32 field_0x748;
    /* 0x74c */ f32 field_0x74c;
    /* 0x750 */ s16 field_0x750;
    /* 0x752 */ s16 field_0x752;
    /* 0x754 */ s16 field_0x754;
    /* 0x758 */ f32 field_0x758;
    /* 0x75c */ f32 field_0x75c;
    /* 0x760 */ f32 field_0x760;
    /* 0x764 */ u8 mInWater;
    /* 0x768 */ dBgS_AcchCir mAcchCir;
    /* 0x7a8 */ dBgS_ObjAcch mAcch;
    /* 0x980 */ dBgW* field_0x980;
    /* 0x984 */ u8 field_0x984;
    /* 0x988 */ u32 mEmID;
    /* 0x98c */ u8 field_0x98c;
    /* 0x98d */ u8 field_0x98d[0x994 - 0x98d];
    /* 0x994 */ u8 field_0x994;
};

STATIC_ASSERT(sizeof(daTurara_c) == 0x998);

#endif /* D_A_OBJ_TURARA_H */
