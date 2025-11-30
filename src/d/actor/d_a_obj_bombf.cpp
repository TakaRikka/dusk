/*
 * d_a_obj_bombf.cpp
 * Bomb Flower
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_bombf.h"
#include "d/d_procname.h"

static const char l_arcName[] = "Bombf";

bool daObjBombf_c::createHeap() {
    field_0x570 = mDoExt_J3DModel__create((J3DModelData*)dComIfG_getObjectRes(l_arcName, 4),
                                          0x80000, 0x11000084);
    return (field_0x570 != NULL);
}

static bool daObjBombf_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjBombf_c*>(i_this)->createHeap();
}

int daObjBombf_c::create() {
    fopAcM_ct(this, daObjBombf_c);

    int phase = dComIfG_resLoad(&field_0x568, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, (heapCallbackFunc)daObjBombf_createHeap, 0x820)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, field_0x570->getBaseTRMtx());
        tevStr.room_no = fopAcM_GetRoomNo(this);
        attention_info.position = current.pos;
        eyePos = attention_info.position;
        field_0x574 = 0xffffffff;
        setMatrix();
        fopAcM_SetMin(this, -75.0f, 0.0f, -75.0f);
        fopAcM_SetMax(this, 75.0f, 30.0f, 75.0f);
    }

    return phase;
}

static int daObjBombf_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjBombf_c*>(i_this)->create();
}

daObjBombf_c::~daObjBombf_c() {
    dComIfG_resDelete(&field_0x568, l_arcName);
}

static int daObjBombf_Delete(daObjBombf_c* i_this) {
    i_this->~daObjBombf_c();
    return 1;
}

void daObjBombf_c::setMatrix() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + field_0x584, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    MTXCopy(mDoMtx_stack_c::now, field_0x570->mBaseTransformMtx);
}

static const Vec bombOffset = {0.0f, 7.0f, 0.0f};

int daObjBombf_c::execute() {
    u32 uVar;
    cXyz vStack;

    setMatrix();

    if (field_0x574 == -1) {
        if (field_0x578 != 0) {
            field_0x578--;
        } else {
            mDoMtx_multVec(field_0x570->getBaseTRMtx(), &bombOffset, &vStack);

            fopAc_ac_c* bomb_actor =
                dBomb_c::createFlowerBomb(&vStack, &shape_angle, fopAcM_GetRoomNo(this));
            field_0x574 = fopAcM_GetID(bomb_actor);
        }
    } else {
        fopAc_ac_c* actor = fopAcM_SearchByID(field_0x574);

        if (actor == NULL) {
            field_0x574 = -1;
            field_0x578 = 150;
        } else if (dBomb_c::checkFlowerBombWait(actor)) {
            mDoMtx_multVec(field_0x570->getBaseTRMtx(), &bombOffset, &actor->current.pos);
        }
    }
    return 1;
}

static int daObjBombf_Execute(daObjBombf_c* i_this) {
    return i_this->execute();
}

int daObjBombf_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(field_0x570, &tevStr);
    mDoExt_modelUpdateDL(field_0x570);
    return 1;
}

static int daObjBombf_Draw(daObjBombf_c* i_this) {
    return i_this->draw();
}

static actor_method_class l_daObjBombf_Method = {
    (process_method_func)daObjBombf_Create,  (process_method_func)daObjBombf_Delete,
    (process_method_func)daObjBombf_Execute, (process_method_func)NULL,
    (process_method_func)daObjBombf_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Bombf = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Obj_Bombf,         // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daObjBombf_c),   // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    251,                    // mPriority
    &l_daObjBombf_Method,   // sub_method
    0x40100,                // mStatus
    fopAc_ENV_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
