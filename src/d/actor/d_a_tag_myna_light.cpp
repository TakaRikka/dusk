// Translation Unit: d_a_tag_myna_light

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_myna_light.h"
#include "d/d_com_inf_game.h"
#include "d/d_stage.h"
#include "d/d_particle.h"

int daTag_MynaLight_c::create() {
    fopAcM_ct(this, daTag_MynaLight_c);
    initialize();
    return 4;
}

int daTag_MynaLight_c::Delete() {
    for (int i = 0; i < 2; i++) {
        if (field_0x570[i] != NULL) {
            field_0x570[i]->quitImmortalEmitter();
            field_0x570[i]->becomeInvalidEmitter();
            field_0x570[i] = NULL;
        }
    }
    return 1;
}

int daTag_MynaLight_c::Execute() {
    u8 var_r30 = (int)(dComIfGs_getTime() * 0.06666667f);
    if (getTurnOffTime() > getTurnOnTime()) {
        if (var_r30 >= getTurnOnTime() && var_r30 < getTurnOffTime()) {
            field_0x578 = true;
        } else {
            field_0x578 = false;
        }
    } else {
        if (var_r30 >= getTurnOffTime() && var_r30 < getTurnOnTime()) {
            field_0x578 = false;
        } else {
            field_0x578 = true;
        }
    }

    if (mTurnOnFlag) {
        if (dComIfGp_roomControl_getStayNo() != 0) {
            mDoAud_seStartLevel(Z2SE_OBJ_MYNA_LIGHT_BURNING, &current.pos, 0,
                                dComIfGp_getReverb(dComIfGp_roomControl_getStayNo()));
        } else {
            mDoAud_seStartLevel(Z2SE_OBJ_MYNA_LIGHT_BURNING, &current.pos, 0, 0);
        }
    }
    return 1;
}

int daTag_MynaLight_c::Draw() {
    if (mTurnOnFlag) {
        cLib_addCalc(&field_0x56c, 1.0f, 0.5f, 0.1f, 0.0001f);
    } else {
        cLib_addCalc(&field_0x56c, 0.0f, 0.5f, 0.1f, 0.0001f);
    }

    if (field_0x56c >= 0.000001f) {
        GXColor color = {0xBC, 0x66, 0x42, 0xFF};
        dKy_BossLight_set(&current.pos, &color, field_0x56c, 0);
    }
    return 1;
}

bool daTag_MynaLight_c::setTurnOnOffChange() {
    if (checkTurnOnOffChange()) {
        mTurnOnFlag = field_0x578;
        if (mTurnOnFlag) {
            if (dComIfGp_roomControl_getStayNo() != 0) {
                mDoAud_seStart(Z2SE_OBJ_MYNA_LIGHT_ON, (Vec*)&current.pos, 0,
                               dComIfGp_getReverb(dComIfGp_roomControl_getStayNo()));
            } else {
                mDoAud_seStart(Z2SE_OBJ_MYNA_LIGHT_ON, (Vec*)&current.pos, 0, 0);
            }

            cXyz unused_scale = cXyz(-5122.0f, -124.0f, -3067.0f);
            cXyz scale_1 = cXyz(0.4f, 0.4f, 0.4f);
            cXyz scale_2 = cXyz(0.25f, 0.25f, 0.25f);
            if (field_0x570[0] == NULL) {
                field_0x570[0] =
                    dComIfGp_particle_set(0x3AD, &current.pos, &current.angle, &scale_1);
            }
            if (field_0x570[1] == NULL) {
                field_0x570[1] =
                    dComIfGp_particle_set(0x3AF, &current.pos, &current.angle, &scale_2);
            }
        } else {
            if (dComIfGp_roomControl_getStayNo() != 0) {
                mDoAud_seStart(Z2SE_OBJ_MYNA_LIGHT_OFF, (Vec*)&current.pos, 0,
                               dComIfGp_getReverb(dComIfGp_roomControl_getStayNo()));
            } else {
                mDoAud_seStart(Z2SE_OBJ_MYNA_LIGHT_OFF, (Vec*)&current.pos, 0, 0);
            }

            for (int i = 0; i < 2; i++) {
                if (field_0x570[i] != NULL) {
                    field_0x570[i]->quitImmortalEmitter();
                    field_0x570[i]->becomeInvalidEmitter();
                    field_0x570[i] = NULL;
                }
            }
        }
    }
    return mTurnOnFlag;
}

u8 daTag_MynaLight_c::getTurnOnTime() {
    u8 param = fopAcM_GetParam(this) & 0x1F;
    return param;
}

u8 daTag_MynaLight_c::getTurnOffTime() {
    u8 param = (fopAcM_GetParam(this) & 0x3E0) >> 5;
    return param;
}

void daTag_MynaLight_c::initialize() {
    fopAcM_setCullSizeBox(this, -30.0, -15.0, -30.0, 30.0, 45.0, 30.0);
    attention_info.flags = 0;
    current.angle.set(0, home.angle.y, 0);
    shape_angle = current.angle;
    field_0x56c = 0.0f;
    mTurnOnFlag = false;
    field_0x578 = false;
    for (int i = 0; i < 2; i++) {
        field_0x570[i] = NULL;
    }
}

static int daTag_MynaLight_Create(void* i_this) {
    return static_cast<daTag_MynaLight_c*>(i_this)->create();
}

static int daTag_MynaLight_Delete(void* i_this) {
    return static_cast<daTag_MynaLight_c*>(i_this)->Delete();
}

static int daTag_MynaLight_Execute(void* i_this) {
    return static_cast<daTag_MynaLight_c*>(i_this)->Execute();
}

static int daTag_MynaLight_Draw(void* i_this) {
    return static_cast<daTag_MynaLight_c*>(i_this)->Draw();
}

static int daTag_MynaLight_IsDelete(void* i_this) {
    return 1;
}

daTag_MynaLight_c::~daTag_MynaLight_c() {}

static actor_method_class daTag_MynaLight_MethodTable = {
    (process_method_func)daTag_MynaLight_Create,  (process_method_func)daTag_MynaLight_Delete,
    (process_method_func)daTag_MynaLight_Execute, (process_method_func)daTag_MynaLight_IsDelete,
    (process_method_func)daTag_MynaLight_Draw,
};

actor_process_profile_definition g_profile_TAG_MNLIGHT = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 7,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_TAG_MNLIGHT_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTag_MynaLight_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_TAG_MNLIGHT_e,
    /* Actor SubMtd */ &daTag_MynaLight_MethodTable,
    /* Status       */ fopAcStts_UNK_0x40000_e | fopAcStts_NOPAUSE_e | fopAcStts_UNK_0x4000_e | fopAcStts_CULL_e,
    /* Group        */ fopAc_UNK_GROUP_5_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
