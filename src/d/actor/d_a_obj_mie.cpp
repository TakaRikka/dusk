/**
 * @file d_a_obj_mie.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_mie.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc4.h"
#include "d/actor/d_a_npc_pouya.h"
#include "d/d_path.h"
#include "d/d_cc_d.h"
#include "SSystem/SComponent/c_counter.h"
#include "Z2AudioLib/Z2Instances.h"

static u32 l_bmdData[1][2] = {
    23, 1,
};

static char* l_resNameList[2] = {
    "",
    "pouyaA",
};

daObj_Mie_HIOParam const daObj_Mie_Param_c::m = {
    0.0f, -4.0f, 1.0f, 100.0f, 255.0f, 20.0f, 10.0f, 14.0f, 37.0f, 50.0f, 18.0f,
};

static dCcD_SrcGObjInf const l_ccDObjData = 
{
        {0x0, {{0x0, 0x0, 0}, {0xD8FBFDFF, 0x1F}, 0x79}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
        {0x0}, // mGObjCo
};

static dCcD_SrcCyl l_ccDCyl = {
    l_ccDObjData, // mObjInf
    {
        {0.0f, 0.0f, 0.0f}, // mCenter
        0.0f, // mRadius
        0.0f // mHeight
    } // mCyl
};

#if DEBUG
void daObj_Mie_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genSlider("注目オフセット  ", &mParams.focus_offset, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("重力            ", &mParams.gravity, -100.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    ctx->genSlider("スケ−ル        ", &mParams.scale, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("リアル影サイズ  ", &mParams.real_shadow_size, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("体重            ", &mParams.weight, 0.0f, 255.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("高さ            ", &mParams.height, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("ひざ丈          ", &mParams.knee_length, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("幅              ", &mParams.width, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("発射速度        ", &mParams.firing_rate, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("発射角度        ", &mParams.launch_angle, 0.0f, 90.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("浮きオフセット  ", &mParams.floating_offset, -100.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    ctx->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}
#endif

daObj_Mie_c::~daObj_Mie_c() {
    OS_REPORT("|%06d:%x|daObj_Mie_c -> デストラクト\n", g_Counter.mCounter0, this);

    #if DEBUG
    if (mHIO) {
        mHIO->removeHIO();
    }
    #endif

    dComIfG_resDelete(&mPhase, l_resNameList[l_bmdData[mType][1]]);
}

int daObj_Mie_c::create() {
    fopAcM_ct(this, daObj_Mie_c);
    mType = getType();
    int rv = dComIfG_resLoad(&mPhase, l_resNameList[l_bmdData[mType][1]]);
    if (rv == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0xd50)) {
            return cPhs_ERROR_e;
        }
        OS_REPORT("\t(%s:%d) <%08x>\n", fopAcM_getProcNameString(this), getType(), fopAcM_GetParam(this));
        
        // @bug - seems like this returns cPhs_ERROR_e no matter what
        if (mType == TYPE_0 && dComIfGs_getPohSpiritNum() >= 50) {
            return cPhs_ERROR_e;
        }

        return cPhs_ERROR_e;
    }

    return rv;
}

int daObj_Mie_c::CreateHeap() {
    J3DModelData* mdlData_p = (J3DModelData*)dComIfG_getObjectRes(
        l_resNameList[l_bmdData[mType][1]],
        l_bmdData[mType][0]);
    JUT_ASSERT(392, NULL != mdlData_p);
    mModel = mDoExt_J3DModel__create(mdlData_p, 0, 0x11000084);
    return mModel != NULL ? TRUE : FALSE;
}

int daObj_Mie_c::Delete() {
    OS_REPORT("|%06d:%x|daObj_Mie_c -> Delete\n", g_Counter.mCounter0, this);
    this->~daObj_Mie_c();
    return 1;
}

int daObj_Mie_c::Execute() {
    BOOL local_a8 = fopAcM_checkCarryNow(this) != 0;
    s16 local_b6;
    f32 dVar16 = daObj_Mie_Param_c::m.floating_offset;
    scale.set(daObj_Mie_Param_c::m.scale, daObj_Mie_Param_c::m.scale,
              daObj_Mie_Param_c::m.scale);
    attention_info.flags = 0;
    attention_info.distances[fopAc_attn_CARRY_e] = 6;
    mAcchCir.SetWallR(daObj_Mie_Param_c::m.width);
    mAcchCir.SetWallH(daObj_Mie_Param_c::m.knee_length);
    gravity = daObj_Mie_Param_c::m.gravity;
    if ((s32)local_a8 || field_0xa40) {
        mAcch.ClrWallHit();
        mAcch.ClrGroundHit();
        mAcch.SetWallNone();
        mAcch.SetGrndNone();
        field_0xa00 = 0.0f;
        mStts.SetWeight(0);
        speed.setall(0.0f);
        speedF = 0.0f;
        gravity = 0.0f;
        field_0x9ec.setall(0);
        field_0xa42 = 0;
        field_0xa43 = 0;
        field_0xa44 = 0;
        field_0xa47 = 0;
    } else {
        mStts.SetWeight(daObj_Mie_Param_c::m.weight);
        mAcch.ClrWallNone();
        mAcch.ClrGrndNone();
        if (field_0xa46 != 0 && cM3d_IsZero(speedF) == 0) {
            fopAcM_carryOffRevise(this);
            popup(daObj_Mie_Param_c::m.firing_rate,
                  daObj_Mie_Param_c::m.launch_angle, NULL);
        } else {
            fopAcM_getWaterY(&current.pos, &field_0xa04);
            if (field_0xa04 != -G_CM3D_F_INF && dVar16 < field_0xa04 - field_0xa00 &&
                current.pos.y <= field_0xa04 && field_0xa43 == 0)
            {
                if (field_0xa44 != 0 || field_0xa47 != 0 || field_0xa46 != 0) {
                    setWaterPrtcl();
                }

                if (field_0xa44 != 0 || field_0xa47 != 0) {
                    if (speed.y < 0.0f) {
                        field_0xa0c = 3000.0f;
                    }
                    field_0xa44 = 0;
                    field_0xa47 = 0;
                }
                speedF *= 0.3f;
                speed.y *= 0.5f;
                field_0xa42 = 0;
                field_0xa43 = 1;
            }
            if (field_0xa43 != 0 && field_0xa42 == 0) {
                if (speed.y < 0.0f) {
                    cLib_addCalc(&speed.y, 2.0f, 0.5f, 5.5f, 0.5f);
                } else {
                    cLib_addCalc(&speed.y, 2.0f, 0.5f, 0.5f, 0.5f);
                }

                if (field_0xa04 < current.pos.y + dVar16) {
                    field_0x9ec.y = 0x100;
                    mAcch.ClrGroundHit();
                    field_0xa42 = 1;
                }
            }
            if (field_0xa42 != 0) {
                if (mAcch.ChkGroundHit()) {
                    field_0xa42 = 0;
                    field_0xa43 = 0;
                    field_0x9ec.setall(0);
                    speedF = 0;
                    speed.setall(0.0f);
                } else {
                    cXyz local_98;
                    int local_ac;
                    if (dPath_GetPolyRoomPathVec(mGndChk, &local_98, &local_ac) != 0) {
                        local_98.normalizeZP();
                        cLib_addCalcAngleS2(&current.angle.y, cM_atan2s(local_98.x, local_98.z), 8, 0x400);
                        s16 local_b0 = field_0x9ec.y < 0 ? -0x80 : 0x80;
                        cLib_chaseAngleS(&field_0x9ec.y, local_b0, 0x10);
                        cLib_addCalc2(&speedF, local_ac * 0.75f, 0.25f, 1.0f);
                    } else {
                        cLib_chaseF(&speedF, 0.0f, 0.1f);
                    }
                    cLib_addCalc2(&current.pos.y, field_0xa04 - dVar16, 0.5f, 2.0f);
                    speed.y = 0.0f;
                    setHamonPrtcl();
                    gravity = 0;
                    if (field_0x9f4 == 0 &&
                         mAcch.ChkWallHit() &&
                        getWallAngle(current.angle.y, &local_b6) != 0)
                    {
                        field_0x9f4 = 10;
                        s16 local_b4 = current.angle.y - local_b6;
                        current.angle.y += (s16)(0x8000 - (local_b4 << 1) + (s16)cM_rndFX(2000.0f));
                        field_0x9ec.y = -field_0x9ec.y / 2;
                        speedF *= 0.3f;
                    }
                }
            } else {
                if (field_0xa43 != 0) {
                    cLib_chaseF(&speedF, 0.0f, 0.3f);
                    gravity = 0;
                } else {
                    if (field_0xa44 != 0) {
                        if ( mAcch.ChkWallHit() &&
                            getWallAngle(current.angle.y, &local_b6) != 0)
                        {
                            s16 local_b4 = current.angle.y - local_b6;
                            current.angle.y += (s16)(0x8000 - (local_b4 << 1) + (s16)cM_rndFX(1000.0f));
                            speedF *= 0.5f;
                        }
                        if (mAcch.ChkGroundLanding()) {
                            setSmokePrtcl();
                            field_0xa44 = 0;
                            speedF = 0;
                            speed.setall(0.0f);
                        }
                    } else {
                        if (field_0xa47 != 0) {
                            if (mAcch.ChkGroundLanding()) {
                                setSmokePrtcl();
                                field_0xa47 = 0;
                                speedF = 0.0f;
                                speed.setall(0.0f);
                            }
                        } else if (field_0xa46 != 0) {
                            setSmokePrtcl();
                        } else if (!mAcch.ChkGroundHit()) {
                            field_0xa47 = 1;
                        }
                    }
                }
                
            }

            if (field_0xa45 == 0 && field_0xa44 == 0 &&
                field_0xa47 == 0)
            {
                fopAcM_OnCarryType(this, fopAcM_CARRY_SIDE);
                cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
            }
        }
    }

    field_0x9d4 = speed;

    if (field_0xa43 != 0) {
        f32 fVar1;
        if (mStts.GetCCMoveP() != NULL) {
            fVar1 = 1200.0f;
        } else {
            fVar1 = 900.0f;
        }
        cLib_addCalc2(&field_0xa0c, fVar1, 0.05f, 100.0f);
    } else {
        cLib_addCalc2(&field_0xa0c, 0.0f, 0.25f, 100.0f);
    }
    shape_angle.x = field_0xa0c * cM_ssin(field_0xa10 * 1000);
    shape_angle.z = field_0xa0c * cM_scos(field_0xa10 * 1000);
    cLib_chaseAngleS(&field_0x9ec.y, 0, 10);
    shape_angle.y += field_0x9ec.y;
    field_0xa10++;
    cLib_chaseF(&field_0x9fc, 0.0f, 0.05f);
    if (field_0xa42 != 0) {
        if (field_0x9f8 == 0) {
            f32 sqrZ = mStts.GetCCMoveP()->z * mStts.GetCCMoveP()->z;
            f32 sqrX = mStts.GetCCMoveP()->x * mStts.GetCCMoveP()->x;
            f32 dist = JMAFastSqrt(sqrX + sqrZ);
            if (1.0f < dist) {
                cXyz* move_p = mStts.GetCCMoveP();
                field_0xa18 = cM_atan2s(mStts.GetCCMoveP()->x, move_p->z);
                field_0x9fc += dist * 0.45f;
                if (4.0f < field_0x9fc) {
                    field_0x9fc = 4.0f;
                }
                field_0x9ec.y = 0x200;
                field_0x9f8 = 10;
            }
        }
    } else {
        field_0x9fc = 0;
    }

    field_0x9e0.x = field_0x9fc * cM_ssin(field_0xa18);
    field_0x9e0.z = field_0x9fc * cM_scos(field_0xa18);
    field_0x9e0.y = 0;
    if (local_a8 == 0 && field_0xa40 == 0) {
        fopAcM_calcSpeed(this);
        speed += field_0x9e0;
        fopAcM_posMove(this, mStts.GetCCMoveP());
    }

    if (field_0xa42 != 0) {
        fopAcM_riverStream(&current.pos, &field_0xa16, &field_0xa08, 0.5f);
    }

    mAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mAcch.m_gnd;
    if (local_a8 == 0 && field_0xa40 == 0) {
        field_0xa00 = mAcch.GetGroundH();
    }

    if (field_0xa00 != -G_CM3D_F_INF) {
        field_0xa12 = daNpcF_getGroundAngle(&mGndChk, shape_angle.y);
        setEnvTevColor();
        setRoomNo();
    }

    attention_info.position = current.pos;
    attention_info.position.y += daObj_Mie_Param_c::m.focus_offset;
    eyePos = current.pos;
    setMtx();
    field_0xa14 = calcRollAngle(field_0xa14, 0x10000);
    if (field_0xa45 == 0 && field_0xa41 == 0) {
        mCyl.SetR(daObj_Mie_Param_c::m.width);
        mCyl.SetH(daObj_Mie_Param_c::m.height);
        mCyl.SetC(current.pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }
    mCyl.ClrCoHit();
    field_0xa46 = local_a8 ? 1 : 0;
    cLib_calcTimer(&field_0x9f4);
    cLib_calcTimer(&field_0x9f8);
    return 1;
}

int daObj_Mie_c::Draw() {
    if (field_0xa45 == 0) {
        J3DModelData* modelData = mModel->getModelData();
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
        fopAcM_setEffectMtx(this, modelData);
        mDoExt_modelUpdateDL(mModel);
        fopAc_ac_c* local_48 = NULL;
        fopAcM_SearchByID(daPy_getPlayerActorClass()->getGrabActorID(), &local_48);
        if (local_48 == this) {
            model = mModel;
        } else if (field_0xa00 != -G_CM3D_F_INF) {
            cM3dGPla plain;
            if (dComIfG_Bgsp().GetTriPla(mGndChk, &plain)) {
                dComIfGd_setSimpleShadow(&current.pos, field_0xa00, 20.0f,
                                                         &plain.mNormal,
                                                         0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
            }
        }
    }
    return 1;
}

int daObj_Mie_c::createHeapCallBack(fopAc_ac_c* a_this) {
    return ((daObj_Mie_c*)a_this)->CreateHeap();
}

void* daObj_Mie_c::srchPouyaa(void* param_1, void* param_2) {
    if (fopAcM_IsActor(param_1) && param_1 != param_2 &&
        fopAcM_GetName(param_1) == PROC_NPC_POUYA &&
        static_cast<daNpc_Pouya_c*>(param_1)->getType() == 1)
    {
        return param_1;
    }
    return NULL;
}

void daObj_Mie_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daObj_Mie_c::setRoomNo() {
    s32 roomId = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, roomId);
    mStts.SetRoomId(roomId);
}

void daObj_Mie_c::setMtx() {
    daNpc_Pouya_c* pouya = NULL;
    if (field_0xa40 != 0) {
        pouya = (daNpc_Pouya_c*)fopAcIt_Judge(srchPouyaa, 0);
    }

    if (pouya != NULL) {
        mDoMtx_stack_c::copy(pouya->getHeadMtx());
    } else {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::scaleM(scale);
    }

    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

s16 daObj_Mie_c::calcRollAngle(s16 param_1, int param_2) {
    int r27 = 4;
    if (param_1 != 0) {
        int iVar1 = param_2 - (u16)param_1;
        int rv;
        if (abs(iVar1) > r27) {
            iVar1 /= r27;
            if (iVar1 > 0x800) {
                rv = (u16)param_1 + 0x800;
            } else {
                if (iVar1 < -0x800) {
                    rv = (u16)param_1 - 0x800;
                } else {
                    rv = (u16)param_1 + iVar1;
                }
            }
        } else {
            rv = param_2;
        }
        return rv;
    }
    return 0;
}

int daObj_Mie_c::getWallAngle(s16 param_1, s16* param_2) {
    cXyz cStack_50;
    cXyz acStack_40[2];
    cXyz local_5c;
    mDoMtx_stack_c::YrotS(param_1);
    local_5c.set(0.0f, 0.0f, -50.0f);
    mDoMtx_stack_c::multVec(&local_5c, &cStack_50);
    cStack_50 += current.pos;
    local_5c.set(5.0f, 0.0f, 200.0f);
    for (int i = 0; i < 2; i++) {
        mDoMtx_stack_c::multVec(&local_5c, &acStack_40[i]);
        local_5c.x = -1.0f * local_5c.x;
        acStack_40[i] += cStack_50;
        mLinChk.Set(&cStack_50, &acStack_40[i], this);
        if (dComIfG_Bgsp().LineCross(&mLinChk)) {
            acStack_40[i] = mLinChk.GetCross();
        } else {
            return 0;
        }
    }

    local_5c = acStack_40[1] - acStack_40[0];
    *param_2 = cM_atan2s(local_5c.x, local_5c.z) + 0x4000;
    return 1;
}

void daObj_Mie_c::setSmokePrtcl() {
    fopAcM_effSmokeSet1(&field_0xa38, &field_0xa3c, &current.pos, NULL, 0.4f, &tevStr, 1);
    dComIfGp_particle_levelEmitterOnEventMove(field_0xa38);
    dComIfGp_particle_levelEmitterOnEventMove(field_0xa3c);
}

static daObj_Mie_Param_c l_HIO;

void daObj_Mie_c::setWaterPrtcl() {
    static u16 emttrId[4] = {
        0x01B8, 0x01B9, 0x01BA, 0x01BB,
    };
    static cXyz scl(0.4f, 0.4f, 0.4f);
    cXyz acStack_28(current.pos.x, field_0xa04, current.pos.z);
    for (int i = 0; i < 4; i++) {
        field_0xa20[i] = dComIfGp_particle_set(field_0xa20[i], emttrId[i], &acStack_28, &tevStr,
                                               NULL, &scl, 0xff, NULL, -1, NULL, NULL, NULL);
        dComIfGp_particle_levelEmitterOnEventMove(field_0xa20[i]);
    }
}

void daObj_Mie_c::setHamonPrtcl() {
    cXyz cStack_18(current.pos.x, field_0xa04, current.pos.z);
    fopAcM_effHamonSet(&field_0xa30, &cStack_18, 0.7f, 0.05f);
}

static int daObj_Mie_Create(void* a_this) {
    return ((daObj_Mie_c*)a_this)->create();
}

static int daObj_Mie_Delete(void* a_this) {
    return ((daObj_Mie_c*)a_this)->Delete();
}

static int daObj_Mie_Execute(void* a_this) {
    return ((daObj_Mie_c*)a_this)->Execute();
}

static int daObj_Mie_Draw(void* a_this) {
    return ((daObj_Mie_c*)a_this)->Draw();
}

static int daObj_Mie_IsDelete(void* a_this) {
    return 1;
}

AUDIO_INSTANCES

static actor_method_class daObj_Mie_MethodTable = {
    (process_method_func)daObj_Mie_Create,
    (process_method_func)daObj_Mie_Delete,
    (process_method_func)daObj_Mie_Execute,
    (process_method_func)daObj_Mie_IsDelete,
    (process_method_func)daObj_Mie_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_MIE = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_MIE,           // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObj_Mie_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  86,                     // mPriority
  &daObj_Mie_MethodTable, // sub_method
  0x00044100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
