#include "dusk/interp/line.h"

#include "dusk/interp/dual_buffer.h"
#include "dusk/interp/frame_interpolation.h"
#include "dusk/interp/lerp.h"

#include "m_Do/m_Do_ext.h"

#include <cstdint>
#include <cstring>
#include <limits>
#include <new>

namespace {

struct Record {
    Record()
        : store(nullptr), previous(nullptr), current(nullptr), sample_capacity(0), tick(0),
          strand_count(0), point_count(0), previous_valid(false), current_valid(false) {}

    ~Record() { delete[] store; }

    Record(const Record&) = delete;
    Record& operator=(const Record&) = delete;

    void invalidate() {
        previous_valid = false;
        current_valid = false;
    }

    bool ensure(size_t required) {
        if (required <= sample_capacity) {
            return store != nullptr;
        }
        if (required > std::numeric_limits<size_t>::max() / (2 * sizeof(cXyz))) {
            return false;
        }

        cXyz* next = new (std::nothrow) cXyz[required * 2];
        if (next == nullptr) {
            return false;
        }

        delete[] store;
        store = next;
        previous = store;
        current = store + required;
        sample_capacity = required;
        invalidate();
        return true;
    }

    cXyz* store;
    cXyz* previous;
    cXyz* current;
    size_t sample_capacity;
    uint64_t tick;
    u16 strand_count;
    u16 point_count;
    bool previous_valid;
    bool current_valid;
};

bool valid(const dusk::interp::line::Points& points) {
    if (points.strands == nullptr || points.strand_count == 0 || points.point_count == 0) {
        return false;
    }
    for (u16 strand = 0; strand < points.strand_count; ++strand) {
        if (points.strands[strand].field_0x0 == nullptr) {
            return false;
        }
    }
    return true;
}

bool same_layout(const Record& record, const dusk::interp::line::Points& points) {
    return record.current_valid && record.strand_count == points.strand_count &&
           record.point_count == points.point_count;
}

void copy_points(cXyz* destination, const dusk::interp::line::Points& points) {
    const size_t points_size = static_cast<size_t>(points.point_count) * sizeof(cXyz);
    for (u16 strand = 0; strand < points.strand_count; ++strand) {
        std::memcpy(destination + static_cast<size_t>(strand) * points.point_count,
            points.strands[strand].field_0x0, points_size);
    }
}

}  // namespace

namespace dusk::interp::line {

void reset(const void* owner) {
    erase_owned_buffers(owner);
}

void capture(const void* owner, Points points) {
    if (owner == nullptr || !should_capture()) {
        return;
    }

    Record& record = get<Record>(owner);
    if (!valid(points)) {
        record.invalidate();
        return;
    }

    const size_t required = static_cast<size_t>(points.strand_count) * points.point_count;
    if (!record.ensure(required)) {
        record.invalidate();
        return;
    }

    const uint64_t tick = sim_tick_seq();
    const bool layout_matches = same_layout(record, points);

    if (layout_matches && tick == record.tick) {
        copy_points(record.current, points);
        return;
    }

    if (layout_matches && tick == record.tick + 1) {
        std::swap(record.previous, record.current);
        record.previous_valid = true;
    } else {
        record.previous_valid = false;
    }

    copy_points(record.current, points);
    record.tick = tick;
    record.strand_count = points.strand_count;
    record.point_count = points.point_count;
    record.current_valid = true;
}

void write(const void* owner, Points points) {
    if (owner == nullptr || !is_enabled() || !valid(points)) {
        return;
    }

    Record& record = get<Record>(owner);
    if (!record.previous_valid || !same_layout(record, points) ||
        record.tick != sim_tick_seq())
    {
        return;
    }

    const f32 step = get_interpolation_step();
    for (u16 strand = 0; strand < points.strand_count; ++strand) {
        cXyz* destination = points.strands[strand].field_0x0;
        const size_t offset = static_cast<size_t>(strand) * points.point_count;
        for (u16 point = 0; point < points.point_count; ++point) {
            lerp(destination[point], record.previous[offset + point],
                record.current[offset + point], step);
        }
    }
}

}  // namespace dusk::interp::line
