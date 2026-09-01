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
#include "../generator/logic/entrance_shuffle.hpp"

#include <fstream>
#include <type_traits>
#include <unordered_set>
#include <mods/svc/log.hpp>

#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_class.h"
#include "m_Do/m_Do_audio.h"

namespace {

const char* flow_node_type_name(RandomizerContext::FlowNodeType type) {
    switch (type) {
    case RandomizerContext::FlowNodeType::MESSAGE:
        return "message";
    case RandomizerContext::FlowNodeType::BRANCH:
        return "branch";
    case RandomizerContext::FlowNodeType::EVENT:
        return "event";
    }
    return "";
}

RandomizerContext::FlowNodeType parse_flow_node_type(const YAML::Node& node) {
    const auto type = node.as<std::string>();
    if (type == "message") {
        return RandomizerContext::FlowNodeType::MESSAGE;
    }
    if (type == "branch") {
        return RandomizerContext::FlowNodeType::BRANCH;
    }
    if (type == "event") {
        return RandomizerContext::FlowNodeType::EVENT;
    }
    throw std::runtime_error("Unknown flow node type: " + type);
}

YAML::Node write_flow_reference(const RandomizerContext::FlowReference& reference) {
    YAML::Node node{};
    if (reference.nativeId.has_value()) {
        node = reference.nativeId.value();
    } else {
        node = reference.name;
    }
    return node;
}

RandomizerContext::FlowReference parse_flow_reference(const YAML::Node& node) {
    if (!node.IsScalar()) {
        throw std::runtime_error("Flow reference must be a node ID or name");
    }
    RandomizerContext::FlowReference reference{};
    try {
        reference.nativeId = node.as<u16>();
    } catch (const YAML::BadConversion&) {
        reference.name = node.as<std::string>();
    }
    return reference;
}

void write_message_style(
    YAML::Node node, const RandomizerContext::MessageStyleData& style) {
    node["eventLabelId"] = style.eventLabelId;
    node["speaker"] = static_cast<u16>(style.speaker);
    node["boxKind"] = static_cast<u16>(style.boxKind);
    node["drawType"] = static_cast<u16>(style.drawType);
    node["boxPosition"] = static_cast<u16>(style.boxPosition);
    node["lineAlignment"] = static_cast<u16>(style.lineAlignment);
    node["speakerMood"] = static_cast<u16>(style.speakerMood);
    node["cameraAttr"] = static_cast<u16>(style.cameraAttr);
    node["talkAnim"] = static_cast<u16>(style.talkAnim);
    node["faceAnim"] = static_cast<u16>(style.faceAnim);
    node["trailingData"] = style.trailingData;
}

RandomizerContext::MessageStyleData parse_message_style(const YAML::Node& node) {
    RandomizerContext::MessageStyleData style{};
    style.eventLabelId = node["eventLabelId"].as<u16>();
    style.speaker = node["speaker"].as<u8>();
    style.boxKind = node["boxKind"].as<u8>();
    style.drawType = node["drawType"].as<u8>();
    style.boxPosition = node["boxPosition"].as<u8>();
    style.lineAlignment = node["lineAlignment"].as<u8>();
    style.speakerMood = node["speakerMood"].as<u8>();
    style.cameraAttr = node["cameraAttr"].as<u8>();
    style.talkAnim = node["talkAnim"].as<u8>();
    style.faceAnim = node["faceAnim"].as<u8>();
    style.trailingData = node["trailingData"].as<u16>();
    return style;
}

}  // namespace

