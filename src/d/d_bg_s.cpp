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
#include "dusk/offset_ptr.h"

#include "d/d_debug_viewer.h"
#include "d/d_bg_s_capt_poly.h"

#include "dusk/imgui/ImGuiConsole.hpp"

#if DEBUG
int g_ground_counter;
int g_line_counter;
int g_acch_wall_counter;
int g_roof_counter;
int g_spl_counter;
int g_shdw_counter;
int g_sph_counter;
int g_capt_poly_counter;
#endif


void dBgS_HIO::genMessage(JORMContext* mctx) {
#if DEBUG
    mctx->genLabel("zelda地形チェック", 0);

    mctx->genLabel("チェック回数 ----------", 0);
    mctx->genCheckBox("チェックカウンター", &m_flags, FLAG_CHECK_COUNTER_e);

    mctx->genLabel("平均処理時間(Mean) -----------", 0);
    mctx->genCheckBox("ラインチェック時間(->terminal)", &m_flags2, FLAG2_LINE_TIMER_e);
    mctx->genCheckBox("アクターチェック時間[壁、床（+天井、水面）](->terminal)", &m_flags, 1);
    mctx->genCheckBox("アクターチェック 壁チェック時間(->terminal)", &m_flags, FLAG_ACCH_WALL_TIMER_e);
    mctx->genCheckBox("天井チェック時間(->terminal)", &m_flags, FLAG_ROOF_TIMER_e);
    mctx->genCheckBox("Splチェック(水、溶岩、毒水面)時間(->terminal)", &m_flags2, FLAG2_SPL_TIMER_e);
    mctx->genCheckBox("床チェック時間(->terminal)", &m_flags2, FLAG2_GROUND_CHECK_TIMER_e);
    mctx->genCheckBox("リアル影描画時間(->terminal)", &m_flags2, FLAG2_SHDW_DRAW_TIMER_e);
    mctx->genCheckBox("球チェック時間[コールバック処理含む](->terminal)", &m_flags2, FLAG2_SPH_CHK_TIMER_e);
    mctx->genCheckBox("ポリゴンキャプチャ時間", &m_flags2, FLAG2_CAPTPOLY_TIMER_e);

    mctx->genLabel("処理off ----------", 0);
    mctx->genCheckBox("ライン off", &m_flags2, FLAG2_LINE_OFF_e);
    mctx->genCheckBox("アクターチェック 壁処理off", &m_flags, FLAG_ACCH_WALL_OFF_e);
    mctx->genCheckBox("天井チェックoff", &m_flags, FLAG_ROOF_OFF_e);
    mctx->genCheckBox("Splチェック(水、溶岩、毒水面)off", &m_flags2, FLAG2_SPL_OFF_e);
    mctx->genCheckBox("リアル影処理Off", &m_flags2, FLAG2_SHDW_DRAW_OFF_e);
    mctx->genCheckBox("球チェックOff", &m_flags2, FLAG2_SPH_CHK_OFF_e);
    mctx->genCheckBox("ポリゴンキャプチャOff", &m_flags2, FLAG2_CAPTPOLY_OFF_e);

    mctx->genLabel("当たりチェック ----------", 0);
    mctx->genCheckBox("ラインチェック", &m_flags, FLAG_OBJ_LINE_CHECK_e);
    mctx->genSlider("start x", &m_linecheck_start.x, -450000.0, 450000.0);
    mctx->genSlider("start y", &m_linecheck_start.y, -450000.0, 450000.0);
    mctx->genSlider("start z", &m_linecheck_start.z, -450000.0, 450000.0);
    mctx->genSlider("end x", &m_linecheck_end.x, -450000.0, 450000.0);
    mctx->genSlider("end y", &m_linecheck_end.y, -450000.0, 450000.0);
    mctx->genSlider("end z", &m_linecheck_end.z, -450000.0, 450000.0);
    mctx->genCheckBox("Obj床チェック", &m_flags2, FLAG2_GND_CHK_e);
    mctx->genSlider("gnd x", &m_gndcheck_pos.x, -450000.0, 450000.0);
    mctx->genSlider("gnd y", &m_gndcheck_pos.y, -450000.0, 450000.0);
    mctx->genSlider("gnd z", &m_gndcheck_pos.z, -450000.0, 450000.0);

    mctx->genLabel("抜けチェック ----------", 0);
    mctx->genCheckBox("アクターチェック 抜けチェック", &m_flags, 0x200);
    mctx->genSlider("抜けチェックアクター番号 ", &m_errorCheck_actor_num, -1, 3000);
    mctx->genCheckBox("プレイヤーのアクター地形チェックの床法線ベクトル", &m_flags2, 0x400);

    mctx->genLabel("デバッグ表示 ----------", 0);
    mctx->genCheckBox("アクターチェック 壁当たり範囲表示", &m_flags, 0x400);
    mctx->genCheckBox("BoundingBox描画", &m_flags, 0x8000);
    mctx->genCheckBox("当たり表示On", &m_flags2, FLAG2_SHAPE_DISP_e);

    mctx->genLabel("その他 ----------", 0);
    mctx->genCheckBox("作業１", &m_flags, 0x20);
    mctx->genCheckBox("作業２", &m_flags, 0x40);
    mctx->genCheckBox("作業３", &m_flags, 0x80);
    mctx->genCheckBox("作業４", &m_flags, 0x100);
#endif
}

dBgS_InsideHIO::~dBgS_InsideHIO() {}

void dBgS_InsideHIO::genMessage(JORMContext* mctx) {
#if DEBUG
    mctx->genLabel("デバッグポリゴン描画 -----", 0);
    mctx->genCheckBox("デバッグポリゴン描画", &m_flags, FLAG_DISP_POLY_e);

    mctx->genLabel("-----", 0);
    mctx->genCheckBox("範囲描画(青い箱)", &m_flags, FLAG_DISP_DP_AREA_e);
    mctx->genCheckBox("プレイヤー位置(±p0 x)", &m_flags, FLAG_PLAYER_AROUND_e);

    mctx->genLabel("-----", 0);
    mctx->genCheckBox("白ワイヤーフレーム", &m_flags, FLAG_WHITE_WIRE_e);

    mctx->genLabel("-----", 0);
    mctx->genCheckBox("水面ポリゴンのみ", &m_flags, FLAG_DISP_WATER_POLY_e);

    mctx->genLabel("-----", 0);
    mctx->genCheckBox("床Off", &m_flags, FLAG_GROUND_OFF_e);
    mctx->genCheckBox("壁Off", &m_flags, FLAG_WALL_OFF_e);
    mctx->genCheckBox("天井Off", &m_flags, FLAG_ROOF_OFF_e);

    mctx->genLabel("-----", 0);
    mctx->genSlider("p0 x", &m_p0.x, -100000.0, 100000.0);
    mctx->genSlider("p0 y", &m_p0.y, -100000.0, 100000.0);
    mctx->genSlider("p0 z", &m_p0.z, -100000.0, 100000.0);
    mctx->genSlider("p1 x", &m_p1.x, -100000.0, 100000.0);
    mctx->genSlider("p1 y", &m_p1.y, -100000.0, 100000.0);
    mctx->genSlider("p1 z", &m_p1.z, -100000.0, 100000.0);
    mctx->genSlider("持ち上げ量", &m_raise_amount, -10.0, 30.0);

    mctx->genLabel("-----", 0);
    mctx->genCheckBox("Obj床 プレイヤー下", &m_flags, FLAG_GNDCHK_PLAYER_UNDER_e);
#endif
}

