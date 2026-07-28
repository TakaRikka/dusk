#pragma once

namespace randomizer::utility::general
{
    template<typename First, typename... T>
    bool IsAnyOf(First&& first, T&&... t)
    {
        return ((first == t) || ...);
    }
} // namespace randomizer::utility::general
