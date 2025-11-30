/**
 * @file d_a_mg_fish.cpp
 *
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_mg_fish.h"

#include "JSystem/JKernel/JKRSolidHeap.h"
#include "SSystem/SComponent/c_cc_d.h"
#include "SSystem/SComponent/c_counter.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_b_ob.h"
#include "d/actor/d_a_e_sg.h"
#include "d/actor/d_a_fr.h"
#include "d/actor/d_a_mg_rod.h"
#include "d/actor/d_a_npc_henna.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/d_vibration.h"
#include "f_op/f_op_kankyo_mng.h"

#define ANM_MG_FISH_MOUTH_CLOSE     4
#define ANM_MG_FISH_MOUTH_OPEN      5

#define ACTION_MG_FISH_MF_SWIM 0
#define ACTION_MG_FISH_MF_STAY 1
#define ACTION_MG_FISH_MF_AWAY 2
#define ACTION_MG_FISH_MF_SWIM_P 4
#define ACTION_MG_FISH_MF_SWIM_S 5
#define ACTION_MG_FISH_RI_SWIM 10
#define ACTION_MG_FISH_MF_AQUA 20
#define ACTION_MG_FISH_RI_AQUA 21
#define ACTION_MG_FISH_MF_AQUA_TILT 22
#define ACTION_MG_FISH_MF_LURE_SEARCH 50
#define ACTION_MG_FISH_MF_BAIT_SEARCH 51
#define ACTION_MG_FISH_MF_HIT 52
#define ACTION_MG_FISH_MF_JUMP 53
#define ACTION_MG_FISH_MF_CATCH 60
#define ACTION_MG_FISH_MF_ESA_SEARCH 70
#define ACTION_MG_FISH_MF_ESA_HIT 71
#define ACTION_MG_FISH_MF_ESA_CATCH 72

#define GEDOU_KIND_LM_1 0
#define GEDOU_KIND_RI_1 1
#define GEDOU_KIND_NP_1 2
#define GEDOU_KIND_CF_1 3
#define GEDOU_KIND_KS_1 4
#define GEDOU_KIND_BG 5
#define GEDOU_KIND_LM_2 6
#define GEDOU_KIND_RI_2 7
#define GEDOU_KIND_NP_2 8
#define GEDOU_KIND_CF_2 9
#define GEDOU_KIND_KS_2 10
#define GEDOU_KIND_BT 20
#define GEDOU_KIND_LH 21
#define GEDOU_KIND_SP 22
#define GEDOU_KIND_BB 23
#define GEDOU_KIND_O_GD_BOTT 24
#define GEDOU_KIND_KN 25
#define GEDOU_KIND_ED 26
#define GEDOU_KIND_SY 27

static u16 check_kind[4] = {
    0xF57F,
    0xF47F,
    0xF37F,
    0xF27F,
};

struct PathNode {
    s16 index;
    Vec pos;
};

daMg_Fish_HIO_c::daMg_Fish_HIO_c() {
    this->field_0x4 = -1;
    this->field_0x8 = 1.0f;
    this->field_0xc = 5.0f;
    this->field_0x10 = 300.0f;
    this->field_0x14 = 400.0f;
    this->field_0x18 = 0x0;
    this->field_0x19 = 0x0;
    this->field_0x1a = 0x0;
}

static void hit_vib_set(mg_fish_class* i_this) {
    static s32 vib_p[4] = { 1, 2, 3, 4 };

    dmg_rod_class* rod = (dmg_rod_class*)fopAcM_SearchByID(i_this->mRodId);
    if (rod != NULL && rod->vib_timer == 0) {
        s32 vib_index = (rod->field_0xf60 - 100.0f) * 0.02f;
        if (vib_index < 0) {
            vib_index = 0;
        } else if (vib_index > 3) {
            vib_index = 3;
        }
        dComIfGp_getVibration().StartShock(vib_p[vib_index], 1,
            cXyz(0.0f, 1.0f, 0.0f));
    }
}

static void sibuki_set(mg_fish_class* i_this, f32 scale, cXyz i_pos) {
    double newScale = 0.4f * scale * i_this->mJointScale;
    i_pos.y = i_this->mSurfaceY;
    fopKyM_createWpillar(&i_pos, newScale, 0);
    fopAcM_effHamonSet(&i_this->field_0xc20, &i_pos, newScale, 0.2f);
}

// needed for .rodata ordering
static f32 dummy_3759() {
    return 30.0f;
}

static void* s_hitfish_sub(void* a, void* b) {
    if (fopAc_IsActor(a)) {
        mg_fish_class* fish = (mg_fish_class*)a;
        if (fopAcM_GetName(fish) == PROC_MG_FISH && fish->mCurAction == ACTION_MG_FISH_MF_HIT) {
            return &fish->actor.base;
        }
    }
    return NULL;
}

static s32 pl_check(mg_fish_class* i_this, f32 speed) {
    if (i_this->mDistToPlayer < fabsf(dComIfGp_getPlayer(0)->speedF) * 10.0f + speed) {
        return 1;
    }

    fopAc_ac_c* actor = (fopAc_ac_c*)fpcEx_Search(s_hitfish_sub, i_this);
    if (actor != NULL) {
        if ((actor->current.pos - i_this->actor.current.pos).abs() < speed) {
            return 3;
        }
    }

    return 0;
}

static void anm_init(mg_fish_class* i_this, int i_resIndex, f32 i_morf, u8 i_mode, f32 i_speed) {
    J3DAnmTransform* anmTransform =
        (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, i_resIndex);
    i_this->mpMorf->setAnm(anmTransform, i_mode, i_morf, i_speed,
        0.0f, -1.0f, NULL);
    i_this->mAnmID = i_resIndex;
}

static void mouth_open(mg_fish_class* i_this) {
    if (i_this->mAnmTransform != NULL) {
        anm_init(i_this, ANM_MG_FISH_MOUTH_OPEN, 2.0f, 0, 1.0f);
    } else {
        if (i_this->mKind2 == 2) {
            i_this->mRotZ = -3000;
        }
    }
}

static void mouth_close(mg_fish_class* i_this) {
    if (i_this->mAnmTransform != NULL) {
        anm_init(i_this, ANM_MG_FISH_MOUTH_CLOSE, 2.0f, 0, 1.0f);
    } else {
        if (i_this->mKind2 == 2) {
            i_this->mRotZ = 0;
        }
    }
}

static void* s_lure_sub(void* a, void* b) {
    if (fopAc_IsActor(a)) {
        if (fopAcM_GetName(a) == PROC_MG_ROD) {
            dmg_rod_class* rod = (dmg_rod_class*)a;
            if (rod->kind == 0 &&
                rod->action == 4 &&
                rod->field_0x10a9 == 0 &&
                rod->field_0x100d != 0) {
                return rod;
            }
        }
    }
    return NULL;
}

static void* s_esa_sub(void* a, void* b) {
    if (fopAc_IsActor(a)) {
        if (fopAcM_GetName(a) == PROC_MG_ROD) {
            dmg_rod_class* rod = (dmg_rod_class*)a;
            if (rod->kind == 1 &&
                rod->action != 5 &&
                rod->field_0x100d != 0 &&
                rod->actor.current.pos.y < rod->field_0x590 - 20.0f) {
                return rod;
            }
        }
    }
    return NULL;
}

static void* s_bait_sub(void* a, void* b) {
    if (fopAc_IsActor(a)) {
        if (fopAcM_GetName(a) == PROC_FR) {
            fr_class* fr = (fr_class*)a;
            if (fr->field_0x5ec != 0 &&
                fr->speedF > 1.0f) {
                return fr;
            }
        }
    }
    return NULL;
}
static u8 lit_1008;
static u8 lit_1007;

static daMg_Fish_HIO_c l_HIO;

static s32 s_fish_ct;

static void* s_other_search_sub(void* a, void* b) {
    if (fopAc_IsActor(a) && b != a && fopAcM_GetName(a) == PROC_MG_FISH) {
        mg_fish_class* fish = (mg_fish_class*)a;
        if (fish->mCurAction == ACTION_MG_FISH_MF_LURE_SEARCH) {
            s_fish_ct += 1;
        }
    }
    return NULL;
}

static s32 search_lure(mg_fish_class* i_this, int param_2) {
    static u8 learn_d[5] = {
        0x01, 0x02, 0x04, 0x08, 0x10,
    };

    if (param_2 == 0) {
        fopAc_ac_c* rod_actor = (fopAc_ac_c*)fpcM_Search(s_lure_sub, i_this);
        if (rod_actor != NULL) {
            dmg_rod_class* rod = (dmg_rod_class*)rod_actor;
            s_fish_ct = 0;
            fpcM_Search(s_other_search_sub, i_this);
            if (s_fish_ct <= 1) {
                f32 fVar1 = i_this->field_0x5ec;
                if (rod->lure_type == MG_LURE_SP) {
                    fVar1 = 1000.0f;
                } else {
                    if (rod->field_0x1009 != 0) {
                        fVar1 *= 1.5f;
                    }
                    if (s_fish_ct > 0 && i_this->mGedouKind != GEDOU_KIND_CF_1) {
                        fVar1 *= 0.5f;
                    }
                }
                if (i_this->field_0xc44 >= 0x14) {
                    fVar1 *= 0.5f;
                }
                if (i_this->mGedouKind != GEDOU_KIND_CF_1 &&
                    rod->lure_type != MG_LURE_SP &&
                    (i_this->field_0x750 & learn_d[rod->lure_type]) != 0)
                {
                    return -1;
                }
                cXyz diff = rod->actor.current.pos - i_this->actor.current.pos;
                if (rod->lure_type == MG_LURE_PO && rod->field_0x100c >= 10) {
                    diff.y = -80.0f;
                }
                if (diff.abs() < fVar1) {
                    return fopAcM_GetID(rod);
                }
            }
        }
    } else if (param_2 == 1) {
        fopAc_ac_c* rod_actor = (fopAc_ac_c*)fpcM_Search(s_esa_sub, i_this);
        if (rod_actor != NULL) {
            dmg_rod_class* rod = (dmg_rod_class*)rod_actor;
            if (i_this->mGedouKind == GEDOU_KIND_KS_2 && rod->hook_kind != 1) {
                return -1;
            }
            if ((i_this->mGedouKind == GEDOU_KIND_BG || rod->hook_kind == 1 || rod->esa_kind != 0) &&
                rod->actor.current.pos.y < i_this->mSurfaceY - 60.0f)
            {
                f32 maxLatDist = i_this->field_0x5ec;
                f32 distX = rod->actor.current.pos.x - i_this->actor.current.pos.x;
                f32 distZ = rod->actor.current.pos.z - i_this->actor.current.pos.z;
                f32 latDist = JMAFastSqrt(distX * distX + distZ * distZ);
                if (latDist < maxLatDist) {
                    return fopAcM_GetID(rod);
                }
            }
        }
    }
    return -1;
}



static PathNode swim_path[43] = {
    { 0,  { -1394.0f, -80.0f, 559.0f } },
    { 1,  { -2327.0f, -80.0f, 1240.0f } },
    { 2,  { -3002.0f, -80.0f, 1481.0f } },
    { 3,  { -4268.0f, -80.0f, 1723.0f } },
    { 4,  { -5114.0f, -80.0f, 1786.0f } },
    { 5,  { -6163.0f, -80.0f, 2230.0f } },
    { 6,  { -6924.0f, -80.0f, 2591.0f } },
    { 7,  { -7840.0f, -80.0f, 2494.0f } },
    { 8,  { -8343.0f, -80.0f, 2120.0f } },
    { 9,  { -8060.0f, -80.0f, 1361.0f } },
    { 10, { -7754.0f, -80.0f, 773.0f } },
    { 11, { -7379.0f, -80.0f, 515.0f } },
    { 12, { -6807.0f, -80.0f, 549.0f } },
    { 13, { -6222.0f, -80.0f, 540.0f } },
    { 14, { -5343.0f, -80.0f, 21.0f } },
    { 15, { -4951.0f, -80.0f, -547.0f } },
    { 16, { -4825.0f, -80.0f, -1112.0f } },
    { 17, { -4132.0f, -80.0f, -1578.0f } },
    { 18, { -3561.0f, -80.0f, -1891.0f } },
    { 19, { -3274.0f, -80.0f, -2824.0f } },
    { 20, { -2693.0f, -80.0f, -3434.0f } },
    { 21, { -1915.0f, -80.0f, -3586.0f } },
    { 22, { -1360.0f, -80.0f, -3800.0f } },
    { 23, { -47.0f, -80.0f, -3921.0f } },
    { 24, { 716,.0f -80.0f, -3180.0f } },
    { 25, { 696,.0f -80.0f, -2490.0f } },
    { 26, { -391.0f, -80.0f, -1086.0f } },
    { 27, { -1250.0f, -80.0f, 338.0f } },
    { 28, { -1465.0f, -80.0f, 1203.0f } },
    { 29, { -1553.0f, -80.0f, 2380.0f } },
    { 30, { -493.0f, -80.0f, 3106.0f } },
    { 31, { 330.0f, -80.0f, 3403.0f } },
    { 32, { 1615.0f, -80.0f, 3361.0f } },
    { 33, { 3422.0f, -80.0f, 2460.0f } },
    { 34, { 3347.0f, -80.0f, 1203.0f } },
    { 35, { 3136.0f, -80.0f, 839.0f } },
    { 36, { 2754.0f, -80.0f, 244.0f } },
    { 37, { 2096.0f, -80.0f, -127.0f } },
    { 38, { 1452.0f, -80.0f, -964.0f } },
    { 39, { 252,.0f -80.0f, -1556.0f } },
    { 40, { -215.0f, -80.0f, -1227.0f } },
    { 41, { -1149.0f, -80.0f, 272.0f } },
    { -1, { 0.0f, 0.0f, 0.0f } },
};

static void* s_other_bait_sub(void* a, void* b) {
    if (fopAc_IsActor(a) && b != a && fopAcM_GetName(a) == PROC_MG_FISH) {
        mg_fish_class* fish = (mg_fish_class*)a;
        if (fish->mCurAction == ACTION_MG_FISH_MF_BAIT_SEARCH &&
            fish->mBaitId == ((mg_fish_class*)b)->mBaitId) {
            return fish;
        }
    }
    return NULL;
}

static s32 search_bait(mg_fish_class* i_this) {
    void* searchRes = fpcM_Search(s_bait_sub, i_this);
    if (searchRes != NULL) {
        i_this->mBaitId = fopAcM_GetID(searchRes);
        if (fpcM_Search(s_other_bait_sub, i_this) == NULL) {
            f32 fVar1 = i_this->field_0x5ec + KREG_F(8);
            cXyz acStack_38 = ((fopAc_ac_c*)searchRes)->current.pos - i_this->actor.current.pos;
            f32 fVar5 = acStack_38.abs();
            if (fVar5 < fVar1) {
                return i_this->mBaitId;
            }
        }
    }
    return -1;
}

static int nodeCallBack(J3DJoint* joint, int param_1) {
    if (param_1 == 0) {
        s32 jointNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        mg_fish_class* fish = (mg_fish_class*)model->getUserArea();
        dComIfGp_getPlayer(0);
        if (fish != 0) {
            if (jointNo <= fish->mNumJoints) {
                MtxTrans(fish->mJointTranslations[jointNo].x, fish->mJointTranslations[jointNo].y,
                    fish->mJointTranslations[jointNo].z, 0);
                cMtx_YrotM(*calc_mtx, fish->mJointRotations[jointNo].y);
                cMtx_XrotM(*calc_mtx, fish->mJointRotations[jointNo].x);
                cMtx_ZrotM(*calc_mtx, fish->mJointRotations[jointNo].z - fish->actor.shape_angle.z);
                cMtx_YrotM(*calc_mtx, -16384);
                MtxScale(fish->mJointScale, fish->mJointScale, fish->mJointScale, 1);
                model->setAnmMtx(jointNo, *calc_mtx);
            } else {
                if (fish->mKind2 == 3 && jointNo >= 9 && jointNo <= 12) {
                    MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
                    MtxScale(0.0f, 0.0f,
                        0.0f, 1);
                    model->setAnmMtx(jointNo, *calc_mtx);
                }

                if (fish->mKind2 == 1 || fish->mKind2 == 2 || fish->mKind2 == 4) {
                    MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
                    if (jointNo == 7 || jointNo == 8) {
                        cMtx_YrotM(*calc_mtx, fish->jointYaws2[jointNo - 3]);
                    }
                    model->setAnmMtx(jointNo, *calc_mtx);
                    MTXCopy(*calc_mtx, &J3DSys::mCurrentMtx[0]);
                } else if (fish->mKind2 == 0) {
                    MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
                    if (jointNo == 5 || jointNo == 6) {
                        cMtx_YrotM(*calc_mtx, fish->jointYaws2[jointNo - 1]);
                    }
                    model->setAnmMtx(jointNo, *calc_mtx);
                    MTXCopy(*calc_mtx, &J3DSys::mCurrentMtx[0]);
                } else if (fish->mGedouKind == GEDOU_KIND_BG) {
                    MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
                    if (jointNo == 5 || jointNo == 6) {
                        cMtx_YrotM(*calc_mtx, fish->jointYaws2[jointNo - 1]);
                    }
                    model->setAnmMtx(jointNo, *calc_mtx);
                    MTXCopy(*calc_mtx, &J3DSys::mCurrentMtx[0]);
                }

                if ((fish->mKind2 == 4 || fish->mKind2 == 2) && jointNo == 10) {
                    MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
                    cMtx_ZrotM(*calc_mtx, fish->mRotZ);
                    model->setAnmMtx(jointNo, *calc_mtx);
                    MTXCopy(*calc_mtx, &J3DSys::mCurrentMtx[0]);
                }
            }
        }
    }

    return 1;
}

static int nodeCallBack2(J3DJoint* joint, int param_1) {
    if (param_1 == 0) {
        s32 jointNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        mg_fish_class* fish = (mg_fish_class*)model->getUserArea();
        dComIfGp_getPlayer(0);
        if (fish != NULL) {
            MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
            if (fish->mKind2 == 3 && jointNo >= 9 && jointNo <= 12) {
                MtxScale(0.0f, 0.0f, 0.0f, 1.0f);
                model->setAnmMtx(jointNo, *calc_mtx);
            }

            if (jointNo >= 1 && jointNo <= fish->mNumJoints) {
                cMtx_YrotM(*calc_mtx, fish->jointYaws1[jointNo - 1]);
            }

            if (fish->mKind2 == 1 || fish->mKind2 == 2 || fish->mKind2 == 4) {
                if (jointNo == 7 || jointNo == 8) {
                    cMtx_YrotM(*calc_mtx, fish->jointYaws2[jointNo - 3]);
                }
            } else if (fish->mKind2 == 0) {
                if (jointNo == 5 || jointNo == 6) {
                    cMtx_YrotM(*calc_mtx, fish->jointYaws2[jointNo - 1]);
                }
            } else if (fish->mGedouKind == GEDOU_KIND_BG && (jointNo == 5 || jointNo == 6)) {
                cMtx_YrotM(*calc_mtx, fish->jointYaws2[jointNo - 1]);
            }
            model->setAnmMtx(jointNo, *calc_mtx);
            MTXCopy(*calc_mtx, &J3DSys::mCurrentMtx[0]);
        }
    }
    return 1;
}

static int nodeCallBackLH(J3DJoint* joint, int param_1) {
    if (param_1 == 0) {
        s32 jointNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        mg_fish_class* fish = (mg_fish_class*)model->getUserArea();
        if (fish != NULL && jointNo == 1) {
            MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
            f32 dVar4 = fish->field_0x74c + 1.0f;
            MtxScale(1.0f / dVar4, dVar4, dVar4, 1.0f);
            model->setAnmMtx(jointNo, *calc_mtx);
            MTXCopy(*calc_mtx, &J3DSys::mCurrentMtx[0]);
        }
    }
    return 1;
}

static int nodeCallBackBB(J3DJoint* joint, int param_1) {
    if (param_1 == 0) {
        s32 jointNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        mg_fish_class* fish = (mg_fish_class*)model->getUserArea();
        if (fish != NULL && (jointNo == 2 || jointNo == 3 || jointNo == 4)) {
            MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
            cMtx_YrotM(*calc_mtx, fish->jointYaws1[jointNo - 2]);
            model->setAnmMtx(jointNo, *calc_mtx);
            MTXCopy(*calc_mtx, &J3DSys::mCurrentMtx[0]);
        }
    }
    return 1;
}

static void* s_bt_sub(void* a, void* b) {
    if (fopAc_IsActor(a) && fopAcM_GetName(a) == PROC_MG_FISH) {
        mg_fish_class* fish = (mg_fish_class*)a;
        if (fish->mGedouKind == GEDOU_KIND_BT) {
            return fish;
        }
    }
    return NULL;
}

static void* s_sp_sub(void* a, void* b) {
    if (fopAc_IsActor(a) && fopAcM_GetName(a) == PROC_MG_FISH) {
        mg_fish_class* fish = (mg_fish_class*)a;
        if (fish->mGedouKind == GEDOU_KIND_SP) {
            return fish;
        }
    }
    return NULL;
}

static void* s_lh_sub(void* a, void* b) {
    if (fopAc_IsActor(a) && fopAcM_GetName(a) == PROC_MG_FISH) {
        mg_fish_class* fish = (mg_fish_class*)a;
        if (fish->mGedouKind == GEDOU_KIND_LH) {
            fish->field_0x74c = ((mg_fish_class*)b)->field_0x74c;
            return fish;
        }
    }
    return NULL;
}

static void* s_bin_sub(void* a, void* b) {
    if (fopAc_IsActor(a) && fopAcM_GetName(a) == PROC_MG_FISH) {
        mg_fish_class* fish = (mg_fish_class*)a;
        if (fish->mGedouKind == GEDOU_KIND_O_GD_BOTT) {
            return fish;
        }
    }
    return NULL;
}

static void* s_kn_sub(void* a, void* b) {
    if (fopAc_IsActor(a) && fopAcM_GetName(a) == PROC_MG_FISH) {
        mg_fish_class* fish = (mg_fish_class*)a;
        if (fish->mGedouKind == GEDOU_KIND_KN) {
            return fish;
        }
    }
    return NULL;
}

static void* s_ed_sub(void* a, void* b) {
    if (fopAc_IsActor(a) && fopAcM_GetName(a) == PROC_MG_FISH) {
        mg_fish_class* fish = (mg_fish_class*)a;
        if (fish->mGedouKind == GEDOU_KIND_ED) {
            return fish;
        }
    }
    return NULL;
}

static void* s_sy_sub(void* a, void* b) {
    if (fopAc_IsActor(a) && fopAcM_GetName(a) == PROC_MG_FISH) {
        mg_fish_class* fish = (mg_fish_class*)a;
        if (fish->mGedouKind == GEDOU_KIND_SY) {
            return fish;
        }
    }
    return NULL;
}

static void* s_bb_sub(void* a, void* b) {
    if (fopAc_IsActor(a) && fopAcM_GetName(a) == PROC_MG_FISH) {
        mg_fish_class* fish = (mg_fish_class*)a;
        if (fish->mGedouKind == GEDOU_KIND_BB) {
            return fish;
        }
    }
    return NULL;
}

static void* s_bb2_sub(void* a, void* b) {
    if (fopAc_IsActor(a) && fopAcM_GetName(a) == PROC_E_BG) {
        return a;
    }
    return NULL;
}

static void* s_sg_sub(void* a, void* b) {
    if (fopAc_IsActor(a) && fopAcM_GetName(a) == PROC_E_SG) {
        return a;
    }
    return NULL;
}

s32 daMg_Fish_Draw(mg_fish_class* i_this) {
    if (i_this->mGedouKind == GEDOU_KIND_BT ||
        i_this->mGedouKind == GEDOU_KIND_SP ||
        i_this->mGedouKind == GEDOU_KIND_LH ||
        i_this->mGedouKind == GEDOU_KIND_O_GD_BOTT ||
        i_this->mGedouKind == GEDOU_KIND_BB ||
        i_this->mGedouKind == GEDOU_KIND_KN ||
        i_this->mGedouKind == GEDOU_KIND_ED ||
        i_this->mGedouKind == GEDOU_KIND_SY) {
        return 1;
    }

    g_env_light.settingTevStruct(0, &i_this->actor.current.pos, &i_this->actor.tevStr);
    if (i_this->mCaughtType != 0) {
        mg_fish_class* fish;
        if (i_this->mCaughtType == 1) {
            fish = (mg_fish_class*)fpcEx_Search(s_bt_sub, i_this);
            if (fish != NULL) {
                g_env_light.setLightTevColorType_MAJI(fish->mpModel[0], &i_this->actor.tevStr);
                mDoExt_modelUpdateDL(fish->mpModel[0]);
            }
        } else if (i_this->mCaughtType == 2) {
            fish = (mg_fish_class*)fpcEx_Search(s_sp_sub, i_this);
            if (fish != NULL) {
                for (s32 i = 0; i < 3; i++) {
                    g_env_light.setLightTevColorType_MAJI(fish->mpModel[i], &i_this->actor.tevStr);
                    mDoExt_modelUpdateDL(fish->mpModel[i]);
                }
            }
        } else if (i_this->mCaughtType == 3) {
            fish = (mg_fish_class*)fpcEx_Search(s_lh_sub, i_this);
            if (fish != NULL) {
                g_env_light.setLightTevColorType_MAJI(fish->mpModel[0], &i_this->actor.tevStr);
                mDoExt_modelUpdateDL(fish->mpModel[0]);
            }
        } else if (i_this->mCaughtType == 5) {
            fish = (mg_fish_class*)fpcEx_Search(s_bb_sub, i_this);
            if (fish != NULL) {
                g_env_light.setLightTevColorType_MAJI(fish->mpModel[0], &i_this->actor.tevStr);
                mDoExt_modelUpdateDL(fish->mpModel[0]);
            }
        } else if (i_this->mCaughtType == 6) {
            fish = (mg_fish_class*)fpcEx_Search(s_bin_sub, i_this);
            if (fish != NULL) {
                g_env_light.setLightTevColorType_MAJI(fish->mpModel[0], &i_this->actor.tevStr);
                fish->mpBtpAnm->setFrame(2.0f);
                fish->mpBtpAnm->entry(fish->mpModel[0]->getModelData());
                mDoExt_modelUpdateDL(fish->mpModel[0]);
            }
        } else if (i_this->mCaughtType == 7) {
            fish = (mg_fish_class*)fpcEx_Search(s_kn_sub, i_this);
            if (fish != NULL) {
                g_env_light.setLightTevColorType_MAJI(fish->mpModel[0], &i_this->actor.tevStr);
                mDoExt_modelUpdateDL(fish->mpModel[0]);
            }
        } else if (i_this->mCaughtType == 8) {
            fish = (mg_fish_class*)fpcEx_Search(s_ed_sub, i_this);
            if (fish != NULL) {
                g_env_light.setLightTevColorType_MAJI(fish->mpModel[0], &i_this->actor.tevStr);
                mDoExt_modelUpdateDL(fish->mpModel[0]);
            }
        } else if (i_this->mCaughtType == 9) {
            fish = (mg_fish_class*)fpcEx_Search(s_sy_sub, i_this);
            if (fish != NULL) {
                g_env_light.setLightTevColorType_MAJI(fish->mpModel[0], &i_this->actor.tevStr);
                mDoExt_modelUpdateDL(fish->mpModel[0]);
            }
        }
    } else {
        if (i_this->field_0x5ec > 10000.0f) {
            return 1;
        }

        J3DModel* i_model = i_this->mpMorf->getModel();
        g_env_light.setLightTevColorType_MAJI(i_model, &i_this->actor.tevStr);
        if (i_this->mpBrkAnm != NULL) {
            i_this->mpBrkAnm->entry(i_model->getModelData());
        }
        i_this->mpMorf->entryDL();
        if (i_this->field_0xc3c == 0 && i_this->mCurAction == ACTION_MG_FISH_MF_JUMP &&
            i_this->mActionPhase >= 10) {
            cXyz pos;
            pos.set(i_this->actor.current.pos.x, 50.0f + i_this->actor.current.pos.y,
                i_this->actor.current.pos.z);
            i_this->mShadowId = dComIfGd_setShadow(i_this->mShadowId, 1, i_model, &pos,
                600.0f * i_this->mJointScale, 0.0f,
                i_this->actor.current.pos.y, i_this->mAcch.m_ground_h, i_this->mAcch.m_gnd,
                &i_this->actor.tevStr, 0, 1.0f, &dDlst_shadowControl_c::mSimpleTexObj);
        }
        if (i_this->mKind2 == 3) {
            _GXColor color;
            color.r = 0x32;
            color.g = 0x2d;
            color.b = 0x14;
            color.a = 0xff;
            i_this->mLineMat.update(5, color, &i_this->actor.tevStr);
            dComIfGd_set3DlineMat(&i_this->mLineMat);
        }
    }
    return 1;
}

static void get_surface_y(mg_fish_class* i_this, cXyz* param_2) {
    dBgS_ObjGndChk_Spl gndChk;
    cXyz pos = *param_2;
    pos.y += 1500.0f;
    gndChk.SetPos(&pos);
    dBgS& bgs = dComIfG_Bgsp();
    f32 gndCross = bgs.GroundCross(&gndChk);
    if (!(gndCross < -100000.0f)) {
        i_this->mSurfaceY = gndCross;
    }
}

static f32 get_ground_y(mg_fish_class* i_this, cXyz* param_2) {
    dBgS_ObjGndChk gndChk;
    cXyz pos = *param_2;
    pos.y += 1500.0f;
    gndChk.SetPos(&pos);
    return dComIfG_Bgsp().GroundCross(&gndChk);
}

static void mf_swim(mg_fish_class* i_this) {
    cXyz delta;
    switch (i_this->mActionPhase) {
    case 0: {
        i_this->mNextPos.x =  i_this->actor.home.pos.x + cM_rndFX(1000.0f);
        i_this->mNextPos.z =  i_this->actor.home.pos.z + cM_rndFX(1000.0f);
        if (i_this->field_0x624[2] == 0) {
            i_this->mNextPos.y = i_this->mSurfaceY;
            i_this->mActionPhase = 2;
        } else {
            i_this->mNextPos.y = i_this->actor.home.pos.y;
            i_this->mActionPhase = 1;
        }
        i_this->mGroundY = get_ground_y(i_this, &i_this->mNextPos);
        f32 deltaY = i_this->mSurfaceY - i_this->mGroundY;
        if (deltaY > 0.0f && deltaY < 2000.0f) {
            if (i_this->mActionPhase == 1) {
                i_this->mNextPos.y = i_this->mGroundY +
                    (cM_rndFX(0.2f) + 0.5f) *
                        (i_this->mSurfaceY - i_this->mGroundY);
            }
            i_this->field_0x624[0] = cM_rndF(50.0f) + 50.0f;
        } else {
            i_this->mActionPhase = 0;
            break;
        }
    }
    case 1:
        delta = i_this->mNextPos - i_this->actor.current.pos;
        if ((i_this->mBobTimer & 3) == 0) {
            i_this->mMovementYaw = cM_atan2s(delta.x, delta.z);
            f32 latMoveDist = JMAFastSqrt(delta.x * delta.x + delta.z * delta.z);
            i_this->mMovementPitch = -cM_atan2s(delta.y, latMoveDist);
        }
        if (i_this->field_0x624[0] == 0 || delta.abs() < 100.0f) {
            i_this->mActionPhase = 0;
        }
        break;
    case 2:
        delta = i_this->mNextPos - i_this->actor.current.pos;
        if ((i_this->mBobTimer & 3) == 0) {
            i_this->mMovementYaw = cM_atan2s(delta.x, delta.z);
            f32 latMoveDist = JMAFastSqrt(delta.x * delta.x + delta.z * delta.z);
            i_this->mMovementPitch = -cM_atan2s(delta.y, latMoveDist);
        }
        if (i_this->actor.current.pos.y > i_this->mSurfaceY - 10.0f) {
            sibuki_set(i_this, 1.5f, i_this->field_0x638);
            i_this->mSound.startCreatureSound(Z2SE_AL_FISH_JUMP_UP_S, 0, -1);
            i_this->actor.current.angle.x = 0x2000;
            i_this->mMovementYaw = 0x2000;
            i_this->mActionPhase = 0;
            i_this->field_0x624[2] = cM_rndF(1000.0f) + 1000.0f;
        }
        break;
    }

    cLib_addCalc2(&i_this->actor.speedF, 1.0f, 1.0f, 0.1f);
    i_this->mMaxStep = 0x400;
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 2, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 2, i_this->mMaxStep);
}

static s32 pos_bg_check(cXyz* param_1, cXyz* param_2) {
    dBgS_LinChk dStack_80;
    dStack_80.Set(param_1, param_2, NULL);
    dBgS& foo = dComIfG_Bgsp();
    s32 iVar1 = foo.LineCross(&dStack_80);
    if (iVar1 != 0) {
        return 1;
    } else {
        return 0;
    }
}

static s16 path_search(mg_fish_class* i_this) {
    f32 maxSpeed = 500.0f;

    s32 visited[50];
    for (s32 i = 0; i < 50; i++) {
        visited[i] = 0;
    }

    for (s16 i = 0; i < 50; i++) {
        if (swim_path[i].index >= 0 && visited[i] == 0) {
            cXyz pos_offset;
            cXyz path_pos = swim_path[i].pos;
            pos_offset = i_this->actor.current.pos - path_pos;
            f32 speedMagnitude = pos_offset.abs();
            if (speedMagnitude < maxSpeed) {
                if (pos_bg_check(&i_this->actor.current.pos, &path_pos)) {
                    visited[i] = 1;
                } else {
                    return i;
                }
            }
        } else {
            i = -1;
            maxSpeed += 300.0f;
            if (maxSpeed > 7000.0f) {
                return -1;
            }
        }
    }
    return -1;
}

static void mf_swim_p(mg_fish_class* i_this) {
    f32 targetSpeed = 1.0f;
    switch (i_this->mActionPhase) {
    case 0:
        i_this->mCurSwimStep = path_search(i_this);
        if (i_this->mCurSwimStep < 0) {
            i_this->actor.home.pos = i_this->actor.current.pos;
            i_this->mActionPhase = 10;
            return;
        }

        i_this->mActionPhase = 1;
    case 1:
        i_this->mNextPos.x = swim_path[i_this->mCurSwimStep].pos.x + cM_rndFX(200.0f);
        i_this->mNextPos.y = swim_path[i_this->mCurSwimStep].pos.y + cM_rndFX(40.0f);
        i_this->mNextPos.z = swim_path[i_this->mCurSwimStep].pos.z + cM_rndFX(200.0f);

        i_this->mActionPhase = 2;
    case 2: {
        cXyz delta = i_this->mNextPos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(delta.x, delta.z);
        f32 latMoveDist = JMAFastSqrt(delta.x * delta.x + delta.z * delta.z);
        i_this->mMovementPitch = -cM_atan2s(delta.y, latMoveDist);

        f32 distToNextPos = delta.abs();
        if (distToNextPos < 100.0f) {
            i_this->mActionPhase = 1;
            i_this->mCurSwimStep += 1;
            i_this->mMaxStep = 0;
            if (swim_path[i_this->mCurSwimStep].index < 0) {
                i_this->mCurSwimStep = 0;
            }
        } else if (i_this->mAcch.ChkWallHit()) {
            i_this->mActionPhase = 0;
        }
        break;
    }
    case 10:
        targetSpeed = 0.0f;
        break;
    }

    cLib_addCalc2(&i_this->actor.speedF, targetSpeed, 1.0f, 0.1f);
    cLib_addCalcAngleS2(&i_this->mMaxStep, 0x400, 4, 0x20);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 2, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 2, i_this->mMaxStep);
}

static void mf_swim_s(mg_fish_class* i_this) {
    cXyz delta;

    switch (i_this->mActionPhase) {
    case 0:
        i_this->mNextPos.x = i_this->actor.home.pos.x + cM_rndFX(1000.0f);
        i_this->mNextPos.z = i_this->actor.home.pos.z + cM_rndFX(1000.0f);
        if (i_this->field_0x5ec < 10000.0f && i_this->field_0x624[2] == 0) {
            i_this->mNextPos.y = i_this->mSurfaceY;
            i_this->mActionPhase = 2;
        } else {
            i_this->mNextPos.y = i_this->actor.home.pos.y;
            i_this->mActionPhase = 1;
        }
        i_this->mGroundY = get_ground_y(i_this, &i_this->mNextPos);
        if (i_this->mSurfaceY - i_this->mGroundY > 400.0f) {
            i_this->mGroundY = i_this->mSurfaceY - 400.0f;
        }
        if (i_this->mSurfaceY - i_this->mGroundY > 0.0f &&
            i_this->mSurfaceY - i_this->mGroundY < 2000.0f) {
            if (i_this->mActionPhase == 1) {
                i_this->mNextPos.y = i_this->mGroundY + (i_this->mSurfaceY - i_this->mGroundY) *
                    (cM_rndFX(0.2f) + 0.5f);
            }
            i_this->field_0x624[0] = cM_rndF(50.0f) + 50.0f;
            // intentional fall-through
        } else {
            i_this->mActionPhase = 0;
            break;
        }
    case 1:
        delta = i_this->mNextPos - i_this->actor.current.pos;
        if ((i_this->mBobTimer & 3) == 0) {
            i_this->mMovementYaw = cM_atan2s(delta.x, delta.z);
            f32 latMoveDist = JMAFastSqrt(delta.x * delta.x + delta.z * delta.z);
            i_this->mMovementPitch = -cM_atan2s(delta.y, latMoveDist);
        }
        if (i_this->field_0x624[0] == 0 || delta.abs() < 100.0f) {
            i_this->mActionPhase = 0;
        }
        break;
    case 2:
        delta = i_this->mNextPos - i_this->actor.current.pos;
        if ((i_this->mBobTimer & 3) == 0) {
            i_this->mMovementYaw = cM_atan2s(delta.x, delta.z);
            f32 latMoveDist = JMAFastSqrt(delta.x * delta.x + delta.z * delta.z);
            i_this->mMovementPitch = -cM_atan2s(delta.y, latMoveDist);
        }
        if (i_this->actor.current.pos.y > i_this->mSurfaceY - 10.0f) {
            sibuki_set(i_this, 1.5f, i_this->field_0x638);
            i_this->mSound.startCreatureSound(Z2SE_AL_FISH_JUMP_UP_S, 0, -1);
            i_this->actor.current.angle.x = 0x2000;
            i_this->mMovementYaw = 0x2000;
            i_this->mActionPhase = 0;
            i_this->field_0x624[2] = cM_rndF(1000.0f) + 1000.0f;
        }
        break;
    }

    cLib_addCalc2(&i_this->actor.speedF, 1.0f, 1.0f, 0.1f);
    i_this->mMaxStep = 0x400;
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 2, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 2, i_this->mMaxStep);

    if (i_this->field_0x632 == 0 &&
        i_this->mGedouKind == GEDOU_KIND_BG &&
        i_this->field_0x5ec > 10000.0f &&
        (i_this->mBobTimer & 0xF) == 0) {
        if (fpcEx_Search(s_sg_sub, i_this) != NULL ||
            fpcEx_Search(s_bb2_sub, i_this) != NULL) {
            if (fpcEx_Search(s_bb2_sub, i_this) != NULL &&
                fpcEx_Search(s_bb_sub, i_this) == NULL) {
                fopAcM_create(PROC_MG_FISH, 0x17, &i_this->actor.current.pos,
                    fopAcM_GetRoomNo(&i_this->actor), NULL, NULL, -1);
            }
        } else {
            fopAcM_delete(&i_this->actor);
        }
    }
}

static void mf_stay(mg_fish_class* i_this) {
    f32 f31 = 0.0f;
    f32 f30 = 0.01f;
    switch (i_this->mActionPhase) {
    case 0:
        i_this->mNextPos = i_this->actor.home.pos;
        i_this->mGroundY = get_ground_y(i_this, &i_this->mNextPos);
        i_this->mActionPhase += 1;
        i_this->mMaxStep = 0;
        break;
    case 1: {
        f31 = 0.4f;
        f30 = 0.02f;
        cXyz delta = i_this->mNextPos - i_this->actor.current.pos;
        f32 fVar6 = delta.abs();
        if (fVar6 < 60.0f) {
            i_this->mActionPhase = 2;
            break;
        }
        i_this->mMovementYaw = cM_atan2s(delta.x, delta.z);
        f32 mag = JMAFastSqrt(delta.x * delta.x + delta.z * delta.z);
        i_this->mMovementPitch = -cM_atan2s(delta.y, mag);
        break;
    }
    case 2:
        cLib_addCalcAngleS2(&i_this->mMovementPitch, 0, 16, 0xa0);
        if (i_this->field_0x624[0] == 1 && i_this->field_0xc44 < 10) {
            i_this->mActionPhase = 0;
        }
        break;
    }

    cLib_addCalc2(&i_this->actor.speedF, f31, 1.0f, f30);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 2, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 2, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->mMaxStep, 0x300, 1, 0x20);
}

static void mf_away(mg_fish_class* i_this) {
    cXyz offsetToNext;

    switch (i_this->mActionPhase) {
    case -1:
        dKy_Sound_set(i_this->actor.current.pos, 30.0f * i_this->mJointScale,
            fopAcM_GetID(i_this), 5);
        i_this->field_0x624[1] = 0;
        i_this->field_0x624[0] = 0;
        i_this->mActionPhase = 0;
    case 0:
        mouth_close(i_this);
        mDoMtx_YrotS(*calc_mtx, i_this->mYawToPlayer + 0x8000 + (s16)cM_rndFX(14000.0f));
        offsetToNext.x = 0.0f;
        offsetToNext.y = 0.0f;
        offsetToNext.z = cM_rndF(150.0f) + 200.0f;
        MtxPosition(&offsetToNext, &i_this->mNextPos);
        i_this->mNextPos += i_this->actor.current.pos;
        i_this->mGroundY = get_ground_y(i_this, &i_this->mNextPos);
        if (i_this->mSurfaceY - i_this->mGroundY > 400.0f) {
            i_this->mGroundY = i_this->mSurfaceY - 400.0f;
        }
        if (i_this->mSurfaceY - i_this->mGroundY > 0.0f &&
            i_this->mSurfaceY - i_this->mGroundY < 2000.0f) {
            i_this->mNextPos.y = i_this->mGroundY + (i_this->mSurfaceY - i_this->mGroundY) *
                (cM_rndF(0.2f) + 0.1f);
            if (i_this->mGedouKind >= GEDOU_KIND_BG) {
                i_this->field_0x624[0] = cM_rndF(50.0f) + 50.0f;
                i_this->mActionPhase += 1;
            } else {
                if (i_this->field_0x624[0] == 0) {
                    i_this->field_0x624[0] = cM_rndF(200.0f) + 500.0f;
                }
                i_this->mActionPhase = 5;
                break;
            }
        } else {
            break;
        }
    case 1:
        offsetToNext = i_this->mNextPos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(offsetToNext.x, offsetToNext.z);
        i_this->mMovementPitch =
            -cM_atan2s(offsetToNext.y, JMAFastSqrt(offsetToNext.x * offsetToNext.x + offsetToNext.z * offsetToNext.z));
        if (i_this->field_0x624[0] == 0 || offsetToNext.abs() < 100.0f) {
            if (pl_check(i_this, 500.0f) == 0) {
                i_this->mCurAction = ACTION_MG_FISH_MF_SWIM_S;
                i_this->mActionPhase = 0;
                i_this->field_0x624[0] = cM_rndF(100.0f) + 100.0f;
                i_this->field_0x62e = cM_rndF(100.0f) + 200.0f;
            } else {
                i_this->mActionPhase = 0;
            }
        }
        break;
    case 5:
        if (i_this->mGedouKind < GEDOU_KIND_BG) {
            if ((i_this->mAcch.m_flags & (1 << 4)) != 0) {
                if (i_this->field_0x624[1] == 0) {
                    i_this->mActionPhase = 0;
                    i_this->field_0x624[1] = 40;
                    break;
                }
            }
        }

        offsetToNext = i_this->mNextPos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(offsetToNext.x, offsetToNext.z);
        i_this->mMovementPitch =
            -cM_atan2s(offsetToNext.y, JMAFastSqrt(offsetToNext.x * offsetToNext.x + offsetToNext.z * offsetToNext.z));
        if (i_this->field_0x624[0] == 0 || offsetToNext.abs() < 100.0f) {
            i_this->mCurAction = ACTION_MG_FISH_MF_STAY;
            i_this->mActionPhase = 2;
            i_this->field_0x624[0] = cM_rndF(1000.0f) + 2000.0f;
            i_this->field_0x62e = cM_rndF(100.0f) + 200.0f;
        }
        break;
    }

    cLib_addCalc2(&i_this->actor.speedF, 1.5f, 1.0f,
        0.3f);
    i_this->mMaxStep = 0x800;
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 2, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 2,i_this-> mMaxStep);
}

static void ri_swim(mg_fish_class* i_this) {
    cXyz offsetToNext;

    f32 target = 0.0f;
    f32 maxStep = 0.01f;
    switch (i_this->mActionPhase) {
    case 0: {
        s16 foo = cM_rndFX(8000.0f);
        foo += i_this->actor.shape_angle.y + 0x8000;
        mDoMtx_YrotS(*calc_mtx, foo);
        offsetToNext.x = 0.0f;
        offsetToNext.y = 0.0f;
        offsetToNext.z = cM_rndF(150.0f) + 300.0f;
        MtxPosition(&offsetToNext, &i_this->mNextPos);
        i_this->mNextPos += i_this->actor.current.pos;
        i_this->mNextPos.y = i_this->actor.home.pos.y;
        i_this->mGroundY = get_ground_y(i_this, &i_this->mNextPos);
        f32 waterDepth = i_this->mSurfaceY - i_this->mGroundY;
        if (waterDepth > 0.0f &&
            waterDepth < 2000.0f) {
            i_this->mNextPos.y = i_this->mGroundY;
            i_this->mActionPhase += 1;
        } else {
            break;
        }
        i_this->mMaxStep = 0;
    }
    case 1:
        target = 0.25f;
        maxStep = 0.01f;
        offsetToNext = i_this->mNextPos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(offsetToNext.x, offsetToNext.z);
        i_this->mMovementPitch =
            -cM_atan2s(offsetToNext.y, JMAFastSqrt(offsetToNext.x * offsetToNext.x + offsetToNext.z * offsetToNext.z));
        offsetToNext.y = 0.0f;
        if (offsetToNext.abs() < 100.0f) {
            i_this->mActionPhase = 2;
        } else {
            break;
        }
        i_this->field_0x624[0] = cM_rndF(200.0f) + 300.0f;
        break;
    case 2:
        cLib_addCalcAngleS2(&i_this->mMovementPitch, 0, 0x20, 0x50);
        if (i_this->field_0x624[0] == 0) {
            mDoMtx_YrotS(*calc_mtx, cM_rndF(65536.0f));
            offsetToNext.x = 0.0f;
            offsetToNext.y = 0.0f;
            offsetToNext.z = cM_rndF(150.0f) + 150.0f;
            MtxPosition(&offsetToNext, &i_this->mNextPos);
            i_this->mNextPos += i_this->actor.home.pos;
            i_this->mGroundY = get_ground_y(i_this, &i_this->mNextPos);
            f32 waterDepth = i_this->mSurfaceY - i_this->mGroundY;
            if (waterDepth > 0.0f &&
                waterDepth < 2000.0f) {
                i_this->mNextPos.y = i_this->mSurfaceY;
                i_this->mActionPhase = 3;
                i_this->mMaxStep = 0;
            }
        }
        break;
    case 3:
        target = 0.25f;
        offsetToNext = i_this->mNextPos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(offsetToNext.x, offsetToNext.z);
        i_this->mMovementPitch = -cM_atan2s(offsetToNext.y,
            JMAFastSqrt(offsetToNext.x * offsetToNext.x + offsetToNext.z * offsetToNext.z));
        offsetToNext.y = 0.0f;
        if (i_this->actor.current.pos.y >= i_this->mSurfaceY + i_this->mJointScale * -30.0f) {
            if (offsetToNext.abs() < 100.0f) {
                i_this->mActionPhase = 4;
                i_this->field_0x624[0] = cM_rndF(50.0f) + 150.0f;
                i_this->actor.speedF = 0.0f;
                i_this->field_0x5e0 = i_this->actor.speed;
                target = 0.0f;
            } else {
                break;
            }
        } else {
            break;
        }
    case 4:
        cLib_addCalc2(&i_this->actor.current.pos.x, i_this->mNextPos.x, 0.05f,
            fabsf(i_this->field_0x5e0.x));
        cLib_addCalc2(&i_this->actor.current.pos.y,
            i_this->mSurfaceY + i_this->mJointScale * -10.0f, 0.05f,
            fabsf(i_this->field_0x5e0.y));
        cLib_addCalc2(&i_this->actor.current.pos.z, i_this->mNextPos.z, 0.05f,
            fabsf(i_this->field_0x5e0.z));
        cLib_addCalcAngleS2(&i_this->mMovementPitch, -0xA00, 0x20, 0x50);
        if (i_this->field_0x624[0] == 0) {
            i_this->mActionPhase = 5;
            i_this->field_0x624[0] = cM_rndF(20.0f) + 50.0f;
        }
        break;
    case 5:
        target = -0.1f;
        maxStep = 0.002f;
        cLib_addCalcAngleS2(&i_this->mMovementPitch, -0xA00, 0x20, 0x50);
        if (i_this->field_0x624[0] != 0) {
            break;
        }
        i_this->mActionPhase = 0;
        break;
    }
    cLib_addCalc2(&i_this->actor.speedF, target, 1.0f, maxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 8, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 8, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->mMaxStep, 0x300, 1, 0x10);
    for (s32 i = 0; i < 3; i++) {
        i_this->mJointRotations[i + 3].z =
            cM_ssin(i_this->mBobTimer * 1300 + i * 16000) * 1300.0f;
    }
}

struct JointOffsets {
    f32 * const offsets[11];
};

static void dmcalc(mg_fish_class* i_this) {
    static f32 joint_offset_LM[3] = { 20.0f, 20.0f, 20.0f };
    static f32 joint_offset_BG[2] = { 20.0f, 20.0f };
    static f32 joint_offset_NP[6] = {
        31.0f / 2.0f, 31.0f / 2.0f, 31.0f / 2.0f,
        10.0f, 10.0f, 15.0f,
    };
    static f32 joint_offset_KS[6] = {
        31.0f / 2.0f, 31.0f / 2.0f, 10.0f,
        5.0f, 10.0f, 15.0f,
    };
    static f32 joint_offset_CF[6] = {
        15.0f, 15.0f, 15.0f,
        15.0f, 15.0f, 15.0f,
    };
    static f32 joint_offset_RI[6] = {
        33.0f / 2.0f, 22.0f, 22.0f,
        10.0f, 15.0f, 15.0f,
    };
    static const JointOffsets jointOffsets = {{
        joint_offset_LM, joint_offset_RI, joint_offset_NP,
        joint_offset_CF, joint_offset_KS, joint_offset_BG,
        joint_offset_LM, joint_offset_RI, joint_offset_NP,
        joint_offset_CF, joint_offset_KS,
    }};

    s32 i;
    s16 pitch;
    s16 yaw;
    s16 yawOffset;
    f32 yawOffsetScalar1;
    f32 yawOffsetScalar2;
    f32 yawScalar1;
    s16 yawScalar2;
    cXyz* trans;
    csXyz* rots;
    f32 mult;
    f32 x;
    f32 y;
    f32 z;
    f32 zScalar1;
    f32* jointOffset;
    cXyz sp20;
    cXyz sp14;
    cXyz sp08;

    i_this->mJointTranslations[0] = i_this->actor.current.pos;

    trans = i_this->mJointTranslations + 1;
    rots = i_this->mJointRotations + 1;

    if (i_this->mGedouKind == GEDOU_KIND_RI_1) {
        zScalar1 = 1.0f;
    } else {
        zScalar1 = 1.5f;
    }

    if (i_this->mGedouKind >= GEDOU_KIND_BG) {
        zScalar1 = 7.0f;
    } else if (i_this->actor.speedF > 0.5f) {
            zScalar1 = 3.0f;
    } else if (i_this->mCurAction == ACTION_MG_FISH_MF_LURE_SEARCH ||
        i_this->mCurAction == ACTION_MG_FISH_MF_BAIT_SEARCH) {
        if (i_this->mGedouKind == GEDOU_KIND_CF_1) {
            zScalar1 = 2.75f;
        } else {
            zScalar1 = 3.0f;
        }
    }
    if (i_this->field_0x65d != 0) {
        i_this->field_0x65d--;
        zScalar1 = 30.0f;
    }

    mDoMtx_YrotS(*calc_mtx, i_this->actor.shape_angle.y);
    mDoMtx_XrotM(*calc_mtx, i_this->actor.shape_angle.x);
    mDoMtx_ZrotM(*calc_mtx, i_this->actor.shape_angle.z);
    mDoMtx_YrotM(*calc_mtx, i_this->jointYaws2[0]);

    sp20.x = 0.0f;
    sp20.y = 0.0f;
    sp20.z = -zScalar1 * i_this->mJointScale;

    MtxPosition(&sp20, &sp08);

    yawOffsetScalar1 = 1.0f;
    yawOffsetScalar2 = 500.0f * i_this->field_0x5d4;

    JointOffsets jointOffsets2 = jointOffsets;
    jointOffset = jointOffsets2.offsets[i_this->mGedouKind];

    yawScalar1 = 1.3f;
    if (i_this->mNumJoints != 4 && i_this->mNumJoints < 6) {
        yawScalar1 = 1.5f;
    }

    yawScalar2 = 11000;
    if (i_this->mNumJoints == 4) {
        yawScalar2 = 12000;
    } else if (i_this->mNumJoints < 4) {
        yawScalar2 = 14000;
    }

    mult = 1.0f;
    i = 1;
    while (i <= i_this->mNumJoints) {
        if (i == 1) {
            sp20.z = 0.0f;
        } else {
            sp20.z = i_this->mJointScale * jointOffset[i - 2];
        }
        x = trans[0].x - trans[-1].x + sp08.x * mult;
        y = trans[0].y - trans[-1].y + sp08.y * mult;
        z = trans[0].z - trans[-1].z + sp08.z * mult;
        yaw = (s16)cM_atan2s(x, z);
        pitch = -cM_atan2s(y, JMAFastSqrt(x * x + z * z));
        yawOffset = cM_ssin(i_this->jointYaws2[1] + i * -yawScalar2) * yawOffsetScalar2 * yawOffsetScalar1;
        yaw += yawOffset;
        yawOffsetScalar1 *= yawScalar1;
        cMtx_YrotS(*calc_mtx, yaw);
        cMtx_XrotM(*calc_mtx, pitch);

        MtxPosition(&sp20, &sp14);
        trans[0].x = trans[-1].x + sp14.x;
        trans[0].y = trans[-1].y + sp14.y;
        trans[0].z = trans[-1].z + sp14.z;
        rots[0].x = pitch;
        rots[0].y = yaw;

        i++;
        trans++;
        rots++;
    }
}

static void mf_lure_search(mg_fish_class* i_this) {
    s32 foundLure = false;

    dmg_rod_class* rod = (dmg_rod_class*)fopAcM_SearchByID(i_this->mRodId);
    if (rod == NULL || rod->field_0x1008 != 0) {
        foundLure = true;
    } else if (rod->field_0x100a != 0 || rod->field_0x100d == 0) {
        foundLure = true;
    } else if (rod->action != 4) {
        if (rod->action >= 5 &&
            rod->mg_fish_id == fopAcM_GetID(i_this)) {
            foundLure = false;
        } else {
            foundLure = true;
        }
    }

    if (foundLure) {
        i_this->mActionPhase = 100;
    }

    i_this->mMaxStep = 0x800;
    f32 target = 0.0f;
    f32 maxStep = 0.1f;
    s16 sVar10 = 0;
    cXyz rodSep;
    switch (i_this->mActionPhase) {
    case 0:
        i_this->field_0x624[0] = 0;
        i_this->mActionPhase++;
        i_this->field_0x5f8 = 0;
    case 1:
        i_this->mActionPhase = 2;
    case 2: {
        rodSep = rod->actor.current.pos - i_this->actor.current.pos;
        rodSep.y -= 10.0f;
        i_this->mMovementYaw = cM_atan2s(rodSep.x, rodSep.z);
        i_this->mMovementPitch = -cM_atan2s(rodSep.y, JMAFastSqrt(rodSep.x * rodSep.x + rodSep.z * rodSep.z));
        if (i_this->mMovementPitch < -0x2000) {
            i_this->mMovementPitch = -0x2000;
        }
        f32 rodDist = sqrtf(VECSquareMag(&rodSep));
        if (rod->lure_type != MG_LURE_SP && rod->lure_type != MG_LURE_PO &&
            rodDist > 2.0f * i_this->field_0x5ec) {
            foundLure = true;
        } else {
            if (rodDist > 100.0f) {
                target = 1.5f;
                i_this->field_0x624[0] = cM_rndF(100.0f);
                i_this->field_0x624[1] = cM_rndF(200.0f) + 100.0f;
                if (i_this->mGedouKind == GEDOU_KIND_NP_1 || i_this->mGedouKind == GEDOU_KIND_LM_1) {
                    if (rod->reel_btn_flags != 0) {
                        target = 2.0f;
                    }
                    f32 fVar11;
                    if (i_this->mGedouKind == GEDOU_KIND_NP_1) {
                        fVar11 = 0.1f;
                    } else {
                        fVar11 = 0.025f;
                    }
                    if (rodDist < 1.3f * i_this->field_0x5ec &&
                        (rod->lure_type == MG_LURE_SP || rod->lure_type == MG_LURE_WS) &&
                        rod->reel_btn_flags != 0 &&
                        (i_this->mBobTimer & 0x1f) == 0 &&
                        cM_rndF(1.0f) < fVar11) {
                        sVar10 = 0x5a;
                    }
                }
            } else {
                if (rodDist < 50.0f) {
                    target = -0.1f;
                    i_this->field_0x624[0] = cM_rndF(30.0f) + 10.0f;
                } else {
                    if (i_this->field_0x624[0] == 0) {
                        if (rod->field_0x1009 != 0) {
                            i_this->field_0x624[0] = cM_rndF(30.0f) + 10.0f;
                            i_this->field_0x624[1] = cM_rndF(200.0f) + 100.0f;
                        }
                    } else if (i_this->field_0x624[0] == 1) {
                        f32 limit1 = 0.15f;
                        f32 limit2 = 0.3f;

                        i_this->field_0x5f8++;

                        if (i_this->mGedouKind == GEDOU_KIND_CF_1) {
                            limit1 *= 2.0f;
                            limit2 *= 2.0f;
                        } else if (i_this->mGedouKind == GEDOU_KIND_RI_1 &&
                            rod->lure_type != MG_LURE_FR &&
                            i_this->field_0x5f8 < 10) {
                            limit1 = limit2 = -1.0f;
                        }

                        dScnKy_env_light_c* envlight = dKy_getEnvlight();
                        s32 iVar6 = (s32)(envlight->daytime / 15.0f);
                        if (iVar6 < 8 || iVar6 > 16) {
                            limit1 *= 1.5f;
                            limit2 *= 1.5f;
                        }

                        if (envlight->raincnt > 10) {
                            limit1 *= 1.75f;
                            limit2 *= 1.75f;
                        }

                        if (l_HIO.field_0x18 != 0 || rod->field_0x10a4 != 0) {
                            limit1 = limit2 = 1.0f;
                        }

                        if ((rod->field_0x1009 != 0 && cM_rndF(1.0f) < limit2) ||
                            cM_rndF(1.0f) < limit1) {
                            sVar10 = 0x3c;
                        }
                    }
                }
            }
        }

        if (sVar10 != 0) {
            i_this->mActionPhase = 3;
            i_this->field_0x624[2] = sVar10;
        }

        cLib_addCalc2(&i_this->actor.current.pos.y, rod->actor.current.pos.y - 20.0f,
            0.05f, 2.0f);

        if (i_this->field_0x624[1] == 1) {
            i_this->mActionPhase = 5;
            i_this->field_0x624[0] = cM_rndF(20.0f) + 10.0f;
            if (i_this->field_0x624[2] == 0) {
                break;
            }
        }
        break;
    }
    case 3: {
        if (i_this->field_0x624[2] == 0) {
            foundLure = true;
            break;
        }

        rodSep = rod->actor.current.pos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(rodSep.x, rodSep.z);
        i_this->mMovementPitch = -cM_atan2s(rodSep.y, JMAFastSqrt(rodSep.x * rodSep.x + rodSep.z * rodSep.z));

        target = 3.0f;
        if (rod->reel_btn_flags != 0) {
            target = 5.0f;
        }

        maxStep = 1.0f;

        f32 rodDist = sqrtf(VECSquareMag(&rodSep));
        if (rodDist < i_this->mJointScale * 150.0f) {
            mouth_open(i_this);
        }

        if (rodDist < i_this->mJointScale * 40.0f) {
            if (i_this->mGedouKind == GEDOU_KIND_CF_1) {
                if (rod->lure_type == MG_LURE_SP) {
                    i_this->mRemainingHookTime = cM_rndF(10.0f) + 20.0f;
                } else {
                    i_this->mRemainingHookTime = cM_rndF(12.0f) + 30.0f;
                }
            } else if (rod->lure_type == MG_LURE_SP) {
                if (i_this->mJointScale >= 0.6f) {
                    if (sVar10 != 0) {
                        i_this->mActionPhase = 3;
                        i_this->field_0x624[2] = sVar10;
                    }
                    i_this->mRemainingHookTime = cM_rndF(5.0f) + 10.0f;
                } else if (i_this->mJointScale >= 0.5f) {
                    i_this->mRemainingHookTime = cM_rndF(7.0f) + 15.0f;
                } else {
                    i_this->mRemainingHookTime = cM_rndF(10.0f) + 20.0f;
                }
            } else if (i_this->mJointScale >= 0.6f) {
                i_this->mRemainingHookTime = cM_rndF(10.0f) + 15.0f;
            } else {
                i_this->mRemainingHookTime = cM_rndF(10.0f) + 25.0f;
            }
        } else {
            break;
        }

        if (rod->lure_type == MG_LURE_FR) {
            i_this->mRemainingHookTime = cM_rndF(10.0f) + 40.0f;
        }

        rod->action = 5;
        rod->field_0x10a5 = 2;
        rod->mg_fish_id = fopAcM_GetID(i_this);
        rod->field_0x1006 = cM_rndFX(7000.0f) + 1000.0f;
        i_this->mHookedState = 1;
        i_this->mMovementYaw = cM_rndFX(65536.0f);
        i_this->mMovementPitch = 0x2000;
        i_this->mMaxStep = 0x1000;
        i_this->mActionPhase = 4;
        mouth_close(i_this);

        if (rod->actor.current.pos.y > i_this->mSurfaceY - 15.0f) {
            sibuki_set(i_this, 2.5f, i_this->field_0x638);
            if (i_this->mGedouKind == GEDOU_KIND_RI_1) {
                i_this->mSound.startCreatureSound(Z2SE_AL_DOJOU_EAT_IMPACT, 0, -1);
            } else {
                i_this->mSound.startCreatureSound(Z2SE_AL_FISH_RELEASE, 0, -1);
            }
        }

        rod->field_0x1410 = 0.0f;
        dKy_Sound_set(i_this->actor.current.pos, 40.0f * i_this->mJointScale,
            fopAcM_GetID(i_this), 5);
        rod->vib_timer = 5;
        rod->vibmode = VIBMODE_S_POWER3;

        break;
    }
    case 4: {
        rod->field_0x10a5 = 2;
        target = 2.5f;
        maxStep = 0.5f;
        i_this->mHookedState = 2;
        if (i_this->mRemainingHookTime == 0) {
            if (rod->lure_type != MG_LURE_SP) {
                rod->field_0x10a6 = 30;
                if (rod->field_0x10a7 != 4) {
                    rod->field_0x10a7 = 1;
                }
            }
            i_this->mCurAction = ACTION_MG_FISH_MF_AWAY;
            i_this->mActionPhase = -1;
            return;
        }
        break;
    }
    case 5: {
        target = -0.15f;
        maxStep = 0.01f;
        if (i_this->field_0x624[0] == 0) {
            foundLure = true;
        }
        break;
    }
    case 0x64: // ???
    default:
        break;
    }

    cLib_addCalc2(&i_this->actor.speedF, target, 1.0f, maxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 2, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 2, i_this->mMaxStep);

    if (foundLure) {
        i_this->mCurAction = ACTION_MG_FISH_MF_STAY;
        i_this->mActionPhase = 0;
        i_this->field_0x624[0] = 0;
        i_this->field_0x62e = cM_rndF(1000.0f) + 2000.0f;
        mouth_close(i_this);
    }
}

static void mf_bait_search(mg_fish_class* i_this) {
    s32 foundBait = false;
    fr_class* rod = (fr_class*)fopAcM_SearchByID(i_this->mBaitId);
    if (rod == NULL) {
        i_this->mActionPhase = 100;
        foundBait = true;
    }
    i_this->mMaxStep = 0x800;
    f32 targetSpeed = 0.0f;
    f32 speedMaxStep = 0.1f;
    cXyz offsetToRod;
    switch (i_this->mActionPhase) {
    case 0: {
        i_this->field_0x624[0] = 0;
        i_this->mActionPhase++;
    }
    case 1: {
        i_this->mActionPhase = 2;
    }
    case 2: {
        offsetToRod = rod->current.pos - i_this->actor.current.pos;
        offsetToRod.y = offsetToRod.y - 10.0f;
        i_this->mMovementYaw = cM_atan2s(offsetToRod.x, offsetToRod.z);
        i_this->mMovementPitch = -cM_atan2s(offsetToRod.y,
            JMAFastSqrt(offsetToRod.x * offsetToRod.x + offsetToRod.z * offsetToRod.z));
        if (i_this->mMovementPitch < -0x2000) {
            i_this->mMovementPitch = -0x2000;
        }
        f32 fVar5 = offsetToRod.abs();
        if (fVar5 > 2.0f * i_this->field_0x5ec) {
            foundBait = true;
        } else if (fVar5 > 100.0f) {
            targetSpeed = 0.7f;
            speedMaxStep = 0.05f;
            i_this->field_0x624[0] = cM_rndF(100.0f);
        } else if (fVar5 < 50.0f) {
            targetSpeed = -0.1f;
        } else if (i_this->field_0x624[0] == 0) {
            i_this->field_0x624[0] = cM_rndF(30.0f) + 10.0f;
        } else if (i_this->field_0x624[0] == 1 && cM_rndF(1.0f) < 0.5f) {
            i_this->mActionPhase = 3;
            i_this->field_0x624[3] = 7;
            i_this->field_0x624[2] = 0x3c;
        }
        cLib_addCalc2(&i_this->actor.current.pos.y, rod->current.pos.y - 20.0f,
            0.05f, 2.0f);
        break;
    }
    case 3: {
        if (i_this->field_0x624[2] == 0) {
            foundBait = 1;
            break;
        }

        offsetToRod = rod->current.pos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(offsetToRod.x, offsetToRod.z);
        i_this->mMovementPitch = -cM_atan2s(offsetToRod.y,
            JMAFastSqrt(offsetToRod.x * offsetToRod.x + offsetToRod.z * offsetToRod.z));
        targetSpeed = 3.0f;
        speedMaxStep = 1.0f;
        f32 fVar10 = offsetToRod.abs();
        if (fVar10 < 40.0f * i_this->mJointScale) {
            rod->field_0x5d2 = 0x32;
            rod->field_0x5d4 = 0;
            rod->field_0x5f8 = fopAcM_GetID(i_this);
            fVar10 = cM_rndFX(65536.0f);
            i_this->mMovementYaw = fVar10;
            i_this->mMovementPitch = 0x2000;
            i_this->mMaxStep = 0x1000;
            i_this->mActionPhase = 4;
            mouth_close(i_this);
            sibuki_set(i_this, 2.5f, i_this->field_0x638);
            i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_WATER_S, 0, -1);
            dKy_Sound_set(i_this->actor.current.pos, 40.0f * i_this->mJointScale,
                fopAcM_GetID(i_this), 5);
            i_this->field_0x624[0] = cM_rndF(10.0f) + 10.0f;
        }
        break;
    }
    case 4: {
        targetSpeed = 1.5f;
        speedMaxStep = 0.2f;
        if (i_this->field_0x624[0] == 0) {
            foundBait = true;
        }
    }
    }

    cLib_addCalc2(&i_this->actor.speedF, targetSpeed, 1.0f, speedMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y,
                                    i_this->mMovementYaw, 2, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x,
                                    i_this->mMovementPitch, 2, i_this->mMaxStep);
    if (foundBait) {
        i_this->mCurAction = ACTION_MG_FISH_MF_STAY;
        i_this->mActionPhase = 0;
        i_this->field_0x624[0] = 0;
        targetSpeed = cM_rndF(100.0f);
        i_this->field_0x62e = targetSpeed + 300.0f;
        mouth_close(i_this);
    }
}

static void mf_hit(mg_fish_class* i_this) {
    dmg_rod_class* pvVar5 = (dmg_rod_class*)fopAcM_SearchByID(i_this->mRodId);
    if (pvVar5 == NULL) {
        i_this->mCurAction = ACTION_MG_FISH_MF_STAY;
        i_this->mActionPhase = 0;
        i_this->field_0x62e = cM_rndF(1000.0f) + 2000.0f;
        return;
    }

    i_this->mHookedState = 2;
    pvVar5->field_0x10a5 = 2;

    s32 iVar1 = 0;
    s32 didCatch = false;
    cXyz commonXyz;
    switch (i_this->mActionPhase) {
    case 0: {
        i_this->field_0x624[1] = cM_rndF(100.0f) + 100.0f;
        i_this->mActionPhase = 1;
        i_this->field_0x65b = 0;
    }
    case 1: {
        cMtx_YrotS(*calc_mtx, i_this->mYawToPlayer + 0x8000 + (s16)cM_rndFX(10000.0f));
        commonXyz.x = 0.0f;
        commonXyz.y = 0.0f;
        commonXyz.z = cM_rndF(150.0f) + 200.0f;
        MtxPosition(&commonXyz, &i_this->mNextPos);
        i_this->mNextPos += i_this->actor.current.pos;
        i_this->mGroundY = get_ground_y(i_this, &i_this->mNextPos);
        if (i_this->mSurfaceY - i_this->mGroundY > 0.0f && i_this->mSurfaceY - i_this->mGroundY < 2000.0f) {
            if (i_this->mGedouKind == GEDOU_KIND_CF_1 || i_this->mGedouKind == GEDOU_KIND_RI_1) {
                i_this->mNextPos.y = i_this->mGroundY +
                    (i_this->mSurfaceY - i_this->mGroundY) *
                        (cM_rndF(0.2f) + 0.1f);
            } else {
                i_this->mNextPos.y = i_this->mGroundY +
                    (i_this->mSurfaceY - i_this->mGroundY) *
                        (cM_rndF(0.8f) + 0.1f);
            }
            i_this->field_0x624[0] = cM_rndF(40.0f) + 10.0f;
            dKy_Sound_set(i_this->actor.current.pos, 30.0f * i_this->mJointScale,
                fopAcM_GetID(i_this), 5);
            i_this->mActionPhase = 2;
            if (i_this->field_0x624[1] == 0 &&
                i_this->mDistToPlayer > 400.0f &&
                i_this->field_0x660 > 0.6f)
            {
                f32 threshold = -1.0f;
                if (i_this->mGedouKind == GEDOU_KIND_LM_1) {
                    if (i_this->mJointScale >= 0.6f) {
                        threshold = 0.5f;
                    } else {
                        threshold = 0.25f;
                    }
                } else if (i_this->mGedouKind == GEDOU_KIND_NP_1 || i_this->mGedouKind == GEDOU_KIND_KS_1) {
                    threshold = 0.35f;
                } else if (i_this->mGedouKind == GEDOU_KIND_RI_1) {
                    threshold = 0.1f;
                }
                if (cM_rndF(1.0f) < threshold) {
                    i_this->mCurAction = ACTION_MG_FISH_MF_JUMP;
                    i_this->mActionPhase = 0;
                    mouth_open(i_this);
                }
            }
        } else {
            break;
        }
    }
    case 2: {
        commonXyz = i_this->mNextPos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(commonXyz.x, commonXyz.z);
        i_this->mMovementPitch = -cM_atan2s(commonXyz.y,
            JMAFastSqrt(commonXyz.x * commonXyz.x + commonXyz.z * commonXyz.z));
        if (i_this->field_0x624[0] == 0) {
            i_this->mActionPhase = 1;
        }
        if (cM_ssin(i_this->jointYaws2[1]) > 0.8f) {
            hit_vib_set(i_this);
        }
        break;
    }
    }
    cLib_addCalc2(&i_this->actor.speedF, 1.5f, 1.0f, 0.5f);
    i_this->mMaxStep = 0x800;
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 2, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 2, i_this->mMaxStep);
    if (l_HIO.field_0x19 != 0) {
        didCatch = true;
    }
    if (i_this->mDistToPlayer < 200.0f &&
        i_this->actor.current.pos.y > i_this->mSurfaceY - 50.0f * i_this->mJointScale)
    {
        if ((mDoCPd_c::m_cpadInfo[0].mPressedButtonFlags & 0x100) != 0) {
            i_this->field_0x624[4] = 60;
        }
        if (i_this->field_0x624[4] == 0) {
            if (i_this->mKind2 == 3) {
                i_this->field_0x624[4] = cM_rndF(20.0f) + 50.0f;
                i_this->field_0x624[2] = cM_rndF(15.0f) + 15.0f;
            } else {
                f32 jointScale = i_this->mJointScale;
                if (i_this->mKind2 == 2) {
                    jointScale -= 0.15f;
                }
                if (jointScale < 0.5f) {
                    i_this->field_0x624[4] = cM_rndF(20.0f) + 50.0f;
                    i_this->field_0x624[2] = cM_rndF(15.0f) + 15.0f;
                } else if (jointScale < 0.55f) {
                    i_this->field_0x624[4] = cM_rndF(30.0f) + 40.0f;
                    i_this->field_0x624[2] = cM_rndF(15.0f) + 13.0f;
                } else if (jointScale < 0.6f) {
                    i_this->field_0x624[4] = cM_rndF(40.0f) + 30.0f;
                    i_this->field_0x624[2] = cM_rndF(10.0f) + 12.0f;
                } else if (jointScale < 0.65f) {
                    i_this->field_0x624[4] = cM_rndF(45.0f) + 30.0f;
                    i_this->field_0x624[2] = cM_rndF(9.0f) + 11.0f;
                } else if (jointScale < 0.65f) {
                    i_this->field_0x624[4] = cM_rndF(50.0f) + 30.0f;
                    i_this->field_0x624[2] = cM_rndF(8.0f) + 10.0f;
                } else if (jointScale < 0.7f) {
                    i_this->field_0x624[4] = cM_rndF(50.0f) + 40.0f;
                    i_this->field_0x624[2] = cM_rndF(7.0f) + 9.0f;
                } else {
                    i_this->field_0x624[4] = cM_rndF(50.0f) + 50.0f;
                    i_this->field_0x624[2] = cM_rndF(6.0f) + 8.0f;
                }
            }
        }
        i_this->field_0x65b++;
        if (i_this->field_0x65b == 200) {
            iVar1 = 2;
            pvVar5->field_0xf5c = 0.5f;
        } else if (i_this->field_0x624[2] != 0) {
            pvVar5->field_0x100f = 1;
            if ((mDoCPd_c::m_cpadInfo[0].mPressedButtonFlags & 0x100) != 0) {
                didCatch = true;
            }
        }
    } else {
        if (pvVar5->reel_btn_flags == 0 ||
            pvVar5->field_0x1515 == 0 ||
            pvVar5->field_0xf60 < 100.0f)
        {
            i_this->field_0x65c++;
        } else {
            i_this->field_0x65c = 0;
        }
        if (i_this->field_0x65c == 90) {
            iVar1 = 1;
        }
    }
    if (didCatch) {
        i_this->mCurAction = ACTION_MG_FISH_MF_CATCH;
        i_this->mActionPhase = 0;
        g_dComIfG_gameInfo.play.mVibration.StartShock(4, 1, cXyz(0.0f, 1.0f, 0.0f));
        Z2AudioMgr::getInterface()->changeBgmStatus(4);
        pvVar5->action = 6;
        pvVar5->play_cam_mode = 10;
        pvVar5->play_cam_timer = 0;
        pvVar5->field_0x14c2 = 0;
        pvVar5->play_cam_fovy = 90.0f;
        pvVar5->field_0x146d = 0;
        pvVar5->field_0x10b0 = 0;
        daPy_py_c* player = daPy_getLinkPlayerActorClass();
        player->onFishingRodGetFish();
        if (pvVar5->lure_type == MG_LURE_SP) {
            u8 bVar7 = g_dComIfG_gameInfo.info.mSavedata.mEvent.getEventReg(0xf11f);
            if (bVar7 < 0x1f) {
                bVar7++;
            }
            g_dComIfG_gameInfo.info.mSavedata.mEvent.setEventReg(0xf11f, bVar7);
        }
    } else if (iVar1 != 0) {
        if (iVar1 == 2) {
            pvVar5->field_0x10a7 = 3;
            pvVar5->timers[2] = cM_rndF(13.0f) + 30.0f;
            pvVar5->field_0x10a6 = 1;
        } else {
            pvVar5->field_0x10a7 = 2;
            pvVar5->field_0x10a6 = 0x19;
            pvVar5->timers[2] = 0;
        }
        i_this->mCurAction = ACTION_MG_FISH_MF_AWAY;
        i_this->mActionPhase = -1;
        i_this->mHookedState = 0;
        i_this->field_0x62e = cM_rndF(1500.0f) + 4000.0f;
    }
}

static void mf_jump(mg_fish_class* i_this) {
    cXyz commonXyz1;
    cXyz commonXyz2;

    dmg_rod_class* rod = (dmg_rod_class*)fopAcM_SearchByID(i_this->mRodId);
    if (i_this->mActionPhase < 10) {
        if (rod == NULL) {
            i_this->mCurAction = ACTION_MG_FISH_MF_STAY;
            i_this->mActionPhase = 0;
            return;
        }
        if (i_this->field_0x65a == 0) {
            i_this->mHookedState = 2;
            rod->field_0x10a5 = 2;
            i_this->field_0x62e = cM_rndF(1500.0f) + 4000.0f;
        }
    }
    i_this->mMaxStep = 0x800;
    switch (i_this->mActionPhase) {
    case 0: {
        i_this->mMovementPitch = (s16)cM_rndFX(2000.0f) + -0x2800;
        i_this->mMovementYaw = i_this->mYawToPlayer + 0x8000 + (s16)cM_rndFX(16384.0f);
        i_this->mActionPhase = 1;
    }
    case 1: {
        i_this->actor.speedF = 1.5f;
        if (cM_ssin(i_this->mBobTimer * 7000) > 0.8f) {
            hit_vib_set(i_this);
        }
        if (i_this->actor.current.pos.y > i_this->mSurfaceY) {
            i_this->actor.speedF = 0.0f;
            i_this->field_0x5e0 = i_this->actor.speed;
            i_this->field_0x5e0.y = 15.0f;
            i_this->jointYaws2[2] = cM_rndFX(1000.0f);
            i_this->jointYaws2[3] = cM_rndF(384.0f);
            i_this->mActionPhase = 2;
            i_this->field_0x624[0] = cM_rndF(10.0f) + 23.0f;
            sibuki_set(i_this, 2.0f, i_this->field_0x638);
            i_this->mSound.startCreatureSound(Z2SE_AL_FISH_JUMP_UP, 0, -1);
        } else {
            break;
        }
    }
    case 2: {
        rod->field_0x100f = 2;
        if (i_this->field_0x65a == 0 && (i_this->mBobTimer & 0xf) == 0) {
            hit_vib_set(i_this);
        }
        i_this->actor.current.angle.z = i_this->actor.current.angle.z + i_this->jointYaws2[2];
        i_this->actor.shape_angle.z = i_this->actor.current.angle.z;
        i_this->mMovementPitch = i_this->mMovementPitch + i_this->jointYaws2[3];
        if (i_this->field_0x624[0] == 1 && rod->rod_stick_y <= -0.5f && cM_rndF(1.0f) < 0.35f) {
            i_this->field_0x65a = 1;
            i_this->mHookedState = 0;
        }
        i_this->actor.current.pos += i_this->field_0x5e0;
        for (s32 i = 0; i <= i_this->mNumJoints; i++) {
            i_this->mJointTranslations[i] += i_this->field_0x5e0;
        }
        i_this->field_0x5e0.y = i_this->field_0x5e0.y - 1.0f;
        if (i_this->field_0x65a != 0) {
            rod->field_0x10a6 = 0x19;
            rod->field_0x10a7 = 0;
        }
        if (i_this->field_0x5e0.y < 0.0f && i_this->actor.current.pos.y < i_this->mSurfaceY) {
            if (i_this->field_0x65a != 0) {
                i_this->mCurAction = ACTION_MG_FISH_MF_AWAY;
                i_this->mActionPhase = -1;
            } else {
                i_this->mCurAction = ACTION_MG_FISH_MF_HIT;
                i_this->mActionPhase = 1;
            }
            i_this->field_0x624[1] = 30;
            sibuki_set(i_this, 2.5f, i_this->field_0x638);
            i_this->mSound.startCreatureSound(Z2SE_AL_FISH_JUMP_DOWN, 0, -1);
            i_this->actor.speedF = 0.5f;
            i_this->mMovementPitch = 0;
            i_this->actor.current.angle.z = 0;
            i_this->field_0x65d = 5;
            mouth_close(i_this);
        }
        break;
    }
    case 10: {
        if (i_this->mGedouKind == GEDOU_KIND_KS_2) {
            i_this->mBaitId = fopAcM_createChild(PROC_KYTAG03, fopAcM_GetID(i_this), 0x10000b3,
                &commonXyz2, fopAcM_GetRoomNo(&i_this->actor), NULL, NULL, -1, 0);
        }
        i_this->mActionPhase = 0xb;
    }
    case 11: {
        daPy_py_c::setLookPos(&i_this->actor.current.pos);
        if (i_this->mGedouKind == GEDOU_KIND_KS_2) {
            fopAc_ac_c* actor = fopAcM_SearchByID(i_this->mBaitId);
            if (actor != NULL) {
                actor->current.pos = i_this->actor.current.pos;
            }
        }
        i_this->field_0x659 = 0;
        i_this->mHookedState = 0;
        if (rod != NULL) {
            rod->field_0x10a5 = 0;
        }
        i_this->mMaxStep = 0x600;
        i_this->actor.current.pos += i_this->field_0x5e0;
        i_this->field_0x5e0.y = i_this->field_0x5e0.y - 2.0f;
        if ((i_this->mAcch.m_flags & 0x20) != 0) {
            i_this->actor.current.pos.x = i_this->actor.old.pos.x;
            i_this->actor.current.pos.y = i_this->actor.old.pos.y;
            i_this->actor.current.pos.z = i_this->actor.old.pos.z;
            mDoMtx_YrotS(*calc_mtx, i_this->field_0x5f6);

            if (i_this->field_0x624[2] == 0) {
                commonXyz1.x = cM_rndFX(3.0f);
                commonXyz1.z = cM_rndFX(2.0f) + 6.0f;
            } else {
                commonXyz1.x = cM_rndFX(3.0f);
                commonXyz1.z = cM_rndFX(3.0f);
            }
            commonXyz1.y = cM_rndF(5.0f) + 10.0f;
            MtxPosition(&commonXyz1, &i_this->field_0x5e0);

            if (i_this->actor.shape_angle.z < 0) {
                i_this->actor.shape_angle.z = -0x4000;
            } else {
                i_this->actor.shape_angle.z = 0x4000;
            }

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->actor.current.angle.z = 0x4000;
            } else {
                i_this->actor.current.angle.z = -0x4000;
            }

            i_this->mMovementYaw = i_this->mMovementYaw + (s16)cM_rndFX(13000.0f);

            fopAcM_effSmokeSet1(&i_this->field_0xc34, &i_this->field_0xc38, &i_this->actor.current.pos,
                NULL, 0.6f * i_this->mJointScale, &i_this->actor.tevStr, 1);

            Z2AudioMgr::getInterface()->seStart(Z2SE_OBJ_NIOIMASU_BOUND, (Vec*)&i_this->actor.current,
                0x14, 0,1.0f, 1.0f,
                -1.0f, -1.0f, 0);
        }
        cLib_addCalcAngleS2(&i_this->actor.shape_angle.z, i_this->actor.current.angle.z, 2, 0xb00);
        if (i_this->field_0x5e0.y < 0.0f && i_this->actor.current.pos.y < i_this->mSurfaceY) {
            sibuki_set(i_this, 2.0f, i_this->field_0x638);
            i_this->mSound.startCreatureSound(Z2SE_AL_FISH_JUMP_DOWN, 0, -1);
            i_this->mCurAction = ACTION_MG_FISH_MF_AWAY;
            i_this->mActionPhase = -1;
            i_this->actor.current.angle.z = 0;
            if (i_this->mGedouKind == GEDOU_KIND_KS_2) {
                fopAc_ac_c* actor = fopAcM_SearchByID(i_this->mBaitId);
                if (actor != NULL) {
                    fopAcM_delete(actor);
                }
            }
            mDoMtx_YrotS(*calc_mtx, i_this->mYawToPlayer);
            commonXyz1.x = 0.0f;
            commonXyz1.y = 0.0f;
            commonXyz1.z = 100.0f;
            MtxPosition(&commonXyz1, &commonXyz2);
            commonXyz2.x += i_this->actor.current.pos.x;
            commonXyz2.z += i_this->actor.current.pos.z;
            commonXyz2.y = 10.0f + i_this->mSurfaceY;
            fopAcM_createItem(&commonXyz2, 0, -1, -1, NULL, NULL, 0);
            g_dComIfG_gameInfo.info.mSavedata.mEvent.onEventBit(
                dSv_event_flag_c::saveBitLabels[0x1d6]);
        }
        break;
    }
    }
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 2, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 2, i_this->mMaxStep);

    s16 sVar1 = 7000;
    if (i_this->mJointScale >= 0.7f) {
        sVar1 = 5000;
    } else if (i_this->mJointScale >= 0.6f) {
        sVar1 = 6000;
    } else if (i_this->mJointScale >= 0.5f) {
        sVar1 = 6500;
    }

    s32 numJoints;
    for (s32 i = 0; i <= i_this->mNumJoints; i++) {
        numJoints = i_this->mNumJoints;
        if (numJoints <= 4) {
            i_this->jointYaws1[i] =
                cM_ssin(i_this->mBobTimer * sVar1 + -8500 * i) * 7000.0f;
        } else {
            i_this->jointYaws1[i] =
                cM_ssin(i_this->mBobTimer * (sVar1 - 500) + -7000 * i) * 4000.0f;
        }
    }
}

static void pota_set(mg_fish_class* i_this) {
    static u16 b_id[3] = {
        0x873A,
        0x873B,
        0x873C,
    };
    static u16 h_id[11] = {
        0x8738,
        0x8739,
        0x8739,
        0x8737,
        0x8739,
        0x8736,
        0x8738,
        0x8739,
        0x8739,
        0x8737,
        0x8739,
    };
    static u32 h_jnt[11] = {
        2,
        2,
        2,
        2,
        2,
        1,
        2,
        2,
        2,
        2,
        2,
    };

    if (i_this->mCaughtType == 1) {
        for (s32 i = 0; i < 3; i++) {
            u32 uVar2 = dComIfGp_particle_set(i_this->mEmitterIds[i],
                b_id[i], &i_this->actor.current.pos, NULL, NULL);
            i_this->mEmitterIds[i] = uVar2;
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mEmitterIds[i]);
            if (emitter != NULL) {
                mg_fish_class* found = (mg_fish_class*)fpcEx_Search(s_bt_sub, i_this);
                if (found != NULL) {
                    emitter->setGlobalSRTMatrix(found->mpModel[0]->getBaseTRMtx());
                }
            }
        }
    } else {
        u32 uVar2 = dComIfGp_particle_set(i_this->mEmitterIds[0],
            h_id[i_this->mGedouKind], &i_this->actor.current.pos, NULL, NULL);
        i_this->mEmitterIds[0] = uVar2;
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mEmitterIds[0]);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(i_this->mpMorf->getModel()->getAnmMtx(h_jnt[i_this->mGedouKind]));
        }
    }
}

static void mf_catch(mg_fish_class* i_this) {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (i_this->actor.health == 2) {
        fopAcM_delete(&i_this->actor);
        return;
    }
    dmg_rod_class* rod = (dmg_rod_class*)fopAcM_SearchByID(i_this->mRodId);
    rod->field_0x10a5 = 0;
    i_this->mHookedState = 2;
    i_this->actor.speed.set(0.0f, 0.0f, 0.0f);
    i_this->actor.speedF = 0.0f;
    switch (i_this->mActionPhase) {
    case 0: {
        i_this->mActionPhase = 1;
        mouth_open(i_this);
        i_this->field_0x624[0] = 100;
        i_this->field_0x740 = 0.0f;
        i_this->actor.health = 0;
    }
    case 1: {
        if (i_this->field_0x624[0] == 70) {
            i_this->mSound.startCreatureSound(Z2SE_AL_FISH_CATCH, 0, -1);
            i_this->field_0x624[1] = 50;
            sibuki_set(i_this, 1.5f, i_this->field_0x638);
        }
        if (i_this->field_0x624[0] == 80) {
            i_this->field_0x740 = cM_rndF(2000.0f) + 4000.0f;
        }
        if (i_this->field_0x624[0] == 0 && i_this->actor.health == 1) {
            i_this->actor.health = 0;
            i_this->mActionPhase = 2;
            i_this->field_0x624[0] = 1;
        }
        i_this->actor.current.angle.x = -0x4000;
        break;
    }
    case 2: {
        if (i_this->field_0x624[0] == 0) {
            i_this->mActionPhase = 3;
            i_this->field_0x624[0] = 40;
            player->onFishingRelease();
            rod->actor.health = 1;
            rod->play_cam_mode = 11;
            rod->play_cam_timer = 0;
            rod->field_0x146d = 0;
            i_this->field_0x740 = cM_rndF(1000.0f) + 3000.0f;
        }
        break;
    }
    case 3: {
        i_this->mHookedState = 0;
        if (i_this->field_0x624[0] <= 25) {
            cXyz local_24 = i_this->actor.current.pos;
            local_24.y = i_this->mSurfaceY;
            fopAcM_effHamonSet(&i_this->field_0xc20, &local_24,
                0.5f, 0.1f);
        }
        if (i_this->field_0x624[0] <= 15) {
            cLib_addCalcAngleS2(&i_this->actor.current.angle.x, 0, 8, 0x800);
        }
        if (i_this->field_0x624[0] == 0) {
            i_this->mCurAction = ACTION_MG_FISH_MF_AWAY;
            i_this->mActionPhase = -1;
            mouth_close(i_this);
            sibuki_set(i_this, 1.3f, i_this->field_0x638);
            i_this->mSound.startCreatureSound(Z2SE_AL_FISH_RELEASE, 0, -1);
            i_this->field_0x624[1] = 0;
            i_this->field_0x65d = 5;
            i_this->field_0x62e = cM_rndF(1500.0f) + 4000.0f;
        }
        break;
    }
    default:
        break;
    }

    i_this->actor.current.angle.y = 26000 +
        (s16)(daPy_getLinkPlayerActorClass()->shape_angle.y +
            daPy_getLinkPlayerActorClass()->mBodyAngle.y);
    int iVar1 = i_this->field_0x740 * cM_ssin(i_this->mBobTimer * 2000);
    cLib_addCalc2(&i_this->field_0x740,
        200.0f, 0.05f, 100.0f);

    for (s32 i = 0; i <= i_this->mNumJoints; i++) {
        if (i == 0) {
            i_this->jointYaws1[i] = 0;
        } else {
            i_this->jointYaws1[i] = iVar1;
        }
    }

    if (i_this->field_0x624[1] != 0) {
        pota_set(i_this);
    }
}

static void mf_esa_search(mg_fish_class* i_this) {
    s32 flag1 = 0;
    s32 flag2 = 0;

    fopAc_ac_c* rod_actor = (fopAc_ac_c*)fopAcM_SearchByID(i_this->mRodId);
    dmg_rod_class* rod = (dmg_rod_class*)rod_actor;

    if (rod_actor == NULL) {
        flag1 = 1;
    } else if (rod->field_0x100d == 0) {
        flag1 = 1;
    } else if (rod->action == 5) {
        flag1 = 1;
    }
    if (flag1) {
        i_this->mCurAction = ACTION_MG_FISH_MF_SWIM_S;
        i_this->mActionPhase = 0;
        i_this->field_0x62e = cM_rndF(100.0f) + 300.0f;
        mouth_close(i_this);
        i_this->field_0x659 = 0;
        return;
    }
    i_this->mMaxStep = 0x800;
    f32 fVar9 = 0.0f;
    switch (i_this->mActionPhase) {
    case 0: {
        i_this->field_0x624[0] = 0;
        i_this->mActionPhase++;
    }
    case 1: {
        i_this->mActionPhase = 2;
    }
    case 2: {
        cXyz xyz = rod->actor.current.pos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(xyz.x, xyz.z);
        i_this->mMovementPitch = -cM_atan2s(xyz.y,
            JMAFastSqrt(xyz.x * xyz.x + xyz.z * xyz.z));
        f32 distFromRod = sqrtf(VECSquareMag(&xyz));
        if (distFromRod > i_this->field_0x5ec + 100.0f) {
            flag2 = 1;
        } else if (distFromRod > i_this->mJointScale * 80.0f) {
            fVar9 = 1.5f;
        } else {
            if (distFromRod < i_this->mJointScale * 50.0f) {
                fVar9 = -0.002f - (i_this->mJointScale * 50.0f - distFromRod) * 0.1f;
                if (fVar9 < -0.2f) {
                    fVar9 = -0.2f;
                }
            } else if (i_this->field_0x5ec > 10000.0f ||
                    ((i_this->mBobTimer & 0x3f) == 0 && cM_rndF(1.0f) > 0.5f)) {
                i_this->actor.speedF = 1.0f;
                fVar9 = i_this->actor.speedF;
                i_this->field_0x624[0] = 0;
                i_this->field_0x654 = -(cM_rndF(0.5f) + 0.2f);
            }
        }

        xyz = i_this->field_0x638 - rod->actor.current.pos;
        if (sqrtf(VECSquareMag(&xyz)) < i_this->mJointScale * 14.0f) {
            fVar9 = i_this->field_0x654;
            i_this->mMaxStep = 0;
            if (i_this->field_0x624[0] == 0) {
                rod->mg_fish_id = fopAcM_GetID(i_this);
                f32 fVar10 = 0.5f;
                if (dComIfGs_getFishNum(5) <= 5) {
                    fVar10 = 1.5f;
                }
                if (i_this->mGedouKind != GEDOU_KIND_BG) {
                    if (rod->hook_kind == 1) {
                        fVar10 = 1.0f;
                    }
                    if (rod->esa_kind == 1) {
                        fVar10 *= 1.5f;
                    } else if (rod->esa_kind == 2) {
                        fVar10 *= 2.0f;
                    }
                }
                if (fVar10 > 2.5f) {
                    fVar10 = 2.5f;
                }
                if (cM_rndF(1.0f) < fVar10 * 0.35f ||
                    i_this->field_0x5ec > 10000.0f) {
                    i_this->mActionPhase = 3;
                    i_this->mMovementPitch = i_this->mMovementPitch + 0x2000;
                    i_this->mMovementYaw += (s16)cM_rndFX(32768.0f);
                    rod->field_0x10a5 = fVar10 * (cM_rndF(20.0f) + 15.0f);
                    i_this->field_0x659 = rod->field_0x10a5;
                } else {
                    rod->field_0x10a5 = fVar10 * (cM_rndF(15.0f) + 15.0f);
                    i_this->field_0x659 = rod->field_0x10a5;
                    i_this->field_0x650 = 0.0f;
                    if (rod->hook_kind == 0 && rod->esa_kind == 0) {
                        i_this->field_0x624[0] = cM_rndF(80.0f) + 50.0f;
                    } else {
                        i_this->field_0x624[0] = cM_rndF(20.0f) + 30.0f;
                    }
                }
            }
        }
        if (i_this->field_0x659 == 0) {
            cLib_addCalc2(&i_this->actor.current.pos.y, rod->actor.current.pos.y, 0.05f, 2.0f);
        } else {
            cLib_addCalc2(&i_this->actor.current.pos.y, rod->actor.current.pos.y - 20.0f, 0.05f, 2.0f);
        }
        break;
    }
    case 3: {
        fVar9 = 1.5f;
        i_this->mMaxStep = 0x1000;
        cLib_addCalc2(&rod->field_0x1090, 0.5f, 1.0f, 0.2f);
        if (i_this->field_0x659 == 0) {
            flag2 = 1;
        }
        break;
    }
    }

    cLib_addCalc2(&i_this->actor.speedF, fVar9, 1.0f, 0.1f);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 2, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 2, i_this->mMaxStep);
    if (i_this->field_0x5ec > 10000.0f) {
        cLib_addCalc2(&i_this->actor.current.pos.x, rod->actor.current.pos.x, 0.1f, 50.0f);
        cLib_addCalc2(&i_this->actor.current.pos.z, rod->actor.current.pos.z, 0.1f, 50.0f);
    }
    if (flag2) {
        i_this->mCurAction = ACTION_MG_FISH_MF_SWIM_S;
        i_this->mActionPhase = 0;
        i_this->field_0x62e = cM_rndF(30.0f) + 30.0f;
        mouth_close(i_this);
    }
}

static void mf_esa_hit(mg_fish_class* i_this) {
    dmg_rod_class* rod = (dmg_rod_class*)fopAcM_SearchByID(i_this->mRodId);
    if (rod == NULL || (rod != NULL && rod->action != 5)) {
        i_this->mCurAction = ACTION_MG_FISH_MF_SWIM_S;
        i_this->mActionPhase = 0;
        i_this->field_0x62e = cM_rndF(100.0f) + 100.0f;
        return;
    }

    i_this->field_0x659 = 2;
    rod->field_0x10a5 = 2;
    i_this->field_0x62e = 500;

    cXyz commonXyz;
    switch (i_this->mActionPhase) {
    case 0:
        i_this->mActionPhase = 1;
    case 1:
        mDoMtx_YrotS(*calc_mtx, i_this->mYawToPlayer + 0x8000 + (s16)cM_rndFX(10000.0f));
        commonXyz.x = 0.0f;
        commonXyz.y = 0.0f;
        commonXyz.z = cM_rndF(150.0f) + 200.0f;
        MtxPosition(&commonXyz, &i_this->mNextPos);
        i_this->mNextPos += i_this->actor.current.pos;

        i_this->mGroundY = get_ground_y(i_this, &i_this->mNextPos);
        if (i_this->mSurfaceY - i_this->mGroundY > 300.0f) {
            i_this->mGroundY = i_this->mSurfaceY - 300.0f;
        }

        if (i_this->mSurfaceY - i_this->mGroundY > 0.0f &&
            i_this->mSurfaceY - i_this->mGroundY < 2000.0f) {
            i_this->mNextPos.y = i_this->mGroundY + (i_this->mSurfaceY - i_this->mGroundY) *
                (cM_rndF(0.2f) + 0.1f);
            i_this->field_0x624[0] = cM_rndF(40.0f) + 10.0f;
            dKy_Sound_set(i_this->actor.current.pos, (s32)(30.0f * i_this->mJointScale),
                fopAcM_GetID(i_this), 5);
            i_this->mActionPhase = 2;
        } else {
            break;
        }
    case 2:
        commonXyz = i_this->mNextPos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(commonXyz.x, commonXyz.z);
        i_this->mMovementPitch =
            -cM_atan2s(commonXyz.y,
                JMAFastSqrt(commonXyz.x * commonXyz.x + commonXyz.z * commonXyz.z));
        if (i_this->field_0x624[0] == 0) {
            i_this->mActionPhase = 1;
        }
        if (cM_ssin(i_this->jointYaws2[1]) > 0.8f)
        {
            hit_vib_set(i_this);
        }
    }

    cLib_addCalc2(&i_this->actor.speedF, 1.5f, 1.0f,
        0.5f);
    i_this->mMaxStep = 0x800;
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 2, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 2, i_this->mMaxStep);
}

static s32 mf_esa_catch(mg_fish_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    dmg_rod_class* rod = (dmg_rod_class*)fopAcM_SearchByID(i_this->mRodId);

    if (rod == NULL) {
        i_this->mCurAction = ACTION_MG_FISH_MF_SWIM_S;
        i_this->mActionPhase = 0;
        i_this->field_0x62e = cM_rndF(100.0f) + 100.0f;
        return 1;
    }

    cXyz position;
    s32 flag = 1;

    i_this->field_0x659 = 2;
    rod->field_0x10a5 = 2;

    switch (i_this->mActionPhase) {
    case 0: {
        if (i_this->mCaughtType == 4 || i_this->mCaughtType == 5) {
            i_this->field_0x740 = 3000.0f;
        } else {
            if (i_this->mGedouKind == GEDOU_KIND_BG) {
                i_this->field_0x740 = cM_rndF(1000.0f) + 8000.0f;
            } else if (i_this->mGedouKind == GEDOU_KIND_CF_2) {
                i_this->field_0x740 = cM_rndF(500.0f) + 4000.0f;
            } else if (i_this->mGedouKind == GEDOU_KIND_NP_2) {
                i_this->field_0x740 = cM_rndF(500.0f) + 4000.0f;
            } else {
                i_this->field_0x740 = cM_rndF(700.0f) + 5000.0f;
            }
        }
        i_this->actor.current.angle.x = -0x3e00;
        i_this->field_0x748 = 0.3f;
        if (i_this->field_0x624[0] != 0) {
            break;
        }
        position = i_this->actor.current.pos;
        position.y = i_this->mSurfaceY;
        fopKyM_createWpillar(&position, 0.7f, 0);
        Z2AudioMgr::getInterface()->changeFishingBgm(4);
        i_this->mActionPhase = 1;
        i_this->mBobAmp = 50.0f;
        i_this->mSound.startCreatureSound(Z2SE_AL_FISH_CATCH, 0, -1);
        if (i_this->mCaughtType == 1) {
            i_this->mSound.startCreatureSound(Z2SE_AL_FISHING_BOOTS_GET, 0, -1);
        }
        i_this->field_0x624[1] = 65;
        i_this->field_0x624[2] = 20;
    }
    case 1: {
        if (i_this->field_0x624[2] != 0) {
            flag = 0;
        }
        mDoMtx_YrotS(*calc_mtx, player->shape_angle.y + (s16)(cM_ssin(i_this->mBobTimer * 800) * 1500.0f));
        position.x = 0.0f;
        if (i_this->mCaughtType == 2) {
            position.y = i_this->mBobAmp * cM_ssin(i_this->mBobTimer * 2000) + 180.0f;
        } else {
            position.y = i_this->mBobAmp * cM_ssin(i_this->mBobTimer * 2000) + 160.0f;
        }
        position.z = 58.0f;
        cXyz local_3c;
        MtxPosition(&position, &local_3c);
        local_3c += player->current.pos;
        cLib_addCalc2((float*)&i_this->actor.current, local_3c.x, 0.1f, 15.0f);
        cLib_addCalc2(&i_this->actor.current.pos.y, local_3c.y, 0.1f, 15.0f);
        cLib_addCalc2(&i_this->actor.current.pos.z, local_3c.z, 0.1f, 15.0f);
        i_this->actor.current.angle.y =
            i_this->actor.current.angle.y + i_this->jointYaws2[2];
        if (i_this->mCaughtType == 2) {
            if (g_Counter.mTimer & 0x80) {
                cLib_addCalcAngleS2(&i_this->jointYaws2[2], 1000, 4, 0x28);
            } else {
                cLib_addCalcAngleS2(&i_this->jointYaws2[2], -1000, 4, 0x28);
            }
        } else if (i_this->mCaughtType >= 7) {
            if (g_Counter.mTimer & 0x100) {
                cLib_addCalcAngleS2(&i_this->jointYaws2[2], 200, 4, 8);
            } else {
                cLib_addCalcAngleS2(&i_this->jointYaws2[2], -200, 4, 8);
            }
        } else if (i_this->mBobTimer & 0x100) {
            cLib_addCalcAngleS2(&i_this->jointYaws2[2], 200, 8, 4);
        } else {
            cLib_addCalcAngleS2(&i_this->jointYaws2[2], -200, 8, 4);
        }
        i_this->actor.current.angle.x = -0x3e00;
    }
    }
    if (i_this->mCaughtType != 0) {
        if (i_this->mCaughtType == 3) {
            i_this->field_0x74c = i_this->field_0x748 * cM_ssin(i_this->mBobTimer * 2000);
            cLib_addCalc2(&i_this->field_0x748, 0.05f, 0.1f, 0.005f);
        } else if (i_this->mCaughtType == 4 || i_this->mCaughtType == 5) {
            cLib_addCalc2(&i_this->field_0x740, 500.0f, 0.05f, 100.0f);
        }
    } else {
        f32 target = 1500.0f;
        s32 jointYaw = 5000;
        if (i_this->mGedouKind == GEDOU_KIND_CF_2) {
            target = 300.0f;
            jointYaw = 2500;
        } else if (i_this->mGedouKind != GEDOU_KIND_BG) {
            target = 500.0f;
            jointYaw = 3000;
        }
        jointYaw = i_this->field_0x740 * cM_ssin(i_this->mBobTimer * jointYaw);
        cLib_addCalc2(&i_this->field_0x740, target, 0.05f, 100.0f);
        for (s32 i = 0; i <= i_this->mNumJoints; i++) {
            if (i == 0) {
                i_this->jointYaws1[i] = 0;
            } else {
                i_this->jointYaws1[i] = jointYaw;
            }
        }
    }
    cLib_addCalc2(&i_this->mBobAmp, 5.0f, 0.05f, 1.0f);
    i_this->actor.speedF = 0.0f;
    if (i_this->field_0x624[1] != 0) {
        pota_set(i_this);
    }
    return flag;
}

static void mf_aqua(mg_fish_class* i_this) {
    f32 targetSpeed = 0.0f;
    switch (i_this->mActionPhase) {
    case 0:
        i_this->mNextPos.z = cM_rndFX(190.0f) + 70.0f;
        i_this->mNextPos.x = cM_rndFX(30.0f) + -720.0f;
        i_this->mNextPos.y = cM_rndF(100.0f) + 70.0f;
        i_this->mActionPhase = 1;
        i_this->field_0x624[0] = cM_rndF(100.0f) + 100.0f;
        i_this->mMaxStep = 0;
    case 1: {
        targetSpeed = 0.4f;
        cXyz local_44 = i_this->mNextPos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(local_44.x, local_44.z);
        f32 fVar7 = JMAFastSqrt(local_44.x * local_44.x + local_44.z * local_44.z);
        i_this->mMovementPitch = -cM_atan2s(local_44.y, fVar7);
        if (i_this->mMovementPitch > 0xc00) {
            i_this->mMovementPitch = 0xc00;
        } else if (i_this->mMovementPitch < -0xc00) {
            i_this->mMovementPitch = -0xc00;
        }
        if (i_this->field_0x624[0] == 0 || sqrtf(VECSquareMag(&local_44)) < 50.0f) {
            if (cM_rndF(1.0f) < 0.2f && i_this->mGedouKind != GEDOU_KIND_BG) {
                i_this->mActionPhase = 2;
                i_this->field_0x624[0] = cM_rndF(100.0f) + 70.0f;
            } else {
                i_this->mActionPhase = 0;
            }
        }
        break;
    }
    case 2:
        cLib_addCalcAngleS2(&i_this->mMovementPitch, 0, 0x10, 100);
        if (i_this->field_0x624[0] == 0) {
            i_this->mActionPhase = 0;
        }
    }

    cLib_addCalc2(&i_this->actor.speedF, targetSpeed, 0.05f, 0.004f);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 8, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 8, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->mMaxStep, 0x400, 1, 0x20);
}


static void ri_aqua(mg_fish_class* i_this) {
    // Two different vectors in Debug, but this messes up the stack for GZ2E01.
    // cXyz commonXyz2;
    cXyz commonXyz;
    f32 targetSpeed = 0.0f;
    f32 speedMaxStep = NREG_F(0) + 0.01f;
    switch (i_this->mActionPhase) {
    case 0: {
        if (i_this->actor.current.pos.z < 70.0f) {
            i_this->mNextPos.z = 120.0f + cM_rndF(140.0f);
        } else {
            i_this->mNextPos.z = 70.0f - (cM_rndF(140.0f) + 50.0f);
        }
        i_this->mNextPos.x = cM_rndFX(30.0f) + -720.0f;
        commonXyz.x = -30.0f - i_this->mNextPos.z;
        commonXyz.z = 220.0f - i_this->mNextPos.z;
        if (fabsf(commonXyz.x) < 70.0f || fabsf(commonXyz.z) < 50.0f) {
            i_this->mNextPos.y = i_this->mGroundY + (15.0f + hREG_F(7));
        } else {
            i_this->mNextPos.y = i_this->mGroundY + hREG_F(6);
        }
        i_this->mActionPhase++;
        i_this->mMaxStep = 0;
    }
    case 1: {
        targetSpeed = NREG_F(1) + 0.25f;
        speedMaxStep = NREG_F(2) + 0.01f;
        commonXyz = i_this->mNextPos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(commonXyz.x, commonXyz.z);
        i_this->mMovementPitch = -cM_atan2s(
            commonXyz.y, JMAFastSqrt(commonXyz.x * commonXyz.x + commonXyz.z * commonXyz.z));
        if (i_this->actor.current.pos.y < i_this->mNextPos.y + (15.0f + hREG_F(8))) {
            i_this->mActionPhase = 2;
            i_this->field_0x624[0] = cM_rndF(500.0f) + 1000.0f;
        }

        break;
    }
    case 2: {
        cLib_addCalcAngleS2(&i_this->mMovementPitch, 0, 0x20, 0x50);
        if (i_this->field_0x624[0] == 0) {
            if (i_this->actor.current.pos.z < 70.0f) {
                i_this->mNextPos.z = cM_rndF(150.0f) + 120.0f;
            } else {
                i_this->mNextPos.z = 70.0f - (cM_rndF(150.0f) + 50.0f);
            }
            i_this->mNextPos.x = cM_rndFX(30.0f) + -720.0f;
            i_this->mNextPos.y = i_this->mSurfaceY;
            i_this->mActionPhase = 3;
            i_this->mMaxStep = 0;
        }
        break;
    }
    case 3:
        targetSpeed = NREG_F(1) + 0.25f;
        speedMaxStep = NREG_F(1) + 0.01f;
        commonXyz = i_this->mNextPos - i_this->actor.current.pos;
        i_this->mMovementYaw = cM_atan2s(commonXyz.x, commonXyz.z);
        i_this->mMovementPitch = -cM_atan2s(
            commonXyz.y, JMAFastSqrt(commonXyz.x * commonXyz.x + commonXyz.z * commonXyz.z));
        commonXyz.y = 0.0f;
        if (i_this->actor.current.pos.y >=
                i_this->mSurfaceY + (KREG_F(15) + -30.0f) * i_this->mJointScale &&
            commonXyz.abs() < 100.0f) {
            i_this->mActionPhase = 4;
            i_this->field_0x624[0] = cM_rndF(50.0f) + 150.0f;
            i_this->actor.speedF = 0.0f;
            i_this->field_0x5e0 = i_this->actor.speed;
            targetSpeed = 0.0f;
      } else {
          break;
      }
    case 4:
        cLib_addCalc2(&i_this->actor.current.pos.x, i_this->mNextPos.x, 0.05f,
                      fabsf(i_this->field_0x5e0.x));
        cLib_addCalc2(&i_this->actor.current.pos.y,
                      i_this->mSurfaceY + (KREG_F(11) + -10.0f) * i_this->mJointScale, 0.05f,
                      fabsf(i_this->field_0x5e0.y));
        cLib_addCalc2(&i_this->actor.current.pos.z, i_this->mNextPos.z, 0.05f,
                      fabsf(i_this->field_0x5e0.z));
        cLib_addCalcAngleS2(&i_this->mMovementPitch, -0xa00, 0x20, 0x50);
        if (i_this->field_0x624[0] == 0) {
            i_this->mActionPhase = 5;
            i_this->field_0x624[0] = cM_rndF(20.0f) + 50.0f;
        }

        break;
    case 5: {
        targetSpeed = TREG_F(4) + -0.1f;
        speedMaxStep = 0.002f;
        cLib_addCalcAngleS2(&i_this->mMovementPitch, -0xa00, 0x20, 0x50);
        if (i_this->field_0x624[0] == 0) {
            i_this->mActionPhase = 0;
        }

        break;
    }
    default:
        break;
    }

    cLib_addCalc2(&i_this->actor.speedF, targetSpeed, 0.1f, speedMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y, i_this->mMovementYaw, 8, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x, i_this->mMovementPitch, 8, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->mMaxStep, 0x300, 1, 0x10);
    for (s32 i = 0; i < 3; i++) {
        i_this->mJointRotations[i + 3].z =
            cM_ssin(i_this->mBobTimer * (YREG_S(0) + 1300) + i * (YREG_S(1) + 16000)) *
                (YREG_F(2) + 1300.0f);
    }
}

static void mf_aqua_tilt(mg_fish_class* i_this) {
    float targetSpeed = 0.0f;

    switch (i_this->mActionPhase) {
    case 0: {
        i_this->mNextPos.z = cM_rndFX(180.0f) + 70.0f;
        i_this->mNextPos.x = cM_rndFX(30.0f) + -720.0f;
        i_this->mNextPos.y = cM_rndF(100.0f) + 70.0f;
        i_this->mActionPhase = 1;
        i_this->field_0x624[0] = cM_rndF(20.0f) + 20.0f;
    }
    case 1: {
        targetSpeed = 1.2f;
        cXyz movement = i_this->mNextPos - i_this->actor.current.pos;

        i_this->mMovementYaw = cM_atan2s(movement.x, movement.z);
        i_this->mMovementPitch = -cM_atan2s(movement.y,
            JMAFastSqrt(movement.x * movement.x + movement.z * movement.z));

        if (i_this->mMovementPitch > 0xc00) {
            i_this->mMovementPitch = 0xc00;
        } else if (i_this->mMovementPitch < -0xc00) {
            i_this->mMovementPitch = 0xf400;
        }

        if (i_this->field_0x624[0] == 0) {
            i_this->mActionPhase = 0;
        }
        break;
    }
    default:
        break;
    }

    cLib_addCalc2(&i_this->actor.speedF, targetSpeed, 0.8f, 0.6f);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.y,
                                    i_this->mMovementYaw, 4, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->actor.current.angle.x,
                                    i_this->mMovementPitch, 4, i_this->mMaxStep);
    cLib_addCalcAngleS2(&i_this->mMaxStep, 0x800, 1, 0x100);
    if (i_this->field_0x624[1] == 0) {
        if (i_this->mGedouKind == GEDOU_KIND_RI_1) {
            i_this->mCurAction = ACTION_MG_FISH_RI_AQUA;
        } else {
            i_this->mCurAction = ACTION_MG_FISH_MF_AQUA;
        }
        i_this->mActionPhase = 0;
    }
}

static void action(mg_fish_class* i_this) {
    i_this->mDistToPlayer = fopAcM_searchPlayerDistanceXZ(&i_this->actor);
    const char* stageName = dComIfGp_getStartStageName();
    if (strcmp(stageName, "F_SP127") == 0) {
        if (i_this->mCurAction == ACTION_MG_FISH_MF_SWIM_S) {
            if (i_this->mDistToPlayer > 2000.0f) {
                return;
            }
        } else if (i_this->mCurAction == ACTION_MG_FISH_MF_STAY) {
            if (i_this->mGedouKind == GEDOU_KIND_RI_1) {
                i_this->mCurAction = ACTION_MG_FISH_RI_SWIM;
                i_this->mActionPhase = 0;
            } else if (i_this->mGedouKind == GEDOU_KIND_CF_1) {
                i_this->mCurAction = ACTION_MG_FISH_MF_SWIM_P;
                i_this->mActionPhase = 0;
            } else if (i_this->mDistToPlayer > 4500.0f) {
                return;
            }
        }
    }
    i_this->mBobTimer += 1;
    for (s32 i = 0; i < 5; i++) {
        if (i_this->field_0x624[i] != 0) {
            i_this->field_0x624[i] -= 1;
        }
    }
    if (i_this->mRemainingHookTime != 0) {
        i_this->mRemainingHookTime -= 1;
    }
    if (i_this->field_0x632 != 0) {
        i_this->field_0x632 -= 1;
    }
    i_this->field_0x5d4 = i_this->actor.speedF;
    if (i_this->field_0x5d4 > 2.7f) {
        i_this->field_0x5d4 = 2.7f;
    }
    if (i_this->mCurAction == ACTION_MG_FISH_RI_AQUA) {
        if (i_this->field_0x5d4 <= TREG_F(2) + 0.1f) {
            i_this->field_0x5d4 = TREG_F(2) + 0.1f;
        }
    } else if (i_this->field_0x5d4 <= TREG_F(2) + 0.15f) {
        i_this->field_0x5d4 = TREG_F(2) + 0.15f;
    }
    i_this->mYawToPlayer = fopAcM_searchPlayerAngleY(&i_this->actor);
    s32 unkFlag1 = false;
    s32 unkFlag2 = false;
    s32 canBeginSearch = false;
    s32 isInUnderwaterState = true;
    s32 unkFlag5 = false;
    s32 unkFlag6 = false;
    s32 unkFlag7 = true;
    s32 isAqua = false;
    switch (i_this->mCurAction) {
    case ACTION_MG_FISH_MF_SWIM:
        mf_swim(i_this);
        unkFlag1 = true;
        unkFlag2 = true;
        canBeginSearch = true;
        unkFlag6 = true;
        break;
    case ACTION_MG_FISH_MF_STAY:
        mf_stay(i_this);
        unkFlag1 = true;
        unkFlag2 = true;
        canBeginSearch = true;
        unkFlag6 = true;
        if (i_this->actor.speedF <= 0.05f) {
            unkFlag7 = false;
        }
        break;
    case ACTION_MG_FISH_MF_AWAY:
        mf_away(i_this);
        unkFlag2 = true;
        break;
    case ACTION_MG_FISH_MF_SWIM_P:
        mf_swim_p(i_this);
        canBeginSearch = true;
        unkFlag6 = true;
        break;
    case ACTION_MG_FISH_MF_SWIM_S:
        mf_swim_s(i_this);
        unkFlag1 = true;
        unkFlag2 = true;
        unkFlag6 = true;
        canBeginSearch = true;
        break;
    case ACTION_MG_FISH_RI_SWIM:
        ri_swim((i_this));
        unkFlag1 = true;
        unkFlag2 = true;
        canBeginSearch = true;
        if (i_this->actor.speedF <= 0.05f) {
            unkFlag7 = false;
        }
        break;
    case ACTION_MG_FISH_MF_AQUA:
        mf_aqua(i_this);
        unkFlag7 = false;
        isAqua = true;
        break;
    case ACTION_MG_FISH_RI_AQUA:
        ri_aqua(i_this);
        unkFlag7 = false;
        isAqua = true;
        break;
    case ACTION_MG_FISH_MF_AQUA_TILT:
        mf_aqua_tilt(i_this);
        unkFlag7 = false;
        isAqua = true;
        break;
    case ACTION_MG_FISH_MF_LURE_SEARCH:
        mf_lure_search(i_this);
        unkFlag2 = true;
        break;
    case ACTION_MG_FISH_MF_BAIT_SEARCH:
        mf_bait_search(i_this);
        unkFlag2 = true;
        break;
    case ACTION_MG_FISH_MF_HIT:
        mf_hit(i_this);
        unkFlag5 = true;
        break;
    case ACTION_MG_FISH_MF_JUMP:
        mf_jump(i_this);
        isInUnderwaterState = false;
        unkFlag5 = true;
        break;
    case ACTION_MG_FISH_MF_CATCH:
        mf_catch(i_this);
        unkFlag6 = true;
        unkFlag7 = false;
        isInUnderwaterState = false;
        break;
    case ACTION_MG_FISH_MF_ESA_SEARCH:
        mf_esa_search(i_this);
        unkFlag2 = true;
        unkFlag6 = true;
        break;
    case ACTION_MG_FISH_MF_ESA_HIT:
        mf_esa_hit(i_this);
        unkFlag5 = true;
        break;
    case ACTION_MG_FISH_MF_ESA_CATCH:
        unkFlag6 = mf_esa_catch(i_this);
        unkFlag7 = true;
        isInUnderwaterState = false;
        break;
    }

    if (unkFlag6 != 0) {
        i_this->mCcCyl.OffCoSetBit();
    } else {
        i_this->mCcCyl.OnCoSetBit();
    }

    if (unkFlag5) {
        f32 jointScale;
        if (i_this->mGedouKind == GEDOU_KIND_LM_1) {
            jointScale = i_this->mJointScale;
            if (jointScale >= 0.7f) {
                jointScale = 0.0001f;
            } else if (jointScale >= 0.65f) {
                jointScale = 0.00015f;
            } else if (jointScale >= 0.6f) {
                jointScale = 0.0002f;
            } else if (jointScale >= 0.55f) {
                jointScale = 0.0003f;
            } else {
                jointScale = 0.0004f;
            }
        } else if (i_this->mGedouKind == GEDOU_KIND_NP_1) {
            jointScale = i_this->mJointScale;
            if (jointScale >= 0.7f) {
                jointScale = 0.00015f;
            } else if (jointScale >= 0.65f) {
                jointScale = 0.0002f;
            } else if (jointScale >= 0.6f) {
                jointScale = 0.00025f;
            } else if (jointScale >= 0.55f) {
                jointScale = 0.0003f;
            } else {
                jointScale = 0.0004f;
            }
        } else if (i_this->mGedouKind == GEDOU_KIND_RI_1) {
            if (i_this->mJointScale >= 0.8f) {
                jointScale = 0.0001f;
            } else {
                jointScale = 0.00015f;
            }
        } else {
            jointScale = 0.0003f;
        }
        cLib_addCalc2(&i_this->field_0x660, 0.5f, 1.0f, jointScale);
    } else {
        cLib_addCalc2(&i_this->field_0x660, 1.0f, 1.0f, 0.002f);
    }

    if (TREG_S(7) == 0 && unkFlag1 && pl_check(i_this, i_this->field_0xc40) != 0) {
        i_this->mCurAction = ACTION_MG_FISH_MF_AWAY;
        i_this->mActionPhase = -1;
    }

    if (unkFlag2 &&
        i_this->mSurfaceY - i_this->mAcch.GetGroundH() < i_this->mJointScale * (TREG_F(5) + 35.0f)) {
        i_this->mCurAction = ACTION_MG_FISH_MF_AWAY;
        i_this->mActionPhase = 1;
        i_this->field_0x624[0] = cM_rndF(30.0f) + 40.0f;
        i_this->mNextPos = i_this->actor.home.pos;
    }

    if (canBeginSearch) {
        if (i_this->field_0x62e == 0 || i_this->field_0x5ec > 10000.0f) {
            s32 unkSint2 = 0;
            if (i_this->mGedouKind >= GEDOU_KIND_BG) {
                unkSint2 = 1;
            }
            i_this->mRodId = search_lure(i_this, unkSint2);
            if (fopAcM_SearchByID(i_this->mRodId) != NULL) {
                if (unkSint2 == 0) {
                    i_this->mCurAction = ACTION_MG_FISH_MF_LURE_SEARCH;
                } else {
                    i_this->mCurAction = ACTION_MG_FISH_MF_ESA_SEARCH;
                }
                i_this->mActionPhase = 0;
            } else {
                if (unkSint2 == 0) {
                    i_this->mBaitId = search_bait(i_this);
                    if (fopAcM_SearchByID(i_this->mBaitId) != NULL) {
                        i_this->mCurAction = ACTION_MG_FISH_MF_BAIT_SEARCH;
                        i_this->mActionPhase = 0;
                    }
                }
            }
        } else {
            i_this->field_0x62e -= 1;
        }
    }

    if (i_this->mCurAction == ACTION_MG_FISH_MF_JUMP && i_this->actor.speedF == 0.0f) {
        s32 unkSint2 = 7000;
        if (i_this->mJointScale >= 0.7f) {
            unkSint2 = 5000;
        } else if (i_this->mJointScale >= 0.6f) {
            unkSint2 = 6000;
        } else if (i_this->mJointScale >= 0.5f) {
            unkSint2 = 6500;
        }
        if (i_this->mNumJoints <= 4) {
            i_this->jointYaws2[0] = cM_ssin(i_this->mBobTimer * unkSint2) *
                (ZREG_F(10) + -9000.0f);
        } else {
            i_this->jointYaws2[0] = cM_ssin(i_this->mBobTimer * (unkSint2 + -500)) *
                (ZREG_F(10) + -9000.0f);
        }
    } else {
        f32 unkFloat0 = -1000.0f * i_this->field_0x5d4 * i_this->field_0x660;
        i_this->jointYaws2[1] += (s16)(i_this->field_0x5d4 * 2500.0f / i_this->mJointScale);
        if (i_this->mGedouKind >= GEDOU_KIND_BG) {
            i_this->jointYaws2[1] += 2000;
        }
        i_this->jointYaws2[0] = unkFloat0 * cM_ssin(i_this->jointYaws2[1]);
    }

    cMtx_YrotS(*calc_mtx, i_this->actor.current.angle.y);
    cMtx_XrotM(*calc_mtx, i_this->actor.current.angle.x);
    cMtx_YrotM(*calc_mtx, i_this->jointYaws2[0]);

    cXyz commonXyz;
    commonXyz.x = 0.0f;
    commonXyz.y = 0.0f;
    commonXyz.z = i_this->actor.speedF * l_HIO.field_0xc * i_this->mJointScale * i_this->field_0x660;
    if (i_this->mGedouKind >= GEDOU_KIND_BG && i_this->mGedouKind != GEDOU_KIND_KS_2) {
        commonXyz.z *= 1.3f;
    }

    f32 unkFloat1 = 10.0f;
    if (i_this->mGedouKind == GEDOU_KIND_CF_1) {
        unkFloat1 = 4.5f;
    } else if (i_this->mGedouKind == GEDOU_KIND_RI_1) {
        unkFloat1 = 5.0f;
    } else if (i_this->mGedouKind == GEDOU_KIND_NP_1 || i_this->mGedouKind == GEDOU_KIND_KS_1) {
        unkFloat1 = 7.0f;
    }
    if (commonXyz.z > unkFloat1) {
        commonXyz.z = unkFloat1;
    }

    MtxPosition(&commonXyz, &i_this->actor.speed);
    i_this->actor.current.pos += i_this->actor.speed;

    i_this->actor.eyePos = i_this->actor.current.pos;
    commonXyz = i_this->actor.eyePos;
    commonXyz.y -= 10.0f;

    i_this->mCcCyl.SetC(commonXyz);

    if (i_this->mCurAction == ACTION_MG_FISH_MF_HIT ||
        i_this->mCurAction >= ACTION_MG_FISH_MF_ESA_HIT) {
        i_this->mCcCyl.SetH(KREG_F(17) + 500.0f);
        if (i_this->mCurAction >= ACTION_MG_FISH_MF_ESA_HIT) {
            i_this->mCcCyl.SetR(30.0f);
        } else {
            i_this->mCcCyl.SetR(hREG_F(16) + 40.0f);
        }
    } else {
        f32 radius = i_this->mJointScale * (KREG_F(13) + 20.0f);
        if (radius > 15.0f) {
            radius = 15.0f;
        }
        i_this->mCcCyl.SetR(radius);
        i_this->mCcCyl.SetH((KREG_F(13) + 30.0f) * i_this->mJointScale);
    }

    dComIfG_Ccsp()->Set(&i_this->mCcCyl);

    if (unkFlag6 == 0) {
        cXyz* ccMoveP = i_this->mCcStatus.GetCCMoveP();
        if (ccMoveP != NULL) {
            if (isAqua) {
                f32 scalar = XREG_F(15) + 0.04f;
                i_this->actor.current.pos.x += ccMoveP->x * scalar;
                i_this->actor.current.pos.z += ccMoveP->z * scalar;
                for (s32 i = 0; i <= i_this->mNumJoints; i++) {
                    i_this->mJointTranslations[i].x += ccMoveP->x * scalar;
                    i_this->mJointTranslations[i].z += ccMoveP->z * scalar;
                }
            } else if (i_this->mCurAction == ACTION_MG_FISH_MF_HIT ||
                i_this->mCurAction >= ACTION_MG_FISH_MF_ESA_HIT) {
                i_this->actor.current.pos.x += ccMoveP->x;
                i_this->actor.current.pos.z += ccMoveP->z;
            } else {
                i_this->actor.current.pos.x += ccMoveP->x * 0.5f;
                i_this->actor.current.pos.z += ccMoveP->z * 0.5f;
            }
        }
    }

    if (unkFlag7) {
        f32 unkFloat2 = i_this->mJointScale * (TREG_F(3) + 10.0f);
        unkFloat2 += fabsf(unkFloat2 * cM_ssin(i_this->actor.current.angle.x) * 2.0f);
        i_this->actor.current.pos.y = i_this->actor.current.pos.y - unkFloat2;
        i_this->actor.old.pos.y = i_this->actor.old.pos.y - unkFloat2;
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
        i_this->actor.current.pos.y = i_this->actor.current.pos.y + unkFloat2;
        i_this->actor.old.pos.y = i_this->actor.old.pos.y + unkFloat2;

        if (i_this->mAcch.ChkGroundHit() && i_this->actor.current.angle.x > 0x800) {
            i_this->mMovementPitch = 0x800;
            i_this->actor.current.angle.x = 0x800;
        }
    }

    if (isAqua) {
        if (i_this->actor.current.pos.x > -675.0f || i_this->actor.current.pos.x < -765.0f) {
            i_this->actor.current.pos.x = i_this->actor.old.pos.x;
        }
        if (i_this->actor.current.pos.z > 290.0f || i_this->actor.current.pos.z < -150.0f) {
            i_this->actor.current.pos.z = i_this->actor.old.pos.z;
        }
        if (i_this->actor.current.pos.y < i_this->mGroundY + 5.0f) {
            i_this->actor.current.pos.y = i_this->actor.old.pos.y;
        }

        if (daPy_getPlayerActorClass()->checkFrontRollCrash() && i_this->mDistToPlayer < 200.0f) {
            i_this->mCurAction = ACTION_MG_FISH_MF_AQUA_TILT;
            i_this->mActionPhase = 0;
            i_this->field_0x624[1] = cM_rndF(30.0f) + 50.0f;

            fopAc_ac_c* henna = fopAcM_SearchByName(PROC_NPC_HENNA);
            if (henna != NULL) {
                ((npc_henna_class*)henna)->field_0x7b5 = 20;
            }
        }
    }

    // surfaceY access is replaced with subtraction operation in debug rom
    if (isInUnderwaterState && i_this->actor.current.pos.y > i_this->mSurfaceY) {
        i_this->actor.current.pos.y = i_this->mSurfaceY;
        if (i_this->actor.current.angle.x < 0) {
            i_this->mMovementPitch = 0;
            i_this->actor.current.angle.x = 0;
        }
    }

    cLib_addCalcAngleS2(&i_this->actor.shape_angle.y, i_this->actor.current.angle.y, 2, 0x1000);
    cLib_addCalcAngleS2(&i_this->actor.shape_angle.x, i_this->actor.current.angle.x, 2, 0x1000);
    cLib_addCalcAngleS2(&i_this->actor.shape_angle.z, i_this->actor.current.angle.z, 0x10, 0x200);

    if (i_this->mGedouKind != GEDOU_KIND_CF_1) {
        f32 unkFloat4 = i_this->actor.speedF * (NREG_F(15) + 4000.0f);
        f32 target = 4000.0f;
        if (i_this->mGedouKind == GEDOU_KIND_RI_1) {
            target = 1000.0f;
        } else if (i_this->mGedouKind == GEDOU_KIND_NP_1 || i_this->mGedouKind == GEDOU_KIND_KS_1) {
            target = 3500.0f;
        }
        if (unkFloat4 > target) {
            unkFloat4 = target;
        }
        i_this->jointYaws2[4] = -(i_this->field_0x73c - unkFloat4) +
            i_this->field_0x73c * cM_ssin(i_this->mBobTimer * (TREG_S(9) + 1900));
        i_this->jointYaws2[5] = i_this->field_0x73c - unkFloat4 +
             i_this->field_0x73c * cM_ssin(i_this->mBobTimer * (TREG_S(9) + 1700));
        target = 0.3f - (NREG_F(14) + i_this->actor.speedF);
        if (target < NREG_F(14) + 0.0f) {
            target = NREG_F(14) + 0.0f;
        }
        target *= 23333.0f;
        cLib_addCalc2(&i_this->field_0x73c, target, 0.2f, 800.0f);
    }
}

static void ke_control(mg_fish_class* i_this, mf_ke_s* rope) {
    s32 i;
    s16 pitch;
    s32 yaw;
    cXyz* p1 = rope->mControlPoints + 1;
    f32 influence = 1.0f;

    cMtx_YrotS(*calc_mtx, i_this->actor.shape_angle.y);
    cMtx_XrotM(*calc_mtx, i_this->actor.shape_angle.x);

    cXyz localOffset(0.0f, 0.0f, -3.0f);
    cXyz newSegmentDelta;
    cXyz transformedOffset;
    MtxPosition(&localOffset, &transformedOffset);

    localOffset.x = 0.0f;
    localOffset.y = 0.0f;
    localOffset.z = 10.0f * i_this->mJointScale;

    for (i = 1; i < 5; i++, p1++) {
        f32 newX = p1->x - p1[-1].x + rope->mMovementDir.x * influence + transformedOffset.x;
        f32 newY = p1->y - p1[-1].y + rope->mMovementDir.y * influence + transformedOffset.y;
        f32 newZ = p1->z - p1[-1].z + rope->mMovementDir.z * influence + transformedOffset.z;

        influence *= 0.4f;

        yaw = cM_atan2s(newX, newZ);
        pitch = -cM_atan2s(newY, JMAFastSqrt(newX * newX + newZ * newZ));
        cMtx_YrotS(*calc_mtx, yaw);
        cMtx_XrotM(*calc_mtx, pitch);
        MtxPosition(&localOffset, &newSegmentDelta);
        p1->x = p1[-1].x + newSegmentDelta.x;
        p1->y = p1[-1].y + newSegmentDelta.y;
        p1->z = p1[-1].z + newSegmentDelta.z;
    }
}

static void ke_move(mg_fish_class* i_this, mf_ke_s* param_1, int param_2) {
    ke_control(i_this, param_1);
    cXyz* pos = i_this->mLineMat.getPos(param_2);
    f32* size = i_this->mLineMat.getSize(param_2);
    for (s32 i = 0; i < 5; i++, pos++, size++) {
        *pos = param_1->mControlPoints[i];
        if (i < 3) {
            *size = 0.3f * i_this->mJointScale;
        } else {
            *size = 0.0f;
        }
    }
}

static int daMg_Fish_Execute(mg_fish_class* i_this) {
    static s32 mouth_idx[11] = { 7, 0, 9, 7, 9, 4, 7, 0, 9, 7, 9 };

    if (i_this->mGedouKind == GEDOU_KIND_BT || i_this->mGedouKind == GEDOU_KIND_SP ||
        i_this->mGedouKind == GEDOU_KIND_LH || i_this->mGedouKind == GEDOU_KIND_O_GD_BOTT ||
        i_this->mGedouKind == GEDOU_KIND_BB || i_this->mGedouKind == GEDOU_KIND_KN ||
        i_this->mGedouKind == GEDOU_KIND_ED || i_this->mGedouKind == GEDOU_KIND_SY ||
        i_this->field_0xc3c != 0) {
        return 1;
    }

    cXyz commonXyz;

    daPy_py_c* player = daPy_getPlayerActorClass();

#if VERSION == VERSION_GCN_JPN
    lit_1008 = 0;
#elif VERSION == VERSION_GCN_PAL
    if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_ENGLISH) {
        lit_1008 = 2;
    } else {
        lit_1008 = 0;
    }
#endif

    if (i_this->mSurfaceY != 0.0f) {
        if ((g_Counter.mTimer + fopAcM_GetID(i_this) & 0xf) == 0) {
            get_surface_y(i_this, &i_this->actor.current.pos);
        }
    }
    i_this->mHookedState = 0;
    action(i_this);
    dmcalc(i_this);

    J3DModel* model = i_this->mpMorf->getModel();
    for (u16 i = 1; i < model->mModelData->getJointNum(); i++) {
        if ((i_this->mCurAction == ACTION_MG_FISH_MF_JUMP && i_this->actor.speedF == 0.0f) ||
            i_this->mCurAction == ACTION_MG_FISH_MF_CATCH ||
            i_this->mCurAction == ACTION_MG_FISH_MF_ESA_CATCH)
        {
            model->mModelData->getJointNodePointer(i)->setCallBack(nodeCallBack2);
        } else {
            model->mModelData->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }

    if (i_this->mCurAction == ACTION_MG_FISH_MF_CATCH) {
        if (i_this->field_0x624[0] < 0x50) {
            MtxP src = daPy_getLinkPlayerActorClass()->getRightItemMatrix();
            MTXCopy(src, *calc_mtx);
            commonXyz.set(0.0f, 5.0f, -5.0f);
            MtxPosition(&commonXyz, &i_this->actor.current.pos);
        } else {
            daPy_py_c* player2 = (daPy_py_c*)dComIfGp_getPlayer(0);
            cMtx_YrotS(*calc_mtx, player2->shape_angle.y);

            if (player2->checkCanoeFishingGetRight() != 0) {
                commonXyz.x = -100.0f;
            } else {
                commonXyz.x = 100.0f;
            }
            commonXyz.y = 0.0f;
            commonXyz.z = 0.0f;
            MtxPosition(&commonXyz, &i_this->actor.current.pos);

            i_this->actor.current.pos.x = i_this->actor.current.pos.x + player2->current.pos.x;
            i_this->actor.current.pos.z = i_this->actor.current.pos.z + player2->current.pos.z;
            i_this->actor.current.pos.y = i_this->mSurfaceY - 10.0f;
        }
        MTXTrans(mDoMtx_stack_c::now, i_this->actor.current.pos.x, i_this->actor.current.pos.y,
                 i_this->actor.current.pos.z);
        cMtx_YrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.y);
        cMtx_XrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.x);
        f32 jointScaleMult = 1.0f;
        f32 jointScale = i_this->mJointScale;
        if (jointScale < 0.6f) {
            jointScaleMult = 0.6f - jointScale + 1.0f;
        }
        if (jointScaleMult > 1.12f) {
            jointScaleMult = 1.12f;
        }
        jointScale *= jointScaleMult;
        mDoMtx_stack_c::scaleM(jointScale, jointScale, jointScale);
        if (i_this->mNumJoints <= 4) {
            mDoMtx_stack_c::transM(0.0f, 15.0f, -25.0f);
        } else {
            mDoMtx_stack_c::transM(0.0f, 10.0f, -15.0f);
        }
    } else {
        MTXTrans(mDoMtx_stack_c::now, i_this->actor.current.pos.x, i_this->actor.current.pos.y,
                 i_this->actor.current.pos.z);
        cMtx_YrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.y);
        cMtx_XrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.x);
        cMtx_ZrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.z);

        f32 jointScale = i_this->mJointScale;
        mDoMtx_stack_c::scaleM(jointScale, jointScale, jointScale);
        if (i_this->mCurAction == ACTION_MG_FISH_MF_JUMP && i_this->actor.speedF == 0.0f) {
            jointScale = 20.0f;
        } else {
            jointScale = 0.0f;
        }
        mDoMtx_stack_c::transM(0.0f, 0.0f, -jointScale);
        cMtx_YrotM(mDoMtx_stack_c::now, i_this->jointYaws2[0]);
        mDoMtx_stack_c::transM(0.0f, 0.0f, jointScale);
        if (i_this->mCurAction == ACTION_MG_FISH_MF_ESA_CATCH) {
            mDoMtx_stack_c::scaleM(1.1f, 1.1f, 1.1f);
        }
    }
    MTXCopy(mDoMtx_stack_c::now, model->getBaseTRMtx());
    if (i_this->mCaughtType != 0) {
        dmg_rod_class* rod = (dmg_rod_class*)fopAcM_SearchByID(i_this->mRodId);
        if (i_this->mCaughtType == 1) {
            mg_fish_class* fish = (mg_fish_class*)fpcEx_Search(s_bt_sub, i_this);
            if (fish != NULL) {
                MTXTrans(mDoMtx_stack_c::now, i_this->actor.current.pos.x, i_this->actor.current.pos.y - 10.0f,
                         i_this->actor.current.pos.z);
                cMtx_YrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.y);
                cMtx_XrotM(mDoMtx_stack_c::now, -0x4000);
                mDoMtx_stack_c::transM(0.0f, 0.0f, 7.0f);
                fish->mpModel[0]->setBaseTRMtx(mDoMtx_stack_c::now);
            }
        } else if (i_this->mCaughtType == 2) {
            mg_fish_class* fish = (mg_fish_class*)fpcEx_Search(s_sp_sub, i_this);
            if (fish != NULL) {
                MTXTrans(mDoMtx_stack_c::now, i_this->actor.current.pos.x, i_this->actor.current.pos.y,
                         i_this->actor.current.pos.z);
                cMtx_YrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.y);
                cMtx_XrotM(mDoMtx_stack_c::now, 0x4000);
                if (rod->hook_kind == 1) {
                    mDoMtx_stack_c::transM(0.0f, 0.0f, 35.0f);
                } else {
                    mDoMtx_stack_c::transM(0.0f, 0.0f, 29.0f);
                }
                mDoMtx_stack_c::scaleM(0.36f, 0.36f, 0.36f);
                fish->mpModel[0]->setBaseTRMtx(mDoMtx_stack_c::now);
                mDoMtx_stack_c::transM(0.0f, 0.0f, -40.0f);
                mDoMtx_stack_c::scaleM(2.5f, 2.5f, 2.5f);
                cMtx_XrotM(mDoMtx_stack_c::now, 0x4000);
                fish->mpModel[1]->setBaseTRMtx(mDoMtx_stack_c::now);
                cMtx_XrotM(mDoMtx_stack_c::now, -0x4000);
                mDoMtx_stack_c::transM(0.0f, 0.0f, 17.0f);
                mDoMtx_stack_c::scaleM(0.5f, 0.5f, 0.5f);
                cMtx_ZrotM(mDoMtx_stack_c::now, 0x4000);
                fish->mpModel[2]->setBaseTRMtx(mDoMtx_stack_c::now);
            }
        } else if (i_this->mCaughtType == 3) {
            mg_fish_class* fish = (mg_fish_class*)fpcEx_Search(s_lh_sub, i_this);
            if (fish != NULL) {
                MTXTrans(mDoMtx_stack_c::now, i_this->actor.current.pos.x, i_this->actor.current.pos.y,
                         i_this->actor.current.pos.z);
                cMtx_YrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.y);
                f32 latScale = 0.5f / (i_this->field_0x74c + 1.0f);
                mDoMtx_stack_c::scaleM(latScale, (i_this->field_0x74c + 1.0f) * 0.5f, latScale);
                mDoMtx_stack_c::transM(0.0f, -30.0f, 0.0f);
                fish->mpModel[0]->setBaseTRMtx(mDoMtx_stack_c::now);
            }
        } else if (i_this->mCaughtType == 4) {
            fopAc_ac_c* skullfish_actor = fopAcM_SearchByID(i_this->mSkullfishId);
            if (skullfish_actor != NULL) {
                e_sg_class* skullfish = (e_sg_class*)skullfish_actor;
                MTXTrans(mDoMtx_stack_c::now, i_this->actor.current.pos.x, i_this->actor.current.pos.y,
                         i_this->actor.current.pos.z);
                cMtx_YrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.y);
                cMtx_XrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.x);
                cMtx_ZrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.z);
                if (i_this->mCurAction == ACTION_MG_FISH_MF_ESA_CATCH) {
                    mDoMtx_stack_c::scaleM(0.8f, 0.8f, 0.8f);
                }
                mDoMtx_stack_c::transM(0.0f, 2.5f, 0.0f);
                skullfish->mpModel->setBaseTRMtx(mDoMtx_stack_c::now);

                f32 fAVar1[] = { 1.0f, 2.0f, 3.5f };

                skullfish->mJointYRot = 0.0f;
                for (s32 i = 0; i < 3; i++) {
                    if (i_this->mCurAction == ACTION_MG_FISH_MF_ESA_CATCH) {
                        skullfish->mJoints[i + 1] = (s32)(i_this->field_0x740 *
                            cM_ssin(i_this->mBobTimer * 4000 + -15000 * i) *
                            fAVar1[i]);
                    } else {
                        skullfish->mJoints[i + 1] = (s32)(cM_ssin(i_this->mBobTimer * 8000 + -15000 * i) *
                            4000.0f *
                            fAVar1[i]);
                    }
                }
            }
        } else if (i_this->mCaughtType == 5) {
            mg_fish_class* fish = (mg_fish_class*)fpcEx_Search(s_bb_sub, i_this);
            if (fish != NULL) {
                MTXTrans(mDoMtx_stack_c::now, i_this->actor.current.pos.x, i_this->actor.current.pos.y,
                    i_this->actor.current.pos.z);

                f32 fAVar2[] = { 1.0f, 2.0f, 3.5f };
                for (s32 i = 0; i < 3; i++) {
                    if (i_this->mCurAction == ACTION_MG_FISH_MF_ESA_CATCH) {
                        fish->jointYaws1[i] = (s32)(i_this->field_0x740 *
                            cM_ssin(i_this->mBobTimer * 4000 + -15000 * i) *
                            fAVar2[i]);
                    } else {
                        fish->jointYaws1[i] = (s32)(cM_ssin(i_this->mBobTimer * 10000 + -15000 * i) *
                            3000.0f *
                            fAVar2[i]);
                    }
                }
                if (i_this->mCurAction == ACTION_MG_FISH_MF_ESA_CATCH) {
                    cMtx_YrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.y);
                } else {
                    cMtx_YrotM(mDoMtx_stack_c::now,
                               i_this->actor.shape_angle.y + fish->jointYaws1[0]);
                }
                cMtx_XrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.x);
                cMtx_ZrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.z);
                if (i_this->mCurAction == ACTION_MG_FISH_MF_ESA_CATCH) {
                    mDoMtx_stack_c::scaleM(0.7f, 0.7f, 0.7f);
                }
                fish->mpModel[0]->setBaseTRMtx(mDoMtx_stack_c::now);
            }
        } else if (i_this->mCaughtType == 6) {
            mg_fish_class* fish = (mg_fish_class*)fpcEx_Search(s_bin_sub, i_this);
            if (fish != NULL) {
                MTXTrans(mDoMtx_stack_c::now, i_this->actor.current.pos.x, i_this->actor.current.pos.y - 10.0f,
                         i_this->actor.current.pos.z);
                cMtx_YrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.y);
                mDoMtx_stack_c::transM(0.0f, -9.0f, 0.0f);
                fish->mpModel[0]->setBaseTRMtx(mDoMtx_stack_c::now);
            }
        } else if (i_this->mCaughtType == 7) {
            mg_fish_class* fish = (mg_fish_class*)fpcEx_Search(s_kn_sub, i_this);
            if (fish != NULL) {
                MTXTrans(mDoMtx_stack_c::now, i_this->actor.current.pos.x, i_this->actor.current.pos.y,
                         i_this->actor.current.pos.z);
                cMtx_YrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.y);
                cMtx_XrotM(mDoMtx_stack_c::now,
                           (s16)(cM_ssin(i_this->mBobTimer * 0x514) * 600.0f + 2600.0f) +
                               -0x4000);
                mDoMtx_stack_c::scaleM(1.0f, 1.0f, 1.0f);
                mDoMtx_stack_c::transM(0.0f, -8.0f, -5.0f);
                fish->mpModel[0]->setBaseTRMtx(mDoMtx_stack_c::now);
            }
        } else if (i_this->mCaughtType == 8) {
            mg_fish_class* fish = (mg_fish_class*)fpcEx_Search(s_ed_sub, i_this);
            if (fish != NULL) {
                MTXTrans(mDoMtx_stack_c::now, i_this->actor.current.pos.x, i_this->actor.current.pos.y,
                         i_this->actor.current.pos.z);
                cMtx_YrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.y);
                cMtx_XrotM(mDoMtx_stack_c::now, -0x4000);
                if (rod->hook_kind == 1) {
                    mDoMtx_stack_c::transM(0.0f, 0.0f, -14.0f);
                } else {
                    mDoMtx_stack_c::transM(0.0f, 0.0f, -8.0f);
                }

                cMtx_YrotM(mDoMtx_stack_c::now, cM_ssin(i_this->mBobTimer * 800) * 1500.0f);
                mDoMtx_stack_c::scaleM(0.5f, 0.5f, 0.5f);
                fish->mpModel[0]->setBaseTRMtx(mDoMtx_stack_c::now);
            }
        } else if (i_this->mCaughtType == 9) {
            mg_fish_class* fish = (mg_fish_class*)fpcEx_Search(s_sy_sub, i_this);
            if (fish != NULL) {
                MTXTrans(mDoMtx_stack_c::now, i_this->actor.current.pos.x, i_this->actor.current.pos.y,
                         i_this->actor.current.pos.z);
                cMtx_YrotM(mDoMtx_stack_c::now, i_this->actor.shape_angle.y);
                cMtx_XrotM(mDoMtx_stack_c::now, -0x4000);
                if (rod->hook_kind == 1) {
                    mDoMtx_stack_c::transM(0.0f, 0.0f, -25.0f);
                } else {
                    mDoMtx_stack_c::transM(0.0f, 0.0f, -19.0f);
                }
                cMtx_YrotM(mDoMtx_stack_c::now, 0xa00);
                mDoMtx_stack_c::scaleM(0.3f, 0.3f, 0.3f);
                fish->mpModel[0]->setBaseTRMtx(mDoMtx_stack_c::now);
            }
        }
    }
    i_this->mpMorf->play(&i_this->actor.eyePos, 0, 0);
    if (i_this->mpBrkAnm != NULL) {
        i_this->mpBrkAnm->play();
    }
    i_this->mpMorf->modelCalc();
    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(&i_this->actor)));
    if (i_this->mKind2 == 3) {
        cXyz* ctrlPtMtx;
        for (s32 i = 0; i < 2; i++) {
            MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(0), *calc_mtx);

            if (i == 0) {
                commonXyz.set(3.0f, 0.0f, 17.0f);
            } else {
                commonXyz.set(-3.0f, 0.0f, 17.0f);
            }

            MtxPosition(&commonXyz, &i_this->field_0x754[i].mControlPoints[0]);

            cMtx_YrotS(*calc_mtx, i_this->actor.shape_angle.y);
            cMtx_XrotM(*calc_mtx, i_this->actor.shape_angle.x);
            cMtx_ZrotM(*calc_mtx, i_this->actor.shape_angle.z);
            cMtx_YrotM(*calc_mtx, i_this->jointYaws2[0]);

            commonXyz.set(i == 0 ? 2.0f : -2.0f, 0.0f, 0.0f);
            MtxPosition(&commonXyz, &i_this->field_0x754[i].mControlPoints[5]);

            ke_move(i_this, (mf_ke_s*)&i_this->field_0x754[i].mControlPoints[0], i);
        }
    }

    static cXyz mouth_off[11] = {
        cXyz(25.0f, 5.0f, -8.0f), cXyz(25.0f, 0.0f, 2.0f),  cXyz(30.0f, 0.0f, 2.0f),
        cXyz(25.0f, -2.0f, 2.0f), cXyz(30.0f, 0.0f, 2.0f),  cXyz(30.0f, 0.0f, 0.0f),
        cXyz(15.0f, 0.0f, 0.0f),  cXyz(15.0f, -1.0f, 0.0f), cXyz(15.0f, -1.0f, 0.0f),
        cXyz(15.0f, -1.0f, 0.0f), cXyz(15.0f, -1.0f, 0.0f),
    };
    MTXCopy(model->getAnmMtx(mouth_idx[i_this->mGedouKind]), *calc_mtx);
    commonXyz = mouth_off[i_this->mGedouKind];
    MtxPosition(&commonXyz, &i_this->field_0x638);
    if (i_this->mHookedState != 0) {
        dmg_rod_class* rod = (dmg_rod_class*)fopAcM_SearchByID(i_this->mRodId);
        if (rod->lure_type == MG_LURE_SP && i_this->mCurAction == ACTION_MG_FISH_MF_CATCH) {
            if (i_this->mKind2 == 0) {
                f32 fVar3 = (i_this->mJointScale - 0.48f) * 100.0f;
                if (fVar3 < 0.0f) {
                    fVar3 = 0.0f;
                }
                commonXyz.x += fVar3 + -45.0f;
                commonXyz.z += -5.0f;
            } else if (i_this->mKind2 == 2) {
                commonXyz.x += -35.0f;
                commonXyz.z += 7.0f;
            } else if (i_this->mKind2 == 3) {
                commonXyz.x += -35.0f;
                commonXyz.y += 11.0f;
                commonXyz.z += 5.0f;
            } else if (i_this->mKind2 == 1) {
                commonXyz.x += -30.0f;
                commonXyz.z += 5.0f;
            }
            MtxPosition(&commonXyz, &i_this->field_0x638);
            rod->actor.current.angle.y = 0;
            rod->actor.current.angle.x = 0x4000;
            rod->actor.current.angle.z = player->shape_angle.y + 8000;
            rod->actor.shape_angle.x = rod->actor.current.angle.x;
            rod->actor.shape_angle.y = rod->actor.current.angle.y;
            rod->actor.shape_angle.z = rod->actor.current.angle.z;
            rod->field_0x114a = 0;
            rod->field_0x114c = 0;
            rod->field_0x1004 = 0;
        } else {
            if (rod->lure_type == MG_LURE_FR && i_this->mGedouKind == GEDOU_KIND_RI_1) {
                commonXyz.y += 8.0f;
                commonXyz.z += -3.0f;
                MtxPosition(&commonXyz, &i_this->field_0x638);
            }
            cLib_addCalcAngleS2(&rod->actor.current.angle.y, i_this->actor.shape_angle.y - 0x1710, 2, 0x800);
            cLib_addCalcAngleS2(&rod->actor.current.angle.x,
                i_this->actor.shape_angle.x + - 0x310c + rod->field_0x1006, 2, 0x800);
            cLib_addCalcAngleS2(&rod->actor.current.angle.z, 0x6328, 2, 0x800);
        }
        rod->actor.current.pos.x = i_this->field_0x638.x;
        rod->actor.current.pos.y = i_this->field_0x638.y;
        rod->actor.current.pos.z = i_this->field_0x638.z;
        rod->field_0x1000 = 0;
        rod->field_0xffc = 0;
    }
    if (i_this->field_0x659 != 0) {
        fopAc_ac_c* rod = fopAcM_SearchByID(i_this->mRodId);
        if (rod != NULL) {
            if (i_this->mCaughtType != 0) {
                i_this->field_0x638 = i_this->actor.current.pos;
            }
            if (i_this->mCurAction >= ACTION_MG_FISH_MF_ESA_HIT) {
                rod->current.pos = i_this->field_0x638;
            } else {
                cLib_addCalc2(&rod->current.pos.x, i_this->field_0x638.x, 1.0f,
                              i_this->field_0x650);
                cLib_addCalc2(&rod->current.pos.y, i_this->field_0x638.y, 1.0f,
                              i_this->field_0x650);
                cLib_addCalc2(&rod->current.pos.z, i_this->field_0x638.z, 1.0f,
                              i_this->field_0x650);
                cLib_addCalc2(&i_this->field_0x650, 100.0f, 1.0f, 2.0f);
            }
            i_this->field_0x659--;
        } else {
            i_this->field_0x659 = 0;
        }
    }
    return 1;
}

static int daMg_Fish_IsDelete(mg_fish_class* i_this) {
    return 1;
}

static int daMg_Fish_Delete(mg_fish_class* i_this) {
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhaseReq, i_this->mResName);
    if (i_this->field_0xc3f != 0) {
        lit_1007 = 0;
    }
    if (i_this->actor.heap != NULL) {
        i_this->mSound.deleteObject();
    }
    return 1;
}

static int useHeapInit2(fopAc_ac_c* i_actor) {
    mg_fish_class* i_this = (mg_fish_class*)i_actor;

    if (i_this->mGedouKind == GEDOU_KIND_BT) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, 3);
        i_this->mpModel[0] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->mpModel[0] == 0) {
            return 0;
        }
    } else if (i_this->mGedouKind == GEDOU_KIND_SP) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, 11);
        i_this->mpModel[0] = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
        if (i_this->mpModel[0] == 0) {
            return 0;
        }
        modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, 5);
        i_this->mpModel[1] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->mpModel[1] == 0) {
            return 0;
        }
        modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, 7);
        i_this->mpModel[2] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->mpModel[2] == 0) {
            return 0;
        }
    } else if (i_this->mGedouKind == GEDOU_KIND_LH) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, 3);
        i_this->mpModel[0] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->mpModel[0] == 0) {
            return 0;
        }
        i_this->mpModel[0]->setUserArea((s32)i_this);
        for (u16 i = 0; i < modelData->getJointNum(); i++) {
            i_this->mpModel[0]->mModelData->getJointNodePointer(i)->setCallBack(nodeCallBackLH);
        }
    } else if (i_this->mGedouKind == GEDOU_KIND_O_GD_BOTT) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, 6);
        i_this->mpModel[0] = mDoExt_J3DModel__create(modelData, 0, 0x11020084);
        if (i_this->mpModel[0] == NULL) {
            return 0;
        }
        i_this->mpBtpAnm = new mDoExt_btpAnm();
        if (i_this->mpBtpAnm == 0) {
            return 0;
        }
        J3DAnmTexPattern* pattern = (J3DAnmTexPattern*)dComIfG_getObjectRes(i_this->mResName, 15);
        if (i_this->mpBtpAnm->init(&modelData->getMaterialTable(),
            pattern, 1, 0, 1.0f, 0, -1) == 0) {
            return 0;
        }
    } else if (i_this->mGedouKind == GEDOU_KIND_BB) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, 3);
        i_this->mpModel[0] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->mpModel[0] == 0) {
            return 0;
        }
        i_this->mpModel[0]->setUserArea((s32)i_this);
        for (u16 i = 0; i < modelData->getJointNum(); i++) {
            i_this->mpModel[0]->mModelData->getJointNodePointer(i)->setCallBack(nodeCallBackBB);
        }
    } else if (i_this->mGedouKind == GEDOU_KIND_KN) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, 6);
        i_this->mpModel[0] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->mpModel[0] == 0) {
            return 0;
        }
    } else if (i_this->mGedouKind == GEDOU_KIND_ED) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, 4);
        i_this->mpModel[0] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->mpModel[0] == 0) {
            return 0;
        }
    } else if (i_this->mGedouKind == GEDOU_KIND_SY) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, 8);
        i_this->mpModel[0] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->mpModel[0] == NULL) {
            return 0;
        }
    }
    for (s32 i = 0; i < 3; i++) {
        if (i_this->mpModel[i] != NULL) {
            mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
            mDoMtx_copy(mDoMtx_stack_c::now, i_this->mpModel[i]->mBaseTransformMtx);
        }
    }
    return 1;
}

static int useHeapImg_fisht(fopAc_ac_c* i_actor) {
    static u32 fish_bmd[11] = {
        8, 3, 3, 3, 4, 3, 8, 3,
        3, 3, 4,
    };
    static u32 fish_joint[11] = {
        4, 6, 6, 6, 6, 3, 4, 6,
        6, 6, 6,
    };

    mg_fish_class* i_this = (mg_fish_class*)i_actor;

    if (i_this->mGedouKind == GEDOU_KIND_LM_1) {
        i_this->mAnmTransform = (J3DAnmTransform*)dComIfG_getObjectRes(i_this->mResName, 4);
    } else {
        i_this->mAnmTransform = 0;
    }
    i_this->mpMorf = new mDoExt_McaMorf(
        (J3DModelData*)dComIfG_getObjectRes(i_this->mResName, fish_bmd[i_this->mGedouKind]),
            NULL, NULL, i_this->mAnmTransform, 0, 1.0f, 0, -1, 1, NULL,
            0x80000, 0x11000084);
    if (i_this->mpMorf == NULL || i_this->mpMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = i_this->mpMorf->getModel();
    model->setUserArea((s32)i_this);
    i_this->mNumJoints = fish_joint[i_this->mGedouKind];
    for (u16 i = 1; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }
    if (i_this->mKind2 == 3 && i_this->mLineMat.init(2, 5, 1) == 0) {
        return 0;
    }

    if (i_this->mKind2 == 4) {
        i_this->mpBrkAnm = new mDoExt_brkAnm();
        if (i_this->mpBrkAnm == NULL) {
            return 0;
        }
        J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(i_this->mResName, 7);
        J3DModelData* modelData = model->getModelData();
        s32 iVar7 = i_this->mpBrkAnm->init(&modelData->getMaterialTable(), brk, 1, 2,
            1.0f, 0, -1);
        if (iVar7 == 0) {
            return 0;
        }
        i_this->mRotZ = 5000;
    }
    return 1;
}

static int daMg_Fish_Create(fopAc_ac_c* i_this) {
    /* 805369FC-80536A40 000558 0044+00 0/1 0/0 0/0 .data            cc_cyl_src$8395 */
    static dCcD_SrcCyl cc_cyl_src = {
        {
            { 0x0, { { 0x0, 0x0, 0x0 }, { 0x0, 0x0 }, 0x75 } }, // mObj
            { dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0 }, // mGObjAt
            { dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2 }, // mGObjTg
            { 0 }, // mGObjCo
        }, // mObjInf
        {
            { 0.0f, 0.0f, 0.0f }, // mCenter
            35.0f, // mRadius
            35.0f // mHeight
        } // mCyl
    };
    static f32 fish_max[11] = {
        0.828f,
        0.998f,
        0.998f,
        0.998f,
        0.708f,
        0.428f,
        0.448f,
        0.588f,
        0.588f,
        0.548f,
        0.708f,
    };

    fopAcM_ct(i_this, mg_fish_class);

    mg_fish_class* a_this = (mg_fish_class*)i_this;

    a_this->mGedouKind = fopAcM_GetParam(i_this);

    bool flag1 = false;
    if (a_this->mGedouKind == 106) {
        a_this->mGedouKind = GEDOU_KIND_BG;
        flag1 = true;
    }

    if (a_this->mGedouKind >= 100) {
        a_this->mGedouKind = a_this->mGedouKind - 95;
    }

    u32 heapSize = 0x1000;
    if (a_this->mGedouKind == GEDOU_KIND_LM_1 || a_this->mGedouKind == GEDOU_KIND_LM_2) {
        a_this->mKind2 = 0;
        a_this->mResName = "Mg_f_lm";
        heapSize = 0x1420;
    } else if (a_this->mGedouKind == GEDOU_KIND_NP_1 || a_this->mGedouKind == GEDOU_KIND_NP_2) {
        a_this->mKind2 = 2;
        a_this->mResName = "Mg_f_np";
        heapSize = 0xd60;
    } else if (a_this->mGedouKind == GEDOU_KIND_CF_1 || a_this->mGedouKind == GEDOU_KIND_CF_2) {
        a_this->mKind2 = 3;
        a_this->mResName = "Mg_f_cf";
        heapSize = 0x1200;
    } else if (a_this->mGedouKind == GEDOU_KIND_RI_1 || a_this->mGedouKind == GEDOU_KIND_RI_2) {
        a_this->mKind2 = 1;
        a_this->mResName = "Mg_f_ri";
        heapSize = 0xdc0;
    } else if (a_this->mGedouKind == GEDOU_KIND_KS_1 || a_this->mGedouKind == GEDOU_KIND_KS_2) {
        a_this->mKind2 = 4;
        a_this->mResName = "Mg_f_ks";
        heapSize = 0x13d0;
    } else if (a_this->mGedouKind == GEDOU_KIND_BG) {
        a_this->mKind2 = 5;
        a_this->mResName = "Mg_f_bg";
        heapSize = 0xc60;
    } else if (a_this->mGedouKind == GEDOU_KIND_BT) {
        a_this->mResName = "Mg_f_bt";
    } else if (a_this->mGedouKind == GEDOU_KIND_SP || a_this->mGedouKind == GEDOU_KIND_KN ||
        a_this->mGedouKind == GEDOU_KIND_ED || a_this->mGedouKind == GEDOU_KIND_SY) {
        a_this->mResName = "Mg_f_sp";
    } else if (a_this->mGedouKind == GEDOU_KIND_BB) {
        a_this->mResName = "Mg_f_bb";
    } else if (a_this->mGedouKind == GEDOU_KIND_LH) {
        a_this->mResName = "Mg_f_lh";
    } else if (a_this->mGedouKind == GEDOU_KIND_O_GD_BOTT) {
        a_this->mResName = "O_gD_bott";
    }