void cBgS_ChkElm::Init() {
    m_bgw_base_ptr = NULL;
    m_used = false;
    m_actor_ptr = NULL;
    m_actor_id = fpcM_ERROR_PROCESS_ID_e;
}

void cBgS_ChkElm::Regist2(dBgW_Base* pbgw, fpc_ProcID actor_id, void* pactor) {
    m_used = true;
    m_bgw_base_ptr = pbgw;
    m_actor_id = actor_id;
    m_actor_ptr = (fopAc_ac_c*)pactor;
}

void cBgS_ChkElm::Release() {
    m_used = false;
    m_bgw_base_ptr = NULL;
    m_actor_id = fpcM_ERROR_PROCESS_ID_e;
    m_actor_ptr = NULL;
}

static int l_SetCounter;

bool cBgS::Regist(dBgW_Base* pbgw, fpc_ProcID actor_id, void* pactor) {
    if (pbgw == NULL) {
        // "cBgS::Regist() data is Null"
        OS_REPORT_ERROR("cBgS::Regist() データがNull\n");
        return true;
    } else if (pbgw->ChkUsed()) {
        // "cBgS::Regist() check duplicate entry."
        OS_REPORT_ERROR("cBgS::Regist() チェック二重登録。\n");
        return true;
    } else if (pbgw->ChkMemoryError()) {
        // "cBgS::Regist() registration rejected (dBgW_Base=%x)"
        OS_REPORT_ERROR("cBgS::Regist() 登録拒否(dBgW_Base=%x)\n", pbgw);
        return true;
    }

    #if DEBUG
    for (int i = 0; i < 0x100; i++) {
        if (m_chk_element[i].ChkUsed() && m_chk_element[i].m_bgw_base_ptr == pbgw) {
            // "cBgS::Regist() will continue processing, but an entry with the same address (address:%x, id = %d) is already registered. This is probably a duplicate entry. Duplicate registrations are dangerous. !!"
            OS_REPORT_ERROR("cBgS::Regist() 処理は続けますが、同じアドレスのもの(address:%x, id = %d)がすでに登録されています。多分ですが多重登録です。多重登録ならキケンです。！！\n", pbgw, i);
            JUT_WARN(368, "%s", "cBgS::Regist() double regist");
        }
    }
    #endif

    int i = l_SetCounter;
    JUT_ASSERT(376, 0 <= i && i < 256);

    do {
        if (!m_chk_element[i].ChkUsed()) {
            m_chk_element[i].Regist2(pbgw, actor_id, pactor);
            pbgw->Regist(i);

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

    JUT_WARN(398, "%s", "cBgS::Regist() regist over.");
    OS_REPORT_ERROR("[ERROR] cBgS::Regist() table is no empty\n");
    pbgw->Release();
    return true;
}

bool cBgS::Release(dBgW_Base* pbgw) {
    if (pbgw == NULL) {
        return 1;
    }

    int id = pbgw->GetId();
    if (pbgw->ChkUsed() && id >= 0 && id < 0x100 && m_chk_element[id].ChkUsed()) {
        #if DEBUG
        if (m_chk_element[id].m_bgw_base_ptr != pbgw) {
            // "[ERROR] cBgS::Release() the work area pointer registered to %d differs from pointer passed as an argument to Release(). (%x,%x) [Releasing]"
            OS_REPORT_ERROR("[ERROR] cBgS::Release() %dに登録されている作業領域のポインターと、Release()に引数で渡されているポインターが異なる。(%x,%x) [解放実行]", id, m_chk_element[id].m_bgw_base_ptr, pbgw);
            JUT_WARN(431, "%s", "cBgS::Release() data error.");
        }
        #endif

        m_chk_element[id].Release();
        pbgw->Release();
    } else {
        #if DEBUG
        if (id == 0x100) {
            // "cBgS::Release() attempting to release unregistered terrain. operation won't be processed."
            OS_WARNING("cBgS::Release() 登録していない地形をReleaseしようとしています。処理しません。\n");
        } else {
            // "[Error] cBgS::Release() Error[Check id=%d won't be released.](dBgW_Base=%x)"
            OS_REPORT_ERROR("[Error] cBgS::Release() Error[id=%d番のチェックはリリースしません。](dBgW_Base=%x)\n", id, pbgw);

            if (!pbgw->ChkUsed()) {
                // "Data [%d] is not being used."
                OS_REPORT_ERROR("データ[%d]は使っていません。\n", id);
            }

            if (!m_chk_element[id].ChkUsed()) {
                // "Table [%d] is not being used."
                OS_REPORT_ERROR("テーブル[%d]は使っていません。\n", id);
            }
        }
        #endif

        return 1;
    }

    return 0;
}

#if DEBUG
u8 lbl_8074C7F0;  // belongs in d_bg_w?
static u8 lbl_8074C7F8;
#endif

void cBgS::Ct() {
    l_SetCounter = 0;
    for (int i = 0; i < 0x100; i++) {
        m_chk_element[i].Init();
    }

    #if DEBUG
    lbl_8074C7F8 = 0;
    lbl_8074C7F0 = 0;
    #endif
}

void cBgS::Dt() {
    for (int i = 0; i < 0x100; i++) {
        if (m_chk_element[i].ChkUsed()) {
            JUT_WARN(499, "%s", "cBgS::Dt() release error.");
            OS_REPORT_ERROR("[ERROR] ========== cBgS::Dt() 消し残し地形チェックがあります。id=%d ==========\n", i);
            OS_REPORT_ERROR("!!!!!!!!! 勝手に消しときます !!!!!!!!\n");
            m_chk_element[i].Release();
        }
    }

    for (int i = 0; i < 0x100; i++) {
        m_chk_element[i].Init();
    }
}

bool cBgS::LineCross(cBgS_LinChk* plinchk) {
    plinchk->ClearPi();
    plinchk->ClrHit();
    plinchk->PreCalc();

    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady() &&
            !plinchk->ChkSameActorPid(elm->m_actor_id) && elm->m_bgw_base_ptr->LineCheck(plinchk))
        {
            plinchk->SetActorInfo(i, elm->m_bgw_base_ptr, elm->m_actor_id);
            plinchk->SetHit();
        }
        elm++;
    }

    return plinchk->ChkHit();
}

f32 cBgS::GroundCross(cBgS_GndChk* pgndchk) {
    pgndchk->SetNowY(-G_CM3D_F_INF);
    pgndchk->ClearPi();
    pgndchk->PreCheck();

    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady() &&
            !pgndchk->ChkSameActorPid(elm->m_actor_id) && elm->m_bgw_base_ptr->GroundCross(pgndchk))
        {
            pgndchk->SetActorInfo(i, elm->m_bgw_base_ptr, elm->m_actor_id);
        }
        elm++;
    }

    return pgndchk->GetNowY();
}

