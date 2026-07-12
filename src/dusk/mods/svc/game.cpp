#include "registry.hpp"

#include "mods/svc/game.h"

namespace dusk::mods::svc {
namespace {

constexpr GameService s_gameService{
    .header = SERVICE_HEADER(GameService, GAME_SERVICE_MAJOR, GAME_SERVICE_MINOR),
};

}  // namespace

constinit const ServiceModule g_gameModule{
    .id = GAME_SERVICE_ID,
    .majorVersion = GAME_SERVICE_MAJOR,
    .minorVersion = GAME_SERVICE_MINOR,
    .service = &s_gameService,
};

}  // namespace dusk::mods::svc
