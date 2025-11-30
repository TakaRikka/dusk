#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_camera.h"
#include "SSystem/SComponent/c_counter.h"
#include "SSystem/SComponent/c_math.h"
#include "cmath.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_boomerang.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_midna.h"
#include "d/actor/d_a_tag_mhint.h"
#include "d/actor/d_a_tag_mstop.h"
#include "d/actor/d_a_tag_mwait.h"
#include "d/d_bg_s_sph_chk.h"
#include "d/d_com_inf_actor.h"
#include "d/d_com_inf_game.h"
#include "d/d_debug_viewer.h"
#include "d/d_demo.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_lib.h"
#include "math.h"

namespace {

static f32 limitf(f32 value, f32 min, f32 max) {
    if (value > max) {
        return max;
    } else if (value < min) {
        return min;
    }

    return value;
}

static inline f32 rangef(f32 value1, f32 value2, f32 ratio) {
    return value1 + (value2 - value1) * ratio;
}

inline static bool is_player(fopAc_ac_c* actor) {
    return fopAcM_GetName(actor) == PROC_ALINK || fopAcM_GetName(actor) == PROC_ALINK;
}

static void hideActor(fopAc_ac_c* actor) {
    if (is_player(actor)) {
        dComIfGp_onCameraAttentionStatus(0, 2);
        if (((daPy_py_c*)actor)->checkHorseRide()) {
            fopAcM_OnStatus(dComIfGp_getHorseActor(), 0x1000000);
        }
    } else {
        fopAcM_OnStatus(actor, 0x1000000);
    }
}

static bool defaultRadius(f32 param_0, f32 param_1, f32* param_2) {
    f32 var_f30;
    f32 var_f31;

    if (param_0 < param_1) {
        var_f31 = param_0;
        var_f30 = param_1;
    } else {
        var_f30 = param_0;
        var_f31 = param_1;
    }

    if (*param_2 > var_f30) {
        *param_2 = var_f30;
        return false;
    }

    if (*param_2 < var_f31) {
        *param_2 = var_f31;
        return false;
    }

    return true;
}

static bool posInLine2D(cXyz* param_0, cXyz* param_1, cXyz* param_2) {
    if (param_0->x <= param_1->x) {
        if (param_2->x < param_0->x)
            return false;
        if (param_2->x > param_1->x)
            return false;
    } else {
        if (param_2->x <= param_1->x)
            return false;
        if (param_2->x >= param_0->x)
            return false;
    }

    if (param_0->z <= param_1->z) {
        if (param_2->z < param_0->z)
            return false;
        if (param_2->z > param_1->z)
            return false;
    } else {
        if (param_2->z <= param_1->z)
            return false;
        if (param_2->z >= param_0->z)
            return false;
    }

    return true;
}

static cXyz relationalPos2(fopAc_ac_c* i_actor1, fopAc_ac_c* i_actor2, cXyz* param_2, f32 param_3,
                           cSAngle param_4) {
    if (i_actor1 == NULL || i_actor2 == NULL) {
        return cXyz::Zero;
    }

    cXyz pos1 = i_actor1->attention_info.position;
    cXyz pos2 = i_actor2->attention_info.position;
    cXyz pos_diff = pos2 - pos1;
    cSGlobe diff_globe(pos_diff);
    pos_diff.normalize();
    pos1 -= pos_diff * i_actor1->attention_info.field_0xa;
    pos2 += pos_diff * i_actor2->attention_info.field_0xa;
    cXyz mid = pos1 + (pos2 - pos1) * 0.5f;
    cXyz vec = *param_2;
    cSAngle angle = param_4 - diff_globe.U();
    if (angle < cSAngle::_0) {
        vec.x = -vec.x;
    }
    cSGlobe globe2(vec);
    globe2.U(diff_globe.U() + globe2.U());
    diff_globe.R(0.5f * diff_globe.R() * angle.Cos() * param_3);
    cXyz ret = mid + diff_globe.Xyz() + globe2.Xyz();
    return ret;
}

inline static cSAngle sAngleY(cXyz& i_vec) {
    return cM_atan2s(i_vec.x, i_vec.z);
}

}  // namespace

int dCamMapToolData::Set(s32 param_0, s32 roomNo, fopAc_ac_c* param_2, u16 param_3, u8 param_4) {
    if ((param_3 & 0xFF) > mPriority) {
        return 0;
    }

    stage_camera_class* room_cam = dComIfGp_getRoomCamera(roomNo);
    stage_arrow_class* room_arrow = dComIfGp_getRoomArrow(roomNo);

    if (room_cam != NULL && param_0 >= 0 && param_0 < room_cam->num) {
        Clr();
        mCameraIndex = param_0;
        mCamData = room_cam->m_entries[mCameraIndex];
        mFlags = (mCamData.field_0x14 >> 0xE) & 3;

        if (param_3 & 0x8000) {
            mFlags |= 0x10;
        }

        if (mCamData.field_0x14 & 0x2000) {
            mCamData.field_0x14 |= 0xC000;
        } else {
            mCamData.field_0x14 &= ~0xC000;
        }

        mArrowIndex = mCamData.m_arrow_idx;

        if (room_arrow != NULL && mArrowIndex >= 0 && mArrowIndex < room_arrow->num) {
            mArrowData = room_arrow->m_entries[mArrowIndex];
        }

        mpActor = param_2;
        mPathId = param_4;
        mPriority = param_3 & 0xFF;
        return 1;
    }

    return 0;
}

engine_fn dCamera_c::engine_tbl[] = {
    &dCamera_c::letCamera,        &dCamera_c::chaseCamera,    &dCamera_c::lockonCamera,
    &dCamera_c::talktoCamera,     &dCamera_c::subjectCamera,  &dCamera_c::fixedPositionCamera,
    &dCamera_c::fixedFrameCamera, &dCamera_c::towerCamera,    &dCamera_c::rideCamera,
    &dCamera_c::manualCamera,     &dCamera_c::eventCamera,    &dCamera_c::hookshotCamera,
    &dCamera_c::colosseumCamera,  &dCamera_c::observeCamera,  &dCamera_c::magneCamera,
    &dCamera_c::railCamera,       &dCamera_c::paraRailCamera, &dCamera_c::oneSideCamera,
    &dCamera_c::test1Camera,      &dCamera_c::test2Camera,
};

namespace {
inline static int get_camera_id(camera_class* i_camera) {
    return fopCamM_GetParam(i_camera);
}

inline static int get_controller_id(camera_class* i_camera) {
    return dComIfGp_getCameraPlayer1ID(get_camera_id(i_camera));
}

inline static fopAc_ac_c* get_player_actor(camera_class* i_camera) {
    return dComIfGp_getPlayer(dComIfGp_getCameraPlayer1ID(get_camera_id(i_camera)));
}

inline static dDlst_window_c* get_window(int param_0) {
    return dComIfGp_getWindow(dComIfGp_getCameraWinID(param_0));
}

inline static dDlst_window_c* get_window(camera_class* i_camera) {
    return dComIfGp_getWindow(dComIfGp_getCameraWinID(get_camera_id(i_camera)));
}

inline static fopAc_ac_c* get_boomerang_actor(fopAc_ac_c* i_actor) {
    if (is_player(i_actor)) {
        return static_cast<daAlink_c*>(i_actor)->getThrowBoomerangActor();
    } else {
        return NULL;
    }
}

inline static cSAngle sAngleX(cXyz& i_vec) {
    return cSAngle(cM_atan2s(i_vec.y, i_vec.z));
}

inline static bool lineCollisionCheck(cXyz param_0, cXyz param_1, fopAc_ac_c* param_2,
                                      fopAc_ac_c* param_3, fopAc_ac_c* param_4) {
    return dComIfG_Ccsp()->ChkCamera(param_0, param_1, 15.0f, param_2, param_3, param_4);
}

inline static f32 playerMaxSpeed() {
    return 40.0f;
}

}  // namespace

dCamera_c::dCamera_c(camera_class* i_camera) : mCamParam(0) {
    initialize(i_camera, get_player_actor(i_camera), get_camera_id(i_camera),
               get_controller_id(i_camera));
}

dCamera_c::~dCamera_c() {
    if (!daPy_py_c::checkPeepEndSceneChange()) {
        dComIfGs_getTurnRestart().setCameraCtr(mCenter);
        dComIfGs_getTurnRestart().setCameraEye(mEye);
        dComIfGs_getTurnRestart().setCameraUp(mUp);
        dComIfGs_getTurnRestart().setCameraFvy(mFovy);
        fopAc_ac_c::setStopStatus(0);
    }
}

enum SpecialType {
    /* 0x00 */ CAM_TYPE_FIELD_S,
    /* 0x01 */ CAM_TYPE_EVENT,
    /* 0x02 */ CAM_TYPE_WATER,
    /* 0x03 */ CAM_TYPE_SCOPE,
    /* 0x04 */ CAM_TYPE_CANOE,
    /* 0x05 */ CAM_TYPE_HORSE_T,
    /* 0x06 */ CAM_TYPE_BOARD,
    /* 0x07 */ CAM_TYPE_07,
    /* 0x08 */ CAM_TYPE_KEEP,
    /* 0x09 */ CAM_TYPE_RODEO,
    /* 0x0A */ CAM_TYPE_MAGNE_BOOTS,
    /* 0x0B */ CAM_TYPE_MAGNE_WALL,
    /* 0x0C */ CAM_TYPE_MAGNE_ROOF,
    /* 0x0D */ CAM_TYPE_WATER_SURF,
    /* 0x0E */ CAM_TYPE_ROTARY,
    /* 0x0F */ CAM_TYPE_STREET,
    /* 0x10 */ CAM_TYPE_STREET_N,
    /* 0x11 */ CAM_TYPE_STREET_P,
    /* 0x12 */ CAM_TYPE_RAMPART_2,
    /* 0x13 */ CAM_TYPE_ALLAY,
    /* 0x14 */ CAM_TYPE_ALLAY_R,
    /* 0x15 */ CAM_TYPE_ALLAY_R2,
    /* 0x16 */ CAM_TYPE_ALLAY_S,
    /* 0x17 */ CAM_TYPE_ALLAY_N,
    /* 0x18 */ CAM_TYPE_ALLAY_R3,
    /* 0x19 */ CAM_TYPE_HOOK_WALL,
    /* 0x1A */ CAM_TYPE_HOOK_ROOF,
    /* 0x1B */ CAM_TYPE_HOOK_ACTOR,
    /* 0x1C */ CAM_TYPE_SPINNER,
    /* 0x1D */ CAM_TYPE_GORON_JUMP,
    /* 0x1E */ CAM_TYPE_GOAT_BATTLE,
    /* 0x1F */ CAM_TYPE_BOAR,
    /* 0x20 */ CAM_TYPE_COCCO_JUMP,
    /* 0x21 */ CAM_TYPE_ROOF_HUNG,
    /* 0x22 */ CAM_TYPE_COPY_ROD_HALL,
    /* 0x23 */ CAM_TYPE_MIDNA_TAG,
    /* 0x24 */ CAM_TYPE_WARP_OBJ,
    /* 0x25 */ CAM_TYPE_LV9_GZELDA_TRI,
    /* 0x26 */ CAM_TYPE_TEPPEI_HOOK,
    /* 0x27 */ CAM_TYPE_LV4_BOSS_SPJP,
    /* 0x28 */ CAM_TYPE_LV7_BOSS,
    /* 0x29 */ CAM_TYPE_PEEP,
};

namespace {

static int specialType[42];

static int Stage;

static f32 WideTurnSaving = 0.86f + OREG_F(1);

inline static u32 check_owner_action(u32 param_0, u32 param_1) {
    return dComIfGp_checkPlayerStatus0(param_0, param_1);
}

inline static u32 check_owner_action1(u32 param_0, u32 param_1) {
    return dComIfGp_checkPlayerStatus1(param_0, param_1);
}

inline static bool isPlayerCharging(u32 param_0) {
    return check_owner_action(param_0, 0x40000000);
}

inline static void setComStat(u32 param_0) {
    dComIfGp_onCameraAttentionStatus(0, param_0);
}

inline static bool getComStat(u32 param_0) {
    return dComIfGp_getCameraAttentionStatus(0) & param_0;
}

inline static void clrComStat(u32 param_0) {
    dComIfGp_offCameraAttentionStatus(0, param_0);
}

inline static void setComZoomScale(f32 param_0) {
    dComIfGp_setCameraZoomScale(0, param_0);
}

inline static void setComZoomForcus(f32 param_0) {
    dComIfGp_setCameraZoomForcus(0, param_0);
}

}  // namespace

void dCamera_c::initialize(camera_class* i_camera, fopAc_ac_c* i_player, u32 i_cameraID,
                           u32 i_padID) {
    char* type_data =
        (char*)dComIfG_getObjectRes(dComIfGp_getCameraParamFileName(0), "camtype.dat");
    mCamTypeData = (dCamera_type_data*)(type_data + 8);
    mCamTypeNum = *(int*)(type_data + 4);

    field_0x0 = i_camera;
    field_0x20 = 1;
    field_0x21 = 0;
    mCurState = 0;
    mpPlayerActor = i_player;
    mCameraID = i_cameraID;
    mPadID = i_padID;

    initMonitor();
    initPad();
    mFocusLine.Init();

    mRoomCtx.mRoomNo = dComIfGp_roomControl_getStayNo();
    const char* stage_name = dComIfGp_getStartStageName();
    if (strcmp(stage_name, "D_MN01A") == 0) {
        Stage = 0x68;
    } else if (strcmp(stage_name, "D_MN10A") == 0) {
        Stage = 0x69;
    } else if (strcmp(stage_name, "D_MN11A") == 0) {
        Stage = 0x6A;
    } else if (strcmp(stage_name, "D_MN04A") == 0) {
        Stage = 0x66;
    } else if (strcmp(stage_name, "D_MN05A") == 0) {
        Stage = 0x65;
    } else if (strcmp(stage_name, "D_MN07A") == 0) {
        Stage = 0x6B;
    }

    specialType[CAM_TYPE_FIELD_S] = GetCameraTypeFromCameraName("FieldS");
    specialType[CAM_TYPE_EVENT] = GetCameraTypeFromCameraName("Event");
    specialType[CAM_TYPE_WATER] = GetCameraTypeFromCameraName("Water");
    specialType[CAM_TYPE_SCOPE] = GetCameraTypeFromCameraName("Scope");
    specialType[CAM_TYPE_WATER_SURF] = GetCameraTypeFromCameraName("WaterSurf");
    specialType[CAM_TYPE_HORSE_T] = GetCameraTypeFromCameraName("HorseT");
    specialType[CAM_TYPE_BOARD] = GetCameraTypeFromCameraName("Board");
    specialType[CAM_TYPE_CANOE] = GetCameraTypeFromCameraName("Canoe");
    specialType[CAM_TYPE_KEEP] = GetCameraTypeFromCameraName("Keep");
    specialType[CAM_TYPE_RODEO] = GetCameraTypeFromCameraName("Rodeo");
    specialType[CAM_TYPE_MAGNE_BOOTS] = GetCameraTypeFromCameraName("MagneBoots");
    specialType[CAM_TYPE_MAGNE_ROOF] = GetCameraTypeFromCameraName("MagneRoof");
    specialType[CAM_TYPE_MAGNE_WALL] = GetCameraTypeFromCameraName("MagneWall");
    specialType[CAM_TYPE_COCCO_JUMP] = GetCameraTypeFromCameraName("CoccoJump");
    specialType[CAM_TYPE_BOAR] = GetCameraTypeFromCameraName("Boar");
    specialType[CAM_TYPE_GOAT_BATTLE] = GetCameraTypeFromCameraName("GoatBattle");
    specialType[CAM_TYPE_GORON_JUMP] = GetCameraTypeFromCameraName("GoronJump");
    specialType[CAM_TYPE_SPINNER] = GetCameraTypeFromCameraName("Spinner");
    specialType[CAM_TYPE_HOOK_WALL] = GetCameraTypeFromCameraName("HookWall");
    specialType[CAM_TYPE_HOOK_ROOF] = GetCameraTypeFromCameraName("HookRoof");
    specialType[CAM_TYPE_HOOK_ACTOR] = GetCameraTypeFromCameraName("HookActor");
    specialType[CAM_TYPE_ROOF_HUNG] = GetCameraTypeFromCameraName("RoofHung");
    specialType[CAM_TYPE_ROTARY] = GetCameraTypeFromCameraName("Rotary");
    specialType[CAM_TYPE_STREET] = GetCameraTypeFromCameraName("Street");
    specialType[CAM_TYPE_STREET_N] = GetCameraTypeFromCameraName("StreetN");
    specialType[CAM_TYPE_STREET_P] = GetCameraTypeFromCameraName("StreetP");
    specialType[CAM_TYPE_RAMPART_2] = GetCameraTypeFromCameraName("Rampart2");
    specialType[CAM_TYPE_ALLAY] = GetCameraTypeFromCameraName("Allay");
    specialType[CAM_TYPE_ALLAY_R] = GetCameraTypeFromCameraName("AllayR");
    specialType[CAM_TYPE_ALLAY_R2] = GetCameraTypeFromCameraName("AllayR2");
    specialType[CAM_TYPE_ALLAY_S] = GetCameraTypeFromCameraName("AllayS");
    specialType[CAM_TYPE_ALLAY_N] = GetCameraTypeFromCameraName("AllayN");
    specialType[CAM_TYPE_ALLAY_R3] = GetCameraTypeFromCameraName("AllayR3");
    specialType[CAM_TYPE_COPY_ROD_HALL] = GetCameraTypeFromCameraName("CopyRodHall");
    specialType[CAM_TYPE_MIDNA_TAG] = GetCameraTypeFromCameraName("MidnaTag");
    specialType[CAM_TYPE_WARP_OBJ] = GetCameraTypeFromCameraName("WarpObj");
    specialType[CAM_TYPE_LV9_GZELDA_TRI] = GetCameraTypeFromCameraName("LV9GZeldaTri");
    specialType[CAM_TYPE_LV4_BOSS_SPJP] = GetCameraTypeFromCameraName("LV4BOSSSPJP");
    specialType[CAM_TYPE_LV7_BOSS] = GetCameraTypeFromCameraName("LV7BOSS");
    specialType[CAM_TYPE_TEPPEI_HOOK] = GetCameraTypeFromCameraName("TeppeiHook");

    mCurType = mMapToolType = specialType[CAM_TYPE_FIELD_S];

    specialType[CAM_TYPE_PEEP] = GetCameraTypeFromCameraName("Peep");
    field_0x698 = 0xFF;
    field_0x69c = 0;
    mIsWolf = daPy_py_c::checkNowWolf() != 0;
    mCurMode = 0;
    mEngineHoldState = 0;
    mForcedMode = 11;
    mSightFitRadius = 0.0f;
    mEventFlags = 0;
    mForwardTiltOffset = cSAngle::_0;
    mFrameCounter = 0;
    mTicks = cM_rndFX(0x7FFF);
    field_0x84 = 1.0f;
    field_0x91c = 0.0f;

    mTrimHeight = 0.0f;
    mTrimSize = 0;
    mTrimTypeForce = -1;
    mGear = 0;
    field_0x944 = 0;
    field_0x950 = mGear;

    mBG.field_0x5c.field_0x0 = 0;
    mBG.field_0x0.field_0x0 = 0;
    mBG.field_0xc0.field_0x3c = 0xFF;
    mBG.field_0x5c.field_0x58 = -G_CM3D_F_INF;
    mBG.field_0x0.field_0x58 = -G_CM3D_F_INF;
    mBG.field_0x0.field_0x4.OffNormalGrp();
    mBG.field_0x0.field_0x4.OnWaterGrp();
    mBG.field_0xc0.field_0x1 = 0;
    mBG.field_0xc0.field_0x0 = 0;
    mBG.field_0xc0.field_0x10 = cXyz::Zero;
    mBG.field_0xc0.field_0x4 = mBG.field_0xc0.field_0x10;
    mBG.field_0xc0.field_0x20 = 0;
    mBG.field_0xc0.field_0x1e = cSAngle::_0;
    mBG.field_0xc0.field_0x1c = mBG.field_0xc0.field_0x1e;
    mBG.field_0xc0.field_0x34 = 0;
    mBG.field_0x108.field_0x0 = 0;
    mBG.field_0x108.field_0x4 = 0.0f;
    mBG.field_0xc0.field_0x38 = -G_CM3D_F_INF;
    mBG.field_0xc0.field_0x40 = 0xFF;

    mWallUpDist = mCamSetup.mBGChk.WallUpDistance();

    mMapToolType = 0xFF;
    mRoomMapTool.Clr();
    mStageCamTool.Clr();
    mDefRoomCamTool.Clr();
    mTagCamTool.Clr();
    field_0x89c.Clr();
    field_0x8d8.Clr();
    setStageMapToolData();

    if (mStageCamTool.mCameraIndex != 0xFF) {
        mMapToolType = GetCameraTypeFromToolData(&mStageCamTool.mCamData);
    }

    dStage_dt_c* stage_dt = dComIfGp_getStage();
    stage_stag_info_class* stag_info;
    if (stage_dt != NULL) {
        stag_info = stage_dt->getStagInfo();
    } else {
        stag_info = NULL;
    }

    if (stag_info != NULL && mMapToolType == 0xFF) {
        switch (dStage_stagInfo_GetSTType(stag_info)) {
        case ST_DUNGEON:
            mMapToolType = GetCameraTypeFromCameraName("DungeonS");
            break;
        case ST_ROOM:
            mMapToolType = GetCameraTypeFromCameraName("Room");
            break;
        default:
            mMapToolType = GetCameraTypeFromCameraName("FieldS");
            break;
        }
    }

    int sp108 = 0xFF;
    s32 sp10C = dComIfGp_roomControl_getStayNo();

    dStage_roomDt_c* room_dt = dComIfGp_roomControl_getStatusRoomDt(sp10C);
    if (room_dt != NULL) {
        dStage_FileList_dt_c* filelist = room_dt->getFileListInfo();
        if (filelist != NULL) {
            sp108 = dStage_FileList_dt_GetDefaultCamera(filelist);
        }
    }

    setRoomMapToolData(&mDefRoomCamTool, sp108, sp10C);
    checkGroundInfo();
    setMapToolData();

    if (mRoomMapTool.mCameraIndex != 0xFF) {
        mCurType = GetCameraTypeFromToolData(&mRoomMapTool.mCamData);
    } else {
        if (check_owner_action(mPadID, 0x8000000)) {
            mCurType = GetCameraTypeFromCameraName("Cave");
        } else {
            mCurType = mMapToolType;
        }
    }

    mCamStyle = mCamTypeData[mCurType].field_0x18[mIsWolf][mCurMode];
    mLockOnActorID = 0xFFFFFFFF;
    mEventData.field_0x14 = -1;
    mEventData.field_0x18 = -1;
    mEventData.mStaffIdx = -1;
    mEventData.field_0xc = -1;
    mEventData.field_0x0 = 1;

    OS_REPORT("camera: init: type %d mode %d style %d\n", mCurType, mCurMode, mCamStyle);

    mCamParam.Change(mCamStyle);
    if (daPy_py_c::checkPeepEndSceneChange() || dComIfGp_getStartStagePoint() == -2 ||
        dComIfGp_getStartStagePoint() == -3)
    {
        mCenter = mViewCache.mCenter = dComIfGs_getTurnRestart().getCameraCtr();
        mEye = mViewCache.mEye = dComIfGs_getTurnRestart().getCameraEye();

        mViewCache.mDirection.Val(mEye - mCenter);
        mDirection = mViewCache.mDirection;
        mControlledYaw = mViewCache.mDirection.U().Inv();

        mUp = dComIfGs_getTurnRestart().getCameraUp();
        mBank = cSAngle::_0;
        mViewCache.mBank = mBank;

        mViewCache.mFovy = mFovy = dComIfGs_getTurnRestart().getCameraFvy();
    } else if (daAlink_getAlinkActorClass()->checkStartFall()) {
        cXyz attn_pos = attentionPos(mpPlayerActor);
        if (mCamParam.Algorythmn() == 1) {
            attn_pos.y += -20.0f;
        }

        cSGlobe spE8(0.0f, cSAngle((s16)0), directionOf(mpPlayerActor));
        mCenter = mViewCache.mCenter = attn_pos + spE8.Xyz();

        cXyz center = dComIfGs_getTurnRestart().getCameraCtr();
        cXyz eye = dComIfGs_getTurnRestart().getCameraEye();
        mViewCache.mDirection.Val(eye - center);
        mControlledYaw = mViewCache.mDirection.U().Inv();

        mEye = mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();

        mUp = dComIfGs_getTurnRestart().getCameraUp();
        mBank = cSAngle::_0;
        mViewCache.mBank = mBank;
        mViewCache.mFovy = mFovy = dComIfGs_getTurnRestart().getCameraFvy();
    } else {
        cXyz attn_pos = attentionPos(mpPlayerActor);
        if (mCamParam.Algorythmn() == 1) {
            attn_pos.y += -20.0f;
        }

        cSGlobe spE8(0.0f, cSAngle((s16)0), directionOf(mpPlayerActor));
        mCenter = mViewCache.mCenter = attn_pos + spE8.Xyz();

        mViewCache.mDirection.Val(300.0f, 0, directionOf(mpPlayerActor).Inv());

        mEye = mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mDirection = mViewCache.mDirection;
        mControlledYaw = mViewCache.mDirection.U().Inv();

        mFovy = 60.0f;
        mViewCache.mFovy = 60.0f;
        mUp.set(0.0f, 1.0f, 0.0f);
        mBank = cSAngle::_0;
        mViewCache.mBank = mBank;
    }

    mFakeAngleSys.field_0x0 = 0;
    field_0x674 = 0xFF;
    field_0x670 = 0xFF;
    field_0x668 = 0;
    field_0x66c = 0;
    field_0x678 = 1;
    mZoomRatio = 0.0f;
    field_0x738 = 85.0f;

    mFastShotState = 0;
    field_0x611 = 0;

    clearInfo(&mSavedView, 0);
    clearInfo(&mSavedViewStack[0], 0);
    clearInfo(&mSavedViewStack[1], 0);

    clearInfo(&mRecovery.field_0x8, 0);
    mRecovery.field_0x4 = 0;
    mRecovery.field_0x28 = cXyz::Zero;
    mRecovery.field_0x0 = -1;

    field_0x93c = 0;
    mThrowTimer = 0;
    mBankOverride = cSAngle::_0;
    mUpOverride.field_0x18 = mUp;
    mUpOverride.field_0xc = mEye;
    mUpOverride.field_0x0 = mCenter;

    setFlag(0x1000);

    daAlink_c* player = daAlink_getAlinkActorClass();
    daMidna_c* midna = daPy_py_c::getMidnaActor();

    mMidnaRidingAndVisible = player->checkMidnaRide() && !midna->checkNoDraw();
    mLastBumpCase = 0;
    field_0x95c = cXyz::Zero;
}

void dCamera_c::Start() {
    if (mCamSetup.CheckFlag(0x8000) && mCurState != 0 && mCurState != 2) {
        OS_REPORT("camera: start\n");
    }

    if (mCurState != 0) {
        mCurState = 2;
    }
}

void dCamera_c::QuickStart() {
    if (mCamSetup.CheckFlag(0x8000) && mCurState != 0) {
        OS_REPORT("camera: quick start\n");
    }

    mCurState = 0;
}

void dCamera_c::Stop() {
    clrFlag(0x200000);

    if (mCamSetup.CheckFlag(0x8000) && mCurState != 3) {
        OS_REPORT("%06d: camera: stop   \n", mFrameCounter);
    }

    mCurState = 3;
}

void dCamera_c::Stay() {
    mCurState = 1;
}

bool dCamera_c::ChangeModeOK(s32 param_0) {
    if (dComIfGp_evmng_cameraPlay() || chkFlag(0x20000000)) {
        return 0;
    }

    return !(mCamTypeData[mCurType].field_0x18[mIsWolf][param_0] < 0);
}

void dCamera_c::initPad() {
    if (chkFlag(0x1000000)) {
        mPadInfo.mMainStick.mLastPosX = 0.0f;
        mPadInfo.mMainStick.mLastPosY = 0.0f;
        mPadInfo.mMainStick.mLastValue = 0.0f;
    } else {
        mPadInfo.mMainStick.mLastPosX = mDoCPd_c::getStickX3D(mPadID);
        mPadInfo.mMainStick.mLastPosY = mDoCPd_c::getStickY(mPadID);
        mPadInfo.mMainStick.mLastValue = mDoCPd_c::getStickValue(mPadID);
    }

    mPadInfo.mMainStick.mPosXDelta = 0.0f;
    mPadInfo.mMainStick.mPosYDelta = 0.0f;
    mPadInfo.mMainStick.mValueDelta = 0.0f;
    mPadInfo.mMainStick.mAngle = cSAngle::_0;

    if (chkFlag(0x800000)) {
        mPadInfo.mMainStick.mLastPosX = 0.0f;
        mPadInfo.mMainStick.mLastPosY = 0.0f;
        mPadInfo.mMainStick.mLastValue = 0.0f;
    } else {
        mPadInfo.mCStick.mLastPosX = mDoCPd_c::getSubStickX(mPadID);
        mPadInfo.mCStick.mLastPosY = mDoCPd_c::getSubStickY(mPadID);
        mPadInfo.mCStick.mLastValue = mDoCPd_c::getSubStickValue(mPadID);
    }

    mPadInfo.mCStick.mPosXDelta = 0.0f;
    mPadInfo.mCStick.mPosYDelta = 0.0f;
    mPadInfo.mCStick.mValueDelta = 0.0f;
    mPadInfo.mCStick.mAngle = cSAngle::_0;

    mCStickYState = mCStickYHoldCount = mCStickUpLatch = 0;

    mTriggerLeftLast = mDoCPd_c::getAnalogL(mPadID);
    mTriggerLeftDelta = 0.0f;
    mHoldLockL = 0;
    mTrigLockL = 0;
    mLockLActive = 0;
    mLockLJustActivated = 0;

    mTriggerRightLast = mDoCPd_c::getAnalogR(mPadID);
    mTriggerRightDelta = 0.0f;
    mHoldLockR = 0;
    mTrigLockR = 0;
    mLockRActive = 0;
    mLockRJustActivated = 0;

    mHoldX = mDoCPd_c::getHoldX(mPadID) ? true : false;
    mTrigX = mDoCPd_c::getTrigX(mPadID) ? true : false;

    mHoldY = mDoCPd_c::getHoldY(mPadID) ? true : false;
    mTrigY = mDoCPd_c::getTrigY(mPadID) ? true : false;

    mHoldY = mDoCPd_c::getHoldY(mPadID) ? true : false;
    mTrigY = mDoCPd_c::getTrigY(mPadID) ? true : false;

    // fakematch (doesn't match in debug)
    mHoldZ = (u8)mDoCPd_c::getHoldZ(mPadID) ? true : false;
    mTrigZ = mDoCPd_c::getTrigZ(mPadID) ? true : false;
    field_0x21f = 0;

    mHoldB = mDoCPd_c::getHoldB(mPadID) ? true : false;
    mTrigB = mDoCPd_c::getTrigB(mPadID) ? true : false;
    field_0x223 = 0;
    mCameraInputActive = 0;
}

void dCamera_c::updatePad() {
    dComIfGp_getAttention();
    int var_r30 = mCamParam.Algorythmn(mCamStyle);

    f32 var_f31;
    f32 var_f30;
    f32 var_f29;

    if (chkFlag(0x1000000)) {
        var_f31 = 0.0f;
        var_f30 = 0.0f;
        var_f29 = 0.0f;
    } else {
        var_f31 = mDoCPd_c::getStickX3D(mPadID);
        var_f30 = mDoCPd_c::getStickY(mPadID);
        var_f29 = mDoCPd_c::getStickValue(mPadID);
    }

    mPadInfo.mMainStick.mPosXDelta = var_f31 - mPadInfo.mMainStick.mLastPosX;
    mPadInfo.mMainStick.mPosYDelta = var_f30 - mPadInfo.mMainStick.mLastPosY;
    mPadInfo.mMainStick.mValueDelta = var_f29 - mPadInfo.mMainStick.mLastValue;
    mPadInfo.mMainStick.mLastPosX = var_f31;
    mPadInfo.mMainStick.mLastPosY = var_f30;
    mPadInfo.mMainStick.mLastValue = var_f29;
    mPadInfo.mMainStick.mAngle.Val(mDoCPd_c::getStickAngle3D(mPadID));

    if (chkFlag(0x800000)) {
        var_f31 = 0.0f;
        var_f30 = 0.0f;
        var_f29 = 0.0f;
    } else {
        var_f31 = mDoCPd_c::getSubStickX3D(mPadID);
        var_f30 = mDoCPd_c::getSubStickY(mPadID);
        var_f29 = mDoCPd_c::getSubStickValue(mPadID);
    }

    mPadInfo.mCStick.mPosXDelta = var_f31 - mPadInfo.mCStick.mLastPosX;
    mPadInfo.mCStick.mPosYDelta = var_f30 - mPadInfo.mCStick.mLastPosY;
    mPadInfo.mCStick.mValueDelta = var_f29 - mPadInfo.mCStick.mLastValue;
    mPadInfo.mCStick.mLastPosX = var_f31;
    mPadInfo.mCStick.mLastPosY = var_f30;
    mPadInfo.mCStick.mLastValue = var_f29;
    mPadInfo.mCStick.mAngle.Val(mDoCPd_c::getSubStickAngle(mPadID));

    f32 analog_l = mDoCPd_c::getAnalogL(mPadID);
    mTriggerLeftDelta = mTriggerLeftLast - analog_l;
    mTriggerLeftLast = analog_l;
    mHoldLockL = mDoCPd_c::getHoldLockL(mPadID) ? true : false;
    mTrigLockL = mDoCPd_c::getTrigLockL(mPadID) ? true : false;

    if (mTriggerLeftLast > mCamSetup.ManualEndVal()) {
        if (mLockLActive == 0) {
            mLockLJustActivated = 1;
        } else {
            mLockLJustActivated = 0;
        }

        mLockLActive = 1;
    } else {
        mLockLJustActivated = 0;
        mLockLActive = 0;
    }

    f32 analog_r = mDoCPd_c::getAnalogR(mPadID);
    mTriggerRightDelta = mTriggerRightLast - analog_r;
    mTriggerRightLast = analog_r;
    mHoldLockR = mDoCPd_c::getHoldLockR(mPadID) ? true : false;
    mTrigLockR = mDoCPd_c::getTrigLockR(mPadID) ? true : false;

    if (mTriggerRightLast > mCamSetup.ManualEndVal()) {
        if (mLockRActive == 0) {
            mLockRJustActivated = 1;
        } else {
            mLockRJustActivated = 0;
        }

        mLockRActive = 1;
    } else {
        mLockRJustActivated = 0;
        mLockRActive = 0;
    }

    mHoldX = mDoCPd_c::getHoldX(mPadID) ? true : false;
    mTrigX = mDoCPd_c::getTrigX(mPadID) ? true : false;
    mHoldY = mDoCPd_c::getHoldY(mPadID) ? true : false;
    mTrigY = mDoCPd_c::getTrigY(mPadID) ? true : false;
    mHoldZ = mDoCPd_c::getHoldZ(mPadID) ? true : false;
    mTrigZ = mDoCPd_c::getTrigZ(mPadID) ? true : false;
    mHoldB = mDoCPd_c::getHoldB(mPadID) ? true : false;
    mTrigB = mDoCPd_c::getTrigB(mPadID) ? true : false;

    bool sp6B = true;
    bool sp6C = true;
    int temp1;
    int sp68;
    if (mCamTypeData[mCurType].field_0x18[mIsWolf][0] > 0) {
        sp68 = mIsWolf;
    } else {
        sp68 = 0;
    }
    temp1 = sp68;

    if (mCamTypeData[mCurType].field_0x18[temp1][4] < 0) {
        sp6B = false;
        if (mGear == -1) {
            mGear = 0;
        }
    }

    if ((var_r30 != 1 && var_r30 != 8 && var_r30 != 7) || mCamParam.Flag(mCamStyle, 0x80)) {
        sp6C = false;
        if (mGear == 1) {
            mGear = 0;
        }
    }

    if (mCurMode == 0 || mCurMode == 4) {
        if (check_owner_action(mPadID, 0x12000)) {
            mGear = -1;
        } else if (mGear == -1) {
            mGear = 0;
        }

        if (mPadInfo.mCStick.mLastPosY < -mCamSetup.mCStick.SwTHH()) {
            if (mCStickYState != -1) {
                if (mGear == -1 && mCurMode == 4) {
                    mGear = 0;
                    setComStat(0x2000);
                } else if (mGear == 0 && sp6C) {
                    mGear = 1;
                }
            }

            mCStickYState = -1;
        } else if (mPadInfo.mCStick.mLastPosY > mCamSetup.mCStick.SwTHH()) {
            if (mCStickYState != 1) {
                if (mGear == 0 && sp6B) {
                    setComStat(0x1000);
                } else if (mGear == 1) {
                    mGear = 0;
                }
            }

            mCStickYState = 1;
        } else {
            mCStickYState = 0;
        }

        if (mCStickYState != 0) {
            mCStickYHoldCount++;
        } else {
            mCStickYHoldCount = 0;
        }
    }

    field_0x223 = 0;
    mCameraInputActive = 0;

    if (mDoCPd_c::getStickValue(mPadID) > 0.001f || mDoCPd_c::getSubStickValue(mPadID) > 0.001f ||
        mDoCPd_c::getHold(mPadID) != 0)
    {
        mCameraInputActive = 1;
    }
}


void dCamera_c::initMonitor() {
    if (mpPlayerActor != NULL) {
        mMonitor.field_0x0 = positionOf(mpPlayerActor);
    } else {
        mMonitor.field_0x0 = cXyz::Zero;
    }

    field_0x2c0 = 0.0f;
    mMonitor.field_0x10 = 0.0f;
    mMonitor.field_0xc = 0.0f;
    mIdleFrameCount = 0;
    field_0x2c8 = 0.0f;
    mMonitor.field_0x14 = cXyz::Zero;
}

void dCamera_c::updateMonitor() {
    if (mpPlayerActor != NULL) {
        cXyz sp24 = positionOf(mpPlayerActor);
        mMonitor.field_0x14 = sp24 - mMonitor.field_0x0;

        if (mBG.field_0xc0.field_0x1) {
            dComIfG_Bgsp().MoveBgMatrixCrrPos(mBG.field_0x5c.field_0x4, true, &mMonitor.field_0x0, NULL, NULL);
        }

        f32 var_f31;
        if (chkFlag(0x10000)) {
            var_f31 = cXyz(sp24 - mMonitor.field_0x0).abs();
        } else {
            var_f31 = dCamMath::xyzHorizontalDistance(sp24, mMonitor.field_0x0);
        }

        field_0x2c0 = var_f31 - mMonitor.field_0xc;
        mMonitor.field_0x10 += (var_f31 - mMonitor.field_0x10) * 0.01f;
        mMonitor.field_0xc = var_f31;
        mMonitor.field_0x0 = sp24;

        if (!push_any_key()) {
            mIdleFrameCount++;
        } else {
            mIdleFrameCount = 0;
        }

        field_0x2c8 = mDirection.R() - field_0x2c8;
    }
}

bool dCamera_c::checkForceLockTarget() {
    bool ret = true;
    if (mLockOnActorID != -1) {
        mpLockOnActor = GetForceLockOnActor();
        if (mpLockOnActor != NULL) {
            if (dComIfGp_getAttention()->Lockon() || mForceLockTimer > mCamSetup.ForceLockOffTimer()
                || cXyz(positionOf(mpLockOnActor) - positionOf(mpPlayerActor)).abs() > mCamSetup.ForceLockOffDist())
            {
                ret = false;
            }
        } else {
            ret = false;
        }
    } else {
        ret = false;
    }
    return ret;
}

void dCamera_c::infoReport() {
#if DEBUG
#endif
}

namespace {

inline bool isPlayerFlying(daAlink_c* link) {
    bool ret = false;
    if (link->checkPlayerFly() && !link->checkBootsOrArmorHeavy()) {
        ret = true;
    }
    return ret;
}

inline bool chkCornerCos(f32 param_0) {
    return param_0 >= -0.5f && param_0 <= 0.5f;
}

}  // namespace

bool dCamera_c::Run() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    daMidna_c* midna = daPy_py_c::getMidnaActor();
    mMidnaRidingAndVisible = link->checkMidnaRide() && !midna->checkNoDraw();
    bool bVar9 = false;
    clrComStat(0x804);
    int iVar8 = mIsWolf;
    mIsWolf = daPy_py_c::checkNowWolf() ? 1 : 0;
    mFocusLine.Off();
    clrFlag(0x10168C21);
    clrFlag(0x10);
    mpAuxTargetActor2 = NULL;
    mpAuxTargetActor1 = NULL;
    s32 stay_no = dComIfGp_roomControl_getStayNo();
    if (stay_no != mRoomCtx.mRoomNo) {
        onRoomChange(stay_no);
    }
    checkGroundInfo();
    setMapToolData();

    if (link->checkRollJump() || link->checkGoronRideWait()) {
        setFlag(0x10000);
        setFlag(0x100000);
    } else {
        if (mBG.field_0xc0.field_0x44 != 0) {
            clrFlag(0x10000);
        }
    }

    if (link->checkGrabThrow() ||  link->checkIronBallThrowMode()) {
        mThrowTimer = mCamSetup.ThrowTimer();
    } else {
        if (mThrowTimer != 0) {
            mThrowTimer--;
        }
    }

    updateMonitor();
    Att();
    clrComStat(0xf400);

    if (!dComIfGp_evmng_cameraPlay() && !chkFlag(0x20000000)) {
        updatePad();
        mCamSetup.mCStick.Shift(mPadID);
    }

    if (dComIfGp_getEvent().runCheck()) {
        mPadInfo.mMainStick.mLastValue = 0.0f;
        mPadInfo.mMainStick.mLastPosY = 0.0f;
        mPadInfo.mMainStick.mLastPosX = 0.0f;
        mPadInfo.mCStick.mLastValue = 0.0f;
        mPadInfo.mCStick.mLastPosY = 0.0f;
        mPadInfo.mCStick.mLastPosX = 0.0f;
    }

    if (!checkForceLockTarget()) {
        mLockOnActorID = -1;
    } else {
        mForceLockTimer++;
    }

    mNextType = nextType(mCurType);
    if (mNextType != mCurType && onTypeChange(mCurType, mNextType)) {
        mCurType = mNextType;
    }

    clrComStat(0x40000);
    if (mCurType == specialType[CAM_TYPE_PEEP]) {
        setComStat(0x40000);
    }

    mNextMode = nextMode(mCurMode);
    if ((iVar8 != mIsWolf || mNextMode != mCurMode)
        && mCamTypeData[mCurType].field_0x18[mIsWolf][mNextMode] >= 0
        && onModeChange(mCurMode, mNextMode))
    {
        mCurMode = mNextMode;
        mCurCamStyleTimer = 0;
    }
    if (!ChangeModeOK(mCurMode)) {
        mCurMode = 0;
    }

    int style = mCamTypeData[mCurType].field_0x18[mIsWolf][mCurMode];
    if (style >= 0 && mCamStyle != style && onStyleChange(mCamStyle, style)) {
        mCamStyle = style;
        mCamParam.Change(mCamStyle);
        setFlag(0x200);
    }

    clrFlag(0);
    clrComStat(0x80);
    if (mGear == -1) {
        setComStat(0x80);
    }
    if (getComStat(0x2000)) {
        setComStat(0x80);
    }

    if (mCamParam.CheckFlag(0x4000) && !check_owner_action(mPadID, 0x4000000)
        && !link->checkMagneBootsOn() && !isPlayerFlying(link))
    {
        mForwardTiltOffset += (forwardCheckAngle() - mForwardTiltOffset) * mCamSetup.mBGChk.FwdCushion();
    } else {
        mForwardTiltOffset = cSAngle::_0;
    }

    mBumpCheckFlags = 0x4001;
    clrFlag(8);
    if (chkFlag(0x200000) && mCamParam.Algorythmn(mCamStyle) != 10) {
        if (push_any_key() || mMonitor.field_0xc > 10.0f || mBG.field_0xc0.field_0x44 == 0) {
            clrFlag(0x200000);
        }
    } else {
        bVar9 = (this->*engine_tbl[mCamParam.Algorythmn(mCamStyle)])(mCamStyle);
        field_0x170++;
        field_0x160++;
        mCurCamStyleTimer++;
    }

    mFrameCounter++;
    mTicks++;
    if (!bVar9) {
        mEngineHoldState = 0;
    }
    defaultTriming();
    if (!chkFlag(0x400)) {
        mViewCache.mBank -= mViewCache.mBank * 0.05f;
    }
    shakeCamera();
    blureCamera();

    if (mCamParam.Algorythmn() != 10) {
        clrFlag(0x80080);
        if (mCamParam.CheckFlag(1)) {
            if (mCurMode == 1 && mCamParam.CheckFlag(2)) {
                mBumpCheckFlags = 0x4007;
            } else if (chkFlag(0x20000)) {
                mBumpCheckFlags = 0x4037;
            } else {
                mBumpCheckFlags = 0x4017;
            }
        } else if (mCamParam.CheckFlag(2)) {
            mBumpCheckFlags = 0x4007;
        }
        if (mCamParam.CheckFlag(8)) {
            mBumpCheckFlags |= 0x80;
        }
        if (mCamParam.CheckFlag(0x10)) {
            mBumpCheckFlags &= ~0x4000;
        }
        mBumpCheckFlags &= ~8;
        if (mCamParam.CheckFlag(4)) {
            mBumpCheckFlags = 0;
        }
    }

    mFovy = mViewCache.mFovy;
    mBank = mViewCache.mBank;
    bumpCheck(mBumpCheckFlags);

    cSAngle angle = mPadInfo.mMainStick.mAngle - mFakeAngleSys.field_0x4;
    if (mPadInfo.mMainStick.mLastValue < mCamSetup.USOValue()
        || angle > cSAngle(mCamSetup.USOAngle()) || angle < cSAngle(-mCamSetup.USOAngle()))
    {
        mFakeAngleSys.field_0x0 = 0;
    }
    if (mFakeAngleSys.field_0x0 != 0) {
        mControlledYaw = getUSOAngle(mPadInfo.mMainStick.mAngle);
    } else {
        mControlledYaw = mDirection.U().Inv();
    }

    if (mCamSetup.CheckFlag(0x8000)) {
        infoReport();
    }

    if (chkFlag(0x10)) {
        mUp = mUpOverride.field_0x18.norm();
    } else if (dComIfGp_getStageWorldRollAngleX() != 0) {
        cSPolar polar;
        polar.R(1.0f);
        polar.V((s16)-dComIfGp_getStageWorldRollAngleX());
        polar.U(dComIfGp_getStageWorldRollDirAngleY());
        mUp = polar.Xyz();
    } else if (mCenter.x == mEye.x && mCenter.z == mEye.z) {
        mUp.set(0.01f, 1.0f, 0.0f);
    } else {
        mUp.set(0.0f, 1.0f, 0.0f);
    }

    if (!chkFlag(0x10)) {
        mUpOverride.field_0x18.set(0.0f, 1.0f, 0.0f);
    }

    if (mBankOverride != cSAngle::_0) {
        mBank = mBankOverride;
    }
    mBankOverride = cSAngle::_0;

    f32 water_height = getWaterSurfaceHeight(&mEye);
    if (water_height > mEye.y) {
        dKy_camera_water_in_status_set(1);
        Z2GetAudioMgr()->setCameraInWaterDepth(water_height - mEye.y);
    } else {
        dKy_camera_water_in_status_set(0);
        Z2GetAudioMgr()->setCameraInWaterDepth(0.0f);
    }

    if (mStyleSettle.mFinished) {
        setComStat(0x10);
    } else {
        clrComStat(0x10);
    }

    f32 hide_dist = mCamSetup.PlayerHideDist();
    if (mDirection.R() < hide_dist) {
        if (chkFlag(0x800) & 1) {
            setComStat(2);
        } else if (chkFlag(0x10000000)) {
            setComStat(0x20);
        }
    }

    runEventRecoveryTrans();
    clrFlag(0x1000);
    mTagCamTool.Clr();
    field_0x89c.Clr();
    return bVar9;
}

bool dCamera_c::NotRun() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    daMidna_c* midna = daPy_py_c::getMidnaActor();
    mMidnaRidingAndVisible = link->checkMidnaRide() && !midna->checkNoDraw();
    clrComStat(0x804);
    clrFlag(0x10168C21);
    checkGroundInfo();
    clrComStat(0x80);

    if (dComIfGp_evmng_cameraPlay() || chkFlag(0x20000000)) {
        if (mCurType != specialType[CAM_TYPE_EVENT]) {
            pushInfo(&mSavedView, 1);
            mEventData.field_0xc = mCurType;
            mCurType = specialType[CAM_TYPE_EVENT];
            mCamStyle = mCamParam.SearchStyle('EN01');
            field_0x170 = 0;
            field_0x160 = 0;
            mCurCamStyleTimer = 0;
        }
        eventCamera(mCamTypeData[mCurType].field_0x18[mIsWolf][3]);
        field_0x170++;
        field_0x160++;
        mCurCamStyleTimer++;
    }

    setComStat(0x14);
    clrFlag(0x80080);
    mFocusLine.Off();
    shakeCamera();
    blureCamera();
    field_0x21 = 0;
    if (mBankOverride != cSAngle::_0) {
        mBank = mBankOverride;
    }
    mBankOverride = cSAngle::_0;

    f32 water_height = getWaterSurfaceHeight(&mEye);
    if (water_height > mEye.y) {
        dKy_camera_water_in_status_set(1);
        Z2GetAudioMgr()->setCameraInWaterDepth(water_height - mEye.y);
    } else {
        dKy_camera_water_in_status_set(0);
        Z2GetAudioMgr()->setCameraInWaterDepth(0.0f);
    }

    mFrameCounter++;
    mTicks++;
    mTagCamTool.Clr();
    field_0x89c.Clr();
    return true;
}

s16 dCamera_c::V() {
    if (chkFlag(0x10)) {
        return mUpOverride.field_0x24.V();
    } else {
        return mDirection.V();
    }
}

s16 dCamera_c::U() {
    if (chkFlag(0x10)) {
        return mUpOverride.field_0x24.U();
    } else {
        return mDirection.U().Inv();
    }
}

bool dCamera_c::SetTrimSize(s32 size) {
    bool rt = mTrimSize != size;
    mTrimSize = size;
    return rt;
}

bool dCamera_c::SetTrimTypeForce(s32 param_0) {
    mTrimTypeForce = param_0;
    return 1;
}

void dCamera_c::CalcTrimSize() {
    if (mCurState != 2) {
        switch (mTrimSize) {
        case 0:
            mTrimHeight += -mTrimHeight * 0.25f;
            break;
        case 2:
            mTrimHeight += (mCamSetup.CinemaScopeTrimHeight() - mTrimHeight) * 0.25f;
            break;
        case 1:
            mTrimHeight += (mCamSetup.VistaTrimHeight() - mTrimHeight) * 0.25f;
            break;
        case 3:
            mTrimHeight += (mCamSetup.CinemaScopeTrimHeight() - mTrimHeight) * 0.25f;
            break;
        case 4:
            mTrimHeight = 0.0f;
            break;
        }
    } else {
        OS_REPORT("%06d: camera: trim: keep\n", mFrameCounter);
    }

    if (mCurState == 1) {
        mCurState = 0;
    } else if (mCurState == 2) {
        if (dComIfGp_getEvent().isOrderOK()) {
            mCurState = 0;
        }
    }

    mForcedMode = 11;
}

int dCamera_c::Draw() {
#if DEBUG
    debugDraw();
#endif
    mFocusLine.Draw();
    return 1;
}

void dCamera_c::setStageMapToolData() {
    int cameraIndex = 0xFF;
    int var_r27 = 0xFF;

    mStageCamTool.Clr();

    dStage_dt_c* stage_dt = (dStage_dt_c*)dComIfGp_getStage();
    if (stage_dt != NULL) {
        stage_camera_class* camera = stage_dt->getCamera();
        stage_arrow_class* arrow = stage_dt->getArrow();

        stage_stag_info_class* staginfo = stage_dt->getStagInfo();
        if (staginfo != NULL) {
            cameraIndex = dStage_stagInfo_DefaultCameraType(staginfo);
        }

        if (camera != NULL && cameraIndex >= 0 && cameraIndex < camera->num) {
            mStageCamTool.mCameraIndex = cameraIndex;
            mStageCamTool.mCamData = camera->m_entries[cameraIndex];
            mStageCamTool.mFlags = (mStageCamTool.mCamData.field_0x14 >> 0xE) & 3;

            if (mStageCamTool.mCamData.field_0x14 & 0x2000) {
                mStageCamTool.mCamData.field_0x14 |= (u16) 0xC000;
            } else {
                mStageCamTool.mCamData.field_0x14 &= (u16) ~0xC000;
            }

            var_r27 = mStageCamTool.mCamData.m_arrow_idx;
            if (arrow != NULL && var_r27 >= 0 && var_r27 < arrow->num) {
                mStageCamTool.mArrowIndex = var_r27;
                mStageCamTool.mArrowData = arrow->m_entries[var_r27];
            }
        }
    }
}

void dCamera_c::setMapToolData() {
    int room_no = dComIfGp_roomControl_getStayNo();
    stage_camera_class* camera = dComIfGp_getRoomCamera(room_no);
    stage_arrow_class* arrow = dComIfGp_getRoomArrow(room_no);

    int var_r29 = 0xFF;
    int var_r26 = 0xFF;

    if (mBG.field_0xc0.field_0x40 == 0x1FF && mRoomMapTool.mpActor == NULL) {
        return;
    }

    if (mBG.field_0xc0.field_0x40 != 0xFF) {
        mRoomMapTool.Clr();
        var_r29 = mBG.field_0xc0.field_0x40;
    } else if (field_0x8d8.mCameraIndex != 0xFF) {
        mRoomMapTool.Clr();
        var_r29 = field_0x8d8.mCameraIndex;
    } else if (mDefRoomCamTool.mCameraIndex != 0xFF) {
        mRoomMapTool = mDefRoomCamTool;
        return;
    } else if (mStageCamTool.mCameraIndex != 0xFF) {
        mRoomMapTool = mStageCamTool;
        return;
    } else {
        mRoomMapTool.Clr();
    }

    setRoomMapToolData(&mRoomMapTool, var_r29, room_no);
}

void dCamera_c::SetTagData(fopAc_ac_c* param_0, s32 param_1, u16 param_2, u8 param_3) {
    s32 room_no;
    if (param_0 != NULL) {
        room_no = fopAcM_GetRoomNo(param_0);
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
    }

    mTagCamTool.Set(param_1, room_no, param_0, param_2, param_3);
}

void dCamera_c::setRoomMapToolData(dCamMapToolData* i_toolData, s32 param_1, s32 i_roomNo) {
    i_toolData->Set(param_1, i_roomNo, NULL, 0xFF, 0xFF);
}

s32 dCamera_c::nextMode(s32 i_curMode) {
    dAttention_c* attn = dComIfGp_getAttention();
    s32 next_mode = i_curMode;
    cXyz player_pos = positionOf(mpPlayerActor);
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (!dComIfGp_evmng_cameraPlay()) {
        if (mBG.field_0x0.field_0x58 > player_pos.y) {
            field_0x223 = 0;
        }
        if (!link->checkFastShotTime()) {
            mFastShotState = 0;
        }
        if (mForcedMode != 11 && mCamTypeData[mCurType].field_0x18[mIsWolf][mForcedMode] >= 0) {
            next_mode = mForcedMode;
        } else if (check_owner_action(mPadID, 0x200000) && !attn->Lockon()) {
            if (check_owner_action(mPadID, 0x25040)) {
                next_mode = 7;
            } else {
                next_mode = 0;
            }
        } else if (link->checkGoatThrow() && dComIfGoat_GetThrow() != NULL) {
            dComIfGp_getAttention()->LockSoundOff();
            mpLockonTarget = dComIfGoat_GetThrow();
            if (fopAcM_GetName(mpLockonTarget) == PROC_E_GOB) {
                if (link->checkGoatThrowAfter()) {
                    next_mode = 2;
                } else {
                    next_mode = 1;
                }
            } else if (fopAcM_GetName(mpLockonTarget) == PROC_OBJ_GRA) {
                next_mode = 1;
            } else {
                next_mode = 2;
            }
        } else if (link->checkGoronSideMove() || link->getSumouCameraMode()) {
            dComIfGp_getAttention()->LockSoundOff();
            next_mode = 1;
        } else if (link->checkFastShotTime()) {
            mFastShotState = 1;
        } else if (check_owner_action1(mPadID, 0x10)) {
            next_mode = 9;
        } else if ((check_owner_action1(mPadID, 0x400000)
                    || check_owner_action1(mPadID, 0x200000)) && mpLockonTarget != NULL) {
            next_mode = 6;
        } else if (link->checkCutHeadProc()) {
            next_mode = 6;
        } else if (check_owner_action(mPadID, 6) && mpLockonTarget != NULL) {
            next_mode = 5;
        } else if (attn->LockonTruth() && mpLockonTarget != NULL
                                        && !check_owner_action(mPadID, 0xC000000)) {
            next_mode = 2;
        } else if (check_owner_action(mPadID, 0x12000)) {
            next_mode = 4;
        } else if (check_owner_action(mPadID, 0x25040) && !attn->Lockon()) {
            next_mode = 7;
        } else if ((check_owner_action(mPadID, 0x80480) && !attn->Lockon())
                                                        || link->checkHawkWait()) {
            next_mode = 8;
        } else if (check_owner_action(mPadID, 0x4000000) || link->checkChainBlockPushPull()) {
            next_mode = 10;
        } else if (attn->Lockon()) {
            next_mode = 1;
        } else if ((check_owner_action(mPadID, 0x400000) || link->checkIronBallThrowMode()
                                                            || link->checkBoomerangAtnKeep())
                            && !check_owner_action(mPadID, 0x36A02311) && mFastShotState == 0
                            && !check_owner_action1(mPadID, 0x11)) {
            mpLockonTarget = get_boomerang_actor(mpPlayerActor);
            next_mode = 2;
            mLockOnActorID = -1;
            mFastShotState = 1;
        } else if ((link->checkCopyRodThrowAfter() || link->checkCopyRodAtnKeep())
                                    && link->getCopyRodCameraActor() == NULL) {
            mpLockonTarget = link->getCopyRodActor();
            next_mode = 2;
            mLockOnActorID = -1;
            mFastShotState = 1;
        } else if (link->checkSpinnerRideWait()) {
            next_mode = 1;
        } else if (mLockOnActorID != -1) {
            if (mpLockOnActor != NULL) {
                next_mode = 2;
                mpLockonTarget = mpLockOnActor;
            } else {
                next_mode = 0;
                mLockOnActorID = -1;
            }
        } else {
            next_mode = 0;
        }
    }

    if (mCamTypeData[mCurType].field_0x18[mIsWolf][next_mode] >= 0) {
        if (next_mode != 2) {
            mLockOnActorID = -1;
        }
        if (next_mode == 1) {
            setFlag(0x100000);
        } else if (next_mode == 10) {
            setFlag(0x100000);
        }
    } else {
        next_mode = i_curMode;
    }

    switch (next_mode) {
    case 4:
        dComIfGp_getAttention()->LockSoundOff();
        break;
    }

    return next_mode;
}

bool dCamera_c::onModeChange(s32 i_curMode, s32 i_nextMode) {
    field_0x160 = 0;
    field_0x164 = 0;
    field_0x168 = 1;
    mSightFitRadius = 0.0f;
    clrFlag(0x2306);

    switch (i_curMode) {
    case 3:
        clrComStat(4);
        break;
    case 4:
    case 7:
    case 8:
        if (i_nextMode == 0) {
            setFlag(0x40000);
        }
        break;
    }

    switch (i_nextMode) {
    case 7:
        mGear = 0;
        break;
    case 1:
        setFlag(0x100000);
        // fallthrough
    case 0:
        if (mCamTypeData[mCurType].field_0x18[mIsWolf][i_curMode]
            == mCamTypeData[mCurType].field_0x18[mIsWolf][i_nextMode])
        {
            field_0x168 = 0;
        }
        // fallthrough
    case 2:
        if (mCamSetup.CheckFlag(0x4000)) {
            mGear = 0;
        }
        break;
    case 10:
        break;
    }

    return true;
}

int dCamera_c::ModeFix(s32 param_0) {
    mForcedMode = param_0;
    return 1;
}

s32 dCamera_c::nextType(s32 i_curType) {
    s32 next_type = i_curType;

    if (mEngineHoldState != 1) {
        s32 iVar14 = 0xff;
        if (mTagCamTool.mCameraIndex != 0xff) {
            s32 type = GetCameraTypeFromToolData(&mTagCamTool.mCamData);
            if (type != 0xff) {
                iVar14 = type;
            }
        }

        daAlink_c* link = daAlink_getAlinkActorClass();

        bool bVar2 = false;
        bool bVar1 = false;
        if (link->checkMagneBootsOn()) {
            cXyz* top_vec = link->getMagneBootsTopVec();
            if (cBgW_CheckBRoof(top_vec->y)) {
                bVar2 = true;
            } else if (cBgW_CheckBWall(top_vec->y)) {
                bVar1 = true;
            }
        }

        bool copy_rod = false;
        fopAc_ac_c* copyRodCameraActor = link->getCopyRodCameraActor();
        if (copyRodCameraActor != NULL) {
            copy_rod = true;
        }

        if (check_owner_action(mPadID, 0x200000) && ChangeModeOK(4)
                                                 && !dComIfGp_getAttention()->Lockon()) {
            next_type = specialType[CAM_TYPE_SCOPE];
        } else if (iVar14 != 0xff && !(mTagCamTool.mFlags & 0x10)) {
            next_type = iVar14;
            mRoomMapTool = mTagCamTool;
        } else if (link->checkMidnaLockJumpPoint() &&
                                    (daPy_py_c::getMidnaActor()->checkFlyWaitAnime()
                                    || daPy_py_c::getMidnaActor()->checkNoInput())) {
            next_type = specialType[CAM_TYPE_MIDNA_TAG];
        } else if (daPy_py_c::getMidnaActor()->checkPortalObjCall()) {
            next_type = specialType[CAM_TYPE_WARP_OBJ];
        } else if (link->checkGoatStopGame()) {
            next_type = specialType[CAM_TYPE_GOAT_BATTLE];
        } else if (chkFlag(0x10000)) {
            next_type = specialType[CAM_TYPE_GORON_JUMP];
        } else if (link->checkHorseRide()) {
            next_type = specialType[CAM_TYPE_HORSE_T];
        } else if (bVar1) {
            next_type = specialType[CAM_TYPE_MAGNE_WALL];
        } else if (bVar2) {
            next_type = specialType[CAM_TYPE_MAGNE_ROOF];
        } else if (copy_rod) {
            next_type = specialType[CAM_TYPE_COPY_ROD_HALL];
        } else if (link->checkBoarRide()) {
            next_type = specialType[CAM_TYPE_BOAR];
        } else if (link->checkCanoeRide()) {
            next_type = specialType[CAM_TYPE_CANOE];
        } else if (link->checkBoardRide()) {
            next_type = specialType[CAM_TYPE_BOARD];
        } else if (link->checkSpinnerRide()) {
            next_type = specialType[CAM_TYPE_SPINNER];
        } else if (check_owner_action1(mPadID, 0x2000000)) {
            next_type = specialType[CAM_TYPE_HOOK_WALL];
        } else if (check_owner_action1(mPadID, 0x10000)) {
            if (link->getHookshotRoofWaitActor() != NULL) {
                next_type = specialType[CAM_TYPE_HOOK_ACTOR];
            } else {
                next_type = specialType[CAM_TYPE_HOOK_ROOF];
            }
        } else if (check_owner_action1(mPadID, 0x100000)) {
            next_type = specialType[CAM_TYPE_ROOF_HUNG];
        } else if (link->checkCokkoGlide()) {
            next_type = specialType[CAM_TYPE_COCCO_JUMP];
        } else if (check_owner_action(mPadID, 0x100000)) {
            if (getComStat(0x800)) {
                next_type = specialType[CAM_TYPE_WATER_SURF];
            } else if (mBG.field_0xc0.field_0x3c != 0xff) {
                setRoomMapToolData(&mRoomMapTool, mBG.field_0xc0.field_0x3c,
                                   dComIfGp_roomControl_getStayNo());
                s32 type = GetCameraTypeFromToolData(&mRoomMapTool.mCamData);
                if (type != 0xff) {
                    next_type = type;
                } else {
                    next_type = specialType[CAM_TYPE_WATER];
                }
            } else {
                next_type = specialType[CAM_TYPE_WATER];
            }
        } else if (iVar14 != 0xff) {
            next_type = iVar14;
            mRoomMapTool = mTagCamTool;
        } else if (mRoomMapTool.mCameraIndex != 0xff) {
            s32 type = GetCameraTypeFromToolData(&mRoomMapTool.mCamData);
            if (type != 0xff) {
                next_type = type;
            }
        } else if (mRoomMapTool.mCameraIndex != 0x1ff) {
            next_type = mMapToolType;
        }
    }

    field_0x698 = 0xff;
    field_0x69c = 0;
    if (mCamTypeData[mCurType].field_0x18[mIsWolf][0] < 0) {
        next_type = mMapToolType;
        if (mRoomMapTool.mCameraIndex != 0xff) {
            s32 type = GetCameraTypeFromToolData(&mRoomMapTool.mCamData);
            if (type != 0xff) {
                next_type = type;
            }
        }
    }

    if (!ChangeModeOK(2)) {
        dComIfGp_getAttention()->LockSoundOff();
    }

    if (dComIfGp_evmng_cameraPlay() || chkFlag(0x20000000)) {
        if (i_curType != specialType[CAM_TYPE_EVENT]) {
            mEventData.field_0xc = next_type;
        }
        next_type = specialType[CAM_TYPE_EVENT];
        dComIfGp_getAttention()->LockSoundOff();
    } else {
        clrFlag(0x40000000);
        if (dComIfGp_getEvent().runCheck()) {
            setComStat(4);
            dComIfGp_getAttention()->LockSoundOff();
        }
    }

    return next_type;
}

bool dCamera_c::onTypeChange(s32 i_curType, s32 i_nextType) {
    if (i_curType == specialType[CAM_TYPE_EVENT]) {
        if (mCamSetup.CheckFlag(0x4000)) {
            mGear = 0;
        }

        if (mRecovery.field_0x4 != 0) {
            cXyz vec = mRecovery.field_0x28 - positionOf(mpPlayerActor);
            setEventRecoveryTrans(mRecovery.field_0x4);
            popInfo(&mSavedView);
            mViewCache.mCenter -= vec;
            mViewCache.mEye -= vec;
        }

        mRecovery.field_0x4 = 0;
        field_0x668 = 0;
        clearInfo(&mSavedView, 0);
        clearInfo(&mSavedViewStack[0], 0);
        clearInfo(&mSavedViewStack[1], 0);

        if (chkFlag(0x400000)) {
            mCamStyle = mCamTypeData[mEventData.field_0xc].field_0x18[mIsWolf][0];
            mCamParam.Change(mCamStyle);
            setFlag(0x200);
            clrFlag(0x400000);
        }

        mEventData.field_0x14 = -1;
        mEventData.field_0x18 = -1;
        mEventData.mStaffIdx = -1;
        mEventData.field_0xc = -1;
        mEventData.field_0x0 = 1;
        setFlag(0x20);

        if (chkFlag(0x40000000)) {
            cM3dGLin line(mViewCache.mCenter, mViewCache.mEye);
            cXyz attn_pos = attentionPos(mpPlayerActor);
            cXyz proj;
            f32 dist;
            if (cM3d_Len3dSqPntAndSegLine(&line, &attn_pos, &proj, &dist)) {
                mViewCache.mCenter = proj;
                mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
            }
            clrFlag(0x40000000);
        }
    } else if (i_nextType == specialType[CAM_TYPE_EVENT]) {
        clrFlag(0x200000);
        pushInfo(&mSavedView, 1);
        mSightFitRadius = 0.0f;
        field_0x668 = 0;
    }

    field_0x170 = 0;
    field_0x16c = 0;
    return true;
}

bool dCamera_c::onStyleChange(s32 param_0, s32 param_1) {
    mCurCamStyleTimer = 0;
    mStyleSettle.mFinished = false;
    bool var_r30 = false;

    switch (mCamParam.Algorythmn(param_0)) {
    case 5:
    case 6:
        if (mFakeAngleSys.field_0x0 == 0) {
            setUSOAngle();
        }
        var_r30 = true;
        break;
    case 4:
        clrComStat(8);
        setComZoomScale(1.0f);
        break;
    }

    switch (mCamParam.Algorythmn(param_1)) {
    case 0:
    case 3:
    default:
        break;
    case 8:
        if (mCamParam.Algorythmn(param_0) == mCamParam.Algorythmn(param_1) && mCurMode == 0) {
            setFlag(0x8000);
        }
        break;
    case 1:
        mCamParam.Algorythmn(param_1);
        mCamParam.Algorythmn(param_0);
        break;
    case 5:
    case 6:
        if (mFakeAngleSys.field_0x0 == 0 || var_r30) {
            setUSOAngle();
        }
        break;
    }

    return true;
}


int dCamera_c::onRoomChange(s32 i_roomNo) {
    mRoomCtx.mRoomNo = i_roomNo;
    setFlag(0x1000);

    int var_r29 = 0xFF;
    dStage_roomDt_c* room_dt = dComIfGp_roomControl_getStatusRoomDt(i_roomNo);
    if (room_dt != NULL) {
        dStage_FileList_dt_c* filelist = room_dt->getFileListInfo();
        if (filelist != NULL) {
            var_r29 = dStage_FileList_dt_GetDefaultCamera(filelist);
        }
    }

    if (var_r29 != 0xFF) {
        setRoomMapToolData(&mDefRoomCamTool, var_r29, i_roomNo);
    } else {
        mDefRoomCamTool.Clr();
    }

    return 1;
}

fopAc_ac_c* dCamera_c::getParamTargetActor(s32 param_0) {
    daAlink_c* player = daAlink_getAlinkActorClass();

    switch (*(u32*)&mCamTypeData[param_0].name[16]) {
    case '@LOC':
        return dComIfGp_getAttention()->LockonTarget(0);
    case '@ACT':
        return dComIfGp_getAttention()->ActionTarget(0);
    case '@CHK':
        return dComIfGp_getAttention()->CheckObjectTarget(0);
    case '@CPY':
        return player->getCopyRodCameraActor();
    }

    return fopAcM_searchFromName4Event(&mCamTypeData[param_0].name[16], -1);
}

int dCamera_c::GetCameraTypeFromMapToolID(s32 param_0, s32 i_roomNo) {
    dStage_dt_c& stage_dt = g_dComIfG_gameInfo.play.getStage();

    int i;
    stage_camera_class* camera;
    stage_arrow_class* arrow;
    if (i_roomNo == -1) {
        camera = stage_dt.getCamera();
        arrow = stage_dt.getArrow();
    } else {
        camera = dComIfGp_getRoomCamera(i_roomNo);
        arrow = dComIfGp_getRoomArrow(i_roomNo);

        if (camera == NULL) {
            if (!(mFrameCounter & 0xFFF)) {
                OS_REPORT("camera: no room camera list. camera ID ignored\n");
            }
            return 0xFF;
        }
    }

    if (param_0 < 0 || camera == NULL || (camera != NULL && param_0 >= camera->num)) {
        int sp28;
        if (camera != NULL) {
            sp28 = camera->num;
        } else {
            sp28 = -99;
        }

        OS_REPORT("camera: bad number %d for map data ID [%x:%d] room %d\n", param_0, camera, sp28,
                  i_roomNo);
        return 0xFF;
    }

    i = 0;
    while (i < mCamTypeNum) {
        if (strcmp(camera->m_entries[param_0].m_cam_type, mCamTypeData[i].name) ==
            0)
        {
            break;
        }

        i++;
    }

    if (i == mCamTypeNum) {
        OS_REPORT("camera: type \'%s\' not found\n", camera->m_entries[param_0].m_cam_type);
        return 0xFF;
    }

    mRoomMapTool.mCamData = camera->m_entries[param_0];
    mRoomMapTool.mCameraIndex = param_0;
    mRoomMapTool.mFlags = (mRoomMapTool.mCamData.field_0x14 >> 0xE) & 3;

    if (mRoomMapTool.mCamData.field_0x14 & 0x2000) {
        mRoomMapTool.mCamData.field_0x14 |= 0xC000;
    } else {
        mRoomMapTool.mCamData.field_0x14 &= ~0xC000;
    }

    mRoomMapTool.mArrowIndex = mRoomMapTool.mCamData.m_arrow_idx;

    if (mRoomMapTool.mArrowIndex != -1 && mRoomMapTool.mArrowIndex < arrow->num) {
        mRoomMapTool.mArrowData = arrow->m_entries[mRoomMapTool.mArrowIndex];
    } else {
        mRoomMapTool.mArrowIndex = 0xFF;
    }

    return i;
}

int dCamera_c::GetCameraTypeFromCameraName(char const* i_name) {
    if (strcmp(i_name, mCamTypeData[mCurType].name) == 0) {
        return mCurType;
    }

    int i = 0;
    while (i < mCamTypeNum) {
        if (strcmp(i_name, mCamTypeData[i].name) == 0) {
            break;
        }

        i++;
    }

    if (i == mCamTypeNum) {
        OS_REPORT("camera: type \'%s\' not found\n", i_name);
        return 0xFF;
    }

    return i;
}

int dCamera_c::GetCameraTypeFromToolData(stage_camera2_data_class* i_data) {
    if (i_data->field_0x16 != 0xFFFF) {
        return i_data->field_0x16;
    }

    int type = GetCameraTypeFromCameraName(i_data->m_cam_type);
    if (type < 0xFFFF) {
        i_data->field_0x16 = type;
    }

    return type;
}

void dCamera_c::pushInfo(dCamera_c::dCamInfo_c* i_info, s16 param_1) {
    i_info->mCenter = mCenter;
    i_info->mEye = mEye;
    i_info->mFovy = mFovy;
    i_info->mBank = mBank;
    i_info->field_0x1e = param_1;
}

void dCamera_c::popInfo(dCamera_c::dCamInfo_c* i_info) {
    mCenter = mViewCache.mCenter = i_info->mCenter;
    mEye = mViewCache.mEye = i_info->mEye;
    mFovy = mViewCache.mFovy = i_info->mFovy;
    mViewCache.mBank = i_info->mBank;
    mBank = mViewCache.mBank;
}

f32 dCamera_c::heightOf(fopAc_ac_c* i_actor) {
    if (is_player(i_actor)) {
        return ((daPy_py_c*)i_actor)->getHeight();
    } else {
        return (i_actor->eyePos.y - i_actor->current.pos.y) * 1.1f;
    }
}

cXyz dCamera_c::relationalPos(fopAc_ac_c* i_actor, cXyz* i_offset) {
    if (i_actor == NULL) {
        return cXyz::Zero;
    }

    cXyz offset = dCamMath::xyzRotateY(*i_offset, cSAngle(i_actor->shape_angle.y));
    return attentionPos(i_actor) + offset;
}

cXyz dCamera_c::relationalPos(fopAc_ac_c* i_actor1, fopAc_ac_c* i_actor2, cXyz* i_offset,
                              f32 param_3) {
    if (i_actor1 == NULL) {
        return cXyz::Zero;
    }

    if (i_actor2 == NULL) {
        return relationalPos(i_actor1, i_offset);
    }

    cXyz pos1 = attentionPos(i_actor1);
    cXyz pos2 = attentionPos(i_actor2);
    cXyz delta = pos2 - pos1;
    cSGlobe delta_globe(delta);
    delta.normalize();
    pos1 -= delta * i_actor1->attention_info.field_0xa;
    pos2 += delta * i_actor2->attention_info.field_0xa;
    cXyz mid = pos1 + (pos2 - pos1) * 0.5f;
    cXyz offset = *i_offset;
    cSAngle angle = mViewCache.mDirection.U() - delta_globe.U();
    if (angle < cSAngle::_0) {
        offset.x = -offset.x;
    }
    cSGlobe offset_globe(offset);
    offset_globe.U(delta_globe.U() + offset_globe.U());
    delta_globe.R(0.5f * delta_globe.R() * angle.Cos() * param_3);
    cXyz ret = mid + delta_globe.Xyz() + offset_globe.Xyz();
    return ret;
}

void dCamera_c::setUSOAngle() {
    mFakeAngleSys.field_0x0 = 1;
    mFakeAngleSys.field_0x2 = mDirection.U().Inv();
    mFakeAngleSys.field_0x4 = mPadInfo.mMainStick.mAngle;
}

cSAngle dCamera_c::getUSOAngle(cSAngle param_0) {
    return mFakeAngleSys.field_0x2;
}

bool dCamera_c::pointInSight(cXyz* i_point) {
    cXyz proj;
    dDlst_window_c* window = get_window(field_0x0);
    scissor_class* scissor = window->getScissor();
    f32 scissor_width = scissor->width;
    f32 scissor_height = scissor->height;
    mDoLib_project(i_point, &proj);
    return (proj.x > 0.0f && proj.x < scissor_width) && (proj.y > 0.0f && proj.y < scissor_height);
}

f32 dCamera_c::radiusActorInSight(fopAc_ac_c* i_actor1, fopAc_ac_c* i_actor2, cXyz* i_center,
                                   cXyz* i_eye, f32 i_fovY, s16 i_bank, f32 param_6) {
    dDlst_window_c* window = get_window(field_0x0);
    scissor_class* scissor = window->getScissor();
    f32 dVar3 = cAngle::d2r(i_fovY) * 0.5f;
    f32 tmp = (scissor->height - mTrimHeight * 2.0f) / FB_HEIGHT * dVar3;
    f32 fVar8 = tmp * (mTrimHeight < 0.01f ? 0.95f : 1.0f);
    dVar3 *= mWindowAspect;
    f32 fVar7 = dVar3 * (scissor->width / FB_WIDTH) * 0.85f;

    cXyz pos1 = attentionPos(i_actor1);
    pos1.y += (positionOf(i_actor1).y - attentionPos(i_actor1).y) * 0.5f;
    cXyz pos2 = attentionPos(i_actor2);
    pos2.y += (positionOf(i_actor2).y - attentionPos(i_actor2).y) * 0.5f;

    cXyz delta = pos1 - pos2;
    f32 dist = delta.abs();
    f32 local_128 = 1.0f - (dist - 0.1f - 200.0f);
    if (local_128 < 0.2f) {
        local_128 = 0.2f;
    }
    if (local_128 > 1.0f) {
        local_128 = 1.0f;
    }
    dist *= param_6 * local_128;

    delta.normalize();
    pos1 += delta * (i_actor1->attention_info.field_0xa + dist);
    pos2 -= delta * (i_actor2->attention_info.field_0xa + dist);

    Mtx look_mtx;
    mDoMtx_lookAt(look_mtx, i_eye, i_center, &mUp, i_bank);
    MTXMultVec(look_mtx, &pos1, &pos1);
    MTXMultVec(look_mtx, &pos2, &pos2);

    int bVar2 = 0;

    f32 fVar5 = fabsf(cM_atan2f(pos1.x, -pos1.z));
    f32 fVar6 = fabsf(cM_atan2f(pos1.y, -pos1.z));
    if (fVar5 > fVar7) {
        bVar2 |= 1;
    }
    if (fVar6 > fVar8) {
        bVar2 |= 2;
    }

    fVar5 = fabsf(cM_atan2f(pos2.x, -pos2.z));
    fVar6 = fabsf(cM_atan2f(pos2.y, -pos2.z));
    if (fVar5 > fVar7) {
        bVar2 |= 4;
    }
    if (fVar6 > fVar8) {
        bVar2 |= 8;
    }

    if (bVar2 == 0) {
        return 0.0f;
    }

    f32 ret = 0.0f;

    f32 local_12c = 1.0f;
    f32 local_130 = 1.0f;
    if (bVar2 & 5) {
        local_12c = i_tanf(fVar7);
    }
    if (bVar2 & 0xA) {
        local_130 = i_tanf(fVar8);
    }

    if (bVar2 & 1) {
        f32 dVar4 = pos1.z + fabsf(pos1.x) / local_12c;
        if (dVar4 > ret) {
            ret = dVar4;
        }
    }
    if (bVar2 & 2) {
        f32 dVar4 = pos1.z + fabsf(pos1.y) / local_130;
        if (dVar4 > ret) {
            ret = dVar4;
        }
    }
    if (bVar2 & 4) {
        f32 dVar4 = pos2.z + fabsf(pos2.x) / local_12c;
        if (dVar4 > ret) {
            ret = dVar4;
        }
    }
    if (bVar2 & 8) {
        f32 dVar4 = pos2.z + fabsf(pos2.y) / local_130;
        if (dVar4 > ret) {
            ret = dVar4;
        }
    }

    return ret;
}

f32 dCamera_c::groundHeight(cXyz* param_0) {
    dBgS_GndChk gndchk;
    gndchk.SetPos(param_0);
    f32 gnd_y = dComIfG_Bgsp().GroundCross(&gndchk);

    dBgS_CamGndChk_Wtr gndchk_wtr;
    gndchk_wtr.SetPos(param_0);
    f32 wtr_y = dComIfG_Bgsp().GroundCross(&gndchk_wtr);

    f32 height;
    if (gnd_y >= wtr_y) {
        height = gnd_y;
    } else {
        height = wtr_y;
    }

    f32 height_correct;
    if (height == -G_CM3D_F_INF) {
        height_correct = param_0->y;
    } else {
        height_correct = height;
    }

    return height_correct;
}

bool dCamera_c::lineBGCheck(cXyz* i_start, cXyz* i_end, dBgS_LinChk* i_linChk, u32 i_flags) {
    if (i_flags & 0x8000) {
        i_linChk->ClrCam();
        i_linChk->SetObj();
    } else {
        i_linChk->ClrObj();
        i_linChk->SetCam();
    }

    i_linChk->Set(i_start, i_end, NULL);

    if (i_flags & 4) {
        i_linChk->ClrSttsRoofOff();
    } else {
        i_linChk->SetSttsRoofOff();
    }

    if (i_flags & 2) {
        i_linChk->ClrSttsWallOff();
    } else {
        i_linChk->SetSttsWallOff();
    }

    if (i_flags & 1) {
        i_linChk->ClrSttsGroundOff();
    } else {
        i_linChk->SetSttsGroundOff();
    }

    if (i_flags & 8) {
        i_linChk->OnWaterGrp();
    } else {
        i_linChk->OffWaterGrp();
    }

    if (dComIfG_Bgsp().LineCross(i_linChk)) {
        return true;
    } else {
        return false;
    }
}

bool dCamera_c::lineBGCheck(cXyz* i_start, cXyz* i_end, cXyz* o_cross, u32 i_flags) {
    dBgS_CamLinChk lin_chk;
    if (lineBGCheck(i_start, i_end, &lin_chk, i_flags)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
        *o_cross = lin_chk.GetCross() + *plane.GetNP();
        return true;
    } else {
        *o_cross = *i_end;
        return false;
    }
}

bool dCamera_c::lineBGCheck(cXyz* i_start, cXyz* i_end, u32 i_flags) {
    dBgS_CamLinChk lin_chk;
    return lineBGCheck(i_start, i_end, &lin_chk, i_flags);
}

u32 dCamera_c::lineCollisionCheckBush(cXyz* i_start, cXyz* i_end) {
    u32 ret = 0;
    u32 result = dComIfG_Ccsp()->GetMassResultCam();
    if (result & 2) {
        ret |= 2;
    }
    if (result & 4) {
        ret |= 8;
    }
    if (result & 8) {
        ret |= 4;
    }

    cM3dGCps cps;
    cps.Set(*i_start, *i_end, 30.0f);
    dComIfG_Ccsp()->SetMassCam(cps);

    return ret;
}

static void sph_chk_callback(dBgS_SphChk* i_sphChk, cBgD_Vtx_t* i_vtxTbl, int i_vtxIdx0,
                             int i_vtxIdx1, int i_vtxIdx2, cM3dGPla* i_plane, void* i_data) {
    camSphChkdata* sph_chk_data = (camSphChkdata*)i_data;
    if (!sph_chk_data->field_0x1c) {
        f32 len = cM3d_SignedLenPlaAndPos(i_plane, &sph_chk_data->field_0xc);
        if (i_plane->getPlaneFunc(sph_chk_data->field_0x8) >= -0.0001f
                                        && len < sph_chk_data->field_0x4) {
            cXyz normal = *i_plane->GetNP();
            sph_chk_data->field_0xc += normal * (sph_chk_data->field_0x4 + 0.1f - len);
            sph_chk_data->field_0x1c = true;
        }
    }
}

cXyz dCamera_c::compWallMargin(cXyz* i_center, cXyz* param_1, f32 i_radius) {
    dBgS_CamSphChk sph_chk;
    camSphChkdata sph_chk_data(i_center, param_1, i_radius);
    sph_chk.SetCallback(&sph_chk_callback);
    sph_chk.Set(*i_center, i_radius);
    sph_chk_data.field_0x18 = i_radius;

    for (int i = 0; i < 5; i++) {
        cXyz vec = sph_chk_data.field_0xc;
        sph_chk_data.Base(&vec);
        sph_chk.Set(vec, i_radius);
        sph_chk_data.field_0x1c = false;
        if (!dComIfG_Bgsp().SphChk(&sph_chk, &sph_chk_data)) break;
    }

    return sph_chk_data.field_0xc;
}

int dCamera_c::defaultTriming() {
    if (mTrimTypeForce >= 0) {
        SetTrimSize(mTrimTypeForce);
        mTrimTypeForce = -1;
    } else {
        switch (mCurMode) {
        case 3:
            SetTrimSize(2);
            break;
        case 1:
        case 2:
        case 5:
        case 6:
            SetTrimSize(1);
            break;
        case 4:
            SetTrimSize(0);
            break;
        case 7:
        case 8:
            SetTrimSize(1);
            break;
        default:
            SetTrimSize(0);
            break;
        }
    }
    return mTrimSize;
}

void dCamera_c::setView(f32 i_xOrig, f32 i_yOrig, f32 i_width, f32 i_height) {
    dDlst_window_c* window = get_window(field_0x0);
    view_port_class* view_port = window->getViewPort();
    window->setViewPort(i_xOrig, i_yOrig, i_width, i_height, view_port->near_z, view_port->far_z);
    window->setScissor(i_xOrig, i_yOrig, i_width, i_height);
}

cSAngle dCamera_c::forwardCheckAngle() {
    dBgS_CamLinChk lin_chk;
    cSAngle ret = cSAngle::_0;
    cSAngle local_1b8;
    cSAngle local_1bc;
    cXyz player_pos = positionOf(mpPlayerActor);
    f32 player_height = heightOf(mpPlayerActor);
    player_pos.y += player_height;
    cXyz cross;

    cSAngle local_1c0(mCamSetup.mBGChk.FwdChkAngle(0));
    cSGlobe local_198(player_height * mCamSetup.mBGChk.FwdDistance(0),
                      local_1c0.Val(), mDirection.U().Inv());
    cXyz local_f4 = player_pos + local_198.Xyz();
    if (lineBGCheck(&player_pos, &local_f4, &cross, 0x40b7)) {
        cXyz local_100 = local_f4 - player_pos;
        if (local_100.abs() < 1.0f) {
            local_f4 = cross;
        } else {
            local_f4 = cross - local_100.norm() * mCamSetup.mBGChk.FwdBackMargin();
        }
    }
    cross = local_f4;
    cross.y = player_height + groundHeight(&local_f4);
    cSGlobe local_1a0(cross - player_pos);
    if (local_1a0.V() >= cSAngle::_0) {
        local_1b8 = local_1a0.V() * mCamSetup.mBGChk.FwdWeightH(0);
    } else {
        local_1b8 = local_1a0.V() * mCamSetup.mBGChk.FwdWeightL(0);
    }

    cSAngle local_1c4(mCamSetup.mBGChk.FwdChkAngle(1));
    cSGlobe local_1a8(player_height * mCamSetup.mBGChk.FwdDistance(1),
                      local_1c4.Val(), mDirection.U().Inv());
    cXyz local_10c = player_pos + local_1a8.Xyz();
    if (lineBGCheck(&player_pos, &local_10c, &cross, 0x40b7)) {
        cXyz local_100 = local_10c - player_pos;
        if (local_100.abs() < 1.0f) {
            local_10c = cross;
        } else {
            local_10c = cross - local_100.norm() * mCamSetup.mBGChk.FwdBackMargin();
        }
    }
    cross = local_10c;
    cross.y = player_height + groundHeight(&local_10c);
    cSGlobe local_1b0(cross - player_pos);
    if (local_1b0.V() >= cSAngle::_0) {
        local_1bc = local_1b0.V() * mCamSetup.mBGChk.FwdWeightH(1);
    } else {
        local_1bc = local_1b0.V() * mCamSetup.mBGChk.FwdWeightL(1);
    }

    if (local_1b8 >= cSAngle::_0 && local_1bc >= cSAngle::_0) {
        ret = local_1b8 >= local_1bc ? -local_1b8 : -local_1bc;
    } else if (local_1b8 <= cSAngle::_0 && local_1bc <= cSAngle::_0) {
        ret = local_1b8 <= local_1bc ? -local_1b8 : -local_1bc;
    } else {
        ret = -local_1b8;
    }

    if (ret <= cSAngle::_0) {
        ret *= 0.75f;
    }
    return ret;
}

bool dCamera_c::bumpCheck(u32 i_flags) {
    int uVar14 = 0;
    static int prev_plat1 = 0;
    static int prev_plat2 = 0;
    int uVar13 = 0;

    f32 gaze_back_margin = mCamSetup.mBGChk.GazeBackMargin() + 0.5f;
    f32 wall_cushion = mCamSetup.mBGChk.WallCushion();
    f32 wall_back_cushion = mCamSetup.mBGChk.WallBackCushion();
    f32 corner_cushion = mCamSetup.mBGChk.CornerCushion();
    f32 corner_angle_max_cos = cDegree(mCamSetup.mBGChk.CornerAngleMax()).Cos();
    f32 wall_up_distance = mCamSetup.mBGChk.WallUpDistance();

    cXyz center = mViewCache.mCenter;
    cXyz eye = mViewCache.mEye;
    cSGlobe direction = mViewCache.mDirection;

    if (mCamSetup.CheckFlag2(0x100) && dKy_TeachWind_existence_chk()) {
        f32 wind_pow = dKyw_get_wind_pow();
        if (wind_pow > 0.3f) {
            cSGlobe wind_globe(*dKyw_get_wind_vec());
            wind_globe.R(mViewCache.mDirection.R() * cM_rndFX(mCamSetup.WindShakeGap4Ctr() * 0.0001f));
            center += wind_globe.Xyz();
            if (wind_pow > 1.0f) {
                wind_pow = 1.0f;
            }
            wind_pow -= 0.3f;
            cSAngle angle = wind_globe.U() - mViewCache.mDirection.U();
            mFovy += mCamSetup.WindShakeGap4Fvy() * cM_rndFX(wind_pow / 0.7f) * (1.0f - fabsf(angle.Norm()));
        }
    }

    if (is_player(mpPlayerActor)) {
        u32 grab_actor_id = static_cast<daPy_py_c*>(mpPlayerActor)->getGrabActorID();
        if (grab_actor_id != -1) {
            fopAc_ac_c* grab_actor = fopAcM_SearchByID(grab_actor_id);
            if (grab_actor != NULL && !fopAcM_CheckCarryType(grab_actor,
                                        (fopAcM_CARRY)(fopAcM_CARRY_ITEM | fopAcM_CARRY_TYPE_8))) {
                if (fopAcM_CheckCarryType(grab_actor, fopAcM_CARRY_SIDE)) {
                    wall_up_distance += 40.0f;
                } else {
                    wall_up_distance += 100.0f;
                }
            }
        }
    }

    f32 fVar1 = mViewCache.mDirection.R() * mViewCache.mDirection.V().Sin();
    if (i_flags & 0x10) {
        if (fVar1 > wall_up_distance) {
            wall_up_distance = fVar1;
        }
    } else {
        clrFlag(0x4000);
    }

    if (chkFlag(0x2002)) {
        if (mpAuxTargetActor1 != NULL && mpAuxTargetActor2 != NULL) {
            f32 sight_radius = radiusActorInSight(mpPlayerActor, mpAuxTargetActor1, mpAuxTargetActor2);
            if (sight_radius > 0.0f) {
                f32 fVar17, fVar18;
                if (chkFlag(2)) {
                    fVar17 = 0.33f;
                } else {
                    fVar17 = 0.08f;
                }
                mSightFitRadius += fVar17 * (sight_radius - mSightFitRadius);
                uVar13 |= 0x40;
                fVar18 = 1.0f;
                if (field_0x160 < 10) {
                    fVar18 = (f32)field_0x160 / 10.0f;
                }
                direction.R(mSightFitRadius * fVar18 + direction.R());
                eye = center + direction.Xyz();
            }
        } else if (mpAuxTargetActor1 != NULL) {
            f32 sight_radius = radiusActorInSight(mpPlayerActor, mpAuxTargetActor1, &mViewCache.mCenter,
                                                  &mViewCache.mEye, mFovy, mBank, 0.05f);
            if (sight_radius > 0.0f) {
                f32 fVar17, fVar18;
                if (chkFlag(2)) {
                    fVar17 = 0.33f;
                } else {
                    fVar17 = 0.08f;
                }
                mSightFitRadius += fVar17 * (sight_radius - mSightFitRadius);
                uVar13 |= 0x40;
                fVar18 = 1.0f;
                if (field_0x160 < 10) {
                    fVar18 = (f32)field_0x160 / 10.0f;
                }
                direction.R(mSightFitRadius * fVar18 + direction.R());
                eye = center + direction.Xyz();
            }
        }
    }

    if (mSightFitRadius > 0.1f && !(uVar13 & 0x40)) {
        mSightFitRadius -= mSightFitRadius * 0.08f;
        direction.R(mSightFitRadius + direction.R());
        eye = center + direction.Xyz();
    }

    if ((i_flags & 0x80) && mBG.field_0x108.field_0x0 != 0) {
        direction.V(mBG.field_0x108.field_0x4 + direction.V().Degree());
        eye = center + direction.Xyz();
        uVar13 |= 0x20;
    }

    dBgS_CamLinChk lin_chk1;
    dBgS_CamLinChk lin_chk2;
    cXyz norm_dir_vec = cXyz(eye - center).norm();
    cXyz vec = eye + norm_dir_vec * gaze_back_margin;
    cXyz pos, mid;
    bool bVar8 = false;

    if (i_flags & 0xb7) {
        if (lineBGCheck(&center, &vec, &lin_chk1, i_flags)) {
            cSGlobe unused1, unused2;
            cM3dGPla plane1;
            dComIfG_Bgsp().GetTriPla(lin_chk1, &plane1);
            cM3dGPla plane2;
            cXyz* normal1 = plane1.GetNP();
            if ((i_flags & 1) && normal1->y >= 0.75f) {
                uVar14 = 4;
            } else if ((i_flags & 4) && normal1->y < -0.5f) {
                uVar14 = 3;
            } else if (i_flags & 2) {
                if (!(i_flags & 0x10)) {
                    uVar14 = 1;
                } else if (!(i_flags & 0x20)) {
                    uVar14 = 2;
                } else {
                    if (lineBGCheck(&vec, &center, &lin_chk2, i_flags)) {
                        cXyz(lin_chk1.GetCross() - lin_chk2.GetCross()).abs();
                        dComIfG_Bgsp().GetTriPla(lin_chk2, &plane2);
                        cXyz* normal2 = plane2.GetNP();
                        cXyz cross;
                        f32 dot = VECDotProduct(normal1, normal2);
                        VECCrossProduct(normal1, normal2, &cross);
                        if (chkCornerCos(dot) && fabsf(cross.y) > 0.5f
                                              && cBgW_CheckBWall(normal2->y)) {
                            uVar14 = 5;
                        } else if (mLastBumpCase != 5 && mLastBumpCase != 6) {
                            uVar14 = 7;
                        } else {
                            uVar14 = 8;
                        }
                    } else if (mLastBumpCase == 5 || mLastBumpCase == 8) {
                        uVar14 = 8;
                    } else {
                        uVar14 = 2;
                    }
                }
            }

            switch (uVar14) {
            case 5:
            case 6: {
                uVar13 |= 2;
                cXyz cross1 = lin_chk1.GetCross();
                cXyz cross2 = lin_chk2.GetCross();
                mid = (cross1 + cross2) * 0.5f;
                if (cM3d_2PlaneLinePosNearPos(plane1, plane2, &mid, &pos)) {
                    mCornerNormalSum = *plane1.GetNP() + *plane2.GetNP();
                    mLastHitPos = pos;
                    cXyz vec3 = pos + mCornerNormalSum * 2.0f;
                    cross1 += *plane1.GetNP();
                    cross2 += *plane2.GetNP();
                    dBgS_CamLinChk lin_chk3;
                    if (!lineBGCheckBoth(&cross1, &vec3, &lin_chk3, i_flags)
                        && !lineBGCheckBoth(&cross2, &vec3, &lin_chk3, i_flags))
                    {
                        vec = eye;
                        if (chkFlag(0x4000)) {
                            if (mWallRecoverStepCount == 0) {
                                f32 dist = cXyz(mEye - mViewCache.mEye).abs();
                                if (dist > 60.0f) {
                                    mWallRecoverStepCount = 40;
                                } else if (dist < 6.0f) {
                                    mWallRecoverStepCount = 4;
                                } else {
                                    mWallRecoverStepCount = dist / 1.5f;
                                }
                            }
                            f32 tmp = 1.0f / mWallRecoverStepCount;
                            direction.V(mDirection.V() + (direction.V() - mDirection.V()) * tmp);
                            vec = center + direction.Xyz();
                            if (--mWallRecoverStepCount == 0) {
                                clrFlag(0x4000);
                            }
                        }
                        if (lineBGCheck(&cross2, &eye, &lin_chk1, i_flags)) {
                            vec = lin_chk1.GetCross();
                        }
                        eye = compWallMargin(&vec, &center, gaze_back_margin);
                        bVar8 = true;
                        setFlag(0x80000);
                        break;
                    }
                }
                // fallthrough
            }

            case 2:
            case 7:
            case 8: {
                uVar13 |= 1;
                vec = mLastHitPos = lin_chk1.GetCross();
                if (i_flags & 0x10) {
                    switch (mLastBumpCase) {
                    case 2:
                    case 7:
                    case 8:
                        bVar8 = false;
                        break;
                    default:
                        bVar8 = true;
                        break;
                    }
                    f32 tmp2 = wall_up_distance - (vec.y - center.y);
                    if (tmp2 < 10.0f) {
                        tmp2 = 10.0f;
                    }
                    cSGlobe globe(*plane1.GetNP());
                    globe.V(globe.V() + cSAngle::_90);
                    globe.R(tmp2 * globe.V().Sin());
                    cXyz vec2 = vec + globe.Xyz();
                    cXyz vec3 = *plane1.GetNP();
                    vec3.y = 0.0f;
                    if (dComIfGp_evmng_cameraPlay() || CheckFlag(0x20000000)) {
                        field_0x968 = 1.0f;
                        field_0x96c = 0.0f;
                    }
                    if (bVar8) {
                        field_0x96c = 0.0f;
                    } else {
                        field_0x96c += (1.0f - field_0x96c) * 0.1f;
                    }
                    if (bVar8 || !(mMonitor.field_0xc < 5.0f)
                              || !(fabsf(mPadInfo.mCStick.mLastPosX) < 0.05f)) {
                        field_0x968 = 0.2f;
                    } else {
                        field_0x968 *= mMonitor.field_0xc / 5.0f;
                    }
                    f32 tmp = field_0x96c * (mIsWolf == 1 ? 30.0f : 30.0f);
                    center += vec3.norm() * (tmp * globe.V().Sin());
                    cSGlobe globe2(vec2 - center);
                    globe2.V(mDirection.V() + (globe2.V() - mDirection.V()) * field_0x968);
                    if (globe2.V() > cSAngle(80.0f)) {
                        globe2.V(cSAngle(80.0f));
                    }
                    globe2.R(gaze_back_margin + globe2.R());
                    vec = center + globe2.Xyz();
                    if (lineBGCheck(&center, &vec, &lin_chk1, i_flags)) {
                        vec = lin_chk1.GetCross();
                    }
                    eye = compWallMargin(&vec, &center, gaze_back_margin);
                    tooNearEscape(&eye);
                    bVar8 = true;
                    setFlag(0x80);
                    setFlag(0x4000);
                    break;
                }
                // fallthrough
            }

            case 1:
            case 3:
            case 4: {
                uVar13 |= 4;
                setFlag(0x80);
                vec = mLastHitPos = lin_chk1.GetCross();
                eye = compWallMargin(&vec, &center, gaze_back_margin);
                tooNearEscape(&eye);
                bVar8 = true;
                break;
            }
            }

        } else {
            uVar14 = 0;
            if (chkFlag(0x4000)) {
                if (mWallRecoverStepCount == 0) {
                    f32 dist = cXyz(mEye - mViewCache.mEye).abs();
                    if (dist > 60.0f) {
                        mWallRecoverStepCount = 40;
                    } else if (dist < 6.0f) {
                        mWallRecoverStepCount = 4;
                    } else {
                        mWallRecoverStepCount = (int)(dist / 1.5f);
                    }
                }

                f32 rate = 1.0f / (f32)mWallRecoverStepCount;
                direction.R(mDirection.R() + rate * (direction.R() - mDirection.R()));
                direction.V(mDirection.V() + (direction.V() - mDirection.V()) * rate);
                vec = center + direction.Xyz();
                if (lineBGCheck(&center, &vec, &lin_chk1, i_flags)) {
                    vec = lin_chk1.GetCross();
                }

                if (--mWallRecoverStepCount == 0) {
                    clrFlag(0x4000);
                }
            } else {
                vec = eye;
            }

            eye = compWallMargin(&vec, &center, gaze_back_margin);
            bVar8 = true;
        }
    }

    mLastBumpCase = uVar14;
    if ((i_flags & 0x4000) && mCamSetup.CheckFlag2(0x400)) {
        cCcD_ShapeAttr::Shape shape;
        for (int i = 0; i < 3; i++) {
            if (!dComIfG_Ccsp()->chkCameraPoint(eye, &shape, mpPlayerActor, NULL)) {
                break;
            }
            bool bVar9 = false;
            cXyz vec;
            f32 var_f29, var_f28;

            switch (shape._0) {
            case 0:
                vec = shape._4;
                vec.y -= shape._10;
                var_f29 = shape._10;
                var_f28 = var_f29 * 2.0f;
                bVar9 = true;
                break;

            case 1:
                vec = shape._4;
                var_f29 = shape._10;
                var_f28 = shape._14;
                bVar9 = true;
                break;
            }

            if (bVar9 && center.y >= vec.y && center.y <= vec.y + var_f28) {
                cXyz vec2;
                vec2.x = vec.x - center.x;
                vec2.y = 0.0f;
                vec2.z = vec.z - center.z;
                if (vec2.abs() <= var_f29) {
                    bVar9 = false;
                }
            }

            if (bVar9) {
                cM3dGCyl cyl(&vec, var_f29, var_f28);
                cM3dGLin lin(center, eye);
                cXyz vec2, vec3, vec4;
                if (cM3d_Cross_CylLin(&cyl, &lin, &vec2, &vec3)) {
                    vec4.x = vec2.x - vec.x;
                    vec4.y = 0.0f;
                    vec4.z = vec2.z - vec.z;
                    vec4.normalize();
                    eye = vec2 + vec4 * 2.0f;
                    bVar8 = true;
                }
            }
        }
    }

    if (i_flags & 0x80) {
        dBgS_GndChk gnd_chk;
        gnd_chk.SetPos(&eye);
        cXyz vec1, vec2;
        if (dComIfG_Bgsp().GroundCross(&gnd_chk) < mBG.field_0x5c.field_0x58 + 40.0f) {
            vec1 = eye;
            vec2 = center;
            vec2 += (vec1 - vec2) * 0.5f;
        } else {
            vec2 = eye;
            vec1 = center;
            vec1 += (vec2 - vec1) * 0.5f;
        }
        if (mBG.field_0xc0.field_0x44 && (lineCollisionCheckBush(&vec1, &vec2) & 6) != 0) {
            cXyz mass_cam_top_pos;
            dComIfG_Ccsp()->GetMassCamTopPos(&mass_cam_top_pos);
            if (eye.y < mass_cam_top_pos.y) {
                eye.y = mEye.y + (mass_cam_top_pos.y - mEye.y) * 0.2f;
                bVar8 = true;
            }
        }
    } else if (i_flags & 0xb7) {
        f32 fVar1 = eye.y - mBG.field_0x0.field_0x58;
        if (fVar1 >= 0.0f && fVar1 < 3.0f) {
            eye.y = mBG.field_0x0.field_0x58 + 3.0f;
            bVar8 = true;
        } else if (fVar1 <= 0.0f && fVar1 > -3.0f) {
            eye.y = mBG.field_0x0.field_0x58 - 3.0f;
            bVar8 = true;
        }
    }

    mCenter = center;
    mEye = eye;
    if (bVar8) {
        direction.Val(eye - center);
    }
    mDirection = direction;
    return uVar13 != 0 ? true : false;
}

bool dCamera_c::lineBGCheckBoth(cXyz* i_start, cXyz* i_end, dBgS_LinChk* i_linchk,
                                u32 i_flags) {
    i_linchk->OnBackFlag();
    i_linchk->OnFrontFlag();
    return lineBGCheck(i_start, i_end, i_linchk, i_flags);
}

f32 dCamera_c::radiusActorInSight(fopAc_ac_c* param_0, fopAc_ac_c* param_1,
                                       fopAc_ac_c* param_2) {
    f32 var_f31 = radiusActorInSight(param_0, param_1, &mViewCache.mCenter, &mViewCache.mEye, mFovy, mBank, 0.05f);
    f32 var_f30 = radiusActorInSight(param_0, param_2, &mViewCache.mCenter, &mViewCache.mEye, mFovy, mBank, 0.05f);

    f32 var_f29;
    if (var_f31 > var_f30) {
        var_f29 = var_f31;
    } else {
        var_f29 = var_f30;
    }

    return var_f29;
}

BOOL dCamera_c::jutOutCheck(cXyz* param_0, f32 param_1) {
    cXyz spB4 = attentionPos(mpPlayerActor);
    spB4 += (positionOf(mpPlayerActor) - spB4) * 0.5f;

    dBgS_CamLinChk linchk;
    if (lineBGCheck(&spB4, param_0, &linchk, 0x40B7)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(linchk, &plane);

        *param_0 = linchk.GetCross();
        *param_0 += *plane.GetNP() * param_1;
        return TRUE;
    }

    return FALSE;
}

void dCamera_c::tooNearEscape(cXyz* param_0) {
    cXyz sp2C = attentionPos(mpPlayerActor);
    cSGlobe sp58(*param_0 - sp2C);

    if (sp58.R() < 40.0f) {
        sp58.R(40.0f);
        *param_0 = sp2C + sp58.Xyz();
    }
}

f32 dCamera_c::getWaterSurfaceHeight(cXyz* param_0) {
    f32 var_f31 = -G_CM3D_F_INF;

    cXyz spF8(*param_0);
    dBgS_RoofChk roofchk;
    roofchk.SetUnderwaterRoof();
    roofchk.SetPos(spF8);

    f32 roof_y = dComIfG_Bgsp().RoofChk(&roofchk);
    if (spF8.y < roof_y) {
        spF8.y = roof_y;
    }

    dBgS_CamGndChk_Wtr gndchk;
    gndchk.SetPos(&spF8);

    f32 gnd_y = dComIfG_Bgsp().GroundCross(&gndchk);
    if (gnd_y > param_0->y) {
        var_f31 = gnd_y;
    }

    return var_f31;
}

void dCamera_c::checkGroundInfo() {
    daAlink_c* player = (daAlink_c*)mpPlayerActor;
    cXyz gnd_chk_pos = positionOf(mpPlayerActor);
    if (check_owner_action(mPadID, 0x8000000)) {
        gnd_chk_pos = eyePos(mpPlayerActor);
        gnd_chk_pos.y = positionOf(mpPlayerActor).y;
    }

    cXyz roof_chk_pos = gnd_chk_pos;
    gnd_chk_pos.y += 20.0f;
    dBgS_RoofChk roof_chk;
    roof_chk.SetUnderwaterRoof();
    roof_chk.SetPos(roof_chk_pos);
    f32 roof_y = dComIfG_Bgsp().RoofChk(&roof_chk);
    if (roof_chk_pos.y < roof_y) {
        roof_chk_pos.y = roof_y;
    }

    dBgS_CamGndChk gnd_chk;
    gnd_chk.ClrCam();
    gnd_chk.SetObj();
    gnd_chk.SetPos(&gnd_chk_pos);
    f32 ground_y = dComIfG_Bgsp().GroundCross(&gnd_chk);

    mBG.field_0x5c.field_0x4.SetCam();
    mBG.field_0x5c.field_0x4.ClrObj();
    mBG.field_0x5c.field_0x4.SetPos(&gnd_chk_pos);
    mBG.field_0x5c.field_0x58 = dComIfG_Bgsp().GroundCross(&mBG.field_0x5c.field_0x4);
    if (mBG.field_0x5c.field_0x58 < ground_y) {
        mBG.field_0x5c.field_0x58 = ground_y;
        mBG.field_0x5c.field_0x4 = gnd_chk;
    }
    mBG.field_0x5c.field_0x0 = mBG.field_0x5c.field_0x58 != -1.0e9f;

    mBG.field_0x0.field_0x4.SetPos(&roof_chk_pos);
    mBG.field_0x0.field_0x58 = dComIfG_Bgsp().GroundCross(&mBG.field_0x0.field_0x4);
    mBG.field_0x0.field_0x0 = mBG.field_0x0.field_0x58 != -1.0e9f;

    if (check_owner_action(mPadID, 0x100000)
        && mBG.field_0x0.field_0x58 < attentionPos(mpPlayerActor).y + 40.0f)
    {
        setComStat(0x800);
        mBG.field_0xc0.field_0x44 = 1;
    } else if (player->checkRide() || player->checkRoofSwitchHang() || player->checkWolfRope()) {
        mBG.field_0xc0.field_0x44 = 1;
    } else if (check_owner_action1(mPadID, 0x2110000)) {
        mBG.field_0xc0.field_0x44 = 1;
    } else if (player->checkSpinnerRide()) {
        mBG.field_0xc0.field_0x44 = 1;
    } else if (player->checkMagneBootsOn()) {
        if (!cBgW_CheckBWall(player->getMagneBootsTopVec()->y)) {
            mBG.field_0xc0.field_0x44 = 1;
        }
    } else if (footHeightOf(mpPlayerActor) - mBG.field_0x5c.field_0x58 > mCamSetup.mBGChk.FloorMargin()) {
        mBG.field_0xc0.field_0x44 = 0;
    } else {
        mBG.field_0xc0.field_0x44 = 1;
    }

    mBG.field_0xc0.field_0x1 = 0;
    mBG.field_0xc0.field_0x20 = NULL;

    if (dComIfG_Bgsp().ChkMoveBG(mBG.field_0x5c.field_0x4)) {
        mBG.field_0xc0.field_0x20 = dComIfG_Bgsp().GetActorPointer(mBG.field_0x5c.field_0x4);
        if (mBG.field_0xc0.field_0x20 != NULL) {
            cXyz pos = positionOf(mBG.field_0xc0.field_0x20);
            cSAngle angle = directionOf(mBG.field_0xc0.field_0x20);
            if (mBG.field_0xc0.field_0x0 != 0) {
                mBG.field_0xc0.field_0x4 = mBG.field_0xc0.field_0x10 - pos;
                mBG.field_0xc0.field_0x1c = mBG.field_0xc0.field_0x1e - angle;
            }
            mBG.field_0xc0.field_0x0 = 1;
            if (!dComIfGp_evmng_cameraPlay() && !chkFlag(0x20000000) && mBG.field_0xc0.field_0x44 != 0) {
                mBG.field_0xc0.field_0x1 = 1;
            }
            if (mBG.field_0xc0.field_0x1 != 0) {
                dComIfG_Bgsp().MoveBgMatrixCrrPos(mBG.field_0x5c.field_0x4, true,
                                                  &mViewCache.mCenter, NULL, NULL);
                dComIfG_Bgsp().MoveBgMatrixCrrPos(mBG.field_0x5c.field_0x4, true,
                                                  &mViewCache.mEye, NULL, NULL);
                mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
            }
            mBG.field_0xc0.field_0x10 = pos;
            mBG.field_0xc0.field_0x1e = angle;
        }
    } else {
        mBG.field_0xc0.field_0x0 = 0;
    }

    if (mBG.field_0x5c.field_0x0) {
        mBG.field_0xc0.field_0x34 = dComIfG_Bgsp().GetCamMoveBG(mBG.field_0x5c.field_0x4);
    } else {
        mBG.field_0xc0.field_0x34 = 0;
    }

    if (mBG.field_0x0.field_0x0 && check_owner_action(mPadID, 0x100000)) {
        mBG.field_0xc0.field_0x3c = dComIfG_Bgsp().GetRoomCamId(mBG.field_0x0.field_0x4);
    } else {
        mBG.field_0xc0.field_0x3c = 0xff;
    }

    if (mBG.field_0xc0.field_0x44 == 0 && mFrameCounter != 0 && !check_owner_action(mPadID, 0x100000)) {
        mBG.field_0xc0.field_0x40 = 0x1ff;
    } else if (mBG.field_0x5c.field_0x0) {
        mBG.field_0xc0.field_0x40 = dComIfG_Bgsp().GetRoomCamId(mBG.field_0x5c.field_0x4);
    } else {
        mBG.field_0xc0.field_0x40 = 0xff;
    }
}

bool dCamera_c::chaseCamera(s32 param_0) {
    static f32 JumpCushion = 0.9f;
    f32 charge_latitude = mCamSetup.ChargeLatitude();
    int charge_timer = mCamSetup.ChargeTimer();
    f32 charge_b_ratio = mCamSetup.ChargeBRatio();
    static cSAngle LatitudeLimitMax = 80.0f;
    daAlink_c* player = (daAlink_c*)mpPlayerActor;
    daMidna_c* midna = daPy_py_c::getMidnaActor();

    if (dComIfGp_getAttention()->GetCheckObjectCount() != 0) {
        mpAuxTargetActor1 = dComIfGp_getAttention()->CheckObjectTarget(0);
        setFlag(2);
    }

    f32 val0 = mCamParam.Val(param_0, 0);
    f32 val2 = mCamParam.Val(param_0, 2);
    f32 val1 = mCamParam.Val(param_0, 1);
    f32 val3 = mCamParam.Val(param_0, 3);
    f32 val4 = mCamParam.Val(param_0, 4);
    f32 val5 = mCamParam.Val(param_0, 5);
    f32 val6 = mCamParam.Val(param_0, 6);
    f32 val7 = mCamParam.Val(param_0, 7);
    f32 val8 = mCamParam.Val(param_0, 8);
    f32 val9 = mCamParam.Val(param_0, 9);
    f32 val10 = mCamParam.Val(param_0, 10);
    f32 val11 = mCamParam.Val(param_0, 11);
    f32 val12 = mCamParam.Val(param_0, 12);
    f32 val13 = mCamParam.Val(param_0, 13);
    f32 val14 = mCamParam.Val(param_0, 14);
    f32 val16 = mCamParam.Val(param_0, 16);
    f32 val15 = mCamParam.Val(param_0, 15);
    f32 val17 = mCamParam.Val(param_0, 17);
    f32 val19 = mCamParam.Val(param_0, 19);
    f32 val18 = mCamParam.Val(param_0, 18);
    f32 val21 = mCamParam.Val(param_0, 21);
    f32 val22 = mCamParam.Val(param_0, 22);
    f32 val23 = mCamParam.Val(param_0, 23);
    f32 val24 = mCamParam.Val(param_0, 24);

    cSAngle ang1 = -75.0f;
    cSAngle ang2 = 75.0f;
    f32 dVar52 = 0.25f;

    ChaseData* chase = (ChaseData*)mWork;

    if (mCamParam.CheckFlag(1)) {
        setFlag(0x20000);
    }

    if (player->checkSpinnerPathMove() || mCurType == specialType[CAM_TYPE_LV4_BOSS_SPJP]) {
        mPadInfo.mMainStick.mLastPosX = 0.0f;
        mPadInfo.mMainStick.mLastPosY = 0.0f;
        mPadInfo.mMainStick.mLastValue = 0.0f;
    }

    if (mCurCamStyleTimer == 0) {
        cXyz player_attention = attentionPos(mpPlayerActor) - positionOf(mpPlayerActor);
        if (chkFlag(0x200) && mRecovery.field_0x8.field_0x1e <= 0) {
            mViewCache.mCenter = mCenter;
            mViewCache.mEye = mEye;
            mViewCache.mDirection = mDirection;
            mViewCache.mFovy = mFovy;
        }
        chase->field_0x40 = 0.0f;
        chase->field_0x71 = false;
        chase->field_0x96 = directionOf(mpPlayerActor).Inv();
        chase->field_0x93 = false;
        chase->field_0x73 = false;
        chase->field_0x9c = 0xfe;
        chase->field_0x18 = 0;
        chase->field_0xa0 = mGear;
        chase->field_0x94 = false;
        chase->field_0xa4 = 0.0f;
        chase->field_0x24 = 0.0f;
        chase->field_0x98 = directionOf(mpPlayerActor);
        if (chkFlag(0x100000)) {
            setUSOAngle();
        }
        if (mCamParam.Flag(param_0, 0x200)) {
            switch (mRoomMapTool.mFlags & 3) {
            case 1:
                mGear = 0;
                break;
            case 2:
                mGear = 1;
                break;
            }
        }
    }

    bool bVar6 = false;
    if (check_owner_action(mPadID, 0x100000) && mBG.field_0xc0.field_0x44 == 0) {
        bVar6 = true;
    }
    cSAngle player_direction = directionOf(mpPlayerActor);
    bool bVar12 = false;
    bool bVar2 = false;
    bool bVar7 = false;
    if (check_owner_action(mPadID, 0x300)) {
        bVar12 = true;
    }
    if (check_owner_action(mPadID, 0x2000008)) {
        bVar2 = true;
    }
    if (player->checkMagneBootsOn()) {
        bVar7 = true;
        if (cBgW_CheckBWall(player->getMagneBootsTopVec()->y)) {
            bVar2 = true;
        }
    }
    cSAngle player_angle_delta = player_direction - mViewCache.mDirection.U();
    field_0x944 = 1;

    if (mRoomMapTool.mCameraIndex != 0xff && mCamParam.Flag(param_0, 0x200)) {
        if (mRoomMapTool.mCameraIndex != chase->field_0x9c) {
            mCamParam.Fovy(mRoomMapTool.mCamData.field_0x11);
            mCamParam.Arg0(mRoomMapTool.mCamData.field_0x12);
            mCamParam.Arg1(mRoomMapTool.mCamData.field_0x13);
            mCamParam.Arg2(mRoomMapTool.mCamData.field_0x14);
            if (param_0 == mCamTypeData[mCurType].field_0x18[mIsWolf][mCurMode]) {
                mCamParam.Change(param_0);
            }
            chase->field_0x9c = mRoomMapTool.mCameraIndex;
        }

        if (mCamParam.Fovy() != 0xff) {
            val19 = (s8)mCamParam.Fovy();
        }
        if (mCamParam.Arg0() != 0xff) {
            val9 = (s8)mCamParam.Arg0() * 100.0f;
        }
        if (mCamParam.Arg1() != 0xff) {
            val14 = (s8)mCamParam.Arg1();
        }
        if (mCamParam.Arg2() != -1) {
            val4 = mCamParam.Arg2();
        }

    } else if (mRoomMapTool.mCameraIndex == 0xff && chase->field_0x9c != 0xff) {
        mCamParam.Fovy(0xff);
        mCamParam.Arg0(0xff);
        mCamParam.Arg1(0xff);
        mCamParam.Arg2(-1);
        if (param_0 == mCamTypeData[mCurType].field_0x18[mIsWolf][mCurMode]) {
            mCamParam.Change(param_0);
        }
        chase->field_0x9c = 0xff;
    }

    if (mGear == 1) {
        val7 = val9;
        val8 = val9 - val9 * 0.2f;
        val2 = val3 = val4;
        val12= val13 = val14;
        val17 = val18 = val19;
    }

    bool bVar10, bVar8;
    bool bVar9 = false;
    bool bVar3 = false;
    bool bVar1 = false;
    if (mIsWolf == 0) {
        if (isPlayerCharging(mPadID)) {
            bVar1 = true;
        }

        if (check_owner_action(mPadID, 0x8000000)) {
            bVar3 = true;
        }

        if (check_owner_action1(mPadID, 0x2000000)) {
            clrFlag(0x80);
            cXyz* left_foot_pos = player->getLeftFootPosP();
            cXyz* right_foot_pos = player->getRightFootPosP();
            if (left_foot_pos->y < right_foot_pos->y) {
                val0 = -val0;
            }
        } else if (mThrowTimer != 0 && !chkFlag(0x100000)) {
            f32 throw_ctr_adjust = mCamSetup.ThrowCtrAdjust();
            val2 += throw_ctr_adjust;
            val3 += throw_ctr_adjust;
            f32 throw_v_angle = mCamSetup.ThrowVAngle();
            if (val12 < throw_v_angle) {
                val12 = throw_v_angle;
            }
            if (val13 < throw_v_angle) {
                val13 = throw_v_angle;
            }
            f32 throw_cushion = mCamSetup.ThrowCushion();
            val16 = throw_cushion;
            val5 = throw_cushion;
            mForwardTiltOffset = cSAngle::_0;
            chase->field_0x94 = true;
        } else if ((player->getGrabActorID() != fpcM_ERROR_PROCESS_ID_e || mThrowTimer != 0)
                                                && chkFlag(0x100000)) {
            val9 = mCamSetup.ThrowCtrAdjust();
            if (mThrowTimer != 0) {
                val9 *= (f32)mThrowTimer / mCamSetup.ThrowTimer();
            }
            val2 += val9;
            val3 += val9;
        } else if (player->checkCanoeRideTandem()) {
            val1 -= 100.0f;
        } else if (check_owner_action(mPadID, 0x100000)) {
            val9 = mMonitor.field_0x10 / 22.0f;
            if (val9 > 1.0f) {
                val9 = 1.0f;
            }
            val1 *= val9;
        }

        if (chase->field_0x94) {
            if (push_any_key()) {
                chase->field_0x94 = false;
            }
            if (mThrowTimer == 0) {
                bVar9 = true;
            }
        } else if (chase->field_0x92 && chase->field_0x1a > 14) {
            f32 min = 550.0f;
            if (val8 < min) {
                val8 = min;
            }
            min *= 1.1f;
            if (val7 < min) {
                val7 = min;
            }
        }

    } else {
        if (isPlayerCharging(mPadID)) {
            bVar1 = true;
        }

        if (mMidnaRidingAndVisible != 0) {
            val2 += 20.0f;
            val3 += 20.0f;
        }

        if (check_owner_action1(mPadID, 0x1000000)) {
            chase->field_0x73 = true;
            chase->field_0x18 = 20;
        } else if (chase->field_0x18 != 0) {
            chase->field_0x73 = true;
            chase->field_0x18--;
        } else {
            chase->field_0x73 = false;
        }

        if ((check_owner_action1(mPadID, 0x800000) || chase->field_0x73) && !chkFlag(0x100000)) {
            val2 = val3 = val1 = -70.0f;
            bVar1 = true;
            f32 search_ball_scale = player->getSearchBallScale();
            search_ball_scale *= 1.5f;
            if (val8 < search_ball_scale) {
                val8 = search_ball_scale;
            }
            search_ball_scale *= 1.1f;
            if (val7 < search_ball_scale) {
                val7 = search_ball_scale;
            }
            if (check_owner_action1(mPadID, 0x800000) && !chase->field_0x73) {
                val10 = 1.0f;
                val12 = val13 = 40.0f;
            } else {
                val8 = val7 * 0.75f;
                val12 = 30.0f;
                val16 = val13 = 1.0f;
            }
            val17 = val18 = 90.0f / mWindowAspect;
        }

        if (chase->field_0x73) {
            val22 = 0.0001f;
            val16 = 0.0001f;
            val10 = 0.0001f;
            val21 = 0.0001f;
        }

        if (mCurMode == 1 && val8 < 300.0f && val7 > 300.0f) {
            val8 = 300.0f;
        }
    }

    dAttention_c* attention = dComIfGp_getAttention();

    if (mGear == 1 && !mCamParam.Flag(param_0, 0x20)) {
        mForwardTiltOffset = cSAngle::_0;
    }

    if (mCurMode == 1) {
        val16 = 0.25f;
    }

    bool bVar33 = false;
    if (check_owner_action1(mPadID, 0x10000)) {
        cXyz hs_sub_chain_top_pos = daAlink_getAlinkActorClass()->getHsSubChainTopPos();
        if (hs_sub_chain_top_pos.y - attentionPos(mpPlayerActor).y > 100.0f) {
            bVar33 = true;
        }
    }

    if (check_owner_action(mPadID, 0x100)) {
        val7 -= 50.0f;
        val8 -= 50.0f;
        if (mIsWolf == 1) {
            val1 = 1.0f;
        } else {
            val1 = -30.0f;
        }
        ang1.Val(-80.0f);
        ang2.Val(80.0f);
        val2 = 40.0f;
        val3 = 30.0f;
        if (player_angle_delta > cSAngle::_270 && player_angle_delta < cSAngle::_90) {
            val12 = 30.0f;
            val13 = 30.0f;
        } else {
            if (val12 < 10.0f) {
                val12 = 10.0f;
            }
            if (val13 < 10.0f) {
                val13 = 10.0f;
            }
        }
        val6 = 0.05f;
        val16 = 0.15f;
    } else if (check_owner_action(mPadID, 0x2000208)) {
        val1 -= 50.0f;
        val6 = 0.25f;
        val7 -= 50.0f;
        val8 -= 50.0f;
        ang1.Val(-60.0f);
        ang2.Val(60.0f);
        if (val2 > 30.0f) {
            val2 = 30.0f;
        }
        if (val3 > 30.0f) {
            val3 = 30.0f;
        }
        mForwardTiltOffset = cSAngle::_0;
        val16 = 0.15f;
    } else if (bVar33) {
        mForwardTiltOffset = cSAngle::_0;
        mBG.field_0xc0.field_0x44 = 0;
    }

    if (mIsWolf == 1 || check_owner_action(mPadID, 0x8000000)) {
        if (val2 < -30.0f) {
            val2 = -30.0f;
        }
        if (val3 < -30.0f) {
            val3 = -30.0f;
        }
    }

    cXyz player_attention_pos = attentionPos(mpPlayerActor);
    lineBGCheck(&player_attention_pos, &mViewCache.mEye, 0x40b7);

    if (mCurCamStyleTimer == 0) {
        chase->field_0x38 = val0;
    } else if (chkFlag(0x100000) && !check_owner_action1(mPadID, 0x2000000)
                                 && check_owner_action(mPadID, 0xa50c0)) {
        if (mPadInfo.mMainStick.mLastPosX < -0.2f) {
            chase->field_0x71 = true;
        }
        if (mPadInfo.mMainStick.mLastPosX > 0.2f) {
            chase->field_0x71 = false;
        }

        f32 tmp;
        if (chase->field_0x71) {
            tmp = -45.0f;
        } else {
            tmp = 45.0f;
        }
        chase->field_0x38 += (tmp - chase->field_0x38) * 0.04f;
    } else {
        chase->field_0x38 += (val0 - chase->field_0x38) * 0.06f;
    }

    if (mGear == 1 || val7 <= val8) {
        chase->field_0xa4 = 0.0f;
    } else if (!mCamParam.Flag(param_0, 0x400)) {
        val14 = mViewCache.mDirection.R();
        if (val14 < val8) {
            val9 = val8;
        } else if (val14 > val7) {
            val9 = val7;
        } else {
            val9 = val14;
        }

        f32 tmp = (val7 - val9) / (val7 - val8);
        if (mCurCamStyleTimer == 0) {
            chase->field_0xa4 = tmp;
        } else {
            chase->field_0xa4 += (tmp - chase->field_0xa4) * 0.75f;
        }
    }

    f32 tmp = val2 + chase->field_0xa4 * (val3 - val2);
    if (mCurCamStyleTimer == 0) {
        chase->field_0x3c = tmp;
    } else {
        chase->field_0x3c += (tmp - chase->field_0x3c) * 0.08f;
    }

    cXyz vec(chase->field_0x38, chase->field_0x3c, val1);

    if (mCurCamStyleTimer == 0) {
        mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
        chase->field_0x0 = 'CHAS';
        if (mBG.field_0xc0.field_0x44 != 0 && !check_owner_action(mPadID, 0x200)) {
            chase->field_0x20 = 0.0f;
        } else {
            chase->field_0x20 = 1.0f;
        }
        chase->field_0x10 = 30;
        chase->field_0x28 = val8;
        chase->field_0x2c = val7;
        chase->field_0x18 = 0;
        chase->field_0x1a = 0;
        chase->field_0x14 = 0;
        chase->field_0x54 = 0.01f;
        chase->field_0x7c = 0.01f;
        chase->field_0x78 = 0.01f;
        chase->field_0x80 = 0.01f;
        chase->field_0x74 = 0.01f;
        chase->field_0x84 = val6;
        chase->field_0x88 = val5;
        chase->field_0x44 = 0;
        chase->field_0x70 = true;
        chase->field_0x34 = mViewCache.mFovy;
        chase->field_0x48 = 0.0f;
        chase->field_0x72 = false;
        chase->field_0x91 = false;
        chase->field_0x92 = false;
        chase->field_0x30 = positionOf(mpPlayerActor).y;
        chase->field_0x95 = false;
        chase->field_0x4c = 1.0f;
        chase->field_0xa8 = 0;

        if (dComIfGp_evmng_cameraPlay()) {
            int timer;
            getEvIntData(&timer, "Timer", 20);
            chase->field_0x4 = timer != 0 ? timer : 1;
            chase->field_0x95 = true;

        } else {
            cXyz pos = relationalPos(mpPlayerActor, &vec);
            cSGlobe globe = mDirection.Invert();
            globe.R(100000.0f);
            cXyz pos2 = mEye + globe.Xyz();
            cM3dGLin lin(pos2, mEye);
            f32 dist;
            if (cM3d_Len3dSqPntAndSegLine(&lin, &pos, &pos2, &dist)) {
                if (mRecovery.field_0x8.field_0x1e <= 0) {
                    mCenter = pos2;
                    mDirection.Val(mEye - mCenter);
                }
                mViewCache.mCenter = pos2;
                mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
            }

            cSAngle ang;
            if ((chkFlag(0x100000) || bVar3 || bVar6) && std::fabsf(mPadInfo.mCStick.mLastPosX) < 0.05f) {
                ang.Val(directionOf(mpPlayerActor).Inv());
            } else {
                ang.Val(mViewCache.mDirection.U());
            }
            cSGlobe globe2((val8 + val7) * 0.5f, cSAngle((val12 + val13) * 0.5f), ang);
            cXyz pos3 = pos + globe2.Xyz();
            f32 dist1 = cXyz(mEye - pos3).abs() * 2.5f;

            dist = val3 + (val2 - val3) * chase->field_0xa4;
            pos3 = pos;
            pos.y += dist;
            f32 dist2 = cXyz(mCenter - pos3).abs() * 8.0f;
            f32 abs_dist_max = std::fabsf(dist1 > dist2 ? dist1 : dist2);

            f32 tmp2 = val18 + (val17 - val18) * chase->field_0xa4;
            f32 fovy_diff = std::fabsf(mFovy - tmp2);
            f32 sin = cSAngle(mFovy > tmp2 ? mFovy : tmp2).Sin();
            f32 tmp3 = 100.0f * (sin * sin) * fovy_diff;
            f32 tmp5;
            if (abs_dist_max > tmp3) {
                tmp5 = abs_dist_max;
            } else {
                tmp5 = tmp3;
            }
            f32 tmp6 = std::fabsf(tmp5);
            tmp6 *= 1.2f;
            tmp6 *= 0.00625f;
            val3 = JMAFastSqrt(tmp6);
            chase->field_0x4 = (int)(val3 * 2.2f) + 1;
            if (chase->field_0x4 < 5) {
                chase->field_0x4 = 5;
            }
        }

        if (player->checkGoronSideMove()) {
            chase->field_0x4 = 20;
        } else if (mCurType == specialType[CAM_TYPE_LV9_GZELDA_TRI]) {
            chase->field_0x4 = 45;
        } else if (chkFlag(0x20)) {
            int tmp2 = chase->field_0x4;
            int tmp = tmp2 / 6;
            chase->field_0x4 = tmp2 + (tmp > 15 ? 15 : tmp < 5 ? 5 : tmp);
        } else if (player->checkMagneBootsOn()) {
            cXyz* magne_boots_top_vec = player->getMagneBootsTopVec();
            cXyz vec2 = mDirection.Xyz().norm();
            if (VECDotProduct(magne_boots_top_vec, &vec2) < -0.1f) {
                vec2 = (vec2 + *magne_boots_top_vec) * 0.5f;
                mViewCache.mDirection.Val(vec2);
                mViewCache.mDirection.R(val8);
                mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
                chase->field_0x4 = 1;
                mStyleSettle.mFinished = true;
            }
        } else if (Stage == 0x6a && player->checkCameraLargeDamage()) {
            mViewCache.mCenter = relationalPos(mpPlayerActor, &vec);
            mViewCache.mDirection.Val(val8, cSAngle(val13), mDirection.U());
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
            mViewCache.mFovy = val18;
            mStyleSettle.mFinished = true;
            chase->field_0x4 = 1;
        } else if (mCurMode == 0 && !chase->field_0x95 && chase->field_0x4 < 10 && !chkFlag(0x50000)
                                                 && (chkFlag(0x8000) || field_0x168 == 0)) {
            mStyleSettle.mFinished = true;
            chase->field_0x4 = 1;
        }

        chase->field_0x50 = mViewCache.mDirection.V().Degree();
        chase->field_0x58 = mViewCache.mCenter;
        chase->field_0x64 = mViewCache.mEye;
        chase->field_0x34 = mViewCache.mFovy;
        if (chkFlag(0x200)) {
            f32 dist = mViewCache.mDirection.R();
            chase->field_0x2c = dist;
            chase->field_0x28 = dist;
        }
        chase->field_0x8 = chase->field_0x4 * (chase->field_0x4 + 1) >> 1;
        chase->field_0xc = 0.0f;
        chase->field_0x1c = 0;
        chase->field_0x8c = 0;
        chase->field_0x90 = false;
    }

    cXyz pos = relationalPos(mpPlayerActor, &vec);

    fopAc_ac_c* copy_rod_camera = player->getCopyRodCameraActor();
    if (copy_rod_camera != NULL) {
        cXyz attention_pos = attentionPos(mpPlayerActor);
        if (!lineBGCheck(&attention_pos, &mViewCache.mEye, 0x40b7)) {
            mpAuxTargetActor1 = copy_rod_camera;
            setFlag(0x2000);
        }
    }

    if (player->checkHorseRide()) {
        pos.y -= 100.0f;
    }
    jutOutCheck(&pos, 5.0f);

    cXyz vec2(0.0f, 0.0f, mMonitor.field_0xc * 10.0f);
    cXyz vec3 = attentionPos(mpPlayerActor) + dCamMath::xyzRotateY(vec2, player_direction);
    dBgS_CamLinChk lin_chk;
    if (lineBGCheck(&mViewCache.mEye, &vec3, &lin_chk, 0x40b7)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
        vec3 = lin_chk.GetCross();
        vec3 += *plane.GetNP();
        val22 *= 3.0f;
    }

    if (mStyleSettle.mFinished == 0) {
        chase->field_0x96 = player_direction.Inv();
        if (mBG.field_0xc0.field_0x1 != 0) {
            dComIfG_Bgsp().MoveBgMatrixCrrPos(mBG.field_0x5c.field_0x4, true,
                                              &chase->field_0x58, NULL, NULL);
        }
        chase->field_0xc = chase->field_0x4 - (int)mCurCamStyleTimer;
        f32 rate = chase->field_0xc / chase->field_0x8;
        chase->field_0x58 = pos;
        if (chase->field_0x95) {
            rate = 0.75f;
            if (push_any_key() && mMonitor.field_0xc < 0.1f && mCurCamStyleTimer > 20) {
                mStyleSettle.mFinished = true;
            }
        }
        mViewCache.mCenter += (pos - mViewCache.mCenter) * rate;

        f32 dist = dCamMath::xyzHorizontalDistance(pos, chase->field_0x58);
        if (dist < std::fabsf(vec.x > vec.z ? vec.x : vec.z) + 20.0f) {
            cXyz attention_pos = attentionPos(mpPlayerActor);
            attention_pos.y -= 15.0f;
            dBgS_CamLinChk lin_chk;
            if (lineBGCheck(&attention_pos, &mViewCache.mCenter, &lin_chk, 0x40b7)) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                mViewCache.mCenter = lin_chk.GetCross();
                mViewCache.mCenter += *plane.GetNP();
            }
        }

        f32 dVar43 = limitf(mViewCache.mDirection.R(), val8, val7);
        cSAngle ang = val12 + chase->field_0xa4 * (val13 - val12);
        if (!chase->field_0x95) {
            ang += mForwardTiltOffset;
        }
        chase->field_0x50 = ang.Degree();
        cSGlobe globe(dVar43, ang, cSAngle(mControlledYaw.Inv()));
        mViewCache.mDirection.R(mViewCache.mDirection.R() + (globe.R() - mViewCache.mDirection.R()) * rate);
        mViewCache.mDirection.V(mViewCache.mDirection.V() + (globe.V() - mViewCache.mDirection.V()) * rate);
        if (chkFlag(0x100000) || bVar3 || bVar6) {
            mViewCache.mDirection.U(mViewCache.mDirection.U() + (chase->field_0x96 - mViewCache.mDirection.U()) * rate);
        }
        chase->field_0x64 = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mEye = chase->field_0x64;

        if (mCurCamStyleTimer >= chase->field_0x4 - 1) {
            mStyleSettle.mFinished = true;
        }

        chase->field_0x28 = chase->field_0x2c = mViewCache.mDirection.R();
        mViewCache.mFovy += (val17 + chase->field_0xa4 * (val18 - val17) - mViewCache.mFovy) * rate;
        chase->field_0x8 -= chase->field_0xc;
        chase->field_0x8c = 0;
        chase->field_0x90 = false;
        return true;
    }

    chase->field_0x95 = false;
    if (mCStickYHoldCount == 1 && chase->field_0xa0 != mGear) {
        chase->field_0x1c = 20;
        chase->field_0xa0 = mGear;
    }
    if (chase->field_0x1c != 0) {
        dVar52 = dCamMath::rationalBezierRatio(1.0f - (chase->field_0x1c - 1) / 20.0f, 0.5f);
    }

    if (player->checkThrowDamage()) {
        chase->field_0x91 = true;
        fopAc_ac_c* target = dComIfGp_getAttention()->LockonTarget(0);
        if (target != NULL && fopAcM_GetName(target) == PROC_E_HZ) {
            setFlag(0x2000);
            mpAuxTargetActor1 = target;
        }
    } else {
        chase->field_0x91 = false;
    }

    bVar10 = false;
    bool bVar11 = false;
    bool bVar34 = false;
    cXyz player_pos = positionOf(mpPlayerActor);
    player_pos.y += 10.0f;
    groundHeight(&player_pos);
    footHeightOf(mpPlayerActor);

    if (mBG.field_0xc0.field_0x44 != 0 && !check_owner_action(mPadID, 0x200)) {
        chase->field_0x20 = 0.0f;
        chase->field_0x10 = 0;
    } else if (chase->field_0x10 < 30) {
        bVar10 = true;
        chase->field_0x10++;
        chase->field_0x20 += (JumpCushion - chase->field_0x20) * dCamMath::rationalBezierRatio(chase->field_0x10 / 30.0f, 1.25f);
    } else if (!bVar2) {
        bVar11 = true;
        if (mBG.field_0x5c.field_0x0 && dComIfG_Bgsp().GetGroundCode(mBG.field_0x5c.field_0x4) == 4) {
            bVar34 = true;
        } else if (player->checkCokkoGlide()) {
            bVar10 = true;
            bVar11 = false;
        }
        val10 = 0.01f;
        chase->field_0x20 += (1.0f - chase->field_0x20) * 0.1f;
    }

    if (check_owner_action(mPadID, 0x2800008)) {
        if (mFakeAngleSys.field_0x0 == 0) {
            setUSOAngle();
            chase->field_0x98 = directionOf(mpPlayerActor);
        } else {
            cSAngle ang = chase->field_0x98 - directionOf(mpPlayerActor);
            if (ang.Abs() > cSAngle(50.0f)) {
                mFakeAngleSys.field_0x0 = 0;
            }
        }
    }

    if (chase->field_0x91) {
        chase->field_0x84 = 0.35f;
    } else if (check_owner_action(mPadID, 0x2000108)
        && !(player_angle_delta > cSAngle::_270 && player_angle_delta < cSAngle::_90))
    {
        chase->field_0x84 = 0.1f;
    } else {
        chase->field_0x84 += (val6 - chase->field_0x84) * 0.4f;
    }

    if (chase->field_0x91) {
        chase->field_0x88 = 0.25f;
    } else if (bVar10 || bVar11) {
        chase->field_0x88 = val5 * 0.1f + (0.9f - val5 * 0.1f) * chase->field_0x20;
        if (chkFlag(0x100000) && chase->field_0x84 > 0.25f) {
            chase->field_0x84 = chase->field_0x20 * 0.75f + 0.25f;
        }
    } else if (chase->field_0x1c != 0) {
        chase->field_0x88 = dVar52;
    } else if (bVar6 && mDoCPd_c::getTrigA(mPadID)) {
        chase->field_0x88 = 0.01f;
    } else if (bVar6) {
        chase->field_0x88 += (val5 - chase->field_0x88) * 0.1f;
    } else {
        chase->field_0x88 += (val5 - chase->field_0x88) * 0.4f;
    }

    cXyz vec4(chase->field_0x84, chase->field_0x88, chase->field_0x84);

    bVar8 = false;
    bool bVar5 = false;
    if (chkFlag(0x80) && mDirection.R() < val8) {
        bVar5 = true;
    }

    if (!chkFlag(0x100000) && !check_owner_action(mPadID, 0x2800108)) {
        isPlayerCharging(mPadID);
    }

    if (bVar5 && mMonitor.field_0xc < 0.1f && !chkFlag(0x100000)
        && !check_owner_action(mPadID, 0x2800108) && !check_owner_action1(mPadID, 0x2110000))
    {
        bVar8 = true;
    }

    if (mCamParam.Flag(param_0, 0x1000)) {
        f32 fVar55 = 0.0f;
        if (push_any_key()) {
            chase->field_0x8c = 0;
            chase->field_0x90 = false;
        } else if (chase->field_0x8c < mCamSetup.WaitRollTimer()) {
            chase->field_0x8c++;
        } else {
            fopAc_ac_c* target = attention->LockonTarget(0);
            if (target != NULL) {
                cXyz delta = positionOf(target) - positionOf(mpPlayerActor);
                cSAngle ang = -mViewCache.mDirection.U();
                delta = dCamMath::xyzRotateY(delta, ang);
                cXyz vec5 = delta;
                if (std::fabsf(vec5.y) < 200.0f) {
                    vec5.y = 0.0f;
                    vec5.x *= 0.5f;
                    f32 tmp = vec5.abs();
                    if (tmp < 500.0f) {
                        fVar55 = -delta.x * 0.55f * (1.0f - tmp / 500.0f);
                    }
                }
            }
        }

        chase->field_0x40 += (fVar55 - chase->field_0x40) * 0.01f;
        cSGlobe globe(chase->field_0x40, mViewCache.mDirection.V(), mViewCache.mDirection.U() + cSAngle::_270);
        pos += globe.Xyz();
    }

    mViewCache.mCenter += (pos - mViewCache.mCenter) * vec4;
    cSGlobe globe = mViewCache.mEye - mViewCache.mCenter;

    if (chase->field_0x1a > 0 && chase->field_0x1a <= 14) {
        f32 tmp = chase->field_0x1a / 14.0f;
        if (tmp > 1.0f) {
            tmp = 1.0f;
        }
        f32 rate = dCamMath::rationalBezierRatio(tmp, charge_b_ratio);
        chase->field_0x70 = true;
        chase->field_0x48 = (1.0f - chase->field_0x48) * rate;
    } else if (bVar6) {
        f32 target = 1.0f;
        f32 tmp = dCamMath::rationalBezierRatio(mPadInfo.mMainStick.mLastPosX, val23);
        if (!mDoCPd_c::getHoldA(mPadID)) {
            target = 1.0f - ((1.0f - val24) + val24 * cSAngle(tmp * 180.0f).Cos());
            target *= mPadInfo.mMainStick.mLastValue;
        }
        chase->field_0x24 += (target - chase->field_0x24) * 0.4f;
        if (chase->field_0x70) {
            chase->field_0x48 = 0.05f;
        }
        chase->field_0x70 = false;
        chase->field_0x48 += (val24 * chase->field_0x24 - chase->field_0x48) * 0.2f;
    } else if (chkFlag(0x100000) || bVar3) {
        if (chase->field_0x70) {
            chase->field_0x48 = 0.05f;
        }
        chase->field_0x70 = false;
        f32 tmp = 0.2f;
        chase->field_0x48 += (1.0f - chase->field_0x48) * tmp;
    } else {
        chase->field_0x70 = true;
        if (mFakeAngleSys.field_0x0 != 0) {
            chase->field_0x48 = 0.0f;
        } else if (mPadInfo.mMainStick.mLastPosY >= 0.0f) {
            f32 tmp = dCamMath::rationalBezierRatio(mPadInfo.mMainStick.mLastPosX, val23);
            chase->field_0x48 = 1.0f - (1.0f - val24 + val24 * cSAngle(tmp * 180.0f).Cos());
        } else {
            f32 tmp = dCamMath::rationalBezierRatio(mPadInfo.mMainStick.mLastPosX, val23 * 2.0f);
            chase->field_0x48 = 1.0f - (1.0f - val24 + val24 * cSAngle(tmp * 180.0f).Cos());
        }
        chase->field_0x48 *= mPadInfo.mMainStick.mLastValue;
        chase->field_0x48 *= val22;
        if (check_owner_action(mPadID, 0x2000008)) {
            if (chase->field_0x14 == 0) {
                if (check_owner_action(mPadID, 8)) {
                    chase->field_0x14 = 1;
                } else if (player_angle_delta > cSAngle::_270 && player_angle_delta < cSAngle::_90) {
                    chase->field_0x14 = 1;
                } else {
                    chase->field_0x14 = -1;
                }
            } else if (chase->field_0x14 < 0) {
                chase->field_0x14--;
            } else if (chase->field_0x14 < 30) {
                chase->field_0x48 = chase->field_0x14 / 30.0f * 0.5f;
                chase->field_0x14++;
            } else {
                chase->field_0x48 = 0.5f;
            }
        } else {
            chase->field_0x14 = 0;
        }
    }

    cSAngle ang3;
    f32 dVar39 = chase->field_0x48 * globe.V().Cos();

    if (chkFlag(0x80000)) {
        cXyz vec5 = mLastHitPos;
        cSGlobe globe = vec5 - mViewCache.mCenter;
        ang3 = globe.U();
        dVar39 = sAngleX(mCornerNormalSum).Cos() * 0.3f + 0.35f;
    } else if (chase->field_0x90) {
        ang3 = player_direction.Inv();
        dVar39 = mCamSetup.WaitRollSpeed();
    } else if (bVar8) {
        ang3 = mViewCache.mDirection.U();
    } else if (bVar3) {
        ang3 = player_direction.Inv();
    } else if (chkFlag(0x100000) || bVar6) {
        if (player->checkChainBlockPushPull()) {
            cSGlobe globe = mViewCache.mCenter - attentionPos(player->getChainGrabActor());
            ang3 = globe.U();
        } else if ((mIsWolf != 1 || mCurMode != 1) && !mCamParam.Flag(param_0, 0x100)) {
            ang3 = player_direction.Inv();
        } else {
            ang3 = chase->field_0x96;
        }
    } else {
        ang3 = player_direction.Inv();
    }

    f32 last_pos_x = mPadInfo.mCStick.mLastPosX;
    std::fabsf(last_pos_x);
    std::fabsf(mPadInfo.mCStick.mLastPosY);
    chase->field_0x93 = false;

    if (!mCamParam.Flag(param_0, 0x40) && std::fabsf(last_pos_x) > 0.05f) {
        chase->field_0xac += (dCamMath::rationalBezierRatio(last_pos_x, 0.5f) * 8.0f - chase->field_0xac) * chase->field_0x4c;
        ang3 = globe.U() + cSAngle(chase->field_0xac);
        dVar39 = std::fabsf(last_pos_x) - 0.05f;
        if (mCamSetup.CheckFlag(0x1000) && mFakeAngleSys.field_0x0 == 0) {
            setUSOAngle();
        }
        if (mCamSetup.CheckFlag2(0x40)) {
            chase->field_0x93 = true;
            chase->field_0x9a = ang3;
        } else if (chkFlag(0x100000)) {
            chase->field_0x93 = true;
            chase->field_0x9a = ang3;
        }
        clrFlag(0x20000);
    } else if (check_owner_action1(mPadID, 0x2010000) && mCurMode == 0) {
        chase->field_0x9a = mDirection.U().Val();
        chase->field_0x93 = true;
    } else if (mIsWolf == 1 && isPlayerCharging(mPadID)) {
        chase->field_0x9a = mDirection.U().Val();
        chase->field_0x93 = true;
    } else {
        chase->field_0xac = 0.0f;
    }

    if (check_owner_action1(mPadID, 0x1000000) || chase->field_0x91) {
        chase->field_0x9a = mDirection.U().Val();
        chase->field_0x93 = true;
    }

    if (chase->field_0x93) {
        mViewCache.mDirection.U(chase->field_0x9a);
    } else {
        mViewCache.mDirection.U(globe.U() + (ang3 - globe.U()) * dVar39);
    }

    cSAngle ang4;

    if (bVar1 && !bVar7) {
        chase->field_0x50 = charge_latitude;
        if (chase->field_0x1a < 14) {
            chase->field_0x78 = dCamMath::rationalBezierRatio(chase->field_0x1a / 14.0f, charge_b_ratio);
            setFlag(0x4000000);
            chase->field_0x1a++;
        } else {
            if (player->checkCutLargeTurnState()) {
                chase->field_0x92 = true;
            }
            chase->field_0x78 = 1.0f;
        }
    } else if (chase->field_0x1a >= 14 && chase->field_0x1a < 34) {
        chase->field_0x50 = charge_latitude;
        chase->field_0x1a++;
        chase->field_0x78 = 1.0f;
    } else {
        if (chase->field_0x1a != 0) {
            chase->field_0x78 = 0.0f;
        }
        chase->field_0x1a = 0;
        if (mCurMode == 1) {
            f32 tmp = val12 + (val13 - val12) * chase->field_0xa4;
            chase->field_0x78 = 0.5f;
            chase->field_0x50 += (tmp - chase->field_0x50) * chase->field_0x48;
        } else {
            check_owner_action1(mPadID, 0x800000);
            if (check_owner_action1(mPadID, 0x1000000) || chase->field_0x73) {
                chase->field_0x78 = 0.75f;
                chase->field_0x50 = val12;
            } else if (chkFlag(0x10000)) {
                chase->field_0x50 += (val12 - chase->field_0x50) * chase->field_0x20;
                chase->field_0x78 = val16;
            } else if (chkFlag(0x80000) || bVar8 || bVar9) {
                chase->field_0x78 = 0.0f;
                chase->field_0x50 = mViewCache.mDirection.V().Degree();
            } else if ((!bVar10 && !bVar11 && (!bVar2 || mCurMode != 0 || mGear != 0)) || bVar12) {
                f32 tmp = val12 + (val13 - val12) * chase->field_0xa4;
                tmp += mForwardTiltOffset.Degree();
                chase->field_0x50 += (tmp - chase->field_0x50) * val16;
                chase->field_0x78 += (JumpCushion - chase->field_0x78) * 0.05f;
            } else if (bVar2) {
                chase->field_0x50 = globe.V().Degree();
                chase->field_0x78 = 0.95f;
            } else if (isPlayerFlying(player)) {
                if (chase->field_0x30 < positionOf(mpPlayerActor).y) {
                    chase->field_0x78 = dCamMath::rationalBezierRatio(chase->field_0x20, val15);
                } else {
                    chase->field_0x78 += (0.75f - chase->field_0x78) * 0.15f;
                }
                chase->field_0x50 = globe.V().Degree();
            } else {
                chase->field_0x50 = globe.V().Degree();
                chase->field_0x78 = dCamMath::rationalBezierRatio(chase->field_0x20, val15);
                if (chase->field_0x91) {
                    ang1.Val(-50.0f);
                    chase->field_0x78 = 0.75f;
                }
            }
        }
    }

    if (chase->field_0x1a == 0) {
        chase->field_0x92 = false;
    }
    chase->field_0x30 = positionOf(mpPlayerActor).y;

    ang4.Val(chase->field_0x50);
    if (ang4 < ang1) {
        ang4.Val(ang1);
    } else if (ang4 > ang2) {
        ang4.Val(ang2);
    }

    if (chase->field_0x1c != 0) {
        chase->field_0x78 = dVar52;
    }
    mViewCache.mDirection.V(mViewCache.mDirection.V() + (ang4 - mViewCache.mDirection.V()) * chase->field_0x78);

    f32 fVar55 = globe.R();

    if (chase->field_0x1c != 0) {
        val11 = 1.0f;
    }
    chase->field_0x28 += (val8 - chase->field_0x28) * val11;
    chase->field_0x2c += (val7 - chase->field_0x2c) * val11;

    bool bVar6a = false;
    bool bVar2a = false;
    if (bVar11 && !check_owner_action(mPadID, 0x100000)) {
        fVar55 = mViewCache.mDirection.R() + (fVar55 - mViewCache.mDirection.R()) * 0.4f;
        if (!bVar34 && fVar55 > val7 * 1.5f) {
            fVar55 = val7 * 1.5f;
        }
    } else if (fVar55 < chase->field_0x28) {
        fVar55 = chase->field_0x28;
        bVar6a = true;
    } else if (fVar55 > chase->field_0x2c) {
        fVar55 = chase->field_0x2c;
        bVar2a = true;
    }

    if (bVar10) {
        chase->field_0x74 += (chase->field_0x20 - chase->field_0x74) * 0.01f;
    } else if (bVar11) {
        chase->field_0x74 += (chase->field_0x20 - chase->field_0x74) * 0.01f;
    } else if (chase->field_0x1c != 0) {
        chase->field_0x74 = dVar52;
    } else if (bVar6a) {
        chase->field_0x74 += (val10 - chase->field_0x74) * 0.01f;
    } else if (bVar2a) {
        chase->field_0x74 += (val10 - chase->field_0x74) * 0.01f;
    } else {
        chase->field_0x74 = 1.0f;
    }

    mViewCache.mDirection.R();
    mViewCache.mDirection.R(mViewCache.mDirection.R() + (fVar55 - mViewCache.mDirection.R()) * chase->field_0x74);
    chase->field_0x64 = mViewCache.mCenter + mViewCache.mDirection.Xyz();
    mViewCache.mEye = chase->field_0x64;

    if (chase->field_0x1c != 0) {
        chase->field_0x80 = dVar52;
    } else {
        chase->field_0x80 += (val21 - chase->field_0x80) * 0.01f;
    }
    mViewCache.mFovy += ((val17 + (val18 - val17) * chase->field_0xa4) - mViewCache.mFovy) * chase->field_0x80;

    if (chase->field_0x1c != 0) {
        chase->field_0x1c--;
    }
    return 1;
}

bool dCamera_c::lockonCamera(s32 param_0) {
    f32 lockon_change_cushion = mCamSetup.LockonChangeCushion();
    cSAngle charge_latitude = cSAngle(mCamSetup.ChargeLatitude());
    f32 charge_b_ratio = mCamSetup.ChargeBRatio();
    int lockon_change_timer = mCamSetup.LockonChangeTimer();
    f32 unkFloatConst1 = 0.15f;
    f32 val1 = mCamParam.Val(param_0, 1);
    f32 val5 = mCamParam.Val(param_0, 5);
    f32 val6 = mCamParam.Val(param_0, 6);
    f32 val25 = mCamParam.Val(param_0, 25);
    f32 val26 = mCamParam.Val(param_0, 26);
    f32 val4 = mCamParam.Val(param_0, 4);
    f32 val13 = mCamParam.Val(param_0, 13);
    f32 val15 = mCamParam.Val(param_0, 15);
    f32 val16 = mCamParam.Val(param_0, 16);
    f32 val27 = mCamParam.Val(param_0, 27);
    f32 val22 = mCamParam.Val(param_0, 22);
    f32 val23 = mCamParam.Val(param_0, 23);
    f32 val24 = mCamParam.Val(param_0, 24);
    f32 val10 = mCamParam.Val(param_0, 10);
    f32 val11 = mCamParam.Val(param_0, 11);
    f32 val20 = mCamParam.Val(param_0, 20);
    f32 val21 = mCamParam.Val(param_0, 21);

    LockOnData* lockon = (LockOnData*)mWork;

    dAttention_c* attention = dComIfGp_getAttention();
    daAlink_c* player = (daAlink_c*)mpPlayerActor;

    if (dComIfGp_evmng_cameraPlay()) {
        fopAc_ac_c* target = getEvActor("Target");
        if (target != NULL) {
            mpLockonTarget = target;
        }
    }

    if (mCurCamStyleTimer == 0) {
        if (mRecovery.field_0x8.field_0x1e <= 0) {
            mViewCache.mDirection = mDirection;
            mViewCache.mCenter = mCenter;
            mViewCache.mEye = mEye;
        }

        lockon->field_0x0 = 'LOCK';
        lockon->field_0xc = 0;
        lockon->field_0x10 = 1.0f;
        lockon->field_0x18 = false;
        lockon->field_0x28 = false;
        lockon->field_0x1c = mViewCache.mCenter;
        cXyz attention_pos = attentionPos(mpPlayerActor);
        lockon->field_0x34.Val(mViewCache.mCenter - attention_pos);
        if (mViewCache.mCenter.x == attention_pos.x && mViewCache.mCenter.z == attention_pos.z) {
            lockon->field_0x34.U(directionOf(mpPlayerActor));
        }
        lockon->field_0x58 = 0.0f;
        lockon->field_0x54 = 0.0f;
        lockon->field_0x5c = mCamSetup.Cushion4Base();
        lockon->field_0x60 = 1.0f;
        lockon->field_0x14 = 0;
        lockon->field_0x29 = false;
        lockon->field_0x2c = 0;
        lockon->field_0x30 = 0;
        lockon->field_0x40 = false;
        lockon->field_0x44 = 0;
        lockon->field_0x48 = mViewCache.mDirection.R();
        lockon->field_0x50 = mViewCache.mDirection.V();
        lockon->field_0x4c = 1.0f;
        lockon->field_0x2a = false;
        lockon->field_0x3c = fpcM_ERROR_PROCESS_ID_e;
        if (player->checkCutHeadProc() && mpLockonTarget != NULL) {
            lockon->field_0x3c = fopAcM_GetID(mpLockonTarget);
        }

        if (mpLockonTarget != NULL) {
            if (fopAcM_GetName(mpLockonTarget) == PROC_Obj_Bemos
                || fopAcM_GetName(mpLockonTarget) == PROC_Obj_Lv6bemos2)
            {
                setUSOAngle();
            }
        } else if (mCamParam.Flag(param_0, 0x2000)) {
            fopAc_ac_c* target = getParamTargetActor(mCurType);
            if (target != NULL) {
                lockon->field_0x3c = fopAcM_GetID(target);
            }
        }

        if (mCurType == specialType[CAM_TYPE_LV7_BOSS]) {
            cSAngle ang = -10.0f;
            if (mViewCache.mDirection.V() < ang) {
                mViewCache.mDirection.V(cSAngle(-10.0f));
                mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
            }
        }

    } else {
        mStyleSettle.mFinished = true;
    }

    if (player->checkCutHeadProc() && lockon->field_0x3c != fpcM_ERROR_PROCESS_ID_e) {
        mpLockonTarget = fopAcM_SearchByID(lockon->field_0x3c);
        if (mpLockonTarget != NULL) {
            dComIfGp_getAttention()->keepLock(30);
        } else {
            dComIfGp_getAttention()->keepLock(0);
        }
    }

    if (mpLockonTarget == NULL && lockon->field_0x3c != fpcM_ERROR_PROCESS_ID_e) {
        mpLockonTarget = fopAcM_SearchByID(lockon->field_0x3c);
    }

    bool bVar1 = false;

    if (check_owner_action(mPadID, 6)) {
        lockon_change_timer = 5;
    } else if (mCurType == specialType[CAM_TYPE_LV9_GZELDA_TRI]) {
        lockon_change_timer = 60;
    } else if (mCurType == specialType[CAM_TYPE_TEPPEI_HOOK]) {
        lockon_change_timer = 40;
    }

    if (mBG.field_0xc0.field_0x1) {
        dComIfG_Bgsp().MoveBgMatrixCrrPos(mBG.field_0x5c.field_0x4, true,
                                          &lockon->field_0x1c, NULL, NULL);
    }

    if (player->checkCopyRodThrowAfter()) {
        lockon->field_0x28 = true;
    } else if (!attention->LockonTruth() && check_owner_action(mPadID, 0x400000)) {
        lockon->field_0x28 = true;
    } else {
        lockon->field_0x28 = false;
    }

    if (lockon->field_0x28) {
        if (chkFlag(0x80080)) {
            lockon->field_0x48 = cXyz(mLastHitPos - mViewCache.mCenter).abs();
            val11 = lockon->field_0x48;
        }
        val10 = lockon->field_0x48;
        if (val23 < 40.0f) {
            val23 = 20.0f;
        }
        if (val24 < 20.0f) {
            val24 = 20.0f;
        }
        val13 = 0.75f;
        val15 = lockon->field_0x50.Degree();
        val16 = 20.0f;
        val27 = 0.1f;
        val22 = 0.4f;
    }

    if (mpLockonTarget != NULL && mLockOnActorID != fpcM_ERROR_PROCESS_ID_e
        && fopAcM_GetName(mpLockonTarget) != PROC_E_HZ)
    {
        val15 = 60.0f;
        val16 = 20.0f;
        val26 = -20.0f;
    }

    if (mpLockonTarget != NULL) {
        if (fopAcM_GetName(mpLockonTarget) == PROC_COW) {
            val27 = 0.8f;
            val22 = 0.8f;
            val23 = 5.0f;
            val24 = 10.0f;
        } else if (fopAcM_GetName(mpLockonTarget) == PROC_Obj_Cdoor) {
            val23 = 15.0f;
            val24 = 5.0f;
        }
    }

    if (player->checkThrowDamage()) {
        bVar1 = true;
        val15 = -10.0f;
        val16 = 0.0f;
        val10 = 300.0f;
        val11 = 250.0f;
        val5 = 0.05f;
        val6 = 0.1f;
    }

    if (!check_owner_action(mPadID, 0) && !check_owner_action1(mPadID, 0x1200000)) {
        bVar1 = true;
    }

    if (dComIfGp_getAttention()->LockEdge()) {
        field_0x160 = mCurCamStyleTimer = 0;
        lockon->field_0x2a = false;
    }

    cSGlobe globe;
    f32 curveWeight = mCamSetup.CurveWeight();
    f32 lockon_release_distance = attention->LockonReleaseDistanse();
    f32 sp19C = 10000.0f;
    f32 fVar42;

    if (mpLockonTarget != NULL) {
        cXyz target_attention_pos = attentionPos(mpLockonTarget);
        cXyz player_attention_pos = attentionPos(mpPlayerActor);

        if (mCurMode == 6) {
            target_attention_pos.x = positionOf(mpLockonTarget).x;
            target_attention_pos.z = positionOf(mpLockonTarget).z;
        }

        // this should probably be an ifdef, but we force it to be compiled
        // to make the function large enough to stop doing inlining
        if (!NDEBUG) {
            if (mCamSetup.CheckFlag(0x8000)) {
                //char name[28];
                fopAcM_getNameString(mpPlayerActor, NULL);
                dDbVw_Report(0x1e0, 0x109, "%s", NULL);
            }
        }

        if (check_owner_action(mPadID, 0x2000008)) {
            cXyz vec(0.0f, 0.0f, -90.0f);
            player_attention_pos += dCamMath::xyzRotateY(vec, directionOf(mpPlayerActor));
        }

        globe.Val(target_attention_pos - player_attention_pos);
        globe.R(globe.R() + mpLockonTarget->attention_info.field_0xa);
        fVar42 = globe.R() / lockon_release_distance;
        if (fVar42 > 1.0f) {
            fVar42 = 1.0f;
        }

        sp19C = dCamMath::xyzHorizontalDistance(target_attention_pos, player_attention_pos);
    } else {
        OS_REPORT("NULL TARGET&&&&&&&&&&&&& \n");
        globe.Val(mCamSetup.ParallelDist(), cSAngle::_0, directionOf(mpPlayerActor));
        fVar42 = 1.0f;
    }

    cSAngle ang1 = globe.U();
    cSAngle ang2 = rangef(val23, val24, fVar42);

    if (player->checkHorseRide()) {
        ang1.Val(directionOf(mpPlayerActor));
    }

    bool sp0F = false;
    bool sp0E = false;
    bool sp0D = false;
    if (check_owner_action(mPadID, 0x100)) {
        sp0F = true;
    }
    if (check_owner_action(mPadID, 0x2000008)) {
        sp0E = true;
    }
    if (player->checkMagneBootsOn()) {
        sp0D = true;
        Vec* magneBootsTopVec = player->getMagneBootsTopVec();
        if ((u8)cBgW_CheckBWall(magneBootsTopVec->y)) {
            sp0E = true;
        }
    }

    f32 fVar14;
    if (mCurCamStyleTimer < lockon_change_timer && !lockon->field_0x2a) {
        fVar14 = dCamMath::rationalBezierRatio((f32)mCurCamStyleTimer / lockon_change_timer, 0.5f);
        ang2 *= fVar14;
    } else if (mCurCamStyleTimer >= lockon_change_timer) {
        lockon->field_0x2a = true;
        fVar14 = 1.0f;
    }

    cSAngle ang3(mViewCache.mDirection.U().Inv() - ang1);

    if (mCurCamStyleTimer != 0 && mCurCamStyleTimer < lockon_change_timer) {
        if (lockon->field_0x2c == 0) {
            ang1 -= ang2;
            ang3 = -ang3;
        } else {
            ang1 += ang2;
        }
    } else if (ang3 < cSAngle::_0) {
        lockon->field_0x2c = 0;
        ang1 -= ang2;
        ang3 = -ang3;
    } else {
        lockon->field_0x2c = 1;
        ang1 += ang2;
    }

    cXyz attention_pos = attentionPos(mpPlayerActor);
    bool bVar4 = false;
    if (bVar4) {
        f32 sp31c = positionOf(mpPlayerActor).x;
        f32 sp324 = positionOf(mpPlayerActor).x;
    }
    u8 unusedByte1 = 0;
    if (chkFlag(0x80080)) {
        cXyz attention_pos = attentionPos(mpPlayerActor);
        if (!pointInSight(&attention_pos)) {
            if (lockon->field_0x14 == 0) {
                lockon->field_0x30 = lockon->field_0x2c == 1 ? 0 : 1;
            }
            bVar4 = true;
            lockon->field_0x14 = 30;
        }
    }

    if (lockon->field_0x14 != 0) {
        lockon->field_0x14--;
        if (lockon->field_0x14 == 0 && mPadInfo.mMainStick.mLastValue <= 0.1f) {
            lockon->field_0x14 = 1;
        }
        bVar4 = true;
    }

    f32 fVar43 = 1.0f - fabsf(mPadInfo.mCStick.mLastPosY);
    f32 fVar44;

    if (bVar1) {
        if (mCurCamStyleTimer == 0) {
            lockon->field_0x5c = 0.01f;
        }
        fVar44 = 0.25f;
    } else if (mBG.field_0xc0.field_0x44 == 0) {
        fVar44 = mCamSetup.Cushion4Jump();
    } else {
        fVar44 = fVar14 * mCamSetup.Cushion4Base();
    }

    lockon->field_0x5c += (fVar44 - lockon->field_0x5c) * mCamSetup.CusCus();
    lockon->field_0x1c.x = attention_pos.x;
    lockon->field_0x1c.z = attention_pos.z;

    f32 dVar37;
    if (bVar4) {
        dVar37 = rangef(val25, val26, fVar42) + 25.0f;
    } else {
        dVar37 = rangef(val25, val26, fVar42);
    }
    lockon->field_0x1c.y +=
        ((attention_pos.y + dVar37) - lockon->field_0x1c.y) * lockon->field_0x5c;

    if (mCurCamStyleTimer == 0) {
        cXyz vec = lockon->field_0x1c;
        lockon->field_0x34.Val(mViewCache.mCenter - vec);
        if (mViewCache.mCenter.x == vec.x && mViewCache.mCenter.z == vec.z) {
            lockon->field_0x34.U(directionOf(mpPlayerActor));
            f32 sp180 = globe.R() * 0.05f;
        }
    }

    f32 fVar44a;
    if (mpLockonTarget != NULL) {
        f32 dVar28 = ang3.Cos();
        f32 dVar37 = cSAngle(globe.V()).Cos();
        if (dVar28 < 0.0f) {
            dVar37 = -dVar37;
        }
        f32 tmp = std::fabs(dVar28) < std::fabs(dVar37) ? dVar28 : dVar37;
        tmp *= val4 < 0.5f ? val4 : 1.0f - val4;
        fVar44a = val4 * globe.R() - tmp * globe.R() * val1;
    } else {
        fVar44a = globe.R() * 0.5f;
        fVar44a += fVar44a * ang3.Cos();
    }

    cSAngle ang4 = globe.U();
    lockon->field_0x58 += (val6 - lockon->field_0x58) * mCamSetup.CusCus();
    lockon->field_0x54 += (val5 - lockon->field_0x54) * mCamSetup.CusCus();

    cSAngle u, v;
    cSAngle ang5 = globe.V() - lockon->field_0x34.V();
    f32 r;
    if (bVar4) {
        r = lockon->field_0x34.R();
        r = r * 0.75f * fabsf(ang5.Cos());
        u.Val(lockon->field_0x34.U() + (ang4 - lockon->field_0x34.U()) * lockon->field_0x58);
        v.Val(lockon->field_0x34.V() + ang5 * 0.05f);
    } else {
        r = lockon->field_0x34.R();
        r = r + ((fVar44a - r) * lockon->field_0x54 * fabsf(ang5.Cos()));
        u.Val(lockon->field_0x34.U() + (ang4 - lockon->field_0x34.U()) * lockon->field_0x58);
        v.Val(lockon->field_0x34.V() + ang5 * lockon->field_0x58);
    }
    lockon->field_0x34.Val(r, v, u);
    mViewCache.mCenter = lockon->field_0x1c + lockon->field_0x34.Xyz();

    dBgS_CamLinChk lin_chk;
    if (mpLockonTarget != NULL && lineBGCheck(&attention_pos, &mViewCache.mCenter, &lin_chk, 0x40b7)) {
        cXyz target_attention_pos = attentionPos(mpLockonTarget);
        dBgS_CamLinChk lin_chk2;
        int iVar5 = 0;
        if (lineBGCheckBoth(&attention_pos, &mViewCache.mEye, &lin_chk2, 0x40b7)) {
            iVar5 |= 1;
        } else if (lineBGCheckBoth(&target_attention_pos, &mViewCache.mEye, &lin_chk2, 0x40b7)) {
            iVar5 |= 2;
        }

        if (iVar5 != 0) {
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
            mViewCache.mCenter = lin_chk.GetCross();
            mViewCache.mCenter += *plane.GetNP() * 5.0f;
            if (fopAcM_GetName(mpLockonTarget) != PROC_E_HZ) {
                ForceLockOff(mLockOnActorID);
            }
        }
    }

    cSGlobe globe2 = mViewCache.mEye - mViewCache.mCenter;
    cSAngle u2 = mViewCache.mDirection.U();
    cSAngle v2 = mViewCache.mDirection.V();
    f32 r2 = mViewCache.mDirection.R();
    cSAngle ang6 = ang3 - ang2;
    f32 curve_weight = mCamSetup.CurveWeight();
    f32 fVar47 = mPadInfo.mCStick.mLastPosX;
    if (mCamParam.Flag(param_0, 0x40)) {
        fVar47 = 0.0f;
    }

    if (mPadInfo.mCStick.mLastPosY > mCamSetup.mCStick.SwTHH()) {
        if (mCStickUpLatch != 1) {
            lockon->field_0x40 = false;
            lockon->field_0x4c = 0.0f;
        }
        mCStickUpLatch = 1;
    } else {
        mCStickUpLatch = 0;
    }

    bool bVar3 = false;
    f32 fVar31;
    if (std::fabsf(fVar47) > 0.05f) {
        cSAngle ang = globe2.U() + cSAngle(dCamMath::rationalBezierRatio(fVar47, 0.5f) * 7.5f);
        fVar31 = std::fabsf(fVar47) - 0.05f;
        lockon->field_0x42 = ang;
        lockon->field_0x4c = 0.0f;
        bVar3 = true;
    } else if (mCamParam.Flag(param_0, 0x100)) {
        if (mCurMode == 0) {
            lockon->field_0x40 = true;
            cSAngle ang = globe2.U() - directionOf(mpPlayerActor);
            lockon->field_0x42 = mViewCache.mDirection.U() +
                                (globe2.U() - mViewCache.mDirection.U()) * val22 * ang.Sin();
        }
        if (mCurMode != 2) {
            ang1.Val(directionOf(mpPlayerActor));
        }
    } else if (lockon->field_0x40) {
        fVar31 = 1.0f;
        lockon->field_0x4c = 1.0f;
    } else {
        lockon->field_0x40 = false;
        lockon->field_0x4c += (1.0f - lockon->field_0x4c) * 0.01f;
    }

    if (lockon->field_0x40 || bVar3) {
        u2 = globe2.U() + (lockon->field_0x42 - globe2.U()) * fVar31;
    } else if (bVar4) {
        cSAngle ang;
        if (lockon->field_0x30 == 1) {
            ang.Val(15.0f);
        } else {
            ang.Val(-15.0f);
        }
        u2 += ((globe.U().Inv() + ang) - u2) * 0.05f;
    } else if (check_owner_action1(mPadID, 0x1200000)) {
        u2 = globe2.U();
    } else {
        if (mpLockonTarget == NULL) {
            fVar31 = fVar14 * 0.15f;
        } else if (ang3 < ang2) {
            f32 ratio = dCamMath::rationalBezierRatio(-((f32)ang6.Val() / ang2.Val()), curve_weight);
            fVar31 = val27 * ratio;
        } else {
            cSAngle ang = ang2 + (cSAngle::_180 - ang2) * 0.5f;
            if (ang6 > ang) {
                ang6 = cSAngle::_180 - ang6;
                ang = cSAngle::_180 - ang;
            }
            fVar31 = val27 + (val22 - val27) * dCamMath::rationalBezierRatio((f32)ang6.Val() / ang.Val(), curve_weight);
        }

        if (!lockon->field_0x2a) {
            int iVar27 = lockon_change_timer >> 1;
            if (mCurCamStyleTimer < iVar27) {
                fVar31 = lockon_change_cushion * ((f32)mCurCamStyleTimer / iVar27);
            } else {
                fVar31 = fVar31 * ((f32)(mCurCamStyleTimer - iVar27) / iVar27)
                    + lockon_change_cushion * (1.0f - (f32)(mCurCamStyleTimer - iVar27) / iVar27);
            }
        }

        ang6 = ang1.Inv() - mViewCache.mDirection.U();
        fabsf(ang6.Degree()) < 2.0f;
        bool temp2 = false;
        if (temp2) {
            lockon->field_0x2a = true;
        }
        u2 += ang6 * fVar31 * lockon->field_0x4c;
    }

    if (sp0E) {
        cSAngle ang7 = ang1.Inv();
        cSAngle ang8 = ang7 - u2;
        if (ang8 < cSAngle::_270) {
            u2 = ang7 - cSAngle::_270;
        } else if (ang8 > cSAngle::_90) {
            u2 = ang7 - cSAngle::_90;
        }
    }

    if (isPlayerCharging(mPadID)) {
        if (lockon->field_0xc <= 20) {
            f32 tmp = lockon->field_0xc / 20.0f;
            v2 += (charge_latitude - v2) * dCamMath::rationalBezierRatio(tmp, charge_b_ratio);
            setFlag(0x4000000);
            lockon->field_0xc++;
        } else {
            v2 = charge_latitude;
        }
    } else {
        lockon->field_0xc = 0;
        if (!mBG.field_0xc0.field_0x44 && !bVar1) {
            v2 += (globe2.V() - v2) * fVar43 * fabsf(mViewCache.mDirection.V().Cos());
        } else {
            cSAngle ang7 = lockon->field_0x34.V();
            ang7 *= cSAngle(lockon->field_0x34.U() - mViewCache.mDirection.U()).Cos();
            ang7 *= val13;
            cSAngle ang8 = rangef(val15, val16, fVar42);
            ang8 *= 1.0f - val13;
            v2 += ((ang7 + ang8) - v2) * 0.15f * fVar14;
        }
        s16 val = v2.Val();
        if (!mCamSetup.CheckLatitudeRange(&val)) {
            v2.Val(val);
        }
    }

    if (bVar4) {
        r2 += (280.0f - r2) * 0.05f;
    } else {
        f32 radius = globe2.R();
        if (defaultRadius(val10, val11, &radius)) {
            f32 radius2 = radius;
            f32 range = rangef(val10, val11, fVar42) - radius2;
            r2 = radius2 + (range) * 0.02f;
        } else {
            r2 += (radius - r2) * 0.4f * fVar14;
        }
    }

    mViewCache.mDirection.Val(r2, v2, u2);
    mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
    mViewCache.mFovy += (rangef(val20, val21, fVar42) - mViewCache.mFovy) * 0.15f * fVar14;

    if (mpLockonTarget != NULL) {
        setFlag(0x2000);
        mpAuxTargetActor1 = mpLockonTarget;
        if (attention->GetLockonCount() >= 2 && mCamParam.Flag(param_0, 0x1000)) {
            mpAuxTargetActor2 = attention->LockonTarget(1);
        }
    }

    if (player->checkHorseRide()) {
        daHorse_c* horse = dComIfGp_getHorseActor();
        if (horse != NULL && horse->getLashDashStart()) {
            onHorseDush();
            lockon->field_0x44 = 16;
        } else if (lockon->field_0x44 != 0) {
            lockon->field_0x44--;
        }
    }

    return true;
}

fopAc_ac_c* dCamera_c::getMsgCmdSpeaker() {
    dComIfG_MesgCamInfo_c* info = dComIfGp_getMesgCameraInfo();
    fopAc_ac_c* actor = NULL;

    if (info->mBasicID >= 1 && info->mBasicID <= 10) {
        int idx = info->mBasicID - 1;
        actor = info->mActor[idx];
    }

    if (info->mID >= 1 && info->mID <= 10) {
        int idx = info->mID - 1;
        actor = info->mActor[idx];
    }

    return actor;
}

s32 dCamera_c::getMsgCmdCut(s32 param_0) {
    dComIfG_MesgCamInfo_c* info = dComIfGp_getMesgCameraInfo();

    if (!(info->mBasicID >= 1 && info->mBasicID <= 10) && info->mBasicID > 0) {
        param_0 = info->mBasicID;
    }

    if (!(info->mID >= 1 && info->mID <= 10) && info->mID > 0) {
        param_0 = info->mID;
    }

    return param_0;
}

bool dCamera_c::talktoCamera(s32 param_0) {
    f32 val0 = mCamParam.Val(param_0, 0);
    f32 val2 = mCamParam.Val(param_0, 2);
    f32 val1 = mCamParam.Val(param_0, 1);
    f32 val3 = mCamParam.Val(param_0, 3);
    f32 val7 = mCamParam.Val(param_0, 7);
    f32 val8 = mCamParam.Val(param_0, 8);
    f32 val16 = mCamParam.Val(param_0, 16);
    f32 val15 = mCamParam.Val(param_0, 15);
    f32 val12 = mCamParam.Val(param_0, 12);
    f32 val6 = mCamParam.Val(param_0, 6);
    f32 val23 = mCamParam.Val(param_0, 23);
    f32 val24 = mCamParam.Val(param_0, 24);
    f32 val17 = mCamParam.Val(param_0, 17);
    f32 val18 = mCamParam.Val(param_0, 18);

    f32 fVar21 = 300.0f;
    fopAc_ac_c* ride_actor = NULL;

    TalkData* talk = (TalkData*)mWork;

    s32 zero = 0;
    bool uVar18 = true;
    daAlink_c* player = (daAlink_c*)mpPlayerActor;

    if (mCurCamStyleTimer == 0) {
        talk->field_0x0 = 'TALK';
        talk->field_0x44 = 0;
        talk->field_0x38 = -1;
        talk->field_0x48 = 20;
        talk->field_0x3c = zero;
        talk->field_0x40 = -1;
        talk->field_0x5c = 999.9f;
        talk->field_0x60 = 999.9f;
        talk->field_0x7c = 1.0f;
        talk->field_0xb4 = cXyz::Zero;
        talk->field_0x89 = false;
        talk->field_0x88 = false;
        talk->field_0x8c = mBumpCheckFlags;
        mBumpCheckFlags &= ~8;

        if (!dComIfGp_evmng_cameraPlay()) {
            talk->field_0x84 = 0;
            talk->field_0x86 = 0;
            talk->field_0x54 = val7;
            talk->field_0x64 = val8;
            talk->field_0x68 = val17;
            talk->field_0x58 = val17;
            talk->field_0x6c = val18;
            talk->field_0x70 = mpPlayerActor;
            talk->field_0x74 = mpLockonTarget;
        } else {
            int val;
            getEvIntData(&val, "Smoothless", 0);
            talk->field_0x84 = val;
            getEvIntData(&val, "Mode", 0);
            talk->field_0x86 = val;
            if (getEvFloatData(&talk->field_0x54, "Radius", val7)) {
                getEvFloatData(&talk->field_0x64, "RadiusNear", talk->field_0x54);
            } else {
                getEvFloatData(&talk->field_0x64, "RadiusNear", val8);
            }
            getEvFloatData(&talk->field_0x5c, "Longitude", 999.9f);
            if (getEvFloatData(&talk->field_0x68, "Fovy", val17)) {
                getEvFloatData(&talk->field_0x6c, "FovyNear", talk->field_0x68);
            } else {
                getEvFloatData(&talk->field_0x6c, "FovyNear", val18);
            }
            talk->field_0x58 = talk->field_0x68;
            getEvFloatData(&talk->field_0x60, "Latitude", 999.9f);
            talk->field_0x70 = getEvActor("Listener", "@STARTER");
            talk->field_0x74 = getEvActor("Speaker", "@TALKPARTNER");
        }

        talk->field_0x78 = talk->field_0x74;
        talk->field_0x80 = val2;
    }

    fopAc_ac_c* speaker;
    fopAc_ac_c* listener;
    fopAc_ac_c* msg_speaker = getMsgCmdSpeaker();
    if (msg_speaker != NULL) {
        listener = talk->field_0x70;
        speaker = msg_speaker;
    } else if (dComIfGp_evmng_cameraPlay()) {
        listener = talk->field_0x70;
        speaker = talk->field_0x74;
    } else {
        listener = mpPlayerActor;
        speaker = mpLockonTarget;
    }

    if (listener == speaker) {
        speaker = NULL;
    }

    if (listener == NULL || speaker == NULL) {
        mStyleSettle.mFinished = true;
        return false;
    }

    if (talk->field_0x78 != speaker) {
        mCurCamStyleTimer = 0;
        mStyleSettle.mFinished = false;
        talk->field_0x44 = 0;
        talk->field_0x78 = speaker;
    }

    bool bVar3 = false;
    if (fopAcM_GetName(speaker) == PROC_NI || fopAcM_GetName(speaker) == PROC_BD
        || fopAcM_GetName(speaker) == PROC_SQ || fopAcM_GetName(speaker) == PROC_FR
        || fopAcM_GetName(speaker) == PROC_DO || fopAcM_GetName(speaker) == PROC_NPC_NE)
    {
        bVar3 = true;
        talk->field_0x54 = 260.0f;
        talk->field_0x64 = 210.0f;
        talk->field_0x68 = 45.0f;
        talk->field_0x58 = 45.0f;
        talk->field_0x6c = 48.0f;
        val24 = 80.0f;
        val23 = 40.0f;
    }

    if (fopAcM_GetName(speaker) == PROC_Tag_Mwait && ((daTagMwait_c*)speaker)->checkEndMessage()) {
        talk->field_0x3c = 35;
        speaker = daPy_py_c::getMidnaActor();
    }

    if (talk->field_0x86 != 0) {
        talk->field_0x3c = talk->field_0x86;
    }
    talk->field_0x3c = getMsgCmdCut(talk->field_0x3c);
    if (talk->field_0x3c != talk->field_0x40) {
        talk->field_0x44 = 0;
        talk->field_0x40 = talk->field_0x3c;
    }

    cSAngle stack_134c = val16;
    cSAngle stack_1350 = val15;
    cSAngle stack_1354 = val24;
    cSAngle stack_1358 = val23;

    if (mCurCamStyleTimer == 0) {
        cSAngle stack_135c;
        cXyz stack_13c;
        cSGlobe stack_12c0 = positionOf(speaker) - positionOf(listener);
        cXyz stack_148;
        cXyz stack_154;

        if (mCamParam.Flag(param_0, 0x400) || player->checkCanoeRide()
            || player->checkHorseRide() || check_owner_action(mPadID, 0x100000))
        {
            stack_148 = attentionPos(listener);
            stack_154 = attentionPos(speaker);
            if (player->checkCanoeRide() && listener == mpPlayerActor) {
                stack_148.y += 40.0f;
            }
        } else {
            stack_13c = attentionPos(listener) - positionOf(listener);
            stack_135c.Val(stack_12c0.U() - directionOf(listener));
            stack_148 = positionOf(listener) + dCamMath::xyzRotateY(stack_13c, stack_135c);
            stack_13c = attentionPos(speaker) - positionOf(speaker);
            stack_135c.Val(stack_12c0.U().Inv() - directionOf(speaker));
            stack_154 = positionOf(speaker) + dCamMath::xyzRotateY(stack_13c, stack_135c);
        }

        if (mIsWolf == 1) {
            if (listener == mpPlayerActor) {
                stack_148.y += 80.0f;
            }
            if (speaker == mpPlayerActor) {
                stack_154.y += 80.0f;
            }
        }

        talk->field_0x28 = mViewCache.mDirection;
        talk->field_0xb4 = stack_148 - stack_154;
        talk->field_0x30.Val(talk->field_0xb4);
        talk->field_0xb4.normalize();
        stack_148 += talk->field_0xb4 * listener->attention_info.field_0xa;
        stack_154 -= talk->field_0xb4 * speaker->attention_info.field_0xa;
        if (talk->field_0x30.R() < 88.0f) {
            talk->field_0x30.R(88.0f);
        }
        f32 dVar25 = dCamMath::xyzHorizontalDistance(stack_148, stack_154);
        f32 fVar1 = dVar25 - 88.0f;
        fVar21 -= 88.0f;
        talk->field_0x7c = fVar1 > fVar21 ? 1.0f : fVar1 / fVar21;
        talk->field_0x80 = val3 + (val2 - val3) * talk->field_0x7c;
        cXyz stack_160(val0, talk->field_0x80, val1);
        f32 dVar21 = talk->field_0x64 + (talk->field_0x54 - talk->field_0x64) * talk->field_0x7c;
        if (dVar21 < dVar25) {
            dVar21 = dVar25;
        }
        talk->field_0x28.R(dVar21);

        if (talk->field_0x84 != 0) {
            talk->field_0x48 = 1;
        } else {
            talk->field_0x48 = (int)(JMAFastSqrt(std::fabsf(mViewCache.mDirection.R() - talk->field_0x28.R())) / 2.0f);
            if (talk->field_0x48 < 2) {
                talk->field_0x48 = 2;
            }
            if (talk->field_0x48 > 22) {
                talk->field_0x48 = 22;
            }
            talk->field_0x48 += 8;
        }

        cSAngle stack_1360;
        cSAngle stack_1364;

        if (talk->field_0x5c >= -180.0f && talk->field_0x5c <= 360.0f) {
            stack_1360.Val(talk->field_0x5c);
            stack_1364 = stack_1360 - talk->field_0x30.U();
            talk->field_0x28.U(stack_1360);
        } else if (fopAcM_GetName(speaker) == PROC_OBJ_KANBAN2
                    || fopAcM_GetName(speaker) == PROC_TAG_KMSG
                    || fopAcM_GetName(speaker) == PROC_KNOB20
                    || fopAcM_GetName(speaker) == PROC_Obj_NamePlate) {
            stack_1360.Val(directionOf(speaker));
            stack_1364 = stack_1360 - talk->field_0x30.U();
            talk->field_0x28.U(stack_1360);
        } else {
            stack_1360.Val(mViewCache.mDirection.U());
            stack_1364 = stack_1360 - talk->field_0x30.U();
            if (stack_1364 > cSAngle::_90) {
                stack_1364 = cSAngle::_90 - (stack_1364 - cSAngle::_90);
            }
            if (stack_1364 < cSAngle::_270) {
                stack_1364 = cSAngle::_270 - (stack_1364 - cSAngle::_270);
            }
            if (stack_1364 > stack_1354) {
                stack_1364 = stack_1354;
            }
            if (stack_1364 > cSAngle::_0 && stack_1364 < stack_1358) {
                stack_1364 = stack_1358;
            }
            if (stack_1364 < -stack_1354) {
                stack_1364 = -stack_1354;
            }
            if (stack_1364 < cSAngle::_0 && stack_1364 > -stack_1358) {
                stack_1364 = -stack_1358;
            }
            talk->field_0x28.U(talk->field_0x30.U() + stack_1364);
        }

        {
            cSAngle stack_1368 = talk->field_0x28.U();
            cXyz stack_16c = stack_148;
            cXyz stack_178 = stack_154;
            cXyz stack_184 = stack_178 - stack_16c;
            cSGlobe stack_12c8 = stack_184;
            stack_184.normalize();
            stack_16c -= stack_184 * listener->attention_info.field_0xa;
            stack_178 += stack_184 * speaker->attention_info.field_0xa;

            cXyz stack_190;
            cXyz stack_19c = stack_178 - stack_16c;
            if (lineBGCheck(&stack_16c, &stack_178, &stack_190, 0x40b7)) {
                stack_178 = stack_190 - stack_19c.norm() * 10.0f;
                stack_19c = stack_178 - stack_16c;
            }

            cXyz stack_1a8 = stack_16c + stack_19c * 0.5f;
            cXyz stack_1b4 = stack_160;
            cSAngle stack_136c = stack_1368 - stack_12c8.U();
            if (stack_136c < cSAngle::_0) {
                stack_1b4.x = -stack_1b4.x;
            }
            cSGlobe stack_12d0 = stack_1b4;
            stack_12d0.U(stack_12c8.U() + stack_12d0.U());
            stack_12c8.R(stack_12c8.R() * 0.5f * stack_136c.Cos() * 0.25f);
            talk->field_0x4 = stack_1a8 + stack_12c8.Xyz() + stack_12d0.Xyz();
            talk->field_0xc0 = stack_178;
        }

        cSAngle stack_1370;
        if (talk->field_0x60 != 999.9f) {
            stack_1370.Val(talk->field_0x60);
        } else {
            stack_1370 = talk->field_0x30.V() * (stack_1364.Cos() + 0.1f) * val6 + cSAngle(val12);
            if (stack_1370 > stack_134c) {
                stack_1370 = stack_134c;
            }
            if (stack_1370 < stack_1350) {
                stack_1370 = stack_1350;
            }
        }
        talk->field_0x28.V(stack_1370);

        cSAngle stack_1374;
        if (player->checkRide()) {
            ride_actor = player->getRideActor();
        }

        if (talk->field_0x30.U() - talk->field_0x28.U() > cSAngle::_0) {
            stack_1374 = cSAngle(10.0f);
        } else {
            stack_1374 = cSAngle(-10.0f);
        }

        talk->field_0x10 = talk->field_0x4 + talk->field_0x28.Xyz();
        talk->field_0x58 = talk->field_0x6c + (talk->field_0x68 - talk->field_0x6c) * talk->field_0x7c;

        bool bVar13 = false;
        if (fopAcM_GetName(speaker) == PROC_MIDNA && mMidnaRidingAndVisible) {
            talk->field_0x4 = attentionPos(speaker);
            talk->field_0x4.y -= 35.0f;
            f32 fVar36 = talk->field_0x30.U() - talk->field_0x28.U() > cSAngle::_0 ? -40.0f : 40.0f;
            talk->field_0x28.U(cSAngle(fVar36) + directionOf(listener));
            talk->field_0x28.V(cSAngle(10.0f));
            talk->field_0x28.R(200.0f);
            talk->field_0x10 = talk->field_0x4 + talk->field_0x28.Xyz();
            talk->field_0x48 = 16;
            talk->field_0x58 = 55.0f;
            talk->field_0x88 = true;
            bVar13 = true;
        }

        if (bVar3) {
            talk->field_0x4.y = attentionPos(speaker).y - 10.0f;
        }

        if (mCamParam.Flag(param_0, 0x100)) {
            talk->field_0x28.U(mViewCache.mDirection.U());
        }

        cSAngle stack_1378;
        int i;
        bool bVar2 = false;
        cXyz stack_1c0 = cXyz::Zero;

        for (i = 0; i < 36; i++) {
            stack_1378 = talk->field_0x28.U() - talk->field_0x30.U();
            if (std::fabsf(stack_1378.Degree()) < 10.0f) {
                talk->field_0x28.U(talk->field_0x28.U() + stack_1374);
            } else {
                if (!bVar13) {
                    f32 radius = radiusActorInSight(listener, speaker, &talk->field_0x4,
                                                    &talk->field_0x10, talk->field_0x58,
                                                    0, 0.1f);
                    if (radius > 0.0f) {
                        talk->field_0x28.R(talk->field_0x28.R() + radius);
                        talk->field_0x10 = talk->field_0x4 + talk->field_0x28.Xyz();
                    }
                }

                if (!lineBGCheck(&stack_148, &talk->field_0x10, talk->field_0x8c)
                    && !lineBGCheck(&talk->field_0x4, &talk->field_0x10, talk->field_0x8c)
                    && !lineCollisionCheck(stack_148, talk->field_0x10, listener, speaker, ride_actor))
                {
                    if (!lineBGCheck(&stack_154, &talk->field_0x10, talk->field_0x8c)
                        && !lineCollisionCheck(stack_154, talk->field_0x10, listener, speaker, ride_actor))
                    {
                        bVar2 = true;
                        break;
                    }
                    stack_1c0 = talk->field_0x10;
                }

                talk->field_0x28.U(talk->field_0x28.U() + stack_1374);

                if (talk->field_0x60 != 999.9f) {
                    stack_1370.Val(talk->field_0x60);
                } else {
                    stack_1370 = talk->field_0x30.V()
                        * (cSAngle(talk->field_0x30.U() - talk->field_0x28.U()).Cos() + 0.1f)
                        * val6 + cSAngle(val12);
                    if (stack_1370 > stack_134c) {
                        stack_1370 = stack_134c;
                    }
                    if (stack_1370 < stack_1350) {
                        stack_1370 = stack_1350;
                    }
                }

                talk->field_0x28.V(stack_1370);

                if (!talk->field_0x88) {
                    talk->field_0x4 = relationalPos2(listener, speaker, &stack_160, 0.25f,
                                                          talk->field_0x28.U());
                }
                talk->field_0x10 = talk->field_0x4 + talk->field_0x28.Xyz();
            }
        }

        if (!bVar2) {
            stack_1c0.set(0.0f, 15.0f, -20.0f);
            talk->field_0x4 = relationalPos(mpPlayerActor, &stack_1c0);
            stack_1c0.set(60.0f, 70.0f, -200.0f);
            talk->field_0x10 = relationalPos(mpPlayerActor, &stack_1c0);
            talk->field_0x28.Val(talk->field_0x10 - talk->field_0x4);
        }

        talk->field_0xcc = stack_148;
        talk->field_0xd8 = stack_154;
        talk->field_0x4c = talk->field_0x48 * (talk->field_0x48 + 1) >> 1;

        if (talk->field_0x38 == -1) {
            if (talk->field_0x30.U() - talk->field_0x28.U() > cSAngle::_0) {
                talk->field_0x38 = 0;
            } else {
                talk->field_0x38 = 1;
            }
        }
    }

    if ((fopAcM_GetName(speaker) == PROC_Tag_Mhint && ((daTagMhint_c*)speaker)->checkNoAttention())
        || (fopAcM_GetName(speaker) == PROC_Tag_Mstop && ((daTagMstop_c*)speaker)->checkNoAttention()))
    {
        bool bVar13 = false;
        if (mIsWolf == 1 && check_owner_action(mPadID, 0x100000)) {
            bVar13 = true;
        }

        if (mCurCamStyleTimer == 0) {
            if (!bVar13) {
                talk->field_0x28.U(cSAngle(15.0f) + directionOf(listener));
                talk->field_0x28.V(cSAngle(5.0f));
                talk->field_0x28.R(140.0f);
            } else {
                talk->field_0x28.U(cSAngle(30.0f) + directionOf(listener));
                talk->field_0x28.V(cSAngle(25.0f));
                talk->field_0x28.R(180.0f);
            }

            talk->field_0x10 = talk->field_0x4 + talk->field_0x28.Xyz();
            talk->field_0x48 = 16;
            talk->field_0x58 = 55.0f;
        }

        talk->field_0x4 = positionOf(listener);
        if (!bVar13) {
            talk->field_0x4.y += 110.0f;
        } else {
            talk->field_0x4.y += 10.0f;
        }
    }

    cXyz stack_1cc = cXyz::Zero;
    bool bVar13 = false;
    if (is_player(listener) && mIsWolf == 1) {
        cXyz stack_1d8(0.0f, 0.0f, 45.0f);
        stack_1cc = dCamMath::xyzRotateY(stack_1d8, directionOf(listener));
        bVar13 = true;
    }

    cXyz stack_1e4;
    cXyz stack_1f0;
    cXyz stack_1fc;
    cXyz stack_208;
    fopAc_ac_c* actor1;
    fopAc_ac_c* actor2;
    int iVar5 = talk->field_0x3c;

    switch (iVar5) {
    case 0:
        break;

    case 50:
        talk->field_0x48 = 1;
        talk->field_0x4c = 1.0f;
        iVar5 = 0;
        break;

    case 20:
    case 21:
    case 62:
        if (iVar5 != 20) {
            actor1 = listener;
            stack_1e4 = talkEyePos(listener);
            stack_1f0 = talkEyePos(speaker);
            stack_1fc = talkBasePos(listener);
            stack_208 = talkBasePos(speaker);
            if (bVar3) {
                stack_1f0.y = attentionPos(speaker).y;
            }
            if (fopAcM_GetName(speaker) == PROC_NPC_KKRI) {
                stack_1f0.y = attentionPos(speaker).y - 40.0f;
            }
            if (bVar13) {
                stack_1fc += stack_1cc;
            }
        } else {
            actor1 = speaker;
            stack_1e4 = talkEyePos(speaker);
            stack_1f0 = talkEyePos(listener);
            stack_1fc = talkBasePos(speaker);
            stack_208 = talkBasePos(listener);
            if (bVar3) {
                stack_1e4.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_208 += stack_1cc;
            }
        }

        mViewCache.mCenter = stack_208;

        if (talk->field_0x44 == 0) {
            cXyz stack_214 = stack_1fc;
            stack_214.y = stack_1e4.y;
            cXyz stack_220 = stack_208;
            stack_220.y = stack_1f0.y;
            mViewCache.mDirection.Val(stack_214 - stack_220);
            if (iVar5 == 62) {
                mViewCache.mDirection.U(directionOf(speaker));
            }
            mViewCache.mDirection.R(125.0f);
            talk->field_0x1c.y = stack_1f0.y - 25.0f - stack_208.y;
            mStyleSettle.mFinished = true;
        }

        mViewCache.mCenter.y = stack_208.y + talk->field_0x1c.y;
        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mFovy = 60.0f;
        hideActor(actor1);
        break;

    case 39:
    case 40:
    case 64:
        if (iVar5 != 39) {
            actor1 = speaker;
            actor2 = listener;
        } else {
            actor1 = listener;
            actor2 = speaker;
        }

        if (talk->field_0x44 == 0) {
            if (iVar5 != 39) {
                stack_1e4 = talkEyePos(actor2);
                stack_1f0 = talkEyePos(actor1);
                stack_1fc = talkBasePos(actor2);
                stack_208 = talkBasePos(actor1);
                if (bVar3) {
                    stack_1f0.y = attentionPos(speaker).y;
                }
                if (bVar13) {
                    stack_1fc += stack_1cc;
                }
            } else {
                stack_1e4 = talkEyePos(actor2);
                stack_1f0 = talkEyePos(actor1);
                stack_1fc = talkBasePos(actor2);
                stack_208 = talkBasePos(actor1);
                if (bVar3) {
                    stack_1e4.y = attentionPos(speaker).y;
                }
                if (bVar13) {
                    stack_208 += stack_1cc;
                }
            }

            mViewCache.mCenter = stack_208;
            cXyz stack_22c = stack_1fc;
            stack_22c.y = stack_1e4.y;
            cXyz stack_238 = stack_208;
            stack_238.y = stack_1f0.y;
            mViewCache.mDirection.Val(stack_22c - stack_238);
            if (iVar5 == 64) {
                mViewCache.mDirection.U(directionOf(speaker));
            }
            mViewCache.mDirection.R(125.0f);
            mViewCache.mDirection.V(cSAngle(35.0f));
            talk->field_0x1c.y = stack_1f0.y - 25.0f - stack_208.y;
            mStyleSettle.mFinished = true;
            mViewCache.mCenter.y = stack_208.y + talk->field_0x1c.y;
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
            mViewCache.mFovy = 60.0f;
        }

        hideActor(actor2);
        break;

    case 16:
    case 17:
    case 61:
        if (iVar5 != 16) {
            actor1 = listener;
            stack_1e4 = talkEyePos(listener);
            stack_1f0 = talkEyePos(speaker);
            stack_1fc = talkBasePos(listener);
            stack_208 = talkBasePos(speaker);
            if (bVar3) {
                stack_1f0.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_1fc += stack_1cc;
            }
        } else {
            actor1 = speaker;
            stack_1e4 = talkEyePos(speaker);
            stack_1f0 = talkEyePos(listener);
            stack_1fc = talkBasePos(speaker);
            stack_208 = talkBasePos(listener);
            if (bVar3) {
                stack_1e4.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_208 += stack_1cc;
            }
        }

        mViewCache.mCenter = stack_208;

        if (talk->field_0x44 == 0) {
            cXyz stack_244 = stack_1fc;
            stack_244.y = stack_1e4.y;
            cXyz stack_250 = stack_208;
            stack_250.y = stack_1f0.y;
            mViewCache.mDirection.Val(stack_244 - stack_250);
            if (iVar5 == 61) {
                mViewCache.mDirection.U(directionOf(speaker));
            }
            mViewCache.mDirection.R(76.0f);
            talk->field_0x1c.y = stack_1f0.y - 10.0f - stack_208.y;
            mStyleSettle.mFinished = true;
        }

        mViewCache.mCenter.y = stack_208.y + talk->field_0x1c.y;
        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mFovy = 50.0f;
        hideActor(actor1);
        break;

    case 22:
    case 23:
    case 63:
        if (iVar5 != 22) {
            actor1 = listener;
            stack_1e4 = talkEyePos(listener);
            stack_1f0 = talkEyePos(speaker);
            stack_1fc = talkBasePos(listener);
            stack_208 = talkBasePos(speaker);
            if (bVar3) {
                stack_1f0.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_1fc += stack_1cc;
            }
        } else {
            actor1 = speaker;
            stack_1e4 = talkEyePos(speaker);
            stack_1f0 = talkEyePos(listener);
            stack_1fc = talkBasePos(speaker);
            stack_208 = talkBasePos(listener);
            if (bVar3) {
                stack_1e4.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_208 += stack_1cc;
            }
        }

        mViewCache.mCenter = stack_208;

        if (talk->field_0x44 == 0) {
            cXyz stack_25c = stack_1fc;
            stack_25c.y = stack_1e4.y;
            cXyz stack_268 = stack_1f0;
            stack_268.y = stack_1f0.y;
            mViewCache.mDirection.Val(stack_25c - stack_268);
            if (iVar5 == 63) {
                mViewCache.mDirection.U(directionOf(speaker));
            }
            mViewCache.mDirection.R(125.0f);
            talk->field_0x1c.y = stack_1f0.y - 15.0f - stack_208.y;
            mStyleSettle.mFinished = true;
        }

        mViewCache.mCenter.y = stack_208.y + talk->field_0x1c.y;
        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mFovy = 45.0f;
        hideActor(actor1);
        break;

    case 41:
    case 42:
    case 65: {
        if (iVar5 != 41) {
            actor1 = speaker;
            actor2 = listener;
            stack_1e4 = talkEyePos(listener);
            stack_1f0 = talkEyePos(speaker);
            stack_1fc = talkBasePos(listener);
            stack_208 = talkBasePos(speaker);
            if (bVar3) {
                stack_1f0.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_1fc += stack_1cc;
            }
        } else {
            actor1 = listener;
            actor2 = speaker;
            stack_1e4 = talkEyePos(speaker);
            stack_1f0 = talkEyePos(listener);
            stack_1fc = talkBasePos(speaker);
            stack_208 = talkBasePos(listener);
            if (bVar3) {
                stack_1e4.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_208 += stack_1cc;
            }
        }

        talk->field_0x90 = stack_208;

        if (talk->field_0x44 == 0) {
            cXyz stack_274 = stack_1fc;
            stack_274.y = stack_1e4.y;
            cXyz stack_280 = stack_1f0;
            stack_280.y = stack_1f0.y;
            talk->field_0xa8.Val(stack_274 - stack_280);
            talk->field_0xa8.R(190.0f);
            if (iVar5 == 0x41) {
                talk->field_0xa8.U(directionOf(speaker));
            }
            talk->field_0x1c.y = stack_1f0.y - 40.0f - stack_208.y;
            mStyleSettle.mFinished = true;
        }

        talk->field_0x90.y = stack_208.y + talk->field_0x1c.y;
        talk->field_0x9c = talk->field_0x90 + talk->field_0xa8.Xyz();
        talk->field_0xb0 = 45.0f;

        cXyz stack_28c = attentionPos(actor1);
        if (lineBGCheck(&stack_28c, &talk->field_0x9c, talk->field_0x8c)
            || lineCollisionCheck(stack_28c, talk->field_0x9c, listener, speaker, NULL))
        {
            iVar5 = 0;
        } else {
            mViewCache.mCenter = talk->field_0x90;
            mViewCache.mEye = talk->field_0x9c;
            mViewCache.mDirection = talk->field_0xa8;
            mViewCache.mFovy = talk->field_0xb0;
            hideActor(actor2);
        }
        break;
    }

    case 14:
    case 15: {
        if (iVar5 == 14) {
            actor1 = speaker;
            actor2 = listener;
            stack_1e4 = talkEyePos(listener);
            stack_1f0 = talkEyePos(speaker);
            cSGlobe stack_12d8 = talk->field_0x30;
            stack_1fc = talkBasePos(listener);
            stack_208 = talkBasePos(speaker);
            if (bVar3) {
                stack_1f0.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_1fc += stack_1cc;
            }
        } else {
            actor1 = listener;
            actor2 = speaker;
            stack_1e4 = talkEyePos(speaker);
            stack_1f0 = talkEyePos(listener);
            stack_1fc = talkBasePos(speaker);
            stack_208 = talkBasePos(listener);
            if (bVar3) {
                stack_1e4.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_208 += stack_1cc;
            }
        }

        mViewCache.mCenter = stack_208;
        mViewCache.mCenter.y = stack_1f0.y - 10.0f - talk->field_0x7c * 10.0f;

        if (talk->field_0x44 == 0) {
            mViewCache.mDirection.Val(stack_1e4 - stack_1f0);
            mViewCache.mDirection.R(mViewCache.mDirection.R() - 5.0f);
            mStyleSettle.mFinished = true;
        }

        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        f32 tmp1 = 35.0f;
        f32 tmp2 = 85.0f;
        mViewCache.mFovy = tmp2 + (tmp1 - tmp2) * talk->field_0x7c;
        cXyz stack_298 = attentionPos(actor1);
        hideActor(actor2);
        break;
    }

    case 18:
    case 19: {
        int uVar17;
        if (iVar5 == 18) {
            actor1 = listener;
            uVar17 = talk->field_0x38;
            stack_1e4 = talkEyePos(speaker);
            stack_1f0 = talkEyePos(listener);
            stack_1fc = talkBasePos(speaker);
            stack_208 = talkBasePos(listener);
            if (bVar3) {
                stack_1e4.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_208 += stack_1cc;
            }
        } else {
            actor1 = speaker;
            uVar17 = talk->field_0x38 ? 0 : 1;
            stack_1e4 = talkEyePos(listener);
            stack_1f0 = talkEyePos(speaker);
            stack_1fc = talkBasePos(listener);
            stack_208 = talkBasePos(speaker);
            if (bVar3) {
                stack_1f0.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_1fc += stack_1cc;
            }
        }

        if (talk->field_0x44 == 0) {
            mStyleSettle.mFinished = true;
            cXyz stack_2a4(0.0f, -15.0f, 15.0f);
            cSGlobe stack_12e0 = stack_1e4 - attentionPos(actor1);
            cSGlobe stack_12e8 = stack_2a4;
            stack_12e8.U(stack_12e8.U() + stack_12e0.U());
            talk->field_0x1c = stack_208 + stack_12e8.Xyz();
            talk->field_0x1c.y += stack_1f0.y - stack_208.y;
            f32 fVar36;
            if (uVar17) {
                fVar36 = -80.0f;
            } else {
                fVar36 = 75.0f;
            }
            cSAngle stack_137c = fVar36;
            talk->field_0x90 = talk->field_0x1c;
            talk->field_0xa8.Val(120.0f, cSAngle::_0, stack_137c + directionOf(actor1));
        }

        talk->field_0x9c = talk->field_0x90 + talk->field_0xa8.Xyz();
        talk->field_0xb0 = 50.0f;

        cXyz stack_2b0 = attentionPos(actor1);
        if (lineBGCheck(&stack_2b0, &talk->field_0x9c, talk->field_0x8c)
            || lineCollisionCheck(stack_2b0, talk->field_0x9c, listener, speaker, NULL))
        {
            iVar5 = 0;
        } else {
            mViewCache.mCenter = talk->field_0x90;
            mViewCache.mEye = talk->field_0x9c;
            mViewCache.mDirection = talk->field_0xa8;
            mViewCache.mFovy = talk->field_0xb0;
        }
        break;
    }

    case 24:
    case 25: {
        int uVar17;
        if (iVar5 == 24) {
            actor1 = listener;
            uVar17 = talk->field_0x38;
            stack_1e4 = talkEyePos(speaker);
            stack_1f0 = talkEyePos(listener);
            stack_1fc = talkBasePos(speaker);
            stack_208 = talkBasePos(listener);
            if (bVar3) {
                stack_1e4.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_208 += stack_1cc;
            }
        } else {
            actor1 = speaker;
            uVar17 = talk->field_0x38 ? 0 : 1;
            stack_1e4 = talkEyePos(listener);
            stack_1f0 = talkEyePos(speaker);
            stack_1fc = talkBasePos(listener);
            stack_208 = talkBasePos(speaker);
            if (bVar3) {
                stack_1f0.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_1fc += stack_1cc;
            }
        }

        if (talk->field_0x44 == 0) {
            mStyleSettle.mFinished = true;
            cXyz stack_2bc(0.0f, -10.0f, 20.0f);
            cSGlobe stack_12f0 = stack_1e4 - attentionPos(actor1);
            cSGlobe stack_12f8 = stack_2bc;
            stack_12f8.U(stack_12f8.U() + stack_12f0.U());
            talk->field_0x1c = stack_1f0 + stack_12f8.Xyz();
            f32 fVar36;
            if (uVar17) {
                fVar36 = -45.0f;
            } else {
                fVar36 = 45.0f;
            }
            cSAngle stack_1380 = fVar36;
            talk->field_0x90 = talk->field_0x1c;
            talk->field_0xa8.Val(120.0f, cSAngle(25.0f), stack_1380 + directionOf(actor1));
        }

        talk->field_0x9c = talk->field_0x90 + talk->field_0xa8.Xyz();
        talk->field_0xb0 = 45.0f;

        cXyz stack_2c8 = attentionPos(actor1);
        if (lineBGCheck(&stack_2c8, &talk->field_0x9c, talk->field_0x8c)
            || lineCollisionCheck(stack_2c8, talk->field_0x9c, listener, speaker, NULL))
        {
            iVar5 = 0;
        } else {
            mViewCache.mCenter = talk->field_0x90;
            mViewCache.mEye = talk->field_0x9c;
            mViewCache.mDirection = talk->field_0xa8;
            mViewCache.mFovy = talk->field_0xb0;
        }
        break;
    }

    case 26:
    case 27: {
        int uVar17;
        if (iVar5 != 26) {
            actor1 = listener;
            actor2 = speaker;
            uVar17 = talk->field_0x38;
            stack_1e4 = talkEyePos(speaker);
            stack_1f0 = talkEyePos(listener);
            stack_1fc = talkBasePos(speaker);
            stack_208 = talkBasePos(listener);
            if (bVar3) {
                stack_1e4.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_208 += stack_1cc;
            }
        } else {
            actor1 = speaker;
            actor2 = listener;
            uVar17 = talk->field_0x38 ? 0 : 1;
            stack_1e4 = talkEyePos(listener);
            stack_1f0 = talkEyePos(speaker);
            stack_1fc = talkBasePos(listener);
            stack_208 = talkBasePos(speaker);
            if (bVar3) {
                stack_1f0.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_1fc += stack_1cc;
            }
        }

        if (talk->field_0x44 == 0) {
            mStyleSettle.mFinished = true;
            cXyz stack_2d4(0.0f, -30.0f, 20.0f);
            if (bVar3 && iVar5 == 27) {
                stack_2d4.y = -5.0f;
            }
            cSGlobe stack_1300 = attentionPos(actor2) - attentionPos(actor1);
            cSGlobe stack_1308 = stack_2d4;
            stack_1308.U(stack_1308.U() + stack_1300.U());
            talk->field_0x1c = stack_1f0 + stack_1308.Xyz();
            f32 fVar36;
            if (uVar17) {
                fVar36 = -30.0f;
            } else {
                fVar36 = 35.0f;
            }
            cSAngle stack_1384 = fVar36;
            if (bVar3) {
                fVar36 = 0.0f;
            } else {
                fVar36 = -35.0f;
            }
            cSAngle stack_1388 = fVar36;
            talk->field_0x90 = talk->field_0x1c;
            talk->field_0xa8.Val(90.0f, cSAngle(-35.0f), stack_1384 + directionOf(actor1));
        }

        talk->field_0x9c = talk->field_0x90 + talk->field_0xa8.Xyz();
        talk->field_0xb0 = 55.0f;

        cXyz stack_2e0 = attentionPos(actor1);
        if (lineBGCheck(&stack_2e0, &talk->field_0x9c, talk->field_0x8c)
            || lineCollisionCheck(stack_2e0, talk->field_0x9c, listener, speaker, NULL))
        {
            iVar5 = 0;
        } else {
            mViewCache.mCenter = talk->field_0x90;
            mViewCache.mEye = talk->field_0x9c;
            mViewCache.mDirection = talk->field_0xa8;
            mViewCache.mFovy = talk->field_0xb0;
        }
        break;
    }

    case 11:
    case 12:
    case 30:
    case 31: {
        fopAc_ac_c* actor;
        int uVar17;
        if (iVar5 != 11 && iVar5 != 30) {
            actor = speaker;
            uVar17 = talk->field_0x38;
            stack_1e4 = talkEyePos(listener);
            stack_1f0 = talkEyePos(speaker);
            stack_1fc = talkBasePos(listener);
            stack_208 = talkBasePos(speaker);
            if (bVar3) {
                stack_1f0.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_1fc += stack_1cc;
            }
        } else {
            actor = listener;
            uVar17 = talk->field_0x38 ? 0 : 1;
            stack_1e4 = talkEyePos(speaker);
            stack_1f0 = talkEyePos(listener);
            stack_1fc = talkBasePos(speaker);
            stack_208 = talkBasePos(listener);
            if (bVar3) {
                stack_1e4.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_208 += stack_1cc;
            }
        }

        if (talk->field_0x44 == 0) {
            f32 fVar36 = (stack_1f0.y - talkBasePos(actor).y) * 1.2f;
            f32 fVar37 = (stack_1e4.y - stack_1f0.y) * 0.45f;
            f32 fVar38 = fVar36 * 0.7f + fVar37;
            cXyz stack_2ec(25.0f, 10.0f, talk->field_0x30.R() * 0.45f);
            cXyz stack_2f8(75.0f, fVar37, -75.0f);
            if (uVar17) {
                stack_2ec.x = -stack_2ec.x;
                stack_2f8.x = -stack_2f8.x;
            }
            cSGlobe stack_1310 = stack_1e4 - stack_1f0;
            cSGlobe stack_1318 = stack_2ec;
            stack_1318.U(stack_1318.U() + stack_1310.U());
            talk->field_0x90 = attentionPos(actor) + stack_1318.Xyz();
            talk->field_0x90.y = stack_208.y + fVar38;
            stack_1318.Val(stack_2f8);
            stack_1318.U(stack_1318.U() + stack_1310.U().Inv());
            stack_1318.V(stack_1318.V() * 0.25f + stack_1310.V() * 0.75f);
            talk->field_0x9c = stack_1e4 + stack_1318.Xyz();
            talk->field_0xa8.Val(talk->field_0x9c - talk->field_0x90);
            mStyleSettle.mFinished = true;
            if (iVar5 == 11 || iVar5 == 12) {
                talk->field_0xb0 = 55.0f;
            } else {
                talk->field_0xb0 = 65.0f;
            }
        }

        cXyz stack_304 = attentionPos(actor);
        if (lineBGCheck(&stack_304, &talk->field_0x9c, talk->field_0x8c)
            || lineCollisionCheck(stack_304, talk->field_0x9c, listener, speaker, NULL))
        {
            iVar5 = 0;
        } else {
            mViewCache.mCenter = talk->field_0x90;
            mViewCache.mEye = talk->field_0x9c;
            mViewCache.mDirection = talk->field_0xa8;
            mViewCache.mFovy = talk->field_0xb0;
        }
        break;
    }

    case 13: {
        if (talk->field_0x44 == 0) {
            mStyleSettle.mFinished = true;
        }
        mViewCache.mCenter = talk->field_0x4;
        mViewCache.mDirection.Val(400.0f, cSAngle(35.0f), talk->field_0x28.U());
        if (mViewCache.mDirection.V() > stack_134c) {
            mViewCache.mDirection.V(stack_134c);
        }
        if (mViewCache.mDirection.V() < stack_1350) {
            mViewCache.mDirection.V(stack_1350);
        }
        cXyz stack_310 = attentionPos(listener);
        cXyz stack_31c = attentionPos(speaker);
        cSAngle stack_138c;
        if (talk->field_0x38) {
            stack_138c = 20.0f;
        } else {
            stack_138c = -20.0f;
        }

        for (int i = 0; i < 18; i++) {
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
            if (!lineBGCheck(&stack_310, &mViewCache.mEye, talk->field_0x8c)
                && !lineBGCheck(&stack_31c, &mViewCache.mEye, talk->field_0x8c)
                && !lineCollisionCheck(stack_310, mViewCache.mEye, listener, speaker, NULL)
                && !lineCollisionCheck(stack_31c, mViewCache.mEye, listener, speaker, NULL))
            {
                break;
            }
            mViewCache.mDirection.U(mViewCache.mDirection.U() + stack_138c);
        }

        mViewCache.mFovy = 60.0f;
        break;
    }

    case 32:
        if (talk->field_0x44 == 0) {
            mStyleSettle.mFinished = true;
            mViewCache.mCenter = talk->field_0x4;
            cSAngle stack_1390;
            if (talk->field_0x38) {
                stack_1390 = talk->field_0x30.U() + cSAngle::_90;
            } else {
                stack_1390 = talk->field_0x30.U() + cSAngle::_270;
            }
            mViewCache.mDirection.Val(400.0f, cSAngle(15.0f), stack_1390);
            if (mViewCache.mDirection.V() > stack_134c) {
                mViewCache.mDirection.V(stack_134c);
            }
            if (mViewCache.mDirection.V() < stack_1350) {
                mViewCache.mDirection.V(stack_1350);
            }
            cXyz stack_328 = attentionPos(listener);
            cXyz stack_334 = attentionPos(speaker);
            cSAngle stack_1394 = cSAngle::_0;

            for (int i = 0; i < 18; i++) {
                mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
                if (!lineBGCheck(&stack_328, &mViewCache.mEye, talk->field_0x8c)
                    && !lineBGCheck(&stack_334, &mViewCache.mEye, talk->field_0x8c)
                    && !lineCollisionCheck(stack_328, mViewCache.mEye, listener, speaker, NULL)
                    && !lineCollisionCheck(stack_334, mViewCache.mEye, listener, speaker, NULL))
                {
                    break;
                }
                if ((i & 1) == 0) {
                    stack_1394 += cSAngle(20.0f);
                    mViewCache.mDirection.U(stack_1390 + stack_1394);
                } else {
                    mViewCache.mDirection.U(stack_1390 - stack_1394);
                }
            }

            mViewCache.mFovy = 60.0f;
        }
        break;

    case 28: {
        if (talk->field_0x44 == 0) {
            mStyleSettle.mFinished = true;
        }
        cXyz stack_340 = talkBasePos(speaker);
        cXyz stack_34c = talkBasePos(listener);
        cXyz stack_358 = stack_340 - stack_34c;
        cSGlobe stack_1320 = stack_358;
        cSAngle stack_1398;
        f32 dVar21 = dCamMath::xyzHorizontalDistance(stack_340, stack_34c) * 2.0f * 0.5f;
        cDegree stack_1348 = mWindowAspect * 60.0f * 0.5f;
        dVar21 /= stack_1348.Tan();
        if (talk->field_0x38) {
            stack_1398 = cSAngle::_270;
        } else {
            stack_1398 = cSAngle::_90;
        }
        talk->field_0x90 = talkBasePos(listener) + stack_358 * 0.5f;
        talk->field_0x90.y = (talkEyePos(speaker).y + talkEyePos(listener).y) * 0.5f - 30.0f;
        talk->field_0xa8.Val(dVar21, cSAngle::_0, stack_1320.U() + stack_1398);
        talk->field_0x9c = talk->field_0x90 + talk->field_0xa8.Xyz();
        talk->field_0xb0 = 60.0f;

        if (lineBGCheck(&stack_340, &talk->field_0x9c, talk->field_0x8c)
            || lineBGCheck(&stack_34c, &talk->field_0x9c, talk->field_0x8c)
            || lineCollisionCheck(stack_340, talk->field_0x9c, listener, speaker, NULL)
            || lineCollisionCheck(stack_34c, talk->field_0x9c, listener, speaker, NULL))
        {
            iVar5 = 0;
        } else {
            mViewCache.mCenter = talk->field_0x90;
            mViewCache.mEye = talk->field_0x9c;
            mViewCache.mDirection = talk->field_0xa8;
            mViewCache.mFovy = talk->field_0xb0;
        }
        break;
    }

    case 29: {
        talk->field_0x90 = talkBasePos(speaker);

        if (talk->field_0x44 == 0) {
            cXyz stack_364 = talkBasePos(listener);
            stack_364.y = talkEyePos(listener).y;
            cXyz stack_370 = talkBasePos(speaker);
            if (bVar3) {
                stack_370.y = attentionPos(speaker).y;
            }
            stack_370.y = talkEyePos(speaker).y;
            talk->field_0xa8.Val(stack_364 - stack_370);
            talk->field_0xa8.R(200.0f);
            talk->field_0x1c.y = attentionPos(speaker).y - 68.0f - talkBasePos(speaker).y;
            mStyleSettle.mFinished = true;
        }

        talk->field_0x90.y = talk->field_0x1c.y + talkBasePos(speaker).y;
        talk->field_0x9c = talk->field_0x90 + talk->field_0xa8.Xyz();
        talk->field_0xb0 = 55.0f;

        cXyz stack_37c = attentionPos(speaker);
        if (lineBGCheck(&stack_37c, &talk->field_0x9c, talk->field_0x8c)
            || lineCollisionCheck(stack_37c, talk->field_0x9c, listener, speaker, NULL))
        {
            iVar5 = 0;
        } else {
            mViewCache.mCenter = talk->field_0x90;
            mViewCache.mEye = talk->field_0x9c;
            mViewCache.mDirection = talk->field_0xa8;
            mViewCache.mFovy = talk->field_0xb0;
            hideActor(listener);
        }
        break;
    }

    case 37:
        if (talk->field_0x44 == 0) {
            stack_1e4 = talkEyePos(listener);
            stack_1f0 = talkEyePos(speaker);
            stack_1fc = talkBasePos(listener);
            stack_208 = talkBasePos(speaker);
            if (bVar3) {
                stack_1f0.y = attentionPos(speaker).y;
            }
            if (bVar13) {
                stack_1fc += stack_1cc;
            }
            mViewCache.mCenter = stack_208;
            cXyz stack_388 = stack_1fc;
            stack_388.y = stack_1e4.y;
            cXyz stack_394 = stack_208;
            stack_394.y = stack_1f0.y;
            mViewCache.mDirection.Val(stack_388 - stack_394);
            mViewCache.mDirection.V(mViewCache.mDirection.V() + cSAngle(5.0f));
            mViewCache.mDirection.R(750.0f);
            talk->field_0x1c.y = stack_1f0.y - stack_208.y;
            mStyleSettle.mFinished = true;
            mViewCache.mCenter.y = stack_208.y + talk->field_0x1c.y;

            cXyz stack_3a0 = attentionPos(listener);
            cXyz stack_3ac = attentionPos(speaker);
            cSAngle stack_139c;
            if (talk->field_0x38) {
                mViewCache.mDirection.U(mViewCache.mDirection.U() + cSAngle(10.0f));
                stack_139c = 20.0f;
            } else {
                mViewCache.mDirection.U(mViewCache.mDirection.U() - cSAngle(10.0f));
                stack_139c = -20.0f;
            }

            int i;
            fopAc_ac_c* midna = daPy_py_c::getMidnaActor();
            for (i = 0; i < 18; i++) {
                mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
                if (!lineBGCheck(&stack_3a0, &mViewCache.mEye, talk->field_0x8c)
                    && !lineBGCheck(&stack_3ac, &mViewCache.mEye, talk->field_0x8c)
                    && !lineCollisionCheck(stack_3a0, mViewCache.mEye, listener, midna, NULL)
                    && !lineCollisionCheck(stack_3ac, mViewCache.mEye, listener, midna, NULL))
                {
                    break;
                }
                mViewCache.mDirection.U(mViewCache.mDirection.U() + stack_139c);
            }

            mViewCache.mFovy = 60.0f;
        }
        break;

    case 38: {
        stack_1e4 = talkEyePos(listener);
        stack_1f0 = talkEyePos(speaker);
        cSGlobe stack_1328 = stack_1e4 - stack_1f0;

        if (talk->field_0x44 == 0) {
            mStyleSettle.mFinished = true;
            cSGlobe stack_1330 = stack_1328;
            stack_1330.R(stack_1328.R() * 0.5f);
            mViewCache.mCenter = stack_1f0 + stack_1330.Xyz();
            mViewCache.mCenter.y = stack_1f0.y - 20.0f - talk->field_0x7c * 10.0f;
            if (talk->field_0x38) {
                stack_1330.Val(cXyz(-45.0f + talk->field_0x7c * 20.0f, 5.0f, -80.0f - talk->field_0x7c * 40.0f));
            } else {
                stack_1330.Val(cXyz(65.0f - talk->field_0x7c * 20.0f, 5.0f, -80.0f - talk->field_0x7c * 40.0f));
            }
            stack_1330.U(stack_1330.U() + directionOf(listener));
            stack_1330.V(stack_1330.V() + stack_1328.V());
            mViewCache.mEye = stack_1e4 + stack_1330.Xyz();
            mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
            f32 tmp1 = 25.0f;
            f32 tmp2 = 40.0f;
            mViewCache.mFovy = tmp2 + (tmp1 - tmp2) * talk->field_0x7c;
        }
        break;
    }

    case 33: {
        cXyz stack_3b8(0.0f, 25.0f, -70.0f);
        cSGlobe stack_1338 = stack_3b8;
        stack_1338.U(stack_1338.U() + directionOf(listener));
        mViewCache.mCenter = attentionPos(listener) + stack_1338.Xyz();
        mViewCache.mDirection.Val(140.0f, cSAngle(-20.0f), cSAngle(-40.0f) + directionOf(listener));
        mStyleSettle.mFinished = true;
        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mFovy = 58.0f;
        break;
    }

    case 34:
        mViewCache.mCenter = talkBasePos(listener);

        if (talk->field_0x44 == 0) {
            mViewCache.mDirection.V(cSAngle(20.0f));
            mViewCache.mDirection.R(160.0f);
            mViewCache.mDirection.U(directionOf(listener).Inv());
            talk->field_0x1c.y = 95.0f;
            mStyleSettle.mFinished = true;
        }

        mViewCache.mCenter.y += talk->field_0x1c.y;
        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mFovy = 58.0f;
        break;

    case 35:
    case 36: {
        stack_1e4 = attentionPos(speaker);
        stack_1f0 = attentionPos(listener);
        stack_1fc = talkBasePos(speaker);
        stack_208 = talkBasePos(listener);

        if (talk->field_0x44 == 0) {
            mStyleSettle.mFinished = true;
        }

        cXyz stack_3c4(0.0f, 10.0f, -60.0f);
        cSGlobe stack_1340 = stack_3c4;
        stack_1340.U(stack_1340.U() + directionOf(listener));
        talk->field_0x90 = stack_1f0 + stack_1340.Xyz();
        cSAngle stack_13a0;

        if (iVar5 == 36) {
            stack_13a0.Val(-150.0f);
            talk->field_0xa8.Val(200.0f, cSAngle::_0, stack_13a0 + directionOf(listener));
        } else {
            stack_13a0.Val(-35.0f);
            talk->field_0xa8.Val(160.0f, cSAngle::_0, stack_13a0 + directionOf(listener));
        }

        talk->field_0xb0 = 60.0f;
        talk->field_0x9c = talk->field_0x90 + talk->field_0xa8.Xyz();

        cXyz stack_3d0 = attentionPos(listener);
        if (lineBGCheck(&stack_3d0, &talk->field_0x9c, talk->field_0x8c)
            || lineCollisionCheck(stack_3d0, talk->field_0x9c, listener, speaker, NULL))
        {
            talk->field_0x90.y -= 20.0f;
            talk->field_0xa8.Val(180.0f, cSAngle(35.0f), stack_13a0 + directionOf(listener));
            talk->field_0x9c = talk->field_0x90 + talk->field_0xa8.Xyz();

            if (lineBGCheck(&stack_3d0, &talk->field_0x9c, talk->field_0x8c)
                || lineCollisionCheck(stack_3d0, talk->field_0x9c, listener, speaker, NULL))
            {
                iVar5 = 0;
            } else {
                mViewCache.mCenter = talk->field_0x90;
                mViewCache.mEye = talk->field_0x9c;
                mViewCache.mDirection = talk->field_0xa8;
                mViewCache.mFovy = talk->field_0xb0;
            }
        } else {
            mViewCache.mCenter = talk->field_0x90;
            mViewCache.mEye = talk->field_0x9c;
            mViewCache.mDirection = talk->field_0xa8;
            mViewCache.mFovy = talk->field_0xb0;
        }
        break;
    }

    default:
        iVar5 = 0;
        break;
    }

    if (talk->field_0x84 != 0) {
        mStyleSettle.mFinished = true;
    }

    if (iVar5 == 0) {
        if (mStyleSettle.mFinished) {
            mViewCache.mCenter = talk->field_0x4;
            mViewCache.mDirection = talk->field_0x28;
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
            mViewCache.mFovy = talk->field_0x58;
        } else {
            talk->field_0x50 = (f32)(talk->field_0x44 + 1) / talk->field_0x48;
            f32 dVar21 = dCamMath::rationalBezierRatio(talk->field_0x50, 0.28f);
            mViewCache.mCenter += (talk->field_0x4 - mViewCache.mCenter) * dVar21;
            mViewCache.mDirection.R(mViewCache.mDirection.R()
                                + (talk->field_0x28.R() - mViewCache.mDirection.R()) * dVar21);
            mViewCache.mDirection.V(mViewCache.mDirection.V()
                                + (talk->field_0x28.V() - mViewCache.mDirection.V()) * dVar21);
            mViewCache.mDirection.U(mViewCache.mDirection.U()
                                + (talk->field_0x28.U() - mViewCache.mDirection.U()) * dVar21);
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
            mViewCache.mFovy += (talk->field_0x58 - mViewCache.mFovy) * dVar21;
            talk->field_0x4c -= talk->field_0x50;
            if (talk->field_0x44 >= talk->field_0x48 - 1) {
                mStyleSettle.mFinished = true;
            }
            uVar18 = false;
        }
    }

    talk->field_0x44++;
    return uVar18;
}

bool dCamera_c::CalcSubjectAngle(s16* param_0, s16* param_1) {
    return false;
}

bool dCamera_c::SaveZoomRatio() {
    if (mCamParam.Algorythmn() != 4) {
        return false;
    }

    SubjectData* subject = (SubjectData*)mWork;

    mZoomRatio = subject->mZoomRatio;
    return true;
}

bool dCamera_c::subjectCamera(s32 param_0) {
    f32 val0 = mCamParam.Val(param_0, 0);
    f32 val2 = mCamParam.Val(param_0, 2);
    f32 val1 = mCamParam.Val(param_0, 1);
    f32 val25 = mCamParam.Val(param_0, 25);
    f32 val4 = mCamParam.Val(param_0, 4);
    f32 val26 = mCamParam.Val(param_0, 26);
    f32 val20 = mCamParam.Val(param_0, 20);
    f32 val19 = mCamParam.Val(param_0, 19);
    f32 val21 = mCamParam.Val(param_0, 21);
    f32 val7 = mCamParam.Val(param_0, 7);
    f32 val10 = mCamParam.Val(param_0, 10);
    f32 val17 = mCamParam.Val(param_0, 17);
    f32 val27 = mCamParam.Val(param_0, 27);
    f32 val22 = mCamParam.Val(param_0, 22);
    f32 val16 = mCamParam.Val(param_0, 16);
    f32 val24 = mCamParam.Val(param_0, 24);
    f32 val18 = mCamParam.Val(param_0, 18);

    daAlink_c* player = (daAlink_c*)mpPlayerActor;

    SubjectData* subject = (SubjectData*)mWork;

    bool bVar13 = check_owner_action(mPadID, 0x1040) != 0;
    bool bVar14 = check_owner_action(mPadID, 0x4000) != 0;
    bool bVar15 = check_owner_action(mPadID, 0x400) != 0;
    bool magne_boots_on = player->checkMagneBootsOn() != 0;
    check_owner_action(mPadID, 0x80080);
    bool bVar16 = check_owner_action(mPadID, 0x40) != 0;

    if (mCurCamStyleTimer == 0) {
        subject->field_0x0 = 'SUBN';
        subject->field_0x2a.Val(mViewCache.mDirection.U().Inv());
        subject->field_0x28.Val(mViewCache.mDirection.V());
        subject->field_0x14 = 7;
        if (check_owner_action1(mPadID, 0x2000000)) {
            subject->field_0x14 = 2;
        }
        subject->field_0x10 = 0;
        subject->mZoomRatio = mZoomRatio;
        mZoomRatio = 0.0f;
        subject->field_0x18 = param_0;
        subject->field_0x1c = false;
        subject->field_0x20 = 0.0f;
        subject->field_0x1d = false;

        if (bVar13 || bVar15) {
            subject->field_0x1d = directionOf(mpPlayerActor) - mViewCache.mDirection.U() > cSAngle::_0;
        } else if (bVar14) {
            subject->field_0x1d = player->getHookshotLeft();
        }
    }

    if (subject->field_0x1d) {
        val0 = -val0;
    }

    cXyz stack_1bc;
    cSAngle stack_448;
    MtxP mtx, inv_mtx;
    if (magne_boots_on) {
        stack_1bc = *player->getMagneBootsTopVec();
        mtx = player->getMagneBootsMtx();
        inv_mtx = player->getMagneBootsInvMtx();
        stack_448 = cSAngle(player->getMagneBootsModelShapeAngle());
    } else {
        stack_1bc = cXyz::BaseY;
        mtx = mDoMtx_getIdentity();
        inv_mtx = mDoMtx_getIdentity();
        stack_448 = directionOf(mpPlayerActor);
    }

    cXyz stack_1c8, stack_1d4, stack_1e0;
    s16 bow_angle_x, bow_angle_y;
    cSAngle angle_x = player->getCameraAngleX();
    cSAngle angle_y = player->getCameraAngleY();
    cXyz* bow_pos = player->checkBowCameraArrowPosP(&bow_angle_x, &bow_angle_y);

    if (check_owner_action(mPadID, 0x200000) && bow_pos != NULL) {
        stack_1c8 = *bow_pos;
        angle_x.Val(bow_angle_x);
        angle_y.Val(bow_angle_y);
    } else if (bVar14) {
        if (player->getHookshotLeft()) {
            stack_1c8 = player->getLeftHandPos();
        } else {
            stack_1c8 = player->getRightHandPos();
        }
    } else if (player->checkIronBallThrowReturnMode()) {
        stack_1c8 = attentionPos(mpPlayerActor);
        cXyz* iron_ball_pos = player->getIronBallCenterPos();
        cSGlobe globe = stack_1c8 - *iron_ball_pos;
        f32 tmp = cXyz(*iron_ball_pos - stack_1c8).abs();
        if (tmp > 200.0f) {
            angle_x = globe.V();
            angle_y = globe.U().Inv();
        } else {
            angle_x = subject->field_0x28;
            angle_y = subject->field_0x2a;
        }
        angle_x += cSAngle(5.0f);
    } else if (bVar15) {
        stack_1c8 = attentionPos(mpPlayerActor);
        subject->field_0x28 = angle_x;
        subject->field_0x2a = angle_y;
        angle_x += cSAngle(5.0f);
    } else {
        stack_1c8 = *player->getSubjectEyePos();
    }

    subject->field_0x2e = angle_x;
    subject->field_0x2c = angle_y;

    if (magne_boots_on) {
        cXyz player_pos = positionOf(mpPlayerActor);
        subject->field_0x4 = mViewCache.mCenter - player_pos;
        subject->field_0x30 = mViewCache.mEye - player_pos;
        mDoMtx_multVecSR(inv_mtx, &subject->field_0x4, &subject->field_0x4);
        mDoMtx_multVecSR(inv_mtx, &subject->field_0x30, &subject->field_0x30);
        subject->field_0x4 += player_pos;
        subject->field_0x30 += player_pos;
        subject->field_0x48.Val(subject->field_0x30 - subject->field_0x4);
    }

    if (mCurCamStyleTimer == 0) {
        subject->field_0x4 = mViewCache.mCenter;
        subject->field_0x30 = mViewCache.mEye;
        subject->field_0x48 = mViewCache.mDirection;
        subject->field_0x3c = mUp;
        field_0x738 = val16;
    }

    bool bVar17 = false;
    if (mGear == -1) {
        bVar17 = true;
    }

    if (mCurType == specialType[CAM_TYPE_SCOPE]) {
        mGear = 0;
        mCamParam.SetFlag(0x10);
        mCamParam.SetFlag(4);
    } else if (mCurMode == 4) {
        mGear = -1;
        if (mPadInfo.mCStick.mLastPosY < -mCamSetup.mCStick.SwTHH()) {
            if (mCStickYState != -1 && mGear == -1) {
                mGear = 0;
                setComStat(0x2000);
            }
            mCStickYState = -1;
        } else {
            mCStickYState = 0;
        }
    } else if (bVar15 || player->checkIronBallThrowReturnMode()) {
        val0 = 0.0f;
        val2 = 40.0f;
        val1 = 50.0f;
        val7 = 270.0f;
        val17 = 70.0f;
    } else if (bVar14 || bVar13) {
        if (mGear == -1) {
            bVar17 = true;
        }

        if (bVar17) {
            val16 = subject->field_0x20;
            subject->field_0x20 = val16 + (1.0f - val16) * 0.3f;
        } else {
            val16 = subject->field_0x20;
            subject->field_0x20 = val16 + (0.0f - val16) * 0.3f;
        }

        subject->field_0x20 = 1.0f;
        mCamParam.SetFlag(0x10);
        mCamParam.SetFlag(4);

        if (bVar14) {
            val20 = player->getHookshotLeft() ? val20 : -val20;
            val0 += (val20 - val0) * subject->field_0x20;
            val2 += (val19 - val2) * subject->field_0x20;
            val1 += (val21 - val1) * subject->field_0x20;
            val7 += (val10 - val7) * subject->field_0x20;
        } else if (bVar16) {
            val0 += (19.0f - val0) * subject->field_0x20;
            val2 += (0.0f - val2) * subject->field_0x20;
            val1 += (val26 - val1) * subject->field_0x20;
            val7 += (val10 - val7) * subject->field_0x20;
        } else if (bVar13) {
            val0 += (val25 - val0) * subject->field_0x20;
            val2 += (val4 - val2) * subject->field_0x20;
            val1 += (val26 - val1) * subject->field_0x20;
            val7 += (val10 - val7) * subject->field_0x20;
        } else {
            val0 += (0.0f - val0) * subject->field_0x20;
            val2 += (0.0f - val2) * subject->field_0x20;
            val1 += (0.0f - val1) * subject->field_0x20;
            val7 += (20.0f - val7) * subject->field_0x20;
        }
    }

    if (mCurMode == 4 && getComStat(0x800)) {
        val2 = 50.0f;
    } else if (player->checkHawkWait()) {
        val7 = 150.0f;
        val2 = 20.0f;
    }

    if (mCamParam.Flag(param_0, 0x4000)) {
        setFlag(0x800);
    }
    if (mCamParam.Flag(param_0, 0x20)) {
        setFlag(0x10000000);
    }

    cXyz stack_1f8(val0, val2, val1);
    stack_1d4 = dCamMath::xyzRotateX(stack_1f8, angle_x);
    stack_1f8 = dCamMath::xyzRotateY(stack_1d4, angle_y);
    f32 tmp = bVar15 ? 40.0f : 0.0f;
    cXyz stack_204(0.0f, tmp, -val7);
    stack_1d4 = dCamMath::xyzRotateX(stack_204, angle_x);
    stack_204 = dCamMath::xyzRotateY(stack_1d4, angle_y);

    if (magne_boots_on) {
        mDoMtx_multVecSR(mtx, &stack_1f8, &stack_1f8);
        mDoMtx_multVecSR(mtx, &stack_204, &stack_204);
    }

    f32 fVar1 = 1.0f;
    if (player->checkIronBallThrowReturnMode()) {
        fVar1 = 0.1f;
    } else if (player->checkHorseRide() || player->checkCanoeRide()) {
        fVar1 = 1.0f;
    }

    cXyz stack_210 = stack_1c8 + stack_1f8;
    dBgS_CamLinChk lin_chk;
    if (mIsWolf == 1) {
        cXyz stack_21c = positionOf(mpPlayerActor);
        stack_21c.y = stack_1c8.y;
        if (lineBGCheck(&stack_21c, &stack_210, &lin_chk, 0x40b7)) {
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
            stack_210 = lin_chk.GetCross();
            stack_210 += *plane.GetNP() * 5.0f;
        }
    }

    stack_210 = mViewCache.mCenter + (stack_210 - mViewCache.mCenter) * fVar1;
    cXyz stack_228 = stack_210 + stack_204;
    stack_228 = mViewCache.mEye + (stack_228 - mViewCache.mEye) * fVar1;

    if (magne_boots_on) {
        setFlag(0x10);
        stack_1e0 = stack_1bc;
    } else {
        stack_1e0 = cXyz::BaseY;
    }

    cXyz stack_234 = stack_228 - stack_210;
    cXyz stack_240, stack_24c;
    stack_234.normalize();
    stack_240 = stack_210 + stack_234 * 40.0f;
    if (lineBGCheck(&stack_240, &stack_210, &stack_24c, 0x40b7)) {
        stack_228 = stack_24c + stack_234 * 10.0f;
    }

    if (mStyleSettle.mFinished) {
        mViewCache.mCenter = stack_210;
        mViewCache.mEye = stack_228;
    } else {
        cSGlobe stack_43c = stack_228 - stack_210;
        f32 tmp = 1.0f / (subject->field_0x14 - subject->field_0x10);
        mViewCache.mCenter += (stack_210 - mViewCache.mCenter) * tmp;
        mViewCache.mDirection.R(mViewCache.mDirection.R() + (stack_43c.R() - mViewCache.mDirection.R()) * tmp);
        mViewCache.mDirection.V(mViewCache.mDirection.V() + (stack_43c.V() - mViewCache.mDirection.V()) * tmp);
        mViewCache.mDirection.U(mViewCache.mDirection.U() + (stack_43c.U() - mViewCache.mDirection.U()) * tmp);
        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mFovy += (val17 - mViewCache.mFovy) * tmp;
        subject->field_0x3c += (stack_1e0 - subject->field_0x3c) * tmp;
        stack_1e0 = subject->field_0x3c.norm();

        if (subject->field_0x10 == subject->field_0x14 - 1) {
            mStyleSettle.mFinished = true;
        }
        subject->field_0x10++;

        if (magne_boots_on) {
            mUpOverride.field_0x0 = mViewCache.mCenter;
            mUpOverride.field_0xc = mViewCache.mEye;
            mUpOverride.field_0x24 = mViewCache.mDirection;
            mUpOverride.field_0x18 = stack_1e0;
        }

        return true;
    }

    mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
    mUpOverride.field_0x0 = mViewCache.mCenter;
    mUpOverride.field_0xc = mViewCache.mEye;
    mUpOverride.field_0x24 = mViewCache.mDirection;
    mUpOverride.field_0x18 = stack_1e0;

    if (mCamParam.Flag(param_0, 0x400)) {
        f32 dVar17 = 0.0f;
        f32 dVar16 = 0.0f;
        int iVar2 = 0;
        if (mPadInfo.mCStick.mLastPosY > 0.01f) {
            dVar17 = dCamMath::rationalBezierRatio(mPadInfo.mCStick.mLastPosY, mCamSetup.CurveWeight());
            iVar2 = -1;
        } else if (mPadInfo.mCStick.mLastPosY < -0.01f) {
            dVar16 = dCamMath::rationalBezierRatio(-mPadInfo.mCStick.mLastPosY, mCamSetup.CurveWeight());
            iVar2 = 1;
        }
        f32 tmp = subject->mZoomRatio + val18 * (dVar17 - dVar16) * 0.1f;

        if (tmp < 0.0f) {
            subject->mZoomRatio = 0.0f;
        } else if (tmp > 1.0f) {
            subject->mZoomRatio = 1.0f;
        } else {
            subject->mZoomRatio = tmp;
            if (iVar2 == -1) {
                mDoAud_seStartLevel(Z2SE_AL_HAWK_EYE_ZOOMIN, NULL, 0, 0);
            } else if (iVar2 == 1) {
                mDoAud_seStartLevel(Z2SE_AL_HAWK_EYE_ZOOMOUT, NULL, 0, 0);
            }
        }

        if (subject->mZoomRatio == 0.0f || subject->mZoomRatio == 0.5f || subject->mZoomRatio == 1.0f) {
            dVar17 = 0.0f;
            dVar16 = 0.0f;
        }

        f32 tmp2 = subject->mZoomRatio * 8.0f + 1.0f;
        f32 zoom_fovy = dCamMath::zoomFovy(val17 * 0.5f, tmp2) * 2.0f;
        mViewCache.mFovy += (zoom_fovy - mViewCache.mFovy) * val22;
        setComZoomScale(tmp2);
        setComZoomForcus(1.0f - fabsf(dVar17 - dVar16) * -511.0f);
        if (check_owner_action(mPadID, 0x200000)) {
            setComStat(8);
        }

    } else {
        mViewCache.mFovy = val17;
    }

    return true;
}

bool dCamera_c::magneCamera(s32 param_0) {
    f32 val5 = mCamParam.Val(param_0, 5);
    f32 val0 = mCamParam.Val(param_0, 0);
    f32 val1 = mCamParam.Val(param_0, 1);
    f32 val2 = mCamParam.Val(param_0, 2);
    f32 val3 = mCamParam.Val(param_0, 3);
    f32 val7 = mCamParam.Val(param_0, 7);
    f32 val8 = mCamParam.Val(param_0, 8);
    f32 val11 = mCamParam.Val(param_0, 11);
    f32 val12 = mCamParam.Val(param_0, 12);
    f32 val13 = mCamParam.Val(param_0, 13);
    f32 val16 = mCamParam.Val(param_0, 16);
    f32 val24 = mCamParam.Val(param_0, 24);
    f32 val17 = mCamParam.Val(param_0, 17);
    f32 val18 = mCamParam.Val(param_0, 18);
    f32 val21 = mCamParam.Val(param_0, 21);
    daAlink_c* player = (daAlink_c*)mpPlayerActor;

    MagneData* magne = (MagneData*)mWork;

    mStyleSettle.mFinished = mCurCamStyleTimer != 0;

    cSAngle stack_234;
    MtxP mtx, inv_mtx;
    if (player->checkMagneBootsOn()) {
        mtx = player->getMagneBootsMtx();
        inv_mtx = player->getMagneBootsInvMtx();
        stack_234 = player->getMagneBootsModelShapeAngle();
    } else {
        mtx = mDoMtx_getIdentity();
        inv_mtx = mDoMtx_getIdentity();
        stack_234 = directionOf(mpPlayerActor);
    }

    cXyz player_pos = positionOf(mpPlayerActor);
    magne->field_0x4 = mViewCache.mCenter - player_pos;
    magne->field_0x10 = mViewCache.mEye - player_pos;
    mDoMtx_multVecSR(inv_mtx, &magne->field_0x4, &magne->field_0x4);
    mDoMtx_multVecSR(inv_mtx, &magne->field_0x10, &magne->field_0x10);
    magne->field_0x4 += player_pos;
    magne->field_0x10 += player_pos;

    if (mCurCamStyleTimer == 0) {
        magne->field_0x0 = 'MAG_';
        magne->field_0x1c.Val(magne->field_0x10 - magne->field_0x4);
    }

    f32 fVar1 = (magne->field_0x1c.R() - val8) / (val7 - val8);
    if (fVar1 > 1.0f) {
        fVar1 = 1.0f;
    } else if (fVar1 < 0.0f) {
        fVar1 = 0.0f;
    }

    cXyz stack_12c(val0, val3 + (val2 - val3) * fVar1, val1);
    stack_12c = dCamMath::xyzRotateY(stack_12c, stack_234);
    cXyz stack_138 = attentionPos(mpPlayerActor) - player_pos;
    mDoMtx_multVecSR(inv_mtx, &stack_138, &stack_138);
    stack_138 += player_pos;
    magne->field_0x4 += ((stack_138 + stack_12c) - magne->field_0x4) * val5;
    cSGlobe stack_230 = magne->field_0x10 - magne->field_0x4;

    f32 fVar2 = stack_230.R();
    if (fVar2 > val7) {
        fVar2 = val7;
    } else if (fVar2 < val8) {
        fVar2 = val8;
    }

    f32 cstick_x = mPadInfo.mCStick.mLastPosX;
    if (mCamParam.Flag(param_0, 0x40)) {
        cstick_x = 0.0f;
    }

    cSAngle stack_238;
    if (mCurMode == 1) {
        stack_238 = stack_234.Inv();
    } else if (fabsf(cstick_x) > 0.05f) {
        f32 tmp = dCamMath::rationalBezierRatio(cstick_x, 0.5f) * 10.0f;
        stack_238 = magne->field_0x1c.U() + cSAngle(tmp);
    } else {
        cSAngle stack_23c = stack_234.Inv() - stack_230.U();
        f32 sin = stack_23c.Sin();
        f32 tmp = fabsf(sin * mPadInfo.mMainStick.mLastValue);
        f32 tmp2 = stack_23c.Cos() > 0.0f ? 8.0f : 4.0f;
        stack_238 = stack_230.U() + cSAngle(sin * tmp2 * dCamMath::rationalBezierRatio(tmp, 1.0f));
    }

    cSAngle stack_240 = val13 + (val12 - val13) * fVar1;
    magne->field_0x1c.Val(fVar2, stack_240, stack_238);
    magne->field_0x10 = magne->field_0x4 + magne->field_0x1c.Xyz();
    cXyz stack_144 = magne->field_0x4 - player_pos;
    cXyz stack_150 = magne->field_0x10 - player_pos;
    mDoMtx_multVecSR(mtx, &stack_144, &stack_144);
    mDoMtx_multVecSR(mtx, &stack_150, &stack_150);
    stack_144 += player_pos;
    stack_150 += player_pos;
    stack_230.Val(stack_150 - stack_144);
    mViewCache.mCenter = stack_144;
    mViewCache.mDirection.R(mViewCache.mDirection.R()
                    + (stack_230.R() - mViewCache.mDirection.R()) * val11);
    mViewCache.mDirection.V(mViewCache.mDirection.V()
                    + (stack_230.V() - mViewCache.mDirection.V()) * val16);
    mViewCache.mDirection.U(mViewCache.mDirection.U()
                    + (stack_230.U() - mViewCache.mDirection.U()) * val24 * stack_230.V().Cos());
    mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
    mViewCache.mFovy += ((val18 + (val17 - val18) * fVar1) - mViewCache.mFovy) * val21;
    return true;
}

bool dCamera_c::colosseumCamera(s32 param_0) {
    f32 val0 = mCamParam.Val(param_0, 0);
    f32 val2 = mCamParam.Val(param_0, 2);
    f32 val1 = mCamParam.Val(param_0, 1);
    f32 val3 = mCamParam.Val(param_0, 3);
    f32 unusedFloat1 = 0.0f;
    f32 unusedFloat2 = 0.0f;
    f32 unusedFloat3 = 0.0f;
    f32 val7 = mCamParam.Val(param_0, 7);
    f32 val8 = mCamParam.Val(param_0, 8);
    f32 unusedFloat4 = 0.0f;
    f32 val11 = mCamParam.Val(param_0, 11);
    f32 val10 = mCamParam.Val(param_0, 10);
    f32 val12 = mCamParam.Val(param_0, 12);
    f32 val13 = mCamParam.Val(param_0, 13);
    f32 unusedFloat5 = 0.0f;
    f32 unusedFloat6 = 0.0f;
    f32 val17 = mCamParam.Val(param_0, 17);
    f32 val18 = mCamParam.Val(param_0, 18);
    f32 unusedFloat7 = 0.0f;
    f32 unusedFloat8 = 0.0f;

    ColosseumData* colosseum = (ColosseumData*)mWork;

    if (Stage == 0x6b && positionOf(mpPlayerActor).y < -1.0f) {
        param_0 = mCamTypeData[specialType[CAM_TYPE_FIELD_S]].field_0x18[mIsWolf][0];
        return chaseCamera(param_0);
    }

    if (mCurCamStyleTimer == 0) {
        colosseum->field_0x14 = 0.0f;
        colosseum->field_0x18 = 40;
        if (mRoomMapTool.mCameraIndex != 0xff && mCamParam.Flag(param_0, 0x200)) {
            colosseum->field_0x8.x = mRoomMapTool.mArrowData.position.x;
            colosseum->field_0x8.y = mRoomMapTool.mArrowData.position.y;
            colosseum->field_0x8.z = mRoomMapTool.mArrowData.position.z;
        } else if (mCamParam.Flag(param_0, 0x2000)) {
            fopAc_ac_c* target = getParamTargetActor(mCurType);
            colosseum->field_0x8.x = positionOf(target).x;
            colosseum->field_0x8.y = attentionPos(mpPlayerActor).y;
            colosseum->field_0x8.z = positionOf(target).z;
        }
        mEventData.field_0xf0.Init(5, 100);
    }

    if (colosseum->field_0x14 < 0.999f) {
        colosseum->field_0x14 += 1.0f / colosseum->field_0x18;
    } else {
        mStyleSettle.mFinished = true;
        colosseum->field_0x14 = 1.0f;
    }

    cXyz stack_e8 = colosseum->field_0x8;
    if (!mCamParam.Flag(param_0, 0x1000)) {
        stack_e8.y = attentionPos(mpPlayerActor).y;
    }
    cXyz stack_f4(val0, 0.0f, val1);
    cXyz stack_100;
    stack_100 = dCamMath::xyzRotateY(stack_f4, mViewCache.mDirection.U().Inv());
    stack_100 += attentionPos(mpPlayerActor);
    cSGlobe stack_1bc = stack_100 - stack_e8;

    f32 tmp;
    if (stack_1bc.R() < val10) {
        tmp = 0.0f;
        stack_1bc.R(val10);
    } else if (stack_1bc.R() > val11) {
        tmp = 1.0f;
        stack_1bc.R(val11);
    } else {
        tmp = (stack_1bc.R() - val10) / (val11 - val10);
        stack_1bc.R(val10 + (val11 - val10) * tmp);
    }

    static f32 Dsp[5] = {0.0f, 0.0f, 0.25f, 1.0f, 1.0f};
    f32 dVar7 = mEventData.field_0xf0.Spot(Dsp, tmp);

    cXyz stack_10c;
    stack_10c = stack_e8 + stack_1bc.Xyz();
    if (!mCamParam.Flag(param_0, 0x800)) {
        stack_10c.y = 0.0f;
    }
    stack_10c.y += val3 + (val2 - val3) * dVar7;
    f32 fVar8 = val8 + (val7 - val8) * dVar7;
    cSAngle stack_1c0 = val13 + (val12 - val13) * dVar7;
    if (mCamParam.Flag(param_0, 0x1000)) {
        stack_1c0 += stack_1bc.V();
    }
    cSAngle stack_1c4 = stack_1bc.U();

    if (!mStyleSettle.mFinished) {
        mViewCache.mCenter += (stack_10c - mViewCache.mCenter) * colosseum->field_0x14;
        mViewCache.mDirection.R(mViewCache.mDirection.R()
                            + (fVar8 - mViewCache.mDirection.R()) * colosseum->field_0x14);
        mViewCache.mDirection.V(mViewCache.mDirection.V()
                            + (stack_1c0 - mViewCache.mDirection.V()) * colosseum->field_0x14);
        mViewCache.mDirection.U(mViewCache.mDirection.U()
                            + (stack_1c4 - mViewCache.mDirection.U()) * colosseum->field_0x14);
    } else {
        mViewCache.mCenter = stack_10c;
        mViewCache.mDirection.Val(fVar8, stack_1c0, stack_1c4);
    }

    mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
    f32 fVar9 = val18 + (val17 - val18) * dVar7;
    mViewCache.mFovy += (fVar9 - mViewCache.mFovy) * colosseum->field_0x14;
    return true;
}

bool dCamera_c::test1Camera(s32 param_0) {
    return false;
}

bool dCamera_c::test2Camera(s32 param_0) {
    return false;
}

bool dCamera_c::towerCamera(s32 param_0) {
    cSAngle stack_444 = cSAngle(mCamSetup.ChargeLatitude());
    cSAngle stack_448 = 80.0f;
    cSAngle stack_44c = -60.0f;
    cSAngle stack_450 = 60.0f;
    f32 unkFloatConst1 = 0.9f;
    f32 val0 = mCamParam.Val(param_0, 0);
    f32 val2 = mCamParam.Val(param_0, 2);
    f32 val1 = mCamParam.Val(param_0, 1);
    f32 val5 = mCamParam.Val(param_0, 5);
    f32 val6 = mCamParam.Val(param_0, 6);
    f32 val3 = mCamParam.Val(param_0, 3);
    f32 val26 = mCamParam.Val(param_0, 26);
    f32 val7 = mCamParam.Val(param_0, 7);
    f32 val8 = mCamParam.Val(param_0, 8);
    f32 val10 = mCamParam.Val(param_0, 10);
    f32 val11 = mCamParam.Val(param_0, 11);
    f32 val12 = mCamParam.Val(param_0, 12);
    f32 val13 = mCamParam.Val(param_0, 13);
    f32 val16 = mCamParam.Val(param_0, 16);
    f32 val17 = mCamParam.Val(param_0, 17);
    f32 val18 = mCamParam.Val(param_0, 18);
    f32 val20 = mCamParam.Val(param_0, 20);
    f32 val21 = mCamParam.Val(param_0, 21);
    cSAngle stack_454 = mCamParam.Val(param_0, 23);
    cSAngle stack_458 = mCamParam.Val(param_0, 24);
    f32 val22 = mCamParam.Val(param_0, 22);
    f32 val27 = mCamParam.Val(param_0, 27);
    f32 val4 = mCamParam.Val(param_0, 4);
    f32 val9 = mCamParam.Val(param_0, 9);
    f32 val14 = mCamParam.Val(param_0, 14);
    f32 val19 = mCamParam.Val(param_0, 19);

    TowerData* tower = (TowerData*)mWork;

    if (mGear == 1) {
        val7 = val9;
        val8 = val9 * 0.9f;
        val2 = val3 = val4;
        val12 = val13 = val14;
        val17 = val18 = val19;
    }

    daAlink_c* player = (daAlink_c*)mpPlayerActor;
    dComIfGp_getAttention();

    if (mCurCamStyleTimer == 0) {
        tower->field_0x54.x = mRoomMapTool.mArrowData.position.x;
        tower->field_0x54.y = mRoomMapTool.mArrowData.position.y;
        tower->field_0x54.z = mRoomMapTool.mArrowData.position.z;
        tower->field_0x60 = mRoomMapTool.mArrowData.angle.y;
        tower->field_0x64 = stack_454 <= cSAngle::_90 ? 1 : 0;
    }

    if (mRoomMapTool.mArrowIndex == 0xff) {
        if (mCurCamStyleTimer == 0) {
            OS_REPORT("%s: %d: no mapdata use towerCamera() engine !!\n", __FILE__, 0x25ff);
        }

        param_0 = mCamTypeData[specialType[CAM_TYPE_FIELD_S]].field_0x18[mIsWolf][mCurMode];
        return chaseCamera(param_0);
    }

    if (check_owner_action(mPadID, 0x8100000)) {
        if (stack_44c < cSAngle(4.0f)) {
            stack_44c.Val(4.0f);
        }
        if (val2 < -10.0f) {
            val2 = -10.0f;
        }
    }

    cSGlobe stack_41c = attentionPos(mpPlayerActor) - tower->field_0x54;
    cSAngle stack_45c = directionOf(mpPlayerActor);
    bool uVar2 = stack_41c.U() - mViewCache.mDirection.U() > cSAngle::_0;
    bool uVar1 = stack_41c.U() - stack_45c > cSAngle::_0;

    if (mCurCamStyleTimer == 0) {
        tower->field_0x0 = 'TOWR';
        mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
        tower->field_0x18 = 0.0f;
        tower->field_0x1c = tower->field_0x20 = mDirection.R();
        tower->field_0x10 = tower->field_0x14 = 0;
        tower->field_0x24 = tower->field_0x40 = mDirection.V().Degree();
        tower->field_0x48 = mViewCache.mCenter;
        tower->field_0x74 = tower->field_0x78 = 0.01f;
        tower->field_0x7c = 0.01f;
        tower->field_0x70 = 0.75f;
        tower->field_0x80 = val6;
        tower->field_0x84 = val5;
        tower->field_0x38 = 0;
        tower->field_0x6b = true;
        tower->field_0x2c = mFovy;
        tower->field_0x3c = 0.0f;
        tower->field_0x44 = 0.0f;
        tower->field_0x6a = false;

        cSAngle stack_460;
        if (mCamParam.Flag(param_0, 0x100)) {
            stack_460.Val(stack_41c.U() - mViewCache.mDirection.U().Inv());
        } else {
            stack_460.Val(stack_41c.U() - directionOf(mpPlayerActor));
        }

        if (stack_460 > cSAngle::_0) {
            tower->field_0x44 = 1.0f;
        } else {
            tower->field_0x44 = -1.0f;
        }

        tower->field_0x68 = false;
        tower->field_0x28 = mViewCache.mDirection.U();
        tower->field_0x30 = 0.0f;
        tower->field_0x34 = 0.0f;
        tower->field_0x6c = false;
    }

    f32 fVar4 = limitf((mViewCache.mDirection.R() - val8) / (val7 - val8), 0.0f, 1.0f);
    cXyz stack_210;

    if (mCamParam.Flag(param_0, 0x400)) {
        cSAngle stack_464 = tower->field_0x60 - stack_41c.U();
        tower->field_0x30 = val0 * stack_464.Sin();
    } else if (chkFlag(0x100000) && check_owner_action(mPadID, 0xa50c0)) {
        if (mPadInfo.mMainStick.mLastPosX < -0.2f) {
            tower->field_0x6c = true;
        }
        if (mPadInfo.mMainStick.mLastPosX > 0.2f) {
            tower->field_0x6c = false;
        }
        tower->field_0x30 += ((tower->field_0x6c ? -45.0f : 45.0f) - tower->field_0x30) * 0.04f;
    } else {
        tower->field_0x30 += (val0 - tower->field_0x30) * 0.06f;
    }

    stack_210.x = tower->field_0x30;
    if (!tower->field_0x68) {
        stack_210.y = val3 + (val2 - val3) * fVar4;
        stack_210.z = val1 * fVar4;
    } else {
        stack_210.y = 20.0f;
        stack_210.z = 1.0f;
    }

    bool bVar4 = Stage == 0x69 && player->checkSpinnerRide();
    bool bVar3 = Stage == 0x65 && dComIfGp_evmng_cameraPlay();
    bool bVar5 = Stage == 0x6b && check_owner_action1(mPadID, 0x10000);

    if (mCurCamStyleTimer == 0) {
        if (bVar3) {
            tower->field_0x4 = 25;
        } else if (bVar4) {
            tower->field_0x4 = 5;
        } else if (dComIfGp_evmng_cameraPlay()) {
            int timer;
            getEvIntData(&timer, "Timer", 20);
            tower->field_0x4 = timer != 0 ? timer : 1;
        } else if (chkFlag(0x8000) || mCurMode == 1) {
            mStyleSettle.mFinished = true;
            tower->field_0x4 = 1;
        } else {
            cXyz stack_21c = relationalPos(mpPlayerActor, &stack_210);
            f32 fVar19 = cXyz(mEye - stack_21c).abs() - val7;
            f32 fVar18 = cXyz(mCenter - stack_21c).abs() - val7;
            f32 fVar17 = fabsf(fVar19 > fVar18 ? fVar19 : fVar18);
            f32 player_height = heightOf(mpPlayerActor);
            f32 tmp = fVar17 / (player_height < 10.0f ? 10.0f : player_height);
            tower->field_0x4 = (int)(JMAFastSqrt(tmp) * 8.0f) + 1;
        }

        tower->field_0x8 = tower->field_0x4 * (tower->field_0x4 + 1) >> 1;
        tower->field_0xc = 0.0f;
    }

    cXyz stack_228;
    cSGlobe stack_424 = stack_210;

    if (mCamParam.Flag(param_0, 0x1000)) {
        stack_424.U(stack_424.U() + directionOf(mpPlayerActor));
    } else {
        stack_424.U(stack_424.U() + stack_41c.U());
    }

    if (mCamParam.Flag(param_0, 0x800) && mpLockonTarget != NULL) {
        stack_228 = attentionPos(mpPlayerActor) + stack_424.Xyz()
                        + (attentionPos(mpLockonTarget) - attentionPos(mpPlayerActor)) * val26;
        mpAuxTargetActor1 = mpLockonTarget;
        setFlag(0x2000);
    } else {
        stack_228 = attentionPos(mpPlayerActor) + stack_424.Xyz();
    }

    cXyz stack_234;

    if (!mStyleSettle.mFinished) {
        if (mBG.field_0xc0.field_0x1) {
            dComIfG_Bgsp().MoveBgMatrixCrrPos(mBG.field_0x5c.field_0x4, true,
                                              &tower->field_0x48, NULL, NULL);
        }

        tower->field_0xc = tower->field_0x4 - (int)mCurCamStyleTimer;
        f32 fVar18;
        f32 ratio = tower->field_0xc / tower->field_0x8;
        tower->field_0x48 += (stack_228 - tower->field_0x48) * ratio;
        mViewCache.mCenter += (tower->field_0x48 - mViewCache.mCenter) * val6;

        stack_234 = attentionPos(mpPlayerActor);
        stack_234.y -= 15.0f;
        dBgS_CamLinChk lin_chk;
        if (lineBGCheck(&stack_234, &mViewCache.mCenter, &lin_chk, 0x40b7)) {
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
            mViewCache.mCenter = lin_chk.GetCross();
            mViewCache.mCenter += *plane.GetNP();
        }

        fVar18 = limitf(mViewCache.mDirection.R(), val8, val7);
        cSAngle stack_468;
        stack_468 = mViewCache.mDirection.V();
        if (bVar5) {
            stack_468 = cSAngle(val12);
        }
        if (stack_468 < stack_44c) {
            stack_468 = stack_44c;
        }
        if (stack_468 > stack_450) {
            stack_468 = stack_450;
        }

        cSGlobe stack_42c(fVar18, stack_468, cSAngle(mControlledYaw.Inv()));
        mViewCache.mDirection.R(mViewCache.mDirection.R()
                                + (stack_42c.R() - mViewCache.mDirection.R()) * ratio);
        mViewCache.mDirection.V(mViewCache.mDirection.V()
                                + (stack_42c.V() - mViewCache.mDirection.V()) * ratio);
        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();

        if (mCurCamStyleTimer >= tower->field_0x4 - 1) {
            mStyleSettle.mFinished = true;
        }

        tower->field_0x24 = mViewCache.mDirection.V().Degree();
        tower->field_0x1c = tower->field_0x20 = mViewCache.mDirection.R();
        mViewCache.mFovy += (val17 - mViewCache.mFovy) * ratio;
        tower->field_0x8 -= tower->field_0xc;
        return true;
    }

    cXyz stack_240 = positionOf(mpPlayerActor);
    stack_240.y += 10.0f;
    f32 heightOffGround = footHeightOf(mpPlayerActor) - groundHeight(&stack_240);
    if (mBG.field_0xc0.field_0x44) {
        tower->field_0x18 = 0.0f;
        tower->field_0x10 = 0;
    } else if (tower->field_0x10 < 80) {
        tower->field_0x10++;
        tower->field_0x18 += (unkFloatConst1 - tower->field_0x18)
                            * dCamMath::rationalBezierRatio(tower->field_0x10 / 80.0f, 1.25f);
    }

    cSAngle stack_46c = stack_45c - mViewCache.mDirection.U();
    cXyz stack_24c(tower->field_0x80, tower->field_0x84, tower->field_0x80);
    mViewCache.mCenter += (stack_228 - mViewCache.mCenter) * stack_24c;
    stack_234 = attentionPos(mpPlayerActor);
    stack_234.y -= 15.0f;
    cSGlobe stack_434 = mViewCache.mEye - mViewCache.mCenter;

    f32 fVar18 = 1.0f;
    if (val7 > val8) {
        fVar18 = (mViewCache.mDirection.R() - val8) / (val7 - val8);
        if (fVar18 < 0.0f) {
            fVar18 = 0.0f;
        }
        if (fVar18 > 1.0f) {
            fVar18 = 1.0f;
        }
    }

    bool spinner_path_move_temp = player->checkSpinnerPathMove();
    bool spinner_path_move = spinner_path_move_temp;
    val6 = mPadInfo.mCStick.mLastPosX;
    val3 = spinner_path_move ? 1.0f : mPadInfo.mMainStick.mLastValue;
    f32 unkFloatConst2 = 0.05f;
    f32 unkFloatConst3 = 8.0f;
    cSAngle stack_470;
    cSAngle stack_474 = stack_454 + (stack_454 - stack_458) * val3;

    if (!mCamParam.Flag(param_0, 0x40) && fabsf(val6) > unkFloatConst2) {
        cSAngle stack_478 = mViewCache.mDirection.U() + cSAngle(dCamMath::rationalBezierRatio(val6, 0.5f) * unkFloatConst3);
        f32 tmp2 = fabsf(val6) - unkFloatConst2;
        stack_470.Val(mViewCache.mDirection.U() + (stack_478 - mViewCache.mDirection.U()) * tmp2);
        tower->field_0x6a = true;
        tower->field_0x78 += (0.8f - tower->field_0x78) * 0.05f;
        tower->field_0x28 = stack_470;

    } else {
        if (tower->field_0x6a) {
            tower->field_0x78 = 0.0f;
            tower->field_0x6a = false;
        }

        if (mCamParam.Flag(param_0, 0x800)) {
            cSAngle stack_47c;
            stack_47c.Val(stack_41c.U() - directionOf(mpPlayerActor));
            f32 fVar19 = mMonitor.field_0x10 / playerMaxSpeed();
            if (fVar19 > 1.0f) {
                fVar19 = 1.0f;
            }

            if (stack_47c > cSAngle::_0) {
                if (tower->field_0x44 < 1.0f) {
                    tower->field_0x44 += fVar19 * 0.025f;
                }
            } else {
                if (tower->field_0x44 > -1.0f) {
                    tower->field_0x44 -= fVar19 * 0.025f;
                }
            }

            tower->field_0x78 += (0.5f - tower->field_0x78) * 0.05f;

            if (tower->field_0x64 == 0) {
                cSAngle stack_480 = cSAngle::_180 - stack_474;
                tower->field_0x28 = stack_41c.U().Inv() - stack_480 * tower->field_0x44;
            } else {
                cSAngle stack_484 = stack_474;
                tower->field_0x28 = stack_41c.U() - stack_484 * tower->field_0x44;
            }

        } else if (mCamParam.Flag(param_0, 0x100)) {
            if (!uVar2) {
                cSAngle stack_488 = stack_434.U() - stack_41c.U();
                f32 tmp = dCamMath::rationalBezierRatio(fVar18, val20);
                if (stack_488 > stack_474) {
                    tmp = 1.0f;
                }
                stack_470.Val(stack_434.U() + ((stack_41c.U() + stack_474) - stack_434.U()) * tmp);
            } else {
                cSAngle stack_48c = stack_41c.U() - stack_434.U();
                f32 tmp = dCamMath::rationalBezierRatio(fVar18, val20);
                if (stack_48c > stack_474) {
                    tmp = 1.0f;
                }
                stack_470.Val(stack_434.U() + ((stack_41c.U() - stack_474) - stack_434.U()) * tmp);
            }

            tower->field_0x28 += (stack_470 - tower->field_0x28) * 0.33f;

            f32 var_f15 = val3;
            if (check_owner_action(mPadID, 0x2000108)) {
                var_f15 = 0.0f;
            }
            tower->field_0x78 = val27 + val22 * var_f15;

        } else {
            if (uVar1) {
                stack_470.Val(stack_434.U() + ((stack_41c.U() + stack_474) - stack_434.U()) * 1.0f);
            } else {
                f32 oneF = 1.0f;
                stack_470.Val(stack_434.U() + ((stack_41c.U() - stack_474) - stack_434.U()) * oneF);
            }

            tower->field_0x28 += (stack_470 - tower->field_0x28) * 0.33f;

            if (check_owner_action(mPadID, 0x2000108) || tower->field_0x69 != uVar1) {
                tower->field_0x78 = 0.0f;
            } else {
                tower->field_0x78 += ((val27 + val22 * val3) - tower->field_0x78) * 0.05f;
            }
        }
    }

    tower->field_0x69 = uVar1;
    mViewCache.mDirection.U(mViewCache.mDirection.U()
                + (tower->field_0x28 - mViewCache.mDirection.U()) * tower->field_0x78);
    f32 temp = (val13 + (val12 - val13) * fVar18);
    tower->field_0x24 += (temp - tower->field_0x24) * val11;

    if (!mCamParam.Flag(param_0, 0x4000)) {
        mForwardTiltOffset = cSAngle::_0;
    }

    cSAngle stack_490;
    if (tower->field_0x10 == 0) {
        stack_490.Val(tower->field_0x24 + mForwardTiltOffset.Degree());
        tower->field_0x74 += (val16 - tower->field_0x74) * 0.01f;
    } else {
        stack_490 = stack_434.V();
        tower->field_0x24 = stack_490.Degree();
        tower->field_0x74 = dCamMath::rationalBezierRatio(tower->field_0x18, 0.7f);
    }

    if (stack_490 < stack_44c) {
        stack_490.Val(stack_44c);
    } else if (stack_490 > stack_450) {
        stack_490.Val(stack_450);
    }

    mViewCache.mDirection.V(mViewCache.mDirection.V()
                                + (stack_490 - mViewCache.mDirection.V()) * tower->field_0x74);
    f32 dVar20 = stack_434.R();
    tower->field_0x1c += (val8 - tower->field_0x1c) * val11;
    tower->field_0x20 += (val7 - tower->field_0x20) * val11;

    if (dVar20 < tower->field_0x1c) {
        tower->field_0x70 += (val10 - tower->field_0x70) * 0.01f;
        dVar20 = tower->field_0x1c;
    } else if (dVar20 > tower->field_0x20) {
        tower->field_0x70 += (val10 - tower->field_0x70) * 0.01f;
        dVar20 = tower->field_0x20;
    } else {
        tower->field_0x70 = 1.0f;
    }

    if (mCamParam.Flag(param_0, 0x400)) {
        cSGlobe stack_43c(dVar20, mViewCache.mDirection.V(), mViewCache.mDirection.U());
        cXyz stack_258;
        cXyz stack_264 = mViewCache.mCenter + stack_43c.Xyz();
        tower->field_0x68 = false;
        if (lineBGCheck(&stack_264, &mViewCache.mCenter, &stack_258, 0x40b7)) {
            tower->field_0x68 = true;
        }
    }

    mViewCache.mDirection.R(mViewCache.mDirection.R()
                                + (dVar20 - mViewCache.mDirection.R()) * tower->field_0x70);
    mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();

    if (mBG.field_0x0.field_0x0 && mViewCache.mEye.y < mBG.field_0x0.field_0x58 + 5.0f) {
        mViewCache.mEye.y = mBG.field_0x0.field_0x58 + 5.0f;
        mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
    }

    tower->field_0x7c += (val21 - tower->field_0x7c) * 0.01f;
    f32 tmp = val18 + (val17 - val18) * fVar18;
    mViewCache.mFovy += (tmp - mViewCache.mFovy) * tower->field_0x7c;
    return true;
}

bool dCamera_c::hookshotCamera(s32 param_0) {
    f32 val0 = mCamParam.Val(param_0, 0);
    f32 val2 = mCamParam.Val(param_0, 2);
    f32 val1 = mCamParam.Val(param_0, 1);
    f32 val5 = mCamParam.Val(param_0, 5);
    f32 val6 = mCamParam.Val(param_0, 6);
    f32 val7 = mCamParam.Val(param_0, 7);
    f32 val8 = mCamParam.Val(param_0, 8);
    f32 val11 = mCamParam.Val(param_0, 11);
    f32 val23 = mCamParam.Val(param_0, 23);
    f32 val17 = mCamParam.Val(param_0, 17);

    HookshotData* hookshot = (HookshotData*)mWork;

    cXyz stack_9c[4] = {
        cXyz(180.0f, 20.0f, -160.0f),
        cXyz(160.0f, 60.0f, -200.0f),
        cXyz(170.0f, -80.0f, -150.0f),
        cXyz(100.0f, -120.0f, -240.0f),
    };
    cXyz stack_cc[4] = {
        cXyz(40.0f, -150.0f, 340.0f),
        cXyz(10.0f, -100.0f, 280.0f),
        cXyz(80.0f, -50.0f, 200.0f),
        cXyz(40.0f, -80.0f, -160.0f),
    };
    cXyz stack_d8;
    cXyz stack_e4(val0, val2, val1);

    if (mCurCamStyleTimer == 0) {
        hookshot->field_0x0 = 'HOOK';
        hookshot->field_0x8 = mCenter;
        cXyz stack_f0 = positionOf(mpPlayerActor);
        cXyz stack_fc = daAlink_getAlinkActorClass()->getHsChainTopPos();
        csXyz stack_20c = daAlink_getAlinkActorClass()->getHsAngle();
        cXyz stack_108 = stack_fc - stack_f0;

        bool iVar2 = stack_20c.x < -0x3333;
        cXyz* array;
        if (iVar2) {
            cSGlobe stack_214 = stack_108;
            stack_20c.x = stack_214.V();
            stack_20c.y = stack_214.U();
            stack_20c.z = 0;
            array = stack_cc;
        } else {
            array = stack_9c;
        }

        if (stack_108.abs() > val11 && mCamParam.Flag(param_0, 0x800)) {
            cXyz stack_114;
            cSGlobe stack_21c;
            bool uVar6 = (mFrameCounter & 0x10) != 0;
            for (int i = 0; i < 8; i++) {
                stack_114 = array[i >> 1];
                if (uVar6) {
                    stack_114.x = -stack_114.x;
                }
                stack_21c.Val(stack_114);
                stack_21c.U(stack_21c.U() + stack_20c.y);
                if (!iVar2) {
                    stack_21c.V(stack_21c.V() + stack_20c.x);
                }
                hookshot->field_0x14 = stack_fc + stack_21c.Xyz();
                if (!lineBGCheck(&stack_f0, &hookshot->field_0x14, 7)) {
                    hookshot->field_0x20 = true;
                    break;
                }
                uVar6 = !uVar6;
            }
        }

        mStyleSettle.mFinished = true;
        return true;
    }

    f32 fovy;
    if (hookshot->field_0x20 && mCurCamStyleTimer > 8 && mCamParam.Flag(param_0, 0x800)) {
        stack_d8 = hookshot->field_0x14;
        fovy = val17;
        val23 = 1.0f;
    } else {
        stack_d8 = mEye;
        fovy = val17;
    }

    cXyz stack_120(val6, val5, val6);
    cXyz stack_12c = relationalPos(mpPlayerActor, &stack_e4);

    if (mBG.field_0xc0.field_0x1) {
        dComIfG_Bgsp().MoveBgMatrixCrrPos(mBG.field_0x5c.field_0x4, true,
                                          &hookshot->field_0x8, NULL, NULL);
        dComIfG_Bgsp().MoveBgMatrixCrrPos(mBG.field_0x5c.field_0x4, true,
                                          &hookshot->field_0x14, NULL, NULL);
    }

    mViewCache.mCenter += (stack_12c - mViewCache.mCenter) * stack_120;
    cSGlobe stack_224 = stack_d8 - mViewCache.mCenter;
    if (stack_224.R() < val8) {
        stack_224.R(val8);
    }
    if (stack_224.R() > val7) {
        stack_224.R(val7);
    }
    mViewCache.mDirection.R(mViewCache.mDirection.R()
                                + (stack_224.R() - mViewCache.mDirection.R()) * val23);
    mViewCache.mDirection.V(mViewCache.mDirection.V()
                                + (stack_224.V() - mViewCache.mDirection.V()) * val23);
    mViewCache.mDirection.U(mViewCache.mDirection.U()
                                + (stack_224.U() - mViewCache.mDirection.U()) * val23);
    mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
    mViewCache.mFovy += (fovy - mViewCache.mFovy) * val23;
    return true;
}

bool dCamera_c::railCamera(s32 param_0) {
    f32 proximityEaseBias = mCamParam.Val(param_0, 15);
    f32 targetRadiusFar = mCamParam.Val(param_0, 7);
    f32 targetRadiusNear = mCamParam.Val(param_0, 10);
    f32 searchRadius = mCamParam.Val(param_0, 11);
    f32 influenceDistMax = mCamParam.Val(param_0, 9);
#if DEBUG
    influenceDistMax = mCamParam.Val(param_0, 12);
#endif
    f32 val27 = mCamParam.Val(param_0, 27);
    f32 fovFar = mCamParam.Val(param_0, 17);
    f32 fovNear = mCamParam.Val(param_0, 20);
    f32 responseGain = mCamParam.Val(param_0, 21);
    f32 responseLerpRate = mCamParam.Val(param_0, 14);
    f32 centerLerpGainY = mCamParam.Val(param_0, 5);
    f32 centerLerpGainXZ = mCamParam.Val(param_0, 6);
    f32 playerOffsetX = mCamParam.Val(param_0, 0);
    f32 playerOffsetY = mCamParam.Val(param_0, 2);
    f32 playerOffsetZ = mCamParam.Val(param_0, 1);

    RailData* rail = (RailData*)mWork;

    if (mCurCamStyleTimer == 0) {
        if (chkFlag(0x1000)) {
            responseGain = 1.0f;
            val27 = 1.0f;
            rail->field_0x10 = 1.0f;
        } else {
            rail->field_0x10 = 0.0f;
        }
        rail->field_0x0d = 0xff;
    }

    rail->field_0x10 += responseLerpRate * (responseGain - rail->field_0x10);
    if (mRoomMapTool.mCameraIndex == 0xff || mRoomMapTool.mPathId == 0xff) {
        OS_REPORT("camera: rail data not found!\n");
        return false;
    }

    dPath* roomPath = dPath_GetRoomPath(mRoomMapTool.mPathId, fopAcM_GetRoomNo(this->mpPlayerActor));
    if (roomPath == NULL) {
        OS_REPORT("camera: rail data not found!\n");
        return false;
    }

    mCamParam.Arg0(mRoomMapTool.mCamData.field_0x12);
    if (mCamParam.Arg0() != 0xff) {
        searchRadius = (f32)(s8)mCamParam.Arg0() * 100.0f;
    }

    s32 roomPathLen = roomPath->m_num;
    if (mCurCamStyleTimer == 0) {
        rail->field_0x00 = 'RAIL';
        mStyleSettle.mFinished = true;
        rail->field_0x0c = 0xff;
        rail->field_0x14 = attentionPos(this->mpPlayerActor);
        if (mCamParam.Flag(param_0, 0x800)) {
            rail->field_0x08 = getParamTargetActor(mCurType);
            if (rail->field_0x00 == NULL) {
                OS_REPORT("camera: target %s actor not found!\n", &mCamTypeData[mCurType].name[16]);
            }
        }
        rail->field_0x0d = 0xfe;
    }

    if (rail->field_0x0d != mRoomMapTool.mPathId) {
        rail->field_0x0d = mRoomMapTool.mPathId;
        rail->field_0x20.x = mRoomMapTool.mArrowData.position.x;
        rail->field_0x20.y = mRoomMapTool.mArrowData.position.y;
        rail->field_0x20.z = mRoomMapTool.mArrowData.position.z;
        if (mCamParam.Flag(param_0, 0x800)) {
            cXyz attnPos = attentionPos(mpPlayerActor);
            cXyz curPointPos = roomPath->m_points->m_position;
            cXyz prevPointPos = roomPath->m_points[roomPathLen - 1].m_position;
            f32 horDist1 = dCamMath::xyzHorizontalDistance(curPointPos, attnPos);
            f32 horDist2 = dCamMath::xyzHorizontalDistance(prevPointPos, attnPos);
            if (horDist1 < horDist2) {
                rail->field_0x14 = prevPointPos;
            } else {
                rail->field_0x14 = curPointPos;
            }
        }
    }

    cXyz focusPos;
    cXyz eyeRefPos;
    if (mCamParam.Flag(param_0, 0x800) && rail->field_0x08 != NULL) {
        eyeRefPos = positionOf(rail->field_0x08);
        eyeRefPos.y += 300.0f;
        if (mpLockonTarget != NULL) {
            mpAuxTargetActor1 = rail->field_0x04 = mpLockonTarget;
            setFlag(0x2000);
            focusPos = attentionPos(rail->field_0x04);
        } else {
            mpAuxTargetActor1 = rail->field_0x04 = rail->field_0x08;
            setFlag(0x2000);
            focusPos = eyeRefPos;
        }
    } else if (mCamParam.Flag(param_0, 0x100)) {
        rail->field_0x08 = rail->field_0x04 = NULL;

        focusPos = attentionPos(mpPlayerActor);

        eyeRefPos = rail->field_0x20;
    } else if (mpLockonTarget != NULL) {
        mpAuxTargetActor1 = rail->field_0x08 = rail->field_0x04 = mpLockonTarget;
        focusPos = attentionPos(rail->field_0x04);

        setFlag(0x2000);

        eyeRefPos = attentionPos(rail->field_0x08);
    } else {
        rail->field_0x08 = rail->field_0x04 = NULL;

        focusPos = attentionPos(mpPlayerActor);

        eyeRefPos = mEye;
    }

    cSGlobe focusFromPlayer = focusPos - attentionPos(mpPlayerActor);
    f32 proximityRatio = 1.0f - focusFromPlayer.R() / influenceDistMax;
    if (focusFromPlayer.R() > searchRadius * 2.0f) {
        rail->field_0x08 = rail->field_0x04 = 0;

        cSGlobe dirGlobe = mViewCache.mDirection;
        dirGlobe.R(-searchRadius);
        focusPos = eyeRefPos = attentionPos(mpPlayerActor) + dirGlobe.Xyz();

        proximityRatio = 0.0f;
    } else {
        if (proximityRatio < 0.0f) {
            proximityRatio = 0.0f;
        } else if (proximityRatio > 1.0f) {
            proximityRatio = 1.0f;
        }
    }

    cXyz desiredCenter;
    cXyz centerLerpGainVec(centerLerpGainXZ, centerLerpGainY, centerLerpGainXZ);
    cXyz playerOffset(playerOffsetX, playerOffsetY, playerOffsetZ);
    focusFromPlayer.R(proximityRatio * focusFromPlayer.R() * 0.75f);

    int i;
    if (mCamParam.Flag(param_0, 0x400)) {
        cXyz unkXyz5;
        bool unkFlag1 = false;
        cM3dGLin unkLine1;

        for (i = 0; i < roomPathLen - 1; i++) {
            unkLine1.set(roomPath->m_points[i].m_position,
                          roomPath->m_points[i + 1].m_position);
            f32 unusedFloat1;
            if (cM3d_Len3dSqPntAndSegLine(&unkLine1, &eyeRefPos, &unkXyz5, &unusedFloat1) != 0) {
                unkXyz5 += (eyeRefPos - unkXyz5) * 0.1f;
                unkFlag1 = true;
                break;
            }
        }

        if (!unkFlag1) {
            unkXyz5 = attentionPos(mpPlayerActor) + focusFromPlayer.Xyz();
        }

        desiredCenter = mViewCache.mCenter + (unkXyz5 - mViewCache.mCenter) * 0.1f;
    } else {
        desiredCenter = relationalPos(mpPlayerActor, &playerOffset) + focusFromPlayer.Xyz();
    }

    mViewCache.mCenter += (desiredCenter - mViewCache.mCenter) * centerLerpGainVec;

    f32 proximityEase = dCamMath::rationalBezierRatio(proximityRatio, proximityEaseBias);

    cM3dGSph searchSphere;
    searchSphere.Set(mViewCache.mCenter, searchRadius);

    f32 bestHorDist = 0.0f;
    cXyz bestEyePos = mViewCache.mEye;
    s32 intersectCount = 0;
    if (mCamParam.Flag(param_0, 0x800)) {
        eyeRefPos = rail->field_0x14;
    }
    cXyz segStart;
    cXyz segEnd;
    cXyz crossPosA;
    cXyz crossPosB;
    cM3dGLin segLine;

    for (i = 0; i < roomPathLen - 1; i++) {
        segStart.set(roomPath->m_points[i].m_position);
        segEnd.set(roomPath->m_points[i + 1].m_position);
        segLine.set(segStart, segEnd);

        f32 horDist;
        switch (cM3d_Cross_LinSph_CrossPos(searchSphere, segLine, &crossPosA, &crossPosB)) {
        case 2:
            if (posInLine2D(&segStart, &segEnd, &crossPosB) != 0) {
                horDist = dCamMath::xyzHorizontalDistance(crossPosB, eyeRefPos);
                if (horDist > bestHorDist) {
                    bestHorDist = horDist;
                    bestEyePos = crossPosB;
                }
                intersectCount++;
            }
        case 1:
            if (posInLine2D(&segStart, &segEnd, &crossPosA) != 0) {
                horDist = dCamMath::xyzHorizontalDistance(crossPosA, eyeRefPos);
                if (horDist > bestHorDist) {
                    bestHorDist = horDist;
                    bestEyePos = crossPosA;
                }
                intersectCount++;
            }
            break;
        }
    }

    if (dPath_ChkClose(roomPath)) {
        segStart.set(roomPath->m_points[i].m_position);
        segEnd.set(roomPath->m_points->m_position);
        segLine.set(segStart, segEnd);

        f32 horDist;
        switch (cM3d_Cross_LinSph_CrossPos(searchSphere, segLine, &crossPosA, &crossPosB)) {
        case 2:
            if (posInLine2D(&segStart, &segEnd, &crossPosB) != 0) {
                horDist = dCamMath::xyzHorizontalDistance(crossPosB, eyeRefPos);
                if (horDist > bestHorDist) {
                    bestHorDist = horDist;
                    bestEyePos = crossPosB;
                }
                intersectCount++;
            }
        case 1:
            if (posInLine2D(&segStart, &segEnd, &crossPosA) != 0) {
                horDist = dCamMath::xyzHorizontalDistance(crossPosA, eyeRefPos);
                if (horDist > bestHorDist) {
                    bestHorDist = horDist;
                    bestEyePos = crossPosA;
                }
                intersectCount++;
            }
            break;
        }

        roomPathLen++;
    }

    cSGlobe eyeFromCenter = bestEyePos - mViewCache.mCenter;
    cSAngle unkAngle1 = eyeFromCenter.U() - mViewCache.mDirection.U();

    static cSAngle yawSnapThreshold = 120.0f;
    if (unkAngle1.Abs() > yawSnapThreshold) {
        setUSOAngle();
    }

    f32 desiredRadius;
    if (mCamParam.Flag(param_0, 0x1000)) {
        desiredRadius = eyeFromCenter.R();
    } else {
        desiredRadius = targetRadiusFar - proximityEase * (targetRadiusFar - targetRadiusNear);
    }

    mViewCache.mDirection.R(mViewCache.mDirection.R() +
                            rail->field_0x10 * (desiredRadius - mViewCache.mDirection.R()));
    mViewCache.mDirection.U(mViewCache.mDirection.U() +
                 (eyeFromCenter.U() - mViewCache.mDirection.U()) * rail->field_0x10);
    mViewCache.mDirection.V(mViewCache.mDirection.V() +
                 (eyeFromCenter.V() - mViewCache.mDirection.V()) * rail->field_0x10);

    mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
    mViewCache.mFovy += responseGain * (fovFar - proximityEase * (fovFar - fovNear) - mViewCache.mFovy);

    return true;
}

bool dCamera_c::paraRailCamera(s32 param_0) {
    int i;

    f32 val7 = mCamParam.Val(param_0, 7);
    f32 val10 = mCamParam.Val(param_0, 10);
    f32 val11 = mCamParam.Val(param_0, 11);
    f32 val9 = mCamParam.Val(param_0, 9);
    f32 val15 = mCamParam.Val(param_0, 15);
    f32 val14 = mCamParam.Val(param_0, 14);
    f32 val17 = mCamParam.Val(param_0, 17);
    f32 val20 = mCamParam.Val(param_0, 20);
    f32 val21 = mCamParam.Val(param_0, 21);
    f32 val5 = mCamParam.Val(param_0, 5);
    f32 val6 = mCamParam.Val(param_0, 6);
    f32 val0 = mCamParam.Val(param_0, 0);
    f32 val2 = mCamParam.Val(param_0, 2);
    f32 val1 = mCamParam.Val(param_0, 1);

    ParaRailData* paraRail = (ParaRailData*)mWork;

    mBumpCheckFlags = 0;
    if (mTagCamTool.mCameraIndex == 0xff || mTagCamTool.mPathId == 0xff) {
        OS_REPORT("camera: rail data not found!\n");
        return false;
    }

    if (mCurCamStyleTimer == 0) {
        if (chkFlag(0x1000)) {
            val21 = 1.0f;
            val6 = val5 = 1.0f;
            paraRail->field_0x1c = 1.0f;
        } else {
            paraRail->field_0x1c = 0.0f;
        }
        paraRail->field_0x0d = 0xff;
    }

    paraRail->field_0x1c += val14 * (val21 - paraRail->field_0x1c);

    s8 roomNo = fopAcM_GetRoomNo(mpPlayerActor);
    dPath* path = dPath_GetRoomPath(mTagCamTool.mPathId, roomNo);
    if (path == NULL) {
        OS_REPORT("camera: rail data not found!\n");
        return false;
    }

    mCamParam.Arg0(mRoomMapTool.mCamData.field_0x12);
    if (mCamParam.Arg0() != 0xff) {
        val11 = (f32)(s32)(s8)mCamParam.Arg0() * 100.0f;
    }

    cXyz sp260;
    cXyz sp254;
    cXyz sp248;
    cM3dGLin sp288;
    f32 sp50;
    f32 sp4C;
    u32 pathLen = path->m_num;
    sp50 = 0.0f;
    bool unkFlag1 = false;
    if (mCurCamStyleTimer == 0) {
        paraRail->field_0x00 = 'RARA';
        mStyleSettle.mFinished = true;
        paraRail->field_0x0c = 0xff;
        unkFlag1 = true;
    }

    if (paraRail->field_0x0d != mRoomMapTool.mPathId) {
        paraRail->field_0x0d = mRoomMapTool.mPathId;
    }

    cXyz sp23C;
    cXyz sp230;
    if (mpLockonTarget != NULL) {
        mpAuxTargetActor1 = paraRail->field_0x08 = paraRail->field_0x04 = mpLockonTarget;
        sp23C = attentionPos(paraRail->field_0x04);
        setFlag(0x2000);
        sp230 = attentionPos(paraRail->field_0x08);
    } else {
        paraRail->field_0x08 = paraRail->field_0x04 = NULL;
        cSGlobe spA8 = mViewCache.mDirection;
        spA8.R(100.0f);
        sp23C = sp230 = attentionPos(mpPlayerActor) + spA8.Xyz();
    }

    cSGlobe sp17C = sp23C - attentionPos(mpPlayerActor);
    f32 var_f31 = 1.0f - sp17C.R() / val9;
    if (sp17C.R() > val11 * 2.0f) {
        paraRail->field_0x08 = paraRail->field_0x04 = NULL;
        cSGlobe sp98 = mViewCache.mDirection;
        sp98.R(-val11);
        sp23C = sp230 = attentionPos(mpPlayerActor) + sp98.Xyz();
        var_f31 = 0.0f;
    } else {
        if (var_f31 < 0.0f) {
            var_f31 = 0.0f;
        } else if (var_f31 > 1.0f) {
            var_f31 = 1.0f;
        }
    }

    cXyz sp224;
    cXyz sp218(val6, val5, val6);
    cXyz sp20C(val0, val2, val1);
    sp17C.R(var_f31 * sp17C.R() * 0.75f);

    if (mCamParam.Flag(param_0, 0x400)) {
        cXyz sp200;
        bool unkFlag2 = false;
        cM3dGLin sp26C;
        for (i = 0; i < (int)(pathLen - 1); i++) {
            sp26C.set(path->m_points[i].m_position,
                          path->m_points[i + 1].m_position);
            f32 sp44;
            if (cM3d_Len3dSqPntAndSegLine(&sp26C, &sp230, &sp200, &sp44) != 0) {
                sp200 += (sp230 - sp200) * 0.1f;
                unkFlag2 = true;
                break;
            }
        }

        if (!unkFlag2) {
            cXyz sp1F4 = attentionPos(mpPlayerActor);
            cXyz sp1E8 = path->m_points->m_position;
            cXyz sp1DC = path->m_points[pathLen - 1].m_position;
            f32 horDist1 = dCamMath::xyzHorizontalDistance(sp1E8, sp1F4);
            f32 horDist2 = dCamMath::xyzHorizontalDistance(sp1DC, sp1F4);
            if (horDist1 < horDist2) {
                sp200 = sp1E8;
            } else {
                sp200 = sp1DC;
            }
            sp200 += (sp230 - sp200) * 0.1f;
        }

        sp224 = sp200;
    } else {
        sp224 = relationalPos(mpPlayerActor, &sp20C) + sp17C.Xyz();
    }

    mViewCache.mCenter += (sp224 - mViewCache.mCenter) * sp218;

    var_f31 = dCamMath::rationalBezierRatio(var_f31, val15);
    sp50 = val11 * val11;
    cXyz cStack_120 = mViewCache.mEye;
    int sp38 = 0;
    for (i = 0; i < (int)(pathLen - 1); i++) {
        sp254.set(path->m_points[i].m_position);
        sp248.set(path->m_points[i + 1].m_position);
        sp288.set(sp254, sp248);
        if (cM3d_Len3dSqPntAndSegLine(&sp288, &mViewCache.mCenter, &sp260,
                                          &sp4C)) {
            if (sp4C < sp50) {
                sp50 = sp4C;
                cStack_120 = sp260;
            }
            sp38++;
        }
    }

    if (dPath_ChkClose(path)) {
        sp254.set(path->m_points[i].m_position);
        sp248.set(path->m_points->m_position);
        sp288.set(sp254, sp248);

        if (cM3d_Len3dSqPntAndSegLine(&sp288, &mViewCache.mCenter, &sp260,
                                          &sp4C) != 0) {
            if (sp4C < sp50) {
                sp50 = sp4C;
                cStack_120 = sp260;
            }
            sp38++;
        }

        pathLen++;
    }

    if (sp38 == 0) {
        if (mCamParam.Flag(param_0, 0x1000)) {
            sp254.set(path->m_points->m_position);
            sp248.set(path->m_points[path->m_num - 1].m_position);
            f32 horDist1 = dCamMath::xyzHorizontalDistance(sp254, sp230);
            f32 horDist2 = dCamMath::xyzHorizontalDistance(sp248, sp230);
            if (horDist1 < horDist2) {
                cStack_120 = sp254;
                sp50 = horDist1;
            } else {
                cStack_120 = sp248;
                sp50 = horDist2;
            }
        } else {
#if DEBUG
            dDbVw_Report(0xb4, 0xf0, "camera: point not found!!");
#endif
        }
    }

    if (unkFlag1) {
        paraRail->field_0x10 = cStack_120;
    } else {
        paraRail->field_0x10 += (cStack_120 - paraRail->field_0x10) * 0.1f;
    }

    cSGlobe cStack_260 = paraRail->field_0x10 - mViewCache.mCenter;
    cSAngle acStack_2cc = cStack_260.U() - mViewCache.mDirection.U();

    static cSAngle paraRailCamera = 120.0f;
    if (acStack_2cc.Abs() > paraRailCamera) {
        setUSOAngle();
    }

    f32 local_2c4;
    if (mCamParam.Flag(param_0, 0x1000)) {
        local_2c4 = cStack_260.R();
    } else {
        local_2c4 = val7 - var_f31 * (val7 - val10);
    }

    mViewCache.mDirection.R(mViewCache.mDirection.R() + paraRail->field_0x1c * (local_2c4 - mViewCache.mDirection.R()));
    mViewCache.mDirection.U(mViewCache.mDirection.U() +
                            (cStack_260.U() - mViewCache.mDirection.U()) * paraRail->field_0x1c);
    mViewCache.mDirection.V(mViewCache.mDirection.V() +
                            (cStack_260.V() - mViewCache.mDirection.V()) * paraRail->field_0x1c);
    mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
    f32 sp28 = val17 - var_f31 * (val17 - val20);
    mViewCache.mFovy += paraRail->field_0x1c * (sp28 - mViewCache.mFovy);

    return true;
}

bool dCamera_c::rideCamera(s32 param_0) {
    static cSAngle LatMin = -80.0f;
    static cSAngle LatMax = 80.0f;

    f32 val2 = mCamParam.Val(param_0, 2);
    f32 val1 = mCamParam.Val(param_0, 1);
    f32 val0 = mCamParam.Val(param_0, 0);
    f32 val25 = mCamParam.Val(param_0, 25);
    f32 val6 = mCamParam.Val(param_0, 6);
    f32 val5 = mCamParam.Val(param_0, 5);
    f32 val7 = mCamParam.Val(param_0, 7);
    f32 val10 = mCamParam.Val(param_0, 10);
    f32 val11 = mCamParam.Val(param_0, 11);
    f32 val12 = mCamParam.Val(param_0, 12);
    f32 val15 = mCamParam.Val(param_0, 15);
    f32 val22 = mCamParam.Val(param_0, 22);
    f32 val27 = mCamParam.Val(param_0, 27);
    cSAngle val23 = mCamParam.Val(param_0, 23);
    f32 val17 = mCamParam.Val(param_0, 17);
    f32 val20 = mCamParam.Val(param_0, 20);
    f32 val21 = mCamParam.Val(param_0, 21);
    f32 val4 = mCamParam.Val(param_0, 4);
    f32 val9 = mCamParam.Val(param_0, 9);
    f32 val14 = mCamParam.Val(param_0, 14);
    f32 val19 = mCamParam.Val(param_0, 19);
    f32 val24 = mCamParam.Val(param_0, 24);
    f32 val26 = mCamParam.Val(param_0, 26);

    if (!(val26 >= 0.0f && val26 <= 1.0f)) {
        val26 = 0.5f;
    }

    cSAngle sp148 = -15.0f;
    f32 sp1F8 = 0.1f;
    int sp1F4 = 13;
    int sp1F0 = 2;
    f32 sp1EC = 1.0500001f;
    f32 sp1E8 = 1.1500001f;

    cSAngle sp144 = cSAngle::_0;

    #if WIDESCREEN_SUPPORT
    if (mDoGph_gInf_c::isWide()) {
        val22 *= WideTurnSaving;
    }
    #endif

    RideData* wk = (RideData*)mWork;

    int sp1E4 = 20;
    f32 var_f31 = 1.0f;
    daAlink_c* player = (daAlink_c*)daAlink_getAlinkActorClass();
    dAttention_c* attn = dComIfGp_getAttention();

    if (mCurCamStyleTimer == 0) {
        if (mRecovery.field_0x8.field_0x1e <= 0) {
            mViewCache.mCenter = mCenter;
            mViewCache.mEye = mEye;
            mViewCache.mDirection = mDirection;
        }

        wk->field_0x98 = 0;
        wk->field_0x9c = mpPlayerActor;
        wk->field_0xa0 = 0;

        if (player->checkHorseRide()) {
            wk->field_0xa0 = dComIfGp_getHorseActor();
            wk->field_0x98 = (daHorse_c*)wk->field_0xa0;
            wk->field_0x00 = 0;
        } else if (player->checkCargoCarry()) {
            fopAc_ac_c* spA0 = fopAcM_SearchByName(PROC_KAGO);
            if (spA0 != NULL) {
                wk->field_0x9c = spA0;
                wk->field_0xa0 = spA0;
                if (mCurCamStyleTimer != 0) {
                    f32 sp1D4 = cXyz(positionOf(wk->field_0x9c) - wk->field_0x84).abs();
                    mMonitor.field_0x10 = sp1D4;
                }
            }

            setFlag(0x8000);
            wk->field_0x00 = 3;
        }

        wk->field_0x1c = 0;
        wk->field_0x68.x = 0.0f;
        wk->field_0x68.y = 0.0f;
        wk->field_0x68.z = 0.0f;
    }

    field_0x944 = 1;
    wk->field_0x48.Val(directionOf(wk->field_0x9c));
    wk->field_0x08 = 0;
    wk->field_0x84 = positionOf(wk->field_0x9c);

    if (mCurCamStyleTimer == 0) {
        wk->prove = 'RIDE';
        wk->field_0x30 = cXyz::Zero;
        wk->field_0x20 = 0;
        wk->field_0x0c = mFrameCounter;
        wk->field_0x1d = 1;
        wk->field_0x5c.x = 0.0f;
        wk->field_0x4c = mViewCache.mDirection.R();
        wk->field_0x50 = 0.0f;
        wk->field_0x54 = mViewCache.mDirection.V().Degree();
        wk->field_0x58 = 0.0f;
        wk->field_0x5c.y = 1.0f;
        wk->field_0x08 = 0;
        wk->field_0x3c = mViewCache.mCenter;
        wk->field_0x24 = mViewCache.mCenter;
        wk->field_0x90 = 0;
        wk->field_0x18 = -1;
        wk->field_0x10 = 0;
        wk->field_0x14 = 0;
        wk->field_0x78 = 0.0f;
        wk->field_0x91 = 0;
        wk->field_0x5c.z = sp1F8;
        wk->field_0x7c = val27;
        wk->field_0x94 = 0;
        wk->field_0x92 = 0;
        wk->field_0x93 = strncmp(&mCamTypeData[mCurType].name[16], "E_RD", 4) == 0;

        if (mCamParam.Flag(param_0, 0x2000)) {
            wk->field_0x94 = getParamTargetActor(mCurType);
        }

        if (mCamParam.Flag(param_0, 0x1000) && player->checkGoatThrow())
        {
            wk->field_0x94 = dComIfGoat_GetThrow();
            wk->field_0x92 = 1;
        }

        if (player->checkBoardRide()) {
            wk->field_0x94 = 0;
            if (dComIfGs_isTmpBit((u16)dSv_event_tmp_flag_c::tempBitLabels[0x54])) {
                wk->field_0x94 = fopAcM_SearchByName(PROC_NPC_YKM);
            } else if (dComIfGs_isTmpBit((u16)dSv_event_tmp_flag_c::tempBitLabels[0x55])) {
                wk->field_0x94 = fopAcM_SearchByName(PROC_NPC_YKW);
            }
        }

        mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
        if (chkFlag(0x8000)) {
            if (wk->field_0x98 == 0 && !player->checkCanoeRide()) {
                wk->field_0x20 = 2;
                mStyleSettle.mFinished = true;
            }
        } else {
            if (chkFlag(32) && mCurType == specialType[9]) {
                wk->field_0x20 = 3;
            } else if (chkFlag(0x10000) || player->checkOctaIealHang()) {
                wk->field_0x20 = 4;
            } else {
                if (mCamParam.Flag(param_0, 0x400)) {
                    wk->field_0x20 = 5;
                }
            }
        }
    }

    JUT_ASSERT(0x2c12, wk->prove == 'RIDE');

    f32 sp1D0 = mMonitor.field_0x10 / val11;
    if (wk->field_0x98 != NULL) {
        if (wk->field_0x98->getLashDashStart()) {
            onHorseDush();

            wk->field_0x10 = wk->field_0x18 = sp1F4;
            wk->field_0x7c = 0.0f;
        } else {
            if (wk->field_0x10 != 0) {
                wk->field_0x10--;
            } else {
                wk->field_0x7c = wk->field_0x7c + (val27 - wk->field_0x7c) * 0.1f;
                wk->field_0x18 = -1;
            }
        }
    } else {
        wk->field_0x10 = 0;
        wk->field_0x18 = -1;
        wk->field_0x7c = wk->field_0x7c + (val27 - wk->field_0x7c) * 0.1f;
    }

    if (sp1D0 < -1.0f) {
        sp1D0 = -1.0f;
    } else {
        if (sp1D0 > 1.0f) {
            sp1D0 = 1.0f;
        }
    }

    bool sp0F;
    bool sp0E;
    if (mGear == 1) {
        val2 = val4;
        val12 = val14;
        val7 = val9;
        val17 = val19;
        sp0F = false;
        sp0E = false;
    }

    f32 sp1CC = 0.0f;
    f32 sp1C8 = 0.0f;
    if (wk->field_0x92 != 0 && wk->field_0x94 != 0) {
        fopAc_ac_c* sp1C4 = wk->field_0x94;
        f32 sp1C0 = 400.0f;
        f32 sp1BC = 1200.0f;

        cXyz sp4F0 = positionOf(sp1C4) - positionOf(wk->field_0x9c);
        f32 sp1B8 = sp4F0.abs();
        cSAngle sp140 = -mViewCache.mDirection.U();
        sp4F0 = dCamMath::xyzRotateY(sp4F0, sp140);

        f32 sp1B4;
        cXyz sp4E4 = sp4F0;
        if (fabsf(sp4E4.y) < 200.0f) {
            sp4E4.y = 0.0f;
            sp4E4.x *= 0.5f;
            f32 sp1B0 = sp4E4.abs();
            if (sp1B0 < sp1BC) {
                sp1B4 = 1.0f - sp1B0 / sp1BC;
                sp1CC = -sp4F0.x * 0.6f * sp1B4;
            }
        }

        if (mGear != 1) {
            if (sp1B8 < sp1C0) {
                sp1B4 = 1.0f;
            } else if (sp1B8 > sp1BC) {
                sp1B4 = 0.0f;
            } else {
                sp1B4 = 1.0f - (sp1B8 - sp1C0) / (sp1BC - sp1C0);
            }

            val2 += (-80.0f - val2) * sp1B4;
            val12 += (28.0f - val12) * sp1B4;
            val7 += (320.0f - val7) * sp1B4;
            val22 += (0.3f - val22) * sp1B4;
            sp140 = mViewCache.mDirection.U() - directionOf(wk->field_0x9c);
            sp1F8 = 0.25f;
        }
    } else {
        if (wk->field_0x98 != 0) {
            fopAc_ac_c* sp1AC = attn->LockonTarget(0);
            fopAc_ac_c* sp1A8 = attn->LockonTarget(1);
            fopAc_ac_c* sp1A4 = attn->CheckObjectTarget(0);

            if (wk->field_0x94 != 0) {
                cXyz sp4D8 = attentionPos(wk->field_0x9c);
                cXyz sp4CC = attentionPos(wk->field_0x94);
                f32 sp1A0 = dCamMath::xyzHorizontalDistance(sp4D8, sp4CC);
                if (sp1A0 < 6000.0f) {
                    sp1AC = wk->field_0x94;
                    sp1A8 = NULL;
                }
            }

            if (sp1AC != NULL &&
                (fopAcM_GetName(sp1AC) == PROC_E_RD || fopAcM_GetName(sp1AC) == PROC_E_FK ||
                 fopAcM_GetName(sp1AC) == PROC_B_GND || fopAcM_GetName(sp1AC) == PROC_E_KR))
            {
                cXyz sp4C0 = positionOf(sp1AC) - positionOf(wk->field_0x9c);
                cSAngle spF8 = -mViewCache.mDirection.U();
                sp4C0 = dCamMath::xyzRotateY(sp4C0, spF8);
                cXyz sp4B4 = sp4C0;
                if (fabsf(sp4B4.y) < 200.0f) {
                    sp4B4.y = 0.0f;
                    sp4B4.x *= 0.5f;
                    f32 sp19C = sp4B4.abs();
                    if (sp19C < 1000.0f) {
                        sp1CC = -sp4C0.x * 0.6f * (1.0f - sp19C / 1000.0f);
                        sp1CC = sp1CC * sp1D0;
                    }
                }
            }

            if (sp1A4 != NULL) {
                cXyz sp4A8 = positionOf(sp1A4) - positionOf(wk->field_0x9c);
                cSAngle spF4 = -mViewCache.mDirection.U();
                sp4A8 = dCamMath::xyzRotateY(sp4A8, spF4);
                cXyz sp49C = sp4A8;
                if (fabsf(sp49C.y) < 200.0f) {
                    sp49C.y = 0.0f;
                    sp49C.x *= 0.5f;
                    f32 sp198 = sp49C.abs();
                    if (sp198 < 800.0f) {
                        sp1CC = -sp4A8.x * 0.8f * (1.0f - sp198 / 800.0f);
                        sp1CC = sp1CC * sp1D0;
                    }
                }
            }

            if (sp1AC != NULL && fopAcM_GetName(sp1AC) == PROC_E_KR) {
                val12 = -5.0f;
                val15 = 10.0f;
                val7 = 800.0f;
                val10 = -100.0f;
                val2 = -20.0f;
            } else {
                if (sp1AC != NULL &&
                    (fopAcM_GetName(sp1AC) == PROC_E_RD &&
                         (fopAcM_GetParam(sp1AC) & 4) != 0 ||
                     fopAcM_GetName(sp1AC) == PROC_B_GND) &&
                    sp1A8 == NULL)
                {
#if DEBUG
                    if (mCamSetup.CheckFlag(0x8000)) {
                        dDbVw_Report(20, 0xeb, "  RB");
                    }
#endif

                    if (wk->field_0x93 != 0) {
                        val7 = 550.0f;
                        val2 = -120.0f;
                        val12 = 23.0f;
                        val17 = 60.0f;
                    } else {
                        val7 = 780.0f;
                        val10 = -150.0f;
                        val12 = 25.0f;
                        val15 = -10.0f;
                        val17 = 55.0f;
                        val20 = 15.0f;
                    }
                } else {
                    if (sp1AC != NULL && (fopAcM_GetName(sp1AC) == PROC_E_RD ||
                                              fopAcM_GetName(sp1AC) == PROC_E_FK) ||
                        sp1A8 != NULL && (fopAcM_GetName(sp1A8) == PROC_E_RD ||
                                              fopAcM_GetName(sp1A8) == PROC_E_FK) ||
                        sp1A4 != NULL)
                    {
#if DEBUG
                        if (mCamSetup.CheckFlag(0x8000)) {
                            dDbVw_Report(20, 0xeb, "  RD");
                        }
#endif

                        if (mGear == 1) {
                            val7 -= 1000.0f;
                            val12 += 5.0f;
                        } else {
                            val2 = -80.0f;
                            val25 = -100.0f;
                            val1 = 300.0f;
                            val12 = 20.0f;
                            val15 = 5.0f;
                            val7 = 1250.0f;
                            val10 = -200.0f;
                            val20 = 10.0f;
                        }
                    }
                }
            }

            if (wk->field_0x98->checkJump()) {
                if (wk->field_0x91 == 0) {
                    wk->field_0x74 = 0.01f;
                    wk->field_0x91 = 1;
                }
                val5 = 0.5f;
                val6 = 0.4f;
                if (mViewCache.mDirection.V() < (cSAngle)10.0f) {
                    val25 = -20.0f;
                    val12 += 10.0f;
                } else {
                    val25 = -100.0f;
                    val12 -= 15.0f;
                }
                val7 = val7 * 0.75f;
                val17 += 5.0f;
                sp1F8 = 0.75f;
                if (wk->field_0x93 != 0) {
                    val2 = 0.0f;
                }
                sp1CC = 0.0f;
            } else {
                wk->field_0x91 = 0;
            }
        } else {
            if (player->checkCanoeRideTandem()) {
                wk->field_0x91 = 0;
                sp1C8 = -100.0f;
            } else {
                wk->field_0x91 = 0;
                if (player->checkBoardRide()) {
                    wk->field_0x91 = 0;
                    daAlink_c* sp194 = (daAlink_c*)wk->field_0x94;
                    if (sp194 != NULL) {
                        cXyz sp490 = positionOf(sp194) - positionOf(wk->field_0x9c);
                        if (sp490.abs() < 1200.0f) {
                            val2 = -30.0f;
                            val25 = 10.0f;
                            val1 = 50.0f;
                            val12 = 34.0f;
                            val15 = 2.0f;
                            val7 = 900.0f;
                            val10 = -50.0f;
                            val20 = 10.0f;
                            cSAngle sp134 = -mViewCache.mDirection.U();
                            sp490 = dCamMath::xyzRotateY(sp490, sp134);
                            cXyz sp484 = sp490;
                            cSAngle sp130 = sAngleY(sp490);
                            f32 var_f30 = sp130.Cos();
                            val7 += var_f30 * 300.0f;
                            val12 += var_f30 * 4.0f;

#if DEBUG
                            if (mCamSetup.CheckFlag(0x8000)) {
                                dDbVw_Report(20, 0xeb, "  YK %f", (f32)var_f30);
                            }
#endif

                            if (fabsf(sp484.y) < 200.0f) {
                                sp484.y = 0.0f;
                                sp484.x *= 0.5f;
                                f32 sp18C = sp484.abs();
                                if (sp18C < 1000.0f) {
                                    sp1CC = -sp490.x * 0.6f * (1.0f - sp18C / 1200.0f);
                                    sp1CC *= sp1D0;
                                }
                            }
                        }
                    }
                }

                if (!isPlayerFlying(player) || !player->checkSpinnerRide() && !player->checkBoardRide()) {
                    wk->field_0x78 = 0.0f;
                    wk->field_0x14 = 0;
                } else if (wk->field_0x14 < 40) {
                    wk->field_0x91 = 1;
                    wk->field_0x78 += (0.5f - wk->field_0x78) * dCamMath::rationalBezierRatio(wk->field_0x14 / 40.0f, 1.25f);
                    wk->field_0x14++;
                } else {
                    sp0E = true;
                    wk->field_0x91 = 1;
                    wk->field_0x78 += (1.0f - wk->field_0x78) * 0.1f;
                }
            }
        }
    }

    wk->field_0x74 += (val5 - wk->field_0x74) * 0.05f;
    wk->field_0x74 += (1.0f - wk->field_0x74) * wk->field_0x78;

    if (mCamSetup.CheckFlag(0x8000)) {
        dComIfGp_evmng_cameraPlay();
    }

    wk->field_0x5c.z += (sp1F8 - wk->field_0x5c.z) * 0.25f;
    wk->field_0x4c += wk->field_0x5c.z * (val7 - wk->field_0x4c);
    wk->field_0x50 += wk->field_0x5c.z * (val10 - wk->field_0x50);
    wk->field_0x54 += wk->field_0x5c.z * (val12 - wk->field_0x54);
    wk->field_0x58 += wk->field_0x5c.z * (val15 - wk->field_0x58);
    f32 sp188 = 0.0f;

    if (check_owner_action1(mPadID, 0x2000000)) {
        if (player->getHookshotLeft()) {
            sp1CC = -15.0f;
        } else {
            sp1CC = 15.0f;
        }
    } else {
        if (sp1D0 > val21 && mGear != 1) {
            f32 sp184 = 1.0f / (1.0f - val21);
            sp188 = dCamMath::rationalBezierRatio(sp184 * (sp1D0 - val21), 0.1f);
        }
    }

    f32 sp180 = wk->field_0x4c + wk->field_0x50 * sp188;
    f32 sp17C = wk->field_0x54 + wk->field_0x58 * sp188;
    f32 sp178 = val17 + val20 * sp188;

    cSGlobe sp260;
    cSAngle sp12C;
    wk->field_0x68.y = val2 + val25 * sp188;
    wk->field_0x68.z += (sp1C8 + val1 * sp188 - wk->field_0x68.z) * 0.1f;
    wk->field_0x68.x += (sp1CC - wk->field_0x68.x) * 0.05f;
    cXyz sp478(wk->field_0x68.x, wk->field_0x68.y, wk->field_0x68.z);
    sp260.Val(sp180, cAngle::d2s(sp17C), wk->field_0x48.Inv());

    if (player->checkOctaIealHang() && mGear != 1) {
        sp260.U(cSAngle(sp148) + directionOf(wk->field_0x9c).Inv());
    }

    if (mCurCamStyleTimer == 0) {
        wk->field_0x74 = val5;
        wk->field_0x80 = 1.0f;
    } else {
        wk->field_0x80 += (1.0f - wk->field_0x80) * 0.025f;
    }

    dBgS_CamLinChk sp510;
    cXyz sp46C = attentionPos(wk->field_0x9c);
    cXyz sp460 = relationalPos(wk->field_0x9c, &sp478);
    if (lineBGCheck(&sp46C, &sp460, &sp510, 0x40b7)) {
        cM3dGPla sp4FC;
        dComIfG_Bgsp().GetTriPla(sp510, &sp4FC);
        sp460 = sp510.GetCross();
        sp460 += *sp4FC.GetNP();
    }

    cXyz sp454(val6, wk->field_0x74, val6);
    wk->field_0x5c.y += (1.0f - wk->field_0x5c.y) * 0.2f;
    wk->field_0x3c += (sp460 - wk->field_0x3c) * wk->field_0x5c.y;
    jutOutCheck(&wk->field_0x3c, 5.0f);

    cSAngle sp128;
    if (!mStyleSettle.mFinished) {
        switch (wk->field_0x20) {
        case 1:
            sp1E4 = 40;
            var_f31 = 1.0f / (f32)(s32)(sp1E4 - mCurCamStyleTimer);
            mViewCache.mCenter += (wk->field_0x3c - mViewCache.mCenter) * var_f31;
            mViewCache.mEye = wk->field_0x24;
            mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);

            break;
        case 2:
            sp1E4 = 1;

            break;
        case 3:
            sp1E4 = 1;
            mViewCache.mCenter = wk->field_0x3c;
            var_f31 = 1.0f;
            mViewCache.mDirection.R(sp260.R());
            mViewCache.mDirection.V(sp260.V());
            mViewCache.mDirection.U(sp260.U());
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();

            break;
        case 4:
            sp1E4 = 8;
            var_f31 = 1.0f / (f32)(s32)(sp1E4 - mCurCamStyleTimer);
            mViewCache.mCenter += (wk->field_0x3c - mViewCache.mCenter) * sp454;
            mViewCache.mDirection.R(mViewCache.mDirection.R() +
                                    (sp260.R() - mViewCache.mDirection.R()) * var_f31);
            mViewCache.mDirection.V(mViewCache.mDirection.V() +
                                    (sp260.V() - mViewCache.mDirection.V()) * var_f31);
            mViewCache.mDirection.U(mViewCache.mDirection.U() +
                                    (sp260.U() - mViewCache.mDirection.U()) * var_f31);
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();

            break;
        case 5:
            sp1E4 = 8;
            var_f31 = 1.0f / (f32)(s32)(sp1E4 - mCurCamStyleTimer);
            mViewCache.mCenter += mMonitor.field_0x14;
            mViewCache.mCenter += (wk->field_0x3c - mViewCache.mCenter) * var_f31;
            sp128 = sp260.V();
            if (wk->field_0x9c != NULL && mCamParam.Flag(param_0, 0x100) && mGear != 1) {
                sp128 += wk->field_0x9c->shape_angle.x;
            }

            mViewCache.mDirection.R(mViewCache.mDirection.R() +
                                    (sp260.R() - mViewCache.mDirection.R()) * var_f31);
            mViewCache.mDirection.V(mViewCache.mDirection.V() +
                                    (sp128 - mViewCache.mDirection.V()) * var_f31);
            mViewCache.mDirection.U(mViewCache.mDirection.U() +
                                    (sp260.U() - mViewCache.mDirection.U()) * var_f31);
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();

            break;
        default:
            sp1E4 = mCurMode == 1 ? 8 : 20;

            var_f31 = 1.0f / (f32)(s32)(sp1E4 - mCurCamStyleTimer);
            mViewCache.mCenter += (wk->field_0x3c - mViewCache.mCenter) * var_f31;
            sp128 = sp260.V();
            if (wk->field_0x9c != 0 && mCamParam.Flag(param_0, 0x100) != 0 && mGear != 1) {
                sp128 += wk->field_0x9c->shape_angle.x;
            }
            mViewCache.mDirection.R(mViewCache.mDirection.R() +
                                    (sp260.R() - mViewCache.mDirection.R()) * var_f31);
            mViewCache.mDirection.V(mViewCache.mDirection.V() +
                                    (sp128 - mViewCache.mDirection.V()) * var_f31);
            mViewCache.mDirection.U(mViewCache.mDirection.U() +
                                    (sp260.U() - mViewCache.mDirection.U()) * var_f31);
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();

            break;
        }

        mViewCache.mFovy += var_f31 * (sp178 - mViewCache.mFovy);

        if (mCurCamStyleTimer == sp1E4 - 1) {
            OS_REPORT("ready ok (%d)\n", wk->field_0x20);
            mStyleSettle.mFinished = true;
        }

        return true;
    }

    bool sp0D = true;
    if (mGear == 1 && !player->checkCanoeRide()) {
        sp0D = false;
    }

    mViewCache.mCenter += (wk->field_0x3c - mViewCache.mCenter) * sp454;
    f32 sp174 = mPadInfo.mCStick.mLastPosX;
    f32 sp170;
    if (wk->field_0x10 != 0 && wk->field_0x18 - wk->field_0x10 > (s32)sp1F0 && mGear == 0) {
        f32 var_f1_0 = 1.0f / wk->field_0x10;
        sp170 = mViewCache.mDirection.R() + var_f1_0 * (sp1EC * sp260.R() - mViewCache.mDirection.R());
    } else {
        sp170 = mViewCache.mDirection.R() + wk->field_0x7c * (sp260.R() - mViewCache.mDirection.R());
    }

    if (sp0D) {
        var_f31 = val22 * val26;
        var_f31 += val22 * (1.0f - val26) * sp1D0;
    } else if (wk->field_0x93 != 0) {
        cSAngle acStack_4ac = directionOf(wk->field_0x9c).Inv() - mViewCache.mDirection.U();
        var_f31 = val24 * fabsf(acStack_4ac.Sin());
    } else if (mCamParam.Flag(param_0, 0x1000) != 0) {
        var_f31 = val22;
    } else {
        cSAngle acStack_4b0 = directionOf(wk->field_0x9c).Inv() - mViewCache.mDirection.U();
        f32 sp168 = sp1D0 * fabsf(acStack_4b0.Sin());
        var_f31 = val24 * sp168;
    }

    cSAngle sp11C;
    cSAngle sp118;
    if (push_any_key()) {
        wk->field_0x90 = 0;
    }

    bool sp0C = false;
    bool sp0B = false;
    f32 sp164 = 8.0f;
    var_f31 *= wk->field_0x80;
    f32 sp160 = 0.05f;
    if (!mCamParam.Flag(param_0, 0x40) && (fabsf(sp174) > sp160 || wk->field_0x90 != 0)) {
        cSAngle sp8C = mViewCache.mDirection.U() + (cSAngle)(sp164 * dCamMath::rationalBezierRatio(sp174, 0.5f));
        var_f31 = fabsf(sp174) - sp160;
        sp11C.Val(mViewCache.mDirection.U() +
                        (sp8C - mViewCache.mDirection.U()) * var_f31);
        wk->field_0x90 = 1;
        wk->field_0x80 = 0.0f;
        sp0B = true;
    } else {
        if (mCamParam.Flag(param_0, 0x1000)) {
            cSAngle sp110 = sp144 + directionOf(wk->field_0x9c).Inv();
            sp11C.Val(mViewCache.mDirection.U() + (sp110 - mViewCache.mDirection.U()) * var_f31);
        } else if (wk->field_0x98 != 0) {
            if (wk->field_0x98->checkTurnStandCamera()) {
                sp11C.Val(mViewCache.mDirection.U());
            } else {
                sp11C.Val(mViewCache.mDirection.U() +
                                (sp260.U() - mViewCache.mDirection.U()) * var_f31);
            }
        } else {
            sp11C.Val(mViewCache.mDirection.U() + (sp260.U() - mViewCache.mDirection.U()) * var_f31);
        }
    }

    if (!sp0B) {
        wk->field_0x80 = wk->field_0x80 + (1.0f - wk->field_0x80) * 0.01f;
    }

    cSGlobe sp258 = mViewCache.mEye - mViewCache.mCenter;
    f32 sp15C = 0.25f;
    if (wk->field_0x9c != 0 && mCamParam.Flag(param_0, 0x100) != 0) {
        if (player->checkBoardRide()) {
            sp258.V(sp260.V() + wk->field_0x9c->shape_angle.x);
        } else {
            sp258.V(wk->field_0x9c->shape_angle.x);
        }
        sp15C = 0.4f;
    } else {
        if (player->checkOctaIealHang() && mGear != 1) {
            f32 sp158 = cSAngle(sp260.U() - sp11C).Cos();
            sp118.Val(wk->field_0x9c->shape_angle.x);
            sp260.V(sp260.V() + sp118 * sp158);
            sp15C = 0.0f;
        }
    }

    if (wk->field_0x98 != 0 && wk->field_0x91 != 0 && mMonitor.field_0x14.y < -1.0f) {
        sp15C = 0.9f;
    } else {
        if (wk->field_0x91 != 0) {
            sp15C = (sp15C + (1.0f - sp15C) * wk->field_0x78);
            sp258.V(sp258.V() + cSAngle(10.0f));
        }
    }

    sp260.V(sp260.V() + (sp258.V() - sp260.V()) * sp15C);

    cSAngle sp10C = mViewCache.mDirection.V() + (sp260.V() - mViewCache.mDirection.V()) * wk->field_0x7c;
    if (sp10C < LatMin) {
        sp10C.Val(LatMin);
    } else {
        if (sp10C > LatMax) {
            sp10C.Val(LatMax);
        }
    }

    mViewCache.mDirection.Val(sp170, sp10C, sp11C);
    mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();

    if (wk->field_0x10 != 0 && wk->field_0x18 - wk->field_0x10 > (s32)sp1F0 && mGear == 0) {
        f32 sp154 = 1.0f / (f32)wk->field_0x10;
        mViewCache.mFovy =
            mViewCache.mFovy + sp154 * (sp178 * sp1E8 - mViewCache.mFovy);
    } else {
        mViewCache.mFovy = mViewCache.mFovy + wk->field_0x7c * (sp178 - mViewCache.mFovy);
    }

    if (sp0D) {
        mViewCache.mBank +=
            (val23 * mPadInfo.mMainStick.mLastPosX - mViewCache.mBank) * (fabsf(sp1D0) * 0.01f);
        setFlag(0x400);
    } else {
        mViewCache.mBank += (cSAngle::_0 - mViewCache.mBank) * 0.05f;
        setFlag(0x400);
    }

    return true;
}

bool dCamera_c::manualCamera(s32 param_0) {
    return true;
}

bool dCamera_c::observeCamera(s32 param_0) {

    f32 dVar8 = mCamSetup.CurveWeight();
    f32 val0 = mCamParam.Val(param_0, 0);
    f32 val17 = mCamParam.Val(param_0, 17);
    cSAngle val16 = mCamParam.Val(param_0, 16);
    cSAngle val24 = mCamParam.Val(param_0, 24);

    ObserveData* observe = (ObserveData*)mWork;

    if (mCurCamStyleTimer == 0) {
        observe->field_0x00 = 'OBSV';
        mStyleSettle.mFinished = true;
        if (mRoomMapTool.mCameraIndex != 0xff && mCamParam.Flag(param_0, 0x200) != 0)
        {
            mCamParam.Fovy((mRoomMapTool).mCamData.field_0x11);
            mCamParam.Arg0((mRoomMapTool).mCamData.field_0x12);
            mCamParam.Arg1((mRoomMapTool).mCamData.field_0x13);
            mCamParam.Arg2((mRoomMapTool).mCamData.field_0x14);
            if (param_0 == mCamTypeData[mCurType].field_0x18[mIsWolf][mCurMode]) {
                mCamParam.Change(param_0);
            }
        } else {
            mCamParam.Fovy(0xff);
            mCamParam.Arg0(0xff);
            mCamParam.Arg1(0xff);
            mCamParam.Arg2(-1);
            if (param_0 == mCamTypeData[mCurType].field_0x18[mIsWolf][mCurMode]) {
                mCamParam.Change(param_0);
            }
        }
        if (mRoomMapTool.mArrowIndex != 0xff) {
            observe->field_0x04 = cXyz(mRoomMapTool.mArrowData.position.x, mRoomMapTool.mArrowData.position.y,
                           mRoomMapTool.mArrowData.position.z);
            cSGlobe cStack_c0(50.0f, -mRoomMapTool.mArrowData.angle.x,
                              mRoomMapTool.mArrowData.angle.y);
            observe->field_0x28 = observe->field_0x04 + cStack_c0.Xyz();
        } else {
            observe->field_0x04 = mViewCache.mEye;
            cSGlobe cStack_c8(50.0f, mViewCache.mDirection.V().Inv(), mViewCache.mDirection.U().Inv());
            observe->field_0x28 = observe->field_0x04 + cStack_c8.Xyz();
        }
        observe->field_0x10.Val(observe->field_0x04 - observe->field_0x28);
        mViewCache.mCenter = observe->field_0x28;
        mViewCache.mEye = observe->field_0x04;
        mViewCache.mDirection = observe->field_0x10;
    }

    if (mCamParam.Flag(param_0, 0x200)) {
        if (mCamParam.Fovy() != 0xff) {
            val17 = (s32)(s8)mCamParam.Fovy();
        }
        if (mCamParam.Arg0() != 0xff) {
            val16 = (f32)(s32)(s8)mCamParam.Arg0();
        }
        if (mCamParam.Arg1() != 0xff) {
            val24 = (f32)(s32)(s8)mCamParam.Arg1();
        }
    }

    f32 sp54;
    f32 sp50;
    int zero = 0;
    if (zero == 0) {
        if (mPadInfo.mMainStick.mLastPosX >= 0.75f) {
            sp50 = 1.0f;
        } else if (mPadInfo.mMainStick.mLastPosX <= -0.75f) {
            sp50 = -1.0f;
        } else {
            sp50 = dCamMath::rationalBezierRatio(mPadInfo.mMainStick.mLastPosX * 1.3333334f, dVar8);
        }

        if (mPadInfo.mMainStick.mLastPosY >= 0.75f) {
            sp54 = 1.0f;
        } else if (mPadInfo.mMainStick.mLastPosY <= -0.75f) {
            sp54 = -1.0f;
        } else {
            sp54 = dCamMath::rationalBezierRatio(mPadInfo.mMainStick.mLastPosY * 1.3333334f, dVar8);
        }

        if (dComIfGs_getOptCameraControl() != 0) {
            sp54 = -sp54;
        }
    }

    cSAngle sp40;
    cSAngle sp3C = mViewCache.mDirection.V() + (cSAngle)(val0 * sp54);
    sp40 = sp3C - observe->field_0x10.V();
    if (sp40 > val16) {
        sp3C = observe->field_0x10.V() + val16;
    } else if (sp40 < -val16) {
        sp3C = observe->field_0x10.V() - val16;
    }

    mViewCache.mDirection.V(sp3C);
    cSAngle sp38 = mViewCache.mDirection.U() - (cSAngle)(val0 * sp50);
    sp40 = sp38 - observe->field_0x10.U();
    if (sp40 > val24) {
        sp38 = observe->field_0x10.U() + val24;
    } else if (sp40 < -val24) {
        sp38 = observe->field_0x10.U() - val24;
    }

    mViewCache.mDirection.U(sp38);
    mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
    mViewCache.mFovy = val17;

    return true;
}

bool dCamera_c::fixedFrameCamera(s32 param_0) {
    f32 val24 = mCamParam.Val(param_0, 24);
    f32 val17 = mCamParam.Val(param_0, 17);

    FixedFrameData* fixedFrame = (FixedFrameData*)mWork;

    if (mCurCamStyleTimer == 0) {
        fixedFrame->mArrowIndex = 0xff;
        mStyleSettle.mFinished = false;
    }

    if (mRoomMapTool.mArrowIndex == 0xff) {
        return false;
    }

    if (fixedFrame->mArrowIndex != mRoomMapTool.mArrowIndex) {
        mCurCamStyleTimer = 0;
        fixedFrame->mArrowIndex = mRoomMapTool.mArrowIndex;
        mStyleSettle.mFinished = false;
    }

    if (mCurCamStyleTimer == 0) {
        fixedFrame->field_0x00 = 'FIXF';
        fixedFrame->field_0x04 = cXyz(mRoomMapTool.mArrowData.position.x, mRoomMapTool.mArrowData.position.y,
                        mRoomMapTool.mArrowData.position.z);

        cSGlobe spFC(100000.0f, -mRoomMapTool.mArrowData.angle.x,
                           mRoomMapTool.mArrowData.angle.y);
        fixedFrame->field_0x2c = fixedFrame->field_0x38 = fixedFrame->field_0x04 + spFC.Xyz();

        cM3dGLin sp14C(fixedFrame->field_0x2c, fixedFrame->field_0x04);
        cXyz sp12C = attentionPos(this->mpPlayerActor);
        cXyz sp120;
        f32 sp30;
        if (cM3d_Len3dSqPntAndSegLine(&sp14C, &sp12C, &sp120, &sp30) != 0) {
            fixedFrame->field_0x2c = sp120;
        } else {
            f32 temp = cXyz(fixedFrame->field_0x04 - attentionPos(mpPlayerActor)).abs();
            spFC.Val(temp, -mRoomMapTool.mArrowData.angle.x,
                           mRoomMapTool.mArrowData.angle.y);
            fixedFrame->field_0x2c = fixedFrame->field_0x04 + spFC.Xyz();
        }

        dBgS_CamLinChk dStack_108;
        if (lineBGCheck(&fixedFrame->field_0x04, &fixedFrame->field_0x2c, &dStack_108, 0x40b7)) {
            cM3dGPla sp138;
            dComIfG_Bgsp().GetTriPla(dStack_108, &sp138);
            cXyz cross = dStack_108.GetCross();
            fixedFrame->field_0x2c = cross - *sp138.GetNP() * 5.0f;
        }
        fixedFrame->field_0x10.Val(fixedFrame->field_0x04 - fixedFrame->field_0x2c);

        fixedFrame->field_0x24 = mRoomMapTool.mCamData.field_0x11 == 0xff ? val17 : mRoomMapTool.mCamData.field_0x11;

        s32 local_240 = mRoomMapTool.mCamData.field_0x13 == 0xff ? -1 : mRoomMapTool.mCamData.field_0x13;

        if (local_240 == -1) {
            f32 temp1 = cXyz(fixedFrame->field_0x04 - mEye).abs();
            temp1 /= heightOf(mpPlayerActor);
            fixedFrame->field_0x18 = (s32)(val24 * JMAFastSqrt(temp1)) + 1;
        } else {
            fixedFrame->field_0x18 = local_240;
        }
        fixedFrame->field_0x1c = (fixedFrame->field_0x18 * (fixedFrame->field_0x18 + 1)) >> 1;
    }
    if (!mStyleSettle.mFinished) {
        fixedFrame->field_0x20 = (f32)(s32)(fixedFrame->field_0x18 - mCurCamStyleTimer);
        f32 fVar11 = fixedFrame->field_0x20 / fixedFrame->field_0x1c;
        fixedFrame->field_0x1c -= fixedFrame->field_0x20;

        mViewCache.mCenter += (fixedFrame->field_0x2c - mViewCache.mCenter) * fVar11;
        mViewCache.mDirection.R(mViewCache.mDirection.R() +
                                fVar11 * (fixedFrame->field_0x10.R() - mViewCache.mDirection.R()));
        mViewCache.mDirection.V(mViewCache.mDirection.V() +
                                (fixedFrame->field_0x10.V() - mViewCache.mDirection.V()) * fVar11);

        mViewCache.mDirection.U(mViewCache.mDirection.U() +
                     (fixedFrame->field_0x10.U() - mViewCache.mDirection.U()) * fVar11);

        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mFovy = mViewCache.mFovy + fVar11 * (fixedFrame->field_0x24 - mViewCache.mFovy);

        if (mCurCamStyleTimer >= fixedFrame->field_0x18 - 1) {
            mStyleSettle.mFinished = true;
        }
    }

    return true;
}

bool dCamera_c::fixedPositionCamera(s32 param_0) {
    f32 focusOffsetX = mCamParam.Val(param_0, 0);
    f32 focusOffsetY = mCamParam.Val(param_0, 2);
    f32 focusOffsetZ = mCamParam.Val(param_0, 1);
    f32 centerLerpGainY = mCamParam.Val(param_0, 5);
    f32 centerLerpGainXZ = mCamParam.Val(param_0, 6);
    f32 targetRadiusMax = mCamParam.Val(param_0, 7);
    f32 targetRadiusMin = mCamParam.Val(param_0, 8);
    f32 val24 = mCamParam.Val(param_0, 24);
    f32 val23 = mCamParam.Val(param_0, 23);
    f32 targetFovY = mCamParam.Val(param_0, 17);

    FixedPositionData* fixedPosition = (FixedPositionData*)mWork;

    cXyz anchorPos;
    if (mCurCamStyleTimer == 0) {
        fixedPosition->field_0x00 = 'FIXP';
        fixedPosition->field_0x1c = cXyz::Zero;
        fixedPosition->field_0x2c = 0xff;
        fixedPosition->field_0x30 = 0xfe;
    }
    if (mRoomMapTool.mArrowIndex != fixedPosition->field_0x2c) {
        mCurCamStyleTimer = 0;
        mStyleSettle.mFinished = false;
        fixedPosition->field_0x2c = mRoomMapTool.mArrowIndex;
    }
    fixedPosition->field_0x28 = 0;

    f32 desiredFovY;

    u32 sp5C;
    if (mCamParam.Flag(param_0, 0x200) && mRoomMapTool.mArrowIndex != 0xff) {
        if (mRoomMapTool.mCameraIndex != fixedPosition->field_0x30) {
            mCamParam.Fovy(mRoomMapTool.mCamData.field_0x11);
            mCamParam.Arg0(mRoomMapTool.mCamData.field_0x12);
            mCamParam.Arg1(mRoomMapTool.mCamData.field_0x13);
            mCamParam.Arg2(mRoomMapTool.mCamData.field_0x14);
            if (param_0 == mCamTypeData[mCurType].field_0x18[mIsWolf][mCurMode]) {
                mCamParam.Change(param_0);
            }
            fixedPosition->field_0x30 = mRoomMapTool.mCameraIndex;
        }

        anchorPos.x = mRoomMapTool.mArrowData.position.x;
        anchorPos.y = mRoomMapTool.mArrowData.position.y;
        anchorPos.z = mRoomMapTool.mArrowData.position.z;
        if (fixedPosition->field_0x1c != anchorPos) {
            setUSOAngle();
        }

        fixedPosition->field_0x1c = anchorPos;
        if (mCamParam.Fovy() != 0xff) {
            f32 fovY = mCamParam.Fovy();
            mCamParam.SetVal(param_0, 17, fovY);
        }

        if (mCamParam.Arg1() == 0xff) {
            sp5C = -1;
        } else {
            sp5C = mCamParam.Arg1();
            fixedPosition->field_0x28 = 1;
        }
    } else {
        anchorPos = mEye;
        sp5C = -1;
    }

    desiredFovY = targetFovY;
    cXyz focusOffset(focusOffsetX, focusOffsetY, focusOffsetZ);
    if (mCurCamStyleTimer == 0) {
        if (mFrameCounter == 0) {
            fixedPosition->field_0x04 = 1;
            fixedPosition->field_0x08 = fixedPosition->field_0x04;
        } else if (fixedPosition->field_0x28 == 0) {
            cXyz sp1A8 = anchorPos;
            cSGlobe sp184 = anchorPos - mViewCache.mCenter;
            if (sp184.R() > targetRadiusMax) {
                sp184.R(targetRadiusMax);
            }
            sp1A8 = mViewCache.mCenter + sp184.Xyz();
            f32 sp54 = cXyz(sp1A8 - mEye).abs();
            f32 sp50 = cXyz(mCenter - relationalPos(mpPlayerActor, &focusOffset)).abs();
            f32 sp4C = sp54 > sp50 ? sp54 : sp50;
            f32 sp48 = heightOf(mpPlayerActor);
            sp4C /= sp48 < 10.0f ? 10.0f : sp48;
            fixedPosition->field_0x04 = (s32)(val24 * JMAFastSqrt(sp4C)) + 1;
            fixedPosition->field_0x08 = (fixedPosition->field_0x04 * (fixedPosition->field_0x04 + 1)) >> 1;
        } else {
            fixedPosition->field_0x04 = sp5C;
            fixedPosition->field_0x08 = fixedPosition->field_0x04;
        }
        fixedPosition->field_0x10 = mViewCache.mCenter;
    }

    cXyz centerLerpGain(centerLerpGainXZ, centerLerpGainY, centerLerpGainXZ);
    cXyz targetCenter = relationalPos(mpPlayerActor, &focusOffset);
    if (mCamParam.Flag(param_0, 0x4000)) {
        targetCenter.y = anchorPos.y;
    }

    if (!mStyleSettle.mFinished) {
        if (mFrameCounter == 0) {
            centerLerpGain.x = 1.0f;
            centerLerpGain.y = 1.0f;
            centerLerpGain.z = 1.0f;
        }

        f32 frameLerpRatio;
        if (fixedPosition->field_0x28 == 0) {
            fixedPosition->field_0x0c = (s32)(fixedPosition->field_0x04 - mCurCamStyleTimer);
            frameLerpRatio = fixedPosition->field_0x0c / fixedPosition->field_0x08;
            fixedPosition->field_0x08 -= fixedPosition->field_0x0c;
        } else {
            frameLerpRatio = 1.0f / fixedPosition->field_0x08;
            fixedPosition->field_0x08 -= 1.0f;
        }

        fixedPosition->field_0x10 += (targetCenter - fixedPosition->field_0x10) * frameLerpRatio;
        mViewCache.mCenter += (fixedPosition->field_0x10 - mViewCache.mCenter) * centerLerpGain;
        cSGlobe targetDirection = anchorPos - mViewCache.mCenter;
        if (mCamParam.Flag(param_0, 0x20)) {
            targetDirection.V((s16)-mRoomMapTool.mArrowData.angle.x);
        }

        if (targetDirection.R() < targetRadiusMin) {
            targetDirection.R(targetRadiusMin);
        }

        if (targetDirection.R() > targetRadiusMax) {
            targetDirection.R(targetRadiusMax);
        }

        mViewCache.mDirection.R((mViewCache.mDirection.R() + (targetDirection.R() - mViewCache.mDirection.R()) * frameLerpRatio));
        mViewCache.mDirection.V(mViewCache.mDirection.V() +
                                (targetDirection.V() - mViewCache.mDirection.V()) * frameLerpRatio);
        mViewCache.mDirection.U(mViewCache.mDirection.U() +
                                (targetDirection.U() - mViewCache.mDirection.U()) * frameLerpRatio);
        mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
        mViewCache.mFovy += (desiredFovY - mViewCache.mFovy) * frameLerpRatio;
        if (mCurCamStyleTimer >= fixedPosition->field_0x04 - 1) {
            mStyleSettle.mFinished = true;
        }

        return true;
    }

    mViewCache.mCenter += (targetCenter - mViewCache.mCenter) * centerLerpGain;
    cSGlobe cStack_1a8 = anchorPos - mViewCache.mCenter;
    if (mCamParam.Flag(param_0, 0x20)) {
        cStack_1a8.V((s16)-mRoomMapTool.mArrowData.angle.x);
    }
    if (cStack_1a8.R() < targetRadiusMin) {
        cStack_1a8.R(targetRadiusMin);
    }
    if (cStack_1a8.R() > targetRadiusMax) {
        cStack_1a8.R(targetRadiusMax);
    }

    mViewCache.mDirection.R(mViewCache.mDirection.R() +
                            (cStack_1a8.R() - mViewCache.mDirection.R()) * val23);
    mViewCache.mDirection.V(mViewCache.mDirection.V() +
                            (cStack_1a8.V() - mViewCache.mDirection.V()) * val23);
    mViewCache.mDirection.U(mViewCache.mDirection.U() +
                            (cStack_1a8.U() - mViewCache.mDirection.U()) * val23);
    mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
    mViewCache.mFovy = mViewCache.mFovy + val23 * (desiredFovY - mViewCache.mFovy);

    return true;
}

bool dCamera_c::oneSideCamera(s32 param_1) {
    f32 unkRatio;

    f32 focusOffsetX = mCamParam.Val(param_1, 0);
    f32 focusOffsetY = mCamParam.Val(param_1, 2);
    f32 focusOffsetZ = mCamParam.Val(param_1, 1);
    f32 val5 = mCamParam.Val(param_1, 5);
    f32 val6 = mCamParam.Val(param_1, 6);
    f32 val7 = mCamParam.Val(param_1, 7);
    f32 val8 = mCamParam.Val(param_1, 8);
    f32 val24 = mCamParam.Val(param_1, 24);
    f32 val23 = mCamParam.Val(param_1, 23);
    f32 val17 = mCamParam.Val(param_1, 17);

    f32 angle180 = 180.0f;

    OneSideData* oneSide = (OneSideData*)mWork;

    cXyz arrowPos;
    if (mCurCamStyleTimer == 0) {
        oneSide->field_0x00 = 'ONES';
        oneSide->field_0x1c = cXyz::Zero;
        oneSide->field_0x2c = 0xff;
        oneSide->field_0x34 = 0xfe;
    }

    if (mRoomMapTool.mArrowIndex != oneSide->field_0x2c) {
        mCurCamStyleTimer = 0;
        mStyleSettle.mFinished = false;
        oneSide->field_0x2c = mRoomMapTool.mArrowIndex;
    }

    oneSide->field_0x28 = 0;
    cSAngle unkAngle1;
    if (mCamParam.Flag(param_1, 0x200) && mRoomMapTool.mArrowIndex != 0xff) {
        if (mRoomMapTool.mCameraIndex != oneSide->field_0x34) {
            mCamParam.Fovy(mRoomMapTool.mCamData.field_0x11);
            mCamParam.Arg0(mRoomMapTool.mCamData.field_0x12);
            mCamParam.Arg1(mRoomMapTool.mCamData.field_0x13);
            mCamParam.Arg2(mRoomMapTool.mCamData.field_0x14);
            if (param_1 == mCamTypeData[mCurType].field_0x18[mIsWolf][mCurMode]) {
                mCamParam.Change(param_1);
            }
            oneSide->field_0x34 = mRoomMapTool.mCameraIndex;
        }
        arrowPos.x = mRoomMapTool.mArrowData.position.x;
        arrowPos.y = mRoomMapTool.mArrowData.position.y;
        arrowPos.z = mRoomMapTool.mArrowData.position.z;
        if (oneSide->field_0x1c != arrowPos) {
            setUSOAngle();
        }
        oneSide->field_0x1c = arrowPos;

        if (mCamParam.Fovy() != 0xff) {
            f32 fovY = mCamParam.Fovy();
            mCamParam.SetVal(param_1, 17, fovY);
        }

        if (mCamParam.Arg0() != 0xff) {
            unkAngle1.Val((f32)mCamParam.Arg0());
        } else {
            unkAngle1.Val(90.0f);
        }

        if (mCamParam.Arg1() != 0xff) {
            val24 = (f32)mCamParam.Arg1() / 100.0f;
        }
    } else {
        arrowPos = mEye;
        unkAngle1.Val(90.0f);
    }

    mStyleSettle.mFinished = true;
    cXyz focusPosOffset(focusOffsetX, focusOffsetY, focusOffsetZ);
    focusPosOffset = dCamMath::xyzRotateY(focusPosOffset, directionOf(mpPlayerActor));
    cXyz focusPosWithOffset = attentionPos(mpPlayerActor) + focusPosOffset;
    cSGlobe focusGlobe = focusPosWithOffset - arrowPos;
    if (focusGlobe.R() >= angle180) {
        unkRatio = 1.0f;
    } else {
        f32 ratio = focusGlobe.R() / angle180;
        unkRatio = ratio;
    }

    if (!mCamParam.Flag(param_1, 0x800)) {
        focusGlobe.V((s16)-mRoomMapTool.mArrowData.angle.x);
    }

    cSAngle arrowAngle = (s16)(s32)mRoomMapTool.mArrowData.angle.y;
    cSAngle unkAngle2 = focusGlobe.U() - arrowAngle;
    cSAngle unkAngle3;
    cSAngle unkAngle4 = unkAngle2 * val24;
    if ((unkAngle2 >= unkAngle1 || unkAngle2 <= -unkAngle1) && mCurCamStyleTimer != 0) {
        unkAngle3 = oneSide->field_0x32;
    } else if (unkAngle4 > unkAngle1) {
        unkAngle3 = arrowAngle + unkAngle1;
    } else if (unkAngle4 < -unkAngle1) {
        unkAngle3 = arrowAngle - unkAngle1;
    } else {
        unkAngle3 = arrowAngle + unkAngle4;
    }

    if (mCurCamStyleTimer == 0) {
        oneSide->field_0x32 = unkAngle3;
    } else {
        oneSide->field_0x32 += (unkAngle3 - oneSide->field_0x32) * val6 * unkRatio;
    }

    focusGlobe.U(oneSide->field_0x32);
    cXyz center = arrowPos + focusGlobe.Xyz();
    mViewCache.mCenter = center;
    mViewCache.mEye = arrowPos;
    mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
    f32 fovY = val17;
    mViewCache.mFovy = fovY;

    return true;
}

bool dCamera_c::eventCamera(s32 param_0) {
    char sp90[12];

    (void)param_0;
    int var_r29 = -1;

    typedef bool (dCamera_c::*func)();
    func l_func[] = {
        &dCamera_c::pauseEvCamera,
        &dCamera_c::pauseEvCamera,
        &dCamera_c::talktoEvCamera,
        &dCamera_c::fixedPositionEvCamera,
        &dCamera_c::fixedFrameEvCamera,
        &dCamera_c::uniformTransEvCamera,
        &dCamera_c::watchActorEvCamera,
        &dCamera_c::restorePosEvCamera,
        &dCamera_c::getItemEvCamera,
        &dCamera_c::gameOverEvCamera,
        &dCamera_c::turnToActorEvCamera,
        &dCamera_c::rollingEvCamera,
        &dCamera_c::tactEvCamera,
        &dCamera_c::pauseEvCamera,
        &dCamera_c::portalWarpEvCamera,
        &dCamera_c::styleEvCamera,
        &dCamera_c::saveEvCamera,
        &dCamera_c::loadEvCamera,
        &dCamera_c::useItem0EvCamera,
        &dCamera_c::useItem1EvCamera,
        &dCamera_c::fixedFramesEvCamera,
        &dCamera_c::bSplineEvCamera,
        &dCamera_c::possessedEvCamera,
        &dCamera_c::twoActor0EvCamera,
        &dCamera_c::stokerEvCamera,
        &dCamera_c::uniformBrakeEvCamera,
        &dCamera_c::uniformAcceleEvCamera,
        &dCamera_c::stbWaitEvCamera,
        &dCamera_c::currentEvCamera,
        &dCamera_c::peepHoleEvCamera,
        &dCamera_c::digHoleEvCamera,
        &dCamera_c::hintTalkEvCamera,
        &dCamera_c::bspTransEvCamera,
        &dCamera_c::maptoolIdEvCamera,
    };

    static char* ActionNames[34] = {
        "PAUSE",
        "WAIT",
        "TALK",
        "FIXEDPOS",
        "FIXEDFRM",
        "UNITRANS",
        "WATCHACTOR",
        "RESTOREPOS",
        "GETITEM",
        "GAMEOVER",
        "TURNTOACTOR",
        "ROLLING",
        "TACT",
        "WINDDIR",
        "PORTALWARP",
        "STYLE",
        "SAVE",
        "LOAD",
        "USEITEM0",
        "USEITEM1",
        "FIXEDFRMS",
        "BSPLINE",
        "POSSESSED",
        "TWOACTOR0",
        "STOKER",
        "UNIBRAKE",
        "UNIACCELE",
        "STBWAIT",
        "CURRENT",
        "PEEPHOLE",
        "DIGHOLE",
        "HINTTALK",
        "BSPTRANS",
        "MAPTOOL",
    };

    int sp38 = 34;
    if (field_0x170 == 0) {
        clrFlag(0x200000);
        mEventData.field_0x1c = 2;
    }

    if (!chkFlag(0x20000000)) {
        int sp34 = dComIfGp_evmng_cameraPlay();
        if (sp34 == 0) {
            OS_REPORT("camera: event: %d: event not running\n", 0x3434);
            return false;
        }
        if (sp34 == 2) {
            SetTrimTypeForce(mEventData.field_0x1c);
            return true;
        }

        int sp30 = dComIfGp_evmng_getMyStaffId("CAMERA", 0, 0);
        if (sp30 < 0) {
            OS_REPORT("camera: event: %d: camera not found\n", 0x343f);
            return false;
        }

        if (mEventData.mStaffIdx != sp30) {
            clrFlag(0x200000);
            field_0x170 = field_0x160 = mCurCamStyleTimer = 0;
        }

        mEventData.mStaffIdx = sp30;
        var_r29 = dComIfGp_evmng_getMyActIdx(mEventData.mStaffIdx, ActionNames, sp38, 0, 0);
        if ((mEventData.field_0xc == specialType[15] || mEventData.field_0xc == specialType[16] ||
             mEventData.field_0xc == specialType[17] || mEventData.field_0xc == specialType[19] ||
             mEventData.field_0xc == specialType[20] || mEventData.field_0xc == specialType[21] ||
             mEventData.field_0xc == specialType[22] || mEventData.field_0xc == specialType[23] ||
             mEventData.field_0xc == specialType[24] || mEventData.field_0xc == specialType[18]) &&
             *(int*)((int)&mEventData + 0xc) != -1) // fakematch to force additional load
        {
            var_r29 = 28;
        } else if (mEventData.field_0xc == specialType[14] && var_r29 != 2) {
            var_r29 = 28;
        } else if (dComIfGp_evmng_getIsAddvance(mEventData.mStaffIdx)) {
            OS_REPORT("camera: event: next cut \n");
            mStyleSettle.mFinished = false;
            mCurCamStyleTimer = 0;
        }
    } else {
        mEventData.mStaffIdx = -1;
        if (field_0x170 == 0) {
            mStyleSettle.mFinished = false;
            field_0x170 = field_0x160 = mCurCamStyleTimer = 0;
        }

        var_r29 = mEventData.field_0x18;
        if (mEventData.field_0xc == specialType[15] || mEventData.field_0xc == specialType[16] ||
            mEventData.field_0xc == specialType[17] || mEventData.field_0xc == specialType[19] ||
            mEventData.field_0xc == specialType[20] || mEventData.field_0xc == specialType[21] ||
            mEventData.field_0xc == specialType[22] || mEventData.field_0xc == specialType[23] ||
            mEventData.field_0xc == specialType[24] || mEventData.field_0xc == specialType[18])
        {
            var_r29 = 28;
        } else if (mEventData.field_0xc == specialType[14] && var_r29 != 2) {
            var_r29 = 28;
        }
    }

    if (field_0x170 == 0) {
        pushInfo(mSavedViewStack, 1);
        pushInfo(mSavedViewStack + 1, 0);
        mEventData.field_0x0 = 0;
        mEventData.field_0xec = NULL;
        int sp2C = dComIfGp_getEvent().getMapToolId();
        if (sp2C != -1) {
            mEventData.field_0xec = dEvt_control_c::searchMapEventData(sp2C);
        }

        if (mEventData.field_0xec != NULL) {
            switch (mEventData.field_0xec->field_0x2) {
            case 2:
                setEventRecoveryTrans(mCamSetup.MapToolCameraLongTimer());
                break;
            case 1:
                setEventRecoveryTrans(mCamSetup.MapToolCameraShortTimer());
                break;
            case 3:
                setEventRecoveryTrans(1);
                break;
            default:
                setEventRecoveryTrans(0);
                break;
            }

            if (mEventData.field_0xec->type == dStage_MapEvent_dt_TYPE_MAPTOOLCAMERA && (mEventData.field_0xec->field_0xC & 0x80) != 0)
            {
                field_0x8d8.mCameraIndex = mEventData.field_0xec->data.maptool.field_0x16;
                OS_REPORT("camera: event: change default type %d \n", field_0x8d8.mCameraIndex);
            }
        } else {
            setEventRecoveryTrans(0);
        }
        OS_REPORT("%06d: camera: event: start (%d)\n", g_Counter.mCounter0, mEventData.mStaffIdx);
    }

    if (var_r29 < 0 || var_r29 >= sp38) {
        OS_REPORT("camera: event: %d: engine not found\n", 0x34ad);
        dComIfGp_evmng_cutEnd((this->mEventData).mStaffIdx);
        return false;
    }

    f32 sp28;
    int sp24;
    int sp20;
    int sp1C;
    int sp18;
    int sp14;
    f32 sp10;
    int sp0C;

    if (mCurCamStyleTimer == 0) {
        OS_REPORT("camera: event: cut [%s]\n",
                              ActionNames[var_r29]);

        if (getEvFloatData(&sp28, "KeepDist") != 0 && mViewCache.mDirection.R() < sp28)
        {
            mViewCache.mDirection.R(sp28);
            mViewCache.mEye = mViewCache.mCenter + mViewCache.mDirection.Xyz();
            mDirection.R(sp28);
            mEye = mCenter + mDirection.Xyz();
        }

        int* sp90_i;
        if (getEvStringData(sp90, "Trim", "DEFAULT") != NULL) {
            sp90_i = (int*)sp90;
            if (*sp90_i == 'STAN') {
                mEventData.field_0x1c = 0;
            } else if (*sp90_i == 'VIST') {
                mEventData.field_0x1c = 1;
            } else if (*sp90_i == 'CINE') {
                mEventData.field_0x1c = 2;
            } else if (*sp90_i == 'DEMO') {
                mEventData.field_0x1c = 3;
            } else if (*sp90_i == 'NONE') {
                mEventData.field_0x1c = 4;
            } else if (*sp90_i == 'KEEP') {
                mEventData.field_0x1c = 999;
            }
        } else if (mEventData.field_0xec != NULL) {
            switch (mEventData.field_0xec->field_0x1) {
            case 0:
                mEventData.field_0x1c = 0;
                break;
            case 1:
                mEventData.field_0x1c = 1;
                break;
            case 2:
                mEventData.field_0x1c = 2;
                break;
            }
        } else {
            mEventData.field_0x1c = 2;
        }

        getEvIntData(&sp24, "Recover", 0);
        mRecovery.field_0x4 = (s16)sp24;

        getEvIntData(&sp24, "WaitAnyKey", 0);
        if (sp24 != 0) {
            setFlag(0x200000);
        }

        if (var_r29 == 27) {
            sp0C = 3;
        } else {
            sp0C = 1;
        }

        sp20 = sp0C;
        getEvIntData(&mEventData.field_0x20, "BGCheck", sp20);

        if (getEvIntData(&sp24, "SavePos") != 0) {
            if ((sp24 == 0) || (sp24 == 1)) {
                pushInfo(&mSavedViewStack[sp24], 1);
            }
            if (sp24 == 2) {
                pushInfo(&mSavedView, 1);
            }
        }

        if (getEvIntData(&sp24, "LoadPos") != 0) {
            if ((sp24 == 0) || (sp24 == 1)) {
                popInfo(&mSavedViewStack[sp24]);
            }
            if (sp24 == 2) {
                popInfo(&mSavedView);
            }
        }
    }

    SetTrimTypeForce(mEventData.field_0x1c);

    getEvIntData(&sp1C, "PlayerHide", 0);
    if (sp1C != 0) {
        setComStat(2);
        setComStat(0x10000);
    }

    getEvIntData(&sp1C, "WideMode", 0);
    #if WIDESCREEN_SUPPORT
    if (sp1C == 1) {
        mDoGph_gInf_c::onWideZoom();
    } else if (sp1C == 2) {
        mDoGph_gInf_c::offWideZoom();
    }
    #endif

#if DEBUG
    if (mCamSetup.CheckFlag(0x8000)) {
        sp18 = 20;
        sp14 = 190;
        dDbVw_Report(sp18, sp14 + 60, "E %s", ActionNames[var_r29]);
    }
#endif

    if (isModeOK()) {
        setComStat(4);
    } else {
        clrComStat(4);
    }

    clrFlag(0x400000);

    switch (mEventData.field_0x20) {
    case 1:
        mBumpCheckFlags = 0x4007;
        break;
    case 2:
        mBumpCheckFlags = 0x4017;
        break;
    case 3:
        mBumpCheckFlags = 0x0;
        break;
    case 4:
        mBumpCheckFlags = 0x4;
        break;
    case 5:
        mBumpCheckFlags = 0xc007;
        break;
    default:
        mBumpCheckFlags = 0x4001;
        break;
    }
    mBumpCheckFlags &= 0x80b7;

    if ((this->*l_func[var_r29])() != 0) {
        mStyleSettle.mFinished = true;
        dComIfGp_evmng_cutEnd(mEventData.mStaffIdx);
        if (mEventData.field_0x0 == 0) {
            OS_REPORT("camera: event: cut end (%d)\n", mEventData.mStaffIdx);
        }
        if (mEventData.field_0xec != NULL) {
            switch (mEventData.field_0xec->field_0x3) {
            case 2:
                mRecovery.field_0x4 = mCamSetup.MapToolCameraLongTimer();
                break;
            case 1:
                mRecovery.field_0x4 = mCamSetup.MapToolCameraShortTimer();
                break;
            case 3:
                mRecovery.field_0x4 = 1;
                break;
            default:
                mRecovery.field_0x4 = 0;
                break;
            }
        }
        mEventData.field_0x0 = 1;
    }

    if (chkFlag(0x40000000)) {
        cM3dGLin sp9C(mViewCache.mCenter, mViewCache.mEye);
        cXyz sp84 = attentionPos(mpPlayerActor);
        cXyz sp78;
        if (cM3d_Len3dSqPntAndSegLine(&sp9C, &sp84, &sp78, &sp10) != 0) {
            mViewCache.mCenter = sp78;
            mViewCache.mDirection.Val(mViewCache.mEye - mViewCache.mCenter);
        }
        clrFlag(0x40000000);
    }

    return true;
}

bool dCamera_c::currentEvCamera() {
    int style = mCamTypeData[mEventData.field_0xc].field_0x18[mIsWolf][0];
    if (style < 0) {
        style = mCamParam.SearchStyle('CN01');
    }
    int i;
    if (getEvIntData(&i, "Continue", 0)) {
        setFlag(0x400000);
    }
    (this->*engine_tbl[mCamParam.Algorythmn(style)])(style);
    setFlag(0x8);
    mBumpCheckFlags = 0x4001;
    clrFlag(0x80080);
    if (mCamParam.Flag(style, 0x1)) {
        if (mCurMode == 1 && mCamParam.Flag(style, 0x2) != 0) {
            mBumpCheckFlags = 0x4007;
        } else if (chkFlag(0x20000)) {
            mBumpCheckFlags = 0x4037;
        } else {
            mBumpCheckFlags = 0x4017;
        }
    } else if (mCamParam.Flag(style, 0x2)) {
        mBumpCheckFlags = 0x4007;
    }

    if (mCamParam.Flag(style, 0x8)) {
        mBumpCheckFlags |= 0x80;
    }

    if (mCamParam.Flag(style, 0x10)) {
        mBumpCheckFlags &= ~0x4000;
    }

    mBumpCheckFlags &= ~0x8;
    if (mCamParam.Flag(style, 0x4)) {
        mBumpCheckFlags = 0;
    }

    return true;
}

bool dCamera_c::letCamera(s32) {
    return true;
}

void dCamera_c::setEventRecoveryTrans(s16 param_0) {
    pushInfo(&mRecovery.field_0x8, param_0);
    mRecovery.field_0x28 = positionOf(mpPlayerActor);
}

s16 dCamera_c::runEventRecoveryTrans() {
    if (mRecovery.field_0x8.field_0x1e > 0) {
        field_0x668++;
        if (field_0x668 >= mRecovery.field_0x8.field_0x1e) {
            mRecovery.field_0x8.field_0x1e = 0;
        } else {
            f32 ratio = dCamMath::rationalBezierRatio(1.0f - (f32)field_0x668 / (f32)mRecovery.field_0x8.field_0x1e, 1.0f);
            mCenter = mViewCache.mCenter + (mRecovery.field_0x8.mCenter - mViewCache.mCenter) * ratio;
            cXyz attPos = attentionPos(mpPlayerActor);
            dBgS_CamLinChk lin_chk;
            cM3dGPla plane;
            if ((mBumpCheckFlags & 0xb7) && lineBGCheck(&attPos, &mCenter, &lin_chk, 0x40b7)) {
                dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                mCenter = lin_chk.GetCross();
                mCenter += *plane.GetNP() * 5.0f;
            }
            mEye = mViewCache.mEye + (mRecovery.field_0x8.mEye - mViewCache.mEye) * ratio;
            if ((mBumpCheckFlags & 0xb7) && lineBGCheck(&mCenter, &mEye, &lin_chk, 0x40b7)) {
                dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                mEye = lin_chk.GetCross();
                mEye += *plane.GetNP() * mCamSetup.mBGChk.GazeBackMargin();
            }
            mDirection.Val(mEye - mCenter);
            mFovy = mViewCache.mFovy + ratio * (mRecovery.field_0x8.mFovy - mViewCache.mFovy);
            mBank = mViewCache.mBank + (mRecovery.field_0x8.mBank - mViewCache.mBank) * ratio;
        }
    }  else {
        clearInfo(&mRecovery.field_0x8, 0);
    }
    return mRecovery.field_0x8.field_0x1e;
}

void dCamera_c::EventRecoverNotime() {
    mRecovery.field_0x4 = 1;
    Reset(mRecovery.field_0x8.mCenter, mRecovery.field_0x8.mEye, mRecovery.field_0x8.mFovy, mRecovery.field_0x8.mBank);
}

int dCamera_c::Set(cXyz i_center, cXyz i_eye) {
    mCenter = i_center;
    mEye = i_eye;
    return 1;
}

int dCamera_c::Set(cXyz i_center, cXyz i_eye, f32 i_fovy, s16 i_bank) {
    mCenter = i_center;
    mEye = i_eye;
    mFovy = i_fovy;
    mBank.Val(i_bank);
    return 1;
}

int dCamera_c::Set(cXyz i_center, cXyz i_eye, s16 i_bank, f32 i_fovy) {
    mCenter = i_center;
    mEye = i_eye;
    mFovy = i_fovy;
    mBank.Val(i_bank);
    return 1;
}

void dCamera_c::Reset(cXyz i_center, cXyz i_eye, f32 i_fovy, s16 i_bank) {
    mCenter = i_center;
    mEye = i_eye;
    mFovy = i_fovy;
    mBank = cSAngle(i_bank);
    Reset();
}

void dCamera_c::Reset(cXyz i_center, cXyz i_eye) {
    mCenter = i_center;
    mEye = i_eye;
    Reset();
}

int dCamera_c::Reset() {
    mViewCache.mCenter = mCenter;
    mViewCache.mEye = mEye;
    mViewCache.mFovy = mFovy;
    mViewCache.mBank = mBank;
    mDirection.Val(mEye - mCenter);
    mViewCache.mDirection = mDirection;
    mControlledYaw = cSAngle(mDirection.U().Inv());
    return 1;
}

// NONMATCHING - minor regalloc
f32 dCamera_c::shakeCamera() {
    static f32 const wave[] = {0.4f, 0.9f, 2.1f, 3.2f};

    f32 var_f31 = 0.0f;
    if (mShake.field_0x10 < mShake.m_length) {
        int var_r29 = mShake.field_0x10 >> 3;
        int var_r27 = mShake.field_0x10 & 7;
        int var_r26 = (mShake.field_0x4[var_r29] << 8) | mShake.field_0x4[var_r29 + 1];
        int var_r28 = 1 << (15 - var_r27);
        f32 var_f30 = 1.0f;

        for (var_r29 = 0; var_r29 < 4; var_r29++) {
            if (var_r28 & var_r26) {
                var_f31 += var_f30 * wave[var_r29];
            } else {
                var_f30 *= 0.43f;
            }

            var_r28 >>= 1;
        }

        mShake.field_0x10++;

        var_f31 *= cM_rndFX(0.05f) + 0.95f;
        f32 var_f29 = var_f31;

        if (mShake.field_0x10 & 1) {
            var_f31 = -var_f31;
        }

        cXyz sp64(mShake.m_pos);
        sp64.x += cM_rndFX(0.045f);
        sp64.z += cM_rndFX(0.045f);
        sp64 = sp64 * var_f31;

        if (field_0x6fc & 2) {
            mShake.field_0x24 = mShake.mEyeShake = sp64 * 1.0f;
        }

        if (field_0x6fc & 0x40) {
            mShake.field_0x24 = mShake.mEyeShake = sp64 * 10.0f;
        }

        if (field_0x6fc & 4) {
            mShake.field_0x3c = var_f31 * cM_rndFX(0.12f);
        }

        if (field_0x6fc & 8) {
            mShake.field_0x40 = cSAngle(var_f31 * cM_rndFX(0.15f));
        }

        if (field_0x6fc & 0x10) {
            mDoGph_gInf_c::setBlureRate((int)(var_f29 * 30.0f));
            mDoGph_gInf_c::onBlure();
        }
    } else {
        mShake.field_0x24 -= mShake.field_0x24 * 0.1f;
        mShake.mEyeShake -= mShake.mEyeShake * 0.1f;
        mShake.field_0x3c -= mShake.field_0x3c * 0.1f;
        mShake.field_0x40 -= mShake.field_0x40 * 0.1f;
    }

    return var_f31;
}

// supposed to be some anonymous static class member or something?
static const int PatternLengthMax[] = {0x00000004};  // array to make it go in correct section

int dCamera_c::StartShake(s32 i_length, u8* i_pattern, s32 i_flags, cXyz i_pos) {
    if (i_length < 0 || i_length > (PatternLengthMax[0] << 3)) {
        i_length = (PatternLengthMax[0] << 3);
    }

    mShake.m_length = i_length;

    if (i_flags & 0x20) {
        field_0x6fc &= ~0x20;
        mBlure.field_0x4 = mBlure.field_0x14;
    }

    int var_r31;
    int var_r28 = i_length >> 3;
    for (var_r31 = 0; var_r31 < PatternLengthMax[0]; var_r31++) {
        mShake.field_0x0[var_r31] = mShake.field_0x4[var_r31] = 0;
    }

    for (var_r31 = 0; var_r31 < var_r28; var_r31++) {
        mShake.field_0x0[var_r31] = mShake.field_0x4[var_r31] = i_pattern[var_r31];
    }

    var_r28 = i_length & 7;
    mShake.field_0x0[var_r31] = (0xFF << (8 - var_r28)) & i_pattern[var_r31];

    if (i_length == (PatternLengthMax[0] << 3)) {
        mShake.field_0x4[var_r31] = mShake.field_0x0[var_r31] | (i_pattern[0] >> var_r28);
    } else {
        mShake.field_0x4[var_r31] = mShake.field_0x0[var_r31];
    }

    mShake.m_pos = i_pos.norm();
    mShake.field_0x10 = 0;
    field_0x6fc = i_flags;
    return 1;
}

int dCamera_c::StopShake() {
    mShake.m_length = 0;
    mShake.field_0x10 = 0;
    field_0x6fc = 0;
    return 1;
}

void dCamera_c::StartBlure(int param_0, fopAc_ac_c* i_actor, f32 i_alpha, f32 i_scale) {
    mBlure.field_0x14 = param_0;
    mBlure.mpActor = NULL;
    mBlure.mAlpha = i_alpha;

    mBlure.mScale.x = i_scale;
    mBlure.mScale.y = i_scale;
    mBlure.mScale.z = 0.0f;

    mBlure.mpActor = i_actor;

    mBlure.mPosition.x = 0.5f;
    mBlure.mPosition.y = 0.5f;
    mBlure.mPosition.z = 0.0f;

    mBlure.field_0x8.x = 0;
    mBlure.field_0x8.y = 0;
    mBlure.field_0x8.z = 0;
    mBlure.field_0x4 = param_0;
}

void dCamera_c::ResetBlure() {
    mBlure.mpActor = NULL;
    mBlure.mAlpha = 0.75f;

    mBlure.mPosition.x = 0.5f;
    mBlure.mPosition.y = 0.5f;
    mBlure.mPosition.z = 0.0f;

    mBlure.mScale.x = 1.0f;
    mBlure.mScale.y = 1.0f;
    mBlure.mScale.z = 0.0f;

    mBlure.field_0x8.x = 0;
    mBlure.field_0x8.y = 0;
    mBlure.field_0x8.z = 0;
    mBlure.field_0x4 = 0;
    mBlure.field_0x14 = 1;
}

void dCamera_c::SetBlureAlpha(f32 i_alpha) {
    mBlure.mAlpha = i_alpha;
}

void dCamera_c::SetBlureScale(f32 i_scalex, f32 i_scaley, f32 i_scalez) {
    mBlure.mScale.x = i_scalex;
    mBlure.mScale.y = i_scaley;
    mBlure.mScale.z = i_scalez;
}

void dCamera_c::SetBlurePosition(f32 i_posx, f32 i_posy, f32 i_posz) {
    SetBlureActor(NULL);
    mBlure.mPosition.x = i_posx;
    mBlure.mPosition.y = i_posy;
    mBlure.mPosition.z = i_posz;
}

void dCamera_c::SetBlureActor(fopAc_ac_c* i_actor) {
    mBlure.mpActor = i_actor;
}

int dCamera_c::blureCamera() {
    if (mBlure.field_0x4 > 0) {
        if (mBlure.mpActor != NULL) {
            dDlst_window_c* window = get_window(field_0x0);
            scissor_class* scissor = window->getScissor();
            cXyz eyePosition = eyePos(mBlure.mpActor);
            cXyz res;

            mDoLib_project(&eyePosition, &res);
            mBlure.mPosition.x = res.x / scissor->width;
            mBlure.mPosition.y = res.y / scissor->height;
            mBlure.mPosition.z = 0.0f;
        }
        mBlure.field_0x4--;

        cXyz xyz;
        f32 mult = (f32)mBlure.field_0x4 / (f32)mBlure.field_0x14;
        xyz.x = mBlure.mScale.x + (1.0f - mBlure.mScale.x) * mult;
        xyz.y = mBlure.mScale.y + (1.0f - mBlure.mScale.y) * mult;
        xyz.z = 0.0f;

        mDoMtx_stack_c::transS(mBlure.mPosition.x, mBlure.mPosition.y, mBlure.mPosition.z);
        mDoMtx_stack_c::scaleM(xyz);
        mDoMtx_stack_c::XrotM(mBlure.field_0x8.x);
        mDoMtx_stack_c::YrotM(mBlure.field_0x8.y);
        mDoMtx_stack_c::ZrotM(mBlure.field_0x8.z);
        mDoMtx_stack_c::transM(-mBlure.mPosition.x, -mBlure.mPosition.y, -mBlure.mPosition.z);
        mDoGph_gInf_c::onBlure(mDoMtx_stack_c::get());
        u8 blureRate = mBlure.mAlpha * 230.0f * mult;
        mDoMtx_stack_c::scaleM(xyz);
        mDoGph_gInf_c::setBlureRate(blureRate);
    }
    else {
        mDoGph_gInf_c::offBlure();
        mBlure.field_0x4 = 0;
    }

    return mBlure.field_0x4;
}

void dCamera_c::onHorseDush() {
    dComIfGp_getVibration().StartShock(VIBMODE_S_POWER2, 1, cXyz(0.0f, 1.0f, 0.0f));
    StartBlure(55, mpPlayerActor, 0.75f, 1.0f);
}

fopAc_ac_c* dCamera_c::GetForceLockOnActor() {
    return fopAcM_SearchByID(mLockOnActorID);
}

int dCamera_c::ForceLockOn(fopAc_ac_c* i_actor) {
    mForceLockTimer = 0;
    mpLockOnActor = i_actor;
    mLockOnActorID = fopAcM_GetID(i_actor);
    return 1;
}

int dCamera_c::ForceLockOff(fpc_ProcID i_id) {
    if (i_id == mLockOnActorID || i_id == fpcM_ERROR_PROCESS_ID_e) {
        mLockOnActorID = fpcM_ERROR_PROCESS_ID_e;
        mpLockOnActor = NULL;
        mForceLockTimer = 0;
        return 1;
    }

    return 0;
}

int dCamera_c::ForceLockOff(fopAc_ac_c* i_actor) {
    if (i_actor == mpLockOnActor) {
        mLockOnActorID = fpcM_ERROR_PROCESS_ID_e;
        mpLockOnActor = NULL;
        mForceLockTimer = 0;
        return 1;
    }

    return 0;
}

s16 dCam_getAngleY(camera_class* i_cam) {
    return i_cam->mCamera.U();
}

s16 dCam_getAngleX(camera_class* i_cam) {
    return i_cam->mCamera.V();
}

s16 dCam_getControledAngleY(camera_class* i_cam) {
    return i_cam->mCamera.U2();
}

camera_class* dCam_getCamera() {
    return dComIfGp_getCamera(0);
}

dCamera_c* dCam_getBody() {
    return &dCam_getCamera()->mCamera;
}

static void preparation(camera_process_class* i_this) {
    camera_class* a_this = (camera_class*)i_this;
    dCamera_c* camera = &a_this->mCamera;

    int camera_id = get_camera_id(a_this);
    dDlst_window_c* window = get_window(camera_id);
    view_port_class* viewport = window->getViewPort();
    f32 aspect = mDoGph_gInf_c::getAspect();

    camera->SetWindow(viewport->width, viewport->height);
    fopCamM_SetAspect(a_this, aspect);

    if (!daAlink_c::getE3Zhint()) {
        dComIfGp_offCameraAttentionStatus(camera_id, 0x1002B);
    }
}

static void view_setup(camera_process_class* i_this) {
    camera_class* a_this = (camera_class*)i_this;
    dDlst_window_c* window = get_window(a_this);

    view_port_class* viewport = window->getViewPort();
    view_class* view = (view_class*)i_this;
    mDoMtx_lookAt(view->viewMtx, &view->lookat.eye, &view->lookat.center, &view->lookat.up, view->bank);
    MTXCopy(view->viewMtx, view->viewMtxNoTrans);

    view->viewMtxNoTrans[0][3] = 0.0f;
    view->viewMtxNoTrans[1][3] = 0.0f;
    view->viewMtxNoTrans[2][3] = 0.0f;

    dComIfGd_setWindow(window);
    dComIfGd_setViewport(viewport);
    dComIfGd_setView(view);

    f32 far;
    if (getComStat(8)) {
        far = view->far;
    } else {
        far = dStage_stagInfo_GetCullPoint(dComIfGp_getStageStagInfo());
    }

    mDoLib_clipper::setup(view->fovy, view->aspect, view->near, far);
}

static void store(camera_process_class* i_camera) {
    camera_class* a_camera = (camera_class*)i_camera;
    dCamera_c* dCamera = &a_camera->mCamera;
    int camera_id = get_camera_id(a_camera);
    dDlst_window_c* window = get_window(camera_id);
    view_port_class* viewport = window->getViewPort();
    bool error = false;
    cXyz center(*fopCamM_GetCenter_p(a_camera));
    cXyz eye(*fopCamM_GetEye_p(a_camera));
    cXyz up(*fopCamM_GetUp_p(a_camera));
    cSAngle angle(fopCamM_GetBank(a_camera));
    f32 fovy = fopCamM_GetFovy(a_camera);

    dDemo_camera_c* demoCamera = dDemo_c::getCamera();
    if (demoCamera != NULL && !dComIfGp_getPEvtManager()->cameraPlay()) {
        if (demoCamera->checkEnable(dDemo_camera_c::ENABLE_VIEW_TARG_POS_e)) {
            center = demoCamera->getTarget();
        }
        if (demoCamera->checkEnable(dDemo_camera_c::ENABLE_VIEW_POS_e)) {
            eye = demoCamera->getTrans();
        }
        if (demoCamera->checkEnable(dDemo_camera_c::ENABLE_VIEW_UP_VEC_e)) {
            up = demoCamera->getUp();
        }
        if (demoCamera->checkEnable(dDemo_camera_c::ENABLE_VIEW_ROLL_e)) {
            angle = cSAngle(cAngle::d2s(-demoCamera->getRoll()));
        }
        if (demoCamera->checkEnable(dDemo_camera_c::ENABLE_PROJ_FOVY_e)) {
            fovy = demoCamera->getFovy();
        }
    } else if (!dCamera->CheckFlag(1)) {
        center = dCamera->Center();
        eye = dCamera->Eye();
        up = dCamera->Up();
        angle = dCamera->Bank();
        fovy = dCamera->Fovy();
    }

    if (eye.x == center.x && eye.z == center.z) {
        error = true;
        OS_REPORT("camera: ERROR: bad direction !!\n");
    }
    if (fovy < 0.0f || isnan(fovy)) {
        error = true;
        OS_REPORT("camera: ERROR: bad fovy !!\n");
    }
    if (isnan(eye.x) || isnan(eye.y) || isnan(eye.z)) {
        error = true;
        OS_REPORT("camera: ERROR: bad eye !!\n");
    }
    if (isnan(center.x) || isnan(center.y) || isnan(center.z)) {
        error = true;
        OS_REPORT("camera: ERROR: bad eye !!\n");
    }

    if (!error) {
        fopCamM_SetCenter(a_camera, center.x, center.y, center.z);
        fopCamM_SetEye(a_camera, eye.x, eye.y, eye.z);
        fopCamM_SetUp(a_camera, up.x, up.y, up.z);
        fopCamM_SetBank(a_camera, angle);
        fopCamM_SetFovy(a_camera, fovy);
    }

    dStage_dt_c* stage = (dStage_dt_c*)dComIfGp_getStage();

    if (a_camera->mCamera.mCamSetup.CheckFlag(0x400)) {
        //TODO
    }

    if (dComIfGp_getCameraAttentionStatus(camera_id) & 8) {
        fopCamM_SetNear(a_camera, 30.0f);
    } else {
        if (stage != NULL) {
            fopCamM_SetNear(a_camera, stage->getStagInfo()->mNear);
        }
    }
    if (stage != NULL) {
        fopCamM_SetFar(a_camera, stage->getStagInfo()->mFar);
    }

    cSGlobe globe(eye - center);
    fopCamM_SetAngleY(a_camera, globe.U().Inv());
    fopCamM_SetAngleX(a_camera, globe.V());
}

cXyz dCamera_c::Up() {
    if (chkFlag(0x10)) {
        return mUpOverride.field_0x18;
    } else {
        return mUp;
    }
}

cXyz dCamera_c::Eye() {
    return mEye + mShake.mEyeShake;
}

cXyz dCamera_c::Center() {
    return mCenter + mShake.field_0x24;
}

static int camera_execute(camera_process_class* i_this) {
    camera_class* a_this = (camera_class*)i_this;
    preparation(i_this);

    if (dDemo_c::getCamera() != NULL) {
        a_this->mCamera.ResetView();
    }

    dComIfGp_offCameraAttentionStatus(0, 0x40);

    if (a_this->mCamera.Active()) {
        a_this->mCamera.Run();
    } else {
        a_this->mCamera.NotRun();
    }

    a_this->mCamera.CalcTrimSize();

    store(i_this);
    view_setup(i_this);
    return 1;
}

static int camera_draw(camera_process_class* i_this) {
    camera_class* a_this = (camera_class*)i_this;
    dCamera_c* body = &((camera_class*)i_this)->mCamera;

    dDlst_window_c* window = get_window(a_this);
    view_port_class* viewport = window->getViewPort();
    int camera_id = get_camera_id(a_this);

    int trim_height = body->TrimHeight();
    window->setScissor(0.0f, trim_height, FB_WIDTH, FB_HEIGHT - trim_height * 2.0f);
    C_MTXPerspective(i_this->projMtx, i_this->fovy, i_this->aspect, i_this->near, i_this->far);
    mDoMtx_lookAt(i_this->viewMtx, &i_this->lookat.eye, &i_this->lookat.center,
                  &i_this->lookat.up, i_this->bank);

    j3dSys.setViewMtx(i_this->viewMtx);
    cMtx_inverse(i_this->viewMtx, i_this->invViewMtx);

    Z2GetAudience()->setAudioCamera(i_this->viewMtx, i_this->lookat.eye, i_this->lookat.center,
                                    i_this->fovy, i_this->aspect, getComStat(0x80), camera_id,
                                    false);

    dBgS_GndChk gndchk;
    gndchk.OnWaterGrp();
    gndchk.SetPos(&i_this->lookat.eye);

    f32 cross = dComIfG_Bgsp().GroundCross(&gndchk);
    if (cross != -G_CM3D_F_INF) {
        if (dComIfG_Bgsp().ChkGrpInf(gndchk, 0x100)) {
            mDoAud_getCameraMapInfo(6);
        } else {
            mDoAud_getCameraMapInfo(dComIfG_Bgsp().GetMtrlSndId(gndchk));
        }

        mDoAud_setCameraGroupInfo(dComIfG_Bgsp().GetGrpSoundId(gndchk));
        Vec spDC;
        spDC.x = i_this->lookat.eye.x;
        spDC.y = cross;
        spDC.z = i_this->lookat.eye.z;

        Z2AudioMgr::getInterface()->setCameraPolygonPos(&spDC);
    } else {
        Z2AudioMgr::getInterface()->setCameraPolygonPos(NULL);
    }

    MTXCopy(i_this->viewMtx, i_this->viewMtxNoTrans);
    i_this->viewMtxNoTrans[0][3] = 0.0f;
    i_this->viewMtxNoTrans[1][3] = 0.0f;
    i_this->viewMtxNoTrans[2][3] = 0.0f;
    cMtx_concatProjView(i_this->projMtx, i_this->viewMtx, i_this->projViewMtx);

    body->Draw();
    return 1;
}

static int init_phase1(camera_class* i_this) {
    int camera_id = get_camera_id(i_this);

    dComIfGp_setCamera(camera_id, i_this);
    fopCamM_SetPrm1(i_this, dComIfGp_getCameraWinID(camera_id));
    fopCamM_SetPrm2(i_this, dComIfGp_getCameraPlayer1ID(camera_id));
    fopCamM_SetPrm3(i_this, dComIfGp_getCameraPlayer2ID(camera_id));
    dComIfGp_setWindowNum(0);

    i_this->field_0x238 = 0;
    i_this->field_0x22f = 71;
    i_this->mCamera.field_0xb0c = 0;
    return cPhs_NEXT_e;
}

static int init_phase2(camera_class* i_this) {
    camera_process_class* a_this = (camera_process_class*)i_this;
    dCamera_c* body = &i_this->mCamera;
    int camera_id = get_camera_id(i_this);
    i_this->field_0x238++;

    fopAc_ac_c* player = (fopAc_ac_c*)get_player_actor(i_this);
    if (player == NULL) {
        return cPhs_INIT_e;
    }

    dBgS_GndChk gndchk;
    cXyz spA4(player->current.pos);
    spA4.y += 50.0f;
    gndchk.SetPos(&spA4);

    if (dComIfG_Bgsp().GroundCross(&gndchk) == -G_CM3D_F_INF) {
#if DEBUG
        if (i_this->field_0x238 < 100) {
            if (i_this->field_0x238 % 100 == 0 && i_this->field_0x238 != 0) {
                OS_REPORT("camera: can not found floor... %d\n", i_this->field_0x238);
            }

            return cPhs_INIT_e;
        }

        OS_REPORT("\ncamera: Warning: give up to get floor info !!\n\n");
#else
        return cPhs_INIT_e;
#endif
    }

    fopAcM_setStageLayer(player);
    dComIfGp_setWindowNum(1);

    new (body) dCamera_c(i_this);

    f32 var_f30 = 160000.0f;

    if (dComIfGp_getStage()->getStagInfo() != NULL) {
        dStage_dt_c* stage_dt = dComIfGp_getStage();
        stage_dt->getStagInfo();

        var_f30 = stage_dt->getStagInfo()->mFar;
    }

    get_window(camera_id)->getViewPort();

    fopCamM_SetNear(i_this, 1.0f);
    fopCamM_SetFar(i_this, var_f30);
    fopCamM_SetFovy(i_this, 30.0f);
    fopCamM_SetAspect(i_this, mDoGph_gInf_c::getAspect());
    fopCamM_SetCenter(i_this, player->current.pos.x, player->current.pos.y, player->current.pos.z);
    fopCamM_SetBank(i_this, 0);

    store(a_this);
    view_setup(a_this);

    i_this->mCamera.field_0xb0c = 1;
    i_this->field_0x238 = 0;
    dComIfGp_getAttention()->Init(player, PAD_1);
    return cPhs_NEXT_e;
}

static int camera_create(camera_class* i_this) {
    static request_of_phase_process_fn l_method[3] = {
        (request_of_phase_process_fn)init_phase1,
        (request_of_phase_process_fn)init_phase2,
        (request_of_phase_process_fn)NULL,
    };

    return dComLbG_PhaseHandler(&i_this->phase_request, l_method, i_this);
}

static int camera_delete(camera_process_class* i_this) {
    dCamera_c* camera = &((camera_class*)i_this)->mCamera;

    if (camera->CameraID() == 0) {
        // not implemented
        //dDbgCamera.Finish();
    }

    camera->~dCamera_c();
    dComIfGp_setCamera(0, NULL);
    return 1;
}

static int is_camera_delete(void* i_this) {
    return 1;
}

void dCamForcusLine::Init() {
    field_0x49 = 0;
    field_0x48 = 1;
    field_0x38 = cXyz(320.0f, 240.0f, 0.0f);

    field_0x44.r = 0xFF;
    field_0x44.g = 0xFF;
    field_0x44.b = 0xFF;
    field_0x44.a = 0x60;

    field_0x4c = 100;
    field_0x50 = 100;
    field_0x54 = 100;
    field_0x58 = 0x50;
    field_0x5a = 0;
    field_0x5c = 4;
    field_0x5e = 4;
    field_0x68 = 180.0f;
    field_0x6c = 0.0f;
    field_0x60 = 180.0f;
    field_0x64 = 60.0f;
}

void dCamForcusLine::Draw() {
    if (field_0x49) {
        if (field_0x48 == 0) {
            mEffectLine.initRnd(field_0x4c, field_0x50, field_0x54);
        }

        mEffectLine.update(field_0x38, field_0x44, field_0x58, field_0x5a, field_0x5c, field_0x5e, field_0x60, field_0x64, field_0x68, field_0x6c);
    }
}

bool dCamForcusLine::Off() {
    field_0x49 = 0;
    return field_0x49 == 0;
}

static leafdraw_method_class method = {
    (process_method_func)camera_create,
    (process_method_func)camera_delete,
    (process_method_func)camera_execute,
    (process_method_func)is_camera_delete,
    (process_method_func)camera_draw,
};

extern camera_process_profile_definition g_profile_CAMERA = {
    fpcLy_CURRENT_e,
    11,
    fpcPi_CURRENT_e,
    PROC_CAMERA,
    &g_fpcLf_Method.base,
    sizeof(dCamera_c),
    0,
    0,
    &g_fopVw_Method,
    0,
    &g_fopCam_Method,
    0,
    0,
    0,
    0,
    0,
    &method,
    0,
};

extern camera_process_profile_definition g_profile_CAMERA2 = {
    fpcLy_CURRENT_e,
    11,
    fpcPi_CURRENT_e,
    PROC_CAMERA2,
    &g_fpcLf_Method.base,
    sizeof(dCamera_c),
    0,
    0,
    &g_fopVw_Method,
    1,
    &g_fopCam_Method,
    0,
    0,
    0,
    0,
    0,
    &method,
    0,
};