template <>
void be_swap(cBgD_Vtx_t& val) {
    be_swap(val.x);
    be_swap(val.y);
    be_swap(val.z);
}

void* cBgS::ConvDzb(void* pdzb) {
    cBgD_t* pbgd = (cBgD_t*)pdzb;

    if (((pbgd->mFlags & 0x80000000) == 0)) {
        pbgd->mFlags |= 0x80000000;
    } else {
        return pdzb;
    }

#if TARGET_PC
    pbgd->m_v_tbl.setBase(pdzb);
    pbgd->m_t_tbl.setBase(pdzb);
    pbgd->m_b_tbl.setBase(pdzb);
    pbgd->m_tree_tbl.setBase(pdzb);
    pbgd->m_g_tbl.setBase(pdzb);
    pbgd->m_ti_tbl.setBase(pdzb);

    for (int i = 0; i < pbgd->m_g_num; i++) {
        ((cBgD_Grp_t*)pbgd->m_g_tbl)[i].m_name.setBase(pdzb);
    }

#if TARGET_LITTLE_ENDIAN
    for (int i = 0; i < pbgd->m_v_num; i++) {
        be_swap(pbgd->m_v_tbl[i]);
    }
#endif

#else
    if (pbgd->m_v_tbl != 0) {
        pbgd->m_v_tbl += (uintptr_t)pdzb;
    }

    pbgd->m_t_tbl += (uintptr_t)pdzb;
    pbgd->m_b_tbl += (uintptr_t)pdzb;
    pbgd->m_tree_tbl += (uintptr_t)pdzb;
    pbgd->m_g_tbl += (uintptr_t)pdzb;
    pbgd->m_ti_tbl += (uintptr_t)pdzb;

    for (int i = 0; i < pbgd->m_g_num; i++) {
        ((cBgD_Grp_t_*)pbgd->m_g_tbl)[i].strOffset = (uintptr_t)pdzb + ((cBgD_Grp_t_*)pbgd->m_g_tbl)[i].strOffset;
    }
#endif

    return pdzb;
}

fopAc_ac_c* cBgS::GetActorPointer(int actor_index) const {
    JUT_ASSERT(631, 0 <= actor_index && actor_index < 256);
    return m_chk_element[actor_index].m_actor_ptr;
}

dBgW_Base* cBgS::GetBgWBasePointer(const cBgS_PolyInfo& polyinfo) const {
    int bg_index = polyinfo.GetBgIndex();

    if (bg_index < 0 || bg_index >= 0x100 || !m_chk_element[bg_index].ChkUsed()) {
        return NULL;
    }
    return m_chk_element[bg_index].m_bgw_base_ptr;
}

bool cBgS::ChkPolySafe(const cBgS_PolyInfo& polyinfo) {
    if (!polyinfo.ChkSetInfo()) {
        return false;
    }

    int bg_index = polyinfo.GetBgIndex();
    if (bg_index < 0 || bg_index >= 0x100) {
        return false;
    }

    if (!m_chk_element[bg_index].ChkUsed()) {
        return false;
    }

    return polyinfo.ChkSafe(m_chk_element[bg_index].m_bgw_base_ptr, m_chk_element[bg_index].m_actor_id);    
}

s32 cBgS::GetGrpRoomId(const cBgS_PolyInfo& polyinfo) const {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(697, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0xFF;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetGrpRoomIndex(polyinfo);
}

bool cBgS::GetTriPla(const cBgS_PolyInfo& polyinfo, cM3dGPla* ppla) const {
    int bg_index = polyinfo.GetBgIndex();
    if (bg_index < 0 || bg_index >= 0x100 || !m_chk_element[bg_index].ChkUsed()) {
        return false;
    }

    *ppla = m_chk_element[bg_index].m_bgw_base_ptr->GetTriPla(polyinfo);
    return true;
}

bool cBgS::GetTriPnt(const cBgS_PolyInfo& polyinfo, cXyz* ppnt0, cXyz* ppnt1, cXyz* ppnt2) const {
    int id = polyinfo.GetBgIndex();
    JUT_ASSERT(737, 0 <= id && id < 256);

    if (!m_chk_element[id].ChkUsed()) {
        return false;
    }

    return m_chk_element[id].m_bgw_base_ptr->GetTriPnt(polyinfo, ppnt0, ppnt1, ppnt2);
}

void cBgS::Draw() {}

void cBgS::ShdwDraw(cBgS_ShdwDraw* pshdw) {
    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed()) {
            elm->m_bgw_base_ptr->ShdwDraw(pshdw);
        }
        elm++;
    }
}

u32 cBgS::GetGrpInf(const cBgS_PolyInfo& polyinfo) const {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(803, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return -1;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetGrpInf(polyinfo);
}

#if DEBUG
static OSStopwatch s_ground_sw;
static OSStopwatch s_line_sw;
static OSStopwatch s_wall_correct_sw;
static OSStopwatch s_roof_sw;
static OSStopwatch s_spl_sw;
static OSStopwatch s_shdw_sw;
static OSStopwatch s_sph_chk_sw;
static OSStopwatch s_capt_poly_sw;
#endif

static dBgS_InsideHIO s_InsideHio;

void dBgS::Ct() {
    cBgS::Ct();

    #if DEBUG
    g_ground_counter = 0;
    g_line_counter = 0;
    g_acch_wall_counter = 0;
    g_roof_counter = 0;
    g_spl_counter = 0;
    g_shdw_counter = 0;
    g_sph_counter = 0;
    g_capt_poly_counter = 0;

    m_hio.id = mDoHIO_CREATE_CHILD("zelda地形チェック", &m_hio);
    s_InsideHio.id = mDoHIO_CREATE_CHILD("zelda地形チェック内部", &s_InsideHio);

    OSInitStopwatch(&s_ground_sw, "GroundCheck");
    OSResetStopwatch(&s_ground_sw);

    OSInitStopwatch(&s_line_sw, "LineCheck");
    OSResetStopwatch(&s_line_sw);

    OSInitStopwatch(&s_wall_correct_sw, "WallCorrect()");
    OSResetStopwatch(&s_wall_correct_sw);

    OSInitStopwatch(&s_roof_sw, "RoofChk()");
    OSResetStopwatch(&s_roof_sw);

    OSInitStopwatch(&s_spl_sw, "SplGrpChk()");
    OSResetStopwatch(&s_spl_sw);

    OSInitStopwatch(&s_shdw_sw, "ShdwDraw()");
    OSResetStopwatch(&s_shdw_sw);

    OSInitStopwatch(&s_sph_chk_sw, "SphChk()");
    OSResetStopwatch(&s_sph_chk_sw);

    OSInitStopwatch(&s_capt_poly_sw, "CaptPoly()");
    OSResetStopwatch(&s_capt_poly_sw);
    #endif
}

void dBgS::Dt() {
    cBgS::Dt();

    #if DEBUG
    mDoHIO_DELETE_CHILD(m_hio.id);
    mDoHIO_DELETE_CHILD(s_InsideHio.id);
    #endif
}

void dBgS::ClrMoveFlag() {
    for (int i = 0; i < 0x100; i++) {
        if (m_chk_element[i].ChkUsed()) {
            dBgW_Base* bgw_base = m_chk_element[i].m_bgw_base_ptr;
            bgw_base->OffMoveFlag();
        }
    }
}

void dBgS::Move() {
    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed()) {
            fopAc_ac_c* bgw_actor = elm->m_actor_ptr;
            elm->m_bgw_base_ptr->CalcDiffShapeAngleY(bgw_actor->shape_angle.y);
        }
        elm++;
    }

    #if DEBUG
    if (lbl_8074C7F8 == TRUE) {
        JUTReport(20, 100, "WARNING:kcl:WCS(wall/roof poly num) is over.");
    }

    if (lbl_8074C7F0 == TRUE) {
        JUTReport(20, 200, "WARNING:dzb:wcs(wall/roof poly num) is over.");
    }
    #endif
}

