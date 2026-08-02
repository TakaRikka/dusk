#ifndef DUSK_RANDOMIZER_CONTEXT_HPP
#define DUSK_RANDOMIZER_CONTEXT_HPP

#include <dolphin/types.h>

#include <array>
#include <filesystem>
#include <iomanip>
#include <list>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>

/*
 * Class holding all the information necessary for playing
 * the current randomizer seed
 */
class RandomizerContext {
public:
    static constexpr size_t ACTR_CRC_SIZE = 32;
    static constexpr size_t TGSC_CRC_SIZE = 35; // 3 extra bytes for scale x, y, z
    static constexpr size_t OBJ_DELETE_SIZE = 1;
    static constexpr u8 ROOM_STAGE = 0xFF;

    RandomizerContext() = default;

    bool mCreatingSave{false};
    u32 mSeedID{0};
    std::string mHash{""};

    // Maps enum of necessary setting to enum of value
    std::unordered_map<int, int> mSettings{};

    std::list<u16> mStartEventFlags{};
    std::unordered_map<u8, std::list<u8>> mStartRegionFlags{};
    std::list<u8> mStartingInventory{};

    struct itemLocationData{
        int itemId{0xFF};
        int stage{0xFF};
        u16 flag{0xFFFF};
    };

    std::unordered_map<u16, u8> mTreasureChestOverrides{};
    std::unordered_map<u16, u8> mPoeOverrides{};
    std::unordered_map<u16, u8> mFreestandingItemOverrides{};
    std::unordered_map<u8, u8> mBugRewardOverrides{};
    std::unordered_map<u16, u8> mSkyCharacterOverrides{};
    std::unordered_map<u16, u8> mGoldenWolfOverrides{};
    std::unordered_map<u16, u8> mShopOverrides{};
    std::unordered_map<u16, u16> mTwilitInsectOverrides{}; // Just used in tracker for now
    std::unordered_map<u32, itemLocationData> mFlowItemMessageOverrides{};
    std::unordered_map<std::string, itemLocationData> mItemLocations{};

    u8 mStartHour{0};
    u8 mMapBits{};

    std::unordered_map<u32, std::unordered_map<u32, std::vector<u8>>> mObjectPatches{};
    std::unordered_map<u32, std::list<std::vector<u8>>> mObjectAdditions{};
    // std::unordered_map<u32, std::unordered_set<u32>> mTgscDeletions{};
    std::unordered_map<u32, u64> mFlowPatches{};
    std::unordered_map<u32, std::vector<u16>> mFlowPatchesBranchOverrides{};
    std::unordered_map<u32, std::array<u8, 20>> mAttributeOverrides{};

    // struct TextOverride {
    //     std::array<u8, 16> mAttributes{};
    //     std::string mText{};
    // };
    // Map of language -> map of key -> string
    std::unordered_map<int, std::unordered_map<u32, std::string>> mTextOverrides{};

    struct EntranceOverride {
        u8 stageId = 0xFF;
        s8 roomNo = -1;
        s8 pointNo = -1;
        s8 mapLayer = -1;
    };

    // keyed by stageId << 24 | pointNo << 16 | roomNo << 8 | mapLayer
    std::unordered_map<int, EntranceOverride> mEntranceOverrides{};

    // Overrides for returning to spawn. Keyed by stageId
    std::unordered_map<int, EntranceOverride> mReturnToPlaceOverrides{};

    std::optional<std::string> WriteToFile();
    std::optional<std::string> LoadFromHash(const std::string& hash);
    std::filesystem::path GetSeedDataPath() const;

    enum Settings {
        HYRULE_BARRIER_REQUIREMENTS,
        HYRULE_BARRIER_FUSED_SHADOWS,
        HYRULE_BARRIER_MIRROR_SHARDS,
        HYRULE_BARRIER_POE_SOULS,
        HYRULE_BARRIER_HEARTS,
        HYRULE_BARRIER_DUNGEONS,
        HYRULE_BIG_KEY_REQUIREMENTS,
        HYRULE_BIG_KEY_FUSED_SHADOWS,
        HYRULE_BIG_KEY_MIRROR_SHARDS,
        HYRULE_BIG_KEY_POE_SOULS,
        HYRULE_BIG_KEY_HEARTS,
        HYRULE_BIG_KEY_DUNGEONS,
        PALACE_OF_TWILIGHT_REQUIREMENTS,
        TEMPLE_OF_TIME_SWORD_REQUIREMENT,
        SKIP_MINOR_CUTSCENES,
        SKIP_MAJOR_CUTSCENES,
        SKIP_BRIDGE_DONATION,
        MIRROR_CHAMBER_ACCESS,
    };

    enum Options {
        ON,
        OFF,
        NONE,
        VANILLA,
        OPEN,
        FUSED_SHADOWS,
        MIRROR_SHARDS,
        POE_SOULS,
        HEARTS,
        DUNGEONS,
        WOODEN_SWORD,
        ORDON_SWORD,
        MASTER_SWORD,
        LIGHT_SWORD,
        BARRIER,
        CLOSED,
    };

