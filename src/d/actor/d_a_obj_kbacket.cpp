/**
 * @file d_a_obj_kbacket.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kbacket.h"
#include "SSystem/SComponent/c_counter.h"
#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"

static s32 l_bmdData[4] = { 8, 0, 9, 0 };

static char* l_resNameList[1] = { "Kakashi" };

const daObj_KBacket_HIOParam daObj_KBacket_Param_c::m = {
    0.0f, -5.0f, 1.0f, 400.0f, 73.0f, 30.0f, 30.0f, 30.0f, 37.0f, 45.0f, 0.0f
};

static const dCcD_SrcGObjInf l_ccDObjData = {
    {0x0, {{0x0, 0x0, 0x0}, {(s32)0xd8fbfdff, 0x0}, 0x79}}, // mObj
    {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
    {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
    {0x0}, // mGObjCo
};

static dCcD_SrcCyl l_ccDCyl = {
    l_ccDObjData,
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        0.0f, // mRadius
        0.0f // mHeight
    } // mCyl
};

static daObj_KBacket_Param_c l_HIO;

static inline const daObj_KBacket_HIOParam* get_params(daObj_KBacket_c* i_this) {
#if DEBUG
    return &i_this->mHIO->param;
#else
    return &daObj_KBacket_Param_c::m;
#endif
}

daObj_KBacket_c::~daObj_KBacket_c() {
    OS_REPORT("|%06d:%x|daObj_KBacket_c -> デストラクト\n", g_Counter.mCounter0, this);
#if DEBUG
    if (mHIO != NULL) {
        mHIO->removeHIO();
    }
#endif
    dComIfG_resDelete(&field_0x56c, l_resNameList[l_bmdData[field_0x9d0 * 2 + 1]]);
}


int daObj_KBacket_c::create() {
    fopAcM_ct(this, daObj_KBacket_c);

    field_0x9d0 = getType();

    s32 res = dComIfG_resLoad(&field_0x56c, l_resNameList[l_bmdData[1]]);
    if (res == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) <%08x> -> roomNo.%d", fopAcM_getProcNameString(this),
            getType() & 0xff, fopAcM_GetParam(this), fopAcM_GetRoomNo(this));
        OS_REPORT("\n");

        if (fopAcM_entrySolidHeap(this, (heapCallbackFunc)createHeapCallBack, 0x810) == 0) {
            return cPhs_ERROR_e;
        }

        mpModel->getModelData();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -50.0f, -50.0f, -50.0f, 50.0f, 50.0f, 50.0f);

#if DEBUG
        //TODO: init mHIO
        mHIO->entryHIO("カカシのバケツ");
#endif

        reset();

        if ((fopAcM_GetParam(this) & 0x80000000) != 0) {
            setLaunchParam(42.0f, 70.0f, shape_angle.y);
        }

        mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                        &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this),
                        fopAcM_GetShapeAngle_p(this));
        mStts.Init(get_params(this)->field_0x10, 0, this);
        mCyl.Set(l_ccDCyl);
        mCyl.SetStts(&mStts);
        mObjAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mObjAcch.m_gnd;
        field_0xa00 = mObjAcch.GetGroundH();
        if (field_0xa00 != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }
        Execute();
    }
    return res;
}

int daObj_KBacket_c::CreateHeap() {
    J3DModelData* modelData;

    switch (field_0x9d0) {
    case 0:
        modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[1]], l_bmdData[0]);
        break;
    case 1:
        modelData = (J3DModelData*)dComIfG_getObjectRes(l_resNameList[l_bmdData[3]], l_bmdData[2]);
        break;
    default:
        modelData = NULL;
        break;
    }

    if (modelData != NULL) {
        mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    } else {
        mpModel = NULL;
    }

    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daObj_KBacket_c::Delete() {
    OS_REPORT("|%06d:%x|daObj_KBacket_c -> Delete\n", g_Counter.mCounter0, this);
    this->~daObj_KBacket_c();
    return 1;
}

int daObj_KBacket_c::Execute() {

    f32 movePMag = get_params(this)->field_0x28;
    f32 scaleFactor = get_params(this)->field_0x8;
    s16 wallAngle = 0;
    scale.set(scaleFactor, scaleFactor, scaleFactor);

    attention_info.flags = 0;

    attention_info.distances[4] = 6;

    mAcchCir.SetWallR(get_params(this)->field_0x1c);
    mAcchCir.SetWallH(get_params(this)->field_0x18);

    gravity = get_params(this)->field_0x4;

    BOOL isCarry = FALSE;
    if (fopAcM_checkCarryNow(this) != 0) {
        isCarry = TRUE;
    }

    if (field_0xa44 == 0) {
        s32 unkInt1;
        if (field_0x9d0 == 0) {
            unkInt1 = 0x4000;
        } else {
            s32 unkInt2;
            if (field_0x9d0 == 1) {
                unkInt2 = -0x4000;
            } else {
                unkInt2 = 0;
            }
            unkInt1 = unkInt2;
        }
        shape_angle.z = unkInt1;
    }
    if (isCarry) {
        mObjAcch.ClrWallHit();
        mObjAcch.ClrGroundHit();
        mObjAcch.SetWallNone();
        mObjAcch.SetGrndNone();

        field_0xa00 = 0.0f;

        mStts.SetWeight(0);

        speed.setall(0.0f);
        speedF = 0.0f;
        field_0x9d4.setall(0.0f);
        field_0x9ec.setall(0);
        gravity = 0.0f;

        field_0xa45 = 0;
        field_0xa46 = 0;
        field_0xa47 = 0;
        field_0xa4a = 0;
    } else {
        s16 wallAngleDiff; // needs to be declared up here for regalloc

        mStts.SetWeight(get_params(this)->field_0x10);
        mObjAcch.ClrWallNone();
        mObjAcch.ClrGrndNone();
        if (field_0xa49 != 0 && cM3d_IsZero(speedF) == FALSE) {
            s16 angle = cM_deg2s(get_params(this)->field_0x24);
            speed.setall(0.0f);
            speed.y = get_params(this)->field_0x20 * cM_ssin(angle);
            speedF = get_params(this)->field_0x20 * cM_scos(angle);

            field_0xa18 = 0x4000;

            field_0xa47 = 1;

            if (fopAcM_carryOffRevise(this) != 0) {
                speed.setall(0.0f);
            }
        } else {
            s32 unkInt3 = 0;
            cXyz streamSpeed;
            s16 unkShort1 = 0x80;

            fopAcM_getWaterY(&current.pos, &field_0xa04);
            if (field_0xa04 != -G_CM3D_F_INF &&
                movePMag < field_0xa04 - field_0xa00 &&
                current.pos.y <= field_0xa04)
            {
                if (field_0xa46 == 0x0) {
                    speedF *= 0.3f;
                    speed.y *= 0.5f;

                    field_0xa45 = 0;
                    field_0xa46 = 1;

                    if (field_0xa47 != 0 || field_0xa4a != 0 || field_0xa49 != 0) {
                        setWaterPrtcl();
                        unkInt3 = 1;
                        if ((field_0xa47 != 0 || field_0xa4a != 0) && speed.y < 0.0f) {
                            field_0xa10 = 3000.0f;
                        }
                    }

                    field_0xa47 = 0;
                    field_0xa4a = 0;
                }

                s32 streamPower;
                if (fopAcM_getWaterStream(&current.pos, mGndChk, &streamSpeed,
                    (int*)&streamPower, 0) != 0) {
                    streamSpeed.normalizeZP();

                    cLib_addCalcAngleS2(&current.angle.y, cM_atan2s(streamSpeed.x, streamSpeed.z), 4,
                        0x200);

                    if (unkInt3 != 0) {
                        current.angle.y = cM_atan2s(streamSpeed.x, streamSpeed.z);
                    }

                    unkShort1 *= streamPower;

                    s32 targetAngle;
                    if (field_0x9ec.y < 0)
                        targetAngle = -unkShort1;
                    else
                        targetAngle = unkShort1;
                    cLib_chaseAngleS(&field_0x9ec.y, targetAngle, 0x10);

                    if (field_0xa45 != 0) {
                        cLib_addCalc2(&speedF, streamPower * 1.55f, 0.15f, 1.0f);
                    } else {
                        cLib_addCalc2(&speedF, streamPower * 0.75f, 0.15f, 1.0f);
                    }
                } else {
                    cLib_chaseF(&speedF, 0.0f, 0.3f);
                }

                if (field_0x9f4 == 0 && mObjAcch.ChkWallHit() != 0) {
                    if (getWallAngle(current.angle.y, &wallAngle) != 0) {
                        current.angle.y = wallAngle;
                        current.angle.y += (s16)cM_rndFX(2000.0f);
                    } else {
                        current.angle.y += (s16)cM_rndFX(2000.0f);
                        current.angle.y += -0x8000;
                    }

                    field_0x9ec.y = -field_0x9ec.y / 2;

                    field_0x9f4 = 10;
                }

                if (field_0xa45 != 0) {
                    cLib_addCalc2(&current.pos.y, field_0xa04 - movePMag, 0.5f, 2.0f);

                    speed.y = 0;

                    setHamonPrtcl();
                } else {
                    if (current.pos.y + movePMag < field_0xa04) {
                        if (speed.y < 0.0f) {
                            cLib_addCalc(&speed.y, 2.0f, 0.8f, 11.0f, 0.1f);
                        } else {
                            cLib_addCalc(&speed.y, 2.0f, 0.5f, 0.5f, 0.1f);
                        }

                        field_0x9ec.y = 0x100;

                        mObjAcch.ClrGroundHit();

                        field_0xa45 = 1;
                    }
                }

                gravity = 0.0f;
            }
        }
        if (field_0xa45 != 0) {
            if (mObjAcch.ChkGroundHit() != 0) {
                field_0xa45 = 0;
                field_0xa46 = 0;

                field_0x9ec.setall(0);

                speedF = 0.0f;
                speed.setall(0.0f);
            }
        } else {
            if (field_0xa46 == 0) {
                if (field_0xa47 != 0) {
                    if (mObjAcch.ChkWallHit() != 0) {
                        if (getWallAngle(current.angle.y, &wallAngle) != 0) {
                            wallAngleDiff = current.angle.y - wallAngle;
                        } else {
                            wallAngleDiff = current.angle.y;
                        }

                        current.angle.y += (s16)((s16)cM_rndFX(1000.0f) + (0x8000 - wallAngleDiff * 2));

                        speedF *= 0.5f;

                        field_0xa47 = 0;
                        field_0xa4a = 1;
                    } else {
                        if (mObjAcch.ChkGroundHit() != 0) {
#if !DEBUG
                            fopAcM_seStart(this, Z2SE_OBJ_WOODSTICK_BOUND, 0);
#endif
                            if (field_0x9d4.y < -30.0f) {
                                speedF *= 0.7f;
                                speed.y = field_0x9d4.y * -0.4f;

                                setSmokePrtcl();
                            } else {
                                speedF = 0.0f;
                                speed.setall(0.0f);

                                field_0xa47 = 0x0;
                            }
                        }
                    }
                } else {
                    if (field_0xa4a != 0) {
                        if (mObjAcch.ChkGroundHit() != 0) {
#if !DEBUG
                            fopAcM_seStart(this, Z2SE_OBJ_WOODSTICK_BOUND, 0);
#endif
                            if (this->field_0x9d4.y < -30.0f) {
                                speedF *= 0.7f;
                                speed.y = field_0x9d4.y * -0.4f;

                                setSmokePrtcl();
                            } else {
                                speedF = 0.0f;
                                speed.setall(0.0f);

                                field_0xa4a = 0x0;
                            }
                        }
                    } else {
                        if (field_0xa49 != 0) {
#if !DEBUG
                            fopAcM_seStart(this, Z2SE_OBJ_WOODSTICK_PUT, 0);
#endif
                        } else {
                            if (mObjAcch.ChkGroundHit() == FALSE) {
                                field_0xa0c = field_0xa00 - current.pos.y;

                                field_0xa4a = 1;
                            }
                        }
                    }
                }
            }
        }

        if (field_0xa48 == 0x0 && field_0xa47 == 0x0 && field_0xa4a == 0x0) {
            cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        }

        field_0x9d4 = speed;

        if (field_0xa46 != 0) {
            if (mStts.GetCCMoveP() != NULL) {
                movePMag = 1200.0f;
            } else {
                movePMag = 900.0f;
            }
            cLib_addCalc2(&field_0xa10, movePMag, 0.05f, 100.0f);
        } else {
            cLib_addCalc2(&field_0xa10, 0.0f, 0.25f, 100.0f);
        }

        shape_angle.x = field_0xa10 * cM_ssin(field_0xa14 * 1000);
        shape_angle.z += (s16)(field_0xa10 * cM_scos(field_0xa14 * 1000));
        cLib_chaseAngleS(&field_0x9ec.y, 0, 10);
        shape_angle.y += field_0x9ec.y;

        field_0xa14++;

        if (field_0xa45 != 0x0) {
            movePMag = JMAFastSqrt(mStts.GetCCMoveP()->x * mStts.GetCCMoveP()->x +
                mStts.GetCCMoveP()->z * mStts.GetCCMoveP()->z);

            if (1.0f < movePMag) {
                field_0xa1c = cM_atan2s(mStts.GetCCMoveP()->x, mStts.GetCCMoveP()->z);
                if (field_0x9f8 == 0) {
                    if (30.0f < movePMag) {
                        movePMag = 30.0f;
                    } else {
                        if (movePMag < 0.0f) {
                            movePMag = 0.0f;
                        }
                    }

                    field_0x9fc = movePMag;

                    field_0x9ec.y = 0x200;

                    field_0x9f8 = 10;
                }
            }
        } else {
            field_0x9fc = 0;
        }

        cLib_addCalc2(&field_0x9fc, 0.0f, 0.33f, 100.0f);

        fopAcM_calcSpeed(this);

        if (field_0xa45 != 0x0) {
            field_0x9e0.x = field_0x9fc * cM_ssin(field_0xa1c);
            field_0x9e0.z = field_0x9fc * cM_scos(field_0xa1c);
            field_0x9e0.y = 0.0f;

            speed += field_0x9e0;
        }

        fopAcM_posMove(this, mStts.GetCCMoveP());

        mObjAcch.CrrPos(dComIfG_Bgsp());

        mGndChk = mObjAcch.m_gnd;
        field_0xa00 = mObjAcch.GetGroundH();

        if (field_0xa00 != -G_CM3D_F_INF) {
            field_0xa16 = daNpcF_getGroundAngle(&mGndChk, shape_angle.y);

            setEnvTevColor();

            setRoomNo();

            if (dComIfGp_roomControl_getStayNo() != fopAcM_GetRoomNo(this) &&
                field_0xa4b == 0 &&
                field_0xa47 == 0 &&
                field_0xa4a == 0)
            {
                fopAcM_delete(this);
                return 1;
            }
        }
    }

    attention_info.position = current.pos;
    eyePos = current.pos;
    setMtx();

    if (field_0xa44 != 0x0) {
        field_0xa18 = calcRollAngle(field_0xa18, 0xc000);
        if (field_0xa18 == -0x4000) {
            field_0xa18 = 0;
            field_0xa44 = 0x0;
        }
    } else {
        field_0xa18 = calcRollAngle(field_0xa18, 0x10000);
    }

    mCyl.SetR(get_params(this)->field_0x1c);
    mCyl.SetH(get_params(this)->field_0x14);
    mCyl.SetC(current.pos);

    dComIfG_Ccsp()->Set(&mCyl);

    mCyl.ClrCoHit();

    field_0xa49 = isCarry != 0;

    cLib_calcTimer((int*)&field_0x9f4);
    cLib_calcTimer((int*)&field_0x9f8);

    field_0xa4b = 0;

    return 1;
}

int daObj_KBacket_c::Draw() {
    if (this->field_0xa48 == 0x0) {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
        mDoExt_modelUpdateDL(mpModel);
        fopAc_ac_c* grabActor = NULL;
        fopAcM_SearchByID(daPy_getPlayerActorClass()->getGrabActorID(), &grabActor);
        if (grabActor == this) {
            model = mpModel;
        } else {
            cM3dGPla plane;
            if (dComIfG_Bgsp().GetTriPla(mGndChk, &plane) != 0) {
                _GXTexObj* p_Var4 = dDlst_shadowControl_c::getSimpleTex();
                dComIfGd_setSimpleShadow(&current.pos, field_0xa00, 50.0f,
                    &plane.mNormal, 0, 1.0f, p_Var4);
            }
        }
        this->field_0xa4b = 1;
    }
    return 1;
}

void daObj_KBacket_c::createHeapCallBack(fopAc_ac_c* param_0) {
    ((daObj_KBacket_c*)param_0)->CreateHeap();
}

void daObj_KBacket_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daObj_KBacket_c::setRoomNo() {
    s8 roomNo = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, roomNo);
    mStts.SetRoomId(roomNo);
}

void daObj_KBacket_c::reset() {
    memset(&field_0x9d4, 0, (size_t)&field_0xa4c - (size_t)&field_0x9d4);
}

void daObj_KBacket_c::setMtx() {
    s16 angleDiff = current.angle.y - shape_angle.y;
    s16 cos = cM_scos(angleDiff) * field_0xa18;
    s16 sin = cM_ssin(angleDiff) * field_0xa18;

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);

    if (field_0xa44 != 0) {
        sin = -sin;
        mDoMtx_stack_c::ZrotM(sin);
    } else {
        if (field_0x9d0 == 0) {
            cos = -cos;
        }
        mDoMtx_stack_c::YrotM(cos);
    }

    mDoMtx_stack_c::scaleM(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

s16 daObj_KBacket_c::calcRollAngle(s16 param_0, int param_1) {
    // this matches in release but the debug match is pretty horrible

    if (param_0 != 0) {
        s32 rv;
        s32 diff = param_1 - (u16)param_0;
        if (abs(diff) > 4) {
            if ((s32)diff / 4 > 0x800) {
                rv = (u16)param_0 + 0x800;
            } else if (diff / 4 < -0x800) {
                rv = (u16)param_0 - 0x800;
            } else {
                rv = (u16)param_0 + diff / 4;
            }
        } else {
            rv = param_1;
        }
        return rv;
    }

    return 0;
}

int daObj_KBacket_c::getWallAngle(s16 param_0, s16* param_1) {
    cXyz unkXyz1;
    cXyz unkXyzArr[2];
    cXyz unkXyz2;

    s16 temp = param_0;
    mDoMtx_stack_c::YrotS(temp);
    unkXyz2.set(0.0f, 0.0f, -50.0f);
    mDoMtx_stack_c::multVec(&unkXyz2, &unkXyz1);
    unkXyz1 += current.pos;
    unkXyz2.set(5.0f, 0.0f, 200.0f);

    for (s32 i = 0; i < 2; i++) {
        mDoMtx_stack_c::multVec(&unkXyz2, &unkXyzArr[i]);
        unkXyz2.x = unkXyz2.x * -1.0f;
        unkXyzArr[i] += unkXyz1;
        mLinChk.Set(&unkXyz1, &unkXyzArr[i], this);
        // should be dBgS::LineCross in debug
        if (dComIfG_Bgsp().LineCross(&mLinChk) != 0) {
            unkXyzArr[i] = mLinChk.GetCross();
        } else {
            return 0;
        }
    }

    unkXyz2 = unkXyzArr[1] - unkXyzArr[0];
    s16 yaw = cM_atan2s(unkXyz2.x, unkXyz2.z);
    *param_1 = yaw + 0x4000;

    return 1;
}

void daObj_KBacket_c::setSmokePrtcl() {
    fopAcM_effSmokeSet1(&field_0xa3c, &field_0xa40, &current.pos, NULL, 0.6f, &tevStr, 1);
    dComIfGp_particle_levelEmitterOnEventMove(field_0xa3c);
    dComIfGp_particle_levelEmitterOnEventMove(field_0xa40);
}

void daObj_KBacket_c::setWaterPrtcl() {
    /* 80587EF0-80587EF8 000078 0008+00 1/1 0/0 0/0 .data            emttrId$4670 */
    static u16 emttrId[4] = {
        0x01B8,
        0x01B9,
        0x01BA,
        0x01BB,
    };

    cXyz scale(0.6f, 0.6f, 0.6f);
    cXyz translation(current.pos.x, field_0xa04, current.pos.z);
    for (s32 i = 0; i < 4; i++) {
        field_0xa24[i] = dComIfGp_particle_set(
            field_0xa24[i], emttrId[i], &translation, &tevStr, NULL, &scale, 0xff, NULL,
            -1, NULL, NULL, NULL);
        dComIfGp_particle_levelEmitterOnEventMove(field_0xa24[i]);
    }
}

