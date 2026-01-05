// NOTE: This filename is a complete conjecture.

#include "d/actor/d_a_obj_gra2.h"

static void* s_sub2(void* i_actor, void* i_data) {
    daObj_GrA_c* i_this = (daObj_GrA_c*)i_data;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_GRA && (u8)i_this->checkGraSub((fopAc_ac_c*)i_actor)) {
        return i_actor;
    }
    return NULL;
}

BOOL daObj_GrA_c::checkGraSub(fopAc_ac_c* a_this) {
    daObj_GrA_c* i_this = (daObj_GrA_c*)a_this;

    if (i_this->mMode != 0) {
        return FALSE;
    }

    if (getTagNo() == i_this->getTagNo()) {
        return TRUE;
    }

    return FALSE;
}

int daObj_GrA_c::waitDemo(void* param_1) {
    cXyz sp34;
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (field_0xa7c) {
        case 0:
            setBaseMotion(0, mpHIO->m.field_0x0c);
            setFaceMotion(13, -1.0f);
            setLookMode(0);
            break;

        case 1:
            if (getTagNo() != 0) {
                bool has_actor = field_0x204c.getActor() == NULL;

                if (field_0x2032 == 0 || has_actor) {
                    if (field_0x2032 == 0) {
                        fopAc_ac_c* actor_p = (fopAc_ac_c*) fpcM_Search(s_sub, this);
                        if (actor_p != NULL) {
                            daTagGra_c* graTag_p = (daTagGra_c*) actor_p;
                            if (graTag_p->getMode() == 2) {
                                mGraPos = graTag_p->getGraPos();
                                mGraExtent = graTag_p->getGraExtent();
                                mGraHeight = graTag_p->getGraHeight();
                                mGraMode = graTag_p->getMode();
                                field_0x2032 = 1;
                                fopAcM_delete(actor_p);
                            }
                        }
                    }

                    if (has_actor) {
                        fopAc_ac_c* o_actor_p = (fopAc_ac_c*)fpcM_Search(s_sub2, this);
                        if (o_actor_p != NULL) {
                            field_0x204c.entry(o_actor_p);
                        }
                    }
                } else {
                    #if DEBUG
                    if (mpHIO->m.field_0x8a) {
                        fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x100);
                    } else {
                        fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x100);
                    }
                    #endif

                    sp34 = player->current.pos - mGraPos;
                    if (sp34.absXZ() < mGraExtent && sp34.z > -30.0f && sp34.z < mGraHeight) {
                        setProcess(&daObj_GrA_c::startDemo);
                    }
                }
            }
            break;

        case 2:
            #if DEBUG
            fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x100);
            #endif
            break;
    }

    return 1;
}

int daObj_GrA_c::skipDemoGra() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    daObj_GrA_c* gra_p = (daObj_GrA_c*)field_0x204c.getActor();
    camera->mCamera.Start();
    camera->mCamera.SetTrimSize(0);
    dComIfGp_event_reset();
    gra_p->soldierDemoSkip();
    gra_p->field_0x204c.entry(NULL);
    dComIfGs_onSwitch(field_0xa7f, fopAcM_GetRoomNo(this));
    return 1;
}

static int skipCallBack(void* param_1, int param_2) {
    daObj_GrA_c* i_this = (daObj_GrA_c*)param_1;
    i_this->skipDemoGra();
    fopAcM_delete(i_this);
    return 1;
}

