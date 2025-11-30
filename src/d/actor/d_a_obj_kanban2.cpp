/**
 * @file d_a_obj_kanban2.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kanban2.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_npc_tk.h"
#include "d/d_s_play.h"
#include "Z2AudioLib/Z2Instances.h"
#include <cmath.h>

#define PARTS_ALL 0x3FFFF
#define PART_TOP_LEFT_UNDER     (1 << 0)
#define PART_TOP_LEFT_UPPER     (1 << 1)
#define PART_TOP_RIGHT_UPPER    (1 << 2)
#define PART_TOP_RIGHT_UNDER    (1 << 3)
#define PART_BOTTOM_RIGHT_UPPER (1 << 4)
#define PART_BOTTOM_RIGHT_UNDER (1 << 5)
#define PART_BOTTOM_LEFT_UNDER  (1 << 6)
#define PART_BOTTOM_LEFT_UPPER  (1 << 7)
#define PART_STUB_LEFT          (1 << 8)
#define PART_STUB_RIGHT         (1 << 9)
#define PART_SPINE_LEFT_UPPER   (1 << 10)
#define PART_SPINE_RIGHT_UPPER  (1 << 11)
#define PART_SPINE_LEFT_UNDER   (1 << 12)
#define PART_SPINE_RIGHT_UNDER  (1 << 13)
#define PART_BASE_LEFT_UPPER    (1 << 14)
#define PART_BASE_RIGHT_UPPER   (1 << 15)
#define PART_BASE_LEFT_UNDER    (1 << 16)
#define PART_BASE_RIGHT_UNDER   (1 << 17)

#define PARTS_LEFT_SIDE           \
    (                            \
        PART_BASE_LEFT_UNDER   | \
        PART_BASE_LEFT_UPPER   | \
        PART_SPINE_LEFT_UNDER  | \
        PART_SPINE_LEFT_UPPER  | \
        PART_STUB_LEFT         | \
        PART_BOTTOM_LEFT_UPPER | \
        PART_BOTTOM_LEFT_UNDER | \
        PART_TOP_LEFT_UPPER    | \
        PART_TOP_LEFT_UNDER      \
    )

#define PARTS_TOP_HALF \
    ( \
        PART_SPINE_RIGHT_UPPER | \
        PART_SPINE_LEFT_UPPER  | \
        PART_STUB_RIGHT        | \
        PART_STUB_LEFT         | \
        PART_TOP_RIGHT_UNDER   | \
        PART_TOP_RIGHT_UPPER   | \
        PART_TOP_LEFT_UPPER    | \
        PART_TOP_LEFT_UNDER      \
    )

#define PARTS_BOTTOM_HALF \
    ( \
        PART_SPINE_RIGHT_UNDER  | \
        PART_SPINE_LEFT_UNDER   | \
        PART_BOTTOM_LEFT_UPPER  | \
        PART_BOTTOM_LEFT_UNDER  | \
        PART_BOTTOM_RIGHT_UNDER | \
        PART_BOTTOM_RIGHT_UPPER   \
    )

enum daObj_Kanban2_Action {
    ACTION_NORMAL_e,
    ACTION_PART_e,
    ACTION_FLOAT_e,
    ACTION_CARRY_e,
};

enum daObj_Kanban2_DamageType {
    DAMAGE_VERTICAL_SPLIT,
    DAMAGE_HORIZONTAL_SPLIT,
    DAMAGE_HORIZONTAL_SPLIT2,
    DAMAGE_BOTTOM_VERTICAL_SPLIT,
    DAMAGE_TOP_VERTICAL_SPLIT,
    DAMAGE_PIECES,
    DAMAGE_TLBR_DIAGONAL,
    DAMAGE_TRBL_DIAGONAL,
    DAMAGE_SHAKE,
    DAMAGE_NONE,
};

namespace {
static char* l_kn2_bmdidx[] = {
    "J_kanban00.bmd",
    "J_kanban00_01.bmd",
    "J_kanban00_02.bmd",
    "J_kanban00_03.bmd",
    "J_kanban00_04.bmd",
    "J_kanban00_05.bmd",
    "J_kanban00_06.bmd",
    "J_kanban00_07.bmd",
    "J_kanban00_08.bmd",
    "J_kanban00_09.bmd",
    "J_kanban00_10.bmd",
    "J_kanban00_11.bmd",
    "J_kanban00_12.bmd",
    "J_kanban00_13.bmd",
    "J_kanban00_14.bmd",
    "J_kanban00_15.bmd",
    "J_kanban00_16.bmd",
    "J_kanban00_17.bmd",
    "J_kanban00_18.bmd",
};

static s16 dKn2_CarryOffset[] = {
    0xD07D,
    0xBA21,
    0xBA21,
    0xE37E,
    0x1D9F,
    0xB36C,
    0xC9C8,
    0xC66D,
};

static struct {
    f32 speed_y;
    f32 speed_f;
} dKb2_BAN_SPEED[] = {
    { 35.0f, 20.0f },
    { 35.0f, 20.0f },
    { 35.0f, 20.0f },
    { 35.0f, 20.0f },
    { 25.0f, 25.0f },
    { 25.0f, 25.0f },
    { 25.0f, 25.0f },
    { 25.0f, 25.0f },
    { 40.0f, 15.0f },
    { 40.0f, 15.0f },
    { 38.0f, 18.0f },
    { 38.0f, 18.0f },
    { 35.0f, 20.0f },
    { 35.0f, 20.0f },
    { 10.0f, 25.0f },
    { 10.0f, 25.0f },
    { 0.0f, 30.0f },
    { 0.0f, 30.0f },
};

static u16 dKb2_BAN_ANGLE[] = {
    0x6800,
    0x7800,
    0x8800,
    0x9800,
    0x6800,
    0x7800,
    0x8800,
    0x9800,
    0x7000,
    0x9000,
    0x7000,
    0x9000,
    0x7000,
    0x9000,
    0x7000,
    0x9000,
    0x7000,
    0x9000,
};

static u32 dKb2_BAN_PARTS[] = {
    PART_TOP_LEFT_UNDER,
    PART_SPINE_LEFT_UPPER | PART_TOP_LEFT_UPPER,
    PART_SPINE_RIGHT_UPPER | PART_TOP_RIGHT_UPPER,
    PART_TOP_RIGHT_UNDER,
    PART_BOTTOM_RIGHT_UPPER,
    PART_SPINE_RIGHT_UNDER | PART_BOTTOM_RIGHT_UNDER,
    PART_SPINE_LEFT_UNDER | PART_BOTTOM_LEFT_UNDER,
    PART_BOTTOM_LEFT_UPPER,
    PART_STUB_LEFT,
    PART_STUB_RIGHT,
    0,
    0,
    0,
    0,
    PART_BASE_LEFT_UNDER | PART_BASE_LEFT_UPPER,
    PART_BASE_RIGHT_UNDER | PART_BASE_RIGHT_UPPER,
    0,
    0,
};

static dCcD_SrcSph cc_kn2_src = {
    {
        {0x0, {{AT_TYPE_1000, 0x2, 0x13}, {0xD8FBFDFF, 0x11}, 0x79}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

static struct {
    f32 x;
    f32 y;
} d_KANBAN_OFFSET[] = {
    { 25.0f, -110.0f },
    { 10.0f, -125.0f },
    { -10.0f, -125.0f },
    { -25.0f, -110.0f },
    { -25.0f, -90.0f },
    { -10.0f, -75.0f },
    { 10.0f, -75.0f },
    { 25.0f, -90.0f },
    { 3.0f, -145.0f },
    { -3.0f, -145.0f },
    { 3.0f, -115.0f },
    { -3.0f, -115.0f },
    { 3.0f, -70.0f },
    { -3.0f, -70.0f },
    { 3.0f, -35.0f },
    { -3.0f, -35.0f },
    { 3.0f, -10.0f },
    { -3.0f, -10.0f },
};
}

daObj_Kanban2_HIO_c::daObj_Kanban2_HIO_c() {
    id = -1;
    base_size = 1.0f;
}

int daObj_Kanban2_c::draw() {
    cXyz sp8;
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);

    if (mPartFlags == PARTS_ALL) {
        g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
        mDoExt_modelUpdateDL(mpModel);
        cullMtx = mpModel->getBaseTRMtx();

        sp8.set(cullMtx[0][3], cullMtx[1][3] - 100.0f, cullMtx[2][3]);
        mShadowId = dComIfGd_setShadow(mShadowId, 1, mpModel, &current.pos, 400.0f, 0.0f, current.pos.y, current.pos.y, mAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    } else {
        for (int i = 0; i < 18; i++) {
            if (mPartFlags & (1 << i)) {
                g_env_light.setLightTevColorType_MAJI(mpPartModel[i], &tevStr);
                mDoExt_modelUpdateDL(mpPartModel[i]);
            }
        }
    }

    return 1;
}

static int daObj_Kanban2_Draw(daObj_Kanban2_c* i_this) {
    return i_this->draw();
}

void daObj_Kanban2_c::setKanbanSE(int i_se) {
    u32 var_r31;
    if (field_0x620 <= 1) {
        var_r31 = 30;
    } else if (field_0x620 <= 3) {
        var_r31 = 60;
    } else {
        var_r31 = 90;
    }

    Z2GetAudioMgr()->seStart(i_se, &current.pos, var_r31, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void daObj_Kanban2_c::createBreakParts(u32 i_parameters, csXyz i_rot) {
    fopAcM_create(PROC_OBJ_KANBAN2, i_parameters, &current.pos, fopAcM_GetRoomNo(this), &i_rot, NULL, -1);
}

void daObj_Kanban2_c::createWallHitBreak() {
    BOOL play_se = FALSE;
    csXyz rotation(shape_angle);
    shape_angle.y = current.angle.y + 0x8000;

    u32 var_r28 = 0;
    for (int i = 0; i < 18; i++) {
        if (mPartFlags & (1 << i)) {
            if (var_r28 != 0) {
                play_se = TRUE;

                u32 parameters;
                if (field_0x620 >= 3) {
                    parameters = dKb2_BAN_PARTS[i];
                } else {
                    parameters = 1 << i;
                }

                if (parameters != 0) {
                    createBreakParts(parameters | 0x20000000, rotation);
                }
            } else {
                if (field_0x620 >= 3) {
                    var_r28 = dKb2_BAN_PARTS[i];
                } else {
                    var_r28 = 1 << i;
                }

                fopAcM_SetMtx(this, mpPartModel[i]->getBaseTRMtx());
            }
        }
    }

    if (play_se) {
        Z2GetAudioMgr()->seStart(Z2SE_OBJ_BOARD_BREAK, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }

    mPartFlags = var_r28;
    setCenterPos();
}

int daObj_Kanban2_c::getKanbanCutType() {
    s16 angle_to_player = cLib_distanceAngleS(shape_angle.y, fopAcM_searchPlayerAngleY(this));

    switch (daPy_getPlayerActorClass()->getCutType()) {
    case daPy_py_c::CUT_TYPE_HEAD_JUMP:
    case daPy_py_c::CUT_TYPE_JUMP:
    case daPy_py_c::CUT_TYPE_LARGE_JUMP:
        return DAMAGE_VERTICAL_SPLIT;
    case daPy_py_c::CUT_TYPE_NM_STAB:
    case daPy_py_c::CUT_TYPE_COMBO_STAB:
        return DAMAGE_TOP_VERTICAL_SPLIT;
    case daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH:
    case daPy_py_c::CUT_TYPE_FINISH_STAB:
        return DAMAGE_PIECES;
    case daPy_py_c::CUT_TYPE_NM_VERTICAL:
    case daPy_py_c::CUT_TYPE_NM_RIGHT:
    case daPy_py_c::CUT_TYPE_FINISH_LEFT:
    case daPy_py_c::CUT_TYPE_DASH_UNK_26:
        if (angle_to_player < 0x3800) {
            return DAMAGE_TLBR_DIAGONAL;
        }
        if (angle_to_player > 0x4800) {
            return DAMAGE_TRBL_DIAGONAL;
        }
        return DAMAGE_HORIZONTAL_SPLIT;
    case daPy_py_c::CUT_TYPE_FINISH_VERTICAL:
    case daPy_py_c::CUT_TYPE_MORTAL_DRAW_A:
    case daPy_py_c::CUT_TYPE_MORTAL_DRAW_B:
        if (angle_to_player < 0x3800) {
            return DAMAGE_TRBL_DIAGONAL;
        }
        if (angle_to_player > 0x4800) {
            return DAMAGE_TLBR_DIAGONAL;
        }
        return DAMAGE_HORIZONTAL_SPLIT;
    case daPy_py_c::CUT_TYPE_NM_LEFT:
    case daPy_py_c::CUT_TYPE_DASH_UNK_25:
        return DAMAGE_HORIZONTAL_SPLIT;
    case daPy_py_c::CUT_TYPE_TURN_RIGHT:
    case daPy_py_c::CUT_TYPE_LARGE_JUMP_INIT:
    case daPy_py_c::CUT_TYPE_TURN_LEFT:
    case daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT:
    case daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT:
    case daPy_py_c::CUT_TYPE_TWIRL:
        return DAMAGE_HORIZONTAL_SPLIT2;
    default:
        return DAMAGE_NONE;
    }
}

int daObj_Kanban2_c::getKanbanWolfCutType() {
    s16 angle_to_player = cLib_distanceAngleS(shape_angle.y, fopAcM_searchPlayerAngleY(this));

    switch (daPy_getPlayerActorClass()->getCutType()) {
    case daPy_py_c::CUT_TYPE_WOLF_TURN_LEFT:
    case daPy_py_c::CUT_TYPE_WOLF_TURN_RIGHT:
        return DAMAGE_HORIZONTAL_SPLIT2;
    case daPy_py_c::CUT_TYPE_WOLF_B_LEFT:
    case daPy_py_c::CUT_TYPE_WOLF_B_RIGHT:
    case daPy_py_c::CUT_TYPE_WOLF_B_FRONT:
        return DAMAGE_SHAKE;
    case daPy_py_c::CUT_TYPE_WOLF_B_BACK:
        if (angle_to_player < 0x3800) {
            return DAMAGE_TRBL_DIAGONAL;
        }

        if (angle_to_player > 0x4800) {
            return DAMAGE_TLBR_DIAGONAL;
        }
        return DAMAGE_HORIZONTAL_SPLIT;
    default:
        return DAMAGE_NONE;
    }

    return DAMAGE_HORIZONTAL_SPLIT;
}

void daObj_Kanban2_c::damage_check() {
    int sp24 = 0;
    u8 mode = 0;

    if (mInvulnerabilityTimer != 0) {
        return;
    }

    mCcStts.Move();
    int new_part_flags = 0;

    if (mCcSph.ChkTgHit()) {
        mInvulnerabilityTimer = 3;

        int damage_type = DAMAGE_NONE;
        cCcD_Obj* hit_obj = mCcSph.GetTgHitObj();
        fopAc_ac_c* hit_actor = dCc_GetAc(hit_obj->GetAc());

        if (hit_obj->ChkAtType(AT_TYPE_40)) {
            damage_type = DAMAGE_HORIZONTAL_SPLIT2;
        } else if (hit_obj->ChkAtType(AT_TYPE_ARROW) || hit_obj->ChkAtType(AT_TYPE_HOOKSHOT)) {
            if (mPartFlags & PARTS_TOP_HALF) {
                fopAc_ac_c* hit_actor = dCc_GetAc(hit_obj->GetAc());
                if (hit_actor != NULL) {
                    if (hit_actor->current.pos.y + (hit_actor->speed.y / 2) >= 40.0f + mCcSph.GetC().y) {
                        damage_type = DAMAGE_TOP_VERTICAL_SPLIT;
                    } else {
                        damage_type = DAMAGE_BOTTOM_VERTICAL_SPLIT;
                    }
                } else {
                    damage_type = DAMAGE_TOP_VERTICAL_SPLIT;
                }
            } else {
                damage_type = DAMAGE_BOTTOM_VERTICAL_SPLIT;
            }
        } else if (hit_obj->ChkAtType(AT_TYPE_BOMB) || hit_obj->ChkAtType(AT_TYPE_IRON_BALL) || hit_obj->ChkAtType(AT_TYPE_CSTATUE_BOSS_SWING) || hit_obj->ChkAtType(AT_TYPE_SPINNER) || hit_obj->ChkAtType(AT_TYPE_HORSE)) {
            damage_type = DAMAGE_PIECES;
        } else if (hit_obj->ChkAtType(AT_TYPE_SHIELD_ATTACK) || hit_obj->ChkAtType(AT_TYPE_BOOMERANG) || hit_obj->ChkAtType(AT_TYPE_COPY_ROD) || hit_obj->ChkAtType(AT_TYPE_1000000) || hit_obj->ChkAtType(AT_TYPE_20000)) {
            damage_type = DAMAGE_SHAKE;
        } else if (hit_obj->ChkAtType(AT_TYPE_THROW_OBJ)) {
            if (fopAcM_GetName(hit_actor) == PROC_NPC_TK) {
                ((daNPC_TK_c*)hit_actor)->setBump();
                damage_type = DAMAGE_PIECES;
            } else {
                damage_type = DAMAGE_SHAKE;
            }
        } else if (hit_obj->ChkAtType(AT_TYPE_NORMAL_SWORD)) {
            damage_type = getKanbanCutType();
        } else if (hit_obj->ChkAtType(AT_TYPE_WOLF_ATTACK | AT_TYPE_WOLF_CUT_TURN | AT_TYPE_10000000 | AT_TYPE_MIDNA_LOCK)) {
            damage_type = getKanbanWolfCutType();
        }

        u32 part_params;
        switch (damage_type) {
        case DAMAGE_VERTICAL_SPLIT:
            mInvulnerabilityTimer = 10;
            setActionMode(ACTION_PART_e, 4);
            mode = 4;

            part_params = mPartFlags & PARTS_LEFT_SIDE;
            mPartFlags &= ~PARTS_LEFT_SIDE;

            if (part_params != 0) {
                createBreakParts(part_params | (mode << 0x1C), shape_angle);
                setCullMtx();
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_BOARD_BREAK, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }

            mCcSph.ClrTgHit();
            return;
        case DAMAGE_HORIZONTAL_SPLIT:
            new_part_flags = PARTS_TOP_HALF;
            break;
        case DAMAGE_HORIZONTAL_SPLIT2:
            if (mPartFlags & PARTS_TOP_HALF) {
                new_part_flags = PARTS_TOP_HALF;
            } else {
                new_part_flags = PARTS_BOTTOM_HALF;
            }
            break;
        case DAMAGE_BOTTOM_VERTICAL_SPLIT:
            part_params = mPartFlags & 0x15C3;
            mPartFlags &= ~0x15C3;

            if (part_params != 0) {
                createBreakParts(part_params, shape_angle);
            }

            new_part_flags = 0x2A3C;
            break;
        case DAMAGE_TOP_VERTICAL_SPLIT:
            part_params = mPartFlags & 0x503;
            mPartFlags &= ~0x503;

            if (part_params != 0) {
                createBreakParts(part_params, shape_angle);
            }

            new_part_flags = 0xA0C;
            break;
        case DAMAGE_PIECES:
            for (int i = 0; i < 10; i++) {
                if (mPartFlags & (1 << i)) {
                    sp24 = 1;
                    createBreakParts(dKb2_BAN_PARTS[i] | 0x10000000, shape_angle);
                }
            }

            if (sp24 != 0) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_BOARD_BREAK, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }

            mPartFlags &= ~0x3FFF;
            setCullMtx();
            mCcSph.ClrTgHit();
            return;
        case DAMAGE_TLBR_DIAGONAL:
            new_part_flags = 0xF1E;
            break;
        case DAMAGE_TRBL_DIAGONAL:
            new_part_flags = 0xF87;
            break;
        case DAMAGE_SHAKE:
            field_0x5e8.y = cLib_distanceAngleS(shape_angle.y, fopAcM_searchPlayerAngleY(this));
            field_0x5e8.x = 0;
            field_0x5fc = 1000.0f + nREG_F(0);
            mCcSph.ClrTgHit();
            return;
        case DAMAGE_NONE:
            mCcSph.ClrTgHit();
            return;
        }

        part_params = mPartFlags & new_part_flags;
        mPartFlags &= ~new_part_flags;
        setCullMtx();

        if (part_params != 0) {
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_BOARD_BREAK, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            createBreakParts(part_params, shape_angle);
        }

        mCcSph.ClrTgHit();
    }
}

static f32 dummyFloat() {
    f32 rt = 15.0f;
    rt += 8000.0f;
    rt += 5.0f;
    return rt;
}

bool daObj_Kanban2_c::float_damage_check() {
    if (mInvulnerabilityTimer != 0) {
        return false;
    }

    mCcStts.Move();

    if (mCcSph.ChkTgHit()) {
        mInvulnerabilityTimer = 10;
        field_0x5e8.y = 0x400;
        speed.y = -3.0f;
        speedF = 10.0f;
        current.angle.y = (fopAcM_searchPlayerAngleY(this) + 0x10000 + (s16)cM_rndFX(4000.0f)) - 0x8000;
        setWaterEffect();
        return true;
    }

    return false;
}

bool daObj_Kanban2_c::deletePart() {
    if (field_0x620 <= 1) {
        if (fopAcM_CheckCondition(this, 4)) {
            f32 player_dist = current.pos.abs(daPy_getPlayerActorClass()->current.pos);
            if (player_dist > 2500.0f) {
                fopAcM_delete(this);
                return true;
            } else if (player_dist > 500.0f && (mPartFlags & 0x1EE55)) {
                fopAcM_delete(this);
                return true;
            }
        }
    } else if (current.pos.abs(daPy_getPlayerActorClass()->current.pos) > 2500.0f) {
        fopAcM_delete(this);
        return true;
    }

    return false;
}

s16 daObj_Kanban2_c::getWallAngle() {
    cXyz line_start;
    cXyz line_end;
    cXyz offset(0.0f, 0.0f, 200.0f);
    dBgS_LinChk line_chk;

    cLib_offsetPos(&line_end, &current.pos, current.angle.y, &offset);
    cLib_offsetPos(&line_start, &current.pos, -current.angle.y, &offset);
    line_chk.Set(&line_start, &line_end, NULL);

    s16 angle;
    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(line_chk, &plane);

        cXyz* normal = plane.GetNP();
        s16 temp_r3_2 = cM_atan2s(normal->x, normal->z);
        angle = (temp_r3_2 - (s16)(current.angle.y - temp_r3_2)) + 0x8000;
    } else {
        angle = current.angle.y + 0x8000;
    }

    return angle;
}

void daObj_Kanban2_c::setGroundAngle() {
    Vec gnd_pos;
    cXyz sp8;
    dBgS_GndChk gnd_chk;

    sp8 = current.pos;
    sp8.y = mAcch.GetGroundH();

    gnd_pos.x = sp8.x;
    gnd_pos.y = 50.0f + sp8.y;
    gnd_pos.z = 10.0f + sp8.z;
    gnd_chk.SetPos(&gnd_pos);
    gnd_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

    if (-G_CM3D_F_INF != gnd_pos.y && std::abs(gnd_pos.y - sp8.y) < 50.0f) {
        field_0x5ee.x = -cM_atan2s(gnd_pos.y - sp8.y, gnd_pos.z - sp8.z);
    }

    gnd_pos.x = 10.0f + sp8.x;
    gnd_pos.y = 50.0f + sp8.y;
    gnd_pos.z = sp8.z;
    gnd_chk.SetPos(&gnd_pos);
    gnd_pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);

    if (-G_CM3D_F_INF != gnd_pos.y && std::abs(gnd_pos.y - sp8.y) < 50.0f) {
        field_0x5ee.z = cM_atan2s(gnd_pos.y - sp8.y, gnd_pos.x - sp8.x);
    }
}

bool daObj_Kanban2_c::checkWaterSurface() {
    dBgS_ObjGndChk_Spl gnd_chk;
    Vec sp8;
    sp8 = current.pos;
    sp8.y += 500.0f;

    gnd_chk.SetPos(&sp8);
    field_0x604 = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (field_0x604 > current.pos.y) {
        return true;
    }

    return false;
}

bool daObj_Kanban2_c::checkPataGround(s16 param_0, s16 param_1) {
    cXyz sp18;
    cXyz spC;
    dBgS_GndChk sp24;
    sp18 = current.pos;

    f32 temp_f30 = 100.0f * cM_ssin(param_0);
    f32 temp_f29 = 100.0f * cM_scos(param_0);

    sp18.x += temp_f30 * cM_ssin(param_1);
    sp18.y += temp_f29;
    sp18.z += temp_f30 * cM_scos(param_1);
    spC = sp18;
    spC.y += 100.0f;
    sp24.SetPos(&spC);

    f32 ground_y = dComIfG_Bgsp().GroundCross(&sp24);
    if (ground_y != -G_CM3D_F_INF && sp18.y < ground_y) {
        return true;
    }

    return false;
}

void daObj_Kanban2_c::setCullMtx() {
    for (int i = 0; i < 18; i++) {
        if (mPartFlags & (1 << i)) {
            fopAcM_SetMtx(this, mpPartModel[i]->getBaseTRMtx());
            return;
        }
    }
}

void daObj_Kanban2_c::setSmokeEffect(cXyz i_pos) {
    fopAcM_effSmokeSet1(&field_0x9e0, &field_0x9e4, &i_pos, NULL, 0.02 * field_0x600, &tevStr, 1);
}

/* 80585CB5 0003+00 l_initHIO None */
static u8 l_initHIO;

