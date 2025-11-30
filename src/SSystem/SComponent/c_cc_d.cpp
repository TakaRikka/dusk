/**
 * c_cc_d.cpp
 *
 */

#include "SSystem/SComponent/c_cc_d.h"
#include "JSystem/JUtility/JUTAssert.h"

#define CHECK_FLOAT_RANGE(line, x) JUT_ASSERT(line, -1.0e32f < x && x < 1.0e32f);

cXyz cCcD_ShapeAttr::m_virtual_center = cXyz::Zero;

void cCcD_DivideInfo::Set(u32 xDivInfo, u32 yDivInfo, u32 zDivInfo) {
    mXDivInfo = xDivInfo;
    mYDivInfo = yDivInfo;
    mZDivInfo = zDivInfo;
}

bool cCcD_DivideInfo::Chk(const cCcD_DivideInfo& other) const {
    if ((mXDivInfo & other.mXDivInfo) == 0 || (mZDivInfo & other.mZDivInfo) == 0 ||
        (mYDivInfo & other.mYDivInfo) == 0)
    {
        return false;
    } else {
        return true;
    }
}

void cCcD_DivideArea::SetArea(const cM3dGAab& aab) {
    Set(aab.GetMinP(), aab.GetMaxP());

    mScaledXDiff = 1.0f / 32.0f * (GetMaxP()->x - GetMinP()->x);
    mXDiffIsZero = cM3d_IsZero(mScaledXDiff);
    if (!mXDiffIsZero) {
        mInvScaledXDiff = 1.0f / mScaledXDiff;
    }

    mScaledYDiff = 1.0f / 32.0f * (GetMaxP()->y - GetMinP()->y);
    mYDiffIsZero = cM3d_IsZero(mScaledYDiff);
    if (!mYDiffIsZero) {
        mInvScaledYDiff = 1.0f / mScaledYDiff;
    }

    mScaledZDiff = 1.0f / 32.0f * (GetMaxP()->z - GetMinP()->z);
    mZDiffIsZero = cM3d_IsZero(mScaledZDiff);
    if (!mZDiffIsZero) {
        mInvScaledZDiff = 1.0f / mScaledZDiff;
    }
}

static const u32 l_base[32] = {
    0x00000001, 0x00000003, 0x00000007, 0x0000000F, 0x0000001F, 0x0000003F, 0x0000007F, 0x000000FF,
    0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF, 0x00001FFF, 0x00003FFF, 0x00007FFF, 0x0000FFFF,
    0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000FFFFF, 0x001FFFFF, 0x003FFFFF, 0x007FFFFF, 0x00FFFFFF,
    0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0FFFFFFF, 0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF,
};