int daObj_GrA_c::startDemo(void* param_1) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    daObj_GrA_c* gra_p = (daObj_GrA_c *)field_0x204c.getActor();
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    dCamera_c* camBody = dCam_getBody();

    switch (field_0xa7c) {
        case 0:
            setBaseMotion(0, mpHIO->m.field_0x0c);
            setFaceMotion(13, -1.0f);
            setLookMode(0);
            mDemoCamMode = 1;
            field_0x209c = 1;
            break;

        case 1: {
            switch (mDemoCamMode) {
                case 0:
                    break;

                case 1: {
                    if (!eventInfo.checkCommandDemoAccrpt()) {
                        fopAcM_orderPotentialEvent(this, 2, 0xFFEF, 0);
                        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                        return 1;
                    }

                    camera->mCamera.Stop();
                    mDemoCamMode = 2;
                    field_0x2056 = 0;
                    mDemoCamFovy = field_0x208c =  camBody->Fovy();
                    mDemoCamCenter = mGraPos;
                    mDemoCamCenter.y += 150.0f;
                    mDemoCamEye = mDemoCamCenter;
                    mDemoCamEye.z += 700.0f;
                    mDemoCamCenter.x += 50.0f;
                    field_0x2080 = mDemoCamCenter;
                    field_0x2074 = mDemoCamEye;

                    camera->mCamera.SetTrimSize(2);
                    player->changeOriginalDemo();
                    gra_p->field_0x204c.entry(this);
                    gra_p->setProcess(&daObj_GrA_c::soldierDemo);
                    fopAcM_OnStatus(gra_p, fopAcM_STATUS_UNK_0x8000);
                    field_0x2056 = 0;
                    dComIfGp_getEvent()->setSkipProc(this, skipCallBack, 0);
                    dComIfGp_getEvent()->onSkipFade();
                }
                // fallthrough
                case 2: {
                    cXyz player_pos = player->current.pos;
                    if ((player_pos - mGraPos).abs() < 50.0f) {
                        if (field_0x2056 >= 60) {
                            mDemoCamMode = 3;
                            field_0x2056 = 0;
                        } else {
                            csXyz player_angle = player->current.angle;
                            s16 tgt_ang = cLib_targetAngleY(&player->current.pos, &current.pos) + 0x800;
                            cLib_addCalcAngleS2(&player_angle.y, tgt_ang, 2, field_0x1528.field_0xa14);
                            player->setPlayerPosAndAngle(&player->current.pos, &player_angle);
                            field_0x2056++;
                            break;
                        }
                    } else {
                        player->changeDemoMode(3, 0, 0, 0);
                        player->changeDemoPos0(&mGraPos);
                        break;
                    }
                }
                // fallthrough
                case 3: {
                    if (field_0x2056 >= 180) {
                        mDemoCamMode = 4;
                        field_0x2056 = 0;
                        field_0x2080 = mDemoCamCenter;
                        field_0x2074 = mDemoCamEye;
                    } else {
                        csXyz player_angle = player->current.angle;
                        s16 sVar1 = cLib_targetAngleY(&player->current.pos, &current.pos) - 0x2800;
                        cLib_addCalcAngleS2(&player_angle.y, sVar1, 4, field_0x1528.field_0xa14);
                        player->setPlayerPosAndAngle(&player->current.pos, &player_angle);

                        mDoMtx_stack_c::YrotS(sVar1);
                        cXyz sp3c(0.0f, 150.0f, 100.0f);
                        mDoMtx_stack_c::multVec(&sp3c, &sp3c);
                        sp3c += player->current.pos;
                        cLib_addCalcPos2(&mDemoCamEye, sp3c, 0.05f, (field_0x2074 - sp3c).abs() / 40.0f);

                        mDoMtx_stack_c::YrotS(sVar1);
                        cXyz sp48(0.0f, 100.0f, 300.0f);
                        mDoMtx_stack_c::multVec(&sp48, &sp48);
                        sp48 += player->current.pos;
                        cLib_addCalcPos2(&mDemoCamCenter, sp48, 0.05f, (sp48 - field_0x2080).abs() / 25.0f);
                        field_0x2056++;
                        break;
                    }
                }
                // fallthrough
                case 4: {
                    if (field_0x2056 >= 180) {
                        mDemoCamMode = 5;
                        field_0x2056 = 0;
                        field_0x2080 = mDemoCamCenter;
                        field_0x2074 = mDemoCamEye;
                    } else {
                        csXyz player_angle = player->current.angle;
                        s16 sVar1 = cLib_targetAngleY(&player->current.pos, &current.pos) + 0x800;
                        cLib_addCalcAngleS2(&player_angle.y, sVar1, 1, field_0x1528.field_0xa14);
                        player->setPlayerPosAndAngle(&player->current.pos, &player_angle);

                        mDoMtx_stack_c::YrotS(sVar1);
                        cXyz sp54(0.0f, 150.0f, 100.0f);
                        mDoMtx_stack_c::multVec(&sp54, &sp54);
                        sp54 += player->current.pos;
                        cLib_addCalcPos2(&mDemoCamEye, sp54, 0.06f, (field_0x2074 - sp54).abs() / 70.0f);

                        mDoMtx_stack_c::YrotS(sVar1);
                        cXyz sp60(0.0f, 230.0f, 300.0f);
                        mDoMtx_stack_c::multVec(&sp60, &sp60);
                        sp60 += player->current.pos;
                        cLib_addCalcPos2(&mDemoCamCenter, sp60, 0.06f, (sp60 - field_0x2080).abs() / 70.0f);

                        if (field_0x2056 == 150) {
                            setBaseMotion(16, 0.0f);
                            setFaceMotion(11, 0.0f);
                            setLookMode(0);
                            gra_p->setBaseMotion(17, mpHIO->m.field_0x0c);
                            gra_p->setFaceMotion(12, -1.0f);
                            gra_p->setLookMode(0);

                            current.angle.y += (s16) 0x4000;
                            field_0x91a.y = shape_angle.y = current.angle.y;
                        }

                        field_0x2056++;
                        break;
                    }
                }
                // fallthrough
                case 5: {
                    if (field_0x2056 >= 40) {
                        mDemoCamMode = 6;
                        field_0x2056 = 0;
                    } else {
                        mDoMtx_stack_c::YrotS(cLib_targetAngleY(&player->current.pos, &current.pos));
                        cXyz sp6c(0.0f, 150.0f, 100.0f);
                        mDoMtx_stack_c::multVec(&sp6c, &sp6c);
                        sp6c += player->current.pos;
                        cLib_addCalcPos2(&mDemoCamEye, sp6c, 0.01f, (field_0x2074 - sp6c).abs() / 60.0f);

                        cXyz player_pos = current.pos;
                        player_pos.y += 280.0f;
                        cLib_addCalcPos2(&mDemoCamCenter, player_pos, 0.5f,
                                         (player_pos - field_0x2080).abs() / (200.0f - field_0x2056 * 3));
                        field_0x2056++;
                        break;
                    }
                }
                // fallthrough
                case 6: {
                    if (field_0x2056 >= 90) {
                        mDemoCamMode = 7;
                        field_0x2056 = 0;
                    } else {
                        mDoMtx_stack_c::YrotS(cLib_targetAngleY(&player->current.pos, &current.pos));
                        cXyz sp84(100.0f, -50.0f, -800.0f);
                        mDoMtx_stack_c::multVec(&sp84, &sp84);
                        sp84 += current.pos;
                        cLib_addCalcPos2(&mDemoCamEye, sp84, 0.5f, (field_0x2074 - sp84).abs() / 35.0f);

                        cXyz player_pos = current.pos;
                        player_pos.y += 280.0f;
                        cLib_addCalcPos2(&mDemoCamCenter, player_pos, 0.35f, (player_pos - field_0x2080).abs() / 20.0f);
                        field_0x2056++;
                        break;
                    }
                }
                // fallthrough
                case 7: {
                    if (mpModelMorf->isStop()) {
                        gra_p->setDemoMode(1);
                        setDemoMode(8);
                        field_0x2056 = 0;
                        setBaseMotion(0, mpHIO->m.field_0x0c);
                        setFaceMotion(13, -1.0f);
                        setLookMode(2);
                    }
                    break;
                }

                case 8: {
                    if (gra_p->field_0xa90 != 20 || !gra_p->mpModelMorf->isStop()) break;

                    setDemoMode(9);
                    setBaseMotion(1, mpHIO->m.field_0x0c);
                    setFaceMotion(13, -1.0f);
                    field_0x2080 = mDemoCamCenter;
                }
                // fallthrough
                case 9: {
                    cXyz sp9c(0.0f, 0.0f, 80.0f);
                    mDoMtx_stack_c::YrotS(gra_p->current.angle.y);
                    mDoMtx_stack_c::multVec(&sp9c, &sp9c);
                    sp9c += gra_p->current.pos;
                    if (field_0xa90 != 15) {
                        if (mAcchCir.ChkWallHit() && gra_p == dComIfG_Bgsp().GetActorPointer(mAcchCir)) {
                            if (field_0xa90 != 15) {
                                setBaseMotion(15, mpHIO->m.field_0x0c);
                                setFaceMotion(13, -1.0f);
                                setLookMode(0);
                                current.angle.y = cLib_targetAngleY(&current.pos, &sp9c);
                                shape_angle.y = current.angle.y;
                                field_0x91a.y = current.angle.y;
                                mAcch.SetGrndNone();
                                mAcch.SetWallNone();
                                gravity = 0.0f;
                                speed.setall(0.0f);
                                speedF = 0.0f;
                            }
                        } else {
                            s16 tgt_ang = cLib_targetAngleY(&current.pos, &sp9c);
                            cLib_addCalcAngleS2(&current.angle.y, tgt_ang, 4, mpHIO->m.field_0x88);
                            shape_angle.y = current.angle.y;
                            field_0x91a.y = current.angle.y;
                            cLib_chasePosXZ(&current.pos, sp9c, mpHIO->m.field_0x90);
                        }
                    } else {
                        if (!mpModelMorf->isStop()) {
                            mAcch.SetGrndNone();
                            mAcch.SetWallNone();
                            gravity = 0.0f;
                        } else {
                            cXyz spa8(0.0f, 146.09f, 161.97f);
                            mDoMtx_stack_c::YrotS(field_0x91a.y);
                            mDoMtx_stack_c::multVec(&spa8, &spa8);
                            current.pos += spa8;
                            old.pos = current.pos;
                            gravity = 0.0f;
                            setLookMode(0);
                            setDemoMode(10);
                            gra_p->setDemoMode(3);
                            setBaseMotion(18, 0.0f);
                            setFaceMotion(13, 0.0f);
                        }

                        cXyz player_pos = gra_p->current.pos;
                        cLib_addCalcPosXZ2(&mDemoCamCenter, player_pos, 0.1f, (player_pos - field_0x2080).abs() / 30.0f);
                        break;
                    }
                }
                // fallthrough
                case 10: {
                    if (gra_p->getDemoMode() == 4) {
                        setBaseMotion(10, 6.0f);
                        setFaceMotion(13, 6.0f);
                        calcMotion();
                        mpModelMorf->setPlaySpeed(3.0f);
                        setDemoMode(11);
                        mAcch.ClrGrndNone();
                        mAcch.ClrWallNone();
                        gravity = mpHIO->m.mGravity;
                        speedF = 13.0f;
                        speed.y = 70.0f;
                        field_0x2056 = 0;
                    } else {
                        cLib_addCalcAngleS2(&current.angle.y, gra_p->home.angle.y + 0x8000, 16, mpHIO->m.field_0x88);
                        shape_angle.y = current.angle.y;
                        field_0x91a.y = current.angle.y;
                        cXyz player_pos = gra_p->current.pos;
                        cLib_addCalcPosXZ2(&mDemoCamCenter, player_pos, 0.1f, (player_pos - field_0x2080).abs() / 30.0f);
                        gravity = 0.0f;
                    }

                    break;
                }

                case 11: {
                    if (field_0x2056 >= 250) {
                        camera->mCamera.Start();
                        camera->mCamera.SetTrimSize(0);
                        dComIfGp_event_reset();
                        fopAcM_OffStatus(gra_p, 0x8000);
                        gra_p->setProcess(&daObj_GrA_c::wait);
                        dComIfGs_onSwitch(field_0xa7f, fopAcM_GetRoomNo(this));
                        fopAcM_delete(this);
                        break;
                    }

                    if (mAcch.ChkGroundLanding()) {
                        cXyz spcc(0.0f, 0.0f, 20.0f);
                        mDoMtx_stack_c::transS(current.pos);
                        mDoMtx_stack_c::YrotM(field_0x91a.y);
                        mDoMtx_stack_c::multVec(&spcc, &field_0xab8[2].field_0x4);
                        field_0xab8[2].field_0x10 = field_0x91a;
                        field_0xab8[2].field_0x0 = 1;

                        mSound.startCreatureSound(Z2SE_GORON_ROLL_BOUND, 0, -1);
                        speedF = 20.0f;
                    } else if (mAcch.ChkGroundHit()) {
                        cXyz spd8(-1482.0f, 3650.0f, -4459.0f);
                        if ((current.pos - spd8).absXZ() > 50.0f) {
                            cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &spd8), 
                                                24, mpHIO->m.field_0x88);
                            shape_angle.y = current.angle.y;
                            field_0x91a.y = current.angle.y;
                        } else {
                            speedF = 0.0f;
                        }

                        field_0x2056++;
                    }

                    if (speed.y > 0.0f) {
                        cXyz spe4 = mDemoCamCenter;
                        mDemoCamEye += (current.pos - spe4);
                    }

                    mDemoCamCenter = current.pos;
                }
            }

            break;
        }

        case 2:
            field_0x209c = 0;
            break;

        default:
            break;
    }

    if (mDemoCamMode != 0) {
        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
        field_0x2056++;
    }

    return 1;
}

