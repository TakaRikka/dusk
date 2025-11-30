//
// d_jnt_col
//

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_jnt_col.h"
#include "SSystem/SComponent/c_math.h"
#include "SSystem/SComponent/c_m3d_g_sph.h"
#include "SSystem/SComponent/c_m3d_g_cyl.h"
#include "f_op/f_op_actor_mng.h"

dJntCol_c::dJntCol_c() {
    mModel = 0;
    mData = 0;
    field_0x8 = 0;
}

int dJntCol_c::init(fopAc_ac_c* i_actorP, dJntColData_c const* i_jntColP, J3DModel* i_modelP,
                    int param_3) {
    mData = i_jntColP;
    mModel = i_modelP;
    field_0x8 = param_3;
    field_0xc = 0;

    if (i_actorP) {
        fopAcM_SetJntCol(i_actorP, this);
    }

    return 1;
}

void dJntCol_c::setNowLine(cM3dGLin* i_line, cXyz const* param_1, csXyz const* param_2,
                           cXyz* param_3) {
    if (param_2 != NULL) {
        mDoMtx_stack_c::ZXYrotS(*param_2);
        mDoMtx_stack_c::multVec(&cXyz::BaseZ, param_3);
    } else {
        *param_3 = cXyz::BaseZ;
    }

    cXyz start_pos = *param_1 - *param_3 * 500.0f;
    cXyz end_pos = *param_1 + *param_3 * 500.0f;
    i_line->SetStartEnd(start_pos, end_pos);
}

