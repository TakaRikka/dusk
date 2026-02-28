//
// d_jnt_col
//

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_jnt_col.h"
#include "JSystem/JHostIO/JORFile.h"
#include "JSystem/JHostIO/JORServer.h"
#include "SSystem/SComponent/c_math.h"
#include "SSystem/SComponent/c_m3d_g_sph.h"
#include "SSystem/SComponent/c_m3d_g_cyl.h"
#include "d/d_debug_viewer.h"
#include "f_op/f_op_actor_mng.h"
#include <cstring>

#if DEBUG
dJntCol_HIO_c l_HIO;

Vec l_debugTmpPos[] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 50.0f, 0.0f},
};

dJntColData_c l_debugColData[32] = {
    {1, 0, 0, 50.0f, &l_debugTmpPos[0]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[2]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[4]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[6]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[8]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[10]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[12]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[14]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[16]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[18]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[20]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[22]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[24]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[26]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[28]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[30]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[32]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[34]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[36]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[38]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[40]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[42]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[44]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[46]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[48]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[50]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[52]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[54]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[56]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[58]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[60]},
    {1, 0, -1, 50.0f, &l_debugTmpPos[62]},
};

void dJntCol_HIO_c::genMessage(JORMContext* mctx) {
    mctx->genButton("ファイルに保存", 0x4000001);
    mctx->genCheckBox("デバッグ表示", &field_0x5, 1);
    mctx->genSliderID("テーブル番号", 0x4000002, 0, 0, 0x1f);
    mctx->startComboBoxID("形", 0x4000003, field_0xc->field_0x0, 0, NULL, 0xffff, 0xffff, 0x100, 0x1a);
    mctx->genComboBoxItem("球", 0);
    mctx->genComboBoxItem("円柱", 1);
    mctx->genComboBoxItem("円盤", 2);
    mctx->endComboBox();
    mctx->startComboBoxID("矢属性", 0x4000004, field_0xc->mType, 0, NULL, 0xffff, 0xffff, 0x100, 0x1a);
    mctx->genComboBoxItem("消える", 0);
    mctx->genComboBoxItem("刺さる", 1);
    mctx->genComboBoxItem("貫通", 2);
    mctx->genComboBoxItem("跳ね返し", 3);
    mctx->endComboBox();
    mctx->genSliderID("ジョイント番号", 0x4000005, field_0xc->mJntNum, -1, 200);
    mctx->genSliderID("中心Ｘ", 0x4000006, field_0xc->field_0x8->x, -10000.0f, 10000.0f);
    mctx->genSliderID("中心Ｙ", 0x4000007, field_0xc->field_0x8->y, -10000.0f, 10000.0f);
    mctx->genSliderID("中心Ｚ", 0x4000008, field_0xc->field_0x8->z, -10000.0f, 10000.0f);
    mctx->genSliderID("次点Ｘ", 0x4000009, field_0xc->field_0x8[1].x, -10000.0f, 10000.0f);
    mctx->genSliderID("次点Ｙ", 0x400000a, field_0xc->field_0x8[1].y, -10000.0f, 10000.0f);
    mctx->genSliderID("次点Ｚ", 0x400000b, field_0xc->field_0x8[1].z, -10000.0f, 10000.0f);
    mctx->genSliderID("半径", 0x400000c, field_0xc->field_0x4, 0.009999999776482582f, 1000.0f);
}

