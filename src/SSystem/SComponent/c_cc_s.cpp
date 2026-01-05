/**
 * c_cc_s.cpp
 *
 */

#include "SSystem/SComponent/c_cc_s.h"
#include "JSystem/JUtility/JUTAssert.h"

#define CHECK_FLOAT_CLASS(line, x) JUT_ASSERT(line, !isnan(x));
#define CHECK_FLOAT_RANGE(line, x) JUT_ASSERT(line, -1.0e32f < x && x < 1.0e32f);

cCcS::cCcS() {}

void cCcS::Ct() {
    for (cCcD_Obj** obj = mpObjAt; obj < mpObjAt + ARRAY_SIZEU(mpObjAt); ++obj) {
        *obj = NULL;
    }
    mObjAtCount = 0;

    for (cCcD_Obj** obj = mpObjTg; obj < mpObjTg + ARRAY_SIZEU(mpObjTg); ++obj) {
        *obj = NULL;
    }
    mObjTgCount = 0;

    for (cCcD_Obj** obj = mpObjCo; obj < mpObjCo + ARRAY_SIZEU(mpObjCo); ++obj) {
        *obj = NULL;
    }
    mObjCoCount = 0;

    for (cCcD_Obj** obj = mpObj; obj < mpObj + ARRAY_SIZEU(mpObj); ++obj) {
        *obj = NULL;
    }
    mObjCount = 0;
}

void cCcS::Dt() {
    Ct();
}

WeightType cCcS::GetWt(u8 param_0) const {
    if (param_0 == 0xFF) {
        return WeightType_0;
    }
    if (param_0 == 0xFE) {
        return WeightType_1;
    }
    return WeightType_2;
}

void cCcS::Set(cCcD_Obj* obj) {
    if (obj->ChkAtSet()) {
        if (mObjAtCount >= ARRAY_SIZEU(mpObjAt)) {
            OS_REPORT("\x1b[43;30m");
            OS_REPORT("cCcS::Set AT Overflow.Now Max is %d.\n", ARRAY_SIZEU(mpObjAt));
            OS_REPORT("\x1b[m");
        } else {
            mpObjAt[mObjAtCount] = obj;
            mObjAtCount++;
        }
    }

    if (obj->ChkTgSet()) {
        if (mObjTgCount >= ARRAY_SIZEU(mpObjTg)) {
            OS_REPORT("\x1b[43;30m");
            OS_REPORT("cCcS::Set TG Overflow.Now Max is %d.\n", ARRAY_SIZEU(mpObjTg));
            OS_REPORT("\x1b[m");
        } else {
            mpObjTg[mObjTgCount] = obj;
            mObjTgCount++;
        }
    }

    if (obj->ChkCoSet()) {
        if (mObjCoCount >= ARRAY_SIZEU(mpObjCo)) {
            OS_REPORT("\x1b[43;30m");
            OS_REPORT("cCcS::Set CO Overflow.Now Max is %d.\n", ARRAY_SIZEU(mpObjCo));
            OS_REPORT("\x1b[m");
        } else {
            mpObjCo[mObjCoCount] = obj;
            mObjCoCount++;
        }
    }

    if (mObjCount >= ARRAY_SIZEU(mpObj)) {
        OS_REPORT("\x1b[43;30m");
        OS_REPORT("cCcS::Set SET Overflow.Now Max is %d.\n", ARRAY_SIZEU(mpObj));
        OS_REPORT("\x1b[m");
    } else {
        mpObj[mObjCount] = obj;
        mObjCount++;
    }
}

void cCcS::ClrCoHitInf() {
    for (cCcD_Obj** obj = mpObjCo; obj < mpObjCo + mObjCoCount; ++obj) {
        if (*obj != NULL) {
            (*obj)->GetGObjInf()->ClrCoHit();

            cCcD_Stts* stts = (*obj)->GetStts();
            if (stts != NULL) {
                stts->ClrCcMove();
            }
        }
    }
}

