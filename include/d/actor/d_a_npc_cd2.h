#ifndef A_NPC_D_A_NPC_CD2_H
#define A_NPC_D_A_NPC_CD2_H

#include "Z2AudioLib/Z2Creature.h"
#include "d/actor/d_a_tag_escape.h"
#include "d/d_npc_lib.h"
#include "d/d_path.h"
#include "dolphin/types.h"

enum {
    MdlMANa_e = 0,
    MdlMADa_e = 1,
    JntM_NUM_e = 0x13,
    JntW_NUM_e = 0x15,
};


enum AnmNum {
    CD2_ANM_0 = 0,
    CD2_ANM_14 = 14,
};

struct daNpcCd2_HIO_Jnt_c : public JORReflexible {
    virtual ~daNpcCd2_HIO_Jnt_c() {}
    daNpcCd2_HIO_Jnt_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ f32 field_0x04[3];
    /* 0x10 */ s16 field_0x10[3];
};

struct daNpcCd2_HIO_MChild_c {
    virtual ~daNpcCd2_HIO_MChild_c() {}
    daNpcCd2_HIO_MChild_c() {}

    /* 0x004 */ daNpcCd2_HIO_Jnt_c field_0x004[19];
    /* 0x1CE */ s16 mMaxLmtX_BACKBONE;
    /* 0x1CA */ s16 mMaxLmtY_BACKBONE;
    /* 0x1CC */ s16 mMinLmtX_BACKBONE;
    /* 0x1CE */ s16 mMinLmtY_BACKBONE;
    /* 0x1D0 */ s16 mMaxLmtX_NECK;
    /* 0x1D2 */ s16 mMaxLmtY_NECK;
    /* 0x1D4 */ s16 mMinLmtX_NECK;
    /* 0x1D6 */ s16 mMinLmtY_NECK;
    /* 0x1D0 */ s16 mMaxLmtX_HEAD;
    /* 0x1D2 */ s16 mMaxLmtY_HEAD;
    /* 0x1D4 */ s16 mMinLmtX_HEAD;
    /* 0x1D6 */ s16 mMinLmtY_HEAD;
    /* 0x1E0 */ f32 mScale;
    /* 0x1E4 */ s16 mAttn;
    /* 0x1E8 */ f32 mAnmPlaySpeed;
    /* 0x1EC */ f32 mWalkAnmPlaySpeed;
    /* 0x1F0 */ f32 mMaxSpeed;
    /* 0x1F4 */ f32 mWalkMaxSpeed;
    /* 0x1F8 */ f32 mCylH;
    /* 0x1FC */ f32 mCylR;
    /* 0x200 */ f32 mChkWallH;
    /* 0x204 */ f32 mChkWallR;
};

struct daNpcCd2_HIO_WChild_c {
    virtual ~daNpcCd2_HIO_WChild_c() {}
    daNpcCd2_HIO_WChild_c() {}

    /* 0x004 */ daNpcCd2_HIO_Jnt_c field_0x004[21];
    /* 0x1FE */ s16 mMaxLmtX_BACKBONE;
    /* 0x1FA */ s16 mMaxLmtY_BACKBONE;
    /* 0x1FC */ s16 mMinLmtX_BACKBONE;
    /* 0x1FE */ s16 mMinLmtY_BACKBONE;
    /* 0x200 */ s16 mMaxLmtX_NECK;
    /* 0x202 */ s16 mMaxLmtY_NECK;
    /* 0x204 */ s16 mMinLmtX_NECK;
    /* 0x206 */ s16 mMinLmtY_NECK;
    /* 0x200 */ s16 mMaxLmtX_HEAD;
    /* 0x202 */ s16 mMaxLmtY_HEAD;
    /* 0x204 */ s16 mMinLmtX_HEAD;
    /* 0x206 */ s16 mMinLmtY_HEAD;
    /* 0x218 */ f32 mScale;
    /* 0x21C */ s16 mAttn;
    /* 0x220 */ f32 mAnmPlaySpeed;
    /* 0x224 */ f32 mWalkAnmPlaySpeed;
    /* 0x228 */ f32 mMaxSpeed;
    /* 0x22C */ f32 mWalkMaxSpeed;
    /* 0x230 */ f32 mCylH;
    /* 0x234 */ f32 mCylR;
    /* 0x238 */ f32 mChkWallH;
    /* 0x23C */ f32 mChkWallR;
};