BOOL daObj_GrA_c::turn(s16 param_1, f32 param_2, int param_3) {
    int o_prm_1 = param_1;
    if (field_0x1fc8 == 0) {
        field_0x1fd0 = fabsf(cM_sht2d((s16)(current.angle.y - param_1)));
        field_0x1fd0 = field_0x1fd0 / 180.0f * param_2;
        field_0x1fd0 = (int)field_0x1fd0 + 1;

        if (field_0x1fd0 < 8.0f) {
            field_0x1fd0 = 8.0f;
        }

        field_0x1fd4 = current.angle.y;
    }

    s16 sVar1 = field_0x1fd4;
    if (param_3 == 0) {
        param_3 = ((s16) (param_1 - field_0x1fd4) >= 0) ? 1 : -1;
    }

    int var_r30 = (u16) (o_prm_1 - sVar1);
    if (param_3 < 0) {
        var_r30 = -(u16)(0xFFFF - var_r30);
    }

    var_r30 *= cM_ssin(field_0x1fc8);
    field_0x1fc8 = field_0x1fc8 + 16384.0f / field_0x1fd0;
    if ((u16)field_0x1fc8 >= 0x4000) {
        current.angle.y = param_1;
        field_0x1fc8 = 0x4000;
    } else {
        current.angle.y = field_0x1fd4 + var_r30;
    }

    return param_1 == current.angle.y;
}

