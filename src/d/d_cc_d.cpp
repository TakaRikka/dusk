#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"
#include "f_pc/f_pc_searcher.h"

void dCcD_GAtTgCoCommonBase::ClrActorInfo() {
    mApid = -1;
    mAc = NULL;
}

void dCcD_GAtTgCoCommonBase::ct() {
    mGFlag = 0;
    mRPrm = 0;
    mHitCallback = NULL;
    mEffCounter = 0;
}

void dCcD_GAtTgCoCommonBase::SetHitApid(fpc_ProcID hitApid) {
    mApid = hitApid;
    mAc = NULL;
}

fopAc_ac_c* dCcD_GAtTgCoCommonBase::GetAc() {
    if (mApid == -1) {
        return NULL;
    }
    if (mAc == NULL) {
        mAc = fopAcM_SearchByID(mApid);
        if (mAc == NULL) {
            mApid = -1;
        }
    }
    return mAc;
}

void dCcD_GAtTgCoCommonBase::Set(dCcD_SrcGAtTgCoCommonBase const& src) {
    mGFlag = src.mGFlag;
    mEffCounter = 0;
}

void dCcD_GAtTgCoCommonBase::SetEffCounterTimer() {
    mEffCounter = 5;
}

void dCcD_GAtTgCoCommonBase::SubtractEffCounter() {
    mEffCounter--;
    if (mEffCounter < 0) {
        mEffCounter = 0;
    }
}

bool dCcD_GAtTgCoCommonBase::ChkEffCounter() {
    return mEffCounter > 0;
}

dCcD_GStts::dCcD_GStts() {
    mAt = 0;
    mTg = 0;
    mRoomId = 0;
    field_0x08 = -1;
    mAtApid = -1;
    mAtOldApid = -1;
    mTgApid = -1;
    mTgOldApid = -1;
    field_0x1C = 0;
}

void dCcD_GStts::Ct() {
    mAt = 0;
    mTg = 0;
    mRoomId = 0;
    field_0x08 = -1;
    mAtApid = -1;
    mAtOldApid = -1;
    mTgApid = -1;
    mTgOldApid = -1;
    field_0x1C = 0;
}

void dCcD_GStts::Move() {
    mAtOldApid = mAtApid;
    mAtApid = 0;
    mTgOldApid = mTgApid;
    mTgApid = 0;
}

cCcD_GStts* dCcD_Stts::GetGStts() {
    return this;
}

void dCcD_Stts::Init(int param_0, int param_1, fopAc_ac_c* pActor) {
    u32 actorPid;
    if (pActor != NULL) {
        actorPid = fopAcM_GetID(pActor);
    } else {
        actorPid = -1;
    }

    cCcD_Stts::Init(param_0, param_1, pActor, actorPid);

    s32 roomNo;
    if (pActor != NULL) {
        roomNo = fopAcM_GetRoomNo(pActor);
    } else {
        roomNo = 0;
        OnNoActor();
    }

    if (roomNo != -1) {
        SetRoomId(roomNo);
    } else {
        SetRoomId(0);
    }
}

void dCcD_Stts::Ct() {
    cCcD_Stts::Ct();
    dCcD_GStts::Ct();
}

void dCcD_Stts::ClrAt() {
    cCcD_Stts::ClrAt();
    dCcD_GStts::ClrAt();
}

void dCcD_Stts::ClrTg() {
    cCcD_Stts::ClrTg();
    dCcD_GStts::ClrTg();
}

void dCcD_GObjAt::Set(dCcD_SrcGObjAt const& src) {
    dCcD_GAtTgCoCommonBase::Set(src.mBase);
    mSe = src.mSe;
    mMtrl = src.mMtrl;
    mHitMark = src.mHitMark;
    mSpl = src.mSpl;
}

void dCcD_GObjTg::Set(dCcD_SrcGObjTg const& src) {
    dCcD_GAtTgCoCommonBase::Set(src.mBase);
    mSe = src.mSe;
    mMtrl = src.mMtrl;
    mHitMark = src.mHitMark;
    mSpl = src.mSpl;
    mHitPos = cXyz::Zero;
    mShieldFrontRangeYAngle = NULL;
    mShieldRange = 0x4000;
}

dCcD_GObjInf::dCcD_GObjInf() {}

dCcD_GObjInf::~dCcD_GObjInf() {}

cCcD_GObjInf* dCcD_GObjInf::GetGObjInf() {
    return this;
}

void dCcD_GObjInf::ClrAtHit() {
    cCcD_ObjHitInf::ClrAtHit();
    mGObjAt.ClrHit();
    OffAtShieldHit();
    OffAtHitNoActor();
    SubtractAtEffCounter();
}

u32 dCcD_GObjInf::ChkAtHit() {
    if (cCcD_ObjHitInf::ChkAtHit() == 0) {
        return 0;
    } else if (ChkAtHitNoActor() == 0 && GetAtHitAc() == NULL) {
        return 0;
    } else {
        return 1;
    }
}

void dCcD_GObjInf::ResetAtHit() {
    ClrAtHit();
    ClrAtEffCounter();
}

cCcD_Obj* dCcD_GObjInf::GetAtHitObj() {
    if (ChkAtHitNoActor() == 0 && GetAtHitAc() == NULL) {
        return NULL;
    } else {
        return cCcD_ObjHitInf::GetAtHitObj();
    }
}

cCcD_GObjInf* dCcD_GObjInf::GetAtHitGObj() {
    cCcD_Obj* obj = GetAtHitObj();
    if (obj == NULL) {
        return NULL;
    } else {
        return dCcD_GetGObjInf(obj);
    }
}

u8 dCcD_GObjInf::ChkAtNoGuard() {
    return GetAtSpl() >= 12;
}

void dCcD_GObjInf::ClrTgHit() {
    cCcD_ObjHitInf::ClrTgHit();
    mGObjTg.ClrHit();
    OffTgHitNoActor();
    OffTgShieldHit();
    OffTgMagneHit();
    SubtractTgEffCounter();
}

u32 dCcD_GObjInf::ChkTgHit() {
    if (cCcD_ObjHitInf::ChkTgHit() == 0) {
        return false;
    } else if (ChkTgHitNoActor() == 0 && GetTgHitAc() == NULL) {
        return false;
    } else {
        return true;
    }
}

void dCcD_GObjInf::ResetTgHit() {
    ClrTgHit();
    ClrTgEffCounter();
}

cCcD_Obj* dCcD_GObjInf::GetTgHitObj() {
    if (ChkTgHitNoActor() == 0 && GetTgHitAc() == NULL) {
        return NULL;
    } else {
        return cCcD_ObjHitInf::GetTgHitObj();
    }
}

dCcD_GObjInf* dCcD_GObjInf::GetTgHitGObj() {
    cCcD_Obj* obj = GetTgHitObj();
    if (obj == NULL) {
        return NULL;
    } else {
        return dCcD_GetGObjInf(obj);
    }
}

u8 dCcD_GObjInf::GetTgHitObjSe() {
    dCcD_GObjInf* objInf = GetTgHitGObj();
    if (objInf == NULL) {
        return NULL;
    } else {
        return objInf->GetAtSe();
    }
}

Z2SoundID const dCcD_GObjInf::m_hitSeID[24] = {
    Z2SE_HIT_DUMMY_CANCEL,    Z2SE_HIT_SWORD,          Z2SE_HIT_SHIELD_ATTACK,
    Z2SE_HIT_WOLFKICK,        Z2SE_HIT_WOLFBITE,       Z2SE_HIT_WOOD_WEAPON,
    Z2SE_HIT_HAMMER,          Z2SE_HIT_ARROW_STICK,    Z2SE_HIT_HOOKSHOT_STICK,
    Z2SE_HIT_METAL_WEAPON,    Z2SE_HIT_STONE_WEAPON,   Z2SE_HIT_HARD_BODY,
    Z2SE_HIT_SOFT_BODY,       Z2SE_HIT_DUMMY_CANCEL,   Z2SE_HIT_SWORD_STAB,
    Z2SE_HIT_WOLFKICK,        Z2SE_HIT_SWORD_RUN,      Z2SE_HIT_EN_WOOD_WEAPON,
    Z2SE_HIT_THOROW_OBJ,      Z2SE_HIT_SPINNER,        Z2SE_HIT_COPYROD,
    Z2SE_HIT_STATUE_HAMMER,   Z2SE_HIT_PACHINKO,       Z2SE_HIT_AL_ARROW_STICK,
};

Z2SoundID dCcD_GObjInf::getHitSeID(u8 seIdx, int useRebound) {
    if (useRebound) {
        if (seIdx == 23) {
            return Z2SE_HIT_AL_ARROW_REBOUND;
        }
        if (seIdx == 7) {
            return Z2SE_HIT_ARROW_REBOUND;
        }
        if (seIdx == 8) {
            return Z2SE_HIT_HOOKSHOT_REBOUND;
        }
    }
    return m_hitSeID[seIdx];
}

void dCcD_GObjInf::ClrCoHit() {
    cCcD_ObjHitInf::ClrCoHit();
    mGObjCo.ClrHit();
    OffCoHitNoActor();
    SubtractCoEffCounter();
}

u32 dCcD_GObjInf::ChkCoHit() {
    if (cCcD_ObjHitInf::ChkCoHit() == 0) {
        return false;
    } else if (ChkCoHitNoActor() == 0 && GetCoHitAc() == NULL) {
        return false;
    } else {
        return true;
    }
}

void dCcD_GObjInf::ResetCoHit() {
    ClrCoHit();
    ClrCoEffCounter();
}

cCcD_Obj* dCcD_GObjInf::GetCoHitObj() {
    if (ChkCoHitNoActor() == 0 && GetCoHitAc() == NULL) {
        return NULL;
    } else {
        return cCcD_ObjHitInf::GetCoHitObj();
    }
}

void dCcD_GObjInf::Set(dCcD_SrcGObjInf const& src) {
    cCcD_Obj::Set(src.mObj);
    mGObjAt.Set(src.mGObjAt);
    mGObjTg.Set(src.mGObjTg);
    mGObjCo.Set(src.mGObjCo);
}

dCcD_GObjInf* dCcD_GetGObjInf(cCcD_Obj* p_obj) {
    return (dCcD_GObjInf*)p_obj->GetGObjInf();
}

cCcD_ShapeAttr* dCcD_Pnt::GetShapeAttr() {
    return this;
}

void dCcD_Cps::Set(dCcD_SrcCps const& src) {
    dCcD_GObjInf::Set(src.mObjInf);
    cCcD_CpsAttr::Set(src.mCpsAttr);
}

cCcD_ShapeAttr* dCcD_Cps::GetShapeAttr() {
    return this;
}

void dCcD_Cps::CalcAtVec() {
    cXyz* dest = GetAtVecP();
    CalcVec(dest);
}

void dCcD_Cps::CalcTgVec() {
    cXyz* dest = GetTgVecP();
    CalcVec(dest);
}

void dCcD_Tri::Set(dCcD_SrcTri const& src) {
    dCcD_GObjInf::Set(src.mObjInf);
}

cCcD_ShapeAttr* dCcD_Tri::GetShapeAttr() {
    return this;
}

void dCcD_Cyl::Set(dCcD_SrcCyl const& src) {
    dCcD_GObjInf::Set(src.mObjInf);
    cCcD_CylAttr::Set(src.mCyl);
}

cCcD_ShapeAttr* dCcD_Cyl::GetShapeAttr() {
    return this;
}

void dCcD_Cyl::StartCAt(cXyz& pos) {
    cXyz zero(cXyz::Zero);
    SetAtVec(zero);
    SetC(pos);
}

void dCcD_Cyl::MoveCAt(cXyz& pos) {
    const cXyz* center = GetCP();
    cXyz diff = pos - *center;
    SetAtVec(diff);
    SetC(pos);
}

void dCcD_Cyl::MoveCTg(cXyz& pos) {
    const cXyz* center = GetCP();
    cXyz diff = pos - *center;
    SetTgVec(diff);
    SetC(pos);
}

void dCcD_Sph::Set(dCcD_SrcSph const& src) {
    dCcD_GObjInf::Set(src.mObjInf);
    cCcD_SphAttr::Set(src.mSphAttr);
}

void dCcD_Sph::StartCAt(cXyz& pos) {
    cXyz zero(cXyz::Zero);
    SetAtVec(zero);
    cM3dGSph::SetC(pos);
}

void dCcD_Sph::MoveCAt(cXyz& pos) {
    const cXyz* center = GetCP();
    cXyz diff = pos - *center;
    SetAtVec(diff);
    SetC(pos);
}

cCcD_ShapeAttr* dCcD_Sph::GetShapeAttr() {
    return this;
}
