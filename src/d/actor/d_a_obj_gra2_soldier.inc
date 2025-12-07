#include "d/actor/d_a_obj_gra2.h"
#include "d/d_camera.h"
#include "d/actor/d_a_tag_gra.h"
#include <cmath.h>

f32 daObj_GrA_c::getSrchCircleR() {
    f32 srchCircleR;
    u8 uVar1 = fopAcM_GetParam(this) & 0x7F;
    if (uVar1 == 0) {
        srchCircleR = mpHIO->m.field_0x6c * 10.0f;
    } else {
        srchCircleR = uVar1 * 10;
    }

    return srchCircleR;
}

int daObj_GrA_c::standWait(void* param_1) {
    cXyz sp5c;

    switch (field_0xa7c) {
        case 0:
            field_0xf50.OffAtSetBit();
            field_0x1518 = 0;
            break;

        case 1: {
            cLib_calcTimer(&field_0x1fdc);
            if (cLib_calcTimer(&field_0x1528.field_0xa18) != 0) {
                if (field_0xa90 != 8) {
                    cXyz sp68(0.0f, 0.0f, 0.0f);
                    if (field_0xa90 == 11) {
                        if (mpModelMorf->getFrame() >= 4.0f) {
                            sp68.z = -0.44806668f;
                        }
                    } else if (field_0xa90 == 12) {
                        sp68.z = -0.6734286f;
                    } else if (field_0xa90 == 13) {
                        sp68.z = 1.7936944f;
                        sp68.x = -0.9048056f;
                    }

                    cLib_offsetPos(&current.pos, &current.pos, shape_angle.y, &sp68);

                    if (mpModelMorf->isStop()) {
                        setBaseMotion(8, mpHIO->m.field_0x0c);
                        setFaceMotion(13, -1.0f);
                        setLookMode(1);
                    }
                }
            } else {
                sp5c = daPy_getPlayerActorClass()->current.pos;
                sp5c.y -= daPy_py_c::getAttentionOffsetY();
                f32 fVar1 = (sp5c - current.pos).absXZ();
                s16 val = cLib_targetAngleY(&attention_info.position, &sp5c);
                val -= cLib_targetAngleY(&current.pos, &eyePos);

                if (
                    (field_0x1fdc == 0 && dComIfGp_checkPlayerStatus0(0, 0x100) == 0)
                    && (
                        (home.pos - current.pos).absXZ() <= getSrchCircleR()
                        && fVar1 <= getSrchCircleR()
                        && std::abs(sp5c.y - current.pos.y) < 300.0f
                    )) {
                    field_0x150c = 1;
                }

                if (field_0x150c != 0) {
                    setProcess(&daObj_GrA_c::walk);
                    setBaseMotion(1, mpHIO->m.field_0x0c);
                    setFaceMotion(13, -1.0f);
                } else if (current.angle.y != home.angle.y) {
                    cLib_addCalcAngleS2(&current.angle.y, home.angle.y, 1, mpHIO->m.field_0x88);
                    shape_angle.y = current.angle.y;
                    field_0x91a.y = current.angle.y;

                    if (current.angle.y == home.angle.y) {
                        setBaseMotion(0, mpHIO->m.field_0x0c);
                        setFaceMotion(13, -1.0f);
                    }
                }
            }

            break;
        }

        case 2: {
            break;
        }
    }

    return 1;
}

int daObj_GrA_c::walk(void* param_1) {
    cXyz player_pos;

    switch (field_0xa7c) {
        case 0:
            setBaseMotion(1, mpHIO->m.field_0x0c);
            setFaceMotion(13, -1.0f);
            break;

        case 1: {
            player_pos = daPy_getPlayerActorClass()->current.pos;
            f32 fVar1 = (player_pos - current.pos).absXZ();
            s16 sVar1 = cLib_targetAngleY(&current.pos, &player_pos);
            s16 dbg_0xa = cLib_targetAngleY(&current.pos, &eyePos);
            (void) dbg_0xa;
            sVar1 -= shape_angle.y;
            cXyz sp6c(home.pos);
            cLib_calcTimer(&field_0x1fdc);

            if (
                (field_0x1fdc == 0 && dComIfGp_checkPlayerStatus0(0, 0x100) == 0)
                && (
                    (sp6c - player_pos).absXZ() <= (getSrchCircleR() + 100.0f)
                    && fVar1 <= getSrchCircleR() + 100.0f
                    && std::abs(player_pos.y - current.pos.y) < 300.0f
                )) {
                field_0x150c = 1;
            } else {
                field_0x150c = 0;
            }

            if (field_0x150c) {
                setLookMode(1);
                sp6c = player_pos;
                if (fVar1 < mpHIO->m.field_0x94) {
                    if (abs(sVar1) < cM_deg2s(mpHIO->m.field_0x80)) {
                        setProcess(&daObj_GrA_c::step);
                        field_0x1098 = mpHIO->m.field_0x50;
                        field_0x108c = current.pos;
                        break;
                    }
                }
            } else if ((sp6c - current.pos).absXZ() < 50.0f) {
                setLookMode(1);
                setProcess(&daObj_GrA_c::wait);
                speedF = 0.0f;
                break;
            }

            cLib_addCalc2(&speedF, mpHIO->m.field_0x90, 0.25f, 3.5f);
            cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &sp6c),
                                1, mpHIO->m.field_0x88);
            shape_angle.y = current.angle.y;
            field_0x91a.y = current.angle.y;
            break;
        }

        case 2: {
            break;
        }
    }

    return 1;
}

int daObj_GrA_c::defence(void* param_1) {
    fopAc_ac_c* aTgHitActor_p;
    s16 sVar1;
    switch (field_0xa7c) {
        case 0:
            setBaseMotion(7, 0.0f);
            setFaceMotion(10, 0.0f);
            calcMotion();

            aTgHitActor_p = field_0xab0.getActor();
            JUT_ASSERT(309, NULL != aTgHitActor_p);

            sVar1 = cLib_targetAngleY(&current.pos, &aTgHitActor_p->current.pos);
            current.angle.y = sVar1;
            sVar1 = current.angle.y;
            field_0x91a.y = sVar1;
            shape_angle.y = sVar1;
            field_0x108c = current.pos;
            field_0x1098 = 0.0f;
            break;

        case 1:
            if (field_0x842 != 0) {
                setProcess(&daObj_GrA_c::walk);
            } else {
                cLib_addCalc2(&field_0x1098, mpHIO->m.field_0x50, 0.25f, 20.0f);
                cXyz sp2c(field_0x108c);
                sp2c.x -= field_0x1098 * cM_ssin(current.angle.y);
                sp2c.z -= field_0x1098 * cM_scos(current.angle.y);
                cLib_addCalcPos2(&current.pos, sp2c, 0.8f, 45.0f);

                mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(26));
                mDoMtx_stack_c::multVecZero(&field_0xab8[0].field_0x4);
                field_0xab8[0].field_0x4.y -= 20.0f;
                field_0xab8[0].field_0x10 = current.angle;
                field_0xab8[0].field_0x10.y -= 0x8000;
                field_0xab8[0].field_0x0 = 1;

                mDoMtx_stack_c::copy(mpModelMorf->getModel()->getAnmMtx(23));
                mDoMtx_stack_c::multVecZero(&field_0xab8[1].field_0x4);
                field_0xab8[1].field_0x4.y -= 20.0f;
                field_0xab8[1].field_0x10 = current.angle;
                field_0xab8[1].field_0x10.y -= 0x8000;
                field_0xab8[1].field_0x0 = 1;
                field_0xaa4 = mpHIO->m.field_0x54;
            }
            break;

        case 2:
            field_0xab0.entry(NULL);
    }

    return 1;
}

