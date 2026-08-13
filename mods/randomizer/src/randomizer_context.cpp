#include "randomizer_context.hpp"

#include "session.hpp"
#include "paths.hpp"
#include "flags.h"
#include "tools.h"
#include "stages.h"
#include "verify_item_functions.h"
#include "item_ids.h"
#include "../generator/utility/crc32.hpp"
#include "../generator/utility/endian.hpp"
#include "../generator/utility/yaml.hpp"
#include "../generator/randomizer.hpp"
#include "../generator/utility/text.hpp"
#include "../generator/utility/string.hpp"

#include <fstream>
#include <mods/svc/log.hpp>

#include "custom_flow_ids.hpp"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_class.h"
#include "d/d_msg_flow.h"
#include "m_Do/m_Do_audio.h"

std::optional<std::string> RandomizerContext::WriteToFile() {

    std::ofstream seedData(this->GetSeedDataPath());
    if (!seedData.is_open()) {
        return "Could not open seed data file";
    }

    YAML::Node out{};

    for (const auto& [setting, option] : this->mSettings) {
        out["mSettings"][setting] = option;
    }

    // NOTE: When dumping u8s, they must be converted to u16s (or higher), otherwise they get dumped
    // as single characters and not numbers

    out["mStartEventFlags"] = this->mStartEventFlags;
    for (const auto& [region, flags] : this->mStartRegionFlags) {
        const std::list<u16> u16Flags(flags.begin(), flags.end());
        out["mStartRegionFlags"][static_cast<u16>(region)] = u16Flags;
    }

    const std::list<u16> u16Inventory(this->mStartingInventory.begin(), this->mStartingInventory.end());
    out["mStartingInventory"] = u16Inventory;

    const std::unordered_map<u16, u16> u16ChestOverrides(this->mTreasureChestOverrides.begin(), this->mTreasureChestOverrides.end());
    out["mTreasureChestOverrides"] = u16ChestOverrides;

    const std::unordered_map<u16, u16> u16PoeOverrides(this->mPoeOverrides.begin(), this->mPoeOverrides.end());
    out["mPoeOverrides"] = u16PoeOverrides;

    const std::unordered_map<u16, u16> u16FreestandingItemOverrides(this->mFreestandingItemOverrides.begin(), this->mFreestandingItemOverrides.end());
    out["mFreestandingItemOverrides"] = u16FreestandingItemOverrides;

    const std::unordered_map<u16, u16> u16BugRewardOverrides(this->mBugRewardOverrides.begin(), this->mBugRewardOverrides.end());
    out["mBugRewardOverrides"] = u16BugRewardOverrides;

    const std::unordered_map<u16, u16> u16SkyCharacterOverrides(this->mSkyCharacterOverrides.begin(), this->mSkyCharacterOverrides.end());
    out["mSkyCharacterOverrides"] = u16SkyCharacterOverrides;

    const std::unordered_map<u16, u16> u16GoldenWolfOverrides(this->mGoldenWolfOverrides.begin(), this->mGoldenWolfOverrides.end());
    out["mGoldenWolfOverrides"] = u16GoldenWolfOverrides;

    const std::unordered_map<u16, u16> u16ShopOverrides(this->mShopOverrides.begin(), this->mShopOverrides.end());
    out["mShopOverrides"] = u16ShopOverrides;

    out["mTwilitInsectOverrides"] = mTwilitInsectOverrides;

    for (const auto& [key, data] : this->mFlowItemMessageOverrides) {
        auto node = out["mFlowItemMessageOverrides"][key];
        node["itemId"] = data.itemId;
        node["stage"] = data.stage;
        node["flag"] = data.flag;
    }

    for (const auto& [name, data] : this->mItemLocations) {
        auto node = out["mItemLocations"][name];
        node["itemId"] = data.itemId;
        node["stage"] = data.stage;
        node["flag"] = data.flag;
    }

    out["mStartHour"] = static_cast<u16>(this->mStartHour);
    out["mMapBits"] = static_cast<u16>(this->mMapBits);

    for (const auto& [stageRoomLayer, actorPatches] : this->mObjectPatches) {
        for (const auto& [actorCRC, actorPatch] : actorPatches) {
            out["mObjectPatches"][stageRoomLayer][actorCRC] = ContainerToHexString(actorPatch);
        }
    }

    for (const auto& [stageRoomLayer, newActors] : this->mObjectAdditions) {
        for (const auto& actor : newActors) {
            out["mObjectAdditions"][stageRoomLayer].push_back(ContainerToHexString(actor));
        }
    }


    out["mFlowPatches"] = this->mFlowPatches;

    for (const auto& [key, branchOverrides]: this->mFlowPatchesBranchOverrides) {
        for (auto override : branchOverrides) {
            out["mFlowPatchesBranchOverrides"][key].push_back(override);
        }
    }

    // Dump text overrides as binary to avoid losing intentional null characters
    YAML::Emitter textData;
    textData << YAML::BeginMap;
    textData << YAML::Key << "mTextOverrides";
    textData << YAML::BeginMap;
    for (auto language : randomizer::supportedLanguages) {
        auto languageStr = randomizer::languageToString(language);
        textData << YAML::Key << languageStr;
        textData << YAML::BeginMap;
        for (const auto& [key, text] : this->mTextOverrides[language]) {
            textData << YAML::Key << key;
            textData << YAML::Value << YAML::Binary(reinterpret_cast<const unsigned char*>(text.data()), text.size());
        }
        textData << YAML::EndMap;
    }
    textData << YAML::EndMap;
    textData << YAML::EndMap;

    for (const auto& [key, override] : mAttributeOverrides) {
        out["mAttributeOverrides"][key] = ContainerToHexString(override);
    }

    for (const auto& [key, override] : mEntranceOverrides) {
        out["mEntranceOverrides"][key] = std::bit_cast<int>(override);
    }

    for (const auto& [key, override] : mReturnToPlaceOverrides) {
        out["mReturnToPlaceOverrides"][key] = std::bit_cast<int>(override);
    }

    seedData << YAML::Dump(out);
    seedData << '\n' << textData.c_str();
    seedData.close();

    return std::nullopt;
}