static daObj_Kanban2_HIO_c l_HIO;

void daObj_Kanban2_c::setWaterEffect() {
    cXyz eff_pos(current.pos.x, field_0x604, current.pos.z);
    static cXyz sc(0.5f, 0.5f, 0.5f);

    for (int i = 0; i < 4; i++) {
        static u16 w_eff_id[] = {0x01B8, 0x01B9, 0x01BA, 0x01BB};
        mWaterEffID[i] = dComIfGp_particle_set(mWaterEffID[i], w_eff_id[i], &eff_pos, &tevStr, NULL, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
    }

    mSound.startSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
}

void daObj_Kanban2_c::setCenterPos() {
    cXyz sp14(0.0f, 0.0f, 0.0f);
    cXyz sp8;
    int var_r29 = 0;

    field_0x620 = 1;
    field_0x600 = 30.0f;

    if (mAction != ACTION_NORMAL_e && mMode != 4) {
        for (int i = 0; i < 8; i++) {
            if (mPartFlags & (1 << i)) {
                sp8.x = d_KANBAN_OFFSET[i].x;
                sp8.y = d_KANBAN_OFFSET[i].y;
                sp8.z = 0.0f;
                sp14 += sp8;

                var_r29++;
            }
        }

        if (var_r29 == 0) {
            for (int i = 8; i < 18; i++) {
                if (mPartFlags & (1 << i)) {
                    field_0x5dc.x = d_KANBAN_OFFSET[i].x;
                    field_0x5dc.y = d_KANBAN_OFFSET[i].y;
                    field_0x5dc.z = 10.0f;
                    break;
                }
            }
        } else {
            field_0x5dc.x = sp14.x / var_r29;
            field_0x5dc.y = sp14.y / var_r29;
            field_0x5dc.z = 0.0f;
            field_0x620 = var_r29 & 0xFF;

            for (int i = 8; i < 18; i++) {
                if (mPartFlags & (1 << i)) {
                    field_0x60a = 0x800;
                    if (mPartFlags & PARTS_LEFT_SIDE) {
                        field_0x60a = -0x800;
                    }
                    break;
                }
            }
        }

        if (var_r29 <= 1) {
            fopAcM_OnCarryType(this, fopAcM_CARRY_UNK_30);
        }

        if (var_r29 <= 1) {
            field_0x600 = 10.0f;
        } else if (var_r29 == 2) {
            field_0x600 = 20.0f;
        } else {
            field_0x600 = 30.0f;
        }
    }
}

BOOL daObj_Kanban2_c::checkCarryOn() {
    cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    if (fopAcM_checkCarryNow(this)) {
        setActionMode(ACTION_CARRY_e, 0);

        if (shape_angle.x > 0) {
            field_0x608 = 0x4000;
        } else {
            field_0x608 = -0x4000;
        }

        field_0x62e = 0;
        setGroundAngle();
        return TRUE;
    }

    return FALSE;
}

void daObj_Kanban2_c::setActionMode(int i_action, int i_mode) {
    if (mAction != i_action) {
        mPrevAction = mAction;
        mAction = i_action;
        mMode = i_mode;

        field_0x5f4.set(0, 0, 0);
        field_0x5e8.set(0, 0, 0);
        field_0x5ee.set(0, 0, 0);

        field_0x62c = 0;
        field_0x62d = 0;
        field_0x62b = 0;
    }
}

void daObj_Kanban2_c::calcNormalSwing() {
    u8 sp8 = 0;
    s16 var_r29 = 0;

    if ((mPartFlags & 0xFF) != 0 && mCcSph.ChkCoHit()) {
        fopAc_ac_c* hit_actor = dCc_GetAc(mCcSph.GetCoHitObj()->GetAc());
        if (fopAcM_GetName(hit_actor) == PROC_HORSE) {
            cXyz sp28(current.pos);
            if (mPartFlags & PARTS_TOP_HALF) {
                sp28.y += 100.0f;
            } else if (mPartFlags & PARTS_BOTTOM_HALF) {
                sp28.y += 70.0f;
            }
            sp28 += *mCcStts.GetCCMoveP();

            s16 spE = -cLib_targetAngleX(&current.pos, &sp28) + 0x4000;
            s16 spC = cLib_targetAngleY(&current.pos, &sp28);
            var_r29 = spE * cM_scos(spC);
            s16 spA = std::abs(3584.0f * cM_scos(spC));

            if (abs(var_r29) > spA && abs(spE) > 0x800) {
                if (field_0x627 >= 20) {
                    u32 temp_r26 = mPartFlags & 0x15C3;
                    mPartFlags &= ~0x15C3;

                    if (temp_r26 != 0) {
                        createBreakParts(temp_r26 | 0x80000000, shape_angle);
                    }

                    temp_r26 = mPartFlags & 0x2A3C;
                    mPartFlags &= ~0x2A3C;
                    setCullMtx();

                    if (temp_r26 != 0) {
                        Z2GetAudioMgr()->seStart(Z2SE_OBJ_BOARD_BREAK, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                        createBreakParts(temp_r26 | 0x80000000, shape_angle);
                    }
                }

                if (var_r29 < 0) {
                    var_r29 = -spA;
                } else {
                    var_r29 = spA;
                }
            }

            if (field_0x626 == 0) {
                field_0x624 = var_r29;
            } else if ((field_0x624 < 0 && var_r29 > 0) || (field_0x624 > 0 && var_r29 < 0)) {
                if (abs((s16)(field_0x624 - var_r29)) < 0x1000) {
                    field_0x624 = var_r29;
                } else {
                    var_r29 = field_0x624;
                    field_0x627 += 10;
                }
            } else {
                field_0x624 = var_r29;
            }

            sp8 = 1;
        }
    }

    if (sp8 != 0) {
        if (field_0x627 < 30) {
            int var_r25 = daPy_getPlayerActorClass()->speedF / 10.0f;
            if (var_r25 < 1) {
                var_r25 = 1;
            }

            if (var_r25 > 10) {
                var_r25 = 10;
            }

            field_0x627 += var_r25;
        }

        cLib_addCalcAngleS2(&shape_angle.x, var_r29, 8, 0x400);
    } else {
        field_0x627 = 0;

        if (field_0x626 != 0) {
            field_0x5e8.x = 0;
            if (field_0x624 < 0x800) {
                if (field_0x624 > 0) {
                    field_0x5fc = 2048.0f;
                } else {
                    field_0x5fc = -2048.0f;
                }
            } else {
                field_0x5fc = field_0x624;
            }
        }

        if (field_0x5fc) {
            cLib_chaseF(&field_0x5fc, 0.0f, 50.0f + nREG_F(2));
            field_0x5e8.x += (nREG_S(0) + 0x2000);

            f32 temp_f31 = field_0x5fc * cM_ssin(field_0x5e8.x);
            shape_angle.x = temp_f31 * cM_scos(field_0x5e8.y);
            shape_angle.z = temp_f31 * cM_ssin(field_0x5e8.y);
        }
    }

    field_0x626 = sp8;
}

// NONMATCHING - gameinfo mEvent load, equivalent
void daObj_Kanban2_c::executeNormal() {
    calcNormalSwing();
    mCcSph.OffAtSetBit();

    eyePos = current.pos;
    eyePos.y += 70.0f;
    attention_info.position = current.pos;
    attention_info.position.y += 150.0f;

    damage_check();
    field_0x600 = 40.0f;

    if (mPartFlags != PARTS_ALL) {
        field_0x600 = 30.0f;
    }

    if ((mPartFlags & 0xFF) == 0) {
        field_0x600 = 10.0f;
    }

    if (mFlowID != -1 && mPartFlags != PARTS_ALL) {
        mFlowID = -1;
    }

    switch (field_0x9fe) {
    case 0:
        if (mFlowID != -1 && cLib_distanceAngleS(shape_angle.y, fopAcM_searchPlayerAngleY(this)) < 0x3000) {
            cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_TALKREAD_e | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
            attention_info.distances[fopAc_attn_TALK_e] = 21;
            attention_info.distances[fopAc_attn_SPEAK_e] = 21;
            eventInfo.onCondition(1);
        } else {
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_TALKREAD_e | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        }

        if (dComIfGp_event_runCheck() && eventInfo.checkCommandTalk()) {
            mMsgFlow.init(this, mFlowID, 0, NULL);
            field_0x9fe = 1;
            mInvulnerabilityTimer = 3;
        }
        break;
    case 1:
        mInvulnerabilityTimer = 3;
#if VERSION != VERSION_SHIELD_DEBUG
        // TODO: gameInfo fake match to force reuse of pointer
        dComIfG_play_c* play = &g_dComIfG_gameInfo.play;
        if (play->getEvent().runCheck() && eventInfo.checkCommandTalk())
#else
        if (dComIfGp_event_runCheck() && eventInfo.checkCommandTalk())
#endif
        {
            if (mMsgFlow.doFlow(this, NULL, 0)) {
#if VERSION != VERSION_SHIELD_DEBUG
                play->getEvent().reset();
#else
                dComIfGp_event_reset();
#endif
                field_0x9fe = 0;
            }
        } else {
            field_0x9fe = 0;
        }
        break;
    }
}

void daObj_Kanban2_c::initPart() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    field_0x62b = 0;
    field_0x62c = 0;
    field_0x62d = 1;
    gravity = -4.0f;

    mCcSph.OffAtSetBit();
    mCcSph.OffTgSetBit();
    mCcSph.OffCoSetBit();
    mCcStts.ClrCcMove();

    switch (mMode) {
    case 0:
        speed.y = 35.0f + cM_rndF(5.0f);
        current.angle.y = (fopAcM_searchPlayerAngleY(this) + 0x10000 + (s16)cM_rndFX(4000.0f)) - 0x8000;
        speedF = 20.0f + cM_rndF(5.0f);
        shape_angle.z = 0;
        shape_angle.x = 0;
        break;
    case 5:
        shape_angle.z = 0;
        shape_angle.x = 0;
        shape_angle.y = current.angle.y;

        if (shape_angle.x < 0) {
            current.pos.y += 10.0f;
        } else {
            current.pos.y -= 20.0f;
        }

        if (fopAcM_GetSpeedF(this) >= 1.0f) {
            field_0x62b = 1;
            mCcSph.OnAtSetBit();

            f32 var_f31;
            if (field_0x620 <= 1) {
                var_f31 = 1.0f;
                shape_angle.y += 0x4000;
                mMode = 9;
                speed.y = 40.0f;
            } else if (field_0x620 == 2) {
                var_f31 = 0.7f;
                field_0x5e8.x = 0x2000;
                field_0x5e8.y = cM_rndFX(1024.0f);
                mMode = 9;
                speed.y = 35.0f;
            } else if (field_0x620 == 3) {
                var_f31 = 0.6f;
                mMode = 10;
                shape_angle.x = field_0x608;
                speed.y = 30.0f;
            } else {
                var_f31 = 0.5f;
                mMode = 10;
                shape_angle.x = field_0x608;
                speed.y = 25.0f;
            }

            field_0x608 -= 0x8000;
            speedF = 50.0f * var_f31;
            if (cLib_distanceAngleS(player->shape_angle.y, player->current.angle.y) < 0x2000) {
                speedF += player->speedF;
            }
        } else {
            field_0x62c = 1;
            speedF = 0.0f;
            speed.y = 0.0f;
            mMode = 11;
            shape_angle.x = field_0x608;
        }
        return;
    case 6:
        shape_angle.z = 0;
        shape_angle.x = 0;
        current.pos.y -= 80.0f;
        speed.y = 40.0f + nREG_F(3);
        current.angle.y = cM_rndFX(8000.0f);
        speedF = nREG_F(4) + (10.0f + cM_rndFX(10.0f));
        field_0x5e8.y = 2048.0f + cM_rndFX(512.0f);
        mMode = 9;
        return;
    case 7:
        speedF = 0.0f;
        speed.y = 0.0f;
        mMode = 11;
        return;
    case 1:
    case 2:
        shape_angle.z = 0;
        shape_angle.x = 0;

        for (int i = 0; i < 18; i++) {
            if (mPartFlags & (1 << i)) {
                speed.y = dKb2_BAN_SPEED[i].speed_y;
                speedF = dKb2_BAN_SPEED[i].speed_f;
                current.angle.y = dKb2_BAN_ANGLE[i] + fopAcM_searchPlayerAngleY(this);
            }
        }
        break;
    case 8:
        shape_angle.z = 0;
        shape_angle.x = 0;
        speed.y = 15.0f + cM_rndFX(5.0f);
        speedF = 10.0f + cM_rndFX(5.0f);
        current.angle.y = fopAcM_searchPlayerAngleY(this) + 4.0f * cM_rndFX(4096.0f);
        break;
    }

    field_0x5e8.x = 8192.0f + cM_rndFX(1024.0f);
    field_0x5e8.y = 2048.0f + cM_rndFX(512.0f);
    mMode = 9;
}

void daObj_Kanban2_c::executePart() {
    cXyz sp4C;
    attention_info.position = eyePos = current.pos;

    fopAcM_SetFoodStatus(this, fopAcM_FOOD_0);
    if (!mAcch.ChkGroundHit()) {
        fopAcM_SetFoodStatus(this, fopAcM_FOOD_1);
    }

    switch (mMode) {
    case 0:
    case 1:
    case 2:
    case 5:
    case 6:
    case 7:
    case 8:
        initPart();
        break;
    case 9:
        if (field_0x620 <= 1) {
            shape_angle.z += 0x1000;
        }

        if (mAcch.ChkGroundHit()) {
            if (speedF >= 25.0f) {
                cM3dGPla sp6C;
                dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &sp6C);
                if (sp6C.GetNP()->y < 0.7f) {
                    mAcch.SetWallHit();
                    mAcch.ClrGroundHit();
                }
            }
        }

        if (mAcch.ChkWallHit()) {
            field_0x62b = 0;
            if (speedF >= 25.0f) {
                createWallHitBreak();
            }

            setKanbanSE(Z2SE_OBJ_BOARD_BOUND);
            speedF *= 0.5f;

            s16 sp8 = getWallAngle();
            current.angle.y = sp8 + cM_rndFX(4096.0f);
        }

        if (mAcch.ChkGroundHit() != 0) {
            setKanbanSE(Z2SE_OBJ_BOARD_BOUND);
            setSmokeEffect(current.pos);
            speed.y = 15.0f + cM_rndF(5.0f);
            speedF *= 0.75f;
            mMode = 11;
            field_0x5e8.set(0, 0, 0);
        }
        break;
    case 10:
        cLib_chaseAngleS(&shape_angle.x, field_0x608, 0x700);

        if (mAcch.ChkGroundHit()) {
            if (speedF >= 25.0f) {
                cM3dGPla sp58;
                dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &sp58);
                if (sp58.GetNP()->y < 0.7f) {
                    mAcch.SetWallHit();
                    mAcch.ClrGroundHit();
                }
            }
        }

        if (mAcch.ChkWallHit()) {
            if (speedF >= 25.0f) {
                createWallHitBreak();
            }
            speedF *= 0.2f;
            current.angle.y = getWallAngle();
        }

        if (mAcch.ChkGroundHit()) {
            shape_angle.x = field_0x608;
            setKanbanSE(Z2SE_OBJ_BOARD_BOUND);
            setSmokeEffect(current.pos);
            speedF = 0.0f;
            speed.y = 0.0f;
            mMode = 11;
            field_0x5e8.set(0, 0, 0);
        }
        break;
    case 11:
        mCcSph.OnTgSetBit();
        field_0x5f4.z = 0;

        if (mAcch.ChkGroundHit()) {
            setGroundAngle();

            if (field_0x62d != 0) {
                if (field_0x62c == 0) {
                    setKanbanSE(Z2SE_OBJ_BOARD_BOUND);
                } else {
                    setKanbanSE(Z2SE_OBJ_BOARD_PUT);
                }

                setSmokeEffect(current.pos);
                field_0x62c = 0;
                field_0x62d = 0;
                field_0x62b = 0;

                mCcSph.OffAtSetBit();
                mCcSph.OnTgSetBit();
            }

            speedF = 0.0f;
            speed.y = 0.0f;

            if (shape_angle.x > 0) {
                shape_angle.x = 0x4000;
                mAcch.SetGroundUpY(10.0f);
            } else {
                shape_angle.x = -0x4000;
                if (field_0x60a != 0) {
                    field_0x5f4.y = field_0x60a;
                    mAcch.SetGroundUpY(10.0f);
                } else {
                    mAcch.SetGroundUpY(0.0f);
                }
            }

            if (deletePart()) {
                return;
            }
            break;
        }

        if (abs(shape_angle.x) < 0x4000) {
            cLib_chaseAngleS(&shape_angle.x, 0x4000, 0x200);
        } else {
            cLib_chaseAngleS(&shape_angle.x, -0x4000, 0x200);
        }

        if (mAcch.ChkWallHit()) {
            speedF *= 0.5f;
            current.angle.y = getWallAngle();
        }
        break;
    case 3:
        mCcSph.OffAtSetBit();
        gravity = 0.0f;
        field_0x629 = 60;
        speedF = 10.0f;
        speed.y = -10.0f;
        current.angle.y = shape_angle.y + 0x4000;
        mMode = 12;
        break;
    case 12:
        if (field_0x629 != 0) {
            speedF = 0.0f;
            speed.y = 0.0f;
        } else {
            mMode = 13;
            field_0x629 = 10;
        }
        break;
    case 13:
        if (field_0x629 != 0) {
            cLib_chaseF(&speed.y, -10.0f, 0.5f);
            cLib_chaseF(&speedF, 10.0f, 0.5f);
        } else {
            mMode = 9;
            gravity = -4.0f;
        }
        break;
    case 4:
        mCcSph.OffAtSetBit();
        mCcSph.OffCoSetBit();
        gravity = 0.0f;
        field_0x5e8.z = 0;
        mMode = 14;
        field_0x629 = 10;
        break;
    case 14:
        if (field_0x629 != 0) {
            return;
        }

        cLib_chaseAngleS(&field_0x5e8.z, 0x1000, 0x80);

        int var_r28 = 0;
        if (!(mPartFlags & PARTS_LEFT_SIDE)) {
            cLib_chaseAngleS(&field_0x5f4.y, 0x4000, 0x400);
            shape_angle.z -= field_0x5e8.z;

            if (checkPataGround(shape_angle.z, (shape_angle.y - 0x4000)) != 0) {
                field_0x5e8.z = (-field_0x5e8.z / 3);
                setKanbanSE(Z2SE_OBJ_BOARD_PUT);

                if ((mPartFlags & 0xFF) != 0) {
                    sp4C = current.pos;
                    sp4C.x -= 100.0f * cM_ssin((shape_angle.y - 0x4000));
                    sp4C.z -= 100.0f * cM_scos((shape_angle.y - 0x4000));
                    setSmokeEffect(sp4C);
                }

                if (abs(field_0x5e8.z) < 0x100) {
                    var_r28 = 1;
                }
            }
        } else {
            shape_angle.z += field_0x5e8.z;
            cLib_chaseAngleS(&field_0x5f4.y, -0x4000, 0x400);

            if (checkPataGround(-shape_angle.z, (shape_angle.y + 0x4000)) != 0) {
                field_0x5e8.z = (-field_0x5e8.z / 3);
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_BOARD_PUT, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

                if ((mPartFlags & 0xFF) != 0) {
                    sp4C = current.pos;
                    sp4C.x -= 100.0f * cM_ssin((shape_angle.y + 0x4000));
                    sp4C.z -= 100.0f * cM_scos((shape_angle.y + 0x4000));
                    setSmokeEffect(sp4C);
                }

                if (abs(field_0x5e8.z) < 0x100) {
                    var_r28 = 1;
                }
            }
        }

        if (var_r28 != 0) {
            field_0x5e8.z = 0;
            shape_angle.x = abs(shape_angle.z);
            shape_angle.y += field_0x5f4.y;
            field_0x5f4.y = 0;
            shape_angle.z = 0;
            setCenterPos();
            gravity = -4.0f;

            f32 temp_f31, temp_f30;
            temp_f30 = field_0x5dc.y * cM_scos(shape_angle.x);
            temp_f31 = field_0x5dc.y * cM_ssin(shape_angle.x);
            current.pos.x -= temp_f31 * cM_ssin(shape_angle.y);
            current.pos.y -= temp_f30 - 10.0f;
            current.pos.z -= temp_f31 * cM_scos(shape_angle.y);

            mMode = 11;
        }
        break;
    }

    if (checkWaterSurface()) {
        setActionMode(ACTION_FLOAT_e, 0);
    } else {
        checkCarryOn();
    }
}