int daObj_GrA_c::toStone(void* param_1) {
    s16 sVar1;
    f32 var_f31;
    f32 var_f30;
    int var_r27 = 0;
    (void) var_r27;

    switch (field_0xa7c) {
        case 0:
            field_0xaac = 1;
            ppMoveInit();
            field_0xaa8 = 0;
            current.angle.y = shape_angle.y;
            field_0x202c = field_0x842;

            if (mMode != 1 && !dComIfGs_isSaveDunSwitch(0x1D)) {
                dComIfGs_onSaveDunSwitch(0x1D);
            }
            break;

        case 1: {
            switch (field_0xaac) {
                case 1:
                    if (field_0x109c != 0) {
                        cXyz sp70(daPy_getPlayerActorClass()->current.pos);
                        var_f30 = (sp70 - current.pos).absXZ();
                        if (field_0xaa8 != 0) {
                            field_0xaa8--;
                            if (field_0xaa8 <= 0) {
                                field_0x10a0 = 90;
                                field_0xaac = 2;
                            }
                        } else if ((var_f30 > getSrchCircleR() || std::abs(sp70.y - current.pos.y) > 300.0f) || field_0x10c4 != 0) {
                            field_0xaa8 = 30;
                        }
                    } else if (mpModelMorf->checkFrame(0.0f)) {
                        setBaseMotion(4, mpHIO->m.field_0x0c / 2);
                        setFaceMotion(13, mpHIO->m.field_0x0c / 2);
                        calcMotion();
                        mpModelMorf->setFrame(mpModelMorf->getEndFrame());
                        mpModelMorf->setPlaySpeed(1.0f);
                        field_0x109c = 1;
                    }
                    break;

                case 2:
                    if (field_0x10a0 != 0) {
                        cXyz sp7c(0.0f, 0.0f, 20.0f);
                        mDoMtx_stack_c::transS(current.pos);
                        mDoMtx_stack_c::YrotM(field_0x91a.y);
                        mDoMtx_stack_c::multVec(&sp7c, &field_0xab8[4].field_0x4);
                        field_0xab8[4].field_0x10 = field_0x91a;
                        field_0xab8[4].field_0x0 = 1;
                        mSound.startCreatureSoundLevel(Z2SE_GORON_TREMBLE, 0, -1);
                        field_0x10a0--;
                    } else {
                        if (field_0x10c4 != 0 && daPy_py_c::checkNowWolf() == 0) {
                            sVar1 = cM_deg2s(mpHIO->m.field_0x4c);

                            if (strcmp(dComIfGp_getStartStageName(), "F_SP109") == 0) {
                                var_f31 = 95.0f;
                            } else {
                                var_f31 = mpHIO->m.field_0x48;
                            }

                            // reversed in debug?!
                            daPy_getPlayerActorClass()->setRollJump(var_f31 * cM_scos(sVar1), var_f31 * cM_ssin(sVar1),
                                                        daPy_getPlayerActorClass()->shape_angle.y);
                        } else {
                            field_0xf50.OnAtSetBit();
                            field_0x109c = 0;
                        }

                        setBaseMotion(6, 0.0f);
                        setFaceMotion(3, 0.0f);
                        mpModelMorf->setPlaySpeed(1.0f);
                        field_0x10a0 = 0;
                        field_0xaac = 3;
                    }
                    break;

                case 3:
                    if (field_0x842 != 0) {
                        field_0xf50.OffAtSetBit();
                        setProcess(&daObj_GrA_c::wait);
                        setLookMode(1);
                        field_0x1528.field_0xa18 = 0;
                        field_0xaac = 0;
                        health = mpHIO->m.field_0x10;
                        field_0x109c = 0;
                    } else if (mpModelMorf->checkFrame(12.0f)) {
                        field_0xab8[3].field_0x4 = current.pos;
                        field_0xab8[3].field_0x10 = field_0x91a;
                        field_0xab8[3].field_0x0 = 1;
                    }
            }

            break;
        }

        case 2: {
            break;
        }
    }

    field_0x10c4 = 0;
    
    return 1;
}

int daObj_GrA_c::step(void* param_1) {
    switch (field_0xa7c) {
        case 0:
            setBaseMotion(9, mpHIO->m.field_0x0c);
            setFaceMotion(9, -1.0f);
            field_0x1098 = 0.0f;
            field_0x1fe0 = 0;
            field_0x1fdc = 0;
            break;

        case 1:
            if (field_0x842 != 0) {
                field_0x1528.field_0xa18 = mpHIO->m.field_0x98;
                setProcess(&daObj_GrA_c::wait);
                setBaseMotion(8, mpHIO->m.field_0x0c);
                setFaceMotion(8, -1.0f);
            } else if (mpModelMorf->getFrame() >= 25.0f && field_0x1fe0 == 0) {
                field_0x1528.field_0xa18 = mpHIO->m.field_0x98;
                setProcess(&daObj_GrA_c::wait);
                setBaseMotion(13, mpHIO->m.field_0x0c);
                setFaceMotion(8, -1.0f);
                setLookMode(0);
                field_0xce0[0].OffAtSetBit();
                field_0xce0[0].ClrAtHit();
                field_0xce0[1].OffAtSetBit();
                field_0xce0[1].ClrAtHit();
            } else {
                cLib_addCalc2(&speedF, mpHIO->m.field_0x90 / 3.0f, 0.25f, 3.5f);
                cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &daPy_getPlayerActorClass()->current.pos),
                                    4, mpHIO->m.field_0x88 / 4);
                shape_angle.y = current.angle.y;
                field_0x91a.y = current.angle.y;

                if (mpModelMorf->getFrame() == 9.0f) {
                    field_0xce0[1].OnAtSetBit();
                } else {
                    if (mpModelMorf->getFrame() == 15.0f) {
                        field_0xce0[1].OffAtSetBit();
                    } else {
                        if (mpModelMorf->getFrame() == 20.0f) {
                            field_0xce0[0].OnAtSetBit();
                        } else {
                            if (mpModelMorf->getFrame() >= 24.0f) {
                                field_0xce0[0].OffAtSetBit();
                            }
                        }
                    }
                }

                if (field_0xce0[1].ChkAtShieldHit()) {
                    field_0xce0[1].ClrAtHit();
                    field_0xab0.entry(field_0xce0[1].GetAtHitAc());
                    field_0x1528.field_0xa18 = mpHIO->m.field_0x98;

                    setProcess(&daObj_GrA_c::wait);
                    setBaseMotion(11, mpHIO->m.field_0x0c);
                    setFaceMotion(8, -1.0f);
                    setLookMode(0);

                    if (!dComIfGs_isSaveDunSwitch(0x1C)) {
                        dComIfGs_onSaveDunSwitch(0x1C);
                    }
                } else if (field_0xce0[1].ChkAtHit() != 0) {
                    field_0x1fe0 = 1;
                    field_0x1fdc = 60;
                    if (!dComIfGs_isSaveDunSwitch(0x1C)) {
                        dComIfGs_onSaveDunSwitch(0x1C);
                    }
                }

                if (field_0xce0[0].ChkAtShieldHit()) {
                    field_0xab0.entry(field_0xce0[0].GetAtHitAc());
                    field_0x1528.field_0xa18 = mpHIO->m.field_0x98;

                    setProcess(&daObj_GrA_c::wait);
                    setBaseMotion(12, mpHIO->m.field_0x0c);
                    setFaceMotion(8, -1.0f);
                    setLookMode(0);

                    if (!dComIfGs_isSaveDunSwitch(0x1C)) {
                        dComIfGs_onSaveDunSwitch(0x1C);
                    }
                } else if (field_0xce0[0].ChkAtHit() != 0) {
                    field_0x1fe0 = 1;
                    field_0x1fdc = 60;

                    if (!dComIfGs_isSaveDunSwitch(0x1C)) {
                        dComIfGs_onSaveDunSwitch(0x1C);
                    }
                }
            }
            break;

        case 2:
            field_0xce0[0].OffAtSetBit();
            field_0xce0[0].ClrAtHit();
            field_0xce0[1].OffAtSetBit();
            field_0xce0[1].ClrAtHit();
            speedF = 0.0f;
    }

    return 1;
}

int daObj_GrA_c::rollReturn(void* param_1) {
    cXyz sp48;
    f32 fVar1;

    switch (field_0xa7c) {
        case 0:
            setLookMode(0);
            field_0x1fe4 = 0;
            field_0x2008 = 0.0f;
            speedF = 0.0f;

            if (mMode == 1) {
                setBaseMotion(20, mpHIO->m.field_0x0c / 2);
                setFaceMotion(13, mpHIO->m.field_0x0c / 2);
            } else {
                if ((home.pos - current.pos).absXZ() < 150.0f) {
                    setBaseMotion(4, 0.0f);
                    setFaceMotion(13, 0.0f);
                } else {
                    s16 tgt_ang = cLib_targetAngleY(&current.pos, &home.pos);
                    if (abs(tgt_ang - current.angle.y) < 0x4000) {
                        current.angle.y = cLib_targetAngleY(&current.pos, &home.pos);
                        shape_angle.y = field_0x91a.y = current.angle.y;
                        setBaseMotion(4, 0.0f);
                        setFaceMotion(13, 0.0f);
                    } else {
                        current.angle.y = cLib_targetAngleY(&current.pos, &home.pos);
                        shape_angle.y = field_0x91a.y = current.angle.y + 0x8000;
                        setBaseMotion(14, 0.0f);
                        setFaceMotion(13, 0.0f);
                    }
                }
            }

            calcMotion();
            break;

        case 1: {
            switch (field_0x1fe4) {
                case 0:
                    if (field_0xa90 == 4 || field_0xa90 == 20) {
                        if (mpModelMorf->isStop()) {
                            if (mMode == 1 || (home.pos - current.pos).absXZ() < 150.0f) {
                                setProcess(&daObj_GrA_c::toStone);
                                field_0x109c = 1;
                            } else {
                                field_0x1fe4 = 1;
                                setBaseMotion(10, 0.0f);
                                setFaceMotion(13, 0.0f);
                                calcMotion();

                                if (abs(current.angle.y - field_0x91a.y) >= 0x4000) {
                                    mpModelMorf->setFrame(mpModelMorf->getEndFrame());
                                    mpModelMorf->setLoopFrame(mpModelMorf->getEndFrame());
                                }

                                mpModelMorf->setPlaySpeed(0.0f);
                            }
                        } else if (mpModelMorf->checkFrame(14.0f)) {
                            cXyz sp54(0.0f, 0.0f, 20.0f);
                            mDoMtx_stack_c::transS(current.pos);
                            mDoMtx_stack_c::YrotM(field_0x91a.y);
                            mDoMtx_stack_c::multVec(&sp54, &field_0xab8[2].field_0x4);
                            field_0xab8[2].field_0x10 = field_0x91a;
                            field_0xab8[2].field_0x0 = 1;
                        }
                    } else {
                        cXyz sp60(home.pos);
                        cLib_addCalc2(&field_0x2008, mpHIO->m.field_0xa0, 0.05f, 1.0f);
                        cLib_chasePos(&current.pos, sp60, field_0x2008);

                        f32 var_f30 = field_0x2008 * 0.1f;
                        if (cM3d_IsZero(field_0x2008)) {
                            var_f30 = 0.0f;
                        }

                        shape_angle.y = field_0x91a.y = current.angle.y + 0x8000;

                        if (mpModelMorf->isStop()) {
                            if ((home.pos - current.pos).absXZ() < 40.0f) {
                                setProcess(&daObj_GrA_c::toStone);
                                setBaseMotion(4, mpHIO->m.field_0x0c / 2);
                                setFaceMotion(13, mpHIO->m.field_0x0c / 2);
                                calcMotion();
                                mpModelMorf->setFrame(mpModelMorf->getEndFrame());
                                mpModelMorf->setPlaySpeed(1.0f);
                                field_0x109c = 1;
                                break;
                            } else {
                                field_0x1fe4 = 1;
                                setBaseMotion(10, 0.0f);
                                setFaceMotion(13, 0.0f);
                                calcMotion();

                                mpModelMorf->setFrame(mpModelMorf->getEndFrame());
                                mpModelMorf->setLoopFrame(mpModelMorf->getEndFrame());
                                mpModelMorf->setPlaySpeed(-var_f30);
                            }
                        }
                    }
                    break;

                case 1: {
                    cXyz sp6c(home.pos);
                    cLib_addCalc2(&field_0x2008, mpHIO->m.field_0xa0, 0.25f, 3.5f);
                    cLib_chasePos(&current.pos, sp6c, field_0x2008);

                    fVar1 = field_0x2008 * 0.1f;
                    if (cM3d_IsZero(field_0x2008)) {
                        fVar1 = 0.0f;
                    }

                    if (abs(current.angle.y - field_0x91a.y) < 0x4000) {
                        mpModelMorf->setPlaySpeed(fVar1);
                        shape_angle.y = field_0x91a.y = current.angle.y;
                    } else {
                        mpModelMorf->setPlaySpeed(-fVar1);
                        shape_angle.y = field_0x91a.y = current.angle.y + 0x8000;
                    }

                    if ((home.pos - current.pos).absXZ() < 40.0f) {
                        setProcess(&daObj_GrA_c::toStone);
                    }

                    break;
                }
            }

            break;
        }

        case 2:
            field_0x2008 = 0.0f;
            field_0x1fe4 = -1;
    }

    return 1;
}