void dBgS_AabDraw(cM3dGAab& aab, GXColor& color) {
    cXyz points[8];

    points[0].x = aab.GetMinP()->x;
    points[0].y = aab.GetMaxP()->y;
    points[0].z = aab.GetMinP()->z;

    points[1].x = aab.GetMaxP()->x;
    points[1].y = aab.GetMaxP()->y;
    points[1].z = aab.GetMinP()->z;

    points[2].x = aab.GetMinP()->x;
    points[2].y = aab.GetMaxP()->y;
    points[2].z = aab.GetMaxP()->z;
    
    points[3].x = aab.GetMaxP()->x;
    points[3].y = aab.GetMaxP()->y;
    points[3].z = aab.GetMaxP()->z;

    points[4].x = aab.GetMinP()->x;
    points[4].y = aab.GetMinP()->y;
    points[4].z = aab.GetMinP()->z;

    points[5].x = aab.GetMaxP()->x;
    points[5].y = aab.GetMinP()->y;
    points[5].z = aab.GetMinP()->z;

    points[6].x = aab.GetMinP()->x;
    points[6].y = aab.GetMinP()->y;
    points[6].z = aab.GetMaxP()->z;
    
    points[7].x = aab.GetMaxP()->x;
    points[7].y = aab.GetMinP()->y;
    points[7].z = aab.GetMaxP()->z;

    dDbVw_drawCube8pOpa(points, color);
}

static int white_draw(dBgS_CaptPoly* capt, cBgD_Vtx_t* vtxList, int v0, int v1, int v2, cM3dGPla* plane) {
    cXyz raise;
    PSVECScale(&plane->mNormal, &raise, s_InsideHio.m_raise_amount + 1.0f);

    cXyz start;
    start.set(vtxList[v0]);
    cXyz end;
    end.set(vtxList[v1]);
    PSVECAdd(&raise, &start, &start);
    PSVECAdd(&raise, &end, &end);

    GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
    dDbVw_drawLineOpa(start, end, color, TRUE, 12);

    start.set(vtxList[v1]);
    end.set(vtxList[v2]);
    PSVECAdd(&raise, &start, &start);
    PSVECAdd(&raise, &end, &end);
    dDbVw_drawLineOpa(start, end, color, TRUE, 12);

    start.set(vtxList[v2]);
    end.set(vtxList[v0]);
    PSVECAdd(&raise, &start, &start);
    PSVECAdd(&raise, &end, &end);
    dDbVw_drawLineOpa(start, end, color, TRUE, 12);
    return 0;
}

static int poly_draw(dBgS_CaptPoly* capt, cBgD_Vtx_t* vtxList, int v0, int v1, int v2, cM3dGPla* plane) {
    UNUSED(capt);

    cXyz points[3];
    GXColor ground_color = {0xFF, 0, 0, 0xFF};
    GXColor roof_color = {0, 0, 0xFF, 0xFF};
    GXColor wall_color = {0, 0xFF, 0, 0xFF};

#if TARGET_PC
    dusk::ImGuiMenuTools::CollisionViewSettings collisionViewSettings = dusk::g_imguiConsole.getCollisionViewSettings();
    f32 view_opacity = 255 * (collisionViewSettings.m_terrainViewOpacity / 100.0f);
    ground_color.a = view_opacity;
    roof_color.a = view_opacity;
    wall_color.a = view_opacity;
#endif

    cXyz raise;
    PSVECScale(&plane->mNormal, &raise, s_InsideHio.m_raise_amount);

    points[0] = vtxList[v0];
    points[1] = vtxList[v1];
    points[2] = vtxList[v2];

    PSVECAdd(&points[0], &raise, &points[0]);
    PSVECAdd(&points[1], &raise, &points[1]);
    PSVECAdd(&points[2], &raise, &points[2]);

    if (cBgW_CheckBGround(plane->mNormal.y)) {
        if (!s_InsideHio.ChkGroundOff()) {
            dDbVw_drawTriangleOpa(points, ground_color, TRUE);
        }
    } else if (cBgW_CheckBRoof(plane->mNormal.y)) {
        if (!s_InsideHio.ChkRoofOff()) {
            dDbVw_drawTriangleOpa(points, roof_color, TRUE);
        }
    } else {
        if (!s_InsideHio.ChkWallOff()) {
            dDbVw_drawTriangleOpa(points, wall_color, TRUE);
        }
    }
    
    return 0;
}

