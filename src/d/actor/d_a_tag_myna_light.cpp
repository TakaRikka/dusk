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
    u8 uVar1 = (int)(dComIfGs_getTime() * 0.06666667f) & 0xFF;
    u8 turn_on_time = getTurnOnTime() & 0xFF;
    u8 turn_off_time = getTurnOffTime() & 0xFF;

    if (turn_off_time > turn_on_time) {
        if (uVar1 >= (getTurnOnTime() & 0xFF) && uVar1 < (getTurnOffTime() & 0xFF)) {
            field_0x578 = true;
        } else {
            field_0x578 = false;
        }
    } else {
        if (uVar1 >= (getTurnOffTime() & 0xFF) && uVar1 < (getTurnOnTime() & 0xFF)) {
            field_0x578 = false;
        } else {
            field_0x578 = true;
        }
    }

    if (mTurnOnFlag) {
        if (dComIfGp_roomControl_getStayNo() != 0) {
            mDoAud_seStartLevel(Z2SE_OBJ_MYNA_LIGHT_BURNING, (Vec*)&current.pos, 0,
                                  dComIfGp_getReverb(dComIfGp_roomControl_getStayNo()));
        } else {
            mDoAud_seStartLevel(Z2SE_OBJ_MYNA_LIGHT_BURNING, (Vec*)&current.pos, 0, 0);
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

u32 daTag_MynaLight_c::getTurnOnTime() {
    return fopAcM_GetParamBit(this, 0, 5);
}

u32 daTag_MynaLight_c::getTurnOffTime() {
    return fopAcM_GetParamBit(this, 5, 5);
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
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_TAG_MNLIGHT,
    &g_fpcLf_Method.base,
    sizeof(daTag_MynaLight_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x49,
    &daTag_MynaLight_MethodTable,
    0x64100,
    0x05,
    fopAc_CULLBOX_CUSTOM_e,
};
