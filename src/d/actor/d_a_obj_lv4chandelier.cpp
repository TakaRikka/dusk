/**
 * @file d_a_obj_lv4chandelier.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4chandelier.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_obj_swchain.h"
#include "d/d_bg_w.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"

#if DEBUG
class daObjLv4Chan_HIO_c : public mDoHIO_entry_c {
public:
    daObjLv4Chan_HIO_c();
    void genMessage(JORMContext*);
};

daObjLv4Chan_HIO_c::daObjLv4Chan_HIO_c() {
}

void daObjLv4Chan_HIO_c::genMessage(JORMContext* ctx) {
}

static daObjLv4Chan_HIO_c l_HIO;
#endif

static void rideCallBack(dBgW* i_bgw, fopAc_ac_c* param_2, fopAc_ac_c* param_3) {
    ((daObjLv4Chan_c*)param_2)->rideActor(param_3);
}

static char* l_arcName = "P_Lv4Chan";

int daObjLv4Chan_c::create1st() {
    int rv = dComIfG_resLoad(this, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        for (int i = 0; i < 3; i++) {
            if (getArg0() == 0xf) {
                mMdlLengths[i] = 18;
            } else {
                mMdlLengths[i] = 35;
            }
        }

        if (getAddChain0() != 0xf) {
            mAddChain0 = getAddChain0();
            mMdlLengths[0] += mAddChain0;
        } else {
            mAddChain0 = 0;
        }

        field_0x2484 = (mMdlLengths[0] - mAddChain0) - 17;
        if (getAddChain1() != 0xf) {
            mMdlLengths[1] += getAddChain1();
        }

        if (getAddChain2() != 0xf) {
            mMdlLengths[2] += getAddChain2();
        }

        for (int i = 0; i < 3; i++) {
            mChainLengths[i] = mMdlLengths[i] + 1;
        }

        rv = MoveBGCreate(l_arcName, 8, dBgS_MoveBGProc_TypicalRotY, 0xf500, NULL);
        if (rv == cPhs_ERROR_e) {
            return rv;
        }

        #if DEBUG
        // “Chandelier (Lv4)”
        l_HIO.entryHIO("シャンデリア(Lv4)");
        #endif
    }
    return rv;
}

static u32 const l_bmdidx[2] = {
    4, 5,
};

void daObjLv4Chan_c::setMtxChain(daObjLv4Chan_c::ChainPos* i_chains, dMdl_obj_c* i_mdls,
                                     int param_3) {
    s16 sVar8 = 0;
    for (int i = 0; i < param_3; i++) {
        Mtx afStack_70;
        cXyz cStack_7c = i_chains[i + 1].field_0xc - i_chains[i].field_0xc;
        cXyz cStack_88;
        VECCrossProduct(&::cXyz::BaseZ, &cStack_7c, &cStack_88);
        if (!cM3d_IsZero(cStack_7c.getSquareMag())) {
            cStack_7c.normalize();
            f32 dVar10 = VECDotProduct(&cStack_7c, &::cXyz::BaseZ);
            if (!cM3d_IsZero(cStack_88.getSquareMag()) && -1.0f <= dVar10 && dVar10 <= 1.0f) {
                cStack_88.normalize();
                f32 fVar11 = acosf(dVar10);
                i_chains[i].field_0x24 = fVar11 - M_PI / 2;
                MTXRotAxisRad(afStack_70, &cStack_88, fVar11);
            } else {
                MTXIdentity(afStack_70);
            }
        } else {
            MTXIdentity(afStack_70);
        }
        sVar8 += 0x4000;
        mDoMtx_stack_c::transS(i_chains[i].field_0xc);
        mDoMtx_stack_c::concat(afStack_70);
        mDoMtx_stack_c::ZrotM(sVar8);
        i_mdls[i].setMtx(mDoMtx_stack_c::get());
    }
}

void daObjLv4Chan_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    MTXCopy(mDoMtx_stack_c::get(), field_0x5d8);
    setMtxChain(mChains[0], mMdls[0],
                mMdlLengths[0]);
    setMtxChain(mChains[1], mMdls[1],
                mMdlLengths[1]);
    setMtxChain(mChains[2], mMdls[2],
                mMdlLengths[2]);
    Mtx afStack_68;
    cXyz local_78;
    cM3d_VectorProduct(&mChains[0][mMdlLengths[0]].field_0xc, &mChains[1][mMdlLengths[1]].field_0xc,
                       &mChains[2][mMdlLengths[2]].field_0xc, &local_78);
    if (!cM3d_IsZero(local_78.getSquareMag())) {
        local_78.normalize();
        cXyz cStack_84;
        VECCrossProduct(&::cXyz::BaseY, &local_78, &cStack_84);
        f32 dVar9 = VECDotProduct(&local_78, &::cXyz::BaseY);
        if (!cM3d_IsZero(cStack_84.getSquareMag()) && -1.0f <= dVar9 && dVar9 <= 1.0f) {
            cStack_84.normalize();
            MTXRotAxisRad(afStack_68, &cStack_84, acosf(dVar9));
            local_78 *= -150.0f;
        } else {
            MTXIdentity(afStack_68);
            local_78.x = 0.0f;
            local_78.y = -150.0f;
            local_78.z = 0.0f;
        }
    } else {
        MTXIdentity(afStack_68);
        local_78.x = 0.0f;
        local_78.y = -150.0f;
        local_78.z = 0.0f;
    }

    field_0x23bc = mChains[0][mMdlLengths[0]].field_0xc * (1 / 3.0f) +
                   mChains[1][mMdlLengths[1]].field_0xc * (1 / 3.0f) +
                   mChains[2][mMdlLengths[2]].field_0xc * (1 / 3.0f);
    field_0x23bc += local_78;
    mDoMtx_stack_c::transS(field_0x23bc);
    mDoMtx_stack_c::concat(afStack_68);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);
    MTXCopy(mDoMtx_stack_c::get(), field_0x5a8);
}

void daObjLv4Chan_c::rideActor(fopAc_ac_c* param_1) {
    field_0x2498 = 1;
    f32 dVar8 = 1.0f;
    if (field_0x23d4 >= 8) {
        if (cM_rndF(1.0f) > 0.01f) {
            return;
        }
        dVar8 = cM_rndF(0.1f);
    }

    field_0x23d8[field_0x23d4] = param_1;
    field_0x23d4++;
    bool bVar3 = false;
    for (int i = 0; i < 8; i++) {
        if (param_1 == field_0x23f8[i]) {
            bVar3 = true;
        }
    }

    if (bVar3) {
        if (cM_rndF(1.0f) > 0.01f) {
            if (cM3d_IsZero(param_1->speed.getSquareMag())) {
                return;
            }
        }
        dVar8 = cM_rndF(0.1f);
    }

    cXyz local_3c;
    local_3c = param_1->current.pos - mChains[0][mChainLengths[0] - 1].field_0xc;
    mChains[0][mChainLengths[0] - 1].field_0x0.x -= dVar8 * (local_3c.x * 0.0075f * (cM_rndFX(0.025f) + 1.0f));
    mChains[0][mChainLengths[0] - 1].field_0x0.z -= dVar8 * (local_3c.z * 0.0075f * (cM_rndFX(0.025f) + 1.0f));
    cXyz local_48;
    local_48 = param_1->current.pos - mChains[1][mChainLengths[1] - 1].field_0xc;
    mChains[1][mChainLengths[1] - 1].field_0x0.x -= dVar8 * (local_48.x * 0.0075f * (cM_rndFX(0.025f) + 1.0f));
    mChains[1][mChainLengths[1] - 1].field_0x0.z -= dVar8 * (local_48.z * 0.0075f * (cM_rndFX(0.025f) + 1.0f));
    cXyz local_54;
    local_54 = param_1->current.pos - mChains[2][mChainLengths[2] - 1].field_0xc;
    mChains[2][mChainLengths[2] - 1].field_0x0.x -= dVar8 * (local_54.x * 0.0075f * (cM_rndFX(0.025f) + 1.0f));
    mChains[2][mChainLengths[2] - 1].field_0x0.z -= dVar8 * (local_54.z * 0.0075f * (cM_rndFX(0.025f) + 1.0f));
}

int daObjLv4Chan_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_bmdidx[0]);
    JUT_ASSERT(478, model_data != NULL);
    mModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mModel == NULL) {
        return 0;
    }

    mChains[0] = new ChainPos[mChainLengths[0]];
    if (mChains[0] == NULL) {
        return 0;
    }

    mChains[1] = new ChainPos[mChainLengths[1]];
    if (mChains[1] == NULL) {
        return 0;
    }

    mChains[2] = new ChainPos[mChainLengths[2]];
    if (mChains[2] == NULL) {
        return 0;
    }
   
    mChainModelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_bmdidx[1]);
    JUT_ASSERT(500, mChainModelData != NULL);

    mMdls[0] = new dMdl_obj_c[mMdlLengths[0]];
    if (mMdls[0] == NULL) {
        return 0;
    }

    mMdls[1] = new dMdl_obj_c[mMdlLengths[1]];
    if (mMdls[1] == NULL) {
        return 0;
    }

    mMdls[2] = new dMdl_obj_c[mMdlLengths[2]];
    if (mMdls[2] == NULL) {
        return 0;
    }

    return 1;
}

static void* searchSwChain(void* i_actor, void* i_this) {
    if (i_actor != NULL && fopAcM_IsActor(i_actor) &&
        fopAcM_GetProfName(i_actor) == PROC_Obj_SwChain)
    {
        if (((daObjSwChain_c*)i_actor)->getChainID() == ((daObjLv4Chan_c*)i_this)->getArg0()) {
            return i_actor;
        }
    }
    return NULL;
}

static dCcD_SrcSph cc_sph_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x11}, 0x79}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x5, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

int daObjLv4Chan_c::Create() {
    mModel->setBaseScale(scale);
    field_0x23d4 = 0;
    for (int i = 0; i < 8; i++) {
        field_0x23d8[i] = NULL;
        field_0x23f8[i] = NULL;
    }

    field_0x2418 = 1;
    field_0x241c = 0;

    for (int i = 0; i < field_0x2418; i = i + 1) {
        mChains[0][i].field_0xc = current.pos;
        mChains[0][i].field_0xc.x += -163.12f;
        mChains[0][i].field_0xc.z += -282.54f;
        mChains[0][i].field_0x0 = cXyz::Zero;
        mChains[0][i].field_0x24 = 0;
        mChains[1][i].field_0xc = current.pos;
        mChains[1][i].field_0xc.x += -163.12f;
        mChains[1][i].field_0xc.z += 282.54f;
        mChains[1][i].field_0x0 = cXyz::Zero;
        mChains[1][i].field_0x24 = 0;
        mChains[2][i].field_0xc = current.pos;
        mChains[2][i].field_0xc.x += 326.25f;
        mChains[2][i].field_0x0 = cXyz::Zero;
        mChains[2][i].field_0x24 = 0;
    }

    for (int i = field_0x2418; i < mChainLengths[0]; i++) {
        mChains[0][i].field_0x0 = cXyz::Zero;
        mChains[0][i].field_0xc.x = mChains[0][0].field_0xc.x;
        mChains[0][i].field_0xc.y = mChains[0][i - 1].field_0xc.y - 35.0f;
        mChains[0][i].field_0xc.z = mChains[0][0].field_0xc.z;
        mChains[0][i].field_0x24 = 0;
    }
    
    for (int i = field_0x2418; i < mChainLengths[1]; i++) {
        mChains[1][i].field_0x0 = cXyz::Zero;
        mChains[1][i].field_0xc.x = mChains[1][0].field_0xc.x;
        mChains[1][i].field_0xc.y = mChains[1][i - 1].field_0xc.y - 35.0f;
        mChains[1][i].field_0xc.z = mChains[1][0].field_0xc.z;
        mChains[1][i].field_0x24 = 0;
    }

    for (int i = field_0x2418; i < mChainLengths[2]; i++) {
        mChains[2][i].field_0x0 = cXyz::Zero;
        mChains[2][i].field_0xc.x = mChains[2][0].field_0xc.x;
        mChains[2][i].field_0xc.y = mChains[2][i - 1].field_0xc.y - 35.0f;
        mChains[2][i].field_0xc.z = mChains[2][0].field_0xc.z;
        mChains[2][i].field_0x24 = 0;
    }

    setMtx();
    field_0x23c8 = field_0x23bc;
    fopAcM_SetMtx(this, field_0x5d8);
    chkGnd();
    fopAcM_setCullSizeBox(this, -800.0f, (-mMdlLengths[0] * 40.0f) - 1700.0f, -600.0f, 800.0f, 10.0f,
                          600.0f);
    mpBgW->SetRideCallback(rideCallBack);
    mStts.Init(0xff, 0, this);
    for (int i = 0; i < 8; i = i + 1) {
        mSphs1[i].Set(cc_sph_src);
        mSphs1[i].SetStts(&mStts);
        mSphs2[i].Set(cc_sph_src);
        mSphs2[i].SetStts(&mStts);
        mSphs3[i].Set(cc_sph_src);
        mSphs3[i].SetStts(&mStts);
    }

    field_0x2488 = 0;
    field_0x2489 = 1;
    field_0x248a = 0;
    field_0x248b = 0;
    field_0x248c = 0;
    field_0x248d = 0;
    field_0x2490 = 1.0f;
    field_0x2494 = 0.0f;
    field_0x249c = 1.0f;
    field_0x24a0 = 0.0f;
    field_0x2498 = 0;
    field_0x2499 = 0;
    return 1;
}

void daObjLv4Chan_c::constraintChain(daObjLv4Chan_c::ChainPos* i_chains, int param_2) {
    bool iVar4 = field_0x2418 == field_0x2484;
    for (int i = field_0x2418; i < param_2; i++) {
        cXyz local_3c = i_chains[i].field_0xc - i_chains[i - 1].field_0xc;
        if (local_3c.y >= -0.7071f) {
            local_3c.y = -0.7071f;
        }

        if (!iVar4) {
            local_3c.z = 0.0f;
            local_3c.x = 0.0f;
        }

        f32 uVar6 = local_3c.getSquareMag();
        if (i == field_0x2418) {
            if (!cM3d_IsZero(uVar6)) {
                if (uVar6 >= 1225.0f) {
                    local_3c.normalize();
                    local_3c *= 35.0f;
                }
            } else {
                local_3c.x = 0.0f;
                local_3c.y = -1.0f;
                local_3c.z = 0.0f;
            }
        } else {
            if (!cM3d_IsZero(uVar6)) {
                local_3c.normalize();
            } else {
                local_3c.x = 0.0f;
                local_3c.y = -1.0f;
                local_3c.z = 0.0f;
            }
            local_3c *= 35.0f;
        }

        i_chains[i].field_0xc = local_3c + i_chains[i - 1].field_0xc;
    }
}

static Vec const l_pos0 = {
    -163.12f, 150.0f, -282.54f,
};

static Vec const l_pos1 = {
    -163.12f, 150.0f, 282.54f,
};

static Vec const l_pos2 = {
    326.25f, 150.0f, 0.0f,
};

void daObjLv4Chan_c::constraintBase() {
    cXyz acStack_20;
    MTXMultVec(field_0x5a8, &l_pos0, &acStack_20);
    mChains[0][mChainLengths[0] - 1].field_0xc = (acStack_20 + mChains[0][mChainLengths[0] - 1].field_0xc) * 0.5f;
    MTXMultVec(field_0x5a8, &l_pos1, &acStack_20);
    mChains[1][mChainLengths[1] - 1].field_0xc = (acStack_20 + mChains[1][mChainLengths[1] - 1].field_0xc) * 0.5f;
    MTXMultVec(field_0x5a8, &l_pos2, &acStack_20);
    mChains[2][mChainLengths[2] - 1].field_0xc = (acStack_20 + mChains[2][mChainLengths[2] - 1].field_0xc) * 0.5f;
}

void daObjLv4Chan_c::calcVec(daObjLv4Chan_c::ChainPos* i_chains, int param_2) {
    for (int i = 0; i < param_2; i++) {
        i_chains[i].field_0x0 = i_chains[i].field_0xc - i_chains[i].field_0x18;
    }
}

void daObjLv4Chan_c::chkGnd() {
    cXyz cStack_18;
    cStack_18 = field_0x23bc;
    cStack_18.y += 300.0f;
    mGndChk.SetPos(&cStack_18);
    mGndChk.SetActorPid(base.base.id);
    field_0x247c = dComIfG_Bgsp().GroundCross(&mGndChk);
}

static int const l_curve[10] = {
    0, 0, 0, 0, 0, 1, 1,
    2, 2, 4,
};

int daObjLv4Chan_c::Execute(Mtx** i_mtx) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    int local_1f0 = 0;
    field_0x2420 = 0;
    if (getArg0() != 0xf) {
        daObjSwChain_c* swChain = (daObjSwChain_c*)fopAcM_Search(
                                searchSwChain, this);
        if ((swChain != NULL)) {
            local_1f0 = swChain->checkTight();
            if (local_1f0) {
                field_0x248b = 0;
                if (field_0x2418 < field_0x2484) {
                    if (player->getBaseAnimeFrame() < 10.0f) {
                        int local_1f8 = player->getBaseAnimeFrame();
                        if (local_1f8 < 0) {
                            local_1f8 = 0;
                        }

                        if (9 < local_1f8) {
                            local_1f8 = 9;
                        }

                        field_0x241c += l_curve[local_1f8];
                        if (field_0x241c >= 10) {
                            field_0x241c = field_0x241c - 10;
                            if (field_0x2418 == 1 && getSw() != 0xff) {
                                fopAcM_onSwitch(this, getSw());
                            }
                            field_0x2418 = field_0x2418 + 1;
                            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CHANDELIER_UP, &field_0x23bc, 0, 0,
                                                     1.0f, 1.0f, -1.0f, -1.0f, 0);
                        }
                    }
                } else {
                    if (field_0x2489 != 0) {
                        ;
                        dComIfGp_getVibration().StartShock(3, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                        mDoAud_seStart(Z2SE_OBJ_CHAND_TOP, &field_0x23bc, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
                        field_0x2489 = 0;
                    }
                    field_0x241c = 0;
                }
                
                if (field_0x241c > 10) {
                    field_0x241c = 10 - 1;
                }

                field_0x2420 = (f32)(field_0x241c - 2) / (f32)(10 - 2);
                if (field_0x2420 < 0.0f) {
                    field_0x2420 = 0.0f;
                }

                if (field_0x2418 > 0.7f * field_0x2484) {
                    field_0x2488 = 1;
                }

                if (field_0x2488 == 1) {
                    field_0x248a = 0;
                } else {
                    field_0x248a = 1;
                }
            } else {
                if (!swChain->checkCarry()) {
                    if (field_0x2418 == field_0x2484) {
                        if (field_0x248b == 0) {
                            field_0x248b = 1;
                            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CHANDELIER_DW_TRG, &field_0x23bc, 0,
                                                     0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                        }
                        field_0x241c++;
                        if (field_0x241c < 120) {
                            field_0x2420 = 0;
                        } else {
                            field_0x2420 = (f32)(field_0x241c - 120) / (f32)3;
                            if (field_0x241c - 120 > 3) {
                                field_0x241c = 0;
                                field_0x2418--;
                                field_0x2420 = 0;
                            }

                            Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_CHANDELIER_DW_LV, &field_0x23bc,
                                                          0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                        }
                    } else if (field_0x2418 > 1) {
                        field_0x241c++;
                        if (field_0x241c >= 3) {
                            field_0x241c = 0;
                            field_0x2418--;
                        }

                        field_0x2420 = (f32)field_0x241c / (f32)3;
                        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_CHANDELIER_DW_LV, &field_0x23bc, 0,
                                                      0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                    }
                    if (field_0x2418 == 1 && field_0x248a != 0) {
                        field_0x248a = 0;
                        dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                        mDoAud_seStart(Z2SE_OBJ_CHAND_FALLDOWN_S, &field_0x23bc, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
                    }
                }
            }

            if (field_0x2418 == 1 && field_0x2488 != 0) {
                field_0x2488 = 0;
                dComIfGp_getVibration().StartShock(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                mDoAud_seStart(Z2SE_OBJ_CHAND_FALLDOWN, &field_0x23bc, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            }

            if (field_0x2418 <= 1) {
                field_0x2418 = 1;
                field_0x2489 = 1;
            }

            if (field_0x2418 > field_0x2484) {
                field_0x2418 = field_0x2484;
            }
            swChain->setRatioForChandelier((f32)(field_0x2484 - field_0x2418) / (f32)field_0x2484);
        }
    }

    if (field_0x2420 > 1.0f) {
        field_0x2420 = 1.0f;
    }

    f32 dVar19 = field_0x2420 * 35.0f;
    if (local_1f0 == 0) {
        dVar19 *= -1.0f;
    }
    for (int i = 0; i < 8; i++) {
        field_0x23f8[i] = field_0x23d8[i];
        field_0x23d8[i] = NULL;
    }
    field_0x23d4 = 0;
    if (cM_rndF(1.0f) < 0.05f) {
        cXyz cStack_8c = dKyw_get_wind_vecpow();
        cStack_8c *= cM_rndF(0.4f);
        for (int i = field_0x2418; i < mChainLengths[0]; i++) {
            mChains[0][i].field_0x0 += cStack_8c;
        }
        for (int i = field_0x2418; i < mChainLengths[1]; i++) {
            mChains[1][i].field_0x0 += cStack_8c;
        }
        for (int i = field_0x2418; i < mChainLengths[2]; i++) {
            mChains[2][i].field_0x0 += cStack_8c;
        }

        cStack_8c *= cM_rndF(6.0f);
        mChains[0][mChainLengths[0] - 1].field_0x0 += cStack_8c;
        mChains[1][mChainLengths[1] - 1].field_0x0 += cStack_8c;
        mChains[2][mChainLengths[2] - 1].field_0x0 += cStack_8c;
    }
    
    bool bVar5 = false;
    for (int i = 0; i < 8; i++) {
        int local_210 = mChainLengths[0] - 1 - 2 * i;
        if (local_210 >= 0 && mSphs1[i].ChkTgHit()) {
            cCcD_Obj* hitObj = mSphs1[i].GetTgHitObj();
            fopAc_ac_c* hitActor = dCc_GetAc(hitObj->GetAc());
            cXyz cStack_98;
            cStack_98 = mChains[0][local_210].field_0xc - hitActor->current.pos;
            if (!cM3d_IsZero(cStack_98.getSquareMag())) {
                cStack_98.normalize();
                cStack_98 *= 15.0f;
                mChains[0][local_210].field_0x0 += cStack_98;
            }

            if (!bVar5) {
                bVar5 = true;
                u8 atSe = ((dCcD_GObjInf*)hitObj)->GetAtSe();
                Z2GetAudioMgr()->seStart(dCcD_GObjInf::getHitSeID(atSe, 0), mSphs1[i].GetCP(), 0x35, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
        }

        int local_224 = mChainLengths[1] - 1 - 2 * i;
        if (local_224 >= 0 && mSphs2[i].ChkTgHit()) {
            cCcD_Obj* hitObj = mSphs2[i].GetTgHitObj();
            fopAc_ac_c* hitActor = dCc_GetAc(hitObj->GetAc());
            cXyz cStack_a4;
            cStack_a4 = mChains[1][local_224].field_0xc - hitActor->current.pos;
            if (!cM3d_IsZero(cStack_a4.getSquareMag())) {
                cStack_a4.normalize();
                cStack_a4 *= 15.0f;
                mChains[1][local_224].field_0x0 += cStack_a4;
            }

            if (!bVar5) {
                bVar5 = true;
                u8 atSe = ((dCcD_GObjInf*)hitObj)->GetAtSe();
                Z2GetAudioMgr()->seStart(dCcD_GObjInf::getHitSeID(atSe, 0), mSphs2[i].GetCP(), 0x35, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
        }

        int local_238 = mChainLengths[2] - 1 - 2 * i;
        if (local_238 >= 0 && mSphs3[i].ChkTgHit()) {
            cCcD_Obj* hitObj = mSphs3[i].GetTgHitObj();
            fopAc_ac_c* hitActor = dCc_GetAc(hitObj->GetAc());
            cXyz cStack_b0;
            cStack_b0 = mChains[2][local_238].field_0xc - hitActor->current.pos;
            if (!cM3d_IsZero(cStack_b0.getSquareMag())) {
                cStack_b0.normalize();
                cStack_b0 *= 15.0f;
                mChains[2][local_238].field_0x0 += cStack_b0;
            }

            if (!bVar5) {
                bVar5 = true;
                u8 atSe = ((dCcD_GObjInf*)hitObj)->GetAtSe();
                Z2GetAudioMgr()->seStart(dCcD_GObjInf::getHitSeID(atSe, 0), mSphs3[i].GetCP(), 0x35, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
        }
    }

    for (int i = field_0x2418; i < mChainLengths[0]; i++) {
        mChains[0][i].field_0x18 = mChains[0][i].field_0xc;
        mChains[0][i].field_0x0.y += -4.0f;
        mChains[0][i].field_0xc += mChains[0][i].field_0x0;
    }

    for (int i = field_0x2418; i < mChainLengths[1]; i++) {
        mChains[1][i].field_0x18 = mChains[1][i].field_0xc;
        mChains[1][i].field_0x0.y += -4.0f;
        mChains[1][i].field_0xc += mChains[1][i].field_0x0;
    }

    for (int i = field_0x2418; i < mChainLengths[2]; i++) {
        mChains[2][i].field_0x18 = mChains[2][i].field_0xc;
        mChains[2][i].field_0x0.y += -4.0f;
        mChains[2][i].field_0xc += mChains[2][i].field_0x0;
    }

    for (int i = 0; i < 2; i++) {
        for (int j = field_0x2418; j < mChainLengths[0] - 1; j++) {
            if (j != mChainLengths[0] - 1) {
                mChains[0][j].field_0xc = (mChains[0][j - 1].field_0xc + mChains[0][j].field_0xc + mChains[0][j + 1].field_0xc) * (1.0f / 3.0f);
            } else {
                mChains[0][j].field_0xc = (mChains[0][j - 1].field_0xc + mChains[0][j].field_0xc) * 0.5f;
            }
        }

        for (int j = field_0x2418; j < mChainLengths[1] - 1; j++) {
            if (j != mChainLengths[1] - 1) {
                mChains[1][j].field_0xc = (mChains[1][j - 1].field_0xc + mChains[1][j].field_0xc + mChains[1][j + 1].field_0xc) * (1.0f / 3.0f);
            } else {
                mChains[1][j].field_0xc = (mChains[1][j - 1].field_0xc + mChains[1][j].field_0xc) * 0.5f;
            }
        }

        for (int j = field_0x2418; j < mChainLengths[2] - 1; j++) {
            if (j != mChainLengths[2] - 1) {
                mChains[2][j].field_0xc = (mChains[2][j - 1].field_0xc + mChains[2][j].field_0xc + mChains[2][j + 1].field_0xc) * (1.0f / 3.0f);
            } else {
                mChains[2][j].field_0xc = (mChains[2][j - 1].field_0xc + mChains[2][j].field_0xc) * 0.5f;
            }
        }

        setMtx();
        constraintBase();

        for (int j = 0; j < field_0x2418; j++) {
            mChains[0][j].field_0xc = current.pos;
            mChains[0][j].field_0xc.x += -163.12f;
            mChains[0][j].field_0xc.y += dVar19;
            mChains[0][j].field_0xc.z += -282.54f;
            mChains[0][j].field_0x0 = ::cXyz::Zero;

            mChains[1][j].field_0xc = current.pos;
            mChains[1][j].field_0xc.x += -163.12f;
            mChains[1][j].field_0xc.y += dVar19;
            mChains[1][j].field_0xc.z += 282.54f;
            mChains[1][j].field_0x0 = ::cXyz::Zero;

            mChains[2][j].field_0xc = current.pos;
            mChains[2][j].field_0xc.x += 326.25f;
            mChains[2][j].field_0xc.y += dVar19;
            mChains[2][j].field_0x0 = ::cXyz::Zero;
        }
        constraintChain(mChains[0], mChainLengths[0]);
        constraintChain(mChains[1], mChainLengths[1]);
        constraintChain(mChains[2], mChainLengths[2]);
    }

    calcVec(mChains[0], mChainLengths[0]);
    calcVec(mChains[1], mChainLengths[1]);
    calcVec(mChains[2], mChainLengths[2]);
    setMtx();

    u8 local_2a8 = 0;
    for (int i = 0; i < mChainLengths[0]; i++) {
        if (!cM3d_IsZero(mChains[0][i].field_0x24)) {
            local_2a8 = 1;
        }
    }

    for (int i = 0; i < mChainLengths[1]; i++) {
        if (!cM3d_IsZero(mChains[1][i].field_0x24)) {
            local_2a8 = 1;
        }
    }

    for (int i = 0; i < mChainLengths[2]; i++) {
        if (!cM3d_IsZero(mChains[2][i].field_0x24)) {
            local_2a8 = 1;
        }
    }

    field_0x23c8.abs2XZ(field_0x23bc);
    cXyz local_bc = field_0x23bc - field_0x23c8;
    local_bc.y = 0.0f;
    if (!cM3d_IsZero(local_bc.getSquareMag())) {
        local_bc.normalize();
        f32 dVar20 = local_bc.x * field_0x2490 + local_bc.z * field_0x2494;
        f32 dVar19 = local_bc.x * field_0x249c + local_bc.z * field_0x24a0;
        if (field_0x248c == 0 && ((local_2a8 == 1 && field_0x248d == 0) ||
                                  ((local_2a8 == 1 &&
                                    (dVar20 < -0.866f || cM_rndF(1.0f) < 0.003f || dVar19 <= 0.0f ||
                                     (field_0x2499 == 0 && field_0x2498 != 0)) &&
                                    cM_rndF(1.0f) < 0.5f))))
        {
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CHANDELIER_SWING, &field_0x23bc, 0, 0, 1.0f, 1.0f,
                                     -1.0f, -1.0f, 0);
            field_0x2490 = local_bc.x;
            field_0x2494 = local_bc.z;
            field_0x248c = 1;
        } else {
            field_0x248c = 0;
        }

        field_0x249c = local_bc.x;
        field_0x24a0 = local_bc.z;
    } else {
        field_0x248c = 0;
    }
    field_0x23c8 = field_0x23bc;
    field_0x248d = local_2a8;
    field_0x2499 = field_0x2498;
    field_0x2498 = 0;
    for (int i = 0; i < 8; i++) {
        int local_27c = mChainLengths[0] - 1 - 2 * i;
        if (local_27c >= 0) {
            mSphs1[i].SetC(mChains[0][local_27c].field_0xc);
            dComIfG_Ccsp()->Set(&mSphs1[i]);
        }

        int local_280 = mChainLengths[1] - 1 - 2 * i;
        if (local_280 >= 0) {
            mSphs2[i].SetC(mChains[1][local_280].field_0xc);
            dComIfG_Ccsp()->Set(&mSphs2[i]);
        }

        int local_284 = mChainLengths[2] - 1 - 2 * i;
        if (local_284 >= 0) {
            mSphs3[i].SetC(mChains[2][local_284].field_0xc);
            dComIfG_Ccsp()->Set(&mSphs3[i]);
        }
    }
    mStts.Move();
    *i_mtx = &field_0x5a8;
    chkGnd();
    return 1;
}

int daObjLv4Chan_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel,
                                          &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    dMdl_c* mdl = dMdl_mng_c::entry(mChainModelData, 0, current.roomNo);
    if (mdl != NULL) {
        for (int i = 0; i < mMdlLengths[0]; i++) {
            mdl->entryObj(&mMdls[0][i]);
        }

        for (int i = 0; i < mMdlLengths[1]; i++) {
            mdl->entryObj(&mMdls[1][i]);
        }

        for (int i = 0; i < mMdlLengths[2]; i++) {
            mdl->entryObj(&mMdls[2][i]);
        }
    }
    dComIfGd_setList();
    if (field_0x247c != -G_CM3D_F_INF) {
        f32 fVar4 = 0.4f + ((field_0x23bc.y - field_0x247c) * -0.2f) / 500.0f;
        if (fVar4 > 0.4f) {
            fVar4 = 0.4f;
        }

        if (fVar4 < 0.2f) {
            fVar4 = 0.2f;
        }

        tevStr.mLightPosWorld.x = field_0x23bc.x;
        tevStr.mLightPosWorld.y = field_0x247c + 2000.0f;
        tevStr.mLightPosWorld.z = field_0x23bc.z;
        tevStr.field_0x344 = fVar4;
        mShadowId = dComIfGd_setShadow(
            mShadowId, 0, mModel, &field_0x23bc,
            1600.0f, 0.0f, field_0x23bc.y, field_0x247c + 160.0f,
            mGndChk, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }
    return 1;
}

int daObjLv4Chan_c::Delete() {
    dComIfG_resDelete(this, l_arcName);

    #if DEBUG
    l_HIO.removeHIO();
    #endif

    return 1;
}

static int daObjLv4Chan_create1st(daObjLv4Chan_c* i_this) {
    fopAcM_ct(i_this, daObjLv4Chan_c);
    return i_this->create1st();
}

static int daObjLv4Chan_MoveBGDelete(daObjLv4Chan_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjLv4Chan_MoveBGExecute(daObjLv4Chan_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjLv4Chan_MoveBGDraw(daObjLv4Chan_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjLv4Chan_METHODS = {
    (process_method_func)daObjLv4Chan_create1st,
    (process_method_func)daObjLv4Chan_MoveBGDelete,
    (process_method_func)daObjLv4Chan_MoveBGExecute,
    NULL,
    (process_method_func)daObjLv4Chan_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_Lv4Chan = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Lv4Chan,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjLv4Chan_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  674,                    // mPriority
  &daObjLv4Chan_METHODS,  // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
