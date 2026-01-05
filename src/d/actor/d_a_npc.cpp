#include "d/dolzel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc.h"

#include "d/actor/d_a_npc_tk.h"
#include "d/actor/d_a_tag_evtarea.h"
#include "d/d_debug_viewer.h"
#include "d/d_k_wmark.h"
#include "d/d_msg_object.h"
#include "SSystem/SComponent/c_math.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_kankyo_mng.h"
#include "m_Do/m_Do_lib.h"

#if DEBUG
void daNpcT_cmnListenPropertyEvent(char* param_0, int* param_1, daNpcT_HIOParam* param_2) {
    sprintf(&param_0[*param_1], "%.3ff,\t//  注目オフセット\n", param_2->attention_offset);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  重力\n", param_2->gravity);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  スケ−ル\n", param_2->scale);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  リアル影サイズ\n", param_2->real_shadow_size);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  体重\n", param_2->weight);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  高さ\n", param_2->height);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  ひざ丈\n", param_2->knee_length);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  幅\n", param_2->width);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  腰のＸ角上限\n", param_2->body_angleX_max);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  腰のＸ角下限\n", param_2->body_angleX_min);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  腰のＹ角上限\n", param_2->body_angleY_max);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  腰のＹ角下限\n", param_2->body_angleY_min);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  頭のＸ角上限\n", param_2->head_angleX_max);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  頭のＸ角下限\n", param_2->head_angleX_min);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  頭のＹ角上限\n", param_2->head_angleY_max);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  頭のＹ角下限\n", param_2->head_angleY_min);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  首の移動割合\n", param_2->neck_rotation_ratio);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  補間フレ−ム\n", param_2->morf_frame);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%d,   \t//  会話距離\n", param_2->talk_distance);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%d,   \t//  会話角度\n", param_2->talk_angle);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%d,   \t//  注目距離\n", param_2->attention_distance);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%d,   \t//  注目角度\n", param_2->attention_angle);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  視界\n", param_2->fov);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  サ−チ距離\n", param_2->search_distance);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  サ−チ高さ\n", param_2->search_height);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  サ−チ低さ\n", param_2->search_depth);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%d,   \t//  ？\n", param_2->attention_time);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%d,   \t//  ？\n", param_2->damage_time);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%d,   \t//  表情\n", param_2->face_expression);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%d,   \t//  動作\n", param_2->motion);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%d,   \t//  注視モ−ド\n", param_2->look_mode);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%d,   \t//  デバグモ−ドＯＮ\n", param_2->debug_mode_ON);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%d,   \t//  デバグ情報ＯＮ\n", param_2->debug_info_ON);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  表情補間フレ−ム\n", param_2->expression_morf_frame);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  箱最小ｘ\n", param_2->box_min_x);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  箱最小ｙ\n", param_2->box_min_y);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  箱最小ｚ\n", param_2->box_min_z);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  箱最大ｘ\n", param_2->box_max_x);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  箱最大ｙ\n", param_2->box_max_y);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  箱最大ｚ\n", param_2->box_max_z);
    *param_1 = strlen(param_0);

    sprintf(&param_0[*param_1], "%.3ff,\t//  箱オフセット\n", param_2->box_offset);
    *param_1 = strlen(param_0);
}

