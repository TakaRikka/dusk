/**
 * d_bg_s.cpp
 * Background (Map) Collision main handler
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_s.h"
#include "d/d_bg_s_sph_chk.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

void cBgS_ChkElm::Init() {
    m_bgw_base_ptr = NULL;
    m_used = false;
    m_actor_ptr = NULL;
    m_actor_id = 0xFFFFFFFF;
}

void cBgS_ChkElm::Regist2(dBgW_Base* p_data, fpc_ProcID actor_id, void* p_actor) {
    m_used = true;
    m_bgw_base_ptr = p_data;
    m_actor_id = actor_id;
    m_actor_ptr = (fopAc_ac_c*)p_actor;
}

void cBgS_ChkElm::Release() {
    m_used = false;
    m_bgw_base_ptr = NULL;
    m_actor_id = 0xFFFFFFFF;
    m_actor_ptr = NULL;
}

static int l_SetCounter;

bool cBgS::Regist(dBgW_Base* p_data, fpc_ProcID actor_id, void* p_actor) {
    if (p_data == NULL) {
        return true;
    } else if (p_data->ChkUsed()) {
        return true;
    } else if (p_data->ChkMemoryError()) {
        return true;
    } else {
        int i = l_SetCounter;
        do {
            if (!m_chk_element[i].m_used) {
                m_chk_element[i].Regist2(p_data, actor_id, p_actor);
                p_data->Regist(i);

                l_SetCounter = i + 1;
                if (l_SetCounter >= 0x100) {
                    l_SetCounter = 0;
                }
                return false;
            }

            i++;
            if (i >= 0x100) {
                i = 0;
            }
        } while (l_SetCounter != i);
        p_data->Release();
    }
    return true;
}

bool cBgS::Release(dBgW_Base* p_data) {
    if (p_data == NULL) {
        return 1;
    }

    int id = p_data->GetId();
    if (p_data->ChkUsed() && id >= 0 && id < 0x100 && m_chk_element[id].ChkUsed()) {
        m_chk_element[id].Release();
        p_data->Release();
    } else {
        return 1;
    }

    return 0;
}

void cBgS::Ct() {
    l_SetCounter = 0;
    for (int i = 0; i < 0x100; i++) {
        m_chk_element[i].Init();
    }
}

void cBgS::Dt() {
    for (int i = 0; i < 0x100; i++) {
        if (m_chk_element[i].ChkUsed()) {
            m_chk_element[i].Release();
        }
    }

    for (int i = 0; i < 0x100; i++) {
        m_chk_element[i].Init();
    }
}

bool cBgS::LineCross(cBgS_LinChk* p_line) {
    p_line->ClearPi();
    p_line->ClrHit();
    p_line->PreCalc();

    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady() &&
            !p_line->ChkSameActorPid(elm->m_actor_id) && elm->m_bgw_base_ptr->LineCheck(p_line))
        {
            p_line->SetActorInfo(i, elm->m_bgw_base_ptr, elm->m_actor_id);
            p_line->SetHit();
        }
        elm++;
    }

    return p_line->ChkHit() ? true : false;
}

f32 cBgS::GroundCross(cBgS_GndChk* p_gnd) {
    p_gnd->SetNowY(-G_CM3D_F_INF);
    p_gnd->ClearPi();
    p_gnd->PreCheck();

    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady() &&
            !p_gnd->ChkSameActorPid(elm->m_actor_id) && elm->m_bgw_base_ptr->GroundCross(p_gnd))
        {
            p_gnd->SetActorInfo(i, elm->m_bgw_base_ptr, elm->m_actor_id);
        }
        elm++;
    }

    return p_gnd->GetNowY();
}

// this is identical to cBgD_t except using u32's for the table offsets.
// u32 is needed to match in ConvDzb ?
struct cBgD_t_ {
    // Vertex Info
    /* 0x00 */ int m_v_num;
    /* 0x04 */ u32 m_v_tbl;

    // Triangle Info
    /* 0x08 */ int m_t_num;
    /* 0x0C */ u32 m_t_tbl;

    // Spatial List Info
    /* 0x10 */ int m_b_num;
    /* 0x14 */ u32 m_b_tbl;

    // Face Group Data Info
    /* 0x18 */ int m_tree_num;
    /* 0x1C */ u32 m_tree_tbl;

    // String Group Info
    /* 0x20 */ int m_g_num;
    /* 0x24 */ u32 m_g_tbl;

    // Surface Property Info
    /* 0x28 */ int m_ti_num;
    /* 0x2C */ u32 m_ti_tbl;

    /* 0x30 */ u32 m_flags;
};  // Size: 0x34

