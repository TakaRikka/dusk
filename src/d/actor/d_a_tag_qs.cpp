//
//  d_a_tag_qs
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_obj_carry.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_tag_qs.h"
#include "d/d_a_item_static.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_debug_viewer.h"
#include "d/d_save.h"

class daObjTagQs_HIO_c : public mDoHIO_entry_c {
public:
    daObjTagQs_HIO_c();

    void genMessage(JORMContext*);

    /* 0x08 */ f32 powerOutsideRadius;
    /* 0x0C */ f32 safeArea;
    /* 0x10 */ f32 powerInsideRadius;
    /* 0x14 */ f32 powerCenter;
    /* 0x18 */ f32 sinkholePowerOutsideRadius;
    /* 0x1C */ f32 sinkholeSafeArea;
    /* 0x20 */ f32 sinkholePowerInsideRadius;
    /* 0x24 */ f32 sinkholePowerCenter;
    /* 0x28 */ s16 startMorfFrame;
    /* 0x2A */ s16 stopMorfFrame;
    /* 0x2C */ f32 centralSinkSpeed;
    /* 0x30 */ u16 flags;
};

#if DEBUG
daObjTagQs_HIO_c l_HIO;
#endif

#define TAG_QS_DEF_POWER_OUTSIDE_RADIUS 4.0f
#define TAG_QS_DEF_SAFE_AREA 0.8f
#define TAG_QS_DEF_POWER_INSIDE_RADIUS 6.0f
#define TAG_QS_DEF_POWER_CENTER 12.0f
#define TAG_QS_DEF_SINKHOLE_POWER_OUTSIDE_RADIUS 5.0f
#define TAG_QS_DEF_SINKHOLE_SAFE_AREA 0.5f
#define TAG_QS_DEF_SINKHOLE_POWER_INSIDE_RADIUS 10.0f
#define TAG_QS_DEF_SINKHOLE_POWER_CENTER 20.0f
#define TAG_QS_DEF_START_MORF_FRAME 30
#define TAG_QS_DEF_STOP_MORF_FRAME 30
#define TAG_QS_DEF_CENTRAL_SINK_SPEED 0.5f
#define TAG_QS_DEF_FLAGS 0

#if DEBUG
#define TAG_QS_POWER_OUTSIDE_RADIUS l_HIO.powerOutsideRadius
#define TAG_QS_SAFE_AREA l_HIO.safeArea
#define TAG_QS_POWER_INSIDE_RADIUS l_HIO.powerInsideRadius
#define TAG_QS_POWER_CENTER l_HIO.powerCenter
#define TAG_QS_SINKHOLE_POWER_OUTSIDE_RADIUS l_HIO.sinkholePowerOutsideRadius
#define TAG_QS_SINKHOLE_SAFE_AREA l_HIO.sinkholeSafeArea
#define TAG_QS_SINKHOLE_POWER_INSIDE_RADIUS l_HIO.sinkholePowerInsideRadius
#define TAG_QS_SINKHOLE_POWER_CENTER l_HIO.sinkholePowerCenter
#define TAG_QS_START_MORF_FRAME l_HIO.startMorfFrame
#define TAG_QS_STOP_MORF_FRAME l_HIO.stopMorfFrame
#define TAG_QS_CENTRAL_SINK_SPEED l_HIO.centralSinkSpeed
#define TAG_QS_FLAGS l_HIO.flags
#else
#define TAG_QS_POWER_OUTSIDE_RADIUS TAG_QS_DEF_POWER_OUTSIDE_RADIUS
#define TAG_QS_SAFE_AREA TAG_QS_DEF_SAFE_AREA
#define TAG_QS_POWER_INSIDE_RADIUS TAG_QS_DEF_POWER_INSIDE_RADIUS
#define TAG_QS_POWER_CENTER TAG_QS_DEF_POWER_CENTER
#define TAG_QS_SINKHOLE_POWER_OUTSIDE_RADIUS TAG_QS_DEF_SINKHOLE_POWER_OUTSIDE_RADIUS
#define TAG_QS_SINKHOLE_SAFE_AREA TAG_QS_DEF_SINKHOLE_SAFE_AREA
#define TAG_QS_SINKHOLE_POWER_INSIDE_RADIUS TAG_QS_DEF_SINKHOLE_POWER_INSIDE_RADIUS
#define TAG_QS_SINKHOLE_POWER_CENTER TAG_QS_DEF_SINKHOLE_POWER_CENTER
#define TAG_QS_START_MORF_FRAME TAG_QS_DEF_START_MORF_FRAME
#define TAG_QS_STOP_MORF_FRAME TAG_QS_DEF_STOP_MORF_FRAME
#define TAG_QS_CENTRAL_SINK_SPEED TAG_QS_DEF_CENTRAL_SINK_SPEED
#define TAG_QS_FLAGS TAG_QS_DEF_FLAGS
#endif

