#include "JSystem/JMath/JMath.h"
#include "SSystem/SComponent/c_list.h"
#include "SSystem/SComponent/c_tag.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_e_rb.h"
#include "d/actor/d_a_e_rd.h"
#include "d/actor/d_a_kytag08.h"
#include "d/actor/d_a_scene_exit.h"
#include "d/actor/d_a_swc00.h"
#include "d/actor/d_a_tag_chgrestart.h"
#include "d/actor/d_a_tag_mstop.h"
#include "d/d_attention.h"
#include "d/d_com_inf_game.h"
#include "d/d_debug_viewer.h"
#include "d/d_path.h"
#include "d/d_stage.h"
#include "dusk/settings.h"
#include "dusk/trigger_viewer.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_actor_tag.h"
#include "f_pc/f_pc_name.h"
#include "m_Do/m_Do_mtx.h"

namespace dusk {
namespace TriggerView {

static u8 s_opacity;

typedef void (*DrawCallback)(fopAc_ac_c*);

static void searchActorForCallback(s16 actorName, DrawCallback callback) {
    node_class* node = g_fopAcTg_Queue.mpHead;

    for (int i = 0; i < g_fopAcTg_Queue.mSize; i++) {
        if (node != NULL) {
            create_tag_class* tag = (create_tag_class*)node;
            fopAc_ac_c* actorData = (fopAc_ac_c*)tag->mpTagData;

            bool checkAll = actorName == -1;
            if (actorData != NULL && (fopAcM_GetName(actorData) == actorName || checkAll)) {
                callback(actorData);
            }
            node = node->mpNextNode;
        }
    }
}

static void drawSceneExit(fopAc_ac_c* actor) {
    daScex_c* scex = (daScex_c*)actor;

    cXyz points[8];
    points[0].set(-actor->scale.x, actor->scale.y, -actor->scale.z);
    points[1].set(actor->scale.x, actor->scale.y, -actor->scale.z);
    points[2].set(-actor->scale.x, actor->scale.y, actor->scale.z);
    points[3].set(actor->scale.x, actor->scale.y, actor->scale.z);
    points[4].set(-actor->scale.x, 0.0f, -actor->scale.z);
    points[5].set(actor->scale.x, 0.0f, -actor->scale.z);
    points[6].set(-actor->scale.x, 0.0f, actor->scale.z);
    points[7].set(actor->scale.x, 0.0f, actor->scale.z);

    mDoMtx_inverse(scex->mMatrix, mDoMtx_stack_c::get());
    mDoMtx_multVecArray(mDoMtx_stack_c::get(), points, points, 8);

    GXColor color = {0xFF, 0x00, 0xFF, s_opacity};
    dDbVw_drawCube8pXlu(points, color);
}

static void drawMidnaStop(fopAc_ac_c* actor) {
    daTagMstop_c* mstop = (daTagMstop_c*)actor;

    GXColor color = {0x4A, 0x36, 0xBA, s_opacity};
    dDbVw_drawCylinderXlu(mstop->current.pos, mstop->scale.x * 100.0f, mstop->scale.y, color, 1);
}

static void drawPlumTag(fopAc_ac_c* actor) {
    GXColor color = {0x00, 0xFF, 0x00, s_opacity};
    dDbVw_drawCylinderXlu(actor->current.pos, actor->scale.x * 100.0f, 1000000.0f, color, 1);
}

static void drawPlumSearch(fopAc_ac_c* actor) {
    GXColor color = {0xFF, 0x00, 0x00, s_opacity};
    const f32 search_dist = 500.0f;
    dDbVw_drawCircleXlu(actor->attention_info.position, search_dist + 160.0f, color, 1, 12);
}

static void drawSwitchArea(fopAc_ac_c* actor) {
    daSwc00_c* swc = (daSwc00_c*)actor;
    int shape_type = (fopAcM_GetParam(actor) >> 0x12) & 3;

    GXColor color = {0x00, 0x00, 0xFF, s_opacity};
    if (shape_type == 3) {
        dDbVw_drawCylinderXlu(swc->current.pos, JMAFastSqrt(swc->scale.x) - 30.0f, swc->scale.y, color, 1);
    } else if (shape_type == 0) {
        cXyz size = swc->field_0x574 - swc->field_0x568;
        size *= 0.5f;
        cXyz pos = swc->field_0x568 + size;
        csXyz angle(swc->current.angle.x, swc->current.angle.y, swc->current.angle.z);
        dDbVw_drawCubeXlu(pos, size, angle, color);
    }
}

static void drawEventArea(fopAc_ac_c* actor) {
    u8 type = (actor->shape_angle.z & 0xFF);
    if (type == 0xFF) {
        type = 0;
    }

    if (type == 15 || type == 16) {
        GXColor color = {0xFF, 0xFF, 0x00, s_opacity};
        cXyz points[8];
        points[0].set(-actor->scale.x, actor->scale.y, -actor->scale.z);
        points[1].set(actor->scale.x, actor->scale.y, -actor->scale.z);
        points[2].set(-actor->scale.x, actor->scale.y, actor->scale.z);
        points[3].set(actor->scale.x, actor->scale.y, actor->scale.z);
        points[4].set(-actor->scale.x, 0.0f, -actor->scale.z);
        points[5].set(actor->scale.x, 0.0f, -actor->scale.z);
        points[6].set(-actor->scale.x, 0.0f, actor->scale.z);
        points[7].set(actor->scale.x, 0.0f, actor->scale.z);

        mDoMtx_stack_c::transS(actor->home.pos.x, actor->home.pos.y, actor->home.pos.z);
        mDoMtx_stack_c::YrotS(actor->current.angle.y);
        mDoMtx_multVecArray(mDoMtx_stack_c::get(), points, points, 8);

        dDbVw_drawCube8pXlu(points, color);
    } else {
        GXColor outer_color = {0xFF, 0x00, 0x00, s_opacity};
        GXColor inner_color = {0x00, 0xFF, 0x00, s_opacity};
        cXyz pos = actor->current.pos;

        daAlink_c* player = (daAlink_c*)dComIfGp_getPlayer(0);
        if (player != NULL && pos.y < player->mLinkAcch.GetGroundH()) {
            pos.y = player->mLinkAcch.GetGroundH() + 100.0f;
        }

        const f32 inner_scale = 0.83f;
        dDbVw_drawCircleXlu(pos, actor->scale.x * inner_scale, inner_color, 1, 20);
        dDbVw_drawCircleXlu(pos, actor->scale.x, outer_color, 1, 20);
    }
}

static void drawEventTag(fopAc_ac_c* actor) {
    GXColor color = {0x00, 0xC8, 0xFF, s_opacity};
    u16 area_type = actor->home.angle.x & 0x8000;

    if (area_type == 0x8000) {
        cXyz start(actor->current.pos.x - (actor->scale.x * 0.5f), actor->current.pos.y,
                   actor->current.pos.z - (actor->scale.z * 0.5f));
        cXyz end(actor->current.pos.x + (actor->scale.x * 0.5f),
                 actor->current.pos.y + actor->scale.y,
                 actor->current.pos.z + (actor->scale.z * 0.5f));

        cXyz points[8];
        points[0].set(start.x, start.y, start.z);
        points[1].set(start.x, start.y, end.z);
        points[2].set(end.x, start.y, end.z);
        points[3].set(end.x, start.y, start.z);
        points[4].set(start.x, end.y, start.z);
        points[5].set(start.x, end.y, end.z);
        points[6].set(end.x, end.y, end.z);
        points[7].set(end.x, end.y, start.z);

        dDbVw_drawCube8pXlu(points, color);
    } else {
        cXyz pos = actor->current.pos;
        pos.y -= actor->scale.y;
        dDbVw_drawCylinderXlu(pos, actor->scale.x, actor->scale.y * 2, color, 1);
    }
}

static void drawTWGate(fopAc_ac_c* actor) {
    GXColor color = {0xFF, 0xFF, 0xFF, s_opacity};
    dDbVw_drawCylinderXlu(actor->current.pos, actor->scale.x * 100.0f, actor->scale.y * 100.0f, color, 1);
}

static u8 s_pathColorIndex = 0;

static void drawPaths(dStage_dPath_c* paths) {
    static const GXColor colors[8] = {
        {0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0x00, 0xFF, 0x00},
        {0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00}, {0xFF, 0x00, 0xFF}, {0x00, 0xFF, 0xFF},
    };

    cXyz cubeSize = {30.0f, 30.0f, 30.0f};
    csXyz cubeAngle = {0, 0, 0};

    for (int i = 0; i < (int)paths->num; i++) {
        dPath* path = &paths->m_path[i];
        GXColor color = colors[(s_pathColorIndex++) & 7];
        color.a = s_opacity;
        cXyz a, b;

        if (dPath_ChkClose(path) && path->m_num > 2) {
            a = (Vec)path->m_points[0].m_position;
            b = (Vec)path->m_points[(int)path->m_num - 1].m_position;
            dDbVw_drawLineXlu(a, b, color, 1, 10);
        }

        for (int j = 0; j < (int)path->m_num - 1; j++) {
            a = (Vec)path->m_points[j].m_position;
            b = (Vec)path->m_points[j + 1].m_position;
            dDbVw_drawLineXlu(a, b, color, 1, 10);
            dDbVw_drawCubeXlu(a, cubeSize, cubeAngle, color);
        }
        dDbVw_drawCubeXlu(b, cubeSize, cubeAngle, color);
    }
}

static void drawStagePaths() {
    dStage_dPath_c* stagePaths = g_dComIfG_gameInfo.play.getStage().getPath2Inf();
    if (stagePaths != nullptr) {
        drawPaths(stagePaths);
    }
}

static void drawCurrentRoomPaths() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (player == nullptr) {
        return;
    }