int dJntCol_c::searchNearPos(cM3dGLin const* param_1, cXyz const* param_2, cXyz* param_3,
                              int param_4) const {
    const dJntColData_c* pcVar12 = mData;
    cXyz const* pcVar9 = param_2;
    cM3dGSph acStack_a0;
    cM3dGCyl acStack_b8;
    cM3dGPla acStack_d0;
    cXyz cStack_f8;
    cXyz local_104;
    cXyz cStack_110;
    cXyz cStack_11c;
    cXyz local_128;
    cXyz cStack_134;
    cM3dGLin acStack_ec;
    cXyz cStack_140;
    cXyz cStack_14c;
    f32 dVar17;
    f32 dVar18 = FLT_MAX;
    int bVar5 = FALSE;
    int rv = -1;
    dVar17 = -10000000.0f;
    for (int i = 0; i < field_0x8; i++, pcVar12++) {
        if ((field_0xc & (1 << i)) == 0 && 
            (param_4 == 0 || pcVar12->mType == 3))
        {
            MtxP dst = mModel->getAnmMtx(pcVar12->mJntNum);
            mDoMtx_stack_c::copy(dst);
            mDoMtx_stack_c::multVec(pcVar12->field_0x8, &local_128);
            if (pcVar12->field_0x0 == 0) {
                acStack_a0.Set(local_128, pcVar12->field_0x4);
                int iVar6 = cM3d_Cross_LinSph_CrossPos(acStack_a0, *param_1, &cStack_110,
                                                                &cStack_11c);
                if (iVar6 != 0) {
                    if (iVar6 == 1) {
                        cStack_f8 = cStack_110;
                    } else {
                        f32 dVar15 = cStack_11c.abs2(param_1->GetStart());
                        f32 dVar16 = cStack_110.abs2(param_1->GetStart());
                        if (dVar16 < dVar15) {
                            cStack_f8 = cStack_110;
                        } else {
                            cStack_f8 = cStack_11c;
                        }
                    }
                } else {
                    local_104 = *param_2 - local_128;
                    local_104.normalizeZP();
                    cStack_f8 = local_128 + local_104 * pcVar12->field_0x4;
                }
            } else if (pcVar12->field_0x0 == 1) {
                mDoMtx_stack_c::multVec(pcVar12->field_0x8 + 1, &cStack_134);
                local_104 = cStack_134 - local_128;
                int bVar13;
                if (local_104.absXZ() > 0.01f) {
                    mDoMtx_stack_c::transS(local_128);
                    mDoMtx_stack_c::XrotM(cM_atan2s(-local_104.absXZ(), local_104.y));
                    mDoMtx_stack_c::YrotM(cM_atan2s(-local_104.x, local_104.z));
                    mDoMtx_stack_c::transM(-local_128.x, -local_128.y, -local_128.z);
                    mDoMtx_stack_c::multVec(&cStack_134, &cStack_11c);
                    mDoMtx_stack_c::multVec(&param_1->GetStartP(), &cStack_140);
                    mDoMtx_stack_c::multVec(&param_1->GetEndP(), &cStack_14c);
                    acStack_ec.SetStartEnd(cStack_140, cStack_14c);
                    bVar13 = 1;
                } else {
                    cStack_11c = cStack_134;
                    acStack_ec.SetStartEnd(param_1->GetStart(), param_1->GetEnd());
                    bVar13 = 0;
                }
                acStack_b8.Set(local_128, pcVar12->field_0x4,
                              cStack_11c.y - local_128.y);
                if (cM3d_Cross_CylLin(&acStack_b8, &acStack_ec, &cStack_110, &cStack_11c)) {
                    if (bVar13) {
                        mDoMtx_stack_c::inverse();
                        mDoMtx_stack_c::multVec(&cStack_110, &cStack_f8);
                    } else {
                        cStack_f8 = cStack_110;
                    }
                } else {
                    acStack_ec.SetStartEnd(local_128, cStack_134);
                    f32 dVar15 = cM3d_lineVsPosSuisenCross(&acStack_ec, param_2,
                                                                            &cStack_110);
                    local_104 =  *param_2 - cStack_110;
                    f32 dVar16 = local_104.abs2();
                    if (dVar16 < 9.999999747378752e-05f) {
                        cStack_110 = acStack_ec.GetStart() - acStack_ec.GetEnd();
                        cStack_11c = param_1->GetStart() - param_1->GetEnd();
                        f32 inprod = cStack_110.inprod(cStack_11c);
                        if (inprod > 0.0f) {
                            cStack_f8 = acStack_ec.GetEnd();
                        } else if (inprod < 0.0f) {
                            cStack_f8 = acStack_ec.GetStart();
                        } else if (dVar15 < 0.5f) {
                            cStack_f8 = acStack_ec.GetStart();
                        } else {
                            cStack_f8 = acStack_ec.GetEnd();
                        }
                    } else {
                        if (dVar15 >= 0.0f && dVar15 <= 1.0f) {
                            cStack_f8 = cStack_110 + (local_104 / JMAFastSqrt(dVar16)) * pcVar12->field_0x4;
                        } else {
                            cStack_110 = acStack_ec.GetStart() - acStack_ec.GetEnd();
                            cStack_11c = param_1->GetStart() - param_1->GetEnd();
                            f32 dVar14 = cStack_110.inprod(cStack_11c);
                            if (dVar14 > 0.0f) {
                                cStack_110 = acStack_ec.GetEnd();
                            } else {
                                if (dVar14 < 0.0f) {
                                    cStack_110 = acStack_ec.GetStart();
                                } else {
                                    if (dVar15 < 0.5f) {
                                        cStack_110 = acStack_ec.GetStart();
                                    } else {
                                        cStack_110 = acStack_ec.GetEnd();
                                    }
                                }
                            }
                            cStack_f8 = cStack_110 + (local_104 / JMAFastSqrt(dVar16)) * pcVar12->field_0x4;
                        }
                    }
                    
                }
            } else {
                mDoMtx_stack_c::multVecSR(pcVar12->field_0x8 + 1, &local_104);
                acStack_d0.SetupNP0(local_104, local_128);
                if (cM3d_Cross_LinPla(param_1, &acStack_d0, &cStack_f8, true, true)) {
                    if (!(local_128.abs(cStack_f8) < pcVar12->field_0x4)) {
                        local_104 = cStack_f8 - local_128;
                        local_104.normalizeZP();
                        cStack_f8 = local_128 + local_104 * pcVar12->field_0x4;
                    }
                } else {
                    if (acStack_d0.crossInfLin(*param_2, param_1->GetEnd(), cStack_f8)) {
                        local_104 = cStack_f8 - local_128;
                        local_104.normalizeZP();
                        cStack_f8 = local_128 + local_104 * pcVar12->field_0x4;
                    } else {
                        f32 pla = cM3d_SignedLenPlaAndPos(&acStack_d0, param_2);
                        local_104.normalizeZP();
                        cStack_f8 = *param_2 - local_104 * pla;
                    }
                }
            }
            f32 cross = cM3d_lineVsPosSuisenCross(param_1, &cStack_f8, &cStack_110);
            f32 dVar16 = cStack_f8.abs2(cStack_110);
            if (dVar16 < 9.999999747378752e-05f) {
                if (!bVar5 || dVar17 < cross) {
                    dVar17 = cross;
                    *param_3 = cStack_f8;
                    rv = i;
                    bVar5 = TRUE;
                }
            } else {
                if (!bVar5 && (dVar18 >= dVar16)) {
                    dVar18 = dVar16;
                    *param_3 = cStack_f8;
                    rv = i;
                }
            }
        }
        ;
    }
    return rv;
}

