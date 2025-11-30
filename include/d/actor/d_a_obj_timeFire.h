#ifndef D_A_OBJ_TIMEFIRE_H
#define D_A_OBJ_TIMEFIRE_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daTimeFire_c
 * @brief Time Fire
 *
 * @details
 *
 */
class daTimeFire_c : public fopAc_ac_c {
public:
    void setBaseMtx();
    int create();
    void lightInit();
    void setLight();
    void cutLight();
    int Execute();
    bool fireCheck(u8);
    int Draw();
    int Delete();

    int getSw() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getStartTime() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getEndTime() { return fopAcM_GetParamBit(this, 16, 8); }
    u8 getScale() { return fopAcM_GetParamBit(this, 24, 8); }

private:
    /* 0x568 */ u8 field_0x568;
    /* 0x569 */ u8 field_0x569;
    /* 0x56A */ u8 mStartTime;
    /* 0x56B */ u8 mEndTime;
    /* 0x56C */ bool field_0x56c;
    /* 0x56D */ bool field_0x56d;
    /* 0x570 */ cXyz field_0x570;
    /* 0x57C */ cXyz mParticleScale;
    /* 0x588 */ int mParticleId1;
    /* 0x58c */ int mParticleId2;
    /* 0x590 */ int mParticleId3;
    /* 0x594 */ cXyz field_0x594;
    /* 0x5A0 */ LIGHT_INFLUENCE mLightInfluence;
};

STATIC_ASSERT(sizeof(daTimeFire_c) == 0x5c0);

class daTimeFire_HIO_c : public mDoHIO_entry_c {
public:
    daTimeFire_HIO_c();
    virtual ~daTimeFire_HIO_c() {}

    void genMessage(JORMContext*);

    u8 field_0x4;
};


#endif /* D_A_OBJ_TIMEFIRE_H */
