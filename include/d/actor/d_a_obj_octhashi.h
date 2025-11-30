#ifndef D_A_OBJ_OCTHASHI_H
#define D_A_OBJ_OCTHASHI_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

/**
 * @ingroup actors-objects
 * @class daObjOCTHASHI_c
 * @brief Morpheel Pillar
 *
 * @details
 *
 */
class daObjOCTHASHI_c : public dBgS_MoveBgActor {
public:
    void initCcCylinder();
    void SetCoSph();
    void SetCoCyl();
    void HakaiSet2(int);
    void HakaiMotion2();
    void CylAction();
    void SphAction();
    void SphAction2();
    void Action();
    void setBaseMtx();
    int CreateHeap();
    int create();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

private:
    /* 0x5a0 */ cXyz field_0x5a0[8];
    /* 0x600 */ cXyz field_0x600[8];
    /* 0x660 */ csXyz field_0x660[8];
    /* 0x690 */ csXyz field_0x690[8];
    /* 0x6c0 */ u8 field_0x6c0[8];
    /* 0x6c8 */ u8 field_0x6c8;
    /* 0x6c9 */ u8 field_0x6c9[0x6cb - 0x6c9];
    /* 0x6cb */ u8 field_0x6cb;
    /* 0x6cc */ s16 field_0x6cc;
    /* 0x6d0 */ f32 field_0x6d0;
    /* 0x6d4 */ f32 field_0x6d4;
    /* 0x6d8 */ f32 field_0x6d8;
    /* 0x6dc */ s16 field_0x6dc;
    /* 0x6de */ s16 field_0x6de;
    /* 0x6e0 */ int mPieceNum;
    /* 0x6e4 */ u32 field_0x6e4;
public:
    /* 0x6e8 */ s16 field_0x6e8;
    /* 0x6ec */ Z2Creature mSound;
private:
    /* 0x077c */ u8 field_0x77c[4];
    /* 0x0780 */ J3DModel* mpModel[8];
    /* 0x07a0 */ u8 field_0x7a0[0x7a4 - 0x7a0];
    /* 0x07a4 */ request_of_phase_process_class mPhaseReq;
    /* 0x07ac */ dCcD_Stts field_0x7ac[8];
    /* 0x098c */ dCcD_Sph mColliders[8];
    /* 0x134c */ dCcD_Cyl mCyl;
    /* 0x1488 */ dCcU_AtInfo field_0x1488;
    /* 0x148C */ int field_0x148c;
};

STATIC_ASSERT(sizeof(daObjOCTHASHI_c) == 0x14b0);


#endif /* D_A_OBJ_OCTHASHI_H */
