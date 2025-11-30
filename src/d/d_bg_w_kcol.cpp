#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bg_w_kcol.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_horse.h"
#include "d/d_bg_s_cap_poly.h"

#include <algorithm.h>
#include <cmath.h>

#include "SSystem/SComponent/c_bg_s_shdw_draw.h"
#include "SSystem/SComponent/c_m2d.h"

#define MAX_DRAW_BIT 0x200

dBgWKCol::dBgWKCol() {
    m_pkc_head = NULL;
}

dBgWKCol::~dBgWKCol() {}

void* dBgWKCol::initKCollision(void* i_kclData) {
    ((KC_Header*)i_kclData)->m_pos_data = (Vec*)((uintptr_t)((KC_Header*)i_kclData) + (uintptr_t)((KC_Header*)i_kclData)->m_pos_data);
    ((KC_Header*)i_kclData)->m_nrm_data = (Vec*)((uintptr_t)((KC_Header*)i_kclData) + (uintptr_t)((KC_Header*)i_kclData)->m_nrm_data);
    ((KC_Header*)i_kclData)->m_prism_data = (KC_PrismData*)((uintptr_t)((KC_Header*)i_kclData) + (uintptr_t)((KC_Header*)i_kclData)->m_prism_data);
    ((KC_Header*)i_kclData)->m_block_data = (KC_PrismData*)((uintptr_t)((KC_Header*)i_kclData) + (uintptr_t)((KC_Header*)i_kclData)->m_block_data);

    return i_kclData;
}

void dBgWKCol::create(void* pprism, void* plc) {
    JUT_ASSERT(0x43, pprism != NULL);
    JUT_ASSERT(0x44, plc != NULL);

    ClrDBgWBase();
    m_pkc_head = (KC_Header*)pprism;

    u32 poly_num = ((uintptr_t)m_pkc_head->m_block_data - (uintptr_t)m_pkc_head->m_prism_data) / 0xC;
    poly_num++;
    JUT_ASSERT(0x50, poly_num <= 0x4000);

    m_code.setBase(plc);
}

void dBgWKCol::getTriNrm(KC_PrismData* pkc, Vec** nrm) const {
    Vec* var_r31 = &m_pkc_head->m_nrm_data[pkc->fnrm_i];
    *nrm = var_r31;
}

bool dBgWKCol::ChkNotReady() const {
    return m_pkc_head == NULL;
}

bool dBgWKCol::ChkLock() const {
    return false;
}

bool dBgWKCol::ChkMoveBg() const {
    return false;
}

u32 dBgWKCol::ChkMoveFlag() const {
    return 0;
}

cM3dGPla dBgWKCol::GetTriPla(const cBgS_PolyInfo& poly) const {
    int poly_index = poly.GetPolyIndex();
    return GetTriPla(poly_index);
}

cM3dGPla dBgWKCol::GetTriPla(int poly_index) const {
    KC_PrismData* pd = getPrismData(poly_index);

    Vec* tri_nrm;
    getTriNrm(pd, &tri_nrm);

    Vec* tri_pos;
    getTri1Pos(pd, &tri_pos);
    f32 ndot = -PSVECDotProduct(tri_nrm, tri_pos);

    cM3dGPla plane;
    plane.SetupNP(*tri_nrm, *tri_pos);

    return plane;
}

bool dBgWKCol::GetTriPnt(const cBgS_PolyInfo& poly, cXyz* ppos, cXyz* param_2,
                         cXyz* param_3) const {
    int poly_index = poly.GetPolyIndex();
    return GetTriPnt(poly_index, ppos, param_2, param_3);
}

bool dBgWKCol::GetTriPnt(int poly_index, Vec* ppos, Vec* param_2, Vec* param_3) const {
    KC_PrismData* pd = getPrismData(poly_index);
    return GetTriPnt(pd, ppos, param_2, param_3);
}

bool dBgWKCol::GetTriPnt(KC_PrismData const* pd, Vec* ppos, Vec* param_3,
                         Vec* param_4) const {
    *ppos = m_pkc_head->m_pos_data[pd->pos_i];

    Vec* face_nrm = &m_pkc_head->m_nrm_data[pd->fnrm_i];
    Vec* edge_nrm1 = &m_pkc_head->m_nrm_data[pd->enrm1_i];
    Vec* edge_nrm2 = &m_pkc_head->m_nrm_data[pd->enrm2_i];
    Vec* edge_nrm3 = &m_pkc_head->m_nrm_data[pd->enrm3_i];

    Vec sp64;
    PSVECCrossProduct(face_nrm, edge_nrm1, &sp64);

    Vec sp70;
    PSVECCrossProduct(edge_nrm2, face_nrm, &sp70);

    f32 var_f30 = PSVECDotProduct(&sp64, edge_nrm3);
    if (cM3d_IsZero(var_f30)) {
        return false;
    }

    f32 var_f31 = pd->height / var_f30;
    PSVECScale(&sp64, &sp64, var_f31);
    PSVECAdd(&sp64, ppos, param_4);
    f32 var_f29 = PSVECDotProduct(&sp70, edge_nrm3);
    if (cM3d_IsZero(var_f29)) {
        return false;
    }

    var_f31 = pd->height / var_f29;
    PSVECScale(&sp70, &sp70, var_f31);
    PSVECAdd(&sp70, ppos, param_3);
    return true;
}

cM3dGAab* dBgWKCol::GetBnd() const {
    return (cM3dGAab*)&m_bnd;
}

u32 dBgWKCol::GetGrpInf(const cBgS_PolyInfo& poly) const {
    int poly_index = poly.GetPolyIndex();
    KC_PrismData* pd = getPrismData(poly_index);

    return m_code.getGrpCode(pd->attribute);
}

void dBgWKCol::OffMoveFlag() {}

void dBgWKCol::getPolyCode(int poly_index, dBgPc* ppoly) const {
    KC_PrismData* pd = getPrismData(poly_index);

    sBgPc* code;
    m_code.getCode(pd->attribute, &code);
    ppoly->setCode(*code);
}

bool dBgWKCol::chkPolyThrough(dBgPc* ppoly, cBgS_PolyPassChk* ppolypasschk,
                                  cBgS_GrpPassChk* pgrppasschk, cXyz& param_4) const {
    JUT_ASSERT(279, ppoly != NULL);

    if (pgrppasschk != NULL) {
        dBgS_GrpPassChk* var_r28 = (dBgS_GrpPassChk*)pgrppasschk;
        if (ppoly->maskNrm() == 0) {
            if (var_r28->MaskNormalGrp() == 0) {
                return true;
            }
        } else if (ppoly->getWtr() != 0 && var_r28->MaskWaterGrp() == 0) {
            return true;
        }
    }

    if (ppolypasschk != NULL) {
        dBgS_PolyPassChk* polypass = (dBgS_PolyPassChk*)ppolypasschk;
        if (polypass->ChkObj() && ppoly->getObjThrough()) {
            return true;
        }

        if (polypass->ChkCam() && ppoly->getCamThrough()) {
            return true;
        }

        if ((polypass->ChkLink() && ppoly->getLinkThrough()) ||
            (polypass->ChkHorse() && ppoly->getLinkThrough()))
        {
            return true;
        }

        if (polypass->ChkArrow() && ppoly->getArrowThrough()) {
            return true;
        }

        if (polypass->ChkBomb() && ppoly->getBombThrough()) {
            return true;
        }

        if (polypass->ChkBoomerang() && ppoly->getBoomerangThrough()) {
            return true;
        }

        if (polypass->ChkRope() && ppoly->getRopeThrough()) {
            return true;
        }

        if (polypass->ChkUnderwaterRoof() && ppoly->getUnderwaterRoof()) {
            return true;
        }

        if (ppoly->getWallCode() == 8 && polypass->ChkNoHorse()) {
            return true;
        }

        if (ppoly->getWallCode() == 9) {
            if (polypass->ChkNoHorse()) {
                return true;
            }

            if (polypass->ChkHorse() && dComIfGp_getHorseActor() != NULL) {
                daHorse_c* horse_p = dComIfGp_getHorseActor();
                if (!horse_p->checkSpecialWallHit(param_4)) {
                    return true;
                }
            }
        }

        if (polypass->ChkStatue() && ppoly->getSpl() == 7) {
            return true;
        }

        if (polypass->ChkIronBall() && ppoly->getIronBallThrough()) {
            return true;
        }
    }

    return false;
}

