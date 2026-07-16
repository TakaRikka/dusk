#pragma once

#include <functional>

class SimpleScopeGuard {
public:
    // Store the function in the constructor
    explicit SimpleScopeGuard(const std::function<void()>& func) : m_func(func) {}

    // Run the function when the object goes out of scope
    ~SimpleScopeGuard() {
        if (m_func) m_func();
    }

private:
    std::function<void()> m_func;
};
