/**
 * @file d_a_L7demo_dr.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_L7demo_dr.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_obj_lv7bridge.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_overlap_mng.h"

void daDr_c::setAction(daDr_c::actionFn i_action) {
    if (mAction != NULL) {
        field_0x654 = -1;
        (this->*mAction)();
    }

    mPrevAction = mAction;
    mAction = i_action;
    field_0x654 = 0;
    (this->*mAction)();
}

void daDr_c::action() {
    (this->*mAction)();
}

void daDr_c::mtx_set() {
    J3DModel* model = mpModelMorf->getModel();

    mDoMtx_stack_c::copy(model->getAnmMtx(1));
    mDoMtx_stack_c::multVecZero(&field_0x618);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(current.angle);

    model->setBaseScale(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    mpModelMorf->modelCalc();
    fopAcM_SetMtx(this, model->getBaseTRMtx());
    eyePos = field_0x618;
}

int daDr_c::draw() {
    if (mIsHide) {
        return 1;
    }

    J3DModel* model = mpModelMorf->getModel();

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    mpModelMorf->entryDL();
    return 1;
}

static int daDr_Draw(daDr_c* i_this) {
    return i_this->draw();
}

void daDr_c::wait() {
    if (field_0x654 == 0) {
        mIsHide = TRUE;
        current.pos.set(-7200.0f, -200.0f, -12450.0f);
        current.angle.y = 0;
        field_0x654++;
    } else if (field_0x654 != -1) {
        camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
        camera_class* camera0 = dComIfGp_getCamera(0);

        if (eventInfo.checkCommandDemoAccrpt()) {
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            daPy_getPlayerActorClass()->changeOriginalDemo();

            if (dComIfG_play_c::getLayerNo(0) != 3) {
                setAction(&daDr_c::pl_walk);
            } else {
                setAction(&daDr_c::pl_turn);
            }
        } else if (dComIfG_play_c::getLayerNo(0) == 3 || check_start()) {
            fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
            eventInfo.onCondition(2);

            mDemoCamEye = camera0->lookat.eye;
            mDemoCamCenter = camera0->lookat.center;
        }
    }
}

void daDr_c::pl_walk() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = dComIfGp_getCamera(0);
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp4C;

    if (field_0x654 == 0) {
        mDemoCamCenter.set(-4383.0f, 285.0f, -12450.0f);
        mDemoCamEye.set(-4688.0f, 340.0f, -12450.0f);
        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

        sp4C.set(-4450.0f, 0.0f, -12450.0f);
        player->setPlayerPosAndAngle(&sp4C, 0x4000, 0);
        player->changeDemoMoveAngle(0x4000);
        player->changeDemoMode(4, 0, 0, 0);

        mTimer = 5;
        field_0x654++;
    } else if (field_0x654 != -1) {
        if (field_0x654 == 1) {
            if (mTimer == 0) {
                mTimer = 92.0f + hREG_F(0);

                mDemoCamCenter.set(-4383.0f, 285.0f, -12450.0f);
                mDemoCamEye.set(-4688.0f, 340.0f, -12450.0f);
                camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

                sp4C.set(-4450.0f, 0.0f, -12450.0f);
                player->setPlayerPosAndAngle(&sp4C, 0x4000, 0);
                player->changeDemoMoveAngle(0x4000);

                sp4C.set(-4100.0f, 0.0f, -12450.0f);
                player->changeDemoPos0(&sp4C);
                player->changeDemoMode(2, 0, 0, 0);

                field_0x654++;
            }
        } else if (mTimer == 0) {
            if (dComIfG_play_c::getLayerNo(0) != 3) {
                dStage_changeScene(6, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
                fopAcM_delete(this);
            } else {
                setAction(&daDr_c::pl_turn);
            }
        }
    }
}

void daDr_c::pl_turn() {
    dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    dComIfGp_getCamera(0);
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (field_0x654 == 0) {
        field_0x654++;
    } else if (field_0x654 != -1) {
        if (field_0x654 == 1) {
            if (!fopOvlpM_IsPeek()) {
                mSound.startCreatureVoice(Z2SE_EN_DR_V_NAKU, -1);
                mTimer = 22.0f + hREG_F(1);
                field_0x654++;
            }
        } else if (field_0x654 == 2) {
            if (mTimer == 0) {
                mTimer = 22.0f + hREG_F(0);
                player->changeDemoMode(0x19, 0, 0, 0);
                field_0x654++;
            }
        } else if (mTimer == 0) {
            setAction(&daDr_c::bridge_destroy);
        }
    }
}

void daDr_c::bridge_destroy() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = dComIfGp_getCamera(0);
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (field_0x654 == 0) {
        cXyz sp68;
        mIsHide = FALSE;

        current.pos.set(-7200.0f, -200.0f, -12450.0f);
        current.angle.y = 0;

        mpModelMorf->setPlaySpeed(1.0f);

        mDemoCamCenter.set(field_0x618.x, field_0x618.y, field_0x618.z);
        mDemoCamEye.set(-4600.0f + hREG_F(4), 80.0f + hREG_F(5), -12450.0f + hREG_F(6));
        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

        field_0x654++;
    } else if (field_0x654 != -1) {
        if (mpModelMorf->getFrame() < 156.0f) {
            mDemoCamCenter.set(field_0x618.x, field_0x618.y, field_0x618.z);
            mDemoCamEye.set(-4600.0f + hREG_F(4), 80.0f + hREG_F(5), -12450.0f + hREG_F(6));
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);
        } else if (156.0f == mpModelMorf->getFrame()) {
            cXyz sp5C;
            daObjLv7Brg_c* brg = (daObjLv7Brg_c*)fopAcM_SearchByName(PROC_Obj_Lv7Bridge);
            if (brg != NULL) {
                brg->setDestroyAnmA();
            }

            dComIfGp_getVibration().StartQuake(8, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            mSound.startCreatureSound(Z2SE_EN_DR_DEMO_DESTROY, 0, -1);
            mTimer = 60.0f + hREG_F(10);

            sp5C.set(-4450.0f, 0.0f, -12450.0f);
            player->setPlayerPosAndAngle(&sp5C, -0x4000, 0);
            player->changeDemoMoveAngle(-0x4000);
            player->changeDemoMode(4, 0, 0, 0);
        } else if (mpModelMorf->getFrame() >= 210.0f) {
            mpModelMorf->setPlaySpeed(0.0f);

            if (mTimer == 0) {
                setAction(&daDr_c::bridge_destroy2);
            }
        }
    }
}

void daDr_c::bridge_destroy2() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    dComIfGp_getCamera(0);
    daPy_getPlayerActorClass();

    if (field_0x654 == 0) {
        daObjLv7Brg_c* brg = (daObjLv7Brg_c*)fopAcM_SearchByName(PROC_Obj_Lv7Bridge);
        if (brg != NULL) {
            brg->setDestroyAnmB();
        }

        mpModelMorf->setPlaySpeed(1.0f);

        mDemoCamCenter.set(-6700.0f, -850.0f, -20300.0f);
        mDemoCamEye.set(-6700.0f, -900.0f, -20700.0f);
        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

        mTimer = 180;
        field_0x65a = 0;
        field_0x654++;
    } else if (field_0x654 != -1) {
        if (field_0x65a < 28) {
            field_0x65a++;
        } else if (field_0x65a == 28) {
            field_0x65a++;
            dComIfGp_getVibration().StopQuake(0x1F);
        }

        if (mTimer == 0) {
            u8 swbit = fopAcM_GetParam(this);
            if (swbit != 0xFF) {
                dComIfGs_onSwitch(swbit, fopAcM_GetRoomNo(this));
            }

            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            dStage_changeScene(7, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            fopAcM_delete(this);
        }
    }
}

int daDr_c::execute() {
    if (mTimer != 0) {
        mTimer--;
    }

    action();

    if (dComIfG_play_c::getLayerNo(0) == 0) {
        return 1;
    }

    if (mpModelMorf->getFrame() == 115.0f || mpModelMorf->getFrame() == 147.0f) {
        mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
    }

    shape_angle = current.angle;
    mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    mtx_set();
    return 1;
}

static int daDr_Execute(daDr_c* i_this) {
    return i_this->execute();
}

static int daDr_IsDelete(daDr_c* i_this) {
    return 1;
}

int daDr_c::_delete() {
    fpc_ProcID id = fopAcM_GetID(this);

    if (heap != NULL) {
        mpModelMorf->stopZelAnime();
    }

    if (dComIfG_play_c::getLayerNo(0) == 0) {
        return 1;
    }

    dComIfG_resDelete(&mPhase, "B_dr");
    return 1;
}

static int daDr_Delete(daDr_c* i_this) {
    return i_this->_delete();
}

int daDr_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("B_dr", 0x49);
    JUT_ASSERT(655, modelData != NULL);

    mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("B_dr", 0x1E), 0, 0.0f, 0, -1, &mSound, 0, 0x11000084);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daDr_c* a_this = (daDr_c*)i_this;
    return a_this->CreateHeap();
}

BOOL daDr_c::check_start() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz player_pos;

    if (dComIfGs_getKeyNum() == 0) {
        return FALSE;
    }

    player_pos = player->current.pos;
    if (-4480.0f < player_pos.x && player_pos.x < -3730.0f && -12800.0f < player_pos.z && player_pos.z < -12100.0f) {
        return TRUE;
    }

    return FALSE;
}

int daDr_c::create() {
    fopAcM_ct(this, daDr_c);

    int phase_state;
    if (dComIfG_play_c::getLayerNo(0) == 3) {
        phase_state = dComIfG_resLoad(&mPhase, "B_dr");
        if (phase_state == cPhs_COMPLEATE_e) {
            OS_REPORT("dr PARAM %x\n", fopAcM_GetParam(this));
            OS_REPORT("dr//////////////DR SET 1 !!\n");

            if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x68C0)) {
                OS_REPORT("//////////////DR SET NON !!\n");
                return cPhs_ERROR_e;
            }

            OS_REPORT("//////////////DR SET 2 !!\n");

            field_0x618.set(-21133.0f, 4660.0f, -24785.0f);
            eyePos = field_0x618;
            mSound.init(&field_0x618, &eyePos, 3, 1);

            health = 1;
            field_0x560 = 1;

            scale.setall(1.0f);

            setAction(&daDr_c::wait);

            u8 swbit = fopAcM_GetParam(this );
            if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(this))) {
                return cPhs_ERROR_e;
            }

            daDr_Execute(this);
        }
    } else {
        phase_state = cPhs_COMPLEATE_e;
        mIsHide = TRUE;
        OS_REPORT("dr PARAM %x\n", fopAcM_GetParam(this));

        health = 1;
        field_0x560 = 1;

        u8 swbit = fopAcM_GetParam(this);
        if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(this))) {
            return cPhs_ERROR_e;
        }

        setAction(&daDr_c::wait);
        daDr_Execute(this);
    }

    return phase_state;
}

static int daDr_Create(daDr_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daDr_Method = {
    (process_method_func)daDr_Create,
    (process_method_func)daDr_Delete,
    (process_method_func)daDr_Execute,
    (process_method_func)daDr_IsDelete,
    (process_method_func)daDr_Draw,
};

extern actor_process_profile_definition g_profile_DR = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_DR,                // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daDr_c),         // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  206,                    // mPriority
  &l_daDr_Method,         // sub_method
  0x00040000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