void daObj_Kanban2_c::executeFloat() {
    checkWaterSurface();

    if (-G_CM3D_F_INF == field_0x604) {
        setActionMode(ACTION_PART_e, 7);
        return;
    }

    cXyz sp38(current.pos.x, field_0x604, current.pos.z);
    u32 var_r28 = field_0x620;
    if (var_r28 < 1) {
        var_r28 = 1;
    }

    attention_info.position = eyePos = current.pos;

    if (mMode == 0) {
        mCcSph.OnTgSetBit();
        mCcSph.OffAtSetBit();
        mMode = 1;

        if (shape_angle.x > 0) {
            shape_angle.x = 0x4000;
        } else {
            shape_angle.x = -0x4000;
        }

        if (speedF) {
            field_0x5e8.y = 0x100;
        }
        if (speedF > 40.0f) {
            speedF = 40.0f;
        }

        speedF *= 0.75f + nREG_F(8);
        gravity = 0.0f;
        speed.y = 0.0f;
        setWaterEffect();
    }

    if (!float_damage_check()) {
        field_0x5e8.x += 0x400;
        field_0x5ee.x += (s16)(70.0f * cM_ssin(field_0x5e8.x));

        if (current.pos.y >= field_0x604) {
            current.pos.y = field_0x604;
            speed.y = 0.0f;
        } else {
            cLib_chaseF(&speed.y, 2.0f, 0.1f);
        }

        if (mAcch.ChkWallHit() && field_0x629 == 0) {
            field_0x629 = 10;

            s16 temp_r3 = mAcchCir.GetWallAngleY();
            if ((s16)cLib_distanceAngleS(temp_r3, current.angle.y) > 0x6000) {
                field_0x5e8.y = -field_0x5e8.y / 2;
                current.angle.y = ((temp_r3 - (s16)(current.angle.y - temp_r3)) + 0x10000 + (s16)cM_rndFX(2000.0f)) - 0x8000;
                speedF *= 0.3f;
            }
        }

        cXyz sp2C(cXyz::Zero);
        cXyz sp20(cXyz::Zero);
        int sp10 = 0;
        f32 var_f31 = 0.0f;

        if (dComIfG_Bgsp().ChkPolySafe(mAcch.m_gnd) && dPath_GetPolyRoomPathVec(mAcch.m_gnd, &sp2C, &sp10)) {
            u32 var_r27 = var_r28;
            if (var_r27 > 3) {
                var_r27 = 3;
            }

            static f32 kn2_speed_rate[] = {1.0f, 1.5f, 2.0f};

            var_f31 = sp10 * kn2_speed_rate[var_r27 - 1];
            s16 spA = cM_atan2s(sp2C.x, sp2C.z);

            sp2C.x = 0.5f * (var_f31 * cM_ssin(spA));
            sp2C.z = 0.5f * (var_f31 * cM_scos(spA));

            sp20.x = speedF * cM_ssin(current.angle.y);
            sp20.z = speedF * cM_scos(current.angle.y);

            sp20 = sp2C + sp20;
            current.angle.y = cM_atan2s(sp20.x, sp20.z);

            s16 sp8 = (0x40 / var_r27) + 0x40;

            if (field_0x5e8.y < 0) {
                cLib_chaseAngleS(&field_0x5e8.y, -sp8, 0x10);
            } else {
                cLib_chaseAngleS(&field_0x5e8.y, sp8, 0x10);
            }
        }

        cLib_chaseF(&speedF, var_f31, 1.0f);

        f32 var_f30;
        if (speedF >= 5.0f) {
            var_f30 = 0.2f * var_r28;
        } else {
            var_f30 = 0.05f * var_r28;
        }

        fopAcM_effHamonSet(&field_0x9f8, &sp38, 1.0f, var_f30);

        if (field_0x62a == 0 && mCcSph.ChkCoHit()) {
            cCcD_Obj* hit_obj = mCcSph.GetCoHitObj();
            if (fopAcM_GetName(dCc_GetAc(hit_obj->GetAc())) == PROC_ALINK && daPy_getPlayerActorClass()->speedF > 1.0f) {
                field_0x5e8.y = 0x100;
                field_0x62a = 10;
            }
        }

        shape_angle.y += field_0x5e8.y;
        cLib_chaseAngleS(&field_0x5e8.y, 0, 10);
        checkCarryOn();

        if (!deletePart()) {
            return;
        }
    }
}

