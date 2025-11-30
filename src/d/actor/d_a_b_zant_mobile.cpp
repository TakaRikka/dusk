/**
 * @file d_a_b_zant_mobile.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_zant_mobile.h"
#include "d/actor/d_a_b_zant.h"
#include "d/d_com_inf_game.h"
#include "d/d_cc_uty.h"
#include "d/actor/d_a_player.h"
#include "SSystem/SComponent/c_math.h"
#include <cmath.h>

namespace {
dCcD_SrcSph cc_zant_src = {
    {
        {0x0, {{0x0, 0x1, 0x0}, {0xD8FBFDFF, 0x3}, 0x55}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x1, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x303}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

static dCcD_SrcCyl cc_zant_snort_src = {
    {
        {0, {{0x400, 0, 0}, {0xD8FBFDFF, 0}, 0x55}},
        {dCcD_SE_METAL, 0, 1, 0, {0}},
        {dCcD_SE_NONE, 0, 0, 0, {2}},
        {0},
    },
    {
        {0.0f, 0.0f, 0.0f},
        40.0f,
        40.0f,
    },
};
}

daB_ZANTZ_HIO_c::daB_ZANTZ_HIO_c() {
    field_0x4 = -1;
    mModelSize = 1.0f;
}

int daB_ZANTZ_c::draw() {
    J3DModel* model = mpMorf->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    mpBrk->entry(model->getModelData());
    mpMorf->entryDL();
    return 1;
}

static int daB_ZANTZ_Draw(daB_ZANTZ_c* i_this) {
    return i_this->draw();
}

void daB_ZANTZ_c::setSnortEffect(int param_0) {
    if (field_0x668 == 0) {
        Z2GetAudioMgr()->seStart(Z2SE_EN_ZZ_HANAIKI, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }

    field_0x668 = param_0;
}

void daB_ZANTZ_c::calcSnortEffect() {
    cXyz sp28;
    cXyz sp34;
    cXyz sp40(5.0f, 1.5f, 5.0f);
    csXyz sp48;

    sp34.set(150.0f, 800.0f, 1000.0f);
    cLib_offsetPos(&sp28, &current.pos, shape_angle.y, &sp34);
    sp48.set(0, 0, 0x7C00);

    field_0xd04 = dComIfGp_particle_set(field_0xd04, 0x8908, &current.pos, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
    sp28.y -= 900.0f;

    mSnortColliders[0].SetC(sp28);
    mSnortColliders[0].SetR(300.0f);
    mSnortColliders[0].SetH(900.0f);
    dComIfG_Ccsp()->Set(&mSnortColliders[0]);

    sp34.set(-150.0f, 800.0f, 1000.0f);
    cLib_offsetPos(&sp28, &current.pos, shape_angle.y, &sp34);
    sp48.set(0, 0, -0x7C00);

    field_0xd08 = dComIfGp_particle_set(field_0xd08, 0x8909, &current.pos, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
    sp28.y -= 900.0f;

    mSnortColliders[1].SetC(sp28);
    mSnortColliders[1].SetR(300.0f);
    mSnortColliders[1].SetH(900.0f);
    dComIfG_Ccsp()->Set(&mSnortColliders[1]);

    cCcD_Obj* var_r28 = NULL;
    if (mSnortColliders[0].ChkCoHit()) {
        var_r28 = mSnortColliders[0].GetCoHitObj();
    }

    if (mSnortColliders[1].ChkCoHit()) {
        var_r28 = mSnortColliders[1].GetCoHitObj();
    }

    if (var_r28 != NULL) {
        if (fopAcM_GetName(dCc_GetAc(var_r28->GetAc())) == PROC_ALINK) {
            daPy_py_c* player = daPy_getPlayerActorClass();
            s16 var_r26 = cLib_targetAngleY(&current.pos, &player->current.pos);
            player->setThrowDamage(var_r26, 30.0f, 10.0f, 0, 0, 2);
        }
    }
}

void daB_ZANTZ_c::setMouthMode(u8 param_0) {
    if (param_0 == 2) {
        if (mMouthMode == 0 || mMouthMode == 1) {
            mMouthMode = 2;
            mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::BCK_ZZ_CLOSE), J3DFrameCtrl::EMode_NONE, 0.0f, 1.0f, 0.0f, -1.0f);
            Z2GetAudioMgr()->seStart(Z2SE_EN_ZZ_CLOSE, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    } else if (param_0 == 0) {
        if (mMouthMode == 2 || mMouthMode == 3) {
            mMouthMode = 0;
            mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::BCK_ZZ_OPEN), J3DFrameCtrl::EMode_NONE, 0.0f, 1.0f, 0.0f, -1.0f);
            Z2GetAudioMgr()->seStart(Z2SE_EN_ZZ_OPEN, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    }
}

void daB_ZANTZ_c::setAppearMode(u8 param_0) {
    cXyz sp34(0.0f, 0.0f, 2000.0f);

    if (param_0 == 2) {
        if (mAppearMode == 0 || mAppearMode == 1) {
            mAppearMode = 2;
            mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::BCK_ZZ_APPEARANCE), J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f, 0.0f, -1.0f);
            field_0x660 = 1.0f;
            dComIfGp_getVibration().StartQuake(1, 31, cXyz(0.0f, 1.0f, 0.0f));
        }
    } else if (param_0 == 0) {
        if (mAppearMode == 2 || mAppearMode == 3) {
            mAppearMode = 0;
            mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::BCK_ZZ_APPEARANCE), J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f, 0.0f, -1.0f);
            field_0x660 = 1.0f;
            dComIfGp_getVibration().StartQuake(1, 31, cXyz(0.0f, 1.0f, 0.0f));

            if (field_0x66f == 0) {
                field_0x66f = 1;
                current.pos.set(sp34.x, current.pos.y, sp34.z);
                current.angle.y = shape_angle.y = fopAcM_searchPlayerAngleY(this);
            } else {
                s16 sp68 = cLib_targetAngleY(&daPy_getPlayerActorClass()->current.pos, &sp34);
                s16 var_r28 = sp68 + 0x2000 + (field_0x670 * 0x4000);

                current.pos.set(sp34.x + 3000.0f * cM_ssin(var_r28), current.pos.y, sp34.z + 3000.0f * cM_scos(var_r28));
                current.angle.y = shape_angle.y = cLib_targetAngleY(&current.pos, &sp34);
            }

            field_0x664 = getGroundPos();
        }
    } else if (param_0 == 4) {
        if (mAppearMode == 1) {
            if (abs((s16)(fopAcM_searchPlayerAngleY(this) - shape_angle.y)) > 0x1800) {
                mAppearMode = 4;
                mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::BCK_ZZ_APPEARANCE), J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f, 0.0f, -1.0f);
                field_0x660 = 1.0f;
                dComIfGp_getVibration().StartQuake(1, 31, cXyz(0.0f, 1.0f, 0.0f));
            }
        }
    } else if (param_0 == 5) {
        if (mAppearMode == 1) {
            mAppearMode = 5;
            mSearchContinue = true;
            mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::BCK_ZZ_APPEARANCE), J3DFrameCtrl::EMode_LOOP, 0.0f, 1.0f, 0.0f, -1.0f);
            field_0x660 = 1.0f;
            dComIfGp_getVibration().StartQuake(1, 31, cXyz(0.0f, 1.0f, 0.0f));
        }
    }
}

f32 daB_ZANTZ_c::getGroundPos() {
    cXyz sp88(current.pos);
    dBgS_ObjGndChk gndchk;
    sp88.y += 4000.0f;

    while (1) {
        gndchk.SetPos(&sp88);
        sp88.y = dComIfG_Bgsp().GroundCross(&gndchk);

        if (sp88.y == -G_CM3D_F_INF) {
            break;
        }

        if (dComIfG_Bgsp().GetActorPointer(gndchk) != this) {
            break;
        }

        sp88.y -= 10.0f;
    }

    return sp88.y;
}

void daB_ZANTZ_c::action() {
    if (mMouthMode == 0) {
        if (mpMorf->checkFrame(4.0f) || mpMorf->checkFrame(10.0f) || mpMorf->checkFrame(15.0f)) {
            dComIfGp_getVibration().StartShock(1, 31, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (mpMorf->checkFrame(40.0f)) {
            dComIfGp_getVibration().StartShock(3, 31, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (mpMorf->isStop()) {
            mMouthMode++;
        }
    } else if (mMouthMode == 2) {
        if (mpMorf->checkFrame(6.0f) || mpMorf->checkFrame(9.0f) || mpMorf->checkFrame(13.0f) || mpMorf->checkFrame(16.0f)) {
            dComIfGp_getVibration().StartShock(1, 31, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (mpMorf->checkFrame(26.0f)) {
            dComIfGp_getVibration().StartShock(3, 31, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (mpMorf->isStop()) {
            mMouthMode++;
        }
    }

    cXyz particle_pos(current.pos.x, field_0x664, current.pos.z);
    
    switch (mAppearMode) {
    case 0: {
        f32 var_f31 = cLib_addCalc(&current.pos.y, home.pos.y + 3500.0f, 0.1f, 15.0f, 1.0f);
        if (std::abs(var_f31) < 15.0f) {
            cLib_chaseF(&field_0x660, 0.0f, 0.2f);
            mpMorf->setFrame(field_0x660);
        }

        if (current.pos.y > home.pos.y + 300.0f) {
            cLib_chaseF(&field_0x664, 0.0f, 1.7f);

            field_0xd0c = dComIfGp_particle_set(field_0xd0c, 0x8906, &particle_pos, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
            Z2GetAudioMgr()->seStartLevel(Z2SE_EN_ZZ_MV, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }

        if (!var_f31) {
            dComIfGp_getVibration().StopQuake(0x1F);
            mAppearMode++;
            mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::BCK_ZZ_OPEN), J3DFrameCtrl::EMode_LOOP, 0.0f, 0.0f, 0.0f, -1.0f);
        }
        break;
    }
    case 2: {
        f32 var_f31 = cLib_addCalc(&current.pos.y, home.pos.y, 0.1f, 15.0f, 1.0f);
        if (std::abs(var_f31) < 15.0f) {
            cLib_chaseF(&field_0x660, 0.0f, 0.2f);
            mpMorf->setFrame(field_0x660);
        }

        if (current.pos.y > home.pos.y + 300.0f) {
            if (current.pos.y < home.pos.y + 500.0f) {
                cLib_chaseF(&field_0x664, getGroundPos(), 1.7f);
            }

            field_0xd0c = dComIfGp_particle_set(field_0xd0c, 0x8907, &particle_pos, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
            Z2GetAudioMgr()->seStartLevel(Z2SE_EN_ZZ_MV, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }

        if (!var_f31) {
            dComIfGp_getVibration().StopQuake(0x1F);
            mAppearMode++;
            mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::BCK_ZZ_OPEN), J3DFrameCtrl::EMode_LOOP, 0.0f, 0.0f, 0.0f, -1.0f);
        }
        break;
    }
    case 3: {
        current.angle.y = shape_angle.y = fopAcM_searchPlayerAngleY(this);
        break;
    }
    case 4:
    case 5: {
        cLib_addCalcAngleS2(&shape_angle.y, fopAcM_searchPlayerAngleY(this), 0x10, 0x80);
        current.angle.y = shape_angle.y;

        field_0xd0c = dComIfGp_particle_set(field_0xd0c, 0x890A, &particle_pos, &tevStr, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
        Z2GetAudioMgr()->seStartLevel(Z2SE_EN_ZZ_MV, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

        BOOL sp6C = false;
        if (mAppearMode == 4) {
            if (abs((s16)(shape_angle.y - fopAcM_searchPlayerAngleY(this))) < 0x1000) {
                sp6C = true;
            }
        } else if (!mSearchContinue) {
            sp6C = true;
        }

        if (sp6C) {
            cLib_chaseF(&field_0x660, 0.0f, 0.1f);
            mpMorf->setFrame(field_0x660);

            if (!field_0x660) {
                dComIfGp_getVibration().StopQuake(0x1F);
                mAppearMode = 1;
                mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::BCK_ZZ_OPEN), J3DFrameCtrl::EMode_LOOP, 0.0f, 0.0f, 0.0f, -1.0f);
            }
        } else {
            cLib_chaseF(&field_0x660, 1.0f, 0.2f);
            mpMorf->setFrame(field_0x660);
        }
        break;
    }
    }

    fopAcM_posMoveF(this, NULL);
    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daB_ZANTZ_c::cc_set() {
    cXyz cc_center;
    eyePos = current.pos;
    eyePos.y += 100.0f;

    for (int i = 0; i < 3; i++) {
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(i + 2));
        mDoMtx_stack_c::transM(0.0f, 0.0f, 130.0f);
        mDoMtx_stack_c::multVecZero(&cc_center);

        field_0x6e4[i].SetC(cc_center);
        field_0x6e4[i].SetR(230.0f);
        dComIfG_Ccsp()->Set(&field_0x6e4[i]);
    }
}

static u8 l_initHIO;

static daB_ZANTZ_HIO_c l_HIO;

int daB_ZANTZ_c::execute() {
    if (field_0x668 != 0) {
        field_0x668--;
    }

    action();

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();

    mpBrk->play();

    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
    
    if (mpBgW != NULL) {
        mpBgW->Move();
    }

    if (field_0x668 != 0) {
        calcSnortEffect();
    }

    cc_set();
    return 1;
}

static int daB_ZANTZ_Execute(daB_ZANTZ_c* i_this) {
    return i_this->execute();
}

static int daB_ZANTZ_IsDelete(daB_ZANTZ_c* i_this) {
    return 1;
}

int daB_ZANTZ_c::_delete() {
    if (mInitHIO) {
        l_initHIO = false;
    }

    dComIfG_resDelete(&mPhase, "B_oh");

    if (mpBgW != NULL) {
        dComIfG_Bgsp().Release(mpBgW);
    }

    mSound.deleteObject();
    return 1;
}

static int daB_ZANTZ_Delete(daB_ZANTZ_c* i_this) {
    return i_this->_delete();
}

int daB_ZANTZ_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::BMDR_ZZ);
    JUT_ASSERT(0, modelData != NULL);

    mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::BCK_ZZ_OPEN), 0, 0.0f, 0, -1, &mSound, 0, 0x11000084);
    if (mpMorf == NULL) {
        return 0;
    }

    mpBrk = new mDoExt_brkAnm();
    if (mpBrk == NULL) {
        return 0;
    }

    if (!mpBrk->init(mpMorf->getModel()->getModelData(), (J3DAnmTevRegKey*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::BRK_ZZ), TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
        return 0;
    }

    mpBgW = new dBgW();
    if (mpBgW == NULL) {
        return 0;
    }

    if (mpBgW->Set((cBgD_t*)dComIfG_getObjectRes("B_zan", daB_ZANT_c::DZB_ZZ), 1, &mBgMtx) == 1) {
        return 0;
    }

    mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return ((daB_ZANTZ_c*)i_this)->CreateHeap();
}

int daB_ZANTZ_c::create() {
    fopAcM_ct(this, daB_ZANTZ_c);

    int phase_state = dComIfG_resLoad(&mPhase, "B_oh");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x3220)) {
            return cPhs_ERROR_e;
        }

        if (!l_initHIO) {
            l_initHIO = true;
            mInitHIO = true;
            l_HIO.field_0x4 = -1;
        }

        OS_REPORT("B_ZANTZ PARAM %x\n", fopAcM_GetParam(this));
        field_0x670 = fopAcM_GetParam(this);
        if (field_0x670 != 0) {
            field_0x66f = 1;
        }

        if (mpBgW != NULL) {
            if (dComIfG_Bgsp().Regist(mpBgW, this)) {
                return cPhs_ERROR_e;
            }
        }

        attention_info.flags = 0;
        
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -400.0f, -400.0f,-400.0f);
        fopAcM_SetMax(this, 400.0f, 400.0f, 400.0f);

        health = 80;
        field_0x560 = 80;

        mCcStts.Init(0xFE, 0, this);

        for (int i = 0; i < 3; i++) {
            field_0x6e4[i].Set(cc_zant_src);
            field_0x6e4[i].SetStts(&mCcStts);
        }

        for (int i = 0; i < 2; i++) {
            mSnortColliders[i].Set(cc_zant_snort_src);
            mSnortColliders[i].SetStts(&mCcStts);
        }

        mMouthMode = 2;
        mAppearMode = 2;
        gravity = 0.0f;

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::scaleM(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);
        mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        mpMorf->modelCalc();
    }

    return phase_state;
}

static int daB_ZANTZ_Create(daB_ZANTZ_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daB_ZANTZ_Method = {
    (process_method_func)daB_ZANTZ_Create,
    (process_method_func)daB_ZANTZ_Delete,
    (process_method_func)daB_ZANTZ_Execute,
    (process_method_func)daB_ZANTZ_IsDelete,
    (process_method_func)daB_ZANTZ_Draw,
};

extern actor_process_profile_definition g_profile_B_ZANTZ = {
  fpcLy_CURRENT_e,        // mLayerID
  4,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_B_ZANTZ,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daB_ZANTZ_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  228,                    // mPriority
  &l_daB_ZANTZ_Method,    // sub_method
  0x00040000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
