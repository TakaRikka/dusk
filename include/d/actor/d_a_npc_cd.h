#ifndef A_NPC_D_A_NPC_CD_H
#define A_NPC_D_A_NPC_CD_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/actor/d_a_npc.h"

class daNpcCd_c : public fopAc_ac_c {
public:
    int NpcCreate(int);
    J3DModel* ObjCreate(int);
    BOOL isM_();
    J3DAnmTransform* getAnmP(int, int);
    int setAttention(int);
    int loadResrc(int, int);
    int removeResrc(int, int);
    void setEnvTevCol();
    void setRoomNo();
    void animation();
    void setAnm(J3DAnmTransformKey*, f32, f32, int, int, int);
    int drawObj(int, J3DModel*, f32);
    int drawNpc();
    int jntNodeCB(J3DJoint*, J3DModel*);
    void setHitodamaParticle();
    J3DModelData* getNpcMdlDataP(int);
    J3DModelData* getObjMdlDataP(int);
    virtual ~daNpcCd_c() {}

    static dCcD_SrcCyl const m_cylDat;

    /* 0x56C */ request_of_phase_process_class mPhase1;
    /* 0x574 */ request_of_phase_process_class mPhase2;
    /* 0x57C */ request_of_phase_process_class mPhase3;
    /* 0x584 */ request_of_phase_process_class mPhase4;
    /* 0x58C */ mDoExt_McaMorfSO* mpMorf;
    /* 0x590 */ Z2CreatureCitizen mCreature;
    /* 0x634 */ dBgS_ObjAcch mAcch;
    /* 0x80C */ dBgS_AcchCir mAcchCir;
    /* 0x84C */ dCcD_Stts mStts;
    /* 0x888 */ dCcD_Cyl mCyl;
    /* 0x9C4 */ int m_type;
    /* 0x9C8 */ int field_0x9c8;
    /* 0x9CC */ int mHitodamaEmitters[2];
    /* 0x9D4 */ int field_0x9d4;
    /* 0x9D8 */ cXyz field_0x9d8;
    /* 0x9E4 */ s16 field_0x9e4;
    /* 0x9E6 */ s16 field_0x9e6;
    /* 0x9E8 */ u8 mIsDarkWorld;
};

STATIC_ASSERT(sizeof(daNpcCd_c) == 0x9EC);


struct daNpcCd_HIO_Jnt_c : public JORReflexible {
    virtual ~daNpcCd_HIO_Jnt_c() {}
    daNpcCd_HIO_Jnt_c() {}

#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 0x04 */ Vec jntT;
    /* 0x10 */ SVec jntR;
};

struct daNpcCd_HIO_Child_c : public JORReflexible {
    virtual ~daNpcCd_HIO_Child_c() {}
    daNpcCd_HIO_Child_c() {}

#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 0x004 */ daNpcCd_HIO_Jnt_c field_0x4[12];
    /* 0x124 */ s16 field_0x124[0x0C];
    /* 0x13C */ f32 mObjScale;
    /* 0x140 */ s16 mAtn;
    /* 0x144 */ f32 mAnmPlaySpeed;
    /* 0x148 */ f32 mWalkAnmPlaySpeed;
    /* 0x14C */ f32 mMaxSpeed;
    /* 0x150 */ f32 mWalkMaxSpeed;
    /* 0x154 */ f32 mCylH;
    /* 0x158 */ f32 mCylR;
    /* 0x15C */ f32 mChkWallH;
    /* 0x160 */ f32 mChkWallR;
};

STATIC_ASSERT(sizeof(daNpcCd_HIO_Child_c) == 0x164);

class daNpcCd_HIO_c : public fOpAcm_HIO_entry_c {
public:
    daNpcCd_HIO_c();
    virtual ~daNpcCd_HIO_c() {}

#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 0x0004 */ daNpcCd_HIO_Child_c field_0x0004[16];
    /* 0x1648 */ daNpcCd_HIO_Child_c field_0x1648[14];
};

STATIC_ASSERT(sizeof(daNpcCd_HIO_c) == 0x29BC);

extern daNpcCd_HIO_c l_Cd_HIO;
inline f32 HIO_atnOfs(int param_1) {
    s16 rv;
    if (param_1 < 16) {
        rv = l_Cd_HIO.field_0x0004[param_1].mAtn;
    } else {
        rv = l_Cd_HIO.field_0x1648[param_1 - 16].mAtn;
    }
    return rv;
}