void cCcS::ClrTgHitInf() {
    for (cCcD_Obj** obj = mpObjTg; obj < mpObjTg + mObjTgCount; ++obj) {
        if (*obj != NULL) {
            (*obj)->GetGObjInf()->ClrTgHit();

            cCcD_Stts* stts = (*obj)->GetStts();
            if (stts != NULL) {
                stts->ClrTg();
            }
        }
    }
}

void cCcS::ClrAtHitInf() {
    for (cCcD_Obj** obj = mpObjAt; obj < mpObjAt + mObjAtCount; ++obj) {
        if (*obj != NULL) {
            (*obj)->GetGObjInf()->ClrAtHit();

            cCcD_Stts* stts = (*obj)->GetStts();
            if (stts != NULL) {
                stts->ClrAt();
            }
        }
    }
}

bool cCcS::ChkNoHitAtTg(cCcD_Obj* pat_obj, cCcD_Obj* ptg_obj) {
    fopAc_ac_c* pat_ac = pat_obj->GetAc();
    fopAc_ac_c* ptg_ac = ptg_obj->GetAc();
    if ((pat_ac != NULL && ptg_ac != NULL && pat_ac == ptg_ac) ||
        (pat_obj->GetAtGrp() & ptg_obj->GetTgGrp()) == 0 ||
        (pat_obj->GetAtType() & ptg_obj->GetTgType()) == 0)
    {
        return true;
    } else {
        return ChkNoHitGAtTg(pat_obj->GetGObjInf(), ptg_obj->GetGObjInf(),
                             pat_obj->GetStts()->GetGStts(), ptg_obj->GetStts()->GetGStts());
    }
}

void cCcS::ChkAtTg() {
    cCcD_Obj** objTgEnd = mpObjTg + mObjTgCount;
    ClrAtHitInf();
    ClrTgHitInf();
    for (cCcD_Obj** pat_obj = mpObjAt; pat_obj < mpObjAt + mObjAtCount; ++pat_obj) {
        if (*pat_obj == NULL || !(*pat_obj)->ChkAtSet())
            continue;

        cCcD_ShapeAttr* pat_sa = (*pat_obj)->GetShapeAttr();
        JUT_ASSERT(0, pat_sa != NULL);

        for (cCcD_Obj** ptg_obj = mpObjTg; ptg_obj < objTgEnd; ++ptg_obj) {
            if (*ptg_obj == NULL || !(*ptg_obj)->ChkTgSet())
                continue;
            if (!(*pat_obj)->GetDivideInfo().Chk((*ptg_obj)->GetDivideInfo()))
                continue;
            if (ChkNoHitAtTg(*pat_obj, *ptg_obj))
                continue;

            cCcD_ShapeAttr* ptg_sa = (*ptg_obj)->GetShapeAttr();
            JUT_ASSERT(0, ptg_sa != NULL);

            static cXyz cross;
            bool didCross = pat_sa->CrossAtTg(*ptg_sa, &cross);
            bool anyBsRevHit = (*pat_obj)->ChkBsRevHit() || (*ptg_obj)->ChkBsRevHit();
            if (!anyBsRevHit && didCross) {
                SetAtTgCommonHitInf(*pat_obj, *ptg_obj, &cross);
            } else if (anyBsRevHit && !didCross) {
                cCcD_ShapeAttr* pat_sa = (*pat_obj)->GetShapeAttr();
                if (pat_sa == NULL) {
                    cross.set(0.0f, 0.0f, 0.0f);
                } else {
                    pat_sa->GetWorkAab().CalcCenter(&cross);
                }

                SetAtTgCommonHitInf(*pat_obj, *ptg_obj, &cross);
            }
        }
    }
}

