#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_attention.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/d_debug_viewer.h"
#include "d/d_stage.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_controller_pad.h"
#include "SSystem/SComponent/c_counter.h"

#define DRAW_TYPE_YELLOW 0
#define DRAW_TYPE_RED    1

class dAttDrawParam_c : public JORReflexible {
public:
    dAttDrawParam_c();
    virtual ~dAttDrawParam_c() {}

    void genMessage(JORMContext*);

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mCursorDistance;
    /* 0xC */ f32 field_0xc;
};

namespace {
static bool padLockTrigger(s32 i_padNo) {
    return mDoCPd_c::getTrigL(i_padNo) != 0;
}

static bool padLockButton(s32 i_padNo) {
    return mDoCPd_c::getHoldLockL(i_padNo) != 0;
}
}  // namespace

type_tbl_entry dAttention_c::loc_type_tbl[3] = {
    {fopAc_attn_LOCK_e, fopAc_AttnFlag_LOCK_e},
    {fopAc_attn_TALK_e, fopAc_AttnFlag_TALK_e},
    {fopAc_attn_BATTLE_e, fopAc_AttnFlag_BATTLE_e},
};

type_tbl_entry dAttention_c::act_type_tbl[5] = {
    {fopAc_attn_SPEAK_e, fopAc_AttnFlag_SPEAK_e},
    {fopAc_attn_CARRY_e, fopAc_AttnFlag_CARRY_e},
    {fopAc_attn_DOOR_e, fopAc_AttnFlag_DOOR_e},
    {fopAc_attn_JUEL_e, fopAc_AttnFlag_JUEL_e},
    {fopAc_attn_ETC_e, fopAc_AttnFlag_ETC_e},
};

static bool l_isFinalBossStg;

dAttParam_c::dAttParam_c(s32) {
    field_0x4 = 45.0f;
    field_0x8 = 30.0f;
    field_0xc = 90.0f;
    mFlags = 1;
    mSWModeDisable = -0.9f;

    mDangerBGMDistance = 3000.0f;
    mBGMDistMargin = 1000.0f;

    mSelCursorScaleX = 6.0f;
    mSelCursorScaleY = 4.5f;

    mAttnCursorScaleX = 14.0f;
    mAttnCursorScaleY = 14.0f;

    mSelCursorOffsetY = 10.0f;
    mAttnCursorOffsetY = 0.0f;

    mAttnCursorAppearFrames = 1;
    mAttnCursorDisappearFrames = 1;

    field_0x38 = 1.7f;
    field_0x3c = 1.0f;

    if (strcmp(dComIfGp_getStartStageName(), "D_MN09B") == 0) {
        l_isFinalBossStg = 1;
    } else {
        l_isFinalBossStg = 0;
    }

#if DEBUG
    // "Attention System"
    connectHIO("注目システム");
#endif
}

dAttParam_c::~dAttParam_c() {
#if DEBUG
    releaseHIO();
#endif
}

