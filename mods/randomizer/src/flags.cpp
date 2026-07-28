#include "flags.h"
#include "stages.h"
#include "tools.h"

goldenWolfFlags getCurrentGoldenWolfFlags(u8 roomNo) {
    switch (getStageID()) {
    case Ordon_Spring:
        return {0x41, HOWLED_AT_DEATH_MOUNTAIN_STONE, GOT_SKILL_FROM_ORDON_WOLF};
    case Faron_Woods:
        return {0xFF, 0xFFFF, 0x3C10}; // Custom flag for rando
    case Kakariko_Graveyard:
        return {0x79, HOWLED_AT_SNOWPEAK_STONE, GOT_SKILL_FROM_GRAVEYARD_WOLF};
    case Outside_Castle_Town:
        if (roomNo == 8) {
            return {0x29, HOWLED_AT_UPPER_ZORAS_RIVER_STONE, GOT_SKILL_FROM_WEST_CT_WOLF};
        }
        return {0x2A, HOWLED_AT_SACRED_GROVE_OUTSIDE_STONE, GOT_SKILL_FROM_SOUTH_CT_FIELD_WOLF};
    case Castle_Town:
        return {0x32, HOWLED_AT_HIDDEN_VILLAGE_STONE, GOT_SKILL_FROM_BARRIER_WOLF};
    case Gerudo_Desert:
        return {0x32, HOWLED_AT_LAKE_HYLIA_STONE, GOT_SKILL_FROM_BULBLIN_CAMP_WOLF};
    default:
        return {0xFF, 0xFFFF, 0xFFFF};
    }
}