struct cBgD_Grp_t_ {
    u32 strOffset;
    u8 data[0x30];
};

void* cBgS::ConvDzb(void* p_dzb) {
    cBgD_t_* pbgd = (cBgD_t_*)p_dzb;

    if (((pbgd->m_flags & 0x80000000) == 0)) {
        pbgd->m_flags |= 0x80000000;
    } else {
        return p_dzb;
    }

    if (pbgd->m_v_tbl != 0) {
        pbgd->m_v_tbl += (uintptr_t)p_dzb;
    }

    pbgd->m_t_tbl += (uintptr_t)p_dzb;
    pbgd->m_b_tbl += (uintptr_t)p_dzb;
    pbgd->m_tree_tbl += (uintptr_t)p_dzb;
    pbgd->m_g_tbl += (uintptr_t)p_dzb;
    pbgd->m_ti_tbl += (uintptr_t)p_dzb;

    for (int i = 0; i < pbgd->m_g_num; i++) {
        ((cBgD_Grp_t_*)pbgd->m_g_tbl)[i].strOffset =
            (uintptr_t)p_dzb + ((cBgD_Grp_t_*)pbgd->m_g_tbl)[i].strOffset;
    }

    return p_dzb;
}

fopAc_ac_c* cBgS::GetActorPointer(int actor_index) const {
    return m_chk_element[actor_index].m_actor_ptr;
}

dBgW_Base* cBgS::GetBgWBasePointer(cBgS_PolyInfo const& poly) const {
    int bg_index = poly.GetBgIndex();

    if (bg_index < 0 || bg_index >= 0x100 || !m_chk_element[bg_index].ChkUsed()) {
        return NULL;
    }
    return m_chk_element[bg_index].m_bgw_base_ptr;
}

bool cBgS::ChkPolySafe(cBgS_PolyInfo const& poly) {
    if (!poly.ChkSetInfo()) {
        return false;
    }

    int bg_index = poly.GetBgIndex();
    if (bg_index < 0 || bg_index >= 0x100) {
        return false;
    } else {
        if (m_chk_element[bg_index].ChkUsed()) {
            return poly.ChkSafe(m_chk_element[bg_index].m_bgw_base_ptr,
                                m_chk_element[bg_index].m_actor_id);
        } else {
            return false;
        }
    }
}

s32 cBgS::GetGrpRoomId(cBgS_PolyInfo const& poly) const {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetGrpRoomIndex(poly);
    } else {
        return 0xFF;
    }
}

bool cBgS::GetTriPla(cBgS_PolyInfo const& poly, cM3dGPla* p_pla) const {
    int bg_index = poly.GetBgIndex();
    if (bg_index < 0 || bg_index >= 0x100 || !m_chk_element[bg_index].ChkUsed()) {
        return false;
    }

    *p_pla = m_chk_element[bg_index].m_bgw_base_ptr->GetTriPla(poly);
    return true;
}

bool cBgS::GetTriPnt(cBgS_PolyInfo const& poly, cXyz* param_1, cXyz* param_2, cXyz* param_3) const {
    int id = poly.GetBgIndex();
    if (!m_chk_element[id].ChkUsed()) {
        return false;
    }

    return m_chk_element[id].m_bgw_base_ptr->GetTriPnt(poly, param_1, param_2, param_3);
}

void cBgS::ShdwDraw(cBgS_ShdwDraw* p_shdw) {
    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed()) {
            elm->m_bgw_base_ptr->ShdwDraw(p_shdw);
        }
        elm++;
    }
}

u32 cBgS::GetGrpInf(cBgS_PolyInfo const& poly) const {
    int id = poly.GetBgIndex();
    if (!m_chk_element[id].ChkUsed()) {
        return -1;
    }

    return m_chk_element[id].m_bgw_base_ptr->GetGrpInf(poly);
}