struct daNpcCd2_HIO_c : public fOpAcm_HIO_entry_c {
    daNpcCd2_HIO_c();
    virtual ~daNpcCd2_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x0004 */ daNpcCd2_HIO_MChild_c field_0x0004[16];
    /* 0x20C4 */ daNpcCd2_HIO_WChild_c field_0x20c4[14];
};

extern daNpcCd2_HIO_c l_Cd2_HIO;

inline s16 Cd2_HIO_atnOfs(int param_1) {
    s16 rv;
    if (param_1 < 16) {
        rv = l_Cd2_HIO.field_0x0004[param_1].mAttn;
    } else {
        rv = l_Cd2_HIO.field_0x20c4[param_1 - 16].mAttn;
    }
    return rv;
}

inline f32 Cd2_HIO_objScale(int param_1) {
    return (f32)(param_1 < 16 ? l_Cd2_HIO.field_0x0004[param_1].mScale : l_Cd2_HIO.field_0x20c4[param_1 - 16].mScale);

}

inline f32 Cd2_HIO_walkMaxSpeed(int param_1) {
    return (f32)(param_1 < 16 ? l_Cd2_HIO.field_0x0004[param_1].mWalkMaxSpeed : l_Cd2_HIO.field_0x20c4[param_1 - 16].mWalkMaxSpeed);
}

inline f32 Cd2_HIO_walkAnmPlaySpeed(int param_1) {
    return (f32)(param_1 < 16 ? l_Cd2_HIO.field_0x0004[param_1].mWalkAnmPlaySpeed : l_Cd2_HIO.field_0x20c4[param_1 - 16].mWalkAnmPlaySpeed);
}

inline f32 Cd2_HIO_maxSpeed(int param_1) {
    return (f32)(param_1 < 16 ? l_Cd2_HIO.field_0x0004[param_1].mMaxSpeed : l_Cd2_HIO.field_0x20c4[param_1 - 16].mMaxSpeed);
}

inline f32 Cd2_HIO_anmPlaySpeed(int param_1) {
    return (f32)(param_1 < 16 ? l_Cd2_HIO.field_0x0004[param_1].mAnmPlaySpeed : l_Cd2_HIO.field_0x20c4[param_1 - 16].mAnmPlaySpeed);
}

inline f32 Cd2_HIO_chkWallH(int param_1) {
    return param_1 < 16 ? l_Cd2_HIO.field_0x0004[param_1].mChkWallH : l_Cd2_HIO.field_0x20c4[param_1 - 16].mChkWallH;
}

inline f32 Cd2_HIO_chkWallR(int param_1) {
    return param_1 < 16 ? l_Cd2_HIO.field_0x0004[param_1].mChkWallR : l_Cd2_HIO.field_0x20c4[param_1 - 16].mChkWallR;
}

inline f32 Cd2_HIO_cylR(int param_1) {
    return param_1 < 16 ? l_Cd2_HIO.field_0x0004[param_1].mCylR : l_Cd2_HIO.field_0x20c4[param_1 - 16].mCylR;
}

inline f32 Cd2_HIO_cylH(int param_1) {
    return param_1 < 16 ? l_Cd2_HIO.field_0x0004[param_1].mCylH : l_Cd2_HIO.field_0x20c4[param_1 - 16].mCylH;
}

inline s16 Cd2_HIO_maxLmtX_BACKBONE(int param_1) {
    return (s16)(param_1 < 16 ?
        l_Cd2_HIO.field_0x0004[param_1].mMaxLmtX_BACKBONE :
        l_Cd2_HIO.field_0x20c4[param_1 - 16].mMaxLmtX_BACKBONE);
}

