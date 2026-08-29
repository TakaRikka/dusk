#pragma once

namespace randomizer::utility::general
{
    template<typename First, typename... T>
    bool IsAnyOf(First&& first, T&&... t)
    {
        return ((first == t) || ...);
    }

    struct PointerLess {
        bool operator()(const auto* lhs, const auto* rhs) const {
            if (!lhs || !rhs) {
                return lhs < rhs;
            }
            return *lhs < *rhs;
        }
    };
} // namespace randomizer::utility::general
