#pragma once

#include <memory>
#include <vector>

#include "dusk/mod_loader.hpp"

namespace dusk::mods::loader {

// Reorders mods so service providers precede their importers, populating
// LoadedMod::dependencies/dependents from manifest imports along the way.
// Mods whose required imports form a cycle are failed; a cycle that can be
// broken by dropping an optional import is broken with a warning. Scan order
// is preserved between mods with no dependency relationship.
void sort_mods(std::vector<std::unique_ptr<LoadedMod>>& mods);

}  // namespace dusk::mods::loader
