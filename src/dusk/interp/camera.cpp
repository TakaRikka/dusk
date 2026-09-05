#include "dusk/interp/camera.h"

#include "dusk/game_clock.h"
#include "dusk/interp/frame_interpolation.h"
#include "dusk/interp/lerp.h"

#include "d/d_com_inf_game.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_graphic.h"

#include <cstring>
#include <utility>

namespace {

struct CameraSnapshot {
    cXyz eye{};
    cXyz center{};
    cXyz up{};
    s16 bank{};
    f32 fovy{};
    f32 aspect{};
    f32 near_{};
    f32 far_{};
    bool wideZoom{};
    bool valid{};
};

CameraSnapshot s_camPrev{};
CameraSnapshot s_camCurr{};

view_class s_presentationViewBackup{};

void copy_view_to_snap(CameraSnapshot* dst, const view_class& v) {
    dst->eye = v.lookat.eye;
    dst->center = v.lookat.center;
    dst->up = v.lookat.up;
    dst->bank = v.bank;
    dst->fovy = v.fovy;
    dst->aspect = v.aspect;
    dst->near_ = v.near_;
    dst->far_ = v.far_;
    dst->valid = true;
}

void apply_presented_view(view_class* view) {
    // FRAME INTERP TODO: Largely copied from d_camera's camera_draw function from this point, got any
    // better ideas?
    C_MTXPerspective(view->projMtx, view->fovy, view->aspect, view->near_, view->far_);
    mDoMtx_lookAt(view->viewMtx, &view->lookat.eye, &view->lookat.center, &view->lookat.up,
                  view->bank);
#if WIDESCREEN_SUPPORT
    mDoGph_gInf_c::setWideZoomProjection(view->projMtx);
#endif
    j3dSys.setViewMtx(view->viewMtx);
    cMtx_inverse(view->viewMtx, view->invViewMtx);

    bool camera_attention_status = dComIfGp_getCameraAttentionStatus(0) & 0x80;
    Z2GetAudience()->setAudioCamera(view->viewMtx, view->lookat.eye, view->lookat.center, view->fovy,
                                    view->aspect, camera_attention_status, 0, false);

    dBgS_GndChk gndchk;
    gndchk.OnWaterGrp();
    gndchk.SetPos(&view->lookat.eye);
    f32 cross = dComIfG_Bgsp().GroundCross(&gndchk);
    if (cross != -G_CM3D_F_INF) {
        if (dComIfG_Bgsp().ChkGrpInf(gndchk, 0x100)) {
            mDoAud_getCameraMapInfo(6);
        } else {
            mDoAud_getCameraMapInfo(dComIfG_Bgsp().GetMtrlSndId(gndchk));
        }
        mDoAud_setCameraGroupInfo(dComIfG_Bgsp().GetGrpSoundId(gndchk));
        Vec spDC;
        spDC.x = view->lookat.eye.x;
        spDC.y = cross;
        spDC.z = view->lookat.eye.z;
        Z2AudioMgr::getInterface()->setCameraPolygonPos(&spDC);
    } else {
        Z2AudioMgr::getInterface()->setCameraPolygonPos(nullptr);
    }

    MTXCopy(view->viewMtx, view->viewMtxNoTrans);
    view->viewMtxNoTrans[0][3] = 0.0f;
    view->viewMtxNoTrans[1][3] = 0.0f;
    view->viewMtxNoTrans[2][3] = 0.0f;
    cMtx_concatProjView(view->projMtx, view->viewMtx, view->projViewMtx);

    f32 far_;
    f32 var_f30;
    if (dComIfGp_getCameraAttentionStatus(0) & 8) {
        far_ = view->far_;
    } else {
#if DEBUG
        if (g_envHIO.mOther.mAdjustCullFar != 0) {
            var_f30 = g_envHIO.mOther.mCullFarValue;
        } else
#endif
        {
            var_f30 = dStage_stagInfo_GetCullPoint(dComIfGp_getStageStagInfo());
        }
        far_ = var_f30;
    }

    mDoLib_clipper::setup(view->fovy, view->aspect, view->near_, far_);

    // FRAME INTERP NOTE: Removed the call to offWideZoom that was here, it causes problems with
    // presentation during cutscenes.
}

}  // namespace

