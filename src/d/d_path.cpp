/**
 * d_path.cpp
 * dolzel2 - Room Path & Point Management
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_path.h"
#include "d/d_com_inf_game.h"

#if DEBUG
#include "d/d_debug_viewer.h"
#endif

class dPath_HIO : public JORReflexible {
public:
    dPath_HIO() {}
    ~dPath_HIO();

    void genMessage(JORMContext* mctx) {
        mctx->genCheckBox("デバッグ表示", &flags, 2);
        mctx->genSlider("部屋番号", &roomNo, -2, 64);
        mctx->genLabel("-3=現在プレイヤーのいる部屋, -2=旧常駐、-1＝常駐", 0);
    }

    void Ct() {
        flags = 0;
        roomNo = -3;
    }

    int GetRoomNo() { return roomNo; }
    bool ChkDispLine() { return flags & 2; }

    /* 0x4 */ s8 id;
    /* 0x6 */ u16 flags;
    /* 0x8 */ s16 roomNo;
};

dPnt* dPath_GetPnt(const dPath* path, int pnt_index) {
    JUT_ASSERT(27, path != NULL);
    JUT_ASSERT(28, 0 <= pnt_index && pnt_index < path->m_num);

    if (path == NULL || path->m_points == NULL || pnt_index < 0 || pnt_index >= path->m_num) {
        return NULL;
    }

    return &path->m_points[pnt_index];
}

dPath* dPath_GetRoomPath(int path_index, int room_no) {
    dStage_dPath_c* pd;

    if (room_no == -1) {
        dStage_dt_c* stage = dComIfGp_getStage();
        pd = stage->getPath2Inf();
    } else {
        JUT_ASSERT(58, 0 <= room_no && room_no < 64);
        dStage_roomDt_c* roomDt = dComIfGp_roomControl_getStatusRoomDt(room_no);
        if (roomDt == NULL) {
            return NULL;
        }

        pd = roomDt->getPath2Inf();
    }

    if (pd == NULL || path_index < 0 || path_index >= pd->num) {
        return NULL;
    }

    return &pd->m_path[path_index];
}

dPath* dPath_GetNextRoomPath(const dPath* path, int room_no) {
    dStage_dPath_c* pd;

    if (room_no == -1) {
        dStage_dt_c* stage = dComIfGp_getStage();
        pd = stage->getPath2Inf();
    } else {
        dStage_roomDt_c* roomDt = dComIfGp_roomControl_getStatusRoomDt(room_no);
        if (roomDt == NULL) {
            return NULL;
        }

        pd = roomDt->getPath2Inf();
    }

    int next_id = path->m_nextID;
    if (pd == NULL || next_id == 0xFFFF) {
        return NULL;
    }

    JUT_ASSERT(111, 0 <= next_id && next_id < pd->num);
    if (next_id < 0 || next_id >= pd->num) {
        return NULL;
    }

    return &pd->m_path[next_id];
}

#if !PLATFORM_GCN  // Fakematch due to differing return type on non-GCN platforms
int dPath_GetPolyRoomPathVec(const cBgS_PolyInfo& polyinfo, cXyz* vec, int* param_2)
#else
u8 dPath_GetPolyRoomPathVec(const cBgS_PolyInfo& polyinfo, cXyz* vec, int* param_2)
#endif
{
    int room_no = dComIfG_Bgsp().GetRoomId(polyinfo);
    int path_idx = dComIfG_Bgsp().GetRoomPathId(polyinfo);

    vec->x = 0.0f;
    vec->y = 0.0f;
    vec->z = 0.0f;
    *param_2 = 0;
    
    if (room_no == -1) {
        return 0;
    }

    dPath* path = dPath_GetRoomPath(path_idx, room_no);
    if (path == NULL) {
        return 0;
    }

    if (path->swbit != 0xFF && dComIfGs_isSwitch(path->swbit, room_no)) {
        return 0;
    }

    int pnt_no = dComIfG_Bgsp().GetRoomPathPntNo(polyinfo);
    if (pnt_no == 0xFF || pnt_no < 0 || pnt_no >= path->m_num) {
        return 0;
    }

    dPnt* pnt_end = &path->m_points[pnt_no];

    dPnt* pnt_begin;
    if (pnt_no == path->m_num - 1) {
        pnt_begin = path->m_points;
    } else {
        pnt_begin = &path->m_points[pnt_no + 1];
    }

    vec->x = pnt_begin->m_position.x - pnt_end->m_position.x;
    vec->y = pnt_begin->m_position.y - pnt_end->m_position.y;
    vec->z = pnt_begin->m_position.z - pnt_end->m_position.z;
    *param_2 = path->field_0x4;

    return 1;
}