inline s16 HIO_jntRX(int param_1, int param_2) {
    s16 rv;
    if (param_1 < 16) {
        rv = l_Cd_HIO.field_0x0004[param_1].field_0x4[param_2].jntR.x;
    } else {
        rv = l_Cd_HIO.field_0x1648[param_1 - 16].field_0x4[param_2].jntR.x;
    }
    return 182.04444885253906f * rv;
}

inline s16 HIO_jntRY(int param_1, int param_2) {
    s16 rv;
    if (param_1 < 16) {
        rv = l_Cd_HIO.field_0x0004[param_1].field_0x4[param_2].jntR.y;
    } else {
        rv = l_Cd_HIO.field_0x1648[param_1 - 16].field_0x4[param_2].jntR.y;
    }
    return 182.04444885253906f * rv;
}

inline s16 HIO_jntRZ(int param_1, int param_2) {
    s16 rv;
    if (param_1 < 16) {
        rv = l_Cd_HIO.field_0x0004[param_1].field_0x4[param_2].jntR.z;
    } else {
        rv = l_Cd_HIO.field_0x1648[param_1 - 16].field_0x4[param_2].jntR.z;
    }
    return 182.04444885253906f * rv;
}

static inline f32 HIO_jntTX(int param_1, int param_2) {
    f32 rv;
    if (param_1 < 16) {
        rv = l_Cd_HIO.field_0x0004[param_1].field_0x4[param_2].jntT.x;
    } else {
        rv = l_Cd_HIO.field_0x1648[param_1 - 16].field_0x4[param_2].jntT.x;
    }
    return 10.0f * rv;
}

static inline f32 HIO_jntTY(int param_1, int param_2) {
    f32 rv;
    if (param_1 < 16) {
        rv = l_Cd_HIO.field_0x0004[param_1].field_0x4[param_2].jntT.y;
    } else {
        rv = l_Cd_HIO.field_0x1648[param_1 - 16].field_0x4[param_2].jntT.y;
    }
    return 10.0f * rv;
}

static inline f32 HIO_jntTZ(int param_1, int param_2) {
    f32 rv;
    if (param_1 < 16) {
        rv = l_Cd_HIO.field_0x0004[param_1].field_0x4[param_2].jntT.z;
    } else {
        rv = l_Cd_HIO.field_0x1648[param_1 - 16].field_0x4[param_2].jntT.z;
    }
    return 10.0f * rv;
}

static inline f32 HIO_maxSpeed(int param_1) { return (f32)(param_1 < 16 ? l_Cd_HIO.field_0x0004[param_1].mMaxSpeed : l_Cd_HIO.field_0x0004[param_1].mMaxSpeed); }

static inline f32 HIO_anmPlaySpeed(int param_1) { return (f32)(param_1 < 16 ? l_Cd_HIO.field_0x0004[param_1].mAnmPlaySpeed : l_Cd_HIO.field_0x0004[param_1].mAnmPlaySpeed); }

static inline f32 HIO_walkMaxSpeed(int param_1) { return (f32)(param_1 < 16 ? l_Cd_HIO.field_0x0004[param_1].mWalkMaxSpeed : l_Cd_HIO.field_0x0004[param_1].mWalkMaxSpeed); }

static inline f32 HIO_walkAnmPlaySpeed(int param_1) { return (f32)(param_1 < 16 ? l_Cd_HIO.field_0x0004[param_1].mWalkAnmPlaySpeed : l_Cd_HIO.field_0x0004[param_1].mWalkAnmPlaySpeed); }

static inline f32 HIO_chkWallR(int param_1) { return (param_1 < 16 ? l_Cd_HIO.field_0x0004[param_1].mChkWallR : l_Cd_HIO.field_0x0004[param_1].mChkWallR); }

static inline f32 HIO_chkWallH(int param_1) { return (param_1 < 16 ? l_Cd_HIO.field_0x0004[param_1].mChkWallH : l_Cd_HIO.field_0x0004[param_1].mChkWallH); }

static inline f32 HIO_cylR(int param_1) { return (param_1 < 16 ? l_Cd_HIO.field_0x0004[param_1].mCylR : l_Cd_HIO.field_0x0004[param_1].mCylR); }

static inline f32 HIO_cylH(int param_1) { return (param_1 < 16 ? l_Cd_HIO.field_0x0004[param_1].mCylH : l_Cd_HIO.field_0x0004[param_1].mCylH); }

static inline f32 HIO_objScale(int param_1) { return (f32)(param_1 < 16 ? l_Cd_HIO.field_0x0004[param_1].mObjScale : l_Cd_HIO.field_0x0004[param_1].mObjScale); }

#endif /* A_NPC_D_A_NPC_CD_H */