void daObj_Kanban2_c::executeCarry() {
    attention_info.position = eyePos = current.pos;
    fopAcM_SetFoodStatus(this, fopAcM_FOOD_2);

    if (daPy_getPlayerActorClass()->getGrabUpStart()) {
        field_0x62e = 1;
        field_0x5ee.set(0, 0, 0);
    }

    if (mMode == 0) {
        mCcSph.OffCoSetBit();
        mCcSph.OffAtSetBit();
        mCcSph.OffTgSetBit();

        gravity = 0.0f;
        speed.y = 0.0f;
        speedF = 0.0f;
        mMode = 1;
    }

    if (!fopAcM_CheckCarryType(this, fopAcM_CARRY_UNK_30)) {
        shape_angle.x = field_0x608;
    }

    if (!fopAcM_checkCarryNow(this)) {
        mCcSph.OnCoSetBit();
        shape_angle.z = 0;
        setActionMode(ACTION_PART_e, 5);

        if (fopAcM_GetSpeedF(this) >= 1.0f) {
            fopAcM_carryOffRevise(this);
        }

        field_0x62e = 0;
    }
}

void daObj_Kanban2_c::action() {
    mAcch.SetGroundUpY(0.0f);

    switch (mAction) {
    case ACTION_NORMAL_e:
        executeNormal();
        break;
    case ACTION_PART_e:
        executePart();
        shape_angle.x -= field_0x5e8.x;
        shape_angle.y += field_0x5e8.y;
        cLib_chaseAngleS(&field_0x5e8.x, 0, 0x100);
        cLib_chaseAngleS(&field_0x5e8.y, 0, 0x100);
        break;
    case ACTION_FLOAT_e:
        executeFloat();
        break;
    case ACTION_CARRY_e:
        executeCarry();
        break;
    }

    if (mAction == ACTION_NORMAL_e) {
        fopAcM_posMoveF(this, NULL);
    } else {
        fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
        mAcch.CrrPos(dComIfG_Bgsp());
    }
}