std::optional<std::string> RandomizerContext::WriteToFile() {

    std::ofstream seedData(this->GetSeedDataPath());
    if (!seedData.is_open()) {
        return "Could not open seed data file";
    }

    YAML::Node out{};
    out["formatVersion"] = FORMAT_VERSION;

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

    const std::unordered_map<u32, u16> u32ShopOverrides(this->mShopOverrides.begin(), this->mShopOverrides.end());
    out["mShopOverrides"] = u32ShopOverrides;

    out["mTwilitInsectOverrides"] = mTwilitInsectOverrides;

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
            auto node = out["mObjectPatches"][stageRoomLayer][actorCRC];
            node["data"] = ContainerToHexString(actorPatch.bytes);
            if (!actorPatch.flow.empty()) {
                node["flow"] = actorPatch.flow;
            }
        }
    }

    for (const auto& [stageRoomLayer, newActors] : this->mObjectAdditions) {
        for (const auto& actor : newActors) {
            YAML::Node node{};
            node["data"] = ContainerToHexString(actor.bytes);
            if (!actor.flow.empty()) {
                node["flow"] = actor.flow;
            }
            out["mObjectAdditions"][stageRoomLayer].push_back(node);
        }
    }

    for (const auto& flow : mFlowNodes) {
        YAML::Node node{};
        node["type"] = flow_node_type_name(flow.type);
        node["group"] = static_cast<u16>(flow.group);
        if (flow.patchIndex.has_value()) {
            node["patchIndex"] = flow.patchIndex.value();
        } else {
            node["name"] = flow.name;
        }
        node["parameters"] = flow.parameters;
        if (!flow.operation.empty()) {
            node["operation"] = flow.operation;
        }
        if (flow.type == FlowNodeType::MESSAGE) {
            node["message"] = write_flow_reference(flow.message);
        }
        if (flow.type != FlowNodeType::BRANCH) {
            node["next"] = write_flow_reference(flow.next);
        }
        for (const auto& result : flow.results) {
            node["results"].push_back(write_flow_reference(result));
        }
        out["mFlowNodes"].push_back(node);
    }

    for (const auto& message : mCustomMessages) {
        YAML::Node node{};
        node["group"] = static_cast<u16>(message.group);
        node["name"] = message.name;
        write_message_style(node["style"], message.style);
        for (const auto& [language, text] : message.text) {
            const auto languageName = randomizer::languageToString(
                static_cast<randomizer::Text::Language>(language));
            node["text"][languageName] = YAML::Binary(
                reinterpret_cast<const unsigned char*>(text.data()), text.size());
        }
        out["mCustomMessages"].push_back(node);
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

    for (const auto& [key, override] : mEntranceOverrides) {
        out["mEntranceOverrides"][std::bit_cast<uint64_t>(key)] = std::bit_cast<uint64_t>(override);
    }

    for (const auto& [key, override] : mReturnToPlaceOverrides) {
        out["mReturnToPlaceOverrides"][key] = std::bit_cast<uint64_t>(override);
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
    if (!in["formatVersion"] || in["formatVersion"].as<u32>() != FORMAT_VERSION) {
        mods::log::error("Seed {} uses an obsolete data format and must be regenerated", hash);
        mHash.clear();
        return "Seed data format is obsolete; regenerate this seed";
    }

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
        u32 key = shopNode.first.as<u32>();
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
            auto& actor = this->mObjectPatches[stageRoomLayer][actorCRC];
            actor.bytes = HexToBytes(actorPatchNode.second["data"].as<std::string>());
            if (actorPatchNode.second["flow"]) {
                actor.flow = actorPatchNode.second["flow"].as<std::string>();
            }
        }
    }

    // Object Additions
    for (const auto& stageNode: in["mObjectAdditions"]) {
        u32 stageRoomLayer = stageNode.first.as<u32>();
        for (const auto& objectData : stageNode.second) {
            ActorData actor{};
            actor.bytes = HexToBytes(objectData["data"].as<std::string>());
            if (objectData["flow"]) {
                actor.flow = objectData["flow"].as<std::string>();
            }
            this->mObjectAdditions[stageRoomLayer].push_back(std::move(actor));
        }
    }

    for (const auto& flowNode : in["mFlowNodes"]) {
        FlowNode flow{};
        flow.type = parse_flow_node_type(flowNode["type"]);
        flow.group = flowNode["group"].as<u8>();
        if (flowNode["patchIndex"]) {
            flow.patchIndex = flowNode["patchIndex"].as<u16>();
        } else {
            flow.name = flowNode["name"].as<std::string>();
        }
        flow.parameters = flowNode["parameters"].as<u32>();
        if (flowNode["operation"]) {
            flow.operation = flowNode["operation"].as<std::string>();
        }
        if (flowNode["message"]) {
            flow.message = parse_flow_reference(flowNode["message"]);
        }
        if (flowNode["next"]) {
            flow.next = parse_flow_reference(flowNode["next"]);
        }
        for (const auto& result : flowNode["results"]) {
            flow.results.push_back(parse_flow_reference(result));
        }
        mFlowNodes.push_back(std::move(flow));
    }

    for (const auto& messageNode : in["mCustomMessages"]) {
        CustomMessage message{};
        message.group = messageNode["group"].as<u8>();
        message.name = messageNode["name"].as<std::string>();
        message.style = parse_message_style(messageNode["style"]);
        for (const auto& textNode : messageNode["text"]) {
            const auto language = randomizer::stringToLanguage(textNode.first.as<std::string>());
            const auto binary = textNode.second.as<YAML::Binary>();
            message.text[language] =
                std::string(reinterpret_cast<const char*>(binary.data()), binary.size());
        }
        mCustomMessages.push_back(std::move(message));
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

    // Entrance Overrides
    for (const auto& entranceNode : in["mEntranceOverrides"]) {
        const auto key = std::bit_cast<EntranceOverride>(entranceNode.first.as<uint64_t>());;
        const auto override = std::bit_cast<EntranceOverride>(entranceNode.second.as<uint64_t>());
        this->mEntranceOverrides[key] = override;
    }

    // Return to Place Overrides
    for (const auto& entranceNode : in["mReturnToPlaceOverrides"]) {
        auto key = entranceNode.first.as<int>();
        const auto override = std::bit_cast<EntranceOverride>(entranceNode.second.as<uint64_t>());
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
    mHasPendingToDChange = false;
    // g_customMenuRing._initialize();
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

    // Make sure Z button isn't dimmed
    if (dComIfGs_isEventBit(MIDNA_ACCOMPANIES_WOLF) && dMeter2Info_getMeterClass() &&
        dMeter2Info_getMeterClass()->getMeterDrawPtr() &&
        dMeter2Info_getMeterClass()->getMeterDrawPtr()->getButtonZAlpha() != 1.f)
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

void randomizer_dontOverrideNextEntrance() {
    g_randomizerState.mTryOverrideNextEntrance = false;
}

void randomizer_checkAndOverrideEntranceData(const char*& stageName, s8& roomNo, s16& pointNo, s8& mapLayer, u32& lastMode) {
    if (!g_randomizerState.mTryOverrideNextEntrance) {
        mods::log::debug("Skipping next entrance override");
        g_randomizerState.mTryOverrideNextEntrance = true;
        return;
    }

    RandomizerContext::EntranceOverride override = {
        .stageId = static_cast<u8>(getStageID(stageName)), .roomNo = roomNo, .mapLayer = mapLayer, .pointNo = static_cast<s16>(pointNo)};

    // Debugging: Dump mEntranceOverrides
    // for (const auto& [key,val] : randomizer_GetContext().mEntranceOverrides) {
    //     mods::log::info("({},{},{},{}) -> ({},{},{},{})",key.stageId,key.roomNo,key.mapLayer,key.pointNo,val.stageId,val.roomNo,val.mapLayer,val.pointNo);
    // }

    // mods::log::info("Original Stage:{}, {}, {}, {}",stageName,roomNo,pointNo,mapLayer);

    auto it = randomizer_GetContext().mEntranceOverrides.find(override);
    if (it == randomizer_GetContext().mEntranceOverrides.end()) {
        // If the specific layer specified isn't found, search the overrides again for layer -1
        // This is used to resolve issues where a transition (like the door to the past) requests
        // A specific layer to load
        override.mapLayer = -1;
        it = randomizer_GetContext().mEntranceOverrides.find(override);
    }

    if (it != randomizer_GetContext().mEntranceOverrides.end()) {
        const auto& newOverride = it->second;
        stageName = allStages[newOverride.stageId];
        pointNo = newOverride.pointNo;
        roomNo = newOverride.roomNo;
        mapLayer = newOverride.mapLayer;

        // Override the lastSceneMode if we are coming from a wolf dig hole
        if ((lastMode&0xF) == 9) {
            lastMode = (lastMode&0xFFFFFFF0);
        }

        // Check if we are going through a randomized entrance with epona, reset the lastMode
        // Todo: Create a list of entrances where epona is allowed and don't reset the lastMode if
        // Epona can be rode there
        if ((lastMode&0xF) == 2 || (lastMode&0xF) == 8) {
            lastMode = (lastMode&0xFFFFFFF0);
        }

        // Override the spawn link will go to if we are going through the Telma's bar entrance during
        // Lanayru twilight. The door is ajar in this layer, so going through the normal spawn doesn't work.
        if (!dComIfGs_isDarkClearLV(2) && newOverride.stageId == 53 && roomNo == 3 && pointNo == 1) {
            pointNo = 30;
        }

        // If this is our starting spawn, also set it as our return place for saving
        if (override.stageId == getStageID("F_SP103") && override.roomNo == 1 && override.pointNo == 1) {
            auto& returnPlace = dComIfGs_getSaveData()->mPlayer.getPlayerReturnPlace();
            returnPlace.set(stageName, roomNo, pointNo);
        }

        // mods::log::info("New Stage:{}, {}, {}, {}",stageName,roomNo,pointNo,mapLayer);
    }
}

void randomizer_setTempFlag(const RandomizerContext::itemLocationData& data) {
    if (data.flag == 0xFFFF) {
        return;
    }

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

        // Don't attempt to override returning to a dungeon entrance
        randomizer_dontOverrideNextEntrance();
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

u8 randomizer_getRandomFoolishItemModelID(std::string_view checkName) {
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

    static constexpr char separator = '\0';
    const std::string& seedHash = randomizer_GetContext().mHash;
    u32 hash = randomizer::utility::crc32(seedHash.data(), seedHash.size());
    hash = randomizer::utility::crc32(&separator, sizeof(separator), hash);
    hash = randomizer::utility::crc32(checkName.data(), checkName.size(), hash);
    const u8 selectedModel = foolishItemModels[hash % foolishItemModels.size()];
    return verifyProgressiveItem(selectedModel);
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

RandomizerContext WriteSeedData(randomizer::logic::world::World* world) {
    RandomizerContext randoData{};

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
        // Keyed by u32 of 0xFF0000 (stage index), 0x00FF00 (room), and 0x0000FF (original shop item)
        if (location->HasCategories("Shop") && world->Setting("Shop Items") == "On") {
            for (const auto& shopNode : metaData["Shop"]) {
                u8 stage = shopNode["Stage"].as<u8>();
                u8 room = shopNode["Room"].as<u8>();
                u8 originalItem = shopNode["Item"].as<u8>();
                u32 key = (stage << 16) | (room << 8) | originalItem;
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
    std::unordered_map<std::string, u8> objectFlowGroups{};
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
                RandomizerContext::ActorData actorData{};
                if (objectNode["flow"]) {
                    actorData.flow = objectNode["flow"].as<std::string>();
                    const int stageId = getStageID(stageName.c_str());
                    if (stageId < 0 ||
                        static_cast<size_t>(stageId) >= std::size(allStageMessageGroups))
                    {
                        throw std::runtime_error("Unknown stage for flow-bearing actor: " + stageName);
                    }
                    const u8 group = allStageMessageGroups[stageId];
                    const auto [found, inserted] = objectFlowGroups.emplace(actorData.flow, group);
                    if (!inserted && found->second != group) {
                        throw std::runtime_error(
                            "Actor flow is referenced from multiple message groups: " + actorData.flow);
                    }
                    object.base.angle.x = 0;
                }
                // If we're patching this object, Then override the object with whatever parts are being patched
                // and add that patch data to our actorData
                if (action == "patch") {
                    parseObjPatchData(object, objectNode["patch"]);
                    actorData.bytes.resize(objDataSize);
                    std::memcpy(actorData.bytes.data(), &object, objDataSize);
                } else if (action == "add") {
                    // If we're adding the object, add it's regular data to the actorData
                    actorData.bytes.resize(objDataSize);
                    std::memcpy(actorData.bytes.data(), &object, objDataSize);
                } else if (action == "delete") {
                    // If we're deleting this actor, give it a specific size to indicate we're deleting it
                    actorData.bytes.resize(RandomizerContext::OBJ_DELETE_SIZE);
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

    auto source_reference = [](const YAML::Node& node) {
        RandomizerContext::FlowReference reference{};
        const auto value = node.as<std::string>();
        if (const auto numeric = randomizer::utility::str::toInt(value); numeric.has_value()) {
            if (numeric.value() < 0 || numeric.value() > 0xffff) {
                throw std::runtime_error("Flow reference is outside the 16-bit range: " + value);
            }
            reference.nativeId = static_cast<u16>(numeric.value());
        } else {
            reference.name = value;
        }
        return reference;
    };

    std::unordered_map<std::string, std::unordered_set<u8>> customMessageGroups{};
    std::unordered_map<std::string, std::string> splitMessageStyles{};
    auto flowPatches = LOAD_EMBED_YAML(RANDO_DATA_PATH "flow_patches.yaml");
    for (const auto& groupNode : flowPatches) {
        const auto groupName = groupNode.first.as<std::string>();
        const bool customSection = groupName == "custom";
        const u8 defaultGroup = customSection ? 0 : groupNode.first.as<u8>();
        for (const auto& flowNode : groupNode.second) {
            if (flowNode["only if"] &&
                !world->EvaluateSettingCondition(flowNode["only if"].as<std::string>()))
            {
                continue;
            }

            RandomizerContext::FlowNode flow{};
            flow.type = parse_flow_node_type(flowNode["type"]);

            std::vector<u16> patchIndices{};
            if (customSection) {
                flow.name = flowNode["name"].as<std::string>();
                if (flowNode["group"]) {
                    flow.group = flowNode["group"].as<u8>();
                } else if (const auto found = objectFlowGroups.find(flow.name);
                           found != objectFlowGroups.end())
                {
                    flow.group = found->second;
                } else {
                    throw std::runtime_error("Custom flow has no message group: " + flow.name);
                }
            } else {
                flow.group = defaultGroup;
                if (flowNode["index"].IsSequence()) {
                    for (const auto& index : flowNode["index"]) {
                        patchIndices.push_back(index.as<u16>());
                    }
                } else {
                    patchIndices.push_back(flowNode["index"].as<u16>());
                }
            }

            if (flow.type == RandomizerContext::FlowNodeType::BRANCH) {
                flow.parameters = flowNode["parameters"].as<u32>();
                flow.operation = flowNode["query"].as<std::string>();
                for (const auto& result : flowNode["results"]) {
                    flow.results.push_back(source_reference(result));
                }
                if (flow.parameters > 0xffff || flow.results.empty() ||
                    flow.results.size() > 0xff)
                {
                    throw std::runtime_error("Flow branch has invalid parameters or results");
                }
            } else if (flow.type == RandomizerContext::FlowNodeType::EVENT) {
                flow.parameters = flowNode["parameters"].as<u32>();
                flow.operation = flowNode["event"].as<std::string>();
                flow.next = source_reference(flowNode["next"]);
            } else {
                flow.message = source_reference(flowNode["message"]);
                flow.next = source_reference(flowNode["next"]);
                if (!flow.message.nativeId.has_value()) {
                    customMessageGroups[flow.message.name].insert(flow.group);
                    if (world->GetTextDatabase().contains(flow.message.name)) {
                        auto& text = world->GetTextObject(flow.message.name);
                        // The game still owns textbox pagination, so preserve the existing
                        // per-message limit while representing overflow as ordinary flow nodes.
                        const auto extraText = text.SplitToFitTextLimits();
                        if (!extraText.empty()) {
                            const auto originalNext = flow.next;
                            std::vector<std::string> extraNames{};
                            for (size_t i = 0; i < extraText.size(); ++i) {
                                auto extraName = flow.message.name + std::to_string(i + 1);
                                world->AddNewText(extraName) = extraText[i];
                                splitMessageStyles[extraName] = flow.message.name;
                                customMessageGroups[extraName].insert(flow.group);
                                extraNames.push_back(std::move(extraName));
                            }
                            flow.next = {.name = extraNames.front()};
                            for (size_t i = 0; i < extraNames.size(); ++i) {
                                RandomizerContext::FlowNode extraFlow{
                                    .type = RandomizerContext::FlowNodeType::MESSAGE,
                                    .group = flow.group,
                                    .name = extraNames[i],
                                    .message = {.name = extraNames[i]},
                                    .next = i + 1 < extraNames.size() ?
                                                RandomizerContext::FlowReference{
                                                    .name = extraNames[i + 1]} :
                                                originalNext,
                                };
                                randoData.mFlowNodes.push_back(std::move(extraFlow));
                            }
                        }
                    }
                }
            }

            if (customSection) {
                randoData.mFlowNodes.push_back(std::move(flow));
            } else {
                for (const u16 patchIndex : patchIndices) {
                    auto patch = flow;
                    patch.patchIndex = patchIndex;
                    randoData.mFlowNodes.push_back(std::move(patch));
                }
            }
        }
    }

    std::array<std::unordered_set<std::string>, 9> flowNames{};
    for (const auto& flow : randoData.mFlowNodes) {
        if (flow.group >= flowNames.size()) {
            throw std::runtime_error("Flow node has an invalid message group");
        }
        if (!flow.patchIndex.has_value() &&
            (flow.name.empty() || !flowNames[flow.group].insert(flow.name).second))
        {
            throw std::runtime_error("Custom flow name is empty or duplicated: " + flow.name);
        }
    }
    auto validate_flow_reference = [&](u8 group, const RandomizerContext::FlowReference& reference) {
        if (!reference.nativeId.has_value() &&
            (reference.name.empty() || !flowNames[group].contains(reference.name)))
        {
            throw std::runtime_error(fmt::format(
                "Unresolved flow reference in group {}: {}", group, reference.name));
        }
    };
    for (const auto& [name, group] : objectFlowGroups) {
        validate_flow_reference(group, {.name = name});
    }
    for (const auto& flow : randoData.mFlowNodes) {
        if (flow.type == RandomizerContext::FlowNodeType::BRANCH) {
            for (const auto& result : flow.results) {
                validate_flow_reference(flow.group, result);
            }
        } else {
            validate_flow_reference(flow.group, flow.next);
        }
    }

    auto parse_style = [](const YAML::Node& node) {
        RandomizerContext::MessageStyleData style{};
        if (!node) {
            return style;
        }
        auto set = [&](const char* key, auto& field) {
            if (node[key]) {
                field = node[key].as<std::remove_reference_t<decltype(field)>>();
            }
        };
        set("Event Label", style.eventLabelId);
        set("Speaker", style.speaker);
        set("Box Kind", style.boxKind);
        set("Draw Type", style.drawType);
        set("Box Position", style.boxPosition);
        set("Line Alignment", style.lineAlignment);
        set("Speaker Mood", style.speakerMood);
        set("Camera", style.cameraAttr);
        set("Talk Animation", style.talkAnim);
        set("Face Animation", style.faceAnim);
        set("Trailing Data", style.trailingData);
        return style;
    };

    auto textOverrides = LOAD_EMBED_YAML(RANDO_DATA_PATH "text/text_overrides.yaml");
    std::unordered_map<std::string, YAML::Node> textDefinitions{};
    for (const auto& overrideNode : textOverrides) {
        const auto name = overrideNode["Name"].as<std::string>();
        textDefinitions.emplace(name, overrideNode);
        if (!overrideNode["Message Id"] ||
            (overrideNode["Only If"] &&
                !world->EvaluateSettingCondition(overrideNode["Only If"].as<std::string>())))
        {
            continue;
        }
        const u8 group = overrideNode["Group"].as<u8>();
        const u16 messageId = overrideNode["Message Id"].as<u16>();
        const u32 key = static_cast<u32>(group) << 16 | messageId;
        for (const auto language : randomizer::supportedLanguages) {
            std::string text = world->GetTextDatabase().contains(name) ?
                                   world->GetText(name, randomizer::Text::STANDARD, language) :
                                   randomizer::getTextStr(name, randomizer::Text::STANDARD, language);
            randomizer::applyMessageCodes(text);
            randoData.mTextOverrides[language][key] = std::move(text);
        }
    }

    for (const auto& [name, groups] : customMessageGroups) {
        const auto styleName = splitMessageStyles.contains(name) ? splitMessageStyles[name] : name;
        const auto definition = textDefinitions.find(styleName);
        if (definition == textDefinitions.end()) {
            throw std::runtime_error("Custom flow message has no text definition: " + styleName);
        }
        for (const u8 group : groups) {
            RandomizerContext::CustomMessage message{
                .group = group,
                .name = name,
                .style = parse_style(definition->second["Style"]),
            };
            for (const auto language : randomizer::supportedLanguages) {
                std::string text = world->GetTextDatabase().contains(name) ?
                                       world->GetText(name, randomizer::Text::STANDARD, language) :
                                       randomizer::getTextStr(name, randomizer::Text::STANDARD, language);
                randomizer::applyMessageCodes(text);
                message.text[language] = std::move(text);
            }
            randoData.mCustomMessages.push_back(std::move(message));
        }
    }

    // Vanilla Return to Place Overrides. Will need to change when boss/miniboss ER is implemented
    static const std::vector<std::pair<std::vector<int>, RandomizerContext::EntranceOverride>> defaultPlaceOverrides{
        {{Forest_Temple, Ook, Diababa},                      {.stageId = Forest_Temple, .roomNo = 22, .mapLayer = -1, .pointNo = 0}},
        {{Goron_Mines, Dangoro, Fyrus},                      {.stageId = Goron_Mines, .roomNo = 1, .mapLayer = -1, .pointNo = 0}},
        {{Lakebed_Temple, Deku_Toad, Morpheel},              {.stageId = Lakebed_Temple, .roomNo = 0, .mapLayer = -1, .pointNo = 0}},
        {{Arbiters_Grounds, Death_Sword, Stallord},          {.stageId = Arbiters_Grounds, .roomNo = 0, .mapLayer = -1, .pointNo = 0}},
        {{Snowpeak_Ruins, Darkhammer, Blizzeta},             {.stageId = Snowpeak_Ruins, .roomNo = 0, .mapLayer = -1, .pointNo = 0}},
        {{Temple_of_Time, Darknut, Armogohma},               {.stageId = Temple_of_Time, .roomNo = 0, .mapLayer = -1, .pointNo = 0}},
        {{City_in_the_Sky, Aeralfos, Argorok},               {.stageId = City_in_the_Sky, .roomNo = 0, .mapLayer = -1, .pointNo = 3}},
        {{Palace_of_Twilight, Phantom_Zant_1,
                Phantom_Zant_2, Zant_Main_Room, Zant_Fight},     {.stageId = Palace_of_Twilight, .roomNo = 0, .mapLayer = -1, .pointNo = 0}},
        {{Hyrule_Castle, Ganondorf_Castle, Ganondorf_Field}, {.stageId = Hyrule_Castle, .roomNo = 11, .mapLayer = -1, .pointNo = 0}},
    };

    // Return to Place Overrides
    for (const auto& [stages, returnPlace] : defaultPlaceOverrides) {
        for (auto stage : stages) {
            randoData.mReturnToPlaceOverrides[stage] = returnPlace;
        }
    }

    // Apply entrance randomization
    if (world->AnyEntranceRandomizerEnabled()) {
        for (const auto& entrance : world->GetShuffledEntrances()) {
            randomizer::logic::entrance::Entrance* replacesEntrance = entrance->GetReplaces();
            RandomizerContext::EntranceOverride forward = {.stageId = entrance->GetStageId(), .roomNo = entrance->GetRoomNo(), .mapLayer = entrance->GetLayerNo(), .pointNo = entrance->GetPointNo()};
            RandomizerContext::EntranceOverride replaces = {.stageId = replacesEntrance->GetStageId(), .roomNo = replacesEntrance->GetRoomNo(), .mapLayer = replacesEntrance->GetLayerNo(), .pointNo = replacesEntrance->GetPointNo()};
            if (forward.stageId == 0xFF || replaces.stageId == 0xFF || forward.roomNo == -1 || replaces.roomNo == -1) {
                continue;
            }
            randoData.mEntranceOverrides[forward] = replaces;

            // Set overrides for all coupled entrances
            for (const auto& point : entrance->getCoupledEntrances()) {
                RandomizerContext::EntranceOverride coupled = {.stageId = entrance->GetStageId(), .roomNo = entrance->GetRoomNo(), .mapLayer = entrance->GetLayerNo(), .pointNo = point};
                randoData.mEntranceOverrides[coupled] = replaces;
            }
        }
    }

    if (world->Setting("Mirror Chamber Access") == "Closed") {
        // Set exiting the Arbiter's Grounds Boss Room to spawn at the Arbiter's Grounds entrance
        // if mirror chamber access is closed
        RandomizerContext::EntranceOverride original = {
            .stageId = StageIDs::Mirror_Chamber,
            .roomNo = 4,
            .mapLayer = -1,
            .pointNo = 0,
        };

        RandomizerContext::EntranceOverride override = {
            .stageId = StageIDs::Bulblin_Camp,
            .roomNo = 3,
            .mapLayer = -1,
            .pointNo = 3,
        };

        // Check if we are already overriding the bulblin camp entrance, and correctly override the entrance
        const auto& it = randoData.mEntranceOverrides.find(override);
        randoData.mEntranceOverrides[original] = (it != randoData.mEntranceOverrides.end()) ? it->second : override;
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