    s32 roomNo = fopAcM_GetRoomNo(player);
    if (roomNo < 0 || roomNo >= 64) {
        return;
    }

    dStage_dPath_c* roomPaths = dStage_roomControl_c::mStatus[roomNo].mRoomDt.getPath2Inf();
    if (roomPaths != nullptr) {
        drawPaths(roomPaths);
    }
}

static void drawCheckpointTag(fopAc_ac_c* actor) {
    daTagChgRestart_c* chk = (daTagChgRestart_c*)actor;

    GXColor color = {0x29, 0xF0, 0xFF, s_opacity};
    cXyz points[8];

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->current.angle.y);

    points[0] = chk->mVertices[0];
    points[1] = chk->mVertices[1];
    points[2] = chk->mVertices[3];
    points[3] = chk->mVertices[2];
    points[4] = chk->mVertices[0];
    points[5] = chk->mVertices[1];
    points[6] = chk->mVertices[3];
    points[7] = chk->mVertices[2];

    mDoMtx_multVecArray(mDoMtx_stack_c::get(), points, points, 8);

    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (player != nullptr) {
        for (int i = 0; i < 8; i++) {
            points[i].y = player->current.pos.y;
        }
    }

    for (int i = 0; i < 4; i++) {
        points[i].y += 1000.0f;
    }