#if DEBUG
void dAttParam_c::genMessage(JORMContext* mctx) {
    mctx->genCheckBox(" リスト表示", &mFlags, 0x4000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox(" 有効", &mFlags, 4, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox(" ホールドモード", &mFlags, 1, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox(" 矢印表示OFF", &mFlags, 0x10, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("カーソル", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("選択時", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("スケールＸ", &mSelCursorScaleX, 1.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("スケールＹ", &mSelCursorScaleY, 1.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("Y Pos Offset", &mSelCursorOffsetY, -100.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("注目時", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("スケールＸ", &mAttnCursorScaleX, 1.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("スケールＹ", &mAttnCursorScaleY, 1.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("Y Pos Offset", &mAttnCursorOffsetY, -100.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("出現フレーム", &mAttnCursorAppearFrames, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("消去フレーム", &mAttnCursorDisappearFrames, 0, 0xff, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("アニメーションスピード", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("選択時", &field_0x38, 1.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("注目時", &field_0x3c, 1.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider(" SW Mode 解除", &mSWModeDisable, -1.0f, 0.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox(" 表示デバック", &mFlags, 2, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider(" 警告ＢＧＭ距離", &mDangerBGMDistance, 0.0f, 8000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("     + マージン", &mBGMDistMargin, 0.0f, 8000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox(" デバッグ表示", &mFlags, 0x8000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("   表示位置 Ｘ", &mDebugDispPosX, 0, 0x280, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("            Ｙ", &mDebugDispPosY, 0, 0x1e0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox(" KEEP挙動", &mFlags, 8, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

dAttDrawParam_c::dAttDrawParam_c() {
    mCursorDistance = 250.0f;
    field_0xc = 2.0f;
}

#if DEBUG
void dAttDrawParam_c::genMessage(JORMContext* mctx) {
    mctx->genLabel("カーソル", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("距離", &mCursorDistance, 0.0f, 500000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}
#endif

int dAttDraw_CallBack_c::execute(u16 param_0, J3DTransformInfo* transform) {
    if (param_0 == 0) {
        transform->mTranslate.y *= YREG_F(17) + 0.6f;
    }

    return 1;
}

dAttDrawParam_c g_AttDwHIO;

dAttention_c::dAttention_c(fopAc_ac_c* i_player, u32 i_padNo) {
    mpPlayer = i_player;
    mPadNo = i_padNo;

    mLockTargetID = fpcM_ERROR_PROCESS_ID_e;
    field_0x32e = 0;
    field_0x32f = 0;

    mLockOnOffset = mActionOffset = mCheckObjectOffset = 0;
    initList(0xFFFFFFFF);

    mPlayerAttentionFlags = 0;
    mAttnStatus = field_0x32a = EState_NONE;
    field_0x32b = 4;
    field_0x32c = 0;
    mAttnBlockTimer = 0;

    heap = mDoExt_createSolidHeapFromGameToCurrent(0x9000, 0);
    JUT_ASSERT(0x198, heap != NULL);

    J3DModelData* modelDataR = (J3DModelData*)dComIfG_getObjectRes("Always", 0x25);
    JUT_ASSERT(0x1BB, modelDataR);

    J3DModelData* modelDataY = (J3DModelData*)dComIfG_getObjectRes("Always", 0x26);
    JUT_ASSERT(0x1BF, modelDataY);

    for (int i = 0; i < 2; i++) {
        draw[i].mModel[DRAW_TYPE_YELLOW] = mDoExt_J3DModel__create(modelDataY, 0x80000, 0x11000285);
        JUT_ASSERT(0x1CA, draw[i].mModel[0] != NULL);

        draw[i].mModel[DRAW_TYPE_RED] = mDoExt_J3DModel__create(modelDataR, 0x80000, 0x11000285);
        JUT_ASSERT(0x1D4, draw[i].mModel[1] != NULL);

        int res;
        res = draw[i].mNoticeCursorBck[DRAW_TYPE_YELLOW].init((J3DAnmTransform*)dComIfG_getObjectRes("Always", 0x11), TRUE, 2, 1.0f, 0, -1, false);
        JUT_ASSERT(0x1DC, res == 1);

        res = draw[i].mNoticeCursorBpk[DRAW_TYPE_YELLOW].init(modelDataY, (J3DAnmColor*)dComIfG_getObjectRes("Always", 0x2B), TRUE, 2, 1.0f, 0, -1);
        JUT_ASSERT(0x1E4, res == 1);

        res = draw[i].mNoticeCursorBrk[DRAW_TYPE_YELLOW].init(modelDataY, (J3DAnmTevRegKey*)dComIfG_getObjectRes("Always", 0x37), TRUE, 2, 1.0f, 0, -1);
        JUT_ASSERT(0x1EC, res == 1);

        res = draw[i].mNoticeCursorBtk[DRAW_TYPE_YELLOW].init(modelDataY, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Always", 0x3F), TRUE, 2, 1.0f, 0,
                                         -1);
        JUT_ASSERT(0x1F4, res == 1);

        res = draw[i].mNoticeCursor02Brk[DRAW_TYPE_YELLOW].init(modelDataY, (J3DAnmTevRegKey*)dComIfG_getObjectRes("Always", 0x38), TRUE, 2, 1.0f, 0, -1);
        JUT_ASSERT(0x1FC, res == 1);
        
        res = draw[i].mNoticeCursorBck[DRAW_TYPE_RED].init((J3DAnmTransform*)dComIfG_getObjectRes("Always", 0x10), TRUE, 2, 1.0f, 0, -1, false);
        JUT_ASSERT(0x204, res == 1);

        res = draw[i].mNoticeCursorBpk[DRAW_TYPE_RED].init(modelDataR, (J3DAnmColor*)dComIfG_getObjectRes("Always", 0x2A), TRUE, 2, 1.0f, 0, -1);
        JUT_ASSERT(0x20B, res == 1);

        res = draw[i].mNoticeCursorBrk[DRAW_TYPE_RED].init(modelDataR, (J3DAnmTevRegKey*)dComIfG_getObjectRes("Always", 0x35), TRUE, 2, 1.0f, 0, -1);
        JUT_ASSERT(0x212, res == 1);

        res = draw[i].mNoticeCursorBtk[DRAW_TYPE_RED].init(modelDataR, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Always", 0x3E), TRUE, 2, 1.0f, 0,
                                         -1);
        JUT_ASSERT(0x219, res == 1);

        res = draw[i].mNoticeCursor02Brk[DRAW_TYPE_RED].init(modelDataR, (J3DAnmTevRegKey*)dComIfG_getObjectRes("Always", 0x36), TRUE, 2, 1.0f, 0, -1);
        JUT_ASSERT(0x220, res == 1);

        res = draw[i].mImpactBck.init((J3DAnmTransform*)dComIfG_getObjectRes("Always", 0xC), TRUE, 0, 1.0f, 0, -1, false);
        JUT_ASSERT(0x228, res == 1);

        res = draw[i].mImpactBpk.init(modelDataR, (J3DAnmColor*)dComIfG_getObjectRes("Always", 0x29), TRUE, 0, 1.0f, 0, -1);
        JUT_ASSERT(0x22F, res == 1);

        res = draw[i].mImpactBrk.init(modelDataR, (J3DAnmTevRegKey*)dComIfG_getObjectRes("Always", 0x32), TRUE, 0, 1.0f, 0, -1);
        JUT_ASSERT(0x236, res == 1);

        res = draw[i].mImpactBtk.init(modelDataR, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Always", 0x3D), TRUE, 0, 1.0f, 0, -1);
        JUT_ASSERT(0x23D, res == 1);

        draw[i].mDrawType = DRAW_TYPE_YELLOW;
        draw[i].mAlphaAnmFrameMax = 1;
        draw[i].mAlphaAnmFrame = 0;
        draw[i].mAlphaAnmPlayDirection = 1;
        draw[i].field_0x173 = 2;
        draw[i].field_0x175 = 0;
    }

    mDoExt_restoreCurrentHeap();
    if ((int)mDoExt_adjustSolidHeap(heap) >= 0) {
        DCStoreRangeNoSync(heap->getStartAddr(), heap->getHeapSize());
    }

    field_0x328 = -1;
    mFlags = 0;

    mZHintTarget.init();
    mCatghTarget.init();
    mLookTarget.init();
    new (&mAttParam) dAttParam_c(0);

    g_AttDwHIO.field_0x4 = mDoHIO_CREATE_CHILD("注目カーソル", &g_AttDwHIO);
}

dAttention_c::~dAttention_c() {
    if (heap != NULL) {
        mDoExt_destroySolidHeap(heap);
        heap = NULL;
    }

    mDoHIO_DELETE_CHILD(g_AttDwHIO.field_0x4);
}

dAttList_c* dAttention_c::GetLockonList(s32 i_no) {
    if (mLockonCount != 0) {
        return &mLockOnList[(mLockOnOffset + i_no) % mLockonCount];
    }

    return NULL;
}

dAttList_c* dAttention_c::getActionBtnB() {
    int i;
    dAttList_c* list = GetLockonList(0);
    fopAc_ac_c* actor;

    if (list != NULL && list->getActor() != NULL && list->mType == fopAc_attn_TALK_e && LockonTruth()) {
        actor = list->getActor();
        if (!(actor->attention_info.flags & fopAc_AttnFlag_NOTALK_e)) {
            return list;
        }   
    }

    if (mActionCount == 0) {
        return NULL;
    }

    for (i = 0; i < mActionCount; i++) {
        if (mActionList[i].mType == fopAc_attn_SPEAK_e) {
            actor = mActionList[i].getActor();
            if (!(actor->attention_info.flags & fopAc_AttnFlag_NOTALK_e)) {
                return &mActionList[i];
            }
            continue;
        }
        return &mActionList[i];
    }

    return NULL;
}

dAttList_c* dAttention_c::getActionBtnXY() {
    int i;
    dAttList_c* list = GetLockonList(0);
    fopAc_ac_c* actor;

    if (list != NULL && list->getActor() != NULL && list->mType == fopAc_attn_TALK_e && LockonTruth()) {
        actor = list->getActor();
        if (actor->eventInfo.chkCondition(dEvtCnd_CANTALKITEM_e)) {
            return list;
        }

        return NULL;
    }

    if (mActionCount == 0) {
        return NULL;
    }

    for (i = 0; i < mActionCount; i++) {
        if (mActionList[i].mType == fopAc_attn_SPEAK_e) {
            actor = mActionList[i].getActor();
            if (actor->eventInfo.chkCondition(dEvtCnd_CANTALKITEM_e)) {
                return &mActionList[i];
            }
        }
    }

    return NULL;
}

int dAttention_c::loc_type_num = 3;

int dAttention_c::chkAttMask(u32 i_type, u32 i_mask) {
    int i;
    for (i = 0; i < loc_type_num; i++) {
        if (i_type == loc_type_tbl[i].type) {
            return i_mask & loc_type_tbl[i].mask;
        }
    }

    return 1;
}

static int check_event_condition(u32 i_attnType, u16 i_condition) {
    switch (i_attnType) {
    case fopAc_attn_TALK_e:
    case fopAc_attn_SPEAK_e:
        if (!(i_condition & dEvtCnd_CANTALK_e)) {
            return true;
        }
        break;
    case fopAc_attn_DOOR_e:
        if (!(i_condition & dEvtCnd_CANDOOR_e)) {
            return true;
        }
        break;
    case fopAc_attn_JUEL_e:
        if (!(i_condition & dEvtCnd_CANDOOR_e)) {
            return true;
        }
        break;
    }

    return false;
}

int dAttention_c::act_type_num = 5;

type_tbl_entry dAttention_c::chk_type_tbl[1] = {
    {fopAc_attn_CHECK_e, fopAc_AttnFlag_CHECK_e},
};

int dAttention_c::chk_type_num = 1;

static bool attn_opt_hold = true;

static int check_flontofplayer(u32 i_checkMask, s16 i_ang1, s16 i_ang2) {
    static int ftp_table[9] = {
        0x4, 0x1, 0x2, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100,
    };

    static s16 ang_table[3] = {
        0x4000, 0x2000, 0x0AAA,
    };

    static s16 ang_table2[6] = {
        0x0AAA, 0x2000, 0x2AAA, 0x4000, 0x4E38, 0x6000,
    };

    if (i_ang1 < 0) {
        i_ang1 = -i_ang1;
    }

    if (i_ang2 < 0) {
        i_ang2 = -i_ang2;
    }

    int i;
    for (i = 0; i < 3; i++) {
        if (i_checkMask & ftp_table[i]) {
            if (i_ang1 > ang_table[i]) {
                return true;
            }
        }
    }

    for (i = 8; i >= 3; i--) {
        if (i_checkMask & ftp_table[i]) {
            if (i_ang2 > ang_table2[i - 3]) {
                return true;
            }
        }
    }

    return false;
}

static f32 distace_weight(f32 i_distance, s16 i_angle, f32 i_ratio) {
    f32 var_f31 = i_angle / (f32)0x8000;
    f32 var_f30 = SQUARE(var_f31);
    return i_distance * ((1.0f - i_ratio) + (i_ratio * var_f30));
}

static f32 distace_angle_adjust(f32 i_distAdjust, s16 i_angle, f32 i_ratio) {
    f32 var_f31 = i_angle / (f32)0x8000;
    if (var_f31 < 0.0f) {
        var_f31 = -var_f31;
    }
    f32 var_f30 = SQUARE(1.0f - var_f31);

    return i_distAdjust * ((1.0f - i_ratio) + (i_ratio * var_f30));
}

static BOOL check_distace(cXyz* i_pos, s16 i_angle, cXyz* i_attnPos, f32 i_distMax, f32 i_distAdjust,
                          f32 i_max_y, f32 i_min_y) {
    cXyz dist = *i_attnPos - *i_pos;

    if (dist.y <= i_min_y || dist.y >= i_max_y) {
        return false;
    }

    f32 adjust = i_distMax + distace_angle_adjust(i_distAdjust, i_angle, 1.0f);
    if (adjust < dist.absXZ()) {
        return false;
    }

    return true;
}

f32 dAttention_c::calcWeight(int i_listType, fopAc_ac_c* i_actor, f32 i_distance, s16 i_angle,
                             s16 i_invAngle, u32* i_attnType) {
    int i;
    int num;
    type_tbl_entry* table;

    switch (i_listType) {
    case 'L':
        if (chkFlag(0x4000)) {
            return 0.0f;
        }

        num = loc_type_num;
        table = loc_type_tbl;
        break;
    default:
        OS_REPORT("attention: %d: illegal type\n", 1049);
    case 'A':
        num = act_type_num;
        table = act_type_tbl;
        break;
    case 'C':
        num = chk_type_num;
        table = chk_type_tbl;
        break;
    }

    f32 weight = 0.0f;
    f32 max_weight = -1.0f;

    int dist_index;
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (player != NULL) {
        fpc_ProcID grab_actor_id = player->getGrabActorID();
        if (i_actor == fopAcM_SearchByID(grab_actor_id)) {
            return 0.0f;
        }
    }

    for (i = 0; i < num; i++) {
        f32 dist_weight;
        type_tbl_entry* type_tbl_entry = &table[i];

        if (mPlayerAttentionFlags & type_tbl_entry->mask & i_actor->attention_info.flags) {
            dist_index = i_actor->attention_info.distances[type_tbl_entry->type];
            dist_entry* dist_tbl_entry = &dist_table[dist_index];

            if (fopAcM_CheckStatus(i_actor, 0x20000000) ||
                check_event_condition(type_tbl_entry->type, i_actor->eventInfo.getCondition())) {
                dist_weight = 0.0f;
            } else if (check_flontofplayer(dist_tbl_entry->mAngleSelect, i_angle, i_invAngle)) {
                dist_weight = 0.0f;
            } else if (!check_distace(&mOwnerAttnPos, i_angle, &i_actor->attention_info.position,
                                      dist_tbl_entry->mDistMax, dist_tbl_entry->mDistanceAdjust, dist_tbl_entry->mUpperY,
                                      dist_tbl_entry->mLowerY)) {
                dist_weight = 0.0f;
            } else {
                dist_weight = distace_weight(i_distance, i_angle, 0.5f);
            }

            f32 dist_tbl_weight = dist_tbl_entry->mWeight;
            if (dist_weight > 0.0f && dist_tbl_weight > max_weight) {
                max_weight = dist_tbl_weight;
                weight = dist_weight / dist_tbl_weight;
                *i_attnType = type_tbl_entry->type;
            }
        }
    }

    return weight;
}

void dAttention_c::setList(int i_listType, fopAc_ac_c* i_actor, f32 i_weight, f32 i_distance,
                           cSAngle i_angle, u32 i_attnType) {
    int max;
    int* list_count;
    int maxIndex;
    dAttList_c* list;

    switch (i_listType) {
    case 'L':
        if (chkFlag(0x4000)) return;
        if (mLockonCount >= 1 && i_actor == mLockOnList[0].getActor() && i_attnType == mLockOnList[0].mType) {
            return;
        }
        max = 8;
        list_count = &mLockonCount;
        list = mLockOnList;
        break;
    default:
        OS_REPORT("attention: %d: illegal type\n", 1169);
    case 'A':
        max = 4;
        list_count = &mActionCount;
        list = mActionList;
        break;
    case 'C':
        max = 4;
        list_count = &mCheckObjectCount;
        list = mCheckObjectList;
        break;
    }

    if (i_weight > 0.0f) {
        if (*list_count < max) {
            maxIndex = *list_count;
            (*list_count)++;
        } else {
            f32 maxWeight = 0.0f;
            int i = 0;
            maxIndex = i;
            for (; i < max; i++) {
                if (list[i].mWeight > maxWeight) {
                    maxWeight = list[i].mWeight;
                    maxIndex = i;
                }
            }
        }

        if (list[maxIndex].mWeight > i_weight) {
            list[maxIndex].setActor(i_actor);
            list[maxIndex].mWeight = i_weight;
            list[maxIndex].mDistance = i_distance;
            list[maxIndex].mAngle = i_angle;
            list[maxIndex].mType = i_attnType;
        }
    }
}

void dAttention_c::initList(u32 flags) {
    mPlayerAttentionFlags = flags;

    if (!chkFlag(0x4000)) {
        int lockonIndex;
        mLockonCount = lockonIndex = mLockTargetID != fpcM_ERROR_PROCESS_ID_e ? 1 : 0;
        if (mLockOnOffset != 0) {
            memcpy(mLockOnList, mLockOnList + mLockOnOffset, sizeof(dAttList_c));
            mLockOnList[0].mWeight = 0.0f;
        }

        mLockOnOffset = 0;
        for (; lockonIndex < 8; lockonIndex++) {
            mLockOnList[lockonIndex].setActor(NULL);
            mLockOnList[lockonIndex].mWeight = FLT_MAX;
        }
    }

    int i;
    mActionCount = mActionOffset = i = 0;
    for (; i < 4; i++) {
        mActionList[i].setActor(NULL);
        mActionList[i].mWeight = FLT_MAX;
    }

    mCheckObjectCount = mCheckObjectOffset = i = 0;
    for (; i < 4; i++) {
        mCheckObjectList[i].setActor(NULL);
        mCheckObjectList[i].mWeight = FLT_MAX;
    }

    setFlag(0x1000);
}

static int select_attention(fopAc_ac_c* i_actor, void* i_attention) {
    if (i_actor->attention_info.flags == 0) {
        return 0;
    }

    dAttention_c* attention = (dAttention_c*)i_attention;
    int rt = attention->SelectAttention(i_actor);
    return rt;
}

int dAttention_c::makeList() {
    fopAcIt_Executor((fopAcIt_ExecutorFunc)select_attention, this);
    setFlag(0x2000);
    return mLockonCount + mActionCount + mCheckObjectCount;
}

void dAttention_c::setOwnerAttentionPos() {
    mOwnerAttnPos = mpPlayer->attention_info.position;

    if (fopAcM_GetName(mpPlayer) == PROC_ALINK) {
        mOwnerAttnPos.y -= ((daPy_py_c*)mpPlayer)->getAttentionOffsetY();
    }
}

int dAttention_c::SelectAttention(fopAc_ac_c* i_actor) {
    cSAngle angle;
    cSAngle inv_angle;
    if (i_actor == mpPlayer || mpPlayer == NULL) {
        return 0;
    }

    mPlayerAttentionFlags = mpPlayer->attention_info.flags;

    cSGlobe globe(i_actor->attention_info.position - mOwnerAttnPos);
    angle = globe.U() - fopAcM_GetShapeAngle_p(mpPlayer)->y;
    inv_angle = cSAngle(globe.U().Inv()) - fopAcM_GetShapeAngle_p(i_actor)->y;

    u32 attn_type;
    if ((i_actor->attention_info.flags & fopAc_AttnFlags_LOCK) && !chkFlag(0x4000)) {
        f32 weight = calcWeight('L', i_actor, globe.R(), angle.Val(), inv_angle.Val(), &attn_type);
        setList('L', i_actor, weight, globe.R(), angle, attn_type);
    }

    if (i_actor->attention_info.flags & fopAc_AttnFlags_ACTION) {
        f32 weight = calcWeight('A', i_actor, globe.R(), angle.Val(), inv_angle.Val(), &attn_type);
        setList('A', i_actor, weight, globe.R(), angle, attn_type);
    }

    if (i_actor->attention_info.flags & fopAc_AttnFlag_CHECK_e) {
        f32 weight = calcWeight('C', i_actor, globe.R(), angle.Val(), inv_angle.Val(), &attn_type);
        setList('C', i_actor, weight, globe.R(), angle, attn_type);
    }

    return 0;
}

void dAttention_c::sortList() {
    int i;
    int j;
    int count;
    dAttList_c swap;
    dAttList_c* list;

    if (!chkFlag(0x4000)) {
        list = mLockOnList;
        for (i = (mLockTargetID != fpcM_ERROR_PROCESS_ID_e) ? 1 : 0, count = mLockonCount; i < count - 1; i++) {
            for (j = i + 1; j < count; j++) {
                if (list[i].mWeight > list[j].mWeight) {
                    memcpy(&swap, &list[j], sizeof(dAttList_c));
                    memcpy(&list[j], &list[i], sizeof(dAttList_c));
                    memcpy(&list[i], &swap, sizeof(dAttList_c));
                }
            }
        }
    }

    list = mActionList;
    count = mActionCount;
    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (list[i].mWeight > list[j].mWeight) {
                memcpy(&swap, &list[j], sizeof(dAttList_c));
                memcpy(&list[j], &list[i], sizeof(dAttList_c));
                memcpy(&list[i], &swap, sizeof(dAttList_c));
            }
        }
    }

    list = mCheckObjectList;
    count = mCheckObjectCount;
    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (list[i].mWeight > list[j].mWeight) {
                memcpy(&swap, &list[j], sizeof(dAttList_c));
                memcpy(&list[j], &list[i], sizeof(dAttList_c));
                memcpy(&list[i], &swap, sizeof(dAttList_c));
            }
        }
    }
}

void dAttention_c::stockAttention() {
    fopAc_ac_c* lockon_actor = LockonTarget(0);

    initList(0xFFFFFFFF);
    if (makeList()) {
        sortList();
    }

    if (lockon_actor != mLockOnList[0].getActor()) {
        if (lockon_actor != NULL) {
            if (mLockOnList[0].getActor() != NULL) {
                setFlag(2);
            }
        } else {
            setFlag(1);
        }
        setFlag(4);
    }

    LockonTarget(0);
}

fopAc_ac_c* dAttention_c::nextAttention() {
    if (!attn_opt_hold && mLockonCount == 1) {
        field_0x32e = 0;
        return NULL;
    }

    if (field_0x32e != 0) {
        mLockOnOffset++;

        if (mLockOnOffset >= mLockonCount) {
            mLockOnOffset = 0;
        }
        return LockonTarget(0);
    }

    fopAc_ac_c* actor = fopAcM_SearchByID(mLockTargetID);
    initList(0xFFFFFFFF);

    if (makeList()) {
        sortList();
    }

    if (actor == mLockOnList[0].getActor() && mLockonCount > 1) {
        mLockOnOffset = 1;
    }

    return LockonTarget(0);
}

int dAttention_c::freeAttention() {
    mLockTargetID = fpcM_ERROR_PROCESS_ID_e;
    initList(0xFFFFFFFF);
    return 0;
}

bool dAttention_c::chaseAttention() {
    cSAngle a1;
    cSAngle a2;

    int offset = mLockOnOffset;
    fopAc_ac_c* actor = mLockOnList[offset].getActor();

    if (actor == NULL) {
        return false;
    }

    if (!chkFlag(0x4000)) {
        cSGlobe g1 = actor->attention_info.position - mOwnerAttnPos;
        a1 = g1.U() - fopAcM_GetShapeAngle_p(mpPlayer)->y;

        cSGlobe g2(mOwnerAttnPos - actor->attention_info.position);
        a2 = g2.U() - fopAcM_GetShapeAngle_p(actor)->y;

        u32 type;
        f32 weight = calcWeight('L', actor, g1.R(), a1.Val(), a2.Val(), &type);
        if (weight <= 0.0f) {
            type = mLockOnList[offset].mType;
            int tbl_idx = actor->attention_info.distances[type];

            if (!chkAttMask(type, actor->attention_info.flags)) {
                return false;
            } else if (check_event_condition(type, actor->eventInfo.getCondition())) {
                return false;
            } else if (check_flontofplayer(dist_table[tbl_idx].mAngleSelect, a1.Val(), a2.Val())) {
                return false;
            } else if (check_distace(&mOwnerAttnPos, a1.Val(), &actor->attention_info.position,
                                     dist_table[tbl_idx].mDistMaxRelease,
                                     dist_table[tbl_idx].mDistanceAdjust,
                                     dist_table[tbl_idx].mUpperY, dist_table[tbl_idx].mLowerY)) {
                f32 weight = distace_weight(g1.R(), a1.Val(), 0.5f);
                mLockOnList[offset].mWeight = weight;
                return true;
            }

            return false;
        }

        mLockOnList[offset].setActor(actor);
        mLockOnList[offset].mWeight = weight;
        mLockOnList[offset].mDistance = g1.R();
        mLockOnList[offset].mType = type;
        return true;
    }

    return (actor->attention_info.flags & fopAc_AttnFlags_LOCK) != false;
}

f32 dAttention_c::EnemyDistance(fopAc_ac_c* i_actor) {
    if (i_actor == mpPlayer || mpPlayer == NULL) {
        return -1.0f;
    } else if (fopAcM_GetProfName(i_actor) == PROC_ALINK) {
        return -1.0f;
    } else if (!(i_actor->attention_info.flags & fopAc_AttnFlag_BATTLE_e) &&
               !(i_actor->attention_info.flags & fopAc_AttnFlag_ENEMY_NOLOCK_e)) {
        return -1.0f;
    } else {
        f32 distance = fopAcM_searchActorDistance(mpPlayer, i_actor);
        int tmp = i_actor->attention_info.distances[fopAc_attn_BATTLE_e];

        if (distance < dist_table[tmp].mDistMax + dist_table[tmp].mDistanceAdjust) {
            return distance;
        }
    }

    return -1.0f;
}

static int sound_attention(fopAc_ac_c* i_actor, void* i_attention) {
    dAttention_c* attention = (dAttention_c*)i_attention;

    f32 dist = attention->EnemyDistance(i_actor);
    if (dist < 0.0f) {
        return 0;
    }

    if (dist < attention->mEnemyDist) {
        attention->mEnemyActorID = fopAcM_GetID(i_actor);
        attention->mEnemyDist = dist;
    }

    return 0;
}

void dAttention_c::runSoundProc() {
    mEnemyActorID = fpcM_ERROR_PROCESS_ID_e;
    mEnemyDist = 10000.0f;

    fopAc_ac_c* enemy_p;
    if (!chkFlag(0x80000000)) {
        fopAcIt_Executor((fopAcIt_ExecutorFunc)sound_attention, this);

        enemy_p = fopAcM_SearchByID(mEnemyActorID);
        if (enemy_p != NULL) {
            mDoAud_bgmNowBattle(mEnemyDist * 0.1f);
            setFlag(0x100);
        }
    }
}

void dAttention_c::runDrawProc() {
    int lockon_cnt = GetLockonCount();

    if (chkFlag(8)) {
        draw[0].setAlphaAnm(mAttParam.mAttnCursorAppearFrames, 0);
        draw[0].setAnm(DRAW_TYPE_RED, mAttParam.field_0x3c);
        draw[0].mCursorSizeX = mAttParam.mAttnCursorScaleX;
        draw[0].mCursorSizeY = mAttParam.mAttnCursorScaleY;
        draw[0].mCursorOffsetY = mAttParam.mAttnCursorOffsetY;
        draw[0].field_0x175 = 1;

        if (!dComIfGp_checkPlayerStatus0(0, 0x36a02311) ||
            dComIfGp_checkPlayerStatus1(0, 0x11)) {
            lockSoundStart(Z2SE_SY_L_FOCUS_SET);
        }
    } else if (chkFlag(0x10)) {
        draw[0].setAlphaAnm(mAttParam.mAttnCursorDisappearFrames, 1);
        if (field_0x328 >= 0) {
            field_0x328 = 1;
            setFlag(0x40000000);
        }

        if (!dComIfGp_checkPlayerStatus0(0, 0x36a02311) ||
            dComIfGp_checkPlayerStatus1(0, 0x11)) {
            lockSoundStart(Z2SE_SY_L_FOCUS_RESET);
        }
    } else if (chkFlag(0x1)) {
        draw[0].setAnm(DRAW_TYPE_YELLOW, mAttParam.field_0x38);
        draw[0].setAlphaAnm(mAttParam.mAttnCursorAppearFrames, 0);

        draw[0].mCursorSizeX = mAttParam.mSelCursorScaleX;
        draw[0].mCursorSizeY = mAttParam.mSelCursorScaleY;
        draw[0].mCursorOffsetY = mAttParam.mSelCursorOffsetY;
        draw[0].field_0x175 = 0;
        setFlag(0x40000000);
    } else if (chkFlag(0x2)) {
        draw[0].setAlphaAnm(mAttParam.mAttnCursorAppearFrames, 0);
        draw[1].setAnm(DRAW_TYPE_YELLOW, mAttParam.field_0x38);
        draw[1].setAlphaAnm(mAttParam.mAttnCursorDisappearFrames, 1);

        draw[1].mCursorSizeX = mAttParam.mSelCursorScaleX;
        draw[1].mCursorSizeY = mAttParam.mSelCursorScaleY;
        draw[1].mCursorOffsetY = mAttParam.mSelCursorOffsetY;
        draw[1].field_0x175 = 0;
        setFlag(0x40000000);
    } else if (mLockonCount <= 0 && field_0x328 == 0) {
        draw[0].setAlphaAnm(mAttParam.mAttnCursorDisappearFrames, 1);
        field_0x328 = 1;
        setFlag(0x40000000);
    }

    if (mAttnStatus == EState_LOCK) {
        if (draw[0].field_0x173 == 3) {
            draw[0].field_0x173 = 4;
            clrFlag(0x40000000);
        }
    } else if (draw[0].field_0x173 == 3) {
        draw[0].field_0x173 = 4;
        clrFlag(0x40000000);
        field_0x328 = -1;
    }
}

#if DEBUG
void dAttention_c::runDebugDisp0() {
    f32 analog_l = mDoCPd_c::getAnalogL(mPadNo);
    if (mAttParam.CheckFlag(0x8000)) {
        dDbVw_Report(mAttParam.mDebugDispPosX, mAttParam.mDebugDispPosY, "attention %d/%d", mLockOnOffset, mLockonCount);
        dDbVw_Report(mAttParam.mDebugDispPosX, mAttParam.mDebugDispPosY + 20, "L %f (%d:%d)", analog_l, padLockTrigger(mPadNo), mDoCPd_c::getHoldL(mPadNo));
        dDbVw_Report(mAttParam.mDebugDispPosX, mAttParam.mDebugDispPosY + 40, "LS %d  stick.y = %f", mAttnStatus, mDoCPd_c::getStickY(mPadNo));

        if (LockonTarget(0)) {
            dDbVw_Report(mAttParam.mDebugDispPosX, mAttParam.mDebugDispPosY + 60, "LTN %s", dStage_getName(fopAcM_GetProfName(LockonTarget(0)), -1));
        }
    }
}
#endif

void dAttention_c::runDebugDisp() {
#if DEBUG
    if (mAttParam.CheckFlag(0x4000)) {
        int i;
        int print_y = 100;
        int no = 0;

        char* type_str[] = {
            "LOCK",
            "TALK",
            "BATL",
            "SPEK",
            "CARY",
            "DOOR",
            "JUEL",
            "ETC?",
            "CHEK",
            "????",
        };

        dDbVw_Report(10, print_y, " **** attention **** %03d", field_0x32e);
        dDbVw_Report(10, print_y += 15, " No TYPE WEIGHT PID NAME");

        if (mLockonCount != 0) {
            for (i = 0; i < mLockonCount; i++) {
                char name[16];
                fopAcM_getNameString(mLockOnList[i].getActor(), name);
                
                char sp9;
                if (i == mLockOnOffset) {
                    sp9 = '*';
                } else {
                    sp9 = ' ';
                }

                dDbVw_Report(10, print_y += 15, "%c%02d %s %06.1f %3d %s", sp9, no, type_str[mLockOnList[i].mType], mLockOnList[i].mWeight, mLockOnList[i].getPid(), name);
                no++;
            }

            print_y += 15;
        }

        if (mActionCount != 0) {
            for (i = 0; i < mActionCount; i++) {
                char name[16];
                fopAcM_getNameString(mActionList[i].getActor(), name);
                dDbVw_Report(10, print_y += 15, " %02d %s %06.1f %3d %s", no, type_str[mActionList[i].mType], mActionList[i].mWeight, mActionList[i].getPid(), name);
                no++;
            }
        }

        if (mCheckObjectCount != 0) {
            for (i = 0; i < mCheckObjectCount; i++) {
                char name[16];
                fopAcM_getNameString(mCheckObjectList[i].getActor(), name);
                
                char sp8;
                if (i == mCheckObjectOffset) {
                    sp8 = '+';
                } else {
                    sp8 = ' ';
                }

                dDbVw_Report(10, print_y += 15, "%c%02d %s %06.1f %3d %s", sp8, no, type_str[mCheckObjectList[i].mType], mCheckObjectList[i].mAngle.Degree(), mCheckObjectList[i].getPid(), name);
                no++;
            }
        }

        if (no == 0) {
            dDbVw_Report(10, print_y += 15, " (EMPTY)");
        }
    }
#endif
}

void dAttention_c::checkButton() {
    if (l_isFinalBossStg && dComIfGp_roomControl_getStayNo() == 0 &&
        !dComIfGs_isSaveDunSwitch(1)) {
        if (field_0x32b == 1 && LockonTarget(0) != NULL && chkFlag(0x20000000)) {
            setFlag(0x1000000);
        }
    }

    if (dComIfGp_checkPlayerStatus0(0, 0x36a02311) || dComIfGp_checkPlayerStatus1(0, 0x11)) {
        switch (field_0x32b) {
        case 0:
        case 1:
            clrFlag(0x1000000);
            field_0x32b = 4;
            break;
        }

        return;
    }

    switch (field_0x32b) {
    case 4:
        if (padLockButton(mPadNo) || chkFlag(0x1000000)) {
            field_0x32b = 0;
            setFlag(0x200);
            clrFlag(0x1000000);
        }
        return;
    case 0:
        field_0x32b = 1;
    case 1:
        if (chkFlag(0x1000000)) {
            field_0x32b = 0;
            field_0x32e = 0x3C;
            setFlag(0x200);
            clrFlag(0x1000000);
            return;
        }

        if (!padLockButton(mPadNo)) {
            field_0x32b = 4;
        }
    }
}

bool dAttention_c::triggerProc() {
    if (chaseAttention()) {
        setFlag(0x8);
        return true;
    }

    return false;
}

int dAttention_c::lostCheck() {
    if (chaseAttention()) {
        return false;
    }

    setFlag(0x10);
    setFlag(0x40);
    return true;
}

void dAttention_c::judgementStatus4Hold() {
    int var_r29 = mAttnStatus;
    int var_r30 = -1;

    switch (mAttnStatus) {
    case EState_NONE:
        var_r30 = 10;
        field_0x32f = 0;
        field_0x32e = 0;
        mLockTargetID = fpcM_ERROR_PROCESS_ID_e;
        stockAttention();

        if (field_0x32b == 0 && triggerProc()) {
            mAttnStatus = EState_LOCK;
            field_0x32e = 15;
            var_r30 = 11;
        }
        break;
    case EState_LOCK:
        mLockTargetID = LockonTargetPId(0);
        field_0x32f = 0;
        var_r30 = 20;

        if (lostCheck()) {
            mLockTargetID = fpcM_ERROR_PROCESS_ID_e;
            stockAttention();

            if (triggerProc()) {
                mAttnStatus = EState_LOCK;
            } else {
                mAttnStatus = EState_NONE;
                freeAttention();
                setFlag(0x800000);
            }
            var_r30 = 21;
        } else if (field_0x32b == 4) {
            mAttnStatus = EState_RELEASE;
            setFlag(0x10);
            field_0x32f = 10;
            var_r30 = 22;
        } else if (field_0x32e == 0) {
            initList(0xFFFFFFFF);

            if (makeList()) {
                sortList();
            }
            var_r30 = 23;
        }

        if (field_0x32e != 0) {
            field_0x32e--;
        }

        break;
    case EState_RELEASE:
        setFlag(0x40);
        var_r30 = 30;

        if (lostCheck()) {
            mLockTargetID = fpcM_ERROR_PROCESS_ID_e;
            stockAttention();

            if (triggerProc()) {
                mAttnStatus = EState_LOCK;
            } else {
                mAttnStatus = EState_NONE;
                freeAttention();
                setFlag(0x800000);
            }
            var_r30 = 31;
        } else if (field_0x32b == 0) {
            if (nextAttention()) {
                setFlag(0x8);
                mAttnStatus = EState_LOCK;
                field_0x32e = 15;
            } else {
                mAttnStatus = EState_NONE;
                freeAttention();
            }
            var_r30 = 32;
        } else if (LockonTarget(0) == NULL || field_0x32f == 0) {
            mAttnStatus = EState_NONE;
            freeAttention();
            var_r30 = 33;
        }

        if (field_0x32f != 0) {
            field_0x32f--;
        }

        break;
    }

    if (!chkFlag(0x1000)) {
        setFlag(0x4000);
        initList(0xFFFFFFFF);

        if (makeList()) {
            sortList();
        }
    }

    if (var_r29 != mAttnStatus) {
        OS_REPORT("%06d: attention: status %d, %d -> %d (%d)\n", g_Counter.mCounter0, mLockonCount, var_r29, mAttnStatus, var_r30);
    }
}

void dAttention_c::judgementStatus4Switch() {
    int var_r29 = mAttnStatus;
    int var_r30 = -1;

    switch (mAttnStatus) {
    case EState_NONE:
        mLockTargetID = fpcM_ERROR_PROCESS_ID_e;
        stockAttention();
        field_0x32f = 0;
        field_0x32e = 0;
        var_r30 = 10;

        if (field_0x32b == 0 && triggerProc()) {
            mAttnStatus = EState_LOCK;
            field_0x32e = 15;
            field_0x32f = 15;
            var_r30 = 11;
        }
        break;
    case EState_LOCK:
        mLockTargetID = LockonTargetPId(0);
        var_r30 = 20;

        if (field_0x32f == 0) {
            mAttnStatus = EState_NONE;
            freeAttention();
            var_r30 = 29;
        } else if (lostCheck()) {
            mLockTargetID = fpcM_ERROR_PROCESS_ID_e;
            stockAttention();

            if (triggerProc()) {
                mAttnStatus = EState_LOCK;
            } else {
                mAttnStatus = EState_NONE;
                freeAttention();
                setFlag(0x800000);
            }
            var_r30 = 21;
        } else if (field_0x32b == 0) {
            if (mDoCPd_c::getStickY(mPadNo) < mAttParam.FreeStick()) {
                mAttnStatus = EState_NONE;
                freeAttention();
            } else if (nextAttention()) {
                setFlag(0x8);
                mAttnStatus = EState_LOCK;
                field_0x32e = 15;
            } else {
                mAttnStatus = EState_NONE;
                freeAttention();
            }
            var_r30 = 22;
        } else if (field_0x32e == 0) {
            initList(0xFFFFFFFF);

            if (makeList()) {
                sortList();
            }
            var_r30 = 23;
        }

        if (!chkFlag(0x8) && field_0x32e != 0) {
            field_0x32e--;
        }

        if (field_0x32b == 1) {
            if (field_0x32f != 0) {
                field_0x32f--;
            }
        } else {
            field_0x32f = 15;
        }

        break;
    case EState_RELEASE:
        setFlag(0x40);
        var_r30 = 30;

        if (lostCheck()) {
            mLockTargetID = fpcM_ERROR_PROCESS_ID_e;
            stockAttention();

            if (triggerProc()) {
                mAttnStatus = EState_LOCK;
            } else {
                mAttnStatus = EState_NONE;
                freeAttention();
                setFlag(0x800000);
            }
            var_r30 = 31;
        } else if (field_0x32b == 0) {
            mAttnStatus = EState_NONE;

            if (triggerProc()) {
                mAttnStatus = EState_LOCK;
                field_0x32e = 15;
                field_0x32f = 15;
            }
            var_r30 = 32;
        } else if (LockonTarget(0) == NULL || field_0x32f == 0) {
            mAttnStatus = EState_NONE;
            freeAttention();
            var_r30 = 33;
        }

        break;
    }

    if (!chkFlag(0x1000)) {
        setFlag(0x4000);
        initList(0xFFFFFFFF);

        if (makeList()) {
            sortList();
        }
    }

    if (var_r29 != mAttnStatus) {
        OS_REPORT("%06d: attention: status %d, %d -> %d (%d)\n", g_Counter.mCounter0, mLockonCount, var_r29, mAttnStatus, var_r30);
    }
}

int dAttention_c::Run() {
    clrFlag(0x3FFFFF);

    bool usingHold = dComIfGs_getOptAttentionType() == 0;
    if (attn_opt_hold != usingHold) {
        setFlag(0x10000);
    }

    attn_opt_hold = usingHold;

#if DEBUG
    if (mAttParam.CheckFlag(4)) {
        attn_opt_hold = mAttParam.CheckFlag(1);
    }
#endif

    if (mAttnBlockTimer != 0) {
        mAttnBlockTimer--;
        return 1;
    }

    if (chkFlag(0x80)) {
        mpPlayer = (fopAc_ac_c*) dComIfGp_getPlayer(0);
        mPadNo = PAD_1;
    }

#if DEBUG
    runDebugDisp0();
#endif

    setOwnerAttentionPos();

    if (dComIfGp_event_runCheck() || chkFlag(0x10000)) {
        mAttnStatus = EState_NONE;
        field_0x32b = 4;
        field_0x32c = 0;
        clrFlag(0x20000000);
        clrFlag(0x10000000);
        mLockTargetID = fpcM_ERROR_PROCESS_ID_e;
        freeAttention();
    } else {
        checkButton();

        if (attn_opt_hold) {
            judgementStatus4Hold();
        } else {
            judgementStatus4Switch();
        }

        if (chkFlag(0x800000)) {
            freeAttention();
            if (LockonTarget(0) == NULL) {
                setFlag(0x20000020);
            }
            setFlag(0x10000000);
            clrFlag(0x800000);
        } else if (chkFlag(0x10000000)) {
            if (!padLockButton(mPadNo)) {
                if (chkFlag(0x20000000)) {
                    lockSoundStart(Z2SE_SY_CAMERA_L_CANCEL);
                    clrFlag(0x20000000);
                }

                clrFlag(0x10000000);
            }
        } else {
            if (padLockButton(mPadNo)) {
                if (LockonTarget(0) == NULL) {
                    lockSoundStart(Z2SE_SY_CAMERA_L_MOVE);
                    setFlag(0x20000020);
                }

                setFlag(0x10000000);
            }
        }
    }

    field_0x32a = mAttnStatus;

    runSoundProc();
    runDrawProc();
    runDebugDisp();

    if (mAttnStatus == EState_LOCK) {
        dComIfGp_onCameraAttentionStatus(mPadNo, 1);
    } else {
        dComIfGp_offCameraAttentionStatus(mPadNo, 1);
    }

    mZHintTarget.proc();
    mCatghTarget.proc();
    mLookTarget.proc();

    return 1;
}

void dAttention_c::Draw() {
    if (mAttParam.CheckFlag(dAttParam_c::EFlag_ARROW_OFF)) {
        draw[0].field_0x173 = 3;
        draw[1].field_0x173 = 3;
        return;
    }

    Mtx inv_m;
    cMtx_inverse(dComIfGd_getViewRotMtx(), inv_m);
    fopAc_ac_c* lockon_actor = LockonTarget(0);

    if (!dComIfGp_event_runCheck()) {
        #if DEBUG
        if (mAttParam.CheckFlag(4) && mAttParam.CheckFlag(0x10)) {
            return;
        }
        #endif

        if (lockon_actor != NULL) {
            #if DEBUG
            if (mAttParam.CheckFlag(8)) {
                dDbVw_Report(300, 40, "000");
            }
            #endif

            draw[0].draw(lockon_actor->attention_info.position, inv_m);

            if (mLockonCount >= 2 && draw[1].field_0x173 == 2) {
                int listIdx = mLockOnOffset;
                if (mLockOnOffset == 0) {
                    listIdx = mLockonCount - 1;
                } else {
                    listIdx--;
                }

                if (mLockOnList[listIdx].getActor() != NULL) {
                    draw[1].draw(mLockOnList[listIdx].getActor()->attention_info.position, inv_m);

                    #if DEBUG
                    if (mAttParam.CheckFlag(8)) {
                        dDbVw_Report(300, 40, "111");
                    }
                    #endif
                }
            }

            mTargetActorID = fopAcM_GetID(lockon_actor);
            mDrawAttnPos = lockon_actor->attention_info.position;
            field_0x328 = 0;
        } else if (field_0x328 > 0) {
            fopAc_ac_c* actor = fopAcM_SearchByID(mTargetActorID);

            if (actor != NULL) {
                draw[0].draw(actor->attention_info.position, inv_m);
                mDrawAttnPos = actor->attention_info.position;
            } else {
                draw[0].draw(mDrawAttnPos, inv_m);
            }

            #if DEBUG
            if (mAttParam.CheckFlag(8)) {
                dDbVw_Report(300, 40, "222");
            }
            #endif
        }
    }
}

void dAttention_c::lockSoundStart(u32 i_sfxID) {
    if (!chkFlag(0x400000)) {
        mDoAud_seStart(i_sfxID, NULL, 0, 0);
    }
}

void dAttDraw_c::setAnm(u8 i_drawType, f32 i_anmSpeed) {
    mDrawType = i_drawType;
    mNoticeCursorBck[mDrawType].reset();
    mNoticeCursorBck[mDrawType].setPlaySpeed(i_anmSpeed);
    mNoticeCursorBpk[mDrawType].reset();
    mNoticeCursorBpk[mDrawType].setPlaySpeed(i_anmSpeed);
    mNoticeCursorBrk[mDrawType].reset();
    mNoticeCursorBrk[mDrawType].setPlaySpeed(i_anmSpeed);
    mNoticeCursor02Brk[mDrawType].reset();
    mNoticeCursor02Brk[mDrawType].setPlaySpeed(i_anmSpeed);
    mNoticeCursorBtk[mDrawType].reset();
    mNoticeCursorBtk[mDrawType].setPlaySpeed(i_anmSpeed);

    if (mDrawType == DRAW_TYPE_RED) {
        mImpactBck.reset();
        mImpactBpk.reset();
        mImpactBrk.reset();
        mImpactBtk.reset();
    }
}

void dAttDraw_c::setAlphaAnm(u8 i_frameMax, u8 i_playDirection) {
    mAlphaAnmFrameMax = i_frameMax;
    mAlphaAnmFrame = 0;
    field_0x173 = 2;
    mAlphaAnmPlayDirection = i_playDirection;
}

void dAttDraw_c::alphaAnm() {
    f32 anm_ratio = (f32)mAlphaAnmFrame / (f32)mAlphaAnmFrameMax;
    f32 alpha_ratio;
    if (mAlphaAnmPlayDirection == 1) {
        alpha_ratio = 1.0f - anm_ratio;
    } else {
        alpha_ratio = anm_ratio;
    }

    if (anm_ratio == 1.0f) {
        if (mDrawType == DRAW_TYPE_RED && mAlphaAnmPlayDirection == 0) {
            if (mImpactBck.isStop() && mImpactBpk.isStop() && mImpactBrk.isStop() && mImpactBtk.isStop()) {
                field_0x175 = 0;
                field_0x173 = 3;
            }
        } else {
            field_0x173 = 3;
        }
    }

    if (mAlphaAnmFrame < mAlphaAnmFrameMax) {
        mAlphaAnmFrame++;
    }

    J3DModelData* modelData = mModel[mDrawType]->getModelData();
    for (int i = 0; i < modelData->getMaterialNum(); i++) {
        J3DMaterial* material = modelData->getMaterialNodePointer(i);
        J3DGXColor* color = (J3DGXColor*)material->getTevKColor(0);
        color->a = 255.0f * alpha_ratio;
        material->setTevKColor(0, color);
    }

    if (field_0x175 == 0) {
        mNoticeCursorBck[mDrawType].play();
        mNoticeCursorBpk[mDrawType].play();
        mNoticeCursorBrk[mDrawType].play();
        mNoticeCursorBtk[mDrawType].play();
        mNoticeCursor02Brk[mDrawType].play();
    } else {
        mImpactBck.play();
        mImpactBpk.play();
        mImpactBrk.play();
        mImpactBtk.play();
    }
}

void dAttDraw_c::draw(cXyz& i_pos, Mtx i_mtx) {
    J3DModelData* modelData = mModel[mDrawType]->getModelData();

    mDoMtx_stack_c::transS(i_pos.x, i_pos.y + mCursorOffsetY, i_pos.z);
    mDoMtx_stack_c::concat(i_mtx);
    mModel[mDrawType]->setBaseTRMtx(mDoMtx_stack_c::get());

    view_class* view = dComIfGd_getView();
    f32 fovy = view->fovy;
    f32 temp_f31 = std::tan(DEG_TO_RAD(0.5f * fovy));
    f32 temp_f30 = (-100.0f + -g_AttDwHIO.mCursorDistance) / temp_f31;

    cXyz view_pos;
    cMtx_multVec(dComIfGd_getViewMtx(), &i_pos, &view_pos);

    f32 size = 1.0f;
    if (view_pos.z < temp_f30) {
        size = (view_pos.z * temp_f31) / (-100.0f + -g_AttDwHIO.mCursorDistance);
    }

    cXyz scale(mCursorSizeX * size, mCursorSizeY * size, mCursorSizeX * size);
    mModel[mDrawType]->setBaseScale(scale);

    alphaAnm();

    if (field_0x175 != 0) {
        mImpactBck.entry(modelData);
        mImpactBpk.entry(modelData);
        mImpactBrk.entry(modelData);
        mImpactBtk.entry(modelData);
    } else {
        mNoticeCursorBck[mDrawType].entry(modelData);
        mNoticeCursorBpk[mDrawType].entry(modelData);
        mNoticeCursorBrk[mDrawType].entry(modelData);
        mNoticeCursorBtk[mDrawType].entry(modelData);
        mNoticeCursor02Brk[mDrawType].entry(modelData);
    }

    dComIfGd_setList3Dlast();
    mDoExt_modelUpdateDL(mModel[mDrawType]);
    dComIfGd_setList();
}

fopAc_ac_c* dAttention_c::LockonTarget(s32 i_no) {
    if (dComIfGp_checkPlayerStatus0(0, 0x36A02311) || dComIfGp_checkPlayerStatus1(0, 0x11)) {
        return NULL;
    }

    if (i_no >= mLockonCount) {
        return NULL;
    }

    int listIdx = mLockOnOffset + i_no;
    if (listIdx >= mLockonCount) {
        listIdx -= mLockonCount;
    }

    fopAc_ac_c* actor = mLockOnList[listIdx].getActor();
    if (actor == NULL || !(actor->attention_info.flags & fopAc_AttnFlags_LOCK)) {
        return NULL;
    }

    return actor;
}

f32 dAttention_c::LockonReleaseDistanse() {
    if (!LockonTruth()) {
        return 0.0f;
    }

    fopAc_ac_c* actor = mLockOnList[mLockOnOffset].getActor();
    if (actor == NULL) {
        return 0.0f;
    }

    int idx =  actor->attention_info.distances[mLockOnList[mLockOnOffset].mType];
    cSGlobe globe(actor->attention_info.position - mOwnerAttnPos);
    cSAngle angle(globe.U() - fopAcM_GetShapeAngle_p(mpPlayer)->y);

    return distace_angle_adjust(dist_table[idx].mDistanceAdjust, angle, 1.0f) + dist_table[idx].mDistMaxRelease;
}

fpc_ProcID dAttention_c::LockonTargetPId(s32 i_no) {
    if (dComIfGp_checkPlayerStatus0(0, 0x36A02311) || dComIfGp_checkPlayerStatus1(0, 0x11)) {
        return fpcM_ERROR_PROCESS_ID_e;
    }

    if (i_no >= mLockonCount) {
        return fpcM_ERROR_PROCESS_ID_e;
    }

    int listIdx = mLockOnOffset + i_no;
    if (listIdx >= mLockonCount) {
        listIdx -= mLockonCount;
    }

    return mLockOnList[listIdx].getPid();
}

fopAc_ac_c* dAttention_c::ActionTarget(s32 i_no) {
    if (i_no >= mActionCount) {
        return NULL;
    }

    int listIdx = mActionOffset + i_no;
    if (listIdx >= mActionCount) {
        listIdx -= mActionCount;
    }

    return mActionList[listIdx].getActor();
}

fopAc_ac_c* dAttention_c::CheckObjectTarget(s32 i_no) {
    if (i_no >= mCheckObjectCount) {
        return NULL;
    }

    int listIdx = mCheckObjectOffset + i_no;
    if (listIdx >= mCheckObjectCount) {
        listIdx -= mCheckObjectCount;
    }

    return mCheckObjectList[listIdx].getActor();
}

bool dAttention_c::LockonTruth() {
    return (mAttnStatus == EState_LOCK || mAttnStatus == EState_RELEASE) && LockonTarget(0) != NULL;
}

int dAttention_c::checkDistance(cXyz* i_pos, s16 i_angle, cXyz* i_attnPos, f32 i_distMax, f32 i_distAdjust,
                                f32 i_max_y, f32 i_min_y) {
    return check_distace(i_pos, i_angle, i_attnPos, i_distMax, i_distAdjust, i_max_y, i_min_y);
}

fopAc_ac_c* dAttList_c::getActor() {
    return fopAcM_SearchByID(mActorID);
}

void dAttList_c::setActor(fopAc_ac_c* i_actor) {
    mActorID = fopAcM_GetID(i_actor);
}

fpc_ProcID dAttHint_c::getPId(void* i_actor) {
    return fopAcM_GetID(i_actor);
}

fopAc_ac_c* dAttHint_c::convPId(fpc_ProcID i_id) {
    return fopAcM_SearchByID(i_id);
}

int dAttHint_c::request(fopAc_ac_c* i_actor, int i_priority) {
    if (i_priority < 0) {
        i_priority = 0x1FF;
    }

    if (i_priority <= mPriority) {
        mHintActorID = getPId(i_actor);
        mPriority = i_priority;
    }

    return 1;
}

void dAttHint_c::init() {
    mHintActorID = fpcM_ERROR_PROCESS_ID_e;
    field_0x8 = fpcM_ERROR_PROCESS_ID_e;
    mPriority = 0x200;
}

void dAttHint_c::proc() {
    field_0x8 = mHintActorID;
    mHintActorID = fpcM_ERROR_PROCESS_ID_e;
    mPriority = 0x200;
}

fopAc_ac_c* dAttCatch_c::convPId(fpc_ProcID i_id) {
    return fopAcM_SearchByID(i_id);
}

void dAttCatch_c::init() {
    mCatchItemNo = fpcNm_ITEM_WATER_BOTTLE;
    mRequestActorID = fpcM_ERROR_PROCESS_ID_e;
    mCatghTargetID = fpcM_ERROR_PROCESS_ID_e;
    field_0x4 = 3;
}

void dAttCatch_c::proc() {
    mCatghTargetID = mRequestActorID;
    mChangeItem = mCatchItemNo;
    mRequestActorID = fpcM_ERROR_PROCESS_ID_e;
    field_0x4 = 3;
    mCatchItemNo = fpcNm_ITEM_WATER_BOTTLE;
}

int dAttCatch_c::request(fopAc_ac_c* i_reqActor, u8 i_itemNo, f32 i_horizontalDist, f32 i_upDist,
                         f32 i_downDist, s16 i_angle, int param_7) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (param_7 > field_0x4) {
        return 0;
    } 

    cXyz vec_to_player = i_reqActor->attention_info.position - player->attention_info.position;
    if (vec_to_player.y < i_downDist || vec_to_player.y > i_upDist) {
        return 0;
    }

    f32 player_xz_dist = vec_to_player.absXZ();
    if (player_xz_dist > i_horizontalDist) {
        return 0;
    }

    if (i_angle != 0) {
        cSGlobe globe(vec_to_player);
        s16 angle = globe.U() - fopAcM_GetShapeAngle_p(player)->y;
        if (angle < 0) {
            angle = -angle;
        }
        if (angle == -0x8000) {
            angle = 0x7fff;
        }
        if (angle > i_angle) {
            return 0;
        }
    }

    if (param_7 < field_0x4 || player_xz_dist < mDistance) {
        field_0x4 = param_7;
        mCatchItemNo = i_itemNo;
        mRequestActorID = fopAcM_GetID(i_reqActor);
        mDistance = player_xz_dist;
        return 1;
    }

    return 0;
}

fopAc_ac_c* dAttLook_c::convPId(fpc_ProcID i_id) {
    return fopAcM_SearchByID(i_id);
}

void dAttLook_c::init() {
    mRequestActorID = fpcM_ERROR_PROCESS_ID_e;
    mLookTargetID = fpcM_ERROR_PROCESS_ID_e;
    field_0x4 = 3;
}

void dAttLook_c::proc() {
    mLookTargetID = mRequestActorID;
    mRequestActorID = fpcM_ERROR_PROCESS_ID_e;
    field_0x4 = 3;
}

int dAttLook_c::request(fopAc_ac_c* i_reqActor, f32 i_horizontalDist, f32 i_upDist, f32 i_downDist,
                        s16 i_angle, int param_6) {
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    if (param_6 > field_0x4) {
        return 0;
    } 

    cXyz vec_to_player = i_reqActor->eyePos - player->eyePos;
    if (vec_to_player.y < i_downDist || vec_to_player.y > i_upDist) {
        return 0;
    }

    f32 player_xz_dist = vec_to_player.absXZ();
    if (player_xz_dist > i_horizontalDist) {
        return 0;
    }

    if (i_angle != 0) {
        vec_to_player = i_reqActor->current.pos - player->current.pos;
        cSGlobe globe(vec_to_player);
        s16 angle = globe.U() - fopAcM_GetShapeAngle_p(player)->y;
        if (angle < 0) {
            angle = -angle;
        }
        if (angle == -0x8000) {
            angle = 0x7fff;
        }
        if (angle > i_angle) {
            return 0;
        }
    }

    if (param_6 < field_0x4 || player_xz_dist < mDistance) {
        field_0x4 = param_6;
        mRequestActorID = fopAcM_GetID(i_reqActor);
        mDistance = player_xz_dist;
        return 1;
    }

    return 0;
}
