#pragma once

#include <cstdint>
#include <vector>

#include "mods/svc/hook.h"

namespace dusk::mods::manifest {

// Symbol flags mirrored from symgen.
constexpr uint32_t kFlagCode = 1u << 0;
constexpr uint32_t kFlagData = 1u << 1;
constexpr uint32_t kFlagLocal = 1u << 2;
constexpr uint32_t kFlagMultiName = 1u << 3;
constexpr uint32_t kFlagDupName = 1u << 4;
constexpr uint32_t kFlagInlineSites = 1u << 5;
constexpr uint32_t kFlagDisplay = 1u << 6;

enum class ResolveStatus {
    Ok,
    Unavailable,  // no manifest loaded (missing, stale, or malformed)
    NotFound,
    Ambiguous,  // name maps to multiple addresses (overloads / per-TU statics)
};

// Maps the symbol manifest next to the game binary and validates it against the
// running image's build id (PDB GUID+age / Mach-O UUID / GNU build-id). A missing or
// stale manifest logs and leaves by-name resolution unavailable; hooks by address are
// unaffected. Safe to call more than once.
void initialize();

bool available();

// Build id of the running executable image (PDB GUID+age / Mach-O UUID / GNU
// build-id), computed once on first use; empty if it couldn't be determined.
// Independent of whether a manifest file was loaded.
const std::vector<uint8_t>& image_build_id();

// Resolve a symbol name to its address in the running image. Names can be either the platform's
// mangled name (i.e. the name passed to dlopen; no Mach-O leading underscore) or the function name
// without parameters (e.g. "daAlink_c::execute").
ResolveStatus resolve(const char* name, void** outAddr, HookSymbolFlags* outFlags = nullptr);

// True if the manifest records that the function at this code address was inlined into
// at least one caller in this build. An entry hook on it only intercepts the calls
// that were not inlined. outName receives the symbol name (valid for the process lifetime)
// when known. False when no manifest is loaded.
bool has_inline_sites(const void* addr, const char** outName = nullptr);

}  // namespace dusk::mods::manifest
