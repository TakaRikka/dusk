#pragma once

#include "dolphin/types.h"

namespace dusk {

/**
 * Load bytes from the main DOL by GameCube virtual address
 */
bool LoadDolAsset(void* dst, u32 virtualAddress, s32 size);

/**
 * Load bytes from a REL file in the ISO filesystem
 */
void* LoadRelAsset(const char* dvdPath, s32 offset, s32 size);

/**
 * Load bytes from a REL inside RELS.arc
 */
bool LoadArchivedRelAsset(void* dst, u32 memType, const char* relFileName, s32 offset, s32 size);

}  // namespace dusk
