#include "dusk/frame_interpolation.h"

#include <memory>
#include "mtx.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_graphic.h"

namespace {

struct Recording {
    std::unordered_map<uintptr_t, Mtx> matrix_values;
};

bool s_initialized = false;

bool g_enabled = false;
bool g_recording = false;
bool g_interpolating = false;
bool g_sync_presentation = false;

float g_step = 0.0f;
bool g_is_sim_frame = false;
bool g_ui_tick_pending = false;

Recording g_current_recording;
Recording g_previous_recording;

std::unordered_map<uintptr_t, Mtx> g_replacements;

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

CameraSnapshot s_cam_prev{};
CameraSnapshot s_cam_curr{};

view_class s_presentation_view_backup{};
int s_presentation_depth = 0;

struct InterpolationCallBackWork {
    dusk::frame_interp::InterpolationCallBack pCallBack;
    void* pUserWork;
};

std::vector<InterpolationCallBackWork> s_interpolationCallBackWork;

void set_enabled(bool enabled) {
    if (g_enabled == enabled)
        return;

    g_enabled = enabled;

    if (!g_enabled)
        s_interpolationCallBackWork.clear();
}

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

inline void lerp_matrix(Mtx out, const Mtx lhs, const Mtx rhs, float step) {
    const float old_weight = 1.0f - step;
    for (size_t row = 0; row < 3; ++row) {
        for (size_t col = 0; col < 4; ++col) {
            out[row][col] = lhs[row][col] * old_weight + rhs[row][col] * step;
        }
    }
}

inline void lerp_xyz(cXyz* out, const cXyz& lhs, const cXyz& rhs, float step) {
    const float old_weight = 1.0f - step;
    out->x = lhs.x * old_weight + rhs.x * step;
    out->y = lhs.y * old_weight + rhs.y * step;
    out->z = lhs.z * old_weight + rhs.z * step;
}

static s16 lerp_bank(s16 a, s16 b, f32 t) {
    const f32 ra = S2RAD(a);
    const f32 d = remainderf(S2RAD(b) - ra, 2.0f * static_cast<f32>(M_PI));
    return cAngle::Radian_to_SAngle(ra + d * t);
}

inline bool matrix_differs(const Mtx lhs, const Mtx rhs, float epsilon = 0.0001f) {
    for (size_t row = 0; row < 3; ++row) {
        for (size_t col = 0; col < 4; ++col) {
            if (std::abs(lhs[row][col] - rhs[row][col]) > epsilon) {
                return true;
            }
        }
    }
    return false;
}

const Mtx* resolve_replacement(const Mtx* source, Mtx* scratch) {
    if (!g_interpolating || source == nullptr || dusk::frame_interp::presentation_sync_active()) {
        return source;
    }

    auto it = g_replacements.find(reinterpret_cast<uintptr_t>(source));
    if (it == g_replacements.end()) {
        return source;
    }

    MTXCopy(it->second, *scratch);
    return scratch;
}

bool has_recording_data(const Recording& recording) {
    return !recording.matrix_values.empty();
}

void clear_replacements() {
    g_replacements.clear();
}

}  // namespace

