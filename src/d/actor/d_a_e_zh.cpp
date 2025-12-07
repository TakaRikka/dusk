/**
 * @file d_a_e_zh.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_zh.h"
#include "d/actor/d_a_obj_carry.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/d_camera.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/d_debug_viewer.h"
#include "f_op/f_op_camera_mng.h"

enum E_ZH_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_ZH_ACT01 = 0x5,
    /* 0x06 */ BCK_ZH_CATCH01,
    /* 0x07 */ BCK_ZH_CATCH02,
    /* 0x08 */ BCK_ZH_CATCHDAMAGE,
    /* 0x09 */ BCK_ZH_CATCHDROP,
    /* 0x0A */ BCK_ZH_CATCHMOVE,
    /* 0x0B */ BCK_ZH_OPEN,
    /* 0x0C */ BCK_ZH_PIYO2,
    /* 0x0D */ BCK_ZH_SERCH,
    /* 0x0E */ BCK_ZH_START,
    /* 0x0F */ BCK_ZH_WAIT01,
    /* 0x10 */ BCK_ZH_WAKEUP,

    /* BMDV */
    /* 0x13 */ BMDV_ZH = 0x13,
    /* 0x14 */ BMDV_ZH_ENTRANCE,

    /* BTK */
    /* 0x17 */ BTK_ZH_LOOP = 0x17,
};

enum Action {
    /* 0x00 */ ACTION_EXECUTE_START_DEMO,
    /* 0x01 */ ACTION_EXECUTE_WAIT,
    /* 0x02 */ ACTION_EXECUTE_BALL_WAIT,
    /* 0x03 */ ACTION_EXECUTE_OPEN_START,
    /* 0x04 */ ACTION_EXECUTE_SEARCH_MOVE,
    /* 0x05 */ ACTION_EXECUTE_BG_HIT_SH_MOVE,
    /* 0x06 */ ACTION_EXECUTE_CATCH_MOVE,
    /* 0x07 */ ACTION_EXECUTE_CATCH_FLY_MOVE,
    /* 0x08 */ ACTION_EXECUTE_FLY_DELETE,
    /* 0x09 */ ACTION_EXECUTE_RETURN_DEMO,
    /* 0x0A */ ACTION_EXECUTE_DAMAGE,
    /* 0x0B */ ACTION_EXECUTE_DEAD_STOP,
    /* 0x0C */ UNK_ACTION
};

enum Particle {
    /* 0x8A62 */ ZL2_HAHEN_1 = 0x8A62,
    /* 0x8A63 */ ZL2_SMOKE00_1,
    /* 0x8A64 */ ZL2_HAHEN_2,
    /* 0x8A65 */ ZL2_SMOKE00_2,
    /* 0x8A66 */ ZL2_HAHEN_3,
    /* 0x8A67 */ ZL2_SMOKE00_3,
    /* 0x8A68 */ ZL2_HAHEN_4,
    /* 0x8A69 */ ZL2_SMOKE00_4,
    /* 0x8A6A */ ZL2_SMOKE00_5,
    /* 0x8A6B */ ZL2_HAHEN_5,
    /* 0x8A6C */ ZL2_SMOKE00_6,
    /* 0x8A6D */ ZL2_RING_HALO02,
    /* 0x8A6E */ ZL2_KAITENGIRI128,
    /* 0x8A6F */ ZL2_SHOCK01,
    /* 0x8A70 */ ZL2_SMOKE00_7,
};

namespace {
    static dCcD_SrcSph cc_zhSph_src = {
        {
            {0, {{AT_TYPE_0, 0, 0}, {0xD84860BA, 3}, 0x15}},
            {dCcD_SE_METAL, 0, 0, 0, 0},
            {dCcD_SE_METAL, 1, 0, 0, 3},
            {0}
        },
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f},
        }
    };

    /* 8082F480-8082F4C4 000060 0044+00 0/1 0/0 0/0 .data cc_zhCyl_src__22@unnamed@d_a_e_zh_cpp@ */
    static dCcD_SrcCyl cc_zhCyl_src = {
        {
            {0, {{AT_TYPE_0, 0, 0}, {0xD84860BA, 3}, 0x15}},
            {dCcD_SE_METAL, 0, 0, 0, 0},
            {dCcD_SE_METAL, 1, 0, 0, 3},
            {0}
        },
        {
            {0.0f, 0.0f, 0.0f}, 50.0f, 300.0f,
        }
    };

    /* 8082F4C4-8082F508 0000A4 0044+00 0/1 0/0 0/0 .data cc_zhCyl_at_src__22@unnamed@d_a_e_zh_cpp@ */
    static dCcD_SrcCyl cc_zhCyl_at_src = {
        {
            {0, {{AT_TYPE_CSTATUE_SWING, 4, 0xD}, {0, 0}, 0}},
            {dCcD_SE_METAL, 0, 2, 0, 0},
            {dCcD_SE_NONE, 1, 0, 0, 2},
            {0},
        },
        {
            {0.0f, 0.0f, 0.0f}, 50.0f, 300.0f,
        }
    };
}

daE_ZH_HIO_c::daE_ZH_HIO_c() {
    field_0x4 = -1;
    model_size = 1.0f;
    ascent_rate = 10.0f;
    wandering_area = 500.0f;
    wait_time = 100;
    piyori_time = 300;
    dungeon_bit_check = 0;
}

/* 8082F725 0003+00 data_8082F725 None */
static u8 l_initHIO;

static daE_ZH_HIO_c l_HIO;

static int mStartFlag;

int daE_ZH_c::draw() {
    GXColor i_color, i_color2;
    if (WREG_S(0) != 0) {
        cXyz i_pos = home.pos;
        i_color.r = 0x82;
        i_color.g = 0x82;
        i_color.b = 0xFF;
        i_color.a = 0xFA;

        if (arg0 == 2) {
            i_pos.y += 10.0f;
            dDbVw_drawCircleXlu(i_pos, field_0x778, i_color, 1, 12);
        }

        if (mActionMode == ACTION_EXECUTE_BG_HIT_SH_MOVE && (field_0x6bc.x || field_0x6bc.y || field_0x6bc.z)) {
            i_color2.r = 0x1E;
            i_color2.g = 0x46;
            i_color2.b = 0x9B;
            i_color2.a = 0xFA;
            dDbVw_drawCircleXlu(field_0x6bc, l_HIO.wandering_area, i_color2, 1, 12);
        }

        if (mS_Ball != NULL) {
            i_color.r = 0x82;
            i_color.g = 0x82;
            i_color.b = 0xFF;
            i_color.a = 0xFA;
            i_pos.set(mS_Ball->current.pos);
            i_pos.y += 50.0f;

            for (int i = 0; i < 8; i++) {
                if (field_0x6f8[i].x || field_0x6f8[i].z) {
                    i_color.r = (u8)i * 20 + 100;

                    if (i > 3) {
                        i_color.b = (u8)i * 20 + 100;
                    }

                    i_color.a = field_0x758[i];
                    dDbVw_drawLineXlu(i_pos, field_0x6f8[i], i_color, 1, 12);
                }
            }
        }

        if (WREG_S(1) != 0) {
            i_color.r = 0x82;
            i_color.g = 0x82;
            i_color.b = 0xFF;
            i_color.a = 0xFA;
            dDbVw_drawLineXlu(current.pos, field_0x680, i_color, 1, 12);
        }

        if (WREG_S(2) != 0) {
            i_color.r = 0xFA;
            i_color.g = 0x82;
            i_color.b = 0x82;
            i_color.a = 0xFA;
            i_pos.set(current.pos);
            i_pos.y += 10.0f;
            dDbVw_drawLineXlu(i_pos, field_0x68c, i_color, 1, 12);
        }

        if (WREG_S(3) != 0) {
            i_color.r = 0x7D;
            i_color.g = 0xFF;
            i_color.b = 0x7D;
            i_color.a = 0xFA;
            if (mRetrunStartLine.x || mRetrunStartLine.z) {
                dDbVw_drawLineXlu(mRetrunStartLine, mRetrunEndLine, i_color, 1, 12);
            }
        }

        if (WREG_S(4) != 0 && mS_Ball != NULL) {
            cXyz i_end = mS_Ball->current.pos + mBallHosei;
            i_color.r = 0xFF;
            i_color.g = 0x14;
            i_color.b = 0x14;
            i_color.a = 0xFA;

            for (int i = 0; i < 3; i++) {
                dDbVw_drawLineXlu(field_0x6c8[i], i_end, i_color, 1, 12);
            }
        }
    }

    J3DModelData* modelData;
    J3DMaterial* matNodeP;

    if (arg0 == 2) {
        return 1;
    }

    if (field_0x7ae != 0) {
        return 1;
    }

    #if DEBUG
    mBgc.DrawWall(dComIfG_Bgsp());
    #endif
    
    if (arg0 == 1) {
        g_env_light.settingTevStruct(8, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mEntranceModel, &tevStr);

        modelData = mEntranceModel->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            matNodeP = modelData->getMaterialNodePointer(i);
            if (matNodeP != NULL) {
                matNodeP->getTevKColor(3)->a = field_0x77c;
            }
        }

        mDoExt_modelUpdateDL(mEntranceModel);

        if (mStartFlag == 0) {
            return 1;
        }
    }

    if ((mActionMode == ACTION_EXECUTE_BALL_WAIT && mMoveMode < 2) || (mActionMode == ACTION_EXECUTE_DEAD_STOP && mMoveMode == 10)) {
        return 1;
    }

    J3DModel* model = mpModelMorf->getModel();
    
    g_env_light.settingTevStruct(8, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    modelData = model->getModelData();
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        matNodeP = modelData->getMaterialNodePointer(i);
        if (matNodeP != NULL) {
            matNodeP->getTevColor(1)->r = (int)field_0x764[0] & 0xFF;
            matNodeP->getTevColor(1)->g = (int)field_0x764[1] & 0xFF;
            matNodeP->getTevColor(1)->b = (int)field_0x764[2] & 0xFF;
        }
    }

    mpBtkAnm->entry(model->getModelData());
    fopAcM_setEffectMtx(this, model->getModelData());
    mpModelMorf->entryDL();

    cXyz sp68;
    sp68.set(current.pos.x, current.pos.y + 10.0f, current.pos.z);
    mShadowKey = dComIfGd_setShadow(mShadowKey, 0, model, &sp68, BREG_F(18) + 1200.0f, BREG_F(19) + 0.0f,
                                    current.pos.y, mBgc.GetGroundH(), mBgc.m_gnd,
                                    &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

static int daE_ZH_Draw(daE_ZH_c* i_this) {
    return i_this->draw();
}

void daE_ZH_c::setBck(int i_anm, u8 i_attr, f32 i_morf, f32 i_rate) {
    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_ZH", i_anm), i_attr, i_morf, 
                        i_rate, 0.0f, -1.0f);
    mAnm = i_anm;
}