void dBgS::Ct() {
    cBgS::Ct();
}

void dBgS::Dt() {
    cBgS::Dt();
}

void dBgS::ClrMoveFlag() {
    for (int i = 0; i < 0x100; i++) {
        if (m_chk_element[i].ChkUsed()) {
            m_chk_element[i].m_bgw_base_ptr->OffMoveFlag();
        }
    }
}

void dBgS::Move() {
    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed()) {
            elm->m_bgw_base_ptr->CalcDiffShapeAngleY(elm->m_actor_ptr->shape_angle.y);
        }
        elm++;
    }
}

bool dBgS::Regist(dBgW_Base* pbgw, fopAc_ac_c* p_actor) {
    if (pbgw == NULL) {
        return true;
    }

    if (p_actor != NULL && pbgw->ChkMoveBg()) {
        pbgw->SetOldShapeAngleY(p_actor->shape_angle.y);
        pbgw->SetRoomId(fopAcM_GetRoomNo(p_actor));
    }

    return cBgS::Regist(pbgw, fopAcM_GetID(p_actor), p_actor);
}

void dBgW_Base::SetOldShapeAngleY(s16 shape_angle) {
    m_old_ShapeAngleY = shape_angle;
}

bool dBgS::ChkMoveBG(cBgS_PolyInfo const& poly) {
    dBgW_Base* base = dComIfG_Bgsp().GetBgWBasePointer(poly);
    if (base != NULL) {
        if (base->ChkLock()) {
            return false;
        }

        if (base->ChkMoveBg()) {
            return true;
        }
    }

    return false;
}

bool dBgS::ChkMoveBG_NoDABg(cBgS_PolyInfo const& poly) {
    dBgW_Base* base = dComIfG_Bgsp().GetBgWBasePointer(poly);
    if (base != NULL && base->ChkMoveBg()) {
        return true;
    }

    return false;
}

s32 dBgS::GetExitId(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetExitId(poly);
    } else {
        return 0x3F;
    }
}

s32 dBgS::GetPolyColor(cBgS_PolyInfo const& poly) {
    if (!poly.ChkSetInfo()) {
        return 0xFF;
    }

    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetPolyColor(poly);
    }

    return 0xFF;
}

BOOL dBgS::GetHorseNoEntry(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetHorseNoEntry(poly);
    }

    return false;
}

int dBgS::GetSpecialCode(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetSpecialCode(poly);
    }

    return 0;
}

int dBgS::GetMagnetCode(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetMagnetCode(poly);
    }

    return 0;
}

int dBgS::GetMonkeyBarsCode(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetMonkeyBarsCode(poly);
    }

    return 0;
}

u32 dBgS::GetUnderwaterRoofCode(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetUnderwaterRoofCode(poly.GetPolyIndex());
    }

    return 0;
}

s32 dBgS::GetWallCode(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetWallCode(poly);
    }

    return 0;
}

int dBgS::GetPolyAtt0(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetPolyAtt0(poly);
    }

    return 0;
}

int dBgS::GetPolyAtt1(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetPolyAtt1(poly);
    }

    return 0;
}

int dBgS::GetGroundCode(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetGroundCode(poly);
    }

    return 0;
}

s32 dBgS::GetCamMoveBG(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetCamMoveBG(poly);
    }

    return 0xFF;
}

s32 dBgS::GetRoomCamId(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetRoomCamId(poly);
    }

    return 0xFF;
}

s32 dBgS::GetRoomPathId(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetRoomPathId(poly);
    }

    return 0xFF;
}

s32 dBgS::GetRoomPathPntNo(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        return m_chk_element[bg_index].m_bgw_base_ptr->GetRoomPathPntNo(poly);
    }

    return 0xFF;
}

int dBgS::GetGrpSoundId(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    JUT_ASSERT(1761, 0 <= bg_index && bg_index < 256);
    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0xFF;
    }
    dBgW_Base* r30 = m_chk_element[bg_index].m_bgw_base_ptr;
    return r30->GetGrpSoundId(poly);
}