std::optional<std::string> RandomizerContext::LoadFromHash(const std::string& hash) {
    this->mHash = hash;

    if (!std::filesystem::exists(this->GetSeedDataPath())) {
        mods::log::error("Failed to load Hash: {}", hash);
        mHash.clear();
        return std::nullopt;
    }

    auto in = LoadYAML(this->GetSeedDataPath());

    // Necessary settings
    for (const auto& settingNode : in["mSettings"] ) {
        const auto& setting = settingNode.first.as<int>();
        const auto& option = settingNode.second.as<int>();
        this->mSettings[setting] = option;
    }

    // Event flags
    for (const auto& flag : in["mStartEventFlags"]) {
        this->mStartEventFlags.push_back(flag.as<u16>());
    }
    // Region Flags
    for (const auto& regionNode : in["mStartRegionFlags"]) {
        const auto& regionId = regionNode.first.as<u8>();
        for (const auto& flag : regionNode.second) {
            this->mStartRegionFlags[regionId].push_back(flag.as<u8>());
        }
    }

    // Starting inventory
    for (const auto& itemId : in["mStartingInventory"]) {
        this->mStartingInventory.push_back(itemId.as<u8>());
    }

    // Chest overrides
    for (const auto& chestNode : in["mTreasureChestOverrides"]) {
        u16 key = chestNode.first.as<u16>();
        u8 itemId = chestNode.second.as<u8>();
        this->mTreasureChestOverrides[key] = itemId;
    }

    // Poe Overrides
    for (const auto& poeNode : in["mPoeOverrides"]) {
        u16 key = poeNode.first.as<u16>();
        u8 itemId = poeNode.second.as<u8>();
        this->mPoeOverrides[key] = itemId;
    }

    // Freestanding overrides
    for (const auto& itemNode : in["mFreestandingItemOverrides"]) {
        u16 key = itemNode.first.as<u16>();
        u8 itemId = itemNode.second.as<u8>();
        this->mFreestandingItemOverrides[key] = itemId;
    }

    // Bug Rewards
    for (const auto& bugNode : in["mBugRewardOverrides"]) {
        u8 bugItemId = bugNode.first.as<u8>();
        u8 itemId = bugNode.second.as<u8>();
        this->mBugRewardOverrides[bugItemId] = itemId;
    }

    // Sky Characters
    for (const auto& skyCharacterNode : in["mSkyCharacterOverrides"]) {
        u16 key = skyCharacterNode.first.as<u16>();
        u8 itemId = skyCharacterNode.second.as<u8>();
        this->mSkyCharacterOverrides[key] = itemId;
    }

    // Golden Wolves
    for (const auto& goldenWolfNode : in["mGoldenWolfOverrides"]) {
        u16 key = goldenWolfNode.first.as<u16>();
        u8 itemId = goldenWolfNode.second.as<u8>();
        this->mGoldenWolfOverrides[key] = itemId;
    }

    // Shop Items
    for (const auto& shopNode : in["mShopOverrides"]) {
        u16 key = shopNode.first.as<u16>();
        u8 itemId = shopNode.second.as<u8>();
        this->mShopOverrides[key] = itemId;
    }

    for (const auto& twilitInsectNode : in["mTwilitInsectOverrides"]) {
        u16 key = twilitInsectNode.first.as<u16>();
        u16 itemId = twilitInsectNode.second.as<u16>();
        this->mTwilitInsectOverrides[key] = itemId;
    }

    // Helper function for getting the item data out of a YAML node
    auto retrieveItemData = [](auto& itemData, const YAML::Node& node) {
        itemData.itemId = node["itemId"].as<int>();
        itemData.stage = node["stage"].as<int>();
        itemData.flag = node["flag"].as<u16>();
    };

    // FLW Override items
    for (const auto& flwNode : in["mFlowItemMessageOverrides"]) {
        u32 key = flwNode.first.as<u32>();
        retrieveItemData(this->mFlowItemMessageOverrides[key], flwNode.second);
    }

    // Items we call by location name
    for (const auto& locationNode : in["mItemLocations"]) {
        const auto& locationName = nameLookupOverride(locationNode.first.as<std::string>());
        retrieveItemData(this->mItemLocations[locationName], locationNode.second);
    }

    // Starting hour
    this->mStartHour = in["mStartHour"].as<u8>();
    // Starting map bits
    this->mMapBits = in["mMapBits"].as<u8>();

    // Object Patches
    for (const auto& stageRoomLayerNode: in["mObjectPatches"]) {
        u32 stageRoomLayer = stageRoomLayerNode.first.as<u32>();
        for (const auto& actorPatchNode : stageRoomLayerNode.second) {
            u32 actorCRC = actorPatchNode.first.as<u32>();
            this->mObjectPatches[stageRoomLayer][actorCRC] = HexToBytes(actorPatchNode.second.as<std::string>());
        }
    }

    // Object Additions
    for (const auto& stageNode: in["mObjectAdditions"]) {
        u32 stageRoomLayer = stageNode.first.as<u32>();
        for (const auto& objectData : stageNode.second) {
            this->mObjectAdditions[stageRoomLayer].emplace_back(HexToBytes(objectData.as<std::string>()));
        }
    }

    // Flow Patches
    for (const auto& flowNode: in["mFlowPatches"]) {
        auto key = flowNode.first.as<u32>();
        auto value = flowNode.second.as<u64>();
        this->mFlowPatches[key] = value;
    }

    // Flow Patch Branch Overrides
    for (const auto& flowNode : in["mFlowPatchesBranchOverrides"]) {
        auto key = flowNode.first.as<u32>();
        for (const auto& branchNode : flowNode.second) {
            auto override = branchNode.as<u16>();
            this->mFlowPatchesBranchOverrides[key].push_back(override);
        }
    }

    // Text Overrides
    for (const auto& languageNode: in["mTextOverrides"]) {
        const auto& languageStr = languageNode.first.as<std::string>();
        auto language = randomizer::stringToLanguage(languageStr);
        for (const auto& textNode : languageNode.second) {
            auto key = textNode.first.as<u32>();
            auto binary = textNode.second.as<YAML::Binary>();
            std::string text(reinterpret_cast<const char*>(binary.data()), binary.size());
            this->mTextOverrides[language][key] = std::move(text);
        }
    }

    // Attribute Overrides
    for (const auto& attributeNode : in["mAttributeOverrides"]) {
        auto key = attributeNode.first.as<u32>();
        std::vector<u8> overrideVec = HexToBytes(attributeNode.second.as<std::string>());
        std::array<u8, 20> override{};
        std::copy(overrideVec.begin(), overrideVec.end(), override.begin());
        this->mAttributeOverrides[key] = override;
    }

    // Entrance Overrides
    for (const auto& entranceNode : in["mEntranceOverrides"]) {
        auto key = entranceNode.first.as<int>();
        auto override = std::bit_cast<EntranceOverride>(entranceNode.second.as<int>());
        this->mEntranceOverrides[key] = override;
    }

    // Return to Place Overrides
    for (const auto& entranceNode : in["mReturnToPlaceOverrides"]) {
        auto key = entranceNode.first.as<int>();
        auto override = std::bit_cast<EntranceOverride>(entranceNode.second.as<int>());
        this->mReturnToPlaceOverrides[key] = override;
    }

    UiToastDesc desc = UI_TOAST_DESC_INIT;
    desc.type = "success";
    desc.title_rml = "Randomizer";
    std::string body_text = fmt::format("Loaded Randomizer Seed {}", this->mHash);
    desc.body_rml = body_text.c_str();
    desc.duration_ms = 3000;
    randomizer::session::svc_mng.ui->push_toast(randomizer::session::svc_mng.mod_ctx, &desc);

    return std::nullopt;
}

std::filesystem::path RandomizerContext::GetSeedDataPath() const {
    return ::randomizer::paths::GetRandomizerSeedsPath() / this->mHash / "seed.dat";
}

int RandomizerContext::SettingToEnum(const std::string& settingName) {
    static const std::map<std::string, int> nameToEnum = {
        {"Hyrule Barrier Dungeons", HYRULE_BARRIER_DUNGEONS},
        {"Hyrule Barrier Requirements", HYRULE_BARRIER_REQUIREMENTS},
        {"Hyrule Barrier Fused Shadows", HYRULE_BARRIER_FUSED_SHADOWS},
        {"Hyrule Barrier Mirror Shards", HYRULE_BARRIER_MIRROR_SHARDS},
        {"Hyrule Castle Big Key Requirements", HYRULE_BIG_KEY_REQUIREMENTS},
        {"Hyrule Barrier Poe Souls", HYRULE_BARRIER_POE_SOULS},
        {"Hyrule Barrier Hearts", HYRULE_BARRIER_HEARTS},
        {"Hyrule Castle Big Key Mirror Shards", HYRULE_BIG_KEY_MIRROR_SHARDS},
        {"Hyrule Castle Big Key Fused Shadows", HYRULE_BIG_KEY_FUSED_SHADOWS},
        {"Hyrule Castle Big Key Dungeons", HYRULE_BIG_KEY_DUNGEONS},
        {"Hyrule Castle Big Key Poe Souls", HYRULE_BIG_KEY_POE_SOULS},
        {"Hyrule Castle Big Key Hearts", HYRULE_BIG_KEY_HEARTS},
        {"Palace of Twilight Requirements", PALACE_OF_TWILIGHT_REQUIREMENTS},
        {"Temple of Time Sword Requirement", TEMPLE_OF_TIME_SWORD_REQUIREMENT},
        {"Skip Minor Cutscenes", SKIP_MINOR_CUTSCENES},
        {"Skip Major Cutscenes", SKIP_MAJOR_CUTSCENES},
        {"Skip Bridge Donation", SKIP_BRIDGE_DONATION},
        {"Mirror Chamber Access", MIRROR_CHAMBER_ACCESS},
    };

    if (nameToEnum.contains(settingName)) {
        return nameToEnum.at(settingName);
    }

    return -1;
}

int RandomizerContext::OptionToEnum(const std::string& optionName) {
    static const std::map<std::string, int> nameToEnum = {
        {"On", ON},
        {"Off", OFF},
        {"None", NONE},
        {"Vanilla", VANILLA},
        {"Open", OPEN},
        {"Fused Shadows", FUSED_SHADOWS},
        {"Mirror Shards", MIRROR_SHARDS},
        {"Poe Souls", POE_SOULS},
        {"Hearts", HEARTS},
        {"Dungeons", DUNGEONS},
        {"Wooden Sword", WOODEN_SWORD},
        {"Ordon Sword", ORDON_SWORD},
        {"Master Sword", MASTER_SWORD},
        {"Light Sword", LIGHT_SWORD},
        {"Closed", CLOSED},
        {"Barrier", BARRIER},
    };

    if (nameToEnum.contains(optionName)) {
        return nameToEnum.at(optionName);
    }

    return -1;
}

RandomizerState g_randomizerState;

int RandomizerState::_create() {
    mInitialized = true;
    mEventItemStatus = QUEUE_EMPTY;
    mHasPendingToDChange = false;
    // g_customMenuRing._initialize();
    for (int i = 0; i < EVENT_ITEM_QUEUE_SIZE; i++) {
        mEventItemQueue[i] = 0;
    }
    return 1;
}

int RandomizerState::_delete() {
    mInitialized = false;
    return 1;
}

static bool checkFoolishItemEffectReady()
{
    // Verify Link is loaded on the map.
    if (!daAlink_getAlinkActorClass())
    {
        return false;
    }

    // Ensure Link is not in a cutscene
    if (daAlink_getAlinkActorClass()->checkEventRun())
    {
        return false;
    }

    // Make sure Link isn't riding anything
    if (daAlink_getAlinkActorClass()->checkRide())
    {
        return false;
    }

    // Ensure there are pointers to the mMeterClass and mpMeterDraw structs
    if (!dMeter2Info_getMeterClass())
    {
        return false;
    }

    if (!dMeter2Info_getMeterClass()->getMeterDrawPtr())
    {
        return false;
    }

    // Make sure Z button isn't dimmed
    if (dMeter2Info_getMeterClass()->getMeterDrawPtr()->getButtonZAlpha() != 1.f)
    {
        return false;
    }

    switch (daAlink_getAlinkActorClass()->mProcID)
    {
        case daAlink_c::PROC_TALK:
        case daAlink_c::PROC_WOLF_SWIM_MOVE:
        case daAlink_c::PROC_SWIM_MOVE:
        case daAlink_c::PROC_SWIM_WAIT:
        case daAlink_c::PROC_WOLF_SWIM_WAIT:
        case daAlink_c::PROC_SWIM_UP:
        case daAlink_c::PROC_SWIM_DIVE:
        {
            return false;
        }
        default:
        {
            break;
        }
    }
    return true;
}

