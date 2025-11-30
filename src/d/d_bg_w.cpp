/**
 * d_bg_w.cpp
 * DZB Collision handling
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_w.h"
#include "SSystem/SComponent/c_bg_s_shdw_draw.h"
#include "SSystem/SComponent/c_m2d.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_bg_s_cap_poly.h"
#include "d/d_bg_s_sph_chk.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_horse.h"

cBgW_RwgElm::cBgW_RwgElm() {}

cBgW_RwgElm::~cBgW_RwgElm() {}

cBgW_NodeTree::cBgW_NodeTree() {}

cBgW_NodeTree::~cBgW_NodeTree() {}

cBgW::cBgW() {
    pm_bgd = NULL;
    mFlags = GLOBAL_e;
    mNeedsFullTransform = 1;
    mMoveCounter = 0;
    mTransVel.set(0.0f, 0.0f, 0.0f);

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
                Vec* vtx = &pm_vtx_tbl[i];
                VECAdd(vtx, &mTransVel, vtx);
            }
        } else {
            for (int i = 0; i < pm_bgd->m_v_num; i++) {
                MTXMultVec(pm_base, &pm_bgd->m_v_tbl[i], &pm_vtx_tbl[i]);
            }
        }
    }
}

bool cBgW::SetVtx() {
    if (mFlags & NO_VTX_TBL_e) {
        pm_vtx_tbl = NULL;
    } else if (mFlags & MOVE_BG_e) {
        pm_vtx_tbl = (cBgD_Vtx_t*)new Vec[pm_bgd->m_v_num];

        if (pm_vtx_tbl == NULL) {
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
                pm_tri[i].m_plane.mD -= VECDotProduct(&pm_tri[i].m_plane.mNormal, &mTransVel);
            }
        } else {
            for (int i = 0; i < pm_bgd->m_t_num; i++) {
                pm_tri[i].m_plane.SetupFrom3Vtx(&pm_vtx_tbl[tri[i].m_vtx_idx0],
                                                &pm_vtx_tbl[tri[i].m_vtx_idx1],
                                                &pm_vtx_tbl[tri[i].m_vtx_idx2]);
            }
        }
    }
}

bool cBgW::SetTri() {
    pm_tri = new cBgW_TriElm[pm_bgd->m_t_num];

    if (pm_tri == NULL) {
        return true;
    }

    CalcPlane();
    return false;
}

cBgW_TriElm::~cBgW_TriElm() {}

cBgW_TriElm::cBgW_TriElm() {}

void cBgW::BlckConnect(u16* i_start_idx, int* i_prev_idx, int i_idx) {
    if (*i_start_idx == 0xFFFF) {
        *i_start_idx = i_idx;
    }

    if (*i_prev_idx != 0xFFFF) {
        pm_rwg[*i_prev_idx].m_next = i_idx;
    }

    *i_prev_idx = i_idx;
    pm_rwg[*i_prev_idx].m_next = 0xFFFF;
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
                    if (!ChkRoofRegist()) {
                        BlckConnect(&pm_blk[i].m_roof_idx, &blk_roof_idx, j);
                    }
                } else {
                    BlckConnect(&pm_blk[i].m_wall_idx, &blk_wall_idx, j);
                }
            }
        }
    }
}

void cBgW::MakeBlckTransMinMax(cXyz* i_min, cXyz* i_max) {
    VECAdd(i_min, &mTransVel, i_min);
    VECAdd(i_max, &mTransVel, i_max);
}

void cBgW::MakeBlckMinMax(int vtx_index, cXyz* i_min, cXyz* i_max) {
    Vec* vtx = &pm_vtx_tbl[vtx_index];

    if (i_min->x > vtx->x) {
        i_min->x = vtx->x;
    }

    if (i_max->x < vtx->x) {
        i_max->x = vtx->x;
    }

    if (i_min->y > vtx->y) {
        i_min->y = vtx->y;
    }

    if (i_max->y < vtx->y) {
        i_max->y = vtx->y;
    }

    if (i_min->z > vtx->z) {
        i_min->z = vtx->z;
    }

    if (i_max->z < vtx->z) {
        i_max->z = vtx->z;
    }
}

void cBgW::MakeBlckBnd(int i_blk_idx, cXyz* i_min, cXyz* i_max) {
    int start = pm_bgd->m_b_tbl[i_blk_idx].field_0x0;
    int max;

    if (i_blk_idx != pm_bgd->m_b_num - 1) {
        max = pm_bgd->m_b_tbl[i_blk_idx + 1].field_0x0 - 1;
    } else {
        max = pm_bgd->m_t_num - 1;
    }

    if (mNeedsFullTransform == 0) {
        MakeBlckTransMinMax(i_min, i_max);
    } else {
        i_min->z = G_CM3D_F_INF;
        i_min->y = G_CM3D_F_INF;
        i_min->x = G_CM3D_F_INF;
        i_max->z = -G_CM3D_F_INF;
        i_max->y = -G_CM3D_F_INF;
        i_max->x = -G_CM3D_F_INF;

        for (int i = start; i <= max; i++) {
            MakeBlckMinMax(pm_bgd->m_t_tbl[i].m_vtx_idx0, i_min, i_max);
            MakeBlckMinMax(pm_bgd->m_t_tbl[i].m_vtx_idx1, i_min, i_max);
            MakeBlckMinMax(pm_bgd->m_t_tbl[i].m_vtx_idx2, i_min, i_max);
        }

        i_min->x -= 1.0f;
        i_min->y -= 1.0f;
        i_min->z -= 1.0f;
        i_max->x += 1.0f;
        i_max->y += 1.0f;
        i_max->z += 1.0f;
    }
}

void cBgW::MakeNodeTreeRp(int i_tree_idx) {
    cBgD_Tree_t* tree_data = &pm_bgd->m_tree_tbl[i_tree_idx];

    if (tree_data->m_flag & 1) {
        int child_idx = tree_data->m_id[0];

        if (child_idx != 0xFFFF) {
            cXyz* max = (cXyz*)pm_node_tree[i_tree_idx].GetMaxP();
            cXyz* min = (cXyz*)pm_node_tree[i_tree_idx].GetMinP();

            MakeBlckBnd(child_idx, min, max);
        }
    } else {
        pm_node_tree[i_tree_idx].ClearForMinMax();

        for (int i = 0; i < 8; i++) {
            int child_idx = tree_data->m_id[i];

            if (child_idx != 0xFFFF) {
                MakeNodeTreeRp(child_idx);
                pm_node_tree[i_tree_idx].SetMinMax(*pm_node_tree[child_idx].GetMinP());
                pm_node_tree[i_tree_idx].SetMinMax(*pm_node_tree[child_idx].GetMaxP());
            }
        }
    }
}

void cBgW::MakeNodeTreeGrpRp(int i_grp_idx) {
    if (pm_bgd->m_g_tbl[i_grp_idx].m_tree_idx != 0xFFFF) {
        MakeNodeTreeRp(pm_bgd->m_g_tbl[i_grp_idx].m_tree_idx);
        pm_grp[i_grp_idx].m_aab.SetMin(
            *pm_node_tree[pm_bgd->m_g_tbl[i_grp_idx].m_tree_idx].GetMinP());
        pm_grp[i_grp_idx].m_aab.SetMax(
            *pm_node_tree[pm_bgd->m_g_tbl[i_grp_idx].m_tree_idx].GetMaxP());
    }

    int child_idx = pm_bgd->m_g_tbl[i_grp_idx].m_first_child;
    while (true) {
        if (child_idx == 0xFFFF)
            break;
        MakeNodeTreeGrpRp(child_idx);
        pm_grp[i_grp_idx].m_aab.SetMin(*pm_grp[child_idx].m_aab.GetMinP());
        pm_grp[i_grp_idx].m_aab.SetMax(*pm_grp[child_idx].m_aab.GetMaxP());
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
    mFlags = GLOBAL_e;
    pm_vtx_tbl = NULL;
    pm_tri = NULL;
    pm_rwg = NULL;
    pm_blk = NULL;
    pm_node_tree = NULL;
    pm_grp = NULL;
    mMoveCounter = cM_rndF(128.0f);

    if (pbgd == NULL) {
        return true;
    }

    mFlags = flags;
    if (mFlags & GLOBAL_e) {
        pm_base = NULL;
        MTXIdentity(m_inv_mtx);
        MTXIdentity(m_mtx);
    } else {
        pm_base = *pbase_mtx;
        MTXCopy(pm_base, m_inv_mtx);
        MTXCopy(pm_base, m_mtx);
    }

    pm_bgd = pbgd;

    if (SetVtx() || SetTri()) {
        FreeArea();
        return true;
    }

    pm_rwg = new cBgW_RwgElm[pm_bgd->m_t_num];
    if (pm_rwg == NULL) {
        FreeArea();
        return true;
    }

    pm_blk = new cBgW_BlkElm[pm_bgd->m_b_num];
    if (pm_blk == NULL) {
        FreeArea();
        return true;
    }

    pm_node_tree = new cBgW_NodeTree[pm_bgd->m_tree_num];
    if (pm_node_tree == NULL) {
        FreeArea();
        return true;
    }

    pm_grp = new cBgW_GrpElm[pm_bgd->m_g_num];
    if (pm_grp == NULL) {
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

bool cBgW::RwgLineCheck(u16 i_poly_index, cBgS_LinChk* i_linchk) {
    bool chk = false;
    cM3dGTri tri;

    while (true) {
        cBgD_Tri_t* tri_t = &pm_bgd->m_t_tbl[i_poly_index];

        tri.setBg(&pm_vtx_tbl[tri_t->m_vtx_idx0], &pm_vtx_tbl[tri_t->m_vtx_idx1],
                  &pm_vtx_tbl[tri_t->m_vtx_idx2], &pm_tri[i_poly_index].m_plane);

        cXyz cross_pos;
        if (tri.cross(i_linchk->GetLinP(), &cross_pos, i_linchk->ChkFrontFlag(),
                      i_linchk->ChkBackFlag()))
        {
            if (!ChkPolyThrough(i_poly_index, i_linchk->GetPolyPassChk())) {
                i_linchk->SetCross(cross_pos);
                i_linchk->SetPolyIndex(i_poly_index);
                chk = true;
            }
        }

        if (pm_rwg[i_poly_index].m_next == 0xFFFF) {
            break;
        }
        i_poly_index = pm_rwg[i_poly_index].m_next;
    }

    return chk;
}

bool cBgW::LineCheckRp(cBgS_LinChk* i_linchk, int i_idx) {
    cBgW_NodeTree* node = &pm_node_tree[i_idx];

    if (!cM3d_Cross_MinMaxBoxLine(node->GetMinP(), node->GetMaxP(),
                                  &i_linchk->GetLinP()->GetStartP(),
                                  &i_linchk->GetLinP()->GetEndP()))
    {
        return false;
    }

    cBgD_Tree_t* tree = &pm_bgd->m_tree_tbl[i_idx];
    bool chk = false;

    if (tree->m_flag & 1) {
        if (i_linchk->GetPreWallChk() && pm_blk[tree->m_id[0]].m_wall_idx != 0xFFFF &&
            RwgLineCheck(pm_blk[tree->m_id[0]].m_wall_idx, i_linchk))
        {
            chk = true;
        }
        if (i_linchk->GetPreGroundChk() && pm_blk[tree->m_id[0]].m_gnd_idx != 0xFFFF &&
            RwgLineCheck(pm_blk[tree->m_id[0]].m_gnd_idx, i_linchk))
        {
            chk = true;
        }
        if (i_linchk->GetPreRoofChk() && pm_blk[tree->m_id[0]].m_roof_idx != 0xFFFF &&
            RwgLineCheck(pm_blk[tree->m_id[0]].m_roof_idx, i_linchk))
        {
            chk = true;
        }

        return chk;
    }

    for (int i = 0; i < 8; i++) {
        if (tree->m_id[i] != 0xFFFF && LineCheckRp(i_linchk, tree->m_id[i])) {
            chk = true;
        }
    }

    return chk;
}

bool cBgW::LineCheckGrpRp(cBgS_LinChk* i_linchk, int i_grp_idx, int depth) {
    cM3dGLin* pline = i_linchk->GetLinP();
    if (!pm_grp[i_grp_idx].m_aab.Cross(pline)) {
        return false;
    }

    if (ChkGrpThrough(i_grp_idx, i_linchk->GetGrpPassChk(), depth)) {
        return false;
    }

    bool chk = false;

    if (pm_bgd->m_g_tbl[i_grp_idx].m_tree_idx != 0xFFFF &&
        LineCheckRp(i_linchk, pm_bgd->m_g_tbl[i_grp_idx].m_tree_idx))
    {
        chk = true;
    }

    int child_idx = pm_bgd->m_g_tbl[i_grp_idx].m_first_child;
    while (true) {
        if (child_idx == 0xFFFF)
            break;

        if (LineCheckGrpRp(i_linchk, child_idx, depth + 1)) {
            chk = true;
        }
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return chk;
}

bool cBgW::LineCheck(cBgS_LinChk* i_linchk) {
    return LineCheckGrpRp(i_linchk, m_rootGrpIdx, 1);
}

bool cBgW::RwgGroundCheckCommon(f32 i_yPos, u16 i_poly_idx, cBgS_GndChk* i_gndchk) {
    if (i_yPos < i_gndchk->GetPointP().y && i_yPos > i_gndchk->GetNowY()) {
        cBgD_Tri_t* tri = &pm_bgd->m_t_tbl[i_poly_idx];
        if (cM3d_CrossY_Tri_Front(pm_vtx_tbl[tri->m_vtx_idx0], pm_vtx_tbl[tri->m_vtx_idx1],
                                  pm_vtx_tbl[tri->m_vtx_idx2], (const Vec*)&i_gndchk->GetPointP()))
        {
            if (!ChkPolyThrough(i_poly_idx, i_gndchk->GetPolyPassChk())) {
                i_gndchk->SetNowY(i_yPos);
                i_gndchk->SetPolyIndex(i_poly_idx);
                return true;
            }
        }
    }

    return false;
}

bool cBgW::RwgGroundCheckGnd(u16 i_poly_idx, cBgS_GndChk* i_gndchk) {
    bool chk = false;
    while (true) {
        cBgW_RwgElm* rwg = &pm_rwg[i_poly_idx];
        f32 tri_y = pm_tri[i_poly_idx].m_plane.getCrossY_NonIsZero(&i_gndchk->GetPointP());
        if (RwgGroundCheckCommon(tri_y, (u32)i_poly_idx, i_gndchk)) {
            chk = true;
        }

        if (rwg->m_next == 0xFFFF)
            break;

        i_poly_idx = rwg->m_next;
    }
    return chk;
}

bool cBgW::RwgGroundCheckWall(u16 i_poly_idx, cBgS_GndChk* i_gndchk) {
    bool chk = false;
    while (true) {
        cBgW_TriElm* tri = &pm_tri[i_poly_idx];
        cBgW_RwgElm* rwg = &pm_rwg[i_poly_idx];
        if (tri->m_plane.mNormal.y >= 0.014f) {
            f32 tri_y = tri->m_plane.getCrossY_NonIsZero(&i_gndchk->GetPointP());
            if (RwgGroundCheckCommon(tri_y, (u32)i_poly_idx, i_gndchk)) {
                chk = true;
            }
        }

        if (rwg->m_next == 0xFFFF)
            break;
        i_poly_idx = rwg->m_next;
    }

    return chk;
}

bool cBgW::GroundCrossRp(cBgS_GndChk* i_gndchk, int i_idx) {
    bool chk = false;
    cBgD_Tree_t* tree = &pm_bgd->m_tree_tbl[i_idx];
    if ((tree->m_flag & 1)) {
        if (pm_blk[tree->m_id[0]].m_gnd_idx != 0xFFFF &&
            RwgGroundCheckGnd(pm_blk[tree->m_id[0]].m_gnd_idx, i_gndchk))
        {
            chk = true;
        }

        if (i_gndchk->GetWallPrecheck() && pm_blk[tree->m_id[0]].m_wall_idx != 0xFFFF &&
            RwgGroundCheckWall(pm_blk[tree->m_id[0]].m_wall_idx, i_gndchk))
        {
            chk = true;
        }
        return chk;
    }

    for (int i = 0; i < 8; i++) {
        if (tree->m_id[i] != 0xFFFF) {
            cM3dGAab* aab = &pm_node_tree[tree->m_id[i]];
            if (aab->CrossY(&i_gndchk->GetPointP())) {
                if (aab->UnderPlaneYUnder(i_gndchk->GetPointP().y)) {
                    if (!aab->TopPlaneYUnder(i_gndchk->GetNowY()) &&
                        GroundCrossRp(i_gndchk, tree->m_id[i]))
                    {
                        chk = true;
                    }
                }
            }
        }
    }

    return chk;
}

bool cBgW::GroundCrossGrpRp(cBgS_GndChk* i_gndchk, int i_grp_idx, int i_depth) {
    cBgW_GrpElm* grp = &pm_grp[i_grp_idx];
    if (!grp->m_aab.CrossY(&i_gndchk->GetPointP()) ||
        !grp->m_aab.UnderPlaneYUnder(i_gndchk->GetPointP().y) ||
        grp->m_aab.TopPlaneYUnder(i_gndchk->GetNowY()))
    {
        return false;
    }

    if (ChkGrpThrough(i_grp_idx, i_gndchk->GetGrpPassChk(), i_depth)) {
        return false;
    }

    bool chk = false;

    if (pm_bgd->m_g_tbl[i_grp_idx].m_tree_idx != 0xFFFF &&
        GroundCrossRp(i_gndchk, pm_bgd->m_g_tbl[i_grp_idx].m_tree_idx))
    {
        chk = true;
    }

    int child_idx = pm_bgd->m_g_tbl[i_grp_idx].m_first_child;
    while (true) {
        if (child_idx == 0xFFFF)
            break;

        if (GroundCrossGrpRp(i_gndchk, child_idx, i_depth + 1)) {
            chk = true;
        }

        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return chk;
}

bool cBgW::GroundCross(cBgS_GndChk* i_gndchk) {
    return GroundCrossGrpRp(i_gndchk, m_rootGrpIdx, 1);
}

void cBgW::CopyOldMtx() {
    if (pm_base != NULL) {
        MTXCopy(m_mtx, m_inv_mtx);
        MTXCopy(pm_base, m_mtx);
    }
}

void cBgW::Move() {
    if (!ChkLock() && (mFlags & MOVE_BG_e)) {
        if (!ChkNoCalcVtx()) {
            if (mMoveCounter >= 0xFF || m_mtx[0][0] != pm_base[0][0] ||
                m_mtx[0][1] != pm_base[0][1] || m_mtx[0][2] != pm_base[0][2] ||
                m_mtx[1][0] != pm_base[1][0] || m_mtx[1][1] != pm_base[1][1] ||
                m_mtx[1][2] != pm_base[1][2] || m_mtx[2][0] != pm_base[2][0] ||
                m_mtx[2][1] != pm_base[2][1] || m_mtx[2][2] != pm_base[2][2])
            {
                mNeedsFullTransform = 1;
            } else if (m_mtx[0][3] == pm_base[0][3] && m_mtx[1][3] == pm_base[1][3] &&
                       m_mtx[2][3] == pm_base[2][3])
            {
                MTXCopy(pm_base, m_inv_mtx);
                if (!ChkFlush()) {
                    return;
                }
            } else {
                mTransVel.x = pm_base[0][3] - m_mtx[0][3];
                mTransVel.y = pm_base[1][3] - m_mtx[1][3];
                mTransVel.z = pm_base[2][3] - m_mtx[2][3];
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

void cBgW::RwgShdwDraw(int i_idx, cBgS_ShdwDraw* i_shdw) {
    while (true) {
        cBgW_RwgElm* rwg = &pm_rwg[i_idx];
        if (!ChkShdwDrawThrough(i_idx, i_shdw->GetPolyPassChk())) {
            (i_shdw->mCallbackFun)(i_shdw, pm_vtx_tbl, pm_bgd->m_t_tbl[i_idx].m_vtx_idx0,
                                   pm_bgd->m_t_tbl[i_idx].m_vtx_idx1,
                                   pm_bgd->m_t_tbl[i_idx].m_vtx_idx2, &pm_tri[i_idx].m_plane);
        }

        if (rwg->m_next == 0xFFFF)
            break;
        i_idx = rwg->m_next;
    }
}

void cBgW::ShdwDrawRp(cBgS_ShdwDraw* i_shdw, int i_idx) {
    if (pm_node_tree[i_idx].Cross(i_shdw->GetBndP())) {
        cBgD_Tree_t* tree = &pm_bgd->m_tree_tbl[i_idx];
        if ((tree->m_flag & 1)) {
            if (pm_blk[tree->m_id[0]].m_wall_idx != 0xFFFF) {
                RwgShdwDraw(pm_blk[tree->m_id[0]].m_wall_idx, i_shdw);
            }
            if (pm_blk[tree->m_id[0]].m_roof_idx != 0xFFFF) {
                RwgShdwDraw(pm_blk[tree->m_id[0]].m_roof_idx, i_shdw);
            }
            if (pm_blk[tree->m_id[0]].m_gnd_idx != 0xFFFF) {
                RwgShdwDraw(pm_blk[tree->m_id[0]].m_gnd_idx, i_shdw);
            }
        } else {
            for (int i = 0; i < 8; i++) {
                if (tree->m_id[i] != 0xFFFF) {
                    ShdwDrawRp(i_shdw, tree->m_id[i]);
                }
            }
        }
    }
}

void cBgW::ShdwDrawGrpRp(cBgS_ShdwDraw* i_shdw, int i_idx) {
    if (pm_grp[i_idx].m_aab.Cross(i_shdw->GetBndP())) {
        if (pm_bgd->m_g_tbl[i_idx].m_tree_idx != 0xFFFF) {
            ShdwDrawRp(i_shdw, pm_bgd->m_g_tbl[i_idx].m_tree_idx);
        }

        int child_idx = pm_bgd->m_g_tbl[i_idx].m_first_child;
        while (true) {
            if (child_idx == 0xFFFF) {
                break;
            }
            ShdwDrawGrpRp(i_shdw, child_idx);
            child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
        }
    }
}

void cBgW::ShdwDraw(cBgS_ShdwDraw* i_shdw) {
    ShdwDrawGrpRp(i_shdw, m_rootGrpIdx);
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

int cBgW::GetGrpRoomIndex(cBgS_PolyInfo const& poly) const {
    int grp_index = GetTriGrp(poly.GetPolyIndex());

    u16 parent_idx = pm_bgd->m_g_tbl[grp_index].m_parent;

    if (parent_idx == 0xFFFF || pm_bgd->m_g_tbl[parent_idx].m_parent == 0xFFFF) {
        return 0xFF;
    }

    int room_index = pm_bgd->m_g_tbl[pm_bgd->m_g_tbl[parent_idx].m_parent].m_room_id;
    if (room_index >= 0xFF) {
        room_index = 0xFF;
    }

    return room_index;
}

cM3dGAab* cBgW::GetBnd() const {
    return &pm_grp[m_rootGrpIdx].m_aab;
}

void cBgW::GetTrans(cXyz* o_trans) const {
    MtxP base = pm_base;
    o_trans->x = base[0][3] - m_inv_mtx[0][3];
    o_trans->y = base[1][3] - m_inv_mtx[1][3];
    o_trans->z = base[2][3] - m_inv_mtx[2][3];
}

bool cBgW::GetTriPnt(cBgS_PolyInfo const& poly, cXyz* o_pntA, cXyz* o_pntB, cXyz* o_pntC) const {
    int poly_index = poly.GetPolyIndex();
    cBgD_Tri_t* tri = &pm_bgd->m_t_tbl[poly_index];

    o_pntA->set(pm_vtx_tbl[tri->m_vtx_idx0]);
    o_pntB->set(pm_vtx_tbl[tri->m_vtx_idx1]);
    o_pntC->set(pm_vtx_tbl[tri->m_vtx_idx2]);
    return true;
}

void cBgW::GetTopUnder(f32* o_top, f32* o_under) const {
    *o_under = pm_grp[m_rootGrpIdx].m_aab.GetMinP()->y;
    *o_top = pm_grp[m_rootGrpIdx].m_aab.GetMaxP()->y;
}

cM3dGPla cBgW::GetTriPla(cBgS_PolyInfo const& poly) const {
    int poly_index = poly.GetPolyIndex();
    JUT_ASSERT(1956, 0 <= poly_index && poly_index < pm_bgd->m_t_num);
    return pm_tri[poly_index].m_plane;
}

u32 cBgW::GetGrpInf(cBgS_PolyInfo const& poly) const {
    int grp = GetTriGrp(poly.GetPolyIndex());
    return pm_bgd->m_g_tbl[grp].m_info;
}

int cBgW::GetPolyInfId(int poly_index) const {
    return pm_bgd->m_t_tbl[poly_index].m_id;
}

u32 cBgW::GetPolyInf0(int poly_index, u32 mask, u32 shift) const {
    int id = GetPolyInfId(poly_index);
    return (pm_bgd->m_ti_tbl[id].m_info0 & mask) >> shift;
}

u32 cBgW::GetMaskPolyInf0_NoShift(int poly_index, u32 mask) const {
    int id = GetPolyInfId(poly_index);
    return (pm_bgd->m_ti_tbl[id].m_info0 & mask);
}

int dBgW::GetExitId(cBgS_PolyInfo const& poly) {
    return GetPolyInf0(poly.GetPolyIndex(), 0x3F, 0);
}

int dBgW::GetPolyColor(cBgS_PolyInfo const& poly) {
    return GetPolyInf0(poly.GetPolyIndex(), 0x3FC0, 6);
}

BOOL dBgW::GetHorseNoEntry(cBgS_PolyInfo const& poly) {
    return GetPolyInf0(poly.GetPolyIndex(), 0x200000, 21);
}

int dBgW::GetSpecialCode(cBgS_PolyInfo const& poly) {
    return GetSpecialCode(poly.GetPolyIndex());
}

int dBgW::GetSpecialCode(int poly_index) {
    return GetPolyInf0(poly_index, 0xF000000, 24);
}

int dBgW::GetMagnetCode(cBgS_PolyInfo const& poly) {
    return GetPolyInf0(poly.GetPolyIndex(), 0x30000000, 28);
}

int dBgW::GetMonkeyBarsCode(cBgS_PolyInfo const& poly) {
    return GetPolyInf0(poly.GetPolyIndex(), 0x80000000, 31);
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
    return (pm_bgd->m_ti_tbl[id].m_info1 & mask) >> shift;
}

int dBgW::GetLinkNo(cBgS_PolyInfo const& poly) {
    return GetPolyInf1(poly.GetPolyIndex(), 0xFF, 0);
}

int dBgW::GetWallCode(cBgS_PolyInfo const& poly) {
    return GetPolyInf1(poly.GetPolyIndex(), 0xF00, 8);
}

int dBgW::GetPolyAtt0(cBgS_PolyInfo const& poly) {
    return GetPolyInf1(poly.GetPolyIndex(), 0xF000, 12);
}

int dBgW::GetPolyAtt1(cBgS_PolyInfo const& poly) {
    return GetPolyInf1(poly.GetPolyIndex(), 0x70000, 16);
}

int dBgW::GetGroundCode(cBgS_PolyInfo const& poly) {
    return GetPolyInf1(poly.GetPolyIndex(), 0xF80000, 19);
}

u32 cBgW::GetMaskPolyInf1_NoShift(int poly_index, u32 mask) const {
    int id = GetPolyInfId(poly_index);
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
    return (pm_bgd->m_ti_tbl[id].m_info2 & mask) >> shift;
}

int dBgW::GetCamMoveBG(cBgS_PolyInfo const& poly) {
    return GetPolyInf2(poly.GetPolyIndex(), 0xFF, 0);
}

int dBgW::GetRoomCamId(cBgS_PolyInfo const& poly) {
    return GetPolyInf2(poly.GetPolyIndex(), 0xFF00, 8);
}

int dBgW::GetRoomPathId(cBgS_PolyInfo const& poly) {
    return GetPolyInf2(poly.GetPolyIndex(), 0xFF0000, 16);
}

int dBgW::GetRoomPathPntNo(cBgS_PolyInfo const& poly) {
    return GetPolyInf2(poly.GetPolyIndex(), 0xFF000000, 24);
}

int cBgW::GetTriGrp(int poly_index) const {
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

void dBgW::positionWallCorrect(dBgS_Acch* i_acch, f32 i_dist, cM3dGPla& plane, cXyz* pupper_pos,
                               f32 speed) {
    i_acch->SetWallHit();

    f32 speed_correct = speed - 1.0f;
    if (speed_correct < 0.0f) {
        speed_correct = 0.0f;
    }

    f32 move = speed_correct * i_dist;
    pupper_pos->x += move * plane.mNormal.x;
    pupper_pos->z += move * plane.mNormal.z;
}

bool dBgW::RwgWallCorrect(dBgS_Acch* pwi, u16 i_poly_idx) {
    bool correct = false;

    while (true) {
        cBgW_RwgElm* rwg_elm = &pm_rwg[i_poly_idx];

        if (!ChkPolyThrough(i_poly_idx, pwi->GetPolyPassChk())) {
            cBgW_TriElm* tri = &pm_tri[i_poly_idx];

            f32 sp68 = JMAFastSqrt(tri->m_plane.GetNP()->x * tri->m_plane.GetNP()->x +
                                   tri->m_plane.GetNP()->z * tri->m_plane.GetNP()->z);
            if (cM3d_IsZero(sp68)) {
                if (rwg_elm->m_next != 0xFFFF) {
                    i_poly_idx = rwg_elm->m_next;
                    continue;
                }
                break;
            }

            f32 sp6C = 1.0f / sp68;
            cBgD_Tri_t* tri_data = &pm_bgd->m_t_tbl[i_poly_idx];
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
                                    pwi->SetWallPolyIndex(cir_index, i_poly_idx);
                                    pwi->SetWallAngleY(cir_index,
                                                       cM_atan2s(tri->m_plane.GetNP()->x,
                                                                 tri->m_plane.GetNP()->z));
                                    correct = true;
                                } else {
                                    cx0 -= sp50.x;
                                    cy0 -= sp50.z;
                                    cx1 -= sp50.x;
                                    cy1 -= sp50.z;

                                    JUT_ASSERT(0, pwi->GetPos()->x ==
                                               pwi->GetWallCirP(cir_index)->GetCx());
                                    JUT_ASSERT(0, pwi->GetPos()->z ==
                                               pwi->GetWallCirP(cir_index)->GetCy());

                                    f32 spE0 =
                                        cM3d_Len2dSq(cx0, cy0, pwi->GetPos()->x, pwi->GetPos()->z);
                                    f32 spE4 =
                                        cM3d_Len2dSq(cx1, cy1, pwi->GetPos()->x, pwi->GetPos()->z);

                                    f32 onx = -tri->m_plane.GetNP()->x;
                                    f32 ony = -tri->m_plane.GetNP()->z;

                                    JUT_ASSERT(0, !(cM3d_IsZero(onx) && cM3d_IsZero(ony)));

                                    if (spE0 < spE4) {
                                        if (!(spE0 > spDC) && !(fabsf(spE0 - spDC) < 0.008f)) {
                                            JUT_ASSERT(0, !isnan(cx0));
                                            JUT_ASSERT(0, !isnan(cy0));

                                            f32 spF0, spF4;
                                            cM2d_CrossCirLin(*pwi->GetWallCirP(cir_index), cx0, cy0,
                                                             onx, ony, &spF0, &spF4);
                                            pwi->GetPos()->x += cx0 - spF0;
                                            pwi->GetPos()->z += cy0 - spF4;

                                            JUT_ASSERT(0, !isnan(pwi->GetPos()->x));
                                            JUT_ASSERT(0, !isnan(pwi->GetPos()->z));

                                            pwi->CalcMovePosWork();
                                            pwi->SetWallCirHit(cir_index);
                                            pwi->SetWallPolyIndex(cir_index, i_poly_idx);
                                            pwi->SetWallAngleY(cir_index,
                                                               cM_atan2s(tri->m_plane.GetNP()->x,
                                                                         tri->m_plane.GetNP()->z));
                                            correct = true;
                                            pwi->SetWallHit();
                                        }
                                    } else if (!(spE4 > spDC) && !(fabsf(spE4 - spDC) < 0.008f)) {
                                        JUT_ASSERT(0, !isnan(cx1));
                                        JUT_ASSERT(0, !isnan(cy1));

                                        f32 spF8, spFC;
                                        cM2d_CrossCirLin(*pwi->GetWallCirP(cir_index), cx1, cy1,
                                                         onx, ony, &spF8, &spFC);
                                        pwi->GetPos()->x += cx1 - spF8;
                                        pwi->GetPos()->z += cy1 - spFC;

                                        JUT_ASSERT(0, !isnan(pwi->GetPos()->x));
                                        JUT_ASSERT(0, !isnan(pwi->GetPos()->z));

                                        pwi->CalcMovePosWork();
                                        pwi->SetWallCirHit(cir_index);
                                        pwi->SetWallPolyIndex(cir_index, i_poly_idx);
                                        pwi->SetWallAngleY(cir_index,
                                                           cM_atan2s(tri->m_plane.GetNP()->x,
                                                                     tri->m_plane.GetNP()->z));
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

        i_poly_idx = rwg_elm->m_next;
    }

    return correct;
}

bool dBgW::WallCorrectRp(dBgS_Acch* i_acch, int i_idx) {
    if (!pm_node_tree[i_idx].Cross(i_acch->GetWallBmdCylP())) {
        return false;
    }

    cBgD_Tree_t* tree = &pm_bgd->m_tree_tbl[i_idx];
    bool chk = false;
    if ((tree->m_flag & 1)) {
        if (pm_blk[tree->m_id[0]].m_wall_idx != 0xFFFF &&
            RwgWallCorrect(i_acch, pm_blk[tree->m_id[0]].m_wall_idx))
        {
            chk = true;
        }
        if (pm_blk[tree->m_id[0]].m_roof_idx != 0xFFFF &&
            RwgWallCorrect(i_acch, pm_blk[tree->m_id[0]].m_roof_idx))
        {
            chk = true;
        }
        return chk;
    }

    for (int i = 0; i < 8; i++) {
        if (tree->m_id[i] != 0xFFFF && WallCorrectRp(i_acch, tree->m_id[i])) {
            chk = true;
        }
    }

    return chk;
}

bool dBgW::WallCorrectGrpRp(dBgS_Acch* i_acch, int i_idx, int i_depth) {
    if (!pm_grp[i_idx].m_aab.Cross(i_acch->GetWallBmdCylP())) {
        return false;
    }

    if (ChkGrpThrough(i_idx, i_acch->GetGrpPassChk(), i_depth)) {
        return false;
    }

    bool chk = false;
    if (pm_bgd->m_g_tbl[i_idx].m_tree_idx != 0xFFFF &&
        WallCorrectRp(i_acch, pm_bgd->m_g_tbl[i_idx].m_tree_idx))
    {
        chk = true;
    }

    int child_idx = pm_bgd->m_g_tbl[i_idx].m_first_child;
    while (true) {
        if (child_idx == 0xFFFF) {
            break;
        }
        if (WallCorrectGrpRp(i_acch, child_idx, i_depth + 1)) {
            chk = true;
        }
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return chk;
}

bool dBgW::WallCorrect(dBgS_Acch* pacch) {
    return WallCorrectGrpRp(pacch, m_rootGrpIdx, 1);
}

struct wcs_data {
    /* 0x0 */ f32 field_0x0;
    /* 0x4 */ u16 poly_idx;
    /* 0x8 */ wcs_data* next;
};