void cCcD_DivideArea::CalcDivideInfo(cCcD_DivideInfo* pDivideInfo, const cM3dGAab& aab,
                                     u32 param_2) {
    if (param_2 != 0) {
        pDivideInfo->Set(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
    } else {
        u32 xDivInfo, yDivInfo, zDivInfo;
        if (!mXDiffIsZero) {
            s32 var1 = mInvScaledXDiff * (aab.GetMinP()->x - GetMinP()->x);
            s32 var3 = mInvScaledXDiff * (aab.GetMaxP()->x - GetMinP()->x);
            if (31 < var3) {
                var3 = 31;
            }

            xDivInfo = l_base[var3];
            if (0 < var1) {
                var1--;
                xDivInfo &= ~l_base[var1];
            }
        } else {
            xDivInfo = 0xFFFFFFFF;
        }

        if (!mYDiffIsZero) {
            s32 var1 = mInvScaledYDiff * (aab.GetMinP()->y - GetMinP()->y);
            s32 var3 = mInvScaledYDiff * (aab.GetMaxP()->y - GetMinP()->y);
            if (31 < var3) {
                var3 = 31;
            }

            yDivInfo = l_base[var3];
            if (0 < var1) {
                var1--;
                yDivInfo &= ~l_base[var1];
            }
        } else {
            yDivInfo = 0xFFFFFFFF;
        }

        if (!mZDiffIsZero) {
            s32 var1 = mInvScaledZDiff * (aab.GetMinP()->z - GetMinP()->z);
            s32 var3 = mInvScaledZDiff * (aab.GetMaxP()->z - GetMinP()->z);
            if (31 < var3) {
                var3 = 31;
            }

            zDivInfo = l_base[var3];
            if (0 < var1) {
                var1--;
                zDivInfo &= ~l_base[var1];
            }
        } else {
            zDivInfo = 0xFFFFFFFF;
        }

        pDivideInfo->Set(xDivInfo, yDivInfo, zDivInfo);
    }
}

void cCcD_DivideArea::CalcDivideInfoOverArea(cCcD_DivideInfo* pDivideInfo, const cM3dGAab& aab) {
    u32 xDivInfo, yDivInfo, zDivInfo;
    if (!mXDiffIsZero) {
        s32 var1 = mInvScaledXDiff * (aab.GetMinP()->x - GetMinP()->x);
        s32 var3 = mInvScaledXDiff * (aab.GetMaxP()->x - GetMinP()->x);
        if (var3 < 0 || 31 < var1) {
            xDivInfo = 0;
        } else {
            if (31 < var3) {
                var3 = 31;
            }
            xDivInfo = l_base[var3];
            if (0 < var1) {
                var1--;
                xDivInfo &= ~l_base[var1];
            }
        }
    } else {
        xDivInfo = 0xFFFFFFFF;
    }

    if (!mYDiffIsZero) {
        s32 var1 = mInvScaledYDiff * (aab.GetMinP()->y - GetMinP()->y);
        s32 var3 = mInvScaledYDiff * (aab.GetMaxP()->y - GetMinP()->y);
        if (var3 < 0 || 31 < var1) {
            yDivInfo = 0;
        } else {
            if (31 < var3) {
                var3 = 31;
            }
            yDivInfo = l_base[var3];
            if (0 < var1) {
                var1--;
                yDivInfo &= ~l_base[var1];
            }
        }
    } else {
        yDivInfo = 0xFFFFFFFF;
    }

    if (!mZDiffIsZero) {
        s32 var1 = mInvScaledZDiff * (aab.GetMinP()->z - GetMinP()->z);
        s32 var3 = mInvScaledZDiff * (aab.GetMaxP()->z - GetMinP()->z);
        if (var3 < 0 || 31 < var1) {
            zDivInfo = 0;
        } else {
            if (31 < var3) {
                var3 = 31;
            }
            zDivInfo = l_base[var3];
            if (0 < var1) {
                var1--;
                zDivInfo &= ~l_base[var1];
            }
        }
    } else {
        zDivInfo = 0xFFFFFFFF;
    }

    pDivideInfo->Set(xDivInfo, yDivInfo, zDivInfo);
}

const cCcD_GStts* cCcD_Stts::GetGStts() const {
    return NULL;
}

cCcD_GStts* cCcD_Stts::GetGStts() {
    return NULL;
}

void cCcD_Stts::Init(int weight, int param_1, void* pactor, fpc_ProcID apid) {
    Ct();
    m_weight = weight;
    field_0x15 = param_1;
    mp_actor = static_cast<fopAc_ac_c*>(pactor);
    m_apid = apid;
}

void cCcD_Stts::Ct() {
    m_cc_move.x = 0.0f;
    m_cc_move.y = 0.0f;
    m_cc_move.z = 0.0f;
    mp_actor = NULL;
    m_apid = fpcM_ERROR_PROCESS_ID_e;
    m_weight = 0;
    field_0x15 = 0;
    m_dmg = 0;
}

void cCcD_Stts::PlusCcMove(f32 x, f32 y, f32 z) {
    m_cc_move.x += x;
    m_cc_move.y += y;
    m_cc_move.z += z;

    JUT_ASSERT(422, !isnan(m_cc_move.x));
    JUT_ASSERT(423, !isnan(m_cc_move.y));
    JUT_ASSERT(424, !isnan(m_cc_move.z));

    CHECK_FLOAT_RANGE(426, m_cc_move.x);
    CHECK_FLOAT_RANGE(427, m_cc_move.y);
    CHECK_FLOAT_RANGE(428, m_cc_move.z);
}

void cCcD_Stts::ClrCcMove() {
    m_cc_move.z = 0.0f;
    m_cc_move.y = 0.0f;
    m_cc_move.x = 0.0f;
}

void cCcD_Stts::PlusDmg(int dmg) {
    if (m_dmg >= dmg) {
        return;
    }
    m_dmg = dmg;
}

f32 cCcD_Stts::GetWeightF() const {
    return (s32)m_weight;
}

void cCcD_ObjCommonBase::ct() {
    mSPrm = 0;
    mRPrm = 0;
    mHitObj = NULL;
}

void cCcD_ObjHitInf::Set(const cCcD_SrcObjHitInf& src) {
    mObjAt.Set(src.mObjAt);
    mObjTg.Set(src.mObjTg);
    mObjCo.Set(src.mObjCo);
}

void cCcD_Obj::ct() {
    mFlags = 0;
}

void cCcD_Obj::Set(const cCcD_SrcObj& src) {
    mFlags = src.mFlags;
    cCcD_ObjHitInf::Set(src.mSrcObjHitInf);
}

fopAc_ac_c* cCcD_Obj::GetAc() {
    if (mStts == NULL) {
        return NULL;
    } else {
        return mStts->GetAc();
    }
}

void cCcD_ShapeAttr::getShapeAccess(cCcD_ShapeAttr::Shape* pshape) const {
    pshape->_0 = 2;
    pshape->_14 = 0.0f;
    pshape->_10 = 0.0f;
    pshape->_4.z = 0.0f;
    pshape->_4.y = 0.0f;
    pshape->_4.x = 0.0f;
}

bool cCcD_PntAttr::GetNVec(cXyz const& param_0, cXyz* param_1) const {
    param_1->x = param_0.x - vtx.x;
    param_1->y = param_0.y - vtx.y;
    param_1->z = param_0.z - vtx.z;
    if (cM3d_IsZero(PSVECMag(param_1))) {
        param_1->x = 0.0f;
        param_1->y = 0.0f;
        param_1->z = 0.0f;
        return false;
    } else {
        PSVECNormalize(param_1, param_1);
        return true;
    }
}


bool cCcD_TriAttr::CrossAtTg(const cCcD_CpsAttr& cpsAttr, cXyz* pxyz) const {
    if (cM3dGTri::Cross(cpsAttr, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_TriAttr::CrossAtTg(const cCcD_CylAttr& cylAttr, cXyz* pxyz) const {
    if (cM3dGTri::Cross(cylAttr, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_TriAttr::CrossAtTg(const cCcD_SphAttr& sphAttr, cXyz* pxyz) const {
    if (cM3dGTri::Cross(sphAttr, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_TriAttr::CrossAtTg(const cCcD_TriAttr& other, cXyz* pxyz) const {
    if (cM3dGTri::Cross(other, pxyz)) {
        return true;
    } else {
        return false;
    }
}

void cCcD_TriAttr::CalcAabBox() {
    mAab.ClearForMinMax();
    mAab.SetMinMax(mA);
    mAab.SetMinMax(mB);
    mAab.SetMinMax(mC);
}

bool cCcD_TriAttr::GetNVec(const cXyz& param_0, cXyz* pOut) const {
    if (getPlaneFunc(&param_0) >= 0.0f) {
        *pOut = mNormal;
    } else {
        *pOut = mNormal;
        VECScale(pOut, pOut, -1.0f);
    }
    return true;
}

bool cCcD_CpsAttr::CrossAtTg(const cCcD_CpsAttr& other, cXyz* pxyz) const {
    if (cM3dGCps::Cross(&other, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_CpsAttr::CrossAtTg(const cCcD_CylAttr& cylAttr, cXyz* pxyz) const {
    if (cM3dGCps::Cross(&cylAttr, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_CpsAttr::CrossAtTg(const cCcD_SphAttr& sphAttr, cXyz* pxyz) const {
    if (cM3dGCps::Cross(&sphAttr, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_CpsAttr::CrossAtTg(const cCcD_TriAttr& triAttr, cXyz* pxyz) const {
    if (triAttr.cM3dGTri::Cross(*this, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_CpsAttr::CrossCo(const cCcD_CpsAttr& other, f32* param_1) const {
    *param_1 = 0.0f;
    cXyz xyz;
    if (cM3dGCps::Cross(&other, &xyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_CpsAttr::CrossCo(const cCcD_CylAttr& cylAttr, f32* param_1) const {
    *param_1 = 0.0f;
    cXyz xyz;
    if (cM3dGCps::Cross(&cylAttr, &xyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_CpsAttr::CrossCo(const cCcD_SphAttr& sphAttr, f32* param_1) const {
    *param_1 = 0.0f;
    cXyz xyz;
    if (cM3dGCps::Cross(&sphAttr, &xyz)) {
        return true;
    } else {
        return false;
    }
}

void cCcD_CpsAttr::CalcAabBox() {
    mAab.ClearForMinMax();
    mAab.SetMinMax(mStart);
    mAab.SetMinMax(mEnd);
    mAab.PlusR(mRadius);
}

bool cCcD_CpsAttr::GetNVec(const cXyz& param_0, cXyz* param_1) const {
    Vec diff;
    const cXyz& endP = GetEndP();
    VECSubtract(&endP, &mStart, &diff);

    f32 diffLen = VECDotProduct(&diff, &diff);
    if (cM3d_IsZero(diffLen)) {
        return false;
    } else {
        Vec vec1, vec2;
        VECSubtract(&param_0, &mStart, &vec1);
        f32 vec1Len = VECDotProduct(&vec1, &diff) / diffLen;
        if (vec1Len < 0.0f) {
            vec2 = mStart;
        } else {
            if (vec1Len > 1.0f) {
                vec2 = endP;
            } else {
                VECScale(&diff, &diff, vec1Len);
                VECAdd(&diff, &mStart, &vec2);
            }
        }

        VECSubtract(&param_0, &vec2, param_1);
        if (cM3d_IsZero(VECMag(param_1))) {
            param_1->set(0.0f, 0.0f, 0.0f);
            return false;
        } else {
            VECNormalize(param_1, param_1);
            return true;
        }
    }
}

bool cCcD_CylAttr::CrossAtTg(const cCcD_CpsAttr& cpsAttr, cXyz* pxyz) const {
    if (cM3dGCyl::Cross(&cpsAttr, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_CylAttr::CrossAtTg(const cCcD_CylAttr& other, cXyz* pxyz) const {
    if (cross(&other, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_CylAttr::CrossAtTg(const cCcD_SphAttr& sphAttr, cXyz* pxyz) const {
    if (cross(&sphAttr, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_CylAttr::CrossAtTg(const cCcD_TriAttr& triAttr, cXyz* pxyz) const {
    if (cM3dGCyl::Cross(triAttr, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_CylAttr::CrossCo(const cCcD_CylAttr& other, f32* f) const {
    if (cM3dGCyl::Cross(&other, f)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_CylAttr::CrossCo(const cCcD_SphAttr& sphAttr, f32* f) const {
    if (cM3dGCyl::Cross(&sphAttr, f)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_CylAttr::CrossCo(const cCcD_CpsAttr& cpsAttr, f32* f) const {
    *f = 0.0f;
    cXyz xyz;
    if (cM3dGCyl::Cross(&cpsAttr, &xyz)) {
        return true;
    } else {
        return false;
    }
}

void cCcD_CylAttr::CalcAabBox() {
    cXyz min;
    cXyz max;
    min.x = GetCP()->x - GetR();
    min.y = GetCP()->y;
    min.z = GetCP()->z - GetR();
    max.x = GetCP()->x + GetR();
    max.y = GetCP()->y + GetH();
    max.z = GetCP()->z + GetR();
    mAab.Set(&min, &max);
}

bool cCcD_CylAttr::GetNVec(const cXyz& param_0, cXyz* param_1) const {
    Vec vec;
    if (GetCP()->y > param_0.y) {
        vec = mCenter;
    } else {
        if (GetCP()->y + GetH() < param_0.y) {
            vec.x = GetCP()->x;
            vec.y = GetCP()->y;
            vec.z = GetCP()->z;
            vec.y = GetCP()->y + GetH();
        } else {
            vec = mCenter;
            vec.y = param_0.y;
        }
    }

    VECSubtract(&param_0, &vec, param_1);
    if (cM3d_IsZero(VECMag(param_1))) {
        param_1->set(0.0f, 0.0f, 0.0f);
        return false;
    } else {
        VECNormalize(param_1, param_1);
        return true;
    }
    return false;
}

void cCcD_CylAttr::getShapeAccess(cCcD_ShapeAttr::Shape* pshape) const {
    pshape->_0 = 1;
    pshape->_4.x = mCenter.x;
    pshape->_4.y = mCenter.y;
    pshape->_4.z = mCenter.z;
    pshape->_10 = mRadius;
    pshape->_14 = mHeight;
}

inline bool inlineCross(const cM3dGSph& sph, const cM3dGCps* pcps, cXyz* pxyz) {
    return cM3d_Cross_CpsSph(*pcps, sph, pxyz);
}

bool cCcD_SphAttr::CrossAtTg(const cCcD_CpsAttr& cpsAttr, cXyz* pxyz) const {
    if (inlineCross(*this, &cpsAttr, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_SphAttr::CrossAtTg(const cCcD_CylAttr& cylAttr, cXyz* pxyz) const {
    if (cross(&cylAttr, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_SphAttr::CrossAtTg(const cCcD_SphAttr& sphAttr, cXyz* pxyz) const {
    if (cross(&sphAttr, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_SphAttr::CrossAtTg(const cCcD_TriAttr& triAttr, cXyz* pxyz) const {
    if (triAttr.cM3dGTri::Cross(*this, pxyz)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_SphAttr::CrossCo(const cCcD_CylAttr& cylAttr, f32* f) const {
    if (cM3dGSph::Cross(&cylAttr, f)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_SphAttr::CrossCo(const cCcD_SphAttr& sphAttr, f32* f) const {
    if (cM3dGSph::Cross(&sphAttr, f)) {
        return true;
    } else {
        return false;
    }
}

bool cCcD_SphAttr::CrossCo(const cCcD_CpsAttr& cpsAttr, f32* f) const {
    *f = 0.0f;
    cXyz xyz;
    if (cM3dGSph::Cross(&cpsAttr, &xyz)) {
        return true;
    } else {
        return false;
    }
}

void cCcD_SphAttr::CalcAabBox() {
    cXyz min;
    cXyz max;

    min = max = *GetCP();

    min.x -= GetR();
    min.y -= GetR();
    min.z -= GetR();

    max.x += GetR();
    max.y += GetR();
    max.z += GetR();

    mAab.Set(&min, &max);
}

bool cCcD_SphAttr::GetNVec(const cXyz& param_0, cXyz* param_1) const {
    param_1->x = param_0.x - mCenter.x;
    param_1->y = param_0.y - mCenter.y;
    param_1->z = param_0.z - mCenter.z;

    if (cM3d_IsZero(VECMag(param_1))) {
        param_1->x = 0.0f;
        param_1->y = 0.0f;
        param_1->z = 0.0f;
        return false;
    } else {
        VECNormalize(param_1, param_1);
        return true;
    }
}

void cCcD_SphAttr::getShapeAccess(cCcD_ShapeAttr::Shape* pshape) const {
    pshape->_0 = 0;
    pshape->_4.x = mCenter.x;
    pshape->_4.y = mCenter.y;
    pshape->_4.z = mCenter.z;
    pshape->_10 = mRadius;
    pshape->_14 = 0.0f;
}

void cCcD_ObjAt::SetHit(cCcD_Obj* pObj) {
    mRPrm = 1;
    mHitObj = pObj;
}

void cCcD_ObjAt::Set(const cCcD_SrcObjAt& src) {
    cCcD_ObjCommonBase::Set(src.mBase);
    mType = src.mType;
    mAtp = src.mAtp;
}

void cCcD_ObjAt::ClrHit() {
    mRPrm &= ~1;
    mHitObj = NULL;
}

void cCcD_ObjTg::Set(const cCcD_SrcObjTg& src) {
    cCcD_ObjCommonBase::Set(src.mBase);
    mType = src.mType;
}

void cCcD_ObjTg::SetGrp(u32 grp) {
    mSPrm &= ~0x1E;
    mSPrm |= grp;
}

void cCcD_ObjTg::ClrHit() {
    mRPrm &= ~1;
    mHitObj = NULL;
}

void cCcD_ObjTg::SetHit(cCcD_Obj* pObj) {
    mRPrm = 1;
    mHitObj = pObj;
}

void cCcD_ObjCo::SetHit(cCcD_Obj* pObj) {
    mRPrm = 1;
    mHitObj = pObj;
}

void cCcD_ObjCo::ClrHit() {
    mRPrm &= ~1;
    mHitObj = NULL;
}

void cCcD_ObjCo::SetIGrp(u32 grp) {
    mSPrm &= ~0xE;
    mSPrm |= grp;
}

void cCcD_ObjCo::SetVsGrp(u32 grp) {
    mSPrm &= ~0x70;
    mSPrm |= grp;
}