void dBgS::Draw() {
    cBgS::Draw();

#if TARGET_PC
    #define IMGUI_TOGGLE_HIO_FLAG(status, flag) \
        if (status) { \
            s_InsideHio.m_flags |= flag; \
        } else { \
            s_InsideHio.m_flags &= ~flag; \
        }

    dusk::ImGuiMenuTools::CollisionViewSettings collisionViewSettings = dusk::g_imguiConsole.getCollisionViewSettings();
    IMGUI_TOGGLE_HIO_FLAG(collisionViewSettings.m_enableTerrainView, dBgS_InsideHIO::FLAG_DISP_POLY_e);
    IMGUI_TOGGLE_HIO_FLAG(collisionViewSettings.m_enableWireframe, dBgS_InsideHIO::FLAG_WHITE_WIRE_e);
#endif

    if (s_InsideHio.ChkDispPoly()) {
        cM3dGAab aab;
        if (s_InsideHio.ChkPlayerAround()) {
            fopAc_ac_c* player = dComIfGp_getPlayer(0);
            cXyz min;
            cXyz max;

            f32 var_f31 = fabsf(s_InsideHio.m_p0.x);

#if TARGET_PC
            dusk::ImGuiMenuTools::CollisionViewSettings collisionViewSettings = dusk::g_imguiConsole.getCollisionViewSettings();
            var_f31 = collisionViewSettings.m_drawRange;
#endif

            min.x = player->current.pos.x - var_f31;
            min.y = player->current.pos.y - var_f31;
            min.z = player->current.pos.z - var_f31;
            max.x = player->current.pos.x + var_f31;
            max.y = player->current.pos.y + var_f31;
            max.z = player->current.pos.z + var_f31;

            aab.Set(&min, &max);
        } else {
            aab.ClearForMinMax();
            aab.SetMinMax(s_InsideHio.m_p0);
            aab.SetMinMax(s_InsideHio.m_p1);
        }

        if (s_InsideHio.ChkDispDpArea()) {
            static GXColor l_color = {0, 0, 0xFF, 0x80};
            dBgS_AabDraw(aab, l_color);
        }

        dBgS_CaptPoly capt;
        capt.Set(*aab.GetMinP(), *aab.GetMaxP());

        if (s_InsideHio.ChkDispWaterPoly()) {
            capt.OffFullGrp();
            capt.OnWaterGrp();
        } else {
            capt.OnFullGrp();
        }

        if (s_InsideHio.ChkWhiteWire()) {
            capt.SetCallback(white_draw);
            CaptPoly(capt);
        }

        capt.SetCallback(poly_draw);
        CaptPoly(capt);
    }

    #if DEBUG
    if (m_hio.ChkCheckCounter()) {
        OSReport("ラインチェック %d回\n", g_line_counter);
        g_line_counter = 0;

        OSReport("アクター地形チェック壁処理 %d回\n", g_acch_wall_counter);
        g_acch_wall_counter = 0;

        OSReport("天井チェック %d回\n", g_roof_counter);
        g_roof_counter = 0;

        OSReport("Splチェック %d回\n", g_spl_counter);
        g_spl_counter = 0;

        OSReport("床チェック %d回\n", g_ground_counter);
        g_ground_counter = 0;

        OSReport("リアル影 %d回\n", g_shdw_counter);
        g_shdw_counter = 0;

        OSReport("球チェック %d回\n", g_sph_counter);
        g_sph_counter = 0;

        OSReport("ポリゴンキャプチャ %d\n", g_capt_poly_counter);
        g_capt_poly_counter = 0;
    }
    #endif

    if (m_hio.ChkObjLineCheck()) {
        dBgS_LinChk linechk;
        linechk.Set(&m_hio.m_linecheck_start, &m_hio.m_linecheck_end, NULL);

        dDbVw_drawLineOpa(m_hio.m_linecheck_start, m_hio.m_linecheck_end, COMPOUND_LITERAL(GXColor){0xFF, 0xFF, 0xFF, 0xFF}, TRUE, 12);
        dDbVw_drawSphereOpa(m_hio.m_linecheck_start, 10.0f, COMPOUND_LITERAL(GXColor){0xFF, 0, 0, 0xFF}, TRUE);
        dDbVw_drawSphereOpa(m_hio.m_linecheck_end, 10.0f, COMPOUND_LITERAL(GXColor){0, 0, 0xFF, 0xFF}, TRUE);

        if (LineCross(&linechk)) {
            dDbVw_drawSphereOpa(*linechk.GetCrossP(), 10.0f, COMPOUND_LITERAL(GXColor){0, 0xFF, 0, 0xFF}, TRUE);
        }
    }

    if (m_hio.ChkGndCheck()) {
        dBgS_ObjGndChk gndchk;
        cXyz sp54;

        if (!s_InsideHio.ChkGndChkPlayerUnder()) {
            sp54 = m_hio.m_gndcheck_pos;
        } else {
            fopAc_ac_c* player = dComIfGp_getPlayer(0);
            cXyz sp48(player->current.pos);
            sp48.y += 200.0f;
        }

        gndchk.SetPos(&sp54);
        f32 var_f30 = dComIfG_Bgsp().GroundCross(&gndchk);

        cXyz sp3C;
        sp3C = sp54;
        sp3C.y = var_f30;

        dDbVw_drawLineOpa(sp54, sp3C, COMPOUND_LITERAL(GXColor){0, 0xFF, 0, 0xFF}, TRUE, 12);
        dDbVw_drawPointOpa(sp3C, COMPOUND_LITERAL(GXColor){0xFF, 0, 0, 0xFF}, TRUE, 12);

        if (var_f30 != G_CM3D_F_INF) {
            DrawPoly(gndchk, COMPOUND_LITERAL(GXColor){0xFF, 0xFF, 0, 0xFF});
        }
    }
}

void dBgS::CaptPoly(dBgS_CaptPoly& capt) {
    if (!m_hio.ChkCaptPolyOff()) {
        #if DEBUG
        if (m_hio.ChkCheckCounter()) {
            g_capt_poly_counter++;
        }

        if (m_hio.ChkCaptPolyTimer()) {
            OSStartStopwatch(&s_capt_poly_sw);
        }
        #endif

        cBgS_ChkElm* elm = m_chk_element;
        for (int i = 0; i < 0x100; i++) {
            if (elm->ChkUsed()) {
                elm->m_bgw_base_ptr->CaptPoly(capt);
            }
            elm++;
        }

        #if DEBUG
        if (m_hio.ChkCaptPolyTimer()) {
            OSStopStopwatch(&s_capt_poly_sw);
            OSDumpStopwatch(&s_capt_poly_sw);
        }
        #endif
    }
}

void dBgS::ChkDeleteActorRegist(fopAc_ac_c* actor) {
    for (int i = 0; i < 0x100; i++) {
        if (m_chk_element[i].ChkUsed() && actor == m_chk_element[i].m_actor_ptr) {
            OS_REPORT_ERROR("dBgS::ChkDeleteActorRegist() [%d]削除されたアクターのポインターが、まだ登録されています。\n", i);
        }
    }
}

bool dBgS::Regist(dBgW_Base* pbgw, fopAc_ac_c* pactor) {
    if (pbgw == NULL) {
        OS_REPORT_ERROR("dBgS::Regist() pbgwがNULLのため登録できません。\n");
        return true;
    }

    if (pactor != NULL && pbgw->ChkMoveBg()) {
        fopAc_ac_c* actor = pactor;
        pbgw->SetOldShapeAngleY(actor->shape_angle.y);
        pbgw->SetRoomId(fopAcM_GetRoomNo(actor));
    }

    return cBgS::Regist(pbgw, fopAcM_GetID(pactor), pactor);
}

void dBgW_Base::SetOldShapeAngleY(s16 shape_angle) {
    m_old_ShapeAngleY = shape_angle;
}

bool dBgS::ChkMoveBG(const cBgS_PolyInfo& polyinfo) {
    dBgW_Base* pbgw = dComIfG_Bgsp().GetBgWBasePointer(polyinfo);
    if (pbgw != NULL) {
        if (pbgw->ChkLock()) {
            return false;
        }

        if (pbgw->ChkMoveBg()) {
            return true;
        }
    }

    return false;
}

bool dBgS::ChkMoveBG_NoDABg(const cBgS_PolyInfo& polyinfo) {
    dBgW_Base* pbgw = dComIfG_Bgsp().GetBgWBasePointer(polyinfo);
    if (pbgw != NULL && pbgw->ChkMoveBg()) {
        return true;
    }

    return false;
}

s32 dBgS::GetExitId(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1421, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0x3F;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetExitId(polyinfo);
}

s32 dBgS::GetPolyColor(const cBgS_PolyInfo& polyinfo) {
    if (!polyinfo.ChkSetInfo()) {
        return 0xFF;
    }

    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1443, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0xFF;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetPolyColor(polyinfo);
}