void daE_ZH_c::setActionMode(int i_action, int i_mode) {
    mActionMode = i_action;
    mMoveMode = i_mode;
}

static void* s_BallSearch(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_Carry && ((daObjCarry_c*)i_actor)->prm_chk_type_lightball() &&
        ((((daE_ZH_c*)i_data)->bitSw == 16 && ((daObjCarry_c*)i_actor)->getType() == 8) || (((daE_ZH_c*)i_data)->bitSw == 17 && ((daObjCarry_c*)i_actor)->getType() == 9))) {
        return i_actor;
    }

    return NULL;
}

static void* s_BallSearch_Tag(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_Carry && ((daObjCarry_c*)i_actor)->prm_chk_type_lightball()) {
        return i_actor;
    }

    return NULL;
}

bool daE_ZH_c::startDemoCheck() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera2 = dComIfGp_getCamera(0);

    if (!eventInfo.checkCommandDemoAccrpt()) {
        fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
        mDemoCamEye = camera2->lookat.eye;
        mDemoCamCenter = camera2->lookat.center;
        return false;
    }

    camera->mCamera.Stop();
    camera->mCamera.SetTrimSize(3);
    daPy_getPlayerActorClass()->changeOriginalDemo();
    return true;
}

void daE_ZH_c::mBallBGCheck() {
    mBallHosei.zero();

    if (mS_Ball != NULL) {
        int i = 0;
        s16 sVar1 = 0;
        cXyz spa4, spb0, start;
        dBgS_LinChk lin_chk;

        start.set(mS_Ball->current.pos);
        start.y += 50.0f;

        for (i = 0; i < 8; i++) {
            cMtx_YrotS(*calc_mtx, sVar1);
            spa4.x = 0.0f;
            spa4.y = 100.0f;
            spa4.z = BREG_F(10) + 180.0f;
            MtxPosition(&spa4, &spb0);
            field_0x6f8[i] = spb0 + mS_Ball->current.pos;
            lin_chk.Set(&start, &field_0x6f8[i], this);

            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                field_0x758[i] ^= 0xFF;
                cMtx_YrotS(*calc_mtx, sVar1 + 0x8000);
                spa4.x = 0.0f;
                spa4.y = 100.0f;
                spa4.z = BREG_F(10) + 180.0f;
                MtxPosition(&spa4, &spb0);

                if (fabsf(mBallHosei.x) < fabsf(spb0.x)) {
                    mBallHosei.x = spb0.x;
                }

                if (fabsf(mBallHosei.z) < fabsf(spb0.z)) {
                    mBallHosei.z = spb0.z;
                }
            } else {
                field_0x758[i] = 0xFF;
            }

            if (i != 3) {
                sVar1 += 0x4000;
            } else if (i == 3) {
                if (mBallHosei.x || mBallHosei.z) {
                    return;
                }

                sVar1 = 0x2000;
            }
        }
    }
}

void daE_ZH_c::mGateOpen() {
    f32 reg_f31 = 0.0f;
    switch (field_0x794[2]) {
        case 0:
            field_0x77c = 0.0f;
            mSound.startCreatureSound(Z2SE_EN_ZH_MV_WALL, 0, -1);
            field_0x794[2]++;
            // fallthrough
        case 1:
            cLib_addCalc2(&field_0x77c, 255.0f, NREG_F(4) + 0.5f, NREG_F(5) + 10.0f);
            break;

        case 2:
            field_0x7a4 += NREG_F(6) + 5.0f;
            reg_f31 = NREG_F(7) + 80.0f;
            reg_f31 += cM_ssin((s16)(field_0x7a4 << 8)) * 40.0f;
            cLib_addCalc2(&field_0x77c, reg_f31, NREG_F(4) + 0.5f, NREG_F(5) + 10.0f);
            break;

        case 3:
            cLib_addCalc0(&field_0x77c, NREG_F(4) + 0.5f, NREG_F(5) + 10.0f);
            break;
    }
}

bool daE_ZH_c::mColorSet(bool param_1, f32 param_2, f32 param_3) {
    bool rv;

    if (!param_1) {
        cLib_addCalc0(&field_0x764[0], 0.8f, param_2);
        cLib_addCalc0(&field_0x764[2], 0.8f, param_3);

        return field_0x764[0] < 1.0f;
    }

    cLib_addCalc2(&field_0x764[0], 255.0f, 0.8f, param_2);
    cLib_addCalc2(&field_0x764[2], 50.0f, 0.8f, param_3);

    if (field_0x764[0] > 253.0f) {
        field_0x764[0] = 255.0f;
        field_0x764[2] = 50.0f;
        return true;
    }

    return false;
}

bool daE_ZH_c::mReturnLineMove() {
    if (mS_Ball == NULL) {
        setActionMode(ACTION_EXECUTE_FLY_DELETE, 0);
        return false;
    }

    static cXyz mRoomLine_dt[6] = {
        cXyz(-3950.0f, -500.0f, 7612.0f),
        cXyz(-3950.0f, -725.0f, 253.0f),
        cXyz(3950.0f, -500.0f, 7442.0f),
        cXyz(3950.0f, -725.0f, 352.0f),
        cXyz(3950.0f, -725.0f, 352.0f),
        cXyz(3950.0f, -725.0f, 352.0f),
    };

    u32 reg_r28;
    u32 uVar1 = 0;
    cXyz sp2c;

    int room_no = fopAcM_GetRoomNo(this);
    switch (room_no) {
        case 1:
            break;

        case 2:
            uVar1 = 1;
            break;

        case 4:
            uVar1 = 2;
            break;

        case 5:
            uVar1 = 3;
            break;

    }

    if (arg0 == 0) {
        mRetrunEndLine.set(home.pos);
        mRetrunStartLine.set(0.0f, 0.0f, -800.0f);
    } else {
        mRetrunEndLine.set(mRoomLine_dt[uVar1]);
        reg_r28 = uVar1 ^ 1;
        mRetrunEndLine.z = mRoomLine_dt[reg_r28].z;
        mRetrunStartLine.set(mRoomLine_dt[uVar1]);
    }

    mRetrunStartLine.y = current.pos.y;
    cLib_addCalc2(&current.pos.x, mRetrunStartLine.x, 0.7f, 5.0f);
    sp2c = mRetrunStartLine - current.pos;
    sp2c.z = 0.0f;
    cLib_addCalcAngleS2(&mRollAngle, 0x400, 8, 0x40);
    cLib_addCalcAngleS2(&current.angle.y, (s16) sp2c.atan2sX_Z(), 8, (s16) mRollAngle);
    cLib_addCalcAngleS2(&shape_angle.y, (s16) current.angle.y, 8, (s16) mRollAngle);

    if (fabsf(current.pos.x - mRetrunStartLine.x) < 20.0f) {
        return true;
    }

    return false;
}

bool daE_ZH_c::mBallBgLineCheck() {
    dBgS_LinChk lin_chk;
    cXyz sp9c, end;
    s16 sVar1 = end.atan2sX_Z();
    s16 sVar2 = 0;
    int iVar1 = 0;

    for (int i = 0; i < 3; i++) {
        cMtx_YrotS(*calc_mtx, sVar1 + sVar2);
        sp9c.x = 0.0f;
        sp9c.y = 100.0f;
        sp9c.z = BREG_F(12) + 170.0f;
        MtxPosition(&sp9c, &field_0x6c8[i]);
        field_0x6c8[i] += current.pos;
        end.set(current.pos);

        if (mS_Ball != NULL) {
            end = mS_Ball->current.pos + mBallHosei;
        }

        end.y += 100.0f;
        lin_chk.Set(&field_0x6c8[i], &end, this);
        field_0x6ec[iVar1] = 0;

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            field_0x6ec[iVar1] = 1;
            iVar1++;
        }

        if (i != 0) {
            sVar2 = -0x4000;
        } else {
            sVar2 = 0x4000;
        }
    }

    if (iVar1 != 0) {
        return true;
    }

    return false;
}

