#ifndef D_A_OBJ_HHASHI_H
#define D_A_OBJ_HHASHI_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjHHASHI_c
 * @brief Pillar
 *
 * @details
 *
 */
class daObjHHASHI_c : public dBgS_MoveBgActor {
public:
    void initCcCylinder();
    void setCcCylinder();
    void Set_Hahen(cXyz*);
    void Set_Speed(s16, s16, f32, f32, f32, f32);
    void Obj_Damage();
    void Wall_Check(s16);
    void Hahen_Motion(s16);
    void CheckCull();
    bool checkViewArea(int);
    void Rolling(cXyz*, f32, f32, s8);
    void Roll_Set(cXyz*, f32, s16);
    void setBaseMtx();
    int CreateHeap();
    int create();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    /* 0x05A0 */ int field_0x5a0;
    /* 0x05A4 */ int field_0x5a4;
    /* 0x05A8 */ s8 field_0x5a8;
    /* 0x05AA */ s16 field_0x5aa;
    /* 0x05AC */ int field_0x5ac;
    /* 0x05B0 */ cXyz field_0x5b0[50];
    /* 0x0808 */ cXyz field_0x808[50];
    /* 0x0A60 */ cXyz field_0xa60[50];
    /* 0x0CB8 */ f32 field_0xcb8;
    /* 0x0CBC */ csXyz field_0xcbc[50];
    /* 0x0DE8 */ csXyz field_0xde8[50];
    /* 0x0F14 */ u8 field_0xf14[50];
    /* 0x0F46 */ s16 field_0xf46;
    /* 0x0F48 */ int field_0xf48;
    /* 0x0F4C */ J3DModel* mModel;
    /* 0x0F50 */ J3DModel* mShardModels[50];
    /* 0x1018 */ request_of_phase_process_class mPhase;
    /* 0x1020 */ dCcD_Stts mStts;
    /* 0x105c */ dCcD_Cyl mCyl;
    /* 0x1198 */ u8 field_0x1198;
    /* 0x1199 */ u8 field_0x1199;
};

STATIC_ASSERT(sizeof(daObjHHASHI_c) == 0x119c);


#endif /* D_A_OBJ_HHASHI_H */
