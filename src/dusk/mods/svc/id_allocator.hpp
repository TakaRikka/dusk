#pragma once

#include <vector>
#include <limits>

namespace dusk::mods::svc {

[[noreturn]] void id_allocator_exhausted();

template<typename T> requires std::is_integral_v<T>
class PlainIdAllocator {
    std::vector<T> reusable;
    T alloc_next;
    T alloc_max;

public:
    explicit PlainIdAllocator(T first, T max=std::numeric_limits<T>::max())
        : alloc_next(first), alloc_max(max) {

    }

    T alloc() {
        if (reusable.empty()) {
            if (alloc_next == alloc_max) {
                id_allocator_exhausted();
            }

            return alloc_next++;
        }

        auto val = reusable.back();
        reusable.pop_back();
        return val;
    }

    void free(T value) {
        assert(value < alloc_max && value < alloc_next);

        reusable.push_back(value);
    }
};

}