u32 dBgS::ChkGrpInf(cBgS_PolyInfo const& poly, u32 param_1) {
    u32 grp_inf = GetGrpInf(poly);
    return grp_inf & param_1;
}

int dBgS::GetRoomId(cBgS_PolyInfo const& poly) {
    if (!poly.ChkSetInfo()) {
        return -1;
    }

    int id = poly.GetBgIndex();
    if (!ChkPolySafe(poly)) {
        return -1;
    }

    s32 roomId = m_chk_element[id].m_bgw_base_ptr->GetRoomId();
    if (roomId == 0xFF) {
        roomId = GetGrpRoomId(poly);
        if (roomId == 0xFF) {
            roomId = -1;
        }
    }

    return roomId;
}

bool dBgS::GetPolyAttackThrough(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    JUT_ASSERT(1894, 0 <= bg_index && bg_index < 256);
    if (!m_chk_element[bg_index].ChkUsed()) {
        return false;
    }
    dBgW_Base* r30 = m_chk_element[bg_index].m_bgw_base_ptr;
    return r30->GetAttackThrough(poly.GetPolyIndex()) != 0;
}

u32 dBgS::ChkPolyHSStick(cBgS_PolyInfo const& poly) {
    int bg_index = poly.GetBgIndex();
    JUT_ASSERT(1912, 0 <= bg_index && bg_index < 256);
    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0;
    }
    dBgW_Base* r30 = m_chk_element[bg_index].m_bgw_base_ptr;
    return r30->GetPolyHSStick(poly.GetPolyIndex());
}

void dBgS::WallCorrect(dBgS_Acch* p_acch) {
    p_acch->CalcWallRR();
    p_acch->CalcMovePosWork();

    for (int i = 0; i < 3; i++) {
        cBgS_ChkElm* elm = m_chk_element;

        for (int j = 0; j < 0x100; j++) {
            if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady()) {
                dBgW_Base* pbgw = elm->m_bgw_base_ptr;

                if (pbgw->ChkPriority(i) && !p_acch->ChkSameActorPid(elm->m_actor_id)) {
                    if (!p_acch->ChkMoveBGOnly() || (pbgw->ChkMoveBg() && !pbgw->ChkLock())) {
                        p_acch->SetNowActorInfo(j, elm->m_bgw_base_ptr, elm->m_actor_id);
                        pbgw->WallCorrect(p_acch);
                    }
                }
            }
            elm++;
        }
    }
}

void dBgS::WallCorrectSort(dBgS_Acch* p_acch) {
    p_acch->CalcWallRR();
    p_acch->CalcMovePosWork();

    for (int i = 0; i < 3; i++) {
        cBgS_ChkElm* elm = m_chk_element;

        for (int j = 0; j < 0x100; j++) {
            if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady()) {
                dBgW_Base* pbgw = elm->m_bgw_base_ptr;

                if (pbgw->ChkPriority(i) && !p_acch->ChkSameActorPid(elm->m_actor_id)) {
                    if (!p_acch->ChkMoveBGOnly() || (pbgw->ChkMoveBg() && !pbgw->ChkLock())) {
                        p_acch->SetNowActorInfo(j, elm->m_bgw_base_ptr, elm->m_actor_id);
                        pbgw->WallCorrectSort(p_acch);
                    }
                }
            }
            elm++;
        }
    }
}

f32 dBgS::RoofChk(dBgS_RoofChk* p_roof) {
    p_roof->Init();

    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady()) {
            if (!p_roof->ChkSameActorPid(elm->m_actor_id)) {
                dBgW_Base* pbgw = elm->m_bgw_base_ptr;

                if (pbgw->RoofChk(p_roof)) {
                    p_roof->SetActorInfo(i, elm->m_bgw_base_ptr, elm->m_actor_id);
                }
            }
        }
        elm++;
    }

    return p_roof->GetNowY();
}

