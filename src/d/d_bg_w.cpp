/**
 * d_bg_w.cpp
 * DZB Collision handling
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_w.h"
#include "SSystem/SComponent/c_bg_s_shdw_draw.h"
#include "SSystem/SComponent/c_m2d.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_bg_s_capt_poly.h"
#include "d/d_bg_s_sph_chk.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_horse.h"

#include "d/d_debug_viewer.h"

#if DEBUG
static void ASSERT_SOLDHEAP() {
    JKRHeap* currentHeap = JKRGetCurrentHeap();
    u32 type = currentHeap->getHeapType();
    if (type != 'SLID') {
        /**
         * ******* THIS IS A BUG *******
         * ******* THIS IS A BUG *******
         * ******* THIS IS A BUG *******
         * cBgW will crash if the current heap is not a solid heap.
         * Make sure to set the current heap to a solide heap.
         * The memory allocated here will never be freed.
         * This will cause memory leaks and certainly other bugs.
         * Please fix immediately. The actor's name should be listed in the blue message below.
         */
        OS_REPORT_ERROR(
            "＊＊＊＊＊＊＊Ａバグです＊＊＊＊＊＊＊\n"
            "＊＊＊＊＊＊＊Ａバグです＊＊＊＊＊＊＊\n"
            "＊＊＊＊＊＊＊Ａバグです＊＊＊＊＊＊＊\n"
            "cBgWはカレントヒープがソリッドヒープ以外だと破綻します。\n"
            "必ず、カレントヒープをソリッドヒープにしてください。\n"
            "ここでnewされた領域は二度と開放されることはありません。\n"
            "慢性的にメモリーリークを繰り返し、いずれ確実にＡバグを引き起こすことでしょう。\n"
            "必ず修正してください。この下の水色のメッセージにアクターの名前が書いてあるはずです。\n");
    }
}
#endif

cBgW_RwgElm::cBgW_RwgElm() {}

cBgW_RwgElm::~cBgW_RwgElm() {}

cBgW_NodeTree::cBgW_NodeTree() {}

cBgW_NodeTree::~cBgW_NodeTree() {}

cBgW::cBgW() {
    pm_bgd = NULL;
    mFlags = GLOBAL_e;
    mNeedsFullTransform = 1;
    mMoveCounter = 0;

    mTransVel.x = 0.0f;
    mTransVel.y = 0.0f;
    mTransVel.z = 0.0f;

    pm_tri = NULL;
    pm_rwg = NULL;
    pm_blk = NULL;
    pm_node_tree = NULL;
    pm_grp = NULL;
    pm_vtx_tbl = NULL;
    pm_base = NULL;

    MTXIdentity(m_inv_mtx);
    MTXIdentity(m_mtx);
    m_rootGrpIdx = 0xFFFF;
    field_0x91 = 0;
}

cBgW::~cBgW() {
    FreeArea();
}

void cBgW::FreeArea() {
    pm_tri = NULL;
    pm_rwg = NULL;
    pm_node_tree = NULL;
    pm_blk = NULL;
    pm_grp = NULL;
    pm_vtx_tbl = NULL;
}

void cBgW::GlobalVtx() {
    if (pm_base != NULL) {
        if (!mNeedsFullTransform) {
            for (int i = 0; i < pm_bgd->m_v_num; i++) {
                PSVECAdd(&pm_vtx_tbl[i], &mTransVel, &pm_vtx_tbl[i]);
            }
        } else {
            // TODO: supposed to be *pm_base, type must be wrong
            JUT_ASSERT(201,
                !isnan((*pm_base)[0][0]) &&
                !isnan((*pm_base)[0][1]) &&
                !isnan((*pm_base)[0][2]) &&
                !isnan((*pm_base)[0][3]) &&
                !isnan((*pm_base)[1][0]) &&
                !isnan((*pm_base)[1][1]) &&
                !isnan((*pm_base)[1][2]) &&
                !isnan((*pm_base)[1][3]) &&
                !isnan((*pm_base)[2][0]) &&
                !isnan((*pm_base)[2][1]) &&
                !isnan((*pm_base)[2][2]) &&
                !isnan((*pm_base)[2][3]));

            for (int i = 0; i < pm_bgd->m_v_num; i++) {
                PSMTXMultVec(*pm_base, &pm_bgd->m_v_tbl[i], &pm_vtx_tbl[i]);
            }
        }
    }
}

#if DEBUG
// stripped func, needed for string data
void cBgW::DebugLocalPos() {
    OSReport("cBgW::DebugLocalPos() {%ff, %ff, %ff}, {%ff, %ff, %ff}\n");
}
#endif

bool cBgW::SetVtx() {
    #if DEBUG
    ASSERT_SOLDHEAP();
    #endif

    if (mFlags & NO_VTX_TBL_e) {
        pm_vtx_tbl = NULL;
    } else if (mFlags & MOVE_BG_e) {
        pm_vtx_tbl = (cBgD_Vtx_t*)JKR_NEW_ARRAY(Vec, pm_bgd->m_v_num);

        if (pm_vtx_tbl == NULL) {
            OS_REPORT_ERROR("c_bg_w.cpp[%d] メモリ確保できません。\n", 272);
            return true;
        }

        if (mFlags & CBGW_UNK_FLAG_40) {
            for (int i = 0; i < pm_bgd->m_v_num; i++) {
                pm_vtx_tbl[i].x = 0.0f;
                pm_vtx_tbl[i].y = 0.0f;
                pm_vtx_tbl[i].z = 0.0f;
            }
        }

        GlobalVtx();
    } else {
        pm_vtx_tbl = pm_bgd->m_v_tbl;
    }

    return false;
}

void cBgW::CalcPlane() {
    cBgD_Tri_t* tri = pm_bgd->m_t_tbl;

    if (pm_vtx_tbl != NULL) {
        if (!mNeedsFullTransform) {
            for (int i = 0; i < pm_bgd->m_t_num; i++) {
                pm_tri[i].m_plane.mD -= PSVECDotProduct(&pm_tri[i].m_plane.mNormal, &mTransVel);
            }
        } else {
            for (int i = 0; i < pm_bgd->m_t_num; i++) {
#if TARGET_LITTLE_ENDIAN
                Vec copy1 = pm_vtx_tbl[tri[i].m_vtx_idx0];
                Vec copy2 = pm_vtx_tbl[tri[i].m_vtx_idx1];
                Vec copy3 = pm_vtx_tbl[tri[i].m_vtx_idx2];
                pm_tri[i].m_plane.SetupFrom3Vtx(
                    &copy1,
                    &copy2,
                    &copy3);
#else
                pm_tri[i].m_plane.SetupFrom3Vtx(&pm_vtx_tbl[tri[i].m_vtx_idx0],
                                                &pm_vtx_tbl[tri[i].m_vtx_idx1],
                                                &pm_vtx_tbl[tri[i].m_vtx_idx2]);
                
                #if DEBUG
                cXyz vec;
                vec = pm_tri[i].m_plane.mNormal;
                
                JUT_ASSERT(327, -1.1f <= vec.x && vec.x <= 1.1f);
                JUT_ASSERT(328, -1.1f <= vec.y && vec.y <= 1.1f);
                JUT_ASSERT(329, -1.1f <= vec.z && vec.z <= 1.1f);
                #endif
#endif
            }
        }
    }
}

bool cBgW::SetTri() {
    #if DEBUG
    ASSERT_SOLDHEAP();
    #endif

    pm_tri = JKR_NEW_ARRAY(cBgW_TriElm, pm_bgd->m_t_num);

    if (pm_tri == NULL) {
        OS_REPORT_ERROR("c_bg_w.cpp[%d] メモリ確保できません\n", 353);
        return true;
    }

    CalcPlane();
    return false;
}

cBgW_TriElm::~cBgW_TriElm() {}

cBgW_TriElm::cBgW_TriElm() {}

void cBgW::BlckConnect(u16* pstart_index, int* pprev_index, int index) {
    if (*pstart_index == 0xFFFF) {
        *pstart_index = index;
    }

    if (*pprev_index != 0xFFFF) {
        pm_rwg[*pprev_index].m_next = index;
    }

    *pprev_index = index;
    pm_rwg[*pprev_index].m_next = 0xFFFF;
}

void cBgW::ClassifyPlane() {
    if (pm_vtx_tbl == NULL) {
        return;
    }

    for (int i = 0; i < pm_bgd->m_b_num; i++) {
        int start_idx = pm_bgd->m_b_tbl[i].field_0x0;
        int end_idx;
        if (i != pm_bgd->m_b_num - 1) {
            end_idx = pm_bgd->m_b_tbl[i + 1].field_0x0 - 1;
        } else {
            end_idx = pm_bgd->m_t_num - 1;
        }
        pm_blk[i].m_roof_idx = 0xFFFF;
        pm_blk[i].m_wall_idx = 0xFFFF;
        pm_blk[i].m_gnd_idx = 0xFFFF;

        int blk_roof_idx, blk_wall_idx, blk_gnd_idx;
        blk_roof_idx = blk_wall_idx = blk_gnd_idx = 0xFFFF;

        for (int j = start_idx; j <= end_idx; j++) {
            f32 norm_y = pm_tri[j].m_plane.GetNP()->y;

            if (!cM3d_IsZero(pm_tri[j].m_plane.GetNP()->x) ||
                !cM3d_IsZero(pm_tri[j].m_plane.GetNP()->y) ||
                !cM3d_IsZero(pm_tri[j].m_plane.GetNP()->z))
            {
                if (cBgW_CheckBGround(norm_y)) {
                    BlckConnect(&pm_blk[i].m_gnd_idx, &blk_gnd_idx, j);
                } else if (cBgW_CheckBRoof(norm_y)) {
                    if (ChkRoofRegist()) {
                        BlckConnect(&pm_blk[i].m_roof_idx, &blk_roof_idx, j);
                    }
                } else {
                    BlckConnect(&pm_blk[i].m_wall_idx, &blk_wall_idx, j);
                }
            }
        }
    }
}

void cBgW::MakeBlckTransMinMax(cXyz* pmin, cXyz* pmax) {
    PSVECAdd(pmin, &mTransVel, pmin);
    PSVECAdd(pmax, &mTransVel, pmax);
}

void cBgW::MakeBlckMinMax(int vtx_index, cXyz* pmin, cXyz* pmax) {
    cBgD_Vtx_t* vtx = &pm_vtx_tbl[vtx_index];

    if (pmin->x > vtx->x) {
        pmin->x = vtx->x;
    }

    if (pmax->x < vtx->x) {
        pmax->x = vtx->x;
    }

    if (pmin->y > vtx->y) {
        pmin->y = vtx->y;
    }

    if (pmax->y < vtx->y) {
        pmax->y = vtx->y;
    }

    if (pmin->z > vtx->z) {
        pmin->z = vtx->z;
    }

    if (pmax->z < vtx->z) {
        pmax->z = vtx->z;
    }
}

void cBgW::MakeBlckBnd(int blck_index, cXyz* pmin, cXyz* pmax) {
    int start = pm_bgd->m_b_tbl[blck_index].field_0x0;
    int max;

    if (blck_index != pm_bgd->m_b_num - 1) {
        max = pm_bgd->m_b_tbl[blck_index + 1].field_0x0 - 1;
    } else {
        max = pm_bgd->m_t_num - 1;
    }

    if (mNeedsFullTransform == 0) {
        MakeBlckTransMinMax(pmin, pmax);
    } else {
        pmin->x = pmin->y = pmin->z = G_CM3D_F_INF;
        pmax->x = pmax->y = pmax->z =-G_CM3D_F_INF;

        for (int i = start; i <= max; i++) {
            MakeBlckMinMax(pm_bgd->m_t_tbl[i].m_vtx_idx0, pmin, pmax);
            MakeBlckMinMax(pm_bgd->m_t_tbl[i].m_vtx_idx1, pmin, pmax);
            MakeBlckMinMax(pm_bgd->m_t_tbl[i].m_vtx_idx2, pmin, pmax);
        }

        pmin->x -= 1.0f;
        pmin->y -= 1.0f;
        pmin->z -= 1.0f;
        pmax->x += 1.0f;
        pmax->y += 1.0f;
        pmax->z += 1.0f;
    }
}

void cBgW::MakeNodeTreeRp(int node_index) {
    cBgD_Tree_t* pnode = &pm_bgd->m_tree_tbl[node_index];

    if (pnode->m_flag & 1) {
        int child_idx = pnode->m_id[0];

        if (child_idx != 0xFFFF) {
            MakeBlckBnd(child_idx, pm_node_tree[node_index].GetMinP(), pm_node_tree[node_index].GetMaxP());
        }
    } else {
        pm_node_tree[node_index].ClearForMinMax();

        for (int i = 0; i < 8; i++) {
            int child_idx = pnode->m_id[i];

            if (child_idx != 0xFFFF) {
                MakeNodeTreeRp(child_idx);
                pm_node_tree[node_index].SetMinMax(*pm_node_tree[child_idx].GetMinP());
                pm_node_tree[node_index].SetMinMax(*pm_node_tree[child_idx].GetMaxP());
            }
        }
    }
}

void cBgW::MakeNodeTreeGrpRp(int g) {
    if (pm_bgd->m_g_tbl[g].m_tree_idx != 0xFFFF) {
        MakeNodeTreeRp(pm_bgd->m_g_tbl[g].m_tree_idx);
        pm_grp[g].m_aab.SetMin(
            *pm_node_tree[pm_bgd->m_g_tbl[g].m_tree_idx].GetMinP());
        pm_grp[g].m_aab.SetMax(
            *pm_node_tree[pm_bgd->m_g_tbl[g].m_tree_idx].GetMaxP());
    }

    int child_idx = pm_bgd->m_g_tbl[g].m_first_child;
    while (true) {
        if (child_idx == 0xFFFF)
            break;
        MakeNodeTreeGrpRp(child_idx);
        pm_grp[g].m_aab.SetMin(*pm_grp[child_idx].m_aab.GetMinP());
        pm_grp[g].m_aab.SetMax(*pm_grp[child_idx].m_aab.GetMaxP());
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }
}

void cBgW::MakeNodeTree() {
    if (pm_vtx_tbl == NULL) {
        for (int i = 0; i < pm_bgd->m_g_num; i++) {
            if (pm_bgd->m_g_tbl[i].m_parent == 0xFFFF) {
                m_rootGrpIdx = i;
                return;
            }
        }
    } else {
        for (int i = 0; i < pm_bgd->m_g_num; i++) {
            pm_grp[i].m_aab.ClearForMinMax();
        }

        for (int i = 0; i < pm_bgd->m_g_num; i++) {
            if (pm_bgd->m_g_tbl[i].m_parent == 0xFFFF) {
                m_rootGrpIdx = i;
                MakeNodeTreeGrpRp(i);
                return;
            }
        }
    }
}

bool cBgW::ChkMemoryError() {
    if (pm_tri == NULL || pm_rwg == NULL || pm_blk == NULL || pm_node_tree == NULL ||
        pm_grp == NULL)
    {
        return true;
    }

    return false;
}

bool cBgW::Set(cBgD_t* pbgd, u32 flags, Mtx* pbase_mtx) {
    #if DEBUG
    ASSERT_SOLDHEAP();
    #endif

    mFlags = GLOBAL_e;
    pm_vtx_tbl = NULL;
    pm_tri = NULL;
    pm_rwg = NULL;
    pm_blk = NULL;
    pm_node_tree = NULL;
    pm_grp = NULL;
    mMoveCounter = (int)cM_rndF(128.0f) & 0xFF;

    if (pbgd == NULL) {
        return true;
    }

    mFlags = flags;
    if (mFlags & GLOBAL_e) {
        pm_base = NULL;
        MTXIdentity(m_inv_mtx);
        MTXIdentity(m_mtx);
    } else {
        pm_base = pbase_mtx;
        PSMTXCopy(*pm_base, m_inv_mtx);
        PSMTXCopy(*pm_base, m_mtx);
    }

    pm_bgd = pbgd;

    if (SetVtx() || SetTri()) {
        FreeArea();
        OS_REPORT_ERROR("cBgW::Set() メモリ足りません。\n");
        return true;
    }

    pm_rwg = JKR_NEW_ARRAY(cBgW_RwgElm, pm_bgd->m_t_num);
    if (pm_rwg == NULL) {
        OS_REPORT_ERROR("c_bg_w.cpp[%d] メモリ確保できません。\n", 892);
        FreeArea();
        return true;
    }

    pm_blk = JKR_NEW_ARRAY(cBgW_BlkElm, pm_bgd->m_b_num);
    if (pm_blk == NULL) {
        OS_REPORT_ERROR("c_bg_w.cpp[%d] メモリ確保できません。\n", 914);
        FreeArea();
        return true;
    }

    pm_node_tree = JKR_NEW_ARRAY(cBgW_NodeTree, pm_bgd->m_tree_num);
    if (pm_node_tree == NULL) {
        OS_REPORT_ERROR("c_bg_w.cpp[%d] メモリ確保できません\n", 936);
        FreeArea();
        return true;
    }

    pm_grp = JKR_NEW_ARRAY(cBgW_GrpElm, pm_bgd->m_g_num);
    if (pm_grp == NULL) {
        OS_REPORT_ERROR("c_bg_w.cpp[%d] メモリ確保できません\n", 958);
        FreeArea();
        return true;
    }

    ClassifyPlane();
    mNeedsFullTransform = 1;
    MakeNodeTree();
    return false;
}

cBgW_GrpElm::~cBgW_GrpElm() {}

cBgW_GrpElm::cBgW_GrpElm() {}

bool cBgW::RwgLineCheck(u16 poly_index, cBgS_LinChk* plinchk) {
    bool chk = false;
    cM3dGTri tri;

    while (true) {
        cBgD_Tri_t* tri_t = &pm_bgd->m_t_tbl[poly_index];

        tri.setBg(&pm_vtx_tbl[tri_t->m_vtx_idx0], &pm_vtx_tbl[tri_t->m_vtx_idx1],
                  &pm_vtx_tbl[tri_t->m_vtx_idx2], &pm_tri[poly_index].m_plane);

        cXyz cross;
        if (tri.cross(plinchk->GetLinP(), &cross, plinchk->ChkFrontFlag(),
                      plinchk->ChkBackFlag()))
        {
            if (!ChkPolyThrough(poly_index, plinchk->GetPolyPassChk())) {
                JUT_ASSERT(1080, !isnan(cross.x));
                JUT_ASSERT(1081, !isnan(cross.y));
                JUT_ASSERT(1082, !isnan(cross.z));
                JUT_ASSERT(1088, -INF < cross.x && cross.x < INF && -INF < cross.y && cross.y < INF && -INF < cross.z && cross.z < INF);

                plinchk->SetCross(cross);
                plinchk->SetPolyIndex(poly_index);
                chk = true;
            }
        }

        cBgW_RwgElm* element = &pm_rwg[poly_index];
        if (element->m_next == 0xFFFF) {
            break;
        }

        poly_index = element->m_next;
    }

    return chk;
}

bool cBgW::LineCheckRp(cBgS_LinChk* plinchk, int node_index) {
    cBgW_NodeTree* node = &pm_node_tree[node_index];

    if (!cM3d_Cross_MinMaxBoxLine(node->GetMinP(), node->GetMaxP(),
                                  plinchk->GetLinP()->GetStartP(),
                                  plinchk->GetLinP()->GetEndP()))
    {
        return false;
    }

    cBgD_Tree_t* pnode = &pm_bgd->m_tree_tbl[node_index];
    bool chk = false;

    if (pnode->m_flag & 1) {
        JUT_ASSERT(1140, pnode->m_id[0] != 0xffff);

        JUT_ASSERT(1142, pnode->m_id[0] >= 0 && pnode->m_id[0] < pm_bgd->m_b_num);

        if (plinchk->GetPreWallChk() && pm_blk[pnode->m_id[0]].m_wall_idx != 0xFFFF &&
            RwgLineCheck(pm_blk[pnode->m_id[0]].m_wall_idx, plinchk))
        {
            chk = true;
        }
        if (plinchk->GetPreGroundChk() && pm_blk[pnode->m_id[0]].m_gnd_idx != 0xFFFF &&
            RwgLineCheck(pm_blk[pnode->m_id[0]].m_gnd_idx, plinchk))
        {
            chk = true;
        }
        if (plinchk->GetPreRoofChk() && pm_blk[pnode->m_id[0]].m_roof_idx != 0xFFFF &&
            RwgLineCheck(pm_blk[pnode->m_id[0]].m_roof_idx, plinchk))
        {
            chk = true;
        }

        return chk;
    }

    for (int i = 0; i < 8; i++) {
        if (pnode->m_id[i] != 0xFFFF && LineCheckRp(plinchk, pnode->m_id[i])) {
            chk = true;
        }
    }

    return chk;
}

bool cBgW::LineCheckGrpRp(cBgS_LinChk* plinchk, int g, int depth) {
    if (!pm_grp[g].m_aab.Cross(plinchk->GetLinP())) {
        return false;
    }

    if (ChkGrpThrough(g, plinchk->GetGrpPassChk(), depth)) {
        return false;
    }

    bool chk = false;

    if (pm_bgd->m_g_tbl[g].m_tree_idx != 0xFFFF &&
        LineCheckRp(plinchk, pm_bgd->m_g_tbl[g].m_tree_idx))
    {
        chk = true;
    }

    int child_idx = pm_bgd->m_g_tbl[g].m_first_child;
    depth++;

    while (true) {
        if (child_idx == 0xFFFF)
            break;

        if (LineCheckGrpRp(plinchk, child_idx, depth)) {
            chk = true;
        }
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return chk;
}

bool cBgW::LineCheck(cBgS_LinChk* plinchk) {
    return LineCheckGrpRp(plinchk, m_rootGrpIdx, 1);
}

bool cBgW::RwgGroundCheckCommon(f32 cy, u16 poly_index, cBgS_GndChk* pgndchk) {
    if (cy < pgndchk->GetPointP().y && cy > pgndchk->GetNowY()) {
        cBgD_Tri_t* tri = &pm_bgd->m_t_tbl[poly_index];
        if (cM3d_CrossY_Tri_Front(pm_vtx_tbl[tri->m_vtx_idx0], pm_vtx_tbl[tri->m_vtx_idx1],
                                  pm_vtx_tbl[tri->m_vtx_idx2], (const Vec*)&pgndchk->GetPointP()))
        {
            if (!ChkPolyThrough(poly_index, pgndchk->GetPolyPassChk())) {
                pgndchk->SetNowY(cy);
                pgndchk->SetPolyIndex(poly_index);

                JUT_ASSERT(1278, !isnan(cy));

                JUT_ASSERT(1280, -INF < cy && cy < INF);
                return true;
            }
        }
    }

    return false;
}

bool cBgW::RwgGroundCheckGnd(u16 poly_index, cBgS_GndChk* pgndchk) {
    bool chk = false;
    while (true) {
        cBgW_RwgElm* rwg = &pm_rwg[poly_index];
        f32 tri_y = pm_tri[poly_index].m_plane.getCrossY_NonIsZero(pgndchk->GetPointP());
        if (RwgGroundCheckCommon(tri_y, (u32)poly_index, pgndchk)) {
            chk = true;
        }

        if (rwg->m_next == 0xFFFF)
            break;

        poly_index = rwg->m_next;
    }
    return chk;
}

bool cBgW::RwgGroundCheckWall(u16 poly_index, cBgS_GndChk* pgndchk) {
    bool chk = false;
    while (true) {
        cBgW_TriElm* tri = &pm_tri[poly_index];
        cBgW_RwgElm* rwg = &pm_rwg[poly_index];
        if (tri->m_plane.mNormal.y >= 0.014f) {
            f32 tri_y = tri->m_plane.getCrossY_NonIsZero(pgndchk->GetPointP());
            if (RwgGroundCheckCommon(tri_y, (u32)poly_index, pgndchk)) {
                chk = true;
            }
        }

        if (rwg->m_next == 0xFFFF)
            break;
        poly_index = rwg->m_next;
    }

    return chk;
}

bool cBgW::GroundCrossRp(cBgS_GndChk* pgndchk, int node_index) {
    JUT_ASSERT(1376, 0 <= node_index && node_index < pm_bgd->m_tree_num);

    bool chk = false;
    cBgD_Tree_t* pnode = &pm_bgd->m_tree_tbl[node_index];
    if ((pnode->m_flag & 1)) {
        JUT_ASSERT(1386, pnode->m_id[0] != 0xffff);

        JUT_ASSERT(1387, pnode->m_id[0] >= 0 && pnode->m_id[0] < pm_bgd->m_b_num);

        if (pm_blk[pnode->m_id[0]].m_gnd_idx != 0xFFFF &&
            RwgGroundCheckGnd(pm_blk[pnode->m_id[0]].m_gnd_idx, pgndchk))
        {
            chk = true;
        }

        if (pgndchk->GetWallPrecheck() && pm_blk[pnode->m_id[0]].m_wall_idx != 0xFFFF &&
            RwgGroundCheckWall(pm_blk[pnode->m_id[0]].m_wall_idx, pgndchk))
        {
            chk = true;
        }
        return chk;
    }

    for (int i = 0; i < 8; i++) {
        if (pnode->m_id[i] != 0xFFFF) {
            cM3dGAab* aab = &pm_node_tree[pnode->m_id[i]];
            if (aab->CrossY(&pgndchk->GetPointP())) {
                if (aab->UnderPlaneYUnder(pgndchk->GetPointP().y)) {
                    if (!aab->TopPlaneYUnder(pgndchk->GetNowY()) &&
                        GroundCrossRp(pgndchk, pnode->m_id[i]))
                    {
                        chk = true;
                    }
                }
            }
        }
    }

    return chk;
}

bool cBgW::GroundCrossGrpRp(cBgS_GndChk* pgndchk, int g, int depth) {
    JUT_ASSERT(1441, 0 <= g && g < pm_bgd->m_g_num);

    cBgW_GrpElm* grp = &pm_grp[g];
    if (!grp->m_aab.CrossY(&pgndchk->GetPointP()) ||
        !grp->m_aab.UnderPlaneYUnder(pgndchk->GetPointP().y) ||
        grp->m_aab.TopPlaneYUnder(pgndchk->GetNowY()))
    {
        return false;
    }

    if (ChkGrpThrough(g, pgndchk->GetGrpPassChk(), depth)) {
        return false;
    }

    bool chk = false;

    if (pm_bgd->m_g_tbl[g].m_tree_idx != 0xFFFF &&
        GroundCrossRp(pgndchk, pm_bgd->m_g_tbl[g].m_tree_idx))
    {
        chk = true;
    }

    int child_idx = pm_bgd->m_g_tbl[g].m_first_child;
    depth++;
    while (true) {
        if (child_idx == 0xFFFF)
            break;

        if (GroundCrossGrpRp(pgndchk, child_idx, depth)) {
            chk = true;
        }

        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return chk;
}

bool cBgW::GroundCross(cBgS_GndChk* pgndchk) {
    return GroundCrossGrpRp(pgndchk, m_rootGrpIdx, 1);
}

void cBgW::CopyOldMtx() {
    if (pm_base != NULL) {
        PSMTXCopy(m_mtx, m_inv_mtx);
        PSMTXCopy(*pm_base, m_mtx);
    }
}

void cBgW::Move() {
    if (!ChkLock() && (mFlags & MOVE_BG_e)) {
        if (!ChkNoCalcVtx()) {
            if (mMoveCounter >= 0xFF || m_mtx[0][0] != (*pm_base)[0][0] ||
                m_mtx[0][1] != (*pm_base)[0][1] || m_mtx[0][2] != (*pm_base)[0][2] ||
                m_mtx[1][0] != (*pm_base)[1][0] || m_mtx[1][1] != (*pm_base)[1][1] ||
                m_mtx[1][2] != (*pm_base)[1][2] || m_mtx[2][0] != (*pm_base)[2][0] ||
                m_mtx[2][1] != (*pm_base)[2][1] || m_mtx[2][2] != (*pm_base)[2][2])
            {
                mNeedsFullTransform = 1;
            } else if (m_mtx[0][3] == (*pm_base)[0][3] && m_mtx[1][3] == (*pm_base)[1][3] &&
                       m_mtx[2][3] == (*pm_base)[2][3])
            {
                PSMTXCopy(*pm_base, m_inv_mtx);
                if (!ChkFlush()) {
                    return;
                }
            } else {
                mTransVel.x = (*pm_base)[0][3] - m_mtx[0][3];
                mTransVel.y = (*pm_base)[1][3] - m_mtx[1][3];
                mTransVel.z = (*pm_base)[2][3] - m_mtx[2][3];
                mNeedsFullTransform = 0;
            }

            if (mMoveCounter >= 0xFF) {
                mMoveCounter = 0;
            } else {
                mMoveCounter++;
            }
            GlobalVtx();
        }

        CopyOldMtx();
        CalcPlane();
        ClassifyPlane();
        MakeNodeTree();
    }
}

static void dummyString() {
    OSReport("Label:%s\n");
    OSReport("cBgW:%x\n");
    OSReport("vtx num:%d\n");
    OSReport("(%f,%f,%f)\n");
}

void cBgW::RwgShdwDraw(int index, cBgS_ShdwDraw* pshdw) {
    cBgW_RwgElm* rwg;

    while (true) {
        rwg = &pm_rwg[index];
        if (!ChkShdwDrawThrough(index, pshdw->GetPolyPassChk())) {
            #if DEBUG
            pshdw->field_0x34++;
            #endif

            (pshdw->mCallbackFun)(pshdw, pm_vtx_tbl, pm_bgd->m_t_tbl[index].m_vtx_idx0,
                                   pm_bgd->m_t_tbl[index].m_vtx_idx1,
                                   pm_bgd->m_t_tbl[index].m_vtx_idx2, &pm_tri[index].m_plane);
        }

        if (rwg->m_next == 0xFFFF)
            break;

        index = rwg->m_next;
        JUT_ASSERT(1681, 0 <= index && index < pm_bgd->m_t_num);
    }
}

void cBgW::ShdwDrawRp(cBgS_ShdwDraw* pshdw, int node_index) {
    if (pm_node_tree[node_index].Cross(pshdw->GetBndP())) {
        cBgD_Tree_t* tree = &pm_bgd->m_tree_tbl[node_index];
        if ((tree->m_flag & 1)) {
            if (pm_blk[tree->m_id[0]].m_wall_idx != 0xFFFF) {
                RwgShdwDraw(pm_blk[tree->m_id[0]].m_wall_idx, pshdw);
            }
            if (pm_blk[tree->m_id[0]].m_roof_idx != 0xFFFF) {
                RwgShdwDraw(pm_blk[tree->m_id[0]].m_roof_idx, pshdw);
            }
            if (pm_blk[tree->m_id[0]].m_gnd_idx != 0xFFFF) {
                RwgShdwDraw(pm_blk[tree->m_id[0]].m_gnd_idx, pshdw);
            }
        } else {
            for (int i = 0; i < 8; i++) {
                if (tree->m_id[i] != 0xFFFF) {
                    ShdwDrawRp(pshdw, tree->m_id[i]);
                }
            }
        }
    }
}

void cBgW::ShdwDrawGrpRp(cBgS_ShdwDraw* pshdw, int node_index) {
    int child_idx;

    if (pm_grp[node_index].m_aab.Cross(pshdw->GetBndP())) {
        if (pm_bgd->m_g_tbl[node_index].m_tree_idx != 0xFFFF) {
            ShdwDrawRp(pshdw, pm_bgd->m_g_tbl[node_index].m_tree_idx);
        }

        child_idx = pm_bgd->m_g_tbl[node_index].m_first_child;
        while (true) {
            if (child_idx == 0xFFFF) {
                break;
            }
            ShdwDrawGrpRp(pshdw, child_idx);
            child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
        }
    }
}

void cBgW::ShdwDraw(cBgS_ShdwDraw* pshdw) {
    ShdwDrawGrpRp(pshdw, m_rootGrpIdx);
}

bool cBgW::ChkPolyThrough(int param_0, cBgS_PolyPassChk* param_1) {
    return false;
}

bool cBgW::ChkShdwDrawThrough(int param_0, cBgS_PolyPassChk* param_1) {
    return ChkPolyThrough(param_0, param_1);
}

bool cBgW::ChkGrpThrough(int param_0, cBgS_GrpPassChk* param_1, int param_2) {
    return false;
}

int cBgW::GetGrpRoomIndex(const cBgS_PolyInfo& polyinfo) const {
    int poly_index = polyinfo.GetPolyIndex();
    JUT_ASSERT(1848, 0 <= poly_index && poly_index < pm_bgd->m_t_num);

    int grp_index = GetTriGrp(poly_index);
    JUT_ASSERT(1850, 0 <= grp_index && grp_index < pm_bgd->m_g_num);

    if (pm_bgd->m_g_tbl[grp_index].m_parent == 0xFFFF || pm_bgd->m_g_tbl[pm_bgd->m_g_tbl[grp_index].m_parent].m_parent == 0xFFFF) {
        return 0xFF;
    }

    int room_index = pm_bgd->m_g_tbl[pm_bgd->m_g_tbl[pm_bgd->m_g_tbl[grp_index].m_parent].m_parent].m_room_id;
    if (room_index >= 0xFF) {
        room_index = 0xFF;
    }

    return room_index;
}

cM3dGAab* cBgW::GetBnd() const {
    return &pm_grp[m_rootGrpIdx].m_aab;
}

void cBgW::GetTrans(cXyz* ptrans) const {
    ptrans->x = (*pm_base)[0][3] - m_inv_mtx[0][3];
    ptrans->y = (*pm_base)[1][3] - m_inv_mtx[1][3];
    ptrans->z = (*pm_base)[2][3] - m_inv_mtx[2][3];
}

bool cBgW::GetTriPnt(const cBgS_PolyInfo& polyinfo, cXyz* ppnt0, cXyz* ppnt1, cXyz* ppnt2) const {
    JUT_ASSERT(1923, pm_bgd != NULL);

    int poly_index = polyinfo.GetPolyIndex();
    cBgD_Tri_t* tri = &pm_bgd->m_t_tbl[poly_index];

    ppnt0->set(pm_vtx_tbl[tri->m_vtx_idx0]);
    ppnt1->set(pm_vtx_tbl[tri->m_vtx_idx1]);
    ppnt2->set(pm_vtx_tbl[tri->m_vtx_idx2]);
    return true;
}

void cBgW::GetTopUnder(f32* ptop, f32* punder) const {
    *punder = pm_grp[m_rootGrpIdx].m_aab.GetMinP()->y;
    *ptop = pm_grp[m_rootGrpIdx].m_aab.GetMaxP()->y;
}

cM3dGPla cBgW::GetTriPla(const cBgS_PolyInfo& polyinfo) const {
    int poly_index = polyinfo.GetPolyIndex();
    JUT_ASSERT(1956, 0 <= poly_index && poly_index < pm_bgd->m_t_num);
    return pm_tri[poly_index].m_plane;
}

u32 cBgW::GetGrpInf(const cBgS_PolyInfo& polyinfo) const {
    int poly_index = polyinfo.GetPolyIndex();
    JUT_ASSERT(1969, 0 <= poly_index && poly_index < pm_bgd->m_t_num);

    int grp_id = GetTriGrp(poly_index);
    JUT_ASSERT(1971, 0 <= grp_id && grp_id < pm_bgd->m_g_num);

    return pm_bgd->m_g_tbl[grp_id].m_info;
}

int cBgW::GetPolyInfId(int poly_index) const {
    JUT_ASSERT(1983, 0 <= poly_index && poly_index < pm_bgd->m_t_num);

    return pm_bgd->m_t_tbl[poly_index].m_id;
}

u32 cBgW::GetPolyInf0(int poly_index, u32 mask, u32 shift) const {
    int id = GetPolyInfId(poly_index);
    JUT_ASSERT(2024, 0 <= id && id < pm_bgd->m_ti_num);

    return (pm_bgd->m_ti_tbl[id].m_info0 & mask) >> shift;
}

u32 cBgW::GetMaskPolyInf0_NoShift(int poly_index, u32 mask) const {
    int id = GetPolyInfId(poly_index);
    JUT_ASSERT(2039, 0 <= id && id < pm_bgd->m_ti_num);

    return (pm_bgd->m_ti_tbl[id].m_info0 & mask);
}

int dBgW::GetExitId(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf0(polyinfo.GetPolyIndex(), 0x3F, 0);
}

int dBgW::GetPolyColor(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf0(polyinfo.GetPolyIndex(), 0x3FC0, 6);
}

BOOL dBgW::GetHorseNoEntry(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf0(polyinfo.GetPolyIndex(), 0x200000, 21);
}

int dBgW::GetSpecialCode(const cBgS_PolyInfo& polyinfo) {
    return GetSpecialCode(polyinfo.GetPolyIndex());
}

int dBgW::GetSpecialCode(int poly_index) {
    return GetPolyInf0(poly_index, 0xF000000, 24);
}

int dBgW::GetMagnetCode(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf0(polyinfo.GetPolyIndex(), 0x30000000, 28);
}

int dBgW::GetMonkeyBarsCode(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf0(polyinfo.GetPolyIndex(), 0x80000000, 31);
}

u32 dBgW::GetPolyObjThrough(int poly_index) {
    return GetMaskPolyInf0_NoShift(poly_index, 0x4000);
}

u32 dBgW::GetPolyCamThrough(int poly_index) {
    return GetMaskPolyInf0_NoShift(poly_index, 0x8000);
}

u32 dBgW::GetPolyLinkThrough(int poly_index) {
    return GetMaskPolyInf0_NoShift(poly_index, 0x10000);
}

u32 dBgW::GetPolyArrowThrough(int poly_index) {
    return GetMaskPolyInf0_NoShift(poly_index, 0x20000);
}

u32 dBgW::GetPolyHSStick(int poly_index) {
    return GetMaskPolyInf0_NoShift(poly_index, 0x40000);
}

u32 dBgW::GetPolyBoomerangThrough(int poly_index) {
    return GetMaskPolyInf0_NoShift(poly_index, 0x80000);
}

u32 dBgW::GetPolyRopeThrough(int poly_index) {
    return GetMaskPolyInf0_NoShift(poly_index, 0x100000);
}

u32 dBgW::GetPolyBombThrough(int poly_index) {
    return GetMaskPolyInf0_NoShift(poly_index, 0x800000);
}

bool dBgW::GetShdwThrough(int poly_index) {
    return GetMaskPolyInf0_NoShift(poly_index, 0x400000) != 0;
}

u32 dBgW::GetUnderwaterRoofCode(int poly_index) {
    return GetMaskPolyInf0_NoShift(poly_index, 0x40000000);
}

u32 cBgW::GetPolyInf1(int poly_index, u32 mask, u32 shift) const {
    int id = GetPolyInfId(poly_index);
    JUT_ASSERT(2259, 0 <= id && id < pm_bgd->m_ti_num);

    return (pm_bgd->m_ti_tbl[id].m_info1 & mask) >> shift;
}

int dBgW::GetLinkNo(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf1(polyinfo.GetPolyIndex(), 0xFF, 0);
}

int dBgW::GetWallCode(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf1(polyinfo.GetPolyIndex(), 0xF00, 8);
}

int dBgW::GetPolyAtt0(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf1(polyinfo.GetPolyIndex(), 0xF000, 12);
}

int dBgW::GetPolyAtt1(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf1(polyinfo.GetPolyIndex(), 0x70000, 16);
}

int dBgW::GetGroundCode(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf1(polyinfo.GetPolyIndex(), 0xF80000, 19);
}

u32 cBgW::GetMaskPolyInf1_NoShift(int poly_index, u32 mask) const {
    int id = GetPolyInfId(poly_index);
    JUT_ASSERT(2324, 0 <= id && id < pm_bgd->m_ti_num);

    return (pm_bgd->m_ti_tbl[id].m_info1 & mask);
}

u32 dBgW::GetIronBallThrough(int poly_index) {
    return GetMaskPolyInf1_NoShift(poly_index, 0x1000000);
}

u32 dBgW::GetAttackThrough(int poly_index) {
    return GetMaskPolyInf1_NoShift(poly_index, 0x2000000);
}

u32 cBgW::GetPolyInf2(int poly_index, u32 mask, u32 shift) const {
    int id = GetPolyInfId(poly_index);
    JUT_ASSERT(2368, 0 <= id && id < pm_bgd->m_ti_num);

    return (pm_bgd->m_ti_tbl[id].m_info2 & mask) >> shift;
}

int dBgW::GetCamMoveBG(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf2(polyinfo.GetPolyIndex(), 0xFF, 0);
}

int dBgW::GetRoomCamId(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf2(polyinfo.GetPolyIndex(), 0xFF00, 8);
}

int dBgW::GetRoomPathId(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf2(polyinfo.GetPolyIndex(), 0xFF0000, 16);
}

int dBgW::GetRoomPathPntNo(const cBgS_PolyInfo& polyinfo) {
    return GetPolyInf2(polyinfo.GetPolyIndex(), 0xFF000000, 24);
}

int cBgW::GetTriGrp(int poly_index) const {
    JUT_ASSERT(2440, 0 <= poly_index && poly_index < pm_bgd->m_t_num);

    return pm_bgd->m_t_tbl[poly_index].m_grp;
}

bool cBgW::ChkNotReady() const {
    return pm_vtx_tbl == NULL;
}

bool cBgW::ChkLock() const {
    return mFlags & LOCK_e;
}

bool cBgW::ChkMoveBg() const {
    return mFlags & MOVE_BG_e;
}

void dBgW::DrawBox() const {
    cXyz min;
    cXyz max;
    cXyz points[8];

    min = *pm_grp[m_rootGrpIdx].m_aab.GetMinP();
    max = *pm_grp[m_rootGrpIdx].m_aab.GetMaxP();

    points[0].x = min.x;
    points[0].y = max.y;
    points[0].z = min.z;

    points[1].x = max.x;
    points[1].y = max.y;
    points[1].z = min.z;

    points[2].x = min.x;
    points[2].y = max.y;
    points[2].z = max.z;
    
    points[3].x = max.x;
    points[3].y = max.y;
    points[3].z = max.z;

    points[4].x = min.x;
    points[4].y = min.y;
    points[4].z = min.z;

    points[5].x = max.x;
    points[5].y = min.y;
    points[5].z = min.z;

    points[6].x = min.x;
    points[6].y = min.y;
    points[6].z = max.z;
    
    points[7].x = max.x;
    points[7].y = min.y;
    points[7].z = max.z;

    dDbVw_drawCube8pXlu(points, COMPOUND_LITERAL(GXColor){0, 0xFF, 0, 0x64});
}

void dBgW::DebugDraw() const {
    GXColor ground_color = {0xFF, 0, 0, 0x80};
    GXColor roof_color = {0, 0, 0xFF, 0x80};
    GXColor wall_color = {0, 0xFF, 0, 0x80};

    cBgD_Tri_t* t_tbl = pm_bgd->m_t_tbl;
    for (int i = 0; i < pm_bgd->m_t_num; i++) {
        cXyz normal;
        normal = pm_tri[i].m_plane.mNormal;

        cBgD_Vtx_t* v0 = &pm_vtx_tbl[t_tbl[i].m_vtx_idx0];
        cBgD_Vtx_t* v1 = &pm_vtx_tbl[t_tbl[i].m_vtx_idx1];
        cBgD_Vtx_t* v2 = &pm_vtx_tbl[t_tbl[i].m_vtx_idx2];

        cXyz points[3];
        points[0].set(*v0);
        points[1].set(*v1);
        points[2].set(*v2);

        points[0] += normal;
        points[1] += normal;
        points[2] += normal;

        GXColor* pcolor;
        if (cBgW_CheckBGround(normal.y)) {
            pcolor = &ground_color;
        } else if (cBgW_CheckBRoof(normal.y)) {
            pcolor = &roof_color;
        } else {
            pcolor = &wall_color;
        }

        dDbVw_drawTriangleXlu(points, *pcolor, TRUE);
    }
}

void dBgW::DrawPoly(const cBgS_PolyInfo& polyinfo, const GXColor& color) const {
    int poly_index = polyinfo.GetPolyIndex();
    JUT_ASSERT(2579, 0 <= poly_index && poly_index < pm_bgd->m_t_num);

    cXyz normal;
    normal = pm_tri[poly_index].m_plane.mNormal;

    cBgD_Tri_t* t_tbl = pm_bgd->m_t_tbl;
    cBgD_Vtx_t* v0 = &pm_vtx_tbl[t_tbl[poly_index].m_vtx_idx0];
    cBgD_Vtx_t* v1 = &pm_vtx_tbl[t_tbl[poly_index].m_vtx_idx1];
    cBgD_Vtx_t* v2 = &pm_vtx_tbl[t_tbl[poly_index].m_vtx_idx2];

    cXyz points[3];
    points[0].set(*v0);
    points[1].set(*v1);
    points[2].set(*v2);

    points[0] += normal;
    points[1] += normal;
    points[2] += normal;

    dDbVw_drawTriangleXlu(points, color, TRUE);
}

dBgW::dBgW() {
    m_crr_func = NULL;
    m_ride_callback = NULL;
    m_arrow_stick_callback = NULL;
    m_flags = 0;
}

void dBgW::Move() {
    OnMoveFlag();
    cBgW::Move();
}

void dBgW::positionWallCorrect(dBgS_Acch* pwi, f32 dist, cM3dGPla& plane, cXyz* pupper_pos,
                               f32 speed) {
    pwi->SetWallHit();

    speed -= 1.0f;
    if (speed < 0.0f) {
        speed = 0.0f;
    }

    f32 move = speed * dist;
    pupper_pos->x += move * plane.GetNP()->x;
    pupper_pos->z += move * plane.GetNP()->z;

    JUT_ASSERT(2682, !isnan(pupper_pos->x));
    JUT_ASSERT(2683, !isnan(pupper_pos->z));
}

bool dBgW::RwgWallCorrect(dBgS_Acch* pwi, u16 poly_index) {
    bool correct = false;

    while (true) {
        cBgW_RwgElm* rwg_elm = &pm_rwg[poly_index];

        if (!ChkPolyThrough(poly_index, pwi->GetPolyPassChk())) {
            cBgW_TriElm* tri = &pm_tri[poly_index];

            f32 sp68 = JMAFastSqrt(tri->m_plane.GetNP()->x * tri->m_plane.GetNP()->x +
                                   tri->m_plane.GetNP()->z * tri->m_plane.GetNP()->z);
            if (cM3d_IsZero(sp68)) {
                if (rwg_elm->m_next != 0xFFFF) {
                    poly_index = rwg_elm->m_next;
                    continue;
                }
                break;
            }

            f32 sp6C = 1.0f / sp68;
            cBgD_Tri_t* tri_data = &pm_bgd->m_t_tbl[poly_index];
            int cir_index = 0;

            while (cir_index < pwi->GetTblSize()) {
                f32 sp78 = sp6C * pwi->GetWallR(cir_index);
                Vec sp50;
                sp50.x = sp78 * tri->m_plane.GetNP()->x;
                sp50.y = 0.0f;
                sp50.z = sp78 * tri->m_plane.GetNP()->z;

                f32 sp7C;
                if (!pwi->ChkWallHDirect(cir_index)) {
                    sp7C =
                        (pwi->GetWallAddY(sp50) + (pwi->GetPos()->y + pwi->GetWallH(cir_index))) -
                        pwi->GetSpeedY();
                } else {
                    sp7C = pwi->GetWallHDirect(cir_index);
                }

                f32 sp5C[3];
                sp5C[0] = pm_vtx_tbl[tri_data->m_vtx_idx0].y - sp7C;
                sp5C[1] = pm_vtx_tbl[tri_data->m_vtx_idx1].y - sp7C;
                sp5C[2] = pm_vtx_tbl[tri_data->m_vtx_idx2].y - sp7C;

                if ((!(sp5C[0] > 0.0f) || !(sp5C[1] > 0.0f) || !(sp5C[2] > 0.0f)) &&
                    (!(sp5C[0] < 0.0f) || !(sp5C[1] < 0.0f) || !(sp5C[2] < 0.0f)))
                {
                    int sp8C = 0;
                    if (cM3d_IsZero(sp5C[0])) {
                        sp8C++;
                    }
                    if (cM3d_IsZero(sp5C[1])) {
                        sp8C++;
                    }
                    if (cM3d_IsZero(sp5C[2])) {
                        sp8C++;
                    }

                    int sp80, sp84, sp88;
                    if (sp8C != 1) {
                        if ((sp5C[0] > 0.0f && (sp5C[1] <= 0.0f) && (sp5C[2] <= 0.0f)) ||
                            (sp5C[0] < 0.0f && (sp5C[1] >= 0.0f) && (sp5C[2] >= 0.0f)))
                        {
                            sp80 = 0;
                            sp84 = 1;
                            sp88 = 2;
                        } else if ((sp5C[1] > 0.0f && (sp5C[0] <= 0.0f) && (sp5C[2] <= 0.0f)) ||
                                   (sp5C[1] < 0.0f && (sp5C[0] >= 0.0f) && (sp5C[2] >= 0.0f)))
                        {
                            sp80 = 1;
                            sp84 = 0;
                            sp88 = 2;
                        } else {
                            sp80 = 2;
                            sp84 = 0;
                            sp88 = 1;
                        }

                        f32 sp90 = sp5C[sp80] - sp5C[sp84];
                        f32 sp94 = sp5C[sp80] - sp5C[sp88];
                        if (!cM3d_IsZero(sp90) && !cM3d_IsZero(sp94)) {
                            f32 sp98 = -sp5C[sp84] / sp90;
                            f32 sp9C = -sp5C[sp88] / sp94;

                            f32 vtx0_x = pm_vtx_tbl[tri_data->m_vtx_idx0].x;
                            f32 vtx0_z = pm_vtx_tbl[tri_data->m_vtx_idx0].z;
                            f32 vtx1_x = pm_vtx_tbl[tri_data->m_vtx_idx1].x;
                            f32 vtx1_z = pm_vtx_tbl[tri_data->m_vtx_idx1].z;
                            f32 vtx2_x = pm_vtx_tbl[tri_data->m_vtx_idx2].x;
                            f32 vtx2_z = pm_vtx_tbl[tri_data->m_vtx_idx2].z;

                            f32 cx0, cy0, cx1, cy1;
                            if (sp80 == 0) {
                                cx0 = vtx1_x + sp98 * (vtx0_x - vtx1_x);
                                cy0 = vtx1_z + sp98 * (vtx0_z - vtx1_z);
                                cx1 = vtx2_x + sp9C * (vtx0_x - vtx2_x);
                                cy1 = vtx2_z + sp9C * (vtx0_z - vtx2_z);
                            } else if (sp80 == 1) {
                                cx0 = vtx0_x + sp98 * (vtx1_x - vtx0_x);
                                cy0 = vtx0_z + sp98 * (vtx1_z - vtx0_z);
                                cx1 = vtx2_x + sp9C * (vtx1_x - vtx2_x);
                                cy1 = vtx2_z + sp9C * (vtx1_z - vtx2_z);
                            } else {
                                cx0 = vtx0_x + sp98 * (vtx2_x - vtx0_x);
                                cy0 = vtx0_z + sp98 * (vtx2_z - vtx0_z);
                                cx1 = vtx1_x + sp9C * (vtx2_x - vtx1_x);
                                cy1 = vtx1_z + sp9C * (vtx2_z - vtx1_z);
                            }

                            cx0 += sp50.x;
                            cy0 += sp50.z;
                            cx1 += sp50.x;
                            cy1 += sp50.z;

                            f32 spC8, spCC, spD0;
                            bool sp107 =
                                cM3d_Len2dSqPntAndSegLine(pwi->GetCx(), pwi->GetCz(), cx0, cy0, cx1,
                                                          cy1, &spCC, &spD0, &spC8);

                            f32 spD4 = spCC - pwi->GetCx();
                            f32 spD8 = spD0 - pwi->GetCz();
                            f32 spDC = pwi->GetWallRR(cir_index);

                            if (!(spC8 > spDC) && !(spD4 * sp50.x + spD8 * sp50.z < 0.0f)) {
                                if (sp107 == 1) {
                                    positionWallCorrect(pwi, sp6C, tri->m_plane, pwi->GetPos(),
                                                        JMAFastSqrt(spC8));
                                    pwi->CalcMovePosWork();
                                    pwi->SetWallCirHit(cir_index);
                                    pwi->SetWallPolyIndex(cir_index, poly_index);

                                    s16 sp10 = cM_atan2s(tri->m_plane.GetNP()->x, tri->m_plane.GetNP()->z);
                                    pwi->SetWallAngleY(cir_index, sp10);
                                    correct = true;
                                } else {
                                    cx0 -= sp50.x;
                                    cy0 -= sp50.z;
                                    cx1 -= sp50.x;
                                    cy1 -= sp50.z;

                                    JUT_ASSERT(2920, pwi->GetPos()->x ==
                                               pwi->GetWallCirP(cir_index)->GetCx());
                                    JUT_ASSERT(2922, pwi->GetPos()->z ==
                                               pwi->GetWallCirP(cir_index)->GetCy());

                                    f32 spE0 =
                                        cM3d_Len2dSq(cx0, cy0, pwi->GetPos()->x, pwi->GetPos()->z);
                                    f32 spE4 =
                                        cM3d_Len2dSq(cx1, cy1, pwi->GetPos()->x, pwi->GetPos()->z);

                                    f32 onx = -tri->m_plane.GetNP()->x;
                                    f32 ony = -tri->m_plane.GetNP()->z;

                                    JUT_ASSERT(2938, !(cM3d_IsZero(onx) && cM3d_IsZero(ony)));

                                    if (spE0 < spE4) {
                                        if (!(spE0 > spDC) && !(fabsf(spE0 - spDC) < 0.008f)) {
                                            JUT_ASSERT(2950, !isnan(cx0));
                                            JUT_ASSERT(2951, !isnan(cy0));

                                            f32 spF0, spF4;
                                            cM2d_CrossCirLin(*pwi->GetWallCirP(cir_index), cx0, cy0,
                                                             onx, ony, &spF0, &spF4);
                                            pwi->GetPos()->x += cx0 - spF0;
                                            pwi->GetPos()->z += cy0 - spF4;

                                            JUT_ASSERT(2968, !isnan(pwi->GetPos()->x));
                                            JUT_ASSERT(2969, !isnan(pwi->GetPos()->z));

                                            pwi->CalcMovePosWork();
                                            pwi->SetWallCirHit(cir_index);
                                            pwi->SetWallPolyIndex(cir_index, poly_index);
                                            s16 spE = cM_atan2s(tri->m_plane.GetNP()->x, tri->m_plane.GetNP()->z);
                                            pwi->SetWallAngleY(cir_index, spE);
                                            correct = true;
                                            pwi->SetWallHit();
                                        }
                                    } else if (!(spE4 > spDC) && !(fabsf(spE4 - spDC) < 0.008f)) {
                                        JUT_ASSERT(2999, !isnan(cx1));
                                        JUT_ASSERT(3000, !isnan(cy1));

                                        f32 spF8, spFC;
                                        cM2d_CrossCirLin(*pwi->GetWallCirP(cir_index), cx1, cy1,
                                                         onx, ony, &spF8, &spFC);
                                        pwi->GetPos()->x += cx1 - spF8;
                                        pwi->GetPos()->z += cy1 - spFC;

                                        JUT_ASSERT(3016, !isnan(pwi->GetPos()->x));
                                        JUT_ASSERT(3017, !isnan(pwi->GetPos()->z));

                                        pwi->CalcMovePosWork();
                                        pwi->SetWallCirHit(cir_index);
                                        pwi->SetWallPolyIndex(cir_index, poly_index);

                                        s16 spC = cM_atan2s(tri->m_plane.GetNP()->x, tri->m_plane.GetNP()->z);
                                        pwi->SetWallAngleY(cir_index, spC);
                                        correct = true;
                                        pwi->SetWallHit();
                                    }
                                }
                            }
                        }
                    }
                }

                cir_index++;
            }
        }

        if (rwg_elm->m_next == 0xFFFF) {
            break;
        }

        poly_index = rwg_elm->m_next;
    }

    return correct;
}

bool dBgW::WallCorrectRp(dBgS_Acch* pwi, int node_index) {
    JUT_ASSERT(3060, 0 <= node_index && node_index < pm_bgd->m_tree_num);

    cBgW_NodeTree* tree = &pm_node_tree[node_index];
    if (!tree->Cross(pwi->GetWallBmdCylP())) {
        return false;
    }

    cBgD_Tree_t* pnode = &pm_bgd->m_tree_tbl[node_index];
    bool chk = false;
    if ((pnode->m_flag & 1)) {
        JUT_ASSERT(3076, pnode->m_id[0] != 0xffff);

        JUT_ASSERT(3078, pnode->m_id[0] >= 0 && pnode->m_id[0] < pm_bgd->m_b_num);

        if (pm_blk[pnode->m_id[0]].m_wall_idx != 0xFFFF &&
            RwgWallCorrect(pwi, pm_blk[pnode->m_id[0]].m_wall_idx))
        {
            chk = true;
        }
        if (pm_blk[pnode->m_id[0]].m_roof_idx != 0xFFFF &&
            RwgWallCorrect(pwi, pm_blk[pnode->m_id[0]].m_roof_idx))
        {
            chk = true;
        }
        return chk;
    }

    for (int i = 0; i < 8; i++) {
        if (pnode->m_id[i] != 0xFFFF && WallCorrectRp(pwi, pnode->m_id[i])) {
            chk = true;
        }
    }

    return chk;
}

bool dBgW::WallCorrectGrpRp(dBgS_Acch* pwi, int g, int depth) {
    JUT_ASSERT(3117, 0 <= g && g < pm_bgd->m_g_num);

    if (!pm_grp[g].m_aab.Cross(pwi->GetWallBmdCylP())) {
        return false;
    }

    if (ChkGrpThrough(g, pwi->GetGrpPassChk(), depth)) {
        return false;
    }

    bool chk = false;
    if (pm_bgd->m_g_tbl[g].m_tree_idx != 0xFFFF &&
        WallCorrectRp(pwi, pm_bgd->m_g_tbl[g].m_tree_idx))
    {
        chk = true;
    }

    int child_idx = pm_bgd->m_g_tbl[g].m_first_child;
    depth++;
    while (true) {
        if (child_idx == 0xFFFF) {
            break;
        }
        if (WallCorrectGrpRp(pwi, child_idx, depth)) {
            chk = true;
        }
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return chk;
}

bool dBgW::WallCorrect(dBgS_Acch* pwi) {
    return WallCorrectGrpRp(pwi, m_rootGrpIdx, 1);
}

struct wcs_data {
    /* 0x0 */ f32 field_0x0;
    /* 0x4 */ u16 poly_idx;
    /* 0x8 */ wcs_data* next;
};

static wcs_data l_wcsbuf[84];

static wcs_data* l_start;

static int l_wcsbuf_num;

void dBgW::RwgWallCorrectSort(dBgS_Acch* pwi, u16 poly_index) {
    while (true) {
        cBgW_RwgElm* rwg_elm = &pm_rwg[poly_index];

        if (!ChkPolyThrough(poly_index, pwi->GetPolyPassChk())) {
            cBgW_TriElm* tri_elm = &pm_tri[poly_index];

            f32 var_f30 = JMAFastSqrt(tri_elm->m_plane.GetNP()->x * tri_elm->m_plane.GetNP()->x +
                                      tri_elm->m_plane.GetNP()->z * tri_elm->m_plane.GetNP()->z);
            if (cM3d_IsZero(var_f30)) {
                if (rwg_elm->m_next != 0xFFFF) {
                    poly_index = rwg_elm->m_next;
                    continue;
                }
                break;
            }

            fopAc_ac_c* actor = pwi->getMyAc();
            cXyz sp88;
            sp88.x = cM_ssin(actor->current.angle.y);
            sp88.z = cM_scos(actor->current.angle.y);

            f32 var_f29 = 1.0f / var_f30;
            f32 var_f31 = var_f29 * ((sp88.x * tri_elm->m_plane.GetNP()->x) +
                                     (sp88.z * tri_elm->m_plane.GetNP()->z));

            if (l_start == NULL) {
                if (l_wcsbuf_num < 84) {
                    l_wcsbuf[l_wcsbuf_num].field_0x0 = var_f31;
                    l_wcsbuf[l_wcsbuf_num].poly_idx = poly_index;
                    l_wcsbuf[l_wcsbuf_num].next = NULL;
                    l_start = &l_wcsbuf[l_wcsbuf_num];
                    l_wcsbuf_num++;
                } else {
                    #if DEBUG
                    lbl_8074C7F0 = 1;
                    #endif
                }
            } else if (poly_index != l_start->poly_idx) {
                if (var_f31 > l_start->field_0x0) {
                    if (l_wcsbuf_num < 84) {
                        l_wcsbuf[l_wcsbuf_num].field_0x0 = var_f31;
                        l_wcsbuf[l_wcsbuf_num].poly_idx = poly_index;
                        l_wcsbuf[l_wcsbuf_num].next = l_start;
                        l_start = &l_wcsbuf[l_wcsbuf_num];
                        l_wcsbuf_num++;
                    } else {
                        #if DEBUG
                        lbl_8074C7F0 = 1;
                        #endif
                    }
                } else {
                    wcs_data* data = l_start;
                    while (true) {
                        if (data->next == NULL) {
                            if (l_wcsbuf_num < 84) {
                                l_wcsbuf[l_wcsbuf_num].field_0x0 = var_f31;
                                l_wcsbuf[l_wcsbuf_num].poly_idx = poly_index;
                                l_wcsbuf[l_wcsbuf_num].next = NULL;
                                data->next = &l_wcsbuf[l_wcsbuf_num];
                                l_wcsbuf_num++;
                            } else {
                                #if DEBUG
                                lbl_8074C7F0 = 1;
                                #endif
                            }
                            break;
                        } else if (poly_index == data->next->poly_idx) {
                            break;
                        }

                        if (var_f31 > data->next->field_0x0) {
                            if (l_wcsbuf_num < 84) {
                                l_wcsbuf[l_wcsbuf_num].field_0x0 = var_f31;
                                l_wcsbuf[l_wcsbuf_num].poly_idx = poly_index;
                                l_wcsbuf[l_wcsbuf_num].next = data->next;
                                data->next = &l_wcsbuf[l_wcsbuf_num];
                                l_wcsbuf_num++;
                            } else {
                                #if DEBUG
                                lbl_8074C7F0 = 1;
                                #endif
                            }
                            break;
                        }

                        data = data->next;
                    }
                }
            }
        }

        if (rwg_elm->m_next == 0xFFFF) {
            return;
        }

        poly_index = rwg_elm->m_next;
    }
}

void dBgW::WallCorrectRpSort(dBgS_Acch* pwi, int node_index) {
    JUT_ASSERT(3330, 0 <= node_index && node_index < pm_bgd->m_tree_num);

    cBgW_NodeTree* tree = &pm_node_tree[node_index];
    if (!tree->Cross(pwi->GetWallBmdCylP())) {
        return;
    }

    cBgD_Tree_t* pnode = &pm_bgd->m_tree_tbl[node_index];
    if ((pnode->m_flag & 1)) {
        JUT_ASSERT(3345, pnode->m_id[0] != 0xffff);

        JUT_ASSERT(3347, pnode->m_id[0] >= 0 && pnode->m_id[0] < pm_bgd->m_b_num);

        if (pm_blk[pnode->m_id[0]].m_wall_idx != 0xFFFF) {
            RwgWallCorrectSort(pwi, pm_blk[pnode->m_id[0]].m_wall_idx);
        }
        if (pm_blk[pnode->m_id[0]].m_roof_idx != 0xFFFF) {
            RwgWallCorrectSort(pwi, pm_blk[pnode->m_id[0]].m_roof_idx);
        }
        return;
    }

    for (int i = 0; i < 8; i++) {
        if (pnode->m_id[i] != 0xFFFF) {
            WallCorrectRpSort(pwi, pnode->m_id[i]);
        }
    }
}

bool dBgW::WallCorrectGrpRpSort(dBgS_Acch* pwi, int g, int depth) {
    JUT_ASSERT(3378, 0 <= g && g < pm_bgd->m_g_num);

    if (!pm_grp[g].m_aab.Cross(pwi->GetWallBmdCylP())) {
        return false;
    }

    if (ChkGrpThrough(g, pwi->GetGrpPassChk(), depth)) {
        return false;
    }

    bool var_r25 = false;

    if (pm_bgd->m_g_tbl[g].m_tree_idx != 0xFFFF) {
        WallCorrectRpSort(pwi, pm_bgd->m_g_tbl[g].m_tree_idx);
    }

    int child_idx = pm_bgd->m_g_tbl[g].m_first_child;
    depth++;
    while (true) {
        if (child_idx == 0xFFFF) {
            break;
        }
        WallCorrectGrpRpSort(pwi, child_idx, depth);
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return false;
}

bool dBgW::WallCorrectSort(dBgS_Acch* pwi) {
    l_start = NULL;
    l_wcsbuf_num = 0;

    WallCorrectGrpRpSort(pwi, m_rootGrpIdx, 1);
    bool correct = false;

    wcs_data* data = l_start;
    while (true) {
        if (data == NULL) {
            break;
        }

        int poly_index = data->poly_idx;
        cBgW_TriElm* tri = &pm_tri[poly_index];
        f32 sp68 = JMAFastSqrt(tri->m_plane.GetNP()->x * tri->m_plane.GetNP()->x +
                               tri->m_plane.GetNP()->z * tri->m_plane.GetNP()->z);

        f32 sp6C = 1.0f / sp68;
        cBgD_Tri_t* tri_data = &pm_bgd->m_t_tbl[poly_index];
        int cir_index = 0;

        while (cir_index < pwi->GetTblSize()) {
            f32 sp78 = sp6C * pwi->GetWallR(cir_index);
            Vec sp50;
            sp50.x = sp78 * tri->m_plane.GetNP()->x;
            sp50.y = 0.0f;
            sp50.z = sp78 * tri->m_plane.GetNP()->z;

            f32 sp7C;
            if (!pwi->ChkWallHDirect(cir_index)) {
                sp7C = (pwi->GetWallAddY(sp50) + (pwi->GetPos()->y + pwi->GetWallH(cir_index))) -
                       pwi->GetSpeedY();
            } else {
                sp7C = pwi->GetWallHDirect(cir_index);
            }

            f32 sp5C[3];
            sp5C[0] = pm_vtx_tbl[tri_data->m_vtx_idx0].y - sp7C;
            sp5C[1] = pm_vtx_tbl[tri_data->m_vtx_idx1].y - sp7C;
            sp5C[2] = pm_vtx_tbl[tri_data->m_vtx_idx2].y - sp7C;

            if ((!(sp5C[0] > 0.0f) || !(sp5C[1] > 0.0f) || !(sp5C[2] > 0.0f)) &&
                (!(sp5C[0] < 0.0f) || !(sp5C[1] < 0.0f) || !(sp5C[2] < 0.0f)))
            {
                int sp8C = 0;
                if (cM3d_IsZero(sp5C[0])) {
                    sp8C++;
                }
                if (cM3d_IsZero(sp5C[1])) {
                    sp8C++;
                }
                if (cM3d_IsZero(sp5C[2])) {
                    sp8C++;
                }

                int sp80, sp84, sp88;
                if (sp8C != 1) {
                    if ((sp5C[0] > 0.0f && (sp5C[1] <= 0.0f) && (sp5C[2] <= 0.0f)) ||
                        (sp5C[0] < 0.0f && (sp5C[1] >= 0.0f) && (sp5C[2] >= 0.0f)))
                    {
                        sp80 = 0;
                        sp84 = 1;
                        sp88 = 2;
                    } else if ((sp5C[1] > 0.0f && (sp5C[0] <= 0.0f) && (sp5C[2] <= 0.0f)) ||
                               (sp5C[1] < 0.0f && (sp5C[0] >= 0.0f) && (sp5C[2] >= 0.0f)))
                    {
                        sp80 = 1;
                        sp84 = 0;
                        sp88 = 2;
                    } else {
                        sp80 = 2;
                        sp84 = 0;
                        sp88 = 1;
                    }

                    f32 sp90 = sp5C[sp80] - sp5C[sp84];
                    f32 sp94 = sp5C[sp80] - sp5C[sp88];
                    if (!cM3d_IsZero(sp90) && !cM3d_IsZero(sp94)) {
                        f32 sp98 = -sp5C[sp84] / sp90;
                        f32 sp9C = -sp5C[sp88] / sp94;

                        f32 vtx0_x = pm_vtx_tbl[tri_data->m_vtx_idx0].x;
                        f32 vtx0_z = pm_vtx_tbl[tri_data->m_vtx_idx0].z;
                        f32 vtx1_x = pm_vtx_tbl[tri_data->m_vtx_idx1].x;
                        f32 vtx1_z = pm_vtx_tbl[tri_data->m_vtx_idx1].z;
                        f32 vtx2_x = pm_vtx_tbl[tri_data->m_vtx_idx2].x;
                        f32 vtx2_z = pm_vtx_tbl[tri_data->m_vtx_idx2].z;

                        f32 cx0, cy0, cx1, cy1;
                        if (sp80 == 0) {
                            cx0 = vtx1_x + sp98 * (vtx0_x - vtx1_x);
                            cy0 = vtx1_z + sp98 * (vtx0_z - vtx1_z);
                            cx1 = vtx2_x + sp9C * (vtx0_x - vtx2_x);
                            cy1 = vtx2_z + sp9C * (vtx0_z - vtx2_z);
                        } else if (sp80 == 1) {
                            cx0 = vtx0_x + sp98 * (vtx1_x - vtx0_x);
                            cy0 = vtx0_z + sp98 * (vtx1_z - vtx0_z);
                            cx1 = vtx2_x + sp9C * (vtx1_x - vtx2_x);
                            cy1 = vtx2_z + sp9C * (vtx1_z - vtx2_z);
                        } else {
                            cx0 = vtx0_x + sp98 * (vtx2_x - vtx0_x);
                            cy0 = vtx0_z + sp98 * (vtx2_z - vtx0_z);
                            cx1 = vtx1_x + sp9C * (vtx2_x - vtx1_x);
                            cy1 = vtx1_z + sp9C * (vtx2_z - vtx1_z);
                        }

                        cx0 += sp50.x;
                        cy0 += sp50.z;
                        cx1 += sp50.x;
                        cy1 += sp50.z;

                        f32 spC8, spCC, spD0;
                        bool sp107 = cM3d_Len2dSqPntAndSegLine(pwi->GetCx(), pwi->GetCz(), cx0, cy0,
                                                               cx1, cy1, &spCC, &spD0, &spC8);

                        f32 spD4 = spCC - pwi->GetCx();
                        f32 spD8 = spD0 - pwi->GetCz();
                        f32 spDC = pwi->GetWallRR(cir_index);

                        if (!(spC8 > spDC) && !(spD4 * sp50.x + spD8 * sp50.z < 0.0f)) {
                            if (sp107 == 1) {
                                positionWallCorrect(pwi, sp6C, tri->m_plane, pwi->GetPos(),
                                                    JMAFastSqrt(spC8));
                                pwi->CalcMovePosWork();
                                pwi->SetWallCirHit(cir_index);
                                pwi->SetWallPolyIndex(cir_index, poly_index);
                                s16 sp10 = cM_atan2s(tri->m_plane.GetNP()->x, tri->m_plane.GetNP()->z);
                                pwi->SetWallAngleY(cir_index, sp10);
                                correct = true;
                            } else {
                                cx0 -= sp50.x;
                                cy0 -= sp50.z;
                                cx1 -= sp50.x;
                                cy1 -= sp50.z;

                                JUT_ASSERT(3625, pwi->GetPos()->x ==
                                           pwi->GetWallCirP(cir_index)->GetCx());
                                JUT_ASSERT(3627, pwi->GetPos()->z ==
                                           pwi->GetWallCirP(cir_index)->GetCy());

                                f32 spE0 =
                                    cM3d_Len2dSq(cx0, cy0, pwi->GetPos()->x, pwi->GetPos()->z);
                                f32 spE4 =
                                    cM3d_Len2dSq(cx1, cy1, pwi->GetPos()->x, pwi->GetPos()->z);

                                f32 onx = -tri->m_plane.GetNP()->x;
                                f32 ony = -tri->m_plane.GetNP()->z;

                                JUT_ASSERT(3643, !(cM3d_IsZero(onx) && cM3d_IsZero(ony)));

                                if (spE0 < spE4) {
                                    if (!(spE0 > spDC) && !(fabsf(spE0 - spDC) < 0.008f)) {
                                        JUT_ASSERT(3655, !isnan(cx0));
                                        JUT_ASSERT(3656, !isnan(cy0));

                                        f32 spF0, spF4;
                                        cM2d_CrossCirLin(*pwi->GetWallCirP(cir_index), cx0, cy0,
                                                         onx, ony, &spF0, &spF4);
                                        pwi->GetPos()->x += cx0 - spF0;
                                        pwi->GetPos()->z += cy0 - spF4;

                                        JUT_ASSERT(3666, !isnan(pwi->GetPos()->x));
                                        JUT_ASSERT(3667, !isnan(pwi->GetPos()->z));

                                        pwi->CalcMovePosWork();
                                        pwi->SetWallCirHit(cir_index);
                                        pwi->SetWallPolyIndex(cir_index, poly_index);
                                        s16 spE = cM_atan2s(tri->m_plane.GetNP()->x, tri->m_plane.GetNP()->z);
                                        pwi->SetWallAngleY(cir_index, spE);
                                        correct = true;
                                        pwi->SetWallHit();
                                    }
                                } else if (!(spE4 > spDC) && !(fabsf(spE4 - spDC) < 0.008f)) {
                                    JUT_ASSERT(3697, !isnan(cx1));
                                    JUT_ASSERT(3698, !isnan(cy1));

                                    f32 spF8, spFC;
                                    cM2d_CrossCirLin(*pwi->GetWallCirP(cir_index), cx1, cy1, onx,
                                                     ony, &spF8, &spFC);
                                    pwi->GetPos()->x += cx1 - spF8;
                                    pwi->GetPos()->z += cy1 - spFC;

                                    JUT_ASSERT(3707, !isnan(pwi->GetPos()->x));
                                    JUT_ASSERT(3708, !isnan(pwi->GetPos()->z));

                                    pwi->CalcMovePosWork();
                                    pwi->SetWallCirHit(cir_index);
                                    pwi->SetWallPolyIndex(cir_index, poly_index);
                                    s16 spC = cM_atan2s(tri->m_plane.GetNP()->x, tri->m_plane.GetNP()->z);
                                    pwi->SetWallAngleY(cir_index, spC);
                                    correct = true;
                                    pwi->SetWallHit();
                                }
                            }
                        }
                    }
                }
            }

            cir_index++;
        }

        data = data->next;
    }

    return correct;
}

bool dBgW::RwgRoofChk(u16 poly_index, dBgS_RoofChk* proofchk) {
    bool chk = false;
    f32 cy;
    while (true) {
        if (pm_tri[poly_index].m_plane.getCrossY(*proofchk->GetPosP(), &cy) &&
            cy > proofchk->GetPosP()->y && cy < proofchk->GetNowY())
        {
            cBgD_Tri_t* tri = &pm_bgd->m_t_tbl[poly_index];
            if (cM3d_CrossY_Tri(pm_vtx_tbl[tri->m_vtx_idx0], pm_vtx_tbl[tri->m_vtx_idx1],
                                pm_vtx_tbl[tri->m_vtx_idx2], pm_tri[poly_index].m_plane,
                                (const Vec*)proofchk->GetPosP()))
            {
                if (!ChkPolyThrough(poly_index, proofchk->GetPolyPassChk())) {
                    proofchk->SetNowY(cy);
                    proofchk->SetPolyIndex(poly_index);
                    chk = true;

                    JUT_ASSERT(3774, !isnan(cy));

                    JUT_ASSERT(3776, -INF < cy && cy < INF);
                }
            }
        }

        cBgW_RwgElm* element = &pm_rwg[poly_index];
        if (element->m_next == 0xFFFF)
            break;

        poly_index = element->m_next;
    }

    return chk;
}

bool dBgW::RoofChkRp(dBgS_RoofChk* proofchk, int node_index) {
    JUT_ASSERT(3802, 0 <= node_index && node_index < pm_bgd->m_tree_num);

    cBgW_NodeTree* node = &pm_node_tree[node_index];
    if (!node->CrossY(proofchk->GetPosP()) || !node->UnderPlaneYUnder(proofchk->GetNowY()) ||
        node->TopPlaneYUnder(proofchk->GetPosP()->y))
    {
        return false;
    }

    cBgD_Tree_t* pnode = &pm_bgd->m_tree_tbl[node_index];
    if ((pnode->m_flag & 1)) {
        JUT_ASSERT(3819, pnode->m_id[0] != 0xffff);

        JUT_ASSERT(3821, pnode->m_id[0] >= 0 && pnode->m_id[0] < pm_bgd->m_b_num);

        if (pm_blk[pnode->m_id[0]].m_roof_idx != 0xFFFF &&
            RwgRoofChk(pm_blk[pnode->m_id[0]].m_roof_idx, proofchk))
        {
            return true;
        }
        return false;
    }

    bool chk = false;
    for (int i = 0; i < 8; i++) {
        if (pnode->m_id[i] != 0xFFFF && RoofChkRp(proofchk, pnode->m_id[i])) {
            chk = true;
        }
    }

    return chk;
}

bool dBgW::RoofChkGrpRp(dBgS_RoofChk* proofchk, int g, int depth) {
    JUT_ASSERT(3856, 0 <= g && g < pm_bgd->m_g_num);

    cBgW_GrpElm* grp = &pm_grp[g];
    if (!grp->m_aab.CrossY(proofchk->GetPosP()) ||
        !grp->m_aab.UnderPlaneYUnder(proofchk->GetNowY()) ||
        grp->m_aab.TopPlaneYUnder(proofchk->GetPosP()->y))
    {
        return false;
    }

    if (ChkGrpThrough(g, proofchk->GetGrpPassChk(), depth)) {
        return false;
    }

    bool chk = false;
    cBgD_Grp_t* data = &pm_bgd->m_g_tbl[g];
    if (data->m_tree_idx != 0xFFFF && RoofChkRp(proofchk, data->m_tree_idx)) {
        chk = true;
    }

    depth++;
    int child_idx = data->m_first_child;
    while (true) {
        if (child_idx == 0xFFFF) {
            break;
        }
        if (RoofChkGrpRp(proofchk, child_idx, depth)) {
            chk = true;
        }
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return chk;
}

bool dBgW::RoofChk(dBgS_RoofChk* proofchk) {
    return RoofChkGrpRp(proofchk, m_rootGrpIdx, 1);
}

bool dBgW::RwgSplGrpChk(u16 poly_index, dBgS_SplGrpChk* psplchk) {
    bool chk = false;
    f32 cy;
    while (true) {
        if (pm_tri[poly_index].m_plane.getCrossY(psplchk->GetPosP(), &cy) &&
            cy < psplchk->GetRoof() && cy > psplchk->GetHeight())
        {
            cBgD_Tri_t* tri = &pm_bgd->m_t_tbl[poly_index];
            if (cM3d_CrossY_Tri(pm_vtx_tbl[tri->m_vtx_idx0], pm_vtx_tbl[tri->m_vtx_idx1],
                                pm_vtx_tbl[tri->m_vtx_idx2], pm_tri[poly_index].m_plane,
                                (const Vec*)&psplchk->GetPosP()))
            {
                if (!ChkPolyThrough(poly_index, psplchk->GetPolyPassChk())) {
                    psplchk->SetHeight(cy);
                    psplchk->SetPolyIndex(poly_index);
                    chk = true;

                    JUT_ASSERT(3951, !isnan(cy));

                    JUT_ASSERT(3953, -INF < cy && cy < INF);
                }
            }
        }

        cBgW_RwgElm* element = &pm_rwg[poly_index];
        if (element->m_next == 0xFFFF)
            break;

        poly_index = element->m_next;
    }

    return chk;
}

bool dBgW::SplGrpChkRp(dBgS_SplGrpChk* psplchk, int node_index) {
    JUT_ASSERT(3980, 0 <= node_index && node_index < pm_bgd->m_tree_num);

    cBgW_NodeTree* node = &pm_node_tree[node_index];
    if (!node->CrossY(&psplchk->GetPosP()) || !node->UnderPlaneYUnder(psplchk->GetRoof()) ||
        node->TopPlaneYUnder(psplchk->GetHeight()))
    {
        return false;
    }

    cBgD_Tree_t* pnode = &pm_bgd->m_tree_tbl[node_index];
    if ((pnode->m_flag & 1)) {
        JUT_ASSERT(3996, pnode->m_id[0] != 0xffff);

        JUT_ASSERT(3998, pnode->m_id[0] >= 0 && pnode->m_id[0] < pm_bgd->m_b_num);

        if (pm_blk[pnode->m_id[0]].m_gnd_idx != 0xFFFF &&
            RwgSplGrpChk(pm_blk[pnode->m_id[0]].m_gnd_idx, psplchk))
        {
            return true;
        }
        return false;
    }

    bool chk = false;
    for (int i = 0; i < 8; i++) {
        if (pnode->m_id[i] != 0xFFFF && SplGrpChkRp(psplchk, pnode->m_id[i])) {
            chk = true;
        }
    }

    return chk;
}

bool dBgW::SplGrpChkGrpRp(dBgS_SplGrpChk* psplchk, int g, int depth) {
    JUT_ASSERT(4030, 0 <= g && g < pm_bgd->m_g_num);

    cBgW_GrpElm* grp = &pm_grp[g];
    if (!grp->m_aab.CrossY(&psplchk->GetPosP()) ||
        !grp->m_aab.UnderPlaneYUnder(psplchk->GetRoof()) ||
        grp->m_aab.TopPlaneYUnder(psplchk->GetHeight()))
    {
        return false;
    }

    if (ChkGrpThrough(g, psplchk->GetGrpPassChk(), depth)) {
        return false;
    }

    bool chk = false;
    cBgD_Grp_t* data = &pm_bgd->m_g_tbl[g];
    if (data->m_tree_idx != 0xFFFF && SplGrpChkRp(psplchk, data->m_tree_idx)) {
        chk = true;
    }

    depth++;
    int child_idx = data->m_first_child;
    while (true) {
        if (child_idx == 0xFFFF) {
            break;
        }
        if (SplGrpChkGrpRp(psplchk, child_idx, depth)) {
            chk = true;
        }
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return chk;
}

bool dBgW::SplGrpChk(dBgS_SplGrpChk* psplchk) {
    return SplGrpChkGrpRp(psplchk, m_rootGrpIdx, 1);
}

void dBgW::RwgCaptPoly(int index, dBgS_CaptPoly& capt) {
    cBgW_RwgElm* rwg;
    while (true) {
        rwg = &pm_rwg[index];
        capt.m_callback(&capt, pm_vtx_tbl, pm_bgd->m_t_tbl[index].m_vtx_idx0,
                        pm_bgd->m_t_tbl[index].m_vtx_idx1, pm_bgd->m_t_tbl[index].m_vtx_idx2,
                        &pm_tri[index].m_plane);

        if (rwg->m_next == 0xFFFF)
            break;
        index = rwg->m_next;

        JUT_ASSERT(4110, 0 <= index && index < pm_bgd->m_t_num);
    }
}

void dBgW::CaptPolyRp(dBgS_CaptPoly& capt, int node_index) {
    if (!pm_node_tree[node_index].Cross(capt.GetBndP())) {
        return;
    }

    cBgD_Tree_t* tree_data = &pm_bgd->m_tree_tbl[node_index];
    if (tree_data->m_flag & 1) {
        if (pm_blk[tree_data->m_id[0]].m_wall_idx != 0xFFFF) {
            RwgCaptPoly(pm_blk[tree_data->m_id[0]].m_wall_idx, capt);
        }
        if (pm_blk[tree_data->m_id[0]].m_roof_idx != 0xFFFF) {
            RwgCaptPoly(pm_blk[tree_data->m_id[0]].m_roof_idx, capt);
        }
        if (pm_blk[tree_data->m_id[0]].m_gnd_idx != 0xFFFF) {
            RwgCaptPoly(pm_blk[tree_data->m_id[0]].m_gnd_idx, capt);
        }
        return;
    }

    for (int i = 0; i < 8; i++) {
        if (tree_data->m_id[i] == 0xFFFF) {
            continue;
        }
        CaptPolyRp(capt, tree_data->m_id[i]);
    }
}

void dBgW::CaptPolyGrpRp(dBgS_CaptPoly& capt, int node_index, int depth) {
    if (!pm_grp[node_index].m_aab.Cross(capt.GetBndP())) {
        return;
    }

    if (ChkGrpThrough(node_index, capt.GetGrpPassChk(), depth)) {
        return;
    }

    if (pm_bgd->m_g_tbl[node_index].m_tree_idx != 0xFFFF) {
        CaptPolyRp(capt, pm_bgd->m_g_tbl[node_index].m_tree_idx);
    }

    depth++;
    int child_idx = pm_bgd->m_g_tbl[node_index].m_first_child;
    while (true) {
        if (child_idx == 0xFFFF) {
            break;
        }
        CaptPolyGrpRp(capt, child_idx, depth);
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }
}

void dBgW::CaptPoly(dBgS_CaptPoly& capt) {
    CaptPolyGrpRp(capt, m_rootGrpIdx, 1);
}

bool dBgW::RwgSphChk(u16 poly_index, dBgS_SphChk* psphchk, void* i_data) {
    cM3dGTri tri;
    cBgW_RwgElm* rwg;
    cBgD_Tri_t* tri_t;

    bool chk = false;
    while (true) {
        rwg = &pm_rwg[poly_index];
        if (!ChkPolyThrough(poly_index, psphchk->GetPolyPassChk())) {
            tri_t = &pm_bgd->m_t_tbl[poly_index];
            tri.setBg(&pm_vtx_tbl[tri_t->m_vtx_idx0], &pm_vtx_tbl[tri_t->m_vtx_idx1],
                      &pm_vtx_tbl[tri_t->m_vtx_idx2], &pm_tri[poly_index].m_plane);

            if (psphchk->cross(&tri)) {
                psphchk->mCallback(psphchk, pm_vtx_tbl, tri_t->m_vtx_idx0, tri_t->m_vtx_idx1,
                                    tri_t->m_vtx_idx2, &pm_tri[poly_index].m_plane, i_data);
                psphchk->SetPolyIndex(poly_index);
                chk = true;
            }
        }

        if (rwg->m_next == 0xFFFF)
            break;
        poly_index = rwg->m_next;
    }

    return chk;
}

bool dBgW::SphChkRp(dBgS_SphChk* psphchk, void* i_data, int node_index) {
    cBgW_NodeTree* tree = &pm_node_tree[node_index];
    if (!tree->Cross(psphchk)) {
        return false;
    }

    cBgD_Tree_t* tree_data = &pm_bgd->m_tree_tbl[node_index];
    bool chk = false;
    if (tree_data->m_flag & 1) {
        if (pm_blk[tree_data->m_id[0]].m_gnd_idx != 0xFFFF &&
            RwgSphChk(pm_blk[tree_data->m_id[0]].m_gnd_idx, psphchk, i_data))
        {
            chk = true;
        }

        if (pm_blk[tree_data->m_id[0]].m_roof_idx != 0xFFFF &&
            RwgSphChk(pm_blk[tree_data->m_id[0]].m_roof_idx, psphchk, i_data))
        {
            chk = true;
        }

        if (pm_blk[tree_data->m_id[0]].m_wall_idx != 0xFFFF &&
            RwgSphChk(pm_blk[tree_data->m_id[0]].m_wall_idx, psphchk, i_data))
        {
            chk = true;
        }

        return chk;
    }

    for (int i = 0; i < 8; i++) {
        if (tree_data->m_id[i] == 0xFFFF) {
            continue;
        }
        if (SphChkRp(psphchk, i_data, tree_data->m_id[i])) {
            chk = true;
        }
    }
    return chk;
}

bool dBgW::SphChkGrpRp(dBgS_SphChk* psphchk, void* i_data, int node_index, int depth) {
    cBgW_GrpElm* pnode = &pm_grp[node_index];
    if (!pnode->m_aab.Cross(psphchk)) {
        return false;
    }

    if (ChkGrpThrough(node_index, psphchk->GetGrpPassChk(), depth)) {
        return false;
    }

    bool chk = false;
    cBgD_Grp_t* data = &pm_bgd->m_g_tbl[node_index];
    if (data->m_tree_idx != 0xFFFF && SphChkRp(psphchk, i_data, data->m_tree_idx)) {
        chk = true;
    }

    depth++;
    int child_idx = data->m_first_child;
    while (true) {
        if (child_idx == 0xFFFF) {
            break;
        }
        if (SphChkGrpRp(psphchk, i_data, child_idx, depth)) {
            chk = true;
        }
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return chk;
}

bool dBgW::SphChk(dBgS_SphChk* psphchk, void* i_data) {
    return SphChkGrpRp(psphchk, i_data, m_rootGrpIdx, 1);
}

u8 dBgW::GetPolyGrpRoomInfId(const cBgS_PolyInfo& polyinfo) {
    u32 inf = GetGrpInf(polyinfo);
    u8 id = inf & 0xFF;
    return id;
}

int dBgW::GetGrpSoundId(const cBgS_PolyInfo& polyinfo) {
    u32 inf = GetGrpInf(polyinfo);
    int id = (inf >> 11) & 0xFF;
    return id;
}

void dBgW::CrrPos(const cBgS_PolyInfo& polyinfo, void* actor_ptr, bool param_2, cXyz* ppos,
                  csXyz* pangle, csXyz* pshapeangle) {
    if (m_crr_func) {
        m_crr_func(this, actor_ptr, polyinfo, param_2, ppos, pangle, pshapeangle);
    }
}

void dBgW::TransPos(const cBgS_PolyInfo& polyinfo, void* actor_ptr, bool param_2, cXyz* ppos,
                    csXyz* pangle, csXyz* pshapeangle) {
    if (m_crr_func) {
        m_crr_func(this, actor_ptr, polyinfo, param_2, ppos, pangle, pshapeangle);
    }
}

void dBgW::MatrixCrrPos(const cBgS_PolyInfo& polyinfo, void* actor_ptr, bool param_2, cXyz* ppos,
                        csXyz* pangle, csXyz* pshapeangle) {
    CrrPos(polyinfo, actor_ptr, param_2, ppos, pangle, pshapeangle);
}

bool dBgW::ChkPolyThrough(int poly_index, cBgS_PolyPassChk* ppass_chk) {
    if (ppass_chk == NULL) {
        return false;
    }
    dBgS_PolyPassChk* chk = (dBgS_PolyPassChk*)ppass_chk;

    if (chk->ChkObj() && GetPolyObjThrough(poly_index)) {
        return true;
    }

    if (chk->ChkCam() && GetPolyCamThrough(poly_index)) {
        return true;
    }

    if ((chk->ChkLink() && GetPolyLinkThrough(poly_index)) ||
        (chk->ChkHorse() && GetPolyLinkThrough(poly_index)))
    {
        return true;
    }

    if (chk->ChkArrow() && GetPolyArrowThrough(poly_index)) {
        return true;
    }

    if (chk->ChkBomb() && GetPolyBombThrough(poly_index)) {
        return true;
    }

    if (chk->ChkBoomerang() && GetPolyBoomerangThrough(poly_index)) {
        return true;
    }

    if (chk->ChkRope() && GetPolyRopeThrough(poly_index)) {
        return true;
    }

    if (chk->ChkUnderwaterRoof() && GetUnderwaterRoofCode(poly_index)) {
        return true;
    }

    int info_f00 = GetPolyInf1(poly_index, 0xF00, 8);
    if (info_f00 == 8 && chk->ChkNoHorse()) {
        return true;
    }

    if (info_f00 == 9) {
        if (chk->ChkNoHorse()) {
            return true;
        }

        cXyz pos(pm_tri[poly_index].m_plane.mNormal);
        if (chk->ChkHorse() && dComIfGp_getHorseActor() != NULL) {
            daHorse_c* horse = dComIfGp_getHorseActor();
            if (!horse->checkSpecialWallHit(pos)) {
                return true;
            }
        }
    }

    if (chk->ChkStatue() && GetSpecialCode(poly_index) == 7) {
        return true;
    }

    if (chk->ChkIronBall() && GetIronBallThrough(poly_index)) {
        return true;
    }

    return false;
}

bool dBgW::ChkShdwDrawThrough(int poly_index, cBgS_PolyPassChk* ppchk) {
    UNUSED(ppchk);

    if (GetShdwThrough(poly_index)) {
        return true;
    }

    if (GetPolyArrowThrough(poly_index)) {
        return true;
    }

    return false;
}

bool dBgW::ChkGrpThrough(int g, cBgS_GrpPassChk* ppasschk, int depth) {
    if (depth != 2 || ppasschk == NULL) {
        return false;
    }

    dBgS_GrpPassChk* var_r31 = (dBgS_GrpPassChk*)ppasschk;

    u32 var_r29 = pm_bgd->m_g_tbl[g].m_info & 0x100;
    if (!var_r29 && var_r31->MaskNormalGrp())
    {
        return false;
    }

    u32 var_r28 = pm_bgd->m_g_tbl[g].m_info & 0x100;
    if (var_r28 && var_r31->MaskWaterGrp())
    {
        return false;
    }

    return true;
}

void dBgW::CallRideCallBack(fopAc_ac_c* param_0, fopAc_ac_c* param_1) {
    if (m_ride_callback != NULL) {
        m_ride_callback(this, param_0, param_1);
    }
}

void dBgW::CallArrowStickCallBack(fopAc_ac_c* param_0, fopAc_ac_c* param_1, cXyz& param_2) {
    if (m_arrow_stick_callback) {
        m_arrow_stick_callback(this, param_0, param_1, param_2);
    }
}

void dBgW::OffMoveFlag() {
    m_flags &= (u8)~1;
}

u32 dBgW::ChkMoveFlag() const {
    return m_flags & 1;
}

dBgW* dBgW_NewSet(cBgD_t* pbgd, u32 flags, Mtx* pbase_mtx) {
    dBgW* nw = JKR_NEW dBgW();

    if (nw == NULL) {
        OS_REPORT_ERROR("dBgW_NewSet():dBgWの確保失敗\n");
        return NULL;
    }

    bool rt = nw->Set(pbgd, flags, pbase_mtx);
    if (rt) {
        OS_REPORT_ERROR("dBgW_NewSet():dBgWのSet失敗\n");
        return NULL;
    }

    return nw;
}

extern u8 struct_80450F88[8];
u8 struct_80450F88[8];
