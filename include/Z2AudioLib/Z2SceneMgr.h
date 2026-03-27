#ifndef Z2SCENEMGR_H
#define Z2SCENEMGR_H

#include "JSystem/JAudio2/JAISound.h"
#include "JSystem/JAudio2/JASGadget.h"

s8 dComIfGp_getReverb(int roomNo);

class Z2SceneMgr : public JASGlobalInstance<Z2SceneMgr> {
public:
    Z2SceneMgr();
    void setInDarkness(bool inDarkness);
    void setSceneExist(bool isSceneExist);
    void setFadeOutStart(u8 fadeType);
    void setFadeInStart(u8 fadeType);
    void setSceneName(char* spot, s32 room, s32 layer);
    void sceneChange(JAISoundID bgm, u8 seWave1, u8 seWave2, u8 bgmWave1, u8 bgmWave2, u8 demoWave,
                     bool);
    void framework();
    void load1stDynamicWave();
    void _load1stWaveInner_1();
    void _load1stWaveInner_2();
    bool check1stDynamicWave();
    void load2ndDynamicWave();
    void sceneBgmStart();
    void loadStaticWaves();
    BOOL checkFirstWaves();
    bool eraseSeWave(u32 wave);
    bool eraseBgmWave(u32 wave);
    s32 getWaveLoadStatus(u32 wave, u32 bank);
    bool loadSeWave(u32 wave);
    bool loadBgmWave(u32 wave);

#if PLATFORM_SHIELD
    bool loadSceneWave(u32 wave, u32 bank);
#endif

    bool isSceneExist() { return sceneExist; }
    int getCurrentSceneNum() { return sceneNum; }
    s8 getCurrentRoomNum() { return roomNum; }
    bool isInGame() { return inGame; }
    void setInGame(bool isInGame) { inGame = isInGame; }
    bool isInDarkness() { return inDarkness_; }
    s8 getRoomReverb() { return dComIfGp_getReverb(roomNum); }
    bool isMovieDemo() { return sceneNum == 2 || sceneNum == 8 || sceneNum == 9; }
    s32 getSeLoadStatus(u32 wave) { return getWaveLoadStatus(wave, 0); }
    s32 getBgmLoadStatus(u32 wave) { return getWaveLoadStatus(wave, 1); }
    u8 getDemoSeWaveNum() { return loadedDemoWave; }

private:
    /* 0x00 */ JAISoundID BGM_ID;
    /* 0x04 */ int sceneNum;
    /* 0x08 */ int timer;
    /* 0x0C */ s8 roomNum;
    /* 0x0D */ u8 requestSeWave_1;
    /* 0x0E */ u8 loadedSeWave_1;
    /* 0x0F */ u8 requestSeWave_2;
    /* 0x10 */ u8 loadedSeWave_2;
    /* 0x11 */ u8 requestBgmWave_1;
    /* 0x12 */ u8 loadedBgmWave_1;
    /* 0x13 */ u8 requestBgmWave_2;
    /* 0x14 */ u8 loadedBgmWave_2;
    /* 0x15 */ u8 requestDemoWave;
    /* 0x16 */ u8 loadedDemoWave;
    /* 0x17 */ s8 load1stWait;
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 field_0x19;
    /* 0x1A */ bool field_0x1a;
    /* 0x1B */ bool field_0x1b;
    /* 0x1C */ bool inGame;
    /* 0x1D */ bool sceneExist;
    /* 0x1E */ bool inDarkness_;
};  // Size = 0x20

inline Z2SceneMgr* Z2GetSceneMgr() {
    return JASGlobalInstance<Z2SceneMgr>::getInstance();
}

