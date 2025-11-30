#ifndef D_A_OBJ_THASHI_H
#define D_A_OBJ_THASHI_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_uty.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
/**
 * @ingroup actors-objects
 * @class daObjTHASHI_c
 * @brief T Pillar
 *
 * @details
 *
 */
class daObjTHASHI_c : public dBgS_MoveBgActor {
public:
    void initCcCylinder();
    void setBaseMtx();
    int CreateHeap();
    int create();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

// private:

    /* 0x5A0  */ dBgW* unk5A0;
    /* 0x5A4  */ u32 unk5A4;
    /* 0x5A8  */ u32 unk5A8;
    /* 0x5AC  */ cXyz unk5AC;
    /* 0x5B8  */ csXyz mRot;
    /* 0x5C0  */ cXyz mTrans;
    /* 0x5CC  */ u32 unk5CC;
    /* 0x5D0  */ cXyz unk5D0;
    /* 0x5DC  */ cXyz unk5DC[5];
    /* 0x618  */ u32 unk618;
    /* 0x61C  */ u32 unk61C;
    /* 0x620  */ u32 unk620;
    /* 0x624  */ cXyz unk624;
    /* 0x630  */ csXyz unk630;
    /* 0x636  */ s16 unk636;
    /* 0x638  */ cXyz mCollectiveTrans[50];
    /* 0x890  */ cXyz unk890[50];
    /* 0xAE8  */ cXyz mCollectiveScale[50];
    /* 0xD40  */ u32 unkD40;
    /* 0xD44  */ u32 unkD44;
    /* 0xD48  */ csXyz mCollectiveRot[50];
    /* 0xE74  */ csXyz unkE74[50];
    /* 0xFA0  */ u8 unkFA0[0x100];
    /* 0x10A0 */ f32 unk10A0;
    /* 0x10A4 */ u32 unk10A4;
    /* 0x10A8 */ u32 unk10A8;
    /* 0x10AC */ Z2SoundObjSimple unk10AC;
    /* 0x10CC */ u32 unk10CC;
    /* 0x10D0 */ u32 unk10D0;
    /* 0x10D4 */ J3DModel* mThashi00Model;
    /* 0x10D8 */ J3DModel* mThashi01Model;
    /* 0x10DC */ J3DModel* mThashi03Models[50];
    /* 0x11A4 */ J3DModel* mThashi02Model;
    /* 0x11A8 */ request_of_phase_process_class unk11A8;
    /* 0x11B0 */ dBgS_AcchCir unk11B0;
    /* 0x11F0 */ dBgS_ObjAcch unk11F0;
    /* 0x13C8 */ dCcD_Stts unk13C8;
    /* 0x1404 */ dCcD_Stts unk1404;
    /* 0x1440 */ dCcD_Sph unk1440[5];
    /* 0x1A58 */ dCcD_Cyl unk1A58;
    /* 0x1B94 */ dCcU_AtInfo unk1B94;
    /* 0x1BB8 */ u32 unk1BB8;
};

STATIC_ASSERT(sizeof(daObjTHASHI_c) == 0x1bbc);


#endif /* D_A_OBJ_THASHI_H */
