/**
 * d_cc_mass_s.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_cc_mass_s.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor.h"

void dCcMassS_Obj::Set(cCcD_Obj* pobj, u8 priority, dCcMassS_ObjCallback callback) {
    mpObj = pobj;
    mPriority = (u8)priority;
    mpAreaCheckFunc = callback;
}

void dCcMassS_Obj::Clear() {
    mpObj = NULL;
    mPriority = 5;
    mpAreaCheckFunc = NULL;
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
    field_0x200 = (u8)param_2;
    field_0x201 = (u8)param_3;
}

void dCcMassS_Mng::Prepare() {
    cM3dGAab aab;
    aab.ClearForMinMax();

    for (dCcMassS_Obj* i = mMassObjs; i < mMassObjs + mMassObjCount; ++i) {
        cCcD_Obj* pobj = i->GetObj();
        JUT_ASSERT(123, pobj != NULL);

        cCcD_ShapeAttr* shape = pobj->GetShapeAttr();
        shape->CalcAabBox();
        aab.SetMinMax(shape->GetWorkAab());
    }

    for (dCcMassS_Obj* i = mMassAreas; i < mMassAreas + mMassAreaCount; ++i) {
        cCcD_Obj* parea = i->GetObj();
        JUT_ASSERT(135, parea != NULL);

        cCcD_ShapeAttr* shape = parea->GetShapeAttr();
        shape->CalcAabBox();
        aab.SetMinMax(shape->GetWorkAab());
    }

    if (field_0x202 & 1) {
        mCpsAttr.CalcAabBox();
        aab.SetMinMax(mCpsAttr.GetWorkAab());
    }

    mDivideArea.SetArea(aab);

    for (dCcMassS_Obj* i = mMassObjs; i < mMassObjs + mMassObjCount; ++i) {
        cCcD_Obj* pobj = i->GetObj();
        JUT_ASSERT(155, pobj != NULL);

        cCcD_ShapeAttr* shape = pobj->GetShapeAttr();
        cCcD_DivideInfo* pdivinfo = i->GetPDivideInfo();
        mDivideArea.CalcDivideInfo(pdivinfo, shape->GetWorkAab(), 0);
    }

    for (dCcMassS_Obj* i = mMassAreas; i < mMassAreas + mMassAreaCount; ++i) {
        cCcD_Obj* parea = i->GetObj();
        JUT_ASSERT(166, parea != NULL);

        cCcD_ShapeAttr* shape = parea->GetShapeAttr();
        cCcD_DivideInfo* pdivinfo = i->GetPDivideInfo();
        mDivideArea.CalcDivideInfo(pdivinfo, shape->GetWorkAab(), 0);
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

u32 dCcMassS_Mng::Chk(cXyz* ppos, fopAc_ac_c** ppactor, dCcMassS_HitInf* phitinf) {
    cCcD_DivideInfo divideInfo;
    u32 flagsMaybe = 0;
    *ppactor = NULL;

    mCylAttr.SetC(*ppos);
    mCylAttr.CalcAabBox();

    mDivideArea.CalcDivideInfoOverArea(&divideInfo, mCylAttr.GetWorkAab());
    phitinf->ClearPointer();

    if (field_0x200 & 8) {
        for (dCcMassS_Obj* massObj = mMassAreas; massObj < mMassAreas + mMassAreaCount; ++massObj) {
            cCcD_DivideInfo* pdivinfo = massObj->GetPDivideInfo();
            if (pdivinfo->Chk(divideInfo)) {
                cCcD_Obj* pobj = massObj->GetObj();
                JUT_ASSERT(226, pobj != NULL);

                cCcD_ShapeAttr* shape = pobj->GetShapeAttr();
                f32 f;
                if (pobj->ChkCoSet() && mCylAttr.CrossCo(*shape, &f)) {
                    flagsMaybe |= 4;
                    *ppactor = pobj->GetStts()->GetActor();
                    if (phitinf != NULL) {
                        phitinf->SetAreaHitObj(pobj);
                    }

                    if (massObj->GetAreaCheckFunc() != NULL) {
                        dCcMassS_ObjCallback sp1C = massObj->GetAreaCheckFunc();
                        sp1C(pobj->GetStts()->GetActor(), ppos, field_0x201);
                    }
                }
            }
        }
    }

    for (dCcMassS_Obj* massObj = mMassObjs; massObj < mMassObjs + mMassObjCount; ++massObj) {
        cCcD_DivideInfo* pdivinfo = massObj->GetPDivideInfo();
        if (pdivinfo->Chk(divideInfo)) {
            cXyz unusedVec;
            cCcD_Obj* pobj = massObj->GetObj();
            dCcD_GObjInf* gobjInf = (dCcD_GObjInf*)pobj->GetGObjInf();
            JUT_ASSERT(266, pobj != NULL);
            
            cCcD_ShapeAttr* shape = pobj->GetShapeAttr();
            
            if (pobj->ChkAtSet() && !gobjInf->ChkAtNoMass() &&
                mCylAttr.CrossAtTg(*shape, &unusedVec) && (field_0x200 & 1))
            {
                flagsMaybe |= 1;
                *ppactor = pobj->GetStts()->GetActor();
                if (phitinf != NULL) {
                    phitinf->SetAtHitObj(pobj);
                }
            }

            f32 f;
            if (pobj->ChkCoSet() && mCylAttr.CrossCo(*shape, &f) && (field_0x200 & 2)) {
                flagsMaybe |= 2;
                *ppactor = pobj->GetStts()->GetActor();

                if (field_0x200 & 0x10) {
                    cXyz vec;
                    PSVECSubtract(&(*ppactor)->current.pos, ppos, &vec);
                    vec.y = 0;
                    f32 vecMag = PSVECMag(&vec);
                    if (cM3d_IsZero(vecMag)) {
                        vec.x = 1;
                    } else {
                        PSVECScale(&vec, &vec, f / vecMag);
                    }
                    pobj->GetStts()->PlusCcMove(vec.x, vec.y, vec.z);
                }

                if (phitinf != NULL) {
                    phitinf->SetCoHitObj(pobj);
                    phitinf->SetCoHitLen(f);
                }
            }
        }
    }

    f32 f;
    if ((field_0x202 & 1) && mDivideInfo.Chk(divideInfo) && mCylAttr.CrossCo(mCpsAttr, &f)) {
        mResultCam |= (u8)1;
        mResultCam |= (1 << (field_0x201 + 1));

        if ((mResultCam & 2) || (mResultCam & 8)) {
            Vec tmpVec;
            f32 plusH = ppos->y + mCylAttr.GetH();
            tmpVec.x = ppos->x;
            tmpVec.y = plusH;
            tmpVec.z = ppos->z;

            if (mCamTopPos.y < (20.0f + plusH)) {
                f32 newCamTopDist = PSVECSquareDistance(&tmpVec, mCpsAttr.GetStartP());
                if (mCamTopDist > newCamTopDist) {
                    mCamTopDist = newCamTopDist;
                    mCamTopPos = tmpVec;
                }
            }

            if (mCamBottomPos.y < (20.0f + plusH)) {
                f32 newCamBottomDist = PSVECSquareDistance(&tmpVec, mCpsAttr.GetEndP());
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

void dCcMassS_Mng::Set(cCcD_Obj* pobj, u8 priority) {
    if (mMassObjCount >= ARRAY_SIZE(mMassObjs)) {
        for (int i = 0; i < ARRAY_SIZE(mMassObjs); ++i) {
            int prevPrio = mMassObjs[i].GetPriority();
            if (prevPrio > priority || (prevPrio == priority && cM_rndF(1.0f) < 0.5f)) {
                mMassObjs[i].Set(pobj, priority, NULL);
                break;
            }
        }
    } else {
        mMassObjs[mMassObjCount].Set(pobj, priority, NULL);
        mMassObjCount++;
    }
}

void dCcMassS_Mng::SetCam(const cM3dGCps& cps) {
    mCpsAttr.SetCps(cps);
    field_0x202 |= (u8)1;
    mResultCam = 0;
}

u32 dCcMassS_Mng::GetResultCam() const {
    return mResultCam;
}

void dCcMassS_Mng::GetCamTopPos(Vec* ppos) {
    *ppos = mCamTopPos;
}
