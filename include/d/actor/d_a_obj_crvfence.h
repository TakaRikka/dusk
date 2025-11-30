#ifndef D_A_OBJ_CRVFENCE_H
#define D_A_OBJ_CRVFENCE_H

#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjCRVFENCE_c
 * @brief Caravan Fence
 *
 * @details
 *
*/

class daObjCRVFENCE_c : public dBgS_MoveBgActor {
public:
    BOOL CheckVec();
    BOOL Wall_Check(cXyz, cXyz);
    void BgRelease();
    void PosSet(cXyz*);
    void FenceMove(u8&, cXyz*, csXyz*, cXyz*, s16&);
    void BrokenAction();
    void NormalAction();
    bool checkViewArea(cXyz*);
    void Action();
    void setBaseMtx();
    int CreateHeap();
    cPhs__Step create();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

private:
    /* 0x05A0 */ int field_0x5a0;
    /* 0x05A4 */ int mAction;
    /* 0x05A8 */ u8 field_0x5a8;
    /* 0x05AA */ s16 field_0x5aa;
    /* 0x05AC */ cXyz field_0x5ac;
    /* 0x05B8 */ cXyz field_0x5b8;
    /* 0x05C4 */ csXyz field_0x5c4;
    /* 0x05CA */ u8 field_0x5ca;
    /* 0x05CC */ s16 field_0x5cc;
    /* 0x05D0 */ cXyz field_0x5d0;
    /* 0x05DC */ cXyz field_0x5dc;
    /* 0x05E8 */ csXyz field_0x5e8;
    /* 0x05EE */ u8 field_0x5ee;
    /* 0x05F0 */ s16 field_0x5f0;
    /* 0x05F2 */ s16 field_0x5f2;
    /* 0x05F4 */ bool field_0x5f4;
    /* 0x05F8 */ cXyz field_0x5f8[6];
    /* 0x0640 */ f32 field_0x640;
    /* 0x0644 */ int field_0x644;
    /* 0x0648 */ u8 field_0x648[0x65c - 0x648];
    /* 0x065C */ J3DModel* field_0x65c;
    /* 0x0660 */ J3DModel* field_0x660;
    /* 0x0664 */ J3DModel* field_0x664;
    /* 0x0668 */ request_of_phase_process_class mPhase;
    /* 0x0670 */ dBgS_AcchCir mAcchCir;
    /* 0x06B0 */ dBgS_ObjAcch mBgc;
    /* 0x0888 */ dCcD_Stts mStts;
    /* 0x08C4 */ dCcD_Sph mSphs[6];
    /* 0x1014 */ u8 field_0x1014[0x103c - 0x1014];
};

STATIC_ASSERT(sizeof(daObjCRVFENCE_c) == 0x103c);


#endif /* D_A_OBJ_CRVFENCE_H */