#if DEBUG
dPath_HIO::~dPath_HIO() {}

static dPath_HIO s_hio;
#endif

#if VERSION == VERSION_SHIELD_DEBUG
void dPath_Ct() {
    #if DEBUG
    s_hio.Ct();
    s_hio.id = mDoHIO_CREATE_CHILD("レール", &s_hio);
    #endif
}

void dPath_Dt() {
    mDoHIO_DELETE_CHILD(s_hio.id);
}

void dPath_Move() {}

void dPath_Draw() {
    #if DEBUG
    if (s_hio.ChkDispLine()) {
        static int start_counter;
        int counter = start_counter / 20;
        dStage_dPath_c* pd;

        int hio_roomNo = s_hio.GetRoomNo();
        if (hio_roomNo == -3) {
            int roomNo = dComIfGp_roomControl_getStayNo();
            dStage_roomDt_c* roomDt = dComIfGp_roomControl_getStatusRoomDt(roomNo);
            if (roomDt == NULL) {
                return;
            }

            pd = roomDt->getPath2Inf();
        } else if (hio_roomNo == -2) {
            pd = dComIfGp_getStage()->getPathInf();
        } else if (hio_roomNo == -1) {
            pd = dComIfGp_getStage()->getPath2Inf();
        } else {
            dStage_roomDt_c* roomDt = dComIfGp_roomControl_getStatusRoomDt(hio_roomNo);
            if (roomDt == NULL) {
                return;
            }

            pd = roomDt->getPath2Inf();
        }

        if (pd != NULL) {
            dPath* path = pd->m_path;
            for (int i = 0; i < pd->num; i++) {
                if (path->m_num >= 1) {
                    dPnt* pnt = path->m_points;
                    cXyz start_pos;
                    cXyz end_pos;

                    for (int j = 0; j < path->m_num - 1; j++) {
                        start_pos.set(pnt[0].m_position);
                        end_pos.set(pnt[1].m_position);

                        if (counter == 0) {
                            dDbVw_drawLineOpa(start_pos, end_pos, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, TRUE, 12);
                        } else {
                            dDbVw_drawLineOpa(start_pos, end_pos, (GXColor){0xFF, 0, 0, 0xFF}, TRUE, 12);
                        }

                        counter++;
                        if (counter >= 8) {
                            counter = 0;
                        }

                        pnt++;
                    }

                    if (dPath_ChkClose(path)) {
                        start_pos.set(pnt[0].m_position);
                        end_pos.set(path[1].m_points->m_position);

                        if (counter == 0) {
                            dDbVw_drawLineOpa(start_pos, end_pos, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, TRUE, 12);
                        } else {
                            dDbVw_drawLineOpa(start_pos, end_pos, (GXColor){0xFF, 0, 0, 0xFF}, TRUE, 12);
                        }

                        counter++;
                        if (counter >= 8) {
                            counter = 0;
                        }
                    }
                }

                path++;
            }
        }

        start_counter--;
        if (start_counter < 0) {
            start_counter = 160;
        }
    }
    #endif
}

static void dummy() {
    cXyz pos;
    GXColor color;
    dDbVw_drawSphereOpa(pos, 0.0f, color, 0);
}
#endif
