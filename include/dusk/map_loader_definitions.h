#pragma once

#include <array>
#include "dusk/ui/lang.hpp"

struct RoomEntry {
    u8 roomNo;
    std::vector<s16> roomPoints = {};

    constexpr RoomEntry() : roomNo(0) {}
    constexpr RoomEntry(const RoomEntry& other) = default;

    template <int N>
    constexpr RoomEntry(const u8 roomNo, const s16 (&points)[N]) :
        roomNo(roomNo) {
        for (int i = 0; i < N; i++) {
            roomPoints.push_back(points[i]);
        }
    }

    constexpr RoomEntry(const u8 roomNo) :
        roomNo(roomNo) {
        roomPoints.push_back(0);
    }
};

struct MapEntry {
    const char* mapName;
    const char* mapFile;
    std::vector<RoomEntry> mapRooms = {};

    constexpr MapEntry() : mapName(nullptr), mapFile(nullptr) {}
    constexpr MapEntry(const MapEntry& other) = default;

    template <int N>
    constexpr MapEntry(const char* mapName, const char* mapFile, const RoomEntry (&rooms)[N], const char*) : mapName(mapName),
        mapFile(mapFile) {
        for (int i = 0; i < N; i++) {
            mapRooms.push_back(rooms[i]);
        }
    }

    template <int N>
    constexpr MapEntry(const char* mapName, const char* mapFile, const RoomEntry (&rooms)[N]) :
    mapName(mapName), mapFile(mapFile) {
        for (int i = 0; i < N; i++) {
            mapRooms.push_back(rooms[i]);
        }
    }

    constexpr MapEntry(const char* mapName, const char* mapFile) : mapName(mapName),
                mapFile(mapFile) {}
};

struct RegionEntry {
    const char* regionName = nullptr;
    std::vector<MapEntry> maps = {};

    template <int N>
    constexpr RegionEntry(const char* regionName, const MapEntry (&maps)[N]) : regionName(regionName) {
        for (int i = 0; i < N; i++) {
            this->maps.push_back(maps[i]);
        }
    }
};