void dJntCol_HIO_c::listenPropertyEvent(const JORPropertyEvent* property) {
    JORReflexible::listenPropertyEvent(property);
    const JORPropertyEvent* r29 = property;
    const f32* r30 = (f32*)&property->field_0x18;
    switch (u32(r29->id)) {
    case 0x4000003:
        field_0xc->field_0x0 = property->field_0x18.U32;
        break;
    case 0x4000004:
        field_0xc->mType = property->field_0x18.U32;
        break;
    case 0x4000005:
        field_0xc->mJntNum = property->field_0x18.U32;
        break;
    case 0x4000006:
        field_0xc->field_0x8[0].x = *r30;
        break;
    case 0x4000007:
        field_0xc->field_0x8[0].y = *r30;
        break;
    case 0x4000008:
        field_0xc->field_0x8[0].z = *r30;
        break;
    case 0x4000009:
        field_0xc->field_0x8[1].x = *r30;
        break;
    case 0x400000A:
        field_0xc->field_0x8[1].y = *r30;
        break;
    case 0x400000B:
        field_0xc->field_0x8[1].z = *r30;
        break;
    case 0x400000C:
        field_0xc->field_0x4 = *r30;
        break;
    case 0x4000002:
        field_0xc = &l_debugColData[property->field_0x18.U32];
        update();
        break;
    case 0x4000001:
        fileOut();
        break;
    }
}

void dJntCol_HIO_c::update() {
    JORMContext* mctx = attachJORMContext(8);
    mctx->startUpdateNode(this);
    mctx->updateComboBoxID(2, 0x4000003, field_0xc->field_0x0, 0);
    mctx->updateComboBoxID(2, 0x4000004, field_0xc->mType, 0);
    mctx->updateSliderID(2, 0x4000005, field_0xc->mJntNum, 0, 0, 0);
    mctx->updateSliderID(2, 0x4000006, field_0xc->field_0x8[0].x, 0.0, 0.0, 0);
    mctx->updateSliderID(2, 0x4000007, field_0xc->field_0x8[0].y, 0.0, 0.0, 0);
    mctx->updateSliderID(2, 0x4000008, field_0xc->field_0x8[0].z, 0.0, 0.0, 0);
    mctx->updateSliderID(2, 0x4000009, field_0xc->field_0x8[1].x, 0.0, 0.0, 0);
    mctx->updateSliderID(2, 0x400000a, field_0xc->field_0x8[1].y, 0.0, 0.0, 0);
    mctx->updateSliderID(2, 0x400000b, field_0xc->field_0x8[1].z, 0.0, 0.0, 0);
    mctx->updateSliderID(2, 0x400000c, field_0xc->field_0x4, 0.0, 0.0, 0);
    mctx->endUpdateNode();
    releaseJORMContext(mctx);
}