void daNpcT_cmnGenMessage(JORMContext* ctx, daNpcT_HIOParam* i_hioParam) {
    ctx->genSlider("注目オフセット  ", &i_hioParam->attention_offset,
                   0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("重力            ", &i_hioParam->gravity,
                   -100.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("スケ−ル        ", &i_hioParam->scale,
                   0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("リアル影サイズ  ", &i_hioParam->real_shadow_size,
                   0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("体重            ", &i_hioParam->weight,
                   0.0f, 255.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("高さ            ", &i_hioParam->height,
                   0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("ひざ丈          ", &i_hioParam->knee_length,
                   0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("幅              ", &i_hioParam->width,
                   0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("腰のＸ角上限    ", &i_hioParam->body_angleX_max, -90.0f, 90.0f, 0, NULL,
                   0xffff, 0xffff, 512, 24);
    ctx->genSlider("腰のＸ角下限    ", &i_hioParam->body_angleX_min, -90.0f, 90.0f, 0, NULL,
                   0xffff, 0xffff, 512, 24);
    ctx->genSlider("腰のＹ角上限    ", &i_hioParam->body_angleY_max, -180.0f, 179.0f, 0, NULL,
                   0xffff, 0xffff, 512, 24);
    ctx->genSlider("腰のＹ角下限    ", &i_hioParam->body_angleY_min, -180.0f, 179.0f, 0, NULL,
                   0xffff, 0xffff, 512, 24);
    ctx->genSlider("頭のＸ角上限    ", &i_hioParam->head_angleX_max, -90.0f, 90.0f, 0, NULL,
                   0xffff, 0xffff, 512, 24);
    ctx->genSlider("頭のＸ角下限    ", &i_hioParam->head_angleX_min, -90.0f, 90.0f, 0, NULL,
                   0xffff, 0xffff, 512, 24);
    ctx->genSlider("頭のＹ角上限    ", &i_hioParam->head_angleY_max, -180.0f, 179.0f, 0, NULL,
                   0xffff, 0xffff, 512, 24);
    ctx->genSlider("頭のＹ角下限    ", &i_hioParam->head_angleY_min, -180.0f, 179.0f, 0, NULL,
                   0xffff, 0xffff, 512, 24);
    ctx->genSlider("首の移動割合    ", &i_hioParam->neck_rotation_ratio,
                   0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("補間フレ－ム    ", &i_hioParam->morf_frame,
                   0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->startComboBox("会話距離        ", &i_hioParam->talk_distance,
                   0, NULL, 0xffff, 0xffff, 0x100, 26);
    ctx->genComboBoxItem("  50", 0);
    ctx->genComboBoxItem(" 100", 1);
    ctx->genComboBoxItem(" 150", 2);
    ctx->genComboBoxItem(" 200", 3);
    ctx->genComboBoxItem(" 250", 4);
    ctx->genComboBoxItem(" 300", 5);
    ctx->genComboBoxItem(" 350", 6);
    ctx->genComboBoxItem(" 400", 7);
    ctx->genComboBoxItem(" 450", 8);
    ctx->genComboBoxItem(" 500", 9);
    ctx->genComboBoxItem(" 550", 10);
    ctx->genComboBoxItem(" 600", 11);
    ctx->genComboBoxItem(" 650", 12);
    ctx->genComboBoxItem(" 700", 13);
    ctx->genComboBoxItem(" 750", 14);
    ctx->genComboBoxItem(" 800", 15);
    ctx->genComboBoxItem(" 850", 16);
    ctx->genComboBoxItem(" 900", 17);
    ctx->genComboBoxItem(" 950", 18);
    ctx->genComboBoxItem("1000", 19);
    ctx->endComboBox();
    ctx->startComboBox("会話角度        ", &i_hioParam->talk_angle,
                       0, NULL, 0xffff, 0xffff, 0x100, 26);
    ctx->genComboBoxItem("  30", 0);
    ctx->genComboBoxItem("  45", 1);
    ctx->genComboBoxItem("  60", 2);
    ctx->genComboBoxItem("  90", 3);
    ctx->genComboBoxItem(" 110", 4);
    ctx->genComboBoxItem(" 135", 5);
    ctx->genComboBoxItem(" 180", 6);
    ctx->endComboBox();
    ctx->startComboBox("注目距離        ", &i_hioParam->attention_distance,
                       0, NULL, 0xffff, 0xffff, 0x100, 26);
    ctx->genComboBoxItem("  50", 0);
    ctx->genComboBoxItem(" 100", 1);
    ctx->genComboBoxItem(" 150", 2);
    ctx->genComboBoxItem(" 200", 3);
    ctx->genComboBoxItem(" 250", 4);
    ctx->genComboBoxItem(" 300", 5);
    ctx->genComboBoxItem(" 350", 6);
    ctx->genComboBoxItem(" 400", 7);
    ctx->genComboBoxItem(" 450", 8);
    ctx->genComboBoxItem(" 500", 9);
    ctx->genComboBoxItem(" 550", 10);
    ctx->genComboBoxItem(" 600", 11);
    ctx->genComboBoxItem(" 650", 12);
    ctx->genComboBoxItem(" 700", 13);
    ctx->genComboBoxItem(" 750", 14);
    ctx->genComboBoxItem(" 800", 15);
    ctx->genComboBoxItem(" 850", 16);
    ctx->genComboBoxItem(" 900", 17);
    ctx->genComboBoxItem(" 950", 18);
    ctx->genComboBoxItem("1000", 19);
    ctx->endComboBox();
    ctx->startComboBox("注目角度        ", &i_hioParam->attention_angle,
                       0, NULL, 0xffff, 0xffff, 0x100, 26);
    ctx->genComboBoxItem("  30", 0);
    ctx->genComboBoxItem("  45", 1);
    ctx->genComboBoxItem("  60", 2);
    ctx->genComboBoxItem("  90", 3);
    ctx->genComboBoxItem(" 110", 4);
    ctx->genComboBoxItem(" 135", 5);
    ctx->genComboBoxItem(" 180", 6);
    ctx->endComboBox();
    ctx->genSlider("視界            ", &i_hioParam->fov,
                   0.0f, 180.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("サ－チ距離      ", &i_hioParam->search_distance,
                   0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("サ－チ高さ      ", &i_hioParam->search_height,
                   -10000.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("サ－チ低さ      ", &i_hioParam->search_depth,
                   -10000.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("？              ", &i_hioParam->attention_time,
                   0, 10000, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("？              ", &i_hioParam->damage_time,
                   0, 10000, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("表情            ", &i_hioParam->face_expression,
                   0, 0xff, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("動作            ", &i_hioParam->motion,
                   0, 0xff, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("注視モ－ド      ", &i_hioParam->look_mode,
                   0, 0xff, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genCheckBox("デバグモ－ドＯＮ", &i_hioParam->debug_mode_ON, 1, 0, NULL,
                   0xffff, 0xffff, 512, 24);
    ctx->genCheckBox("デバグ情報ＯＮ  ", &i_hioParam->debug_info_ON, 1, 0, NULL,
                   0xffff, 0xffff, 512, 24);
    ctx->genSlider("表情補間フレ－ム", &i_hioParam->expression_morf_frame,
                   0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("箱最小ｘ        ", &i_hioParam->box_min_x,
                   -1000.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("箱最小ｙ        ", &i_hioParam->box_min_y,
                   -1000.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("箱最小ｚ        ", &i_hioParam->box_min_z,
                   -1000.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("箱最大ｘ        ", &i_hioParam->box_max_x,
                   -1000.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("箱最大ｙ        ", &i_hioParam->box_max_y,
                   -1000.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("箱最大ｚ        ", &i_hioParam->box_max_z,
                   -1000.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("箱オフセット    ", &i_hioParam->box_offset,
                   -1000.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
}
#endif

void daNpcT_ActorMngr_c::initialize() {
    mActorID = fpcM_ERROR_PROCESS_ID_e;
}

void daNpcT_ActorMngr_c::entry(fopAc_ac_c* i_actor) {
    mActorID = fopAcM_GetID(i_actor);
}

void daNpcT_ActorMngr_c::remove() {
    mActorID = fpcM_ERROR_PROCESS_ID_e;
}

fopAc_ac_c* daNpcT_ActorMngr_c::getActorP() {
    fopAc_ac_c* actor = NULL;

    s32 actor2 = fopAcM_SearchByID(mActorID, &actor);
    if (actor2 == 1 && actor != NULL && fopAcM_IsActor(actor)) {
        return actor;
    }

    return NULL;
}

void daNpcT_MatAnm_c::initialize() {
    field_0xF4 = 0.0f;
    field_0xF8 = 0.0f;
    mNowOffsetX = 0.0f;
    mNowOffsetY = 0.0f;
    mEyeMoveFlg = 0;
    mMorfFrm = 0;
}

void daNpcT_MatAnm_c::calc(J3DMaterial* i_material) const {
    J3DMaterialAnm::calc(i_material);

    for (u32 i = 0; i < 8; i++) {
        if (getTexMtxAnm(i).getAnmFlag()) {
            J3DTextureSRTInfo* curr_mtx_info =
                &i_material->getTexGenBlock()->getTexMtx(i)->getTexMtxInfo().mSRT;

            if (mEyeMoveFlg != 0) {
                curr_mtx_info->mTranslationX = mNowOffsetX;
                curr_mtx_info->mTranslationY = mNowOffsetY;
            }

            if (mMorfFrm != 0) {
                f32 tmp8 = 1.0f / (mMorfFrm + 1);

                curr_mtx_info->mTranslationX =
                    field_0xF4 * (1.0f - tmp8) + curr_mtx_info->mTranslationX * tmp8;
                curr_mtx_info->mTranslationY =
                    field_0xF8 * (1.0f - tmp8) + curr_mtx_info->mTranslationY * tmp8;
            }

            field_0xF4 = curr_mtx_info->mTranslationX;
            field_0xF8 = curr_mtx_info->mTranslationY;
        }
    }
}

void daNpcT_MotionSeqMngr_c::initialize() {
    mNo = 0;
    mOffset = 0;
    mStepNo = 0;
    mPrevStepNo = -1;
    mInitialMorf = -1.0f;
    mEndSequence = false;
}

int daNpcT_MotionSeqMngr_c::play(u16 i_loopNo, int* o_motionNo_p, f32* o_morfFrm_p) {
    int ret = 0;
    JUT_ASSERT(471, NULL != o_motionNo_p);
    JUT_ASSERT(472, NULL != o_morfFrm_p);

    if (mPrevStepNo == mStepNo && mStepNo < mStepNum) {
        if ((&mpSeqData[mNo * mStepNum])[mStepNo].mAnmIdx != -1) {
            if ((&mpSeqData[mNo * mStepNum])[mStepNo].mLoopCount > 0
                && (&mpSeqData[mNo * mStepNum])[mStepNo].mLoopCount <= i_loopNo)
            {
                mStepNo++;
            } else if ((&mpSeqData[mNo * mStepNum])[mStepNo].mLoopCount == 0 && i_loopNo != 0) {
                mEndSequence = true;
            }
        }
    }

    if (mPrevStepNo != mStepNo && mStepNo < mStepNum) {
        if ((&mpSeqData[mNo * mStepNum])[mStepNo].mAnmIdx != -1) {
            *o_motionNo_p = (&mpSeqData[mNo * mStepNum])[mStepNo].mAnmIdx;
            *o_morfFrm_p = (&mpSeqData[mNo * mStepNum])[mStepNo].mMorf;

            if (mPrevStepNo == -1 && -1.0f < mInitialMorf) {
                *o_morfFrm_p = mInitialMorf;
            }
            ret = 1;
        } else {
            mEndSequence = true;
        }
    }

    mPrevStepNo = mStepNo;
    return ret;
}

s32 daNpcT_MotionSeqMngr_c::checkEndSequence() {
    return (u8)mEndSequence == true;
}

static BOOL daNpcT_addIdx(int i_step, int i_num, u16& i_idx, BOOL i_closed) {
    BOOL rt = FALSE;
    int new_idx = i_idx;
    new_idx += i_step;

    if (i_num <= new_idx) {
        if (i_closed) {
            new_idx %= i_num;
        } else {
            new_idx = i_num - 1;
            rt = TRUE;
        }
    }

    i_idx = new_idx;
    return rt;
}

static BOOL daNpcT_subIdx(int i_step, int i_num, u16& i_idx, BOOL i_closed) {
    BOOL rt = FALSE;
    int new_idx = i_idx;
    new_idx -= i_step;

    if (0 > new_idx) {
        if (i_closed) {
            new_idx = (i_num + new_idx % i_num) % i_num;
        } else {
            new_idx = 0;
            rt = TRUE;
        }
    }

    i_idx = new_idx;
    return rt;
}

static int daNpcT_incIdx(int i_num, u16& i_idx, BOOL i_closed, int i_direction) {
    int result = 0;
    if (i_direction) {
        result = daNpcT_subIdx(1, i_num, i_idx, i_closed);
    } else {
        result = daNpcT_addIdx(1, i_num, i_idx, i_closed);
    }
    return result;
}

static int daNpcT_decIdx(int i_num, u16& i_idx, BOOL i_closed, int i_direction) {
    int result = 0;
    if (i_direction) {
        result = daNpcT_addIdx(1, i_num, i_idx, i_closed);
    } else {
        result = daNpcT_subIdx(1, i_num, i_idx, i_closed);
    }
    return result;
}

void daNpcT_Path_c::hermite(cXyz& i_start, cXyz& i_startTan, cXyz& i_end, cXyz& i_endTan,
                            daNpcT_Hermite_c& i_hermite, cXyz& o_result) {
    o_result.x = i_start.x * i_hermite.GetH00() + i_end.x * i_hermite.GetH01() +
                 i_startTan.x * i_hermite.GetH10() + i_endTan.x * i_hermite.GetH11();

    o_result.y = i_start.y * i_hermite.GetH00() + i_end.y * i_hermite.GetH01() +
                 i_startTan.y * i_hermite.GetH10() + i_endTan.y * i_hermite.GetH11();

    o_result.z = i_start.z * i_hermite.GetH00() + i_end.z * i_hermite.GetH01() +
                 i_startTan.z * i_hermite.GetH10() + i_endTan.z * i_hermite.GetH11();
}

void daNpcT_Path_c::initialize() {
    mPathInfo = NULL;

    mPosition.setall(0.0f);
    field_0x10 = 0.0f;
    field_0x14 = 0.0f;
    field_0x18 = 0.0f;

    mIdx = 0;
    mDirection = 0;
    mIsClosed = FALSE;
}

int daNpcT_Path_c::setPathInfo(u8 i_pathIdx, s8 i_roomNo, u8 i_direction) {
    mPathInfo = NULL;
    mIdx = 0;
    mDirection = i_direction;

    if (i_pathIdx != 0xFF) {
        mPathInfo = dPath_GetRoomPath(i_pathIdx, i_roomNo);
        if (mPathInfo == NULL) {
            return 0;
        }

        mIsClosed = dPath_ChkClose(mPathInfo);
        mPosition = getPntPos(getIdx());
    }

    return 1;
}

int daNpcT_Path_c::setNextPathInfo(s8 i_roomNo, u8 i_direction) {
    mIdx = 0;
    mDirection = i_direction;

    if (mPathInfo->m_nextID != 0xFFFF) {
        mPathInfo = dPath_GetRoomPath(mPathInfo->m_nextID, i_roomNo);
        if (mPathInfo == NULL) {
            return 0;
        }

        mIsClosed = dPath_ChkClose(mPathInfo);
    }

    return 1;
}

void daNpcT_Path_c::reverse() {
    if (chkReverse()) {
        offReverse();
    } else {
        onReverse();
    }
}

int daNpcT_Path_c::setNextIdx(int i_idx) {
    int numPnts = getNumPnts();

    if (chkClose() && numPnts == i_idx) {
        return daNpcT_incIdx(i_idx, mIdx, TRUE, mDirection);
    } else {
        return daNpcT_incIdx(i_idx, mIdx, FALSE, mDirection);
    }
}

int daNpcT_Path_c::getDstPos(cXyz i_pnt, cXyz* o_pos_p, int i_idx) {
    int passed1;

    if (i_idx == mIdx) {
        *o_pos_p = getPntPos(getIdx() - 1);
        return 1;
    }

    while (true) {
        passed1 = chkPassed1(i_pnt, i_idx);
        if (!passed1) {
            break;
        }

        field_0x1E = 1;

        if (setNextIdx(i_idx)) {
            mIdx = i_idx;
            *o_pos_p = getPntPos(getIdx() - 1);
            return 1;
        }
    }

    *o_pos_p = getPntPos(getIdx());

    return 0;
}

int daNpcT_Path_c::getDstPosH(cXyz i_pnt, cXyz* o_pos_p, int i_idx, int param_3) {
    int passed1;

    if (i_idx == mIdx) {
        *o_pos_p = getPntPos(getIdx() - 1);
        return 1;
    }

    while (true) {
        passed1 = chkPassed1(i_pnt, i_idx);
        if (!passed1) {
            break;
        }

        field_0x1E = 1;

        if (setNextIdx(i_idx)) {
            mIdx = i_idx;
            *o_pos_p = getPntPos(getIdx() - 1);
            return 1;
        }
    }

    while (true) {
        if (field_0x1E < param_3) {
            if (chkPassed2(i_pnt, o_pos_p, i_idx, param_3)) {
                field_0x1E++;
                continue;
            }
        } else {
            *o_pos_p = getPntPos(getIdx());
        }
    
        return 0;
    }
}

int daNpcT_Path_c::chkPassed1(cXyz i_pnt, int i_num) {
    cXyz prev_pos;
    cXyz cur_pos;
    cXyz next_pos;
    cXyz sp5C;

    u16 cur_idx = (int)(u16)getIdx();
    u16 prev_idx, next_idx;
    prev_idx = next_idx = cur_idx;

    if (chkClose() && i_num == getNumPnts()) {
        daNpcT_decIdx(getNumPnts(), prev_idx, TRUE, chkReverse());
        daNpcT_incIdx(getNumPnts(), next_idx, TRUE, chkReverse());
    } else {
        daNpcT_decIdx(i_num, prev_idx, FALSE, chkReverse());
        daNpcT_incIdx(i_num, next_idx, FALSE, chkReverse());
    }

    if (!chkClose()) {
        if (chkReverse()) {
            if (cur_idx < next_idx || prev_idx < cur_idx) {
                JUT_ASSERT(964, FALSE);
            }
        } else {
            if (cur_idx < prev_idx || next_idx < cur_idx) {
                JUT_ASSERT(971, FALSE);
            }
        }
    }

    if (prev_idx == cur_idx && cur_idx == next_idx) {
        JUT_ASSERT(978, FALSE);
    }

    prev_pos = getPntPos(prev_idx);
    cur_pos = getPntPos(cur_idx);
    next_pos = getPntPos(next_idx);

    s16 var_r29;
    if (prev_idx == cur_idx) {
        sp5C.set(0.0f, 0.0f, 100.0f);

        var_r29 = cLib_targetAngleY(&next_pos, &cur_pos);
        mDoMtx_stack_c::YrotS(var_r29);
        mDoMtx_stack_c::multVec(&sp5C, &sp5C);
        prev_pos = sp5C + cur_pos;
    }

    if (cur_idx == next_idx) {
        sp5C.set(0.0f, 0.0f, 100.0f);

        var_r29 = cLib_targetAngleY(&prev_pos, &cur_pos);
        mDoMtx_stack_c::YrotS(var_r29);
        mDoMtx_stack_c::multVec(&sp5C, &sp5C);
        next_pos = sp5C + cur_pos;
    }

    f32 proj_pnt_x, proj_cur_x, proj_pnt_z, proj_cur_z, spC;
    if (cM3d_Len2dSqPntAndSegLine(i_pnt.x, i_pnt.z, prev_pos.x, prev_pos.z,
                                  next_pos.x, next_pos.z, &proj_pnt_x, &proj_pnt_z, &spC) &&
        cM3d_Len2dSqPntAndSegLine(cur_pos.x, cur_pos.z, prev_pos.x, prev_pos.z,
                                  next_pos.x, next_pos.z, &proj_cur_x, &proj_cur_z, &spC))
    {
        var_r29 = cM_atan2s(next_pos.x - prev_pos.x, next_pos.z - prev_pos.z);
        var_r29 = var_r29 - cM_atan2s(proj_cur_x - proj_pnt_x, proj_cur_z - proj_pnt_z);
        if (0x4000 < (u16)abs(var_r29))
        {
            return 1;
        }
    }

    return 0;
}

int daNpcT_Path_c::chkPassed2(cXyz i_pnt, cXyz* param_2, int i_num, int param_4) {
    cXyz cStack_80;
    cXyz prev_pos;
    cXyz cur_pos;
    cXyz next_pos;
    cXyz h_startTan;
    cXyz h_endTan;

    u16 cur_idx = getIdx();
    u16 sp10;
    u16 prev_idx;
    u16 next_idx;
    prev_idx = next_idx = cur_idx;

    if (chkClose() && getNumPnts() == i_num) {
        daNpcT_decIdx(getNumPnts(), prev_idx, TRUE, chkReverse());
        daNpcT_incIdx(getNumPnts(), next_idx, TRUE, chkReverse());
    } else {
        daNpcT_decIdx(i_num, prev_idx, FALSE, chkReverse());
        daNpcT_incIdx(i_num, next_idx, FALSE, chkReverse());
    }

    sp10 = prev_idx;

    if (chkClose() && getNumPnts() == i_num) {
        daNpcT_decIdx(getNumPnts(), sp10, TRUE, chkReverse());
    } else {
        daNpcT_decIdx(i_num, sp10, FALSE, chkReverse());
    }

    if (!chkClose()) {
        if (chkReverse()) {
            if (cur_idx < next_idx || prev_idx < cur_idx || sp10 < prev_idx) {
                JUT_ASSERT(1082, FALSE);
            }
        } else {
            if (prev_idx < sp10 || cur_idx < prev_idx || next_idx < cur_idx) {
                JUT_ASSERT(1090, FALSE);
            }
        }
    }

    if (sp10 == prev_idx && prev_idx == cur_idx && cur_idx == next_idx) {
        JUT_ASSERT(1098, FALSE);
    }

    cStack_80 = getPntPos(sp10);
    prev_pos = getPntPos(prev_idx);
    cur_pos = getPntPos(cur_idx);
    next_pos = getPntPos(next_idx);

    if (prev_idx == cur_idx) {
        h_startTan = next_pos - prev_pos;
    } else {
        h_startTan = cur_pos - prev_pos;
    }

    if (next_idx == cur_idx) {
        h_endTan = next_pos - prev_pos;
    } else {
        h_endTan = next_pos - cur_pos;
    }

    daNpcT_Hermite_c h;
    cXyz hermite_result;

    f32 var_f31 = 1.0f / param_4;
    f32 var_f30 = field_0x1E * var_f31;

    h.Set(var_f30);
    hermite(prev_pos, h_startTan, cur_pos, h_endTan, h, hermite_result);

    param_2->x = hermite_result.x;
    param_2->y = hermite_result.y;
    param_2->z = hermite_result.z;

    f32 proj_pnt_x;
    f32 proj_cur_x;
    f32 proj_pnt_z;
    f32 proj_cur_z;
    f32 auStack_13c;
    s16 var_r28;
    if (cM3d_Len2dSqPntAndSegLine(i_pnt.x, i_pnt.z, prev_pos.x, prev_pos.z, cur_pos.x,
                                  cur_pos.z, &proj_pnt_x, &proj_pnt_z, &auStack_13c) &&
        cM3d_Len2dSqPntAndSegLine(param_2->x, param_2->z, prev_pos.x, prev_pos.z, cur_pos.x,
                                  cur_pos.z, &proj_cur_x, &proj_cur_z, &auStack_13c))
    {
        var_r28 = cM_atan2s(cur_pos.x - prev_pos.x, cur_pos.z - prev_pos.z);
        var_r28 = var_r28 - cM_atan2s(proj_cur_x - proj_pnt_x, proj_cur_z - proj_pnt_z);

        if ((u16)abs(var_r28) > 0x4000) {
            return 1;
        }
    }

    return 0;
}

#if DEBUG
int daNpcT_Path_c::drawDbgInfo(f32 param_0, int param_1) {
    GXColor green = {0x80, 0xff, 0x80, 0xff};
    GXColor blue = {0x80, 0x80, 0xff, 0xff};
    GXColor red = {0xff, 0x80, 0x80, 0xff};
    GXColor yellow = {0xff, 0xff, 0x80, 0xff};

    cXyz unkXyz1;
    cXyz unkXyz2;

    for (int i = 0; i <= getNumPnts(); i++) {
        if (i < getNumPnts()) {
            unkXyz1.set(getPntPos(i).x, param_0, getPntPos(i).z);
            if (i != 0) {
                unkXyz2.set(getPntPos(i - 1).x, param_0, getPntPos(i - 1).z);
                dDbVw_drawLineOpa(unkXyz1, unkXyz2, red, 1, 12);
            }
            dDbVw_drawSphereOpa(unkXyz1, 9.0f, green, 1);
        } else if (mIsClosed) {
            unkXyz1.set(getPntPos(i - 1).x, param_0, getPntPos(i - 1).z);
            unkXyz2.set(getPntPos(0).x, param_0, getPntPos(0).z);
            dDbVw_drawLineOpa(unkXyz1, unkXyz2, red, 1, 12);
        }
    }
}
#endif

static BOOL daNpcT_chkPassed(cXyz i_pos, dPnt* i_points, u16 i_idx, u16 i_num, BOOL i_closed,
                             int param_5, cXyz* o_proj, int* o_prevIdx, int* o_nextIdx) {
    u16 idx = i_idx;
    u16 prev_idx = i_idx;
    u16 next_idx = i_idx;
    daNpcT_incIdx(i_num, next_idx, i_closed, 0);
    daNpcT_decIdx(i_num, prev_idx, i_closed, 0);

    cXyz prev_pnt(i_points[prev_idx].m_position.x, i_points[prev_idx].m_position.y,
                  i_points[prev_idx].m_position.z);
    cXyz next_pnt(i_points[next_idx].m_position.x, i_points[next_idx].m_position.y,
                  i_points[next_idx].m_position.z);
    cXyz cur_pnt(i_points[idx].m_position.x, i_points[idx].m_position.y,
                 i_points[idx].m_position.z);
    
    cXyz vec(0.0f, 0.0f, 2.0f);
    
    s16 angle;
    if (prev_idx < idx && idx < next_idx) {
        angle = cM_atan2s(next_pnt.x - prev_pnt.x, next_pnt.z - prev_pnt.z);
    } else if (prev_idx < idx) {
        angle = cM_atan2s(cur_pnt.x - prev_pnt.x, cur_pnt.z - prev_pnt.z);
    } else if (idx < next_idx) {
        angle = cM_atan2s(next_pnt.x - cur_pnt.x, next_pnt.z - cur_pnt.z);
    } else {
        JUT_ASSERT(1470, FALSE);
    }

    mDoMtx_stack_c::transS(next_pnt);
    mDoMtx_stack_c::YrotM(angle);
    mDoMtx_stack_c::multVec(&vec, &next_pnt);
    mDoMtx_stack_c::transS(prev_pnt);
    mDoMtx_stack_c::YrotM(angle + 0x8000);
    mDoMtx_stack_c::multVec(&vec, &prev_pnt);

    f32 len, proj1_x, proj1_z, proj2_x, proj2_z;
    cM3d_Len2dSqPntAndSegLine(i_pos.x, i_pos.z, prev_pnt.x, prev_pnt.z, next_pnt.x, next_pnt.z,
                              &proj1_x, &proj1_z, &len);
    if (cM3d_IsZero(len)) {
        return FALSE;
    }

    cM3d_Len2dSqPntAndSegLine(cur_pnt.x, cur_pnt.z, prev_pnt.x, prev_pnt.z, next_pnt.x, next_pnt.z,
                              &proj2_x, &proj2_z, &len);
    
    if (param_5) {
        angle = cM_atan2s(prev_pnt.x - next_pnt.x, prev_pnt.z - next_pnt.z);
    } else {
        angle = cM_atan2s(next_pnt.x - prev_pnt.x, next_pnt.z - prev_pnt.z);
    }
    angle = angle - cM_atan2s(proj2_x - proj1_x, proj2_z - proj1_z);
    
    if (o_proj != NULL) {
        o_proj->set(proj1_x, 0.0f, proj1_z);
    }
    if (o_prevIdx != NULL) {
        *o_prevIdx = prev_idx;
    }
    if (o_nextIdx != NULL) {
        *o_nextIdx = next_idx;
    }

    return (u16)abs(angle) > 0x4000;
}

static f32 dummy_literal() {
    cXyz vec;
    return vec.abs();
}

void daNpcT_JntAnm_c::initialize() {
    mActrMngr.initialize();
    memset(&mAttnPos, 0, (uintptr_t)&field_0x158 - (uintptr_t)&mAttnPos);
}

void daNpcT_JntAnm_c::setParam(fopAc_ac_c* i_actor, J3DModel* i_model, cXyz* i_eyeOffset_p,
                               int i_backboneJointNo, int i_neckJointNo, int i_headJointNo,
                               f32 i_bodyUpAngle, f32 i_bodyDownAngle, f32 i_bodyLeftAngle,
                               f32 i_bodyRightAngle, f32 i_headUpAngle, f32 i_headDownAngle,
                               f32 i_headLeftAngle, f32 i_headRightAngle, f32 param_14,
                               f32 param_15, cXyz* param_16) {
    fopAc_ac_c* actor = NULL;
    camera_class* camera = NULL;
    cXyz vec1, vec2, vec3;
    csXyz angle;
    mPos = i_actor->current.pos;

    if (i_headJointNo >= 0) {
        mDoMtx_stack_c::copy(i_model->getAnmMtx(i_headJointNo));
        mDoMtx_stack_c::multVecZero(&mJntPos[0]);
        mHeadPos.setall(0.0f);
        mDoMtx_stack_c::multVec(&mHeadPos, &mHeadPos);

        JUT_ASSERT(1620, NULL != i_eyeOffset_p);

        mEyePos.set(i_eyeOffset_p->x, i_eyeOffset_p->y, i_eyeOffset_p->z);
        mDoMtx_stack_c::multVec(&mEyePos, &mEyePos);
        field_0x150.x = cLib_targetAngleX(&mJntPos[0], &mEyePos);
        field_0x150.y = cLib_targetAngleY(&mJntPos[0], &mEyePos);
    } else {
        field_0x150.y = 0;
    }

    if (i_neckJointNo >= 0) {
        mDoMtx_stack_c::copy(i_model->getAnmMtx(i_neckJointNo));
        mDoMtx_stack_c::multVecZero(&mJntPos[1]);
    }

    if (i_backboneJointNo >= 0) {
        mDoMtx_stack_c::copy(i_model->getAnmMtx(i_backboneJointNo));
        mDoMtx_stack_c::multVecZero(&mJntPos[2]);
    }

    mMinRad[2].x = cM_s2rad(cM_deg2s(-i_bodyDownAngle));
    mMaxRad[2].x = cM_s2rad(cM_deg2s(-i_bodyUpAngle));
    mMinRad[2].y = cM_s2rad(cM_deg2s(i_bodyLeftAngle));
    mMaxRad[2].y = cM_s2rad(cM_deg2s(i_bodyRightAngle));
    mMinRad[1].setall(0.0f);
    mMaxRad[1].setall(0.0f);
    mMinRad[0].x = cM_s2rad(cM_deg2s(-i_headDownAngle));
    mMaxRad[0].x = cM_s2rad(cM_deg2s(-i_headUpAngle));
    mMinRad[0].y = cM_s2rad(cM_deg2s(i_headLeftAngle));
    mMaxRad[0].y = cM_s2rad(cM_deg2s(i_headRightAngle));
    mAttnPos.setall(0.0f);

    switch (mMode) {
    case LOOK_NONE:
        mAttnPosP = NULL;
        break;
    case LOOK_PLAYER:
        mAttnPosP = &daPy_getPlayerActorClass()->attention_info.position;
        mAttnPos = *mAttnPosP;
        break;
    case LOOK_ACTOR: {
        actor = mActrMngr.getActorP();
        if (actor != NULL) {
            mAttnPosP = &actor->attention_info.position;
            mAttnPos = *mAttnPosP;
            mAttnPos.y += field_0x14c;
        } else {
            mAttnPosP = NULL;
        }
        break;
    }
    case LOOK_POS:
        if (mAttnPosP != NULL) {
            mAttnPos = *mAttnPosP;
        }
        break;
    case LOOK_CAMERA: {
        int cameraId = dComIfGp_getPlayerCameraID(0);
        camera = dComIfGp_getCamera(cameraId);
        if (camera != NULL) {
            mAttnPosP = fopCamM_GetEye_p(camera);
            mAttnPos = *mAttnPosP;
        }
        break;
    }
    case LOOK_MODE_5:
        if (mAttnPosP != NULL) {
            mMinRad[2].y = 0.0f;
            mMaxRad[2].y = 0.0f;
            mMinRad[0].y = 0.0f;
            mMaxRad[0].y = 0.0f;
            mAttnPos = *mAttnPosP;
        }
        break;
    case LOOK_MODE_6:
        if (mAttnPosP != NULL) {
            mMinRad[2].y = 0.0f;
            mMaxRad[2].y = 0.0f;
            mMinRad[0].x = 0.0f;
            mMaxRad[0].x = 0.0f;
            mAttnPos = *mAttnPosP;
        }
        break;
    case LOOK_MODE_7:
    case LOOK_MODE_8: {
        if (cLib_calcTimer(&mTimer) == 0) {
            field_0x13c ^= 1;
            mTimer = getTime();
        }

        s16 angle_y_offset;
        if (field_0x13c != 0) {
            angle_y_offset = cM_deg2s(-110.0f);
        } else {
            angle_y_offset = cM_deg2s(110.0f);
        }

        angle.y = (int)angle_y_offset;
        angle.y += static_cast<daNpcT_c*>(i_actor)->mCurAngle.y;
        vec3.set(0.0f, mJntPos[0].y, 1000.0f);
        if (mMode == 8) {
            vec3.y = i_actor->current.pos.y;
        }

        mDoMtx_stack_c::YrotS(angle.y);
        mDoMtx_stack_c::multVec(&vec3, &mAttnPos);
        mAttnPos += i_actor->current.pos;
        mAttnPosP = &mAttnPos;
        break;
    }
    default:
        mAttnPosP = NULL;
        break;
    }

    field_0x120 = mAttnPos;
    
    Mtx mtx;
    cMtx_copy(i_model->getBaseTRMtx(), mtx);
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mDoMtx_stack_c::copy(mtx);
    mDoMtx_stack_c::inverse();

    vec3 = field_0x120 - i_actor->current.pos;
    mDoMtx_stack_c::multVec(&vec3, &vec3);
    field_0x120 = vec3 + i_actor->current.pos;

    for (int i = 0; i < 3; i++) {
        vec3 = mJntPos[i] - i_actor->current.pos;
        mDoMtx_stack_c::multVec(&vec3, &vec3);
        mJntPos[i] = vec3 + i_actor->current.pos;
    }

    vec3 = mHeadPos - i_actor->current.pos;
    mDoMtx_stack_c::multVec(&vec3, &vec3);
    mHeadPos = vec3 + i_actor->current.pos;

    vec3 = mEyePos - i_actor->current.pos;
    mDoMtx_stack_c::multVec(&vec3, &vec3);
    mEyePos = vec3 + i_actor->current.pos;

    vec3 = field_0x120 - i_actor->current.pos;
    mDoMtx_stack_c::YrotS(-cM_rad2s(param_15));
    mDoMtx_stack_c::multVec(&vec3, &vec3);
    field_0x120 = vec3 + i_actor->current.pos;

    mEyePos = mEyePos - mHeadPos;
    mHeadPos = mHeadPos - mJntPos[0];

    for (int i = 0; i < 3; i++) {
        field_0x54[i] = mJntPos[i];
    }

    for (int i = 2; i >= 0; i--) {
        angle.set(getJntRadX2S(i), getJntRadY2S(i), 0);
        mDoMtx_stack_c::ZXYrotS(angle);
        mDoMtx_stack_c::inverse();

        if (i != 0) {
            vec3 = field_0x54[i - 1] - field_0x54[i];
            mDoMtx_stack_c::multVec(&vec3, &vec3);
            mJntPos[i - 1] = mJntPos[i] + vec3;
        } else {
            mDoMtx_stack_c::multVec(&mEyePos, &vec1);
            mDoMtx_stack_c::multVec(&mHeadPos, &vec2);
        }
    }

    for (int i = 0; i < 3; i++) {
        field_0x54[i] = mJntPos[i];
    }

    mJntPos[0] += vec2;
    field_0x24 = mJntPos[0] + vec1;

    field_0x148 = param_14;
}

void daNpcT_JntAnm_c::calc(f32 param_0) {
    cXyz vec1, vec2;
    cXyz vec3 = field_0x24;
    s16 angle = cLib_targetAngleY(&mPos, &field_0x120);
    vec1.set(0.0f, 0.0f, 1000.0f);

    mDoMtx_stack_c::YrotS(angle);
    mDoMtx_stack_c::multVec(&vec1, &vec1);
    vec1 += mPos;

    for (int i = 0; i < 3; i++) {
        if (i != 1) {
            angle = cLib_targetAngleY(&mJntPos[i], &vec1);
            angle = angle - cLib_targetAngleY(&mJntPos[i], &vec3);

            mRad[i].y = cM_s2rad(angle);
            if (mMaxRad[i].y < mRad[i].y) {
                mRad[i].y = mMaxRad[i].y;
            }
            if (mRad[i].y < mMinRad[i].y) {
                mRad[i].y = mMinRad[i].y;
            }
        } else {
            mRad[i].y = 0.0f;
        }

        switch (i) {
        case 0:
            mDoMtx_stack_c::YrotS(cM_rad2s(mRad[i].y * (1.0f - field_0x148)));
            break;
        case 1:
            mDoMtx_stack_c::YrotS(cM_rad2s(mRad[0].y * field_0x148));
            break;
        default:
            mDoMtx_stack_c::YrotS(cM_rad2s(mRad[i].y));
            break;
        }

        vec2 = vec3 - mJntPos[i];
        mDoMtx_stack_c::multVec(&vec2, &vec3);
        vec3 += mJntPos[i];
    }

    vec3 = field_0x24;
    mJntPos[0].y = field_0x24.y;

    for (int i = 0; i < 3; i++) {
        if (i != 1) {
            (void)mJntPos[i];
            s16 var_r26 = -cLib_targetAngleX(&mJntPos[i], &field_0x120);
            var_r26 = var_r26 - (s16)-cLib_targetAngleX(&mJntPos[i], &field_0x24);

            mRad[i].x = cM_s2rad(var_r26);
            if (mMaxRad[i].x < mRad[i].x) {
                mRad[i].x = mMaxRad[i].x;
            }
            if (mRad[i].x < mMinRad[i].x) {
                mRad[i].x = mMinRad[i].x;
            }
        } else {
            mRad[i].x = 0.0f;
        }

        switch (i) {
        case 0:
            mDoMtx_stack_c::XrotS(cM_rad2s(mRad[i].x * (1.0f - field_0x148)));
            break;
        case 1:
            mDoMtx_stack_c::XrotS(cM_rad2s(mRad[0].x * field_0x148));
            break;
        default:
            mDoMtx_stack_c::XrotS(cM_rad2s(mRad[i].x));
        }

        vec2 = vec3 - mJntPos[i];
        mDoMtx_stack_c::multVec(&vec2, &vec3);
        vec3 += mJntPos[i];
    }
}

void daNpcT_JntAnm_c::calcJntRad(f32 param_0, f32 param_1, f32 param_2) {
    if (mMode == 0) {
        clrRad();
    } else {
        calc(param_2);
    }

    for (int i = 0; i < 3; i++) {
        if (param_0 < 1.0f && mDirectFlag == 0) {
            cLib_addCalc2(&field_0xe4[i].x, mRad[i].x, param_0, M_PI);
            cLib_addCalc2(&field_0xe4[i].y, mRad[i].y, param_0, M_PI);
            cLib_addCalc2(&field_0xe4[i].z, mRad[i].z, param_0, M_PI);

            if (0.0f < param_2 && 0.0f < mRad[i].y && mRad[i].y - field_0xe4[i].y < 0.0f) {
                field_0xe4[i].y = mRad[i].y;
            }
            if (param_2 < 0.0f && mRad[i].y < 0.0f && 0.0f < mRad[i].y - field_0xe4[i].y) {
                field_0xe4[i].y = mRad[i].y;
            }
        } else {
            field_0xe4[i].x = mRad[i].x;
            field_0xe4[i].y = mRad[i].y;
            field_0xe4[i].z = mRad[i].z;
        }
    }
}

#if DEBUG
void daNpcT_JntAnm_c::drawDbgInfo() {
    GXColor green = {0x80, 0xff, 0x80, 0xff};
    GXColor blue = {0x80, 0x80, 0xff, 0xff};
    GXColor red = {0xff, 0x80, 0x80, 0xff};
    GXColor yellow = {0xff, 0xff, 0x80, 0xff};

    cXyz unusedXyz1;
    cXyz unusedXyz2;

    for (int i = 2; i >= 0; i--) {
        if (i != 2) {
            dDbVw_drawLineOpa(mJntPos[i + 1], mJntPos[i], red, 1, 12);
        }

        dDbVw_drawSphereOpa(mJntPos[i], 4.5f, green, 1);
    }

    dDbVw_drawLineOpa(mJntPos[0], field_0x24, red, 1, 12);
    dDbVw_drawSphereOpa(field_0x24, 9.0f, green, 1);
    dDbVw_drawSphereOpa(mAttnPos, 18.0f, green, 1);
}
#endif

void daNpcT_DmgStagger_c::setParam(fopAc_ac_c* i_actor1, fopAc_ac_c* i_actor2, s16 i_angle) {
    if (i_actor2 && i_actor1) {
        mPower[0] = cM_deg2s(60.0f);
        mPower[1] = cM_deg2s(40.0f);
        field_0x16 = fopAcM_searchActorAngleY(i_actor1, i_actor2) - i_angle;
        mStagger = 0x4000;
    }
}

void daNpcT_DmgStagger_c::calc(BOOL param_0) {
    cXyz vec1, vec2;
    mDoMtx_stack_c::YrotS(field_0x16);
    cLib_addCalc2(&mPower[1], 0.0f, 0.1f, 125.0f);
    cLib_addCalc2(&mPower[0], 0.0f, 0.1f, 125.0f);

    for (int i = 0; i < 2; i++) {
        vec1.set(0.0f, 0.0f, mPower[i] * cM_ssin(mStagger));
        mDoMtx_stack_c::multVec(&vec1, &vec2);
        mAngle[i].x = -vec2.z;
        mAngle[i].z = -vec2.x;
    }

    if (mStagger != 0) {
        cLib_chaseS(&mStagger, 0, 0x555);
        if (mStagger == 0) {
            if (param_0) {
                mStagger = 1;
            } else {
                mRebirth = true;
            }
        }
    } else {
        mRebirth = false;
    }
}

dCcD_SrcCyl daNpcT_c::mCcDCyl = {
    daNpcT_c::mCcDObjData,
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            0.0f,  // mRadius
            0.0f  // mHeight
        }  // mCyl
    }
};

dCcD_SrcSph daNpcT_c::mCcDSph = {
    daNpcT_c::mCcDObjData,
    {
        {{0.0f, 0.0f, 0.0f}, 0.0f}  // mSph
    }  // mSphAttr
};

void daNpcT_c::tgHitCallBack(fopAc_ac_c* i_actor1, dCcD_GObjInf* i_obj1, fopAc_ac_c* i_actor2,
                             dCcD_GObjInf* i_obj2) {
    fopAc_ac_c* actor1 = i_actor1;
    u8 cut_type = 0;
    if (i_actor2 != NULL) {
        if (fopAcM_GetProfName(i_actor2) == PROC_ALINK) {
            cut_type = static_cast<daPy_py_c*>(i_actor2)->getCutType();
        } else {
            cut_type = daPy_py_c::CUT_TYPE_NM_VERTICAL;
        }
    }

    static_cast<daNpcT_c*>(actor1)->setCutType(cut_type);
}

fopAc_ac_c* daNpcT_c::mFindActorPtrs[50];

char l_fileNameBuf[0x15];

s16 daNpcT_c::mSrchName;

int daNpcT_c::mFindCount;

void* daNpcT_c::srchActor(void* i_actor1, void* i_actor2) {
    fopAc_ac_c* actor2 = (fopAc_ac_c*)i_actor2;
    void* foundActor = NULL;
    if (mFindCount < 50 && fopAcM_IsActor(i_actor1) && i_actor1 != i_actor2) {
        if (mSrchName == fopAcM_GetName((fopAc_ac_c*)i_actor1)) {
            foundActor = (fopAc_ac_c*)i_actor1;
            mFindActorPtrs[mFindCount] = (fopAc_ac_c*)foundActor;
            mFindCount++;
        }
    }

    return NULL;
}

J3DAnmTransform* daNpcT_c::getTrnsfrmAnmP(char const* i_arcName, int i_resIdx) {
    return (J3DAnmTransform*)dComIfG_getObjectRes(i_arcName, i_resIdx);
}

J3DAnmTransformKey* daNpcT_c::getTrnsfrmKeyAnmP(char const* i_arcName, int i_resIdx) {
    return (J3DAnmTransformKey*)dComIfG_getObjectRes(i_arcName, i_resIdx);
}

J3DAnmTexPattern* daNpcT_c::getTexPtrnAnmP(char const* i_arcName, int i_resIdx) {
    return (J3DAnmTexPattern*)dComIfG_getObjectRes(i_arcName, i_resIdx);
}

J3DAnmTextureSRTKey* daNpcT_c::getTexSRTKeyAnmP(char const* i_arcName, int i_resIdx) {
    return (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(i_arcName, i_resIdx);
}

J3DAnmTevRegKey* daNpcT_c::getTevRegKeyAnmP(char const* i_arcName, int i_resIdx) {
    return (J3DAnmTevRegKey*)dComIfG_getObjectRes(i_arcName, i_resIdx);
}

J3DAnmColor* daNpcT_c::getColorAnmP(char const* i_arcName, int i_resIdx) {
    return (J3DAnmColor*)dComIfG_getObjectRes(i_arcName, i_resIdx);
}

int daNpcT_c::setBckAnm(J3DAnmTransform* i_bck, f32 i_speed, int i_mode, int i_start, int i_end,
                        bool i_modify) {
    s16 start = i_start;
    s16 end = i_end;
    return mBckAnm.init(i_bck, TRUE, i_mode, i_speed, start, end, i_modify);
}

int daNpcT_c::setMcaMorfAnm(J3DAnmTransformKey* i_bck, f32 i_speed, f32 i_morf, int i_mode, int i_start,
                            int i_end) {
    f32 start = i_start;
    f32 end = i_end;

    mpMorf[0]->setAnm(i_bck, i_mode, i_morf, i_speed, start, end);

    if (mpMorf[1] != NULL) {
        mpMorf[1]->setAnm(i_bck, i_mode, i_morf, i_speed, start, end);
    }

    return 1;
}

int daNpcT_c::setBtpAnm(J3DAnmTexPattern* i_btp, J3DModelData* i_modelData, f32 i_speed, int i_mode) {
    return mBtpAnm.init(i_modelData, i_btp, TRUE, i_mode, i_speed, 0, -1);
}

int daNpcT_c::setBtkAnm(J3DAnmTextureSRTKey* i_btk, J3DModelData* i_modelData, f32 i_speed, int i_mode) {
    return mBtkAnm.init(i_modelData, i_btk, TRUE, i_mode, i_speed, 0, -1);
}

int daNpcT_c::setBrkAnm(J3DAnmTevRegKey* i_brk, J3DModelData* i_modelData, f32 i_speed, int i_mode) {
    return mBrkAnm.init(i_modelData, i_brk, TRUE, i_mode, i_speed, 0, -1);
}

int daNpcT_c::setBpkAnm(J3DAnmColor* i_bpk, J3DModelData* i_modelData, f32 i_speed, int i_mode) {
    return mBpkAnm.init(i_modelData, i_bpk, TRUE, i_mode, i_speed, 0, -1);
}

int daNpcT_c::loadRes(s8 const* i_resNoList, char const** i_resNameList) {
    int phase_state;
    int resLoad_cnt = 0;
    for (int i = 0; i < 10; i++) {
        if (i_resNoList[i] == -1) {
            if (i != resLoad_cnt) {
                break;
            }
            return cPhs_COMPLEATE_e;
        }

        phase_state = dComIfG_resLoad(&mPhase[i], i_resNameList[i_resNoList[i]]);
        if (phase_state == cPhs_COMPLEATE_e) {
            resLoad_cnt++;
        }

        if (phase_state == cPhs_ERROR_e || phase_state == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }
    }

    return cPhs_INIT_e;
}

void daNpcT_c::deleteRes(s8 const* i_resNoList, char const** i_resNameList) {
    for (int i = 0; i < 10 && i_resNoList[i] != -1; i++) {
        dComIfG_resDelete(&mPhase[i], i_resNameList[i_resNoList[i]]);
    }
}

int daNpcT_c::execute() {
    BOOL procResult = FALSE;

    setParam();

    procResult = evtProc();
    if (!procResult) {
        action();
    }

    beforeMove();
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());

    mAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mAcch.m_gnd;
    mGroundAngle = fopAcM_getPolygonAngle(mGndChk, current.angle.y);
    mGroundH = mAcch.GetGroundH();
    if (mGroundH != -G_CM3D_F_INF) {
        mPolSound = dKy_pol_sound_get(&mAcch.m_gnd);
        mReverb = dComIfGp_getReverb(mCcStts.GetRoomId());

        if (mAcch.ChkWaterHit() && mAcch.m_wtr.GetHeight() > mGroundH) {
            mPolSound = dKy_pol_sound_get(&mAcch.m_wtr);
        }
    }

    afterMoved();

    if (mGroundH != -G_CM3D_F_INF) {
        setEnvTevColor();
        setRoomNo();
    }

    if (mFaceMotionSeqMngr.checkEntryNewMotion()) {
        ctrlFaceMotion();
        ctrlSubFaceMotion(1);
    }

    if (mMotionSeqMngr.checkEntryNewMotion()) {
        ctrlMotion();
    }

    playAllAnm();
    ctrlFaceMotion();
    ctrlSubFaceMotion(0);
    ctrlMotion();
    setAttnPos();

    cLib_chaseF(&field_0xdfc, 0.0f, 1.0f);

    twilight();
    setCollision();

    if (!field_0xe2c) {
        if ((field_0xe33 && dComIfGp_getEvent()->isOrderOK()) || dComIfGp_event_runCheck() == FALSE) {
            evtOrder();
        }
    }

    clrParam();
    decTmr();
    return 1;
}

int daNpcT_c::draw(BOOL param_0, BOOL i_setEffMtx, f32 param_2, GXColorS10* i_color, f32 param_4,
                   BOOL i_drawGhost, BOOL i_noShadow, BOOL i_simpleShadow) {
    J3DModel* model = mpMorf[0]->getModel();
    J3DModelData* modelData = model->getModelData();
    field_0xe34 = 1;

    if (drawDbgInfo() || checkHide() || mNoDraw) {
        return 1;
    }

    tevStr.TevColor.r = 0;
    tevStr.TevColor.g = 0;
    tevStr.TevColor.b = 0;
    tevStr.TevColor.a = 0;

    if (i_color != NULL) {
        tevStr.TevColor.r = i_color->r;
        tevStr.TevColor.g = i_color->g;
        tevStr.TevColor.b = i_color->b;
        tevStr.TevColor.a = i_color->a;
    }

    if (mDamageTimerStart != 0 && mDamageTimer != 0) {
        f32 r = (f32)mDamageTimer / (f32)mDamageTimerStart;

        if (cM3d_IsZero(r) == false) {
            tevStr.TevColor.r = r * 20.0f;
        }
    } else if (param_0) {
        tevStr.TevColor.g = 20;
    }

    if (mAnmFlags & ANM_PLAY_BTP) {
        mBtpAnm.entry(modelData);
    }

    if (mAnmFlags & ANM_PLAY_BTK) {
        mBtkAnm.entry(modelData);
    }

    if (mAnmFlags & ANM_PLAY_BRK) {
        mBrkAnm.entry(modelData);
    }

    if (mAnmFlags & ANM_PLAY_BPK) {
        mBpkAnm.entry(modelData);
    }

    if (i_setEffMtx) {
        fopAcM_setEffectMtx(this, modelData);
    }

    if (i_drawGhost) {
        drawGhost();
    } else if (mTwilight) {
        g_env_light.settingTevStruct(4, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);
        dComIfGd_setListDark();
        mpMorf[0]->entryDL();
        dComIfGd_setList();
    } else {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);
        mpMorf[0]->entryDL();
    }

    if (mAnmFlags & ANM_PLAY_BTP) {
        mBtpAnm.remove(modelData);
    }

    if (mAnmFlags & ANM_PLAY_BTK) {
        mBtkAnm.remove(modelData);
    }

    if (mAnmFlags & ANM_PLAY_BRK) {
        mBrkAnm.remove(modelData);
    }

    if (mAnmFlags & ANM_PLAY_BPK) {
        mBpkAnm.remove(modelData);
    }

    if (!i_noShadow) {
        if (!i_simpleShadow) {
            mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &current.pos, param_2, param_4,
                                            current.pos.y, mGroundH, mGndChk, &tevStr, 0,
                                            1.0f, dDlst_shadowControl_c::getSimpleTex());
        } else {
            dComIfGd_setSimpleShadow(&current.pos, mGroundH, param_4, mGndChk, 0, 1.0f,
                                     dDlst_shadowControl_c::getSimpleTex());
        }
    }

    drawOtherMdl();
    return 1;
}

void daNpcT_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daNpcT_c::setRoomNo() {
    s8 roomId = (s8)dComIfG_Bgsp().GetRoomId(mGndChk);

    fopAcM_SetRoomNo(this, roomId);
    mCcStts.SetRoomId(roomId);
}

int daNpcT_c::checkEndAnm(f32 i_speed) {
    switch ((u8)mpMorf[0]->getPlayMode()) {
    case J3DFrameCtrl::EMode_LOOP:
        return mpMorf[0]->isLoop();
    case J3DFrameCtrl::EMode_NONE:
    case J3DFrameCtrl::EMode_RESET:
        return mpMorf[0]->isStop() && cM3d_IsZero(mpMorf[0]->getPlaySpeed()) && !cM3d_IsZero(i_speed);
    case J3DFrameCtrl::EMode_REVERSE:
    case J3DFrameCtrl::EMode_LOOP_REVERSE:
        break;
    }

    return 0;
}

int daNpcT_c::checkEndAnm(J3DFrameCtrl* i_anm, f32 i_speed) {
    switch (i_anm->getAttribute()) {
    case J3DFrameCtrl::EMode_LOOP:
        return i_anm->checkState(2);
    case J3DFrameCtrl::EMode_NONE:
    case J3DFrameCtrl::EMode_RESET:
        return (i_anm->checkState(1) && i_anm->getRate() == 0.0f) && i_speed != 0.0f;
    case J3DFrameCtrl::EMode_REVERSE:
    case J3DFrameCtrl::EMode_LOOP_REVERSE:
        break;
    }

    return 0;
}

void daNpcT_c::playAllAnm() {
    f32 speed;
    if (!(mAnmFlags & ANM_FLAG_4000)) {
        if (mAnmFlags & ANM_PLAY_BCK) {
            speed = mBckAnm.getPlaySpeed();
            if (mAnmFlags & ANM_PAUSE_BCK) {
                mBckAnm.setPlaySpeed(0.0f);
            }

            mBckAnm.play();

            if (checkEndAnm(mBckAnm.getFrameCtrl(), speed)) {
                mBckLoops++;
            }

            mBckAnm.setPlaySpeed(speed);
        }

        if (mAnmFlags & ANM_PLAY_BTP) {
            speed = mBtpAnm.getPlaySpeed();
            if (mAnmFlags & ANM_PAUSE_BTP) {
                mBtpAnm.setPlaySpeed(0.0f);

                if (mAnmFlags & ANM_PAUSE_TIMER_BTP) {
                    mBtpPauseTimer = cLib_getRndValue(90, 90);
                }
            }

            if (cLib_calcTimer(&mBtpPauseTimer) != 0) {
                mBtpAnm.setPlaySpeed(0.0f);
            }

            mBtpAnm.play();

            if (mAnmFlags & ANM_PAUSE_TIMER_BTP) {
                if (checkEndAnm(mBtpAnm.getFrameCtrl(), mBtpAnm.getPlaySpeed())) {
                    mBtpPauseTimer = cLib_getRndValue(90, 90);
                }
            } else {
                mBtpPauseTimer = 0;
            }

            mBtpAnm.setPlaySpeed(speed);
        }
    }

    if (mAnmFlags & ANM_PLAY_MORF) {
        speed = mpMorf[0]->getPlaySpeed();
        if (mAnmFlags & ANM_PAUSE_MORF) {
            mpMorf[0]->setPlaySpeed(0.0f);
            mpMorf[0]->play(mPolSound, mReverb);
            mpMorf[0]->setPlaySpeed(speed);

            if (mpMorf[1] != NULL) {
                mpMorf[1]->setPlaySpeed(0.0f);
                mpMorf[1]->play(0, 0);
                mpMorf[1]->setPlaySpeed(speed);
            }
        } else {
            mpMorf[0]->play(mPolSound, mReverb);

            if (mpMorf[1] != NULL) {
                mpMorf[1]->play(0, 0);
            }

            if (checkEndAnm(speed)) {
                mMorfLoops++;
            }
        }
    }

    if (mAnmFlags & ANM_PLAY_BTK) {
        speed = mBtkAnm.getPlaySpeed();
        if (mAnmFlags & ANM_PAUSE_BTK) {
            mBtkAnm.setPlaySpeed(0.0f);
        }

        ctrlBtk();
        mBtkAnm.play();
        mBtkAnm.setPlaySpeed(speed);
    }

    if (mAnmFlags & ANM_PLAY_BRK) {
        speed = mBrkAnm.getPlaySpeed();
        if (mAnmFlags & ANM_PAUSE_BRK) {
            mBrkAnm.setPlaySpeed(0.0f);
        }

        mBrkAnm.play();
        mBrkAnm.setPlaySpeed(speed);
    }

    if (mAnmFlags & ANM_PLAY_BPK) {
        speed = mBpkAnm.getPlaySpeed();
        if (mAnmFlags & ANM_PAUSE_BPK) {
            mBpkAnm.setPlaySpeed(0.0f);
        }

        mBpkAnm.play();
        mBpkAnm.setPlaySpeed(speed);
    }

    mAnmFlags &= ~(ANM_PAUSE_MORF | ANM_PAUSE_BTK | ANM_PAUSE_BRK | ANM_PAUSE_BPK | ANM_PAUSE_BCK
                    | ANM_PAUSE_BTP | ANM_FLAG_4000);
}

BOOL daNpcT_c::ctrlBtk() {
    if (mpMatAnm[0] != NULL) {
        if (field_0xe29 != 0) {
            J3DAnmTextureSRTKey* btkAnm = mBtkAnm.getBtkAnm();
            if (btkAnm != NULL) {
                mpMatAnm[0]->setNowOffsetX(field_0xde4 * cM_ssin(mJntAnm.getEyeAngleY()));
                mpMatAnm[0]->setNowOffsetY(field_0xde0 * cM_ssin(mJntAnm.getEyeAngleX()));

                if (field_0xe2a != 0) {
                    mpMatAnm[0]->setMorfFrm(field_0xe2a);
                    field_0xe2a = 0;
                }

                mpMatAnm[0]->onEyeMoveFlag();
                return TRUE;
            }
        }

        if (field_0xe2a != 0) {
            mpMatAnm[0]->setMorfFrm(field_0xe2a);
            field_0xe2a = 0;
        }

        mpMatAnm[0]->offEyeMoveFlag();
    }

    return FALSE;
}

void daNpcT_c::setMtx() {
    J3DModel* model = mpMorf[0]->getModel();
    J3DModelData* modelData = model->getModelData();

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mCurAngle);
    mDoMtx_stack_c::scaleM(scale);

    model->setBaseTRMtx(mDoMtx_stack_c::get());
    model->setUserArea((uintptr_t)this);

    mpMorf[0]->onMorfNone();
    if (cM3d_IsZero(field_0xdfc) != FALSE) {
        mpMorf[0]->offMorfNone();
    }

    if (mAnmFlags & ANM_PLAY_BCK) {
        mBckAnm.getBckAnm()->setFrame(mBckAnm.getFrame());
    }

    mpMorf[0]->modelCalc();
}

void daNpcT_c::ctrlFaceMotion() {
    int motionNo;
    f32 morfFrm;

    if (mFaceMotionSeqMngr.play(mBckLoops, &motionNo, &morfFrm)) {
        setFaceMotionAnm(motionNo, true);

        if (morfFrm < 0.0f) {
            mpMorf[0]->setMorf(mExpressionMorfFrame);
            field_0xdfc = mExpressionMorfFrame;

            if (mpMorf[1]) {
                mpMorf[1]->setMorf(mExpressionMorfFrame);
            }
        } else {
            mpMorf[0]->setMorf(morfFrm);
            field_0xdfc = morfFrm;

            if (mpMorf[1]) {
                mpMorf[1]->setMorf(morfFrm);
            }
        }
    }
}

void daNpcT_c::ctrlMotion() {
    int motionNo;
    f32 morfFrm;

    if (mMotionSeqMngr.play(mMorfLoops, &motionNo, &morfFrm)) {
        BOOL restart = FALSE;

        if (morfFrm < -1.0f) {
            restart = TRUE;
        }

        setMotionAnm(motionNo, morfFrm < 0.0f ? mMorfFrames : morfFrm, restart);

        field_0xdfc = 0.0f;

        if (mCreating) {
            mpMorf[0]->setMorf(0.0f);

            field_0xdfc = 0.0f;

            if (mpMorf[1]) {
                mpMorf[1]->setMorf(0.0f);
            }
        }
    }
}

int daNpcT_c::ctrlMsgAnm(int* o_faceAnmAttr_p, int* o_anmAttr_p, fopAc_ac_c* i_talkPartner_p, int param_3) {
    *o_faceAnmAttr_p = -1;
    *o_anmAttr_p = -1;

    if (param_3 != 0 || eventInfo.checkCommandTalk() || mStaffId != -1) {
        if (dComIfGp_event_getTalkPartner() == i_talkPartner_p) {
            msg_class* msg_p = dMsgObject_c::getActor();
            JUT_ASSERT(3147, NULL != msg_p);

            if (msg_p->mode == 2 || msg_p->mode == 3) {
                mMsgId = fpcM_ERROR_PROCESS_ID_e;
            } else if (msg_p->mode == 6) {
                if (msg_p->msg_idx != mMsgId) {
                    *o_faceAnmAttr_p = dComIfGp_getMesgFaceAnimeAttrInfo();
                    *o_anmAttr_p = dComIfGp_getMesgAnimeAttrInfo();
                    mMsgId = msg_p->msg_idx;
                }

                if (dMsgObject_isMouthCheck()) {
                    mAnmFlags &= ~ANM_FLAG_4000;
                } else {
                    mAnmFlags |= ANM_FLAG_4000;
                }

                field_0xdb4 = 0x14;
            } else {
                if (mAnmFlags & ANM_FLAG_4000) {
                    mAnmFlags &= ~ANM_FLAG_4000;
                }
                mMsgId = fpcM_ERROR_PROCESS_ID_e;
            }
        } else {
            mMsgId = fpcM_ERROR_PROCESS_ID_e;
            if (field_0xdb4) {
                field_0xdb4 = 1;
            }
        }
    }

    if (param_3 == 0) {
        cLib_calcTimer(&field_0xdb4);
    }

    return field_0xdb4;
}

int daNpcT_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* joint = i_joint;
    int joint_no = joint->getJntNo();
    cXyz pos;
    csXyz angle;
    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_no));

    if (joint_no == getBackboneJointNo() || joint_no == getNeckJointNo() || joint_no == getHeadJointNo()) {
        Mtx joint_mtx;
        cMtx_copy(mDoMtx_stack_c::get(), joint_mtx);
        pos.set(joint_mtx[0][3], joint_mtx[1][3], joint_mtx[2][3]);
        joint_mtx[0][3] = joint_mtx[1][3] = joint_mtx[2][3] = 0.0f;

        mDoMtx_stack_c::ZXYrotS(mCurAngle.x, mCurAngle.y + field_0xd8a.y, mCurAngle.z);
        if (joint_no == getNeckJointNo()) {
            angle.set(mJntAnm.getJntRadX2S(2), mJntAnm.getJntRadY2S(2), 0);
            mDoMtx_stack_c::ZXYrotM(angle);
        }
        if (joint_no == getHeadJointNo()) {
            angle.set(mJntAnm.getJntRadX2S(2), mJntAnm.getJntRadY2S(2), 0);
            mDoMtx_stack_c::ZXYrotM(angle);
        }

        mDoMtx_stack_c::inverse();
        Mtx inverse_mtx;
        cMtx_copy(mDoMtx_stack_c::get(), inverse_mtx);

        mDoMtx_stack_c::transS(pos);
        mDoMtx_stack_c::ZXYrotM(mCurAngle.x, mCurAngle.y + field_0xd8a.y, mCurAngle.z);
        if (joint_no == getBackboneJointNo()) {
            angle.set(mJntAnm.getJntRadX2S(2), mJntAnm.getJntRadY2S(2), 0);
            mDoMtx_stack_c::ZXYrotM(angle);
        }
        if (joint_no == getNeckJointNo()) {
            angle.set(mJntAnm.getJntRadX2S(1), mJntAnm.getJntRadY2S(1), 0);
            mDoMtx_stack_c::ZXYrotM(angle);
        }
        if (joint_no == getHeadJointNo()) {
            angle.set(mJntAnm.getJntRadX2S(0), mJntAnm.getJntRadY2S(0), 0);
            mDoMtx_stack_c::ZXYrotM(angle);
        }
        mDoMtx_stack_c::concat(inverse_mtx);
        mDoMtx_stack_c::concat(joint_mtx);
    }

    afterJntAnm(joint_no);
    i_model->setAnmMtx(joint_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);

    if ((mAnmFlags & ANM_PLAY_BCK) && (checkChangeJoint(joint_no) || checkRemoveJoint(joint_no))) {
        J3DAnmTransform* bck = mBckAnm.getBckAnm();
        mBckAnm.changeBckOnly(mpMorf[0]->getAnm());
        mpMorf[0]->changeAnm(bck);
    }

    if (cM3d_IsZero(field_0xdfc) == false) {
        if (checkChangeJoint(joint_no)) {
            mpMorf[0]->offMorfNone();
        } else if (checkRemoveJoint(joint_no)) {
            mpMorf[0]->onMorfNone();
        }
    }

    return 1;
}

BOOL daNpcT_c::evtProc() {
    BOOL ret = FALSE;
    if (dComIfGp_event_runCheck()) {
        if (eventInfo.checkCommandTalk()) {
            if (!checkChangeEvt()) {
                evtTalk();
            }
            ret = TRUE;
        } else if (eventInfo.checkCommandDemoAccrpt()
                        && dComIfGp_getEventManager().endCheck(mEvtId)) {
            if (evtEndProc()) {
                dComIfGp_event_reset();
                mEvtId = -1;
            }
        } else {
            if (!strcmp(dComIfGp_getEventManager().getRunEventName(), "DEFAULT_GETITEM")
                                                    && mItemPartnerId != fpcM_ERROR_PROCESS_ID_e) {
                dComIfGp_event_setTalkPartner(NULL);
                dComIfGp_event_setItemPartnerId(mItemPartnerId);
                mItemPartnerId = fpcM_ERROR_PROCESS_ID_e;
            }
            ret = evtCutProc();
        }

        int tmp = field_0xdb4;
        int face_motion, motion;
        if (ctrlMsgAnm(&face_motion, &motion, this, field_0xe2d)) {
            if (field_0xe26) {
                mFaceMotionSeqMngr.setNo(face_motion, -1.0f, TRUE, mFaceMotionSeqMngr.getOffset());
                mMotionSeqMngr.setNo(motion, -1.0f, FALSE, mMotionSeqMngr.getOffset());
            }
        } else if (tmp != 0 && field_0xe26) {
            setAfterTalkMotion();
        }
        field_0xe26 = true;
    } else {
        if (mStaffId != -1) {
            mMode = MODE_INIT;
            mStaffId = -1;
        }

        field_0xdb4 = 0;
        field_0xe26 = true;
    }

    return ret;
}

void daNpcT_c::setFootPos() {
    if (getFootLJointNo() >= 0) {
        mOldFootLOffset = mFootLOffset;
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getFootLJointNo()));
        mDoMtx_stack_c::multVecZero(&mFootLPos);
        mFootLOffset = mFootLPos - current.pos;
    }

    if (getFootRJointNo() >= 0) {
        mOldFootROffset = mFootROffset;
        mDoMtx_stack_c::copy(mpMorf[0]->getModel()->getAnmMtx(getFootRJointNo()));
        mDoMtx_stack_c::multVecZero(&mFootRPos);
        mFootROffset = mFootRPos - current.pos;
    }

    if (mCreating) {
        mOldFootLOffset = mFootLOffset;
        mOldFootROffset = mFootROffset;
    }
}

void daNpcT_c::setFootPrtcl(cXyz* param_0, f32 param_1, f32 param_2) {
    cXyz foot_l_pos = mFootLPos;
    cXyz foot_r_pos = mFootRPos;
    cXyz start, end;
    u8 cross1;
    u8 foot_l_att;
    u8 cross2;
    u8 foot_r_att;

    start = mFootLPos;
    end = start;
    end.y -= param_1;
    mLinChk.Set(&start, &end, this);
    cross1 = dComIfG_Bgsp().LineCross(&mLinChk);
    if (cross1) {
        foot_l_pos.y = mLinChk.GetCross().y;
        mFootLPolyAngle = fopAcM_getPolygonAngle(mLinChk, mCurAngle.y);
        foot_l_att = dComIfG_Bgsp().GetPolyAtt0(mLinChk);
    } else {
        mFootLPolyAngle = 0;
    }

    start = mFootRPos;
    end = start;
    end.y -= param_1;
    mLinChk.Set(&start, &end, this);
    cross2 = dComIfG_Bgsp().LineCross(&mLinChk);
    if (cross2) {
        foot_r_pos.y = mLinChk.GetCross().y;
        mFootRPolyAngle = fopAcM_getPolygonAngle(mLinChk, mCurAngle.y);
        foot_r_att = dComIfG_Bgsp().GetPolyAtt0(mLinChk);
    } else {
        mFootRPolyAngle = 0;
    }

    field_0x9c0.setEffectTwo(&tevStr, &current.pos, 0, 0, NULL,
                             mFootLOnGround == 0 && cross1 == 1 ? &foot_l_pos : NULL,
                             mFootROnGround == 0 && cross2 == 1 ? &foot_r_pos : NULL,
                             &mCurAngle, param_0, fopAcM_GetRoomNo(this), param_2, speedF);

    if (mFootLOnGround == 0 && cross1 == 1) {
        switch (foot_l_att) {
        case 3:
            break;
        case 13:
            dkWmark_c::setFootMark(&foot_l_pos, mCurAngle.y, 3);
            break;
        }
    }

    if (mFootROnGround == 0 && cross2 == 1) {
        switch (foot_r_att) {
        case 3:
            break;
        case 13:
            dkWmark_c::setFootMark(&foot_r_pos, mCurAngle.y, 3);
            break;
        }
    }

    mFootLOnGround = cross1;
    mFootROnGround = cross2;
}

bool daNpcT_c::checkCullDraw() {
    bool result = false;
    return result;
}

void daNpcT_c::twilight() {
    if (mTwilight) {
        attention_info.flags |= fopAc_AttnFlag_UNK_0x400000;
        mNoDraw = false;
        attention_info.flags |= fopAc_AttnFlag_UNK_0x800000;
        setHitodamaPrtcl();
    }
}

void daNpcT_c::evtOrder() {
    if (strlen(mpEvtData[mEvtNo].eventName) != 0) {
        if (strlen(mpArcNames[mpEvtData[mEvtNo].num]) != 0) {
            eventInfo.setArchiveName(mpArcNames[mpEvtData[mEvtNo].num]);
            dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
        }

        mEvtId = dComIfGp_getEventManager().getEventIdx(this, mpEvtData[mEvtNo].eventName, 0xFF);
        fopAcM_orderOtherEventId(this, mEvtId, 0xFF, 0xFFFF, 40, 1);
    } else if (
#if !PLATFORM_SHIELD
        (!mTwilight || daPy_py_c::checkNowWolfEyeUp()) &&
#endif
        ((attention_info.flags & fopAc_AttnFlag_SPEAK_e) || (attention_info.flags & fopAc_AttnFlag_TALK_e))) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (chkXYItems()) {
            eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
        }
        if (mSpeakEvent) {
            fopAcM_orderSpeakEvent(this, 0, 0);
        }
    }
}

void daNpcT_c::evtChange() {
    if (strlen(mpEvtData[mEvtNo].eventName) != 0) {
        if (strlen(mpArcNames[mpEvtData[mEvtNo].num]) != 0) {
            eventInfo.setArchiveName(mpArcNames[mpEvtData[mEvtNo].num]);
            dComIfGp_getEventManager().setObjectArchive(eventInfo.getArchiveName());
        }

        mEvtId = dComIfGp_getEventManager().getEventIdx(this, mpEvtData[mEvtNo].eventName, 0xFF);
        dComIfGp_getEvent()->reset(this);
        fopAcM_orderChangeEventId(this, mEvtId, 1, 0xFFFF);
    }
}

void daNpcT_c::clrParam() {
    field_0xd7e = mCurAngle;
    mCutType = 0;
    memset(&mEvtNo, 0, (uintptr_t)&field_0xe38 - (uintptr_t)&mEvtNo);
    mJntAnm.clrDirectFlag();
}

bool daNpcT_c::setFaceMotionAnm(int i_idx, bool i_modify) {
    J3DAnmTransform* bck = NULL;
    J3DAnmTexPattern* btp = NULL;

    mAnmFlags &= ~(ANM_PAUSE_BCK | ANM_PAUSE_BTP | ANM_PLAY_BCK | ANM_PLAY_BTP
                    | ANM_FLAG_1000 | ANM_PAUSE_TIMER_BTP | ANM_FLAG_4000);

    daNpcT_faceMotionAnmData_c anm_data = getFaceMotionAnm(mpFaceMotionAnmData[i_idx]);

    if (anm_data.mBckFileIdx != -1) {
        int arc_idx = anm_data.mBckArcIdx;
        int file_idx = anm_data.mBckFileIdx;
        changeBck(&file_idx, &arc_idx);
        bck = getTrnsfrmAnmP(mpArcNames[arc_idx], file_idx);
    }

    if (bck != NULL) {
        if (setBckAnm(bck, 1.0f, anm_data.mBckAttr, 0, -1, i_modify)) {
            mAnmFlags |= ANM_PAUSE_BCK | ANM_PLAY_BCK;
            mBckLoops = 0;
        } else {
            bck = NULL;
        }
    }

    if (bck == NULL && anm_data.mBckFileIdx != -1) {
        return false;
    }

    if (anm_data.mBtpFileIdx != -1) {
        int arc_idx = anm_data.mBtpArcIdx;
        int file_idx = anm_data.mBtpFileIdx;
        changeBtp(&file_idx, &arc_idx);
        btp = getTexPtrnAnmP(mpArcNames[arc_idx], file_idx);
    }

    if (btp != NULL) {
        if (btp == mBtpAnm.getBtpAnm() && anm_data.field_0x18) {
            mAnmFlags |= ANM_PLAY_BTP | ANM_PAUSE_TIMER_BTP;
        } else if (setBtpAnm(btp, mpMorf[0]->getModel()->getModelData(), 1.0f, anm_data.mBtpAttr)) {
            mAnmFlags |= ANM_PAUSE_BTP | ANM_PLAY_BTP;
            if (anm_data.field_0x18) {
                mAnmFlags |= ANM_PAUSE_TIMER_BTP;
            }
        } else {
            btp = NULL;
        }
    }

    if (btp == NULL && anm_data.mBtpFileIdx != -1) {
        return false;
    }

    return afterSetFaceMotionAnm(i_idx, anm_data.mBckAttr, 0.0f, anm_data.mBtpAttr);
}

bool daNpcT_c::setMotionAnm(int i_idx, f32 i_morf, BOOL i_restart) {
    J3DAnmTransformKey* bck = NULL;
    J3DAnmTextureSRTKey* btk = NULL;
    BOOL restarted = FALSE;

    mAnmFlags &= ~(ANM_PAUSE_MORF | ANM_PAUSE_BTK | ANM_PAUSE_BRK | ANM_PAUSE_BPK
                    | ANM_PLAY_MORF | ANM_PLAY_BTK | ANM_PLAY_BRK | ANM_PLAY_BPK);

    daNpcT_motionAnmData_c anm_data = getMotionAnm(mpMotionAnmData[i_idx]);

    if (anm_data.mBckFileIdx != -1) {
        int arc_idx = anm_data.mBckArcIdx;
        int file_idx = anm_data.mBckFileIdx;
        changeAnm(&file_idx, &arc_idx);
        bck = getTrnsfrmKeyAnmP(mpArcNames[arc_idx], file_idx);
    }

    if (bck != NULL) {
        if (i_restart && bck == mpMorf[0]->getAnm()) {
            mAnmFlags |= ANM_PLAY_MORF;
            mMorfLoops = 0;
            restarted = TRUE;
        } else if (setMcaMorfAnm(bck, 1.0f, i_morf, anm_data.mBckAttr, 0, -1)) {
            mAnmFlags |= ANM_PAUSE_MORF | ANM_PLAY_MORF;
            mMorfLoops = 0;
        } else {
            bck = NULL;
        }
    }

    if (bck == NULL && anm_data.mBckFileIdx != -1) {
        return false;
    }

    field_0xe29 = 0;
    field_0xe2a = 0;

    if (anm_data.mBtkFileIdx != -1) {
        int arc_idx = anm_data.mBtkArcIdx;
        int file_idx = anm_data.mBtkFileIdx;
        changeBtk(&file_idx, &arc_idx);
        field_0xe29 = anm_data.field_0x18;
        field_0xe2a = anm_data.field_0x1a;
        btk = getTexSRTKeyAnmP(mpArcNames[arc_idx], file_idx);
    }

    if (btk != NULL) {
        if (restarted) {
            mAnmFlags |= ANM_PLAY_BTK;
        } else if (setBtkAnm(btk, mpMorf[0]->getModel()->getModelData(), 1.0f, anm_data.mBtkAttr)) {
            mAnmFlags |= ANM_PAUSE_BTK | ANM_PLAY_BTK;
        } else {
            btk = NULL;
        }
    }

    if (btk == NULL && anm_data.mBtkFileIdx != -1) {
        return false;
    }

    return afterSetMotionAnm(i_idx, anm_data.mBckAttr, i_morf, anm_data.mBtkAttr);
}

void daNpcT_c::setPos(cXyz i_pos) {
    i_pos.y += 300.0f;
    mGndChk.SetPos(&i_pos);
    
    i_pos.y = dComIfG_Bgsp().GroundCross(&mGndChk);
    JUT_ASSERT(3922, -G_CM3D_F_INF != i_pos.y)

    current.pos = i_pos;
    old.pos = current.pos;
}

void daNpcT_c::setAngle(csXyz i_angle) {
    current.angle = i_angle;
    shape_angle = current.angle;
    mCurAngle = current.angle;
    field_0xd7e = mCurAngle;
    old.angle = current.angle;
}

void daNpcT_c::setAngle(s16 i_angle) {
    current.angle.y = i_angle;
    shape_angle.y = current.angle.y;
    mCurAngle.y = current.angle.y;
    field_0xd7e.y = mCurAngle.y;
    old.angle.y = current.angle.y;
}

fopAc_ac_c* daNpcT_c::hitChk(dCcD_GObjInf* i_objInf, u32 param_1) {
    fopAc_ac_c* hit_actor_p = NULL;

    ((dCcD_Stts*)i_objInf->GetStts())->Move();

    if (mDamageTimer == 0) {
        hit_actor_p = i_objInf->GetTgHitAc();
        if (hit_actor_p != NULL) {
            if (fopAcM_GetName(hit_actor_p) == PROC_NPC_TK && (param_1 & 1)) {
                ((daNPC_TK_c*)hit_actor_p)->setBump();
                return hit_actor_p;
            }
            return hit_actor_p;
        }
    }

    return NULL;
}

void daNpcT_c::setDamage(int param_0, int i_faceMotion, int i_motion) {
    mDamageTimerStart = 8;
    if (mCutType == daPy_py_c::CUT_TYPE_TURN_RIGHT      ||
        mCutType == daPy_py_c::CUT_TYPE_TURN_LEFT       ||
        mCutType == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT ||
        mCutType == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT)
    {
        mDamageTimerStart = 20;
    }

    mDamageTimer = mDamageTimerStart;

    if (i_faceMotion > -1) {
        mFaceMotionSeqMngr.setNo(i_faceMotion, 0.0f, FALSE, mFaceMotionSeqMngr.getOffset());
    }
    if (i_motion > -1) {
        //! @bug The parameters to this function are passed in the wrong order
        mMotionSeqMngr.setNo(i_motion, 0.0f, mMotionSeqMngr.getOffset(), FALSE);
    }

    speed.setall(0.0f);
    speedF = 0.0f;
}

BOOL daNpcT_c::chkActorInSight(fopAc_ac_c* i_actor, f32 i_fovY, s16 i_angleY) {
    if (cM3d_IsZero(i_fovY)) {
        return FALSE;
    }

    if (i_fovY < 180.0f) {
        s16 fov_s = cM_deg2s(i_fovY);
        cXyz attn_pos = getAttnPos(i_actor);
        cSGlobe globe(attention_info.position - attn_pos);
        cSAngle angle_diff = cSAngle(globe.U().Inv()) - i_angleY;
        s16 angle_diff_s = angle_diff.Val();
        if (angle_diff_s < 0) {
            angle_diff_s = -angle_diff_s;
        }
        return (angle_diff_s > fov_s) == false;
    }

    return TRUE;
}

BOOL daNpcT_c::chkPointInArea(cXyz i_pos, cXyz i_center, f32 i_boundXZ, f32 i_boundY1,
                              f32 i_boundY2, s16 i_angle) {
    cXyz bounds, center;
    f32 max_y = i_boundY1;
    f32 min_y = i_boundY2;
    if (max_y < min_y) {
        min_y = i_boundY1;
        max_y = i_boundY2;
    }

    bounds.set(i_boundXZ, fabsf(max_y - min_y), i_boundXZ);
    center = i_center;
    center.y += min_y;
    return chkPointInArea(i_pos, center, bounds, i_angle);
}

BOOL daNpcT_c::chkPointInArea(cXyz i_pos, cXyz i_center, cXyz i_bounds, s16 i_angle) {
    return daNpcT_chkPointInArea(i_pos, i_center, i_bounds, i_angle, TRUE);
}

BOOL daNpcT_c::chkFindActor(fopAc_ac_c* i_actor, BOOL i_release, s16 i_angleY) {
    cXyz attn_pos = getAttnPos(i_actor);
    
    int dist_index = i_release == FALSE ? attention_info.distances[fopAc_attn_SPEAK_e]
                                        : attention_info.distances[fopAc_attn_TALK_e];

    f32 dist_max = i_release == FALSE ? dComIfGp_getAttention()->getDistTable(dist_index).mDistMax
                                      : dComIfGp_getAttention()->getDistTable(dist_index).mDistMaxRelease;
    f32 lower_y = dComIfGp_getAttention()->getDistTable(dist_index).mLowerY * -1.0f;
    f32 upper_y = dComIfGp_getAttention()->getDistTable(dist_index).mUpperY * -1.0f;
    u32 angle_select = dComIfGp_getAttention()->getDistTable(dist_index).mAngleSelect;

    f32 fov = 180.0f;
    if (angle_select & 8) {
        fov = 30.0f;
    } else if (angle_select & 0x10) {
        fov = 45.0f;
    } else if (angle_select & 0x20) {
        fov = 60.0f;
    } else if (angle_select & 0x40) {
        fov = 90.0f;
    } else if (angle_select & 0x80) {
        fov = 110.0f;
    } else if (angle_select & 0x100) {
        fov = 135.0f;
    }

    if (chkPointInArea(attn_pos, attention_info.position, dist_max, lower_y, upper_y, 0)) {
        if (chkActorInSight(i_actor, fov, i_angleY)) {
            return TRUE;
        }
    }

    return FALSE;
}

BOOL daNpcT_c::chkWolfAction() {
    if (daPy_py_c::checkNowWolf()) {
        return (!daPy_getPlayerActorClass()->checkWolfWait() && !dComIfGp_checkPlayerStatus0(0, 0x8000000)) &&
                !daPy_getPlayerActorClass()->checkWolfRSit();
    }

    return FALSE;
}

BOOL daNpcT_c::chkFindWolf(s16 i_angleY, int i_distIndex, int param_2, f32 i_boundXZ, f32 i_fovY,
                           f32 i_boundY1, f32 i_boundY2, BOOL param_7) {
    cXyz attn_pos = getAttnPos(daPy_getPlayerActorClass());

    if (daPy_py_c::checkNowWolf()) {
        if (chkPointInArea(attn_pos, attention_info.position, i_boundXZ, i_boundY1, i_boundY2, 0)) {
            if (chkActorInSight(daPy_getPlayerActorClass(), i_fovY, i_angleY)) {
                return TRUE;
            }
        }

        f32 dist_max = dComIfGp_getAttention()->getDistTable(i_distIndex).mDistMax;
        f32 upper_y = dComIfGp_getAttention()->getDistTable(i_distIndex).mUpperY;
        f32 lower_y = dComIfGp_getAttention()->getDistTable(i_distIndex).mLowerY;
        if (chkPointInArea(attn_pos, attention_info.position, dist_max, upper_y, lower_y, 0)) {
            return TRUE;
        }

        int dist_index = attention_info.distances[fopAc_attn_TALK_e];
        dist_max = dComIfGp_getAttention()->getDistTable(dist_index).mDistMax;
        upper_y = dComIfGp_getAttention()->getDistTable(dist_index).mUpperY;
        lower_y = dComIfGp_getAttention()->getDistTable(dist_index).mLowerY;
        if (chkPointInArea(attn_pos, attention_info.position, dist_max, upper_y, lower_y, 0)) {
            if (param_2 < 2 && param_7 && chkWolfAction()) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL daNpcT_c::srchPlayerActor() {
    if (mPlayerActorMngr.getActorP() != NULL) {
        if (!chkFindPlayer(TRUE, mCurAngle.y)) {
            mPlayerActorMngr.remove();
        }
    } else {
        if (chkFindPlayer(FALSE, mCurAngle.y)) {
            mPlayerActorMngr.entry(daPy_getPlayerActorClass());
        }
    }

    return NULL != mPlayerActorMngr.getActorP();
}

cXyz daNpcT_c::getAttnPos(fopAc_ac_c* i_actor) {
    cXyz attn_pos = i_actor->attention_info.position;
    if (fopAcM_GetName(i_actor) == PROC_ALINK) {
        attn_pos.y -= daPy_py_c::getAttentionOffsetY();
        if (daPy_py_c::checkNowWolf()) {
            attn_pos = i_actor->current.pos;
            attn_pos.y = i_actor->attention_info.position.y;
        }
    }

    return attn_pos;
}

BOOL daNpcT_c::turn(s16 i_angle, int i_count, int i_direction) {
    if (mTurnAmount == 0) {
        mTurnCount = fabsf(cM_sht2d((s16)(i_angle - mStartAngle)));
        mTurnCount = mTurnCount / 180.0f * i_count;
        mTurnCount = (int)mTurnCount + 1;
        if (mTurnCount < 8.0f) {
            mTurnCount = 8.0f;
        }
    }

    int tmp1 = mStartAngle;
    int tmp2 = i_angle;
    if (i_direction == 0) {
        i_direction = (s16)(i_angle - mStartAngle) >= 0 ? 1 : -1;
    }

    int angle_diff = (u16)(tmp2 - tmp1);
    if (i_direction == -1) {
        angle_diff = -(u16)(0xffff - angle_diff);
    }

    s16 turn = angle_diff * cM_ssin((s16)mTurnAmount);
    mTurnAmount += (s16)(0x4000 / mTurnCount);

    if ((u16)mTurnAmount < 0x4000) {
        mCurAngle.y = mStartAngle + turn;
    } else {
        mCurAngle.y = i_angle;
        mTurnAmount = 0x4000;
    }

    return i_angle == mCurAngle.y;
}

BOOL daNpcT_c::step(s16 i_angle, int i_faceMotion, int i_motion, int i_turnCount,
                   int i_turnDirection) {
    if (mStepMode >= 2 && i_angle != mCurAngle.y) {
        mStepMode = 0;
    }

    if (mStepMode == 0) {
        s16 angle_diff = i_angle - mCurAngle.y;
        if (angle_diff == 0) {
            mStepMode = 2;
        } else {
            mTargetAngle = i_angle;
            mStartAngle = mCurAngle.y;
            mTurnAmount = 0;
            if ((int)fabsf(cM_sht2d(angle_diff)) > 64) {
                mFaceMotionSeqMngr.setNo(i_faceMotion, -1.0f, TRUE, mFaceMotionSeqMngr.getOffset());
                mMotionSeqMngr.setNo(i_motion, -1.0f, TRUE, mMotionSeqMngr.getOffset());
            }
            current.angle.y = mCurAngle.y;
            shape_angle.y = current.angle.y;
            mStepMode++;
        }
    } else if (mStepMode == 1) {
        if (turn(mTargetAngle, i_turnCount, i_turnDirection)) {
            current.angle.y = mCurAngle.y;
            shape_angle.y = current.angle.y;
            field_0xd7e.y = current.angle.y;
            mStepMode++;
        } else {
            current.angle.y = mCurAngle.y;
            shape_angle.y = current.angle.y;
        }
    }

    return mStepMode > 1;
}

void daNpcT_c::calcSpeedAndAngle(cXyz i_target, BOOL i_stop, s16 i_angleScale, s16 i_angleStep) {
    if (i_stop) {
        speedF = 0.0f;
        return;
    }

    current.angle.y = cLib_targetAngleY(&current.pos, &i_target);
    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, i_angleScale, i_angleStep);
    mCurAngle.y = shape_angle.y;

    if (mFootLOffset.y < mFootROffset.y) {
        speedF = (mFootLOffset - mOldFootLOffset).absXZ();
    } else {
        speedF = (mFootROffset - mOldFootROffset).absXZ();
    }

    speedF *= cM_scos(mGroundAngle);
    if (mGroundAngle < 0) {
        speedF *= 0.85f;
    }
}

int daNpcT_c::getActorDistance(fopAc_ac_c* i_actor, int i_distIndex1, int i_distIndex2) {
    int ret = 0;
    f32 dist1 = dComIfGp_getAttention()->getDistTable(i_distIndex1).mDistMax;
    f32 dist2 = dComIfGp_getAttention()->getDistTable(i_distIndex2).mDistMax;
    f32 actor_dist = fopAcM_searchActorDistanceXZ(this, i_actor);
    
    if (actor_dist < dist1) {
        return ret;
    }

    actor_dist -= dist1;
    dist2 -= dist1;
    dist2 /= 6.0f;
    for (int i = 0; i < 5; i++, ret++) {
        if (actor_dist < dist2 * (i + 1)) {
            return ret;
        }
    }

    return ret;
}

BOOL daNpcT_c::initTalk(int i_flowID, fopAc_ac_c** i_partnerList_p) {
    mFlow.init(this, i_flowID, 0, i_partnerList_p);
    mPlayerAngle = fopAcM_searchPlayerAngleY(this);

    if (checkStep()) {
        mStepMode = 0;
    }

    speedF = 0.0f;
    speed.setall(0.0f);
    return TRUE;
}

BOOL daNpcT_c::talkProc(int* param_0, BOOL param_1, fopAc_ac_c** i_partnerList_p, BOOL param_3) {
    BOOL ret = FALSE;

    if (dMsgObject_isMsgSendControl()) {
        if (param_1) {
            dMsgObject_offMsgSendControl();
        }
    } else if (mFlow.doFlow(this, i_partnerList_p, 0)) {
        ret = TRUE;
    } else {
        if (mFlow.getMsg() != NULL) {
            switch (mFlow.getMsg()->mode) {
            case 2:
            case 6:
                if (param_0 != NULL) {
                    for (int i = 0; param_0[i] > 0; i++) {
                        if (param_0[i] == mFlow.getMsgNo()) {
                            ret = TRUE;
                            break;
                        }
                    }
                }
                break;
            case 17:
                if (param_3) {
                    ret = TRUE;
                }
                break;
            }
        }

        int tmp1, face_motion, motion, tmp2;
        if (mFlow.checkEventRender(&tmp1, &motion, &face_motion, &tmp2)) {
            if (face_motion != mFaceMotionSeqMngr.getNo()) {
                mFaceMotionSeqMngr.setNo(face_motion, -1.0f, FALSE, mFaceMotionSeqMngr.getOffset());
            }
            if (motion != mMotionSeqMngr.getNo()) {
                mMotionSeqMngr.setNo(motion, -1.0f, FALSE, mMotionSeqMngr.getOffset());
            }
        }
    }

    return ret;
}

fopAc_ac_c* daNpcT_c::getNearestActorP(s16 i_srchActorName) {
    fopAc_ac_c* actor = NULL;
    f32 minDistance = G_CM3D_F_INF;

    mFindCount = 0;
    mSrchName = i_srchActorName;
    fpcM_Search(srchActor, this);

    for (s32 i = 0; i < mFindCount; i++) {
        if (fopAcM_searchActorDistance(this, mFindActorPtrs[i]) < minDistance) {
            minDistance = fopAcM_searchActorDistance(this, mFindActorPtrs[i]);
            actor = mFindActorPtrs[i];
        }
    }

    return actor;
}

fopAc_ac_c* daNpcT_c::getEvtAreaTagP(int i_type, int i_no) {
    int var_r29 = 0;
    f32 var_f31 = G_CM3D_F_INF;

    mFindCount = 0;
    mSrchName = PROC_TAG_EVTAREA;
    fpcM_Search(srchActor, this);

    for (int i = 0; i < mFindCount; i++) {
        if (((daTag_EvtArea_c*)(mFindActorPtrs[i]))->getType() == i_type && ((daTag_EvtArea_c*)(mFindActorPtrs[i]))->getNo() == i_no) {
            return mFindActorPtrs[i];
        }
    }

    return NULL;
}

fopAc_ac_c* daNpcT_c::getShopItemTagP() {
    int unusedInt1 = 0;
    mFindCount = 0;
    mSrchName = PROC_TAG_SHOPITM;
    fpcM_Search(srchActor, this);

    for (s32 i = 0; i < mFindCount; i++) {
        u32 param = fopAcM_GetParam(mFindActorPtrs[i]) & 0xF0000000;
        if (0xF0000000 == param) {
            return mFindActorPtrs[i];
        }
    }

    return NULL;
}

void daNpcT_c::setHitodamaPrtcl() {
    JPABaseEmitter* emitter = NULL;
    cXyz pos;
    field_0xe18 = (u16)(field_0xe1a * 2);
    field_0xe00.x = cM_ssin(field_0xe1a) * 8.0f;
    field_0xe00.y = cM_ssin(field_0xe18) * 4.0f;
    field_0xe00.z = field_0xe00.x * -cM_ssin(shape_angle.y);
    field_0xe00.x = field_0xe00.x * cM_scos(shape_angle.y);
    field_0xe1a += (s16)0x400;

    pos.x = eyePos.x + field_0xe00.x + field_0xe0c.x;
    pos.y = eyePos.y + field_0xe00.y + field_0xe0c.y;
    pos.z = eyePos.z + field_0xe00.z + field_0xe0c.z;

    for (int i = 0; i < 2; i++) {
        static const u16 id[2] = {0x8497, 0x8498};
        mHitodamaPrtclKey[i] = dComIfGp_particle_set(mHitodamaPrtclKey[i], id[i], &pos,
                                                     &mCurAngle, NULL);

        emitter = dComIfGp_particle_getEmitter(mHitodamaPrtclKey[i]);
        if (emitter != NULL) {
            u8 alpha = dComIfGs_wolfeye_effect_check() == FALSE ? 0xFF : 0;
            emitter->setGlobalTranslation(pos.x, pos.y, pos.z);
            emitter->setGlobalAlpha(alpha);
        }
    }
}

BOOL daNpcT_chkActorInScreen(fopAc_ac_c* i_ActorP, f32 param_1, f32 param_2, f32 param_3,
                             f32 param_4, f32 param_5, f32 param_6, f32 param_7, int param_8) {
    if (fopAcM_GetMtx(i_ActorP)) {
        cXyz pos_array[8];
        cXyz proj;

        pos_array[0].set(param_1, param_5, param_3);
        pos_array[1].set(param_4, param_5, param_3);
        pos_array[2].set(param_1, param_5, param_6);
        pos_array[3].set(param_4, param_5, param_6);
        pos_array[4].set(param_1, param_2, param_3);
        pos_array[5].set(param_4, param_2, param_3);
        pos_array[6].set(param_1, param_2, param_6);
        pos_array[7].set(param_4, param_2, param_6);

        mDoMtx_stack_c::copy(fopAcM_GetMtx(i_ActorP));
        mDoMtx_stack_c::transM(0.0f, param_7, 0.0f);
        cMtx_multVecArray(mDoMtx_stack_c::get(), pos_array, pos_array, 8);

        if (param_8) {
#if DEBUG
            dDbVw_drawCube8pOpa(pos_array, (GXColor){0x0, 0xc0, 0x0, 0xff});
#endif

            return false;
        }

        for (int i = 0; i < 8; i++) {
            mDoLib_project(&pos_array[i], &proj);
            if (0.0f < proj.x && proj.x < FB_WIDTH && 0.0f < proj.y && proj.y < FB_HEIGHT) {
                continue;
            }
            return false;
        }

        return true;
    }
    return false;
}

BOOL daNpcT_chkPointInArea(cXyz i_pos, cXyz i_center, cXyz i_bounds, s16 i_angle,
                           BOOL i_checkHeight) {
    cXyz l_pos;

    mDoMtx_stack_c::YrotS(-i_angle);
    mDoMtx_stack_c::transM(-i_center.x, -i_center.y, -i_center.z);
    mDoMtx_stack_c::multVec(&i_pos, &l_pos);

    f32 axis_x = fabsf(i_bounds.x);
    f32 axis_z = fabsf(i_bounds.z);
    f32 pos_x = fabsf(l_pos.x);
    f32 pos_z = fabsf(l_pos.z);

    if (i_checkHeight) {
        cXyz diff = i_pos - i_center;
        if (!(0.0f <= diff.y && diff.y < i_bounds.y)) {
            return FALSE;
        }
    }
    
    if ((pos_x * pos_x) / (axis_x * axis_x) + (pos_z * pos_z) / (axis_z * axis_z) <= 1.0f) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL daNpcT_getPlayerInfoFromPlayerList(int param_0, int i_roomNo, cXyz* o_spawnPos,
                                        csXyz* o_angle) {
    BOOL ret = FALSE;
    dStage_roomDt_c* room_dt = dComIfGp_roomControl_getStatusRoomDt(i_roomNo);
    stage_actor_data_class* entry = room_dt->getPlayer()->m_entries;

    for (int i = 0; i < room_dt->getPlayerNum();) {
        if (param_0 == (entry->base.angle.z & 0xFF)) {
            *o_spawnPos = entry->base.position;
            *o_angle = entry->base.angle;
            ret = TRUE;
            break;
        }
        entry++;
        i++;
    }

    return ret;
}

BOOL daNpcT_chkDoBtnIsSpeak(fopAc_ac_c* i_actor_p) {
    BOOL ret = FALSE;

    if (dComIfGp_getDoStatus() == 28) {
        if (daPy_getPlayerActorClass()->checkPriActorOwn(i_actor_p)) {
            JUT_ASSERT(4965, NULL != dComIfGp_getAttention());

            for (int i = 0; i < dComIfGp_getAttention()->GetActionCount(); i++) {
                if (dComIfGp_getAttention()->ActionTarget(i) == i_actor_p &&
                    dComIfGp_getAttention()->getActionBtnB() != NULL &&
                    dComIfGp_getAttention()->getActionBtnB()->mType == fopAc_attn_SPEAK_e)
                {
                    ret = TRUE;
                }
            }

            for (int i = 0; i < dComIfGp_getAttention()->GetLockonCount(); i++) {
                if (dComIfGp_getAttention()->LockonTarget(i) == i_actor_p &&
                    dComIfGp_getAttention()->getActionBtnB() != NULL &&
                    dComIfGp_getAttention()->getActionBtnB()->mType == fopAc_attn_TALK_e)
                {
                    ret = TRUE;
                }
            }
        }
    }

    return ret;
}

int daNpcT_judgeRace(dPath* i_path, fopAc_ac_c** param_1, daNpcT_pntData_c* i_pntData,
                      int param_3, int* param_4) {
    cXyz proj;
    int prev_idx;
    int next_idx;
    f32 proj1_x, proj2_x, proj1_z, proj2_z, tmp;
    for (int i = 0; i < param_3; i++) {

        proj.setall(0.0f);
        prev_idx = 0;
        next_idx = 0;

        u16 idx = i_pntData[i].field_0x0;
        if (i_pntData[i].field_0x0 < 0) {
            idx = i_path->m_num;
        }

        while (idx < i_path->m_num) {
            BOOL passed =
                daNpcT_chkPassed(param_1[i]->current.pos, i_path->m_points, idx, i_path->m_num,
                                 dPath_ChkClose(i_path), 0, &proj, &prev_idx, &next_idx);
            if (!passed) {
                break;
            }

            if (daNpcT_incIdx(i_path->m_num, idx, dPath_ChkClose(i_path), 0)) {
                idx = i_path->m_num;
            }
        }

        if (idx != i_path->m_num) {
            i_pntData[i].field_0x0 = idx;
        } else {
            i_pntData[i].field_0x0 = -1;
        }

        i_pntData[i].field_0x4 = prev_idx;
    }

    if (param_4 != NULL) {
        for (int i = 0; i < param_3; i++) {
            int count = 0;
            if (i_pntData[i].field_0x0 >= 0) {
                for (int j = 0; j < param_3; j++) {
                    if (i == j) {
                        continue;
                    }

                    if (i_pntData[j].field_0x0 < 0) {
                        count++;
                    } else if (i_pntData[i].field_0x0 == i_pntData[j].field_0x0) {
                        if (cM3d_Len2dSqPntAndSegLine(param_1[i]->current.pos.x,
                                            param_1[i]->current.pos.z,
                                            i_path->m_points[i_pntData[i].field_0x4].m_position.x,
                                            i_path->m_points[i_pntData[i].field_0x4].m_position.z,
                                            i_path->m_points[i_pntData[i].field_0x0].m_position.x,
                                            i_path->m_points[i_pntData[i].field_0x0].m_position.z,
                                            &proj1_x, &proj1_z, &tmp)
                            && cM3d_Len2dSqPntAndSegLine(param_1[j]->current.pos.x,
                                            param_1[j]->current.pos.z,
                                            i_path->m_points[i_pntData[i].field_0x4].m_position.x,
                                            i_path->m_points[i_pntData[i].field_0x4].m_position.z,
                                            i_path->m_points[i_pntData[i].field_0x0].m_position.x,
                                            i_path->m_points[i_pntData[i].field_0x0].m_position.z,
                                            &proj2_x, &proj2_z, &tmp)) {
                            s16 angle = cM_atan2s(
                                i_path->m_points[i_pntData[i].field_0x0].m_position.x
                                - i_path->m_points[i_pntData[i].field_0x4].m_position.x,
                                i_path->m_points[i_pntData[i].field_0x0].m_position.z
                                - i_path->m_points[i_pntData[i].field_0x4].m_position.z);
                            angle -= (int)cM_atan2s(proj2_x - proj1_x, proj2_z - proj1_z);
                            if ((u16)abs(angle) >= 0x4000) {
                                continue;
                            }

                            count++;
                        }
                    } else if (i_pntData[i].field_0x0 < i_pntData[j].field_0x0) {
                        count++;
                    }
                }

                param_4[i] = count;
            }
        }
    }

    return 1;
}

u8 daNpcT_getDistTableIdx(int param_0, int param_1) {
    return param_0 + param_1 * 0x14 + 0x5e;
}

void daNpcT_onEvtBit(u32 i_no) {
    dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[i_no]);
}

void daNpcT_offEvtBit(u32 i_no) {
    dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[i_no]);
}

BOOL daNpcT_chkEvtBit(u32 i_no) {
    return dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[i_no]);
}

void daNpcT_onTmpBit(u32 i_no) {
    dComIfGs_onTmpBit((u16)dSv_event_tmp_flag_c::tempBitLabels[i_no]);
}

void daNpcT_offTmpBit(u32 i_no) {
    dComIfGs_offTmpBit((u16)dSv_event_tmp_flag_c::tempBitLabels[i_no]);
}

BOOL daNpcT_chkTmpBit(u32 i_no) {
    return dComIfGs_isTmpBit((u16)dSv_event_tmp_flag_c::tempBitLabels[i_no]);
}

const dCcD_SrcGObjInf daNpcT_c::mCcDObjData = {
    {0, {{0, 0, 0}, {0, 0x00}, {0x79}}},
    {dCcD_SE_NONE, 0, 0, 0, 0},
    {dCcD_SE_NONE, 0, 0, 0, 0},
    {0},
};

#include "d/actor/d_a_npc2.cpp"

#include "d/actor/d_a_npc4.cpp"
