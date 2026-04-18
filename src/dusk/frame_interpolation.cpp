#include "dusk/frame_interpolation.h"

#include <memory>
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_graphic.h"

namespace {
enum class Op : uint8_t {
    OpenChild,
    FinalMtx,
};

struct Label {
    const void* key = nullptr;
    int32_t id = 0;

    bool operator==(const Label& other) const {
        return key == other.key && id == other.id;
    }
};

struct Data {
    Label child_label{};
    size_t child_index = 0;
    Mtx matrix{};
    const Mtx* dest = nullptr;
    uint64_t stable_tag = 0;
};

struct Path;

struct ChildBucket {
    Label label{};
    std::vector<std::unique_ptr<Path>> nodes;
};

struct OpBucket {
    Op op = Op::OpenChild;
    std::vector<Data> values;
};

struct Path {
    std::vector<ChildBucket> children;
    std::vector<OpBucket> ops;
    std::vector<std::pair<Op, size_t>> items;
    Label draw_scope{};
    uint32_t simple_shadow_pair_seq = 0;
};

struct Recording {
    Path root;
};

struct MatrixValue {
    Mtx value;
};

using FinalMtxLookup = std::unordered_map<const Mtx*, const Data*>;
using FinalMtxLookupTagged = std::unordered_map<uint64_t, const Data*>;

bool s_initialized = false;

bool g_enabled = false;
bool g_recording = false;
bool g_interpolating = false;
bool g_sync_presentation = false;

float g_step = 0.0f;
bool g_ui_tick_pending = false;

Recording g_current_recording;
Recording g_previous_recording;
std::vector<Path*> g_current_path;

std::unordered_map<const Mtx*, MatrixValue> g_replacements;

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

inline void copy_matrix(const Mtx src, Mtx dst) {
    MTXCopy(src, dst);
}

inline void concat_matrix(const Mtx lhs, const Mtx rhs, Mtx out) {
    MTXConcat(lhs, rhs, out);
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

Data& append_op(Op op) {
    auto& items = g_current_path.back()->items;
    auto& buckets = g_current_path.back()->ops;
    auto it = std::find_if(buckets.begin(), buckets.end(),
                           [op](const OpBucket& bucket) { return bucket.op == op; });
    if (it == buckets.end()) {
        buckets.push_back({op, {}});
        it = buckets.end() - 1;
    }
    items.emplace_back(op, it->values.size());
    return it->values.emplace_back();
}

const Data* find_matching_data(const Path& path, Op op, size_t index) {
    auto it = std::find_if(path.ops.begin(), path.ops.end(),
                           [op](const OpBucket& bucket) { return bucket.op == op; });
    if (it == path.ops.end() || index >= it->values.size()) {
        return nullptr;
    }
    return &it->values[index];
}

const OpBucket* find_op_bucket(const Path& path, Op op) {
    auto it = std::find_if(path.ops.begin(), path.ops.end(),
                           [op](const OpBucket& bucket) { return bucket.op == op; });
    if (it == path.ops.end()) {
        return nullptr;
    }
    return &*it;
}

void build_final_mtx_lookups(const Path& path, FinalMtxLookup& dest_lookup, FinalMtxLookupTagged& tag_lookup) {
    dest_lookup.clear();
    tag_lookup.clear();

    const OpBucket* bucket = find_op_bucket(path, Op::FinalMtx);
    if (bucket == nullptr) {
        return;
    }

    for (const Data& data : bucket->values) {
        if (data.dest != nullptr) {
            dest_lookup[data.dest] = &data;
        }
        if (data.stable_tag != 0) {
            tag_lookup[data.stable_tag] = &data;
        }
    }
}

const Data* find_matching_final_mtx(const FinalMtxLookup& lookup, const Data& new_data) {
    if (new_data.dest == nullptr) {
        return nullptr;
    }

    auto it = lookup.find(new_data.dest);
    if (it == lookup.end()) {
        return nullptr;
    }
    return it->second;
}

ChildBucket& get_child_bucket(Path& path, const Label& label) {
    auto it = std::find_if(path.children.begin(), path.children.end(),
                           [&label](const ChildBucket& bucket) { return bucket.label == label; });
    if (it == path.children.end()) {
        path.children.push_back({});
        it = path.children.end() - 1;
        it->label = label;
    }
    return *it;
}

const ChildBucket* find_child_bucket(const Path& path, const Label& label) {
    auto it = std::find_if(path.children.begin(), path.children.end(),
                           [&label](const ChildBucket& bucket) { return bucket.label == label; });
    if (it == path.children.end()) {
        return nullptr;
    }
    return &*it;
}

void store_replacement(const Data& old_data, const Data& new_data, float step) {
    if (new_data.dest == nullptr) {
        return;
    }

    auto& replacement = g_replacements[new_data.dest];
    lerp_matrix(replacement.value, old_data.matrix, new_data.matrix, step);
}

void interpolate_branch(const Path& old_path, const Path& new_path, float step) {
    FinalMtxLookup old_final_mtx_lookup;
    FinalMtxLookupTagged old_final_mtx_lookup_tagged;
    build_final_mtx_lookups(old_path, old_final_mtx_lookup, old_final_mtx_lookup_tagged);

    for (const auto& item : new_path.items) {
        const Op op = item.first;
        const size_t index = item.second;
        const Data* new_data = find_matching_data(new_path, op, index);
        if (new_data == nullptr) {
            continue;
        }

        if (op == Op::OpenChild) {
            const ChildBucket* new_children = find_child_bucket(new_path, new_data->child_label);
            if (new_children == nullptr || new_data->child_index >= new_children->nodes.size())
            {
                continue;
            }

            const Path& new_child = *new_children->nodes[new_data->child_index];
            const ChildBucket* old_children = find_child_bucket(old_path, new_data->child_label);
            if (old_children != nullptr && new_data->child_index < old_children->nodes.size())
            {
                interpolate_branch(*old_children->nodes[new_data->child_index], new_child, step);
            } else {
                interpolate_branch(new_child, new_child, step);
            }
            continue;
        }

        const Data* indexed_old_data = find_matching_data(old_path, op, index);
        const Data* old_data = nullptr;
        if (op == Op::FinalMtx) {
            if (new_data->stable_tag != 0) {
                const auto it = old_final_mtx_lookup_tagged.find(new_data->stable_tag);
                old_data = it != old_final_mtx_lookup_tagged.end() ? it->second : nullptr;
            } else {
                old_data = find_matching_final_mtx(old_final_mtx_lookup, *new_data);
            }
        } else {
            old_data = indexed_old_data;
        }
        if (op == Op::FinalMtx) {
            store_replacement(old_data != nullptr ? *old_data : *new_data, *new_data, step);
        }
    }
}

const Mtx* resolve_replacement(const Mtx* source, Mtx* scratch) {
    if (!g_interpolating || source == nullptr || dusk::frame_interp::presentation_sync_active()) {
        return source;
    }

    auto it = g_replacements.find(source);
    if (it == g_replacements.end()) {
        return source;
    }

    copy_matrix(it->second.value, *scratch);
    return scratch;
}

bool has_recording_data(const Recording& recording) {
    return !recording.root.items.empty() || !recording.root.children.empty();
}

void clear_replacements() {
    g_replacements.clear();
}

}  // namespace

namespace dusk::frame_interp {
void ensure_initialized() {
    g_enabled = getSettings().game.enableFrameInterpolation;
    s_initialized = true;
}

void begin_record() {
    ensure_initialized();

    if (!g_enabled) {
        g_interpolating = false;
        g_sync_presentation = false;
        g_previous_recording = {};
        g_current_recording = {};
        g_current_path.clear();
        clear_replacements();
        s_cam_prev.valid = false;
        s_cam_curr.valid = false;
        return;
    }

    g_sync_presentation = false;
    g_previous_recording = std::move(g_current_recording);
    g_current_recording = {};
    g_current_path.clear();
    g_current_path.push_back(&g_current_recording.root);
    g_recording = true;
    g_interpolating = false;
    clear_replacements();

    ::camera_process_class* cam = dComIfGp_getCamera(0);
    if (cam == nullptr) {
        s_cam_prev.valid = false;
        s_cam_curr.valid = false;
        return;
    } else {
        copy_view_to_snap(&s_cam_prev, cam->view);
#if WIDESCREEN_SUPPORT
        s_cam_prev.wideZoom = s_cam_curr.valid ? s_cam_curr.wideZoom : false;
#endif
    }
}

void end_record() {
    g_recording = false;
}

void interpolate(float step) {
    ensure_initialized();
    clear_replacements();
    g_step = std::clamp(step, 0.0f, 1.0f);
    g_interpolating = g_enabled && !g_recording && !g_sync_presentation && has_recording_data(g_current_recording);
    if (!g_interpolating) {
        return;
    }
    const Path& old_root = has_recording_data(g_previous_recording) ? g_previous_recording.root : g_current_recording.root;
    interpolate_branch(old_root, g_current_recording.root, g_step);
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

void open_child(const void* key, int32_t id) {
    if (!s_initialized || !g_recording) {
        return;
    }

    Label label{key, id};
    auto& siblings = get_child_bucket(*g_current_path.back(), label).nodes;
    Data& data = append_op(Op::OpenChild);
    data.child_label = label;
    data.child_index = siblings.size();
    siblings.emplace_back(std::make_unique<Path>());
    Path* const child = siblings.back().get();
    child->draw_scope = label;
    g_current_path.push_back(child);
}

void close_child() {
    if (!s_initialized || !g_recording || g_current_path.size() <= 1) {
        return;
    }

    g_current_path.pop_back();
}

void record_final_mtx_raw(const Mtx* dest, const Mtx src) {
    if (!s_initialized || !g_recording || dest == nullptr) {
        return;
    }

    Data& data = append_op(Op::FinalMtx);
    data.dest = dest;
    data.stable_tag = 0;
    copy_matrix(src, data.matrix);
}

void record_final_mtx_raw_tagged(const Mtx* dest, const Mtx src, uint64_t stable_tag) {
    if (!s_initialized || !g_recording || dest == nullptr) {
        return;
    }

    Data& data = append_op(Op::FinalMtx);
    data.dest = dest;
    data.stable_tag = stable_tag;
    copy_matrix(src, data.matrix);
}

bool lookup_replacement(const void* source, Mtx out) {
    if (presentation_sync_active() || !g_interpolating || source == nullptr) {
        return false;
    }

    auto it = g_replacements.find(reinterpret_cast<const Mtx*>(source));
    if (it == g_replacements.end()) {
        return false;
    }

    copy_matrix(it->second.value, out);
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

    concat_matrix(*resolved_lhs, *resolved_rhs, out);
    return true;
}

void record_camera(::camera_process_class* cam, int camera_id) {
    if (!g_enabled || camera_id != 0 || cam == nullptr) {
        return;
    }
    copy_view_to_snap(&s_cam_curr, cam->view);
#if WIDESCREEN_SUPPORT
    s_cam_curr.wideZoom = mDoGph_gInf_c::isWideZoom();
#endif
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

    // FRAME INTERP TODO: It might be better if I rewired the game to not clear this flag until the next sim frame, but I don't care enough to right now
#if WIDESCREEN_SUPPORT
    if (mDoGph_gInf_c::isWide() && !mDoGph_gInf_c::isWideZoom() && step >= 0.5f ? s_cam_curr.wideZoom : s_cam_prev.wideZoom) {
        mDoGph_gInf_c::onWideZoom();
    }
#endif

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

uint64_t alloc_simple_shadow_pair_base() {
    if (!s_initialized || !g_recording || g_current_path.size() <= 1) {
        return 0;
    }

    Path* const scope = g_current_path.back();
    const uint64_t h = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(scope->draw_scope.key));
    const uint32_t lo = scope->simple_shadow_pair_seq;
    scope->simple_shadow_pair_seq += 2;
    uint64_t tag0 = (h << 17) ^ (static_cast<uint64_t>(lo) << 1u);
    if (tag0 == 0) {
        tag0 = 2;
    }
    return tag0;
}
}  // namespace dusk::frame_interp