static void handleFoolishItem() {
    u32 count = g_randomizerState.mFoolishItemCount;
    if (count == 0) {
        return;
    }

    if (!checkFoolishItemEffectReady())
    {
        return;
    }

    // Failsafe: Make sure the count does not somehow exceed 100
    if (count > 100) {
        count = 100;
    }

    // Reset count
    g_randomizerState.mFoolishItemCount = 0;

    /* Store the currently loaded sound wave to local variables as we will need to load them back later.
     * We use this method because if we just loaded the sound waves every time the item was gotten, we'd
     * eventually run out of memory so it is safer to unload everything and load it back in. */

    auto sceneMgr = Z2GetSceneMgr();
    const u32 seWave1 = Z2AudioMgr::getInterface()->loadedSeWave_1;
    const u32 seWave2 = Z2AudioMgr::getInterface()->loadedSeWave_2;
    sceneMgr->eraseSeWave(seWave1);
    sceneMgr->eraseSeWave(seWave2);
    sceneMgr->loadSeWave(0x46);
    mDoAud_seStartLevel(0x10040, nullptr, 0, 0);
    sceneMgr->loadSeWave(seWave1);
    sceneMgr->loadSeWave(seWave2);

    // Initiate the appropriate visual damage process
    if (daAlink_getAlinkActorClass()->checkWolf())
    {
        daAlink_getAlinkActorClass()->procWolfDamageInit(nullptr);
    }
    else
    {
        daAlink_getAlinkActorClass()->procDamageInit(nullptr, 0);
    }

    daPy_py_c::setPlayerDamage(count, TRUE);
}

/*
 * Updates flags for Hyrule Castle Barrier, Palace of Twilight Access,
 * and Hyrule Castle Big Key chest. Maybe a bit overkill to check this every frame, but
 * it keeps it all in one place for now.
 */
static void updateGoalFlags() {
    auto& settings = randomizer_GetContext().mSettings;

    // Hyrule Castle Barrier
    if (!dComIfGs_isEventBit(BARRIER_GONE)) {
        bool destroyBarrier = false;
        switch (settings[RandomizerContext::HYRULE_BARRIER_REQUIREMENTS]) {
        case RandomizerContext::VANILLA:
            destroyBarrier = dComIfGs_isEventBit(PALACE_OF_TWILIGHT_CLEARED);
            break;
        case RandomizerContext::FUSED_SHADOWS:
            destroyBarrier = numFusedShadows() >= settings[RandomizerContext::HYRULE_BARRIER_FUSED_SHADOWS];
            break;
        case RandomizerContext::MIRROR_SHARDS:
            destroyBarrier = numMirrorShards() >= settings[RandomizerContext::HYRULE_BARRIER_MIRROR_SHARDS];
            break;
        case RandomizerContext::DUNGEONS:
            destroyBarrier = numCompletedDungeons() >= settings[RandomizerContext::HYRULE_BARRIER_DUNGEONS];
            break;
        case RandomizerContext::POE_SOULS:
            destroyBarrier = dComIfGs_getPohSpiritNum() >= settings[RandomizerContext::HYRULE_BARRIER_POE_SOULS];
            break;
        case RandomizerContext::HEARTS:
            destroyBarrier = dComIfGs_getMaxLife() >= 5 * settings[RandomizerContext::HYRULE_BARRIER_HEARTS];
            break;
        default:
            break;
        }

        if (destroyBarrier) {
            dComIfGs_onEventBit(BARRIER_GONE);
        }
    }

    // Hyrule Castle Big Key Gate
    if (!dComIfGs_isStageSwitch(0x18, 0x4B)) {
        bool openGate = false;
        switch (settings[RandomizerContext::HYRULE_BIG_KEY_REQUIREMENTS]) {
        case RandomizerContext::FUSED_SHADOWS:
            openGate = numFusedShadows() >= settings[RandomizerContext::HYRULE_BIG_KEY_FUSED_SHADOWS];
            break;
        case RandomizerContext::MIRROR_SHARDS:
            openGate = numMirrorShards() >= settings[RandomizerContext::HYRULE_BIG_KEY_MIRROR_SHARDS];
            break;
        case RandomizerContext::DUNGEONS:
            openGate = numCompletedDungeons() >= settings[RandomizerContext::HYRULE_BIG_KEY_DUNGEONS];
            break;
        case RandomizerContext::POE_SOULS:
            openGate = dComIfGs_getPohSpiritNum() >= settings[RandomizerContext::HYRULE_BIG_KEY_POE_SOULS];
            break;
        case RandomizerContext::HEARTS:
            openGate = dComIfGs_getMaxLife() >= 5 * settings[RandomizerContext::HYRULE_BIG_KEY_HEARTS];
            break;
        default:
            break;
        }

        if (openGate) {
            dComIfGs_onStageSwitch(0x18, 0x4B);
        }
    }

    // Palace of Twilight Access
    if (!dComIfGs_isEventBit(FIXED_THE_MIRROR_OF_TWILIGHT)) {
        bool openPalace = false;
        switch (settings[RandomizerContext::PALACE_OF_TWILIGHT_REQUIREMENTS]) {
        case RandomizerContext::VANILLA:
            openPalace = dComIfGs_isEventBit(CITY_IN_THE_SKY_CLEARED);
            break;
        case RandomizerContext::FUSED_SHADOWS:
            openPalace = numFusedShadows() >= 3;
            break;
        case RandomizerContext::MIRROR_SHARDS:
            openPalace = numMirrorShards() >= 4;
            break;
        default:
            break;
        }

        if (openPalace) {
            dComIfGs_onEventBit(FIXED_THE_MIRROR_OF_TWILIGHT);
        }
    }
}

int RandomizerState::execute() {
    if (!mInitialized) {
        return 0;
    }

    // Always check for and handle time of day changes
    if (getTimeChange() != NO_CHANGE) {
        handleTimeSpeed();
    }

    bool currentReloadingState;
    // Any custom functionality that relies on Link's actor being on a stage
    if (daAlink_getAlinkActorClass()) {
        currentReloadingState = daAlink_getAlinkActorClass()->checkRestartRoom();
        // Handle giving item to the player at any time.
        initGiveItemToPlayer();
    }
    else {
        currentReloadingState = true;
    }

    bool prevReloadingState = getRoomReloadingState();
    if (!currentReloadingState) {
        if (prevReloadingState) {
            offLoad();
        }
    }
    setRoomReloadingState(currentReloadingState);

    if (getStageID() != Title_Screen) {
        handleFoolishItem();
    }

    return 1;
}

int RandomizerState::draw() {
    return 1;
}

void RandomizerState::handlePoeItem(u8 bitSw)
{
    u16 key = getStageID() << 8 | bitSw;
    u8 item = randomizer_GetContext().mPoeOverrides[key];
    addItemToEventQueue(item);
    daAlink_getAlinkActorClass()->procWolfAtnActorMoveInit();
}

void RandomizerState::addItemToEventQueue(u8 item)
{
    for (int i = 0; i < EVENT_ITEM_QUEUE_SIZE; i++)
    {
        if (mEventItemQueue[i] == 0)
        {
            mEventItemQueue[i] = item;
            break;
        }
    }
}

void RandomizerState::initGiveItemToPlayer()
{
    switch (daAlink_getAlinkActorClass()->mProcID)
    {
        case daAlink_c::PROC_WAIT:
        case daAlink_c::PROC_TIRED_WAIT:
        case daAlink_c::PROC_MOVE:
        case daAlink_c::PROC_WOLF_WAIT:
        case daAlink_c::PROC_WOLF_TIRED_WAIT:
        case daAlink_c::PROC_WOLF_MOVE:
        case daAlink_c::PROC_ATN_MOVE:
        case daAlink_c::PROC_WOLF_ATN_AC_MOVE:
        {
            // Check if link is currently in a cutscene
            if (daAlink_getAlinkActorClass()->checkEventRun())
            {
                break;
            }

            // Ensure that link is not currently in a message-based event.
            int event_item_id = 0;
            if (daAlink_getAlinkActorClass()->mMsgFlow.getEventId(&event_item_id) != 0)
            {
                break;
            }

            u8 itemToGive = 0xFF;

            for (int i = 0; i < EVENT_ITEM_QUEUE_SIZE; i++)
            {
                const u8 storedItem = mEventItemQueue[i];

                if (storedItem)
                {
                    const u8 giveItemToPlayerStatus = getGiveItemToPlayerStatus();

                    // If we have the call to clear the queue, then we want to clear the item and break out.
                    if (giveItemToPlayerStatus == CLEAR_QUEUE)
                    {
                        mEventItemQueue[i] = 0;
                        setGiveItemToPlayerStatus(QUEUE_EMPTY);
                        break;
                    }

                    // If the queue is empty and we have an item to give, update the queue state.
                    else if (giveItemToPlayerStatus == QUEUE_EMPTY)
                    {
                        setGiveItemToPlayerStatus(ITEM_IN_QUEUE);
                    }

                    itemToGive = verifyProgressiveItem(storedItem);
                    break;
                }
            }

            // if there is no item to give, break out of the case.
            if (itemToGive == 0xFF)
            {
                break;
            }

            g_dComIfG_gameInfo.play.getEvent()->setGtItm(itemToGive);

            // Set the process value for getting an item to start the "get item" cutscene when next available.
            daAlink_getAlinkActorClass()->mProcID = daAlink_c::PROC_GET_ITEM;

            //  Get the event index for the "Get Item" event.
            const s16 eventIdx = dComIfGp_getEventManager().getEventIdx((fopAc_ac_c*)daAlink_getAlinkActorClass(),"DEFAULT_GETITEM",0xFF);

            // Finally we want to modify the event stack to prioritize our custom event so that it happens next.
            fopAcM_orderChangeEventId(daAlink_getAlinkActorClass(), eventIdx, 1, 0xFFFF);
        }
        default:
        {
            break;
        }
    }
}

