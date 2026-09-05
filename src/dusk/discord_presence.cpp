#if BOREALIS_HAS_DISCORD
#include "discord_presence.hpp"

#include "app_info.hpp"
#include "logging.h"
#include "main.h"
#include "map_loader_definitions.h"

#include "d/d_com_inf_game.h"

#include <borealis/discord.hpp>
#include <fmt/format.h>

#include <chrono>
#include <string>
#include <string_view>
#include <utility>

namespace dusk::discord {
namespace {
constexpr borealis::Log Log{"dusk::discord"};
int64_t g_startTime = 0;
bool g_initialized = false;
}  // namespace

static void on_ready(const borealis::discord::User& user) {
    Log.info("Connected as {}", user.username);
}

static void on_disconnected(int errorCode, std::string_view message) {
    Log.warn("Disconnected ({}: {})", errorCode, message);
}

static void on_error(int errorCode, std::string_view message) {
    Log.warn("Error ({}: {})", errorCode, message);
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

    borealis::discord::EventHandlers handlers{};
    handlers.ready = on_ready;
    handlers.disconnected = on_disconnected;
    handlers.error = on_error;
    g_initialized = borealis::discord::initialize(AppInfo, std::move(handlers));
    if (!g_initialized) {
        return;
    }

    Log.info("Discord Rich Presence initialized");
}

void run_callbacks() {
    if (!g_initialized)
        return;
    borealis::discord::run_callbacks();
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

    borealis::discord::Presence presence{};
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

    if (borealis::discord::update_presence(std::move(presence))) {
        Log.debug("Discord Rich Presence changed");
    }
}

void shutdown() {
    if (!g_initialized)
        return;
    borealis::discord::clear_presence();
    borealis::discord::shutdown();
    g_initialized = false;
    Log.info("Discord Rich Presence shut down");
}

}  // namespace dusk::discord

#endif  // BOREALIS_HAS_DISCORD
