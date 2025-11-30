#ifndef D_A_OBJ_SWPROPELLER_H
#define D_A_OBJ_SWPROPELLER_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjSwPr_c
 * @brief Boomerang Switch
 *
 * @details
 *
 */
class daObjSwPr_c : public fopAc_ac_c {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int setRotateTime();
    void switchCtrl();
    void execute_type_boomerang();
    void execute_type_wind();
    int execute();
    int draw();
    int _delete();

    u8 getNameArg() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getMdlType() { return fopAcM_GetParamBit(this, 0x10, 4); }
    int getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    u32 getSwbit2() { return fopAcM_GetParamBit(this, 0x18, 8); }

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ dCcD_Stts mCcStts;
    /* 0x5B0 */ dCcD_Cyl mCyl1;
    /* 0x6EC */ dCcD_Cyl mCyl2;
    /* 0x828 */ s16 mRotateSpeed;
    /* 0x82A */ s16 mRotateInitSpeed;
    /* 0x82C */ s16 field_0x82c;  // rotate angle?
    /* 0x82E */ s16 mRotateTime;
    /* 0x830 */ u16 mKaitenJntID;
    /* 0x832 */ s16 mSwDelayTimer;
    /* 0x834 */ u16 field_0x834;
    /* 0x836 */ u16 mSwOffDelayTimer;
    /* 0x838 */ u8 field_0x838;
    /* 0x839 */ u8 field_0x839;
    /* 0x83A */ u8 field_0x83a;
    /* 0x83B */ u8 field_0x83b;
    /* 0x83C */ u8 mNameArg;
    /* 0x83D */ u8 field_0x83d;
    /* 0x83E */ u8 mModelType;
};

STATIC_ASSERT(sizeof(daObjSwPr_c) == 0x840);

#endif /* D_A_OBJ_SWPROPELLER_H */
