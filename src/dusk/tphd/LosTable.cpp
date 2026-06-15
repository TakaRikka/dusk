#include "LosTable.hpp"

#include <fstream>
#include <vector>

#include "aurora/lib/logging.hpp"
#include "d/actor/d_a_player.h"
#include "dusk/endian.h"

static aurora::Module LosLog("dusk::tphd::los");

namespace dusk::tphd {

namespace {

struct LosHeader {
    /* 0x00 */ BE(u32) count;
    /* 0x04 */ BE(u32) unk04;
    /* 0x08 */ BE(u32) unk08;
};

struct LosEntry {
    /* 0x00 */ BE(u32) roomNo;
    /* 0x04 */ BE(u32) id1;  
    /* 0x08 */ BE(u32) id2;  
    /* 0x0C */ BE(u32) id3;
    /* 0x10 */ BE(f32) x;
    /* 0x14 */ BE(f32) y;  
    /* 0x18 */ BE(f32) z;
    /* 0x1C */ BE(s16) unk1C;
    /* 0x1E */ BE(s16) angleY;
};

static_assert(sizeof(LosHeader) == 0x0C);
static_assert(sizeof(LosEntry) == 0x20);

std::vector<u8> g_data;
u32 g_count = 0;

const LosEntry* entries() {
    return reinterpret_cast<const LosEntry*>(g_data.data() + sizeof(LosHeader));
}

}  // namespace

void load_los_table(const std::filesystem::path& contentPath) {
    g_data.clear();
    g_count = 0;
    if (contentPath.empty()) {
        return;
    }

    const std::filesystem::path losPath = contentPath / "los.bin";
    std::ifstream in(losPath, std::ios::binary);
    if (!in) {
        LosLog.info("no los.bin at {}", losPath.string());
        return;
    }

    std::vector<u8> data((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    if (data.size() < sizeof(LosHeader)) {
        LosLog.warn("los.bin too small: {} bytes", data.size());
        return;
    }

    u32 count = reinterpret_cast<const LosHeader*>(data.data())->count;
    if (sizeof(LosHeader) + size_t(count) * sizeof(LosEntry) > data.size()) {
        LosLog.warn("los.bin truncated: count={} size={}", count, data.size());
        return;
    }

    g_data = std::move(data);
    g_count = count;
    LosLog.info("loaded los.bin: {} room transforms from {}", count, losPath.string());
}

// Mirrors HD FUN_02ababbc:
bool los_get_room_trans(int roomNo, f32* o_x, f32* o_y, f32* o_z, s16* o_angle) {    
    if (g_count == 0 || roomNo < 0 || u32(roomNo) >= g_count) {
        return false;
    }
    const LosEntry& src = entries()[roomNo];
    *o_x = src.x;
    *o_y = src.y;
    *o_z = src.z;
    *o_angle = src.angleY;
    return true;
}

bool los_loaded() {
    return g_count != 0;
}

int los_room_count() {
    return int(g_count);
}

int los_next_floor(int roomNo) {
    if (g_count == 0 || roomNo < 0 || u32(roomNo) >= g_count) {
        return -1;
    }
    return s32(u32(entries()[roomNo].id1));
}

int los_prev_floor(int roomNo) {
    if (g_count == 0 || roomNo < 0 || u32(roomNo) >= g_count) {
        return -1;
    }
    return s32(u32(entries()[roomNo].id2));
}

int los_floor_index(int roomNo) {
    if (g_count == 0 || roomNo < 0 || u32(roomNo) >= g_count) {
        return 0;
    }
    return s32(u32(entries()[roomNo].id3));
}

// D_SB11 (Cave of Shadows, HD): true when the los table is loaded and we are in D_SB11.
// Mirrors HD's `g_dComIfG_gameInfo.field4_0x1e448 != 0` gate (set once in phase_1 for D_SB11).
bool is_los_active() {
    return tphd_active() &&
           los_loaded() &&
           std::strcmp(dComIfGp_getStartStageName(), "D_SB11") == 0;
}

void set_los_next_stage() {
    dComIfGp_setNextStage("D_SB11", 0, 19, -1, 0.0f, 0, 1, 0, 0, 1, 0);
    g_dComIfG_gameInfo.info.getRestart().setLastSceneInfo(20.0f, 0, 0);
    dComIfGs_setTurnRestart(dComIfGs_getTurnRestartPos(), 0, 0, daPy_py_c::setParamData(0, 1, 0xCA, 0));
    dComIfGs_setLife(dComIfGs_getMaxLife());
}

}  // namespace dusk::tphd
