#ifndef D_A_OBJ_BHASHI_H
#define D_A_OBJ_BHASHI_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

class Hahen_c {
public:
    Hahen_c();
    void Wall_Check();
    void HahenMotion();
    bool CheckCull();
    bool checkViewArea();
    void Roll_Set(cXyz*, f32, s16);

    /* 0x00 */ cXyz pos;
    /* 0x0C */ cXyz speed;
    /* 0x18 */ cXyz scale;
    /* 0x24 */ csXyz rotation;
    /* 0x2A */ csXyz rot_speed;
    /* 0x30 */ u8 status;
    /* 0x31 */ u8 field_0x31;
};

/**
 * @ingroup actors-objects
 * @class daObjBHASHI_c
 * @brief Pillar
 *
 * @details
 *
 */
class daObjBHASHI_c : public dBgS_MoveBgActor {
public:
    void initCcCylinder();
    void setCcCylinder();
    void Set_Hahen();
    void Set_Speed(s16, s16, f32, f32);
    void Obj_Damage(cXyz);
    void Obj_Damage();
    void Rolling(cXyz*, f32, f32);
    void setBaseMtx();
    int create();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

private:
    /* 0x05A0 */ u8 field_0x5A0[0x05A4 - 0x05A0];
    /* 0x05A4 */ int mMode;
    /* 0x05A8 */ s16 field_0x5a8;
    /* 0x05AC */ Hahen_c mHahen[60];
    /* 0x11DC */ f32 field_0x11dc;
    /* 0x11E0 */ s16 field_0x11e0;
    /* 0x11E2 */ u8 mSwbit;
    /* 0x11E4 */ s16 mDamageTimer;
    /* 0x11E6 */ u8 field_0x11E6[0x11EC - 0x11E6];
    /* 0x11EC */ J3DModel* mpPillarMdl;
    /* 0x11F0 */ J3DModel* mpHahenMdls[60];
    /* 0x12E0 */ request_of_phase_process_class mPhase;
    /* 0x12E8 */ dCcD_Stts mCcStts;
    /* 0x1324 */ dCcD_Cyl mCcCyl;
    /* 0x1460 */ u8 field_0x1460;
    /* 0x1461 */ u8 mIsHahenSimpleEntry;
};

STATIC_ASSERT(sizeof(daObjBHASHI_c) == 0x1464);

#endif /* D_A_OBJ_BHASHI_H */
