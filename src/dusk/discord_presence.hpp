#pragma once

#ifdef DUSK_DISCORD

namespace dusk::discord {

void initialize();
void run_callbacks();
void update_presence();
void shutdown();

}  // namespace dusk::discord

#endif  // DUSK_DISCORD