void dJntCol_HIO_c::fileOut() {
    char* buffer = new(0x20) char[0x10000];
    if (!buffer) {
        OSReport("書き込み用領域確保できませんでした\n");
        return;
    }
    buffer[0] = 0;
    JORFile file;
    int i;
    if (file.open(JORFile::EFlags_WRITE | JORFile::EFlags_UNK_0x4, "すべてのファイル(*.*)\0*.*\0", NULL, NULL, NULL)) {
        for (i = 0; i < ARRAY_SIZE(l_debugColData); i++) {
            if (l_debugColData[i].mJntNum >= 0) {
                if (l_debugColData[i].field_0x0 == 0) {
                    sprintf(buffer + strlen(buffer), "static Vec l_jntColPos%d = {\n", i);
                    sprintf(buffer + strlen(buffer), "\t%ff, %ff, %ff,\n};\n", l_debugColData[i].field_0x8->x, l_debugColData[i].field_0x8->y, l_debugColData[i].field_0x8->z);
                } else {
                    sprintf(buffer + strlen(buffer), "static Vec l_jntColPos%d[2] = {\n", i);
                    sprintf(buffer + strlen(buffer), "\t{%ff, %ff, %ff},\n", l_debugColData[i].field_0x8[0].x, l_debugColData[i].field_0x8[0].y, l_debugColData[i].field_0x8[0].z);
                    sprintf(buffer + strlen(buffer), "\t{%ff, %ff, %ff},\n};\n", l_debugColData[i].field_0x8[1].x, l_debugColData[i].field_0x8[1].y, l_debugColData[i].field_0x8[1].z);
                }
            }
        }
        sprintf(buffer + strlen(buffer), "static dJntColData_c l_jntColData[] = {\n");
        for (i = 0; i < ARRAY_SIZE(l_debugColData); i++) {
            if (l_debugColData[i].mJntNum >= 0) {
                sprintf(buffer + strlen(buffer), "\t{\n\t\t");
                if (l_debugColData[i].field_0x0 == 0) {
                    sprintf(buffer + strlen(buffer), "dJntCol_c::SHAPE_SPH_e, ");
                } else if (l_debugColData[i].field_0x0 == 1) {
                    sprintf(buffer + strlen(buffer), "dJntCol_c::SHAPE_CYL_e, ");
                } else {
                    sprintf(buffer + strlen(buffer), "dJntCol_c::SHAPE_CIRCLE_e, ");
                }
                if (l_debugColData[i].mType == 0) {
                    sprintf(buffer + strlen(buffer), "dJntCol_c::TYPE_NORMAL_e,\n");
                } else if (l_debugColData[i].mType == 1) {
                    sprintf(buffer + strlen(buffer), "dJntCol_c::TYPE_STOP_e,\n");
                } else if (l_debugColData[i].mType == 2) {
                    sprintf(buffer + strlen(buffer), "dJntCol_c::TYPE_THROUGH_e,\n");
                } else {
                    sprintf(buffer + strlen(buffer), "dJntCol_c::TYPE_SHIELD_e,\n");
                }
                sprintf(buffer + strlen(buffer), "\t\t%d, %ff, ", l_debugColData[i].mJntNum, l_debugColData[i].field_0x4);
                if (l_debugColData[i].field_0x0 == 0) {
                    sprintf(buffer + strlen(buffer), "&l_jntColPos%d\n", i);
                } else {
                    sprintf(buffer + strlen(buffer), "l_jntColPos%d\n", i);
                }
                sprintf(buffer + strlen(buffer), "\t},\n");
            }
        }
        sprintf(buffer + strlen(buffer), "};\n");
        file.writeData(buffer, strlen(buffer));
        if (strlen(buffer) > 0x10000) {
            OSReport("メモリ破壊が起こりました！！！！！\n");
        }
        file.close();
    } else {
        OSReport("書き込みファイルが開けません\n");
    }
    delete[] buffer;
}
#endif

dJntCol_c::dJntCol_c() {
    mModel = 0;
    mData = 0;
    field_0x8 = 0;
}

#if DEBUG
void dJntCol_setDebugHIO() {
    l_HIO.field_0x4 = mDoHIO_createChild("ジョイントコリジョン", &l_HIO);
}

