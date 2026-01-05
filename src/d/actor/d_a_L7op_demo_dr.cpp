/**
 * @file d_a_L7op_demo_dr.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_L7op_demo_dr.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/actor/d_a_npc4.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_msg_mng.h"

void daL7ODR_c::setAction(daL7ODR_c::actionFn i_action) {
    if (mAction != NULL) {
        field_0x888 = -1;
        (this->*mAction)();
    }

    mPrevAction = mAction;
    mAction = i_action;
    field_0x888 = 0;
    (this->*mAction)();
}

void daL7ODR_c::action() {
    (this->*mAction)();
}

void daL7ODR_c::setDrAction(daL7ODR_c::actionFn i_action) {
    if (mDrAction != NULL) {
        field_0x88c = -1;
        (this->*mDrAction)();
    }

    mPrevDrAction = mDrAction;
    mDrAction = i_action;
    field_0x88c = 0;
    (this->*mDrAction)();
}

void daL7ODR_c::dr_action() {
    (this->*mDrAction)();
}

void daL7ODR_c::mtx_set() {
    J3DModel* model = mpModelMorf->getModel();

    mDoMtx_stack_c::copy(model->getAnmMtx(0x15));
    mDoMtx_stack_c::multVecZero(&field_0x830);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(current.angle);

    model->setBaseScale(scale);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    mpModelMorf->modelCalc();
    fopAcM_SetMtx(this, model->getBaseTRMtx());
}

int daL7ODR_c::draw() {
    if (field_0x8b4) {
        return 1;
    }

    J3DModel* model = mpModelMorf->getModel();

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    mpModelMorf->entryDL();
    
    cXyz sp8;
    sp8.set(current.pos.x, 10.0f + current.pos.y, current.pos.z);
    mShadowKey = dComIfGd_setShadow(mShadowKey, 0, model, &sp8, 6000.0f + BREG_F(19), 0.0f, current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static int daL7ODR_Draw(daL7ODR_c* i_this) {
    return i_this->draw();
}

void daL7ODR_c::wait() {
    if (field_0x888 == 0) {
        field_0x888++;
    } else if (field_0x888 != -1) {
        camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
        camera_class* camera0 = dComIfGp_getCamera(0);

        if (eventInfo.checkCommandDemoAccrpt()) {
            dComIfGs_onZoneSwitch(Z2SE_DPD_SCREEN_OUT, fopAcM_GetRoomNo(this));
            camera->mCamera.Stop();
            camera->mCamera.SetTrimSize(3);
            setAction(&daL7ODR_c::pl_walk);
            return;
        }

        if (check_start()) {
            fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
            eventInfo.onCondition(2);
            mDemoCamEye = camera0->lookat.eye;
            mDemoCamCenter = camera0->lookat.center;
        }
    }
}

void daL7ODR_c::pl_walk() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = dComIfGp_getCamera(0);
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (field_0x888 == 0) {
        cXyz player_pos(-790.0f, 0.0f, 3125.0f);
        cXyz demo_pos(0.0f, 0.0f, 0.0f);
        csXyz angle;
        angle.y = cLib_targetAngleY(&player_pos, &demo_pos);

        mDemoCamCenter.set(-900.0f, 500.0f, 4800.0f);
        mDemoCamEye.set(-910.0f, 520.0f, 5500.0f);
        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, 64.0f, 0);

        player->changeOriginalDemo();
        player->setPlayerPosAndAngle(&player_pos, angle.y, 0);
        player->changeDemoMoveAngle(angle.y);
        player->changeDemoPos0(&demo_pos);
        player->changeDemoMode(4, 0, 0, 0);

        field_0x8b5 = 5;
        daNpcF_getPlayerInfoFromPlayerList(2, fopAcM_GetRoomNo(this), player_pos, angle);
        dComIfGs_setRestartRoom(player_pos, angle.y, fopAcM_GetRoomNo(this));
        Z2GetAudioMgr()->subBgmStart(Z2BGM_DRAGON_D01);

        field_0x8b7 = 1;
        field_0x888++;
    } else if (field_0x888 != -1) {
        if (field_0x888 == 1) {
            if (field_0x8b5 == 0) {
                cXyz player_pos(-790.0f, 0.0f, 3125.0f);
                cXyz demo_pos(0.0f, 0.0f, 0.0f);
                s16 angle = cLib_targetAngleY(&player_pos, &demo_pos);

                player->setPlayerPosAndAngle(&player_pos, angle, 0);
                player->changeDemoMoveAngle(angle);
                player->changeDemoPos0(&demo_pos);
                player->changeDemoMode(2, 0, 0, 0);

                field_0x8b5 = 90;
                field_0x898 = 2.0f;
                
                dScnKy_env_light_c* kankyo = dKy_getEnvlight();
                kankyo->wether = 1;
                field_0x888++;
            }
        } else if (field_0x888 == 2) {
            if (field_0x8b5 != 0) {
                if (field_0x8b5 <= 10) {
                    cLib_chaseF(&field_0x898, 0.0f, 0.2f);
                }
                mDemoCamCenter.y += field_0x898;
                camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, 64.0f, 0);
            } else {
                field_0x8b5 = 60;
                field_0x888++;
            }
        } else if (field_0x888 == 3) {
            if (field_0x8b5 == 0) {
                mDemoCamCenter.set(0.0f, 120.0f, 2300.0f);
                mDemoCamEye.set(0.0f, 100.0f, 3000.0f);
                camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, 64.0f, 0);
                field_0x8b5 = 20;
                field_0x888++;
            } else if (field_0x8b5 == 1) {
                cXyz player_pos(100.0f, 0.0f, 3000.0f);
                cXyz demo_pos(0.0f, 0.0f, 1400.0f);
                s16 angle = cLib_targetAngleY(&player_pos, &demo_pos);

                player->setPlayerPosAndAngle(&player_pos, angle, 0);
                player->changeDemoMoveAngle(angle);
                player->changeDemoPos0(&demo_pos);
                player->changeDemoMode(4, 0, 0, 0);
            }
        } else if (field_0x888 == 4) {
            cXyz player_pos(100.0f, 0.0f, 3000.0f);
            cXyz demo_pos(0.0f, 0.0f, 1400.0f);
            s16 angle = cLib_targetAngleY(&player_pos, &demo_pos);

            if (field_0x8b5 == 0) {
                player->setPlayerPosAndAngle(&player_pos, angle, 0);
                player->changeDemoMoveAngle(angle);
                player->changeDemoPos0(&demo_pos);
                player->changeDemoMode(2, 0, 0, 0);

                field_0x8b5 = 70;
                field_0x888++;
            }
        } else if (field_0x888 == 5) {
            if (field_0x8b5 == 0) {
                field_0x8b5 = 20;
                player->changeDemoMode(0, 0, 0, 0);
                field_0x888++;
            }
        } else if (field_0x888 == 6) {
            if (field_0x8b5 == 0) {
                field_0x8b5 = 92;
                player->changeDemoMode(0x14, 0, 0, 0);
                field_0x888++;
            }
        } else if (field_0x888 == 7) {
            if (field_0x8b5 == 0) {
                field_0x8b6 = 150;
                player->changeDemoMode(2, 0, 0, 0);
                setDrAction(&daL7ODR_c::dr_fly);
                field_0x888 = 9;
            }
        } else if (field_0x888 == 8) {
            if (field_0x8b5 == 0) {
                setDrAction(&daL7ODR_c::dr_fly);
                field_0x8b6 = 90;
                field_0x888++;
            }
        } else if (field_0x8b6 == 0) {
            cXyz player_pos(player->current.pos);
            if (player_pos.x) {
                cLib_chaseF(&player_pos.x, 0.0f, 1.0f);
                player->setPlayerPosAndAngle(&player_pos, -0x8000, 0);
                player->changeDemoMoveAngle(-0x8000);
                player->changeDemoPos0(&player_pos);
            }
        }
    }
}

void daL7ODR_c::setZoomOutCamPos(cXyz& param_0, cXyz& param_1, f32 param_2) {
    csXyz spC;
    spC.x = -cLib_targetAngleX(&param_1, &param_0);
    spC.y = cLib_targetAngleY(&param_1, &param_0);
    spC.z = 0;

    mDoMtx_stack_c::transS(param_1);
    mDoMtx_stack_c::YrotM(spC.y);
    mDoMtx_stack_c::XrotM(spC.x);
    mDoMtx_stack_c::transM(0.0f, 0.0f, param_2);
    mDoMtx_stack_c::multVecZero(&param_0);
}

void daL7ODR_c::dr_wait() {
    if (field_0x88c == 0) {
        cXyz sp8(0.0f, 0.0f, 0.0f);
        field_0x8b4 = 1;

        current.pos.set(5000.0f * cM_ssin(0x400), 7000.0f, 5000.0f * cM_scos(0x400));
        current.angle.y = cLib_targetAngleY(&current.pos, &sp8) + 0x4000;

        mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_dr", 0x2F), 2, 3.0f, 1.0f, 0.0f, -1.0f);
        field_0x890 = 0x2F;
        mpModelMorf->setFrame(20.0f);
        mpModelMorf->setPlaySpeed(0.0f);

        speedF = 0.0f;
        gravity = 0.0f;
        field_0x88c++;
    }
}

void daL7ODR_c::dr_fly() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = dComIfGp_getCamera(0);
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp264(0.0f, 0.0f, 0.0f);

    if (field_0x88c == 0) {
        field_0x8b4 = 0;
        mpModelMorf->setPlaySpeed(1.0f);
    
        field_0x898 = 600.0f;
        mDemoCamCenter = player->current.pos;
        mDemoCamCenter.y += 160.0f;

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(current.angle);
        mDoMtx_stack_c::transM(0.0f, 0.0f, 1400.0f);
        mDoMtx_stack_c::multVecZero(&mDemoCamEye);
        mDemoCamEye.y += 400.0f;
        setZoomOutCamPos(mDemoCamEye, mDemoCamCenter, field_0x898);

        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);
        speedF = 60.0f;
        field_0x8b5 = 60;
        field_0x88c++;
    } else if (field_0x88c != -1) {
        if (field_0x88c == 1) {
            cLib_chaseAngleS(&current.angle.x, 0x800, 0x10);
            current.angle.y = cLib_targetAngleY(&current.pos, &sp264) + 0x4000;
            speed.y = -speedF * cM_ssin(current.angle.x);
            mAcch.CrrPos(dComIfG_Bgsp());
            fopAcM_posMoveF(this, NULL);

            mDemoCamCenter = player->current.pos;
            mDemoCamCenter.y += 160.0f;

            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::ZXYrotM(current.angle);
            mDoMtx_stack_c::transM(0.0f, 0.0f, 1400.0f);
            mDoMtx_stack_c::multVecZero(&mDemoCamEye);
            mDemoCamEye.y += 400.0f;
            setZoomOutCamPos(mDemoCamEye, mDemoCamCenter, field_0x898);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

            if (8.0f == mpModelMorf->getFrame()) {
                mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
            }

            if (field_0x8b5 == 0) {
                field_0x89c = 0.0f;
                field_0x88c++;
            }
        } else if (field_0x88c == 2) {
            cLib_chaseAngleS(&current.angle.x, 0x800, 0x10);
            current.angle.y = cLib_targetAngleY(&current.pos, &sp264) + 0x4000;
            speed.y = -speedF * cM_ssin(current.angle.x);
            mAcch.CrrPos(dComIfG_Bgsp());
            fopAcM_posMoveF(this, NULL);

            mDemoCamCenter = player->current.pos;
            mDemoCamCenter.y += 160.0f;
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::ZXYrotM(current.angle);
            mDoMtx_stack_c::transM(0.0f, 0.0f, 1400.0f);
            mDoMtx_stack_c::multVecZero(&mDemoCamEye);
            mDemoCamEye.y += 400.0f;

            f32 temp_f30 = mDemoCamCenter.abs(mDemoCamEye) - 1800.0f;
            cLib_chaseF(&field_0x89c, 140.0f, 2.0f);
            cLib_chaseF(&field_0x898, temp_f30, field_0x89c);
            setZoomOutCamPos(mDemoCamEye, mDemoCamCenter, field_0x898);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

            if (8.0f == mpModelMorf->getFrame()) {
                mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
            }

            if (field_0x898 == temp_f30) {
                field_0x89c = 140.0f;
                field_0x88c++;
            }
        } else if (field_0x88c == 3) {
            cXyz sp258;
            cLib_chaseAngleS(&current.angle.x, 0x800, 0x10);
            current.angle.y = cLib_targetAngleY(&current.pos, &sp264) + 0x4000;
            speed.y = -speedF * cM_ssin(current.angle.x);
            mAcch.CrrPos(dComIfG_Bgsp());
            fopAcM_posMoveF(this, NULL);

            sp258 = player->current.pos;
            sp258.y += 160.0f;

            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::ZXYrotM(current.angle);
            mDoMtx_stack_c::transM(0.0f, 0.0f, 1400.0f);
            mDoMtx_stack_c::multVecZero(&mDemoCamEye);
            mDemoCamEye.y += 400.0f;

            cLib_chaseF(&field_0x89c, 0.0f, 4.0f);
            field_0x898 += field_0x89c;
            setZoomOutCamPos(mDemoCamEye, sp258, field_0x898);
            cLib_chasePos(&mDemoCamCenter, field_0x830, 100.0f);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

            if (8.0f == mpModelMorf->getFrame()) {
                mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
            }

            if (!field_0x89c && field_0x830 == mDemoCamCenter) {
                field_0x898 = 0.0f;
                field_0x89c = 1400.0f;
                field_0x8a0 = 0.0f;
                field_0x8a4 = 0.0f;
                field_0x88c++;
            }
        } else if (field_0x88c == 4) {
            cLib_chaseAngleS(&current.angle.x, 0x800, 0x10);
            current.angle.y = cLib_targetAngleY(&current.pos, &sp264) + 0x4000;
            cLib_chaseF(&speedF, 80.0f, 0.5f);
            speed.y = -speedF * cM_ssin(current.angle.x);
            mAcch.CrrPos(dComIfG_Bgsp());
            fopAcM_posMoveF(this, NULL);

            cLib_chasePos(&mDemoCamCenter, field_0x830, 100.0f);
            cLib_chaseF(&field_0x8a0, 100.0f, 0.5f);
            cLib_chaseF(&field_0x89c, -1400.0f, field_0x8a0);

            cXyz sp24C;
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::ZXYrotM(current.angle);
            mDoMtx_stack_c::transM(0.0f, 0.0f, field_0x89c);
            mDoMtx_stack_c::multVecZero(&sp24C);
            sp24C.y += 400.0f;

            cLib_chaseF(&field_0x898, 100.0f, 1.0f);
            cLib_chasePos(&mDemoCamEye, sp24C, field_0x898);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

            if (8.0f == mpModelMorf->getFrame()) {
                mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
            }

            if (current.pos.z > 0.0f) {
                field_0x88c++;
            }
        } else if (field_0x88c == 5) {
            cLib_chaseAngleS(&current.angle.x, 0x800, 0x10);
            cLib_chaseAngleS(&current.angle.y, 0, 0x80);
            cLib_chaseF(&speedF, 80.0f, 0.5f);
            mAcch.CrrPos(dComIfG_Bgsp());
            fopAcM_posMoveF(this, NULL);

            mDemoCamCenter = field_0x830;
            cLib_chaseF(&field_0x8a0, 100.0f, 0.5f);
            cLib_chaseF(&field_0x89c, -1400.0f, field_0x8a0);

            cXyz sp240;
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::ZXYrotM(current.angle);
            mDoMtx_stack_c::transM(0.0f, 0.0f, field_0x89c);
            mDoMtx_stack_c::multVecZero(&sp240);
            sp240.y += 400.0f;

            cLib_chaseF(&field_0x898, 100.0f, 1.0f);
            cLib_chasePos(&mDemoCamEye, sp240, field_0x898);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

            if (8.0f == mpModelMorf->getFrame()) {
                mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
            }

            if (mpModelMorf->getFrame() >= 33.0f) {
                mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_dr", 0x30), 0, 3.0f, 1.0f, 0.0f, -1.0f);
                field_0x890 = 0x30;
                current.angle.y = 0;
                field_0x88c++;
            }
        } else if (field_0x88c == 6) {
            cLib_chaseAngleS(&current.angle.x, 0, 0x20);
            speed.y = -speedF * cM_ssin(current.angle.x);
            mAcch.CrrPos(dComIfG_Bgsp());
            fopAcM_posMoveF(this, NULL);

            mDemoCamCenter = field_0x830;
            cLib_chaseF(&field_0x8a0, 100.0f, 0.5f);
            cLib_chaseF(&field_0x89c, -1400.0f, field_0x8a0);

            cXyz sp234;
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::ZXYrotM(current.angle);
            mDoMtx_stack_c::transM(0.0f, 0.0f, field_0x89c);
            mDoMtx_stack_c::multVecZero(&sp234);
            sp234.y += 400.0f;

            cLib_chaseF(&field_0x898, 100.0f, 1.0f);
            cLib_chasePos(&mDemoCamEye, sp234, field_0x898);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

            if (field_0x890 == 0x30) {
                if (mpModelMorf->isStop()) {
                    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_dr", 0x31), 2, 3.0f, 1.0f, 0.0f, -1.0f);
                    field_0x890 = 0x31;
                } else if (8.0f == mpModelMorf->getFrame()) {
                    mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
                }
            } else if (field_0x890 == 0x31) {
                mSound.startCreatureSoundLevel(Z2SE_EN_DR_GLIDE, 0, -1);
            }

            if (current.pos.z >= 3000.0f) {
                field_0x8b5 = 10;
                field_0x88c++;
            }
        } else if (field_0x88c == 7) {
            cXyz sp228(2674.0f, 0.0f, 3038.0f);

            if (field_0x890 == 0x30) {
                if (mpModelMorf->isStop()) {
                    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_dr", 0x31), 2, 3.0f, 1.0f, 0.0f, -1.0f);
                    field_0x890 = 0x31;
                } else if (8.0f == mpModelMorf->getFrame()) {
                    mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
                }
            } else if (field_0x890 == 0x31) {
                mSound.startCreatureSoundLevel(Z2SE_EN_DR_GLIDE, 0, -1);
            }

            cLib_chaseAngleS(&current.angle.x, 0x800, 0x10);
            sp228 += current.pos;
            sp228 /= 2.0f;

            if ((u16)current.angle.y > 0xE000) {
                cLib_addCalcAngleS(&current.angle.z, 0x2000, 30, 0x800, 0x40);
            } else {
                cLib_addCalcAngleS(&current.angle.z, 0, 60, 0x800, 0x40);
                cLib_chaseF(&speedF, 240.0f, 1.0f);
            }

            current.angle.y = cLib_targetAngleY(&current.pos, &sp228) + 0x4000;
            speed.y = -speedF * cM_ssin(current.angle.x);
            mAcch.CrrPos(dComIfG_Bgsp());
            fopAcM_posMoveF(this, NULL);
            mDemoCamCenter = field_0x830;

            cXyz sp21C;
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::ZXYrotM(current.angle);
            mDoMtx_stack_c::transM(0.0f, 0.0f, -1400.0f);
            mDoMtx_stack_c::multVecZero(&sp21C);
            sp21C.y += 400.0f;

            cLib_chasePos(&mDemoCamEye, sp21C, 90.0f);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);
            mSound.startCreatureSoundLevel(Z2SE_EN_DR_GLIDE, 0, -1);

            if (field_0x8b5 == 0 && (u16)current.angle.y < 0x8000) {
                cXyz sp210;
                current.angle.y = -0x8000;
                field_0x8b4 = 1;
                mpModelMorf->setPlaySpeed(0.0f);
                mDemoCamCenter.set(0.0f, -140.0f, 2045.0f);
                mDemoCamEye.set(0.0f, 460.0f, 2800.0f);
                camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

                sp210.set(0.0f, 0.0f, 1400.0f);
                player->changeDemoMode(0x19, 0, 0, 0);
                player->setPlayerPosAndAngle(&sp210, -0x8000, 0);
                player->changeDemoPos0(&sp210);
                player->changeDemoMoveAngle(-0x8000);

                field_0x898 = 40.0f;
                field_0x88c++;
            }
        } else if (field_0x88c == 8) {
            cXyz sp204(0.0f, 220.0f, 1200.0f);
            if (mDemoCamEye.z > 1800.0f) {
                cLib_chaseF(&field_0x898, 80.0f, 2.0f);
            } else {
                cLib_chaseF(&field_0x898, 0.0f, 4.5f);
            }

            cLib_chasePos(&mDemoCamEye, sp204, field_0x898);
            mDemoCamCenter.z -= 85.0f;
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);
            mSound.startCreatureSoundLevel(Z2SE_EN_DR_GLIDE, 0, -1);

            if (mDemoCamEye.z <= sp204.z && !field_0x898) {
                cXyz sp1F8;
                field_0x8b4 = 0;
                mpModelMorf->setPlaySpeed(1.0f);
                field_0x8b5 = 0;

                sp1F8.set(0.0f, 0.0f, 1400.0f);
                player->setPlayerPosAndAngle(&sp1F8, -0x8000, 0);
                player->changeDemoPos0(&sp1F8);
                player->changeDemoMoveAngle(-0x8000);

                current.pos.set(0.0f, 800.0f, 1720.0f);
                current.angle.set(0x670, -0x8000, 0);
                speedF = 135.0f;
                speed.y = -21.0f;
                field_0x88c++;
            }
        } else if (field_0x88c == 9) {
            if (field_0x8b5 == 0) {
                mpModelMorf->setPlaySpeed(1.0f);
                cLib_addCalcAngleS(&current.angle.z, 0, 0x20, 0x800, 0x40);
                cLib_chaseF(&field_0x898, 128.0f, 4.0f);
                cLib_chaseAngleS(&current.angle.x, 0x200, field_0x898);
                cLib_chaseF(&speedF, 240.0f, 1.0f);
                speed.y = -speedF * cM_ssin(current.angle.x);
                mAcch.CrrPos(dComIfG_Bgsp());
                fopAcM_posMoveF(this, NULL);
            }

            mDemoCamCenter.set(0.0f, -140.0f, -930.0f);
            mDemoCamEye.set(0.0f, 220.0f, 1200.0f);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);
            mSound.startCreatureSoundLevel(Z2SE_EN_DR_GLIDE, 0, -1);

            if (1000.0f > current.pos.z) {
                mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_dr", 0x45), 0, 20.0f, 1.0f, 0.0f, -1.0f);
                field_0x890 = 0x45;
                field_0x898 = 0.0f;
                field_0x89c = 0.0f;
                field_0x88c++;
            }
        } else if (field_0x88c == 10) {
            cLib_chaseF(&field_0x898, 3072.0f, 96.0f);
            cLib_chaseAngleS(&current.angle.x, -0x4000, field_0x898);
            cLib_chaseF(&field_0x89c, 8.0f, 0.3f);
            cLib_chaseF(&speedF, 0.0f, field_0x89c);
            speed.y = -speedF * cM_ssin(current.angle.x);
            mAcch.CrrPos(dComIfG_Bgsp());
            fopAcM_posMoveF(this, NULL);

            if (field_0x830.z > -400.0f) {
                mDemoCamCenter.set(0.0f, -140.0f, -930.0f);
            } else {
                cLib_chasePos(&mDemoCamCenter, current.pos, 100.0f);
            }

            mDemoCamEye.set(0.0f, 220.0f, 1200.0f);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);
            mSound.startCreatureSoundLevel(Z2SE_EN_DR_GLIDE, 0, -1);

            if (current.angle.x < -0x2000) {
                mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_dr", 0x42), 0, 3.0f, 1.0f, 0.0f, -1.0f);
                field_0x890 = 0x42;
                field_0x88c++;
            }
        } else if (field_0x88c == 11) {
            if (speedF < 40.0f) {
                cLib_addCalcAngleS(&current.angle.x, 0, 0x20, 0x800, 0x200);
            } else {
                cLib_chaseF(&field_0x898, 3072.0f, 96.0f);
                cLib_chaseAngleS(&current.angle.x, -0x4000, field_0x898);
            }

            cLib_chaseF(&field_0x89c, 8.0f, 0.3f);
            cLib_chaseF(&speedF, 0.0f, field_0x89c);
            speed.y = -speedF * cM_ssin(current.angle.x);
            mAcch.CrrPos(dComIfG_Bgsp());
            
            s16 sp8 = current.angle.y;
            current.angle.y = -0x8000;
            fopAcM_posMoveF(this, NULL);
            current.angle.y = sp8;

            cLib_chasePos(&mDemoCamCenter, current.pos, 100.0f);
            mDemoCamEye.set(0.0f, 220.0f, 1200.0f);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

            if (5.0f == mpModelMorf->getFrame() || 35.0f == mpModelMorf->getFrame()) {
                mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
            }

            if (mpModelMorf->isStop()) {
                mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_dr", 0x39), 0, 0.0f, 1.0f, 0.0f, -1.0f);
                fopMsgM_messageSetDemo(0x485);
                field_0x890 = 0x39;
                current.angle.y = 0;
                field_0x88c++;
            }
        } else if (field_0x88c == 12) {
            cLib_chasePos(&mDemoCamCenter, field_0x830, 100.0f);
            mDemoCamEye.set(0.0f, 1700.0f, -680.0f);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

            f32 temp_f1 = mpModelMorf->getFrame();
            if (22.0f == temp_f1 || 59.0f == temp_f1 || 92.0f == temp_f1 || 125.0f == temp_f1) {
                mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
            } else if (29.0f == temp_f1) {
                mSound.startCreatureVoice(Z2SE_EN_DR_V_SCREAM, -1);
            } else if (33.0f == temp_f1) {
                dComIfGp_getVibration().StartQuake(VIBMODE_Q_POWER4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
            } else if (132.0f == temp_f1) {
                dComIfGp_getVibration().StopQuake(0x1F);
            }

            if (mpModelMorf->isStop()) {
                mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_dr", 0x35), 2, 3.0f, 1.0f, 0.0f, -1.0f);
                field_0x890 = 0x35;
                field_0x8b5 = 120;
                field_0x88c++;
            }
        } else if (field_0x88c == 13) {
            cXyz sp1EC;
            cLib_chasePos(&mDemoCamCenter, field_0x830, 100.0f);
            sp1EC = player->current.pos;
            sp1EC.y += 200.0f;
            sp1EC.z += 200.0f;
            cLib_chasePos(&mDemoCamEye, sp1EC, 10.0f);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);

            if (7.0f == mpModelMorf->getFrame()) {
                mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
            }

            if (field_0x8b5 == 0) {
                if (mpModelMorf->getFrame() >= 33.0f) {
                    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_dr", 0x34), 0, 3.0f, 1.0f, 0.0f, -1.0f);
                    field_0x890 = 0x34;
                    field_0x88c++;
                }
            }
        } else {
            if (7.0f == mpModelMorf->getFrame() || 37.0f == mpModelMorf->getFrame()) {
                mSound.startCreatureSound(Z2SE_EN_DR_WING, 0, -1);
            } else if (62.0f == mpModelMorf->getFrame()) {
                argument = 1;
            }

            if (mpModelMorf->isStop()) {
                mDemoCamEye.x = 0.0f;
                mDemoCamEye.z = 2200.0f;
                camera->mCamera.Reset(mDemoCamCenter, mDemoCamEye);
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                dComIfGp_event_reset();
                fopAcM_delete(this);
            }
        }
    }
}

int daL7ODR_c::execute() {
    if (field_0x8b5 != 0) {
        field_0x8b5--;
    }

    if (field_0x8b6 != 0) {
        field_0x8b6--;
    }

    action();
    dr_action();
    shape_angle = current.angle;
    mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mtx_set();
    return 1;
}

static int daL7ODR_Execute(daL7ODR_c* i_this) {
    return i_this->execute();
}

static int daL7ODR_IsDelete(daL7ODR_c* i_this) {
    return 1;
}

int daL7ODR_c::_delete() {
    fopAcM_GetID(this);
    dComIfG_resDelete(&mPhase, "B_dr");

    if (heap != NULL) {
        mpModelMorf->stopZelAnime();
    }

    if (field_0x8b7 != 0) {
        Z2GetAudioMgr()->subBgmStop();
    }

    return 1;
}

static int daL7ODR_Delete(daL7ODR_c* i_this) {
    return i_this->_delete();
}

int daL7ODR_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("B_dr", 0x49);
    JUT_ASSERT(1146, modelData != NULL);
    mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("B_dr", 0x2F), 2, 1.0f, 0, -1, &mSound, 0, 0x11000084);
    field_0x890 = 0x2F;
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daL7ODR_c* a_this = (daL7ODR_c*)i_this;
    return a_this->CreateHeap();
}

BOOL daL7ODR_c::check_start() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz player_pos;

    player_pos = player->current.pos;
    if (0.0f == player_pos.y && -890.0f < player_pos.x && player_pos.x < -690.0f && 3000.0f < player_pos.z && player_pos.z < 3200.0f && !dComIfGp_checkPlayerStatus0(0, 0x100) && !dComIfGp_checkPlayerStatus0(0, 0x200)) {
        return TRUE;
    }

    return FALSE;
}

int daL7ODR_c::create() {
    fopAcM_ct(this, daL7ODR_c);
    field_0x8b7 = 0;

    int phase_state = dComIfG_resLoad(&mPhase, "B_DR");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("L7ODR PARAM %x\n", fopAcM_GetParam(this));
        OS_REPORT("L7ODR//////////////L7ODR SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x68E0)) {
            OS_REPORT("//////////////L7ODR SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////L7ODR SET 2 !!\n");

        fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -20000.0f, -20000.0f, -20000.0f);
        fopAcM_SetMax(this, 20000.0f, 20000.0f, 20000.0f);

        mSound.init(&current.pos, &eyePos, 3, 1);

        health = 1;
        field_0x560 = 1;
        
        scale.setall(1.0f);

        attention_info.distances[fopAc_attn_BATTLE_e] = 0;
        attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
    
        fopAcM_SetGroup(this, 0);
        fopAcM_OffStatus(this, 0);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
        mAcchCir.SetWall(20.0f, 40.0f);

        setAction(&daL7ODR_c::wait);
        setDrAction(&daL7ODR_c::dr_wait);

        daL7ODR_Execute(this);
    }

    return phase_state;
}

static int daL7ODR_Create(daL7ODR_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daL7ODR_Method = {
    (process_method_func)daL7ODR_Create,
    (process_method_func)daL7ODR_Delete,
    (process_method_func)daL7ODR_Execute,
    (process_method_func)daL7ODR_IsDelete,
    (process_method_func)daL7ODR_Draw,
};

actor_process_profile_definition g_profile_L7ODR = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_L7ODR,             // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daL7ODR_c),      // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  222,                    // mPriority
  &l_daL7ODR_Method,      // sub_method
  0x00040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