enum Z2Scene {
    /* 0x00 */ Z2SCENE_ORDON_RANCH,
    /* 0x01 */ Z2SCENE_ORDON_VILLAGE,
    /* 0x02 */ Z2SCENE_ORDON_INTERIOR,
    /* 0x03 */ Z2SCENE_ORDON_SPRING,
    /* 0x04 */ Z2SCENE_TWILIGHT_HYRULE_CASTLE,
    /* 0x05 */ Z2SCENE_FARON_WOODS,
    /* 0x06 */ Z2SCENE_CORO_SHOP,
    /* 0x07 */ Z2SCENE_SACRED_GROVE,
    /* 0x08 */ Z2SCENE_KAKARIKO_VILLAGE,
    /* 0x09 */ Z2SCENE_KAKARIKO_INTERIOR,
    /* 0x0A */ Z2SCENE_SANCTUARY_BASEMENT,
    /* 0x0B */ Z2SCENE_DEATH_MOUNTAIN,
    /* 0x0C */ Z2SCENE_SUMO_HALL,
    /* 0x0D */ Z2SCENE_KAKARIKO_GRAVEYARD,
    /* 0x0E */ Z2SCENE_HIDDEN_VILLAGE,
    /* 0x0F */ Z2SCENE_IMPAZ_HOUSE,
    /* 0x10 */ Z2SCENE_LAKE_HYLIA,
    /* 0x11 */ Z2SCENE_ZORAS_RIVER,
    /* 0x12 */ Z2SCENE_UPPER_ZORAS_RIVER,
    /* 0x13 */ Z2SCENE_FISHING_POND,
    /* 0x14 */ Z2SCENE_HENA_CABIN,
    /* 0x15 */ Z2SCENE_ZORAS_DOMAIN,
    /* 0x16 */ Z2SCENE_CASTLE_TOWN,
    /* 0x17 */ Z2SCENE_CASTLE_TOWN_INTERIOR,
    /* 0x18 */ Z2SCENE_CASTLE_TOWN_SHOPS,
    /* 0x19 */ Z2SCENE_STAR_TENT,
    /* 0x1A */ Z2SCENE_SNOWPEAK,
    /* 0x1B */ Z2SCENE_ARBITERS_GROUNDS_EXTERIOR,
    /* 0x1C */ Z2SCENE_GERUDO_DESERT,
    /* 0x1D */ Z2SCENE_MIRROR_CHAMBER,
    /* 0x1E */ Z2SCENE_HYRULE_FIELD,
    /* 0x1F */ Z2SCENE_CASTLE_TOWN_GATES,
    /* 0x20 */ Z2SCENE_HYLIA_BRIDGE_BATTLE,
    /* 0x21 */ Z2SCENE_SHADES_REALM,
    /* 0x22 */ Z2SCENE_ELDIN_BRIDGE_BATTLE,
    /* 0x23 */ Z2SCENE_NONE,
    /* 0x24 */ Z2SCENE_LIGHT_SPIRIT_CHAMBER,
    /* 0x25 */ Z2SCENE_CASTLE_THRONE_ROOM,
    /* 0x26 */ Z2SCENE_ENEMY_TEST,
    /* 0x27 */ Z2SCENE_FORTRESS,
    /* 0x28 */ Z2SCENE_FOREST_TEMPLE,
    /* 0x29 */ Z2SCENE_FOREST_TEMPLE_MINIBOSS,
    /* 0x2A */ Z2SCENE_FOREST_TEMPLE_BOSS,
    /* 0x2B */ Z2SCENE_GORON_MINES,
    /* 0x2C */ Z2SCENE_GORON_MINES_MINIBOSS,
    /* 0x2D */ Z2SCENE_GORON_MINES_BOSS,
    /* 0x2E */ Z2SCENE_LAKEBED_TEMPLE,
    /* 0x2F */ Z2SCENE_LAKEBED_TEMPLE_MINIBOSS,
    /* 0x30 */ Z2SCENE_LAKEBED_TEMPLE_BOSS,
    /* 0x31 */ Z2SCENE_ARBITERS_GROUNDS,
    /* 0x32 */ Z2SCENE_ARBITERS_GROUNDS_MINIBOSS,
    /* 0x33 */ Z2SCENE_ARBITERS_GROUNDS_BOSS,
    /* 0x34 */ Z2SCENE_SNOWPEAK_RUINS,
    /* 0x35 */ Z2SCENE_SNOWPEAK_RUINS_MINIBOSS,
    /* 0x36 */ Z2SCENE_SNOWPEAK_RUINS_BOSS,
    /* 0x37 */ Z2SCENE_TEMPLE_OF_TIME,
    /* 0x38 */ Z2SCENE_TEMPLE_OF_TIME_MINIBOSS,
    /* 0x39 */ Z2SCENE_TEMPLE_OF_TIME_BOSS,
    /* 0x3A */ Z2SCENE_CITY_IN_THE_SKY,
    /* 0x3B */ Z2SCENE_CITY_IN_THE_SKY_MINIBOSS,
    /* 0x3C */ Z2SCENE_CITY_IN_THE_SKY_BOSS,
    /* 0x3D */ Z2SCENE_PALACE_OF_TWILIGHT,
    /* 0x3E */ Z2SCENE_PALACE_OF_TWILIGHT_MINIBOSS_A,
    /* 0x3F */ Z2SCENE_PALACE_OF_TWILIGHT_MINIBOSS_B,
    /* 0x40 */ Z2SCENE_PALACE_OF_TWILIGHT_THRONE_ROOM,
    /* 0x41 */ Z2SCENE_PALACE_OF_TWILIGHT_BOSS,
    /* 0x42 */ Z2SCENE_HYRULE_CASTLE,
    /* 0x43 */ Z2SCENE_FINAL_BATTLE_THRONE_ROOM,
    /* 0x44 */ Z2SCENE_FINAL_BATTLE_FIELD,
    /* 0x45 */ Z2SCENE_FINAL_BATTLE_CUTSCENE,
    /* 0x46 */ Z2SCENE_ICE_BLOCK_PUZZLE,
    /* 0x47 */ Z2SCENE_CAVE_OF_ORDEALS,
    /* 0x48 */ Z2SCENE_GORGE_LANTERN_CAVE,
    /* 0x49 */ Z2SCENE_LAKE_LANTERN_CAVE,
    /* 0x4A */ Z2SCENE_ELDIN_MAGNET_CAVE,
    /* 0x4B */ Z2SCENE_GROTTO_GRASS_1,
    /* 0x4C */ Z2SCENE_GROTTO_GRASS_2,
    /* 0x4D */ Z2SCENE_GROTTO_ROCK_1,
    /* 0x4E */ Z2SCENE_GROTTO_ROCK_2,
    /* 0x4F */ Z2SCENE_GROTTO_POND,
    /* 0x50 */ Z2SCENE_FARON_WOODS_CAVE,
};

#endif /* Z2SCENEMGR_H */
