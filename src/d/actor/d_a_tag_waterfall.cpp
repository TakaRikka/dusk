/**
 * d_a_tag_waterfall.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_waterfall.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "f_op/f_op_camera_mng.h"

static u32 m_master_id = static_cast<u32>(0xFFFFFFFF);

daTagWaterFall_HIO_c::daTagWaterFall_HIO_c() {
    field_0x04 = -1;
    mBaseFogNear = 0.0f;
    mBaseFogFar = 10000.0f;
    mColorBlendStepDivisor = 3.0f;
    mShouldModifyFog = false;
    field_0x14 = 0;
    field_0x15 = 0;
    field_0x18 = 0.0f;
    field_0x1C = 0.0f;
    field_0x20 = 0.0f;
}

int daTagWaterFall_c::draw() {
    return 1;
}

static int daTagWaterFall_Draw(daTagWaterFall_c* i_this) {
    return i_this->draw();
}

void* s_waterfall(void* pProc, void* unused) {
    if (fopAc_IsActor(pProc)) {
        if (fopAcM_GetName(pProc) == PROC_Tag_WaterFall) {
            daTagWaterFall_c* pProcWaterfallTag = static_cast<daTagWaterFall_c*>(pProc);

            s32 id = fpcM_GetID(pProc);
            if (!fpcM_IsCreating(id)) {
                if (pProcWaterfallTag->checkHitWaterFallCamera()) {
                    return pProc;
                }
            }
        }
    }

    return NULL;
}

f32 daTagWaterFall_c::getEllipseY(f32 x) {
    // Ellipse is in standard form;
    // x,y-plane of ellipse is the horizontal world plane

    f32 ellipseRadiusB = mCylinderRadiusB;
    f32 ellipseRadiusA = mCylinderRadiusA;
    f32 ellipseRadiusBSquared = ellipseRadiusB * ellipseRadiusB;
    f32 ellipseRadiusASquared = ellipseRadiusA * ellipseRadiusA;

    f32 ellipseYSquared_OverBSquared = (1.0f - ((x * x) / (ellipseRadiusASquared)));
    f32 ellipseYSquared = (ellipseRadiusBSquared)*ellipseYSquared_OverBSquared;

    return JMAFastSqrt(ellipseYSquared);
}

bool daTagWaterFall_c::checkHitWaterFall(cXyz objPos) {
    Vec dst;

    f32 ellipseRadiusA = mCylinderRadiusA;

    mDoMtx_stack_c::YrotS(-fopAcM_GetShapeAngle_p(this)->y);

    const cXyz& waterfallTagPos = fopAcM_GetPosition(this);
    mDoMtx_stack_c::transM(-waterfallTagPos.x, -waterfallTagPos.y, -waterfallTagPos.z);

    mDoMtx_stack_c::multVec(&objPos, &dst);

    if (fabsf(dst.x) < ellipseRadiusA) {
        const f32 absEllipseY = fabsf(getEllipseY(dst.x));
        if (fabsf(dst.z) < absEllipseY) {
            if (waterfallTagPos.y < objPos.y) {
                // Instruction reordering occurs when mCylinderHeight == 0.0f is used
                if (!mCylinderHeight || (waterfallTagPos.y + mCylinderHeight) > objPos.y) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool daTagWaterFall_c::checkHitWaterFallCamera() {
    const camera_class* camera = dComIfGp_getCamera(0);
    return checkHitWaterFall(camera->lookat.eye);
}

static bool masterWaterfallTagExists;

static daTagWaterFall_HIO_c l_HIO;

int daTagWaterFall_c::execute() {
    u32 masterId = m_master_id;
    u32 waterfallId = fpcM_GetID(this);

    if (masterId == waterfallId) {
        if (mFrameCountdown) {
            const dScnKy_env_light_c* const envLight = dKy_getEnvlight();
            mFogNear = envLight->mFogNear;
            mFogFar = envLight->mFogFar;
            mFrameCountdown--;
            mColpat = envLight->mColpatWeather;
            mColorBlend = 1.0f;
        } else {
            bool currCameraInWaterfall = false;
            if (fpcEx_Search(s_waterfall, this)) {
                currCameraInWaterfall = true;
            }

            if (!currCameraInWaterfall) {
                if (!mPrevCameraInWaterfall && mColorBlend == 1.0f) {
                    return 1;
                }
            }

            if ((!mPrevCameraInWaterfall && currCameraInWaterfall) ||
                (mPrevCameraInWaterfall && !currCameraInWaterfall))
            {
                mColorBlend = 1.0f - mColorBlend;
                mPrevCameraInWaterfall = currCameraInWaterfall;
            }

            cLib_chaseF(&mColorBlend, 1.0f, 1.0f / l_HIO.mColorBlendStepDivisor);

            if (!mPrevCameraInWaterfall) {
                dKy_custom_colset(7, mColpat, mColorBlend);
            } else {
                dKy_custom_colset(mColpat, 7, mColorBlend);
            }

            f32 instanceFogMultiplier;
            f32 HIOFogMultiplier;

            if (l_HIO.mShouldModifyFog) {
                if (mPrevCameraInWaterfall) {
                    instanceFogMultiplier = 1.0f - mColorBlend;
                    HIOFogMultiplier = mColorBlend;
                } else {
                    instanceFogMultiplier = mColorBlend;
                    HIOFogMultiplier = 1.0f - mColorBlend;
                }

                f32 fogParam0 =
                    (mFogNear * instanceFogMultiplier) + (l_HIO.mBaseFogNear * HIOFogMultiplier);
                f32 fogParam1 =
                    (mFogFar * instanceFogMultiplier) + (l_HIO.mBaseFogFar * HIOFogMultiplier);

                dKy_fog_startendz_set(fogParam0, fogParam1, 1.0f);
            }
        }
    }

    return 1;
}


static int daTagWaterFall_Execute(daTagWaterFall_c* i_this) {
    return i_this->execute();
}

static int daTagWaterFall_IsDelete(daTagWaterFall_c* i_this) {
    return 1;
}

int daTagWaterFall_c::_delete() {
    if (mIsMasterWaterfallTag) {
        masterWaterfallTagExists = false;
    }
    return 1;
}

static int daTagWaterFall_Delete(daTagWaterFall_c* i_this) {
    return i_this->_delete();
}

cPhs__Step daTagWaterFall_c::create() {
    fopAcM_ct(this, daTagWaterFall_c);

    if (masterWaterfallTagExists == false) {
        masterWaterfallTagExists = true;
        mIsMasterWaterfallTag = 1;
        l_HIO.field_0x04 = -1;
    }

    mCylinderRadiusA = static_cast<f32>(fpcM_GetParam(this) & 0x0000FFFF);

    if (mCylinderRadiusA < 0.0f) {
        mCylinderRadiusB = 0.0f;
    }

    mCylinderHeight = static_cast<f32>((fpcM_GetParam(this) & 0xFFFF0000) >> 16);

    if (mCylinderHeight < 0.0f) {
        mCylinderHeight = 0.0f;
    }

    mCylinderRadiusB = static_cast<f32>((fopAcM_GetAngle_p(this)->x & 0x0000FFFF));

    if (mCylinderRadiusB <= 0.0f) {
        mCylinderRadiusB = mCylinderRadiusA;
    }

    fopAcM_GetShapeAngle_p(this)->x = 0;
    fopAcM_GetAngle_p(this)->x = 0;

    if (m_master_id + (1 << 16) == 0xFFFF) {
        u32 id = fopAcM_GetID(this);

        m_master_id = id;

        mColorBlend = 0.0f;
        mFrameCountdown = 30;
    }

    return cPhs_COMPLEATE_e;
}

static int daTagWaterFall_Create(daTagWaterFall_c* i_this) {
    return i_this->create();
}

daTagWaterFall_HIO_c::~daTagWaterFall_HIO_c() {}

static actor_method_class l_daTagWaterFall_Method = {
    (process_method_func)daTagWaterFall_Create,  (process_method_func)daTagWaterFall_Delete,
    (process_method_func)daTagWaterFall_Execute, (process_method_func)daTagWaterFall_IsDelete,
    (process_method_func)daTagWaterFall_Draw,
};

extern actor_process_profile_definition g_profile_Tag_WaterFall = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_WaterFall,
    &g_fpcLf_Method.base,
    sizeof(daTagWaterFall_c),
    0,
    0,
    &g_fopAc_Method.base,
    198,
    &l_daTagWaterFall_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
