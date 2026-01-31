/**
 * d_a_tag_chgrestart.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_chgrestart.h"

#include "d/d_debug_viewer.h"
#include "d/d_procname.h"

class daTagChgRestart_HIO_c : public mDoHIO_entry_c {
public:
    daTagChgRestart_HIO_c();

    void genMessage(JORMContext* ctx);

    u8 show_range;
};

#if DEBUG
daTagChgRestart_HIO_c l_HIO;

daTagChgRestart_HIO_c::daTagChgRestart_HIO_c() {
    show_range = FALSE;
}

void daTagChgRestart_HIO_c::genMessage(JORMContext* ctx) {
    // "Room return position change tag"
    ctx->genLabel("部屋戻し位置変更タグ", 0);
    // "Display range"
    ctx->genCheckBox("範囲表示", &show_range, 1);
}
#endif

int daTagChgRestart_c::Create() {
    mVertices[0].x = -100.0f * scale.x;
    mVertices[0].z = -100.0f * scale.z;

    mVertices[1].x = 100.0f * scale.x;
    mVertices[1].z = -100.0f * scale.z;

    mVertices[2].x = 100.0f * scale.x;
    mVertices[2].z = 100.0f * scale.z;

    mVertices[3].x = -100.0f * scale.x;
    mVertices[3].z = 100.0f * scale.z;

    mVertices[0].y = mVertices[1].y = mVertices[2].y = mVertices[3].y = 0.0f;

    return 1;
}

int daTagChgRestart_c::create() {
    fopAcM_ct(this, daTagChgRestart_c);

    if (!Create()) {
        return cPhs_ERROR_e;
    }

#if DEBUG
    // "Room return position change tag"
    l_HIO.entryHIO("部屋戻し位置変更タグ");
#endif

    return cPhs_COMPLEATE_e;
}

int daTagChgRestart_c::execute() {
    if (fopAcM_isSwitch(this, daTagChgRestart_prm::getSwNo(this))) {
        return 1;
    }

    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz pos = player->current.pos;
    pos -= home.pos;

    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&pos, &pos);

    if (mVertices[0].x < pos.x && mVertices[0].z < pos.z && mVertices[2].x > pos.x &&
        mVertices[2].z > pos.z)
    {
        cXyz restart_pos = home.pos;
        s16 restart_angle = home.angle.y;

        u8 playerNo = daTagChgRestart_prm::getPlayerNo(this);
        if (playerNo != 0xFF) {
            int roomNo = fopAcM_GetRoomNo(this);
            dStage_roomDt_c* roomDt = dComIfGp_roomControl_getStatusRoomDt(roomNo);
            stage_actor_class* player_data = roomDt->getPlayer();

            stage_actor_data_class* entry_p = player_data->m_entries;
            int i = 0;
            for (i = 0; i < player_data->num; i++) {
                if ((entry_p->base.angle.z & 0xFF) == playerNo) {
                    break;
                }

                entry_p++;
            }

            if (i == player_data->num) {
                OS_REPORT_ERROR("部屋戻し位置変更の為のプレイヤー番号が不正です!<%d>\n", i);
                JUT_ASSERT(317, FALSE);
            }

            restart_pos = entry_p->base.position;
            restart_angle = entry_p->base.angle.y;
        }

        dComIfGs_setRestartRoom(restart_pos, restart_angle, fopAcM_GetRoomNo(player));
        fopAcM_onSwitch(this, daTagChgRestart_prm::getSwNo(this));
    }

    return 1;
}

int daTagChgRestart_c::draw() {
#if DEBUG
    if (l_HIO.show_range) {
        GXColor color = {0, 0, 0x80, 0x80};
        cXyz sp10[8];

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(current.angle.y);

        sp10[0] = mVertices[0];
        sp10[1] = mVertices[1];
        sp10[2] = mVertices[3];
        sp10[3] = mVertices[2];
        sp10[4] = mVertices[0];
        sp10[5] = mVertices[1];
        sp10[6] = mVertices[3];
        sp10[7] = mVertices[2];

        for (int i = 0; i < 8; i++) {
            mDoMtx_stack_c::multVec(&sp10[i], &sp10[i]);
        }
        for (int i = 0; i < 4; i++) {
            sp10[i].y += 1000.0f;
        }

        dDbVw_drawCube8pXlu(sp10, color);
    }
#endif

    return 1;
}

int daTagChgRestart_c::_delete() {
#if DEBUG
    l_HIO.removeHIO();
#endif
    return 1;
}

static int daTagChgRestart_Execute(daTagChgRestart_c* i_this) {
    return i_this->execute();
}

static int daTagChgRestart_Delete(daTagChgRestart_c* i_this) {
    int id = fopAcM_GetID(i_this);
    return i_this->_delete();
}

static int daTagChgRestart_Create(fopAc_ac_c* i_this) {
    daTagChgRestart_c* chgRestart = static_cast<daTagChgRestart_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return chgRestart->create();
}

static int daTagChgRestart_Draw(daTagChgRestart_c* i_this) {
    return i_this->draw();
}

static actor_method_class l_daTagChgRestart_Method = {
    (process_method_func)daTagChgRestart_Create,
    (process_method_func)daTagChgRestart_Delete,
    (process_method_func)daTagChgRestart_Execute,
    NULL,
#if DEBUG
    (process_method_func)daTagChgRestart_Draw,
#else
    NULL,
#endif
};

actor_process_profile_definition g_profile_Tag_ChgRestart = {
    fpcLy_CURRENT_e,            // mLayerID
    7,                          // mListID
    fpcPi_CURRENT_e,            // mListPrio
    PROC_Tag_ChgRestart,        // mProcName
    &g_fpcLf_Method.base,      // sub_method
    sizeof(daTagChgRestart_c),  // mSize
    0,                          // mSizeOther
    0,                          // mParameters
    &g_fopAc_Method.base,       // sub_method
    591,                        // mPriority
    &l_daTagChgRestart_Method,  // sub_method
    0x40000,                    // mStatus
    fopAc_ACTOR_e,              // mActorType
    fopAc_CULLBOX_CUSTOM_e,     // cullType
};
