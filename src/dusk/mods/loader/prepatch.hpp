#pragma once

#include <optional>

namespace dusk::mods::prepatch {

struct Site {
    void** slot = nullptr;
    void* original = nullptr;
};

void initialize();
bool available();
const char* unavailable_reason();
// Returns the target function's prepatched hook site, or nullopt if it is not prepatched.
std::optional<Site> lookup(void* runtimeTarget);
// Publishes a trampoline (or nullptr to deactivate).
void publish(const Site& site, void* trampoline);

}  // namespace dusk::mods::prepatch
