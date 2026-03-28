#pragma once

struct RoomEntry {
    static constexpr int MAX_POINTS = 70;

    u8 roomNo;
    s16 roomPoints[MAX_POINTS] = {};
    int numPoints;

    constexpr RoomEntry() : roomNo(0), numPoints(0) {}
    constexpr RoomEntry(const RoomEntry& other) = default;

    template <int N>
    constexpr RoomEntry(const u8 roomNo, const s16 (&points)[N]) :
        roomNo(roomNo), numPoints(N) {
        static_assert(N <= MAX_POINTS);
        for (int i = 0; i < N; i++) {
            roomPoints[i] = points[i];
        }
    }

    constexpr RoomEntry(const u8 roomNo) :
        roomNo(roomNo), numPoints(1) {
        roomPoints[0] = 0;
    }
};

struct MapEntry {
    static constexpr int MAX_ROOMS = 50;

    const char* mapName;
    const char* mapFile;
    RoomEntry mapRooms[MAX_ROOMS] = {};
    int numRooms;

    constexpr MapEntry() : mapName(nullptr), mapFile(nullptr), numRooms(0) {}
    constexpr MapEntry(const MapEntry& other) = default;

    template <int N>
    constexpr MapEntry(const char* mapName, const char* mapFile, const RoomEntry (&rooms)[N], const char*) : mapName(mapName),
        mapFile(mapFile), numRooms(N) {
        static_assert(N <= MAX_ROOMS);
        for (int i = 0; i < N; i++) {
            mapRooms[i] = rooms[i];
        }
    }

    template <int N>
    constexpr MapEntry(const char* mapName, const char* mapFile, const RoomEntry (&rooms)[N]) :
    mapName(mapName), mapFile(mapFile), numRooms(N) {
        static_assert(N <= MAX_ROOMS);
        for (int i = 0; i < N; i++) {
            mapRooms[i] = rooms[i];
        }
    }

    constexpr MapEntry(const char* mapName, const char* mapFile) : mapName(mapName),
                mapFile(mapFile), numRooms(0) {}
};

struct RegionEntry {
    static constexpr int MAX_MAPS = 22;
    const char* regionName = nullptr;
    int numMaps = 0;
    MapEntry maps[MAX_MAPS] = {};

    template <int N>
    constexpr RegionEntry(const char* regionName, const MapEntry (&maps)[N]) : regionName(regionName), numMaps(N) {
        static_assert(N <= MAX_MAPS);
        for (int i = 0; i < N; i++) {
            this->maps[i] = maps[i];
        }
    }
};

