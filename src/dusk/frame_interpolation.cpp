#include "f_op/f_op_camera_mng.h"
#include "dusk/frame_interpolation.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

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
};

struct Recording {
    Path root;
};

struct MatrixValue {
    Mtx value;
};

using FinalMtxLookup = std::unordered_map<const Mtx*, const Data*>;

bool s_initialized = false;

bool g_enabled = false;
bool g_recording = false;
bool g_interpolating = false;
bool g_sync_presentation = false;
uint32_t g_presentation_counter = 0;
uint32_t g_presentation_sync_end = 0;

float g_step = 0.0f;
uint32_t g_pending_presentation_ui_ticks = 0;
uint32_t g_current_presentation_ui_ticks = 0;

Recording g_current_recording;
Recording g_previous_recording;
std::vector<Path*> g_current_path;

std::unordered_map<const Mtx*, MatrixValue> g_replacements;

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

void build_final_mtx_lookup(const Path& path, FinalMtxLookup& lookup) {
    lookup.clear();

    const OpBucket* bucket = find_op_bucket(path, Op::FinalMtx);
    if (bucket == nullptr) {
        return;
    }

    for (const Data& data : bucket->values) {
        if (data.dest == nullptr) {
            continue;
        }
        lookup[data.dest] = &data;
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
    build_final_mtx_lookup(old_path, old_final_mtx_lookup);

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
        const Data* old_data = op == Op::FinalMtx ? find_matching_final_mtx(old_final_mtx_lookup, *new_data) : indexed_old_data;
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
        return;
    }

    if (g_sync_presentation && g_presentation_counter > g_presentation_sync_end) {
        g_sync_presentation = false;
    }

    g_previous_recording = std::move(g_current_recording);
    g_current_recording = {};
    g_current_path.clear();
    g_current_path.push_back(&g_current_recording.root);
    g_recording = true;
    g_interpolating = false;
    clear_replacements();
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

void notify_presentation_frame() {
    ensure_initialized();
    ++g_presentation_counter;
}

void request_presentation_sync() {
    ensure_initialized();
    if (!g_enabled) {
        return;
    }
    g_sync_presentation = true;
    g_presentation_sync_end = g_presentation_counter + 1;
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

void notify_sim_tick_complete() {
    ensure_initialized();
    g_pending_presentation_ui_ticks++;
}

uint32_t begin_presentation_ui_pass() {
    ensure_initialized();
    g_current_presentation_ui_ticks = g_pending_presentation_ui_ticks;
    g_pending_presentation_ui_ticks = 0;
    return g_current_presentation_ui_ticks;
}

uint32_t get_presentation_ui_advance_ticks() {
    if (!s_initialized) {
        return 0;
    }
    if (!g_enabled) {
        return 1;
    }
    return g_current_presentation_ui_ticks;
}

void end_presentation_ui_pass() {
    if (!s_initialized) {
        return;
    }
    g_current_presentation_ui_ticks = 0;
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
    g_current_path.push_back(siblings.back().get());
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

// TODO: Is there already a built-in function for this?
void camera_eye_from_view_mtx(MtxP view_mtx, cXyz* o_eye) {
    o_eye->x = -(view_mtx[0][0] * view_mtx[0][3] + view_mtx[1][0] * view_mtx[1][3] + view_mtx[2][0] * view_mtx[2][3]);
    o_eye->y = -(view_mtx[0][1] * view_mtx[0][3] + view_mtx[1][1] * view_mtx[1][3] + view_mtx[2][1] * view_mtx[2][3]);
    o_eye->z = -(view_mtx[0][2] * view_mtx[0][3] + view_mtx[1][2] * view_mtx[1][3] + view_mtx[2][2] * view_mtx[2][3]);
}

namespace {
struct CamSnap {
    cXyz eye{};
    cXyz center{};
    cXyz up{};
    s16 bank{};
    f32 fovy{};
    bool valid{};
};

CamSnap s_star_prev{};
CamSnap s_star_curr{};

static void copy_view_to_snap(CamSnap* dst, const view_class& v) {
    dst->eye = v.lookat.eye;
    dst->center = v.lookat.center;
    dst->up = v.lookat.up;
    dst->bank = v.bank;
    dst->fovy = v.fovy;
    dst->valid = true;
}

static void billboard_base_from_view(MtxP view_mtx, MtxP o_cam_billboard_base) {
    Mtx rot;
    MTXCopy(view_mtx, rot);
    rot[0][3] = rot[1][3] = rot[2][3] = 0.0f;
    MTXInverse(rot, o_cam_billboard_base);
}
}  // namespace

void begin_record_camera() {
    ::camera_process_class* cam = dComIfGp_getCamera(0);
    if (cam == nullptr) {
        return;
    }
    copy_view_to_snap(&s_star_prev, cam->view);
}

void record_camera(::camera_process_class* cam, int camera_id) {
    if (!getSettings().game.enableFrameInterpolation || camera_id != 0 || cam == nullptr) {
        return;
    }
    copy_view_to_snap(&s_star_curr, cam->view);
}

bool build_star_view(Mtx o_view, Mtx o_cam_billboard_base, cXyz* o_anchor_eye, float* o_fovy) {
    if (!getSettings().game.enableFrameInterpolation || !s_star_prev.valid || !s_star_curr.valid) {
        return false;
    }

    const f32 step = get_interpolation_step();
    cXyz eye;
    cXyz center;
    cXyz up;
    lerp_xyz(&eye, s_star_prev.eye, s_star_curr.eye, step);
    lerp_xyz(&center, s_star_prev.center, s_star_curr.center, step);
    lerp_xyz(&up, s_star_prev.up, s_star_curr.up, step);
    if (!up.normalizeRS()) {
        up = s_star_curr.up;
        up.normalizeRS();
    }

    const f32 bank_rad = S2RAD(s_star_prev.bank) * (1.0f - step) + S2RAD(s_star_curr.bank) * step;
    const s16 bank = cAngle::Radian_to_SAngle(bank_rad);

    mDoMtx_lookAt(o_view, &eye, &center, &up, bank);
    billboard_base_from_view(o_view, o_cam_billboard_base);

    *o_anchor_eye = eye;
    *o_fovy = s_star_prev.fovy + (s_star_curr.fovy - s_star_prev.fovy) * step;
    return true;
}
}  // namespace dusk::frame_interp