bool dBgWKCol::LineCheck(cBgS_LinChk* plinchk) {
    cXyz* pstart = plinchk->GetStartP();
    cXyz* pcross = plinchk->GetCrossP();
    f32 var_f31 = JMAFastSqrt(VECSquareDistance(pstart, pcross));
    f32 temp_f30;
    if (cM3d_IsZero(var_f31)) {
        return 0;
    }

    f32 var_f25 = 500.0f / var_f31;
    f32 var_f29 = 0.0f;

    cXyz sp144;
    PSVECSubtract(pcross, pstart, &sp144);
    PSVECScale(&sp144, &sp144, var_f25);

    cXyz sp138;
    cXyz sp12C;
    sp12C = *pstart;

    bool spB = false;

    do {
        sp138 = sp12C;
        if (var_f29 + 500.0f <= var_f31) {
            PSVECAdd(&sp12C, &sp144, &sp12C);
        } else {
            sp12C = *pcross;
        }

        cXyz sp120(sp12C);
        cXyz sp114;
        cXyz sp108;

        PSVECSubtract(&sp138, &m_pkc_head->m_area_min_pos, &sp114);
        sp108 = sp114;

        cXyz spFC;
        PSVECSubtract(&sp12C, &m_pkc_head->m_area_min_pos, &spFC);

        if (sp114.x > spFC.x) {
            sp114.x = spFC.x;
        } else {
            sp108.x = spFC.x;
        }

        if (sp114.y > spFC.y) {
            sp114.y = spFC.y;
        } else {
            sp108.y = spFC.y;
        }

        if (sp114.z > spFC.z) {
            sp114.z = spFC.z;
        } else {
            sp108.z = spFC.z;
        }

        sp114.x -= 1.0f;
        sp114.y -= 1.0f;
        sp114.z -= 1.0f;
        sp108.x += 1.0f;
        sp108.y += 1.0f;
        sp108.z += 1.0f;

        int sp90 = (u32)sp114.x;
        if (sp90 < 0) {
            sp90 = 0;
        }

        int sp8C;
        int sp88;
        int sp84 = (u32)sp108.x;
        if (sp84 > (int)~m_pkc_head->m_area_x_width_mask) {
            sp84 = (int)~m_pkc_head->m_area_x_width_mask;
        }

        if (sp90 < sp84) {
            sp8C = (u32)sp114.y;
            if (sp8C < 0) {
                sp8C = 0;
            }

            int sp80 = (u32)sp108.y;
            if (sp80 > (int)~m_pkc_head->m_area_y_width_mask) {
                sp80 = (int)~m_pkc_head->m_area_y_width_mask;
            }

            if (sp8C < sp80) {
                sp88 = (u32)sp114.z;
                if (sp88 < 0) {
                    sp88 = 0;
                }

                int sp7C = (u32)sp108.z;
                if (sp7C > (int)~m_pkc_head->m_area_z_width_mask) {
                    sp7C = (int)~m_pkc_head->m_area_z_width_mask;
                }

                if (sp88 < sp7C) {
                    cXyz spF0;
                    int sp78;
                    int sp74;
                    int sp70;
                    int sp6C;
                    int sp68;
                    int sp64;
                    int sp60;
                    int sp5C;
                    u16* sp58 = NULL;
                    u16* sp54 = NULL;
                    u16* sp50 = NULL;
                    u16* sp4C = NULL;
                    u16* sp48 = NULL;
                    u16* sp44 = NULL;
                    int z_sp40 = sp88;

                    do {
                        sp68 = 1000000;
                        int y_sp3C = sp8C;
                        do {
                            sp6C = 1000000;
                            sp64 = 0;
                            sp60 = 0;
                            sp5C = 0;
                            int x_sp38 = sp90;

                            do {
                                u16* block = (u16*)m_pkc_head->m_block_data;
                                u32 shift = m_pkc_head->m_block_width_shift;
                                int offset = (((u32)z_sp40 >> shift) << m_pkc_head->m_area_xy_blocks_shift |
                                              ((u32)y_sp3C >> shift) << m_pkc_head->m_area_x_blocks_shift |
                                               (u32)x_sp38 >> shift) << 2;

                                while ((offset = *(int*)((int)block + offset)) >= 0) {
                                    block = (u16*)((int)block + offset);
                                    shift--;
                                    offset = (((u32)z_sp40 >> shift & 1) << 2 |
                                              ((u32)y_sp3C >> shift & 1) << 1 |
                                              ((u32)x_sp38 >> shift & 1) << 0) << 2;
                                }

                                u16* sp28 = (u16*)((int)block + (offset & 0x7FFFFFFF));

                                shift = 1 << shift;
                                int cellSize = shift - 1;
                                sp78 = shift - (x_sp38 & cellSize);
                                sp74 = shift - (y_sp3C & cellSize);
                                sp70 = shift - (z_sp40 & cellSize);

                                if (sp70 < sp68) {
                                    sp68 = sp70;
                                }

                                if (sp74 < sp6C) {
                                    sp6C = sp74;
                                }

                                if (sp28[1] != 0 && sp74 > sp5C) {
                                    if (sp74 > sp60) {
                                        if (sp74 > sp64) {
                                            sp5C = sp60;
                                            sp60 = sp64;
                                            sp64 = sp74;
                                            sp50 = sp54;
                                            sp54 = sp58;
                                            sp58 = sp28;
                                        } else {
                                            sp5C = sp60;
                                            sp60 = sp74;
                                            sp50 = sp54;
                                            sp54 = sp28;
                                        }
                                    } else {
                                        sp5C = sp74;
                                        sp50 = sp28;
                                    }
                                }

                                if (sp28 == sp4C || sp28 == sp48 || sp28 == sp44) {
                                } else {
                                    while (*(++sp28) != 0) {
                                        KC_PrismData* sp20 = getPrismData(sp28[0]);
                                        Vec* sp1C = &m_pkc_head->m_nrm_data[sp20->fnrm_i];
                                        Vec* sp18 = &m_pkc_head->m_pos_data[sp20->pos_i];

                                        cXyz spE4;
                                        PSVECSubtract(&sp138, sp18, &spE4);
                                        temp_f30 = PSVECDotProduct(sp1C, &spE4);

                                        cXyz spD8;
                                        PSVECSubtract(&sp120, sp18, &spD8);
                                        f32 temp_f28 = PSVECDotProduct(sp1C, &spD8);

                                        if (temp_f30 >= 0.0f && temp_f28 <= 0.0f) {
                                            if (!plinchk->ChkFrontFlag()) {
                                                continue;
                                            }
                                        } else if (!(temp_f30 <= 0.0f && temp_f28 >= 0.0f && plinchk->ChkBackFlag())) {
                                            continue;
                                        }

                                        f32 temp_f27 = temp_f30 - temp_f28;
                                        if (!cM3d_IsZero(fabsf(temp_f27))) {
                                            f32 temp_f24 = temp_f30 / temp_f27;
                                            Vec spCC, spC0;
                                            PSVECSubtract(&spD8, &spE4, &spCC);
                                            PSVECScale(&spCC, &spC0, temp_f24);

                                            cXyz spB4;
                                            PSVECAdd(&spE4, &spC0, &spB4);

                                            Vec* sp14 = &m_pkc_head->m_nrm_data[sp20->enrm1_i];

                                            if (PSVECDotProduct(&spB4, sp14) <= 0.0075f) {
                                                Vec* sp10 = &m_pkc_head->m_nrm_data[sp20->enrm2_i];
                                                if (PSVECDotProduct(&spB4, sp10) <= 0.0075f) {
                                                    Vec* spC =
                                                        &m_pkc_head->m_nrm_data[sp20->enrm3_i];
                                                    f32 var_f26 = PSVECDotProduct(&spB4, spC);
                                                    if (var_f26 >= -0.0075f &&
                                                        var_f26 <= sp20->height + 0.0075f)
                                                    {
                                                        if (cBgW_CheckBGround(sp1C->y)) {
                                                            if (!plinchk->GetPreGroundChk()) {
                                                                continue;
                                                            }
                                                        } else if (cBgW_CheckBRoof(sp1C->y)) {
                                                            if (!plinchk->GetPreRoofChk()) {
                                                                continue;
                                                            }
                                                        } else if (!plinchk->GetPreWallChk()) {
                                                            continue;
                                                        }

                                                        dBgPc sp150;
                                                        getPolyCode(sp28[0], &sp150);

                                                        cXyz spA8(*sp1C);
                                                        if (!chkPolyThrough(
                                                                &sp150,
                                                                plinchk->GetPolyPassChk(),
                                                                plinchk->GetGrpPassChk(), spA8))
                                                        {
                                                            cXyz sp9C;
                                                            PSVECAdd(&spB4, sp18, &sp9C);
                                                            spB = 1;
                                                            *pcross = sp9C;
                                                            sp120 = sp9C;

                                                            JUT_ASSERT(738, !isnan(pcross->x));
                                                            JUT_ASSERT(739, !isnan(pcross->y));
                                                            JUT_ASSERT(740, !isnan(pcross->z));
                                                            JUT_ASSERT(745,
                                                                -FP_INFINITE < pcross->x &&
                                                                    pcross->x < FP_INFINITE);
                                                            JUT_ASSERT(747,
                                                                -FP_INFINITE < pcross->y &&
                                                                    pcross->y < FP_INFINITE);
                                                            JUT_ASSERT(749,
                                                                -FP_INFINITE < pcross->z &&
                                                                    pcross->z < FP_INFINITE);

                                                            plinchk->SetPolyIndex(sp28[0]);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            } while ((u32)(x_sp38 += sp78) <= sp84);

                            sp4C = sp58;
                            sp48 = sp54;
                            sp44 = sp50;
                        } while ((u32)(y_sp3C += sp6C) <= sp80);
                    } while ((u32)(z_sp40 += sp68) <= sp7C);

                    if (spB)
                        break;
                }
            }
        }

        var_f29 += 500.0f;
    } while (var_f29 <= var_f31);

    return spB;
}

bool dBgWKCol::GroundCross(cBgS_GndChk* i_chk) {
    cXyz* point_p = (cXyz*)&i_chk->GetPointP();
    cXyz sp58;


    PSVECSubtract(point_p, &m_pkc_head->m_area_min_pos, &sp58);

    int sp38 = (u32)sp58.x;
    if (sp38 < 0) {
        return 0;
    } else if (sp38 > (int)~m_pkc_head->m_area_x_width_mask) {
        return 0;
    }

    int sp34 = (u32)sp58.z;
    if (sp34 < 0) {
        return 0;
    } else if (sp34 > (int)~m_pkc_head->m_area_z_width_mask) {
        return 0;
    }

    int sp30 = (u32)sp58.y;
    if (sp30 < 0) {
        return 0;
    } else if (sp30 > (int)~m_pkc_head->m_area_y_width_mask) {
        sp30 = (int)~m_pkc_head->m_area_y_width_mask;
    }

    bool var_r24 = 0;

    cXyz sp4C;
    int sp2C = 0;
    do {
        uintptr_t block = (uintptr_t)m_pkc_head->m_block_data;
        u32 shift = m_pkc_head->m_block_width_shift;
        int sp20 = 4 * (((u32)sp34 >> shift) << m_pkc_head->m_area_xy_blocks_shift |
                        ((u32)sp30 >> shift) << m_pkc_head->m_area_x_blocks_shift |
                         (u32)sp38 >> shift);
        while ((sp20 = (*(int*)((int)block + sp20))) >= 0) {
            block = (int)block + sp20;
            shift--;
            sp20 = (((u32)sp34 >> shift & 1) << 2 |
                    ((u32)sp30 >> shift & 1) << 1 |
                    ((u32)sp38 >> shift & 1) << 0) << 2;
        }

        u16* sp1C = (u16*)(block + (sp20 & 0x7FFFFFFF));

        while (*++sp1C != 0) {
            KC_PrismData* sp18 = getPrismData(sp1C[0]);
            Vec* sp14 = &m_pkc_head->m_nrm_data[sp18->fnrm_i];

            if (!(sp14->y < 0.014f) && !cM3d_IsZero(sp14->y) && (!cBgW_CheckBWall(sp14->y) || i_chk->GetWallPrecheck())) {
                Vec* sp10 = &m_pkc_head->m_pos_data[sp18->pos_i];
                sp4C.x = point_p->x - sp10->x;
                sp4C.z = point_p->z - sp10->z;
                sp4C.y = -(sp4C.x * sp14->x + sp4C.z * sp14->z) / sp14->y;

                if (!(PSVECDotProduct(&sp4C, &m_pkc_head->m_nrm_data[sp18->enrm1_i]) > 0.0075f) &&
                    !(PSVECDotProduct(&sp4C, &m_pkc_head->m_nrm_data[sp18->enrm2_i]) > 0.0075f))
                {
                    f32 var_f30 = PSVECDotProduct(&sp4C, &m_pkc_head->m_nrm_data[sp18->enrm3_i]);
                    if (!(var_f30 > 0.0075f + sp18->height) && !(var_f30 < -0.0075f)) {
                        dBgPc sp64;
                        getPolyCode(sp1C[0], &sp64);
                        cXyz sp40(*sp14);

                        if (!chkPolyThrough(&sp64, i_chk->GetPolyPassChk(), i_chk->GetGrpPassChk(), sp40)) {
                            f32 tmp_height_kcw = sp4C.y + sp10->y;
                            if (i_chk->GetNowY() < tmp_height_kcw && point_p->y > tmp_height_kcw) {
                                i_chk->SetPolyIndex(sp1C[0]);
                                i_chk->SetNowY(tmp_height_kcw);
                                var_r24 = 1;
                                JUT_ASSERT(934, !isnan(tmp_height_kcw));
                                JUT_ASSERT(937, -INF < tmp_height_kcw && tmp_height_kcw < INF);
                                sp2C = (u32)(tmp_height_kcw - m_pkc_head->m_area_min_pos.y);
                            }
                        }
                    }
                }
            }
        }

        shift = 1 << shift;
        u32 sp0C = shift - 1;
        sp30 &= ~sp0C;
        sp30--;
    } while (sp30 >= sp2C);

    return var_r24;
}

// NONMATCHING - stack
void dBgWKCol::ShdwDraw(cBgS_ShdwDraw* param_0) {
    dBgPc polyCode_sp108;

    cM3dGAab* bnd_spAC = param_0->GetBndP();
    cXyz* minP_spA8 = bnd_spAC->GetMinP();
    cXyz* maxP_spA4 = bnd_spAC->GetMaxP();

    Vec offset1_sp134;
    Vec offset2_sp128;
    Vec prismPos_sp11C;

    cXyz localMin_spD4;
    cXyz localMax_spC8;
    PSVECSubtract(minP_spA8, &m_pkc_head->m_area_min_pos, &localMin_spD4);
    PSVECSubtract(maxP_spA4, &m_pkc_head->m_area_min_pos, &localMax_spC8);

    localMin_spD4.x -= 1.0f;
    localMin_spD4.y -= 1.0f;
    localMin_spD4.z -= 1.0f;
    localMax_spC8.x += 1.0f;
    localMax_spC8.y += 1.0f;
    localMax_spC8.z += 1.0f;

    int minX_spA0 = (u32)localMin_spD4.x;
    if (minX_spA0 < 0) {
        minX_spA0 = 0;
    }
    int maxX_sp9C = (u32)localMax_spC8.x;
    if (maxX_sp9C > (int)~m_pkc_head->m_area_x_width_mask) {
        maxX_sp9C = ~m_pkc_head->m_area_x_width_mask;
    }

    if (minX_spA0 < maxX_sp9C) {
        int minY_sp98 = (u32)localMin_spD4.y;
        if (minY_sp98 < 0) {
            minY_sp98 = 0;
        }
        int maxY_sp94 = (u32)localMax_spC8.y;
        if (maxY_sp94 > (int)~m_pkc_head->m_area_y_width_mask) {
            maxY_sp94 = ~m_pkc_head->m_area_y_width_mask;
        }

        if (minY_sp98 < maxY_sp94) {
            int minZ_sp90 = (u32)localMin_spD4.z;
            if (minZ_sp90 < 0) {
                minZ_sp90 = 0;
            }
            int maxZ_sp8C = (u32)localMax_spC8.z;
            if (maxZ_sp8C > (int)~m_pkc_head->m_area_z_width_mask) {
                maxZ_sp8C = ~m_pkc_head->m_area_z_width_mask;
            }

            if (minZ_sp90 < maxZ_sp8C) {
                u32 drawBits_sp140[512];
                for (u32* el_sp88 = drawBits_sp140; el_sp88 < drawBits_sp140 + 512; el_sp88++) {
                    *el_sp88 = 0;
                }

                int remX_sp84;
                int remY_sp80;
                int remZ_sp7C;

                int stepY_sp78;
                int stepZ_sp74;

                int best1_sp70;
                int best2_sp6C;
                int best3_sp68;

                u16* topPrism1_sp64 = NULL;
                u16* topPrism2_sp60 = NULL;
                u16* topPrism3_sp5C = NULL;

                u16* prev1_sp58 = NULL;
                u16* prev2_sp54 = NULL;
                u16* prev3_sp50;

                prev3_sp50 = NULL;
                int z_sp4C = minZ_sp90;
                do {
                    stepZ_sp74 = 1000000;
                    int y_sp48 = minY_sp98;

                    do {
                        stepY_sp78 = 1000000;
                        best1_sp70 = NULL;
                        best2_sp6C = NULL;
                        best3_sp68 = NULL;

                        int x_sp44 = minX_spA0;
                        do {
                            u32 block_sp40 = (u32)m_pkc_head->m_block_data;
                            u32 shift_sp3C = m_pkc_head->m_block_width_shift;
                            int offset_sp38 =
                                4 * (((u32)z_sp4C >> shift_sp3C) << m_pkc_head->m_area_xy_blocks_shift |
                                     ((u32)y_sp48 >> shift_sp3C) << m_pkc_head->m_area_x_blocks_shift |
                                      (u32)x_sp44 >> shift_sp3C);

                            while ((offset_sp38 = *(u32*)((int)block_sp40 + offset_sp38)) >= 0) {
                                block_sp40 = (int)block_sp40 + offset_sp38;
                                shift_sp3C--;
                                offset_sp38 = (((u32)z_sp4C >> shift_sp3C & 1) << 2 |
                                               ((u32)y_sp48 >> shift_sp3C & 1) << 1 |
                                               ((u32)x_sp44 >> shift_sp3C & 1) << 0) << 2;
                            }

                            u16* prism_sp34 = (u16*)(block_sp40 + (offset_sp38 & 0x7fffffff));

                            shift_sp3C = 1 << shift_sp3C;
                            u32 mask_sp30 = shift_sp3C - 1;
                            remX_sp84 = shift_sp3C - (x_sp44 & mask_sp30);
                            remY_sp80 = shift_sp3C - (y_sp48 & mask_sp30);
                            remZ_sp7C = shift_sp3C - (z_sp4C & mask_sp30);

                            if (remZ_sp7C < stepZ_sp74) {
                                stepZ_sp74 = remZ_sp7C;
                            }
                            if (remY_sp80 < stepY_sp78) {
                                stepY_sp78 = remY_sp80;
                            }

                            if (prism_sp34[1] != 0 && remY_sp80 > best3_sp68) {
                                if (remY_sp80 > best2_sp6C) {
                                    if (remY_sp80 > best1_sp70) {
                                        best3_sp68 = best2_sp6C;
                                        best2_sp6C = best1_sp70;
                                        best1_sp70 = remY_sp80;
                                        topPrism3_sp5C = topPrism2_sp60;
                                        topPrism2_sp60 = topPrism1_sp64;
                                        topPrism1_sp64 = prism_sp34;
                                    } else {
                                        best3_sp68 = best2_sp6C;
                                        best2_sp6C = remY_sp80;
                                        topPrism3_sp5C = topPrism2_sp60;
                                        topPrism2_sp60 = prism_sp34;
                                    }
                                } else {
                                    best3_sp68 = remY_sp80;
                                    topPrism3_sp5C = prism_sp34;
                                }
                            }

                            if (prism_sp34 == prev1_sp58 || prism_sp34 == prev2_sp54 || prism_sp34 == prev3_sp50) {
                            } else {
                                while (*++prism_sp34 != 0) {
#if PLATFORM_GCN
                                    u32 bitMask_sp28 = 1 << (prism_sp34[0] & 0x1f);
                                    s32 sp2c = *(prism_sp34) >> 5;
#else
                                    s32 sp2c = *(prism_sp34) >> 5;
                                    u32 bitMask_sp28 = 1 << (prism_sp34[0] & 0x1f);
#endif
                                    void* unk_sp24;

                                    KC_PrismData* prismData_sp20;
                                    Vec* nrm1_sp1C;
                                    Vec* nrm2_sp18;
                                    Vec* unk_sp14;
                                    Vec* temp_sp10;

                                    u32 temp_sp0C;
                                    bool temp2_sp08 = true;

                                    if (sp2c > (int)MAX_DRAW_BIT) {
                                        OS_PANIC(0x47c,
                                                 "Failed assertion shift <= MAX_DRAW_BIT");

                                        temp_sp0C = 0;
                                        if (temp_sp0C == 0) {
                                            temp2_sp08 = false;
                                        }
                                    }

                                    unk_sp24 = drawBits_sp140 + sp2c;
                                    if ((*(u32*)((uintptr_t)unk_sp24 + 0) & bitMask_sp28) == 0) {
                                        *(u32*)((uintptr_t)unk_sp24 + 0) |= bitMask_sp28;

                                        getPolyCode(prism_sp34[0], &polyCode_sp108);

                                        Vec cross1_spBC;
                                        Vec cross2_spB0;

                                        if (!ChkShdwDrawThrough(&polyCode_sp108)) {
                                            prismData_sp20 = getPrismData(prism_sp34[0]);

                                            prismPos_sp11C =
                                                m_pkc_head->m_pos_data[prismData_sp20->pos_i];

                                            nrm1_sp1C =
                                                m_pkc_head->m_nrm_data + prismData_sp20->fnrm_i;
                                            nrm2_sp18 = m_pkc_head->m_nrm_data +
                                                        prismData_sp20->enrm1_i;

                                            unk_sp14 = m_pkc_head->m_nrm_data +
                                                       prismData_sp20->enrm3_i;
                                            PSVECCrossProduct(nrm1_sp1C, nrm2_sp18,
                                                              &cross1_spBC);
                                            f32 dot = PSVECDotProduct(&cross1_spBC, unk_sp14);
                                            if (!cM3d_IsZero(dot)) {
                                                PSVECScale(&cross1_spBC, &cross1_spBC,
                                                           prismData_sp20->height / dot);
                                                PSVECAdd(&cross1_spBC, &prismPos_sp11C,
                                                         &offset1_sp134);

                                                // Second edge direction
                                                temp_sp10 = m_pkc_head->m_nrm_data +
                                                            prismData_sp20->enrm2_i;
                                                PSVECCrossProduct(temp_sp10, nrm1_sp1C,
                                                                  &cross2_spB0);
                                                f32 dot2 =
                                                    PSVECDotProduct(&cross2_spB0, unk_sp14);
                                                if (!cM3d_IsZero(dot2)) {
                                                    PSVECScale(&cross2_spB0, &cross2_spB0,
                                                               prismData_sp20->height / dot2);
                                                    PSVECAdd(&cross2_spB0, &prismPos_sp11C,
                                                             &offset2_sp128);

                                                    cM3dGPla pla_spF4;
                                                    pla_spF4 = GetTriPla(prism_sp34[0]);
                                                    (param_0->mCallbackFun)(
                                                        param_0, (cBgD_Vtx_t*)&prismPos_sp11C,
                                                        0, 1, 2, &pla_spF4);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        } while ((x_sp44 += remX_sp84) <= (u32)maxX_sp9C);

                        prev1_sp58 = topPrism1_sp64;
                        prev2_sp54 = topPrism2_sp60;
                        prev3_sp50 = topPrism3_sp5C;
                    } while ((y_sp48 += stepY_sp78) <= (u32)maxY_sp94);
                } while ((z_sp4C += stepZ_sp74) <= (u32)maxZ_sp8C);
            }
        }
    }
}


bool dBgWKCol::ChkShdwDrawThrough(dBgPc* pcode) {
    if (pcode->getShdwThrough()) {
        return true;
    } else if (pcode->getArrowThrough()) {
        return true;
    }
    return false;
}

void dBgWKCol::CaptPoly(dBgS_CaptPoly& i_captpoly) {
    const cM3dGAab* pbounds = i_captpoly.GetBndP();
    cXyz min(*pbounds->GetMinP());
    cXyz max(*pbounds->GetMaxP());

    min.x -= 1.0f;
    min.y -= 1.0f;
    min.z -= 1.0f;
    max.x += 1.0f;
    max.y += 1.0f;
    max.z += 1.0f;

    Vec* area_min = &m_pkc_head->m_area_min_pos;

    int sp5C = (u32)(min.x - area_min->x);
    if (sp5C < 0) {
        sp5C = 0;
    }

    int sp58 = (u32)(max.x - area_min->x);
    if (sp58 > (int)~m_pkc_head->m_area_x_width_mask) {
        sp58 = (int)~m_pkc_head->m_area_x_width_mask;
    }

    if (sp5C < sp58) {
        int sp54 = (u32)(min.y - area_min->y);
        if (sp54 < 0) {
            sp54 = 0;
        }

        int sp50 = (u32)(max.y - area_min->y);
        if (sp50 > (int)~m_pkc_head->m_area_y_width_mask) {
            sp50 = (int)~m_pkc_head->m_area_y_width_mask;
        }

        if (sp54 < sp50) {
            int sp4C = (u32)(min.z - area_min->z);
            if (sp4C < 0) {
                sp4C = 0;
            }

            int sp48 = (u32)(max.z - area_min->z);
            if (sp48 > (int)~m_pkc_head->m_area_z_width_mask) {
                sp48 = (int)~m_pkc_head->m_area_z_width_mask;
            }

            int sp44;
            int sp40;
            int sp3C;
            int sp38;
            int sp34;
            int sp30;
            if (sp4C < sp48) {
                u16* sp2C = NULL;
                u16* sp28 = NULL;
                int sp24 = sp4C;

                do {
                    sp34 = 1000000;
                    int sp20 = sp54;

                    do {
                        sp38 = 1000000;
                        sp30 = 0;
                        int sp1C = sp5C;

                        do {
                            u16* sp18 = (u16*)m_pkc_head->m_block_data;
                            int r29 = m_pkc_head->m_block_width_shift;
                            int sp14 =
                                4 * (((u32)sp24 >> r29) << m_pkc_head->m_area_xy_blocks_shift |
                                     ((u32)sp20 >> r29) << m_pkc_head->m_area_x_blocks_shift |
                                      (u32)sp1C >> r29);
                            while ((sp14 = (*(int*)((int)sp18 + sp14))) >= 0) {
                                sp18 = (u16*)((int)sp18 + sp14);
                                r29--;
                                sp14 = (((u32)sp24 >> r29 & 1) << 2 |
                                        ((u32)sp20 >> r29 & 1) << 1 |
                                        ((u32)sp1C >> r29 & 1) << 0) << 2;
                            }

                            u16* r28 = (u16*)((int)sp18 + (sp14 & 0x7FFFFFFF));

                            r29 = 1 << r29;
                            int sp10 = r29 - 1;
                            sp44 = r29 - (sp1C & sp10);
                            sp40 = r29 - (sp20 & sp10);
                            sp3C = r29 - (sp24 & sp10);

                            if (sp3C < sp34) {
                                sp34 = sp3C;
                            }

                            if (sp40 < sp38) {
                                sp38 = sp40;
                            }

                            if (sp40 > sp30 && r28[1] != 0) {
                                sp30 = sp40;
                                sp2C = r28;
                                sp2C = r28;
                            }

                            if (r28 != sp28) {
                                while (*++r28 != 0) {
                                    KC_PrismData* spC = getPrismData(r28[0]);
                                    Vec* sp8 = &m_pkc_head->m_nrm_data[spC->fnrm_i];

                                    dBgPc spD8;
                                    getPolyCode(r28[0], &spD8);

                                    cXyz sp8C(*sp8);
                                    if (!chkPolyThrough(&spD8, i_captpoly.GetPolyPassChk(), i_captpoly.GetGrpPassChk(), sp8C)) {
                                        cXyz sp80;
                                        cXyz sp74;
                                        cXyz sp68;
                                        if (GetTriPnt(r28[0], &sp80, &sp74, &sp68)) {
                                            static Vec vtx_tbl[3];
                                            vtx_tbl[0] = sp80;
                                            vtx_tbl[1] = sp74;
                                            vtx_tbl[2] = sp68;

                                            cM3dGPla plane;
                                            plane = GetTriPla(r28[0]);

                                            i_captpoly.m_callback(i_captpoly, (cBgD_Vtx_t*)&vtx_tbl, 0, 1, 2, &plane);
                                        }
                                    }
                                }
                            }

                        } while ((u32)(sp1C += sp44) <= sp58);

                        sp28 = sp2C;
                    } while ((u32)(sp20 += sp38) <= sp50);

                } while ((u32)(sp24 += sp34) <= sp48);
            }
        }
    }
}

struct wcs_data {
    /* 0x0 */ f32 _0;
    /* 0x4 */ u16 _4;
    /* 0x8 */ wcs_data* next;
};

static wcs_data l_wcsbuf[84];

#ifdef DEBUG
static u8 lit_5300[8] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
#endif

// NONMATCHING - regalloc
bool dBgWKCol::WallCorrectSort(dBgS_Acch* pwi) {
    bool sp_10 = false;

    cM3dGCyl* sp_140 = pwi->GetWallBmdCylP();
    cXyz sp_1a4;
    cXyz sp_198;
    sp_140->calcMinMax(&sp_1a4, &sp_198);
    sp_1a4.x -= 1.0f;
    sp_1a4.y -= 1.0f;
    sp_1a4.z -= 1.0f;
    sp_198.x += 1.0f;
    sp_198.y += 1.0f;
    sp_198.z += 1.0f;
    Vec sp_18c;
    Vec* sp_13c = &m_pkc_head->m_area_min_pos;
    PSVECSubtract(&sp_1a4, sp_13c, &sp_1a4);
    PSVECSubtract(&sp_198, sp_13c, &sp_198);
    int sp_138 = (u32)sp_1a4.x;
    if (sp_138 < 0) {
        sp_138 = 0;
    }
    int sp_134 = (u32)sp_198.x;
    if (sp_134 > (int)~m_pkc_head->m_area_x_width_mask) {
        sp_134 = (int)~m_pkc_head->m_area_x_width_mask;
    }
    if (sp_138 >= sp_134) {
        return false;
    }

    int sp_130 = (u32)sp_1a4.y;
    if (sp_130 < 0) {
        sp_130 = 0;
    }
    int sp_12c = (u32)sp_198.y;
    if (sp_12c > (int)~m_pkc_head->m_area_y_width_mask) {
        sp_12c = (int)~m_pkc_head->m_area_y_width_mask;
    }
    if (sp_130 >= sp_12c) {
        return false;
    }

    int sp_128 = (u32)sp_1a4.z;
    if (sp_128 < 0) {
        sp_128 = 0;
    }
    int sp_124 = (u32)sp_198.z;
    if (sp_124 > ~m_pkc_head->m_area_z_width_mask) {
        sp_124 = ~m_pkc_head->m_area_z_width_mask;
    }

    if (sp_128 >= sp_124) {
        return false;
    }

    u16* sp_120 = NULL;
    u16* sp_11c = NULL;
    u16* sp_118 = NULL;
    u16* sp_114 = NULL;
    u16* sp_110 = NULL;
    u16* sp_10c = NULL;
    int sp_108;
    int sp_104;
    int sp_100;
    sp_18c.y = 0.0f;
    int sp_fc;
    int sp_f8;
    int sp_f4;
    int sp_f0;
    int sp_ec;
    wcs_data* sp_e8 = NULL;
    int wcsIndex_e4 = 0;
    int sp_e0 = sp_128;
    do {
        sp_f8 = 1000000;
        int sp_dc = sp_130;
        do {
            sp_fc = 1000000;
            sp_f4 = 0;
            sp_f0 = 0;
            sp_ec = 0;
            int sp_d8 = sp_138;
            do {
                u16* block_d4 = (u16*)m_pkc_head->m_block_data;
                int shift_d0 = m_pkc_head->m_block_width_shift;
                int sp_cc = 4 * (
                    ((u32)sp_e0 >> shift_d0) << m_pkc_head->m_area_xy_blocks_shift |
                    ((u32)sp_dc >> shift_d0) << m_pkc_head->m_area_x_blocks_shift |
                    ((u32)sp_d8 >> shift_d0));
                while ((sp_cc = *(int*)((int)block_d4 + sp_cc)) >= 0) {
                    block_d4 = (u16*)((int)block_d4 + sp_cc);
                    shift_d0--;
                    sp_cc = 4 * (
                        ((((u32)sp_e0 >> shift_d0) & 1) << 2) |
                        ((((u32)sp_dc >> shift_d0) & 1) << 1) |
                        ((((u32)sp_d8 >> shift_d0) & 1) << 0)
                    );
                }
                u16* sp_c8 = (u16*)((int)block_d4 + (sp_cc & 0x7fffffff));
                shift_d0 = 1 << shift_d0;
                int sp_c4 = shift_d0 - 1;
                sp_108 = shift_d0 - (sp_d8 & sp_c4);
                sp_104 = shift_d0 - (sp_dc & sp_c4);
                sp_100 = shift_d0 - (sp_e0 & sp_c4);
                if (sp_100 < sp_f8) {
                    sp_f8 = sp_100;
                }
                if (sp_104 < sp_fc) {
                    sp_fc = sp_104;
                }
                if (sp_c8[1] != 0 && (sp_104 > sp_ec)) {
                    if (sp_104 > sp_f0) {
                        if (sp_104 > sp_f4) {
                            sp_ec = sp_f0;
                            sp_f0 = sp_f4;
                            sp_f4 = sp_104;
                            sp_10c = sp_110;
                            sp_110 = sp_114;
                            sp_114 = sp_c8;
                        } else {
                            sp_ec = sp_f0;
                            sp_f0 = sp_104;
                            sp_10c = sp_110;
                            sp_110 = sp_c8;
                        }
                    } else {
                        sp_ec = sp_104;
                        sp_10c = sp_c8;
                    }
                }

                if (sp_c8 == sp_120 || sp_c8 == sp_11c || sp_c8 == sp_118) {
                } else {
                    while (*++sp_c8 != 0) {
                        KC_PrismData* sp_c0 = getPrismData(*sp_c8);
                        Vec* sp_bc = m_pkc_head->m_nrm_data + sp_c0->fnrm_i;
                        if (!cBgW_CheckBGround(sp_bc->y)) {
                            f32 sp_b8 = JMAFastSqrt(sp_bc->x * sp_bc->x + sp_bc->z * sp_bc->z);
                            if (!cM3d_IsZero(sp_b8)) {
                                dBgPc sp_1b0;
                                getPolyCode(*sp_c8, &sp_1b0);
                                cXyz sp_180 = *sp_bc;
                                if (!chkPolyThrough(&sp_1b0, pwi->GetPolyPassChk(),
                                                    pwi->GetGrpPassChk(), sp_180))
                                {
                                    fopAc_ac_c* sp_b4 = pwi->getMyAc();
                                    cXyz sp_174;
                                    sp_174.x = cM_ssin(sp_b4->current.angle.y);
                                    sp_174.z = cM_scos(sp_b4->current.angle.y);
                                    f32 sp_b0 = 1.0f / sp_b8;
                                    f32 sp_ac = sp_b0 * (sp_174.x * sp_bc->x + sp_174.z * sp_bc->z);
                                    if (sp_e8 == NULL) {
                                        if (wcsIndex_e4 < 84) {
                                            l_wcsbuf[wcsIndex_e4]._0 = sp_ac;
                                            l_wcsbuf[wcsIndex_e4]._4 = *sp_c8;
                                            l_wcsbuf[wcsIndex_e4].next = NULL;
                                            sp_e8 = &l_wcsbuf[wcsIndex_e4];
                                            wcsIndex_e4++;
                                        } else {
                                            #ifdef DEBUG
                                            lit_5300[0] = 1;
                                            #endif
                                        }
                                    } else if (*sp_c8 != sp_e8->_4) {
                                        if (sp_ac > sp_e8->_0) {
                                            if (wcsIndex_e4 < 84) {
                                                l_wcsbuf[wcsIndex_e4]._0 = sp_ac;
                                                l_wcsbuf[wcsIndex_e4]._4 = *sp_c8;
                                                l_wcsbuf[wcsIndex_e4].next = sp_e8;
                                                sp_e8 = &l_wcsbuf[wcsIndex_e4];
                                                // local_118->_0 = l_wcsbuf[local_11c]._0;
                                                wcsIndex_e4++;
                                            } else {
                                                #ifdef DEBUG
                                                lit_5300[0] = 1;
                                                #endif
                                            }
                                        } else {
                                            wcs_data* sp_a8 = sp_e8;
                                            while (true) {
                                                if (sp_a8->next == NULL) {
                                                    if (wcsIndex_e4 < 84) {
                                                        l_wcsbuf[wcsIndex_e4]._0 = sp_ac;
                                                        l_wcsbuf[wcsIndex_e4]._4 = *sp_c8;
                                                        l_wcsbuf[wcsIndex_e4].next = NULL;
                                                        sp_a8->next = &l_wcsbuf[wcsIndex_e4];
                                                        wcsIndex_e4++;
                                                    } else {
                                                        #ifdef DEBUG
                                                        lit_5300[0] = 1;
                                                        #endif
                                                    }

                                                    break;
                                                }

                                                if (*sp_c8 == sp_a8->next->_4) {
                                                    break;
                                                }

                                                if (sp_ac > sp_a8->next->_0) {
                                                    if (wcsIndex_e4 < 84) {
                                                        l_wcsbuf[wcsIndex_e4]._0 = sp_ac;
                                                        l_wcsbuf[wcsIndex_e4]._4 = *sp_c8;
                                                        l_wcsbuf[wcsIndex_e4].next = sp_a8->next;
                                                        sp_a8->next = &l_wcsbuf[wcsIndex_e4];
                                                        wcsIndex_e4++;
                                                    } else {
                                                        #ifdef DEBUG
                                                        lit_5300[0] = 1;
                                                        #endif
                                                    }

                                                    break;
                                                }
                                                sp_a8 = sp_a8->next;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } while ((u32)(sp_d8 += sp_108) <= sp_134);
            sp_120 = sp_114;
            sp_11c = sp_110;
            sp_118 = sp_10c;
        } while ((u32)(sp_dc += sp_fc) <= sp_12c);
    } while ((u32)(sp_e0 += sp_f8) <= sp_124);

    wcs_data* sp_a4 = sp_e8;
    while (true) {
        if (sp_a4 == NULL) {
            break;
        }
        u32 sp_a0 = (u16)sp_a4->_4;
        KC_PrismData* sp_9c = getPrismData(sp_a0);
        Vec* sp_98 = m_pkc_head->m_nrm_data + sp_9c->fnrm_i;
        f32 sp_94 = JMAFastSqrt(sp_98->x * sp_98->x + sp_98->z * sp_98->z);
        cXyz sp_168;
        cXyz sp_15c;
        cXyz sp_150;
        if (GetTriPnt(sp_9c, &sp_168, &sp_15c, &sp_150)) {
            f32 sp_90 = 1.0f / sp_94;
            for (s32 cir_index_8c = 0; cir_index_8c < pwi->GetTblSize(); cir_index_8c++) {
                f32 sp_88 = sp_90 * pwi->GetWallR(cir_index_8c);
                sp_18c.x = sp_98->x * sp_88;
                sp_18c.z = sp_98->z * sp_88;
                f32 sp_84;
                if (!pwi->ChkWallHDirect(cir_index_8c)) {
                    sp_84 = pwi->GetPos()->y + pwi->GetWallH(cir_index_8c) + pwi->GetWallAddY(sp_18c) - pwi->GetSpeedY();
                } else {
                    sp_84 = pwi->GetWallHDirect(cir_index_8c);
                }
                f32 sp_144[3];
                sp_144[0] = sp_168.y - sp_84;
                sp_144[1] = sp_15c.y - sp_84;
                sp_144[2] = sp_150.y - sp_84;
                if (sp_144[0] > 0.0f && sp_144[1] > 0.0f && sp_144[2] > 0.0f ||
                    sp_144[0] < 0.0f && sp_144[1] < 0.0f && sp_144[2] < 0.0f)
                {
                    continue;
                }

                int sp_80;
                int sp_7c;
                int sp_78;
                int sp_74 = 0;

                if (cM3d_IsZero(sp_144[0])) {
                    sp_74++;
                }
                if (cM3d_IsZero(sp_144[1])) {
                    sp_74++;
                }
                if (cM3d_IsZero(sp_144[2])) {
                    sp_74++;
                }
                if (sp_74 != 1) {
                    if (((sp_144[0] > 0.0f) && (sp_144[1] <= 0.0f) && (sp_144[2] <= 0.0f)) ||
                        (sp_144[0] < 0.0f && (sp_144[1] >= 0.0f) && (sp_144[2] >= 0.0f)))
                    {
                        sp_80 = 0;
                        sp_7c = 1;
                        sp_78 = 2;
                    } else if (((sp_144[1] > 0.0f) && (sp_144[0] <= 0.0f) &&
                                (sp_144[2] <= 0.0f)) ||
                               (sp_144[1] < 0.0f && (sp_144[0] >= 0.0f) &&
                                (sp_144[2] >= 0.0f)))
                    {
                        sp_80 = 1;
                        sp_7c = 0;
                        sp_78 = 2;
                    } else {
                        sp_80 = 2;
                        sp_7c = 0;
                        sp_78 = 1;
                    }
                    f32 sp_70 = sp_144[sp_80] - sp_144[sp_7c];
                    if (!cM3d_IsZero(sp_70)) {
                        f32 sp_6c = sp_144[sp_80] - sp_144[sp_78];
                        if (!cM3d_IsZero(sp_6c)) {
                            f32 sp_68 = -sp_144[sp_7c] / sp_70;
                            f32 sp_64 = -sp_144[sp_78] / sp_6c;
                            f32 cx0_60;
                            f32 cy0_5c;
                            f32 cx1_58;
                            f32 cy1_54;
                            if (sp_80 == 0) {
                                cx0_60 = sp_15c.x + sp_68 * (sp_168.x - sp_15c.x);
                                cy0_5c = sp_15c.z + sp_68 * (sp_168.z - sp_15c.z);
                                cx1_58 = sp_150.x + sp_64 * (sp_168.x - sp_150.x);
                                cy1_54 = sp_150.z + sp_64 * (sp_168.z - sp_150.z);
                            } else if (sp_80 == 1) {
                                cx0_60 = sp_168.x + sp_68 * (sp_15c.x - sp_168.x);
                                cy0_5c = sp_168.z + sp_68 * (sp_15c.z - sp_168.z);
                                cx1_58 = sp_150.x + sp_64 * (sp_15c.x - sp_150.x);
                                cy1_54 = sp_150.z + sp_64 * (sp_15c.z - sp_150.z);
                            } else {
                                cx0_60 = sp_168.x + sp_68 * (sp_150.x - sp_168.x);
                                cy0_5c = sp_168.z + sp_68 * (sp_150.z - sp_168.z);
                                cx1_58 = sp_15c.x + sp_64 * (sp_150.x - sp_15c.x);
                                cy1_54 = sp_15c.z + sp_64 * (sp_150.z - sp_15c.z);
                            }
                            cx0_60 += sp_18c.x;
                            cy0_5c += sp_18c.z;
                            cx1_58 += sp_18c.x;
                            cy1_54 += sp_18c.z;
                            f32 sp_50;
                            f32 sp_4c;
                            f32 sp_48;
                            bool sp_0f =
                                cM3d_Len2dSqPntAndSegLine(pwi->GetCx(), pwi->GetCz(), cx0_60, cy0_5c, cx1_58,
                                                          cy1_54, &sp_4c, &sp_48, &sp_50);
                            f32 sp_44 = sp_4c - pwi->GetCx();
                            f32 sp_40 = sp_48 - pwi->GetCz();
                            f32 sp_3c = pwi->GetWallRR(cir_index_8c);
                            if (sp_50 > sp_3c || sp_44 * sp_18c.x + sp_40 * sp_18c.z < 0.0f)
                            {
                                continue;
                            }

                            if (sp_0f == true) {
                                pwi->SetWallHit();
                                f32 sp_38 = sp_90 * std::sqrt(sp_50);
                                sp_38 -= 1.0f;
                                if (sp_38 < 0.0f) {
                                    sp_38 = 0.0f;
                                }

                                pwi->GetPos()->x += sp_38 * sp_98->x;
                                pwi->GetPos()->z += sp_38 * sp_98->z;

                                JUT_ASSERT(0x77e, !isnan(pwi->GetPos()->x));
                                JUT_ASSERT(0x77f, !isnan(pwi->GetPos()->z));
                                JUT_ASSERT(0x782, -INF < pwi->GetPos()->x &&
                                                      pwi->GetPos()->x < INF);
                                JUT_ASSERT(0x784, -INF < pwi->GetPos()->z &&
                                                      pwi->GetPos()->z < INF);

                                pwi->CalcMovePosWork();
                                pwi->SetWallCirHit(cir_index_8c);
                                pwi->SetWallPolyIndex(cir_index_8c, sp_a0);
                                s16 sp_16 = cM_atan2s(sp_98->x, sp_98->z);
                                pwi->SetWallAngleY(cir_index_8c, sp_16);
                                sp_10 = true;
                            } else {
                                cx0_60 -= sp_18c.x;
                                cy0_5c -= sp_18c.z;
                                cx1_58 -= sp_18c.x;
                                cy1_54 -= sp_18c.z;

                                JUT_ASSERT(0x797,
                                           pwi->GetPos()->x == pwi->GetWallCirP(cir_index_8c)->GetCx())
                                JUT_ASSERT(0x799, pwi->GetPos()->z ==
                                                      pwi->GetWallCirP(cir_index_8c)->GetCy());

                                f32 sp_34 =
                                    cM3d_Len2dSq(cx0_60, cy0_5c, pwi->GetPos()->x, pwi->GetPos()->z);
                                f32 sp_30 =
                                    cM3d_Len2dSq(cx1_58, cy1_54, pwi->GetPos()->x, pwi->GetPos()->z);
                                f32 onx_2c = -sp_98->x;
                                f32 ony_28 = -sp_98->z;

                                JUT_ASSERT(0x7a8, !(cM3d_IsZero(onx_2c) && cM3d_IsZero(ony_28)));

                                if (sp_34 < sp_30) {
                                    if (sp_34 > sp_3c) {
                                        continue;
                                    }

                                    if (fabsf(sp_34 - sp_3c) < 0.008f) {
                                        continue;
                                    }

                                    f32 sp_1c;
                                    f32 sp_18;
                                    JUT_ASSERT(0x7b3, !isnan(cx0_60));
                                    JUT_ASSERT(0x7b4, !isnan(cy0_5c));

                                    cM2d_CrossCirLin(*pwi->GetWallCirP(cir_index_8c), cx0_60,
                                                     cy0_5c, onx_2c, ony_28, &sp_1c, &sp_18);

                                    pwi->GetPos()->x += (cx0_60 - sp_1c);
                                    pwi->GetPos()->z += (cy0_5c - sp_18);

                                    JUT_ASSERT(0x7bf, !isnan(pwi->GetPos()->x));
                                    JUT_ASSERT(0x7c0, !isnan(pwi->GetPos()->z));
                                    JUT_ASSERT(0x7c3, -INF < pwi->GetPos()->x &&
                                                          pwi->GetPos()->x < INF)
                                    JUT_ASSERT(0x7c5, -INF < pwi->GetPos()->z &&
                                                          pwi->GetPos()->z < INF)

                                    pwi->CalcMovePosWork();
                                    pwi->SetWallCirHit(cir_index_8c);
                                    pwi->SetWallPolyIndex(cir_index_8c, sp_a0);
                                    s16 sp_14 = cM_atan2s(sp_98->x, sp_98->z);
                                    pwi->SetWallAngleY(cir_index_8c, sp_14);
                                    sp_10 = true;
                                    pwi->SetWallHit();
                                } else if (!(sp_30 > sp_3c || fabsf(sp_30 - sp_3c) < 0.008f)) {
                                    f32 sp_1c;
                                    f32 sp_18;
                                    JUT_ASSERT(0x7e2, !isnan(cx1_58));
                                    JUT_ASSERT(0x7e3, !isnan(cy1_54));

                                    cM2d_CrossCirLin(*pwi->GetWallCirP(cir_index_8c), cx1_58, cy1_54, onx_2c, ony_28, &sp_1c, &sp_18);

                                    pwi->GetPos()->x += (cx1_58 - sp_1c);
                                    pwi->GetPos()->z += (cy1_54 - sp_18);

                                    JUT_ASSERT(0x7ed, !isnan(pwi->GetPos()->x));
                                    JUT_ASSERT(0x7ed, !isnan(pwi->GetPos()->z));

                                    JUT_ASSERT(0x7f1, -INF < pwi->GetPos()->x &&
                                                          pwi->GetPos()->x < INF)
                                    JUT_ASSERT(0x7f1, -INF < pwi->GetPos()->z &&
                                                          pwi->GetPos()->z < INF)

                                    pwi->CalcMovePosWork();
                                    pwi->SetWallCirHit(cir_index_8c);
                                    pwi->SetWallPolyIndex(cir_index_8c, sp_a0);
                                    s16 sp_12 = cM_atan2s(sp_98->x, sp_98->z);
                                    pwi->SetWallAngleY(cir_index_8c, sp_12);
                                    sp_10 = true;
                                    pwi->SetWallHit();
                                }
                            }
                        }
                    }
                }
            }
        }
        sp_a4 = sp_a4->next;
    }

    return sp_10;
}

// NONMATCHING - missing mr
bool dBgWKCol::WallCorrect(dBgS_Acch* pwi) {
    bool sp10 = false;

    cM3dGCyl* sp114 = pwi->GetWallBmdCylP();
    cXyz sp16C;
    cXyz sp160;
    sp114->calcMinMax(&sp16C, &sp160);
    sp16C.x -= 1.0f;
    sp16C.y -= 1.0f;
    sp16C.z -= 1.0f;
    sp160.x += 1.0f;
    sp160.y += 1.0f;
    sp160.z += 1.0f;
    Vec* sp110 = &m_pkc_head->m_area_min_pos;
    PSVECSubtract(&sp16C, sp110, &sp16C);
    PSVECSubtract(&sp160, sp110, &sp160);

    int sp10C = (u32)sp16C.x;
    if (sp10C < 0) {
        sp10C = 0;
    }

    int sp108 = (u32)sp160.x;
    if (sp108 > (int)~m_pkc_head->m_area_x_width_mask) {
        sp108 = (int)~m_pkc_head->m_area_x_width_mask;
    }

    if (sp10C >= sp108) {
        return false;
    }

    int sp104 = (u32)sp16C.y;
    if (sp104 < 0) {
        sp104 = 0;
    }

    int sp100 = (u32)sp160.y;
    if (sp100 > (int)~m_pkc_head->m_area_y_width_mask) {
        sp100 = ~m_pkc_head->m_area_y_width_mask;
    }

    if (sp104 >= sp100) {
        return false;
    }

    int spFC = (u32)sp16C.z;
    if (spFC < 0) {
        spFC = 0;
    }

    int spF8 = (u32)sp160.z;
    if (spF8 > ~m_pkc_head->m_area_z_width_mask) {
        spF8 = ~m_pkc_head->m_area_z_width_mask;
    }

    if (spFC >= spF8) {
        return false;
    }

    u16* spF4 = NULL;
    u16* spF0 = NULL;
    u16* spEC = NULL;
    u16* spE8 = NULL;
    u16* spE4 = NULL;
    u16* spE0 = NULL;

    Vec sp154;
    sp154.y = 0.0f;

    int spDC;
    int spD8;
    int spD4;
    int spD0;
    int spCC;
    int spC8;
    int spC4;
    int spC0;
    int spBC = spFC;
    do {
        spCC = 1000000;

        int spB8 = sp104;
        do {
            spD0 = 1000000;
            spC8 = 0;
            spC4 = 0;
            spC0 = 0;
            int spB4 = sp10C;

            do {
                u16* spB0 = (u16*)m_pkc_head->m_block_data;
                int spAC = m_pkc_head->m_block_width_shift;
                int spA8 =
                    4 * (((u32)spBC >> spAC) << m_pkc_head->m_area_xy_blocks_shift |
                         ((u32)spB8 >> spAC) << m_pkc_head->m_area_x_blocks_shift |
                          (u32)spB4 >> spAC);

                while ((spA8 = *(int*)((int)spB0 + spA8)) >= 0) {
                    spB0 = (u16*)((int)spB0 + spA8);
                    spAC--;
                    spA8 = ((((u32)spBC >> spAC) & 1) << 2 |
                            (((u32)spB8 >> spAC) & 1) << 1 |
                            (((u32)spB4 >> spAC) & 1) << 0) * 4;
                }

                u16* spA4 = (u16*)((int)spB0 + (spA8 & 0x7FFFFFFF));
                spAC = 1 << spAC;
                u32 spA0 = spAC - 1;
                spDC = spAC - (spB4 & spA0);
                spD8 = spAC - (spB8 & spA0);
                spD4 = spAC - (spBC & spA0);

                if (spD4 < spCC) {
                    spCC = spD4;
                }

                if (spD8 < spD0) {
                    spD0 = spD8;
                }

                if (spA4[1] != 0 && spD8 > spC0) {
                    if (spD8 > spC4) {
                        if (spD8 > spC8) {
                            spC0 = spC4;
                            spC4 = spC8;
                            spC8 = spD8;
                            spE0 = spE4;
                            spE4 = spE8;
                            spE8 = spA4;
                        }
                        else {
                            spC0 = spC4;
                            spC4 = spD8;
                            spE0 = spE4;
                            spE4 = spA4;
                        }
                    } else {
                        spC0 = spD8;
                        spE0 = spA4;
                    }
                }

                if (spA4 == spF4 || spA4 == spF0 || spA4 == spEC) {
                    continue;
                }

                while (*++spA4 != 0) {
                    KC_PrismData* sp9C = getPrismData(*spA4);
                    Vec* sp98 = m_pkc_head->m_nrm_data + sp9C->fnrm_i;
                    if (cBgW_CheckBGround(sp98->y)) {
                        continue;
                    }

                    f32 sp94 = JMAFastSqrt(sp98->x * sp98->x + sp98->z * sp98->z);
                    if (cM3d_IsZero(sp94)) {
                        continue;
                    }

                    dBgPc adStack_58;
                    getPolyCode(*spA4, &adStack_58);
                    cXyz cStack_88 = *sp98;
                    if (chkPolyThrough(&adStack_58, pwi->GetPolyPassChk(),
                                        pwi->GetGrpPassChk(), cStack_88)) {
                        continue;
                    }

                    cXyz sp13C;
                    cXyz sp130;
                    cXyz sp124;
                    if (!GetTriPnt(sp9C, &sp13C, &sp130, &sp124)) {
                        continue;
                    }

                    f32 sp90 = 1.0f / sp94;
                    for (int sp8C = 0; sp8C < pwi->GetTblSize(); sp8C++) {
                        f32 sp88 = sp90 * pwi->GetWallR(sp8C);
                        sp154.x = sp98->x * sp88;
                        sp154.z = sp98->z * sp88;

                        f32 sp84;
                        if (!pwi->ChkWallHDirect(sp8C)) {
                            sp84 = pwi->GetWallAddY(sp154) +
                                   (pwi->GetPos()->y + pwi->GetWallH(sp8C)) -
                                   pwi->GetSpeedY();
                        } else {
                            sp84 = pwi->GetWallHDirect(sp8C);
                        }


                        f32 sp118[3];
                        sp118[0] = sp13C.y - sp84;
                        sp118[1] = sp130.y - sp84;
                        sp118[2] = sp124.y - sp84;
                        if (sp118[0] > 0.0f && sp118[1] > 0.0f && sp118[2] > 0.0f ||
                            sp118[0] < 0.0f && sp118[1] < 0.0f && sp118[2] < 0.0f) {
                            continue;
                        }

                        int sp80;
                        int sp7C;
                        int sp78;
                        int sp74 = 0;
                        if (cM3d_IsZero(sp118[0])) {
                            sp74++;
                        }
                        if (cM3d_IsZero(sp118[1])) {
                            sp74++;
                        }
                        if (cM3d_IsZero(sp118[2])) {
                            sp74++;
                        }

                        if (sp74 == 1) {
                            continue;
                        }

                        if (sp118[0] > 0.0f && sp118[1] <= 0.0f &&
                                sp118[2] <= 0.0f ||
                            sp118[0] < 0.0f && sp118[1] >= 0.0f &&
                                sp118[2] >= 0.0f)
                        {
                            sp80 = 0;
                            sp7C = 1;
                            sp78 = 2;
                        } else if (sp118[1] > 0.0f && sp118[0] <= 0.0f && sp118[2] <= 0.0f ||
                                   sp118[1] < 0.0f && sp118[0] >= 0.0f && sp118[2] >= 0.0f)
                        {
                            sp80 = 1;
                            sp7C = 0;
                            sp78 = 2;
                        } else {
                            sp80 = 2;
                            sp7C = 0;
                            sp78 = 1;
                        }

                        f32 sp70 = sp118[sp80] - sp118[sp7C];
                        if (cM3d_IsZero(sp70)) {
                            continue;
                        }

                        f32 sp6C = sp118[sp80] - sp118[sp78];
                        if (cM3d_IsZero(sp6C)) {
                            continue;
                        }

                        f32 sp68 = -sp118[sp7C] / sp70;
                        f32 sp64 = -sp118[sp78] / sp6C;
                        f32 cx0;
                        f32 cy0;
                        f32 cx1;
                        f32 cy1;
                        if (sp80 == 0) {
                            cx0 = sp130.x + sp68 * (sp13C.x - sp130.x);
                            cy0 = sp130.z + sp68 * (sp13C.z - sp130.z);
                            cx1 = sp124.x + sp64 * (sp13C.x - sp124.x);
                            cy1 = sp124.z + sp64 * (sp13C.z - sp124.z);
                        } else if (sp80 == 1) {
                            cx0 = sp13C.x + sp68 * (sp130.x - sp13C.x);
                            cy0 = sp13C.z + sp68 * (sp130.z - sp13C.z);
                            cx1 = sp124.x + sp64 * (sp130.x - sp124.x);
                            cy1 = sp124.z + sp64 * (sp130.z - sp124.z);
                        } else {
                            cx0 = sp13C.x + sp68 * (sp124.x - sp13C.x);
                            cy0 = sp13C.z + sp68 * (sp124.z - sp13C.z);
                            cx1 = sp130.x + sp64 * (sp124.x - sp130.x);
                            cy1 = sp130.z + sp64 * (sp124.z - sp130.z);
                        }

                        cx0 += sp154.x;
                        cy0 += sp154.z;
                        cx1 += sp154.x;
                        cy1 += sp154.z;
                        f32 sp50;
                        f32 sp4C;
                        f32 sp48;
                        bool sp0F = cM3d_Len2dSqPntAndSegLine(pwi->GetCx(), pwi->GetCz(), cx0, cy0,
                                                              cx1, cy1, &sp4C, &sp48, &sp50);

                        f32 sp44 = sp4C - pwi->GetCx();
                        f32 sp40 = sp48 - pwi->GetCz();
                        f32 sp3C = pwi->GetWallRR(sp8C);
                        if (sp50 > sp3C || sp44 * sp154.x + sp40 * sp154.z < 0.0f) {
                            continue;
                        }

                        if (sp0F == true) {
                            pwi->SetWallHit();
                            f32 sp38 = sp90 * std::sqrt(sp50);
                            sp38 -= 1.0f;
                            if (sp38 < 0.0f) {
                                sp38 = 0.0f;
                            }
                            pwi->GetPos()->x += sp38 * sp98->x;
                            pwi->GetPos()->z += sp38 * sp98->z;
                            JUT_ASSERT(0x989, !isnan(pwi->GetPos()->x));
                            JUT_ASSERT(0x98a, !isnan(pwi->GetPos()->z));
                            JUT_ASSERT(0x98d, -INF < pwi->GetPos()->x && pwi->GetPos()->x < INF);
                            JUT_ASSERT(0x98f, -INF < pwi->GetPos()->z && pwi->GetPos()->z < INF);
                            pwi->CalcMovePosWork();
                            pwi->SetWallCirHit(sp8C);
                            pwi->SetWallPolyIndex(sp8C,
                                                  *spA4);
                            s16 sp16 = cM_atan2s(sp98->x, sp98->z);
                            pwi->SetWallAngleY(sp8C, sp16);
                            sp10 = true;
                        } else {
                            cx0 -= sp154.x;
                            cy0 -= sp154.z;
                            cx1 -= sp154.x;
                            cy1 -= sp154.z;
                            JUT_ASSERT(0x9a2, pwi->GetPos()->x == pwi->GetWallCirP(sp8C)->GetCx());
                            JUT_ASSERT(0x9a4, pwi->GetPos()->z == pwi->GetWallCirP(sp8C)->GetCy());
                            f32 sp34 = cM3d_Len2dSq(cx0, cy0, pwi->GetPos()->x, pwi->GetPos()->z);
                            f32 sp30 = cM3d_Len2dSq(cx1, cy1, pwi->GetPos()->x, pwi->GetPos()->z);
                            f32 onx = -sp98->x;
                            f32 ony = -sp98->z;
                            JUT_ASSERT(0x9b3, !(cM3d_IsZero(onx) && cM3d_IsZero(ony)));
                            if (sp34 < sp30) {
                                if (sp34 > sp3C || fabsf(sp34 - sp3C) < 0.008f) {
                                    continue;
                                }

                                JUT_ASSERT(0x9be, !isnan(cx0));
                                JUT_ASSERT(0x9bf, !isnan(cy0));
                                f32 sp24;
                                f32 sp20;
                                cM2d_CrossCirLin(*pwi->GetWallCirP(sp8C), cx0, cy0, onx, ony,
                                                 &sp24, &sp20);
                                pwi->GetPos()->x += cx0 - sp24;
                                pwi->GetPos()->z += cy0 - sp20;
                                JUT_ASSERT(0x9d1, !isnan(pwi->GetPos()->x));
                                JUT_ASSERT(0x9d2, !isnan(pwi->GetPos()->z));


                                JUT_ASSERT(0x9d5, -INF < pwi->GetPos()->x && pwi->GetPos()->x < INF);

                                JUT_ASSERT(0x9d7, -INF < pwi->GetPos()->z && pwi->GetPos()->z < INF);

                                pwi->CalcMovePosWork();
                                pwi->SetWallCirHit(sp8C);
                                pwi->SetWallPolyIndex(sp8C, *spA4);
                                s16 sp14 = cM_atan2s(sp98->x, sp98->z);
                                pwi->SetWallAngleY(sp8C, sp14);
                                sp10 = true;
                                pwi->SetWallHit();
                            } else {
                                if (sp30 > sp3C || fabsf(sp30 - sp3C) < 0.008f) {
                                    continue;
                                }

                                JUT_ASSERT(0x9f4, !isnan(cx1));
                                JUT_ASSERT(0x9f5, !isnan(cy1));
                                f32 sp1C;
                                f32 sp18;
                                cM2d_CrossCirLin(*pwi->GetWallCirP(sp8C), cx1, cy1, onx, ony, &sp1C,
                                                 &sp18);
                                pwi->GetPos()->x += cx1 - sp1C;
                                pwi->GetPos()->z += cy1 - sp18;
                                JUT_ASSERT(0xa06, !isnan(pwi->GetPos()->x));
                                JUT_ASSERT(0xa07, !isnan(pwi->GetPos()->z));

                                JUT_ASSERT(0xa0a, -INF < pwi->GetPos()->x && pwi->GetPos()->x < INF);

                                JUT_ASSERT(0xa0c, -INF < pwi->GetPos()->z && pwi->GetPos()->z < INF);

                                pwi->CalcMovePosWork();
                                pwi->SetWallCirHit(sp8C);
                                pwi->SetWallPolyIndex(sp8C, *spA4);
                                s16 sp12 = cM_atan2s(sp98->x, sp98->z);
                                pwi->SetWallAngleY(sp8C, sp12);
                                sp10 = true;
                                pwi->SetWallHit();
                            }
                        }
                    }
                }
            } while ((u32)(spB4 += spDC) <= sp108);

            spF4 = spE8;
            spF0 = spE4;
            spEC = spE0;
        } while ((u32)(spB8 += spD0) <= sp100);
    } while ((u32)(spBC += spCC) <= spF8);

    return sp10;
}

bool dBgWKCol::RoofChk(dBgS_RoofChk* param_0) {
    KC_PrismData* local_94;
    dBgPc adStack_4c;

    Vec* sp40 = param_0->GetPosP();
    cXyz sp74;
    PSVECSubtract(sp40, &m_pkc_head->m_area_min_pos, &sp74);

    u32 sp3C = sp74.x;
    if ((int)sp3C < 0) {
        return false;
    }

    if ((int)sp3C > (int)~m_pkc_head->m_area_x_width_mask) {
        return false;
    }

    u32 sp38 = sp74.z;
    if ((int)sp38 < 0) {
        return false;
    }

    if ((int)sp38 > (int)~m_pkc_head->m_area_z_width_mask) {
        return false;
    }

    u32 sp34 = sp74.y;
    if ((int)sp34 < 0) {
        sp34 = 0;
    }
    if ((int)sp34 > (int)~m_pkc_head->m_area_y_width_mask) {
        return false;
    }

    bool sp0A = false;
    u32 sp30 = ~m_pkc_head->m_area_y_width_mask;
    do {
        uintptr_t sp2C = (uintptr_t)m_pkc_head->m_block_data;
        u32 sp28 = m_pkc_head->m_block_width_shift;
        int sp24 = 4 * (((u32)sp38 >> sp28) << m_pkc_head->m_area_xy_blocks_shift |
                       ((u32)sp34 >> sp28) << m_pkc_head->m_area_x_blocks_shift |
                       (u32)sp3C >> sp28);
        while ((sp24 = (*(int*)(sp2C + (sp24 & 0x7fffffff)))) >= 0) {
            sp2C += sp24;
            sp28--;
            sp24 = (((u32)sp38 >> sp28 & 1) << 2 |
                    ((u32)sp34 >> sp28 & 1) << 1 |
                    ((u32)sp3C >> sp28 & 1) << 0) << 2;
        }

        u16* sp20 = (u16*)(sp2C + (sp24 & 0x7fffffff));

        KC_PrismData* sp1C;
        Vec* sp18;
        Vec* sp14;
        int sp10;
        int sp0C;

        while (*++sp20 != 0) {
            sp1C = getPrismData(*sp20);
            sp18 = m_pkc_head->m_nrm_data + sp1C->fnrm_i;
            if (cBgW_CheckBRoof(sp18->y)) {
                sp14 = m_pkc_head->m_pos_data + sp1C->pos_i;

                cXyz sp5C;
                sp5C.x = sp40->x - sp14->x;
                sp5C.z = sp40->z - sp14->z;
                sp5C.y = -(sp5C.x * sp18->x + sp5C.z * sp18->z) / sp18->y;

                if (PSVECDotProduct(&sp5C, &m_pkc_head->m_nrm_data[sp1C->enrm1_i]) >
                    0.0075f)
                {
                    continue;
                }

                if (PSVECDotProduct(&sp5C,
                                    &m_pkc_head->m_nrm_data[sp1C->enrm2_i]) > 0.0075f)
                {
                    continue;
                }

                f32 dot_f30 = PSVECDotProduct(&sp5C, &m_pkc_head->m_nrm_data[sp1C->enrm3_i]);
                if (dot_f30 < -0.0075f || dot_f30 > sp1C->height + 0.0075f) {
                    continue;
                }

                cXyz sp58;
                PSVECSubtract(sp40, sp14, &sp58);
                if (PSVECDotProduct(&sp58, sp18) < 0.0f) {
                    continue;
                }

                getPolyCode(*sp20, &adStack_4c);
                cXyz sp44 = *sp18;
                if (!chkPolyThrough(&adStack_4c, param_0->GetPolyPassChk(), param_0->GetGrpPassChk(), sp44)) {
                    f32 tmp_height_kcw = sp5C.y + sp14->y;
                    if (param_0->GetNowY() > tmp_height_kcw && sp40->y < tmp_height_kcw) {
                        param_0->SetPolyIndex(*sp20);
                        param_0->SetNowY(tmp_height_kcw);
                        sp0A = true;
                        JUT_ASSERT(0xac8, !isnan(tmp_height_kcw));
                        JUT_ASSERT(0xacb, -FP_INFINITE < tmp_height_kcw && tmp_height_kcw < FP_INFINITE);
                        bool sp08 = true;
                        if (!(tmp_height_kcw - m_pkc_head->m_area_min_pos.y >= 0.0f)) {
                            OS_PANIC(0xacf, "Failed assertion tmp_height_kcw - m_pkc_head->m_area_min_pos.y >= 0.0f");
                            sp0C = 0;
                            if (sp0C == 0) {
                                sp08 = false;
                            }
                        }
                        sp30 = tmp_height_kcw - m_pkc_head->m_area_min_pos.y;
                    }
                }
            }
        }

        sp28 = 1 << sp28;
        sp10 = sp28 - 1;
        sp34 += sp28 - (sp34 & sp10);
    } while ((int)sp34 <= (int)sp30);

    return sp0A;
}

bool dBgWKCol::SplGrpChk(dBgS_SplGrpChk* param_0) {
    Vec* sp3C = &param_0->GetPosP();
    cXyz sp54;
    PSVECSubtract(sp3C, &m_pkc_head->m_area_min_pos, &sp54);

    u32 sp38 = sp54.x;
    if ((int)sp38 < 0) {
        return false;
    }

    if ((int)sp38 > (int)~m_pkc_head->m_area_x_width_mask) {
        return false;
    }

    u32 sp34 = sp54.z;
    if ((int)sp34 < 0) {
        return false;
    }

    if ((int)sp34 > (int)~m_pkc_head->m_area_z_width_mask) {
        return false;
    }

    u32 sp30 = sp54.y;
    if ((int)sp30 < 0) {
        sp30 = 0;
    }

    u32 sp2C = param_0->GetRoof() - m_pkc_head->m_area_min_pos.y;
    if ((int)sp2C > (int)~m_pkc_head->m_area_y_width_mask) {
        sp2C = ~m_pkc_head->m_area_y_width_mask;
    }
    if ((int)sp30 >= (int)sp2C) {
        return false;
    }

    bool sp09 = false;
    do {
        u32 sp28 = (u32)m_pkc_head->m_block_data;
        u32 sp24 = m_pkc_head->m_block_width_shift;
        int sp20 = 4 * (((u32)sp34 >> sp24) << m_pkc_head->m_area_xy_blocks_shift |
                        ((u32)sp2C >> sp24) << m_pkc_head->m_area_x_blocks_shift |
                         (u32)sp38 >> sp24);
        while ((sp20 = *(int*)((int)sp28 + sp20)) >= 0) {
            sp28 = ((int)sp28 + sp20);
            sp24--;
            sp20 = 4 *
                (((u32)sp34 >> sp24 & 1) << 2 |
                 ((u32)sp2C >> sp24 & 1) << 1 |
                 ((u32)sp38 >> sp24 & 1) << 0);
        }

        u16* sp1C = (u16*)(sp28 + (sp20 & 0x7fffffff));
        while (*++sp1C != 0) {
            KC_PrismData* sp18 = getPrismData(*sp1C);
            Vec* sp14 = m_pkc_head->m_nrm_data + sp18->fnrm_i;
            if (!(sp14->y <= 0.0f) && !cM3d_IsZero(sp14->y)) {
                dBgPc sp64;
                getPolyCode(*sp1C, &sp64);
                cXyz sp4C = *sp14;
                if (!chkPolyThrough(&sp64, param_0->GetPolyPassChk(),
                                    param_0->GetGrpPassChk(), sp4C))
                {
                    Vec* sp10 = m_pkc_head->m_pos_data + sp18->pos_i;
                    cXyz sp40;
                    sp40.x = sp3C->x - sp10->x;
                    sp40.z = sp3C->z - sp10->z;
                    sp40.y = -(sp40.x * sp14->x + sp40.z * sp14->z) / sp14->y;
                    if (PSVECDotProduct(&sp40, &m_pkc_head->m_nrm_data[sp18->enrm1_i]) >
                        0.0075f)
                    {
                        continue;
                    }

                    if (PSVECDotProduct(&sp40, &m_pkc_head->m_nrm_data[sp18->enrm2_i]) >
                            0.0075f)
                    {
                        continue;
                    }

                    f32 var_f30 = PSVECDotProduct(&sp40, &m_pkc_head->m_nrm_data[sp18->enrm3_i]);
                    if (var_f30 < -0.0075f || var_f30 > sp18->height + 0.0075f) {
                        continue;
                    }

                    f32 tmp_height_kcw = sp40.y + sp10->y;
                    if (param_0->GetHeight() < tmp_height_kcw && sp3C->y < tmp_height_kcw &&
                        param_0->GetRoof() > tmp_height_kcw)
                    {
                        param_0->SetHeight(tmp_height_kcw);
                        param_0->SetPolyIndex(*sp1C);

                        sp09 = true;

                        JUT_ASSERT(0xb73, !isnan(tmp_height_kcw));
                        JUT_ASSERT(0xb76,
                                   -FP_INFINITE < tmp_height_kcw && tmp_height_kcw < FP_INFINITE);
                    }
                }
            }
        }

        //sp28 = 1 << sp28;
        //sp10 = sp28 - 1;
        //sp34 += sp28 - (sp34 & sp10);
        sp24 = 1 << sp24;
        u32 sp0C = sp24 - 1;
        sp2C = sp2C & ~sp0C;
        sp2C--;
    } while ((int)sp2C >= (int)sp30);

    return sp09;
}

bool dBgWKCol::SphChk(dBgS_SphChk* param_0, void* param_1) {
    static Vec vtx_tbl[3];

    dBgPc spD4;

    cXyz spB0;
    cXyz spA4;
    param_0->GetMinMaxCube(spB0, spA4);
    PSVECSubtract(&spB0, &m_pkc_head->m_area_min_pos, &spB0);
    PSVECSubtract(&spA4, &m_pkc_head->m_area_min_pos, &spA4);
    spB0.x -= 1.0f;
    spB0.y -= 1.0f;
    spB0.z -= 1.0f;
    spA4.x += 1.0f;
    spA4.y += 1.0f;
    spA4.z += 1.0f;

    int sp64;
    int sp60;
    int sp5C;
    int sp58;
    int sp54;
    int sp50;

    sp64 = spB0.x;
    if (sp64 < 0) {
        sp64 = 0;
    }

    sp58 = spA4.x;
    if (sp58 > (int)~m_pkc_head->m_area_x_width_mask) {
        sp58 = ~m_pkc_head->m_area_x_width_mask;
    }
    if (sp64 >= sp58) {
        return false;
    }

    sp60 = spB0.y;
    if (sp60 < 0) {
        sp60 = 0;
    }

    sp54 = spA4.y;
    if (sp54 > (int)~m_pkc_head->m_area_y_width_mask) {
        sp54 = ~m_pkc_head->m_area_y_width_mask;
    }

    if (sp60 >= sp54) {
        return false;
    }

    sp5C = spB0.z;
    if (sp5C < 0) {
        sp5C = 0;
    }

    sp50 = spA4.z;
    if (sp50 > (int)~m_pkc_head->m_area_z_width_mask) {
        sp50 = ~m_pkc_head->m_area_z_width_mask;
    }

    if (sp5C >= sp50) {
        return false;
    }

    bool sp0D = false;
    cXyz sp98;
    int sp4C;
    int sp48;
    int sp44;
    int sp40;
    int sp3C;
    int sp38;
    u16* sp34 = NULL;
    u16* sp30 = NULL;
    bool sp0C = false;
    int sp2C = sp5C;
    do {
        sp3C = 1000000;
        int sp28 = sp60;
        do {
            sp40 = 1000000;
            sp38 = 0;
            int sp24 = sp64;
            do {
                u16* sp20 = (u16*)m_pkc_head->m_block_data;
                u32 var_r29 = m_pkc_head->m_block_width_shift;
                int sp1C = (((u32)sp2C >> var_r29 << m_pkc_head->m_area_xy_blocks_shift) |
                            ((u32)sp28 >> var_r29 << m_pkc_head->m_area_x_blocks_shift) |
                            ((u32)sp24 >> var_r29)) * 4;
                while ((sp1C = *(int*)((int)sp20 + sp1C)) >= 0) {
                    sp20 = (u16*)((int)sp20 + sp1C);
                    var_r29--;
                    sp1C = (((u32)sp2C >> var_r29 & 1) << 2 |
                            ((u32)sp28 >> var_r29 & 1) << 1 |
                            ((u32)sp24 >> var_r29 & 1)) * 4;
                }

                u16* var_r28 = (u16*)((int)sp20 + (sp1C & 0x7fffffff));
                var_r29 = 1 << var_r29;
                int sp18 = var_r29 - 1;
                sp4C = var_r29 - (sp24 & sp18);
                sp48 = var_r29 - (sp28 & sp18);
                sp44 = var_r29 - (sp2C & sp18);
                if (sp44 < sp3C) {
                    sp3C = sp44;
                }
                if (sp48 < sp40) {
                    sp40 = sp48;
                }
                if (sp48 > sp38 && *(var_r28 + 1) != 0) {
                    sp38 = sp48;
                    sp34 = var_r28;
                }
                if (var_r28 != sp30) {
                    while (*++var_r28 != 0) {
                        KC_PrismData* sp14 = getPrismData(*var_r28);
                        Vec* sp10 = &m_pkc_head->m_nrm_data[sp14->fnrm_i];
                        getPolyCode(*var_r28, &spD4);
                        cXyz sp90 = *sp10;
                        if (!chkPolyThrough(&spD4, param_0->GetPolyPassChk(),
                                            param_0->GetGrpPassChk(), sp90))
                        {
                            cM3dGTri spE8;
                            cXyz sp80;
                            cXyz sp74;
                            cXyz sp68;
                            if (!GetTriPnt(*var_r28, &sp80, &sp74, &sp68)) {
                                continue;
                            }

                            spE8.set(&sp80, &sp74, &sp68, sp10);
                            if (param_0->cross(&spE8)) {
                                vtx_tbl[0] = sp80;
                                vtx_tbl[1] = sp74;
                                vtx_tbl[2] = sp68;
                                cM3dGPla spBC;
                                spBC = spE8;
                                (param_0->mCallback)(param_0, (cBgD_Vtx_t*)vtx_tbl, 0, 1, 2,
                                                     &spBC, param_1);
                                param_0->SetPolyIndex(*var_r28);
                                sp0C = true;
                            }
                        }
                    }
                }
            } while ((u32)(sp24 += sp4C) <= sp58);

            sp30 = sp34;
        } while ((u32)(sp28 += sp40) <= sp54);

    } while ((u32)(sp2C += sp3C) <= sp50);

    return sp0C;
}

void dBgWKCol::GetTopUnder(f32* param_0, f32* param_1) const {
    *param_1 = m_pkc_head->m_area_min_pos.y;
    *param_0 = *param_1 + (f32)~m_pkc_head->m_area_y_width_mask;
}

int dBgWKCol::GetGrpRoomIndex(const cBgS_PolyInfo& poly) const {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getRoom();
}

int dBgWKCol::GetExitId(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getExit();
}

int dBgWKCol::GetPolyColor(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getPolyCol();
}

BOOL dBgWKCol::GetHorseNoEntry(const cBgS_PolyInfo& poly) {
    dBgPc bgpc;

    u32 poly_index = poly.GetPolyIndex();
    getPolyCode(poly_index, &bgpc);
    return bgpc.getHorseNoEntry() != 0;
}

int dBgWKCol::GetSpecialCode(const cBgS_PolyInfo& poly) {
    return GetSpecialCode(poly.GetPolyIndex());
}

int dBgWKCol::GetSpecialCode(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getSpl();
}

int dBgWKCol::GetMagnetCode(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getMagnet();
}

int dBgWKCol::GetMonkeyBarsCode(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getMonkeyBars();
}

u32 dBgWKCol::GetPolyObjThrough(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getObjThrough();
}

u32 dBgWKCol::GetPolyCamThrough(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getCamThrough();
}

u32 dBgWKCol::GetPolyLinkThrough(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getLinkThrough();
}

u32 dBgWKCol::GetPolyArrowThrough(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getArrowThrough();
}

u32 dBgWKCol::GetPolyHSStick(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getHSStick();
}

u32 dBgWKCol::GetPolyBoomerangThrough(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getBoomerangThrough();
}

u32 dBgWKCol::GetPolyRopeThrough(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getRopeThrough();
}

u32 dBgWKCol::GetPolyBombThrough(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getBombThrough();
}

u32 dBgWKCol::GetUnderwaterRoofCode(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getUnderwaterRoof();
}

bool dBgWKCol::GetShdwThrough(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getShdwThrough();
}

int dBgWKCol::GetLinkNo(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getLinkNo();
}

int dBgWKCol::GetWallCode(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getWallCode();
}

int dBgWKCol::GetPolyAtt0(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getAtt0Code();
}

int dBgWKCol::GetPolyAtt1(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getAtt1Code();
}

int dBgWKCol::GetGroundCode(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getGroundCode();
}

u32 dBgWKCol::GetIronBallThrough(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getIronBallThrough();
}

u32 dBgWKCol::GetAttackThrough(int poly_index) {
    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);

    return bgpc.getAttackThrough();
}

int dBgWKCol::GetCamMoveBG(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getCamMoveBG();
}

int dBgWKCol::GetRoomCamId(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getRoomCam();
}

int dBgWKCol::GetRoomPathId(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getRoomPath();
}

int dBgWKCol::GetRoomPathPntNo(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getRoomPathPnt();
}

u8 dBgWKCol::GetPolyGrpRoomInfId(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getRoomInf();
}

int dBgWKCol::GetGrpSoundId(const cBgS_PolyInfo& poly) {
    int poly_index = poly.GetPolyIndex();

    dBgPc bgpc;
    getPolyCode(poly_index, &bgpc);
    return bgpc.getSnd();
}

void dBgWKCol::CrrPos(const cBgS_PolyInfo& param_0, void* param_1, bool param_2, cXyz* param_3,
                      csXyz* param_4, csXyz* param_5) {}

void dBgWKCol::TransPos(const cBgS_PolyInfo& param_0, void* param_1, bool param_2, cXyz* param_3,
                        csXyz* param_4, csXyz* param_5) {}

void dBgWKCol::MatrixCrrPos(const cBgS_PolyInfo& param_0, void* param_1, bool param_2,
                            cXyz* param_3, csXyz* param_4, csXyz* param_5) {}