constexpr auto gameRegions = std::to_array({
    RegionEntry("Hyrule Field", {
        MapEntry("Hyrule Field", "F_SP121",
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
    RegionEntry("Ordon", {
        MapEntry("Ordon Village", "F_SP103", {
            {0, {0, 1, 2, 4, 5, 6, 7, 9, 11, 13, 14, 15, 20, 21, 22, 23, 24, 25, 26, 27, 30, 99, 100, 101, 102, 103}},
        }),
        MapEntry("Outside Link's House", "F_SP103", {
            {1, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 20, 21, 23, 24, 25, 26, 27, 30, 99, 100}},
        }, "F_SP103_1"),
        MapEntry("Ordon Ranch", "F_SP00", {
            {0, {0, 1, 2, 3, 4, 5, 6, 7, 20, 30, 99, 127}},
        }),
        MapEntry("Ordon Spring", "F_SP104", {
            {1, {0, 1, 2, 3, 4, 5, 6, 10, 20, 21, 22, 23, 24, 25, 26, 30, 99, 100, 111, 200, 254}}
        }),
        MapEntry("Bo's House", "R_SP01", {
            {0, {0, 1, 2, 3}},
        }),
        MapEntry("Sera's Sundries", "R_SP01", {
            {1, {0}},
        }, "R_SP01_1"),
        MapEntry("Jaggle's House", "R_SP01", {
            {2, {0, 1, 2, 3}},
        }, "R_SP01_2"),
        MapEntry("Link's House", "R_SP01", {
            {4, {0, 1, 2, 3, 4}},
            {7, {0}},
        }, "R_SP01_4"),
        MapEntry("Rusl's House", "R_SP01", {
            {5, {0, 1, 2}},
        }, "R_SP01_5"),
    }),
    RegionEntry("Faron", {
        MapEntry("South Faron Woods", "F_SP108", {
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
        MapEntry("North Faron Woods", "F_SP108", {
            {6, {0, 1, 2, 3, 10, 50, 100, 150, 200, 254}},
        }, "F_SP108"),
        MapEntry("Lost Woods", "F_SP117", {
            {3, {0, 1, 2, 3, 4, 5, 6}},
        }),
        MapEntry("Sacred Grove", "F_SP117", {
            {1, {1, 3, 4, 5, 6, 10, 20, 21, 50, 51, 99, 100, 102, 150, 200, 254}}
        }, "F_SP117_1"),
        MapEntry("Temple of Time (Past)", "F_SP117", {
            {2, {0, 1, 3, 52, 101, 102}},
        }, "F_SP117_2"),
        MapEntry("Faron Woods Cave", "D_SB10", {
            {0, {0, 1, 20, 21}},
        }),
        MapEntry("Coro's House", "R_SP108", {
            {0, {0, 1}},
        }),
    }),
    RegionEntry("Eldin", {
        MapEntry("Kakariko Village", "F_SP109", {
            {0, {
                0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 30, 31, 32, 33, 34, 35,
                36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
                52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
                68, 69, 70, 71, 100, 101,
            }},
        }),
        MapEntry("Death Mountain Trail", "F_SP110", {
            {0, {0, 1, 2, 100, 200}},
            {1, {0}},
            {2, {0}},
            {3, {0, 1, 2, 3, 4, 5, 6}},
        }),
        MapEntry("Kakariko Graveyard", "F_SP111", {
            {0, {0, 1, 2, 3, 4, 5, 6, 111}},
        }),
        MapEntry("Hidden Village", "F_SP128", {
            {0, {0, 1, 2, 3, 4, 5, 100}},
        }),
        MapEntry("Renado's Sanctuary", "R_SP109", {
            {0, {0, 2, 3, 5, 6, 7, 8, 10, 20, 21, 22}},
        }),
        MapEntry("Sanctuary Basement", "R_SP209", {
            {7, {0, 1, 2}},
        }),
        MapEntry("Barnes' Bombs", "R_SP109", {
            {1, {0, 1, 2, 3}},
        }, "R_SP109_1"),
        MapEntry("Elde Inn", "R_SP109", {
            {2, {0, 1, 2, 3}},
        }, "R_SP109_2"),
        MapEntry("Malo Mart", "R_SP109", {
            {3, {0, 1}},
        }, "R_SP109_3"),
        MapEntry("Lookout Tower", "R_SP109", {
            {4, {0, 1, 2}},
        }, "R_SP109_4"),
        MapEntry("Bomb Warehouse", "R_SP109", {
            {5, {0, 1}},
        }, "R_SP109_5"),
        MapEntry("Abandoned House", "R_SP109", {
            {6, {0, 1, 5}},
        }, "R_SP109_6"),
        MapEntry("Goron Elder's Hall", "R_SP110", {
            {0, {0, 1, 2, 3, 4, 100}},
        }),
    }),
    RegionEntry("Lanayru", {
        MapEntry("Outside Castle Town - West", "F_SP122", {
            {8, {0, 1, 2, 3, 4, 5, 6, 7, 76, 100, 101, 111, 200, 254}},
        }),
        MapEntry("Outside Castle Town - South", "F_SP122", {
            {16, {0, 1, 2, 3, 4, 111}},
        }, "F_SP122_16"),
        MapEntry("Outside Castle Town - East", "F_SP122", {
            {17, {0, 1, 4}},
        }, "F_SP122_17"),
        MapEntry("Castle Town", "F_SP116", {
            {0, {0, 3, 4, 5, 6, 11, 12, 13, 14, 15, 16, 20, 50, 99, 100}},
            {1, {0, 1, 30, 40, 50, 100, 111}},
            {2, {0, 1, 2, 3, 4}},
            {3, {0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 12, 13, 30}},
            {4, {0, 2, 3, 4, 5, 6}},
        }),
        MapEntry("Zora's River", "F_SP112", {
            {1, {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 16, 17}},
        }),
        MapEntry("Zora's Domain", "F_SP113", {
            {0, {0, 1, 3, 4, 5, 7, 8, 10, 50, 97, 99, 254}},
            {1, {0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 30, 34, 98, 100, 101}},
        }),
        MapEntry("Lake Hylia", "F_SP115", {
            {0, {
                0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                16, 17, 20, 25, 29, 30, 31, 32, 33, 34, 40, 50, 55, 70, 75, 76,
                77, 78, 99, 100, 101, 133, 134, 150, 200, 254,
            }},
        }),
        MapEntry("Lanayru Spring", "F_SP115", {
            {1, {0, 1, 20, 21, 22, 23, 100}},
        }, "F_SP115_1"),
        MapEntry("Upper Zora's River", "F_SP126", {
            {0, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 99, 100, 101, 200}},
        }),
        MapEntry("Fishing Pond", "F_SP127", {
            {0, {0, 1, 2, 3, 4, 5, 100}},
        }),
        MapEntry("Castle Town Sewers", "R_SP107", {
            {0, {0, 1, 2, 3, 21, 22, 23, 24, 25}},
            {1, {0, 1, 2, 3, 4, 5, 6, 7}},
            {2, {0, 1, 2, 2, 20}},
            {3, {0, 1, 20, 21, 22, 23, 24}},
        }),
        MapEntry("Telma's Bar / Secret Passage", "R_SP116", {
            {5, {0, 1, 2, 3, 4, 5, 6, 20, 30}},
            {6, {10, 11, 12, 20, 21}},
        }),
        MapEntry("Hena's Cabin", "R_SP127", {
            {0, {0, 1}},
        }),
        MapEntry("Impaz's House", "R_SP128", {
            {0, {0}},
        }),
        MapEntry("Malo Mart", "R_SP160", {
            {0, {0, 1, 2}},
        }),
        MapEntry("Fanadi's Palace", "R_SP160", {
            {1, {0, 1, 2}},
        }, "R_SP160_1"),
        MapEntry("Medical Clinic", "R_SP160", {
            {2, {0, 1, 2}},
        }, "R_SP160_2"),
        MapEntry("Agitha's Castle", "R_SP160", {
            {3, {0, 1, 2}},
        }, "R_SP160_3"),
        MapEntry("Goron Shop", "R_SP160", {
            {4, {0, 1, 2}},
        }, "R_SP160_4"),
        MapEntry("Jovani's House", "R_SP160", {
            {5, {0, 1, 2, 3, 4}},
        }, "R_SP160_5"),
        MapEntry("STAR Tent", "R_SP161", {
            {7, {0, 1, 2, 3, 4}},
        }),
    }),
    RegionEntry("Gerudo Desert", {
        MapEntry("Bulblin Camp", "F_SP118", {
            {0, {0}}, //TODO: can't load this one far enough to see its valid points
            {1, {0, 1, 2, 6}},
            {3, {0, 2, 3, 4, 5, 7}},
        }),
        MapEntry("Bulblin Camp Beta Room", "F_SP118", {
            {2, {0}},
        }, "F_SP118_2"),
        MapEntry("Gerudo Desert", "F_SP124", {
            {0, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 50, 51, 52, 53, 111}},
        }),
        MapEntry("Mirror Chamber", "F_SP125", {
            {4, {0, 1, 2, 3, 4, 5, 6, 7, 8, 51, 52, 54, 55, 56, 57, 58}},
        }),
    }),
    RegionEntry("Snowpeak", {
        MapEntry("Snowpeak Mountain", "F_SP114", {
            {0, {0, 1, 2, 4, 5, 6, 7, 10, 13, 14, 15, 100}},
            {1, {1, 2, 3, 5, 6, 9, 10, 11, 12, 13, 20, 21, 22, 100}},
            {2, {8, 12, 13}},
        }),
    }),
    RegionEntry("Forest Temple", {
        MapEntry("Forest Temple", "D_MN05", {
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
        MapEntry("Diababa Arena", "D_MN05A", {
            {50, {0, 1}},
        }),
        MapEntry("Ook Arena", "D_MN05B", {
            {51, {0, 1, 2}},
        }),
    }),
    RegionEntry("Goron Mines", {
        MapEntry("Goron Mines", "D_MN04", {
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
        MapEntry("Fyrus Arena", "D_MN04A", {
            {50, {0, 1}},
        }),
        MapEntry("Dangoro Arena", "D_MN04B", {
            {51, {0, 1, 2, 3}},
        }),
    }),
    RegionEntry("Lakebed Temple", {
        MapEntry("Lakebed Temple", "D_MN01", {
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
        MapEntry("Morpheel Arena", "D_MN01A", {
            {50, {0, 1, 2, 3}},
        }),
        MapEntry("Deku Toad Arena", "D_MN01B", {
            {51, {0, 1, 2, 3}},
        }),
    }),
    RegionEntry("Arbiter's Grounds", {
        MapEntry("Arbiter's Grounds", "D_MN10", {
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
        MapEntry("Stallord Arena", "D_MN10A", {
            {50, {0, 1, 2, 3}},
        }),
        MapEntry("Death Sword Arena", "D_MN10B", {
            {51, {0, 1, 2, 3}},
        }),
    }),
    RegionEntry("Snowpeak Ruins", {
        MapEntry("Snowpeak Ruins", "D_MN11", {
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
        MapEntry("Blizzeta Arena", "D_MN11A", {
            {50, {0, 1, 2, 3}},
        }),
        MapEntry("Darkhammer Arena", "D_MN11B", {
            {51, {0, 1, 2, 3}},
        }),
        MapEntry("Darkhammer Beta Arena", "D_MN11B", {
            {49, {0, 1, 2}},
        }),
    }),
    RegionEntry("Temple of Time", {
        MapEntry("Temple of Time", "D_MN06", {
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
        MapEntry("Armogohma Arena", "D_MN06A", {
            {50, {0, 1}},
        }),
        MapEntry("Darknut Arena", "D_MN06B", {
            {51, {0}},
        }),
    }),
    RegionEntry("City in the Sky", {
        MapEntry("City in the Sky", "D_MN07", {
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
        MapEntry("Argorok Arena", "D_MN07A", {
            {50, {0, 1, 2, 3}},
        }),
        MapEntry("Aeralfos Arena", "D_MN07B", {
            {51, {0, 1, 2}},
        }),
    }),
    RegionEntry("Palace of Twilight", {
        MapEntry("Palace of Twilight", "D_MN08", {
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
        MapEntry("Palace of Twilight Throne Room", "D_MN08A", {
            {10, {0, 1, 21, 23, 24, 25}},
        }),
        MapEntry("Phantom Zant Arena 1", "D_MN08B", {
            {51, {0, 1, 2, 3}},
        }),
        MapEntry("Phantom Zant Arena 2", "D_MN08C", {
            {52, {0, 2}},
        }),
        MapEntry("Zant Arenas", "D_MN08D", {
            {50, {0, 20}},
            {53, {0}},
            {54, {0}},
            {55, {0, 1}},
            {56, {0}},
            {57, {0}},
            {60, {0}},
        }),
    }),
    RegionEntry("Hyrule Castle", {
        MapEntry("Hyrule Castle", "D_MN09", {
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
        MapEntry("Hyrule Castle Throne Room", "D_MN09A", {
            {50, {0, 1, 2, 10, 20, 21, 22, 120, 121, 122}},
            {51, {0, 1, 2, 10, 20, 21, 22, 120, 121, 122}},
        }),
        MapEntry("Horseback Ganondorf Arena", "D_MN09B", {
            {0, {0, 1}},
        }),
        MapEntry("Dark Lord Ganondorf Arena", "D_MN09C", {
            {0, {0, 20, 21, 22, 23}},
        }),
    }),
    RegionEntry("Mini-Dungeons and Grottos", {
        MapEntry("Ice Cavern", "D_SB00", {
            {0, {0}},
        }),
        MapEntry("Cave Of Ordeals", "D_SB01", {
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
        MapEntry("Kakariko Gorge Cavern", "D_SB02", {
            {0, {0}},
        }),
        MapEntry("Lake Hylia Cavern", "D_SB03", {
            {0, {0, 1}},
        }),
        MapEntry("Goron Stockcave", "D_SB04", {
            {10, {0, 1}},
        }),
        MapEntry("Grotto 1", "D_SB05", {
            {0, {0, 1}},
        }),
        MapEntry("Grotto 2", "D_SB06", {
            {1, {0, 1}},
        }),
        MapEntry("Grotto 3", "D_SB07", {
            {2, {0, 1}},
        }),
        MapEntry("Grotto 4", "D_SB08", {
            {3, {0, 1}},
        }),
        MapEntry("Grotto 5", "D_SB09", {
            {4, {0, 1}},
        }),
    }),
    RegionEntry("Misc", {
        MapEntry("Title Screen / King Bulblin 1", "F_SP102", {
            {0, {0, 1, 2, 3, 4, 5, 20, 53, 100, 101}},
        }),
        MapEntry("King Bulblin 2", "F_SP123", {
            {13, {0}},
        }),
        MapEntry("Wolf Howling Cutscene Map", "F_SP200", {
            {0, {0, 1, 2, 3, 4, 5, 6, 7}},
        }),
        MapEntry("Cutscene: Light Arrow Area", "R_SP300", {
            {0, {0, 20, 120}},
        }),
        MapEntry("Cutscene: Hyrule Castle Throne Room", "R_SP301", {
            {0, {0, 20, 100}},
        }),
        MapEntry("Title screen movie map", "S_MV000", {
            {0, {0, 1}},
        }),
    })
});