static wcs_data l_wcsbuf[84];

static wcs_data* l_start;

static int l_wcsbuf_num;

void dBgW::RwgWallCorrectSort(dBgS_Acch* i_acch, u16 i_poly_idx) {
    while (true) {
        cBgW_RwgElm* rwg_elm = &pm_rwg[i_poly_idx];

        if (!ChkPolyThrough(i_poly_idx, i_acch->GetPolyPassChk())) {
            cBgW_TriElm* tri_elm = &pm_tri[i_poly_idx];

            f32 var_f30 = JMAFastSqrt(tri_elm->m_plane.GetNP()->x * tri_elm->m_plane.GetNP()->x +
                                      tri_elm->m_plane.GetNP()->z * tri_elm->m_plane.GetNP()->z);
            if (cM3d_IsZero(var_f30)) {
                if (rwg_elm->m_next != 0xFFFF) {
                    i_poly_idx = rwg_elm->m_next;
                    continue;
                }
                break;
            }

            fopAc_ac_c* actor = i_acch->getMyAc();
            cXyz sp88;
            sp88.x = cM_ssin(actor->current.angle.y);
            sp88.z = cM_scos(actor->current.angle.y);

            f32 var_f29 = 1.0f / var_f30;
            f32 var_f31 = var_f29 * ((sp88.x * tri_elm->m_plane.GetNP()->x) +
                                     (sp88.z * tri_elm->m_plane.GetNP()->z));

            wcs_data* data = l_start;
            if (data == NULL) {
                if (l_wcsbuf_num < 84) {
                    l_wcsbuf[l_wcsbuf_num].field_0x0 = var_f31;
                    l_wcsbuf[l_wcsbuf_num].poly_idx = i_poly_idx;
                    l_wcsbuf[l_wcsbuf_num].next = NULL;
                    l_start = &l_wcsbuf[l_wcsbuf_num];
                    l_wcsbuf_num++;
                }
            } else if (i_poly_idx != l_start->poly_idx) {
                if (var_f31 > l_start->field_0x0) {
                    if (l_wcsbuf_num < 84) {
                        l_wcsbuf[l_wcsbuf_num].field_0x0 = var_f31;
                        l_wcsbuf[l_wcsbuf_num].poly_idx = i_poly_idx;
                        l_wcsbuf[l_wcsbuf_num].next = l_start;
                        l_start = &l_wcsbuf[l_wcsbuf_num];
                        l_wcsbuf_num++;
                    }
                } else {
                    while (true) {
                        wcs_data* tmp_data = data->next;
                        if (tmp_data == NULL) {
                            if (l_wcsbuf_num < 84) {
                                l_wcsbuf[l_wcsbuf_num].field_0x0 = var_f31;
                                l_wcsbuf[l_wcsbuf_num].poly_idx = i_poly_idx;
                                l_wcsbuf[l_wcsbuf_num].next = NULL;
                                data->next = &l_wcsbuf[l_wcsbuf_num];
                                l_wcsbuf_num++;
                            }
                            break;
                        } else if (i_poly_idx == tmp_data->poly_idx) {
                            break;
                        }

                        if (var_f31 > tmp_data->field_0x0) {
                            if (l_wcsbuf_num < 84) {
                                l_wcsbuf[l_wcsbuf_num].field_0x0 = var_f31;
                                l_wcsbuf[l_wcsbuf_num].poly_idx = i_poly_idx;
                                l_wcsbuf[l_wcsbuf_num].next = data->next;
                                data->next = &l_wcsbuf[l_wcsbuf_num];
                                l_wcsbuf_num++;
                            }
                            break;
                        }

                        data = tmp_data;
                    }
                }
            }
        }

        if (rwg_elm->m_next == 0xFFFF) {
            return;
        }

        i_poly_idx = rwg_elm->m_next;
    }
}