bool dBgS::SplGrpChk(dBgS_SplGrpChk* p_grp) {
    bool ret = false;
    p_grp->Init();

    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady()) {
            if (!p_grp->ChkSameActorPid(elm->m_actor_id)) {
                dBgW_Base* pbgw = elm->m_bgw_base_ptr;

                if (!p_grp->ChkMoveBGOnly() || (pbgw->ChkMoveBg() && !pbgw->ChkLock())) {
                    if (pbgw->SplGrpChk(p_grp)) {
                        ret = true;
                        p_grp->SetActorInfo(i, elm->m_bgw_base_ptr, elm->m_actor_id);
                        p_grp->OnFind();
                    }
                }
            }
        }
        elm++;
    }

    return ret;
}

bool dBgS::SphChk(dBgS_SphChk* p_sph, void* param_1) {
    if (p_sph->mCallback == NULL) {
        return false;
    }

    bool ret = false;
    p_sph->ClearPi();

    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady()) {
            if (!p_sph->ChkSameActorPid(elm->m_actor_id)) {
                dBgW_Base* pbgw = elm->m_bgw_base_ptr;

                if (pbgw->SphChk(p_sph, param_1)) {
                    p_sph->SetActorInfo(i, elm->m_bgw_base_ptr, elm->m_actor_id);
                    ret = true;
                }
            }
        }
        elm++;
    }

    return ret;
}

void dBgS::MoveBgCrrPos(cBgS_PolyInfo const& i_poly, bool param_1, cXyz* i_pos, csXyz* i_angle,
                        csXyz* i_shapeAngle, bool param_5, bool param_6) {
    if (!param_1 || !i_poly.ChkBgIndex()) {
        return;
    }

    int bg_index = i_poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        dBgW_Base* bgw_p = m_chk_element[bg_index].m_bgw_base_ptr;

        if ((!param_5 || bgw_p->chkStickWall()) && (!param_6 || bgw_p->chkStickRoof()) &&
            bgw_p->ChkMoveFlag() && ChkPolySafe(i_poly))
        {
            bgw_p->CrrPos(i_poly, m_chk_element[bg_index].m_actor_ptr, param_1, i_pos, i_angle,
                          i_shapeAngle);
        }
    }
}

void dBgS::MoveBgTransPos(cBgS_PolyInfo const& i_poly, bool param_1, cXyz* i_pos, csXyz* i_angle,
                          csXyz* i_shapeAngle) {
    if (!param_1 || !i_poly.ChkBgIndex()) {
        return;
    }

    int bg_index = i_poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        dBgW_Base* bgw_p = m_chk_element[bg_index].m_bgw_base_ptr;

        if (bgw_p->ChkMoveFlag() && ChkPolySafe(i_poly)) {
            bgw_p->TransPos(i_poly, m_chk_element[bg_index].m_actor_ptr, param_1, i_pos, i_angle,
                            i_shapeAngle);
        }
    }
}

void dBgS::MoveBgMatrixCrrPos(cBgS_PolyInfo const& i_poly, bool param_1, cXyz* i_pos,
                              csXyz* i_angle, csXyz* i_shapeAngle) {
    if (!param_1 || !i_poly.ChkBgIndex()) {
        return;
    }

    int bg_index = i_poly.GetBgIndex();
    if (m_chk_element[bg_index].ChkUsed()) {
        dBgW_Base* bgw_p = m_chk_element[bg_index].m_bgw_base_ptr;

        if (bgw_p->ChkMoveFlag()) {
            bgw_p->MatrixCrrPos(i_poly, m_chk_element[bg_index].m_actor_ptr, param_1, i_pos,
                                i_angle, i_shapeAngle);
        }
    }
}

void dBgS_MoveBGProc_Typical(dBgW* i_bgw, void* i_actor_ptr, cBgS_PolyInfo const& i_poly,
                             bool param_3, cXyz* i_pos, csXyz* i_angle, csXyz* i_shapeAngle) {
    Mtx m;
    if (i_bgw->GetOldInvMtx(m) != NULL) {
        cXyz move_old;
        MTXMultVec(m, i_pos, &move_old);

        cXyz move_pos;
        MTXMultVec(i_bgw->GetBaseMtxP(), &move_old, &move_pos);
        i_pos->x = move_pos.x;
        i_pos->y = move_pos.y;
        i_pos->z = move_pos.z;
    }
}