void RandomizerState::handleTimeOfDayChange()
{
    if (dComIfGp_roomControl_getTimePass())
    {
        // No point in changing values if we are already changing the time.
        if (getTimeChange() == NO_CHANGE)
        {
            if (!dKy_daynight_check()) // Day time
            {
                setTimeChange(CHANGE_TO_NIGHT);
            }
            else
            {
                setTimeChange(CHANGE_TO_DAY);
            }
            g_env_light.time_change_rate = 1.f; // Increase time speed
        }
    }
    else
    {
        if (!dKy_daynight_check()) // Day time
        {
            dComIfGs_setTime(285.f);
        }
        else
        {
            dComIfGs_setTime(105.f);
        }

        static_cast<dStage_nextStage_c*>(dComIfGp_getNextStartStage())->onEnable();
    }
}

void RandomizerState::handleTimeSpeed()
{

    if (!dKy_daynight_check()) // Day time
    {
        if (getTimeChange() == CHANGE_TO_DAY)
        {
            g_env_light.time_change_rate = 0.012f; // Set time speed to normal
            setTimeChange(NO_CHANGE);
        }
    }
    else if (getTimeChange() == CHANGE_TO_NIGHT)
    {
        g_env_light.time_change_rate = 0.012f; // Set time speed to normal
        setTimeChange(NO_CHANGE);
    }
}

void RandomizerState::offLoad()
{
    if ((getStageID() == City_in_the_Sky) && (dStage_roomControl_c::mStayNo == 0) && (dComIfGp_getStartStagePoint() == 3))
    {
        // Fan in the main room active
        dComIfGs_offSaveSwitch(0xA);

        // Main Room 1F explored
        dComIfGs_offSaveSwitch(0xF);
    }

    if (playerIsInRoomStage(1, allStages[Sacred_Grove]))
    {
        // If the portal in SG isn't active then we want to spawn the shadow beasts.
        if (!dComIfGs_isSaveSwitch(0x64))
        {
            dComIfGs_onSvOneZoneSwitch(0, 0xE);
        }
    }

    if ((getStageID() == Ordon_Ranch) && (dComIfGp_getStartStagePoint() == 1))
    {
        // Clear the danBit that starts a conversation when entering the ranch so the player can do goats as needed.
        dComIfGs_offSaveDunSwitch(0x0);
    }

    // Check and update our goal flags
    updateGoalFlags();
}

RandomizerContext& randomizer_GetContext() {
    static RandomizerContext instance;
    return instance;
}

bool randomizer_IsActive() {
    return (!playerIsOnTitleScreen() || randomizer_GetContext().mCreatingSave) && !randomizer_GetContext().mHash.empty();
}