bool daE_ZH_c::mSearchMove(u8 param_1) {
    if (mS_Ball == NULL) {
        setActionMode(ACTION_EXECUTE_FLY_DELETE, 0);
        return NULL;
    }

    cXyz sp50, sp5c;
    bool rv = false;

    mBallBGCheck();

    if (param_1 != 0 || (!mBallHosei.x && !mBallHosei.z)) {
        cMtx_YrotS(*calc_mtx, current.angle.y);
        sp50.x = 0.0f;
        sp50.y = 0.0f;
        sp50.z = field_0x774;
        MtxPosition(&sp50, &sp5c);

        if (param_1 == 0) {
            sp5c += mS_Ball->current.pos;

            if (mBgc.GetGroundH() != -G_CM3D_F_INF && mBgc.GetGroundH() > mS_Ball->current.pos.y) {
                sp5c.y = mBgc.GetGroundH();
            }
        } else {
            sp5c += home.pos;

            if (param_1 == 2) {
                sp5c.y -= 600.0f;
            }
        }
    } else if (mBallHosei.x || mBallHosei.z) {
        sp5c = mS_Ball->current.pos + mBallHosei;
    }

    sp5c.y = current.pos.y;
    f32 suu = cLib_addCalcPos(&current.pos, sp5c, 0.7f, 5.0f, 1.0f);
    sp50 = sp5c - current.pos;
    cLib_addCalcAngleS2(&mRollAngle, BREG_S(5) + 0x400, 8, BREG_S(6) + 0x40);

    if (mBallHosei.x || mBallHosei.z) {
        field_0x7af = 1;
    } else {
        field_0x7af = 0;
    }

    s16 angle;
    if (param_1 == 0) {
        angle = fopAcM_searchActorAngleY(this, mS_Ball);
        Z2GetAudioMgr()->changeBgmStatus(suu + 100.0f);
    } else {
        angle = (s16)sp50.atan2sX_Z();
        Z2GetAudioMgr()->changeBgmStatus(99);
    }

    cLib_addCalcAngleS2(&shape_angle.y, angle, 8, mRollAngle);
    current.angle.y = shape_angle.y;

    if (NREG_S(0) != 0) {
        OS_REPORT("suu %f\n", suu);
        OS_REPORT("angle %x\n", angle);
        OS_REPORT("mRollAngle %x\n", mRollAngle);
        OS_REPORT("mBallHosei.x %f\n", mBallHosei.x);
        OS_REPORT("mBallHosei.z %f\n", mBallHosei.z);
        OS_REPORT("アングル %x\n", abs((s16)(shape_angle.y - angle))); // Angle %x\n
        OS_REPORT("\n");
    }

    if (param_1 != 1) {
        if (suu <= BREG_F(13) + 8.0f && abs((s16)(shape_angle.y - angle)) < 0x1000) {
            rv = true;
        }
    } else if (suu <= BREG_F(14) + 100.0f && abs((s16)(shape_angle.y - angle)) < 0x2000) {
        rv = true;
    }

    if (mBallBgLineCheck()) {
        if (mBgc.ChkWallHit() && mActionMode != ACTION_EXECUTE_CATCH_FLY_MOVE) {
            mRollAngle = 0;
            setActionMode(ACTION_EXECUTE_BG_HIT_SH_MOVE, 0);
            rv = false;
        }
    } else if (mBgc.ChkWallHit() && suu <= BREG_F(15) + 170.0f) {
        field_0x7af = 1;
        rv = true;
    }

    return rv;
}

struct eff_dt {
    /* 0x0 */ u16 field_0x0;
    /* 0x2 */ s16 field_0x2;
    /* 0x4 */ int field_0x4;
};

void daE_ZH_c::mStartParticleSet() {
    static eff_dt const START_EFF_DT[11] = {
        {ZL2_HAHEN_1, 0, 7}, {ZL2_SMOKE00_1, 0, 7}, {ZL2_HAHEN_2, 0, 9}, {ZL2_SMOKE00_2, 0, 9},
        {ZL2_HAHEN_3, 0, 16}, {ZL2_SMOKE00_3, 0, 16}, {ZL2_HAHEN_4, 0, 4}, {ZL2_SMOKE00_4, 0, 4}, 
        {ZL2_SMOKE00_5, 0, 5}, {ZL2_HAHEN_5, 0, 5}, {ZL2_SMOKE00_6, 0, 5},
    };

    for (int i = 0; i < 11; i++) {
        JPABaseEmitter* emitter = dComIfGp_particle_set(START_EFF_DT[i].field_0x0, &current.pos, NULL, NULL);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(mpModelMorf->getModel()->getAnmMtx(START_EFF_DT[i].field_0x4));
        }
    }

    fopAcM_OnStatus(this, 0);
    attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
    mSound.startCreatureSound(Z2SE_EN_ZH_START, 0, -1);
    setBck(BCK_ZH_START, 0, 3.0f, 1.0f);
}

bool daE_ZH_c::mCutTypeCheck() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_JUMP || player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH ||
        player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT || player->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT ||
        player->getCutType() == daPy_py_c::CUT_TYPE_MORTAL_DRAW_A || player->getCutType() == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B) {
        return true;
    }

    if (player->getCutType() == daPy_py_c::CUT_TYPE_HEAD_JUMP || player->getCutType() == daPy_py_c::CUT_TYPE_TWIRL) {
        return true;
    }

    return false;
}

void daE_ZH_c::damage_check() {
    if (field_0x7a8 != 0 || mActionMode == ACTION_EXECUTE_BALL_WAIT || mActionMode == ACTION_EXECUTE_RETURN_DEMO) {
        return;
    }

    if ((l_HIO.dungeon_bit_check == 0 && strcmp(dComIfGp_getStartStageName(), "T_ENE02") != 0 &&
        ((bitSw == 16 && !fopAcM_isSwitch(this, 20)) || (bitSw == 17 && !fopAcM_isSwitch(this, 21))))) {
        return;
    }

    if (field_0xa08.ChkTgHit() || field_0xb40.ChkTgHit()) {
        csXyz local_28;
        cXyz sp20;

        field_0x7a8 = 8;

        if (field_0xa08.ChkTgHit()) {
            mAtInfo.mpCollider = field_0xa08.GetTgHitObj();
            sp20.set(*field_0xa08.GetTgHitPosP());
        } else {
            mAtInfo.mpCollider = field_0xb40.GetTgHitObj();
            sp20.set(*field_0xb40.GetTgHitPosP());
        }

        local_28.x = 0;
        local_28.y = sp20.atan2sX_Z();
        local_28.z = 0;

        if (mActionMode != ACTION_EXECUTE_DEAD_STOP && mActionMode != ACTION_EXECUTE_DAMAGE && mActionMode != ACTION_EXECUTE_OPEN_START && 
            !mAtInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT) && mAnm != BCK_ZH_OPEN && mAnm != BCK_ZH_START) {
            dComIfGp_setHitMark(1, this, &sp20, &local_28, NULL, 0);
            cc_at_check(this, &mAtInfo);
            health = 1000;
            field_0x560 = 1000;
            field_0xa08.ClrTgHit();
            field_0xb40.ClrTgHit();

            if (mActionMode == ACTION_EXECUTE_WAIT) {
                setActionMode(ACTION_EXECUTE_OPEN_START, 0);
            } else {
                if (mCutTypeCheck()) {
                    field_0x78c = 2;
                }

                dComIfGs_offOneZoneSwitch(5, fopAcM_GetRoomNo(this));

                if (mS_Ball != NULL) {
                    mS_Ball->offCarryHookOK();
                }

                setActionMode(ACTION_EXECUTE_DAMAGE, 0);
            }
        } else {
            dCcD_GObjInf* tgHitObj;
            if (!field_0xa08.ChkTgHit()) {
                tgHitObj = (dCcD_GObjInf*)field_0xb40.GetTgHitObj();
            } else {
                tgHitObj = (dCcD_GObjInf*)field_0xa08.GetTgHitObj();
            }

            dComIfGp_setHitMark(2, this, &sp20, &local_28, NULL, 0);

            if (!mAtInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT)) {
                mSound.startCollisionSE(tgHitObj->getHitSeID(tgHitObj->GetAtSe(), 0), 0x28);
            } else {
                mSound.startCollisionSE(Z2SE_HIT_PACHINKO, 0x2D);
            }
        }
    }

    mStts.Move();
}

void daE_ZH_c::executeStartDemo() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp48;
    f32 fVar1, fVar2;

    switch (mMoveMode) {
        case 0:
            if (startDemoCheck()) {
                dComIfGp_getEvent().startCheckSkipEdge(this);
                mDemoCamCenter.set(-30.0f, 650.0f, 1955.0f);
                mDemoCamEye.set(-25.0f, 720.0f, 2330.0f);
                field_0x794[0] = 60;
                mMoveMode++;
            }
            break;

        case 1:
            if (field_0x794[0] != 0) {
                if (field_0x794[0] == 30) {
                    mDemoCamCenter.set(-25.0f, 435.0f, -3355.0f);
                    mDemoCamEye.set(-30.0f, 505.0f, -3140.0f);
                }
                break;
            }

            sp48.set(-10.0f, 280.0f, -3835.0f);
            fVar1 = cLib_addCalcPos(&mDemoCamCenter, sp48, 0.7f, ZREG_F(10) + 10.0f, ZREG_F(11) + 2.0f);
            sp48.set(-15.0f, 350.0f, -3620.0f);
            fVar2 = cLib_addCalcPos(&mDemoCamEye, sp48, 0.7f, ZREG_F(10) + 10.0f, ZREG_F(11) + 2.0f);
            
            if (fVar1 > 4.0f || fVar2 > 4.0f) {
                break;
            }

            field_0x794[0] = 30;
            mMoveMode++;
            // fallthrough
        case 2:
            if (field_0x794[0] == 0) {
                mDemoCamCenter.set(-5.0f, 590.0f, 2130.0f);
                mDemoCamEye.set(-10.0f, 680.0f, 2340.0f);
                camera->mCamera.Reset(mDemoCamCenter, mDemoCamEye);
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                dComIfGp_event_reset();
                fopAcM_onSwitch(this, 0x12);
                fopAcM_OffStatus(this , fopAcM_STATUS_UNK_0x4000);
                setActionMode(ACTION_EXECUTE_WAIT, 1);
            }
            break;
    }

    if (mMoveMode != 0 && mActionMode != ACTION_EXECUTE_WAIT) {
        if (dComIfGp_getEvent().checkSkipEdge()) {
            fopAcM_onSwitch(this, 0x12);
            dStage_changeScene(2, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
        }
    }

    if (mMoveMode != 0) {
        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye);
    }
}

void daE_ZH_c::executeWait() {
    if (mS_Ball != NULL) {
        mS_Ball->current.pos.set(field_0x668);
    }

    switch (mMoveMode) {
        case 0:
            attention_info.distances[2] = 3;
            fopAcM_OnStatus(this, 0);
            attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
            setBck(BCK_ZH_WAIT01, 0, 3.0f, 1.0f);
            field_0x78c = 0;
            mMoveMode = 1;
            break;

        case 1:
            break;
    }

    cLib_addCalcPos(&current.pos, home.pos, 0.7f, 5.0f, 1.0f);
}