int daObj_GrA_c::soldierDemoSkip() {
    setLookMode(1);
    setBaseMotion(0, 0.0f);
    setFaceMotion(13, 0.0f);
    field_0x109c = 0;
    setProcess(&daObj_GrA_c::wait);
    mDemoCamMode = 0;
    field_0x209c = 0;
    return 1;
}

int daObj_GrA_c::soldierDemo(void* param_1) {
    cXyz sp1c;

    switch (field_0xa7c) {
        case 0:
            setBaseMotion(0, mpHIO->m.field_0x0c);
            setFaceMotion(13, -1.0f);
            setLookMode(2);
            mDemoCamMode = 0;
            field_0x209c = 1;
            break;

        case 1:
            switch (mDemoCamMode) {
                case 1:
                    if (field_0xa90 == 17) {
                        setBaseMotion(20, mpHIO->m.field_0x0c / 2);
                        setFaceMotion(13, mpHIO->m.field_0x0c / 2);
                        setLookMode(0);
                    } else if (mpModelMorf->isStop()) {
                        field_0x109c = 1;
                    } else if (mpModelMorf->checkFrame(14.0f)) {
                        cXyz sp28(0.0f, 0.0f, 20.0f);
                        mDoMtx_stack_c::transS(current.pos);
                        mDoMtx_stack_c::YrotM(field_0x91a.y);
                        mDoMtx_stack_c::multVec(&sp28, &field_0xab8[2].field_0x4);
                        field_0xab8[2].field_0x10 = field_0x91a;
                        field_0xab8[2].field_0x0 = 1;
                    }
                    break;

                case 3:
                    if (field_0x10a0 != 0) {
                        cXyz sp34(0.0f, 0.0f, 20.0f);
                        mDoMtx_stack_c::transS(current.pos);
                        mDoMtx_stack_c::YrotM(field_0x91a.y);
                        mDoMtx_stack_c::multVec(&sp34, &field_0xab8[4].field_0x4);
                        field_0xab8[4].field_0x10 = field_0x91a;
                        field_0xab8[4].field_0x0 = 1;

                        mSound.startCreatureSoundLevel(Z2SE_GORON_TREMBLE, 0, -1);

                        field_0x10a0--;
                        if (field_0x10a0 == 0) {
                            setBaseMotion(6, 0.0f);
                            setFaceMotion(3, 0.0f);
                            mpModelMorf->setPlaySpeed(1.0f);
                            field_0x10a0 = 0;
                            field_0x109c = 0;
                            mDemoCamMode = 4;
                        }
                    } else {
                        field_0x10a0 = 90;
                    }
                    break;

                case 4:
                    if (mpModelMorf->isStop() && field_0xa90 != 0) {
                        setLookMode(1);
                        setBaseMotion(0, mpHIO->m.field_0x0c);
                        setFaceMotion(13, -1.0f);
                    } else if (mpModelMorf->checkFrame(12.0f)) {
                        field_0xab8[3].field_0x4 = current.pos;
                        field_0xab8[3].field_0x10 = field_0x91a;
                        field_0xab8[3].field_0x0 = 1;
                    }
            }
            break;

        case 2:
            mDemoCamMode = 0;
            field_0x209c = 0;
    }

    return 1;
}

f32 daObj_GrA_c::getSrchLinkR() {
    int iVar1 = fopAcM_GetParam(this) & 0x7F;
    if (iVar1 == 0) {
        return mpHIO->m.field_0x70 * 100.0f;
    }

    return iVar1 * 100;
}

f32 daObj_GrA_c::getSrchGraR() {
    if (field_0x200c == 0) {
        return mpHIO->m.field_0x74 * 100.0f;
    }

    return field_0x200c * 100;
}

f32 daObj_GrA_c::getSpeed() {
    u32 uVar1 = fopAcM_GetParam(this) >> 14 & 63;
    if (uVar1 == 63) {
        return mpHIO->m.field_0x84;
    }

    return uVar1;
}

int daObj_GrA_c::standRoll(void* param_1) {
    cXyz sp38;
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (field_0xa7c) {
        case 0:
            setBaseMotion(0, mpHIO->m.field_0x0c);
            setFaceMotion(13, -1.0f);
            setLookMode(1);
            field_0x2014 = 0;
            break;

        case 1:
            if (field_0x2031 == 0 && player != NULL) {
                if (player->current.pos.y > current.pos.y) {
                    fopAcM_delete(this);
                } else {
                    field_0x2031 = 1;
                }
            }

            if (getTagNo() != 0) {
                if (field_0x2032 == 0) {
                    fopAc_ac_c* gra_base_p = (fopAc_ac_c*) fpcM_Search(s_sub, this);
                    if (gra_base_p != NULL) {
                        daTagGra_c* graTag_p = (daTagGra_c*) gra_base_p;
                        mGraPos = graTag_p->getGraPos();
                        mGraExtent = graTag_p->getGraExtent();
                        mGraHeight = graTag_p->getGraHeight();
                        fopAcM_delete(gra_base_p);
                        mGraMode = graTag_p->getMode();
                        field_0x2032 = 1;
                    }
                } else if (mGraMode == 1) {
                    sp38 = player->current.pos - current.pos;
                    sp38.y = 2.0f * sp38.y;
                    if (sp38.abs() < getSrchLinkR() && player->checkPlayerFly() == 0 && !player->checkClimbMove() &&
                        dComIfGp_checkPlayerStatus0(0, 8) == 0) {
                        setProcess(&daObj_GrA_c::rollDemo);
                        dComIfGoat_SetThrow(this);
                    }
                } else {
                    sp38 = player->current.pos - mGraPos;
                    if (sp38.absXZ() < mGraExtent && player->checkPlayerFly() == 0 && sp38.y > -30.0f && sp38.y < mGraHeight &&
                        !player->checkClimbMove() && dComIfGp_checkPlayerStatus0(0, 8) == 0) {
                        setProcess(&daObj_GrA_c::rollDemo);
                        dComIfGoat_SetThrow(this);
                    }
                }
            } else {
                sp38 = player->current.pos - current.pos;
                sp38.y = sp38.y * 2.0f;
                if (sp38.abs() < getSrchLinkR() && player->checkPlayerFly() == 0 && !player->checkClimbMove() &&
                    dComIfGp_checkPlayerStatus0(0, 8) == 0) {
                    setProcess(&daObj_GrA_c::rollDemo);
                    dComIfGoat_SetThrow(this);
                }
            }
            break;

        case 2:
            break;
    }

    return 1;
}