std::vector<u8> HexToBytes(std::string hex) {
    std::vector<u8> bytes;
    // Strip "0x" if present
    if (hex.substr(0, 2) == "0x") hex = hex.substr(2);

    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        u8 byte = static_cast<u8>(strtol(byteString.c_str(), nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

int randomizer_getItemAtLocation(const std::string& locationName) {
    return randomizer_GetContext().mItemLocations[nameLookupOverride(locationName)].itemId;
}

void randomizer_checkAndOverrideEntranceData(const char*& stageName, s8& roomNo, s16& pointNo, s8& mapLayer) {
    RandomizerContext::EntranceOverride override = {
        static_cast<u8>(getStageID(stageName)), roomNo, static_cast<s8>(pointNo), mapLayer
    };

    int key = std::bit_cast<int>(override);
    if (randomizer_GetContext().mEntranceOverrides.contains(key)) {
        auto& newOverride = randomizer_GetContext().mEntranceOverrides[key];
        stageName = allStages[newOverride.stageId];
        pointNo = newOverride.pointNo;
        roomNo = newOverride.roomNo;
        mapLayer = newOverride.mapLayer;
    }
}

void randomizer_setTempFlag(RandomizerContext::itemLocationData data) {
    // If stage is 0xFF, then this is an event flag
    if (data.stage == 0xFF) {
        g_randomizerState.mTrackerTempEventFlag = data.flag;
    }
    // If it's less than 0x80 then it's a switch flag
    else if (data.flag < 0x80) {
        g_randomizerState.mTrackerTempSwitchFlag.stage = getStageSaveId(data.stage);
        g_randomizerState.mTrackerTempSwitchFlag.flag = data.flag;
    }
    // Otherwise it's an item flag. Currently, any item flags that go through here are custom
    // so we just set the bit directly.
    else {
        dComIfGs_onItem(data.flag, getStageSaveId(data.stage));
    }
}

void randomizer_setTempFlagForLocation(const std::string& locationName) {
    randomizer_setTempFlag(randomizer_GetContext().mItemLocations[nameLookupOverride(locationName)]);
}

void randomizer_setTempFlagForFLWOverride(u32 key) {
    randomizer_setTempFlag(randomizer_GetContext().mFlowItemMessageOverrides[key]);
}

bool randomizer_checkTempleOfTimeRequirement() {
    auto swordRequirement = randomizer_GetContext().mSettings[RandomizerContext::TEMPLE_OF_TIME_SWORD_REQUIREMENT];
    u8 roomNo = dComIfGp_getStartStageRoomNo();

    // Don't strike the pedestal again if we've already set the flag for striking it
    if (roomNo == 1 && dComIfGs_isSwitch(0x63, roomNo)) {
        return false;
    }

    // Make sure we have a sword in Link's hands.
    auto equippedSword = dComIfGs_getSelectEquipSword();
    if (equippedSword != 0xFF) {
        // Fallthrough is intentional to check each potential sword requirement below the current equipped sword
        switch (equippedSword) {
        case dItemNo_LIGHT_SWORD_e:
            if (swordRequirement == RandomizerContext::LIGHT_SWORD) {
                return true;
            }
        case dItemNo_MASTER_SWORD_e:
            if (swordRequirement == RandomizerContext::MASTER_SWORD) {
                return true;
            }
        case dItemNo_SWORD_e:
            if (swordRequirement == RandomizerContext::ORDON_SWORD) {
                return true;
            }
        case dItemNo_WOOD_STICK_e:
            if (swordRequirement == RandomizerContext::WOODEN_SWORD) {
                return true;
            }
        default:
            return false;
        }
    }

    return false;
}

bool randomizer_mirrorChamberWallShouldExist() {
    auto mirrorChamberAccess = randomizer_GetContext().mSettings[RandomizerContext::MIRROR_CHAMBER_ACCESS];
    return mirrorChamberAccess == RandomizerContext::CLOSED ||
          (mirrorChamberAccess == RandomizerContext::BARRIER && !dComIfGs_isStageBossEnemy(0x13));
}

void randomizer_returnToSpawn(bool tryOverride) {

    auto& placeOverrides = randomizer_GetContext().mReturnToPlaceOverrides;
    auto stageId = getStageID();

    // If we're trying to override the default return to spawn
    if (tryOverride && placeOverrides.contains(stageId)) {
        auto entrance = placeOverrides[stageId];

        // If in lakebed temple, spawn on land if shadow crystal is obtained like vanilla
        if (entrance.stageId == Lakebed_Temple && dComIfGs_isEventBit(TRANSFORMING_UNLOCKED)) {
            entrance.pointNo = 2;
        }

        dComIfGp_setNextStage(allStages[entrance.stageId], entrance.pointNo, entrance.roomNo, entrance.mapLayer);
        return;
    }

    // If a player hasn't completed a twilight/MDH, we want to unset the transform flag so they aren't forced to be wolf
    // unnecessarily.
    for (int32_t i = 0; i < 4; i++) {
        if (!dComIfGs_isDarkClearLV(i)) {
            dComIfGs_offTransformLV(i);
        }
    }

    // If Midna's Desperate Hour is not complete, unset the flags that trigger it incase the player
    // used return to spawn while MDH was active
    if (!dComIfGs_isEventBit(MIDNAS_DESPERATE_HOUR_COMPLETED)) {
        dComIfGs_offStageSwitch(4, 0xE);
        dComIfGs_offEventBit(MIDNAS_DESPERATE_HOUR_STARTED);
    }

    // Turn the player back into Link if they are currently wolf
    dComIfGs_setTransformStatus(TF_STATUS_HUMAN);

    // Return to spawn. If the spawn has been randomized, that's taken care of within the function
    dComIfGp_setNextStage("F_SP103", 1, 1, -1);
}

u8 randomizer_getRandomFoolishItemModelID() {
    static constexpr auto foolishItemModels = std::to_array<u8>({
        dItemNo_Randomizer_ARMOR_e,
        dItemNo_Randomizer_WOOD_STICK_e,
        dItemNo_Randomizer_WOOD_SHIELD_e,
        dItemNo_Randomizer_HYLIA_SHIELD_e,
        dItemNo_Randomizer_MAGIC_LV1_e,
        dItemNo_Randomizer_FISHING_ROD_1_e,
        dItemNo_Randomizer_HAWK_EYE_e,
        dItemNo_Randomizer_BOOMERANG_e,
        dItemNo_Randomizer_SPINNER_e,
        dItemNo_Randomizer_IRONBALL_e,
        dItemNo_Randomizer_BOW_e,
        dItemNo_Randomizer_COPY_ROD_e,
        dItemNo_Randomizer_HOOKSHOT_e,
        dItemNo_Randomizer_HVY_BOOTS_e,
        dItemNo_Randomizer_PACHINKO_e,
        dItemNo_Randomizer_BOMB_BAG_LV1_e,
        dItemNo_Randomizer_ANCIENT_DOCUMENT_e,
    });

    u8 selectedModal = foolishItemModels[static_cast<int>(cM_rnd() * foolishItemModels.size()) % foolishItemModels.size()];
    return verifyProgressiveItem(selectedModal);
}

u32 getActorPatchesCurrentStageKey(u8 roomNo) {
    u32 actorPatchesStageKey{};
    actorPatchesStageKey |= getStageID(dComIfGp_getStartStageName()) << 16;
    actorPatchesStageKey |= roomNo << 8;
    actorPatchesStageKey |= dComIfG_play_c::getLayerNo(0);
    return actorPatchesStageKey;
}

u32 getStageObjCRC32(u8* data, size_t size) {
    return randomizer::utility::crc32(data, size);
}

stage_tgsc_data_class parseObjData(const YAML::Node& objectNode) {
    using namespace Utility::Endian;
    // Get all the data for the actor (with endian shenanigans)
    stage_tgsc_data_class object{};
    const auto& actorName = objectNode["name"].as<std::string>();
    strncpy(object.name, actorName.c_str(), 8);
    object.base.parameters = toPlatform(target, objectNode["parameters"].as<u32>());
    object.base.position.x = toPlatform(target, objectNode["position"]["x"].as<f32>());
    object.base.position.y = toPlatform(target, objectNode["position"]["y"].as<f32>());
    object.base.position.z = toPlatform(target, objectNode["position"]["z"].as<f32>());
    // Have to retrieve as u16 and then cast as s16 because otherwise yaml-cpp
    // complains about values over 32767 not fitting in s16
    object.base.angle.x = toPlatform(target, static_cast<s16>(objectNode["angle"]["x"].as<u16>()));
    object.base.angle.y = toPlatform(target, static_cast<s16>(objectNode["angle"]["y"].as<u16>()));
    object.base.angle.z = toPlatform(target, static_cast<s16>(objectNode["angle"]["z"].as<u16>()));
    object.base.setID = toPlatform(target, static_cast<s16>(objectNode["set id"].as<u16>()));

    if (objectNode["scale"]) {
        object.scale.x = objectNode["scale"]["x"].as<u8>();
        object.scale.y = objectNode["scale"]["y"].as<u8>();
        object.scale.z = objectNode["scale"]["z"].as<u8>();
    } else {
        object.scale = fopAcM_prmScale_class{0, 0, 0};
    }

    return object;
}

void parseObjPatchData(stage_tgsc_data_class& object, const YAML::Node& patchNode) {
    using namespace Utility::Endian;
    if (patchNode["name"]) {
        const auto& newName = patchNode["name"].as<std::string>();
        strncpy(object.name, newName.c_str(), 8);
    }
    if (patchNode["parameters"]) {
        object.base.parameters = toPlatform(target, patchNode["parameters"].as<u32>());
    }
    if (auto patchPosition = patchNode["position"]) {
        if (patchPosition["x"]) {
            object.base.position.x = toPlatform(target, patchPosition["x"].as<f32>());
        }
        if (patchPosition["y"]) {
            object.base.position.y = toPlatform(target, patchPosition["y"].as<f32>());
        }
        if (patchPosition["z"]) {
            object.base.position.z = toPlatform(target, patchPosition["z"].as<f32>());
        }
    }
    if (auto patchAngle = patchNode["angle"]) {
        // Have to retrieve as u16 and then cast as s16 because otherwise yaml-cpp
        // complains about values over 32767 not fitting in s16
        if (patchAngle["x"]) {
            object.base.angle.x = toPlatform(target, static_cast<s16>(patchAngle["x"].as<u16>()));
        }
        if (patchAngle["y"]) {
            object.base.angle.y = toPlatform(target, static_cast<s16>(patchAngle["y"].as<u16>()));
        }
        if (patchAngle["z"]) {
            object.base.angle.z = toPlatform(target, static_cast<s16>(patchAngle["z"].as<u16>()));
        }
    }
    if (auto patchScale = patchNode["scale"]) {
        // Have to retrieve as u16 and then cast as s16 because otherwise yaml-cpp
        // complains about values over 32767 not fitting in s16
        if (patchScale["x"]) {
            object.scale.x = toPlatform(target, static_cast<s16>(patchScale["x"].as<u16>()));
        }
        if (patchScale["y"]) {
            object.scale.y = toPlatform(target, static_cast<s16>(patchScale["y"].as<u16>()));
        }
        if (patchScale["z"]) {
            object.scale.z = toPlatform(target, static_cast<s16>(patchScale["z"].as<u16>()));
        }
    }
}

static std::array<u8, 20> CreateAttributeData(const YAML::Node& node, const std::string& name) {
    auto attributesStr = node.as<std::string>();
    auto attributesVec = HexToBytes(attributesStr);
    if (attributesVec.size() != 16) {
        throw std::runtime_error(fmt::format("Attributes for Text Override {} "
                                             "are the wrong length. (Expected: 16, Actual: {}", name, attributesVec.size()));
    }

    std::array<u8, 20> attributes{};
    for (size_t i = 0; i < attributesVec.size(); ++i) {
        attributes[i + 4] = attributesVec[i];
    }
    return attributes;
}

RandomizerContext WriteSeedData(randomizer::logic::world::World* world) {
    RandomizerContext randoData{};

    // Give custom flows and messages new indices as we read them in/create them
    std::unordered_map<std::string, u16> customMessageIDs{};
    std::unordered_map<std::string, u16> customFlowIDs{};
    std::unordered_set<u16> usedMessageIDs{};
    std::unordered_set<u16> usedFlowIDs{};
    u16 curCustomMessageID = BASE_CUSTOM_MSG_AND_FLOW_ID;
    u16 curCustomFlowID = BASE_CUSTOM_MSG_AND_FLOW_ID;

    // Helper functions for assigning new custom flow IDs/message IDs
    auto handleCustomID = [](const std::string& name, auto& customIds, auto& usedIds, u16& curCustomID) {
        u16 resultIndex{};
        // Check to see if we're setting a custom index
        auto resultInt = randomizer::utility::str::toInt(name);
        // If we have a regular index, then use that directly
        if (resultInt.has_value()) {
            resultIndex = resultInt.value();
        } else {
            // If we don't, assume we're setting the index as custom
            if (customIds.contains(name)) {
                resultIndex = customIds[name];
            } else {
                while (usedIds.contains(curCustomID)) {
                    ++curCustomID;
                }
                auto newIndex = curCustomID++;
                resultIndex = newIndex;
                customIds[name] = newIndex;
            }
        }

        usedIds.insert(resultIndex);
        return resultIndex;
    };


    auto handleCustomFlowID = [&](const std::string& name) {
        return handleCustomID(name, customFlowIDs, usedFlowIDs, curCustomFlowID);
    };


    auto handleCustomMessageID = [&](const std::string& name) {
        return handleCustomID(name, customMessageIDs, usedMessageIDs, curCustomMessageID);
    };

    // Settings we need to check ingame
    for (const auto& [setting, info] : *randomizer::seedgen::settings::GetAllSettingsInfo()) {
        if (info->NeedInGame()) {
            auto settingEnum = RandomizerContext::SettingToEnum(setting);
            if (settingEnum == -1) {
                throw std::runtime_error("Setting \"" + setting + "\" does not have an associated enum value");
            }
            auto option = world->Setting(setting).GetCurrentOption();
            int optionEnum{};
            // If this setting's options are just numbers, get the numeric value
            if (info->OptionsAreNumbers()) {
                optionEnum = world->Setting(setting).GetCurrentOptionAsNumber();
            } else {
                optionEnum = RandomizerContext::OptionToEnum(option);
            }
            if (optionEnum == -1) {
                throw std::runtime_error("Option \"" + option + "\" for setting \"" + setting + "\" does not have an associated enum value");
            }
            randoData.mSettings[settingEnum] = optionEnum;
        }
    }

    // Set data for all locations
    for (const auto& location : world->GetAllLocations()) {
        const auto& metaData = location->GetMetadata();

        // Chest Overrides
        // Keyed by u16 of 0xFF00 (stage index) and 0x00FF (tbox id)
        if (location->HasCategories("Chest")) {
            for (const auto& chestNode : metaData["Chest"]) {
                u8 stage = chestNode["Stage"].as<u8>();
                u8 tboxId = chestNode["Tbox Id"].as<u8>();
                u8 itemId = location->GetCurrentItem()->GetID();
                u16 key = (stage << 8) | tboxId;
                randoData.mTreasureChestOverrides[key] = itemId;
            }
        }

        // Poe Overrides
        // Keyed by u16 of 0xFF00 (stage index) and 0x00FF (collectible flag)
        if (location->HasCategories("Poe")) {
            for (const auto& poeNode : metaData["Poe"]) {
                const auto& stage = poeNode["Stage"].as<u8>();
                const auto& flag = poeNode["Flag"].as<u8>();
                u8 itemId = location->GetCurrentItem()->GetID();
                u16 key = (stage << 8) | flag;
                randoData.mPoeOverrides[key] = itemId;
            }
        }

        // Freestanding Overrides
        // Keyed by the stage index and collectible flag of the item
        if (location->HasCategories("Freestanding Item")) {
            for (const auto& freestandingItemNode: metaData["Freestanding Item"]) {
                u8 stage = freestandingItemNode["Stage"].as<u8>();
                u8 flag = freestandingItemNode["Flag"].as<u8>();
                u8 itemId = location->GetCurrentItem()->GetID();
                u16 key = (stage << 8) | flag;
                randoData.mFreestandingItemOverrides[key] = itemId;
            }
        }

        // Bug Rewards
        // Keyed by the item id of the original bug
        if (location->HasCategories("Bug Reward")) {
            for (const auto& bugRewardNode : metaData["Bug Reward"]) {
                u8 bugItemId = bugRewardNode["Item Id"].as<u8>();
                u8 itemId = location->GetCurrentItem()->GetID();
                randoData.mBugRewardOverrides[bugItemId] = itemId;
            }
        }

        // Sky Characters
        // Keyed by u16 of 0xFF00 (stage index) and 0x00FF (roomNo)
        if (location->HasCategories("Sky Character")) {
            for (const auto& skyCharacterNode : metaData["Sky Character"]) {
                u8 stageIdx = skyCharacterNode["Stage"].as<u8>();
                u8 roomNo = skyCharacterNode["Room"].as<u8>();
                u8 itemId = location->GetCurrentItem()->GetID();
                u16 key = (stageIdx << 8) | roomNo;
                randoData.mSkyCharacterOverrides[key] = itemId;
            }
        }

        // Golden Wolves
        // Keyed by u16 of the event flag for obtaining the golden wolf item
        if (location->HasCategories("Golden Wolf")) {
            for (const auto& goldenWolfNode : metaData["Golden Wolf"]) {
                u16 flag = goldenWolfNode["Flag"].as<u16>();
                u8 itemId = location->GetCurrentItem()->GetID();
                randoData.mGoldenWolfOverrides[flag] = itemId;
            }
        }

        // Shop Items
        // Keyed by u16 of the stage and original shop item
        if (location->HasCategories("Shop") && world->Setting("Shop Items") == "On") {
            for (const auto& shopNode : metaData["Shop"]) {
                u8 stage = shopNode["Stage"].as<u8>();
                u8 originalItem = shopNode["Item"].as<u8>();
                u16 key = (stage << 8) | originalItem;
                randoData.mShopOverrides[key] = location->GetCurrentItem()->GetID();
            }
        }

        // Twilit Insect Overrides
        // Keyed by u16 of 0xFF00 (stage index) and 0x00FF (flag, which is a tbox id)
        if (location->HasCategories("Twilit Insect")) {
            for (const auto& twilitInsectNode : metaData["Twilit Insect"]) {
                u8 stage = twilitInsectNode["Stage"].as<u8>();
                u8 tboxId = twilitInsectNode["Flag"].as<u8>();
                u16 itemId = location->GetCurrentItem()->GetID();
                u16 key = (stage << 8) | tboxId;
                randoData.mTwilitInsectOverrides[key] = itemId;
            }
        }

        // Helper function for getting flag values
        auto getNodeFlags = [](auto& itemData, const YAML::Node& metaData) {
            if (metaData["Event Flag"]) {
                itemData.flag = metaData["Event Flag"].as<u16>();
            } else if (metaData["Switch Flag"]) {
                itemData.stage = metaData["Switch Flag"]["Stage"].as<u8>();
                itemData.flag = metaData["Switch Flag"]["Flag"].as<u8>();
            } else if (metaData["Item Flag"]) {
                itemData.stage = metaData["Item Flag"]["Stage"].as<u8>();
                itemData.flag = metaData["Item Flag"]["Flag"].as<u8>();
            }
        };

        // Items that we determine the text of and then give during a FLW message
        if (location->HasCategories("FLW Message")) {
            for (const auto& flwMessageNode : metaData["FLW Message"]) {
                u8 group = flwMessageNode["Group"].as<u8>();
                u16 messageId = flwMessageNode["Message Id"].as<u16>();
                u32 key = (group << 16) | messageId;
                randoData.mFlowItemMessageOverrides[key].itemId = location->GetCurrentItem()->GetID();
                getNodeFlags(randoData.mFlowItemMessageOverrides[key], metaData);
            }
        }

        // Items that are given by FLW events. Override the item in the existing event with the
        // randomized item
        if (location->HasCategories("FLW Event")) {
            for (const auto& flwEventNode : metaData["FLW Event"]) {
                u8 group = flwEventNode["Group"].as<u8>();
                u16 index = handleCustomFlowID(flwEventNode["Index"].as<std::string>());
                mesg_flow_node_event event{};
                event.type = 3; // event type node
                event.event_idx = 8;
                event.next_node_idx = handleCustomFlowID(flwEventNode["Next Node Index"].as<std::string>());
                auto params = flwEventNode["Parameters"].as<u32>();
                // Zero out the spot for the item id
                params &= 0xFFFFFF00;
                // Put in the item id
                params |= location->GetCurrentItem()->GetID();
                // Set the params in the correct order
                event.params[0] = (params >> 24) & 0xFF;
                event.params[1] = (params >> 16) & 0xFF;
                event.params[2] = (params >> 8) & 0xFF;
                event.params[3] = params & 0xFF;

                // Construct another FLW node to set the associated flag in a temporary variable
                // right before we receive the item. This ensures that a tracker/AP can pick up
                // on the fact that we've received the item.
                auto newFlwIndex = handleCustomFlowID(location->GetName() + " Flag Set Node");
                mesg_flow_node_event flagEvent{};
                flagEvent.type = 3; // event type node
                flagEvent.event_idx = 46; // Set temporary randomizer flag

                u8 stage{0xFF};
                u16 flag{0xFFFF};
                if (metaData["Event Flag"]) {
                    flag = metaData["Event Flag"].as<u16>();
                } else if (metaData["Switch Flag"]) {
                    stage = metaData["Switch Flag"]["Stage"].as<u8>();
                    flag = metaData["Switch Flag"]["Flag"].as<u8>();
                }
                flagEvent.params[0] = 0;
                flagEvent.params[1] = stage;
                flagEvent.params[2] = (flag >> 8) & 0xFF;
                flagEvent.params[3] = flag & 0xFF;

                // Store the modified FLW nodes. The flag event takes the place of the original
                // index we're modifying and then leads into the custom index of the event that
                // sets up the item id
                flagEvent.next_node_idx = newFlwIndex;
                u32 key = (group << 16) | index;
                randoData.mFlowPatches[key] = std::bit_cast<u64>(flagEvent);

                key = (CUSTOM_BMG_GROUP << 16) | newFlwIndex;
                randoData.mFlowPatches[key] = std::bit_cast<u64>(event);
            }
        }

        // Items that we lookup just by calling their location name
        if (location->HasCategories("Name Lookup")) {
            for (const auto& locationNameNode : metaData["Name Lookup"]) {
                const auto& locationName = nameLookupOverride(locationNameNode.as<std::string>());
                const int itemId = location->GetCurrentItem()->GetID();
                randoData.mItemLocations[locationName].itemId = itemId;
                getNodeFlags(randoData.mItemLocations[locationName], metaData);
            }
        }
    }

    // Set starting inventory
    for (const auto& item: world->GetStartingItemPool()) {
        randoData.mStartingInventory.push_back(item->GetID());
    }

    // Set starting flags
    auto startFlags = LOAD_EMBED_YAML(RANDO_DATA_PATH "startflags.yaml");
    // Event Flags
    for (const auto& flagNode : startFlags["EventFlags"]) {
        if (flagNode.IsScalar()) {
            const auto& flag = flagNode.as<u16>();
            randoData.mStartEventFlags.push_back(flag);
        } else if (flagNode.IsMap()) {
            const auto& condition = flagNode.begin()->first.as<std::string>();
            if (world->EvaluateSettingCondition(condition)) {
                for (const auto& conditionalFlag : flagNode.begin()->second) {
                    const auto& flag = conditionalFlag.as<u16>();
                    randoData.mStartEventFlags.push_back(flag);
                }
            }
        }
    }

    // Region Flags
    for (const auto& regionNode : startFlags["RegionFlags"]) {
        const auto& region = regionNode.first.as<std::string>();
        const auto& index = regionNode.second["Index"].as<int>();
        const auto& flags = regionNode.second["Flags"];
        // This seems kinda scuffed so maybe we change it later
        for (const auto& flagNode : flags) {
            if (flagNode.IsScalar()) {
                const auto& flag = flagNode.as<int>();
                randoData.mStartRegionFlags[index].push_back(flag);
            } else if (flagNode.IsMap()) {
                const auto& condition = flagNode.begin()->first.as<std::string>();
                if (world->EvaluateSettingCondition(condition)) {
                    for (const auto& conditionalFlag : flagNode.begin()->second) {
                        const auto& flag = conditionalFlag.as<int>();
                        randoData.mStartRegionFlags[index].push_back(flag);
                    }
                }
            }
        }
    }

    if (world->Setting("Unlock Map Regions") == "On")
    {
        auto& bits = randoData.mMapBits;
        bits = 0x20;
        if (world->Setting("Snowpeak Does Not Require Reekfish Scent") == "On") {bits |= 0x40;}
        if (world->Setting("Lanayru Twilight Cleared") == "On") {bits |= 0x10;}
        if (world->Setting("Eldin Twilight Cleared") == "On") {bits |= 0x08;}
        if (world->Setting("Faron Twilight Cleared") == "On") {bits |= 0x04;}
        if (world->Setting("Skip Prologue") == "On") {bits |= 0x02;}
    }

    // Set starting time of day
    const auto startTimeSetting = world->Setting("Starting Time of Day");
    if (startTimeSetting == "Morning")
        randoData.mStartHour = 6;
    else if (startTimeSetting == "Noon")
        randoData.mStartHour = 12;
    else if (startTimeSetting == "Evening")
        randoData.mStartHour = 18;
    else if (startTimeSetting == "Night")
        randoData.mStartHour = 24;

    // Actor Patches
    auto actorPatches = LOAD_EMBED_YAML(RANDO_DATA_PATH "object_patches.yaml");
    for (const auto& stageNode : actorPatches) {
        const auto& stageName = stageNode.first.as<std::string>();
        for (const auto& roomNode : stageNode.second) {
            u8 roomNo{};
            // Special value for actors always on the stage and not just one specific room
            if (roomNode.first.as<std::string>() == "Stage") {
                roomNo = RandomizerContext::ROOM_STAGE;
            } else {
                roomNo = roomNode.first.as<u8>();
            }
            for (const auto& objectNode : roomNode.second) {
                const auto& action = objectNode["action"].as<std::string>();

                // Get all the data for the actor (with endian shenanigans)
                auto object = parseObjData(objectNode);

                size_t objDataSize = RandomizerContext::TGSC_CRC_SIZE;
                // If the scale of this object is all zeros, it's an ACTR
                if (object.scale.x == 0 && object.scale.y == 0 && object.scale.z == 0) {
                    objDataSize = RandomizerContext::ACTR_CRC_SIZE;
                }

                // Create unique hash based off of actor data
                u32 objectCRC32 = getStageObjCRC32(reinterpret_cast<u8*>(&object), objDataSize);

                // Depending on the action, store data on this actor
                std::vector<u8> actorData(0);
                // If we're patching this object, Then override the object with whatever parts are being patched
                // and add that patch data to our actorData
                if (action == "patch") {
                    parseObjPatchData(object, objectNode["patch"]);
                    actorData.resize(objDataSize);
                    std::memcpy(actorData.data(), &object, objDataSize);
                } else if (action == "add") {
                    // If we're adding the object, add it's regular data to the actorData
                    actorData.resize(objDataSize);
                    std::memcpy(actorData.data(), &object, objDataSize);
                } else if (action == "delete") {
                    // If we're deleting this actor, give it a specific size to indicate we're deleting it
                    actorData.resize(RandomizerContext::OBJ_DELETE_SIZE);
                } else {
                    // Unknown action. Don't continue
                    throw std::runtime_error("object patch action \"" + action + "\" not recognized");
                }

                // Loop through all of our layers to apply this action to
                for (const auto& layerNode : objectNode["layers"]) {
                    u8 layerNo = layerNode.as<u8>();
                    // Create key based off of stage index, room, and layer
                    u32 stageRoomLayerKey{};
                    stageRoomLayerKey |= getStageID(stageName.c_str()) << 16;
                    stageRoomLayerKey |= roomNo << 8;
                    stageRoomLayerKey |= layerNo;

                    if (action == "add") {
                        randoData.mObjectAdditions[stageRoomLayerKey].push_back(actorData);
                    } else { // patch or delete
                        randoData.mObjectPatches[stageRoomLayerKey][objectCRC32] = actorData;
                    }
                }
            }
        }
    }

    // Flow Patches
    auto flowPatches = LOAD_EMBED_YAML(RANDO_DATA_PATH "flow_patches.yaml");
    for (const auto& groupNode : flowPatches) {
        u8 groupNo = groupNode.first.as<u8>();
        for (const auto& flowNode : groupNode.second) {
            // Check to see if this patch is contingent on a specific setting
            if (flowNode["only if"]) {
                const auto& condition = flowNode["only if"].as<std::string>();
                // If the required condition isn't set, then skip this one
                if (!world->EvaluateSettingCondition(condition)) {
                    continue;
                }
            }
            std::string name{};
            std::list<u16> indices{};
            if (flowNode["index"]) {
                // If we're specifying a sequence of indices
                if (flowNode["index"].IsSequence()) {
                    for (const auto& indexNode : flowNode["index"]) {
                        auto index = indexNode.as<u16>();
                        indices.push_back(index);
                        usedFlowIDs.insert(index);
                    }
                    name = std::to_string(indices.front());
                }
                // If we have just a single index
                else if (flowNode["index"].IsScalar()) {
                    auto index = flowNode["index"].as<u16>();
                    indices.push_back(index);
                    name = std::to_string(index);
                    usedFlowIDs.insert(index);
                }

                // If we're specifying an index as well as a name, add the index to the custom
                // ids
                if (flowNode["name"]) {
                    name = flowNode["name"].as<std::string>();
                    customFlowIDs[name] = indices.front();
                }
            } else {
                name = flowNode["name"].as<std::string>();
                indices.push_back(handleCustomFlowID(flowNode["name"].as<std::string>()));
            }

            const auto& type = flowNode["type"].as<std::string>();
            u64 value{};
            if (type == "branch") {
                auto branch = reinterpret_cast<mesg_flow_node_branch*>(&value);
                branch->type = 2;
                branch->field_0x1 = flowNode["num results"].as<u8>();
                branch->query_idx = flowNode["query"].as<u16>();
                branch->param = flowNode["parameters"].as<u16>();
                branch->next_node_idx = flowNode["next node index"].as<u16>();
                // If we're using custom result indices
                if (flowNode["results"]) {
                    auto& results = flowNode["results"];
                    if (results.size() != branch->field_0x1) {
                        throw std::runtime_error(fmt::format("Flow results size for {} "
                            "do not match num results. (expected: {}. size: {})", name, branch->field_0x1, results.size()));
                    }
                    for (const auto& resultNode : results) {
                        auto resultIndex = handleCustomFlowID(resultNode.as<std::string>());
                        for (auto index : indices) {
                            u32 key = (groupNo << 16) | index;
                            randoData.mFlowPatchesBranchOverrides[key].push_back(resultIndex);
                        }
                    }
                }
            }
            else if (type == "event") {
                auto event = reinterpret_cast<mesg_flow_node_event*>(&value);
                event->type = 3;
                event->event_idx = flowNode["event"].as<u8>();
                event->next_node_idx = handleCustomFlowID(flowNode["next node index"].as<std::string>());
                u32 params = flowNode["parameters"].as<u32>();
                event->params[0] = (params >> 24) & 0xFF;
                event->params[1] = (params >> 16) & 0xFF;
                event->params[2] = (params >> 8) & 0xFF;
                event->params[3] = params & 0xFF;
            } else if (type == "message") {
                auto message = reinterpret_cast<mesg_flow_node*>(&value);
                message->type = 1;
                message->msg_index = handleCustomMessageID(flowNode["inf index"].as<std::string>());
                message->next_node_idx = handleCustomFlowID(flowNode["next flow index"].as<std::string>());

                // If a custom message is too long, split it up among additional flow/message nodes
                if (message->msg_index >= BASE_CUSTOM_MSG_AND_FLOW_ID) {
                    auto textName = flowNode["inf index"].as<std::string>();
                    if (world->GetTextDatabase().contains(textName)) {
                        auto& text = world->GetTextObject(textName);
                        if (text.IsTooLong()) {
                            // Get the attributes for the text at this ID. Pretty inefficient since we
                            // have to loop through every element unfortunately
                            std::optional<std::array<u8, 20>> customAttributes{};
                            auto textOverrides = LOAD_EMBED_YAML(RANDO_DATA_PATH "text/text_overrides.yaml");
                            for (const auto& overrideNode : textOverrides) {
                                const auto& overrideName = overrideNode["Name"].as<std::string>();
                                if (overrideName == textName && overrideNode["Attributes"]) {
                                    customAttributes = CreateAttributeData(overrideNode["Attributes"], textName);
                                    break;
                                }
                            }


                            // Add each split text as a new custom message entry. The original entry
                            // still exists but has been sliced down to fit properly.
                            auto extraText = text.SplitToFitTextLimits();
                            std::vector<mesg_flow_node> newMsgFlows{};
                            for (size_t i = 0; i < extraText.size(); i++) {
                                // Add this custom text to the world
                                auto extraTextName = textName + std::to_string(i + 1);
                                world->AddNewText(extraTextName) = extraText[i];

                                // Create new Flow and Message Ids for the split text object
                                auto newCustomFlowIndex = handleCustomFlowID(extraTextName);
                                auto newCustomMessageIndex = handleCustomMessageID(extraTextName);

                                // Create the new flow node. We're storing its own flow index with
                                // itself for now, but we'll shift it back to the previous node later
                                mesg_flow_node newMsgFlow{};
                                newMsgFlow.type = 1;
                                newMsgFlow.msg_index = newCustomMessageIndex;
                                newMsgFlow.next_node_idx = newCustomFlowIndex;

                                newMsgFlows.push_back(newMsgFlow);

                                //Add the custom text to the rando data
                                u32 key = (CUSTOM_BMG_GROUP << 16) | newCustomMessageIndex;
                                for (auto language : randomizer::supportedLanguages) {
                                    std::string newText = extraText[i].mText[language];
                                    randomizer::applyMessageCodes(newText);
                                    randoData.mTextOverrides[language][key] = newText;
                                }

                                // Add custom attribute data as well if it exists
                                if (customAttributes.has_value()) {
                                    auto attributes = customAttributes.value();

                                    // Set the message id in the attribute data
                                    attributes[4] = newCustomMessageIndex >> 8;
                                    attributes[5] = newCustomMessageIndex & 0xFF;

                                    randoData.mAttributeOverrides[key] = attributes;
                                }
                            }

                            // Shift all the next_node_idx fields back a node and set the original
                            // next node idx as the next node idx for the final of the new flows
                            auto finalNodeIdx = message->next_node_idx;
                            message->next_node_idx = newMsgFlows[0].next_node_idx;
                            for (size_t i = 0; i < newMsgFlows.size(); i++) {
                                auto& curFlow = newMsgFlows[i];
                                auto curFlowIdx = curFlow.next_node_idx;
                                if (i == newMsgFlows.size() - 1) {
                                    curFlow.next_node_idx = finalNodeIdx;
                                } else {
                                    curFlow.next_node_idx = newMsgFlows[i + 1].next_node_idx;
                                }

                                // Also Add the new custom flows to our rando data
                                u32 key = (CUSTOM_BMG_GROUP << 16) | curFlowIdx;
                                randoData.mFlowPatches[key] = std::bit_cast<u64>(curFlow);
                            }
                        }
                    }
                }
            }
            for (auto index : indices) {
                u32 key = (groupNo << 16) | index;
                randoData.mFlowPatches[key] = value;
            }
        }
    }

    // Text Overrides
    auto textOverrides = LOAD_EMBED_YAML(RANDO_DATA_PATH "text/text_overrides.yaml");
    for (const auto& overrideNode : textOverrides) {
        // Check to see if this override is contingent on a specific setting
        if (overrideNode["Only If"]) {
            const auto& condition = overrideNode["Only If"].as<std::string>();
            // If the required condition isn't set, then skip this one
            if (!world->EvaluateSettingCondition(condition)) {
                continue;
            }
        }
        const auto& name = overrideNode["Name"].as<std::string>();
        u8 group;
        u16 messageId;
        if (overrideNode["Group"]) {
            group = overrideNode["Group"].as<u8>();
        } else {
            // If no group specified, assume custom bmg group
            group = CUSTOM_BMG_GROUP;
        }
        if (overrideNode["Message Id"]) {
            messageId = overrideNode["Message Id"].as<u16>();
        } else {
            // If no message id specified, assume a custom one
            messageId = handleCustomMessageID(overrideNode["Name"].as<std::string>());
        }
        u32 key = (group << 16) | messageId;
        for (auto language : randomizer::supportedLanguages) {
            std::string text;
            if (world->GetTextDatabase().contains(name)) {
                text = world->GetText(name, randomizer::Text::STANDARD, language);
            } else {
                text = randomizer::getTextStr(name, randomizer::Text::STANDARD, language);
            }

            randomizer::applyMessageCodes(text);
            randoData.mTextOverrides[language][key] = text;
        }

        // If we have custom attributes
        if (overrideNode["Attributes"]) {
            auto attributes = CreateAttributeData(overrideNode["Attributes"], name);

            // Set the message id in the attribute data
            attributes[4] = messageId >> 8;
            attributes[5] = messageId & 0xFF;

            randoData.mAttributeOverrides[key] = attributes;
        }
    }

    // Entrance Overrides
    if (world->Setting("Mirror Chamber Access") == "Closed") {
        // Set exiting the Arbiter's Grounds Boss Room to spawn at the Arbiter's Grounds entrance
        // if mirror chamber access is closed
        RandomizerContext::EntranceOverride original = {
            StageIDs::Mirror_Chamber,
            4,
            0,
            -1
        };

        RandomizerContext::EntranceOverride override = {
            StageIDs::Bulblin_Camp,
            3,
            3,
            -1
        };

        randoData.mEntranceOverrides[std::bit_cast<int>(original)] = override;
    }

    // Vanilla Return to Place Overrides. Will need to change when boss/miniboss ER is implemented
    static const std::list<std::pair<std::list<int>, RandomizerContext::EntranceOverride>> defaultPlaceOverrides{
        {{Forest_Temple, Ook, Diababa},                      {Forest_Temple, 22, 0, -1}},
        {{Goron_Mines, Dangoro, Fyrus},                      {Goron_Mines, 1, 0, -1}},
        {{Lakebed_Temple, Deku_Toad, Morpheel},              {Lakebed_Temple, 0, 0, -1}},
        {{Arbiters_Grounds, Death_Sword, Stallord},          {Arbiters_Grounds, 0, 0, -1}},
        {{Snowpeak_Ruins, Darkhammer, Blizzeta},             {Snowpeak_Ruins, 0, 0, -1}},
        {{Temple_of_Time, Darknut, Armogohma},               {Temple_of_Time, 0, 0, -1}},
        {{City_in_the_Sky, Aeralfos, Argorok},               {City_in_the_Sky, 0, 3, -1}},
        {{Palace_of_Twilight, Phantom_Zant_1,
                Phantom_Zant_2, Zant_Main_Room, Zant_Fight}, {Palace_of_Twilight, 0, 0, -1}},
        {{Hyrule_Castle, Ganondorf_Castle, Ganondorf_Field}, {Hyrule_Castle, 11, 0, -1}},
    };

    // Return to Place Overrides
    for (const auto& [stages, returnPlace] : defaultPlaceOverrides) {
        for (auto stage : stages) {
            randoData.mReturnToPlaceOverrides[stage] = returnPlace;
        }
    }

    return std::move(randoData);
}

static void DeleteFailedGenerationFiles(randomizer::Randomizer& rando) {
    // If the hash is empty, then we never generated any files
    if (!rando.GetConfig().GetHash().empty()) {
        std::filesystem::remove_all(rando.GetSeedOutputPath());
    }
}

bool GenerateAndWriteSeed(std::string& generationStatusMsg) {
    auto r = randomizer::Randomizer{::randomizer::paths::GetRandomizerPath()};

    auto generationResult = r.Generate();
    if (generationResult.has_value()) {
        generationStatusMsg = fmt::format("Failed to generate seed. Reason:\n{}", generationResult.value());
        DeleteFailedGenerationFiles(r);
        return false;
    }

    const auto world = r.GetWorld();
    RandomizerContext randoData{};
    try {
        randoData = WriteSeedData(world);
    } catch (const std::runtime_error& e) {
        generationStatusMsg =
            fmt::format("Failed to write seed data. Reason:\n{}", e.what());
        DeleteFailedGenerationFiles(r);
        return false;
    }

    randoData.mHash = r.GetConfig().GetHash();
    auto writeToFileResult = randoData.WriteToFile();
    if (writeToFileResult.has_value()) {
        generationStatusMsg =
            fmt::format("Failed to write seed data to file. Reason:\n{}", writeToFileResult.value());
        DeleteFailedGenerationFiles(r);
        return false;
    }

    generationStatusMsg = fmt::format("Seed generated! Hash: {}", randoData.mHash);
    return true;
}