bool cCcS::ChkNoHitCo(cCcD_Obj* pco1_obj, cCcD_Obj* pco2_obj) {
    fopAc_ac_c* ac1 = pco1_obj->GetAc();
    fopAc_ac_c* ac2 = pco2_obj->GetAc();
    if (!(((ac1 == NULL || ac2 == NULL) || ac1 != ac2 || pco1_obj->ChkCoSameActorHit() ||
           pco2_obj->ChkCoSameActorHit()) &&
          (pco1_obj->GetCoIGrp() & (pco2_obj->GetCoVsGrp() >> 3)) &&
          ((pco1_obj->GetCoVsGrp() >> 3) & pco2_obj->GetCoIGrp() &&
           !ChkNoHitGCo(pco1_obj, pco2_obj))))
    {
        return true;
    } else {
        return false;
    }
}

void cCcS::ChkCo() {
    ClrCoHitInf();
    if (mObjCoCount <= 1)
        return;

    cCcD_Obj** objCoEnd = mpObjCo + mObjCoCount;
    for (cCcD_Obj** pco1_obj = mpObjCo; pco1_obj < objCoEnd - 1; ++pco1_obj) {
        if (*pco1_obj == NULL || !(*pco1_obj)->ChkCoSet())
            continue;

        cCcD_ShapeAttr* pco1_sa = (*pco1_obj)->GetShapeAttr();
        JUT_ASSERT(0, pco1_sa != NULL);

        for (cCcD_Obj** pco2_obj = pco1_obj + 1; pco2_obj < objCoEnd; ++pco2_obj) {
            if (*pco2_obj == NULL || !(*pco2_obj)->ChkCoSet())
                continue;
            if (!(*pco1_obj)->GetDivideInfo().Chk((*pco2_obj)->GetDivideInfo()))
                continue;
            if (ChkNoHitCo(*pco1_obj, *pco2_obj))
                continue;

            cCcD_ShapeAttr* pco2_sa = (*pco2_obj)->GetShapeAttr();
            JUT_ASSERT(0, pco2_sa != NULL);

            f32 cross_len;
            if (pco1_sa->CrossCo(*pco2_sa, &cross_len)) {
                cXyz& co2_center = pco2_sa->GetCoCP();
                cXyz& co1_center = pco1_sa->GetCoCP();
                SetCoCommonHitInf(*pco1_obj, &co1_center, *pco2_obj, &co2_center, cross_len);
            }
        }
    }
}

void cCcS::CalcTgPlusDmg(cCcD_Obj* pat_obj, cCcD_Obj* ptg_obj, cCcD_Stts* pat_stts,
                         cCcD_Stts* ptg_stts) {
    ptg_stts->PlusDmg(pat_obj->GetAtAtp());
}

void cCcS::SetAtTgCommonHitInf(cCcD_Obj* pat_obj, cCcD_Obj* ptg_obj, cXyz* pcross) {
    cCcD_Stts* pat_stts = pat_obj->GetStts();
    cCcD_Stts* ptg_stts = ptg_obj->GetStts();
    if (!ChkAtTgHitAfterCross(!ptg_obj->ChkTgNoAtHitInfSet(), !pat_obj->ChkAtNoTgHitInfSet(),
                              pat_obj->GetGObjInf(), ptg_obj->GetGObjInf(), pat_stts, ptg_stts,
                              pat_stts->GetGStts(), ptg_stts->GetGStts()))
    {
        if (!ptg_obj->ChkTgNoAtHitInfSet()) {
            pat_obj->SetAtHit(ptg_obj);
        }

        bool tmp = !(pat_obj->ChkAtNoTgHitInfSet() ||
                     (pat_obj->ChkAtType(AT_TYPE_SLINGSHOT) && ptg_obj->ChkTgNoSlingHitInfSet()));
        if (tmp) {
            ptg_obj->SetTgHit(pat_obj);
            CalcTgPlusDmg(pat_obj, ptg_obj, pat_stts, ptg_stts);
        }

        SetAtTgGObjInf(!ptg_obj->ChkTgNoAtHitInfSet(), tmp, pat_obj, ptg_obj, pat_obj->GetGObjInf(),
                       ptg_obj->GetGObjInf(), pat_stts, ptg_stts, pat_stts->GetGStts(),
                       ptg_stts->GetGStts(), pcross);
    }
}

