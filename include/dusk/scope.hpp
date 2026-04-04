#ifndef DUSK_SCOPE_HPP
#define DUSK_SCOPE_HPP

namespace dusk {

/**
 * A simple value wrapper that will destroy the value at the end of its scope.
 * @tparam T The type of value contained.
 * @tparam Destructor The type of function used to destroy the value.
 */
template <typename T, typename Destructor>
    struct ScopeValue {
        T value;
        Destructor destructor;

        explicit ScopeValue(T value, Destructor destructor) : value(value), destructor(destructor) {
        }

        ~ScopeValue() {
            destructor(value);
        }

        constexpr operator T&() const noexcept {
            return value;
        }
    };
}

#endif  // DUSK_SCOPE_HPP