#if VERSION == VERSION_GCN_JPN
    lit_1008 = 0;
#elif VERSION == VERSION_GCN_PAL
    if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_ENGLISH) {
        lit_1008 = 2;
    } else {
        lit_1008 = 0;
    }
#else
    lit_1008 = 1;
#endif

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&a_this->mPhaseReq, a_this->mResName);
    cPhs__Step retval = phase;

    if (phase == cPhs_COMPLEATE_e) {
        s32 params_0 = fopAcM_GetParam(i_this) >> 24;
        if (params_0 != 0 && params_0 != 0xff &&
            g_dComIfG_gameInfo.info.isSwitch(params_0, fopAcM_GetRoomNo(i_this)))
        {
            return cPhs_ERROR_e;
        }

        if (a_this->mGedouKind == GEDOU_KIND_BT ||
            a_this->mGedouKind == GEDOU_KIND_SP ||
            a_this->mGedouKind == GEDOU_KIND_BB ||
            a_this->mGedouKind == GEDOU_KIND_LH ||
            a_this->mGedouKind == GEDOU_KIND_O_GD_BOTT ||
            a_this->mGedouKind == GEDOU_KIND_KN ||
            a_this->mGedouKind == GEDOU_KIND_ED ||
            a_this->mGedouKind == GEDOU_KIND_SY)
        {
            s32 params_2 = fopAcM_GetParam(i_this) >> 8 & 0xff;
            if (params_2 == 0xff) {
                params_2 = 0x1e;
            }
            a_this->mJointScale = params_2 * 100;
            heapSize = 0x3000;
            if (a_this->mGedouKind == GEDOU_KIND_BT) {
                heapSize = 0x800;
            } else if (a_this->mGedouKind == GEDOU_KIND_LH) {
                heapSize = 0x840;
            } else if (a_this->mGedouKind == GEDOU_KIND_SP) {
                heapSize = 0x1d40;
            } else if (a_this->mGedouKind == GEDOU_KIND_KN) {
                heapSize = 0x820;
            } else if (a_this->mGedouKind == GEDOU_KIND_ED) {
                heapSize = 0x800;
            } else if (a_this->mGedouKind == GEDOU_KIND_SY) {
                heapSize = 0x820;
            }

            if (!fopAcM_entrySolidHeap(i_this, useHeapInit2, heapSize)) {
                return cPhs_ERROR_e;
            }

            return phase;
        }

        if (!fopAcM_entrySolidHeap(i_this, useHeapImg_fisht, heapSize)) {
            return cPhs_ERROR_e;
        }

        if (lit_1007 == 0) {
            a_this->field_0xc3f = 1;
            lit_1007 = 1;
            l_HIO.field_0x4 = -1;
        }
        a_this->mCcStatus.Init(0x1e, 0, i_this);
        a_this->mCcCyl.Set(cc_cyl_src);
        a_this->mCcCyl.SetStts(&a_this->mCcStatus);
        i_this->cullMtx = a_this->mpMorf->getModel()->getBaseTRMtx();
        fopAcM_SetMin(i_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(i_this, 200.0f, 200.0f, 200.0f);
        a_this->mAcch.Set(&i_this->current.pos, &i_this->old.pos, i_this, 1, &a_this->mAcchCir,
            &i_this->speed, NULL, NULL);
        a_this->field_0xc44 = dComIfGs_getEventReg(0xf11f);
        a_this->mJointScale = 0.0001f + (fopAcM_GetParam(i_this) >> 8 & 0xff) * 0.01f;
        if (a_this->mGedouKind >= GEDOU_KIND_BG) {
            f32 fishMaxSize;
            if (lit_1008 == 1) {
                fishMaxSize = dComIfGs_getFishSize(a_this->mKind2) * 2.54f;
            } else {
                fishMaxSize = dComIfGs_getFishSize(a_this->mKind2);
            }
            if (fishMaxSize > 0.1f) {
                if (lit_1008 != 0) {
                    a_this->mJointScale = fishMaxSize * 0.01f + cM_rndFX(0.05334f);
                } else {
                    a_this->mJointScale = fishMaxSize * 0.01f + cM_rndFX(0.021f);
                }
            } else {
                a_this->mJointScale = cM_rndFX(0.02f) + 0.28f;
                if (a_this->mGedouKind != GEDOU_KIND_BG) {
                    if (a_this->mGedouKind == GEDOU_KIND_LM_2) {
                        a_this->mJointScale += 0.04f;
                    } else if (a_this->mGedouKind == GEDOU_KIND_KS_2) {
                        a_this->mJointScale = cM_rndFX(0.039f) + 0.67f;
                    } else if (a_this->mGedouKind == GEDOU_KIND_CF_2) {
                        a_this->mJointScale += 0.1f;
                    } else {
                        a_this->mJointScale += 0.1f;
                    }
                }
            }
        } else {
            if (a_this->mGedouKind <= GEDOU_KIND_CF_1) {
                f32 jointScale = 100.0f * a_this->mJointScale;
                f32 fVar1 =
                    lit_1008 == 1 ?
                        dComIfGs_getEventReg(check_kind[a_this->mGedouKind << 0]) * 2.54f :
                        dComIfGs_getEventReg(check_kind[a_this->mGedouKind << 0]);
                if ((s32)jointScale <= (s32)fVar1) {
                    a_this->mJointScale = fVar1 * 0.01f;
                    f32 fVar2 = 0.05f;
                    if (a_this->mGedouKind == GEDOU_KIND_RI_1) {
                        fVar2 = 0.15f;
                    } else if (fVar1 >= 0.6f) {
                        fVar2 = 0.03f;
                    }
                    if (cM_rndF(1.0f) < fVar2) {
                        if (lit_1008 != 0) {
                            a_this->mJointScale += 0.028194f;
                        } else {
                            a_this->mJointScale += 0.0111f;
                        }
                    } else if (a_this->mGedouKind == GEDOU_KIND_RI_1) {
                        a_this->mJointScale *= 1.0f - cM_rndF(0.1f);
                    } else {
                        a_this->mJointScale *= 1.0f - cM_rndF(0.3f);
                    }
                    if (a_this->field_0xc44 >= 20 ||
                        (a_this->mGedouKind != GEDOU_KIND_RI_1 && cM_rndF(1.0f) < 0.2f))
                    {
                        a_this->mJointScale *= 0.8f;
                    } else if (a_this->field_0xc44 >= 10) {
                        a_this->mJointScale *= 0.85f;
                    } else if (a_this->field_0xc44 >= 5) {
                        a_this->mJointScale *= 0.9f;
                    }
                }
                if (a_this->mJointScale < 0.42f) {
                    a_this->mJointScale = cM_rndF(0.05f) + 0.4f;
                }
            }
        }
        if (a_this->mJointScale > fish_max[a_this->mGedouKind]) {
            a_this->mJointScale = fish_max[a_this->mGedouKind];
        }
        a_this->mAcchCir.SetWall(3.0f * a_this->mJointScale, 30.0f * a_this->mJointScale);
        a_this->mBobTimer = cM_rndF(65536.0f);
        a_this->jointYaws2[1] = cM_rndF(65536.0f);
        a_this->mSound.init(&i_this->current.pos, &i_this->current.pos, 3, 1);
        a_this->field_0x660 = 1.0f;
        a_this->field_0x5ec = l_HIO.field_0x10;
        if (a_this->mGedouKind == GEDOU_KIND_RI_1) {
            a_this->mCurAction = ACTION_MG_FISH_RI_SWIM;
            a_this->mActionPhase = 2;
            a_this->field_0x624[0] = cM_rndF(200.0f) + 300.0f;
        } else if (a_this->mGedouKind == GEDOU_KIND_CF_1) {
            a_this->mCurAction = ACTION_MG_FISH_MF_SWIM_P;
            a_this->field_0x5ec = 500.0f;
        } else if (a_this->mGedouKind >= GEDOU_KIND_BG) {
            a_this->mCurAction = ACTION_MG_FISH_MF_SWIM_S;
            a_this->field_0x5ec = l_HIO.field_0x14;
            if (flag1) {
                a_this->field_0x5ec = 20000.0f;
            } else {
                a_this->field_0x5ec = 1000.0f;
            }
            a_this->field_0x624[2] = cM_rndF(1000.0f) + 1000.0f;
        } else {
            a_this->mCurAction = ACTION_MG_FISH_MF_STAY;
            a_this->mActionPhase = 2;
        }
        s16 newYaw = cM_rndF(65536.0f);
        i_this->current.angle.y = newYaw;
        i_this->shape_angle.y = newYaw;
        a_this->mMovementYaw = newYaw;
        if (strcmp(dComIfGp_getStartStageName(), "R_SP127") == 0) {
            a_this->mSurfaceY = 200.0f;
            a_this->mGroundY = 60.0f;
            i_this->current.pos.set(cM_rndFX(30.0f) + -720.0f, cM_rndF(30.0f) + 110.0f,
                                    cM_rndFX(180.0f) + 70.0f);
            if (a_this->mGedouKind == GEDOU_KIND_RI_1) {
                a_this->mCurAction = ACTION_MG_FISH_RI_AQUA;
            } else {
                a_this->mCurAction = ACTION_MG_FISH_MF_AQUA;
                if (a_this->mGedouKind == GEDOU_KIND_BG) {
                    a_this->mJointScale = cM_rndFX(0.03f) + 0.25f;
                }
            }
            a_this->mActionPhase = 0;
        } else if (strcmp(dComIfGp_getStartStageName(), "F_SP127") != 0) {
            a_this->mSurfaceY = 200000.0f;
        }
        a_this->mDistToPlayer = 100000.0f;
        a_this->field_0x632 = 20;
        a_this->mNodeCallBack = nodeCallBack2;
        if (a_this->mGedouKind < GEDOU_KIND_BG) {
            a_this->field_0xc40 = 50.0f * a_this->field_0xc44 + 500.0f;
        } else {
            a_this->field_0xc40 = 300.0f;
        }
        daMg_Fish_Execute(a_this);
    }
    return retval;
}

static actor_method_class l_daMg_Fish_Method = {
    (process_method_func)daMg_Fish_Create,
    (process_method_func)daMg_Fish_Delete,
    (process_method_func)daMg_Fish_Execute,
    (process_method_func)daMg_Fish_IsDelete,
    (process_method_func)daMg_Fish_Draw,
};

extern actor_process_profile_definition g_profile_MG_FISH = {
    (uint)fpcLy_CURRENT_e,  // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_MG_FISH,           // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(mg_fish_class),  // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    720,                    // mPriority
    &l_daMg_Fish_Method,    // sub_method
    0x00040100,             // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES
