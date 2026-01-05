/**
 * d_a_tag_stream.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_stream.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_procname.h"

int daTagStream_c::create() {
    fopAcM_ct(this, daTagStream_c);

    mPriority = fopAcM_GetParam(this) >> 8;
    mPower = fopAcM_GetParam(this);
    field_0x56b = fopAcM_GetParam(this) >> 16;

    fopAcM_GetParam(this) >> 24 != 0 ? mParameters = 0 : mParameters = 1;

    scale.x *= 100.0f;
    scale.y *= 100.0f;
    scale.z *= 100.0f;

    if (m_top) {
        daTagStream_c* top = m_top;
        daTagStream_c* top_loop = m_top;

        for (; top; top = top->mNext) {
            if (top->getPriority() > this->getPriority()) {
                break;
            }
            top_loop = top;
        }

        if (top_loop == top) {
            mNext = m_top;
            m_top->field_0x570 = this;
            m_top = this;
        } else {
            top_loop->mNext = this;
            field_0x570 = top_loop;

            if (top) {
                top->field_0x570 = this;
                mNext = top;
            }
        }
    } else {
        m_top = this;
    }
    
    f32 float1 =  cM_scos(shape_angle.y) * cM_scos(shape_angle.x);
    f32 float2 = -cM_ssin(shape_angle.x);
    f32 float3 = cM_ssin(shape_angle.y) * cM_scos(shape_angle.x);

    speed.set(float3,float2,float1);

    return cPhs_COMPLEATE_e;
}

static int daTagStream_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagStream_c*>(i_this)->create();
}

daTagStream_c::~daTagStream_c() {
    if (field_0x570) {
        field_0x570->mNext = mNext;
    }

    if (mNext) {
        mNext->field_0x570 = field_0x570;
    }

    if (m_top == this) {
        m_top = mNext;
    }
};

static int daTagStream_Delete(daTagStream_c* i_this) {
    i_this->~daTagStream_c();
    return 1;
}

int daTagStream_c::execute() {
    if (field_0x56b == 0xFF || fopAcM_isSwitch(this,field_0x56b)) {
        mStreamOn = 1;
    } else {
        mStreamOn = 0;
    }

    return 1;
}

static int daTagStream_Execute(daTagStream_c* i_this) {
    return i_this->execute();
}

static bool daTagStream_Draw(daTagStream_c* param_0) {
    return true;
}

static actor_method_class l_daTagStream_Method = {
    (process_method_func)daTagStream_Create,
    (process_method_func)daTagStream_Delete,
    (process_method_func)daTagStream_Execute,
    0, // ?
    (process_method_func)daTagStream_Draw
};

actor_process_profile_definition g_profile_Tag_Stream = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Tag_Stream,        // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daTagStream_c),  // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    259,                    // mPriority
    &l_daTagStream_Method,  // sub_method
    0x44000,                // mStatus
    fopAc_ENV_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
