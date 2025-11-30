/**
* @file d_a_obj_movebox.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_movebox.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_player.h"
#include "d/d_path.h"
#include "d/actor/d_a_obj_burnbox.h"
#include "d/actor/d_a_obj_eff.h"

const daObjMovebox::BgcSrc_c daObjMovebox::Bgc_c::M_lin5[] = {
    {0.0f, 0.0f, 0.0f, 0.0f},   {-0.5f, -0.5f, 1.0f, 1.0f}, {0.5f, -0.5f, -1.0f, 1.0f},
    {0.5f, 0.5f, -1.0f, -1.0f}, {-0.5f, 0.5f, 1.0f, -1.0f},
};

const daObjMovebox::BgcSrc_c daObjMovebox::Bgc_c::M_lin20[] = {
    {0.0f, 0.0f, 0.0f, 0.0f},    {-0.25f, -0.25f, 0.0f, 0.0f}, {-0.5f, -0.5f, 1.0f, 1.0f},
    {0.0f, -0.5f, -1.0f, 1.0f},  {0.0f, 0.0f, -1.0f, -1.0f},   {-0.5f, 0.0f, 1.0f, -1.0f},
    {0.25f, -0.25f, 0.0f, 0.0f}, {0.0f, -0.5f, 1.0f, 1.0f},    {0.5f, -0.5f, -1.0f, 1.0f},
    {0.5f, 0.0f, -1.0f, -1.0f},  {0.0f, 0.0f, 1.0f, -1.0f},    {0.25f, 0.25f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 1.0f},    {0.5f, 0.0f, -1.0f, 1.0f},    {0.5f, 0.5f, -1.0f, -1.0f},
    {0.0f, 0.5f, 1.0f, -1.0f},   {-0.25f, 0.25f, 0.0f, 0.0f},  {-0.5f, 0.0f, 1.0f, 1.0f},
    {0.0f, 0.0f, -1.0f, 1.0f},   {0.0f, 0.5f, -1.0f, -1.0f},   {-0.5f, 0.5f, 1.0f, -1.0f},
    {-0.75f, 0.25f, 0.0f, 0.0f}, {-0.75f, -0.25f, 0.0f, 0.0f},
};

daObjMovebox::Bgc_c::Bgc_c() {
    for (int i = 0; i < 23; i++) {
        field_0x0[i] = 0.0f;
        field_0x64[i] = cXyz::Zero;
    }

    field_0x5c = 0xFFFFFFFF;
    field_0x178 = 0xFFFFFFFF;
    field_0x17c = FLT_MAX;
    field_0x60 = 0.0f;
    mState = STATE_0_e;
}

dBgS_ObjGndChk daObjMovebox::Bgc_c::M_gnd_work[23];

void daObjMovebox::Bgc_c::gnd_pos(const daObjMovebox::Act_c* i_actor,
                                  const daObjMovebox::BgcSrc_c* i_bgcSrc, int i_checkNum,
                                  f32 param_3) {
    cXyz sp50;
    cXyz sp5C;
    f32 var_f31 = -G_CM3D_F_INF;

    mDoMtx_stack_c::transS(i_actor->current.pos);
    mDoMtx_stack_c::YrotM(i_actor->home.angle.y);
    field_0x5c = -1;

    for (int i = 0; i < i_checkNum; i++, i_bgcSrc++) {
        sp50.set(i_bgcSrc->field_0xc + i_bgcSrc->field_0x4 * i_actor->attr().field_0x70, param_3,
                 i_bgcSrc->field_0x8 + i_bgcSrc->field_0x0 * i_actor->attr().field_0x78);

        mDoMtx_stack_c::multVec(&sp50, &sp5C);
        M_gnd_work[i].SetPos(&sp5C);
        M_gnd_work[i].SetActorPid(i_actor->base.base.id);
        field_0x0[i] = dComIfG_Bgsp().GroundCross(&M_gnd_work[i]);

        if (field_0x0[i] > var_f31) {
            fopAc_ac_c* bg_actor = dComIfG_Bgsp().GetActorPointer(M_gnd_work[i]);
            if (bg_actor == NULL || fopAcM_GetName(bg_actor) != PROC_Obj_Movebox ||
                ((daObjMovebox::Act_c*)bg_actor)->field_0x5ac != 2)
            {
                var_f31 = field_0x0[i];
                field_0x5c = i;
            }
        }
    }
}

dBgS_WtrChk daObjMovebox::Bgc_c::M_wrt_work;

void daObjMovebox::Bgc_c::wrt_pos(cXyz const& param_0) {
    cXyz sp28(param_0);

    f32 var_f31 = param_0.y + 400.0f;
    sp28.y -= 100.0f;

    M_wrt_work.Set(sp28, var_f31);

    if (dComIfG_Bgsp().WaterChk(&M_wrt_work)) {
        field_0x60 = M_wrt_work.GetHeight();

        cLib_onBit<daObjMovebox::Bgc_c::State_e>(mState, STATE_8_e);
        if (field_0x60 > param_0.y) {
            cLib_onBit<daObjMovebox::Bgc_c::State_e>(mState, STATE_10_e);
        }
    }
}

dBgS_ObjLinChk daObjMovebox::Bgc_c::M_wall_work[23];

void daObjMovebox::Bgc_c::wall_pos(daObjMovebox::Act_c const* i_actor,
                                   daObjMovebox::BgcSrc_c const* i_bgcSrc, int i_num, s16 param_3,
                                   f32 param_4) {
    const s16 sp8C = i_actor->home.angle.y + param_3;

    cXyz sp48;
    cXyz sp54;
    cXyz sp60;
    cXyz sp6C;
    cXyz sp78;

    field_0x178 = -1;
    field_0x17c = FLT_MAX;

    mDoMtx_stack_c::YrotS(sp8C);
    mDoMtx_stack_c::XrotM(0x4000);
    mDoMtx_stack_c::multVec(&cXyz::BaseY, &sp6C);
    sp6C *= param_4 + i_actor->attr().field_0x78 * 0.5f;

    for (int i = 0; i < i_num; i++, i_bgcSrc++) {
        mDoMtx_stack_c::XrotS(0x4000);
        sp78.set(i_bgcSrc->field_0xc, 0.0f, i_bgcSrc->field_0x8);
        mDoMtx_stack_c::multVec(&sp78, &sp60);

        mDoMtx_stack_c::YrotS(sp8C);
        mDoMtx_stack_c::transM(sp60);
        mDoMtx_stack_c::scaleM(i_actor->attr().field_0x70, i_actor->attr().field_0x64,
                               i_actor->attr().field_0x70);
        mDoMtx_stack_c::transM(0.0f, 0.5f, 0.0f);
        mDoMtx_stack_c::XrotM(0x4000);

        sp78.set(i_bgcSrc->field_0x4, 0.0f, i_bgcSrc->field_0x0);
        mDoMtx_stack_c::multVec(&sp78, &sp48);
        sp48 += i_actor->current.pos;
        sp54 = sp48 + sp6C;

        M_wall_work[i].Set(&sp48, &sp54, i_actor);
        M_wall_work[i].SetActorPid(i_actor->base.base.id);

        if (dComIfG_Bgsp().LineCross(&M_wall_work[i])) {
            field_0x64[i] = M_wall_work[i].GetCross();

            f32 var_f31 = sp48.abs2(field_0x64[i]);
            if (var_f31 < field_0x17c) {
                field_0x17c = var_f31;
                field_0x178 = i;
            }
        } else {
            field_0x64[i] = cXyz::Zero;
        }
    }
}

void daObjMovebox::Bgc_c::proc_vertical(daObjMovebox::Act_c* i_actor) {
    State_e var_r29 = mState;
    mState = STATE_0_e;

    int check_num = i_actor->attr().field_0x9e ? 21 : 5;
    const BgcSrc_c* pbgc = i_actor->attr().field_0x9e ? M_lin20 : M_lin5;

    gnd_pos(i_actor, pbgc, check_num, 100.0f);

    if (field_0x5c >= 0 && field_0x0[field_0x5c] > i_actor->current.pos.y) {
        i_actor->current.pos.y = field_0x0[field_0x5c];
        cLib_onBit<State_e>(mState, STATE_1_e);
        i_actor->speed.y = 0.0f;
    }

    wrt_pos(i_actor->current.pos);

    if (cLib_checkBit<State_e>(mState, STATE_8_e) && field_0x60 > i_actor->current.pos.y) {
        cLib_onBit<State_e>(mState, STATE_10_e);
    }

    if (!cLib_checkBit<State_e>(mState, STATE_1_e) && cLib_checkBit<State_e>(var_r29, STATE_1_e)) {
        cLib_onBit<State_e>(mState, STATE_2_e);
        i_actor->speed.y = i_actor->attr().mFallYInitSpeed;
    }

    if (cLib_checkBit<State_e>(mState, STATE_1_e) && !cLib_checkBit<State_e>(var_r29, STATE_1_e)) {
        cLib_onBit<State_e>(mState, STATE_4_e);
    }

    if (cLib_checkBit<State_e>(mState, STATE_10_e) && !cLib_checkBit<State_e>(var_r29, STATE_10_e))
    {
        cLib_onBit<State_e>(mState, STATE_20_e);
    }

    if (cLib_checkBit<State_e>(mState, STATE_1_e) && field_0x5c >= 0) {
        dComIfG_Bgsp().RideCallBack(M_gnd_work[field_0x5c], i_actor);
    }
}

bool daObjMovebox::Bgc_c::chk_wall_pre(daObjMovebox::Act_c const* i_actor,
                                       daObjMovebox::BgcSrc_c const* i_bgcSrc, int i_num,
                                       s16 param_3) {
    wall_pos(i_actor, i_bgcSrc, i_num, param_3, i_actor->attr().mUnitDistance - 1.0f);
    return field_0x178 >= 0;
}

bool daObjMovebox::Bgc_c::chk_wall_touch(daObjMovebox::Act_c const* i_actor,
                                         daObjMovebox::BgcSrc_c const* i_bgcSrc, s16 param_2) {
    static dBgS_ObjLinChk touch_work;

    const s16 sp8C = i_actor->home.angle.y + param_2;

    cXyz sp38;
    cXyz sp44;
    cXyz sp50;
    cXyz sp5C;
    cXyz sp68;

    mDoMtx_stack_c::YrotS(sp8C);
    mDoMtx_stack_c::XrotM(0x4000);
    sp50.set(i_bgcSrc->field_0xc, 0.0f, i_bgcSrc->field_0x8);
    mDoMtx_stack_c::multVec(&cXyz::BaseY, &sp68);
    sp68 *= i_actor->attr().field_0x70 * 0.5f + 10.0f;

    mDoMtx_stack_c::XrotM(0x4000);
    sp50.set(i_bgcSrc->field_0xc, 0.0f, i_bgcSrc->field_0x8);
    mDoMtx_stack_c::multVec(&sp50, &sp5C);
    mDoMtx_stack_c::YrotS(sp8C);

    mDoMtx_stack_c::transM(sp5C);
    mDoMtx_stack_c::scaleM(i_actor->attr().field_0x70, i_actor->attr().field_0x64,
                           i_actor->attr().field_0x70);
    mDoMtx_stack_c::transM(0.0f, 0.5f, 0.0f);
    mDoMtx_stack_c::XrotM(0x4000);

    sp50.set(i_bgcSrc->field_0x4, 0.0f, i_bgcSrc->field_0x0);
    mDoMtx_stack_c::multVec(&sp50, &sp38);
    sp38 += i_actor->current.pos;
    sp44 = sp38 + sp68;

    touch_work.SetActorPid(i_actor->base.base.id);
    touch_work.Set(&sp38, &sp44, i_actor);
    return dComIfG_Bgsp().LineCross(&touch_work);
}

bool daObjMovebox::Bgc_c::chk_wall_touch2(daObjMovebox::Act_c const* i_actor,
                                          daObjMovebox::BgcSrc_c const* i_bgcSrc, int i_num,
                                          s16 param_3) {
    bool chk = false;
    for (int i = 0; i < i_num; i++) {
        if (chk_wall_touch(i_actor, &i_bgcSrc[i], param_3)) {
            chk = true;
            break;
        }
    }

    return chk;
}

int daObjMovebox::Act_c::prm_get_swSave1() const {
    return prm_get_swSave();
}

void daObjMovebox::Act_c::prmZ_init() {
    if (!field_0x900) {
        field_0x900 = 1;

        if (prm_get_dmy()) {
            mPrmZ = 0xFFFF;
        } else {
            mPrmZ = home.angle.z;
        }

        home.angle.z = 0;
        current.angle.z = 0;
        shape_angle.z = 0;
    }
}

void daObjMovebox::Act_c::prmX_init() {
    if (!field_0x901) {
        mPrmX = home.angle.x;
        field_0x901 = 1;
    }

    home.angle.x = 0;
    current.angle.x = 0;
    shape_angle.x = 0;
}

const char* const daObjMovebox::Act_c::M_arcname[8] = {
    "Kkiba_00", "Osiblk", "H_Box20", "A_SMBlock", "P_Lv4blk", "P_Lv4blk2", "H_OsiHaka", "dummy4",
};

const dCcD_SrcCyl daObjMovebox::Act_c::M_cyl_src = {
    {
        0,
        {{0, 0, 0}, {0xD8FAFDBF, 0x11}, {0x78}},  // mObj
        {dCcD_SE_NONE, 0, 0, 0, 0},               // mGObjAt
        {dCcD_SE_NONE, 0, 0, 0, 0},               // mGObjTg
        {0},                                      // mGObjCo
    },                                            // mObjInf
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        75.0f,               // mRadius
        150.0f,              // mHeight
    }  // mCyl
};

const daObjMovebox::Hio_c::Attr_c daObjMovebox::Act_c::M_attr[8] = {
    {
        0x6,     0xE,     0xA,     0x6,           0xE,    0xA,           75.0f,  90.0f,   -3.0f,
        0.005f,  0.001f,  0.0f,    1.8f,          3.9f,   -0.39f,        -0.2f,  0.02f,   0x3E8,
        0.04f,   0.013f,  0.15f,   0.1f,          0.1f,   0.06f,         0.075f, 4,       7,
        0xC00,   150.0f,  150.0f,  0.0066666668f, 150.0f, 0.0066666668f, 150.0f, 0x8013C, 0x8013D,
        0x80151, 0x8002A, 0x8002D, 0xFFA6,        0xFFFF, 0xFFA6,        0x5A,   0x97,    0x5A,
        0,       0,       0,
    },
    {
        0x8,     0x13,    0xD,     0x8,           0x13,   0xD,           75.0f,  90.0f,   -5.0f,
        0.005f,  0.001f,  0.0f,    1.8f,          4.5f,   -0.5f,         -0.2f,  0.02f,   0x3E8,
        0.04f,   0.013f,  0.15f,   0.1f,          0.1f,   0.06f,         0.075f, 4,       7,
        0xC00,   150.0f,  150.0f,  0.0066666668f, 150.0f, 0.0066666668f, 150.0f, 0x20038, 0x20039,
        0x8002F, 0x8002B, 0x8002E, 0xFFA6,        0xFFFF, 0xFFA6,        0x5A,   0x97,    0x5A,
        0,       0,       0,
    },
    {
        0x8,    0x13,   0xD,    0x8,    0x13,    0xD,     50.0f,   110.0f,  -5.0f,   0.005f,
        0.001f, 0.0f,   1.8f,   4.5f,   -0.5f,   -0.2f,   0.02f,   0x3E8,   0.04f,   0.013f,
        0.15f,  0.1f,   0.1f,   0.06f,  0.075f,  4,       7,       0x1000,  200.0f,  200.0f,
        0.005f, 200.0f, 0.005f, 200.0f, 0x80154, 0x80155, 0x8002F, 0x8002B, 0x8002E, 0xFF92,
        0xFFFF, 0xFF92, 0x6E,   0xD2,   0x6E,    0,       1,       0,
    },
    {
        0x8,     0x13,    0xD,     0x8,           0x13,   0xD,           75.0f,  180.0f,  -5.0f,
        0.005f,  0.001f,  0.0f,    1.8f,          4.5f,   -0.5f,         -0.2f,  0.02f,   0x3E8,
        0.04f,   0.013f,  0.15f,   0.1f,          0.1f,   0.06f,         0.075f, 4,       7,
        0xC00,   300.0f,  300.0f,  0.0033333334f, 300.0f, 0.0033333334f, 300.0f, 0x20038, 0x20039,
        0x8002F, 0x8002B, 0x8002E, 0xFF4C,        0xFFFF, 0xFF4C,        0xB4,   0x136,   0xB4,
        0,       0,       0,
    },
    {
        0x8,     0x13,    0xD,     0x8,           0x13,   0xD,           75.0f,  180.0f,  -5.0f,
        0.005f,  0.001f,  0.0f,    1.8f,          4.5f,   -0.5f,         -0.2f,  0.02f,   0x3E8,
        0.04f,   0.013f,  0.15f,   0.1f,          0.1f,   0.06f,         0.075f, 4,       7,
        0x2670,  300.0f,  300.0f,  0.0033333334f, 300.0f, 0.0033333334f, 300.0f, 0x20038, 0x20039,
        0x8002F, 0x8002B, 0x8002E, 0xFF4C,        0xFFFF, 0xFF4C,        0xB4,   0x136,   0xB4,
        0,       0,       0,
    },
    {
        0x8,     0x13,    0xD,     0x8,           0x13,   0xD,           75.0f,  90.0f,   -5.0f,
        0.005f,  0.001f,  0.0f,    1.8f,          4.5f,   -0.5f,         -0.2f,  0.02f,   0x3E8,
        0.04f,   0.013f,  0.15f,   0.1f,          0.1f,   0.06f,         0.075f, 4,       7,
        0xC00,   150.0f,  150.0f,  0.0066666668f, 150.0f, 0.0066666668f, 150.0f, 0x20038, 0x20039,
        0x8002F, 0x8002B, 0x8002E, 0xFFA6,        0xFFFF, 0xFFA6,        0x5A,   0x168,   0x5A,
        0,       0,       0,
    },
    {
        0x8,    0x13,   0xD,           0x8,    0x13,    0xD,     50.0f,   90.0f,   -3.0f,   0.005f,
        0.001f, 0.0f,   1.8f,          3.9f,   -0.39f,  -0.2f,   0.02f,   0x3E8,   0.04f,   0.013f,
        0.15f,  0.1f,   0.1f,          0.06f,  0.075f,  4,       7,       0x1620,  200.0f,  200.0f,
        0.005f, 230.0f, 0.0043478259f, 100.0f, 0x8013C, 0x8013D, 0x80151, 0x8002A, 0x8002D, 0xFF7E,
        0xFFF6, 0xFFC4, 0x82,          0xD2,   0x3C,    0,       1,       0,
    },
    {
        0x4,     0x4,     0x14,    0x4,           0x4,    0x14,          75.0f,  90.0f,   -3.0f,
        0.005f,  0.001f,  0.0f,    1.8f,          3.9f,   -0.39f,        -0.2f,  0.02f,   0x3E8,
        0.04f,   0.013f,  0.15f,   0.1f,          0.1f,   0.06f,         0.075f, 4,       7,
        0x1620,  150.0f,  150.0f,  0.0066666668f, 150.0f, 0.0066666668f, 150.0f, 0x8013C, 0x8013D,
        0x80151, 0x8002A, 0x8002D, 0xFFA6,        0xFFFF, 0xFFA6,        0x5A,   0x97,    0x5A,
        0,       0,       0,
    },
};

const daObjMovebox::Hio_c::Attr_c& daObjMovebox::Act_c::attr() const {
    return M_attr[mType];
}

void daObjMovebox::Act_c::set_mtx() {
    bool var_r29 = field_0x5ac == 2;

    mDoMtx_stack_c::transS(current.pos);

    if (var_r29) {
        cXyz sp28(field_0x8c8, 1.0f, field_0x8cc);
        mDoMtx_stack_c::transM(0.0f, field_0x8d8, 0.0f);

        Quaternion quat;
        daObj::quat_rotBaseY(&quat, sp28);
        mDoMtx_stack_c::quatM(&quat);
    }

    mDoMtx_stack_c::ZXYrotM(shape_angle);

    if (var_r29) {
        mDoMtx_stack_c::transM(0.0f, -field_0x8d8, 0.0f);
    }

    if (mpModel != NULL) {
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
}

void daObjMovebox::Act_c::init_mtx() {
    if (mpModel != NULL) {
        mpModel->setBaseScale(scale);
    }

    set_mtx();
}

void daObjMovebox::Act_c::path_init() {
    int path_id = prmZ_get_pathId();
    int sw1 = prm_get_swSave1();

    if (path_id != 0xFF && sw1 != 0xFF) {
        int sw2 = prmZ_get_swSave2();
        bool is_sw1 = is_switch1();
        bool var_r22 = sw2 == 0xFF ? false : is_switch2();

        int pnt_no = 0;
        if (is_sw1) {
            pnt_no++;
        }

        if (var_r22) {
            pnt_no += 2;
        }

        mpPath = dPath_GetRoomPath(path_id, fopAcM_GetHomeRoomNo(this));
        dPnt* point = dPath_GetPnt(mpPath, pnt_no);
        home.pos.set(point->m_position);
        current.pos.set(point->m_position);
    }
}

void daObjMovebox::Act_c::path_save() {
    int path_id = prmZ_get_pathId();
    int sw1 = prm_get_swSave1();
    u8 answer = prm_get_answer();

    if (path_id != 0xFF && sw1 != 0xFF) {
        int sw2 = prmZ_get_swSave2();

        int var_r27 = sw2 == 0xFF ? 2 : 4;
        if (var_r27 > mpPath->m_num) {
            var_r27 = mpPath->m_num;
        }

        int var_r29 = 0;
        for (; var_r29 < var_r27; var_r29++) {
            dPnt* pnt = dPath_GetPnt(mpPath, var_r29);
            cXyz sp38;
            sp38.set(pnt->m_position);

            if (current.pos.abs2(sp38) < 9.0f) {
                break;
            }
        }

        if (var_r29 < var_r27) {
            if (var_r29 & 1) {
                on_switch1();
            } else {
                off_switch1();
            }

            if (sw2 != 0xFF) {
                if (var_r29 & 2) {
                    on_switch2();
                } else {
                    off_switch2();
                }
            }
        }

        if (answer != 0xF) {
            int var_r28 = 0;
            if (is_switch1()) {
                var_r28++;
            }

            if (is_switch2()) {
                var_r28 += 2;
            }

            if (answer == var_r28) {
                mpBgW->OffPushPullOk();
            }
        }
    }
}

int daObjMovebox::Act_c::CreateHeap() {
    BOOL var_r29 = true;

    if (i_attr().field_0x58 >= 0) {
        J3DModelData* modelData =
            (J3DModelData*)dComIfG_getObjectRes(M_arcname[mType], i_attr().field_0x58);
        JUT_ASSERT(0, modelData != NULL);

        mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        var_r29 = mpModel != NULL;
    } else {
        mpModel = NULL;
    }

    return var_r29;
}

void daObjMovebox::Act_c::RideCallBack(dBgW* i_bgw, fopAc_ac_c* i_actor, fopAc_ac_c* i_rideActor) {
    Act_c* a_this = (Act_c*)i_actor;

    if (a_this->field_0x5ac == 2) {
        f32 var_f31;
        f32 var_f30;
        f32 var_f26;
        f32 var_f29;
        f32 var_f28;
        f32 var_f27;

        var_f26 = a_this->i_attr().mPlayerGravity + a_this->i_attr().mMiscGravity;
        var_f29 = i_rideActor->current.pos.x - a_this->current.pos.x;
        var_f28 = i_rideActor->current.pos.z - a_this->current.pos.z;

        if (fopAcM_GetProfName(i_rideActor) == PROC_ALINK) {
            var_f30 = a_this->i_attr().mPlayerGravity;
            var_f27 = a_this->i_attr().field_0x74 * a_this->i_attr().mPlayerTiltPower;
        } else {
            var_f30 = a_this->i_attr().mMiscGravity;
            var_f27 = a_this->i_attr().field_0x74 * a_this->i_attr().mMiscTiltPower;
        }

        var_f31 = 1.0f - (JMAFastSqrt(var_f29 * var_f29 + var_f28 * var_f28) *
                          a_this->i_attr().field_0x74);
        var_f31 = (var_f31 * 0.9f) + 0.1f;
        if (var_f31 < 0.0f) {
            var_f31 = 0.0f;
        }

        var_f30 *= var_f31;

        a_this->field_0x8bc += var_f30;
        if (a_this->field_0x8bc < var_f26) {
            a_this->field_0x8bc = var_f26;
        }

        a_this->field_0x8c0 += var_f27 * var_f29;
        a_this->field_0x8c4 += var_f27 * var_f28;
    } else if (fopAcM_GetProfName(i_rideActor) == PROC_Obj_Movebox) {
        cLib_onBit<daObjMovebox::Bgc_c::State_e>(a_this->mBgc.mState,
                                                 daObjMovebox::Bgc_c::STATE_40_e);
    }
}

enum MOVE_DIR {
    DIR_SOUTH,
    DIR_EAST,
    DIR_NORTH,
    DIR_WEST,
};

fopAc_ac_c* daObjMovebox::Act_c::PPCallBack(fopAc_ac_c* i_bgActor, fopAc_ac_c* i_pushActor,
                                            s16 i_angle, dBgW_Base::PushPullLabel i_pplabel) {
    Act_c* a_this = (Act_c*)i_bgActor;
    dBgW::PushPullLabel var_r26 = cLib_checkBit<dBgW::PushPullLabel>(
        i_pplabel, (dBgW::PushPullLabel)(dBgW::PPLABEL_PUSH | dBgW::PPLABEL_PULL));

    bool sp34;
    if (a_this->i_attr().field_0x9e) {
        sp34 = cLib_checkBit<dBgW::PushPullLabel>(i_pplabel, dBgW::PPLABEL_HEAVY) != 0;
    } else {
        sp34 = true;
    }

    if (var_r26 != 0 && sp34) {
        s16 var_r29 = (s16)(cLib_checkBit<dBgW::PushPullLabel>(var_r26, dBgW::PPLABEL_PULL) ?
                                i_angle - 0x8000 :
                                i_angle) -
                      a_this->home.angle.y;

        a_this->mPPLabel = i_pplabel;

        int move_dir;
        if (var_r29 >= -0x2000 && var_r29 < 0x2000) {
            move_dir = DIR_SOUTH;
        } else if (var_r29 >= 0x2000 && var_r29 < 0x6000) {
            move_dir = DIR_EAST;
        } else if (var_r29 >= 0x6000 || var_r29 < -0x6000) {
            move_dir = DIR_NORTH;
        } else {
            move_dir = DIR_WEST;
        }

        for (int i = 0; i < 4; i++) {
            if (i == move_dir) {
                a_this->field_0x8f0[i]++;
            } else {
                a_this->field_0x8f0[i] = 0;
            }
        }

        a_this->field_0x8fe = 1;
    }
}

int daObjMovebox::Act_c::Create() {
    field_0x8b8 = 0;
    field_0x8bc = 0.0f;
    field_0x8c0 = 0.0f;
    field_0x8c4 = 0.0f;
    field_0x8c8 = 0.0f;
    field_0x8cc = 0.0f;
    field_0x8d0 = 0.0f;
    field_0x8d4 = 0.0f;
    field_0x8d8 = i_attr().field_0x68 * 0.5f;
    field_0x8dc = 0;
    field_0x8e0 = 0;
    field_0x8e4 = 0.0f;
    field_0x8e8 = -1;
    mPPLabel = dBgW::PPLABEL_NONE;

    clr_moment_cnt();

    field_0x8f8 = 0;
    field_0x8fa = 0;
    field_0x8fc = 20;
    field_0x8fe = 0;

    mpBgW->SetRideCallback(RideCallBack);
    mpBgW->SetPushPullCallback(PPCallBack);

    mCcStts.Init(0xFF, 0xFF, this);
    mCcCyl.Set(M_cyl_src);
    mCcCyl.SetStts(&mCcStts);
    mCcCyl.SetTgVec(const_cast<cXyz&>(cXyz::Zero));
    mCcCyl.OnTgNoHitMark();
    mCcCyl.OffCoSPrmBit(1);

    fopAcM_SetMtx(this, mBgMtx);

    fopAcM_setCullSizeBox(this, i_attr().field_0x90, i_attr().field_0x92, i_attr().field_0x94,
                          i_attr().field_0x96, i_attr().field_0x98, i_attr().field_0x9a);
    fopAcM_SetSpeedF(this, 0.0f);
    fopAcM_SetGravity(this, i_attr().mGravity);
    fopAcM_posMoveF(this, NULL);

    mBgc.proc_vertical(this);
    cLib_offBit<daObjMovebox::Bgc_c::State_e>(
        mBgc.mState, (daObjMovebox::Bgc_c::State_e)(daObjMovebox::Bgc_c::STATE_20_e |
                                                    daObjMovebox::Bgc_c::STATE_4_e |
                                                    daObjMovebox::Bgc_c::STATE_2_e));
    field_0x902 = 1;
    mReverb = dComIfGp_getReverb(home.roomNo);

    u8 answer = prm_get_answer();
    int var_r30 = 0;
    if (is_switch1()) {
        var_r30++;
    }

    if (is_switch2()) {
        var_r30 += 2;
    }

    if (answer != 0xF && answer == var_r30) {
        mpBgW->OffPushPullOk();
    }

    init_mtx();
    mpBgW->Move();
    field_0x904 = 0;
    eyePos = current.pos;

    mode_wait_init();
    return 1;
}

void daObjMovebox::Act_c::afl_sway() {
    const BgcSrc_c* pbgc;
    s32 check_num;
    bool var_r22;
    bool var_r21;

    f32 var_f31 = field_0x8c0 * field_0x8c0 + field_0x8c4 * field_0x8c4;
    f32 var_f29 = i_attr().mMaxTiltPower * i_attr().mMaxTiltPower;

    if (i_attr().field_0x9e) {
        pbgc = mBgc.M_lin20;
    } else {
        pbgc = mBgc.M_lin5;
    }

    check_num = i_attr().field_0x9e ? 21 : 5;

    var_r22 = mBgc.chk_wall_touch2(this, pbgc, check_num, M_dir_base[0]) ||
              mBgc.chk_wall_touch2(this, pbgc, check_num, M_dir_base[2]);

    var_r21 = mBgc.chk_wall_touch2(this, pbgc, check_num, M_dir_base[1]) ||
              mBgc.chk_wall_touch2(this, pbgc, check_num, M_dir_base[3]);

    if (var_f31 > var_f29) {
        f32 var_f30 = i_attr().mMaxTiltPower / JMAFastSqrt(var_f31);
        field_0x8c0 *= var_f30;
        field_0x8c4 *= var_f30;
    }

    f32 var_f28;
    f32 var_f27;
    f32 var_f26;
    f32 var_f25;
    f32 var_f24_debug = field_0x8c8 - field_0x8c0;
    f32 var_f23_debug = field_0x8cc - field_0x8c4;
    var_f28 = i_attr().mTiltSpringFactor * -(var_f24_debug);
    var_f27 = i_attr().mTiltSpringFactor * -(var_f23_debug);
    var_f26 = -field_0x8d0 * i_attr().mTiltViscoscityResistance;
    var_f25 = -field_0x8d4 * i_attr().mTiltViscoscityResistance;

    field_0x8d0 += var_f28 + var_f26;
    field_0x8d4 += var_f27 + var_f25;
    field_0x8c8 += field_0x8d0;
    field_0x8cc += field_0x8d4;

    if (var_r21) {
        field_0x8c8 = 0.0f;
    }

    if (var_r22) {
        field_0x8cc = 0.0f;
    }

    field_0x8c0 = 0.0f;
    field_0x8c4 = 0.0f;
}

int daObjMovebox::Act_c::check_to_walk() {
    int var_r27 = -1;
    bool var_r26 = true;

    if (field_0x8fe != 0 &&
        cLib_checkBit<daObjMovebox::Bgc_c::State_e>(mBgc.mState, daObjMovebox::Bgc_c::STATE_1_e) &&
        !cLib_checkBit<daObjMovebox::Bgc_c::State_e>(mBgc.mState, daObjMovebox::Bgc_c::STATE_40_e))
    {
        dBgW::PushPullLabel sp28 = cLib_checkBit<dBgW::PushPullLabel>(mPPLabel, dBgW::PPLABEL_PULL);
        dBgW::PushPullLabel var_r25 = cLib_checkBit<dBgW::PushPullLabel>(mPPLabel, dBgW::PPLABEL_4);

        s16 var_r28;
        if (sp28) {
            if (var_r25) {
                var_r28 = i_attr().mFirstPullStopTime;
            } else {
                var_r28 = i_attr().mRepeatPullStopTime;
            }
        } else {
            if (var_r25) {
                var_r28 = i_attr().mFirstPushStopTime;
            } else {
                var_r28 = i_attr().mRepeatPushStopTime;
            }
        }

        for (int i = 0; i < 4; i++) {
            if (field_0x8f0[i] >= var_r28) {
                const BgcSrc_c* pbgc = i_attr().field_0x9e ? mBgc.M_lin20 : mBgc.M_lin5;
                int check_num = i_attr().field_0x9e ? 21 : 5;

                if (!mBgc.chk_wall_pre(this, pbgc, check_num, M_dir_base[i])) {
                    var_r27 = i;
                }
            } else {
                var_r26 = false;
            }
        }
    }

    if (var_r26) {
        clr_moment_cnt();
    }

    field_0x8fe = 0;
    return var_r27;
}

void daObjMovebox::Act_c::clr_moment_cnt() {
    for (int i = 0; i < 4; i++) {
        field_0x8f0[i] = 0;
    }
}

bool daObjMovebox::Act_c::chk_appear() const {
    int path_id = prmZ_get_pathId();
    int sw1 = prm_get_swSave1();

    if (path_id != 0xFF || sw1 == 0xFF) {
        return true;
    }

    BOOL is_sw1 = is_switch1();
    BOOL temp_r29 = prm_get_dmy();

    return (!is_sw1 && !temp_r29) || (is_sw1 && temp_r29);
}

void daObjMovebox::Act_c::eff_smoke_slip_start() {
    static cXyz scl(0.6f, 0.6f, 0.6f);

    s16 var_r30 = cLib_checkBit<dBgW::PushPullLabel>(mPPLabel, dBgW::PPLABEL_PULL) != 0 ?
                      i_attr().mPushMoveTime :
                      i_attr().mPullMoveTime;
}

void daObjMovebox::Act_c::mode_wait_init() {
    fopAcM_SetSpeedF(this, 0.0f);
    fopAcM_SetGravity(this, i_attr().mGravity);
    mpBgW->SetCrrFunc(dBgS_MoveBGProc_Trans);
    clr_moment_cnt();
    field_0x8e8 = -1;
    field_0x5ac = 0;
}

void daObjMovebox::Act_c::mode_wait() {
    if (field_0x8fa > 0) {
        field_0x8fa--;
    }

    int var_r30 = check_to_walk();
    if (field_0x902 == 0 &&
        cLib_checkBit<daObjMovebox::Bgc_c::State_e>(mBgc.mState, daObjMovebox::Bgc_c::STATE_1_e))
    {
        field_0x902 = 1;
        path_save();
    }

    daObj::posMoveF_stream(this, NULL, &cXyz::Zero, i_attr().mViscosityResistance,
                           i_attr().mInertiaResistance);

    cXyz sp48;
    mDoMtx_stack_c::transS(home.pos);
    mDoMtx_stack_c::YrotM(home.angle.y);
    mDoMtx_stack_c::transM(field_0x8dc * i_attr().mUnitDistance, 0.0f,
                           field_0x8e0 * i_attr().mUnitDistance);
    mDoMtx_stack_c::multVecZero(&sp48);

    current.pos.x = sp48.x;
    current.pos.z = sp48.z;

    if (var_r30 != -1) {
        field_0x8e8 = var_r30;
        eff_smoke_slip_start();
        ((daPy_py_c*)dComIfGp_getPlayer(0))->onPushPullKeep();

        mode_walk_init();

        if (cLib_checkBit<dBgW::PushPullLabel>(mPPLabel, dBgW::PPLABEL_PULL)) {
            field_0x8f8 = i_attr().mPullMoveTime;
            field_0x8e4 = 32768.0f / i_attr().mPullMoveTime;
        } else {
            field_0x8f8 = i_attr().mPushMoveTime;
            field_0x8e4 = 32768.0f / i_attr().mPushMoveTime;
        }
    }
}

void daObjMovebox::Act_c::mode_walk_init() {
    fopAcM_SetSpeedF(this, 0.0f);
    field_0x902 = 0;
    field_0x5ac = 1;
}

void daObjMovebox::Act_c::mode_walk() {
    static cXyz dir_vec[] = {
        cXyz(0.0f, 0.0f, 1.0f),
        cXyz(1.0f, 0.0f, 0.0f),
        cXyz(0.0f, 0.0f, -1.0f),
        cXyz(-1.0f, 0.0f, 0.0f),
    };

    bool var_r28 = --field_0x8f8 <= 0;
    f32 var_f31 = (cM_scos(field_0x8f8 * field_0x8e4) + 1.0f) * 0.5f;

    cXyz sp78;
    mDoMtx_stack_c::transS(home.pos);
    mDoMtx_stack_c::YrotM(home.angle.y);
    mDoMtx_stack_c::transM(
        (field_0x8dc + var_f31 * dir_vec[field_0x8e8].x) * i_attr().mUnitDistance, 0.0f,
        (field_0x8e0 + var_f31 * dir_vec[field_0x8e8].z) * i_attr().mUnitDistance);
    mDoMtx_stack_c::multVecZero(&sp78);

    current.pos.x = sp78.x;
    current.pos.z = sp78.z;
    eyePos = current.pos;

    sound_slip();

    if (var_r28) {
        const BgcSrc_c* pbgc = i_attr().field_0x9e ? mBgc.M_lin20 : mBgc.M_lin5;
        int check_num = i_attr().field_0x9e ? 21 : 5;

        if (mBgc.chk_wall_pre(this, pbgc, check_num, M_dir_base[field_0x8e8])) {
            sound_limit();
        }
    }

    daObj::posMoveF_stream(this, NULL, &cXyz::Zero, i_attr().mViscosityResistance,
                           i_attr().mInertiaResistance);

    current.pos.x = sp78.x;
    current.pos.z = sp78.z;

    if (var_r28) {
        if (field_0x8e8 == 0) {
            field_0x8e0++;
        } else if (field_0x8e8 == 1) {
            field_0x8dc++;
        } else if (field_0x8e8 == 2) {
            field_0x8e0--;
        } else if (field_0x8e8 == 3) {
            field_0x8dc--;
        }

        ((daPy_py_c*)dComIfGp_getPlayer(0))->offPushPullKeep();
        mode_wait_init();
    }
}

void daObjMovebox::Act_c::mode_afl_init() {
    fopAcM_SetSpeedF(this, 0.0f);
    mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
    field_0x5ac = 2;
}

void daObjMovebox::Act_c::mode_afl() {
    f32 var_f31;
    f32 var_f30 = current.pos.y - mBgc.field_0x60;
    if (var_f30 >= 0.0f) {
        var_f31 = 0.0f;
    } else if (var_f30 <= -i_attr().field_0x68) {
        var_f31 = 1.0f;
    } else {
        var_f31 = -var_f30 * i_attr().field_0x6c;
    }

    field_0x8b8 += (s16)(i_attr().mWaterOscillationAngleSpeed * (cM_rnd() + 1.0f));
    fopAcM_SetGravity(this, var_f31 * i_attr().mBuoyancy + i_attr().mGravity +
                      i_attr().mWaterOscillationAccel * cM_ssin(field_0x8b8) + field_0x8bc);
    field_0x8bc = 0.0f;

    if (!cLib_checkBit<daObjMovebox::Bgc_c::State_e>(mBgc.mState, daObjMovebox::Bgc_c::STATE_1_e)) {
        afl_sway();
    }

    f32 tmp = 1.0f - var_f31;
    f32 viscosity_resistance = var_f31 * i_attr().mWaterViscoscityResistance
                             + tmp * i_attr().mViscosityResistance;
    f32 inertia_resistance = var_f31 * i_attr().mWaterInertiaResistance
                             + tmp * i_attr().mInertiaResistance;

    field_0x8d8 = mBgc.field_0x60 - current.pos.y;
    if (field_0x8d8 < 0.0f) {
        field_0x8d8 = 0.0f;
    } else if (field_0x8d8 > i_attr().field_0x68) {
        field_0x8d8 = i_attr().field_0x68;
    }

    daObj::posMoveF_stream(this, NULL, &cXyz::Zero, viscosity_resistance, inertia_resistance);
}

static void dummy() {
    delete (dBgS_ObjGndChk*)NULL;
}

void daObjMovebox::Act_c::mode_proc_call() {
    typedef void (daObjMovebox::Act_c::*modeFunc)();
    static modeFunc mode_proc[] = {
        &daObjMovebox::Act_c::mode_wait,
        &daObjMovebox::Act_c::mode_walk,
        &daObjMovebox::Act_c::mode_afl,
    };

    (this->*mode_proc[field_0x5ac])();
}

void daObjMovebox::Act_c::sound_slip() {
    u32 var_r29 = 0;
    if (mBgc.field_0x5c >= 0) {
        const dBgS_ObjGndChk& chk = mBgc.M_gnd_work[mBgc.field_0x5c];
        int bg_index = chk.GetBgIndex();
        if (bg_index >= 0 && bg_index < 0x100) {
            var_r29 = dComIfG_Bgsp().GetMtrlSndId(chk);
        }
    }

    mDoAud_seStartLevel(i_attr().field_0x7c, &eyePos, var_r29, mReverb);
}

void daObjMovebox::Act_c::sound_limit() {
    u32 var_r29 = 0;
    if (mBgc.field_0x178 >= 0) {
        const dBgS_ObjLinChk& chk = mBgc.M_wall_work[mBgc.field_0x178];
        int bg_index = chk.GetBgIndex();
        if (bg_index >= 0 && bg_index < 0x100) {
            var_r29 = dComIfG_Bgsp().GetMtrlSndId(chk);
        }
    }

    mDoAud_seStart(i_attr().field_0x80, &eyePos, var_r29, mReverb);
}

void daObjMovebox::Act_c::sound_land() {
    u32 var_r29 = 0;
    if (mBgc.field_0x5c >= 0) {
        const dBgS_ObjGndChk& chk = mBgc.M_gnd_work[mBgc.field_0x5c];
        int bg_index = chk.GetBgIndex();
        if (bg_index >= 0 && bg_index < 0x100) {
            var_r29 = dComIfG_Bgsp().GetMtrlSndId(chk);
        }
    }

    mDoAud_seStart(i_attr().field_0x84, &eyePos, var_r29, mReverb);
}

void daObjMovebox::Act_c::vib_land() {
    dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
}

f32 dummy_literal(f32 x) {
    return x + 75.0f + 2.0f;
}

void daObjMovebox::Act_c::eff_land_smoke() {
    daObjEff::Act_c::make_land_smoke(&current.pos, i_attr().mLandSmokeScale);
}

void dummy_static() {
    static cXyz particle_scale;
}

AUDIO_INSTANCES;

int daObjMovebox::Act_c::Execute(Mtx** param_0) {
    if (field_0x908 != 0) {
        if (--field_0x908 == 0) {
            fopAcM_delete(this);
        }

        return 1;
    }

    if (mCcCyl.ChkTgHit()) {
        dCcD_GObjInf* hit_obj = mCcCyl.GetTgHitGObj();

        if (mCcCyl.ChkTgHit() && hit_obj != NULL) {
            if (!hit_obj->ChkAtType(AT_TYPE_LANTERN_SWING) && hit_obj->GetAtMtrl() == dCcD_MTRL_FIRE) {
                u32 params;
                daObjBurnBox_c::make_prm_burnBox(&params, 1);

                fopAcM_createChild(PROC_Obj_BurnBox, fopAcM_GetID(this), params, &current.pos,
                                   fopAcM_GetRoomNo(this), &shape_angle, &scale, -1, NULL);
                field_0x908 = 5;
            } else if (hit_obj->ChkAtType(AT_TYPE_BOMB) || hit_obj->ChkAtType(AT_TYPE_IRON_BALL)) {
                fopAc_ac_c* hit_ac = mCcCyl.GetTgHitAc();
                if (hit_ac != NULL && fopAcM_GetProfName(hit_ac) != PROC_Obj_Carry) {
                    cXyz sp48(current.pos);
                    J3DModelData* kibako_bmd =
                        (J3DModelData*)dComIfG_getObjectRes("Always", "BreakWoodBox.bmd");
                    JUT_ASSERT(0, kibako_bmd != NULL);

                    JPABaseEmitter* emitter = dComIfGp_particle_set(
                        0x82AF, &sp48, NULL, NULL, 0xFF, &dPa_modelEcallBack::getEcallback(),
                        fopAcM_GetRoomNo(this), NULL, NULL, &scale);
                    dPa_modelEcallBack::setModel(emitter, kibako_bmd, tevStr, 3, NULL, 0, 0);

                    for (int i = 0; i < 3; i++) {
                        static const u16 particle_id[] = {0x82AB, 0x82AC, 0x82AD};
                        dComIfGp_particle_set(particle_id[i], &sp48, NULL, &scale, 0xFF, NULL, -1,
                                              NULL, NULL, NULL);
                    }

                    fopAcM_delete(this);
                }
            }
        }
    }

    if (field_0x908 == 0) {
        if (cLib_checkBit<daObjMovebox::Bgc_c::State_e>(mBgc.mState,
                                                        daObjMovebox::Bgc_c::STATE_20_e))
        {
            mDoAud_seStart(i_attr().field_0x88, &eyePos, 0, mReverb);
        }

        if (field_0x5ac == 0) {
            if (cLib_checkBit<daObjMovebox::Bgc_c::State_e>(mBgc.mState,
                                                            daObjMovebox::Bgc_c::STATE_10_e) &&
                field_0x8fa == 0)
            {
                if (cLib_checkBit<daObjMovebox::Bgc_c::State_e>(mBgc.mState,
                                                                daObjMovebox::Bgc_c::STATE_1_e))
                {
                    if (i_attr().mBuoyancy + i_attr().mGravity > 0.0f) {
                        mode_afl_init();
                    }
                } else {
                    mode_afl_init();
                }
            }
        } else if (field_0x5ac == 2 && cLib_checkBit<daObjMovebox::Bgc_c::State_e>(
                                           mBgc.mState, daObjMovebox::Bgc_c::STATE_1_e))
        {
            mode_wait_init();
            field_0x8fa = 20;
        }

        mode_proc_call();

        mBgc.proc_vertical(this);
        if (mBgc.field_0x5c >= 0) {
            tevStr.room_no = current.roomNo;
            tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mBgc.M_gnd_work[mBgc.field_0x5c]);
        }

        if (field_0x8fc > 0) {
            field_0x8fc--;
        } else if (cLib_checkBit<daObjMovebox::Bgc_c::State_e>(mBgc.mState,
                                                               daObjMovebox::Bgc_c::STATE_4_e))
        {
            sound_land();
            vib_land();

            if (!cLib_checkBit<daObjMovebox::Bgc_c::State_e>(mBgc.mState,
                                                             daObjMovebox::Bgc_c::STATE_10_e))
            {
                eff_land_smoke();
            }
        }

        set_mtx();

        if (mType == 0) {
            mCcCyl.MoveCTg(current.pos);
            dComIfG_Ccsp()->Set(&mCcCyl);
        }
    }

    *param_0 = &mBgMtx;
    return 1;
}

int daObjMovebox::Act_c::Draw() {
    if (mpModel != NULL) {
        g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_setList();
    }

    if (i_attr().field_0x9d == 0 && mBgc.field_0x5c >= 0) {
        f32 var_f31 = mBgc.field_0x0[mBgc.field_0x5c];
        cM3dGPla sp40;

        if (dComIfG_Bgsp().GetTriPla(mBgc.M_gnd_work[mBgc.field_0x5c], &sp40) &&
            var_f31 != -G_CM3D_F_INF)
        {
            dComIfGd_setSimpleShadow(&current.pos, var_f31, i_attr().mShadowSize, &sp40.mNormal,
                                     shape_angle.y, -0.4f, NULL);
        }
    }

    return 1;
}

int daObjMovebox::Act_c::Delete() {
    if (mpBgW != NULL) {
        mpBgW->SetRideCallback(NULL);
        mpBgW->SetPushPullCallback(NULL);
    }

    return 1;
}

int daObjMovebox::Act_c::Mthd_Create() {
    fopAcM_ct(this, daObjMovebox::Act_c);
    mType = prm_get_type();

    prmX_init();
    prmZ_init();

    if (!chk_appear()) {
        return cPhs_ERROR_e;
    }

    int phase_state = dComIfG_resLoad(&mPhase, M_arcname[mType]);
    if (phase_state == cPhs_COMPLEATE_e) {
        u32 heap_size = i_attr().field_0x60;
        path_init();

        phase_state = MoveBGCreate(M_arcname[mType], i_attr().field_0x5c, dBgS_MoveBGProc_Trans,
                                   heap_size, NULL);
        JUT_ASSERT(0, (phase_state == cPhs_COMPLEATE_e) || (phase_state == cPhs_ERROR_e));
    }

    return phase_state;
}

int daObjMovebox::Act_c::Mthd_Delete() {
    int rt = MoveBGDelete();
    dComIfG_resDelete(&mPhase, M_arcname[mType]);
    return rt;
}

namespace daObjMovebox {
namespace {
int Mthd_Create(void* i_this) {
    return ((daObjMovebox::Act_c*)i_this)->Mthd_Create();
}

int Mthd_Delete(void* i_this) {
    return ((daObjMovebox::Act_c*)i_this)->Mthd_Delete();
}

int Mthd_Execute(void* i_this) {
    return ((daObjMovebox::Act_c*)i_this)->MoveBGExecute();
}

int Mthd_Draw(void* i_this) {
    return ((daObjMovebox::Act_c*)i_this)->MoveBGDraw();
}

int Mthd_IsDelete(void* i_this) {
    return ((daObjMovebox::Act_c*)i_this)->MoveBGIsDelete();
}

static actor_method_class Mthd_Table = {
    (process_method_func)Mthd_Create,  (process_method_func)Mthd_Delete,
    (process_method_func)Mthd_Execute, (process_method_func)Mthd_IsDelete,
    (process_method_func)Mthd_Draw,
};
};  // namespace
};  // namespace daObjMovebox

extern actor_process_profile_definition g_profile_Obj_Movebox = {
    fpcLy_CURRENT_e,              // mLayerID
    3,                            // mListID
    fpcPi_CURRENT_e,              // mListPrio
    PROC_Obj_Movebox,             // mProcName
    &g_fpcLf_Method.base,        // sub_method
    sizeof(daObjMovebox::Act_c),  // mSize
    0,                            // mSizeOther
    0,                            // mParameters
    &g_fopAc_Method.base,         // sub_method
    17,                           // mPriority
    &daObjMovebox::Mthd_Table,    // sub_method
    0x00040504,                   // mStatus
    fopAc_ACTOR_e,                // mActorType
    fopAc_CULLBOX_CUSTOM_e,       // cullType
};