void daE_ZH_c::executeBallWait() {
    dBgS_LinChk lin_chk;
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz spa4, spb0;

    switch (mMoveMode) {
        case 0:
            cMtx_YrotS(*calc_mtx, shape_angle.y);
            spa4.x = 0.0f;
            spa4.y = 0.0f;
            spa4.z = WREG_F(1) + 2000.0f;
            MtxPosition(&spa4, &spb0);
            field_0x680 = spb0 + current.pos;
            lin_chk.OnBackFlag();
            lin_chk.Set(&current.pos, &field_0x680, this);

            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                cMtx_YrotS(*calc_mtx, shape_angle.y);
                spa4.x = 0.0f;
                spa4.y = 0.0f;
                spa4.z = WREG_F(2) + 5.0f;
                MtxPosition(&spa4, &spb0);
                field_0x68c = lin_chk.GetCross() + spb0;

                cMtx_YrotS(*calc_mtx, shape_angle.y);
                spa4.x = 0.0f;
                spa4.y = 0.0f;
                spa4.z = -(WREG_F(3) + 300.0f);
                MtxPosition(&spa4, &spb0);
                current.pos = lin_chk.GetCross() + spb0;
                home.pos.x = current.pos.x;
                home.pos.z = current.pos.z;
            }

            setBck(BCK_ZH_SERCH, 2, 20.0f, 1.0f);
            mMoveMode++;
            break;

        case 1:
            if (bitSw != 0xFF && fopAcM_isSwitch(this, bitSw) && mStartFlag != 0) {
                fopAcM_OnStatus(this, 0);
                attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
                attention_info.distances[2] = 3;
                mMoveMode = 10;
            }
            break;

        case 2:
            mGateOpen();
            mSound.startCreatureSoundLevel(Z2SE_EN_ZH_MV, 0, -1);

            if (field_0x77c < 254.0f) break;

            mMoveMode = 20;
            break;

        case 10:
            if (!startDemoCheck()) break;

            field_0x794[0] = 20;
            mMoveMode++;
            // fallthrough
        case 11:
            mSound.startCreatureSoundLevel(Z2SE_EN_ZH_MV, 0, -1);

            if (field_0x794[0] != 0 || field_0x77c < 254.0f) {
                break;
            }

            dCam_getBody()->StartBlure(100, this, 0.8f, 1.4f);
            dComIfGp_getEvent().startCheckSkipEdge(this);
            mMoveMode++;
            // fallthrough
        case 12:
            if (dComIfGp_getEvent().checkSkipEdge()) {
                dCam_getBody()->ResetBlure();
                field_0x794[2] = 2;
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                dComIfGp_event_reset();
                mMoveMode = 20;
            }
            // fallthrough
        case 20:
            mSound.startCreatureSoundLevel(Z2SE_EN_ZH_MV, 0, -1);
            cMtx_YrotS(*calc_mtx, shape_angle.y);
            spa4.x = 0.0f;
            spa4.y = 0.0f;
            spa4.z = WREG_F(3) + 200.0f;
            MtxPosition(&spa4, &spb0);
            spb0 += field_0x68c;

            if (cLib_addCalcPos(&current.pos, spb0, 0.8f, WREG_F(4) + 3.0f, WREG_F(5) + 1.0f) <= WREG_F(6) + 20.0f) {
                if (mMoveMode == 20) {
                    field_0x794[2] = 2;
                    setActionMode(ACTION_EXECUTE_SEARCH_MOVE, 0);
                } else {
                    field_0x794[2] = 2;
                    camera->mCamera.Start();
                    camera->mCamera.SetTrimSize(0);
                    dComIfGp_event_reset();
                    setActionMode(ACTION_EXECUTE_SEARCH_MOVE, 0);
                }
            }
            break;
    }

    if (mMoveMode >= 11 && mMoveMode <= 12 || mMoveMode == 20) {
        mGateOpen();
    }

    if (mMoveMode == 11) {
        cMtx_YrotS(*calc_mtx, shape_angle.y);
        spa4.x = ZREG_F(0);
        spa4.y = ZREG_F(1) + 20.0f;
        spa4.z = ZREG_F(2);
        MtxPosition(&spa4, &spb0);
        spb0 += field_0x68c;
        mDemoCamCenter.set(spb0);

        cMtx_YrotS(*calc_mtx, shape_angle.y);
        spa4.x = ZREG_F(3) + -200.0f;
        spa4.y = ZREG_F(4) + -200.0f;
        spa4.z = ZREG_F(5) + 600.0f;
        MtxPosition(&spa4, &spb0);
        spb0 += field_0x68c;
        mDemoCamEye.set(spb0);

        camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, 62.0f, 0);
    }
}

void daE_ZH_c::executeOpenStart() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp28, sp34;

    field_0xb40.OnCoSPrmBit(0x40);

    switch (mMoveMode) {
        case 0:
            setBck(BCK_ZH_OPEN, 0, 3.0f, 1.0f);

            if (mS_Ball != NULL) {
                fopAcM_cancelCarryNow(mS_Ball);

                if (bitSw != 0xFF) {
                    fopAcM_onSwitch(this, bitSw);
                }
            } else {
                setActionMode(ACTION_EXECUTE_FLY_DELETE, 0);
                break;
            }
            
            field_0x794[0] = BREG_S(8) + 6 & 0xFF;
            mMoveMode = 1;
            break;

        case 1:
            if (!mpModelMorf->isStop()) {
                if (field_0x794[0] != 0) {
                    mColorSet(true, 42.5f, 8.333333f);
                    break;
                }

                mColorSet(false, 42.5f, 8.333333f);
                break;
            }

            if (mStartFlag == 0 && ((fopAcM_GetRoomNo(this) == 51 && !fopAcM_isSwitch(this, 0x5A)) || fopAcM_GetRoomNo(this) == 52 && !fopAcM_isSwitch(this, 0x5B))) {
                mMoveMode = 10;
                break;
            }

            field_0x794[0] = l_HIO.wait_time & 0xFF;
            mMoveMode = 2;
            // fallthrough
        case 2:
            if (field_0x794[0] == 0 && mColorSet(true, 10.0f, 10.0f)) {
                mStartParticleSet();
                mMoveMode = 3;
            }
            break;

        case 3:
            if (mpModelMorf->isStop()) {
                setActionMode(ACTION_EXECUTE_SEARCH_MOVE, 0);
            }
            break;

        case 10:
            if (mStartFlag == 0) {
                break;
            }

            if (!startDemoCheck()) {
                break;
            }

            if (fopAcM_GetRoomNo(this) == 51) {
                fopAcM_onSwitch(this, 0x5A);
            } else {
                fopAcM_onSwitch(this, 0x5B);
            }

            mStartFlag = 2;
            field_0x794[0] = 20;
            mMoveMode++;
            // fallthrough
        case 11:
            mDemoCamCenter.set(-60.0f, 180.0f, -3875.0f);
            mDemoCamEye.set(-250.0f, 50.0f, -3515.0f);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, 40.0f, 0);

            if (field_0x794[0] == 0) {
                mMoveMode++;
            }
            break;

        case 12:
            if (mColorSet(true, 10.0f, 10.0f)) {
                mStartParticleSet();
                mMoveMode++;
            }
            break;

        case 13:
            if ((int)mpModelMorf->getFrame() < 84) {
                break;
            }

            if ((int)mpModelMorf->getFrame() == 84) {
                dCam_getBody()->StartBlure(50, this, 0.8f, 1.2f);
            }

            mDemoCamCenter.set(-60.0f, 180.0f, -3875.0f);
            sp34.set(-280.0f, 30.0f, -3460.0f);
            cLib_addCalcPos(&mDemoCamEye, sp34, 0.7f, 30.0f, 10.0f);
            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, 40.0f, 0);

            if (!mpModelMorf->isStop()) {
                break;
            }

            #if DEBUG
            if (ZREG_S(0) == 0) {
                break;
            }
            #endif

            field_0x794[0] = 20;
            mMoveMode++;
            // fallthrough
        case 14:
            if (field_0x794[0] != 0) {
                break;
            }

            field_0x794[0] = 50;
            mMoveMode++;
            // fallthrough
        case 15:
            sp34.y = mS_Ball->current.pos.y + 600.0f;
            if (mBgc.GetGroundH() != -G_CM3D_F_INF) {
                sp34.y = mBgc.GetGroundH() + 600.0f;
            }

            cLib_addCalc2(&current.pos.y, sp34.y, 0.7f, 10.0f);
            mSound.startCreatureSoundLevel(Z2SE_EN_ZH_MV, 0, -1);

            if (field_0x794[0] != 0) {
                break;
            }

            mMoveMode++;
            // fallthrough
        case 16:
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            setActionMode(ACTION_EXECUTE_SEARCH_MOVE, 0);
            break;
    }
}

void daE_ZH_c::executeSearchMove() {
    if (mS_Ball == NULL) {
        setActionMode(ACTION_EXECUTE_FLY_DELETE, 0);
        return;
    }

    cXyz ato;

    ato.y = mS_Ball->current.pos.y + 600.0f;
    
    if (mBgc.GetGroundH() != -G_CM3D_F_INF && mBgc.GetGroundH() + 500.0f > ato.y) {
        ato.y = mBgc.GetGroundH() + 500.0f;
    }

    if (mBgc.GetRoofHeight() != G_CM3D_F_INF && ato.y > mBgc.GetRoofHeight()) {
        ato.y = mBgc.GetRoofHeight() - 150.0f;
    }

    switch (mMoveMode) {
        case 0:
            cLib_addCalc2(&current.pos.y, ato.y, 0.7f, 10.0f);

            if (XREG_S(0) != 0) {
                OS_REPORT("mBgc.GetRoofHeight() %f\n", mBgc.GetRoofHeight());
                OS_REPORT("ato.y %f\n", ato.y);
                OS_REPORT("mBgc.ChkRoofHit() %d\n", mBgc.ChkRoofHit());
            }

            if (fabsf(current.pos.y - ato.y) < 50.0f || mBgc.ChkRoofHit()) {
                if (mAnm != BCK_ZH_SERCH) {
                    setBck(BCK_ZH_SERCH, 2, 20.0f, 1.0f);
                    field_0x794[0] = 20;
                }

                mRollAngle = 0;
                field_0xb40.OffCoSPrmBit(0x40);
                field_0x774 = BREG_F(5) + -150.0f;
                mMoveMode = 1;
            }
            break;

        case 1:
            if (fabsf(current.pos.y - ato.y) > 20.0f) {
                cLib_addCalc2(&current.pos.y, ato.y, 0.7f, 10.0f);
            }
            break;
    }

    mSound.startCreatureSoundLevel(Z2SE_EN_ZH_MV, 0, -1);

    if (mMoveMode != 0 && field_0x794[0] == 0 && mSearchMove(0)) {
        setActionMode(ACTION_EXECUTE_CATCH_MOVE, 0);
    }
}