int daObj_GrA_c::rollDemo(void* param_1) {
    switch (field_0xa7c) {
        case 0:
            if (isFirstGra()) {
                mDemoCamMode = 1;
            }

            field_0x209c = 1;
            break;

        case 1:
            if (isFirstGra()) {
                switch (field_0x2000) {
                    case -1:
                        cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos,
                                            &daPy_getPlayerActorClass()->current.pos),
                                            4, mpHIO->m.field_0x88);
                        shape_angle.y = current.angle.y;
                        field_0x91a.y = current.angle.y;

                        if (mDemoCamMode >= 5) {
                            field_0x848 = 4;
                            evtcutTalk(-1, 1);
                            field_0x2000 = 0;
                        }
                        break;

                    case 0:
                        if (evtcutTalk(-1, 0) != 0) {
                            setBaseMotion(20, mpHIO->m.field_0x0c);
                            setFaceMotion(13, -1.0f);
                            setLookMode(0);
                            field_0x2000 = 1;
                        }

                        field_0x2056 = 0;
                        break;

                    case 1:
                        if (mpModelMorf->checkFrame(14.0f)) {
                            cXyz sp30(0.0f, 0.0f, 20.0f);
                            mDoMtx_stack_c::transS(current.pos);
                            mDoMtx_stack_c::YrotM(field_0x91a.y);
                            mDoMtx_stack_c::multVec(&sp30, &field_0xab8[2].field_0x4);
                            field_0xab8[2].field_0x10 = field_0x91a;
                            field_0xab8[2].field_0x0 = 1;

                            setLookMode(0);
                            setBaseMotion(10, mpHIO->m.field_0x0c);
                            setFaceMotion(13, -1.0f);
                            calcMotion();

                            mpModelMorf->setEndFrame(29.0f);
                            mpModelMorf->setPlaySpeed(0.0f);

                            field_0x2000 = 2;
                        }
                        break;

                    case 2:
                        f32 playSpeed = mpModelMorf->getPlaySpeed();
                        if (playSpeed < 1.0f) {
                            cLib_addCalc2(&playSpeed, 1.0f, 0.25f, 3.5f);
                            mpModelMorf->setPlaySpeed(playSpeed);
                        }

                        if (mDemoCamMode) {
                            break;
                        }

                        setProcess(&daObj_GrA_c::rollAttack);
                }
            } else if (field_0xa90 != 4) {
                setBaseMotion(4, mpHIO->m.field_0x0c);
                setFaceMotion(13, -1.0f);
                setLookMode(1);
            } else if (mpModelMorf->checkFrame(14.0f)) {
                cXyz sp3c(0.0f, 0.0f, 20.0f);
                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(field_0x91a.y);
                mDoMtx_stack_c::multVec(&sp3c, &field_0xab8[2].field_0x4);
                field_0xab8[2].field_0x10 = field_0x91a;
                field_0xab8[2].field_0x0 = 1;

                setLookMode(0);
                setBaseMotion(10, mpHIO->m.field_0x0c);
                setFaceMotion(13, -1.0f);
                calcMotion();

                mpModelMorf->setEndFrame(29.0f);
                mpModelMorf->setPlaySpeed(0.0f);

                setProcess(&daObj_GrA_c::rollAttack);
            }
            break;

        case 2:
            field_0x2000 = -1;
            field_0x209c = 0;
    }

    demo_camera();
    return 1;
}

int daObj_GrA_c::rollAttack(void* param_1) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp3c;
    f32 fVar1, fVar2;
    bool bVar1;

    switch (field_0xa7c) {
        case 0:
            field_0x1fe9 = 0;
            setLookMode(0);
            field_0x2005 = 0;
            field_0x209c = 1;
            break;

        case 1: {
            if (fallAbyssCheck()) {
                break;
            }

            cXyz sp48 = player->current.pos;
            fVar1 = (sp48 - current.pos).abs();

            if (fVar1 <= 1800.0f) {
                if (field_0x2005 != 2) {
                    dComIfGp_getVibration().StartQuake(2, 15, cXyz(0.0f, 1.0f, 0.0f));
                    field_0x2005 = 2;
                }
            } else if (fVar1 <= 2000.0f) {
                if (field_0x2005 != 1) {
                    dComIfGp_getVibration().StartQuake(1, 15, cXyz(0.0f, 1.0f, 0.0f));
                    field_0x2005 = 1;
                }
            } else  {
                if (dComIfGp_getVibration().CheckQuake()) {
                    dComIfGp_getVibration().StopQuake(31);
                }

                field_0x2005 = 0;
            }

            rollLanding();

            bVar1 = false;
            if (field_0x201c < 10 && field_0x1fe9 == 0 && fVar1 < getSrchGraR()) {
                cXyz sp54(current.pos);
                sp54.y += 100.0f;
                field_0x1f4c.Set(&sp54, &player->eyePos, this);
                field_0x1f4c.SetActorPid(base.base.id);

                s16 sVar1 = cLib_targetAngleX(&sp54, &player->eyePos);
                if (isFirstGra() || !dComIfG_Bgsp().LineCross(&field_0x1f4c)) {
                    if (!player->checkClimbMove() && dComIfGp_checkPlayerStatus0(0, 8) == 0 && sVar1 > -0x2000 && sVar1 < 0x2000) {
                        field_0x150c = 1;
                        bVar1 = true;

                        if (field_0x2014 == 0 && (isFirstGra() || player->checkEquipHeavyBoots())) {
                            player->setGoronSideMove(this);
                            field_0x2014 = 1;
                        }
                    }
                }
            }

            if (isFirstGra()) {
                if (player->current.pos.y < -4500.0f) {
                    setProcess(&daObj_GrA_c::rollAttacked);
                    return 1;
                }
            } else {
                cXyz sp60(current.pos);
                sp60.y += 100.0f;
                field_0x1f4c.Set(&sp60, &player->eyePos, this);
                field_0x1f4c.SetActorPid(base.base.id);

                if (player->checkPlayerFly() == 0 && (player->current.pos.y - current.pos.y) > 50.0f) {
                    player->offGoronSideMove();
                    field_0x2014 = 1;
                    field_0x1fe9 = 1;
                }
            }

            if (!bVar1) {
                field_0x150c = 0;
                if (field_0x2014 == 0 && (player->current.pos.y - current.pos.y) > 0.0f) {
                    field_0x1fe9 = 1;
                    player->offGoronSideMove();
                    field_0x2014 = 1;
                }
            }

            if (field_0x150c != 0 && field_0x1fe9 == 0) {
                setCarryStatus();
                if (checkThrow()) {
                    return 1;
                }

                if (field_0x1fe9 == 0) {
                    cXyz sp6c(sp48);
                    s16 dbg_0x10 = current.angle.y;
                    cLib_chaseF(&speedF, getSpeed(), mpHIO->m.field_0xa8);

                    if (field_0x201c < 10) {
                        cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &sp6c), 
                                            1, mpHIO->m.field_0x88);
                        shape_angle.y = current.angle.y;
                        field_0x91a.y = current.angle.y;
                    }
                }

                BOOL bVar2 = FALSE;
                if (field_0x1528.isPath()) {
                    while (!bVar2) {
                        if (field_0x1528.chkPnt(current.pos) == 0) break;

                        if (field_0x1528.setNextPnt()) {
                            bVar2 = TRUE;
                            field_0x1fe9 = 1;
                            field_0x2014 = 1;
                        }
                    }
                } else {
                    bVar2 = TRUE;
                }
            } else {
                if (field_0x1fe9 == 1 && !dComIfGp_event_runCheck() && mDemoCamMode == 0) {
                    if (dComIfGoat_GetThrow() == this) {
                        dComIfGoat_SetThrow(NULL);
                    }

                    if (field_0x2030 == 1 && field_0x2005 == 0 && fopAcM_otherBgCheck(this, dComIfGp_getPlayer(0))) {
                        fopAcM_delete(this);
                        if (dComIfGp_getVibration().CheckQuake()) {
                            dComIfGp_getVibration().StopQuake(31);
                        }

                        return 1;
                    }
                }

                BOOL o_bool = false;
                if (field_0x1528.isPath()) {
                    while (!o_bool) {
                        if (field_0x1528.chkPnt(current.pos) == 0) break;

                        if (field_0x1528.setNextPnt()) {
                            o_bool = true;
                        }
                    }
                } else {
                    o_bool = true;
                }

                if (!o_bool) {
                    cXyz sp78(field_0x1528.getPntPos(field_0x1528.getIdx() & 0xFFFF));
                    s16 cur_ang = current.angle.y;
                    cLib_chaseF(&speedF, getSpeed(), mpHIO->m.field_0xa8);

                    if (field_0x201c < 10) {
                        cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &sp78),
                                            8, field_0x1528.field_0xa14);
                        field_0x1528.field_0xa14 = abs(current.angle.y - cur_ang);

                        cLib_addCalcAngleS2(&field_0x1528.field_0xa14, mpHIO->m.field_0x88, 8, 0x40);
                        shape_angle.y = current.angle.y;
                        field_0x91a.y = current.angle.y;
                    }
                } else if (cM3d_IsZero(speedF)) {
                    speedF = 0.0f;
                    if (mAcch.ChkGroundHit()) {
                        if (modeChangeCheck()) {
                            break;
                        }
                    }
                } else {
                    cLib_addCalc2(&speedF, 0.0f, 0.25f, 3.5f);
                    if (mpModelMorf->isLoop()) {
                        speedF = 0.0f;
                    } else if (speedF < 1.0f) {
                        speedF = 1.0f;
                    }

                    if (mpModelMorf->getFrame() < 2.0f) {
                        speedF = 0.0f;
                        if (mAcch.ChkGroundHit()) {
                            if (modeChangeCheck()) {
                                break;
                            }
                        }
                    }
                }
            }

            fVar2 = speedF * 0.1f;
            if (fVar2 < 0.5f) {
                mpModelMorf->setPlaySpeed(0.5f);
            } else {
                mpModelMorf->setPlaySpeed(fVar2);
            }

            if (field_0x1fe9 == 0 && field_0xf50.ChkCoHit() != 0) {
                fopAc_ac_c* hitobj_p = dCc_GetAc(field_0xf50.GetCoHitObj()->GetAc());
                if (fopAcM_GetName(hitobj_p) == PROC_ALINK) {
                    field_0x1fe9 = 1;
                    player->offGoronSideMove();
                    player->offGoronUpStopCancel();

                    s16 sVar1;
                    if (isFirstGra()) {
                        sVar1 = home.angle.y;
                        if (player->checkEquipHeavyBoots() != 0) {
                            player->setThrowDamage(sVar1, 30.0f, 40.0f, 1, 0, 0);
                        } else {
                            player->setThrowDamage(sVar1, mpHIO->m.field_0xb8,
                                                    mpHIO->m.field_0xbc, 2, 0, 0);
                        }

                        setProcess(&daObj_GrA_c::rollAttacked);
                    } else {
                        sVar1 = shape_angle.y;
                        if (player->checkEquipHeavyBoots() != 0) {
                            player->setThrowDamage(sVar1, 30.0f, 40.0f, 1, 0, 0);
                        } else {
                            player->setThrowDamage(sVar1, mpHIO->m.field_0xb8,
                                                    mpHIO->m.field_0xbc, 2, 0, 0);
                        }

                        field_0x1fd8 = 60;
                    }
                }
            }

            break;
        }

        case 2:
            dComIfGp_getVibration().StopQuake(31);
            field_0x209c = 1;
            break;
    }

    return 1;
}

