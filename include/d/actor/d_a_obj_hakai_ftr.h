#ifndef D_A_OBJ_HAKAI_FTR_H
#define D_A_OBJ_HAKAI_FTR_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjHFtr_c
 * @brief Destruction Furniture
 *
 * @details
 *
 */
class daObjHFtr_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    daObjHFtr_c();
    virtual ~daObjHFtr_c();
    inline u8 getNameId() {
        return fopAcM_GetParamBit(this, 0, 4);
    }
    int createHeap();
    int create();
    int Delete();
    int draw();
    int execute();
    void init();
    void setModelMtx();
    bool chkHit();
    void callEmt();

    static dCcD_SrcCyl const s_CcDCyl;

private:
    /* 0x574 */ Mtx mMtx;
    /* 0x5A4 */ dBgW* field_0x5a4;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ dCcD_Stts mStts;
    /* 0x5E8 */ dCcD_Cyl mCyls[2];
};

STATIC_ASSERT(sizeof(daObjHFtr_c) == 0x860);


#endif /* D_A_OBJ_HAKAI_FTR_H */