void daE_ZH_c::executeBgHitSHMove() {
    if (mS_Ball == NULL) {
        setActionMode(ACTION_EXECUTE_FLY_DELETE, 0);
        return;
    }

    dBgS_LinChk lin_chk;
    cXyz spa8, spb4, spc0;

    mBallBGCheck();

    if (NREG_S(0) != 0) {
        OS_REPORT("mRetrunStartLine.x %f\n", mRetrunStartLine.x);
        OS_REPORT("mRetrunEndLine.x %f\n", mRetrunEndLine.x);
        OS_REPORT("mRetrunStartLine.z %f\n", mRetrunStartLine.z);
        OS_REPORT("mRetrunEndLine.z %f\n", mRetrunEndLine.z);
        OS_REPORT("\n");
    }

    switch (mMoveMode) {
        case 0:
            field_0x6bc.zero();

            if (!mReturnLineMove()) break;

            field_0x6bc.set(current.pos);
            mMoveMode++;
            // fallthrough
        case 1:
            field_0x794[0] = cM_rndF(100.0f) + 100.0f;
            spa8 = field_0x6bc - current.pos;

            if (spa8.abs() < l_HIO.wandering_area) {
                field_0x7b2 = shape_angle.y + cM_rndFX(16384.0f);
            } else {
                field_0x794[0] = 20;
                field_0x7b2 = spa8.atan2sX_Z();
            }

            cMtx_YrotS(*calc_mtx, field_0x7b2);
            spb4.x = 0.0f;
            spb4.y = 0.0f;
            spb4.z = l_HIO.wandering_area - 50.0f;
            MtxPosition(&spb4, &spc0);
            spc0 += field_0x6bc;
            spb4 = spc0 - current.pos;
            field_0x7b2 = spb4.atan2sX_Z();
            mRollAngle = 0;
            mMoveMode++;
            // fallthrough
        case 2:
            if (field_0x794[0] == 0) {
                mMoveMode = 1;
            } else {
                cLib_addCalcAngleS2(&mRollAngle, 0x400, 8, 0x40);
                cLib_addCalcAngleS2(&current.angle.y, field_0x7b2, 8, mRollAngle);
                cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 8, mRollAngle);

                if (abs((s16)(shape_angle.y - field_0x7b2)) <= 0x200) {
                    cMtx_YrotS(*calc_mtx, shape_angle.y);
                    spb4.x = 0.0f;
                    spb4.y = 0.0f;
                    spb4.z = l_HIO.wandering_area - 50.0f;
                    MtxPosition(&spb4, &spc0);
                    spc0 += field_0x6bc;
                    cLib_addCalcPos(&current.pos, spc0, 0.7f, 5.0f, 1.0f);
                }
            }
            break;
    }

    if (!mBallBgLineCheck()) {
        setActionMode(ACTION_EXECUTE_SEARCH_MOVE, 0);
    }
}

void daE_ZH_c::executeCatchMove() {
    if (mS_Ball == NULL) {
        setActionMode(ACTION_EXECUTE_FLY_DELETE, 0);
        return;
    }

    cXyz sp3c, sp48;
    f32 fVar1 = 150.0f;

    sp48 = mS_Ball->current.pos - current.pos;
    sp48.y = 0.0f;
    f32 fVar2 = sp48.abs();
    Z2GetAudioMgr()->changeBgmStatus(fVar2 + 100.0f);

    switch (mMoveMode) {
        case 0:
            field_0x794[0] = (int)JREG_S(1) + 11 & 0xFF;
            field_0x794[1] = (int)JREG_S(1) + 11 & 0xFF;
            field_0x7ad = (int)JREG_S(1) + 11;
            setBck(BCK_ZH_ACT01, 0, 3.0f, 1.0f);
            mSound.startCreatureSound(Z2SE_EN_ZH_ATK_PRE, 0, -1);
            mMoveMode++;
            break;

        case 1:
            if (!mpModelMorf->isStop()) {
                if (field_0x794[0] != 0) {
                    mColorSet(false, 31.875f, 6.25f);

                    if (field_0x794[0] == 1) {
                        field_0x794[1] = field_0x7ad;
                    }
                } else if (field_0x794[1] != 0) {
                    mColorSet(true, 31.875f, 6.25f);
                    if (field_0x794[1] == 1) {
                        field_0x7ad -= 2;

                        if (field_0x7ad < 2) {
                            field_0x7ad = 2;
                        }

                        field_0x794[0] = field_0x7ad;
                    }
                }
            } else {
                field_0x764[0] = 255.0f;
                field_0x764[2] = 50.0f;
                field_0x760 = 40.0f;
                setBck(BCK_ZH_CATCH01, 0, 3.0f, 1.0f);
                mSound.startCreatureSound(Z2SE_EN_ZH_ATK, 0, -1);
                mMoveMode++;
            }
            break;

        case 2:
            gravity = -5.0f;
            if (mBgc.GetGroundH() + 100.0f > current.pos.y) {
                mMoveMode++;
            }
            break;

        case 3:
            if (mAnm == BCK_ZH_CATCH02) {
                Z2GetAudioMgr()->changeBgmStatus(99);
            }

            if (mBgc.ChkGroundHit()) {
                gravity = 0.0f;
                speed.y = 0.0f;
                dComIfGp_getVibration().StartShock(5, 31, cXyz(0.0f, 1.0f, 0.0f));
                cXyz sp54(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);

                static u16 effId[4] = {ZL2_RING_HALO02, ZL2_KAITENGIRI128, ZL2_SHOCK01, ZL2_SMOKE00_7};
                for (int i = 0; i < 3; i++) {
                    field_0xddc[i] = dComIfGp_particle_set(field_0xddc[i], effId[i], &field_0x668, NULL, NULL);
                }

                field_0xde8 = dComIfGp_particle_setPolyColor(field_0xde8, effId[3], mBgc.m_gnd, &field_0x668, &tevStr, 
                                                             &shape_angle, &sp54, 0, NULL, -1, NULL);

                if (mAnm == BCK_ZH_CATCH02) {
                    dComIfGs_onOneZoneSwitch(5, fopAcM_GetRoomNo(this));
                    mMoveMode++;
                } else {
                    field_0x794[0] = 30;
                    mMoveMode = 5;
                    setBck(BCK_ZH_CATCH02, 0, 3.0f, 1.0f);
                    mSound.startCreatureSound(Z2SE_EN_ZH_FALL, 0, -1);
                }
            }
            break;

        case 4:
            Z2GetAudioMgr()->changeBgmStatus(99);
            mS_Ball->current.pos.set(field_0x668);
            field_0x794[0] = 30;
            setActionMode(ACTION_EXECUTE_CATCH_FLY_MOVE, 0);
            break;

        case 5:
            if (field_0x794[0] == 0) {
                setBck(BCK_ZH_SERCH, 2, 10.0f, 1.0f);
                setActionMode(ACTION_EXECUTE_SEARCH_MOVE, 0);
            }
            break;
    }

    if (mMoveMode == 2 || mMoveMode == 3) {
        sp48 = eyePos;
        sp48.y = current.pos.y;
        sp3c = mS_Ball->current.pos - sp48;

        if (field_0x7af != 0) {
            fVar1 = 270.0f;
        }

        if (sp3c.abs() < fVar1) {
            if (mAnm != BCK_ZH_CATCH02 && fopAcM_checkCarryNow(mS_Ball)) {
                fopAcM_cancelCarryNow(mS_Ball);
            } else if (mAnm != BCK_ZH_CATCH02) {
                setBck(BCK_ZH_CATCH02, 0, 3.0f, 1.0f);
                mSound.startCreatureSound(Z2SE_EN_ZH_FALL, 0, -1);
                fopAcM_setCarryNow(mS_Ball, 0);
                Z2GetAudioMgr()->changeBgmStatus(99);

                if (bitSw != 0xFF && fopAcM_isSwitch(this, bitSw)) {
                    fopAcM_offSwitch(this, bitSw);
                }
            } else {
                mS_Ball->current.pos.set(field_0x668);
            }
        }
    }
}