inline s16 Cd2_HIO_maxLmtY_BACKBONE(int param_1) {
    return (s16)(param_1 < 16 ?
        l_Cd2_HIO.field_0x0004[param_1].mMaxLmtY_BACKBONE :
        l_Cd2_HIO.field_0x20c4[param_1 - 16].mMaxLmtY_BACKBONE);
}

inline s16 Cd2_HIO_maxLmtX_NECK(int param_1) {
    return (s16)(param_1 < 16 ?
        l_Cd2_HIO.field_0x0004[param_1].mMaxLmtX_NECK :
        l_Cd2_HIO.field_0x20c4[param_1 - 16].mMaxLmtX_NECK);
}

inline s16 Cd2_HIO_maxLmtY_NECK(int param_1) {
    return (s16)(param_1 < 16 ?
        l_Cd2_HIO.field_0x0004[param_1].mMaxLmtY_NECK :
        l_Cd2_HIO.field_0x20c4[param_1 - 16].mMaxLmtY_NECK);
}

inline s16 Cd2_HIO_maxLmtX_HEAD(int param_1) {
    return (s16)(param_1 < 16 ?
        l_Cd2_HIO.field_0x0004[param_1].mMaxLmtX_HEAD :
        l_Cd2_HIO.field_0x20c4[param_1 - 16].mMaxLmtX_HEAD);
}

inline s16 Cd2_HIO_maxLmtY_HEAD(int param_1) {
    return (s16)(param_1 < 16 ?
        l_Cd2_HIO.field_0x0004[param_1].mMaxLmtY_HEAD :
        l_Cd2_HIO.field_0x20c4[param_1 - 16].mMaxLmtY_HEAD);
}

inline s16 Cd2_HIO_minLmtX_BACKBONE(int param_1) {
    return (s16)(param_1 < 16 ?
        l_Cd2_HIO.field_0x0004[param_1].mMinLmtX_BACKBONE :
        l_Cd2_HIO.field_0x20c4[param_1 - 16].mMinLmtX_BACKBONE);
}

inline s16 Cd2_HIO_minLmtY_BACKBONE(int param_1) {
    return (s16)(param_1 < 16 ?
        l_Cd2_HIO.field_0x0004[param_1].mMinLmtY_BACKBONE :
        l_Cd2_HIO.field_0x20c4[param_1 - 16].mMinLmtY_BACKBONE);
}

inline s16 Cd2_HIO_minLmtX_NECK(int param_1) {
    return (s16)(param_1 < 16 ?
        l_Cd2_HIO.field_0x0004[param_1].mMinLmtX_NECK :
        l_Cd2_HIO.field_0x20c4[param_1 - 16].mMinLmtX_NECK);
}

inline s16 Cd2_HIO_minLmtY_NECK(int param_1) {
    return (s16)(param_1 < 16 ?
        l_Cd2_HIO.field_0x0004[param_1].mMinLmtY_NECK :
        l_Cd2_HIO.field_0x20c4[param_1 - 16].mMinLmtY_NECK);
}

inline s16 Cd2_HIO_minLmtX_HEAD(int param_1) {
    return (s16)(param_1 < 16 ?
        l_Cd2_HIO.field_0x0004[param_1].mMinLmtX_HEAD :
        l_Cd2_HIO.field_0x20c4[param_1 - 16].mMinLmtX_HEAD);
}

inline s16 Cd2_HIO_minLmtY_HEAD(int param_1) {
    return (s16)(param_1 < 16 ?
        l_Cd2_HIO.field_0x0004[param_1].mMinLmtY_HEAD :
        l_Cd2_HIO.field_0x20c4[param_1 - 16].mMinLmtY_HEAD);
}

