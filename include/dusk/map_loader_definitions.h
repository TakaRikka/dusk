#pragma once

struct MapEntry {
    static constexpr int MAX_ROOMS = 50;

    const char* mapName;
    const char* mapFile;
    u8 mapRooms[MAX_ROOMS] = {};
    int numRooms;

    constexpr MapEntry() : mapName(nullptr), mapFile(nullptr), numRooms(0) {}
    constexpr MapEntry(const MapEntry& other) = default;

    template <int N>
    constexpr MapEntry(const char* mapName, const char* mapFile, const int (&rooms)[N], const char*) : mapName(mapName),
        mapFile(mapFile), numRooms(N) {
        static_assert(N <= MAX_ROOMS);
        for (int i = 0; i < N; i++) {
            mapRooms[i] = rooms[i];
        }
    }

    template <int N>
    constexpr MapEntry(const char* mapName, const char* mapFile, const int (&rooms)[N]) :
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
        MapEntry("Hyrule Field", "F_SP121", {0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15}),
    }), RegionEntry("Ordon", {
        MapEntry("Ordon Village", "F_SP103"),
        MapEntry("Outside Link's House", "F_SP103", {1}, "F_SP103_1"),
        MapEntry("Ordon Ranch", "F_SP00"),
        MapEntry("Ordon Spring", "F_SP104", {1}),
        MapEntry("Bo's House", "R_SP01", {0}),
        MapEntry("Sera's Sundries", "R_SP01", {1}, "R_SP01_1"),
        MapEntry("Jaggle's House", "R_SP01", {2}, "R_SP01_2"),
        MapEntry("Link's House", "R_SP01", {4, 7}, "R_SP01_4"),
        MapEntry("Rusl's House", "R_SP01", {5}, "R_SP01_5"),
    }), RegionEntry("Faron", {
        MapEntry("South Faron Woods", "F_SP108", {0, 1, 2, 3, 4, 5, 8, 11, 14}),
        MapEntry("North Faron Woods", "F_SP108", {6}, "F_SP108"),
        MapEntry("Lost Woods", "F_SP117", {3}),
        MapEntry("Sacred Grove", "F_SP117", {1}, "F_SP117_1"),
        MapEntry("Temple of Time (Past)", "F_SP117", {2}, "F_SP117_2"),
        MapEntry("Faron Woods Cave", "D_SB10"),
        MapEntry("Coro's House", "R_SP108"),
    }), RegionEntry("Eldin", {
        MapEntry("Kakariko Village", "F_SP109"),
        MapEntry("Death Mountain Trail", "F_SP110", {0, 1, 2, 3}),
        MapEntry("Kakariko Graveyard", "F_SP111"),
        MapEntry("Hidden Village", "F_SP128"),
        MapEntry("Renado's Sanctuary", "R_SP109", {0}),
        MapEntry("Sanctuary Basement", "R_SP209", {7}),
        MapEntry("Barnes' Bombs", "R_SP109", {1}, "R_SP109_1"),
        MapEntry("Elde Inn", "R_SP109", {2}, "R_SP109_2"),
        MapEntry("Malo Mart", "R_SP109", {3}, "R_SP109_3"),
        MapEntry("Lookout Tower", "R_SP109", {4}, "R_SP109_4"),
        MapEntry("Bomb Warehouse", "R_SP109", {5}, "R_SP109_5"),
        MapEntry("Abandoned House", "R_SP109", {6}, "R_SP109_6"),
        MapEntry("Goron Elder's Hall", "R_SP110"),
    }), RegionEntry("Lanayru", {
        MapEntry("Outside Castle Town - West", "F_SP122", {8}),
        MapEntry("Outside Castle Town - South", "F_SP122", {16}, "F_SP122_16"),
        MapEntry("Outside Castle Town - East", "F_SP122", {17}, "F_SP122_17"),
        MapEntry("Castle Town", "F_SP116", {0, 1, 2, 3, 4}),
        MapEntry("Zora's River", "F_SP112", {1}),
        MapEntry("Zora's Domain", "F_SP113", {0, 1}),
        MapEntry("Lake Hylia", "F_SP115"),
        MapEntry("Lanayru Spring", "F_SP115", {1}, "F_SP115_1"),
        MapEntry("Upper Zora's River", "F_SP126", {0}),
        MapEntry("Fishing Pond", "F_SP127", {0}),
        MapEntry("Castle Town Sewers", "R_SP107", {0, 1, 2, 3}),
        MapEntry("Telma's Bar / Secret Passage", "R_SP116", {5, 6}),
        MapEntry("Hena's Cabin", "R_SP127", {0}),
        MapEntry("Impaz's House", "R_SP128", {0}),
        MapEntry("Malo Mart", "R_SP160", {0}),
        MapEntry("Fanadi's Palace", "R_SP160", {1}, "R_SP160_1"),
        MapEntry("Medical Clinic", "R_SP160", {2}, "R_SP160_2"),
        MapEntry("Agitha's Castle", "R_SP160", {3}, "R_SP160_3"),
        MapEntry("Goron Shop", "R_SP160", {4}, "R_SP160_4"),
        MapEntry("Jovani's House", "R_SP160", {5}, "R_SP160_5"),
        MapEntry("STAR Tent", "R_SP161", {7}),
    }), RegionEntry("Gerudo Desert", {
        MapEntry("Bulblin Camp", "F_SP118", {0, 1, 3}),
        MapEntry("Bulblin Camp Beta Room", "F_SP118", {2}, "F_SP118_2"),
        MapEntry("Gerudo Desert", "F_SP124", {0}),
        MapEntry("Mirror Chamber", "F_SP125", {4}),
    }), RegionEntry("Snowpeak", {
        MapEntry("Snowpeak Mountain", "F_SP114", {0, 1, 2}),
    }), RegionEntry("Forest Temple", {
        MapEntry("Forest Temple", "D_MN05", {0, 1, 2, 3, 4, 5, 7, 9, 10, 11, 12, 19, 22}),
        MapEntry("Diababa Arena", "D_MN05A", {50}),
        MapEntry("Ook Arena", "D_MN05B", {51}),
    }), RegionEntry("Goron Mines", {
        MapEntry("Goron Mines", "D_MN04", {1, 3, 4, 5, 6, 7, 9, 11, 12, 13, 14, 16, 17}),
        MapEntry("Fyrus Arena", "D_MN04A", {50}),
        MapEntry("Dangoro Arena", "D_MN04B", {51}),
    }), RegionEntry("Lakebed Temple", {
        MapEntry("Lakebed Temple", "D_MN01", {0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13}),
        MapEntry("Morpheel Arena", "D_MN01A", {50}),
        MapEntry("Deku Toad Arena", "D_MN01B", {51}),
    }), RegionEntry("Arbiter's Grounds", {
        MapEntry("Arbiter's Grounds", "D_MN10", {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}),
        MapEntry("Stallord Arena", "D_MN10A", {50}),
        MapEntry("Death Sword Arena", "D_MN10B", {51}),
    }), RegionEntry("Snowpeak Ruins", {
        MapEntry("Snowpeak Ruins", "D_MN11", {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 13}),
        MapEntry("Blizzeta Arena", "D_MN11A", {50}),
        MapEntry("Darkhammer Arena", "D_MN11B", {51}),
        MapEntry("Darkhammer Beta Arena", "D_MN11B", {49}),
    }), RegionEntry("Temple of Time", {
        MapEntry("Temple of Time", "D_MN06", {0, 1, 2, 3, 4, 5, 6, 7, 8}),
        MapEntry("Armogohma Arena", "D_MN06A", {50}),
        MapEntry("Darknut Arena", "D_MN06B", {51}),
    }), RegionEntry("City in the Sky", {
        MapEntry("City in the Sky", "D_MN07", {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16}),
        MapEntry("Argorok Arena", "D_MN07A", {50}),
        MapEntry("Aeralfos Arena", "D_MN07B", {51}),
    }), RegionEntry("Palace of Twilight", {
        MapEntry("Palace of Twilight", "D_MN08", {0, 1, 2, 4, 5, 7, 8, 9, 10, 11}),
        MapEntry("Palace of Twilight Throne Room", "D_MN08A", {10}),
        MapEntry("Phantom Zant Arena 1", "D_MN08B", {51}),
        MapEntry("Phantom Zant Arena 2", "D_MN08C", {52}),
        MapEntry("Zant Arenas", "D_MN08D", {50, 53, 54, 55, 56, 57, 60}),
    }), RegionEntry("Hyrule Castle", {
        MapEntry("Hyrule Castle", "D_MN09", {1, 2, 3, 4, 5, 6, 8, 9, 11, 12, 13, 14, 15}),
        MapEntry("Hyrule Castle Throne Room", "D_MN09A", {50, 51}),
        MapEntry("Horseback Ganondorf Arena", "D_MN09B", {0}),
        MapEntry("Dark Lord Ganondorf Arena", "D_MN09C", {0}),
    }), RegionEntry("Mini-Dungeons and Grottos", {
        MapEntry("Ice Cavern", "D_SB00", {0}),
        MapEntry("Cave Of Ordeals", "D_SB01",
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
            20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
            30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
            40, 41, 42, 43, 44, 45, 46, 47, 48, 49}
        ),
        MapEntry("Kakariko Gorge Cavern", "D_SB02", {0}),
        MapEntry("Lake Hylia Cavern", "D_SB03", {0}),
        MapEntry("Goron Stockcave", "D_SB04", {10}),
        MapEntry("Grotto 1", "D_SB05"),
        MapEntry("Grotto 2", "D_SB06", {1}),
        MapEntry("Grotto 3", "D_SB07", {2}),
        MapEntry("Grotto 4", "D_SB08", {3}),
        MapEntry("Grotto 5", "D_SB09", {4}),
    }), RegionEntry("Misc", {
        MapEntry("Title Screen / King Bulblin 1", "F_SP102"),
        MapEntry("King Bulblin 2", "F_SP123", {13}),
        MapEntry("Wolf Howling Cutscene Map", "F_SP200"),
    //  MapEntry("Cutscene: Light Arrow Area", "R_SP300"),
        MapEntry("Cutscene: Hyrule Castle Throne Room", "R_SP301"),
    })
});