int dJntCol_c::getArrowOffsetPosAndAngle(cXyz const* param_1, csXyz const* param_2,
                                              cXyz* param_3, cXyz* param_4) const {
    cM3dGLin acStack_34;
    cXyz cStack_40;
    setNowLine(&acStack_34, param_1, param_2, &cStack_40);
    cXyz cStack_4c;
    int iVar1 = searchNearPos(&acStack_34, param_1, &cStack_4c, 0);
    if (iVar1 < 0) {
        return -3;
    }
    const dJntColData_c* jntColData = &mData[iVar1];
    if (jntColData->mType == 0) {
        return -3;
    }
    if (jntColData->mType == 2) {
        return -1;
    }
    if (jntColData->mType == 3) {
        return -2;
    }
    mDoMtx_stack_c::copy(mModel->getAnmMtx(jntColData->mJntNum));
    mDoMtx_stack_c::inverse();
    mDoMtx_stack_c::multVecSR(&cStack_40, param_4);
    mDoMtx_stack_c::multVec(&cStack_4c, param_3);
    return jntColData->mJntNum;
}

int dJntCol_c::getHitmarkPosAndAngle(cXyz const* param_1, csXyz const* param_2, cXyz* param_3,
                                         csXyz* param_4, int param_5) const {
    cM3dGLin acStack_54;
    cXyz cStack_60;
    setNowLine(&acStack_54, param_1, param_2, &cStack_60);
    int iVar3 = searchNearPos(&acStack_54, param_1, param_3, param_5);
    if (iVar3 < 0) {
        return -1;
    }
    const dJntColData_c* jntColData = &mData[iVar3];
    cXyz cStack_6c;
    cXyz local_78;
    mDoMtx_stack_c::copy(mModel->getAnmMtx(jntColData->mJntNum));
    mDoMtx_stack_c::multVec(jntColData->field_0x8, &cStack_6c);
    if (jntColData->field_0x0 == 0) {
        local_78 = *param_3 - cStack_6c;
    } else if (jntColData->field_0x0 == 1) {
        cXyz cStack_84;
        cXyz cStack_90;
        mDoMtx_stack_c::multVec(jntColData->field_0x8 + 1, &cStack_84);
        cM3d_lineVsPosSuisenCross(cStack_6c, cStack_84, *param_3, &cStack_90);
        local_78 = *param_3 - cStack_90;
        if (local_78.abs2() < 9.999999747378752e-05f) {
            if (param_3->abs2(cStack_6c) < param_3->abs2(cStack_84)) {
                local_78 = cStack_6c - cStack_84;
            } else {
                local_78 = cStack_84 - cStack_6c;
            }
        }
    } else {
        mDoMtx_stack_c::multVecSR(jntColData->field_0x8 + 1, &local_78);
    }
    param_4->x = cM_atan2s(local_78.y, local_78.absXZ());
    param_4->y = cM_atan2s(-local_78.x, -local_78.z);
    param_4->z = 0;
    return iVar3;
}

void dJntCol_c::setArrowPosAndAngle(cXyz const* param_0, cXyz const* i_srcPos, int i_anmMtxIdx,
                                    cXyz* param_3, csXyz* i_arrowPosP) {
    cXyz dst_pos;
    mDoMtx_multVecSR(mModel->getAnmMtx(i_anmMtxIdx), i_srcPos, &dst_pos);

    i_arrowPosP->x = dst_pos.atan2sY_XZ();
    i_arrowPosP->y = dst_pos.atan2sX_Z();
    mDoMtx_multVec(mModel->getAnmMtx(i_anmMtxIdx), param_0, param_3);
}