BOOL dBgS::GetHorseNoEntry(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1460, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return false;
    }
    
    return m_chk_element[bg_index].m_bgw_base_ptr->GetHorseNoEntry(polyinfo);
}

int dBgS::GetSpecialCode(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1477, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetSpecialCode(polyinfo);
}

int dBgS::GetMagnetCode(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1494, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetMagnetCode(polyinfo);
}

int dBgS::GetMonkeyBarsCode(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1509, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetMonkeyBarsCode(polyinfo);
}

u32 dBgS::GetUnderwaterRoofCode(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1524, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetUnderwaterRoofCode(polyinfo.GetPolyIndex());
}

s32 dBgS::GetWallCode(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1574, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetWallCode(polyinfo);
}

int dBgS::GetPolyAtt0(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1591, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetPolyAtt0(polyinfo);
}

int dBgS::GetPolyAtt1(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1608, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0;
    }
    
    return m_chk_element[bg_index].m_bgw_base_ptr->GetPolyAtt1(polyinfo);
}

int dBgS::GetGroundCode(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1625, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0;
    }
    
    return m_chk_element[bg_index].m_bgw_base_ptr->GetGroundCode(polyinfo);
}

s32 dBgS::GetCamMoveBG(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1667, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0xFF;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetCamMoveBG(polyinfo);
}

s32 dBgS::GetRoomCamId(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1684, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0xFF;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetRoomCamId(polyinfo);
}

s32 dBgS::GetRoomPathId(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1700, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0xFF;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetRoomPathId(polyinfo);
}

s32 dBgS::GetRoomPathPntNo(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1717, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0xFF;
    }

    return m_chk_element[bg_index].m_bgw_base_ptr->GetRoomPathPntNo(polyinfo);
}

int dBgS::GetGrpSoundId(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1761, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0xFF;
    }

    dBgW_Base* pbgw = m_chk_element[bg_index].m_bgw_base_ptr;
    return pbgw->GetGrpSoundId(polyinfo);
}

u32 dBgS::ChkGrpInf(const cBgS_PolyInfo& polyinfo, u32 mask) {
    u32 grp_inf = GetGrpInf(polyinfo);
    return grp_inf & mask;
}

int dBgS::GetRoomId(const cBgS_PolyInfo& polyinfo) {
    if (!polyinfo.ChkSetInfo()) {
        return -1;
    }

    int id = polyinfo.GetBgIndex();
    JUT_ASSERT(1804, 0 <= id && id < 256);
    if (!ChkPolySafe(polyinfo)) {
        return -1;
    }

    dBgW_Base* pbgw = m_chk_element[id].m_bgw_base_ptr;
    int roomId = pbgw->GetRoomId();
    if (roomId == 0xFF) {
        roomId = GetGrpRoomId(polyinfo);
        if (roomId == 0xFF) {
            roomId = -1;
        }
    }

    return roomId;
}

bool dBgS::GetPolyAttackThrough(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1894, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return false;
    }

    dBgW_Base* pbgw = m_chk_element[bg_index].m_bgw_base_ptr;
    return pbgw->GetAttackThrough(polyinfo.GetPolyIndex()) != 0;
}

u32 dBgS::ChkPolyHSStick(const cBgS_PolyInfo& polyinfo) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(1912, 0 <= bg_index && bg_index < 256);

    if (!m_chk_element[bg_index].ChkUsed()) {
        return 0;
    }

    dBgW_Base* pbgw = m_chk_element[bg_index].m_bgw_base_ptr;
    return pbgw->GetPolyHSStick(polyinfo.GetPolyIndex());
}

// these don't appear to be inlined based on the static var usage and location within the TU,
// but if they're not inlined then the calls in retail don't resolve to cBgS variants.
// Ifdef'ing for now to preserve both versions behavior
#if DEBUG
f32 dBgS::GroundCross(cBgS_GndChk* pgndchk) {
    if (m_hio.ChkCheckCounter()) {
        g_ground_counter++;
    }

    if (m_hio.ChkGroundCheckTimer()) {
        OSStartStopwatch(&s_ground_sw);
    }

    f32 rt = cBgS::GroundCross(pgndchk);
    if (m_hio.ChkGroundCheckTimer()) {
        OSStopStopwatch(&s_ground_sw);
        OSDumpStopwatch(&s_ground_sw);
    }

    return rt;
}

bool dBgS::LineCross(cBgS_LinChk* plinchk) {
    if (m_hio.ChkLineOff()) {
        return false;
    }

    if (m_hio.ChkCheckCounter()) {
        g_line_counter++;
    }

    if (m_hio.ChkLineTimer()) {
        OSStartStopwatch(&s_line_sw);
    }

    bool rt = cBgS::LineCross(plinchk);
    if (m_hio.ChkLineTimer()) {
        OSStopStopwatch(&s_line_sw);
        OSDumpStopwatch(&s_line_sw);
    }

    return rt;
}

void dBgS::ShdwDraw(cBgS_ShdwDraw* pshdwDraw) {
    if (!m_hio.ChkShdwDrawOff()) {
        if (m_hio.ChkCheckCounter()) {
            g_shdw_counter++;
        }

        if (m_hio.ChkShdwDrawTimer()) {
            OSStartStopwatch(&s_shdw_sw);
        }

        cBgS::ShdwDraw(pshdwDraw);
        if (m_hio.ChkShdwDrawTimer()) {
            OSStopStopwatch(&s_shdw_sw);
            OSDumpStopwatch(&s_shdw_sw);
        }
    }
}
#endif

void dBgS::WallCorrect(dBgS_Acch* pacch) {
    #if DEBUG
    if (m_hio.ChkAcchWallOff()) {
        return;
    }

    if (m_hio.ChkCheckCounter()) {
        g_acch_wall_counter++;
    }

    if (m_hio.ChkAcchWallTimer()) {
        OSStartStopwatch(&s_wall_correct_sw);
    }
    #endif
    
    pacch->CalcWallRR();
    pacch->CalcMovePosWork();

    for (int i = 0; i < 3; i++) {
        cBgS_ChkElm* elm = m_chk_element;

        for (int j = 0; j < 0x100; j++) {
            if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady()) {
                dBgW_Base* pbgw = elm->m_bgw_base_ptr;

                if (pbgw->ChkPriority(i) && !pacch->ChkSameActorPid(elm->m_actor_id)) {
                    if (!pacch->ChkMoveBGOnly() || (pbgw->ChkMoveBg() && !pbgw->ChkLock())) {
                        pacch->SetNowActorInfo(j, elm->m_bgw_base_ptr, elm->m_actor_id);
                        pbgw->WallCorrect(pacch);
                    }
                }
            }
            elm++;
        }
    }

    #if DEBUG
    if (m_hio.ChkAcchWallTimer()) {
        OSStopStopwatch(&s_wall_correct_sw);
        OSDumpStopwatch(&s_wall_correct_sw);
    }
    #endif
}