void daE_ZH_c::executeCatchFlyMove() {
    if (mS_Ball == NULL) {
        setActionMode(ACTION_EXECUTE_FLY_DELETE, 0);
        return;
    }

    if (mS_Ball->checkRecover()) {
        dComIfGs_offOneZoneSwitch(5, fopAcM_GetRoomNo(this));
        mS_Ball->offCarryHookOK();
        field_0x78c = 2;
        setActionMode(ACTION_EXECUTE_DAMAGE, 0);
        return;
    }

    cXyz sp30, sp3c;

    if (mMoveMode != 5) {
        pmRoofPos = home.pos.y;

        if (arg0 == 0) {
            if (mBgc.GetGroundH() != -G_CM3D_F_INF) {
                pmRoofPos = mBgc.GetGroundH() + 600.0f;
            } else {
                pmRoofPos = home.pos.y + 600.0f;
            }
        }

        if (mBgc.GetRoofHeight() != G_CM3D_F_INF && pmRoofPos > mBgc.GetRoofHeight()) {
            pmRoofPos = mBgc.GetRoofHeight() - 150.0f;
        }

        f32 fVar1 = l_HIO.ascent_rate;

        if (mMoveMode > 1) {
            fVar1 = l_HIO.ascent_rate * 0.1f;
        }

        cLib_addCalc2(&current.pos.y, pmRoofPos, 0.7f, fVar1);
    }

    switch (mMoveMode) {
        case 0:
            mS_Ball->onCarryHookOK();
            field_0xa08.SetTgType(0xd84820ba);
            field_0xb40.SetTgType(0xd84820ba);

            if (field_0x794[0] != 0) break;

            gravity = 0.0f;
            speed.y = 0.0f;
            setBck(BCK_ZH_CATCHMOVE, 2, 20.0f, 1.0f);
            mMoveMode++;
            // fallthrough
        case 1:
            if (XREG_S(0) != 0) {
                OS_REPORT("mBgc.GetRoofHeight() %f\n", mBgc.GetRoofHeight());
                OS_REPORT("pmRoofPos %f\n", pmRoofPos);
                OS_REPORT("mBgc.ChkRoofHit() %d\n", mBgc.ChkRoofHit());
            }

            if (current.pos.y >= pmRoofPos - 20.0f) {
                if (arg0 != 0) {
                    mRollAngle = 0;
                    field_0x794[0] = 20;
                    mReturnLineMove();
                    mMoveMode = 10;
                } else {
                    mRollAngle = 0;
                    field_0x794[0] = 20;
                    mMoveMode++;
                }
            }
            break;

        case 2:
            if (field_0x794[0] != 0 || !mReturnLineMove()) break;

            mMoveMode++;
            // fallthrough
        case 3:
            field_0x774 = BREG_F(7);

            if (mSearchMove(1)) {
                field_0x760 = 5.0f;
                setBck(BCK_ZH_OPEN, 0, 30.0f, -1.0f);
                mMoveMode++;
            }
            break;

        case 4:
            cMtx_YrotS(*calc_mtx, current.angle.y);
            sp30.x = 0.0f;
            sp30.y = 0.0f;
            sp30.z = field_0x774;
            MtxPosition(&sp30, &sp3c);
            sp3c += home.pos;
            cLib_addCalcPos(&current.pos, sp3c, 0.8f, 10.0f, 5.0f);
            cLib_addCalcAngleS2(&shape_angle.y, home.angle.y, 4, BREG_S(5) + 0x400);
            current.angle.y = shape_angle.y;

            if (mBgc.ChkGroundHit()) {
                dComIfGp_getVibration().StartShock(5, 31, cXyz(0.0f, 1.0f, 0.0f));
                field_0x760 = 0.0f;
                current.angle.y = home.angle.y;
                current.pos.set(home.pos);
                field_0x774 = BREG_F(5) + -150.0f;
                mSound.startCreatureSound(JA_SE_OBJ_BLOCK_FALL_NORMAL, 0, -1);
                mMoveMode++;
            }
            break;

        case 5:
            if (!mColorSet(false, 5.0f, 5.0f)) break;
            
            setActionMode(ACTION_EXECUTE_WAIT, 0);
            // fallthrough
        case 12:
            fopAcM_OffStatus(this, 0);
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
            attention_info.distances[2] = 0;

            for (int i = 0; i < 3; i++) {
                field_0x764[i] = 0.0f;
            }

            dComIfGs_offOneZoneSwitch(5, fopAcM_GetRoomNo(this));

            if (bitSw == 16) {
                /* dSv_event_flag_c::F_0311 - Palace of Twilight - Palace of Twilight use 1 */
                dComIfGs_offEventBit(0x2640);
                /* dSv_event_flag_c::F_0313 - Palace of Twilight - Palace of Twilight use 3 */
                dComIfGs_offEventBit(0x2610);
                /* dSv_event_flag_c::F_0315 - Palace of Twilight - Palace of Twilight use 5 */
                dComIfGs_offEventBit(0x2604);
                /* dSv_event_flag_c::F_0317 - Palace of Twilight - Palace of Twilight use 7 */
                dComIfGs_offEventBit(0x2601);
                /* dSv_event_flag_c::F_0319 - Palace of Twilight - Palace of Twilight use 9 */
                dComIfGs_offEventBit(0x2740);
            } else {
                /* dSv_event_flag_c::F_0312 - Palace of Twilight - Palace of Twilight use 2 */
                dComIfGs_offEventBit(0x2620);
                /* dSv_event_flag_c::F_0314 - Palace of Twilight - Palace of Twilight use 4 */
                dComIfGs_offEventBit(0x2608);
                /* dSv_event_flag_c::F_0316 - Palace of Twilight - Palace of Twilight use 6 */
                dComIfGs_offEventBit(0x2602);
                /* dSv_event_flag_c::F_0318 - Palace of Twilight - Palace of Twilight use 8 */
                dComIfGs_offEventBit(0x2780);
                /* dSv_event_flag_c::F_0320 - Palace of Twilight - Palace of Twilight use 10 */
                dComIfGs_offEventBit(0x2720);
            }

            field_0xa08.SetTgType(0xd84860ba);
            field_0xb40.SetTgType(0xd84860ba);
            mS_Ball->offCarryHookOK();

            if (mMoveMode == 12) {
                fopAcM_delete(mS_Ball);
                setActionMode(ACTION_EXECUTE_DEAD_STOP, 10);
            }
            break;

        case 10:
            if (!mReturnLineMove()) break;

            mRollAngle = 0;
            mMoveMode++;
            // fallthrough
        case 11:
            field_0x774 = 0.0f;
            mSearchMove(2);
            sp3c = field_0x68c - current.pos;

            if (sp3c.abs() < 400.0f) {
                dComIfGs_onOneZoneSwitch(6, fopAcM_GetRoomNo(this));
                dComIfGs_onSaveDunSwitch(0x28);
                setActionMode(ACTION_EXECUTE_RETURN_DEMO, 0);
            }
    }

    mS_Ball->current.pos.set(field_0x668);
}

void daE_ZH_c::executeReturnDemo() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp1c, sp28;
    s16 sVar1;

    switch (mMoveMode) {
        case 0:
            sVar1 = home.angle.y + 0x8000;
            current.angle.y = sVar1;
            shape_angle.y = sVar1;
            cMtx_YrotS(*calc_mtx, shape_angle.y + 0x8000);
            sp1c.x = ZREG_F(0);
            sp1c.y = ZREG_F(1);
            sp1c.z = ZREG_F(2) + 200.0f;
            MtxPosition(&sp1c, &sp28);
            current.pos = field_0x68c + sp28;
            field_0x794[2] = 0;

            if (!startDemoCheck()) break;

            mMoveMode++;
            // fallthrough
        case 1:
            cMtx_YrotS(*calc_mtx, shape_angle.y + 0x8000);
            sp1c.x = ZREG_F(0);
            sp1c.y = ZREG_F(1) + 20.0f;
            sp1c.z = ZREG_F(2);
            MtxPosition(&sp1c, &sp28);
            sp28 += field_0x68c;
            mDemoCamCenter.set(sp28);

            cMtx_YrotS(*calc_mtx, shape_angle.y + 0x8000);
            sp1c.x = ZREG_F(3) + 300.0f;
            sp1c.y = ZREG_F(4) + 200.0f;
            sp1c.z = ZREG_F(5) + 420.0f;
            MtxPosition(&sp1c, &sp28);
            sp28 += field_0x68c;
            mDemoCamEye.set(sp28);

            camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, 62.0f, 0);

            cLib_addCalcPos(&current.pos, home.pos, 0.8f, WREG_F(4) + 3.0f, WREG_F(5) + 1.0f);
            sp28 = field_0x68c - current.pos;

            if (sp28.abs() > 5.0f || ZREG_S(0) != 0) break;

            field_0x794[0] = 70;
            mMoveMode++;
            // fallthrough
        case 2:
            cLib_addCalcPos(&current.pos, home.pos, 0.8f, WREG_F(4) + 3.0f, WREG_F(5) + 1.0f);

            if (field_0x794[0] == 0) {
                field_0x794[2] = 3;
                if (field_0x77c > 2.0f) {
                    break;
                }

                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                dComIfGp_event_reset();
                setActionMode(ACTION_EXECUTE_CATCH_FLY_MOVE, 12);
            }
            break;
    }

    mGateOpen();

    if (mS_Ball != NULL) {
        mS_Ball->current.pos.set(field_0x668);
    }
}

void daE_ZH_c::executeFlyDelete() {
    switch (mMoveMode) {
        case 0:
            fopAcM_OffStatus(this, 0);
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
            attention_info.distances[2] = 0;

            if (bitSw != 0xFF && fopAcM_isSwitch(this, bitSw)) {
                fopAcM_offSwitch(this, bitSw);
            }

            gravity = 0.0f;
            speed.y = 0.0f;

            if (mAnm != BCK_ZH_SERCH) {
                setBck(BCK_ZH_SERCH, 2, 10.0f, 1.0f);
            }

            mMoveMode = 1;
            break;

        case 1:
            if (fopAcM_rc_c::roofCheck(&current.pos)) {
                cLib_addCalc2(&current.pos.y, fopAcM_rc_c::getRoofY() + 200.0f, 0.7f, 10.0f);

                if (fabsf(current.pos.y - (fopAcM_rc_c::getRoofY() + 200.0f)) < 20.0f) {
                    fopAcM_OffStatus(this, 0);
                    attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
                    attention_info.distances[2] = 0;

                    if (arg0 == 0) {
                        setActionMode(ACTION_EXECUTE_WAIT, 0);
                    } else {
                        setActionMode(ACTION_EXECUTE_BALL_WAIT, 0);
                    }
                }
            }
            break;
    }
}

