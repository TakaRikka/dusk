#ifndef D_A_OBJ_FLAG2_H
#define D_A_OBJ_FLAG2_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

struct daObjFlag2_Attr_c {
    /* 0x00 */ f32 mGravity;
    /* 0x04 */ f32 mSpringCoeeficient;
    /* 0x08 */ f32 mDecayRate;
    /* 0x0C */ f32 mWindCoefficient;
    /* 0x10 */ f32 mTornado;
};

class FlagCloth_c : public J3DPacket {
public:
    void initFlagPos(cXyz*, fopAc_ac_c*);
    cXyz calcFlagFactor(cXyz*, cXyz*, cXyz*, int);
    void calcFlagFactorSub(cXyz*, cXyz*, cXyz*, f32);
    void calcFlagNormal(cXyz*, int);
    inline void calcFlagNormalBack();
    void initCcSphere(fopAc_ac_c*);
    void setCcSphere();
    void execute();
    cXyz getTargetPos() { return mPositions[4]; }

    void draw();

    cXyz* getPos() { return mPositions; }
    cXyz* getNormal() { return mNormals; }
    cXyz* getNormalBack() { return mNormalBacks; }
    cXyz* getVec() { return mVecs; }
    void setSpringRate(f32 rate) { mSpringRate = rate; }
    void setWindRate(f32 rate) { mWindRate = rate; }
    void setDecayRate(f32 rate) { mDecayRate = rate; }
    void setGravity(f32 rate) { mGravity = rate; }
    void setTornado(f32 tornado) { mTornado = tornado; }
    GXTexObj* getImageTexObj() { return &mTexObj; }
    void setTexCoord_p(void* texCoordP) { mpTexCoord = texCoordP; }

    /* 0x010 */ dKy_tevstr_c mTevStr;
    /* 0x398 */ GXTexObj mTexObj;
    /* 0x3B8 */ dCcD_Stts mStts;
    /* 0x3F4 */ dCcD_Sph mSph;
    /* 0x52C */ u8 field_0x52c[0x540 - 0x52c];
    /* 0x540 */ cXyz mPositions[21];
    /* 0x63C */ f32 field_0x63c;
    /* 0x640 */ cXyz mNormals[21];
    /* 0x73C */ f32 field_0x73c;
    /* 0x740 */ cXyz mNormalBacks[21];
    /* 0x740 */ cXyz mVecs[21];
    /* 0x938 */ cXyz* mpFlagPosition;
    /* 0x93C */ Mtx mModelMtx;
    /* 0x96C */ void* mpTexCoord;
    /* 0x970 */ f32 mSpringRate;
    /* 0x974 */ f32 mWindRate;
    /* 0x978 */ f32 mDecayRate;
    /* 0x97C */ f32 mGravity;
    /* 0x980 */ f32 mTornado;
};

/**
 * @ingroup actors-objects
 * @class daObjFlag2_c
 * @brief Flag 2
 *
 * @details
 *
 */
class daObjFlag2_c : public fopAc_ac_c {
public:
    void create_init();
    void initBaseMtx();
    void initCollision();
    void setCollision();
    int createHeap();
    int create();
    int draw();
    ~daObjFlag2_c();
    inline int execute();

    const daObjFlag2_Attr_c& attr() const { return M_attr; }

    static daObjFlag2_Attr_c const M_attr;

private:
    /* 0x0568 */ J3DModel* mModel;
    /* 0x056C */ request_of_phase_process_class mFlagPhase;
    /* 0x0574 */ request_of_phase_process_class mArcPhase;
    /* 0x057C */ dCcD_Stts mStts;
    /* 0x05B8 */ dCcD_Cyl mCyl;
    /* 0x06F4 */ u8 field_0x6f4[0x700 - 0x6f4];
    /* 0x0700 */ FlagCloth_c mFlagCloth;
    /* 0x1084 */ u8 field_0x1084[0x10a0 - 0x1084];
    /* 0x10A0 */ cXyz mFlagPosition;
    /* 0x10AC */ bool mFlagValid;
    /* 0x10AD */ char mFlagName[16];
};

STATIC_ASSERT(sizeof(daObjFlag2_c) == 0x10c0);

#endif /* D_A_OBJ_FLAG2_H */
