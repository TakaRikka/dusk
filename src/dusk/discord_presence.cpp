#ifdef DUSK_DISCORD

#include "dusk/discord_presence.hpp"
#include "d/d_com_inf_game.h"
#include "discord.hpp"
#include "dusk/logging.h"
#include "dusk/main.h"
#include "dusk/map_loader_definitions.h"
#include "fmt/format.h"

#include <chrono>
#include <string>
#include <string_view>
#include <utility>

namespace dusk::discord {

static int64_t g_startTime = 0;
static bool g_initialized = false;
static constexpr const char* kApplicationId = "1495632471994405035";

static void on_ready(const rpc::User& user) {
    DuskLog.info("Discord: Connected as {}", user.username);
}

static void on_disconnected(int errorCode, std::string_view message) {
    DuskLog.warn("Discord: Disconnected ({}: {})", errorCode, message);
}

static void on_error(int errorCode, std::string_view message) {
    DuskLog.warn("Discord: Error ({}: {})", errorCode, message);
}

static const char* lookup_map_name(const char* mapFile) {
    if (!mapFile || mapFile[0] == '\0')
        return nullptr;
    for (const auto& region : gameRegions) {
        for (const auto& map : region.maps) {
            if (map.mapFile && strcmp(mapFile, map.mapFile) == 0) {
                return map.mapName;
            }
        }
    }
    return nullptr;
}

void initialize() {
    g_startTime = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch())
                      .count();

    rpc::EventHandlers handlers{};
    handlers.ready = on_ready;
    handlers.disconnected = on_disconnected;
    handlers.error = on_error;
    rpc::initialize(kApplicationId, std::move(handlers));
    g_initialized = true;

    DuskLog.info("Discord Rich Presence initialized");
}

void run_callbacks() {
    if (!g_initialized)
        return;
    rpc::run_callbacks();
}

void update_presence() {
    if (!g_initialized)
        return;

    static auto sLastUpdate = std::chrono::steady_clock::time_point{};
    const auto now = std::chrono::steady_clock::now();
    if (now - sLastUpdate < std::chrono::seconds(15))
        return;
    sLastUpdate = now;

    static std::string sDetailsBuf;
    static std::string sStateBuf;

    rpc::Presence presence{};
    presence.startTimestamp = g_startTime;
    presence.largeImageKey = "icon";
    presence.largeImageText = "Dusklight";

    if (IsGameLaunched) {
        const char* stageName = dComIfGp_getLastPlayStageName();

        // stageName is empty until a room is actually entered
        if (stageName[0] != '\0') {
            const char* locationName = lookup_map_name(stageName);

            if (locationName) {
                sDetailsBuf = locationName;
            } else {
                sDetailsBuf = "Twilight Princess";
            }

            presence.details = sDetailsBuf;

            sStateBuf = fmt::format(FMT_STRING("{}/{} \u2665  |  {} Rupees"),
                dComIfGs_getLife() / 4, dComIfGs_getMaxLife() / 5, dComIfGs_getRupee());

            presence.state = sStateBuf;
        }
    }

    rpc::update_presence(std::move(presence));
    DuskLog.debug("Discord Rich Presence sent");
}

void shutdown() {
    if (!g_initialized)
        return;
    rpc::clear_presence();
    rpc::shutdown();
    g_initialized = false;
    DuskLog.info("Discord Rich Presence shut down");
}

}  // namespace dusk::discord

#endif  // DUSK_DISCORD
