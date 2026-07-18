#include <mods/service.hpp>
#include <mods/svc/game.h>

// Linking the game feature makes the loader enforce the game ABI epoch.
IMPORT_SERVICE(GameService, svc_game);