void dBgW::WallCorrectRpSort(dBgS_Acch* i_acch, int i_idx) {
    if (!pm_node_tree[i_idx].Cross(i_acch->GetWallBmdCylP())) {
        return;
    }

    cBgD_Tree_t* tree = &pm_bgd->m_tree_tbl[i_idx];
    if ((tree->m_flag & 1)) {
        if (pm_blk[tree->m_id[0]].m_wall_idx != 0xFFFF) {
            RwgWallCorrectSort(i_acch, pm_blk[tree->m_id[0]].m_wall_idx);
        }
        if (pm_blk[tree->m_id[0]].m_roof_idx != 0xFFFF) {
            RwgWallCorrectSort(i_acch, pm_blk[tree->m_id[0]].m_roof_idx);
        }
        return;
    }

    for (int i = 0; i < 8; i++) {
        if (tree->m_id[i] != 0xFFFF) {
            WallCorrectRpSort(i_acch, tree->m_id[i]);
        }
    }
}

bool dBgW::WallCorrectGrpRpSort(dBgS_Acch* i_acch, int i_idx, int i_depth) {
    if (!pm_grp[i_idx].m_aab.Cross(i_acch->GetWallBmdCylP())) {
        return false;
    }

    if (ChkGrpThrough(i_idx, i_acch->GetGrpPassChk(), i_depth)) {
        return false;
    }

    if (pm_bgd->m_g_tbl[i_idx].m_tree_idx != 0xFFFF) {
        WallCorrectRpSort(i_acch, pm_bgd->m_g_tbl[i_idx].m_tree_idx);
    }

    int child_idx = pm_bgd->m_g_tbl[i_idx].m_first_child;
    while (true) {
        if (child_idx == 0xFFFF) {
            break;
        }
        WallCorrectGrpRpSort(i_acch, child_idx, i_depth + 1);
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
                                pwi->SetWallAngleY(cir_index, cM_atan2s(tri->m_plane.GetNP()->x,
                                                                        tri->m_plane.GetNP()->z));
                                correct = true;
                            } else {
                                cx0 -= sp50.x;
                                cy0 -= sp50.z;
                                cx1 -= sp50.x;
                                cy1 -= sp50.z;

                                JUT_ASSERT(0, pwi->GetPos()->x ==
                                           pwi->GetWallCirP(cir_index)->GetCx());
                                JUT_ASSERT(0, pwi->GetPos()->z ==
                                           pwi->GetWallCirP(cir_index)->GetCy());

                                f32 spE0 =
                                    cM3d_Len2dSq(cx0, cy0, pwi->GetPos()->x, pwi->GetPos()->z);
                                f32 spE4 =
                                    cM3d_Len2dSq(cx1, cy1, pwi->GetPos()->x, pwi->GetPos()->z);

                                f32 onx = -tri->m_plane.GetNP()->x;
                                f32 ony = -tri->m_plane.GetNP()->z;

                                JUT_ASSERT(0, !(cM3d_IsZero(onx) && cM3d_IsZero(ony)));

                                if (spE0 < spE4) {
                                    if (!(spE0 > spDC) && !(fabsf(spE0 - spDC) < 0.008f)) {
                                        JUT_ASSERT(0, !isnan(cx0));
                                        JUT_ASSERT(0, !isnan(cy0));

                                        f32 spF0, spF4;
                                        cM2d_CrossCirLin(*pwi->GetWallCirP(cir_index), cx0, cy0,
                                                         onx, ony, &spF0, &spF4);
                                        pwi->GetPos()->x += cx0 - spF0;
                                        pwi->GetPos()->z += cy0 - spF4;

                                        JUT_ASSERT(0, !isnan(pwi->GetPos()->x));
                                        JUT_ASSERT(0, !isnan(pwi->GetPos()->z));

                                        pwi->CalcMovePosWork();
                                        pwi->SetWallCirHit(cir_index);
                                        pwi->SetWallPolyIndex(cir_index, poly_index);
                                        pwi->SetWallAngleY(cir_index,
                                                           cM_atan2s(tri->m_plane.GetNP()->x,
                                                                     tri->m_plane.GetNP()->z));
                                        correct = true;
                                        pwi->SetWallHit();
                                    }
                                } else if (!(spE4 > spDC) && !(fabsf(spE4 - spDC) < 0.008f)) {
                                    JUT_ASSERT(0, !isnan(cx1));
                                    JUT_ASSERT(0, !isnan(cy1));

                                    f32 spF8, spFC;
                                    cM2d_CrossCirLin(*pwi->GetWallCirP(cir_index), cx1, cy1, onx,
                                                     ony, &spF8, &spFC);
                                    pwi->GetPos()->x += cx1 - spF8;
                                    pwi->GetPos()->z += cy1 - spFC;

                                    JUT_ASSERT(0, !isnan(pwi->GetPos()->x));
                                    JUT_ASSERT(0, !isnan(pwi->GetPos()->z));

                                    pwi->CalcMovePosWork();
                                    pwi->SetWallCirHit(cir_index);
                                    pwi->SetWallPolyIndex(cir_index, poly_index);
                                    pwi->SetWallAngleY(cir_index,
                                                       cM_atan2s(tri->m_plane.GetNP()->x,
                                                                 tri->m_plane.GetNP()->z));
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

bool dBgW::RwgRoofChk(u16 i_poly_idx, dBgS_RoofChk* i_roofchk) {
    bool chk = false;
    f32 crossY;
    while (true) {
        if (pm_tri[i_poly_idx].m_plane.getCrossY(*i_roofchk->GetPosP(), &crossY) &&
            crossY > i_roofchk->GetPosP()->y && crossY < i_roofchk->GetNowY())
        {
            cBgD_Tri_t* tri = &pm_bgd->m_t_tbl[i_poly_idx];
            if (cM3d_CrossY_Tri(pm_vtx_tbl[tri->m_vtx_idx0], pm_vtx_tbl[tri->m_vtx_idx1],
                                pm_vtx_tbl[tri->m_vtx_idx2], pm_tri[i_poly_idx].m_plane,
                                (const Vec*)i_roofchk->GetPosP()))
            {
                if (!ChkPolyThrough(i_poly_idx, i_roofchk->GetPolyPassChk())) {
                    i_roofchk->SetNowY(crossY);
                    i_roofchk->SetPolyIndex(i_poly_idx);
                    chk = true;
                }
            }
        }

        if (pm_rwg[i_poly_idx].m_next == 0xFFFF)
            break;
        i_poly_idx = pm_rwg[i_poly_idx].m_next;
    }

    return chk;
}

bool dBgW::RoofChkRp(dBgS_RoofChk* i_roofchk, int i_idx) {
    cBgW_NodeTree* node = &pm_node_tree[i_idx];
    if (!node->CrossY(i_roofchk->GetPosP()) || !node->UnderPlaneYUnder(i_roofchk->GetNowY()) ||
        node->TopPlaneYUnder(i_roofchk->GetPosP()->y))
    {
        return false;
    }

    cBgD_Tree_t* tree = &pm_bgd->m_tree_tbl[i_idx];
    if ((tree->m_flag & 1)) {
        if (pm_blk[tree->m_id[0]].m_roof_idx != 0xFFFF &&
            RwgRoofChk(pm_blk[tree->m_id[0]].m_roof_idx, i_roofchk))
        {
            return true;
        }
        return false;
    }

    bool chk = false;
    for (int i = 0; i < 8; i++) {
        if (tree->m_id[i] != 0xFFFF && RoofChkRp(i_roofchk, tree->m_id[i])) {
            chk = true;
        }
    }

    return chk;
}

bool dBgW::RoofChkGrpRp(dBgS_RoofChk* i_roofchk, int i_idx, int i_depth) {
    cBgW_GrpElm* grp = &pm_grp[i_idx];
    if (!grp->m_aab.CrossY(i_roofchk->GetPosP()) ||
        !grp->m_aab.UnderPlaneYUnder(i_roofchk->GetNowY()) ||
        grp->m_aab.TopPlaneYUnder(i_roofchk->GetPosP()->y))
    {
        return false;
    }

    if (ChkGrpThrough(i_idx, i_roofchk->GetGrpPassChk(), i_depth)) {
        return false;
    }

    bool chk = false;
    cBgD_Grp_t* data = &pm_bgd->m_g_tbl[i_idx];
    if (data->m_tree_idx != 0xFFFF && RoofChkRp(i_roofchk, data->m_tree_idx)) {
        chk = true;
    }

    int child_idx = data->m_first_child;
    while (true) {
        if (child_idx == 0xFFFF) {
            break;
        }
        if (RoofChkGrpRp(i_roofchk, child_idx, i_depth + 1)) {
            chk = true;
        }
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return chk;
}

bool dBgW::RoofChk(dBgS_RoofChk* pchk) {
    return RoofChkGrpRp(pchk, m_rootGrpIdx, 1);
}

bool dBgW::RwgSplGrpChk(u16 i_poly_idx, dBgS_SplGrpChk* i_splchk) {
    bool chk = false;
    f32 crossY;
    while (true) {
        if (pm_tri[i_poly_idx].m_plane.getCrossY(i_splchk->GetPosP(), &crossY) &&
            crossY < i_splchk->GetRoof() && crossY > i_splchk->GetHeight())
        {
            cBgD_Tri_t* tri = &pm_bgd->m_t_tbl[i_poly_idx];
            if (cM3d_CrossY_Tri(pm_vtx_tbl[tri->m_vtx_idx0], pm_vtx_tbl[tri->m_vtx_idx1],
                                pm_vtx_tbl[tri->m_vtx_idx2], pm_tri[i_poly_idx].m_plane,
                                (const Vec*)&i_splchk->GetPosP()))
            {
                if (!ChkPolyThrough(i_poly_idx, i_splchk->GetPolyPassChk())) {
                    i_splchk->SetHeight(crossY);
                    i_splchk->SetPolyIndex(i_poly_idx);
                    chk = true;
                }
            }
        }

        if (pm_rwg[i_poly_idx].m_next == 0xFFFF)
            break;
        i_poly_idx = pm_rwg[i_poly_idx].m_next;
    }

    return chk;
}

bool dBgW::SplGrpChkRp(dBgS_SplGrpChk* i_splchk, int i_idx) {
    cBgW_NodeTree* node = &pm_node_tree[i_idx];
    if (!node->CrossY(&i_splchk->GetPosP()) || !node->UnderPlaneYUnder(i_splchk->GetRoof()) ||
        node->TopPlaneYUnder(i_splchk->GetHeight()))
    {
        return false;
    }

    cBgD_Tree_t* tree = &pm_bgd->m_tree_tbl[i_idx];
    if ((tree->m_flag & 1)) {
        if (pm_blk[tree->m_id[0]].m_gnd_idx != 0xFFFF &&
            RwgSplGrpChk(pm_blk[tree->m_id[0]].m_gnd_idx, i_splchk))
        {
            return true;
        }
        return false;
    }

    bool chk = false;
    for (int i = 0; i < 8; i++) {
        if (tree->m_id[i] != 0xFFFF && SplGrpChkRp(i_splchk, tree->m_id[i])) {
            chk = true;
        }
    }

    return chk;
}

bool dBgW::SplGrpChkGrpRp(dBgS_SplGrpChk* i_splchk, int i_idx, int i_depth) {
    cBgW_GrpElm* grp = &pm_grp[i_idx];
    if (!grp->m_aab.CrossY(&i_splchk->GetPosP()) ||
        !grp->m_aab.UnderPlaneYUnder(i_splchk->GetRoof()) ||
        grp->m_aab.TopPlaneYUnder(i_splchk->GetHeight()))
    {
        return false;
    }

    if (ChkGrpThrough(i_idx, i_splchk->GetGrpPassChk(), i_depth)) {
        return false;
    }

    bool chk = false;
    cBgD_Grp_t* data = &pm_bgd->m_g_tbl[i_idx];
    if (data->m_tree_idx != 0xFFFF && SplGrpChkRp(i_splchk, data->m_tree_idx)) {
        chk = true;
    }

    int child_idx = data->m_first_child;
    while (true) {
        if (child_idx == 0xFFFF) {
            break;
        }
        if (SplGrpChkGrpRp(i_splchk, child_idx, i_depth + 1)) {
            chk = true;
        }
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return chk;
}

bool dBgW::SplGrpChk(dBgS_SplGrpChk* pchk) {
    return SplGrpChkGrpRp(pchk, m_rootGrpIdx, 1);
}

void dBgW::RwgCaptPoly(int i_idx, dBgS_CaptPoly& i_captpoly) {
    while (true) {
        cBgW_RwgElm* rwg = &pm_rwg[i_idx];
        i_captpoly.m_callback(i_captpoly, pm_vtx_tbl, pm_bgd->m_t_tbl[i_idx].m_vtx_idx0,
                              pm_bgd->m_t_tbl[i_idx].m_vtx_idx1, pm_bgd->m_t_tbl[i_idx].m_vtx_idx2,
                              &pm_tri[i_idx].m_plane);

        if (rwg->m_next == 0xFFFF)
            break;
        i_idx = rwg->m_next;
    }
}

void dBgW::CaptPolyRp(dBgS_CaptPoly& i_captpoly, int i_idx) {
    if (!pm_node_tree[i_idx].Cross(i_captpoly.GetBndP())) {
        return;
    }

    cBgD_Tree_t* tree_data = &pm_bgd->m_tree_tbl[i_idx];
    if (tree_data->m_flag & 1) {
        if (pm_blk[tree_data->m_id[0]].m_wall_idx != 0xFFFF) {
            RwgCaptPoly(pm_blk[tree_data->m_id[0]].m_wall_idx, i_captpoly);
        }
        if (pm_blk[tree_data->m_id[0]].m_roof_idx != 0xFFFF) {
            RwgCaptPoly(pm_blk[tree_data->m_id[0]].m_roof_idx, i_captpoly);
        }
        if (pm_blk[tree_data->m_id[0]].m_gnd_idx != 0xFFFF) {
            RwgCaptPoly(pm_blk[tree_data->m_id[0]].m_gnd_idx, i_captpoly);
        }
        return;
    }

    for (int i = 0; i < 8; i++) {
        if (tree_data->m_id[i] == 0xFFFF) {
            continue;
        }
        CaptPolyRp(i_captpoly, tree_data->m_id[i]);
    }
}

void dBgW::CaptPolyGrpRp(dBgS_CaptPoly& i_captpoly, int i_idx, int i_depth) {
    cBgW_GrpElm* grp = &pm_grp[i_idx];
    if (!grp->m_aab.Cross(i_captpoly.GetBndP())) {
        return;
    }

    if (ChkGrpThrough(i_idx, i_captpoly.GetGrpPassChk(), i_depth)) {
        return;
    }

    cBgD_Grp_t* data = &pm_bgd->m_g_tbl[i_idx];
    if (pm_bgd->m_g_tbl[i_idx].m_tree_idx != 0xFFFF) {
        CaptPolyRp(i_captpoly, pm_bgd->m_g_tbl[i_idx].m_tree_idx);
    }

    int child_idx = pm_bgd->m_g_tbl[i_idx].m_first_child;
    while (true) {
        if (child_idx == 0xFFFF) {
            break;
        }
        CaptPolyGrpRp(i_captpoly, child_idx, i_depth + 1);
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }
}

void dBgW::CaptPoly(dBgS_CaptPoly& i_captpoly) {
    CaptPolyGrpRp(i_captpoly, m_rootGrpIdx, 1);
}

bool dBgW::RwgSphChk(u16 i_poly_idx, dBgS_SphChk* i_sphchk, void* i_data) {
    cM3dGTri tri;
    cBgW_RwgElm* rwg;
    cBgD_Tri_t* tri_t;

    bool chk = false;
    while (true) {
        rwg = &pm_rwg[i_poly_idx];
        if (!ChkPolyThrough(i_poly_idx, i_sphchk->GetPolyPassChk())) {
            tri_t = &pm_bgd->m_t_tbl[i_poly_idx];
            tri.setBg(&pm_vtx_tbl[tri_t->m_vtx_idx0], &pm_vtx_tbl[tri_t->m_vtx_idx1],
                      &pm_vtx_tbl[tri_t->m_vtx_idx2], &pm_tri[i_poly_idx].m_plane);

            if (i_sphchk->cross(&tri)) {
                i_sphchk->mCallback(i_sphchk, pm_vtx_tbl, tri_t->m_vtx_idx0, tri_t->m_vtx_idx1,
                                    tri_t->m_vtx_idx2, &pm_tri[i_poly_idx].m_plane, i_data);
                i_sphchk->SetPolyIndex(i_poly_idx);
                chk = true;
            }
        }

        if (rwg->m_next == 0xFFFF)
            break;
        i_poly_idx = rwg->m_next;
    }

    return chk;
}

bool dBgW::SphChkRp(dBgS_SphChk* i_sphchk, void* i_data, int i_idx) {
    if (!pm_node_tree[i_idx].Cross(i_sphchk)) {
        return false;
    }

    cBgD_Tree_t* tree_data = &pm_bgd->m_tree_tbl[i_idx];
    bool chk = false;
    if (tree_data->m_flag & 1) {
        if (pm_blk[tree_data->m_id[0]].m_gnd_idx != 0xFFFF &&
            RwgSphChk(pm_blk[tree_data->m_id[0]].m_gnd_idx, i_sphchk, i_data))
        {
            chk = true;
        }

        if (pm_blk[tree_data->m_id[0]].m_roof_idx != 0xFFFF &&
            RwgSphChk(pm_blk[tree_data->m_id[0]].m_roof_idx, i_sphchk, i_data))
        {
            chk = true;
        }

        if (pm_blk[tree_data->m_id[0]].m_wall_idx != 0xFFFF &&
            RwgSphChk(pm_blk[tree_data->m_id[0]].m_wall_idx, i_sphchk, i_data))
        {
            chk = true;
        }

        return chk;
    }

    for (int i = 0; i < 8; i++) {
        if (tree_data->m_id[i] == 0xFFFF) {
            continue;
        }
        if (SphChkRp(i_sphchk, i_data, tree_data->m_id[i])) {
            chk = true;
        }
    }
    return chk;
}

bool dBgW::SphChkGrpRp(dBgS_SphChk* i_sphchk, void* i_data, int i_idx, int i_depth) {
    if (!pm_grp[i_idx].m_aab.Cross(i_sphchk)) {
        return false;
    }

    if (ChkGrpThrough(i_idx, i_sphchk->GetGrpPassChk(), i_depth)) {
        return false;
    }

    bool chk = false;
    cBgD_Grp_t* data = &pm_bgd->m_g_tbl[i_idx];
    if (data->m_tree_idx != 0xFFFF && SphChkRp(i_sphchk, i_data, data->m_tree_idx)) {
        chk = true;
    }

    int child_idx = data->m_first_child;
    while (true) {
        if (child_idx == 0xFFFF) {
            break;
        }
        if (SphChkGrpRp(i_sphchk, i_data, child_idx, i_depth + 1)) {
            chk = true;
        }
        child_idx = pm_bgd->m_g_tbl[child_idx].m_next_sibling;
    }

    return chk;
}

bool dBgW::SphChk(dBgS_SphChk* pchk, void* i_data) {
    return SphChkGrpRp(pchk, i_data, m_rootGrpIdx, 1);
}

u8 dBgW::GetPolyGrpRoomInfId(cBgS_PolyInfo const& poly) {
    return GetGrpInf(poly) & 0xFF;
}

int dBgW::GetGrpSoundId(cBgS_PolyInfo const& poly) {
    return (GetGrpInf(poly) >> 11) & 0xFF;
}

void dBgW::CrrPos(cBgS_PolyInfo const& i_poly, void* i_actor_ptr, bool param_2, cXyz* i_pos,
                  csXyz* i_angle, csXyz* i_shapeAngle) {
    if (m_crr_func) {
        m_crr_func(this, i_actor_ptr, i_poly, param_2, i_pos, i_angle, i_shapeAngle);
    }
}

void dBgW::TransPos(cBgS_PolyInfo const& i_poly, void* i_actor_ptr, bool param_2, cXyz* i_pos,
                    csXyz* i_angle, csXyz* i_shapeAngle) {
    if (m_crr_func) {
        m_crr_func(this, i_actor_ptr, i_poly, param_2, i_pos, i_angle, i_shapeAngle);
    }
}

void dBgW::MatrixCrrPos(cBgS_PolyInfo const& i_poly, void* i_actor_ptr, bool param_2, cXyz* i_pos,
                        csXyz* i_angle, csXyz* i_shapeAngle) {
    CrrPos(i_poly, i_actor_ptr, param_2, i_pos, i_angle, i_shapeAngle);
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
            if (!dComIfGp_getHorseActor()->checkSpecialWallHit(pos)) {
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

bool dBgW::ChkShdwDrawThrough(int poly_index, cBgS_PolyPassChk*) {
    if (GetShdwThrough(poly_index)) {
        return true;
    }

    return GetPolyArrowThrough(poly_index);
}

bool dBgW::ChkGrpThrough(int i_grp_idx, cBgS_GrpPassChk* i_passChk, int i_depth) {
    if (i_depth != 2 || i_passChk == NULL) {
        return false;
    }
    if ((pm_bgd->m_g_tbl[i_grp_idx].m_info & 0x100) == 0 &&
        ((dBgS_GrpPassChk*)i_passChk)->MaskNormalGrp())
    {
        return false;
    }

    if ((pm_bgd->m_g_tbl[i_grp_idx].m_info & 0x100) &&
        ((dBgS_GrpPassChk*)i_passChk)->MaskWaterGrp())
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
    m_flags &= ~1;
}

u32 dBgW::ChkMoveFlag() const {
    return m_flags & 1;
}

dBgW* dBgW_NewSet(cBgD_t* pbgd, u32 flags, Mtx* pbase_mtx) {
    dBgW* nw = new dBgW();

    if (nw == NULL) {
        return NULL;
    }

    if (nw->Set(pbgd, flags, pbase_mtx)) {
        return NULL;
    }

    return nw;
}

/* 80450F88 0001+00 data_80450F88 None */
/* 80450F89 0001+00 data_80450F89 None */
/* 80450F8A 0001+00 data_80450F8A None */
/* 80450F8B 0005+00 data_80450F8B None */
extern u8 struct_80450F88[8];
u8 struct_80450F88[8];