namespace dusk::interp {

void record_camera(::camera_process_class* cam, int camera_id) {
    if (!is_enabled() || camera_id != 0 || cam == nullptr) {
        return;
    }
    copy_view_to_snap(&s_camCurr, cam->view);
#if WIDESCREEN_SUPPORT
    s_camCurr.wideZoom = mDoGph_gInf_c::isWideZoom();
#endif
}

void interp_view(::view_class* view) {
    if (!is_enabled())
        return;

    if (!s_camPrev.valid || !s_camCurr.valid)
        return;

    const f32 step = get_interpolation_step();
    const bool is_cam_curr_authoritative = game_clock::is_sim_frame() && step <= 0.0f;

    cXyz eye;
    cXyz center;
    cXyz up;
    if (is_cam_curr_authoritative) {
        eye = s_camCurr.eye;
        center = s_camCurr.center;
        up = s_camCurr.up;
    } else {
        lerp(eye, s_camPrev.eye, s_camCurr.eye, step);
        lerp(center, s_camPrev.center, s_camCurr.center, step);
        lerp(up, s_camPrev.up, s_camCurr.up, step);
    }
    if (!up.normalizeRS()) {
        up = s_camCurr.up;
        up.normalizeRS();
    }

    view->lookat.eye = eye;
    view->lookat.center = center;
    view->lookat.up = up;
    if (is_cam_curr_authoritative) {
        view->bank = s_camCurr.bank;
        view->fovy = s_camCurr.fovy;
        view->aspect = s_camCurr.aspect;
        view->near_ = s_camCurr.near_;
        view->far_ = s_camCurr.far_;
    } else {
        view->bank = lerp(s_camPrev.bank, s_camCurr.bank, step);
        view->fovy = s_camPrev.fovy + (s_camCurr.fovy - s_camPrev.fovy) * step;
        view->aspect = s_camPrev.aspect + (s_camCurr.aspect - s_camPrev.aspect) * step;
        view->near_ = s_camPrev.near_ + (s_camCurr.near_ - s_camPrev.near_) * step;
        view->far_ = s_camPrev.far_ + (s_camCurr.far_ - s_camPrev.far_) * step;
    }

    // FRAME INTERP TODO: It might be better if I rewired the game to not clear this flag until the
    // next sim frame, but I don't care enough to right now
#if WIDESCREEN_SUPPORT
    const f32 wide_step = is_cam_curr_authoritative ? 1.0f : step;
    if (mDoGph_gInf_c::isWide() && !mDoGph_gInf_c::isWideZoom() &&
        wide_step >= 0.5f ? s_camCurr.wideZoom : s_camPrev.wideZoom)
    {
        mDoGph_gInf_c::onWideZoom();
    }
#endif
}

void camera_on_sim_tick() {
    s_camPrev = std::move(s_camCurr);
}

void camera_invalidate_snapshots() {
    s_camPrev.valid = false;
    s_camCurr.valid = false;
}

void camera_on_begin_record() {
    if (dComIfGp_getCamera(0) == nullptr) {
        camera_invalidate_snapshots();
    }
}

bool camera_apply_presentation() {
    if (!s_camPrev.valid || !s_camCurr.valid) {
        return false;
    }

    view_class* const view = dComIfGd_getView();
    if (view == nullptr) {
        return false;
    }

    std::memcpy(&s_presentationViewBackup, view, sizeof(view_class));
    interp_view(view);
    apply_presented_view(view);
    return true;
}

void camera_restore_presentation() {
    view_class* const view = dComIfGd_getView();
    if (view != nullptr) {
        std::memcpy(view, &s_presentationViewBackup, sizeof(view_class));
    }
}

}  // namespace dusk::interp
