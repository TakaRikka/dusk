#include "d/dolzel_rel.h" // IWYU pragma: keep

#include <cmath>
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_tag_camera.h"
#include "d/d_com_inf_game.h"
#include "d/d_debug_viewer.h"
#include "d/d_s_play.h"

namespace {
bool always_true() {
    return true;
}

daAlink_c* Player;

bool is_player_riding_horse() {
    return Player->checkHorseRide() || Player->checkBoarRide();
}

bool is_player_swimming() {
    return dComIfGp_checkPlayerStatus0(0, 0x100000);
}

bool is_player_riding_canoe() {
    return Player->checkCanoeRide() != 0;
}

bool is_player_jumping_by_horse() {
    daHorse_c* phorse = dComIfGp_getHorseActor();

    return Player->checkHorseRide() && phorse->checkJump();
}

bool is_player_climbing() {
    return dComIfGp_checkPlayerStatus0(0, 0x2000008) != 0;
}

bool is_player_wear_magneboots() {
    return Player->checkMagneBootsOn();
}

bool is_player_wolf() {
    return Player->checkNowWolf();
}

bool is_player_pulling_chainblock() {
    return Player->checkChainBlockPushPull();
}

bool is_player_playing_spinner() {
    return Player->checkSpinnerRide() != 0;
}

bool is_player_sliding_slope() {
    return Player->checkLv3Slide();
}

bool is_player_on_watersurface() {
    return dComIfGp_checkCameraAttentionStatus(0, 0x800) != 0;
}

bool is_player_moving_on_spinnerrail() {
    return Player->checkSpinnerPathMove();
}

bool is_player_gliding() {
    return Player->checkCokkoGlide();
}

bool is_player_hunging_by_hookshot() {
    return dComIfGp_checkPlayerStatus1(0, 0x10000) != 0;
}

bool is_player_on_rope() {
    return Player->checkWolfRope() != 0;
}

bool is_player_using_copyrod() {
    return Player->getCopyRodCameraActor() != NULL;
}

bool is_player_hunging_wall_by_hookshot() {
    return dComIfGp_checkPlayerStatus1(0, 0x2000000) != 0;
}

bool is_player_carried_by_cargo() {
    return Player->checkCargoCarry();
}

bool is_player_playing_rodeo() {
    daHorse_c* phorse = dComIfGp_getHorseActor();

    return Player->checkHorseRide() && phorse->checkRodeoMode();
}

bool is_player_in_water() {
    return dComIfGp_checkPlayerStatus0(0, 0x100000) && !is_player_on_watersurface();
}

bool is_player_hugging_eal() {
    return Player->checkOctaIealHang();
}
}  // namespace

u8 daTag_Cam_c::getSwType() {
    return fopAcM_GetParam(this) & 0xF;
}

u8 daTag_Cam_c::getPrio() {
    return (fopAcM_GetParam(this) & 0xF0) >> 4;
}

u8 daTag_Cam_c::getSwBit() {
    return (fopAcM_GetParam(this) & 0xFF00) >> 8;
}

u8 daTag_Cam_c::getCondition() {
    return (fopAcM_GetParam(this) & 0xFF0000) >> 16;
}

u8 daTag_Cam_c::getCameraId() {
    return (fopAcM_GetParam(this) & 0xFF000000) >> 24;
}

u8 daTag_Cam_c::getRailID() {
    return home.angle.z & 0xFF;
}

u16 daTag_Cam_c::getAreaType() {
    return home.angle.z & 0x100;
}

u16 daTag_Cam_c::getAreaNoChk() {
    return home.angle.z & 0x200;
}

