/**
 * d_a_tag_watchge.cpp
 * Tag - Guay
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_watchge.h"
#include "f_pc/f_pc_name.h"

int daTagWatchGe_c::draw() {
    return 1;
}

static int daTagWatchGe_Draw(daTagWatchGe_c* i_this) {
    return i_this->draw();
}

static s32 m_group_count;

static void* s_watchge(void* i_guayP, void* i_this) {
    u8 group_no = static_cast<daTagWatchGe_c*>(i_this)->getGroupNo();
    if (fopAcM_GetName(i_guayP) == fpcNm_E_GE_e) {
        if (static_cast<daE_GE_c*>(i_guayP)->getGroupNo() == group_no) {
            m_group_count++;
        }
    }

    return 0;
}

int daTagWatchGe_c::execute() {
    if (field_0x56a != 0) {
        field_0x56a--;
    } else {
        m_group_count = 0;
        fopAcM_Search(s_watchge,this);

        if (m_group_count == 0) {
            dComIfGs_onSwitch(field_0x568,fopAcM_GetRoomNo(this));
            fopAcM_delete(this);
        }
    }

    return 1;
}

static int daTagWatchGe_Execute(daTagWatchGe_c* i_this) {
    return i_this->execute();
}

static int daTagWatchGe_IsDelete(daTagWatchGe_c* i_this) {
    return 1;
}

static int daTagWatchGe_Delete(daTagWatchGe_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "Tag_WatchGe");
    return 1;
}

int daTagWatchGe_c::create() {
    fopAcM_ct(this, daTagWatchGe_c);
    OS_REPORT("Tag_WatchGe PARAM %x\n", fopAcM_GetParam(this));
    mGroupNo = fopAcM_GetParam(this);

    if (mGroupNo == 0xFF) {
        OS_REPORT("監視するグェーのグループが指定されてません！");
        return cPhs_ERROR_e;
    } else {
        field_0x568 = (fopAcM_GetParam(this) >> 8) & 0xFF;

        if (field_0x568 == 0xFF) {
            OS_REPORT("グェー監視タグのスイッチが指定されてません！");
            return cPhs_ERROR_e;
        }
        if (dComIfGs_isSwitch(field_0x568, fopAcM_GetRoomNo(this)) != 0) {
            return cPhs_ERROR_e;
        }
    }

    field_0x56a = 0x1e;
    return cPhs_COMPLEATE_e;
}

static int daTagWatchGe_Create(daTagWatchGe_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daTagWatchGe_Method = {
    (process_method_func)daTagWatchGe_Create,
    (process_method_func)daTagWatchGe_Delete,
    (process_method_func)daTagWatchGe_Execute,
    (process_method_func)daTagWatchGe_IsDelete,
    (process_method_func)daTagWatchGe_Draw
};

actor_process_profile_definition g_profile_Tag_WatchGe = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Tag_WatchGe_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagWatchGe_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Tag_WatchGe_e,
    /* Actor SubMtd */ &l_daTagWatchGe_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