    static int SettingToEnum(const std::string& settingName);

    static int OptionToEnum(const std::string& optionName);
};

/*
 * Class holding seed-agnostic dynamic information about current randomizer play.
 * This gets reset when resetting to the title screen.
 */
class RandomizerState {
public:
    enum TimeChange {
        NO_CHANGE = 0,
        CHANGE_TO_NIGHT,
        CHANGE_TO_DAY,
    };

    enum EventItemStatus {
        QUEUE_EMPTY,
        ITEM_IN_QUEUE,
        CLEAR_QUEUE,
    };

    static constexpr u8 EVENT_ITEM_QUEUE_SIZE = 10;

    RandomizerState() {mInitialized = false;}

    int _create();
    int _delete();
    int execute();
    int draw();
    void addItemToEventQueue(u8 item);
    void initGiveItemToPlayer();
    //void handleBonkDamage();
    void handleTimeOfDayChange();
    void handleTimeSpeed();
    void offLoad();

    void handlePoeItem(u8 bitSw);

    u8 getGiveItemToPlayerStatus() const { return mEventItemStatus;}
    u8 getTimeChange() const { return mTimeChange; }
    bool getRoomReloadingState() const { return mRoomReloadingState; }
    bool getHasPendingToDChange() const { return mHasPendingToDChange; }

    void setGiveItemToPlayerStatus(u8 status) { mEventItemStatus = status;}
    void setHasPendingToDChange(bool hasPending) { mHasPendingToDChange = hasPending; }
    void setTimeChange(u8 newTimeChange) { mTimeChange = newTimeChange; }
    void setRoomReloadingState(bool newState) { mRoomReloadingState = newState; }

    bool mInitialized{false};
    int mFileNum{-1};
    u8 mEventItemStatus{};
    bool mHasPendingToDChange{false};
    u8 mTimeChange{};
    u8 mEventItemQueue[EVENT_ITEM_QUEUE_SIZE];
    bool mRoomReloadingState{false};

    // Used to store an item id for a flow message override so that we can give the item
    // once the textbox is closed instead of when the message appears. This lines up
    // more naturally with the timing of when the game normally gives items and affects
    // things like the sound of the rupee counter going up.
    u8 mFlowMessageItemId{0};

    int mFoolishItemCount{0};
    bool mUpdateTracker{false};
    bool mShowTracker{false};
    u16 mTrackerTempEventFlag{0};
    struct {
        int stage{-1};
        int flag{-1};
    } mTrackerTempSwitchFlag;
    struct {
        int stage{-1};
        int flag{-1};
    } mTrackerTempItemFlag;
};

extern RandomizerState g_randomizerState;

RandomizerContext& randomizer_GetContext();

bool randomizer_IsActive();

int randomizer_getItemAtLocation(const std::string& locationName);

/*
 * @brief Overrides the given entrance paramaters if an override exists for them
 */
void randomizer_checkAndOverrideEntranceData(const char*& i_Name, s8& i_RoomNo, s16& i_Point, s8& i_Layer);
/*
 * @brief Puts the associated flag into the randomizer state's temporary flag
 * variable. This allows the tracker/Archipelago to know a location has been checked
 * when the item is received instead of some indeterminate amount of time afterward.
 */
void randomizer_setTempFlagForLocation(const std::string& locationName);

void randomizer_setTempFlagForFLWOverride(u32 key);

bool randomizer_checkTempleOfTimeRequirement();

bool randomizer_mirrorChamberWallShouldExist();

void randomizer_returnToSpawn(bool tryDungeon);

u8 randomizer_getRandomFoolishItemModelID();

/**
 * Helper function to convert raw bytes of a container to a hex string
 */
template <typename T>
std::string ContainerToHexString(const T& container, bool includePrefix = true) {
    std::ostringstream oss;

    if (includePrefix) {
        oss << "0x";
    }

    // Get the raw byte pointer to the start of the data
    const auto* rawBytes = reinterpret_cast<const u8*>(container.data());

    // Calculate total byte size (number of elements * size of each element)
    size_t totalBytes = container.size() * sizeof(typename T::value_type);

    oss << std::hex << std::setfill('0') << std::uppercase;

    for (size_t i = 0; i < totalBytes; ++i) {
        // static_cast to not u8 is necessary so oss treats it as a number, not a char
        oss << std::setw(2) << static_cast<u16>(rawBytes[i]);
    }

    return oss.str();
}

/**
 * Helper function to convert hex string to raw bytes
 */
std::vector<u8> HexToBytes(std::string hex);

/*
 * Gets the current stage id, room no, and layer no in the format for a key in mActorPatches
 */
u32 getActorPatchesCurrentStageKey(u8 roomNo);

/*
 * Gets the CRC32 hash of an actors name, parameters, position, and angle
 */
u32 getStageObjCRC32(u8* data, size_t size);

/*
 * Generates a seed and writes the necessary seed files to the players seed directory
 * Returns true if generation was successful, false otherwise.
 */
bool GenerateAndWriteSeed(std::string& generationStatusMsg);

#endif //DUSK_RANDOMIZER_CONTEXT_HPP
