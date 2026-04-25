#pragma once

#ifdef DUSK_DISCORD_RPC

namespace dusk {
namespace discord {

void Initialize();

void RunCallbacks();

void UpdatePresence();

void Shutdown();
}
}

#endif // DUSK_DISCORD_RPC
