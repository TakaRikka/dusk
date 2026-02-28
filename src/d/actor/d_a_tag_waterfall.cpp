/**
 * d_a_tag_waterfall.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_waterfall.h"
#include "d/d_com_inf_game.h"
#include "d/d_debug_viewer.h"
#include "d/d_procname.h"
#include "f_op/f_op_camera_mng.h"

daTagWaterFall_HIO_c::daTagWaterFall_HIO_c() {
    child = -1;
    base_fog_far = 0.0f;
    base_fog_near = 10000.0f;
    color_blend_step_divisor = 3.0f;
    enable_fog_adjust = false;
    draw_ellipse = 0;
    enable_ellipse_adjust = 0;
    ellipse_radius_a = 0.0f;
    ellipse_radius_b = 0.0f;
    ellipse_rotate = 0.0f;
}

#if DEBUG
void daTagWaterFall_HIO_c::genMessage(JORMContext* ctx) {
    // "Zora river waterfall"
    ctx->genLabel("ゾーラ川の滝", 0x80000001);
    ctx->genLabel("-", 0x80000001);

    // "Fog adjustment parameters"
    ctx->genLabel("フォグ調整パラメータ", 0x80000001);
    ctx->genLabel("-", 0x80000001);
    // "Fog adjustment ON?"
    ctx->genCheckBox("フォグ調整ＯＮ？", &enable_fog_adjust, 0x1);
    // "Near direct value"
    ctx->genSlider("near直値", &base_fog_far, -100000.0f, 100000.0f);
    // "Far direct value"
    ctx->genSlider("far直値", &base_fog_near, -100000.0f, 100000.0f);
    // "Switching time"
    ctx->genSlider("切り替わり時間", &color_blend_step_divisor, 0.0f, 100.0f);
    ctx->genLabel("-", 0x80000001);
    ctx->genLabel("-", 0x80000001);
    ctx->genLabel("-", 0x80000001);

    // "Ellipse adjustment parameters"
    ctx->genLabel("楕円調整パラメータ", 0x80000001);
    ctx->genLabel("-", 0x80000001);
    // "Ellipse display"
    ctx->genCheckBox("楕円表示", &draw_ellipse, 0x1);
    ctx->genLabel("-", 0x80000001);
    // "Ellipse adjustment ON?"
    ctx->genCheckBox("楕円調整ＯＮ？", &enable_ellipse_adjust, 0x1);
    // "Ellipse X radius"
    ctx->genSlider("楕円Ｘ半径", &ellipse_radius_a, 0.0f, 10000.0f);
    // "Ellipse Z radius"
    ctx->genSlider("楕円Ｚ半径", &ellipse_radius_b, 0.0f, 10000.0f);
    // "Ellipse rotation"
    ctx->genSlider("楕円回転", &ellipse_rotate, 0.0f, 65536.0f);
}
#endif

static bool masterWaterfallTagExists;

static u32 m_master_id = 0xFFFFFFFF;

static daTagWaterFall_HIO_c l_HIO;

int daTagWaterFall_c::draw() {
#if DEBUG
    f32 paramX;
    f32 paramY;
    f32 rotCos;
    f32 rotSin;
    s16 angle;
    if (l_HIO.draw_ellipse || l_HIO.enable_ellipse_adjust) {
        angle = 0;
        cXyz startPos = current.pos;
        if (!l_HIO.draw_ellipse) {
            paramX = l_HIO.ellipse_radius_a * cM_ssin(angle);
            paramY = l_HIO.ellipse_radius_b * cM_scos(angle);
            rotCos = cM_scos(l_HIO.ellipse_rotate);
            rotSin = cM_ssin(l_HIO.ellipse_rotate);
        } else {
            paramX = mCylinderRadiusA * cM_ssin(angle);
            paramY = mCylinderRadiusB * cM_scos(angle);
            rotCos = cM_scos(shape_angle.y);
            rotSin = cM_ssin(shape_angle.y);
        }

        startPos.x += paramX * rotCos + paramY * rotSin;
        startPos.z += -paramX * rotSin + paramY * rotCos;

        cXyz endPos;
        for (int i = 0; i < 0x10; i++) {
            ANGLE_ADD(angle, 0x1000);
            endPos = current.pos;
            if (l_HIO.draw_ellipse == 0) {
                paramX = l_HIO.ellipse_radius_a * cM_ssin(angle);
                paramY = l_HIO.ellipse_radius_b * cM_scos(angle);
            } else {
                paramX = mCylinderRadiusA * cM_ssin(angle);
                paramY = mCylinderRadiusB * cM_scos(angle);
            }
            endPos.x += paramX * rotCos + paramY * rotSin;
            endPos.z += -paramX * rotSin + paramY * rotCos;

            dDbVw_drawLineXlu(startPos, endPos, (GXColor){0, 0xff, 0, 0xff}, 1, 0xc);

            startPos = endPos;
        }
    }
#endif

    return 1;
}

static int daTagWaterFall_Draw(daTagWaterFall_c* i_this) {
    return i_this->draw();
}

void* s_waterfall(void* pProc, void* param_1) {
    UNUSED(param_1);

    if (fopAcM_IsActor(pProc) &&
        fopAcM_GetName(pProc) == PROC_Tag_WaterFall &&
        !fpcM_IsCreating(fopAcM_GetID(pProc)) &&
        static_cast<daTagWaterFall_c*>(pProc)->checkHitWaterFallCamera()) {
        return pProc;
    }

    return NULL;
}

f32 daTagWaterFall_c::getEllipseY(f32 x) {
    // Ellipse is in standard form;
    // x,y-plane of ellipse is the horizontal world plane

    f32 ellipseRadiusB = mCylinderRadiusB;
    f32 ellipseRadiusA = mCylinderRadiusA;

#if DEBUG
    if (l_HIO.enable_ellipse_adjust) {
        ellipseRadiusB = l_HIO.ellipse_radius_b;
        ellipseRadiusA = l_HIO.ellipse_radius_a;
    }
#endif

    f32 result = JMAFastSqrt((ellipseRadiusB * ellipseRadiusB) *
                             (1.0f - ((x * x) / (ellipseRadiusA * ellipseRadiusA))));
    return result;
}

bool daTagWaterFall_c::checkHitWaterFall(cXyz objPos) {
    cXyz dst;

    s16 angleY = shape_angle.y;
    f32 ellipseRadiusA = mCylinderRadiusA;

#if DEBUG
    if (l_HIO.enable_ellipse_adjust) {
        angleY = l_HIO.ellipse_rotate;
        ellipseRadiusA = l_HIO.ellipse_radius_a;
    }
#endif

    mDoMtx_stack_c::YrotS(-angleY);

    mDoMtx_stack_c::transM(-current.pos.x, -current.pos.y, -current.pos.z);

    mDoMtx_stack_c::multVec(&objPos, &dst);

    f32 absEllipseY;
    if (std::abs(dst.x) < ellipseRadiusA) {
        absEllipseY = std::abs(getEllipseY(dst.x));
        if (std::abs(dst.z) < absEllipseY && current.pos.y < objPos.y) {
            // Instruction reordering occurs when mCylinderHeight == 0.0f is used
            if (!mCylinderHeight || (current.pos.y + mCylinderHeight) > objPos.y) {
                return true;
            }
        }

    }
    return false;
}

bool daTagWaterFall_c::checkHitWaterFallCamera() {
    return checkHitWaterFall(dComIfGp_getCamera(0)->lookat.eye);
}

int daTagWaterFall_c::execute() {
    if (m_master_id == fopAcM_GetID(this)) {
        if (mFrameCountdown) {
            const dScnKy_env_light_c* const envLight = dKy_getEnvlight();
            mFogNear = envLight->mFogNear;
            mFogFar = envLight->mFogFar;
            mFrameCountdown--;
            mColpat = g_env_light.mColpatWeather;
            mColorBlend = 1.0f;
        } else {
            u8 currCameraInWaterfall = FALSE;
            if (fpcM_Search(s_waterfall, this)) {
                currCameraInWaterfall = TRUE;
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

            cLib_chaseF(&mColorBlend, 1.0f, 1.0f / l_HIO.color_blend_step_divisor);

            if (!mPrevCameraInWaterfall) {
                dKy_custom_colset(7, mColpat, mColorBlend);
            } else {
                dKy_custom_colset(mColpat, 7, mColorBlend);
            }

            f32 instanceFogMultiplier;
            f32 hioFogMultiplier;

            if (l_HIO.enable_fog_adjust) {
                if (mPrevCameraInWaterfall) {
                    instanceFogMultiplier = 1.0f - mColorBlend;
                    hioFogMultiplier = mColorBlend;
                } else {
                    instanceFogMultiplier = mColorBlend;
                    hioFogMultiplier = 1.0f - mColorBlend;
                }

                dKy_fog_startendz_set(
                    (mFogNear * instanceFogMultiplier) +
                    (l_HIO.base_fog_far * hioFogMultiplier),
                    (mFogFar * instanceFogMultiplier) +
                    (l_HIO.base_fog_near * hioFogMultiplier),
                    1.0f
                );
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

#if DEBUG
        mDoHIO_deleteChild(l_HIO.child);
#endif
    }

    return 1;
}

static int daTagWaterFall_Delete(daTagWaterFall_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "Tag_WaterFall");
    return i_this->_delete();
}

cPhs_Step daTagWaterFall_c::create() {
    fopAcM_ct(this, daTagWaterFall_c);

    OS_REPORT("Tag_WaterFall PARAM %x %x %x \n", fopAcM_GetParam(this), current.angle.x, current.angle.y);

    if (masterWaterfallTagExists == false) {
        masterWaterfallTagExists = true;
        mIsMasterWaterfallTag = 1;
#if DEBUG
        // "Zora river waterfall"
        l_HIO.child = mDoHIO_createChild("ゾーラ川の滝", &l_HIO);
#else
        l_HIO.child = -1;
#endif
    }

    mCylinderRadiusA = static_cast<f32>(fopAcM_GetParam(this) & 0x0000FFFF);

    if (mCylinderRadiusA < 0.0f) {
        mCylinderRadiusB = 0.0f;
    }

    mCylinderHeight = static_cast<f32>((fopAcM_GetParam(this) >> 16) & 0xFFFF);

    if (mCylinderHeight < 0.0f) {
        mCylinderHeight = 0.0f;
    }

    mCylinderRadiusB = static_cast<f32>(current.angle.x & 0xFFFF);

    if (mCylinderRadiusB <= 0.0f) {
        mCylinderRadiusB = mCylinderRadiusA;
    }

    current.angle.x = shape_angle.x = 0;

    if (m_master_id + (1 << 16) == 0xFFFF) {
        m_master_id = fopAcM_GetID(this);

        mColorBlend = 0.0f;
        mFrameCountdown = 30;
    }

    return cPhs_COMPLEATE_e;
}

static int daTagWaterFall_Create(daTagWaterFall_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daTagWaterFall_Method = {
    (process_method_func)daTagWaterFall_Create,  (process_method_func)daTagWaterFall_Delete,
    (process_method_func)daTagWaterFall_Execute, (process_method_func)daTagWaterFall_IsDelete,
    (process_method_func)daTagWaterFall_Draw,
};

actor_process_profile_definition g_profile_Tag_WaterFall = {
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