void cCcS::SetCoCommonHitInf(cCcD_Obj* pco1_obj, cXyz* ppos1, cCcD_Obj* pco2_obj, cXyz* ppos2,
                             f32 cross_len) {
    bool co2_inf_set = !pco2_obj->ChkCoNoCoHitInfSet();
    bool co1_inf_set = !pco1_obj->ChkCoNoCoHitInfSet();
    if (co2_inf_set) {
        pco1_obj->SetCoHit(pco2_obj);
    }

    if (co1_inf_set) {
        pco2_obj->SetCoHit(pco1_obj);
    }

    if (co2_inf_set && co1_inf_set) {
        SetPosCorrect(pco1_obj, ppos1, pco2_obj, ppos2, cross_len);
    }

    cCcD_Stts* pco1_stts = pco1_obj->GetStts();
    cCcD_Stts* pco2_stts = pco2_obj->GetStts();
    SetCoGObjInf(co2_inf_set, co1_inf_set, pco1_obj->GetGObjInf(), pco2_obj->GetGObjInf(),
                 pco1_stts, pco2_stts, pco1_stts->GetGStts(), pco2_stts->GetGStts());
}

void cCcS::SetPosCorrect(cCcD_Obj* pco1_obj, cXyz* ppos1, cCcD_Obj* pco2_obj, cXyz* ppos2,
                         f32 cross_len) {
    CHECK_FLOAT_CLASS(616, cross_len);
    CHECK_FLOAT_RANGE(617, cross_len);

    if (pco1_obj->ChkCoNoCrr() || pco2_obj->ChkCoNoCrr())
        return;
    if (pco1_obj->GetStts() == NULL || pco2_obj->GetStts() == NULL)
        return;
    if (pco1_obj->GetStts()->GetAc() != NULL &&
        pco1_obj->GetStts()->GetAc() == pco2_obj->GetStts()->GetAc())
        return;

    if (!(fabsf(cross_len) < (1.0f / 125.0f))) {
        SetCoGCorrectProc(pco1_obj, pco2_obj);
        bool bothCoSph3DCrr = pco1_obj->ChkCoSph3DCrr() && pco2_obj->ChkCoSph3DCrr();
        WeightType obj1WeightType = GetWt(pco1_obj->GetStts()->GetWeightUc());
        WeightType obj2WeightType = GetWt(pco2_obj->GetStts()->GetWeightUc());
        f32 obj1SrcWeight = pco1_obj->GetStts()->GetWeightF();
        f32 obj2SrcWeight = pco2_obj->GetStts()->GetWeightF();
        f32 combinedWeight = obj1SrcWeight + obj2SrcWeight;

        f32 obj2Weight, obj1Weight;
        if (cM3d_IsZero(combinedWeight)) {
            obj1SrcWeight = 1;
            obj2SrcWeight = 1;
            combinedWeight = 2;
        }

        f32 invCombinedWeight = 1 / combinedWeight;
        if (obj1WeightType == WeightType_0) {
            if (obj2WeightType == WeightType_0) {
                return;
            } else {
                obj2Weight = 0;
                obj1Weight = 1;
            }
        } else if (obj1WeightType == WeightType_1) {
            if (obj2WeightType == WeightType_0) {
                obj2Weight = 1;
                obj1Weight = 0;
            } else if (obj2WeightType == WeightType_1) {
                obj2Weight = 0.5;
                obj1Weight = 0.5;
            } else {
                obj2Weight = 0;
                obj1Weight = 1;
            }
        } else {
            if (obj2WeightType == WeightType_2) {
                obj2Weight = obj2SrcWeight * invCombinedWeight;
                obj1Weight = obj1SrcWeight * invCombinedWeight;
            } else {
                obj2Weight = 1;
                obj1Weight = 0;
            }
        }

        f32 objDistLen;
        Vec vec1;
        Vec vec2;
        Vec objsDist;
        if (bothCoSph3DCrr) {
            VECSubtract(ppos2, ppos1, &objsDist);
            objDistLen = VECMag(&objsDist);
        } else {
            objsDist.x = ppos2->x - ppos1->x;
            objsDist.y = 0;
            objsDist.z = ppos2->z - ppos1->z;
            objDistLen = sqrtf(objsDist.x * objsDist.x + objsDist.z * objsDist.z);
        }

        if (!cM3d_IsZero(objDistLen)) {
            if (bothCoSph3DCrr) {
                VECScale(&objsDist, &objsDist, cross_len / objDistLen);
                obj2Weight *= -1;
                VECScale(&objsDist, &vec1, obj2Weight);
                VECScale(&objsDist, &vec2, obj1Weight);
            } else {
                f32 pushFactor = cross_len / objDistLen;
                objsDist.x *= pushFactor;
                objsDist.z *= pushFactor;
                vec1.x = -objsDist.x * obj2Weight;
                vec1.y = 0;
                vec1.z = -objsDist.z * obj2Weight;
                vec2.x = objsDist.x * obj1Weight;
                vec2.y = 0;
                vec2.z = objsDist.z * obj1Weight;
            }
        } else {
            vec1.y = 0;
            vec1.z = 0;
            vec2.y = 0;
            vec2.z = 0;
            if (!cM3d_IsZero(cross_len)) {
                vec1.x = -cross_len * obj2Weight;
                vec2.x = cross_len * obj1Weight;
            } else {
                vec1.x = -obj2Weight;
                vec2.x = obj1Weight;
            }
        }

        CHECK_FLOAT_CLASS(767, vec1.x);
        CHECK_FLOAT_CLASS(768, vec1.y);
        CHECK_FLOAT_CLASS(769, vec1.z);

        CHECK_FLOAT_CLASS(771, vec2.x);
        CHECK_FLOAT_CLASS(772, vec2.y);
        CHECK_FLOAT_CLASS(773, vec2.z);

        CHECK_FLOAT_RANGE(775, vec1.x);
        CHECK_FLOAT_RANGE(776, vec1.y);
        CHECK_FLOAT_RANGE(777, vec1.z);

        CHECK_FLOAT_RANGE(779, vec2.x);
        CHECK_FLOAT_RANGE(780, vec2.y);
        CHECK_FLOAT_RANGE(781, vec2.z);

        pco1_obj->GetStts()->PlusCcMove(vec1.x, vec1.y, vec1.z);
        pco2_obj->GetStts()->PlusCcMove(vec2.x, vec2.y, vec2.z);
        (*ppos1) += vec1;
        (*ppos2) += vec2;

        CHECK_FLOAT_CLASS(790, ppos1->x);
        CHECK_FLOAT_CLASS(791, ppos1->y);
        CHECK_FLOAT_CLASS(792, ppos1->z);

        CHECK_FLOAT_CLASS(794, ppos2->x);
        CHECK_FLOAT_CLASS(795, ppos2->y);
        CHECK_FLOAT_CLASS(796, ppos2->z);

        CHECK_FLOAT_RANGE(798, ppos1->x);
        CHECK_FLOAT_RANGE(799, ppos1->y);
        CHECK_FLOAT_RANGE(800, ppos1->z);

        CHECK_FLOAT_RANGE(802, ppos2->x);
        CHECK_FLOAT_RANGE(803, ppos2->y);
        CHECK_FLOAT_RANGE(804, ppos2->z);
    }
}

