#pragma once

#include "dusk/interp/frame_interpolation.h"
#include "dusk/interp/lerp.h"

#include <cstring>

#ifdef __cplusplus
namespace dusk::interp {

template <typename T, int capacity>
class DualBuffer {
public:
    explicit DualBuffer(T* dst = NULL)
        : m_prev_valid(false),
          m_curr_valid(false),
          m_count(0),
          m_dst(dst),
          m_post(NULL),
          m_post_user(NULL) {}

    void bind(T* dst) { m_dst = dst; }

    void reset() {
        m_prev_valid = false;
        m_curr_valid = false;
        m_count = 0;
    }

    bool ready() const { return m_prev_valid && m_curr_valid; }

    void capture_and_schedule(const T* src, int count, void (*post)(void*) = NULL,
                              void* post_user = NULL) {
        roll();
        capture(src, count);
        schedule(post, post_user);
    }

    void writeback(T* src_and_dst, int count, void (*post)(void*) = NULL, void* post_user = NULL) {
        bind(src_and_dst);
        capture_and_schedule(src_and_dst, count, post, post_user);
    }

private:
    bool fits(int count) const {
        if (count > capacity) {
            return false;
        }
        return count > 0;
    }

    void roll() {
        if (!is_enabled() || !m_curr_valid || m_count <= 0) {
            return;
        }
        std::memcpy(m_prev, m_curr, static_cast<size_t>(m_count) * sizeof(T));
        m_prev_valid = true;
    }

    void capture(const T* src, int count) {
        if (!fits(count) || !is_enabled() || src == NULL) {
            return;
        }
        std::memcpy(m_curr, src, static_cast<size_t>(count) * sizeof(T));
        m_count = count;
        m_curr_valid = true;
    }

    void apply(T* dst, int count) const {
        if (!fits(count) || dst == NULL || !ready()) {
            return;
        }
        const f32 step = get_interpolation_step();
        for (int i = 0; i < count; ++i) {
            lerp(dst[i], m_prev[i], m_curr[i], step);
        }
    }

    void schedule(void (*post)(void*) = NULL, void* post_user = NULL) {
        if (!is_enabled() || m_dst == NULL || !fits(m_count)) {
            return;
        }
        m_post = post;
        m_post_user = post_user;
        add_interpolation_callback(&present_trampoline, this);
    }

    static void present_trampoline(void* user) { static_cast<DualBuffer*>(user)->present(); }

    void present() {
        apply(m_dst, m_count);
        if (m_post != NULL) {
            m_post(m_post_user);
        }
    }

    T m_prev[capacity];
    T m_curr[capacity];
    bool m_prev_valid;
    bool m_curr_valid;
    int m_count;
    T* m_dst;
    void (*m_post)(void*);
    void* m_post_user;
};

namespace detail {
void* acquire(const void* key, const void* type, void* (*make)(), void (*destroy)(void*));
}

template <typename Record>
Record& get(const void* key) {
    static const char token{};
    return *static_cast<Record*>(detail::acquire(
        key, &token, []() -> void* { return new Record; },
        [](void* p) { delete static_cast<Record*>(p); }));
}

void erase_owned_buffers(const void* key);
void clear_owned_buffers();

}  // namespace dusk::interp
#endif