static void dBgS_MoveBGProc_RotY(dBgW* i_bgw, void* i_actor_ptr, cBgS_PolyInfo const& i_poly,
                                 bool param_3, cXyz* i_pos, csXyz* i_angle, csXyz* i_shapeAngle) {
    if (i_shapeAngle != NULL) {
        s16 y = i_bgw->GetDiffShapeAngleY();

        if (i_shapeAngle != NULL) {
            i_shapeAngle->y += y;
        }

        if (i_angle != NULL) {
            i_angle->y += y;
        }
    }
}

void dBgS_MoveBGProc_TypicalRotY(dBgW* i_bgw, void* i_actor_ptr, cBgS_PolyInfo const& i_poly,
                                 bool param_3, cXyz* i_pos, csXyz* i_angle, csXyz* i_shapeAngle) {
    dBgS_MoveBGProc_Typical(i_bgw, i_actor_ptr, i_poly, param_3, i_pos, i_angle, i_shapeAngle);
    dBgS_MoveBGProc_RotY(i_bgw, i_actor_ptr, i_poly, param_3, i_pos, i_angle, i_shapeAngle);
}

void dBgS_MoveBGProc_Trans(dBgW* i_bgw, void* i_actor_ptr, cBgS_PolyInfo const& i_poly,
                           bool param_3, cXyz* i_pos, csXyz* i_angle, csXyz* i_shapeAngle) {
    cXyz trans;
    i_bgw->GetTrans(&trans);

    VECAdd(i_pos, &trans, i_pos);
}

void dBgS::RideCallBack(cBgS_PolyInfo const& poly, fopAc_ac_c* param_1) {
    int bg_index = poly.GetBgIndex();
    dBgW_Base* base = m_chk_element[bg_index].m_bgw_base_ptr;
    if (base->ChkUsed()) {
        base->CallRideCallBack(m_chk_element[bg_index].m_actor_ptr, param_1);
    }
}

void dBgS::ArrowStickCallBack(cBgS_PolyInfo const& poly, fopAc_ac_c* param_1, cXyz& param_2) {
    int bg_index = poly.GetBgIndex();
    dBgW_Base* base = m_chk_element[bg_index].m_bgw_base_ptr;
    if (base->ChkUsed()) {
        base->CallArrowStickCallBack(m_chk_element[bg_index].m_actor_ptr, param_1, param_2);
    }
}

fopAc_ac_c* dBgS::PushPullCallBack(cBgS_PolyInfo const& param_0, fopAc_ac_c* i_pushActor, s16 i_angle,
                            dBgW_Base::PushPullLabel i_label) {
    int bg_index = param_0.GetBgIndex();
    dBgW_Base* base = m_chk_element[bg_index].m_bgw_base_ptr;
    if (!base->ChkUsed()) {
        return NULL;
    }

    if (m_chk_element[bg_index].m_actor_ptr == NULL) {
        return NULL;
    }

    if (base->GetPushPullCallback() == NULL) {
        return NULL;
    }

    return base->GetPushPullCallback()(m_chk_element[bg_index].m_actor_ptr, i_pushActor, i_angle,
                                       i_label);
}

bool dBgS_CheckBWallPoly(cBgS_PolyInfo const& poly) {
    cM3dGPla pla;

    if (!dComIfG_Bgsp().GetTriPla(poly, &pla)) {
        return false;
    }

    return cBgW_CheckBWall(pla.mNormal.y);
}

bool dBgS_CheckBGroundPoly(cBgS_PolyInfo const& poly) {
    cM3dGPla pla;

    if (!dComIfG_Bgsp().GetTriPla(poly, &pla)) {
        return false;
    }

    return cBgW_CheckBGround(pla.mNormal.y);
}

bool dBgS_CheckBRoofPoly(cBgS_PolyInfo const& poly) {
    cM3dGPla pla;

    if (!dComIfG_Bgsp().GetTriPla(poly, &pla)) {
        return false;
    }

    return cBgW_CheckBRoof(pla.mNormal.y);
}

f32 dBgS_GetNY(cBgS_PolyInfo const& poly) {
    cM3dGPla pla;

    dComIfG_Bgsp().GetTriPla(poly, &pla);
    return pla.mNormal.y;
}
