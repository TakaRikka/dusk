#pragma once

namespace dusk::hq_minimap {

/// Adds a mapping of an image resource index (into `Always.arc`) to the pointer to its image data.
/// If `initialize_if_needed` has been called, this is a no-op. Pointers are expected to be stable
/// and valid for the program's entire lifetime.
void register_pointer(int idx, u8* ptr);

/// Sets whether HQ minimap texture replacements should be active or not. Does not manage 
/// replacement registrations itself; see `update`.
void set_active(bool active);

/// Registers or unregisters texture replacements depending on active state.
void update();

/// Called once after registering image pointers, in which their HQ replacements are procedurally
/// drawn and `update` is called. Further calls are no-ops.
void initialize_if_needed();

}