    dDbVw_drawCube8pXlu(points, color);
}

static void drawTransformDists(fopAc_ac_c* actor) {
    if (fopAcM_GetGroup(actor) == 4 && !(actor->actor_status & fopAcStts_UNK_0x8000000_e)) {
        GXColor near_color = {0x00, 0xFF, 0x00, s_opacity};
        GXColor far_color = {0xFF, 0x00, 0x00, s_opacity};

        const f32 near_dist = 400.0f;
        const f32 far_dist = 5000.0f;

        dDbVw_drawCircleXlu(actor->eyePos, near_dist, near_color, 1, 20);
        dDbVw_drawCircleXlu(actor->eyePos, far_dist, far_color, 1, 20);

        const s16 view_range = 0x4000;
        cXyz offset(0.0f, 0.0f, far_dist);
        cXyz endpos;

        mDoMtx_stack_c::transS(actor->eyePos.x, actor->eyePos.y, actor->eyePos.z);
        mDoMtx_stack_c::YrotM(actor->shape_angle.y);
        mDoMtx_stack_c::YrotM(-view_range);
        mDoMtx_stack_c::multVec(&offset, &endpos);
        dDbVw_drawLineXlu(actor->eyePos, endpos, far_color, 1, 10);

        mDoMtx_stack_c::transS(actor->eyePos.x, actor->eyePos.y, actor->eyePos.z);
        mDoMtx_stack_c::YrotM(actor->shape_angle.y);
        mDoMtx_stack_c::YrotM(view_range);
        mDoMtx_stack_c::multVec(&offset, &endpos);
        dDbVw_drawLineXlu(actor->eyePos, endpos, far_color, 1, 10);

        mDoMtx_stack_c::transS(actor->eyePos.x, actor->eyePos.y, actor->eyePos.z);
        mDoMtx_stack_c::YrotM(actor->shape_angle.y);
        mDoMtx_stack_c::multVec(&offset, &endpos);
        dDbVw_drawLineXlu(actor->eyePos, endpos, far_color, 1, 10);
    }
}