void daE_ZH_c::executeDamage() {
    cXyz sp2c, sp38;

    switch (mMoveMode) {
        case 0:
            gravity = 0.0f;
            speed.y = 0.0f;
            field_0xa08.SetTgType(0xd84860ba);
            field_0xb40.SetTgType(0xd84860ba);

            if (field_0x78c == 2) {
                setBck(BCK_ZH_CATCHDROP, 0, 3.0f, 1.0f);
                mSound.startCreatureSound(Z2SE_EN_ZH_DROP, 0, -1);

                if (current.pos.y < mBgc.GetGroundH() + 400.0f) {
                    speedF = -40.0f;
                }
            } else {
                setBck(BCK_ZH_CATCHDAMAGE, 0, 3.0f, 1.0f);
                mSound.startCreatureSound(Z2SE_EN_ZH_DMG, 0, -1);
            }

            field_0x794[0] = (int)BREG_S(8) + 8 & 0xFF;
            mMoveMode++;
            return;

        case 1:
            if (field_0x78c == 2) {
                if (mS_Ball != NULL) {
                    if ((int)mpModelMorf->getFrame() == 1 && bitSw != 0xFF && !fopAcM_isSwitch(this, bitSw)) {
                        fopAcM_cancelCarryNow(mS_Ball);
                        fopAcM_onSwitch(this, bitSw);
                    }
                }

                if ((int)mpModelMorf->getFrame() >= 1) {
                    if ((int)mpModelMorf->getFrame() <= 6 && current.pos.y < mBgc.GetGroundH() + 300.0f) {
                        cLib_addCalc2(&current.pos.y, mBgc.GetGroundH() + 300.0f, 0.7f, 30.0f);
                    }
                }

                cLib_addCalc0(&speedF, 0.8f, 4.0f);
            } else if (bitSw != 0xFF && !fopAcM_isSwitch(this, bitSw) && mS_Ball != NULL) {
                mS_Ball->current.pos.set(field_0x668);
            }

            if (!mpModelMorf->isStop()) {
                if (field_0x794[0] != 0) {
                    mColorSet(false, 31.875f, 6.25f);
                    return;
                }

                mColorSet(true, 31.875f, 6.25f);
                return;
            }

            if (mS_Ball != NULL) {
                field_0x78c++;

                if (field_0x78c < 3) {
                    if (bitSw != 0xFF && !fopAcM_isSwitch(this, bitSw)) {
                        field_0x794[0] = 0;
                        setActionMode(ACTION_EXECUTE_CATCH_FLY_MOVE, 0);
                        return;
                    }

                    gravity = 0.0f;
                    speed.y = 0.0f;
                    setActionMode(ACTION_EXECUTE_SEARCH_MOVE, 0);
                    return;
                }

                setBck(BCK_ZH_PIYO2, 2, 3.0f, 1.0f);
                field_0x794[0] = l_HIO.piyori_time;
                mMoveMode++;
            } else {
                field_0x78c = 0;
                setActionMode(ACTION_EXECUTE_FLY_DELETE, 0);
            }
            return;

        case 2:
            if (field_0x794[0] != 0) {
                if (field_0x794[0] > l_HIO.piyori_time / 3.0f) {
                    mColorSet(false, 10.0f, 10.0f);
                    return;
                }

                if (field_0x794[0] == l_HIO.piyori_time / 3.0f) {
                    mSound.startCreatureSound(Z2SE_EN_ZH_BLINK, 0, -1);
                }

                if ((field_0x794[0] & 16) == 0) {
                    mColorSet(true, 10.0f, 10.0f);
                    return;
                }

                mColorSet(false, 10.0f, 10.0f);
                return;
            }

            mColorSet(true, 10.0f, 10.0f);

            if (!mColorSet(true, 10.0f, 10.0f)) {
                return;
            }

            mMoveMode++;
            // fallthrough
        case 3:
            cLib_addCalc2(&field_0x764[0], 255.0f, 0.8f, 10.0f);
            cLib_addCalc2(&field_0x764[1], 255.0f, 0.8f, 10.0f);
            cLib_addCalc2(&field_0x764[2], 255.0f, 0.8f, 10.0f);

            if (field_0x764[1] > 254.0f) {
                return;
            }

            field_0x764[2] = 255.0f;
            field_0x764[1] = 255.0f;
            field_0x764[0] = 255.0f;
            setBck(BCK_ZH_WAKEUP, 0, 3.0f, 1.0f);
            mSound.startCreatureSound(Z2SE_EN_ZH_WAKEUP, 0, -1);
            mMoveMode++;
            // fallthrough
        case 4:
            if (!mpModelMorf->isStop()) {
                cLib_addCalc2(&field_0x764[0], 255.0f, 0.8f, 10.0f);
                cLib_addCalc0(&field_0x764[1], 0.8f, 10.0f);
                cLib_addCalc2(&field_0x764[2], 50.0f, 0.8f, 10.0f);
                return;
            }

            mMoveMode++;
            // fallthrough
        case 5:
            cLib_addCalc2(&field_0x764[0], 255.0f, 0.8f, 10.0f);
            cLib_addCalc0(&field_0x764[1], 0.8f, 10.0f);
            cLib_addCalc2(&field_0x764[2], 50.0f, 0.8f, 10.0f);

            if (field_0x764[1] > 2.0f) {
                return;
            }

            field_0x764[0] = 255.0f;
            field_0x764[1] = 0.0f;
            field_0x764[2] = 50.0f;
            field_0x78c = 0;
            setActionMode(ACTION_EXECUTE_SEARCH_MOVE, 0);
    }
}

void daE_ZH_c::executeDeadStop() {
    switch (mMoveMode) {
        case 0:
            mMoveMode++;
            break;

        case 1:
            break;

        case 10:
            break;
    }
}

void daE_ZH_c::executeEntryCheck() {
    daObjCarry_c* s_Ball = (daObjCarry_c *)fpcM_Search(s_BallSearch_Tag, this);
    if (s_Ball != NULL) {
        cXyz sp1c;

        switch (mMoveMode) {
            case 0:
                mMoveMode++;
                // fallthrough
            case 1:
                sp1c = s_Ball->current.pos - current.pos;
                sp1c.y = 0.0f;

                if (sp1c.abs() < field_0x778 && mStartFlag == 0) {
                    mStartFlag = 1;
                }
                break;
        }
    }
}

void daE_ZH_c::tagAction() {
    executeEntryCheck();
}