int daObj_GrA_c::rollAttacked(void* param_1) {
    BOOL bVar1;
    s16 sVar1;
    daPy_py_c* player = daPy_getPlayerActorClass();
    attention_info.flags = 0;

    switch (field_0xa7c) {
        case 0:
            speedF = 0.0f;
            field_0x2018 = -1;
            mDemoCamMode = 1;
            field_0x209c = 1;
            break;

        case 1:
            switch (field_0x2018) {
            case -1:
                if (mpModelMorf->checkFrame(0.0f)) {
                    mpModelMorf->setPlaySpeed(0.0f);
                    field_0x2018 = 0;
                }
                // fallthrough
            case 0:
                if (player->checkPlayerFly() == 0) {
                    if (player->current.pos.y < -4500.0f) {
                        field_0x2018 = 1;
                        old.pos = current.pos = mGraPos;
                        current.angle.y = cLib_targetAngleY(&current.pos, &player->current.pos);
                        shape_angle.y = current.angle.y;
                        field_0x91a.y = current.angle.y;
                        field_0x1fec = 0;
                        setBaseMotion(0, 0.0f);
                        setFaceMotion(13, 0.0f);
                        setLookMode(1);
                        mpModelMorf->setPlaySpeed(1.0f);
                        player->offGoronSideMove();

                        if (dComIfGoat_GetThrow() == this) {
                            dComIfGoat_SetThrow(NULL);
                        }
                    } else {
                        field_0x1fec = 60;
                        field_0x2018 = 5;
                        field_0x1528.reverseDir();
                        current.angle.y += 0x8000;
                        mpModelMorf->setLoopFrame(mpModelMorf->getEndFrame());
                        player->setGoronSideMove(this);
                        field_0x2014 = 1;
                        mDemoCamMode = 7;
                    }
                }

                field_0x2056 = 0;
                break;

                case 1:
                    if (cLib_calcTimer(&field_0x1fec) == 0) {
                        if (dComIfGs_getLife() == 0) {
                            setProcess(&daObj_GrA_c::wait);
                            speedF = 0.0f;
                            current.pos = home.pos;
                            old.pos = home.pos;
                            current.angle.y = home.angle.y;
                            shape_angle.y = current.angle.y;
                            field_0x91a.y = current.angle.y;
                            mDemoCamMode = 7;
                        } else {
                            field_0x2018 = 2;
                        }
                    }
                    break;

                case 2:
                    if (mDemoCamMode >= 5) {
                        field_0x848 = 5;
                        evtcutTalk(-1, 1);
                        field_0x2018 = 3;
                    }
                    break;

                case 3:
                    if (evtcutTalk(-1, 0) != 0) {
                        setLookMode(0);
                        field_0x2018 = 4;
                        setBaseMotion(1, 0.0f);
                        setFaceMotion(13, 0.0f);
                    }

                    field_0x2056 = 0;
                    break;

                case 4:
                    if (mDemoCamMode != 0) {
                        cLib_addCalc2(&speedF, mpHIO->m.field_0x90 / 2, 0.25f, 3.5f);
                        cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &home.pos),
                                            1, mpHIO->m.field_0x88);
                        shape_angle.y = current.angle.y;
                        field_0x91a.y = current.angle.y;
                    } else {
                        setProcess(&daObj_GrA_c::wait);
                        speedF = 0.0f;
                        current.pos = home.pos;
                        old.pos = home.pos;
                        current.angle.y = home.angle.y;
                        shape_angle.y = current.angle.y;
                        field_0x91a.y = current.angle.y;
                    }
                    break;

                case 5:
                    if (cLib_calcTimer(&field_0x1fec) == 0) {
                        current.angle.y += 0x8000;
                        shape_angle.y = current.angle.y;
                        field_0x91a.y = current.angle.y;
                        setProcess(&daObj_GrA_c::rollAttack);
                        field_0x1528.reverseDir();
                        mpModelMorf->setLoopFrame(mpModelMorf->getStartFrame());
                    } else {
                        bVar1 = false;
                        if (field_0x1528.isPath()) {
                            while (!bVar1) {
                                if (field_0x1528.chkPnt(current.pos) == 0) break;

                                if (field_0x1528.setNextPnt()) {
                                    bVar1 = true;
                                }
                            }
                        } else {
                            bVar1 = true;
                        }

                    if (!bVar1) {
                        cXyz sp3c = field_0x1528.getPntPos(field_0x1528.getIdx() & 0xFFFF);
                        sVar1 = current.angle.y;
                        cLib_chaseF(&speedF, getSpeed(), mpHIO->m.field_0xa8);
                        cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &sp3c), 
                                            8, field_0x1528.field_0xa14);
                        field_0x1528.field_0xa14 = abs(current.angle.y - sVar1);
                        cLib_addCalcAngleS2(&field_0x1528.field_0xa14, mpHIO->m.field_0x88,
                                            8, 0x40);
                        shape_angle.y = current.angle.y + 0x8000;
                        field_0x91a.y = current.angle.y + 0x8000;
                    } else {
                        cLib_addCalc2(&speedF, 0.0f, 0.25f, 3.5f);
                    }

                    f32 play_speed = speedF * 0.1f;
                    mpModelMorf->setPlaySpeed(-play_speed);
                }
            }
            break;

        case 2:
            field_0x2014 = 0;
            field_0x209c = 0;
    }

    rollAttackedDemo();
    return 1;
}

int daObj_GrA_c::rollEscape(void* param_1) {
    BOOL bVar1;
    cXyz sp68;
    s16 sVar1;

    switch (field_0xa7c) {
        case 0:
            field_0x2005 = 0;
            mpModelMorf->setLoopFrame(mpModelMorf->getEndFrame());
            field_0x209c = 1;
            break;

        case 1: {
            cXyz player_pos = daPy_getPlayerActorClass()->current.pos;
            f32 fVar1 = (player_pos - current.pos).absXZ();
            if (fVar1 <= 1800.0f) {
                if (field_0x2005 != 2) {
                    dComIfGp_getVibration().StartQuake(2, 15, cXyz(0.0f, 1.0f, 0.0f));
                    field_0x2005 = 2;
                }
            } else if (fVar1 <= 2000.0f) {
                if (field_0x2005 != 1) {
                    dComIfGp_getVibration().StartQuake(1, 15, cXyz(0.0f, 1.0f, 0.0f));
                    field_0x2005 = 1;
                }
            } else {
                if (dComIfGp_getVibration().CheckQuake()) {
                    dComIfGp_getVibration().StopQuake(31);
                }

                field_0x2005 = 0;
            }

            rollLanding();
            if (!dComIfGp_event_runCheck() || dComIfGp_getEvent().isOrderOK()) {
                if (dComIfGoat_GetThrow() == this) {
                    dComIfGoat_SetThrow(0);
                    if (isFirstGra()) {
                        mDemoCamMode = 3;
                        graThrowDemo();
                        daNpcF_onEvtBit(0x52);
                    }
                }

                if (field_0x2030 == 1 && field_0x2005 == 0 && 
                    (fopAcM_otherBgCheck(this, dComIfGp_getPlayer(0)) || fopAcM_CheckCondition(this, 4) != 0)) {
                    if (dComIfGp_getVibration().CheckQuake()) {
                        dComIfGp_getVibration().StopQuake(31);
                    }

                    fopAcM_delete(this);
                    return 1;
                }
            }

            bVar1 = FALSE;
            if (field_0x1528.isPath()) {
                while (!bVar1) {
                    if (field_0x1528.chkPnt(current.pos) == 0) break;

                    if (field_0x1528.setNextPnt()) {
                        bVar1 = TRUE;
                    }
                }
            } else {
                bVar1 = TRUE;
            }

            int iVar2;
            if (!bVar1) {
                cXyz vec_0x48 = field_0x1528.getPntPos(field_0x1528.getIdx() & 0xFFFF);
                sVar1 = current.angle.y;
                cLib_chaseF(&speedF, getSpeed(), mpHIO->m.field_0xa8);
                cLib_addCalcAngleS2(&current.angle.y, cLib_targetAngleY(&current.pos, &vec_0x48), 
                                    8, field_0x1528.field_0xa14);
                field_0x1528.field_0xa14 = abs(current.angle.y - sVar1);
                cLib_addCalcAngleS2(&field_0x1528.field_0xa14, mpHIO->m.field_0x88, 8, 0x40);
                cLib_addCalcAngleS2(&shape_angle.y, current.angle.y + 0x8000, 8, 0x1000);
                field_0x91a.y = shape_angle.y;
            } else if (cM3d_IsZero(speedF)) {
                speedF = 0.0f;
                if (mAcch.ChkGroundHit()) {
                    if (modeChangeCheck()) {
                        break;
                    }
                }
            } else {
                cLib_addCalc2(&speedF, 0.0f, 0.25f, 3.5f);
                if (mpModelMorf->isLoop()) {
                    speedF = 0.0f;
                } else if (speedF < 1.0f) {
                    speedF = 1.0f;
                }

                if (mpModelMorf->getFrame() < 2.0f) {
                    speedF = 0.0f;
                    if (mAcch.ChkGroundHit()) {
                        if (modeChangeCheck()) {
                            break;
                        }
                    }
                }
            }

            f32 var_f30 = speedF * 0.1f;
            if (cM3d_IsZero(speedF)) {
                var_f30 = 0.0f;
            }

            mpModelMorf->setPlaySpeed(-var_f30);
            break;
        }

        case 2:
            dComIfGp_getVibration().StopQuake(31);
            field_0x209c = 0;
    }

    graThrowDemo();
    return 1;
}

