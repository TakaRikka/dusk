/**
 * @file d_a_obj_gadget.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_gadget.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc4.h"
#include "d/d_path.h"
#include "d/d_cc_d.h"
#include "Z2AudioLib/Z2Instances.h"
#include "SSystem/SComponent/c_counter.h"
#include "JSystem/JHostIO/JORFile.h"

#if DEBUG
#define PARAM field_0x568->mAttr
#define PARAM_CLASS daObj_Gadget_HIO_c

class daObj_Gadget_HIO_c : public mDoHIO_entry_c {
public:
    daObj_Gadget_HIO_c() {
        mAttr = daObj_Gadget_Param_c::m;
    }
    void genMessage(JORMContext* ctx);
    void listenPropertyEvent(const JORPropertyEvent*);

    /* 0x8 */ daObj_Gadget_HIOParam mAttr;
};
#else
#define PARAM daObj_Gadget_Param_c::m
#define PARAM_CLASS daObj_Gadget_Param_c
#endif


daObj_Gadget_HIOParam const daObj_Gadget_Param_c::m = {
    0.0f, -4.0f, 1.0f, 100.0f, 37.0f, 26.0f, 13.0f, 14.0f,
     37.0f, 50.0f, 18.0f,
};

static dCcD_SrcGObjInf const l_ccDObjData = 
{
        {0x0, {{0x0, 0x0, 0}, {0xD8FBFDFF, 0x1F}, 0x79}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
        {0x0}, // mGObjCo
};

static u32 l_bmdData[2][2] = {
    35, 1, 34, 1,
};

static char* l_resNameList[2] = {
    "",
    "ykM1",
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

void daObj_Gadget_HIO_c::listenPropertyEvent(const JORPropertyEvent* i_event) {
    char buffer[2000];
    size_t len;
    JORReflexible::listenPropertyEvent(i_event);
    JORFile jorFile;
    switch ((u32)i_event->id) {
    case 0x40000002:
        if (jorFile.open(6, "すべてのファイル(*.*)", NULL, NULL, NULL)) {
            memset(buffer, 0, 2000);
            len = 0;
            sprintf(buffer + len, "%.3ff,\t//  注目オフセット\n", mAttr.focus_offset);
            len = strlen(buffer);
            sprintf(buffer + len, "%.3ff,\t//  重力\n", mAttr.gravity);
            len = strlen(buffer);
            sprintf(buffer + len, "%.3ff,\t//  スケ−ル\n", mAttr.scale);
            len = strlen(buffer);
            sprintf(buffer + len, "%.3ff,\t//  リアル影サイズ\n", mAttr.real_shadow_size);
            len = strlen(buffer);
            sprintf(buffer + len, "%.3ff,\t//  体重\n", mAttr.weight);
            len = strlen(buffer);
            sprintf(buffer + len, "%.3ff,\t//  高さ\n", mAttr.height);
            len = strlen(buffer);
            sprintf(buffer + len, "%.3ff,\t//  ひざ丈\n", mAttr.knee_length);
            len = strlen(buffer);
            sprintf(buffer + len, "%.3ff,\t//  幅\n", mAttr.width);
            len = strlen(buffer);
            sprintf(buffer + len, "%.3ff,\t//  発射速度\n", mAttr.fire_rate);
            len = strlen(buffer);
            sprintf(buffer + len, "%.3ff,\t//  発射角度\n", mAttr.launch_angle);
            len = strlen(buffer);
            sprintf(buffer + len, "%.3ff,\t//  浮きオフセット\n", mAttr.floating_offset);
            len = strlen(buffer);
            jorFile.writeData(buffer, len);
            jorFile.close();
            OS_REPORT("write append success!::%6d\n", len);
        } else {
            OS_REPORT("write append failure!\n");
        }
    }
}

void daObj_Gadget_HIO_c::genMessage(JORMContext* ctx) {
    ctx->genSlider("注目オフセット  ", &mAttr.focus_offset, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("重力            ", &mAttr.gravity, -100.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("スケ−ル        ", &mAttr.scale, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("リアル影サイズ  ", &mAttr.real_shadow_size, 0.0f, 10000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    ctx->genSlider("体重            ", &mAttr.weight, 0.0f, 255.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("高さ            ", &mAttr.height, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("ひざ丈          ", &mAttr.knee_length, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("幅              ", &mAttr.width, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("発射速度        ", &mAttr.fire_rate, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("発射角度        ", &mAttr.launch_angle, 0.0f, 90.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("浮きオフセット  ", &mAttr.floating_offset, -100.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    ctx->genButton("ファイル書き出し", 0x40000002, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

#endif

static PARAM_CLASS l_HIO;

daObj_Gadget_c::~daObj_Gadget_c() {
    OS_REPORT("|%06d:%x|daObj_Gadget_c -> デストラクト\n", g_Counter.mCounter0, this);
    
    #if DEBUG
    if (field_0x568 != NULL) {
        field_0x568->removeHIO();
    }
    #endif

    dComIfG_resDelete(
        &mPhase,
        l_resNameList[l_bmdData[field_0x9d0][1]]);
}

int daObj_Gadget_c::create() {
    fopAcM_ct(this, daObj_Gadget_c);
    field_0x9d0 = getType();
    int rv = dComIfG_resLoad(&mPhase, l_resNameList[l_bmdData[field_0x9d0][1]]);
    if (rv == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0)) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("\t(%s:%d) <%08x>\n", fopAcM_getProcNameString(this), getType(), fopAcM_GetParam(this));
        J3DModelData* modelData = mModel->getModelData();
        fopAcM_SetMtx(this, mModel->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -300.0f, -50.0f, -300.0f, 300.0f, 450.0f, 300.0f);

        #if DEBUG
        field_0x568 = &l_HIO;
        field_0x568->entryHIO("小物");
        #endif

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mStts.Init(PARAM.weight, 0, this);
        mCyl.Set(l_ccDCyl);
        mCyl.SetStts(&mStts);
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        field_0xa00 = mAcch.GetGroundH();

        if (field_0xa00 != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }

        reset();
        Execute();
    }

    return rv;
}

int daObj_Gadget_c::CreateHeap() {
    J3DModelData* mdlData_p = (J3DModelData*)dComIfG_getObjectRes(
        l_resNameList[l_bmdData[field_0x9d0][1]],
        l_bmdData[field_0x9d0][0]);
    JUT_ASSERT(384, NULL != mdlData_p)
    mModel = mDoExt_J3DModel__create(mdlData_p, 0x80000, 0x11000084);
    return mModel != NULL ? TRUE : FALSE;
}

int daObj_Gadget_c::Delete() {
    OS_REPORT("|%06d:%x|daObj_Gadget_c -> Delete\n", g_Counter.mCounter0, this);
    this->~daObj_Gadget_c();
    return 1;
}

int daObj_Gadget_c::Execute() {
    BOOL carryNow = fopAcM_checkCarryNow(this) != FALSE;
    
    f32 floatingOffset = PARAM.floating_offset;
    f32 scaleParam = PARAM.scale;
    s16 wallAngle;
    scale.set(PARAM.scale, PARAM.scale, PARAM.scale);
    attention_info.flags = 0;
    attention_info.distances[fopAc_attn_CARRY_e] = 6;
    mAcchCir.SetWallR(PARAM.width);
    mAcchCir.SetWallH(PARAM.knee_length);
    gravity = PARAM.gravity;
    
    if (carryNow) {
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
        field_0xa41 = 0;
        field_0xa42 = 0;
        field_0xa43 = 0;
        field_0xa46 = 0;
    } else {
        mStts.SetWeight(PARAM.weight);
        mAcch.ClrWallNone();
        mAcch.ClrGrndNone();
        
        if (field_0xa45 != 0 && cM3d_IsZero(speedF) == FALSE) {
            fopAcM_carryOffRevise(this);
            popup(PARAM.fire_rate, PARAM.launch_angle, NULL);
        } else {
            fopAcM_getWaterY(&current.pos, &field_0xa04);
            
            if (field_0xa04 != -G_CM3D_F_INF && 
                floatingOffset < field_0xa04 - field_0xa00 &&
                current.pos.y <= field_0xa04 && 
                field_0xa42 == 0) {
                
                if (field_0xa43 != 0 || field_0xa46 != 0 || field_0xa45 != 0) {
                    setWaterPrtcl();
                }
                
                if (field_0xa43 != 0 || field_0xa46 != 0) {
                    if (speed.y < 0.0f) {
                        field_0xa0c = 3000.0f;
                    }
                    field_0xa43 = 0;
                    field_0xa46 = 0;
                }
                
                speedF *= 0.3f;
                speed.y *= 0.5f;
                field_0xa41 = 0;
                field_0xa42 = 1;
            }
            
            if (field_0xa42 != 0 && field_0xa41 == 0) {
                if (speed.y < 0.0f) {
                    cLib_addCalc(&speed.y, 2.0f, 0.5f, 5.5f, 0.5f);
                } else {
                    cLib_addCalc(&speed.y, 2.0f, 0.5f, 0.5f, 0.5f);
                }
                
                if (field_0xa04 < (current.pos.y + floatingOffset)) {
                    field_0x9ec.y = 0x100;
                    mAcch.ClrGroundHit();
                    field_0xa41 = 1;
                }
            }
            
            if (field_0xa41 != 0) {
                if (mAcch.ChkGroundHit()) {
                    field_0xa41 = 0;
                    field_0xa42 = 0;
                    field_0x9ec.setall(0);
                    speedF = 0.0f;
                    speed.setall(0.0f);
                } else {
                    cXyz pathVec;
                    int pathNum;
                    if (dPath_GetPolyRoomPathVec(mGndChk, &pathVec, &pathNum)) {
                        pathVec.normalizeZP();
                        s16 targetAngle = cM_atan2s(pathVec.x, pathVec.z);
                        cLib_addCalcAngleS2(&current.angle.y, targetAngle, 8, 0x400);
                        
                        s16 rollTarget = (field_0x9ec.y < 0) ? -0x80 : 0x80;
                        cLib_chaseAngleS(&field_0x9ec.y, rollTarget, 0x10);
                        cLib_addCalc2(&speedF, pathNum * 0.75f, 0.25f, 1.0f);
                    } else {
                        cLib_chaseF(&speedF, 0.0f, 0.1f);
                    }
                    
                    cLib_addCalc2(&current.pos.y, field_0xa04 - floatingOffset, 0.5f, 2.0f);
                    speed.y = 0.0f;
                    setHamonPrtcl();
                    gravity = 0.0f;
                    
                    if (field_0x9f4 == 0 && mAcch.ChkWallHit()) {
                        if (getWallAngle(current.angle.y, &wallAngle)) {
                            field_0x9f4 = 10;
                            s16 angleDiff = current.angle.y - wallAngle;
                            current.angle.y += (s16)((0x8000 - (angleDiff * 2)) + (s16)cM_rndFX(2000.0f));
                            field_0x9ec.y = -field_0x9ec.y / 2;
                            speedF *= 0.3f;
                        }
                    }
                }
            } else if (field_0xa42 != 0) {
                cLib_chaseF(&speedF, 0.0f, 0.3f);
                gravity = 0.0f;
            } else if (field_0xa43 != 0) {
                if (mAcch.ChkWallHit()) {
                    if (getWallAngle(current.angle.y, &wallAngle)) {
                        s16 angleDiff = current.angle.y - wallAngle;
                        current.angle.y += (s16)((0x8000 - (angleDiff << 1)) + (s16)cM_rndFX(1000.0f));
                        speedF *= 0.5f;
                    }
                }
                
                if (mAcch.ChkGroundLanding()) {
                    setSmokePrtcl();
                    field_0xa43 = 0;
                    speedF = 0.0f;
                    speed.setall(0.0f);
                }
            } else if (field_0xa46 != 0) {
                if (mAcch.ChkGroundLanding()) {
                    setSmokePrtcl();
                    field_0xa46 = 0;
                    speedF = 0.0f;
                    speed.setall(0.0f);
                }
            } else if (field_0xa45 != 0) {
                setSmokePrtcl();
            } else if (!mAcch.ChkGroundHit()) {
                field_0xa46 = 1;
            }
            
            if (mHide == 0 && field_0xa43 == 0 && field_0xa46 == 0) {
                fopAcM_OnCarryType(this, fopAcM_CARRY_SIDE);
                cLib_onBit(attention_info.flags, (u32)fopAc_AttnFlag_CARRY_e);
            }
        }
    }
    
    field_0x9d4 = speed;
    
    if (field_0xa42 != 0) {
        f32 targetWave = mStts.GetCCMoveP() ? 1200.0f : 900.0f;
        cLib_addCalc2(&field_0xa0c, targetWave, 0.05f, 100.0f);
    } else {
        cLib_addCalc2(&field_0xa0c, 0.0f, 0.25f, 100.0f);
    }
    
    shape_angle.x = field_0xa0c * cM_ssin(field_0xa10 * 1000);
    shape_angle.z = field_0xa0c * cM_scos(field_0xa10 * 1000);
    
    cLib_chaseAngleS(&field_0x9ec.y, 0, 10);
    shape_angle.y += field_0x9ec.y;
    field_0xa10++;
    
    cLib_chaseF(&field_0x9fc, 0.0f, 0.05f);
    
    if (field_0xa41 != 0) {
        if (field_0x9f8 == 0) {
            f32 moveSpeed = JMAFastSqrt(mStts.GetCCMoveP()->x * mStts.GetCCMoveP()->x + mStts.GetCCMoveP()->z * mStts.GetCCMoveP()->z);
            if (1.0f < moveSpeed) {
                field_0xa18 = cM_atan2s(mStts.GetCCMoveP()->x, mStts.GetCCMoveP()->z);
                field_0x9fc += moveSpeed * 0.45f;
                if (4.0f < field_0x9fc) {
                    field_0x9fc = 4.0f;
                }
                field_0x9ec.y = 0x200;
                field_0x9f8 = 10;
            }
        }
    } else {
        field_0x9fc = 0.0f;
    }
    
    field_0x9e0.x = field_0x9fc * cM_ssin(field_0xa18);
    field_0x9e0.z = field_0x9fc * cM_scos(field_0xa18);
    field_0x9e0.y = 0.0f;
    
    if (!carryNow) {
        fopAcM_calcSpeed(this);
        speed += field_0x9e0;
        fopAcM_posMove(this, mStts.GetCCMoveP());
    }
    
    if (field_0xa41 != 0) {
        fopAcM_riverStream(&current.pos, &field_0xa16, &field_0xa08, 0.5f);
    }
    
    mAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mAcch.m_gnd;
    
    if (!carryNow) {
        field_0xa00 = mAcch.GetGroundH();
    }
    
    if (field_0xa00 != -G_CM3D_F_INF) {
        field_0xa12 = daNpcF_getGroundAngle(&mGndChk, shape_angle.y);
        setEnvTevColor();
        setRoomNo();
    }
    
    attention_info.position = current.pos;
    attention_info.position.y += PARAM.focus_offset;
    eyePos = current.pos;
    setMtx();
    field_0xa14 = calcRollAngle(field_0xa14, 0x10000);
    
    if (mHide == 0 && field_0xa40 == 0) {
        mCyl.SetR(PARAM.width);
        mCyl.SetH(PARAM.height);
        mCyl.SetC(current.pos);
        dComIfG_Ccsp()->Set(&mCyl);
    }

    mCyl.ClrCoHit();
    field_0xa45 = carryNow ? 1 : 0;
    cLib_calcTimer(&field_0x9f4);
    cLib_calcTimer(&field_0x9f8);
    
    return 1;
}

int daObj_Gadget_c::Draw() {
    if (mHide == 0) {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
        mDoExt_modelUpdateDL(mModel);
        
        fopAc_ac_c* carryActor = NULL;
        fopAcM_SearchByID(daPy_getPlayerActorClass()->getGrabActorID(), &carryActor);

        if (carryActor == this) {
            model = mModel;
        } else if (field_0xa00 != -G_CM3D_F_INF) {
            cM3dGPla groundPla;
            if (dComIfG_Bgsp().GetTriPla(mGndChk, &groundPla)) {
                dComIfGd_setSimpleShadow(&current.pos, field_0xa00, 20.0f, &groundPla.mNormal, 0,
                                         1.0f, dDlst_shadowControl_c::getSimpleTex());
            }
        }
    }
    return 1;
}

int daObj_Gadget_c::createHeapCallBack(fopAc_ac_c* a_this) {
    return ((daObj_Gadget_c*)a_this)->CreateHeap();
}

void daObj_Gadget_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daObj_Gadget_c::setRoomNo() {
    s8 room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, room_no);
    mStts.SetRoomId(room_no);
}

void daObj_Gadget_c::reset() {
    memset(&field_0x9d4, 0, (u8*)&field_0xa47 - (u8*)&field_0x9d4);
    switch (field_0x9d0) {
    case 0:
    case 1:
        mHide = 1;
        break;
    }
}

void daObj_Gadget_c::setMtx() {
    s16 angleDiff = shape_angle.y - current.angle.y;
    s16 cosVal = field_0xa14 * cM_scos(angleDiff);
    s16 sinVal = field_0xa14 * cM_ssin(angleDiff);

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::XrotM(cosVal);
    mDoMtx_stack_c::ZrotM(sinVal);
    mDoMtx_stack_c::scaleM(scale);
    
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

s16 daObj_Gadget_c::calcRollAngle(s16 param_1, int param_2) {
    int result;
    if (param_1 != 0) {
        result = param_2 - (u16)param_1;
        if (abs(result) > 4) {
            result /= 4;
            if (result > 0x800) {
                result = (u16)param_1 + 0x800;
            } else {
                if (result < -0x800) {
                    result = (u16)param_1 - 0x800;
                } else {
                    result = (u16)param_1 + result;
                }
            }
        } else {
            result = param_2;
        }
        return result;
    }
    return 0;
}

BOOL daObj_Gadget_c::getWallAngle(s16 param_0, s16* param_1) {
    cXyz startPos;
    cXyz checkPos[2];
    cXyz direction;
    
    mDoMtx_stack_c::YrotS(param_0);
    direction.set(0.0f, 0.0f, -50.0f);
    mDoMtx_stack_c::multVec(&direction, &startPos);
    startPos += current.pos;
    
    direction.set(5.0f, 0.0f, 200.0f);
    
    for (int i = 0; i < 2; i++) {
        mDoMtx_stack_c::multVec(&direction, &checkPos[i]);
        direction.x = direction.x * -1.0f;
        checkPos[i] += startPos;
        
        mLinChk.Set(&startPos, &checkPos[i], this);
        if (dComIfG_Bgsp().LineCross(&mLinChk)) {
            checkPos[i] = mLinChk.GetCross();
        } else {
            return FALSE;
        }
    }
    
    direction = checkPos[1] - checkPos[0];
    s16 wallAngle = cM_atan2s(direction.x, direction.z);
    *param_1 = wallAngle + 0x4000;
    return TRUE;
}

void daObj_Gadget_c::setSmokePrtcl() {
    fopAcM_effSmokeSet1(&field_0xa38, &field_0xa3c, &current.pos, NULL, 0.4f, &tevStr, 1);
    dComIfGp_particle_levelEmitterOnEventMove(field_0xa38);
    dComIfGp_particle_levelEmitterOnEventMove(field_0xa3c);
}

void daObj_Gadget_c::setWaterPrtcl() {
    static cXyz scl(0.4f, 0.4f, 0.4f);
    static u16 emttrId[4] = {
        ID_ZI_J_DOWNWTRA_A,
        ID_ZI_J_DOWNWTRA_B,
        ID_ZI_J_DOWNWTRA_C,
        ID_ZI_J_DOWNWTRA_D,
    };

    cXyz pos(current.pos.x, field_0xa04, current.pos.z);

    for (int i = 0; i < 4; i++) {
        field_0xa20[i] = dComIfGp_particle_set(field_0xa20[i], emttrId[i], &pos, &tevStr, 0, &scl,
                                               0xff, 0, -1, 0, 0, 0);
        dComIfGp_particle_levelEmitterOnEventMove(field_0xa20[i]);
    }
}

void daObj_Gadget_c::setHamonPrtcl() {
    cXyz pos(current.pos.x, field_0xa04, current.pos.z);
    fopAcM_effHamonSet(&field_0xa30, &pos, 0.7f, 0.05f);
}

static int daObj_Gadget_Create(void* i_this) {
    return ((daObj_Gadget_c*)i_this)->create();
}

static int daObj_Gadget_Delete(void* i_this) {
    return ((daObj_Gadget_c*)i_this)->Delete();
}

static int daObj_Gadget_Execute(void* i_this) {
    return ((daObj_Gadget_c*)i_this)->Execute();
}

static int daObj_Gadget_Draw(void* i_this) {
    return ((daObj_Gadget_c*)i_this)->Draw();
}

static int daObj_Gadget_IsDelete(void* i_this) {
    return 1;
}

AUDIO_INSTANCES

static actor_method_class daObj_Gadget_MethodTable = {
    (process_method_func)daObj_Gadget_Create,
    (process_method_func)daObj_Gadget_Delete,
    (process_method_func)daObj_Gadget_Execute,
    (process_method_func)daObj_Gadget_IsDelete,
    (process_method_func)daObj_Gadget_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_GADGET = {
  fpcLy_CURRENT_e,           // mLayerID
  8,                         // mListID
  fpcPi_CURRENT_e,           // mListPrio
  PROC_OBJ_GADGET,           // mProcName
  &g_fpcLf_Method.base,     // sub_method
  sizeof(daObj_Gadget_c),    // mSize
  0,                         // mSizeOther
  0,                         // mParameters
  &g_fopAc_Method.base,      // sub_method
  82,                        // mPriority
  &daObj_Gadget_MethodTable, // sub_method
  0x00044100,                // mStatus
  fopAc_ACTOR_e,             // mActorType
  fopAc_CULLBOX_CUSTOM_e,    // cullType
};
