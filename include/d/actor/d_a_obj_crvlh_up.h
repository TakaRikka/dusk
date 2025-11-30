#ifndef D_A_OBJ_CRVLH_UP_H
#define D_A_OBJ_CRVLH_UP_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjCRVLH_UP_c
 * @brief Bulblin Tower (Upper Half)
 *
 * @details
 *
 */
class daObjCRVLH_UP_c : public dBgS_MoveBgActor {
public:
    void HakaiSet();
    void UpMotion();
    void Hakai();
    void setBaseMtx();
    int create();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

private:
    /* 0x568 */ u8 field_0x568[0x5a4 - 0x5a0];
    /* 0x5A4 */ fpc_ProcID field_0x5a4;
    /* 0x5A8 */ fpc_ProcID field_0x5a8;
    /* 0x5AC */ cXyz field_0x5ac[3];
    /* 0x5D0 */ f32 field_0x5d0;
    /* 0x5D4 */ u8 field_0x5d4;
    /* 0x5D6 */ s16 field_0x5d6;
    /* 0x5D8 */ s16 field_0x5d8;
    /* 0x5DA */ u8 field_0x5da;
    /* 0x5DB */ u8 field_0x5db;
    /* 0x5DC */ f32 field_0x5dc;
    /* 0x5E0 */ u8 field_0x5e0[8];
    /* 0x5E8 */ J3DModel* mpModel;
    /* 0x5EC */ request_of_phase_process_class mPhase;
    /* 0x5F4 */ dCcD_Stts mStts;
    /* 0x630 */ dCcD_Cyl mCyl;
    /* 0x76C */ u8 field_0x76c;
};

STATIC_ASSERT(sizeof(daObjCRVLH_UP_c) == 0x770);

#endif /* D_A_OBJ_CRVLH_UP_H */