void daObj_GrA_c::rollLanding() {
    if (mAcch.ChkGroundLanding() && field_0x201c >= 10) {
        cXyz sp28(0.0f, 0.0f, 20.0f);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(field_0x91a.y);
        mDoMtx_stack_c::multVec(&sp28, &field_0xab8[5].field_0x4);
        field_0xab8[5].field_0x10 = field_0x91a;
        field_0xab8[5].field_0x0 = 1;

        dComIfGp_getVibration().StartShock(8, 15, cXyz(0.0f, 1.0f, 0.0f));
        mSound.startCreatureSound(Z2SE_GORON_ROLL_BOUND, 0, -1);
    }
}

int daObj_GrA_c::thrown(void* param_1) {
    int rv = 0;

    switch (field_0xa7c) {
        case 0:
            fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
            field_0x1ff0 = 1;
            mDemoCamMode = 0;
            field_0x209c = 1;
            break;

        case 1:
            if (field_0x1ff8 != 0) {
                field_0x1ff8--;
            }

            switch (field_0x1ff0) {
                case 1:
                    executeCrazyBeforeCatch();
                    break;

                case 2:
                    executeCrazyCatch();
                    break;

                case 3:
                    executeCrazyThrow();
                    break;

                case 4:
                    executeCrazyJumping();
            }

            rv = 1;
            break;

        case 2:
            field_0x209c = 0;
    }

    graThrowDemo();
    return rv;
}