void cCcS::CalcArea() {
    cM3dGAab aab;
    aab.ClearForMinMax();
    for (cCcD_Obj** pset_obj = mpObj; pset_obj < mpObj + mObjCount; ++pset_obj) {
        if (*pset_obj != NULL) {
            cCcD_ShapeAttr* pset_sa = (*pset_obj)->GetShapeAttr();
            JUT_ASSERT(0, pset_sa != NULL);

            pset_sa->CalcAabBox();
            aab.SetMinMax(pset_sa->GetWorkAab());
        }
    }

    mDivideArea.SetArea(aab);
    for (cCcD_Obj** pset_obj = mpObj; pset_obj < mpObj + mObjCount; ++pset_obj) {
        if (*pset_obj != NULL) {
            const cCcD_ShapeAttr* pset_sa = (*pset_obj)->GetShapeAttr();
            JUT_ASSERT(0, pset_sa != NULL);

            cCcD_DivideInfo* divideInfo = &(*pset_obj)->GetDivideInfo();
            mDivideArea.CalcDivideInfo(divideInfo, pset_sa->GetWorkAab(),
                                       (*pset_obj)->ChkBsRevHit());
        }
    }
}

void cCcS::Move() {
    CalcArea();
    ChkAtTg();
    ChkCo();
    MoveAfterCheck();
    mObjAtCount = 0;
    mObjTgCount = 0;
    mObjCoCount = 0;
    mObjCount = 0;
}

