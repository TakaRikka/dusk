#ifdef DUSK_DISCORD_RPC

#include "dusk/discord_presence.hpp"
#include "dusk/logging.h"
#include "dusk/main.h"
#include "dusk/map_loader_definitions.h"
#include "d/d_com_inf_game.h"
#include "discord_rpc.h"
#include "fmt/format.h"

#include <chrono>
#include <cstring>
#include <string>

namespace dusk {
namespace discord {

static int64_t g_startTime = 0;
static bool g_initialized = false;
static const char* APPLICATION_ID = "1495632471994405035";

static void OnReady(const DiscordUser* user) {
    DuskLog.info("Discord: Connected as {}", user->username);
}

static void OnDisconnected(int errorCode, const char* message) {
    DuskLog.warn("Discord: Disconnected ({}: {})", errorCode, message);
}

static void OnError(int errorCode, const char* message) {
    DuskLog.warn("Discord: Error ({}: {})", errorCode, message);
}

static const char* LookupMapName(const char* mapFile) {
    if (!mapFile || mapFile[0] == '\0') return nullptr;
    for (const auto& region : gameRegions) {
        for (const auto& map : region.maps) {
            if (map.mapFile && strcmp(mapFile, map.mapFile) == 0) {
                return map.mapName;
            }
        }
    }
    return nullptr;
}

void Initialize() {
    g_startTime = static_cast<int64_t>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count()
    );

    DiscordEventHandlers handlers{};
    handlers.ready = OnReady;
    handlers.disconnected = OnDisconnected;
    handlers.errored = OnError;
    Discord_Initialize(APPLICATION_ID, &handlers, 0, nullptr);
    g_initialized = true;

    DuskLog.info("Discord Rich Presence initialized");
}

void RunCallbacks() {
    if (!g_initialized) return;
    Discord_RunCallbacks();
}

void UpdatePresence() {
    if (!g_initialized) return;

    static auto lastUpdate = std::chrono::steady_clock::time_point{};
    const auto now = std::chrono::steady_clock::now();
    if (now - lastUpdate < std::chrono::seconds(15)) return;
    lastUpdate = now;

    static std::string detailsBuf;
    static std::string stateBuf;

    DiscordRichPresence presence{};
    presence.startTimestamp = g_startTime;
    presence.largeImageKey = "icon";
    presence.largeImageText = "Dusk";

    if (dusk::IsGameLaunched) {
        const char* stageName = dComIfGp_getLastPlayStageName();

        // stageName is empty until a room is actually entered
        if (stageName[0] != '\0') {
            const char* locationName = LookupMapName(stageName);

            if (locationName) {
                detailsBuf = locationName;
            }
            else {
                detailsBuf = "Twilight Princess";
            }

            presence.details = detailsBuf.c_str();

            stateBuf = fmt::format(FMT_STRING("{}/{} \u2665  |  {} Rupees"),
                dComIfGs_getLife() / 4, dComIfGs_getMaxLife() / 5, dComIfGs_getRupee());

            presence.state = stateBuf.c_str();
        }
    }

    Discord_UpdatePresence(&presence);
    DuskLog.debug("Discord Rich Presence sent");
}

void Shutdown() {
    if (!g_initialized) return;
    Discord_ClearPresence();
    Discord_Shutdown();
    g_initialized = false;
    DuskLog.info("Discord Rich Presence shut down");
}

} // namespace discord
} // namespace dusk

#endif // DUSK_DISCORD_RPC