static u8 const lit_3874[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

int daTag_Cam_c::create() {
    fopAcM_ct(this, daTag_Cam_c);

    if (getAreaType() == 0) {
        scale.x *= 500.0f;
        scale.y *= 1000.0f;
        scale.z *= 500.0f;

        mBoundsLo.set(current.pos.x - scale.x, current.pos.y, current.pos.z - scale.z);
        mBoundsHi.set(current.pos.x + scale.x, current.pos.y + scale.y, current.pos.z + scale.z);
    } else {
        scale.x *= 1000.0f;
        scale.y *= 1000.0f;
        scale.z *= 0.0f;

        mBoundsLo.set(0.0f, current.pos.y, 0.0f);
        mBoundsHi.set(0.0f, current.pos.y + scale.y, 0.0f);
    }

    Player = daAlink_getAlinkActorClass();

    bool (*check_func[])() = {
        is_player_riding_horse,
        is_player_swimming,
        is_player_riding_canoe,
        always_true,
        is_player_jumping_by_horse,
        is_player_climbing,
        is_player_wear_magneboots,
        is_player_wolf,
        is_player_pulling_chainblock,
        is_player_playing_spinner,
        is_player_sliding_slope,
        is_player_on_watersurface,
        is_player_moving_on_spinnerrail,
        is_player_gliding,
        is_player_hunging_by_hookshot,
        is_player_on_rope,
        is_player_using_copyrod,
        is_player_hunging_wall_by_hookshot,
        is_player_carried_by_cargo,
        is_player_playing_rodeo,
        is_player_in_water,
        is_player_hugging_eal,
    };

    int var_r27 = 22;

    u8 func_index = getCondition();
    if ((int)func_index > var_r27) {
        func_index = 3;
    }

    mCheckFunc = check_func[func_index];
    return cPhs_COMPLEATE_e;
}

int daTag_Cam_c::execute() {
    if (fopAcM_GetRoomNo(this) != dComIfGp_roomControl_getStayNo()) {
        return 0;
    }

    u8 sw_type = getSwType();
    u8 sw_bit = getSwBit();
    bool var_r29 = false;
    bool do_area_check = false;

    if (sw_bit != 0xFF) {
        bool is_switch_on = dComIfGs_isSwitch(sw_bit, fopAcM_GetRoomNo(this)) != 0;
        if ((sw_type != 0 && is_switch_on) || (sw_type == 0 && !is_switch_on)) {
            do_area_check = true;
        }
    } else {
        do_area_check = true;
    }

    if (do_area_check) {
        cXyz pos(dComIfGp_getLinkPlayer()->current.pos);
        if (dComIfGp_checkPlayerStatus0(0, 8)) {
            pos = dComIfGp_getLinkPlayer()->attention_info.position;
            pos.y -= 80.0f;
        }

        if (getAreaNoChk()) {
            var_r29 = true;
        } else if (getAreaType() == 0) {
            if (home.angle.y != 0) {
                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(-home.angle.y);

                cXyz sp84 = pos - current.pos;
                mDoMtx_stack_c::multVec(&sp84, &pos);
            }

            if (mBoundsLo.x <= pos.x && pos.x <= mBoundsHi.x && mBoundsLo.y <= pos.y &&
                pos.y <= mBoundsHi.y && mBoundsLo.z <= pos.z && pos.z <= mBoundsHi.z)
            {
                var_r29 = true;
            }
        } else {
            f32 temp_f31 = current.pos.x - pos.x;
            f32 temp_f30 = current.pos.z - pos.z;
            f32 sq_dist = std::sqrt(temp_f31 * temp_f31 + temp_f30 * temp_f30);
            if (sq_dist < scale.x && mBoundsLo.y <= pos.y && pos.y <= mBoundsHi.y) {
                var_r29 = true;
            }
        }
    }

    if (var_r29) {
        u16 priority = getPrio();
        u8 condition = getCondition();
        bool set_camera = mCheckFunc();

        if (condition == 0xFF) {
#if PLATFORM_SHIELD
            priority |= (u16)0x8000;
#else
            priority |= 0x8000;
#endif
        } else if (condition == 0xFA) {
            if (dCam_getBody()->CheckFlag(0x8000000)) {
                set_camera = true;
            }
        }

        if (set_camera) {
            u8 cam_id = getCameraId();
            u8 rail_id = getRailID();
            dCam_getBody()->SetTagData(this, cam_id, priority, rail_id);
        }
    }

    return 1;
}

static int daTag_Cam_Draw(daTag_Cam_c* i_this) {
#if DEBUG
    return i_this->draw();
#else
    return 1;
#endif
}

int daTag_Cam_c::draw() {
#if DEBUG
    if (g_envHIO.mOther.mDisplayTransparentCyl != 0) {
        if (getAreaType() == 0) {
            cXyz sp3C = (mBoundsHi - mBoundsLo) * 0.5f;
            cXyz sp30 = mBoundsLo + sp3C;
            csXyz sp10(0, 0, 0);
            dDbVw_drawCubeXlu(sp30, sp3C, sp10, (GXColor){0xc0, 0xff, 0x78, 0xa0});
        } else {
            cXyz cStack_3c = current.pos;
            dDbVw_drawCylinderXlu(cStack_3c, scale.x, scale.y, (GXColor){0xc0, 0xff, 0x78, 0xa0},
                                  1);
        }
    }
#endif
    return 1;
}

static int daTag_Cam_Execute(daTag_Cam_c* i_this) {
    i_this->execute();
    return 1;
}

static int daTag_Cam_IsDelete(daTag_Cam_c* i_this) {
    return 1;
}

static int daTag_Cam_Delete(daTag_Cam_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTag_Cam_c();
    return 1;
}

static int daTag_Cam_Create(fopAc_ac_c* i_this) {
    daTag_Cam_c* cam = (daTag_Cam_c*)i_this;
    int id = fopAcM_GetID(i_this);
    int result = cam->create();
    return result;
}

static actor_method_class l_daTag_Cam_Method = {
    (process_method_func)daTag_Cam_Create,  (process_method_func)daTag_Cam_Delete,
    (process_method_func)daTag_Cam_Execute, (process_method_func)daTag_Cam_IsDelete,
    (process_method_func)daTag_Cam_Draw,
};

actor_process_profile_definition g_profile_TAG_CAMERA = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_TAG_CAMERA,        // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daTag_Cam_c),    // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    279,                    // mPriority
    &l_daTag_Cam_Method,    // sub_method
    0x00044000,             // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_6_e,      // cullType
};
