/**
 * d_bg_s_acch.cpp
 * BG collision Actor Check
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/actor/d_a_player.h"
#include "d/d_bg_s.h"
#include "d/d_bg_s_acch.h"
#include "d/d_com_inf_game.h"
#include "global.h"

#if DEBUG
#include "d/d_debug_viewer.h"
#endif

dBgS_AcchCir::dBgS_AcchCir() {
    m_flags = 0;
    m_wall_rr = 0.0f;
    field_0x2c = 0.0f;
    m_wall_h = 0.0f;
    m_wall_r = 0.0f;
    m_wall_angle_y = 0;
    m_wall_h_direct = 0.0f;
}

void dBgS_AcchCir::ClrWallHit() {
    m_flags &= ~FLAG_WALL_HIT;
    ClearPi();
}

void dBgS_AcchCir::SetWallR(f32 wall_r) {
    m_wall_r = wall_r;
}

void dBgS_AcchCir::CalcWallRR() {
    m_wall_rr = m_wall_r * m_wall_r;
}

void dBgS_AcchCir::SetWall(f32 wall_h, f32 wall_r) {
    SetWallH(wall_h);
    SetWallR(wall_r);
}

void dBgS_AcchCir::SetWallHDirect(f32 h_direct) {
    m_flags |= WALL_H_DIRECT;
    m_wall_h_direct = h_direct;
}

dBgS_Acch::~dBgS_Acch() {}

dBgS_Acch::dBgS_Acch() {
    SetPolyPassChk(GetPolyPassChkInfo());
    SetGrpPassChk(GetGrpPassChkInfo());

    m_flags = 0;
    SetRoofNone();

    pm_pos = NULL;
    pm_old_pos = NULL;
    pm_speed = NULL;
    m_ground_h = -G_CM3D_F_INF;
    field_0x90 = 0.0f;
    m_gnd_chk_offset = 60.0f;
    field_0x94 = 0.0f;
    field_0xb4 = 0;
    field_0xb8 = 0.0f;
    field_0xbc = 0.0f;

    m_tbl_size = 0;
    pm_acch_cir = NULL;

    m_roof_height = G_CM3D_F_INF;
    m_roof_crr_height = 0.0f;
    field_0xcc = 0.0f;

    m_wtr_mode = 1;
    m_wtr_chk_offset = 1000.0f;

    pm_angle = NULL;
    pm_shape_angle = NULL;
    m_my_ac = NULL;
    pm_out_poly_info = NULL;
    field_0xd8 = -G_CM3D_F_INF;
}

void dBgS_Acch::Init() {
    ClrWallHit();
    for (int i = 0; i < m_tbl_size; i++) {
        pm_acch_cir[i].ClrWallHit();
        pm_acch_cir[i].ClrWallHDirect();
    }
}

void dBgS_Acch::Set(cXyz* ppos, cXyz* pold_pos, fopAc_ac_c* pactor, int tbl_size,
                    dBgS_AcchCir* pacchcir, cXyz* pspeed, csXyz* pangle, csXyz* pshape_angle) {
    pm_pos = ppos;
    pm_old_pos = pold_pos;

    JUT_ASSERT(246, pm_pos != NULL);
    JUT_ASSERT(247, pm_old_pos != NULL);

    m_my_ac = pactor;

    fpc_ProcID id = fopAcM_GetID(pactor);
    SetActorPid(id);

    pm_speed = pspeed;
    m_tbl_size = tbl_size;
    pm_acch_cir = pacchcir;
    pm_angle = pangle;
    pm_shape_angle = pshape_angle;
}

void dBgS_Acch::Set(fopAc_ac_c* pactor, int tbl_size, dBgS_AcchCir* pacchcir) {
    m_tbl_size = tbl_size;
    pm_acch_cir = pacchcir;
    m_my_ac = pactor;

    SetActorPid(fopAcM_GetID(pactor));
    pm_pos = fopAcM_GetPosition_p(pactor);
    pm_old_pos = fopAcM_GetOldPosition_p(pactor);
    pm_speed = fopAcM_GetSpeed_p(pactor);
    pm_angle = fopAcM_GetAngle_p(pactor);
    pm_shape_angle = fopAcM_GetShapeAngle_p(pactor);
}

void dBgS_Acch::GroundCheckInit(dBgS& bgs) {
    UNUSED(bgs);

    if (!(m_flags & FLAG_GRND_NONE)) {
        m_ground_h = -G_CM3D_F_INF;
        m_gnd.SetExtChk(*this);
        field_0xb4 = ChkGroundHit();
        ClrGroundHit();
        ClrGroundLanding();
        ClrGroundAway();
    }
}

void dBgS_Acch::GroundCheck(dBgS& bgs) {
    if (!(m_flags & FLAG_GRND_NONE)) {
        cXyz grnd_pos;
        grnd_pos = *pm_pos;
        grnd_pos.y += field_0x94 + (m_gnd_chk_offset - field_0x90);

        if (!ChkGndThinCellingOff()) {
            static dBgS_RoofChk tmpRoofChk;
            tmpRoofChk.SetActorPid(m_gnd.GetActorPid());
            tmpRoofChk.SetPos(*pm_pos);

            f32 roof_y = bgs.RoofChk(&tmpRoofChk);
            if (grnd_pos.y > roof_y) {
                grnd_pos.y = roof_y;
            }
        }

        field_0x94 = 0.0f;
        m_gnd.SetPos(&grnd_pos);
        m_ground_h = bgs.GroundCross(&m_gnd);

        if (m_ground_h != -G_CM3D_F_INF) {
            field_0xbc = m_ground_h + field_0x90;
            if (field_0xbc > field_0xb8) {
                pm_pos->y = field_0xbc;

                if (ChkClrSpeedY() && pm_speed != NULL) {
                    pm_speed->y = 0.0f;
                }

                bgs.GetTriPla(m_gnd, &field_0xa0);
                SetGroundFind();
                SetGroundHit();

                if (field_0xc0 == 0) {
                    field_0xc0 = 1;
                    bgs.RideCallBack(m_gnd, m_my_ac);
                }

                if (field_0xb4 == 0) {
                    SetGroundLanding();
                }
            }
        }

        if (field_0xb4 && !ChkGroundHit()) {
            SetGroundAway();
        }
    }
}

void dBgS_Acch::GroundRoofProc(dBgS& bgs) {
    if (m_ground_h != -G_CM3D_F_INF) {
        if (field_0xbc < field_0xcc && field_0xcc < pm_pos->y) {
            pm_pos->y = field_0xcc;
        }

        if (!(m_flags & FLAG_ROOF_NONE) && m_ground_h >= m_roof_height) {
            m_roof.SetExtChk(*this);
            ClrRoofHit();
            cXyz roof_pos(*pm_pos);
            m_roof.SetPos(roof_pos);
            m_roof_height = bgs.RoofChk(&m_roof);
        }
    }
}

void dBgS_Acch::LineCheck(dBgS& bgs) {
    dBgS_RoofChk roof_chk;
    roof_chk.SetActorPid(m_gnd.GetActorPid());
    roof_chk.SetPos(*GetOldPos());

    f32 roof_y = dComIfG_Bgsp().RoofChk(&roof_chk);

    f32 var_f30 = G_CM3D_F_INF;
    bool var_r30 = false;
    bool var_r29 = false;

    for (int i = 0; i < GetTblSize(); i++) {
        cBgS_LinChk lin_chk;
        cXyz old_pos;
        cXyz pos;

        old_pos = *GetOldPos();
        pos = *GetPos();

        f32 wall_h = GetWallH(i);
        if (roof_y < old_pos.y + wall_h) {
            if (var_r29) {
                continue;
            } else {
                wall_h = (roof_y - old_pos.y) - 1.0f;
                var_r29 = true;
            }
        }

        if (var_f30 > wall_h) {
            var_f30 = wall_h;
        }

        old_pos.y += wall_h;
        pos.y += wall_h;

        if (GetSpeedY() < 0.0f) {
            if (m_my_ac != NULL && fopAcM_GetName(m_my_ac) == fpcNm_ALINK_e &&
                !static_cast<daPy_py_c*>(m_my_ac)->checkPlayerFly() && ChkLink())
            {
                pos.y -= GetSpeedY();
            }
        }

        lin_chk.Set2(&old_pos, &pos, GetActorPid());
        lin_chk.SetExtChk(*this);

        if (bgs.LineCross(&lin_chk)) {
            *GetPos() = lin_chk.GetCross();
            OnLineCheckHit();

            if (pm_out_poly_info != NULL)
                *pm_out_poly_info = lin_chk;

            cM3dGPla plane;
            bgs.GetTriPla(lin_chk, &plane);
            if (!cBgW_CheckBGround(plane.mNormal.y)) {
                PSVECAdd(GetPos(), &plane.mNormal, GetPos());
                f32 var_f28 = JMAFastSqrt(SQUARE(plane.mNormal.x) + SQUARE(plane.mNormal.z));
                if (!cM3d_IsZero(var_f28)) {
                    pm_acch_cir[i].SetWallHDirect(GetPos()->y);
                }

                GetPos()->y -= GetWallH(i);
            } else {
                GetPos()->y -= 1.0f;
                GroundCheck(bgs);
                var_r30 = true;
            }
        }
    }

    if (ChkLineDown()) {
        f32 temp_f1 = GetOldPos()->y - GetPos()->y;
        if (var_f30 != G_CM3D_F_INF && !var_r30 && !cM3d_IsZero(temp_f1)) {
            cBgS_LinChk lin_chk;
            cXyz old_pos;
            cXyz pos;

            old_pos = *GetPos();
            pos = old_pos;

            old_pos.y += var_f30;

            lin_chk.Set2(&old_pos, &pos, GetActorPid());
            lin_chk.SetExtChk(*this);

            if (bgs.LineCross(&lin_chk)) {
                *GetPos() = lin_chk.GetCross();
                OnLineCheckHit();

                if (pm_out_poly_info != NULL)
                    *pm_out_poly_info = lin_chk;

                GetPos()->y -= 1.0f;
                GroundCheck(bgs);
            }
        }
    }
}

void dBgS_Acch::CrrPos(dBgS& bgs) {
    bool bvar9;
    if (!(m_flags & 1)) {
        JUT_ASSERT(792, pm_pos != NULL);
        JUT_ASSERT(793, pm_old_pos != NULL);

        JUT_ASSERT(833, !isnan(pm_pos->x));
        JUT_ASSERT(834, !isnan(pm_pos->y));
        JUT_ASSERT(835, !isnan(pm_pos->z));

        JUT_ASSERT(837, -1.0e32f < pm_pos->x && pm_pos->x < 1.0e32f);
        JUT_ASSERT(838, -1.0e32f < pm_pos->y);
        JUT_ASSERT(839, pm_pos->y < 1.0e32f);
        JUT_ASSERT(840, -1.0e32f < pm_pos->z && pm_pos->z < 1.0e32f);

        bgs.MoveBgCrrPos(m_gnd, ChkGroundHit(), pm_pos, pm_angle, pm_shape_angle, false, false);

        if (!ChkGroundHit()) {
            bvar9 = false;

            if (ChkWallHit()) {
                for (int i = 0; i < m_tbl_size; i++) {
                    if (pm_acch_cir[i].ChkWallHit()) {
                        bgs.MoveBgCrrPos(pm_acch_cir[i], true, pm_pos, pm_angle, pm_shape_angle,
                                           true, false);
                        bvar9 = true;
                        break;
                    }
                }
            }

            if (!bvar9 && ChkRoofHit()) {
                bgs.MoveBgCrrPos(m_roof, true, pm_pos, pm_angle, pm_shape_angle, false, true);
            }
        }

        GroundCheckInit(bgs);
        Init();

        f32 wall_all_low_h_r = GetWallAllLowH_R();
        f32 old_pos_dist2XZ = GetOldPos()->abs2XZ(*GetPos());
        f32 old_pos_dist_y = GetOldPos()->y - GetPos()->y;
        f32 wall_all_low_h = GetWallAllLowH();

        field_0xb8 = GetPos()->y;
        field_0xc0 = 0;
        f32 fvar12 = wall_all_low_h + GetOldPos()->y;
        f32 fvar1 = m_gnd_chk_offset + GetPos()->y;
        bool bvar2 = false;

        OffLineCheckHit();
        if (!ChkLineCheckNone() && !cM3d_IsZero(wall_all_low_h_r) &&
            (old_pos_dist2XZ > SQUARE(wall_all_low_h_r) || fvar12 > fvar1 || old_pos_dist_y > m_gnd_chk_offset || ChkLineCheck()))
        {
            bvar2 = true;
            LineCheck(bgs);
        }

        if (!(m_flags & FLAG_WALL_NONE)) {
            if (ChkWallSort()) {
                bgs.WallCorrectSort(this);
            } else {
                bgs.WallCorrect(this);
            }
        }

        if (ChkWallHit() && bvar2) {
            LineCheck(bgs);
        }

        field_0xcc = G_CM3D_F_INF;

        if (!(m_flags & FLAG_ROOF_NONE)) {
            m_roof.SetExtChk(*this);
            ClrRoofHit();

            cXyz roof_pos(*pm_pos);
            m_roof.SetPos(roof_pos);
            m_roof_height = bgs.RoofChk(&m_roof);

            if (m_roof_height != G_CM3D_F_INF) {
                if (pm_pos->y + m_roof_crr_height > m_roof_height) {
                    field_0xcc = m_roof_height - m_roof_crr_height;
                    SetRoofHit();
                }
            }
        }

        if (!(m_flags & FLAG_GRND_NONE)) {
            ClrGroundFind();
            GroundCheck(bgs);
            GroundRoofProc(bgs);
        } else {
            if (field_0xcc < pm_pos->y) {
                pm_pos->y = field_0xcc;
            }
        }

        if (!(m_flags & FLAG_WATER_NONE)) {
            ClrWaterHit();
            ClrWaterIn();
            m_wtr.SetHeight(-G_CM3D_F_INF);

            bool spD = false;

            f32 var_f29;
            f32 top;
            if (m_ground_h == -G_CM3D_F_INF) {
                var_f29 = pm_pos->y - 50.0f;
                spD = true;
            } else {
                var_f29 = m_ground_h;
            }

            if (m_wtr_mode == 1) {
                top = var_f29 + m_wtr_chk_offset;
            } else {
                dBgS_RoofChk roof_chk;
                roof_chk.SetUnderwaterRoof();
                roof_chk.SetPos(*pm_pos);

                top = bgs.RoofChk(&roof_chk);
                if (top == G_CM3D_F_INF) {
                    top = pm_pos->y + 1000000.0f;
                }
            }

            cXyz ground;
            ground = *pm_pos;
            ground.y = var_f29;

            m_wtr.Set(ground, top);
            m_wtr.SetPassChkInfo(*this);

            if (bgs.WaterChk(&m_wtr)) {
                SetWaterHit();
                if (m_wtr.GetHeight() > pm_pos->y) {
                    SetWaterIn();
                }

                JUT_ASSERT(1095, m_wtr.GetHeight() >= ground.y);
                JUT_ASSERT(1096, m_wtr.GetHeight() <= top);
            }
        }

        JUT_ASSERT(1124, !isnan(pm_pos->x));
        JUT_ASSERT(1125, !isnan(pm_pos->y));
        JUT_ASSERT(1126, !isnan(pm_pos->z));

        JUT_ASSERT(1128, -1.0e32f < pm_pos->x && pm_pos->x < 1.0e32f);
        JUT_ASSERT(1129, -1.0e32f < pm_pos->y && pm_pos->y < 1.0e32f);
        JUT_ASSERT(1130, -1.0e32f < pm_pos->z && pm_pos->z < 1.0e32f);
    }
}

f32 dBgS_Acch::GetWallAllR() {
    f32 rt = 0.0f;

    for (int i = 0; i < m_tbl_size; i++) {
        if (rt < pm_acch_cir[i].GetWallR()) {
            rt = pm_acch_cir[i].GetWallR();
        }
    }

    return rt;
}

void dBgS_Acch::SetWallCir() {
    for (int i = 0; i < m_tbl_size; i++) {
        pm_acch_cir[i].SetCir(*pm_pos);
    }
}

void dBgS_Acch::CalcWallBmdCyl() {
    if (m_tbl_size <= 0) {
        m_wall_cyl.Set(*pm_pos, 0.0f, 0.0f);
    } else {
        f32 wall_all_r = GetWallAllR();
        f32 wall_h_max = pm_acch_cir->GetWallH();
        f32 wall_h_min = pm_acch_cir->GetWallH();

        if (m_tbl_size >= 1) {
            for (int i = 0; i < m_tbl_size; i++) {
                if (wall_h_max > pm_acch_cir[i].GetWallH()) {
                    wall_h_max = pm_acch_cir[i].GetWallH();
                }

                if (wall_h_min < pm_acch_cir[i].GetWallH()) {
                    wall_h_min = pm_acch_cir[i].GetWallH();
                }
            }
        }

        cXyz xyz(*pm_pos);
        xyz.y += wall_h_max;

        m_wall_cyl.Set(xyz, wall_all_r, wall_h_min - wall_h_max);
    }
}

void dBgS_Acch::SetGroundUpY(f32 param_0) {
    field_0x94 = param_0 - field_0x90;
    field_0x90 = param_0;
}

f32 dBgS_Acch::GetWallAllLowH() {
    if (m_tbl_size <= 0) {
        return 0.0f;
    }

    f32 wall_low_h = pm_acch_cir->GetWallH();
    for (int i = 1; i < m_tbl_size; i++) {
        if (wall_low_h > pm_acch_cir[i].GetWallH()) {
            wall_low_h = pm_acch_cir[i].GetWallH();
        }
    }

    return wall_low_h;
}

f32 dBgS_Acch::GetWallAllLowH_R() {
    if (m_tbl_size <= 0) {
        return 0.0f;
    }

    int index = 0;
    f32 wall_h = pm_acch_cir->GetWallH();
    for (int i = 1; i < m_tbl_size; i++) {
        if (wall_h > pm_acch_cir[i].GetWallH()) {
            wall_h = pm_acch_cir[i].GetWallH();
            index = i;
        }
    }

    return pm_acch_cir[index].GetWallR();
}

f32 dBgS_Acch::GetSpeedY() {
    if (pm_speed != NULL) {
        return pm_speed->y;
    }

    return 0.0f;
}

#if DEBUG
static GXColor blue = {0, 0, 0xFF, 0x80};
static GXColor red = {0xFF, 0, 0, 0x80};

void dBgS_Acch::DrawWall(dBgS& bgs) {
    if (bgs.m_hio.ChkAcchDrawWall()) {
        if (!(m_flags & FLAG_WALL_NONE)) {
            for (int i = 0; i < m_tbl_size; i++) {
                cM3dGCir cir;
                cir = *pm_acch_cir[i].GetCirP();

                cXyz pos;
                pos.x = cir.GetCx();
                pos.y = cir.GetHeight();
                pos.z = cir.GetCy();

                f32 radius = cir.GetR();
                dDbVw_drawCylinderXlu(pos, radius, 3.0f, blue, TRUE);

                if (pm_acch_cir[i].ChkWallHit()) {
                    bgs.DrawPoly(pm_acch_cir[i], blue);
                }
            }
        } else {
            SetWallCir();

            for (int i = 0; i < m_tbl_size; i++) {
                cM3dGCir cir;
                cir = *pm_acch_cir[i].GetCirP();

                cXyz pos;
                pos.x = cir.GetCx();
                pos.y = cir.GetHeight();
                pos.z = cir.GetCy();

                f32 radius = cir.GetR();
                dDbVw_drawCylinderXlu(pos, radius, 3.0f, red, TRUE);
            }
        }
    }
}
#endif

f32 dBgS_Acch::GetWallAddY(Vec& param_0) {
    if (!ChkGroundFind() || field_0xa0.mNormal.y < 0.5f) {
        return 0.0f;
    }

    f32 tmp;
    if (field_0xa0.getCrossYLessD(param_0, &tmp)) {
        if (tmp > 0.0f) {
            tmp = 0.0f;
        }
        return -tmp;
    }

    return 0.0f;
}

void dBgS_Acch::SetNowActorInfo(int bg_index, void* param_1, fpc_ProcID proc_id) {
    m_bg_index = bg_index;
    field_0x7c = param_1;
    field_0x80 = proc_id;
}

void dBgS_Acch::SetWallPolyIndex(int index, int poly_index) {
    JUT_ASSERT(1534, index <= m_tbl_size);

    pm_acch_cir[index].SetActorInfo(m_bg_index, field_0x7c, field_0x80);
    pm_acch_cir[index].SetWallPolyIndex(poly_index);
}

void dBgS_Acch::CalcMovePosWork() {
    SetWallCir();
    SetLin();
    CalcWallBmdCyl();
}

void dBgS_Acch::CalcWallRR() {
    for (int i = 0; i < m_tbl_size; i++) {
        pm_acch_cir[i].CalcWallRR();
    }
}

void dBgS_Acch::SetMoveBGOnly() {
    m_flags |= FLAG_MOVE_BG_ONLY;
    m_wtr.OnMoveBGOnly();
}

void dBgS_Acch::ClrMoveBGOnly() {
    m_flags &= ~FLAG_MOVE_BG_ONLY;
    m_wtr.OffMoveBGOnly();
}

void dBgS_Acch::SetGndThinCellingOff() {
    m_flags |= FLAG_GND_THIN_CELLING_OFF;
}

void dBgS_Acch::ClrGndThinCellingOff() {
    m_flags &= ~FLAG_GND_THIN_CELLING_OFF;
}

bool dBgS_Acch::ChkGndThinCellingOff() {
    return m_flags & FLAG_GND_THIN_CELLING_OFF;
}

void dBgS_Acch::OnWallSort() {
    m_flags |= FLAG_WALL_SORT;
}

bool dBgS_Acch::ChkWallSort() {
    return m_flags & FLAG_WALL_SORT;
}

bool dBgS_Acch::ChkLineDown() {
    return m_flags & FLAG_LINE_DOWN;
}