static void drawAttentionDists(fopAc_ac_c* actor) {
    if (fopAcM_GetGroup(actor) != 4) {
        return;
    }

    GXColor lock_color = {0x00, 0x00, 0xFF, s_opacity};
    GXColor talk_color = {0x00, 0xFF, 0x00, s_opacity};

    dist_entry& lock_inf = dAttention_c::getDistTable(actor->attention_info.distances[fopAc_attn_LOCK_e]);
    dist_entry& talk_inf = dAttention_c::getDistTable(actor->attention_info.distances[fopAc_attn_TALK_e]);

    dDbVw_drawCircleXlu(actor->attention_info.position, lock_inf.mDistMax, lock_color, 1, 20);
    dDbVw_drawCircleXlu(actor->attention_info.position, talk_inf.mDistMax, talk_color, 1, 20);
}

static void drawPurpleMistAvoid(fopAc_ac_c* actor) {
    kytag08_class* tag = (kytag08_class*)actor;

    GXColor avoidColor = {0x00, 0xFF, 0x00, s_opacity};
    GXColor targetColor = {0xFF, 0x00, 0xFF, s_opacity};

    dDbVw_drawCircleXlu(tag->mAvoidPos, tag->mSize.x * 45.0f * tag->mSizeScale, avoidColor, 1, 20);

    cXyz cubeSize(10.0f, 10.0f, 10.0f);
    csXyz cubeAngle(0, 0, 0);
    dDbVw_drawCubeXlu(tag->mAvoidPos, cubeSize, cubeAngle, avoidColor);
    dDbVw_drawCubeXlu(tag->mTargetAvoidPos, cubeSize, cubeAngle, targetColor);
}

static void drawLeeverData(fopAc_ac_c* actor) {
    e_rb_class* leever = (e_rb_class*)actor;

    if (leever->isChild) {
        return;
    }

    GXColor color = {0xFF, 0x00, 0x00, s_opacity};
    GXColor color2 = {0x00, 0x00, 0xFF, s_opacity};

    cXyz pos = actor->current.pos;
    daAlink_c* player = (daAlink_c*)dComIfGp_getPlayer(0);
    if (player != nullptr && pos.y < player->mLinkAcch.GetGroundH()) {
        pos.y = player->mLinkAcch.GetGroundH() + 100.0f;
    }

    dDbVw_drawCircleXlu(pos, leever->appearRange * 100.0f, color, 1, 20);
    dDbVw_drawCircleXlu(pos, leever->field_0xa69 * 100.0f, color2, 1, 20);
}

void execute() {
    const auto& settings = getTransientSettings().triggerView;
    s_opacity = (u8)(255.0f * (settings.opacity / 100.0f));

    if (settings.loadZones) {
        searchActorForCallback(fpcNm_SCENE_EXIT_e, drawSceneExit);
    }

    if (settings.midnaStops) {
        searchActorForCallback(fpcNm_Tag_Mstop_e, drawMidnaStop);
    }

    if (settings.switchAreas) {
        searchActorForCallback(fpcNm_SWC00_e, drawSwitchArea);
    }

    if (settings.eventAreas) {
        searchActorForCallback(fpcNm_TAG_EVENT_e, drawEventTag);
        searchActorForCallback(fpcNm_TAG_EVTAREA_e, drawEventArea);
        searchActorForCallback(fpcNm_TAG_MYNA2_e, drawPlumTag);
        searchActorForCallback(fpcNm_MYNA2_e, drawPlumSearch);
    }

    if (settings.twilightGates) {
        searchActorForCallback(fpcNm_Tag_TWGate_e, drawTWGate);
    }

    if (settings.paths) {
        s_pathColorIndex = 0;
        drawStagePaths();
        drawCurrentRoomPaths();
    }

    if (settings.checkpoints) {
        searchActorForCallback(fpcNm_Tag_ChgRestart_e, drawCheckpointTag);
    }

    if (settings.transformDists) {
        searchActorForCallback(-1, drawTransformDists);
    }

    if (settings.attentionDists) {
        searchActorForCallback(-1, drawAttentionDists);
    }

    if (settings.purpleMistAvoid) {
        searchActorForCallback(fpcNm_KYTAG08_e, drawPurpleMistAvoid);
    }

    if (settings.leevers) {
        searchActorForCallback(fpcNm_E_RB_e, drawLeeverData);
    }
}

}  // namespace TriggerView
}  // namespace dusk