void daObj_Kanban2_c::mtx_set() {
    if (field_0x62e != 0) {
        csXyz sp8(0, 0, 0);
        cXyz sp10(0.0f, 0.0f, 0.0f);

        if (field_0x620 == 1) {
            for (int i = 0; i < 18; i++) {
                if (mPartFlags & (1 << i)) {
                    if (i < 8) {
                        sp10.set(10.0f, 0.0f, -10.0f);
                        sp8.z = dKn2_CarryOffset[i];
                    } else {
                        sp10.set(0.0f, 0.0f, -10.0f);
                        sp8.z = -15000;
                    }
                    break;
                }
            }

            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::ZXYrotM(shape_angle);
            mDoMtx_stack_c::ZXYrotM(sp8);
            mDoMtx_stack_c::transM(field_0x5dc);
            mDoMtx_stack_c::transM(sp10);
        } else {
            if (shape_angle.x < 0) {
                sp10.set(0.0f, 0.0f, 10.0f);
            } else {
                sp10.set(0.0f, 0.0f, -20.0f);
            }

            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::ZXYrotM(shape_angle);
            mDoMtx_stack_c::transM(field_0x5dc);
            mDoMtx_stack_c::transM(sp10);
        }
    } else {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(field_0x5ee);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::transM(field_0x5dc);
        mDoMtx_stack_c::ZXYrotM(field_0x5f4);
    }

    mDoMtx_stack_c::scaleM(l_HIO.base_size, l_HIO.base_size, l_HIO.base_size);

    if (mPartFlags == PARTS_ALL) {
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        return;
    }

    for (int i = 0; i < 18; i++) {
        if (mPartFlags & (1 << i)) {
            mpPartModel[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }
}

void daObj_Kanban2_c::cc_set() {
    cXyz sph_center(current.pos);

    if (mAction == ACTION_NORMAL_e) {
        if (mPartFlags & PARTS_TOP_HALF) {
            sph_center.y += 100.0f;
        } else if (mPartFlags & PARTS_BOTTOM_HALF) {
            sph_center.y += 70.0f;
        } else {
            sph_center.y += 40.0f;
        }
    }

    mCcSph.SetC(sph_center);

    if (dComIfGp_event_runCheck()) {
        mCcSph.SetR(10.0f + field_0x600);
    } else {
        mCcSph.SetR(field_0x600);
    }

    dComIfG_Ccsp()->Set(&mCcSph);
}

int daObj_Kanban2_c::execute() {
    if (mPartFlags == 0) {
        fopAcM_delete(this);
        return 1;
    }

    if (field_0x629 != 0) {
        field_0x629--;
    }

    if (field_0x62a != 0) {
        field_0x62a--;
    }

    if (mInvulnerabilityTimer != 0) {
        mInvulnerabilityTimer--;
    }

    action();
    mtx_set();
    cc_set();

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    return 1;
}

static int daObj_Kanban2_Execute(daObj_Kanban2_c* i_this) {
    return i_this->execute();
}

static int daObj_Kanban2_IsDelete(daObj_Kanban2_c* i_this) {
    return 1;
}

int daObj_Kanban2_c::_delete() {
    dComIfG_resDelete(&mPhase, "Obj_kn2");

    if (mInitHIO) {
        l_initHIO = false;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daObj_Kanban2_Delete(daObj_Kanban2_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    return i_this->_delete();
}

int daObj_Kanban2_c::CreateHeap() {
    void* modelData;
    u32 part_flags = fopAcM_GetParam(this) & 0x3FFFF;

    if (part_flags == PARTS_ALL) {
        modelData = dComIfG_getObjectRes("Obj_kn2", l_kn2_bmdidx[0]);
        JUT_ASSERT(2214, modelData != NULL);

        mpModel = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
        if (mpModel == NULL) {
            return 0;
        }
    }

    for (int i = 0; i < 18; i++) {
        if (part_flags & (1 << i)) {
            modelData = dComIfG_getObjectRes("Obj_kn2", l_kn2_bmdidx[i + 1]);
            JUT_ASSERT(2228, modelData != NULL);
            void* shareModelData = dComIfG_getObjectRes("Obj_kn2", l_kn2_bmdidx[0]);
            mDoExt_setupShareTexture((J3DModelData*)modelData, (J3DModelData*)shareModelData);
            mpPartModel[i] = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
            if (mpPartModel[i] == NULL) {
                return 0;
            }
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daObj_Kanban2_c* a_this = (daObj_Kanban2_c*)i_this;
    return a_this->CreateHeap();
}

int daObj_Kanban2_c::create() {
    fopAcM_ct(this, daObj_Kanban2_c);

    int phase_state = dComIfG_resLoad(&mPhase, "Obj_kn2");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_KANBAN2 PARAM %x %d %d %d %d \n", fopAcM_GetParam(this), current.angle.x, shape_angle.x, current.angle.z, shape_angle.z);
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x9800)) {
            return cPhs_ERROR_e;
        }

        if (!l_initHIO) {
            l_initHIO = true;
            mInitHIO = true;
            l_HIO.id = mDoHIO_CREATE_CHILD("細切れ看板", &l_HIO);
        }

        mFlowID = current.angle.x;
        shape_angle.x = 0;
        current.angle.x = 0;

        mPartFlags = fopAcM_GetParam(this) & 0x3FFFF;
        u8 mode = (fopAcM_GetParam(this) & 0xF0000000) >> 0x1C;
        if (mode == 0xF) {
            mode = 0;
        }

        if (mPartFlags == PARTS_ALL) {
            setActionMode(ACTION_NORMAL_e, 0);
            fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        } else {
            setActionMode(ACTION_PART_e, mode);
            setCullMtx();
        }

        setCenterPos();
        fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
        mAcchCir.SetWall(10.0f, 10.0f + field_0x600);

        health = 10;
        field_0x560 = 10;
    
        if (mPartFlags == PARTS_ALL) {
            mCcStts.Init(nREG_S(0) + 250, 0, this);
        } else {
            mCcStts.Init(100, 0, this);
        }
    
        mCcSph.Set(cc_kn2_src);
        mCcSph.SetStts(&mCcStts);
        mCcSph.OnTgNoHitMark();

        mSound.init(&current.pos, 1);
        mAtInfo.mPowerType = 1;

        //! @bug probably intended to be "||"
        if (mAction != ACTION_PART_e | mMode != 2) {
            current.pos -= field_0x5dc;
        }

        daObj_Kanban2_Execute(this);
    }

    return phase_state;
}

static int daObj_Kanban2_Create(daObj_Kanban2_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daObj_Kanban2_Method = {
    (process_method_func)daObj_Kanban2_Create,
    (process_method_func)daObj_Kanban2_Delete,
    (process_method_func)daObj_Kanban2_Execute,
    (process_method_func)daObj_Kanban2_IsDelete,
    (process_method_func)daObj_Kanban2_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_KANBAN2 = {
  fpcLy_CURRENT_e,         // mLayerID
  7,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_OBJ_KANBAN2,        // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daObj_Kanban2_c), // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  63,                      // mPriority
  &l_daObj_Kanban2_Method, // sub_method
  0x00044100,              // mStatus
  fopAc_ENV_e,             // mActorType
  fopAc_CULLBOX_0_e,       // cullType
};

AUDIO_INSTANCES