#if DEBUG
daObjTagQs_HIO_c::daObjTagQs_HIO_c() {
    powerOutsideRadius = TAG_QS_DEF_POWER_OUTSIDE_RADIUS;
    safeArea = TAG_QS_DEF_SAFE_AREA;
    powerInsideRadius = TAG_QS_DEF_POWER_INSIDE_RADIUS;
    powerCenter = TAG_QS_DEF_POWER_CENTER;
    sinkholePowerOutsideRadius = TAG_QS_DEF_SINKHOLE_POWER_OUTSIDE_RADIUS;
    sinkholeSafeArea = TAG_QS_DEF_SINKHOLE_SAFE_AREA;
    sinkholePowerInsideRadius = TAG_QS_DEF_SINKHOLE_POWER_INSIDE_RADIUS;
    sinkholePowerCenter = TAG_QS_DEF_SINKHOLE_POWER_CENTER;
    startMorfFrame = TAG_QS_DEF_START_MORF_FRAME;
    stopMorfFrame = TAG_QS_DEF_STOP_MORF_FRAME;
    flags = TAG_QS_DEF_FLAGS;
    centralSinkSpeed = TAG_QS_DEF_CENTRAL_SINK_SPEED;
}

void daObjTagQs_HIO_c::genMessage(JORMContext* ctx) {
    // "Quicksand"
    ctx->genLabel("流砂", 0);
    // "Power (outside safety radius)"
    ctx->genSlider("パワー(安全半径外)", &powerOutsideRadius, 0.0f, 100.0f);
    // "Safety radius adjust"
    ctx->genSlider("安全半径変更", &safeArea, 0.0f, 1.0f);
    // "Power (inside safety radius)"
    ctx->genSlider("パワー(安全半径)", &powerInsideRadius, 0.0f, 100.0f);
    // "Power (center)"
    ctx->genSlider("パワー(中心)", &powerCenter, 0.0f, 100.0f);

    // "Quicksand (10x)" (sinkhole?)
    ctx->genLabel("流砂(10倍)", 0);
    // "Power (outside safety radius)"
    ctx->genSlider("パワー（安全半径外）", &sinkholePowerOutsideRadius, 0.0f, 100.0f);
    // "Safety radius adjust"
    ctx->genSlider("安全半径変更", &sinkholeSafeArea, 0.0f, 1.0f);
    // "Power (inside safety radius)"
    ctx->genSlider("パワー(安全半径)", &sinkholePowerInsideRadius, 0.0f, 100.0f);
    // "Power (center)"
    ctx->genSlider("パワー(中心)", &sinkholePowerCenter, 0.0f, 100.0f);

    // "Others"
    ctx->genLabel("その他", 0);
    // "Start interpolation frame"
    ctx->genSlider("スタート補間フレーム", &startMorfFrame, 0, 120);
    // "Stop interpolation frame"
    ctx->genSlider("ストップ補間フレーム", &stopMorfFrame, 0, 120);
    // "Draw"
    ctx->genCheckBox("描画", &flags, 0x1);
    // "Stop"
    ctx->genCheckBox("停止", &flags, 0x2);
    // "Central sink speed"
    ctx->genSlider("中心沈み速度", &centralSinkSpeed, 0.0f, 10.0f);
}
#endif

static dCcD_SrcCyl l_cc_cyl_src = {
    {
        {0x0, {{AT_TYPE_CSTATUE_SWING, 0x0, 0x11}, {0x0, 0x0}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x5, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            50.0f, // mRadius
            50.0f // mHeight
        } // mCyl
    }
};

daTagQs_c::~daTagQs_c() {}

int daTagQs_c::create() {
    fopAcM_ct(this, daTagQs_c);

#if DEBUG
    // "Quicksand"
    l_HIO.entryHIO("流砂");
#endif

    f32 typeScale;
    if (getType() == 0) {
        typeScale = 1.0f;
    } else {
        typeScale = 20.0f;
    }
    field_0x568 = scale.x * 50.0f * typeScale;
    field_0x56c = scale.y * 50.0f * typeScale;
    field_0x570 = 0;
    field_0x574 = 0;
    mStts.Init(0xff, 0, this);
    mCyl.Set(l_cc_cyl_src);
    mCyl.SetStts(&mStts);
    return cPhs_COMPLEATE_e;
}

f32 daTagQs_c::getPower() {
    if (getType() == 0) {
        return TAG_QS_POWER_OUTSIDE_RADIUS;
    } else {
        return TAG_QS_SINKHOLE_POWER_OUTSIDE_RADIUS;
    }
}

f32 daTagQs_c::getSafeArea() {
    if (getType() == 0) {
        return TAG_QS_SAFE_AREA;
    } else {
        return TAG_QS_SINKHOLE_SAFE_AREA;
    }
}

f32 daTagQs_c::getPower80() {
    if (getType() == 0) {
        return TAG_QS_POWER_INSIDE_RADIUS;
    } else {
        return TAG_QS_SINKHOLE_POWER_INSIDE_RADIUS;
    }
}

f32 daTagQs_c::getCenterPower() {
    if (getType() == 0) {
        return TAG_QS_POWER_CENTER;
    } else {
        return TAG_QS_SINKHOLE_POWER_CENTER;
    }
}

f32 daTagQs_c::calcPower(f32 param_1, f32 param_2) {
    f32 dVar12 = 1.0f;
    if (getSwNo() != 0xff) {
        if (fopAcM_isSwitch(this, getSwNo())) {
            dVar12 = field_0x570 / (f32)TAG_QS_START_MORF_FRAME;
        } else {
            dVar12 = (TAG_QS_STOP_MORF_FRAME - field_0x574) / (f32)TAG_QS_STOP_MORF_FRAME;
        }
    }
    if (param_2 > param_1 * TAG_QS_SAFE_AREA) {
        return dVar12 * getPower();
    }

    return dVar12 *
           ((1.0f - param_2 / (param_1 * getSafeArea())) * (getCenterPower() - getPower80()) +
            getPower80());
}

static int hikiyose(cXyz* param_1, cXyz* param_2, cXyz* param_3, f32 param_4) {
    cXyz posDiff;
    posDiff = *param_1 - *param_2;
    cXyz posDiffLat(posDiff);
    posDiffLat.y = 0.0f;
    f32 dist;
    if (fabsf(posDiffLat.getSquareMag()) < 4.0f) {
        param_2->x = param_1->x + cM_rndFX(1.0f);
        param_2->y -= TAG_QS_CENTRAL_SINK_SPEED;
        param_2->z = param_1->z + cM_rndFX(1.0f);
        param_3->y = param_2->y;
        if (param_2->y < param_1->y - 100.0f) {
            return 2;
        }
        return 1;
    } else {
        dist = posDiff.abs();
        if (dist < param_4) {
            param_2->x = param_1->x;
            param_2->y = param_1->y;
            param_2->z = param_1->z;
        } else {
            posDiff.normalize();
            posDiff *= param_4;
            *param_2 += posDiff;
        }
    }
    return 0;
}

static void* search(void* param_1, void* param_2) {
    UNUSED(param_2);

    if (param_1 == NULL || !fopAcM_IsActor(param_1)) {
        return NULL;
    }

    daTagQs_c* qs = (daTagQs_c*)param_2;
    fopAc_ac_c* target = (fopAc_ac_c*)param_1;

    f32 latDist = qs->current.pos.absXZ(target->current.pos);
    if (latDist > qs->field_0x568) {
        return NULL;
    }

    if (qs->current.pos.y - 110.0f > target->current.pos.y ||
        qs->current.pos.y + qs->field_0x56c < target->current.pos.y)
    {
        return NULL;
    }

    if (target->current.pos.y >
        qs->current.pos.y + qs->field_0x56c * latDist / (qs->field_0x568 * 0.9f) + 20.0f)
    {
        return NULL;
    }

    if (fopAcM_GetProfName(param_1) == PROC_ITEM) {
        daItem_c* item = (daItem_c*)target;
        // somehow this condition is supposed to produce a double `li r0, 0x1` instruction,
        // this only produces one and is likely a fakematch anyway
        if (((u8)item->getItemNo(), true) && item->getItemNo() <= 3) {
            int sp14 = hikiyose(&qs->current.pos, &item->current.pos, &item->old.pos,
                                qs->calcPower(qs->field_0x568, latDist));
            if (sp14 == 1) {
                item->startCtrl();
            } else if (sp14 == 2) {
                fopAcM_delete(item);
            }
        }
        return NULL;
    }

    if (fopAcM_GetProfName(param_1) == PROC_Obj_Carry) {
        daObjCarry_c* carry = (daObjCarry_c*)target;
        if (carry->getType() == 7 || carry->getType() == 1 || carry->getType() == 5) {
            int sp10 = hikiyose(&qs->current.pos, &carry->current.pos, &carry->old.pos,
                                qs->calcPower(qs->field_0x568, latDist));
            if (sp10 == 1) {
                carry->startCtrl();
            } else if (sp10 == 2) {
                fopAcM_delete(carry);
            }
        }
        return NULL;
    }

    if (param_1 == daPy_getPlayerActorClass()) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        cXyz posDiff = qs->current.pos - player->current.pos;
        if (posDiff.getSquareMag() >= qs->getPower()) {
            s16 yaw = cM_atan2s(posDiff.x, posDiff.z);
            if (!player->checkPlayerFly()) {
                player->setOutPower(qs->calcPower(qs->field_0x568, latDist), yaw, 0);
            }
        }
        return NULL;
    }

    return NULL;
}

int daTagQs_c::execute() {
    if ((TAG_QS_FLAGS & 0x2) == 0) {
        fopAcM_Search(search, this);
    }
    if (getSwNo() != 0xff) {
        int bossLife = 0;
        if (fopAcM_isSwitch(this, getSwNo())) {
            field_0x570++;
            if (field_0x570 > TAG_QS_START_MORF_FRAME) {
                field_0x570 = TAG_QS_START_MORF_FRAME;
            }
            field_0x574 = 0;
            bossLife = (field_0x570 * 100.0f) / (f32)TAG_QS_START_MORF_FRAME;
            if (getType() == 0) {
                Z2GetAudioMgr()->seStartLevel(Z2SE_ENV_QUICKSAND_LOOP, &current.pos, 0,
                                              dComIfGp_getReverb(fopAcM_GetRoomNo(this)), 1.0f,
                                              1.0f, -1.0f, -1.0f, 0);
            }
        } else {
            field_0x574++;
            if (field_0x574 > TAG_QS_STOP_MORF_FRAME) {
                field_0x574 = TAG_QS_STOP_MORF_FRAME;
            }
            field_0x570 = 0;
            bossLife = 100.0f - field_0x574 * 100.0f / (f32)TAG_QS_STOP_MORF_FRAME;
        }
        dComIfGs_BossLife_public_Set(bossLife);
    } else {
        if (getType() == 0) {
            Z2GetAudioMgr()->seStartLevel(Z2SE_ENV_QUICKSAND_LOOP, &current.pos, 0,
                                          dComIfGp_getReverb(fopAcM_GetRoomNo(this)), 1.0f, 1.0f,
                                          -1.0f, -1.0f, 0);
        }
    }
    return 1;
}

int daTagQs_c::draw() {
    if (TAG_QS_FLAGS & 0x1) {
        GXColor color;
        color.r = 0xff;
        color.g = 0;
        color.b = 0xff;
        color.a = 0x80;
        dDbVw_drawCylinderXlu(current.pos, field_0x568, field_0x56c, color, 1);
    }

    return 1;
}

static int daTagQs_Draw(daTagQs_c* i_this) {
    return i_this->draw();
}

static int daTagQs_Execute(daTagQs_c* i_this) {
    return i_this->execute();
}

static int daTagQs_IsDelete(daTagQs_c* i_this) {
    UNUSED(i_this);
#if DEBUG
    l_HIO.removeHIO();
#endif
    return 1;
}

static int daTagQs_Delete(daTagQs_c* i_this) {
    i_this->~daTagQs_c();
    return 1;
}

static int daTagQs_Create(fopAc_ac_c* i_this) {
    daTagQs_c* qs = static_cast<daTagQs_c*>(i_this);
    return qs->create();
}

static actor_method_class l_daTagQs_Method = {
    (process_method_func)daTagQs_Create,     (process_method_func)daTagQs_Delete,
    (process_method_func)daTagQs_Execute, (process_method_func)daTagQs_IsDelete,
    (process_method_func)daTagQs_Draw,
};

actor_process_profile_definition g_profile_TAG_QS = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_TAG_QS,
    &g_fpcLf_Method.base,
    sizeof(daTagQs_c),
    0,
    0,
    &g_fopAc_Method.base,
    274,
    &l_daTagQs_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