void daObj_KBacket_c::setHamonPrtcl() {
    cXyz translation(current.pos.x, field_0xa04, current.pos.z);
    fopAcM_effHamonSet(&field_0xa34, &translation, 0.7f, 0.05f);
}

static int daObj_KBacket_Create(void* i_this) {
    return ((daObj_KBacket_c*)i_this)->create();
}

static int daObj_KBacket_Delete(void* i_this) {
    return ((daObj_KBacket_c*)i_this)->Delete();
}

static int daObj_KBacket_Execute(void* i_this) {
    return ((daObj_KBacket_c*)i_this)->Execute();
}

static int daObj_KBacket_Draw(void* i_this) {
    return ((daObj_KBacket_c*)i_this)->Draw();
}

static int daObj_KBacket_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daObj_KBacket_MethodTable = {
    (process_method_func)daObj_KBacket_Create,
    (process_method_func)daObj_KBacket_Delete,
    (process_method_func)daObj_KBacket_Execute,
    (process_method_func)daObj_KBacket_IsDelete,
    (process_method_func)daObj_KBacket_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_KBACKET = {
    fpcLy_CURRENT_e,            // mLayerID
    8,                          // mListID
    fpcPi_CURRENT_e,            // mListPrio
    PROC_OBJ_KBACKET,           // mProcName
    &g_fpcLf_Method.base,      // sub_method
    sizeof(daObj_KBacket_c),    // mSize
    0,                          // mSizeOther
    0,                          // mParameters
    &g_fopAc_Method.base,       // sub_method
    714,                        // mPriority
    &daObj_KBacket_MethodTable, // sub_method
    0x00044100,                 // mStatus
    fopAc_ACTOR_e,              // mActorType
    fopAc_CULLBOX_CUSTOM_e,     // cullType
};