void dBgS::WallCorrectSort(dBgS_Acch* pacch) {
    #if DEBUG
    if (m_hio.ChkAcchWallOff()) {
        return;
    }

    if (m_hio.ChkCheckCounter()) {
        g_acch_wall_counter++;
    }

    if (m_hio.ChkAcchWallTimer()) {
        OSStartStopwatch(&s_wall_correct_sw);
    }
    #endif

    pacch->CalcWallRR();
    pacch->CalcMovePosWork();

    for (int i = 0; i < 3; i++) {
        cBgS_ChkElm* elm = m_chk_element;

        for (int j = 0; j < 0x100; j++) {
            if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady()) {
                dBgW_Base* pbgw = elm->m_bgw_base_ptr;

                if (pbgw->ChkPriority(i) && !pacch->ChkSameActorPid(elm->m_actor_id)) {
                    if (!pacch->ChkMoveBGOnly() || (pbgw->ChkMoveBg() && !pbgw->ChkLock())) {
                        pacch->SetNowActorInfo(j, elm->m_bgw_base_ptr, elm->m_actor_id);
                        pbgw->WallCorrectSort(pacch);
                    }
                }
            }
            elm++;
        }
    }

    #if DEBUG
    if (m_hio.ChkAcchWallTimer()) {
        OSStopStopwatch(&s_wall_correct_sw);
        OSDumpStopwatch(&s_wall_correct_sw);
    }
    #endif
}

f32 dBgS::RoofChk(dBgS_RoofChk* proof) {
    #if DEBUG
    if (m_hio.ChkRoofOff()) {
        return G_CM3D_F_INF;
    }

    if (m_hio.ChkCheckCounter()) {
        g_roof_counter++;
    }

    if (m_hio.ChkRoofTimer()) {
        OSStartStopwatch(&s_roof_sw);
    }
    #endif

    proof->Init();

    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady()) {
            if (!proof->ChkSameActorPid(elm->m_actor_id)) {
                dBgW_Base* pbgw = elm->m_bgw_base_ptr;

                if (pbgw->RoofChk(proof)) {
                    proof->SetActorInfo(i, elm->m_bgw_base_ptr, elm->m_actor_id);
                }
            }
        }
        elm++;
    }

    #if DEBUG
    if (m_hio.ChkAcchWallTimer()) {
        OSStopStopwatch(&s_roof_sw);
        OSDumpStopwatch(&s_roof_sw);
    }
    #endif

    return proof->GetNowY();
}

bool dBgS::SplGrpChk(dBgS_SplGrpChk* pspl) {
    #if DEBUG
    if (m_hio.ChkSplOff()) {
        return false;
    }

    if (m_hio.ChkCheckCounter()) {
        g_spl_counter++;
    }

    if (m_hio.ChkSplTimer()) {
        OSStartStopwatch(&s_spl_sw);
    }
    #endif

    bool ret = false;
    pspl->Init();

    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady()) {
            if (!pspl->ChkSameActorPid(elm->m_actor_id)) {
                dBgW_Base* pbgw = elm->m_bgw_base_ptr;

                if (!pspl->ChkMoveBGOnly() || (pbgw->ChkMoveBg() && !pbgw->ChkLock())) {
                    if (pbgw->SplGrpChk(pspl)) {
                        ret = true;
                        pspl->SetActorInfo(i, elm->m_bgw_base_ptr, elm->m_actor_id);
                        pspl->OnFind();
                    }
                }
            }
        }
        elm++;
    }

    #if DEBUG
    if (m_hio.ChkAcchWallTimer()) {
        OSStopStopwatch(&s_spl_sw);
        OSDumpStopwatch(&s_spl_sw);
    }
    #endif

    return ret;
}

bool dBgS::SphChk(dBgS_SphChk* psphchk, void* param_1) {
    if (psphchk->mCallback == NULL) {
        return false;
    }

    #if DEBUG
    if (m_hio.ChkSphChkOff()) {
        return false;
    }

    if (m_hio.ChkCheckCounter()) {
        g_sph_counter++;
    }

    if (m_hio.ChkSphChkTimer()) {
        OSStartStopwatch(&s_sph_chk_sw);
    }
    #endif

    bool ret = false;
    psphchk->Init();

    cBgS_ChkElm* elm = m_chk_element;
    for (int i = 0; i < 0x100; i++) {
        if (elm->ChkUsed() && !elm->m_bgw_base_ptr->ChkNotReady()) {
            if (!psphchk->ChkSameActorPid(elm->m_actor_id)) {
                dBgW_Base* pbgw = elm->m_bgw_base_ptr;

                if (pbgw->SphChk(psphchk, param_1)) {
                    psphchk->SetActorInfo(i, elm->m_bgw_base_ptr, elm->m_actor_id);
                    ret = true;
                }
            }
        }
        elm++;
    }

    #if DEBUG
    if (m_hio.ChkAcchWallTimer()) {
        OSStopStopwatch(&s_sph_chk_sw);
        OSDumpStopwatch(&s_sph_chk_sw);
    }
    #endif

    return ret;
}

void dBgS::MoveBgCrrPos(const cBgS_PolyInfo& i_poly, bool param_1, cXyz* i_pos, csXyz* i_angle,
                        csXyz* i_shapeAngle, bool param_5, bool param_6) {
    if (!param_1 || !i_poly.ChkBgIndex()) {
        return;
    }

    int bg_index = i_poly.GetBgIndex();
    JUT_ASSERT(2414, 0 <= bg_index && bg_index < 256);

    dBgW_Base* pbgw;
    if (m_chk_element[bg_index].ChkUsed()) {
        pbgw = m_chk_element[bg_index].m_bgw_base_ptr;

        if ((!param_5 || pbgw->chkStickWall()) && (!param_6 || pbgw->chkStickRoof()) &&
            pbgw->ChkMoveFlag() && ChkPolySafe(i_poly))
        {
            pbgw->CrrPos(i_poly, m_chk_element[bg_index].m_actor_ptr, param_1, i_pos, i_angle,
                          i_shapeAngle);
        }
    }
}

void dBgS::MoveBgTransPos(const cBgS_PolyInfo& i_poly, bool param_1, cXyz* i_pos, csXyz* i_angle,
                          csXyz* i_shapeAngle) {
    if (!param_1 || !i_poly.ChkBgIndex()) {
        return;
    }

    int bg_index = i_poly.GetBgIndex();
    JUT_ASSERT(2466, 0 <= bg_index && bg_index < 256);

    dBgW_Base* pbgw;
    if (m_chk_element[bg_index].ChkUsed()) {
        pbgw = m_chk_element[bg_index].m_bgw_base_ptr;

        if (pbgw->ChkMoveFlag() && ChkPolySafe(i_poly)) {
            pbgw->TransPos(i_poly, m_chk_element[bg_index].m_actor_ptr, param_1, i_pos, i_angle,
                            i_shapeAngle);
        }
    }
}

