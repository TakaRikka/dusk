#pragma once

#include <cstdint>
#include <type_traits>

template<typename T> requires std::is_arithmetic_v<T>
T roundUp(const T& val, const T& multiple) {
    if(val % multiple == 0) return val;
    return val + multiple - (val % multiple);
}