BOOL daObj_GrA_c::turn_step(s16 param_1, int param_2, int param_3, int param_4) {
    if (field_0x1fcc == 0) {
        if ((int)fabsf(cM_sht2d((s16)(param_1 - field_0x91a.y))) > 20) {
            if (param_2 >= 0 && field_0xa92 != param_2) {
                setFaceMotion(param_2, 3.0f);
            }

            if (param_3 >= 0 && field_0xa90 != param_3) {
                setBaseMotion(param_3, 3.0f);
            }
        }

        field_0x1fd6 = param_1;
        field_0x1fc8 = 0;

        if (field_0x91a.y == field_0x1fd6) {
            field_0x1fcc++;
        }

        current.angle.y = field_0x91a.y;
        shape_angle.y = current.angle.y;
        field_0x1fcc++;
    } else if (field_0x1fcc == 1) {
        if (turn(field_0x1fd6, param_4, 0) != 0) {
            shape_angle.y = current.angle.y;
            field_0x91a.y = current.angle.y;
            field_0x1fcc++;
        } else {
            shape_angle.y = current.angle.y;
            field_0x91a.y = current.angle.y;
        }
    }

    return (field_0x1fcc > 1) ? 1 : 0;
}

BOOL daObj_GrA_c::chkFindPlayer2(int param_1, s16 param_2) {
    cXyz sp80 = getAttentionPos(daPy_getPlayerActorClass());
    s16 angle_diff = fopAcM_searchPlayerAngleY(this) - current.angle.y;

    int dist_tbl_idx;
    if (abs(angle_diff) < 0x3C00) {
        dist_tbl_idx = (param_1 == 0) ? getDistTableIdx(10, 6) : getDistTableIdx(12, 6);
    } else {
        dist_tbl_idx = (param_1 == 0) ? getDistTableIdx(4, 6) : getDistTableIdx(6, 6);
    }

    f32 fVar1 = (param_1 == 0) ? dComIfGp_getAttention()->getDistTable(dist_tbl_idx).mDistMax
                               : dComIfGp_getAttention()->getDistTable(dist_tbl_idx).mDistMaxRelease;
    f32 fVar2 = dComIfGp_getAttention()->getDistTable(dist_tbl_idx).mLowerY * -1.0f;
    f32 fVar3 = dComIfGp_getAttention()->getDistTable(dist_tbl_idx).mUpperY * -1.0f;
    u32 uVar2 = dComIfGp_getAttention()->getDistTable(dist_tbl_idx).mAngleSelect;

    f32 fVar4 = 180.0f;
    if ((uVar2 & 8) != 0) {
        fVar4 = 30.0f;
    } else if ((uVar2 & 16) != 0) {
        fVar4 = 45.0f;
    } else if ((uVar2 & 32) != 0) {
        fVar4 = 60.0f;
    } else if ((uVar2 & 64) != 0) {
        fVar4 = 90.0f;
    } else if ((uVar2 & 128) != 0) {
        fVar4 = 110.0f;
    } else if ((uVar2 & 256) != 0) {
        fVar4 = 135.0f;
    }

    if (chkPointInArea(sp80, attention_info.position, fVar1, fVar2, fVar3, 0)) {
        if (chkActorInSight2(daPy_getPlayerActorClass(), fVar4, param_2)) {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL daObj_GrA_c::chkPointInArea(cXyz param_1, cXyz param_2, f32 param_3, f32 param_4, f32 param_5, s16 param_6) {
    cXyz sp68, sp74;
    f32 var_f30 = param_4;
    f32 var_f31 = param_5;

    if (var_f30 < var_f31) {
        var_f31 = param_4;
        var_f30 = param_5;
    }

    sp68.set(param_3, fabsf(var_f30 - var_f31) / 2, param_3);
    sp74 = param_2;
    sp74.y += var_f31;
    sp74.y += sp68.y;

    return chkPointInArea(param_1, sp74, sp68, param_6); 
}

BOOL daObj_GrA_c::chkPointInArea(cXyz param_1, cXyz param_2, cXyz param_3, s16 param_4) {
    return daNpcF_chkPointInArea(param_1, param_2, param_3, param_4);
}

BOOL daObj_GrA_c::chkActorInSight2(fopAc_ac_c* actor, f32 param_2, s16 param_3) {
    if (param_2 < 180.0f) {
        s16 sVar1 = cM_deg2s(param_2);
        cXyz sp34 = getAttentionPos(actor);
        cSGlobe cStack_48(attention_info.position - sp34);
        cSAngle acStack_4c(cSAngle(cStack_48.U().Inv()) - param_3);
        s16 sVar2 = acStack_4c.Val();
        if (sVar2 < 0) {
            sVar2 = -sVar2;
        }

        return (sVar2 > sVar1) == 0;
    }

    return 1;
}

cXyz daObj_GrA_c::getAttentionPos(fopAc_ac_c* i_actor_p) {
    cXyz pos = i_actor_p->attention_info.position;

    if (fopAcM_GetName(i_actor_p) == PROC_ALINK) {
        pos.y -= daPy_py_c::getAttentionOffsetY();
    }

    return pos;
}

int daObj_GrA_c::standWaitJump(void* param_1) {
    switch (field_0xa7c) {
        case 0:
            field_0x1fcc = 0;
            break;

        case 1:
            if (getJumpType() == 1) {
                setLookMode(0);
                if (home.angle.y != field_0x91a.y) {
                    if (turn_step(home.angle.y, 0, 19, 20) != 0) {
                        field_0x1fcc = 0;
                        setBaseMotion(21, mpHIO->m.field_0x0c);
                        setFaceMotion(0, -1.0f);
                    }
                } else if (field_0xa90 != 21) {
                    setBaseMotion(21, mpHIO->m.field_0x0c);
                    setFaceMotion(0, -1.0f);
                }
            } else {
                BOOL bVar1 = field_0x14f8.getActor() != NULL;
                if (chkFindPlayer2(bVar1, shape_angle.y) != 0) {
                    if (!bVar1) {
                        field_0x14f8.entry(daPy_getPlayerActorClass());
                    }
                } else if (bVar1) {
                    field_0x14f8.entry(NULL);
                }

                if (field_0x14f8.getActor() != NULL) {
                    setLookMode(1);
                } else {
                    setLookMode(0);
                    if ((home.angle.y != field_0x91a.y) && (turn_step(home.angle.y, 0, 19, 20) != 0)) {
                        field_0x1fcc = 0;
                        setBaseMotion(0, mpHIO->m.field_0x0c);
                        setFaceMotion(0, -1.0f);
                    }
                }
            }
            break;

        case 2:
            break;
    }

    return 1;
}

void dummy_aVec(daObj_GrA_c* a_this) {
    static cXyz aVec[4] = {
        cXyz(0.0f, 0.0f, 0.0f),
        cXyz(0.0f, 0.0f, 0.0f),
        cXyz(0.0f, 0.0f, 0.0f),
        cXyz(0.0f, 0.0f, 0.0f),
    };
}