void dBgS::MoveBgMatrixCrrPos(const cBgS_PolyInfo& i_poly, bool param_1, cXyz* i_pos,
                              csXyz* i_angle, csXyz* i_shapeAngle) {
    if (!param_1 || !i_poly.ChkBgIndex()) {
        return;
    }

    int bg_index = i_poly.GetBgIndex();
    JUT_ASSERT(2509, 0 <= bg_index && bg_index < 256);

    dBgW_Base* pbgw;
    if (m_chk_element[bg_index].ChkUsed()) {
        pbgw = m_chk_element[bg_index].m_bgw_base_ptr;

        if (pbgw->ChkMoveFlag()) {
            pbgw->MatrixCrrPos(i_poly, m_chk_element[bg_index].m_actor_ptr, param_1, i_pos,
                                i_angle, i_shapeAngle);
        }
    }
}

void dBgS_MoveBGProc_Typical(dBgW* pbgw, void* i_actor_ptr, const cBgS_PolyInfo& i_poly,
                             bool param_3, cXyz* i_pos, csXyz* i_angle, csXyz* i_shapeAngle) {
    UNUSED(i_actor_ptr);
    UNUSED(i_poly);
    UNUSED(param_3);
    UNUSED(i_angle);
    UNUSED(i_shapeAngle);

    Mtx m;
    if (pbgw->GetOldInvMtx(m) == 0) {
        return;
    }

    cXyz move_old;
    cXyz move_pos;

    PSMTXMultVec(m, i_pos, &move_old);        
    PSMTXMultVec(pbgw->GetBaseMtxP(), &move_old, &move_pos);

    *i_pos = move_pos;
}

static void dBgS_MoveBGProc_RotY(dBgW* pbgw, void* i_actor_ptr, const cBgS_PolyInfo& i_poly,
                                 bool param_3, cXyz* i_pos, csXyz* i_angle, csXyz* i_shapeAngle) {
    UNUSED(i_actor_ptr);
    UNUSED(i_poly);
    UNUSED(param_3);
    UNUSED(i_pos);

    if (i_shapeAngle != NULL) {
        s16 y = pbgw->GetDiffShapeAngleY();

        if (i_shapeAngle != NULL) {
            i_shapeAngle->y += y;
        }

        if (i_angle != NULL) {
            i_angle->y += y;
        }
    }
}

void dBgS_MoveBGProc_TypicalRotY(dBgW* pbgw, void* i_actor_ptr, const cBgS_PolyInfo& i_poly,
                                 bool param_3, cXyz* i_pos, csXyz* i_angle, csXyz* i_shapeAngle) {
    dBgS_MoveBGProc_Typical(pbgw, i_actor_ptr, i_poly, param_3, i_pos, i_angle, i_shapeAngle);
    dBgS_MoveBGProc_RotY(pbgw, i_actor_ptr, i_poly, param_3, i_pos, i_angle, i_shapeAngle);
}

void dBgS_MoveBGProc_Trans(dBgW* pbgw, void* i_actor_ptr, const cBgS_PolyInfo& i_poly,
                           bool param_3, cXyz* i_pos, csXyz* i_angle, csXyz* i_shapeAngle) {
    UNUSED(i_actor_ptr);
    UNUSED(i_poly);
    UNUSED(param_3);
    UNUSED(i_angle);
    UNUSED(i_shapeAngle);

    cXyz trans;
    pbgw->GetTrans(&trans);

    PSVECAdd(i_pos, &trans, i_pos);
}

void dBgS::RideCallBack(const cBgS_PolyInfo& polyinfo, fopAc_ac_c* param_1) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(2682, 0 <= bg_index && bg_index < 256);

    dBgW_Base* pbgw = m_chk_element[bg_index].m_bgw_base_ptr;
    if (!pbgw->ChkUsed()) {
        OS_REPORT_ERROR("dBgS::RideCallBack() Error\n");
    } else {
        pbgw->CallRideCallBack(m_chk_element[bg_index].m_actor_ptr, param_1);
    }
}

void dBgS::ArrowStickCallBack(const cBgS_PolyInfo& polyinfo, fopAc_ac_c* param_1, cXyz& param_2) {
    int bg_index = polyinfo.GetBgIndex();
    JUT_ASSERT(2704, 0 <= bg_index && bg_index < 256);

    dBgW_Base* pbgw = m_chk_element[bg_index].m_bgw_base_ptr;
    if (!pbgw->ChkUsed()) {
        OS_REPORT_ERROR("dBgS::ArrowStickCallBack() Used Error\n");
    } else {
        pbgw->CallArrowStickCallBack(m_chk_element[bg_index].m_actor_ptr, param_1, param_2);
    }
}

fopAc_ac_c* dBgS::PushPullCallBack(const cBgS_PolyInfo& param_0, fopAc_ac_c* i_pushActor, s16 i_angle,
                            dBgW_Base::PushPullLabel i_label) {
    int bg_index = param_0.GetBgIndex();
    JUT_ASSERT(2733, 0 <= bg_index && bg_index < 256);

    dBgW_Base* pbgw = m_chk_element[bg_index].m_bgw_base_ptr;
    if (!pbgw->ChkUsed()) {
        OS_REPORT_ERROR("dBgS::PushPullCallBack() Error\n");
        return NULL;
    }

    fopAc_ac_c* bg_actor = m_chk_element[bg_index].m_actor_ptr;
    if (bg_actor == NULL) {
        return NULL;
    }

    dBgW::PushPull_CallBack callback = pbgw->GetPushPullCallback();
    if (callback == NULL) {
        return NULL;
    }

    return callback(bg_actor, i_pushActor, i_angle, i_label);
}

void dBgS::DebugDrawPoly(const dBgW_Base& param_1) {
    if (m_hio.ChkShapeDisp()) {
        param_1.DebugDraw();
    }
}

void dBgS::DrawPoly(const cBgS_PolyInfo& polyInfo, GXColor const& color) {
    dBgW_Base* pdBgw = dComIfG_Bgsp().GetBgWBasePointer(polyInfo);
    if (pdBgw != NULL) {
        pdBgw->DrawPoly(polyInfo, color);
    }
}

bool dBgS_CheckBWallPoly(const cBgS_PolyInfo& polyinfo) {
    cM3dGPla pla;

    bool rt = dComIfG_Bgsp().GetTriPla(polyinfo, &pla);
    if (!rt) {
        return false;
    }

    return cBgW_CheckBWall(pla.mNormal.y);
}

bool dBgS_CheckBGroundPoly(const cBgS_PolyInfo& polyinfo) {
    cM3dGPla pla;

    bool rt = dComIfG_Bgsp().GetTriPla(polyinfo, &pla);
    if (!rt) {
        return false;
    }

    return cBgW_CheckBGround(pla.mNormal.y);
}

bool dBgS_CheckBRoofPoly(const cBgS_PolyInfo& polyinfo) {
    cM3dGPla pla;

    bool rt = dComIfG_Bgsp().GetTriPla(polyinfo, &pla);
    if (!rt) {
        return false;
    }

    return cBgW_CheckBRoof(pla.mNormal.y);
}

f32 dBgS_GetNY(const cBgS_PolyInfo& polyinfo) {
    cM3dGPla pla;

    dComIfG_Bgsp().GetTriPla(polyinfo, &pla);
    return pla.mNormal.y;
}

// TODO hack just to get the inlines to appear for debug
static void dummy(dBgS& i_bgs) {
    i_bgs.LineCross(NULL);
    i_bgs.GroundCross(NULL);
}
