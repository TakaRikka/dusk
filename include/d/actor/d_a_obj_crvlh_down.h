#ifndef D_A_OBJ_CRVLH_DOWN_H
#define D_A_OBJ_CRVLH_DOWN_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjCRVLH_DW_c
 * @brief Bulblin Tower (Lower Half)
 *
 * @details
 *
 */
class daObjCRVLH_DW_c : public dBgS_MoveBgActor {
public:
    void initCcCylinder();
    void SetCcSyl();
    void BgRelease();
    void Break();
    void Obj_Hit();
    void setBaseMtx();
    int create();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

private:
    /* 0x5A0 */ fpc_ProcID field_0x5a0;
    /* 0x5A4 */ fpc_ProcID field_0x5a4;
    /* 0x5A8 */ cXyz field_0x5a8;
    /* 0x5B4 */ f32 field_0x5b4;
    /* 0x5B8 */ u8 field_0x5b8;
    /* 0x5B9 */ u8 field_0x5b9;
    /* 0x5BA */ u8 field_0x5ba;
    /* 0x5BB */ u8 field_0x5bb;
    /* 0x5BC */ u8 mParam;
    /* 0x5BD */ u8 field_0x5bd[6];
    /* 0x5C4 */ J3DModel* mpModel;
    /* 0x5C8 */ request_of_phase_process_class mPhase;
    /* 0x5D0 */ dCcD_Stts mStts;
    /* 0x60C */ dCcD_Cyl mCyl;
    /* 0x748 */ u8 field_0x748[4];
};

STATIC_ASSERT(sizeof(daObjCRVLH_DW_c) == 0x74c);

#endif /* D_A_OBJ_CRVLH_DOWN_H */
