/**
 * d_a_tag_guard.cpp
 * Tag - Guard
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_guard.h"
#include "d/d_stage.h"

int daTagGuard_c::getAppearPoint(Vec* i_appearPnt) {
    if (mPath == NULL) {
        return 0;
    }

    dPnt* pnt = dPath_GetPnt(mPath, mPath->m_num - 1);
    *i_appearPnt = pnt->m_position;
    return 1;
}

void daTagGuard_c::createGuard(u32 i_parameters) {
    cXyz pos = current.pos;

    if (getPathID() != 0xFF) {
        dPnt* pnt = dPath_GetPnt(mPath, 0);
        pos.set(pnt->m_position);
        i_parameters |= getPathID() << 0x10;
    }

    fopAcM_createChild(PROC_NPC_GUARD, fopAcM_GetID(this), i_parameters, &pos,
                       fopAcM_GetRoomNo(this), &current.angle, NULL, -1, NULL);
}

int daTagGuard_c::create() {
    fopAcM_ct(this, daTagGuard_c);
    create_init();
    return cPhs_COMPLEATE_e;
}

void daTagGuard_c::create_init() {
    mPath = dPath_GetRoomPath(getPathID(), fopAcM_GetHomeRoomNo(this));

    JUT_ASSERT(135, mPath != NULL);

    dPnt* pnt1 = dPath_GetPnt(mPath, 0);
    dPnt* pnt2 = dPath_GetPnt(mPath, 1);

#if DEBUG
    current.pos.set(pnt1->m_position);
#else
    //TODO: fakematch, using ::set causes a misplaced mr on GCN versions
    current.pos.x = pnt1->m_position.x;
    current.pos.y = pnt1->m_position.y;
    current.pos.z = pnt1->m_position.z;
#endif

    current.angle.y = cLib_targetAngleY(&pnt1->m_position, &pnt2->m_position);
}

static int daTagGuard_Create(fopAc_ac_c* i_this) {
    daTagGuard_c* guard = static_cast<daTagGuard_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return guard->create();
}

static int daTagGuard_Delete(daTagGuard_c* i_this) {
    int id = fopAcM_GetID(i_this);
    i_this->~daTagGuard_c();
    return 1;
}

static actor_method_class l_daTagGuard_Method = {
    (process_method_func)daTagGuard_Create,
    (process_method_func)daTagGuard_Delete,
};

actor_process_profile_definition g_profile_TAG_GUARD = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_TAG_GUARD,
    &g_fpcLf_Method.base,
    sizeof(daTagGuard_c),
    0,
    0,
    &g_fopAc_Method.base,
    408,
    &l_daTagGuard_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
