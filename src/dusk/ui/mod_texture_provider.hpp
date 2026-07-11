#pragma once

#include <string>
#include <string_view>

namespace dusk::mods {
struct LoadedMod;
}  // namespace dusk::mods

namespace dusk::ui {

// Serves PNG images out of mod bundles to RmlUi via the mod:// texture provider scheme.
// Sources embed the mod's cacheGeneration so reloads bust RmlUi's texture cache.
std::string mod_image_source(const mods::LoadedMod& mod, std::string_view bundlePath);

void register_mod_texture_provider() noexcept;
void unregister_mod_texture_provider() noexcept;

}  // namespace dusk::ui
