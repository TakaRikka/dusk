/**
 * d_path.cpp
 * dolzel2 - Room Path & Point Management
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_path.h"
#include "d/d_com_inf_game.h"

dPnt* dPath_GetPnt(dPath const* path, int pnt_index) {
    if (path == NULL || path->m_points == NULL || pnt_index < 0 || pnt_index >= path->m_num) {
        return NULL;
    }

    return &path->m_points[pnt_index];
}

dPath* dPath_GetRoomPath(int path_index, int room_no) {
    dStage_dPath_c* path;

    if (room_no == -1) {
        path = dComIfGp_getStage()->getPath2Inf();
    } else {
        dStage_roomDt_c* roomDt = dComIfGp_roomControl_getStatusRoomDt(room_no);
        if (roomDt == NULL) {
            return NULL;
        }

        path = roomDt->getPath2Inf();
    }

    if (path == NULL || path_index < 0 || path_index >= path->m_num) {
        return NULL;
    }

    return &path->m_path[path_index];
}

dPath* dPath_GetNextRoomPath(dPath const* p_path, int room_no) {
    dStage_dPath_c* path;

    if (room_no == -1) {
        path = dComIfGp_getStage()->getPath2Inf();
    } else {
        dStage_roomDt_c* roomDt = dComIfGp_roomControl_getStatusRoomDt(room_no);
        if (roomDt == NULL) {
            return NULL;
        }

        path = roomDt->getPath2Inf();
    }

    int next_id = p_path->m_nextID;
    if (path == NULL || next_id == 0xFFFF) {
        return NULL;
    }

    if (next_id < 0 || next_id >= path->m_num) {
        return NULL;
    }

    return &path->m_path[next_id];
}

#if !PLATFORM_GCN  // Fakematch due to differing return type on non-GCN platforms
int dPath_GetPolyRoomPathVec(cBgS_PolyInfo const& poly, cXyz* p_pathVec, int* param_2) {
#else
u8 dPath_GetPolyRoomPathVec(cBgS_PolyInfo const& poly, cXyz* p_pathVec, int* param_2) {
#endif
    int roomId = dComIfG_Bgsp().GetRoomId(poly);
    int roomPathId = dComIfG_Bgsp().GetRoomPathId(poly);

    p_pathVec->x = 0.0f;
    p_pathVec->y = 0.0f;
    p_pathVec->z = 0.0f;
    *param_2 = 0;
    
    if (roomId == -1) {
        return 0;
    }

    dPath* path = dPath_GetRoomPath(roomPathId, roomId);
    if (path == NULL) {
        return 0;
    }

    if (path->field_0x6 != 0xFF && dComIfGs_isSwitch(path->field_0x6, roomId)) {
        return 0;
    }

    int pnt_no = dComIfG_Bgsp().GetRoomPathPntNo(poly);
    if (pnt_no == 0xFF || pnt_no < 0 || pnt_no >= path->m_num) {
        return 0;
    }

    dPnt* pnt_start = path->m_points;
    dPnt* pnt_end = &pnt_start[pnt_no];

    dPnt* pnt_begin;
    if (pnt_no == path->m_num - 1) {
        pnt_begin = pnt_start;
    } else {
        pnt_begin = &pnt_start[pnt_no + 1];
    }

    p_pathVec->x = pnt_begin->m_position.x - pnt_end->m_position.x;
    p_pathVec->y = pnt_begin->m_position.y - pnt_end->m_position.y;
    p_pathVec->z = pnt_begin->m_position.z - pnt_end->m_position.z;
    *param_2 = path->field_0x4;

    return 1;
}
