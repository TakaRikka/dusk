#pragma once

#include "mods/svc/host.h"
#include "mods/svc/log.h"
#include "mods/svc/config.h"
#include "mods/svc/hook.h"
#include "mods/svc/ui.h"
#include "mods/svc/resource.h"
#include "mods/svc/save.h"
#include "mods/svc/stage.h"
#include "mods/svc/item.h"
#include "mods/svc/flow.h"
#include "mods/svc/message.h"
#include "mods/svc/game_mode.h"
#include "mods/svc/texture.h"

#include <string>

namespace randomizer::session {
struct ServiceManager {
    ModContext* mod_ctx;
    const HostService* host;
    const LogService* log;
    const HookService* hook;
    const UiService* ui;
    const ResourceService* resource;
    const ConfigService* config;
    const SaveService* save;
    const StageService* stage;
    const ItemService* item;
    const FlowService* flow;
    const MessageService* message;
    const GameModeService* game_mode;
    const TextureService* texture;
};

extern ServiceManager svc_mng;
extern std::string g_pending_seed_hash;

ModResult initialize(const ServiceManager& services);
void update();
void shutdown();

void deactivateSeed();
void setupRandomizerFile();
void registerStageEdits();
void registerStartingLocation();
}
