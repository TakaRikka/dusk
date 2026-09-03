#pragma once

#include <cstdint>

namespace dusk::texture_replacements {

// Mod replacements are prioritized *over* user replacements (<data folder>/texture_replacements/)
inline constexpr int32_t kUserTextureReplacementPriority = -1'000'000;

void reload();
void shutdown();

}