void dJntCol_deleteDebugHIO() {
    mDoHIO_deleteChild(l_HIO.field_0x4);
}
#endif

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
    const dJntColData_c* r30 = mData;
    cM3dGSph sp210;
    cM3dGCyl sp1F8;
    cM3dGPla sp1E0;
    cXyz sp1B8;
    cXyz sp1AC;
    cXyz sp1A0;
    cXyz sp194;
    cXyz sp188;
    cXyz sp17C;
    cM3dGLin sp1C4;
    cXyz sp170;
    cXyz sp164;
    f32 f28;
    f32 f27;
    int sp1C;
    int sp18;
    int rv;
    f28 = FLT_MAX;
    sp18 = FALSE;
    rv = -1;
    f27 = -10000000.0f;
    for (int i = 0; i < field_0x8; i++, r30++) {
        if ((field_0xc & (1 << i)) == 0 &&
#if DEBUG
            r30->mJntNum >= 0 &&
#endif
            (param_4 == 0 || r30->mType == 3))
        {
            mDoMtx_stack_c::copy(mModel->getAnmMtx(r30->mJntNum));
            mDoMtx_stack_c::multVec(r30->field_0x8, &sp188);
            f32 cross;
            f32 len;
            f32 inprod;
            if (r30->field_0x0 == 0) {
                sp210.Set(sp188, r30->field_0x4);
                int sp10 = cM3d_Cross_LinSph_CrossPos(sp210, *param_1, &sp1A0,
                                                                &sp194);
                if (sp10 != 0) {
                    if (sp10 == 1) {
                        sp1B8 = sp1A0;
                    } else if (sp1A0.abs2(param_1->GetStart()) < sp194.abs2(param_1->GetStart())) {
                        sp1B8 = sp1A0;
                    } else {
                        sp1B8 = sp194;
                    }
                } else {
                    sp1AC = *param_2 - sp188;
                    sp1AC.normalizeZP();
                    sp1B8 = sp188 + sp1AC * r30->field_0x4;
                }
            } else if (r30->field_0x0 == 1) {
                mDoMtx_stack_c::multVec(r30->field_0x8 + 1, &sp17C);
                sp1AC = sp17C - sp188;
                if (sp1AC.absXZ() > 0.01f) {
                    mDoMtx_stack_c::transS(sp188);
                    mDoMtx_stack_c::XrotM(cM_atan2s(-sp1AC.absXZ(), sp1AC.y));
                    mDoMtx_stack_c::YrotM(cM_atan2s(-sp1AC.x, sp1AC.z));
                    mDoMtx_stack_c::transM(-sp188.x, -sp188.y, -sp188.z);
                    mDoMtx_stack_c::multVec(&sp17C, &sp194);
                    mDoMtx_stack_c::multVec(&param_1->GetStartP(), &sp170);
                    mDoMtx_stack_c::multVec(&param_1->GetEndP(), &sp164);
                    sp1C4.SetStartEnd(sp170, sp164);
                    sp1C = 1;
                } else {
                    sp194 = sp17C;
                    sp1C4.SetStartEnd(param_1->GetStart(), param_1->GetEnd());
                    sp1C = 0;
                }
                sp1F8.Set(sp188, r30->field_0x4,
                              sp194.y - sp188.y);
                if (cM3d_Cross_CylLin(&sp1F8, &sp1C4, &sp1A0, &sp194)) {
                    if (sp1C) {
                        mDoMtx_stack_c::inverse();
                        mDoMtx_stack_c::multVec(&sp1A0, &sp1B8);
                    } else {
                        sp1B8 = sp1A0;
                    }
                } else {
                    sp1C4.SetStartEnd(sp188, sp17C);
                    cross = cM3d_lineVsPosSuisenCross(&sp1C4, param_2, &sp1A0);
                    sp1AC =  *param_2 - sp1A0;
                    len = sp1AC.abs2();
                    if (len < 0.0001f) {
                        sp1A0 = sp1C4.GetStart() - sp1C4.GetEnd();
                        sp194 = param_1->GetStart() - param_1->GetEnd();
                        inprod = sp1A0.inprod(sp194);
                        if (inprod > 0.0f) {
                            sp1B8 = sp1C4.GetEnd();
                        } else if (inprod < 0.0f) {
                            sp1B8 = sp1C4.GetStart();
                        } else if (cross < 0.5f) {
                            sp1B8 = sp1C4.GetStart();
                        } else {
                            sp1B8 = sp1C4.GetEnd();
                        }
                    } else {
                        if (cross >= 0.0f && cross <= 1.0f) {
                            sp1B8 = sp1A0 + (sp1AC / JMAFastSqrt(len)) * r30->field_0x4;
                        } else {
                            sp1A0 = sp1C4.GetStart() - sp1C4.GetEnd();
                            sp194 = param_1->GetStart() - param_1->GetEnd();
                            inprod = sp1A0.inprod(sp194);
                            if (inprod > 0.0f) {
                                sp1A0 = sp1C4.GetEnd();
                            } else {
                                if (inprod < 0.0f) {
                                    sp1A0 = sp1C4.GetStart();
                                } else {
                                    if (cross < 0.5f) {
                                        sp1A0 = sp1C4.GetStart();
                                    } else {
                                        sp1A0 = sp1C4.GetEnd();
                                    }
                                }
                            }
                            sp1B8 = sp1A0 + (sp1AC / JMAFastSqrt(len)) * r30->field_0x4;
                        }
                    }
                }
            } else {
                mDoMtx_stack_c::multVecSR(r30->field_0x8 + 1, &sp1AC);
                sp1E0.SetupNP0(sp1AC, sp188);
                if (cM3d_Cross_LinPla(param_1, &sp1E0, &sp1B8, true, true)) {
                    if (!(sp188.abs(sp1B8) < r30->field_0x4)) {
                        sp1AC = sp1B8 - sp188;
                        sp1AC.normalizeZP();
                        sp1B8 = sp188 + sp1AC * r30->field_0x4;
                    }
                } else {
                    if (sp1E0.crossInfLin(*param_2, param_1->GetEnd(), sp1B8)) {
                        sp1AC = sp1B8 - sp188;
                        sp1AC.normalizeZP();
                        sp1B8 = sp188 + sp1AC * r30->field_0x4;
                    } else {
                        len = cM3d_SignedLenPlaAndPos(&sp1E0, param_2);
                        sp1AC.normalizeZP();
                        sp1B8 = *param_2 - sp1AC * len;
                    }
                }
            }
            cross = cM3d_lineVsPosSuisenCross(param_1, &sp1B8, &sp1A0);
            len = sp1B8.abs2(sp1A0);
            if (len < 9.999999747378752e-05f) {
                if (!sp18 || f27 < cross) {
                    f27 = cross;
                    *param_3 = sp1B8;
                    rv = i;
                    sp18 = TRUE;
                }
            } else {
                if (!sp18 && (f28 >= len)) {
                    f28 = len;
                    *param_3 = sp1B8;
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

#if DEBUG
void dJntCol_c::debugDraw() {
    static const GXColor color = {255, 0, 0, 128};

    if (!l_HIO.field_0x5) {
        return;
    }

    const dJntColData_c* r30 = mData;
    cXyz local_b4;
    cXyz local_c0;
    cXyz local_cc;
    for (int i = 0; i < field_0x8; i++, r30++) {
        if (field_0xc & 1 << i) {
            continue;
        }
        if (r30->mJntNum < 0) {
            continue;
        }
        mDoMtx_stack_c::copy(mModel->getAnmMtx(r30->mJntNum));
        mDoMtx_stack_c::multVec(r30->field_0x8, &local_b4);
        if (r30->field_0x0 == 0) {
            dDbVw_drawSphereXlu(local_b4, r30->field_0x4, color, 1);
            continue;
        } else if (r30->field_0x0 == 1) {
            mDoMtx_stack_c::multVec(&r30->field_0x8[1], &local_c0);
        } else {
            mDoMtx_stack_c::multVecSR(&r30->field_0x8[1], &local_c0);
            local_c0.normalizeZP();
            local_c0 += local_b4;
        }
        local_cc = local_c0 - local_b4;
        Mtx local_48;
        Mtx local_78;
        Mtx local_a8;
        cMtx_trans(local_78, local_b4.x, local_b4.y, local_b4.z);
        cM3d_UpMtx(local_cc, local_a8);
        cMtx_concat(local_78, local_a8, local_48);
        cMtx_scale(local_78, r30->field_0x4, local_cc.abs() * 0.5f, r30->field_0x4);
        cMtx_concat(local_48, local_78, local_48);
        cMtx_trans(local_78, 0.0f, 1.0f, 0.0f);
        cMtx_concat(local_48, local_78, local_48);
        cMtx_XrotS(local_78, 0x4000);
        cMtx_concat(local_48, local_78, local_48);
        dDbVw_drawCylinderMXlu(local_48, color, 1);
    }
}
#endif