void daE_ZH_c::action() {
    mS_Ball = (daObjCarry_c*)fpcM_Search(s_BallSearch, this);
    damage_check();
    mBallHosei.zero();

    for (int i = 0; i < 8; i++) {
        field_0x6f8[i].zero();
    }

    if (field_0x794[2] == 2) {
        mGateOpen();
    }

    switch (mActionMode) {
        case ACTION_EXECUTE_START_DEMO:
            executeStartDemo();
            break;

        case ACTION_EXECUTE_WAIT:
            executeWait();
            break;

        case ACTION_EXECUTE_BALL_WAIT:
            executeBallWait();
            break;

        case ACTION_EXECUTE_OPEN_START:
            executeOpenStart();
            break;

        case ACTION_EXECUTE_SEARCH_MOVE:
            executeSearchMove();
            break;

        case ACTION_EXECUTE_BG_HIT_SH_MOVE:
            executeBgHitSHMove();
            break;

        case ACTION_EXECUTE_CATCH_MOVE:
            executeCatchMove();
            break;

        case ACTION_EXECUTE_CATCH_FLY_MOVE:
            executeCatchFlyMove();
            break;

        case ACTION_EXECUTE_FLY_DELETE:
            executeFlyDelete();
            break;

        case ACTION_EXECUTE_RETURN_DEMO:
            executeReturnDemo();
            break;

        case ACTION_EXECUTE_DAMAGE:
            executeDamage();
            break;

        case ACTION_EXECUTE_DEAD_STOP:
            executeDeadStop();
            break;
    }

    if (mActionMode != ACTION_EXECUTE_SEARCH_MOVE && mActionMode != ACTION_EXECUTE_CATCH_MOVE && mActionMode != ACTION_EXECUTE_CATCH_FLY_MOVE) {
        Z2GetAudioMgr()->changeBgmStatus(99);
    }
    
    if (NREG_S(0) != 0) {
        OS_REPORT("\n\n\n");
        OS_REPORT("mActionMode %d\n", mActionMode);
        OS_REPORT("mMoveMode %d\n", mMoveMode);
        OS_REPORT("\n\n\n");
    }

    if ((mAnm == BCK_ZH_SERCH && mActionMode != ACTION_EXECUTE_BALL_WAIT || mAnm == BCK_ZH_CATCHMOVE) && mActionMode != ACTION_EXECUTE_DEAD_STOP) {
        mSound.startCreatureSoundLevel(Z2SE_EN_ZH_MV, 0, -1);
    }

    if (mAnm == BCK_ZH_PIYO2) {
        mSound.startCreatureSoundLevel(Z2SE_EN_ZH_PIYO, 0, -1);
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());

    if (mActionMode != ACTION_EXECUTE_BALL_WAIT) {
        if (mActionMode != ACTION_EXECUTE_BALL_WAIT && mActionMode != ACTION_EXECUTE_RETURN_DEMO && mActionMode != ACTION_EXECUTE_FLY_DELETE) {
            mBgc.SetRoofCrrHeight(BREG_F(2) + 100.0f);
            mAcchCir.SetWall(100.0f, BREG_F(1) + 150.0f);
            mBgc.SetGroundUpY(field_0x760 + BREG_F(0));
            mBgc.CrrPos(dComIfG_Bgsp());
        }

        mpBtkAnm->play();
        mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
}

void daE_ZH_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::transM(0.0f, 0.0f, BREG_F(4) + -100.0f);
    mDoMtx_stack_c::scaleM(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
    J3DModel* model = mpModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModelMorf->modelCalc();
    mDoMtx_stack_c::copy(model->getAnmMtx(5));

    cXyz sp28;
    sp28.set(JREG_F(0) + 20.0f, JREG_F(1) + -105.0f, JREG_F(2));
    mDoMtx_stack_c::multVec(&sp28, &field_0x668);

    if (arg0 == 1) {
        mDoMtx_stack_c::transS(field_0x68c.x, field_0x68c.y, field_0x68c.z);
        mDoMtx_stack_c::scaleM(l_HIO.model_size, l_HIO.model_size, l_HIO.model_size);
        mEntranceModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

void daE_ZH_c::cc_set() {
    cXyz sp24, sp30;
    J3DModel* model = mpModelMorf->getModel();

    mDoMtx_stack_c::copy(model->getAnmMtx(0));

    if (mActionMode == ACTION_EXECUTE_WAIT) {
        sp24.set(200.0f, -100.0f, 0.0f);
    } else if (mActionMode == ACTION_EXECUTE_OPEN_START || mAnm == BCK_ZH_OPEN || mAnm == BCK_ZH_START) {
        sp24.set(250.0f, 0.0f, 0.0f);
    } else {
        sp24.set(150.0f, 50.0f, 0.0f);
    }

    mDoMtx_stack_c::multVec(&sp24, &eyePos);
    attention_info.position = eyePos;
    attention_info.position.y += 25.0f;

    if (mActionMode == ACTION_EXECUTE_CATCH_FLY_MOVE && mS_Ball != NULL) {
        cXyz sp3c, sp48;

        cMtx_YrotS(*calc_mtx, shape_angle.y);
        sp3c.x = 0.0f;
        sp3c.y = 0.0f;
        sp3c.z = ZREG_F(17) + 30.0f;
        MtxPosition(&sp3c, &sp48);
        sp48 += mS_Ball->current.pos;
        eyePos.set(sp48);
        attention_info.position = eyePos;
        attention_info.position.y += ZREG_F(18) + 100.0f;
    }

    mDoMtx_stack_c::copy(model->getAnmMtx(0));

    if (mActionMode == ACTION_EXECUTE_WAIT || mActionMode == ACTION_EXECUTE_DEAD_STOP || field_0x78c >= 3) {
        sp24.set(140.0f, -90.0f, 0.0f);
        mDoMtx_stack_c::multVec(&sp24, &sp30);
        field_0xa08.SetC(sp30);
        field_0xa08.SetR(120.0f);
        dComIfG_Ccsp()->Set(&field_0xa08);
    } else if (mActionMode == ACTION_EXECUTE_OPEN_START || mAnm == BCK_ZH_OPEN || mAnm == BCK_ZH_START) {
        mDoMtx_stack_c::multVecZero(&sp30);
        field_0xb40.SetC(sp30);
        field_0xb40.SetH(250.0f);
        field_0xb40.SetR(55.0f);
        dComIfG_Ccsp()->Set(&field_0xb40);
    } else {
        sp24.set(170.0f, -20.0f, 0.0f);
        mDoMtx_stack_c::multVec(&sp24, &sp30);
        field_0xb40.SetC(sp30);
        field_0xb40.SetH(100.0f);
        field_0xb40.SetR(170.0f);
        dComIfG_Ccsp()->Set(&field_0xb40);
    }

    if (mActionMode == ACTION_EXECUTE_WAIT || mActionMode == ACTION_EXECUTE_OPEN_START) {
        return;
    }

    sp24.set(BREG_F(10) + 170.0f, BREG_F(11) + -20.0f, BREG_F(12));
    mDoMtx_stack_c::multVec(&sp24, &sp30);
    field_0xc7c.SetC(sp30);
    field_0xc7c.SetH(BREG_F(13) + 100.0f);

    if (field_0x7af != 0) {
        field_0xc7c.SetR(BREG_F(14) + 150.0f);
    } else {
        field_0xc7c.SetR(BREG_F(14) + 130.0f);
    }

    if (gravity) {
        dComIfG_Ccsp()->Set(&field_0xc7c);
    }
}

int daE_ZH_c::execute() {
    if (field_0x7ae != 0) {
        return 1;
    }

    for (int i = 0; i < 2; i++) {
        if (field_0x794[i]) {
            field_0x794[i]--;
        }
    }

    if (field_0x7a8 != 0) {
        field_0x7a8--;
    }

    if (field_0x7a0 != 0) {
        field_0x7a0--;
    }

    if (arg0 == 2) {
        tagAction();
    } else {
        action();
        mtx_set();
        cc_set();
    }

    return 1;
}

static int daE_ZH_Execute(daE_ZH_c* i_this) {
    return i_this->execute();
}

static int daE_ZH_IsDelete(daE_ZH_c* i_this) {
    return 1;
}

int daE_ZH_c::_delete() {
    dComIfG_resDelete(&mPhase, "E_ZH");

    if (arg0 == 2) {
        return 1;
    }

    if (mActionMode == ACTION_EXECUTE_CATCH_FLY_MOVE && bitSw != 0xFF && !fopAcM_isSwitch(this, bitSw)) {
        daObjCarry_c* s_Ball = (daObjCarry_c*)fpcM_Search(s_BallSearch, this);
        if (s_Ball != NULL) {
            s_Ball->reset();
        }
    }

    if (field_0xe04 != 0) {
        l_initHIO = 0;
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daE_ZH_Delete(daE_ZH_c* i_this) {
    return i_this->_delete();
}

int daE_ZH_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_ZH", BMDV_ZH);
    JUT_ASSERT(3305, modelData != NULL);

    mpModelMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_ZH", BCK_ZH_WAIT01), 
                                       2, 1.0f, 0, -1, &mSound, 0, 0x31000284);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL) {
        return 0;
    }

    if (!mpBtkAnm->init(mpModelMorf->getModel()->getModelData(), 
                        (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("E_ZH", BTK_ZH_LOOP), 1, 2, 
                        1.0f, 0, -1)) {
        return 0;
    }

    if (arg0 == 1) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("E_ZH", BMDV_ZH_ENTRANCE);
        JUT_ASSERT(3342, modelData != NULL);

        mEntranceModel = mDoExt_J3DModel__create(modelData, 0, 0x11020203);
        if (mEntranceModel == NULL) {
            return 0;
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    daE_ZH_c* i_this = (daE_ZH_c*)a_this;
    return i_this->CreateHeap();
}

cPhs__Step daE_ZH_c::create() {
    fopAcM_ct(this, daE_ZH_c);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, "E_ZH");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_ZH PARAM %x\n", fopAcM_GetParam(this));
        arg0 = fopAcM_GetParam(this);
        bitSw = fopAcM_GetParam(this) >> 8;
        bitSwEnd = fopAcM_GetParam(this) >> 16;
        field_0x7ac = fopAcM_GetParam(this) >> 24;

        if (bitSwEnd != 0xFF && fopAcM_isSwitch(this, bitSwEnd)) {
            return cPhs_ERROR_e;
        }

        if (arg0 == 0xFF) {
            arg0 = 0;
        }

        OS_REPORT("arg0 %d\n", arg0);
        OS_REPORT("bitSw %d\n", bitSw);
        OS_REPORT("bitSwEnd %d\n", bitSwEnd);

        if (arg0 == 2) {
            fopAcM_OffStatus(this, 0);
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
            attention_info.distances[2] = 0;
            fopAcM_SetGroup(this, 0);
            fopAcM_SetMin(this, -2000.0f, -2000.0f, -2000.0f);
            fopAcM_SetMax(this, 2000.0f, 2000.0f, 2000.0f);

            if (field_0x7ac == 0xFF || field_0x7ac == 0) {
                field_0x778 = 100.0f;
            } else {
                field_0x778 = field_0x7ac * 10.0f;
            }

            mS_Ball = (daObjCarry_c*)fpcM_Search(s_BallSearch_Tag, this);
            if (mS_Ball == NULL) {
                return cPhs_ERROR_e;
            }

            setActionMode(UNK_ACTION, 0);
        } else {
            u32 i_size = 0x17C0;
            if (arg0 == 1) {
                i_size = 0x2500;
            }

            if (!fopAcM_entrySolidHeap(this, useHeapInit, i_size)) {
                return cPhs_ERROR_e;
            }

            if (l_initHIO == 0) {
                l_initHIO = 1;
                field_0xe04 = 1;
                l_HIO.field_0x4 = -1;
            }

            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
            fopAcM_SetMin(this, -20000.0f, -20000.0f, -20000.0f);
            fopAcM_SetMax(this, 20000.0f, 20000.0f, 20000.0f);
            mBgc.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
            mBgc.ClrRoofNone();
            mBgc.SetRoofCrrHeight(100.0f);
            mAcchCir.SetWall(100.0f, 150.0f);
            mStts.Init(0xFF, 0, this);
            health = 1000;
            field_0x560 = 1000;
            field_0x774 = -150.0f;

            field_0xa08.Set(cc_zhSph_src);
            field_0xa08.SetStts(&mStts);
            field_0xb40.Set(cc_zhCyl_src);
            field_0xb40.SetStts(&mStts);
            field_0xc7c.Set(cc_zhCyl_at_src);
            field_0xc7c.SetStts(&mStts);

            mSound.init(&current.pos, &eyePos, 3, 1);
            mAtInfo.mpSound = &mSound;
            mAtInfo.mPowerType = 1;
            gravity = 0.0f;

            for (int i = 0; i < 3; i++) {
                field_0x764[i] = 0.0f;
            }

            shape_angle.z = 0;
            shape_angle.x = 0;
            attention_info.distances[2] = 3;
            fopAcM_OffStatus(this, 0);
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

            for (int i = 0; i < 8; i++) {
                field_0x758[i] = 0xFF;
            }

            for (int i = 0; i < 3; i++) {
                field_0x6c8[i].set(current.pos);
            }

            field_0xa08.OnTgNoHitMark();
            field_0xb40.OnTgNoHitMark();
            onWolfNoLock();

            if (arg0 == 0) {
                mS_Ball = (daObjCarry_c*)fpcM_Search(s_BallSearch, this);
                if (mS_Ball != NULL) {
                    if (!fopAcM_checkCarryNow(mS_Ball)) {
                        mS_Ball->current.pos.set(current.pos);
                        mS_Ball->current.pos.y += 100.0f;
                        mS_Ball->current.pos.z += 90.0f;
                        fopAcM_setCarryNow(mS_Ball, 0);
                        mStartFlag = 0;

                        setBck(BCK_ZH_WAIT01, 0, 3.0f, 1.0f);
                        field_0x78c = 0;
                        attention_info.distances[2] = 3;
                        fopAcM_OffStatus(this, 0);
                        attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;

                        if (fopAcM_GetRoomNo(this) == 0x33 && !fopAcM_isSwitch(this, 18)) {
                            fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x4000);
                            setActionMode(ACTION_EXECUTE_START_DEMO, 0);
                        } else {
                            setActionMode(ACTION_EXECUTE_WAIT, 1);
                        }
                    } else {
                        setActionMode(ACTION_EXECUTE_SEARCH_MOVE, 0);
                    }
                } else {
                    setActionMode(ACTION_EXECUTE_DEAD_STOP, 0);
                }
            } else {
                attention_info.distances[2] = 0;
                field_0x764[0] = 255.0f;
                field_0x764[2] = 50.0f;
                mStartFlag = 0;
                setActionMode(ACTION_EXECUTE_BALL_WAIT, 0);
            }
        }

        daE_ZH_Execute(this);

        if (arg0 == 0 && mS_Ball != NULL) {
            mS_Ball->current.pos.set(field_0x668);
        }
    }

    return phase;
}

daE_ZH_c::daE_ZH_c() {}

static int daE_ZH_Create(daE_ZH_c* i_this) {
    return i_this->create();
}

AUDIO_INSTANCES;

static actor_method_class l_daE_ZH_Method = {
    (process_method_func)daE_ZH_Create,
    (process_method_func)daE_ZH_Delete,
    (process_method_func)daE_ZH_Execute,
    (process_method_func)daE_ZH_IsDelete,
    (process_method_func)daE_ZH_Draw,
};

extern actor_process_profile_definition g_profile_E_ZH = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_ZH,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_ZH_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  173,                    // mPriority
  &l_daE_ZH_Method,       // sub_method
  0x00040100,             // mStatus
  fopAc_ENV_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
