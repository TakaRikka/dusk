#pragma once

#if BOREALIS_HAS_DISCORD

namespace dusk::discord {

void initialize();
void run_callbacks();
void update_presence();
void shutdown();

}  // namespace dusk::discord

#endif  // BOREALIS_HAS_DISCORD
