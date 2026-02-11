/**
 * d_a_tag_lv6CstaSw.cpp
 * Temple of Time Statue switch trigger
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_lv6CstaSw.h"

#include "d/d_debug_viewer.h"
#include "d/d_procname.h"

static daLv6CstaSw_HIO_c l_HIO;

daLv6CstaSw_HIO_c::daLv6CstaSw_HIO_c() {
    show_range = 0;
}

#if DEBUG
void daLv6CstaSw_HIO_c::genMessage(JORMContext* ctx) {
    // "Display range"
    ctx->genCheckBox("範囲表示", &show_range, 1);
}
#endif

void daLv6CstaSw_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
}

int daLv6CstaSw_c::create() {
    fopAcM_ct(this, daLv6CstaSw_c);
    mSwitch = getSw();
    setBaseMtx();

#if DEBUG
    l_HIO.entryHIO("石像ＳＷタグ");
#endif

    return cPhs_COMPLEATE_e;
}

fopAc_ac_c* daLv6CstaSw_c::searchSekizoAct(void* i_actor, void* param_1) {
    fopAc_ac_c* actor2 = (fopAc_ac_c*)param_1;
    fopAc_ac_c* actor = (fopAc_ac_c*)i_actor;
    if (actor != NULL && fopAcM_IsActor(actor) && fopAcM_GetProfName(actor) == PROC_CSTATUE) {
        return actor;
    }

    return NULL;
}

bool daLv6CstaSw_c::areaCheck() {
    bool in_area = false;

    fopAc_ac_c* sekizo_p = fopAcM_Search((fopAcIt_JudgeFunc)searchSekizoAct, this);
    if (sekizo_p == NULL) {
        return false;
    }

    cXyz vec_to_actor = current.pos - sekizo_p->current.pos;
    f32 dist_to_actor = vec_to_actor.absXZ();
    if (dist_to_actor <= scale.x * 100.0f) {
        in_area = true;
    }

    return in_area;
}

int daLv6CstaSw_c::Execute() {
    bool result = areaCheck();
    if (result == true) {
        if (!fopAcM_isSwitch(this, mSwitch)) {
            fopAcM_onSwitch(this, mSwitch);
        }
    } else {
        if (fopAcM_isSwitch(this, mSwitch)) {
            fopAcM_offSwitch(this, mSwitch);
        }
    }

    return 1;
}

int daLv6CstaSw_c::Draw() {
#if DEBUG
    if (l_HIO.show_range) {
        g_env_light.settingTevStruct(16, &current.pos, &tevStr);
        GXColor color = (GXColor){0, 0, 0xff, 0x80};
        dDbVw_drawCylinderXlu(current.pos, scale.x * 100.0f, scale.y * 100.0f, color, 1);
    }
#endif

    return 1;
}

int daLv6CstaSw_c::Delete() {
#if DEBUG
    l_HIO.removeHIO();
#endif

    return 1;
}

static int daLv6CstaSw_Draw(daLv6CstaSw_c* i_this) {
    return i_this->Draw();
}

static int daLv6CstaSw_Execute(daLv6CstaSw_c* i_this) {
    return i_this->Execute();
}

static int daLv6CstaSw_Delete(daLv6CstaSw_c* i_this) {
    int id = fopAcM_GetID(i_this);
    return i_this->Delete();
}

static int daLv6CstaSw_Create(fopAc_ac_c* i_this) {
    daLv6CstaSw_c* cStaSw = static_cast<daLv6CstaSw_c*>(i_this);
    int id = fopAcM_GetID(i_this);
    return cStaSw->create();
}

static actor_method_class l_daLv6CstaSw_Method = {
    (process_method_func)daLv6CstaSw_Create,  (process_method_func)daLv6CstaSw_Delete,
    (process_method_func)daLv6CstaSw_Execute, (process_method_func)NULL,
    (process_method_func)daLv6CstaSw_Draw,
};

actor_process_profile_definition g_profile_Tag_Lv6CstaSw = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_Lv6CstaSw,
    &g_fpcLf_Method.base,
    sizeof(daLv6CstaSw_c),
    0,
    0,
    &g_fopAc_Method.base,
    658,
    &l_daLv6CstaSw_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