static const auto gameRegions = std::to_array({
    RegionEntry("warp-menu.wh01-d01-btn1", {
        MapEntry("warp-menu.wh01-d01-btn1", "F_SP121",
            {
                {0, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21, 50}},
                {1, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21, 50}},
                {2, {0, 1, 10, 20, 30}},
                {3, {0, 1, 2, 3, 4, 5, 6, 10, 14, 15, 16, 17, 20, 21, 22, 88, 99}},
                {4, {0, 1}},
                {5, {0}},
                {6, {0, 1, 2, 3, 10, 11, 12, 21, 100, 101}},
                {7, {0, 1, 2, 6, 14, 22}},
                {9, {0, 1, 2, 10}},
                {10, {0, 1, 2, 3, 4, 5, 6, 7, 8, 14, 15, 16, 20, 21, 22, 23}},
                {11, {0, 1}},
                {12, {0, 1, 2, 3, 20, 21}},
                {13, {0, 1, 2, 3, 4, 14, 20, 21, 22, 23, 98, 99}},
                {14, {0, 1}},
                {15, {0, 1, 2, 3, 4, 5, 20, 53, 100, 101}},
            }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn2", {
        MapEntry("warp-menu.wh01-h01-btn2", "F_SP103", {
            {0, {0, 1, 2, 4, 5, 6, 7, 9, 11, 13, 14, 15, 20, 21, 22, 23, 24, 25, 26, 27, 30, 99, 100, 101, 102, 103}},
        }),
        MapEntry("warp-menu.wh01-h01-btn3", "F_SP103", {
            {1, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 20, 21, 23, 24, 25, 26, 27, 30, 99, 100}},
        }, "F_SP103_1"),
        MapEntry("warp-menu.wh01-h01-btn4", "F_SP00", {
            {0, {0, 1, 2, 3, 4, 5, 6, 7, 20, 30, 99, 127}},
        }),
        MapEntry("warp-menu.wh01-h01-btn5", "F_SP104", {
            {1, {0, 1, 2, 3, 4, 5, 6, 10, 20, 21, 22, 23, 24, 25, 26, 30, 99, 100, 111, 200, 254}}
        }),
        MapEntry("warp-menu.wh01-h01-btn6", "R_SP01", {
            {0, {0, 1, 2, 3}},
        }),
        MapEntry("warp-menu.wh01-h01-btn7", "R_SP01", {
            {1, {0}},
        }, "R_SP01_1"),
        MapEntry("warp-menu.wh01-h01-btn8", "R_SP01", {
            {2, {0, 1, 2, 3}},
        }, "R_SP01_2"),
        MapEntry("warp-menu.wh01-h01-btn9", "R_SP01", {
            {4, {0, 1, 2, 3, 4}},
            {7, {0}},
        }, "R_SP01_4"),
        MapEntry("warp-menu.wh01-h01-btn10", "R_SP01", {
            {5, {0, 1, 2}},
        }, "R_SP01_5"),
    }),
    RegionEntry("warp-menu.wh01-d01-btn3", {
        MapEntry("warp-menu.wh01-h01-btn11", "F_SP108", {
            {0, {0, 3, 4, 20, 21, 22, 23, 24, 25, 100, 254}},
            {1, {0, 1, 2, 3, 6, 20, 21, 100}},
            {2, {0}},
            {3, {0, 5, 99}},
            {4, {0, 1, 2, 7, 8, 9, 23, 100}},
            {5, {0, 1, 2, 3, 4, 6, 7, 10, 24, 25, 50, 60, 98, 100}},
            {8, {0, 1, 2, 3}},
            {11, {0}},
            {14, {0, 1, 2, 3, 10, 50, 100, 150, 200, 254}},
        }),
        MapEntry("warp-menu.wh01-h01-btn12", "F_SP108", {
            {6, {0, 1, 2, 3, 10, 50, 100, 150, 200, 254}},
        }, "F_SP108"),
        MapEntry("warp-menu.wh01-h01-btn13", "F_SP117", {
            {3, {0, 1, 2, 3, 4, 5, 6}},
        }),
        MapEntry("warp-menu.wh01-h01-btn14", "F_SP117", {
            {1, {1, 3, 4, 5, 6, 10, 20, 21, 50, 51, 99, 100, 102, 150, 200, 254}}
        }, "F_SP117_1"),
        MapEntry("warp-menu.wh01-h01-btn15", "F_SP117", {
            {2, {0, 1, 3, 52, 101, 102}},
        }, "F_SP117_2"),
        MapEntry("warp-menu.wh01-h01-btn16", "D_SB10", {
            {0, {0, 1, 20, 21}},
        }),
        MapEntry("warp-menu.wh01-h01-btn17", "R_SP108", {
            {0, {0, 1}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn4", {
        MapEntry("warp-menu.wh01-h01-btn18", "F_SP109", {
            {0, {
                0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 30, 31, 32, 33, 34, 35,
                36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
                52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
                68, 69, 70, 71, 100, 101,
            }},
        }),
        MapEntry("warp-menu.wh01-h01-btn19", "F_SP110", {
            {0, {0, 1, 2, 100, 200}},
            {1, {0}},
            {2, {0}},
            {3, {0, 1, 2, 3, 4, 5, 6}},
        }),
        MapEntry("warp-menu.wh01-h01-btn20", "F_SP111", {
            {0, {0, 1, 2, 3, 4, 5, 6, 111}},
        }),
        MapEntry("warp-menu.wh01-h01-btn21", "F_SP128", {
            {0, {0, 1, 2, 3, 4, 5, 100}},
        }),
        MapEntry("warp-menu.wh01-h01-btn22", "R_SP109", {
            {0, {0, 2, 3, 5, 6, 7, 8, 10, 20, 21, 22}},
        }),
        MapEntry("warp-menu.wh01-h01-btn23", "R_SP209", {
            {7, {0, 1, 2}},
        }),
        MapEntry("warp-menu.wh01-h01-btn24", "R_SP109", {
            {1, {0, 1, 2, 3}},
        }, "R_SP109_1"),
        MapEntry("warp-menu.wh01-h01-btn25", "R_SP109", {
            {2, {0, 1, 2, 3}},
        }, "R_SP109_2"),
        MapEntry("warp-menu.wh01-h01-btn26", "R_SP109", {
            {3, {0, 1}},
        }, "R_SP109_3"),
        MapEntry("warp-menu.wh01-h01-btn27", "R_SP109", {
            {4, {0, 1, 2}},
        }, "R_SP109_4"),
        MapEntry("warp-menu.wh01-h01-btn28", "R_SP109", {
            {5, {0, 1}},
        }, "R_SP109_5"),
        MapEntry("warp-menu.wh01-h01-btn29", "R_SP109", {
            {6, {0, 1, 5}},
        }, "R_SP109_6"),
        MapEntry("warp-menu.wh01-h01-btn30", "R_SP110", {
            {0, {0, 1, 2, 3, 4, 100}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn5", {
        MapEntry("warp-menu.wh01-h01-btn31", "F_SP122", {
            {8, {0, 1, 2, 3, 4, 5, 6, 7, 76, 100, 101, 111, 200, 254}},
        }),
        MapEntry("warp-menu.wh01-h01-btn32", "F_SP122", {
            {16, {0, 1, 2, 3, 4, 111}},
        }, "F_SP122_16"),
        MapEntry("warp-menu.wh01-h01-btn33", "F_SP122", {
            {17, {0, 1, 4}},
        }, "F_SP122_17"),
        MapEntry("warp-menu.wh01-h01-btn34", "F_SP116", {
            {0, {0, 3, 4, 5, 6, 11, 12, 13, 14, 15, 16, 20, 50, 99, 100}},
            {1, {0, 1, 30, 40, 50, 100, 111}},
            {2, {0, 1, 2, 3, 4}},
            {3, {0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 12, 13, 30}},
            {4, {0, 2, 3, 4, 5, 6}},
        }),
        MapEntry("warp-menu.wh01-h01-btn35", "F_SP112", {
            {1, {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 16, 17}},
        }),
        MapEntry("warp-menu.wh01-h01-btn36", "F_SP113", {
            {0, {0, 1, 3, 4, 5, 7, 8, 10, 50, 97, 99, 254}},
            {1, {0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 30, 34, 98, 100, 101}},
        }),
        MapEntry("warp-menu.wh01-h01-btn37", "F_SP115", {
            {0, {
                0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                16, 17, 20, 25, 29, 30, 31, 32, 33, 34, 40, 50, 55, 70, 75, 76,
                77, 78, 99, 100, 101, 133, 134, 150, 200, 254,
            }},
        }),
        MapEntry("warp-menu.wh01-h01-btn38", "F_SP115", {
            {1, {0, 1, 20, 21, 22, 23, 100}},
        }, "F_SP115_1"),
        MapEntry("warp-menu.wh01-h01-btn39", "F_SP126", {
            {0, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 99, 100, 101, 200}},
        }),
        MapEntry("warp-menu.wh01-h01-btn40", "F_SP127", {
            {0, {0, 1, 2, 3, 4, 5, 100}},
        }),
        MapEntry("warp-menu.wh01-h01-btn41", "R_SP107", {
            {0, {0, 1, 2, 3, 21, 22, 23, 24, 25}},
            {1, {0, 1, 2, 3, 4, 5, 6, 7}},
            {2, {0, 1, 2, 2, 20}},
            {3, {0, 1, 20, 21, 22, 23, 24}},
        }),
        MapEntry("warp-menu.wh01-h01-btn42", "R_SP116", {
            {5, {0, 1, 2, 3, 4, 5, 6, 20, 30}},
            {6, {10, 11, 12, 20, 21}},
        }),
        MapEntry("warp-menu.wh01-h01-btn43", "R_SP127", {
            {0, {0, 1}},
        }),
        MapEntry("warp-menu.wh01-h01-btn44", "R_SP128", {
            {0, {0}},
        }),
        MapEntry("warp-menu.wh01-h01-btn45", "R_SP160", {
            {0, {0, 1, 2}},
        }),
        MapEntry("warp-menu.wh01-h01-btn46", "R_SP160", {
            {1, {0, 1, 2}},
        }, "R_SP160_1"),
        MapEntry("warp-menu.wh01-h01-btn47", "R_SP160", {
            {2, {0, 1, 2}},
        }, "R_SP160_2"),
        MapEntry("warp-menu.wh01-h01-btn48", "R_SP160", {
            {3, {0, 1, 2}},
        }, "R_SP160_3"),
        MapEntry("warp-menu.wh01-h01-btn49", "R_SP160", {
            {4, {0, 1, 2}},
        }, "R_SP160_4"),
        MapEntry("warp-menu.wh01-h01-btn50", "R_SP160", {
            {5, {0, 1, 2, 3, 4}},
        }, "R_SP160_5"),
        MapEntry("warp-menu.wh01-h01-btn51", "R_SP161", {
            {7, {0, 1, 2, 3, 4}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn6", {
        MapEntry("warp-menu.wh01-h01-btn52", "F_SP118", {
            {0, {0}}, //TODO: can't load this one far enough to see its valid points
            {1, {0, 1, 2, 6}},
            {3, {0, 2, 3, 4, 5, 7}},
        }),
        MapEntry("warp-menu.wh01-h01-btn53", "F_SP118", {
            {2, {0}},
        }, "F_SP118_2"),
        MapEntry("warp-menu.wh01-d01-btn6", "F_SP124", {
            {0, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 50, 51, 52, 53, 111}},
        }),
        MapEntry("warp-menu.wh01-h01-btn54", "F_SP125", {
            {4, {0, 1, 2, 3, 4, 5, 6, 7, 8, 51, 52, 54, 55, 56, 57, 58}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn7", {
        MapEntry("warp-menu.wh01-h01-btn55", "F_SP114", {
            {0, {0, 1, 2, 4, 5, 6, 7, 10, 13, 14, 15, 100}},
            {1, {1, 2, 3, 5, 6, 9, 10, 11, 12, 13, 20, 21, 22, 100}},
            {2, {8, 12, 13}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn8", {
        MapEntry("warp-menu.wh01-d01-btn8", "D_MN05", {
            {0, {0}},
            {1, {0}},
            {2, {0}},
            {3, {0, 1}},
            {4, {0, 1, 2}},
            {5, {0, 1}},
            {7, {0}},
            {9, {0}},
            {10, {0}},
            {11, {0}},
            {12, {0, 1}},
            {19, {0}},
            {22, {0}},
        }),
        MapEntry("warp-menu.wh01-h01-btn56", "D_MN05A", {
            {50, {0, 1}},
        }),
        MapEntry("warp-menu.wh01-h01-btn57", "D_MN05B", {
            {51, {0, 1, 2}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn9", {
        MapEntry("warp-menu.wh01-d01-btn9", "D_MN04", {
            {1, {0, 1}},
            {3, {0}},
            {4, {0, 1}},
            {5, {0}},
            {6, {0, 1}},
            {7, {0, 1}},
            {9, {0, 1, 2, 3}},
            {11, {0, 1}},
            {12, {0, 1}},
            {13, {0}},
            {14, {0, 1}},
            {16, {0}},
            {17, {0, 1}},
        }),
        MapEntry("warp-menu.wh01-h01-btn58", "D_MN04A", {
            {50, {0, 1}},
        }),
        MapEntry("warp-menu.wh01-h01-btn59", "D_MN04B", {
            {51, {0, 1, 2, 3}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn10", {
        MapEntry("warp-menu.wh01-d01-btn10", "D_MN01", {
            {0, {0, 1, 2}},
            {1, {0}},
            {2, {0}},
            {3, {0, 1, 2}},
            {5, {0, 1, 2}},
            {6, {0, 1, 2}},
            {7, {0}},
            {8, {0, 2}},
            {9, {0, 1, 2, 3, 4}},
            {10, {0, 1}},
            {11, {0}},
            {12, {0, 1, 2}},
            {13, {0}},
        }),
        MapEntry("warp-menu.wh01-h01-btn60", "D_MN01A", {
            {50, {0, 1, 2, 3}},
        }),
        MapEntry("warp-menu.wh01-h01-btn61", "D_MN01B", {
            {51, {0, 1, 2, 3}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn11", {
        MapEntry("warp-menu.wh01-d01-btn11", "D_MN10", {
            {0, {0, 1, 2, 3}},
            {1, {0}},
            {2, {0, 1, 2, 3}},
            {3, {0}},
            {4, {0, 1, 2, 3}},
            {5, {0}},
            {6, {0, 1}},
            {7, {0}},
            {8, {0}},
            {9, {0, 1, 2}},
            {10, {0}},
            {11, {0, 1, 2, 3}},
            {12, {0}},
            {13, {0, 1}},
            {14, {0}},
            {15, {0, 1}},
            {16, {0}},
        }),
        MapEntry("warp-menu.wh01-h01-btn62", "D_MN10A", {
            {50, {0, 1, 2, 3}},
        }),
        MapEntry("warp-menu.wh01-h01-btn63", "D_MN10B", {
            {51, {0, 1, 2, 3}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn12", {
        MapEntry("warp-menu.wh01-d01-btn12", "D_MN11", {
            {0, {0, 1, 2, 3}},
            {1, {0}},
            {2, {0, 1, 2}},
            {3, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
            {4, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}},
            {5, {0, 1, 2, 3, 4}},
            {6, {0, 1, 2}},
            {7, {0, 10}},
            {8, {0}},
            {9, {0}},
            {11, {0}},
            {13, {0}},
        }),
        MapEntry("warp-menu.wh01-h01-btn64", "D_MN11A", {
            {50, {0, 1, 2, 3}},
        }),
        MapEntry("warp-menu.wh01-h01-btn65", "D_MN11B", {
            {51, {0, 1, 2, 3}},
        }),
        MapEntry("warp-menu.wh01-h01-btn66", "D_MN11B", {
            {49, {0, 1, 2}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn13", {
        MapEntry("warp-menu.wh01-d01.btn13", "D_MN06", {
            {0, {0, 1}},
            {1, {0}},
            {2, {0, 1, 2}},
            {3, {0, 1}},
            {4, {0, 1}},
            {5, {0}},
            {6, {0}},
            {7, {0, 1, 2}},
            {8, {0, 1, 2}},
        }),
        MapEntry("warp-menu.wh01-h01-btn67", "D_MN06A", {
            {50, {0, 1}},
        }),
        MapEntry("warp-menu.wh01-h01-btn68", "D_MN06B", {
            {51, {0}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn14", {
        MapEntry("warp-menu.wh01-d01-btn14", "D_MN07", {
            {0, {0, 1, 2, 3, 4, 5}},
            {1, {0}},
            {2, {0, 1, 2, 3, 4}},
            {3, {0, 1, 2, 3}},
            {4, {0, 1, 2}},
            {5, {0, 1, 2}},
            {6, {0, 1, 2, 3, 4, 5, 6, 7, 8}},
            {7, {0}},
            {8, {0}},
            {10, {0, 1, 3}},
            {11, {0, 1}},
            {12, {0, 1, 2, 3}},
            {13, {0, 1}},
            {14, {0, 1, 3}},
            {15, {0, 1, 3, 4}},
            {16, {0, 1, 2}},
        }),
        MapEntry("warp-menu.wh01-h01-btn69", "D_MN07A", {
            {50, {0, 1, 2, 3}},
        }),
        MapEntry("warp-menu.wh01-h01-btn70", "D_MN07B", {
            {51, {0, 1, 2}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn15", {
        MapEntry("warp-menu.wh01-d01-btn15", "D_MN08", {
            {0, {0, 1, 2, 3, 4, 10, 20, 21, 22}},
            {1, {0, 1}},
            {2, {0, 1}},
            {4, {0, 1}},
            {5, {0, 1, 2}},
            {7, {0, 1}},
            {8, {0}},
            {9, {0, 1, 20, 21}},
            {10, {0, 1}},
            {11, {0, 1, 20, 21, 22}},
        }),
        MapEntry("warp-menu.wh01-h01-btn71", "D_MN08A", {
            {10, {0, 1, 21, 23, 24, 25}},
        }),
        MapEntry("warp-menu.wh01-h01-btn72", "D_MN08B", {
            {51, {0, 1, 2, 3}},
        }),
        MapEntry("warp-menu.wh01-h01-btn73", "D_MN08C", {
            {52, {0, 2}},
        }),
        MapEntry("warp-menu.wh01-h01-btn74", "D_MN08D", {
            {50, {0, 20}},
            {53, {0}},
            {54, {0}},
            {55, {0, 1}},
            {56, {0}},
            {57, {0}},
            {60, {0}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn16", {
        MapEntry("warp-menu.wh01-d01-btn16", "D_MN09", {
            {1, {0, 1, 2, 3}},
            {2, {0, 2, 3}},
            {3, {0}},
            {4, {0, 1, 2}},
            {5, {0}},
            {6, {0, 1}},
            {8, {0}},
            {9, {0, 1, 2}},
            {11, {0, 1, 2, 3, 5}},
            {12, {0, 1, 2, 3, 4, 5, 6, 7, 8}},
            {13, {0}},
            {14, {0, 1, 2, 3, 4, 5}},
            {15, {0, 1, 2, 3, 4, 5, 6, 7}},
        }),
        MapEntry("warp-menu.wh01-h01-btn75", "D_MN09A", {
            {50, {0, 1, 2, 10, 20, 21, 22, 120, 121, 122}},
            {51, {0, 1, 2, 10, 20, 21, 22, 120, 121, 122}},
        }),
        MapEntry("warp-menu.wh01-h01-btn76", "D_MN09B", {
            {0, {0, 1}},
        }),
        MapEntry("warp-menu.wh01-h01-btn77", "D_MN09C", {
            {0, {0, 20, 21, 22, 23}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn17", {
        MapEntry("warp-menu.wh01-h01-btn78", "D_SB00", {
            {0, {0}},
        }),
        MapEntry("warp-menu.wh01-h01-btn79", "D_SB01", {
            {0, {0}},
            {1, {0}},
            {2, {0}},
            {3, {0}},
            {4, {0}},
            {5, {0}},
            {6, {0}},
            {7, {0}},
            {8, {0, 1}},
            {9, {0}},
            {10, {0}},
            {11, {0}},
            {12, {0}},
            {13, {0}},
            {14, {0}},
            {15, {0}},
            {16, {0}},
            {17, {0}},
            {18, {0, 1}},
            {19, {0}},
            {20, {0}},
            {21, {0}},
            {22, {0}},
            {23, {0}},
            {24, {0}},
            {25, {0}},
            {26, {0}},
            {27, {0}},
            {28, {0, 1}},
            {29, {0}},
            {30, {0}},
            {31, {0}},
            {32, {0}},
            {33, {0}},
            {34, {0}},
            {35, {0}},
            {36, {0}},
            {37, {0}},
            {38, {0, 1}},
            {39, {0}},
            {40, {0}},
            {41, {0}},
            {42, {0}},
            {43, {0}},
            {44, {0}},
            {45, {0}},
            {46, {0}},
            {47, {0}},
            {48, {0, 1}},
            {49, {0}},
        }
        ),
        MapEntry("warp-menu.wh01-h01-btn80", "D_SB02", {
            {0, {0}},
        }),
        MapEntry("warp-menu.wh01-h01-btn81", "D_SB03", {
            {0, {0, 1}},
        }),
        MapEntry("warp-menu.wh01-h01-btn82", "D_SB04", {
            {10, {0, 1}},
        }),
        MapEntry("warp-menu.wh01-h01-btn83", "D_SB05", {
            {0, {0, 1}},
        }),
        MapEntry("warp-menu.wh01-h01-btn84", "D_SB06", {
            {1, {0, 1}},
        }),
        MapEntry("warp-menu.wh01-h01-btn85", "D_SB07", {
            {2, {0, 1}},
        }),
        MapEntry("warp-menu.wh01-h01-btn86", "D_SB08", {
            {3, {0, 1}},
        }),
        MapEntry("warp-menu.wh01-h01-btn87", "D_SB09", {
            {4, {0, 1}},
        }),
    }),
    RegionEntry("warp-menu.wh01-d01-btn18", {
        MapEntry("warp-menu.wh01-h01-btn88", "F_SP102", {
            {0, {0, 1, 2, 3, 4, 5, 20, 53, 100, 101}},
        }),
        MapEntry("warp-menu.wh01-h01-btn89", "F_SP123", {
            {13, {0}},
        }),
        MapEntry("warp-menu.wh01-h01-btn90", "F_SP200", {
            {0, {0, 1, 2, 3, 4, 5, 6, 7}},
        }),
        MapEntry("warp-menu.wh01-h01-btn91", "R_SP300", {
            {0, {0, 20, 120}},
        }),
        MapEntry("warp-menu.wh01-h01-btn92", "R_SP301", {
            {0, {0, 20, 100}},
        }),
        MapEntry("warp-menu.wh01-h01-btn93", "S_MV000", {
            {0, {0, 1}},
        }),
    })
});
