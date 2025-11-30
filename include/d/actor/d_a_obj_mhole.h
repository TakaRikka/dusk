#ifndef D_A_OBJ_MHOLE_H
#define D_A_OBJ_MHOLE_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjMHole_c
 * @brief Magnet Hole
 *
 * @details
 *
 */
class daObjMHole_c : public fopAc_ac_c {
public:
    enum EFF_ID_e {
        EFF_LARGE_e,
        EFF_SMALL_e
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int checkParent();
    int CreateHeap();
    int create();
    int execute();
    void effectSet();
    void effectEnd();
    int draw();
    int _delete();

    void setOn() { mIsOn = TRUE; }
    u8 getType() { return fopAcM_GetParamBit(this, 8, 4); }
    u8 getKind() { return fopAcM_GetParamBit(this, 12, 4); }
    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x574 */ mDoExt_btkAnm* mpBtk1Anm;
    /* 0x57C */ mDoExt_btkAnm* mpBtk2Anm;
    /* 0x580 */ dCcD_Stts mStts;
    /* 0x5BC */ dCcD_Cps mCps;
    /* 0x700 */ u8 mIsOn;
    /* 0x701 */ u8 field_0x701;                      // Value set to mIsOn, but never used
    /* 0x702 */ u8 mKind;
    /* 0x704 */ JPABaseEmitter* mpEmitter[4];
    /* 0x714 */ Z2SoundObjSimple mSound;
};

STATIC_ASSERT(sizeof(daObjMHole_c) == 0x734);

class daObjMHole_HIO_c : public mDoHIO_entry_c {
public:
    daObjMHole_HIO_c();
    ~daObjMHole_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x08 */ cXyz scale;
    /* 0x14 */ u8 scale_adjust_on;
    /* 0x15 */ u8 collision_display;
    /* 0x16 */ u8 angle;
    /* 0x18 */ csXyz angl;
};

#endif /* D_A_OBJ_MHOLE_H */
