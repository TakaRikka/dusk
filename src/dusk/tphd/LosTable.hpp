#ifndef DUSK_TPHD_LOS_TABLE_HPP
#define DUSK_TPHD_LOS_TABLE_HPP

#include <filesystem>

#include <dolphin/types.h>

namespace dusk::tphd {

// Loads `<contentPath>/los.bin` — the TP HD per-room transform table 
void load_los_table(const std::filesystem::path& contentPath);

// HD room map transform (mirrors HD `getMapTrans`/FUN_02905328 ) which
// fills world X/Y/Z translation and Y-rotation for `roomNo` from los.bin.
bool los_get_room_trans(int roomNo, f32* o_x, f32* o_y, f32* o_z, s16* o_angle);

bool los_loaded();

// Number of room entries in the los table 
int los_room_count();

int los_next_floor(int roomNo);
int los_prev_floor(int roomNo);

int los_floor_index(int roomNo);

bool is_los_active();
void set_los_next_stage();

}

#endif