class PathTrace_c {
public:
    bool setPath(int, int, int, cXyz*, bool);
    bool checkPoint(cXyz, f32);
    bool checkPathEnd(cXyz, f32);
    void getTargetPoint(Vec*);
    void setAvoidPoint();
    void setNextPoint();
    void setNextPoint(cXyz&);
    int incIndex(int);
    int decIndex(int);
    fopAc_ac_c* checkObstacle(fopAc_ac_c*);
    void checkObstacleSub(fopAc_ac_c*);

    void getAvoidPoint(Vec* avoidPoint) { *avoidPoint = mAvoidPoint; }
    BOOL isClose() { return dPath_ChkClose(mPath); }
    bool isPath() { return mPath != NULL; }
    s8 getArg0() { return mPath->m_points[field_0x20].mArg0; }
    s8 getArg1() { return mPath->m_points[field_0x20].mArg1; }
    Vec getPoint(u16 index) { return mPath->m_points[(u16)index].m_position; }
    void setLastPointNum() { field_0x20 = mPath->m_num - 1; }
    u16 getLastPointNum() {
        return field_0x24 == 1 ? mPath->m_num - 1 : 0;
    }

    /* 0x00 */ fopAc_ac_c* mMyself;
    /* 0x04 */ fopAc_ac_c* mObstacle;
    /* 0x08 */ dPath* mPath;
    /* 0x0C */ Vec mAvoidPoint;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 mNextPoint;
    /* 0x20 */ int field_0x20;
    /* 0x24 */ s8 field_0x24;
};

class daNpcCd2_c : public fopAc_ac_c {
public:
    int NpcCreate(int);
    J3DModel* ObjCreate(int);
    J3DModel* ChairCreate(f32);
    bool isM_();
    J3DAnmTransform* getAnmP(int, int);
    int setAttention(int);
    int loadResrc(int, int);
    int removeResrc(int, int);
    void setEnvTevCol();
    void setRoomNo();
    void animation(int);
    void setAnm(J3DAnmTransformKey*, f32, f32, int, int, int);
    int drawShadow(f32);
    int drawObj(int, J3DModel*, f32);
    int drawNpc();
    int jntNodeCB(J3DJoint*, J3DModel*);
    void setHitodamaParticle();
    daTagEscape_c* getEscapeTag();
    bool checkFearSituation();
    J3DModelData* getNpcMdlDataP(int);
    J3DModelData* getObjMdlDataP(int);
    J3DAnmTexPattern* getTexAnmP(int);
    virtual ~daNpcCd2_c() {}

    static dCcD_SrcCyl const m_cylDat;

    /* 0x56C */ request_of_phase_process_class mPhase1;
    /* 0x574 */ request_of_phase_process_class mPhase2;
    /* 0x57C */ request_of_phase_process_class mPhase3;
    /* 0x584 */ request_of_phase_process_class mPhase4;
    /* 0x58C */ mDoExt_McaMorfSO* mpMorf;
    /* 0x590 */ mDoExt_btpAnm* mpBtpAnm;
    /* 0x594 */ Z2CreatureCitizen mCitizen;
    /* 0x638 */ dBgS_ObjAcch mAcch;
    /* 0x810 */ dBgS_AcchCir mAcchCir;
    /* 0x850 */ dCcD_Stts mStts;
    /* 0x88C */ dCcD_Cyl mCyl;
    /* 0x9C8 */ dNpcLib_lookat_c mLookat;
    /* 0xA98 */ int m_type;
    /* 0xA9C */ int field_0xa9c;
    /* 0xAA4 */ int field_0xaa0;
    /* 0xAA4 */ int field_0xaa4;
    /* 0xAA8 */ int mHitodamaEmitters[2];
    /* 0xAB0 */ daTagEscape_c* mEscapeTag;
    /* 0xAB4 */ cXyz field_0xab4;
    /* 0xAC0 */ s16 field_0xac0;
    /* 0xAC2 */ s16 field_0xac2;
    /* 0xAC4 */ s16 field_0xac4;
    /* 0xAC6 */ u8 mIsDarkWorld;
};

#endif /* A_NPC_D_A_NPC_CD2_H */