namespace dusk::frame_interp {
void ensure_initialized() {
    set_enabled(getSettings().game.enableFrameInterpolation);
    s_initialized = true;
}

bool is_enabled() {
    return g_enabled;
}

void begin_frame(bool is_sim_frame, float step) {
    g_is_sim_frame = is_sim_frame;
    g_step = std::clamp(step, 0.0f, 1.0f);
    if (is_sim_frame) {
        s_interpolationCallBackWork.clear();
    }
}

void begin_record() {
    ensure_initialized();

    if (!g_enabled) {
        g_interpolating = false;
        g_sync_presentation = false;
        g_previous_recording = {};
        g_current_recording = {};
        clear_replacements();
        s_cam_prev.valid = false;
        s_cam_curr.valid = false;
        return;
    }

    g_sync_presentation = false;
    g_previous_recording = std::move(g_current_recording);
    g_current_recording = {};
    g_recording = true;
    g_interpolating = false;
    clear_replacements();

    ::camera_process_class* cam = dComIfGp_getCamera(0);
    if (cam == nullptr) {
        s_cam_prev.valid = false;
        s_cam_curr.valid = false;
        return;
    }
}

void end_record() {
    g_recording = false;
}

void interpolate() {
    ensure_initialized();
    clear_replacements();
    g_interpolating = g_enabled && !g_recording && !g_sync_presentation && has_recording_data(g_current_recording);
    if (!g_interpolating) {
        return;
    }
    for (auto const& old : g_previous_recording.matrix_values) {
        if (auto it = g_current_recording.matrix_values.find(old.first);
            it != g_current_recording.matrix_values.end())
        {
            lerp_matrix(g_replacements[old.first], old.second, it->second, g_step);
        }
    }
}

void request_presentation_sync() {
    ensure_initialized();
    if (!g_enabled) {
        return;
    }
    g_sync_presentation = true;
}

bool presentation_sync_active() {
    if (!s_initialized || !g_enabled) {
        return false;
    }
    return g_sync_presentation;
}

float get_interpolation_step() {
    ensure_initialized();
    return presentation_sync_active() ? 1.0f : g_step;
}

void set_ui_tick_pending(bool value) {
    if (g_ui_tick_pending == value) { return; }
    g_ui_tick_pending = value;
}

bool get_ui_tick_pending() {
    ensure_initialized();
    return g_enabled ? g_ui_tick_pending : true;
}

void record_final_mtx(Mtx m, const void* key) {
    if (!s_initialized || !g_recording || m == nullptr) {
        return;
    }

    auto& it = g_current_recording.matrix_values[reinterpret_cast<uintptr_t>(key)];
    MTXCopy(m, it);
}

void record_final_mtx(Mtx m) {
    record_final_mtx(m, m);
}

bool lookup_replacement(const void* key, Mtx out) {
    if (presentation_sync_active() || !g_interpolating || key == nullptr) {
        return false;
    }

    auto it = g_replacements.find(reinterpret_cast<uintptr_t>(key));
    if (it == g_replacements.end()) {
        return false;
    }

    MTXCopy(it->second, out);
    return true;
}

bool lookup_concat_replacement(const void* lhs, const void* rhs, Mtx out) {
    if (presentation_sync_active() || !g_interpolating || lhs == nullptr || rhs == nullptr) {
        return false;
    }

    Mtx lhs_scratch;
    Mtx rhs_scratch;
    const Mtx* resolved_lhs = resolve_replacement(reinterpret_cast<const Mtx*>(lhs), &lhs_scratch);
    const Mtx* resolved_rhs = resolve_replacement(reinterpret_cast<const Mtx*>(rhs), &rhs_scratch);
    if (resolved_lhs == reinterpret_cast<const Mtx*>(lhs) && resolved_rhs == reinterpret_cast<const Mtx*>(rhs)) {
        return false;
    }

    MTXConcat(*resolved_lhs, *resolved_rhs, out);
    return true;
}

void record_camera(::camera_process_class* cam, int camera_id) {
    if (!g_enabled || camera_id != 0 || cam == nullptr) {
        return;
    }
    s_cam_prev = std::move(s_cam_curr);
    copy_view_to_snap(&s_cam_curr, cam->view);
#if WIDESCREEN_SUPPORT
    s_cam_curr.wideZoom = mDoGph_gInf_c::isWideZoom();
#endif
}

void interp_view(::view_class* view) {
    const f32 step = get_interpolation_step();
    cXyz eye;
    cXyz center;
    cXyz up;
    lerp_xyz(&eye, s_cam_prev.eye, s_cam_curr.eye, step);
    lerp_xyz(&center, s_cam_prev.center, s_cam_curr.center, step);
    lerp_xyz(&up, s_cam_prev.up, s_cam_curr.up, step);
    if (!up.normalizeRS()) {
        up = s_cam_curr.up;
        up.normalizeRS();
    }

    view->lookat.eye = eye;
    view->lookat.center = center;
    view->lookat.up = up;
    view->bank = lerp_bank(s_cam_prev.bank, s_cam_curr.bank, step);
    view->fovy = s_cam_prev.fovy + (s_cam_curr.fovy - s_cam_prev.fovy) * step;
    view->aspect = s_cam_prev.aspect + (s_cam_curr.aspect - s_cam_prev.aspect) * step;
    view->near_ = s_cam_prev.near_ + (s_cam_curr.near_ - s_cam_prev.near_) * step;
    view->far_ = s_cam_prev.far_ + (s_cam_curr.far_ - s_cam_prev.far_) * step;

    // FRAME INTERP TODO: It might be better if I rewired the game to not clear this flag until the
    // next sim frame, but I don't care enough to right now
#if WIDESCREEN_SUPPORT
    if (mDoGph_gInf_c::isWide() && !mDoGph_gInf_c::isWideZoom() && step >= 0.5f ?
            s_cam_curr.wideZoom :
            s_cam_prev.wideZoom)
    {
        mDoGph_gInf_c::onWideZoom();
    }
#endif
}

static void run_interpolation_callbacks() {
    for (size_t i = 0; i < s_interpolationCallBackWork.size(); i++) {
        auto const& work = s_interpolationCallBackWork[i];
        work.pCallBack(g_is_sim_frame, work.pUserWork);
    }
}

void add_interpolation_callback(InterpolationCallBack pCallBack, void* pUserWork) {
    if (!is_enabled() || s_presentation_depth > 0 || !g_is_sim_frame)
        return;

    s_interpolationCallBackWork.emplace_back(pCallBack, pUserWork);
}

void begin_presentation_camera() {
    ensure_initialized();
    if (!g_enabled) {
        return;
    }
    if (s_presentation_depth > 0) {
        s_presentation_depth++;
        return;
    }
    if (!s_cam_prev.valid || !s_cam_curr.valid) {
        return;
    }

    view_class* const view = dComIfGd_getView();
    if (view == nullptr) {
        return;
    }

    std::memcpy(&s_presentation_view_backup, view, sizeof(view_class));
    interp_view(view);

    // FRAME INTERP TODO: Largely copied from d_camera's camera_draw function from this point, got any better ideas?
    C_MTXPerspective(view->projMtx, view->fovy, view->aspect, view->near_, view->far_);
    mDoMtx_lookAt(view->viewMtx, &view->lookat.eye, &view->lookat.center, &view->lookat.up, view->bank);
#if WIDESCREEN_SUPPORT
    mDoGph_gInf_c::setWideZoomProjection(view->projMtx);
#endif
    j3dSys.setViewMtx(view->viewMtx);
    cMtx_inverse(view->viewMtx, view->invViewMtx);

    bool camera_attention_status = dComIfGp_getCameraAttentionStatus(0) & 0x80;
    Z2GetAudience()->setAudioCamera(view->viewMtx, view->lookat.eye, view->lookat.center, view->fovy, view->aspect, camera_attention_status, 0, false);

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
    
#if WIDESCREEN_SUPPORT
    mDoGph_gInf_c::offWideZoom();
#endif

    s_presentation_depth = 1;

    run_interpolation_callbacks();
}

void end_presentation_camera() {
    if (s_presentation_depth == 0) {
        return;
    }
    s_presentation_depth--;
    if (s_presentation_depth > 0) {
        return;
    }

    view_class* const view = dComIfGd_getView();
    if (view != nullptr) {
        std::memcpy(view, &s_presentation_view_backup, sizeof(view_class));
    }
}
}  // namespace dusk::frame_interp