void daObj_GrA_c::demo_camera() {
    f32 fVar1;
    int prm_short;
    daPy_py_c* player = daPy_getPlayerActorClass();
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    dCamera_c* camBody = dCam_getBody();

    switch (mDemoCamMode) {
        case 0: {
            break;
        }

        case 1: {
            if (!eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(this, 0, 0xFFEF, 0);
                eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            mDemoCamMode = 2;
            field_0x2056 = 0;
            mDemoCamFovy = field_0x208c = camBody->Fovy();
            mDemoCamCenter = field_0x2080 = camBody->Center();
            mDemoCamEye = field_0x2074 = camBody->Eye();
            camera->mCamera.SetTrimSize(2);
        }
        // fallthrough intentional
        case 2: {
            prm_short = mpHIO->m.field_0xac;
            if (field_0x2056 >= prm_short) {
                mDemoCamMode++;
                field_0x2056 = 0;
            } else {
                cXyz sp40 = current.pos;
                sp40.y += 150.0f;
                cLib_addCalcPos2(&mDemoCamCenter, sp40, 0.1f, (field_0x2080 - sp40).abs() / 25.0f);
                break;
            }
        }
        // fallthrough intentional
        case 3: {
            prm_short = mpHIO->m.field_0xae;
            if (field_0x2056 >= prm_short) {
                mDemoCamMode++;
                field_0x2056 = 0;
            } else {
                cXyz sp4c = current.pos;
                sp4c.y += 150.0f;
                cLib_addCalcPos2(&mDemoCamCenter, sp4c, 0.1f, (field_0x2080 - sp4c).abs() / 25.0f);
                break;
            }
        }
        // fallthrough intentional
        case 4: {
            prm_short = mpHIO->m.field_0xb0;
            if (field_0x2056 >= prm_short) {
                mDemoCamMode++;
                field_0x2056 = 0;
            } else {
                mDoMtx_stack_c::YrotS(shape_angle.y);
                cXyz sp58;
                sp58.x = 0.0f;
                sp58.y = 250.0f;
                sp58.z = 500.0f;
                mDoMtx_stack_c::multVec(&sp58, &sp58);
                sp58 += current.pos;
                cLib_addCalcPos2(&mDemoCamEye, sp58, 0.1f, (field_0x2074 - sp58).abs() / 25.0f);

                cXyz sp64 = current.pos;
                sp64.y += 150.0f;
                cLib_addCalcPos2(&mDemoCamCenter, sp64, 0.1f, (field_0x2080 - sp64).abs() / 25.0f);

                mDemoCamFovy += (55.0f - field_0x208c) / prm_short;
                break;
            }
        }
        // fallthrough intentional
        case 5: {
            prm_short = mpHIO->m.field_0xb2;
            if (field_0x2056 >= prm_short) {
                mDemoCamMode++;
                field_0x2056 = 0;
                csXyz cStack_144 = player->current.angle;
                cStack_144.y = cLib_targetAngleY(&player->current.pos, &current.pos);
                player->setPlayerPosAndAngle(&player->current.pos, &cStack_144);
                field_0x2074 = mDemoCamEye;
                field_0x2080 = mDemoCamCenter;
            } else {
                mDoMtx_stack_c::YrotS(shape_angle.y);
                cXyz sp70;
                sp70.x = 0.0f;
                sp70.y = 250.0f;
                sp70.z = 500.0f;
                mDoMtx_stack_c::multVec(&sp70, &sp70);
                sp70 += current.pos;
                cLib_addCalcPos2(&mDemoCamEye, sp70, 0.1f, (field_0x2074 - sp70).abs() / 25.0f);

                cXyz sp7c = current.pos;
                sp7c.y += 150.0f;
                cLib_addCalcPos2(&mDemoCamCenter, sp7c, 0.1f, (field_0x2080 - sp7c).abs() / 25.0f);
                break;
            }
        }
        // fallthrough intentional
        case 6: {
            prm_short = mpHIO->m.field_0xb4;
            if (field_0x2056 >= prm_short) {
                camera->mCamera.Reset(mDemoCamCenter, mDemoCamEye);
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                dComIfGp_event_reset();
                mDemoCamMode = 0;
                field_0x2056 = 0;
            } else {
                cXyz sp88 = player->current.pos;
                sp88.y += 150.0f;
                cLib_chasePos(&mDemoCamCenter, sp88, (field_0x2080 - sp88).abs() / prm_short);

                mDoMtx_stack_c::YrotS(player->current.angle.y);
                cXyz sp94;
                sp94.x = 0.0f;
                sp94.y = 210.0f;
                sp94.z = -335.0f;
                mDoMtx_stack_c::multVec(&sp94, &sp94);
                sp94 += player->current.pos;
                cLib_chasePos(&mDemoCamEye, sp94, (field_0x2074 - sp94).abs() / prm_short);
                mDemoCamFovy += 17.0f / prm_short;
            }
        }
    }

    if (!mDemoCamMode) {
        return;
    }

    camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
    field_0x2056++;
}

void daObj_GrA_c::rollAttackedDemo() {
    f32 fVar1;

    daPy_py_c* player = daPy_getPlayerActorClass();
    int cmp_val;
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    dCamera_c* camBody = dCam_getBody();

    switch (mDemoCamMode) {
        case 1:
            if (!eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(this, 0, 0xFFEF, 0);
                eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            camera->mCamera.Stop();
            mDemoCamMode = 2;
            field_0x2056 = 0;
            mDemoCamFovy = field_0x208c = camBody->Fovy();
            mDemoCamCenter = field_0x2080 = camBody->Center();

            mDemoCamEye = field_0x2074 = camBody->Eye();
            // fallthrough
        case 2:
            if (!dComIfGp_event_runCheck() && !eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(this, 0, 0xFFEF, 0);
                eventInfo.onCondition(dEvtCnd_CANDEMO_e);
                return;
            }

            cmp_val = 20;
            if (field_0x2056 >= cmp_val) {
                mDemoCamMode = 4;
                field_0x2056 = 0;
                mDemoCamCenter = field_0x2080 = camBody->Center();
                mDemoCamEye = field_0x2074 = camBody->Eye();

                camera->mCamera.SetTrimSize(2);
            } else {
                mDemoCamCenter += player->current.pos - player->old.pos;
                mDemoCamCenter.y -= 2.0f;
                mDemoCamEye += player->current.pos - player->old.pos;
                break;
            }
            // fallthrough
        case 4: {
            cmp_val = 30;
            if (field_0x2056 >= cmp_val) {
                ++mDemoCamMode;
                field_0x2056 = 0;
            } else {
                mDoMtx_stack_c::YrotS(player->current.angle.y);
                cXyz sp44(0.0f, 150.0f, 250.0f);
                mDoMtx_stack_c::multVec(&sp44, &sp44);
                sp44 += player->current.pos;
                cXyz sp50 = current.pos;
                sp50.y += 150.0f;
                cLib_addCalcPos2(&mDemoCamCenter, sp50, 0.15f, (sp44 - sp50).abs() / 60.0f);
                break;
            }
        }
        // fallthrough
        case 5: {
            cmp_val = mpHIO->m.field_0xb2;
            if (field_0x2056 >= cmp_val) {
                ++mDemoCamMode;
                field_0x2056 = 0;
                field_0x2074 = mDemoCamEye;
                field_0x2080 = mDemoCamCenter;
                csXyz cStack_118 = player->current.angle;
                cStack_118.y = cLib_targetAngleY(&player->current.pos, &current.pos);
                player->setPlayerPosAndAngle(&player->current.pos, &cStack_118);
            } else {
                mDoMtx_stack_c::YrotS(player->current.angle.y);
                cXyz sp2c(0.0f, 150.0f, 250.0f);
                mDoMtx_stack_c::multVec(&sp2c, &sp2c);
                sp2c += player->current.pos;
                cXyz sp38 = current.pos;
                sp38.y += 150.0f;
                cLib_addCalcPos2(&mDemoCamCenter, sp38, 0.1f, (sp2c - sp38).abs() / 60.0f);
                break;
            }
        }

        case 6:
            cmp_val = 60;
            if (field_0x2056 >= cmp_val) {
                mDemoCamMode = 7;
            } else {
                mDoMtx_stack_c::YrotS(player->current.angle.y);
                cXyz sp5c(0.0f, 100.0f, -300.0f);
                mDoMtx_stack_c::multVec(&sp5c, &sp5c);
                sp5c += player->current.pos;
                cLib_addCalcPos2(&mDemoCamEye, sp5c, 0.1f, (field_0x2074 - sp5c).abs() / 30.0f);

                cXyz sp68 = player->current.pos;
                sp68.y += 90.0f;
                cLib_addCalcPos2(&mDemoCamCenter, sp68, 0.1f, (sp68 - field_0x2080).abs() / 30.0f);
                break;
            }
            // fallthrough
        case 7:
            camera->mCamera.Reset(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            mDemoCamMode = 0;
            field_0x2056 = 0;
            return;

        default:
            break;
    }

    if (mDemoCamMode != 0) {
        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
        ++field_0x2056;
    }
    return;
}

void daObj_GrA_c::graThrowDemo() {
    cXyz sp2c;
    daPy_py_c* player = daPy_getPlayerActorClass();
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    dCamera_c* camBody = dCam_getBody();

    switch (mDemoCamMode) {
        case 0:
            break;

        case 1: {
            if (dComIfGp_event_runCheck() && checkProcess(&daObj_GrA_c::thrown)) {
                camera->mCamera.Stop();
                mDemoCamMode = 2;
                mDemoCamFovy = camBody->Fovy();
                mDemoCamCenter = camBody->Center();
                mDemoCamEye = camBody->Eye();
                mDoMtx_stack_c::YrotS(player->current.angle.y);

                if (field_0x1ffc) {
                    field_0x2090.x = -300.0f;
                } else {
                    field_0x2090.x = 300.0f;
                }
                field_0x2090.y = 500.0f;
                field_0x2090.z = 0.0f;

                mDoMtx_stack_c::multVec(&field_0x2090, &field_0x2090);
                field_0x2090 += current.pos;

                if (fopAcM_lc_c::lineCheck(&mDemoCamEye, &field_0x2090, this)) {
                    if (field_0x1ffc) {
                        field_0x2090.x = 300.0f;
                    } else {
                        field_0x2090.x = -300.0f;
                    }

                    field_0x20a0 = 1;
                } else {
                    if (field_0x1ffc) {
                        field_0x2090.x = -200.0f;
                    } else {
                        field_0x2090.x = 200.0f;
                    }

                    field_0x20a0 = 0;
                }

                mDoMtx_stack_c::YrotS(player->current.angle.y);
                field_0x2090.y = 500.0f;
                field_0x2090.z = 0.0f;
                mDoMtx_stack_c::multVec(&field_0x2090, &field_0x2090);
                field_0x2090 += current.pos;
            } else {
                return;
            }
        }
        // fallthrough
        case 2: {
            sp2c.set(0.0f ,150.0f, -200.0f);
            if (field_0x1ff8 != 0) {
                mDoMtx_stack_c::YrotS((s16) cLib_targetAngleY(&player->current.pos, &current.pos));
                mDoMtx_stack_c::multVec(&sp2c, &sp2c);
                sp2c += player->current.pos;

                if (fopAcM_lc_c::lineCheck(&mDemoCamEye, &sp2c, this)) {
                    field_0x20a0 = 1;
                }
            }

            if (field_0x1ff8 != 0 && field_0x20a0 == 0) {
                cLib_addCalcPos2(&mDemoCamEye, sp2c, 0.2f, 100.0f);
                cLib_addCalcPos2(&mDemoCamCenter, current.pos, 0.2f, 200.0f);
            } else {
                cLib_addCalcPos2(&mDemoCamEye, field_0x2090, 0.1f, 15.0f);
                cLib_addCalcPos2(&mDemoCamCenter, (current.pos + player->current.pos) / 2.0f, 0.1f, 15.0f);
            }
            break;
        }

        case 3: {
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            field_0x1520 = 1;
            mDemoCamMode = 0;
            break;
        }
    }

    if (mDemoCamMode != 0) {
        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
    }
}

void daObj_GrA_c::setCarryStatus() {
    cXyz sp38;
    mDoMtx_stack_c::YrotS(-shape_angle.y);
    mDoMtx_stack_c::transM(-current.pos.x, -current.pos.y, -current.pos.z);
    mDoMtx_stack_c::multVec(&daPy_getPlayerActorClass()->current.pos, &sp38);
    if (fabsf(sp38.x) < 100.0f && sp38.z > 0.0f && sp38.z < 800.0f) {
        attention_info.flags |= fopAc_AttnFlag_CARRY_e;
    }
}

bool daObj_GrA_c::checkThrow() {
    if (field_0xa48 != 0) {
        if ((field_0xa48 & 1) != 0) {
            setProcess(&daObj_GrA_c::thrown);
            initCrazyBeforeCatch(0);
            field_0xa48 &= 0xFFFE;
            return true;
        }

        if ((field_0xa48 & 2) != 0) {
            setProcess(&daObj_GrA_c::thrown);
            initCrazyCatch(0);
            field_0xa48 &= ~0x2;
            return true;
        }
    }

    return false;
}

void daObj_GrA_c::calcCatchPos(f32 param_1, int param_2) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 opp_shape_angle = player->shape_angle.y + 0x8000;
    // (void) opp_shape_angle;  // affects dbg if uncommented..
    f32 fVar1 = player->current.pos.y - mGroundY;
    f32 player_dist = current.pos.absXZ(player->current.pos);
    int HandsHeight = cM_atan2s(player_dist, fVar1);
    cXyz sp6c(0.0f, param_1 * (cM_scos(HandsHeight)), param_1 * cM_ssin(HandsHeight));
    cXyz sp78(player->current.pos);
    cXyz sp84, sp90, sp9c, spa8;

    mDoMtx_stack_c::copy(player->getLeftHandMatrix());
    mDoMtx_stack_c::multVecZero(&sp84);
    mDoMtx_stack_c::copy(player->getRightHandMatrix());
    mDoMtx_stack_c::multVecZero(&sp90);

    sp9c = (sp84 + sp90) / 2.0f;

    mDoMtx_stack_c::copy(player->getHeadMtx());
    mDoMtx_stack_c::multVecZero(&spa8);

    spa8.y = sp78.y;
    HandsHeight = cLib_targetAngleY(&sp9c, &spa8);

    if (param_2 != 0) {
        cXyz spb4;
        sp9c.y -= mpHIO->m.field_0x60 / 2;
        cLib_offsetPos(&spb4, &sp9c, player->shape_angle.y + 0x8000, &sp6c);

        if (param_2 == 1) {
            cLib_chasePos(&current.pos, spb4, 100.0f);
        } else if (param_2 == 2) {
            cLib_chasePos(&current.pos, spb4, 5.0f);
            cLib_addCalcAngleS2(&shape_angle.y, HandsHeight, 8, 0x400);
            field_0x91a.y = shape_angle.y;
        }
    } else {
        #if DEBUG
        dDbVw_Report(100, 100, "pos.y = %f", sp9c.y - player->current.pos.y);
        #endif

        cXyz spc0;
        sp9c.y -= mpHIO->m.field_0x60 / 2;
        cXyz spcc(0.0f, 0.0f, param_1);
        cLib_offsetPos(&spc0, &sp9c, HandsHeight, &spcc);

        OS_REPORT("angle = %d, HandsHeight = %f\n", sp9c.y, (s16) HandsHeight);

        current.pos = spc0;

        if (field_0x1ffc) {
            current.angle.y = HandsHeight + 0x5000;
        } else {
            current.angle.y = HandsHeight + -0x5000;
        }

        cLib_addCalcAngleS2(&shape_angle.y, HandsHeight, 2, 0x2000);
        field_0x91a.y = shape_angle.y;
    }
}

void daObj_GrA_c::initCrazyBeforeCatch(int param_1) {
    field_0x1ff0 = 1;
    speedF = 0.0f;
    gravity = 0.0f;
}

void daObj_GrA_c::executeCrazyBeforeCatch() {
    calcCatchPos(-120.0f, 1);
    if ((field_0xa48 & 2) != 0) {
        initCrazyCatch(0);
        field_0xa48 &= ~2;
    } else if ((field_0xa48 & 4) != 0) {
        initCrazyAttack();
        field_0xa48 &= ~4;
    }
}

void daObj_GrA_c::initCrazyCatch(int param_1) {
    dComIfGp_getVibration().StartQuake(8, 15, cXyz(0.0f, 1.0f, 0.0f));
    field_0x1ff0 = 2;
    speedF = 0.0f;
    field_0x1ff4 = 0;
    calcCatchPos(-120.0f, 1);
    gravity = 0.0f;
}

void daObj_GrA_c::executeCrazyCatch() {
    switch (field_0x1ff4) {
        case 0:
            if (!daPy_getPlayerActorClass()->speedF) {
                field_0x1ff4 = 1;
                calcCatchPos(-110.0f, 2);
                dComIfGp_getVibration().StartQuake(1, 15, cXyz(0.0f, 1.0f, 0.0f));
            } else {
                calcCatchPos(-110.0f, 1);
            }
            break;

        case 1:
            calcCatchPos(-110.0f, 2);
            break;
    }

    f32 fVar1 = mpModelMorf->getPlaySpeed();
    if (fVar1 > 0.2f) {
        mpModelMorf->setPlaySpeed(fVar1 - 0.2f);
    } else {
        mpModelMorf->setPlaySpeed(0.0f);
    }

    if (field_0xa48 != 0) {
        if ((field_0xa48 & 8) != 0) {
            initCrazyThrow(0);
        } else if ((field_0xa48 & 16) != 0) {
            initCrazyThrow(1);
        } else if ((field_0xa48 & 4) != 0) {
            if (!daPy_getPlayerActorClass()->speedF) {
                initCrazyAttack();
            } else {
                initCrazyAttack();
            }
        }

        field_0xa48 = 0;
    }
}

void daObj_GrA_c::initCrazyThrow(int param_1) {
    field_0x1ffc = param_1 & 0xFF;
    field_0x1ff4 = 0;
    field_0x1ff0 = 3;
    gravity = 0.0f;
    speedF = 0.0f;
    speed.y = 0.0f;

    if (field_0x1fe8 == 0) {
        mDemoCamMode = 1;
    }

    field_0x1ff8 = 34;
    calcCatchPos(-110.0f, 2);
    field_0x2004 = 0;
    gravity = 0.0f;
}

void daObj_GrA_c::executeCrazyThrow() {
    f32 fVar1;
    cXyz spa0;
    dBgS_LinChk dStack_90;

    switch (field_0x1ff4) {
        case 0:
            fVar1 = mpModelMorf->getPlaySpeed();
            // for whatever reason, the original devs forgot to add the f suffix here?!
            if (fVar1 > 0.2) {
                mpModelMorf->setPlaySpeed(fVar1 - 0.2f);
            } else {
                mpModelMorf->setPlaySpeed(0.0f);
            }

            if (field_0x1ff8 == 0) {
                field_0x1ff0 = 4;
                gravity = -0.7f;
                speedF = 25.0f;
                speed.y = 15.0f;
                cLib_addCalcAngleS2(&shape_angle.y, current.angle.y + 0x8000, 4, 0x1000);
                field_0x91a.y = shape_angle.y;
            } else if (field_0x1ff8 <= 10) {
                if (field_0x1ff8 == 10) {
                    if (isFirstGra()) {
                        mSound.startCreatureVoice(Z2SE_GORON_V_THROWN_FIRST, -1);
                    } else {
                        mSound.startCreatureVoice(Z2SE_GORON_V_THROWN, -1);
                    }
                }

                calcCatchPos(-110.0f, 0);
            } else {
                calcCatchPos(-110.0f, 2);
            }
            break;

        case 1:
            executeCrazyJumping();
    }

    if (field_0xa48 != 0) {
        if ((field_0xa48 & 4) != 0) {
            initCrazyAttack();
        }

        field_0xa48 = 0;

        return;
    }
}

void daObj_GrA_c::initCrazyAttack() {
    camera_class* camera;

    if (field_0x1ff0 == 3 && field_0x1ff8 == 0) {
        field_0x1ff0 = 4;
        field_0x1fe9 = 1;

        daPy_getPlayerActorClass()->offGoronSideMove();
        daPy_getPlayerActorClass()->offGoronUpStopCancel();

        if (isFirstGra()) {
            camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
            mDemoCamMode = 0;
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            daNpcF_onEvtBit(0x52);
            gravity = -0.7f;
            speedF = 25.0f;
            speed.y = 15.0f;
            cLib_addCalcAngleS2(&shape_angle.y, current.angle.y + 0x8000, 4, 0x1000);
            field_0x91a.y = shape_angle.y;
        }
    } else {
        daPy_getPlayerActorClass()->offGoronSideMove();

        if (isFirstGra()) {
            if (mDemoCamMode != 0) {
                camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                mDemoCamMode = 0;
            }

            setProcess(&daObj_GrA_c::rollAttacked);
        } else {
            setProcess(&daObj_GrA_c::rollAttack);
            field_0x1fe9 = 1;
            field_0x1fd8 = 60;
        }

        gravity = -4.0f;
        current.angle.y = cLib_targetAngleY(&current.pos, &daPy_getPlayerActorClass()->current.pos);
    }
}

void daObj_GrA_c::executeCrazyJumping() {
    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y + 0x8000, 4, 0x1000);
    field_0x91a.y = shape_angle.y;

    if (mAcch.ChkGroundLanding()) {
        cXyz sp28(0.0f, 0.0f, 20.0f);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(field_0x91a.y);
        mDoMtx_stack_c::multVec(&sp28, &field_0xab8[2].field_0x4);
        field_0xab8[2].field_0x10 = field_0x91a;
        field_0xab8[2].field_0x0 = 1;

        mSound.startCreatureSound(Z2SE_GORON_ROLL_BOUND, 0, -1);

        if (field_0x2004 != 0) {
            dComIfGp_getVibration().StartShock(5, 15, cXyz(0.0f, 1.0f, 0.0f));
            speed.y = 0.0f;
            setProcess(&daObj_GrA_c::rollEscape);
            field_0x1fe9 = 1;
            daPy_getPlayerActorClass()->offGoronSideMove();
            field_0x2004 = 0;
            gravity = -4.0f;
        } else {
            dComIfGp_getVibration().StartShock(8, 15, cXyz(0.0f, 1.0f, 0.0f));
            speed.y = 15.0f;
            field_0x2004 = 1;
            mpModelMorf->setLoopFrame(mpModelMorf->getEndFrame());
            mpModelMorf->setPlaySpeed(-0.1f);
        }
    } else if (mAcch.ChkWallHit() != 0) {
        cXyz sp34(0.0f, 0.0f, 20.0f);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(field_0x91a.y);
        mDoMtx_stack_c::multVec(&sp34, &field_0xab8[2].field_0x4);
        field_0xab8[2].field_0x10 = field_0x91a;
        field_0xab8[2].field_0x0 = 1;

        mSound.startCreatureSound(Z2SE_GORON_ROLL_BOUND, 0, -1);
        dComIfGp_getVibration().StartShock(5, 15, cXyz(0.0f, 1.0f, 0.0f));
        speed.y = 0.0f;

        setProcess(&daObj_GrA_c::rollEscape);
        field_0x1fe9 = 1;
        daPy_getPlayerActorClass()->offGoronSideMove();
        field_0x2004 = 0;
        gravity = -4.0f;
    }
}

BOOL daObj_GrA_c::modeChangeCheck() {
    BOOL rv = (field_0x2030 == 0);

    if (rv != FALSE) {
        mMode = 0;
        home.angle.y = cLib_targetAngleY(&current.pos, &home.pos);
        home.pos = current.pos;
        field_0xf50.OffAtSetBit();
        setProcess(&daObj_GrA_c::toStone);
        field_0x109c = 1;
        setBaseMotion(4, mpHIO->m.field_0x0c / 2);
        setFaceMotion(13, mpHIO->m.field_0x0c / 2);
        calcMotion();
        mpModelMorf->setFrame(mpModelMorf->getEndFrame());
        mpModelMorf->setPlaySpeed(1.0f);
        return TRUE;
    }

    return FALSE;
}
