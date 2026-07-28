#pragma once

#include "dolphin/types.h"
#include "../generator/logic/item_pool.hpp"
#include "SSystem/SComponent/c_xyz.h"
#include "SSystem/SComponent/c_sxyz.h"

namespace randomizer::logic::location {
class Location;
}
namespace randomizer::logic::world {
class World;
}

bool playerIsInRoomStage(s32 room, const char* stage);
void checkTransformFromWolf();
u8 setNextWarashibeItem();
void offWarashibeItem(u8 item);
int initCreatePlayerItem(u32 item, u32 flag, const cXyz* pos, int roomNo, const csXyz* angle, const cXyz* scale);
/*
 * Returns the ID of the passed in stage name. If no stage name is passed in, the id of the current
 * stage is returned
 */
int getStageID(const char* stage = NULL);
bool playerIsOnTitleScreen();
u16 getItemMessageID(u8 itemId);
int numCompletedDungeons();
int numFusedShadows();
int numMirrorShards();
int getTempleKeysFound(int saveId);

/*
 * Reads the current player inventory and returns an ItemPool that can be used for logic searches
 *
 */
randomizer::logic::item_pool::ItemPool getSaveItemPool(randomizer::logic::world::World* world);

/*
 * Finds locations relevant flag in save (using its metadata) and checks if it's been set.
 */
bool isLocationObtained(randomizer::logic::location::Location* location);

/*
 * Pulls location item data from rando context using locations metadata.
 */
int getLocationItem(randomizer::logic::location::Location* location);

// Used to get a stage's Area ID used for save flags
int getStageSaveId(int id);
int getStageSaveId(const char* stage);

bool tracker_isEventBit(u16 flag);
bool tracker_isStageSwitch(int stage, int flag);
bool tracker_isStageItem(int stage, int flag);