void cCcS::DrawClear() {
    for (cCcD_Obj** obj = mpObjAt; obj < mpObjAt + mObjAtCount; ++obj) {
        *obj = NULL;
    }

    mObjAtCount = 0;
    for (cCcD_Obj** obj = mpObjTg; obj < mpObjTg + mObjTgCount; ++obj) {
        *obj = NULL;
    }

    mObjTgCount = 0;
    for (cCcD_Obj** obj = mpObjCo; obj < mpObjCo + mObjCoCount; ++obj) {
        *obj = NULL;
    }

    mObjCoCount = 0;
    for (cCcD_Obj** obj = mpObj; obj < mpObj + mObjCount; ++obj) {
        *obj = NULL;
    }

    mObjCount = 0;
}

void cCcS::SetCoGCorrectProc(cCcD_Obj* pco1_obj, cCcD_Obj* pco2_obj) {}

void cCcS::SetCoGObjInf(bool co2_set, bool co1_set, cCcD_GObjInf* pco1_gobj,
                        cCcD_GObjInf* pco2_gobj, cCcD_Stts* pco1_stts, cCcD_Stts* pco2_stts,
                        cCcD_GStts* pco1_gstts, cCcD_GStts* pco2_gstts) {}

void cCcS::SetAtTgGObjInf(bool param_0, bool param_1, cCcD_Obj* pat_obj, cCcD_Obj* ptg_obj,
                          cCcD_GObjInf* pat_gobj, cCcD_GObjInf* ptg_gobj, cCcD_Stts* pat_stts,
                          cCcD_Stts* ptg_stts, cCcD_GStts* pat_gstts, cCcD_GStts* ptg_gstts,
                          cXyz* pcross) {}

bool cCcS::ChkNoHitGAtTg(const cCcD_GObjInf* pat_gobj, const cCcD_GObjInf* ptg_gobj,
                         cCcD_GStts* pat_gstts, cCcD_GStts* ptg_gstts) {
    return false;
}

bool cCcS::ChkAtTgHitAfterCross(bool param_0, bool param_1, const cCcD_GObjInf* pat_gobj,
                                const cCcD_GObjInf* ptg_gobj, cCcD_Stts* pat_stts, cCcD_Stts* ptg_stts,
                                cCcD_GStts* pat_gstts, cCcD_GStts* ptg_gstts) {
    return false;
}

bool cCcS::ChkNoHitGCo(cCcD_Obj* pco1_obj, cCcD_Obj* pco2_obj) {
    return false;
}

void cCcS::MoveAfterCheck() {}
