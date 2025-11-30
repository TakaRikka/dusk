/**
 * d_cc_mass_s.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_cc_mass_s.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor.h"

void dCcMassS_Obj::Set(cCcD_Obj* p_obj, u8 priority, dCcMassS_ObjCallback callback) {
    mpObj = p_obj;
    mPriority = priority;
    mpCallback = callback;
}

void dCcMassS_Obj::Clear() {
    mpObj = NULL;
    mPriority = 5;
    mpCallback = NULL;
    mDivideInfo.Set(0, 0, 0);
}

void dCcMassS_HitInf::ClearPointer() {
    mpArea = NULL;
    mpAtObj = NULL;
    mpCoObj = NULL;
    mCoHitLen = 0.0f;
}

dCcMassS_Mng::dCcMassS_Mng() {
    Ct();
}

dCcMassS_Obj::dCcMassS_Obj() {}

void dCcMassS_Mng::Ct() {
    field_0x202 = 0;
    mResultCam = 0;
    mCamTopPos.x = 0.0f;
    mCamTopPos.y = -G_CM3D_F_INF;
    mCamTopPos.z = 0.0f;

    mCamBottomPos.x = 0.0f;
    mCamBottomPos.y = -G_CM3D_F_INF;
    mCamBottomPos.z = 0.0f;

    Clear();
}

void dCcMassS_Mng::SetAttr(f32 radius, f32 height, u8 param_2, u8 param_3) {
    mCylAttr.SetR(radius);
    mCylAttr.SetH(height);
    field_0x200 = param_2;
    field_0x201 = param_3;
}

void dCcMassS_Mng::Prepare() {
    cM3dGAab aab;
    aab.ClearForMinMax();

    for (dCcMassS_Obj* pObj = mMassObjs; pObj < mMassObjs + mMassObjCount; ++pObj) {
        cCcD_ShapeAttr* objShape = pObj->GetObj()->GetShapeAttr();
        objShape->CalcAabBox();
        aab.SetMinMax(objShape->GetWorkAab());
    }

    for (dCcMassS_Obj* pObj = mMassAreas; pObj < mMassAreas + mMassAreaCount; ++pObj) {
        cCcD_ShapeAttr* objShape = pObj->GetObj()->GetShapeAttr();
        objShape->CalcAabBox();
        aab.SetMinMax(objShape->GetWorkAab());
    }

    if (field_0x202 & 1) {
        mCpsAttr.CalcAabBox();
        aab.SetMinMax(mCpsAttr.GetWorkAab());
    }

    mDivideArea.SetArea(aab);
    for (dCcMassS_Obj* pObj = mMassObjs; pObj < mMassObjs + mMassObjCount; ++pObj) {
        cCcD_ShapeAttr* objShape = pObj->GetObj()->GetShapeAttr();
        cCcD_DivideInfo* divideInfo = &pObj->GetDivideInfo();
        mDivideArea.CalcDivideInfo(divideInfo, objShape->GetWorkAab(), 0);
    }

    for (dCcMassS_Obj* pObj = mMassAreas; pObj < mMassAreas + mMassAreaCount; ++pObj) {
        cCcD_ShapeAttr* objShape = pObj->GetObj()->GetShapeAttr();
        cCcD_DivideInfo* divideInfo = &pObj->GetDivideInfo();
        mDivideArea.CalcDivideInfo(divideInfo, objShape->GetWorkAab(), 0);
    }

    if (field_0x202 & 1) {
        mDivideArea.CalcDivideInfo(&mDivideInfo, mCpsAttr.GetWorkAab(), 0);
    }

    mCamTopPos.x = 0.0f;
    mCamTopPos.y = -G_CM3D_F_INF;
    mCamTopPos.z = 0.0f;
    mCamTopDist = G_CM3D_F_INF;

    mCamBottomPos.x = 0.0f;
    mCamBottomPos.y = -G_CM3D_F_INF;
    mCamBottomPos.z = 0.0f;
    mCamBottomDist = G_CM3D_F_INF;
}

u32 dCcMassS_Mng::Chk(cXyz* p_xyz, fopAc_ac_c** p_actor, dCcMassS_HitInf* p_hitInf) {
    cCcD_DivideInfo divideInfo;
    u32 flagsMaybe = 0;
    *p_actor = NULL;

    mCylAttr.SetC(*p_xyz);
    mCylAttr.CalcAabBox();

    mDivideArea.CalcDivideInfoOverArea(&divideInfo, mCylAttr.GetWorkAab());
    p_hitInf->ClearPointer();

    if (field_0x200 & 8) {
        for (dCcMassS_Obj* massObj = mMassAreas; massObj < mMassAreas + mMassAreaCount; ++massObj) {
            if (massObj->GetDivideInfo().Chk(divideInfo)) {
                cCcD_Obj* obj = massObj->GetObj();
                cCcD_ShapeAttr* objShape = obj->GetShapeAttr();
                f32 f;
                if (obj->ChkCoSet() && mCylAttr.CrossCo(*objShape, &f)) {
                    flagsMaybe |= 4;
                    *p_actor = obj->GetStts()->GetAc();
                    if (p_hitInf != NULL) {
                        p_hitInf->SetAreaHitObj(obj);
                    }

                    if (massObj->GetCallback() != NULL) {
                        massObj->GetCallback()(obj->GetStts()->GetAc(), p_xyz, field_0x201);
                    }
                }
            }
        }
    }

    for (dCcMassS_Obj* massObj = mMassObjs; massObj < mMassObjs + mMassObjCount; ++massObj) {
        if (massObj->GetDivideInfo().Chk(divideInfo)) {
            cCcD_Obj* obj = massObj->GetObj();
            dCcD_GObjInf* gobjInf = (dCcD_GObjInf*)obj->GetGObjInf();
            cCcD_ShapeAttr* objShape = obj->GetShapeAttr();
            cXyz unusedVec;
            if (obj->ChkAtSet() && !gobjInf->ChkAtNoMass() &&
                mCylAttr.CrossAtTg(*objShape, &unusedVec) && (field_0x200 & 1))
            {
                flagsMaybe |= 1;
                *p_actor = obj->GetStts()->GetAc();
                if (p_hitInf != NULL) {
                    p_hitInf->SetAtHitObj(obj);
                }
            }

            f32 f;
            if (obj->ChkCoSet() && mCylAttr.CrossCo(*objShape, &f) && (field_0x200 & 2)) {
                flagsMaybe |= 2;
                *p_actor = obj->GetStts()->GetAc();

                if (field_0x200 & 0x10) {
                    Vec vec;
                    VECSubtract(&(*p_actor)->current.pos, p_xyz, &vec);
                    vec.y = 0;
                    f32 vecMag = VECMag(&vec);
                    if (cM3d_IsZero(vecMag)) {
                        vec.x = 1;
                    } else {
                        VECScale(&vec, &vec, f / vecMag);
                    }
                    obj->GetStts()->PlusCcMove(vec.x, vec.y, vec.z);
                }

                if (p_hitInf != NULL) {
                    p_hitInf->SetCoHitObj(obj);
                    p_hitInf->SetCoHitLen(f);
                }
            }
        }
    }

    f32 f;
    if ((field_0x202 & 1) && mDivideInfo.Chk(divideInfo) && mCylAttr.CrossCo(mCpsAttr, &f)) {
        mResultCam |= 1;
        mResultCam |= 1 << (field_0x201 + 1);

        if ((mResultCam & 2) || (mResultCam & 8)) {
            cXyz tmpVec;
            f32 plusH = p_xyz->y + mCylAttr.GetH();
            tmpVec.x = p_xyz->x;
            tmpVec.y = plusH;
            tmpVec.z = p_xyz->z;

            if (mCamTopPos.y < (20.0f + plusH)) {
                f32 newCamTopDist = VECSquareDistance(&tmpVec, &mCpsAttr.GetStartP());
                if (mCamTopDist > newCamTopDist) {
                    mCamTopDist = newCamTopDist;
                    mCamTopPos = tmpVec;
                }
            }

            if (mCamBottomPos.y < (20.0f + plusH)) {
                f32 newCamBottomDist = VECSquareDistance(&tmpVec, &mCpsAttr.GetEndP());
                if (mCamBottomDist > newCamBottomDist) {
                    mCamBottomDist = newCamBottomDist;
                    mCamBottomPos = tmpVec;
                }
            }
        }
    }

    return flagsMaybe;
}

void dCcMassS_Mng::Clear() {
    mMassObjCount = 0;
    mMassAreaCount = 0;

    for (int i = 0; i < ARRAY_SIZE(mMassObjs); ++i) {
        mMassObjs[i].Clear();
    }

    for (int i = 0; i < ARRAY_SIZE(mMassAreas); ++i) {
        mMassAreas[i].Clear();
    }

    mCylAttr.SetR(0.0f);
    mCylAttr.SetH(0.0f);
    field_0x200 = 0;
    field_0x201 = 4;
}

void dCcMassS_Mng::Set(cCcD_Obj* p_obj, u8 priority) {
    if (mMassObjCount >= ARRAY_SIZE(mMassObjs)) {
        for (int i = 0; i < ARRAY_SIZE(mMassObjs); ++i) {
            int prevPrio = mMassObjs[i].GetPriority();
            if (prevPrio > priority || (prevPrio == priority && cM_rndF(1.0f) < 0.5f)) {
                mMassObjs[i].Set(p_obj, priority, NULL);
                break;
            }
        }
    } else {
        mMassObjs[mMassObjCount].Set(p_obj, priority, NULL);
        mMassObjCount++;
    }
}

void dCcMassS_Mng::SetCam(cM3dGCps const& cps) {
    mCpsAttr.SetCps(cps);
    field_0x202 |= 1;
    mResultCam = 0;
}

u32 dCcMassS_Mng::GetResultCam() const {
    return mResultCam;
}

void dCcMassS_Mng::GetCamTopPos(Vec* p_out) {
    *p_out = mCamTopPos;
}
