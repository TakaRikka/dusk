#ifndef D_A_OBJ_DAMCPS_H
#define D_A_OBJ_DAMCPS_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor.h"
#include "m_Do/m_Do_hostIO.h"

/**
 * @ingroup actors-objects
 * @class daObjDamCps_c
 * @brief Damage Cylinder
 *
 * @details
 *
 */
class daObjDamCps_c : public fopAc_ac_c {
#if DEBUG
public:
    daObjDamCps_c() {};
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int create();
    int execute();
    int draw();
    int _delete();
    u8 getSwNo();
    
private:
    /* 0x0570 */ dCcD_Stts mStts;
    /* 0x05AC */ dCcD_Cps mCps;
    /* 0x06F0 */ cM3dGCpsS mCapsule;
    /* 0x070C */ Mtx mMtx;
    /* 0x073C */ cXyz mScale;
#endif
};

class daObjDamCps_HIO_c : public mDoHIO_entry_c {
public:
    daObjDamCps_HIO_c();

    void genMessage(JORMContext*);
    virtual ~daObjDamCps_HIO_c() {};

    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ u8 mAdj;
    /* 0x0E */ csXyz mAngle;
    /* 0x14 */ cXyz mScale;
};

#endif /* D_A_OBJ_DAMCPS_H */
