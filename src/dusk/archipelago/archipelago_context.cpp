#include <dusk/archipelago/archipelago_context.hpp>

#include <array>
#include <deque>
#include <thread>
#include <unordered_map>

#include "Archipelago.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_map_path_dmap.h"
#include "d/actor/d_a_alink.h"
#include "dusk/config.hpp"
#include "dusk/logging.h"
#include "dusk/randomizer/game/tools.h"
#include "dusk/randomizer/game/verify_item_functions.h"
#include "dusk/randomizer/generator/logic/hints.hpp"
#include "dusk/ui/rando_config.hpp"
#include "dusk/ui/ui.hpp"

namespace dusk::archi
{

static constexpr int ARCHI_ITEM_OFFSET = 2320000;

struct SettingsNameConvert {
    static constexpr std::string kDefaultYes = "On";
    static constexpr std::string kDefaultNo = "Off";

    std::string apName;
    std::string dusklightName;
    std::vector<std::pair<std::string, std::string>> optionsConvert;

    const std::string& tryGetOptionConvert(const std::string& option) const {
        if (optionsConvert.empty()) {
            if (option == "Yes" || option == "True" || option == "true")
                return kDefaultYes;
            if (option == "No" || option == "False" || option == "false")
                return kDefaultNo;
            return option;
        }

        for (const auto& value : optionsConvert) {
            if (value.first == option) {
                return value.second;
            }
        }
        return option;
    }
};

static auto sArchiSettingToDusklight = std::to_array<SettingsNameConvert>({
    {"", ""},
    {"Golden Bugs Shuffled", "Golden Bugs"},
    {"Sky Chracters Shuffled", "Sky Characters"},
    {"NPC Items Shuffled", "Gifts From NPCs"},
    {"Shop Items Shuffled", "Shop Items"},
    {"Hidden Skills Shuffled", "Hidden Skills"},
    {"Skip Prologue", "Skip Prologue"},
    {"Faron Twilight Cleared", "Faron Twilight Cleared"},
    {"Eldin Twilight Cleared", "Eldin Twilight Cleared"},
    {"Lanayru Twilight Cleared", "Lanayru Twilight Cleared"},
    {"Skip MDH", "Skip Midna's Desparate Hour"},
    {"Open Map", "Unlock Map Regions"},
    {"Increase Wallet", "Logic Increase Wallet Capacity"},
    {"Transform Anywhere", "Logic Transform Anywhere"},
    {"Bonks do Damage", "Bonks Do Damage"},
    {"Lakebed Entrance Requirements", "Lakebed Does Not Require Water Bombs"},
    {"Arbiters Grounds Entrance Requirements", "Arbiters Does Not Require Bulblin Camp"},
    {"Snowpeak Entrance Requirements", "Snowpeak Does Not Require Reekfish Scent"},
    {"City in the Sky Entrance Requirements", "City Does Not Require Filled Skybook"},
    {"Goron Mines Entrance Requirements", "Goron Mines Entrance"},
    {"Palace of Twilight Requirements", "Palace of Twilight Requirements"},
    {"Faron Woods Logic", "Faron Woods Logic"},
{"Starting ToD", "Starting Time of Day"},
   {"Skip Major Cutscenes", "Skip Major Cutscenes"},
{"Skip Minor Cutscenes", "Skip Minor Cutscenes"},
   {"Open Door of Time", "Open Door of Time"},

    {"Dungeon Rewards Progression", "Dungeon Rewards Can Be Anywhere", {
         // these two are functionally identical in terms of tracker logic, so treat it as such
         {"Anything", "On"},
         {"Any Progressive", "On"},
         {"Vanilla", "Off"},
     }},
    {"Small Key Settings", "Small Keys", {
         {"Startwith", "Keysy"},
     }},
    {"Big Key Settings", "Big Keys", {
         {"Startwith", "Keysy"},
     }},
    {"Map and Compass Settings", "Maps and Compasses", {
         {"Startwith", "Start With"},
     }},
    {"Trap Frequency", "Trap Item Frequency", {
         {"No Traps", "None"},
     }},
    {"Damage Magnification", "Logic Damage Multiplier", {
         {"Ohko", "OHKO"},
     }},
    {"Logic Settings", "Logic Rules", {
         {"Glitchless", "All Locations Reachable"},
         {"Glitched", "Beatable Only"},
    }},
    {"Poes Shuffled", "Poe Souls", {
        {"Yes", "All"},
        {"No", "Vanilla"},
        {"True", "All"},
        {"False", "Vanilla"}
    }}
});

ArchipelagoContext& instance() {
    static ArchipelagoContext instance;
    return instance;
}

// Stage code -> display name, mirroring the apworld's ClientUtils.py STAGE_TO_NAME. poptracker's
// autotracking keys off these exact strings, so keep it in sync with the apworld client.
static const std::unordered_map<std::string, std::string> sStageCodeToName = {
    {"D_MN01", "Lakebed Temple"},
    {"D_MN04", "Goron Mines"},
    {"D_MN05", "Forest Temple"},
    {"D_MN06", "Temple of Time"},
    {"D_MN07", "City in the Sky"},
    {"D_MN08", "Palace of Twilight"},
    {"D_MN09", "Hyrule Castle"},
    {"D_MN10", "Arbiter's Grounds"},
    {"D_MN11", "Snowpeak Ruins"},
    {"D_SB00", "Ice Block Cave"},
    {"D_SB01", "Cave or Ordeals"},
    {"D_SB02", "Kakariko Gorge Lantern Cave"},
    {"D_SB03", "Lake Hylia Lantern Cave"},
    {"D_SB04", "Goron Stockcave"},
    {"D_SB05", "Grotto 1"},
    {"D_SB06", "Grotto 2"},
    {"D_SB07", "Grotto 3"},
    {"D_SB08", "Grotto 4"},
    {"D_SB09", "Grotto 5"},
    {"D_SB10", "Faron Woods Cave"},
    {"F_SP00", "Ordon Ranch"},
    {"F_SP102", "Title Screen"},
    {"F_SP103", "Ordon Village"},
    {"F_SP104", "Ordon Spring"},
    {"F_SP108", "Faron Woods"},
    {"F_SP109", "Kakariko Village"},
    {"F_SP110", "Death Mountain"},
    {"F_SP111", "Kakariko Graveyard"},
    {"F_SP112", "Zora's River"},
    {"F_SP113", "Zora's Domain"},
    {"F_SP114", "Snowpeak"},
    {"F_SP115", "Lake Hylia"},
    {"F_SP116", "Hyrule Castle Town"},
    {"F_SP117", "Sacred Grove"},
    {"F_SP118", "Bulblin Camp"},
    {"F_SP121", "Hyrule Field"},
    {"F_SP122", "Castle Town Fields"},
    {"F_SP123", "King Bulbin Fights"},
    {"F_SP124", "Gerudo Desert"},
    {"F_SP125", "Mirror Chamber"},
    {"F_SP126", "Upper Zora's River"},
    {"F_SP127", "Fishing Pond"},
    {"F_SP128", "Hidden Village"},
    {"F_SP200", "Shade's Realm"},
    {"R_SP01", "Ordon Interiors"},
    {"R_SP107", "Sewers"},
    {"R_SP108", "Coro's Lantern Shop"},
    {"R_SP109", "Kakriko Interiors"},
    {"R_SP110", "Death Mountain Sumo Hall"},
    {"R_SP116", "Hyrule Castle Town Interiors (Telma's bar)"},
    {"R_SP127", "Hena's Cabin"},
    {"R_SP128", "Impaz's House"},
    {"R_SP160", "Hyrule Castle Town Interiors (Jovani, Agitha, Shops, etc)"},
    {"R_SP161", "Star Tent"},
    {"R_SP209", "Sanctuary Basement"},
    {"R_SP300", "Light Arrow cutscenes"},
    {"R_SP301", "Hyrule Castle cutscenes"},
    {"S_MV000", "Deleted"},
};

// Reads the null-or-length-terminated 8 byte stage code out of the live game state.
static std::string ReadCurrentStageCode() {
    const char* raw = dComIfGp_getStartStageName();
    if (raw == nullptr) {
        // null between stages / during load
        return {};
    }
    std::string code;
    for (int i = 0; i < 8 && raw[i] != '\0'; ++i) {
        code.push_back(raw[i]);
    }
    return code;
}

// Resolves a raw stage code to its AP display name, or nullptr if unknown. Dungeon boss-room
// sub-stages ("D_MN01A") are folded to the base 6-char dungeon code first, as ClientUtils.py does.
static const std::string* ResolveStageName(std::string stageCode) {
    if (stageCode.rfind("D_MN", 0) == 0 && stageCode.size() > 6) {
        stageCode.resize(6);
    }

    auto it = sStageCodeToName.find(stageCode);
    if (it == sStageCodeToName.end())
        return nullptr;
    return &it->second;
}

static std::string ToJsonStringLiteral(const std::string& value) {
    std::string out = "\"";
    for (char c : value) {
        if (c == '"' || c == '\\')
            out.push_back('\\');
        out.push_back(c);
    }
    out.push_back('"');
    return out;
}

// AP_BulkSetServerData() stashes &request->status and AP_CommitServerData() writes back through it
// later, so the request must outlive the commit. The caller keeps these in a std::deque (stable
// element addresses) rather than a local that dies on helper return.
struct PendingServerData {
    AP_SetServerDataRequest request;
    std::string jsonDefault;
    std::string jsonValue;
    int intDefault = 0;
    int intValue = 0;
};

static void SetTrackedIntDataStorage(std::deque<PendingServerData>& pending,
    const std::string& key, int value, int defaultValue) {
    auto& p = pending.emplace_back();
    p.intValue = value;
    p.intDefault = defaultValue;
    p.request.key = key;
    p.request.type = AP_DataType::Int;
    p.request.want_reply = false;
    p.request.default_value = &p.intDefault;
    AP_DataStorageOperation replaceOp;
    replaceOp.operation = "replace";
    replaceOp.value = &p.intValue;
    p.request.operations = {replaceOp};
    AP_BulkSetServerData(&p.request);
}

static void SetTrackedStringDataStorage(std::deque<PendingServerData>& pending,
    const std::string& key, const std::string& value, const std::string& defaultValue) {
    auto& p = pending.emplace_back();
    p.jsonDefault = ToJsonStringLiteral(defaultValue);
    p.jsonValue = ToJsonStringLiteral(value);
    p.request.key = key;
    p.request.type = AP_DataType::Raw;
    p.request.want_reply = false;
    p.request.default_value = &p.jsonDefault;
    AP_DataStorageOperation replaceOp;
    replaceOp.operation = "replace";
    replaceOp.value = &p.jsonValue;
    p.request.operations = {replaceOp};
    AP_BulkSetServerData(&p.request);
}

// Sent as bare JSON literals (true/false), not JSON strings, matching the apworld client and what
// poptracker's autotracking expects.
static void SetTrackedBoolDataStorage(std::deque<PendingServerData>& pending,
    const std::string& key, bool value, bool defaultValue) {
    auto& p = pending.emplace_back();
    p.jsonDefault = defaultValue ? "true" : "false";
    p.jsonValue = value ? "true" : "false";
    p.request.key = key;
    p.request.type = AP_DataType::Raw;
    p.request.want_reply = false;
    p.request.default_value = &p.jsonDefault;
    AP_DataStorageOperation replaceOp;
    replaceOp.operation = "replace";
    replaceOp.value = &p.jsonValue;
    p.request.operations = {replaceOp};
    AP_BulkSetServerData(&p.request);
}

// Howling Stones: byte+bitmask offsets into dComIfGs_getSaveInfo(), from the apworld's
// ClientUtils.py. Scents/quest-items/Memory Reward don't read correctly this way on this build;
// they use sEventBitTrackingEntries below instead.
struct FlagTrackingEntry {
    const char* key;
    std::size_t offset;
    u8 mask;
};

static const std::array<FlagTrackingEntry, 6> sFlagTrackingEntries = {{
    {"Death Mountain Stone", 0x82A, 0x80},
    {"Zora River Stone", 0x82A, 0x40},
    {"Sacred Grove Stone", 0x82A, 0x20},
    {"Lake Hylia Stone", 0x82A, 0x10},
    {"Snowpeak Stone", 0x82A, 0x8},
    {"Hidden Village Stone", 0x82A, 0x4},
}};

static bool ReadSaveInfoFlag(std::size_t offset, u8 mask) {
    auto* base = reinterpret_cast<const u8*>(dComIfGs_getSaveInfo());
    return (base[offset] & mask) != 0;
}

// Scents/quest-items/Memory Reward, read via dComIfGs_isEventBit(rawFlagValue). Most values are
// cross-checked against randomizer/generator/data/startflags.yaml; "Youth Scent", "Ilias Scent"
// and "Memory Reward" are inferred from d_save_bit_labels.inc and lower-confidence.
struct EventBitTrackingEntry {
    const char* key;
    u16 flagValue;
};

static const std::array<EventBitTrackingEntry, 10> sEventBitTrackingEntries = {{
    {"Youth Scent", 0x2240},
    {"Ilias Scent", 0x2220},
    {"Medicine Scent", 0x2F04},
    {"ReekFish Scent", 0x6120},
    {"Poe Scent", 0x6210},
    {"Renados letter", 0x0F80},
    {"Telmas Invoice", 0x2710},
    {"Wooden Statue", 0x2204},
    {"Ilias Charm", 0x2280},
    {"Memory Reward", 0x2320},
}};

static bool ReadEventBit(u16 flagValue) {
    return dComIfGs_isEventBit(flagValue) != 0;
}

// The 8 boss-defeated keys. clearFlagValue is the vanilla dungeon-clear event bit, which
// UpdateFlagTrackingData() reads (see the note there).
struct BossTrackingEntry {
    const char* key;
    u16 clearFlagValue;
};

static const std::array<BossTrackingEntry, 8> sBossTrackingEntries = {{
    {"Diababa Defeated", 0x0602},
    {"Fyrus Defeated", 0x0701},
    {"Morpheel Defeated", 0x0904},
    {"Stallord Defeated", 0x2010},
    {"Blizzeta Defeated", 0x2008},
    {"Armogohma Defeated", 0x2004},
    {"Argorok Defeated", 0x2002},
    {"Zant Defeated", 0x4680},
}};

const SettingsNameConvert& GetAPSettingNameConvert(const std::string& apSettingName) {
    for (const auto& entry : sArchiSettingToDusklight) {
        if (entry.apName == apSettingName)
            return entry;
    }
    return sArchiSettingToDusklight[0];
}

const char* getMessageTypeName(AP_MessageType type) {
    switch (type) {
    case AP_MessageType::Plaintext:
        return "Plaintext";
    case AP_MessageType::ItemSend:
        return "ItemSend";
    case AP_MessageType::ItemRecv:
        return "ItemRecv";
    case AP_MessageType::Hint:
        return "Hint";
    case AP_MessageType::Countdown:
        return "Countdown";
    default:
        return nullptr;
    }
}

void ParseMessageData() {
    auto msg = AP_GetLatestMessage();
    if (msg == nullptr) {
        // race with the pending-message poll
        return;
    }

    switch (msg->type) {
    case AP_MessageType::ItemSend: {
        auto sendMsg = (AP_ItemSendMessage*)msg;
        ui::push_toast({
            .title = "Item Sent",
            .content = fmt::format("Sent {} to {}", sendMsg->item, sendMsg->recvPlayer),
            .duration = std::chrono::seconds(3),
        });

        DuskLog.info("[{}] {}", getMessageTypeName(msg->type), msg->text);
        break;
    }
    case AP_MessageType::ItemRecv: {
        auto recvMsg = (AP_ItemRecvMessage*)msg;

        ui::push_toast({
            .title = "Item Received",
            .content = fmt::format("Got {} From {}", recvMsg->item, recvMsg->sendPlayer),
            .duration = std::chrono::seconds(3),
        });
        // fallthrough for debug logging text contents
    }
    case AP_MessageType::Plaintext:
    case AP_MessageType::Hint:
    case AP_MessageType::Countdown:
        DuskLog.info("[{}] {}", getMessageTypeName(msg->type), msg->text);
        break;
    default:
        DuskLog.warn("Unknown message type! Type: {}", fmt::underlying(msg->type));
        break;
    }

    AP_ClearLatestMessage();
}

void ArchipelagoContext::LoadTempItemInfo() {
    auto itemDataTree = LOAD_EMBED_YAML(RANDO_DATA_PATH "items.yaml");
    for (const auto& itemNode : itemDataTree) {
        if (!itemNode["APItemId"]) {
            DuskLog.warn("Item {} missing APItemId field!", itemNode["Name"].as<std::string>());
            continue;
        }
        auto apItemId = itemNode["APItemId"].as<int>();

        if (apItemId == -1)
            continue;

        auto id = itemNode["Id"].as<int>();
        auto importance = randomizer::logic::item::ImportanceFromStr(itemNode["Importance"].as<std::string>());
        auto itemName = itemNode["Name"].as<std::string>();

        m_apItemToGameItem[apItemId] = {
            id,
            importance,
            itemName
        };
    }

    // add temporary replacement IDs for items not included in the base rando

    m_apItemToGameItem[16] = {  // Water Bombs (3)
        0x16,
        randomizer::logic::item::Importance::JUNK,
        "Water Bombs 5"
    };

    m_apItemToGameItem[20] = {  // Bomblings (3)
        0x1A,
        randomizer::logic::item::Importance::JUNK,
        "Bomblings 5"
    };
}

void ArchipelagoContext::LoadTempLocationInfo() {
    auto locDataTree = LOAD_EMBED_YAML(RANDO_DATA_PATH "locations.yaml");
    for (const auto& locNode : locDataTree) {
        const auto& metadata = locNode["Metadata"];
        auto locationName =  locNode["Name"].as<std::string>();

        if (!metadata.IsMap()) {
            DuskLog.warn("Location {} missing correct Metadata field!", locationName);
            continue;
        }

        if (!metadata["APLocationId"]) {
            DuskLog.warn("Location {} missing APLocationId field!", locationName);
            continue;
        }

        auto apLocationId = metadata["APLocationId"].as<int>();

        if (apLocationId == -1)
            continue;

        m_apLocToGameLoc.push_back({
            apLocationId,
            locationName
        });
    }
}

void ArchipelagoContext::itemRecvImpl(int id, bool notify) {
    if (!m_apItemToGameItem.contains(id)) {
        DuskLog.warn("[AP] Got an invalid Item Id: {}", id);
        return;
    }

    m_isAllowUpdateLocations = true; // guards against triggering UpdateCheckedLocations

    auto& item = m_apItemToGameItem[id];

    if (notify && item.importance == randomizer::logic::item::Importance::MAJOR) {
        DuskLog.info("[AP] Adding Item: {}", item.itemName);
        auto verifiedId = verifyProgressiveItem(item.itemId);
        ApItemLog.info("recv: '{}' (apId {} -> gameItemId {} -> verified {}), routing to event queue",
            item.itemName, id, item.itemId, verifiedId);
        g_randomizerState.addItemToEventQueue(verifiedId);
    }else {
        DuskLog.info("[AP] Silently Adding Item: {}", item.itemName);
        ApItemLog.info("recv: '{}' (apId {} -> gameItemId {}), granting directly via execItemGet",
            item.itemName, id, item.itemId);
        execItemGet(item.itemId);
    }

    m_isAllowUpdateLocations = false;
}

int ArchipelagoContext::getItemIdFromApId(int apId) {
    if (!m_apItemToGameItem.contains(apId)) {
        DuskLog.warn("Got an invalid Item Id: {}", apId);
        return -1;
    }

    auto& item = m_apItemToGameItem[apId];

    return item.itemId;
}

std::string ArchipelagoContext::getLocationNameFromApId(int apId) const {
    for (const auto& entry : m_apLocToGameLoc) {
        if (entry.apId == apId)
            return entry.locName;
    }
    return "";
}

bool ArchipelagoContext::tryKillPlayer() {
    if (!m_isNeedPlayerDeath)
        return false;

    auto linkActor = daAlink_getAlinkActorClass();

    if (!linkActor)
        return false;

    switch (linkActor->mProcID) {
        case daAlink_c::PROC_WAIT:
        case daAlink_c::PROC_TIRED_WAIT:
        case daAlink_c::PROC_MOVE:
        case daAlink_c::PROC_WOLF_WAIT:
        case daAlink_c::PROC_WOLF_TIRED_WAIT:
        case daAlink_c::PROC_WOLF_MOVE:
        case daAlink_c::PROC_ATN_MOVE:
        case daAlink_c::PROC_WOLF_ATN_AC_MOVE: {
            // Check if link is currently in a cutscene
            if (linkActor->checkEventRun())
                break;

            // Ensure that link is not currently in a message-based event.
            if (linkActor->getEventId() != 0)
                break;

            dComIfGs_setLife(0);

            m_isNeedPlayerDeath = false;

            return true;
        }
        default:
            break;
    }

    return false;
}

ArchipelagoContext::ArchipelagoContext() = default;

void ArchipelagoContext::SetServerIp(const std::string_view& ip, int file) {
    getSettings().archipelago.savesServerIP[file].setValue(std::string(ip));
}

void ArchipelagoContext::SetSlotName(const std::string_view& name, int file) {
    getSettings().archipelago.savesSlotName[file].setValue(std::string(name));
}

void ArchipelagoContext::SetPassword(const std::string_view& pass, int file) {
    getSettings().archipelago.savesServerPass[file].setValue(std::string(pass));
}

const std::string& ArchipelagoContext::GetServerIp(int file) {
    return getSettings().archipelago.savesServerIP[file].getValue();
}

const std::string& ArchipelagoContext::GetSlotName(int file) {
    return getSettings().archipelago.savesSlotName[file].getValue();
}

const std::string& ArchipelagoContext::GetPassword(int file) {
    return getSettings().archipelago.savesServerPass[file].getValue();
}

std::string ArchipelagoContext::GetArchipelagoSeedName() {
    if (IsConnected()) {
        auto& roomInfo = instance().m_roomInfo;
        if (roomInfo.seed_name.empty()) {
            DuskLog.warn("Got an invalid Seed Name!");
        }
        return fmt::format("AP_{}", roomInfo.seed_name);
    }else {
        DuskLog.fatal("Archipelago was not connected when attempting to get seed name!");
    }
}

void ArchipelagoContext::GetSeedDirectoryPath(std::filesystem::path& outPath) {
    // Also require IsRoomInfoReady(): with an empty seed_name this would resolve to "archipelago/AP_",
    // a bogus directory with no dedup history. Leaving outPath untouched makes that visible to callers.
    if (IsConnected() && IsRoomInfoReady()) {
        outPath = ui::GetRandomizerPath() / "archipelago" / GetArchipelagoSeedName();
    }
}

bool ArchipelagoContext::IsSeedHashArchipelago(const std::string& seedStr) {
    return seedStr.starts_with("AP_");
}

bool ArchipelagoContext::IsCurrentSeedHash(const std::string& seedStr) {
    return GetArchipelagoSeedName() == seedStr;
}

bool ArchipelagoContext::ConnectToServer(int file, bool isBlocking) {
    config::Save();

    // m_locationItemInfo (and the collect-state snapshot it seeds from) is never otherwise cleared,
    // so a reconnect within the same process run would see the prior session's stale location data.
    // IsReceivedLocationScouts() (`!m_locationItemInfo.empty()`) would then report true immediately,
    // skipping the wait for the new session's real scout response and letting GenerateLocalWorldData()
    // proceed against last session's location/item info. m_receivedItemsQueue has the same problem -
    // items queued via HandleItemReceived() before a disconnect (e.g. the scout-retry loop's
    // Disconnect/reconnect cycle) would otherwise survive into the new session and get drained
    // against data from a different connection attempt.
    {
        std::lock_guard<std::mutex> lock(instance().m_locationInfoMutex);
        instance().m_locationItemInfo.clear();
        instance().m_initLocationCollectState.clear();
    }
    {
        std::lock_guard<std::mutex> lock(instance().m_queueMutex);
        instance().m_receivedItemsQueue.clear();
    }

    // Reset the tracking-data "last sent" caches so the new connection resends a full set.
    instance().m_lastSentStageName.clear();
    instance().m_lastSentRoom = std::numeric_limits<int>::min();
    instance().m_lastSentFloor = std::numeric_limits<int>::min();
    instance().m_lastSentLayer = std::numeric_limits<int>::min();
    instance().m_lastSentFlags.clear();

    instance().LoadTempItemInfo();

    instance().LoadTempLocationInfo();

    AP_SetLogCallback([](const std::string& msg) {
       DuskLog.info("{}", msg);
    });

    AP_Init(GetServerIp(file).c_str(), "Twilight Princess", GetSlotName(file).c_str(), GetPassword(file).c_str());

    AP_NetworkVersion ver{0, 6,7};
    AP_SetClientVersion(&ver);

    AP_SetDeathLinkSupported(true);

    AP_SetDeathLinkRecvCallback([](std::string source, std::string cause) {
        DuskLog.info("Player {} sent death link. Cause: {}", source, cause);
        RequestPlayerDeath(true);
    });

    AP_SetItemClearCallback([]() {
        DuskLog.info("Item Clear Callback Called!");
        instance().m_isNeedResetInv = true;
    });

    AP_SetItemRecvCallback([](AP_NetworkItem& item, bool notify) {
        DuskLog.debug("Item Receive Callback Called! Item: {} Notify: {}", item.item, notify);
        HandleItemReceived(item, notify);
    });

    AP_SetLocationCheckedCallback([](int loc) {
        DuskLog.info("Location Checked Callback Called! Location: {}", loc);
        SetLocationChecked(loc, true);
    });

    AP_SetLocationInfoCallback([](std::vector<AP_NetworkItem> items) {
        DuskLog.info("Got {} Location Scouts from Server.", items.size());
        HandleReceiveLocationScout(items);
    });

    AP_RegisterSlotDataRawCallback("Settings", [](std::string data) {
        DuskLog.info("Got Settings from Slot Data.");
        instance().m_SettingsFile = data;
    });

    AP_RegisterSlotDataRawCallback("World Version", [](std::string data) {
        DuskLog.info("TP APWorld Version: {}", data);
    });

    AP_Start();

    // above func spawns a websocket thread, but there isn't really a good way to ensure a connection
    // attempt has been made except to wait for that thread to tick once

    if (isBlocking) {
        // wait for ws thread to run a frame before checking for status
        std::this_thread::sleep_for(std::chrono::seconds(1));

        while (AP_GetConnectionStatus() == AP_ConnectionStatus::Connecting)
            std::this_thread::yield();

        if (!IsConnected()) {
            DuskLog.error("Failed to connect to Archipelago Server!");
            return false;
        }
    }

    std::thread messageThread = std::thread(MessageThreadFunc);
    messageThread.detach();

    return true;
}

void ArchipelagoContext::DisconnectFromServer() {
    AP_Shutdown();
}

bool ArchipelagoContext::IsConnected() {
    auto status = AP_GetConnectionStatus();
    return status == AP_ConnectionStatus::Connected || status == AP_ConnectionStatus::Authenticated;
}

bool ArchipelagoContext::IsRoomInfoReady() {
    return !instance().m_roomInfo.seed_name.empty();
}

void ArchipelagoContext::MessageThreadFunc() {
    DuskLog.info("AP Thread started.");

    if (IsConnected()) {
        AP_GetRoomInfo(&instance().m_roomInfo);
        instance().m_isEnableDeathLink = AP_IsDeathLinkEnabled();
        RequestAllLocationScout();
    }

    while (IsConnected()) {
        if (AP_IsMessagePending())
            ParseMessageData();
    }

    DuskLog.info("AP Thread ended.");
}

void ArchipelagoContext::UpdateMapTrackingData() {
    // Team is hardcoded to 0 (AP doesn't use multiple teams yet; matches poptracker's fallback).
    const int team = 0;
    const int slot = AP_GetPlayerID();
    const std::string keyPrefix = "TP_" + std::to_string(team) + "_" + std::to_string(slot) + "_";

    // requests must outlive the AP_CommitServerData() below
    std::deque<PendingServerData> pending;

    std::string stageCode = ReadCurrentStageCode();
    const std::string* stageName = ResolveStageName(stageCode);
    if (!stageName) {
        DuskLog.warn("UpdateMapTrackingData: unrecognized stage code '{}', not sending Current Stage update", stageCode);
    } else if (*stageName != instance().m_lastSentStageName) {
        SetTrackedStringDataStorage(pending, keyPrefix + "Current Stage", *stageName, "Menu");
        instance().m_lastSentStageName = *stageName;
    }

    int room = dStage_roomControl_c::getStayNo();
    if (room != instance().m_lastSentRoom) {
        SetTrackedIntDataStorage(pending, keyPrefix + "Current Room", room, -1);
        instance().m_lastSentRoom = room;
    }

    // Send the raw unsigned byte (basements wrap to 255/254/253), matching the apworld's client.
    int floor = static_cast<u8>(dMapInfo_c::mNowStayFloorNo);
    if (floor != instance().m_lastSentFloor) {
        SetTrackedIntDataStorage(pending, keyPrefix + "Current Floor", floor, -1);
        instance().m_lastSentFloor = floor;
    }

    int layer = dComIfGp_getStartStageLayer();
    if (layer != instance().m_lastSentLayer) {
        SetTrackedIntDataStorage(pending, keyPrefix + "Current Layer", layer, -1);
        instance().m_lastSentLayer = layer;
    }

    AP_CommitServerData();
}

void ArchipelagoContext::UpdateFlagTrackingData() {
    // See UpdateMapTrackingData() for why team is hardcoded to 0.
    const int team = 0;
    const int slot = AP_GetPlayerID();
    const std::string keyPrefix = "TP_" + std::to_string(team) + "_" + std::to_string(slot) + "_";

    // requests must outlive the AP_CommitServerData() below
    std::deque<PendingServerData> pending;

    for (const auto& entry : sFlagTrackingEntries) {
        bool value = ReadSaveInfoFlag(entry.offset, entry.mask);
        auto it = instance().m_lastSentFlags.find(entry.key);
        if (it != instance().m_lastSentFlags.end() && it->second == value)
            continue;
        SetTrackedBoolDataStorage(pending, keyPrefix + entry.key, value, false);
        instance().m_lastSentFlags[entry.key] = value;
    }

    for (const auto& entry : sEventBitTrackingEntries) {
        bool value = ReadEventBit(entry.flagValue);
        auto it = instance().m_lastSentFlags.find(entry.key);
        if (it != instance().m_lastSentFlags.end() && it->second == value)
            continue;
        SetTrackedBoolDataStorage(pending, keyPrefix + entry.key, value, false);
        instance().m_lastSentFlags[entry.key] = value;
    }

    // Boss-defeated state comes from the dungeon-clear event bit; dComIfGs_isStageBossEnemy()
    // reads false for bosses in dungeons other than the one you're currently in.
    for (const auto& entry : sBossTrackingEntries) {
        bool defeated = ReadEventBit(entry.clearFlagValue);
        auto it = instance().m_lastSentFlags.find(entry.key);
        if (it != instance().m_lastSentFlags.end() && it->second == defeated)
            continue;
        SetTrackedBoolDataStorage(pending, keyPrefix + entry.key, defeated, false);
        instance().m_lastSentFlags[entry.key] = defeated;
    }

    AP_CommitServerData();
}

void ArchipelagoContext::Execute() {
    if (!IsConnected())
        return;

    UpdateMapTrackingData();
    UpdateFlagTrackingData();

    // backfill the per-species "Misc." flag for any golden bugs already obtained before this fix
    // existed, so previously-received bugs are picked up by Agitha/the bug menu without needing to
    // receive a new item first
    dComIfGs_syncInsectMiscFlags();

    // reset player inventory if server requested it
    if (instance().m_isNeedResetInv) {
        HandleResetInventory();
        instance().m_isNeedResetInv = false;
        return; // end execution early so next frame can re-add inventory if needed
    }

    // process death links
    if (instance().tryKillPlayer()) {
        // if successful, don't bother processing item queue or location checks
        return;
    }

    // Drain pending item queue here. HandleItemReceived() on the AP network thread needs
    // m_queueMutex just to enqueue, so the lock is only held long enough to swap the queue out into
    // a local vector, and all the actual (possibly slow) processing happens after unlocking, so a
    // burst of items doesn't block the network thread's ability to enqueue more.
    //
    // Hold the whole queue until location scouts are back - shouldSkipDuplicateItem() needs
    // m_locationItemInfo fully populated to correctly dedup (self or foreign) items tied to a
    // location, and this runs every frame, so this is a short deferral on first connect, not a
    // hang. Processing the batch in order (rather than skipping just the unready entries) keeps
    // items applied in the order they were received.
    std::vector<ReceivedItemEntry> itemsToProcess;
    {
        std::lock_guard<std::mutex> lock(instance().m_queueMutex);
        if (!instance().m_receivedItemsQueue.empty() && IsReceivedLocationScouts()) {
            // std::vector move-assignment already leaves the source empty - no need to also clear() it.
            itemsToProcess = std::move(instance().m_receivedItemsQueue);
        }
    }

    // shouldSkipDuplicateItem() only records new dedup entries in-memory (mProcessedNetworkItems);
    // persist once here, after the whole batch, instead of once per newly-processed item - a
    // multi-item resync burst would otherwise do N synchronous full-context seed.dat writes on the
    // main thread in a single frame.
    size_t processedCountBefore = randomizer_GetContext().mProcessedNetworkItems.size();

    for (auto& item : itemsToProcess) {
        if (instance().shouldSkipDuplicateItem(item))
            continue;

        instance().itemRecvImpl(item.relativeId, item.notify);
    }

    if (randomizer_GetContext().mProcessedNetworkItems.size() != processedCountBefore) {
        instance().persistProcessedItems();
    }

    // update location checks here if we need to
    if (instance().m_isUpdateLocations) {
        UpdateCheckedLocations(true);
        instance().m_isUpdateLocations = false;
        instance().m_locationRescanTimer = 0;
    }
    // ~1s backstop for checks whose flag is set outside execItemGet() (AG poe soul pulls, Agitha
    // bug rewards). Re-reads the real game flags, so it's independent of how the flag was set.
    else if (IsReceivedLocationScouts() && ++instance().m_locationRescanTimer >= 60) {
        instance().m_locationRescanTimer = 0;
        UpdateCheckedLocations();
    }
}

void ArchipelagoContext::HandleItemReceived(AP_NetworkItem& netItem, bool notify) {
    int relativeId = netItem.item - ARCHI_ITEM_OFFSET;

    ApItemLog.info("HandleItemReceived: raw item={} location={} player={} notify={} relativeId={}",
        netItem.item, netItem.location, netItem.player, notify, relativeId);

    // TODO: modify this to also include junk items like ammo
    if (!notify && ((relativeId >= 0 && relativeId <= 6) || relativeId == 7)) {
        // skip rupee refills so players cant abuse disconnect/reconnect
        ApItemLog.info("HandleItemReceived: skipped (rupee refill dedup)");
        return;
    }

    // The location-based dedup check (see shouldSkipDuplicateItem()) depends on m_locationItemInfo,
    // which is populated asynchronously by location scouts (AP_SetLocationInfoCallback), and on
    // GetSeedDirectoryPath(), which depends on IsConnected(). Neither is guaranteed ready yet when
    // this callback fires - it runs on the AP network thread as soon as a message arrives, which can
    // be before scouts return or before the client-side connection state has settled, especially
    // during the very first resync burst right after a reconnect. Running the dedup check here would
    // race against that state and silently under-dedup. So we capture everything the dedup check
    // will need and defer the actual check to the main thread's queue drain in Execute(), which only
    // runs once IsConnected() is true and (per the drain loop) only processes the queue once location
    // scouts have actually come back.
    ApItemLog.info("HandleItemReceived: pushed to receive queue (relativeId={}, notify={})", relativeId, notify);

    instance().m_queueMutex.lock();
    instance().m_receivedItemsQueue.push_back({relativeId, notify, netItem.player, netItem.location, instance().m_isNeedResetInv});
    instance().m_queueMutex.unlock();
}

void ArchipelagoContext::persistProcessedItems() {
    std::filesystem::path workingDir;
    GetSeedDirectoryPath(workingDir);
    // GetSeedDirectoryPath() silently leaves workingDir untouched (empty) if IsConnected() is
    // false at the exact moment this runs - which would make the write below silently target
    // a bogus relative "seed.dat" instead of the real seed folder, never actually persisting
    // this dedup entry. This is now called from Execute(), which only runs while IsConnected()
    // is true, so this should be rare in practice, but IsConnected() isn't re-checked between
    // Execute()'s top-of-function guard and this call, so a disconnect racing in between is a
    // real (if narrow) window - log loudly instead of failing silently.
    if (workingDir.empty()) {
        ApItemLog.error("persistProcessedItems: GetSeedDirectoryPath() returned empty - "
            "IsConnected()={}, cannot persist new dedup entries!", IsConnected());
        return;
    }
    auto writeResult = randomizer_GetContext().WriteToFile(workingDir / "seed.dat");
    if (writeResult.has_value()) {
        ApItemLog.error("persistProcessedItems: failed to persist dedup entries to {}: {}",
            (workingDir / "seed.dat").string(), writeResult.value());
    } else {
        ApItemLog.info("persistProcessedItems: persisted dedup entries to {}", (workingDir / "seed.dat").string());
    }
}

bool ArchipelagoContext::shouldSkipDuplicateItem(const ReceivedItemEntry& entry) {
    // Small Key items (AP ids 54-62 for the 9 dungeons, 66 for the Bulblin Camp key) are pure
    // interchangeable counters with no "already have this one" gating in their item_func - unlike
    // most items, re-processing an already-received key is NOT idempotent, it just adds another.
    // The reset-inventory wipe never touches key counts at all (they live in separate per-stage save
    // data, not the general inventory getItem() wipes), so on every single reconnect (which triggers
    // this reset unconditionally, per AP protocol) skipping the dedup below would let the entire
    // key-receive history replay and re-increment on top of whatever was already there - including
    // keys already spent opening doors, since consumption isn't tracked here at all. So keys must
    // always be deduped, reset or not; everything else keeps the original reset-bypass behavior,
    // since re-granting equipment/one-off items after the wipe is harmless.
    //
    // Piece of Heart/Heart Container and Progressive Sky Book also lack an "already counted" guard,
    // but unlike keys their backing state IS wiped by HandleResetInventory(), so replaying their
    // full history against the wiped baseline reconstructs the correct total. Deduping them
    // permanently would instead lose them after a reset (observed with Progressive Sky Book).
    const bool isSmallKeyItem = (entry.relativeId >= 54 && entry.relativeId <= 62) || entry.relativeId == 66;

    // Ice Trap (AP id 131) has no backing state at all: its freeze effect already played and
    // HandleResetInventory() can't wipe it, so a processed trap must stay deduped, reset or not, or
    // every reconnect resync stacks another freeze. Dedup below is keyed by (player, location), so
    // each distinct trap still fires once.
    const bool isIceTrap = entry.relativeId == 131;

    const bool isNonIdempotentCounter = isSmallKeyItem || isIceTrap;

    // Location-less items (AP precollected/start-inventory) can't use location-based dedup. That's
    // harmless for idempotent items, but a location-less key would over-count on every reconnect,
    // so dedup those via (player, relativeId) in a key range (bit 47 set) that can't collide with
    // a real apLocationId.
    if (entry.location == -1) {
        if (!isNonIdempotentCounter)
            return false;

        auto& processedItems = randomizer_GetContext().mProcessedNetworkItems;
        constexpr int64_t kLocationlessDedupBase = 0x800000000000LL;
        u64 key = RandomizerContext::EncodeNetworkItemKey(entry.player, kLocationlessDedupBase | entry.relativeId);
        if (processedItems.contains(key)) {
            ApItemLog.info("shouldSkipDuplicateItem: skipped, already processed location-less item relativeId={} for player {}",
                entry.relativeId, entry.player);
            return true;
        }
        // Persistence is batched by the caller (Execute()) after the whole drain loop, not per-item.
        processedItems.insert(key);
        return false;
    }

    // entry.wasResetPending captures m_isNeedResetInv as it was at the moment the item was actually
    // received (in HandleItemReceived, on the network thread) - by the time this runs, on a later
    // Execute() call, m_isNeedResetInv has always already been cleared, so re-reading it live here
    // would make this condition always true and change behavior for non-counter items.
    const bool shouldDedup = isNonIdempotentCounter || !entry.wasResetPending;

    // Chests etc. grant their item locally the instant the player interacts with them, so the
    // network copy is normally redundant and safe to skip once IsLocationChecked() is true. But
    // that "checked" flag survives HandleResetInventory()'s wipe while the actual item (inventory
    // slot, collect flag, ...) doesn't - so during a reset resync, trusting it here means the item
    // never comes back. Only skip outside of a reset; during one, fall through to the general dedup
    // below so the item gets re-granted like everything else.
    if (entry.player == AP_GetPlayerID() && hasAtomicLocalGrant(entry.location) && !entry.wasResetPending) {
        if (IsLocationChecked(entry.location)) {
            // Also seed the persisted dedup table, so a later reset resync (which can't use
            // IsLocationChecked() - see above) still knows this was handled, and doesn't
            // double-grant a non-idempotent counter (e.g. a small key found in a chest).
            randomizer_GetContext().mProcessedNetworkItems.insert(
                RandomizerContext::EncodeNetworkItemKey(entry.player, entry.location));
            ApItemLog.info("shouldSkipDuplicateItem: skipped, location {} already granted locally", entry.location);
            return true;
        }
        return false;
    }

    if (!shouldDedup)
        return false;

    // Track (player, location) pairs we've actually processed, persisted so it survives
    // reconnects, for everything else (self non-chest locations + foreign locations).
    //
    // Self-locations used to be deduped via IsLocationChecked() unconditionally instead, on the
    // theory that our own save-file state is authoritative and safe to trust. That's only true for
    // locations with a local, atomic grant like chests (handled above) - for everything else,
    // IsLocationChecked() reflects whether the location's "checked" flag has been set (either from
    // the AP_SetLocationCheckedCallback's SetLocationChecked() confirmation, or a live
    // isLocationObtained() read of the location's own completion condition), not whether we've
    // actually been granted the ITEM that location contains. For those, the server can send the
    // LocationChecked confirmation for a location in the same instant it sends the item itself -
    // so on the item's very first, legitimate delivery, IsLocationChecked() could already read
    // true and this would wrongly skip granting the item, permanently losing it. That's exactly
    // what happened to a Progressive Dominion Rod during a big multi-item burst: HandleItemReceived
    // queued it, then the LocationChecked callback fired for the same location before Execute()
    // drained the queue, and the old logic saw IsLocationChecked() == true and silently dropped it.
    //
    // AP location IDs are shared globally across every player of the same game (confirmed via
    // the apworld's get_apid(), which is a flat base_id + code with no per-player component), so a
    // same-numbered location can exist in both our world and another player's world - but that's
    // fine here since the (player, location) pair disambiguates them.
    auto& processedItems = randomizer_GetContext().mProcessedNetworkItems;
    u64 key = RandomizerContext::EncodeNetworkItemKey(entry.player, entry.location);
    if (processedItems.contains(key)) {
        ApItemLog.info("shouldSkipDuplicateItem: skipped, already processed gift from player {} at location {}",
            entry.player, entry.location);
        return true;
    }
    // Persistence is batched by the caller (Execute()) after the whole drain loop, not per-item.
    processedItems.insert(key);

    return false;
}

void ArchipelagoContext::HandleResetInventory() {
    DuskLog.info("Resetting Inventory.");
    // NOTE: this does not clear ALL things from save, so if a player managed to do something while disconnected from the archi, it might mess with things

    auto& playerInfo = g_dComIfG_gameInfo.info.getPlayer();

    // reset items
    playerInfo.getItem().init();
    playerInfo.getGetItem().init();

    // reset collect (poes, shards, swords)
    playerInfo.getCollect().init();

    // The above don't touch the Sky Book letter count; zero it too so its replay (see
    // shouldSkipDuplicateItem()) starts from a clean baseline.
    dComIfGs_setAncientDocumentNum(0);

    playerInfo.getPlayerStatusA().setMaxLife(15);
    playerInfo.getPlayerStatusA().setWalletSize(WALLET);
    // dont reset rupees, and instead reject rupee updates while refilling inv

    // add back default items

    execItemGet(dItemNo_WEAR_KOKIRI_e);

    // sync all location collect flags with current collection status obtained from initial room connection
    UpdateAllLocationState();

    // clear all item-related flags

    dComIfGs_offEventBit(0x2580); // Power up dominion rod

    // shadow crystal
    dComIfGs_offEventBit(0xD04); // Can transform at will
    dComIfGs_offEventBit(0x501); // Midna Charge Unlocked

    // hidden skills
    dComIfGs_offEventBit(0x2904); // ENDING BLOW
    dComIfGs_offEventBit(0x2908); // SHIELD ATTACK
    dComIfGs_offEventBit(0x2902); // BACK SLICE
    dComIfGs_offEventBit(0x2901); // HELM SPLITTER
    dComIfGs_offEventBit(0x2A80); // MORTAL DRAW
    dComIfGs_offEventBit(0x2A40); // JUMP STRIKE
    dComIfGs_offEventBit(0x2A20); // GREAT SPIN

}

void ArchipelagoContext::HandleReceiveLocationScout(const std::vector<AP_NetworkItem>& items) {
    // Runs on the AP network thread (AP_SetLocationInfoCallback) - m_locationItemInfo is also read
    // from the main thread every frame (Execute() and everything it calls), so this whole loop must
    // hold m_locationInfoMutex to avoid a concurrent-mutation-during-iteration race.
    std::lock_guard<std::mutex> lock(instance().m_locationInfoMutex);
    for (const auto& item : items) {
        int parsedItemId;
        std::string parsedItemName;
        if (item.player == AP_GetPlayerID()) {
            int adjustedId = item.item - ARCHI_ITEM_OFFSET;

            if (instance().m_apItemToGameItem.contains(adjustedId)) {
                auto& itemInfo = instance().m_apItemToGameItem[adjustedId];
                parsedItemId = itemInfo.itemId;
                parsedItemName = itemInfo.itemName;
            }else {
                parsedItemId = -1;
                parsedItemName = "Unknown";
            }
        }else {
            parsedItemId = dItemNo_Randomizer_ARCHIPELAGO_ITEM_e;
            parsedItemName = "Archipelago Item";
        }
        int locationId = item.location - ARCHI_ITEM_OFFSET;

        auto locName = instance().getLocationNameFromApId(locationId);

        if (locName.empty()) {
            DuskLog.info("No location with ID {} found.", locationId);
            continue;
        }

        // HandleReceiveLocationScout() isn't guaranteed to run exactly once - the connect modal's
        // scout-wait loop can re-request scouts after a timeout, and if the original (merely slow,
        // not actually lost) response then arrives late, its response arrives even later still,
        // potentially well after gameplay has already started and this location has been checked.
        // Blindly overwriting the map entry here would reset collected back to the stale snapshot
        // taken at connect time, making UpdateCheckedLocations() think the location needs sending
        // again - which is exactly what caused a Forest Temple Diababa Heart Container check to be
        // sent to the server twice. So: only seed collected from the initial snapshot the first
        // time we see this location; if we've already recorded progress on it, keep that.
        auto existingIt = instance().m_locationItemInfo.find(locName);
        bool collected = existingIt != instance().m_locationItemInfo.end() ? existingIt->second.collected : false;
        if (existingIt == instance().m_locationItemInfo.end() && instance().m_initLocationCollectState.contains(item.location))
            collected = instance().m_initLocationCollectState[item.location];

        instance().m_locationItemInfo[locName] = {
            parsedItemId,
            parsedItemName,
            locName,
            item.location,
            collected
        };
    }
}
// TODO: atm this is a sort of lazy solution to not having direct access to what location was checked when an execItemGet is called
// so eventually finding a way to properly associate locations with their respective item get funcs would benefit this system
void ArchipelagoContext::UpdateCheckedLocations(bool warnIfNoChange) {
    auto& world = instance().m_archiWorld;
    if (world == nullptr) {
        // Execute() calls this every frame once scouts arrive; world data may not be ready yet
        DuskLog.warn("UpdateCheckedLocations: archipelago world not generated yet - skipping");
        return;
    }
    std::lock_guard<std::mutex> lock(instance().m_locationInfoMutex);

    bool changed = false;

    for (auto location : world->GetAllLocations()) {
        // skip locations that aren't progression, which are locations that just aren't randomized
        if (!location->IsProgression()) {
            continue;
        }

        auto locName = location->GetName();

        if (!instance().m_locationItemInfo.contains(locName)) {
            DuskLog.debug("No item found for ({}).", locName);
            continue;
        }

        auto& cachedLocData = instance().m_locationItemInfo[locName];

        bool isCollected = isLocationObtained(location);

        if (isCollected && !cachedLocData.collected) {
            cachedLocData.collected = true;
            AP_SendItem(cachedLocData.apLocationId);
            changed = true;
        }
    }

    if (!changed && warnIfNoChange) {
        DuskLog.warn("No locations had any changes! this might not be normal.");
    }
}

void ArchipelagoContext::SetNeedUpdateLocations(bool update) {
    if (!instance().m_isAllowUpdateLocations)
        instance().m_isUpdateLocations = update;
}

bool ArchipelagoContext::IsLocationChecked(int locId) {
    auto& world = instance().m_archiWorld;
    if (world == nullptr)
        return false;
    std::lock_guard<std::mutex> lock(instance().m_locationInfoMutex);

    for (const auto& [locName, locInfo] : instance().m_locationItemInfo) {
        if (locInfo.apLocationId == locId) {
            if (locInfo.collected)
                return true;

            if (auto location = world->GetLocation(locInfo.locationName, true)) {
                return isLocationObtained(location);
            }

            DuskLog.error("Failed to obtain location: {}", locName);
            return false;
        }
    }
    return false;
}

bool ArchipelagoContext::hasAtomicLocalGrant(int64_t apLocationId) {
    // The category list this checks against (RandomizerContext::kAtomicallyGrantedLocationCategories)
    // is shared with randomizer_context.cpp's world-gen override-table population, so the two sides
    // can't drift independently - see that constant's declaration for the full reasoning.
    if (m_archiWorld == nullptr)
        return false;
    std::lock_guard<std::mutex> lock(m_locationInfoMutex);
    for (const auto& [locName, locInfo] : m_locationItemInfo) {
        if (locInfo.apLocationId == apLocationId) {
            if (auto* location = m_archiWorld->GetLocation(locInfo.locationName, true)) {
                for (const auto* category : RandomizerContext::kAtomicallyGrantedLocationCategories) {
                    if (!location->HasCategories(category))
                        continue;

                    // World-gen only populates mShopOverrides for "Shop" locations when the
                    // "Shop Items" setting is On (randomizer_context.cpp's world-gen loop) - if a
                    // Shop location is tagged with the category but that setting is off, no local
                    // override was ever embedded, so treating it as atomically-locally-granted here
                    // would wrongly dedup via IsLocationChecked() an item that only the network
                    // delivery actually grants, risking losing it on first delivery (the same class
                    // of bug already hit for the Progressive Dominion Rod).
                    if (std::string_view(category) == "Shop" && m_archiWorld->Setting("Shop Items") != "On")
                        continue;

                    return true;
                }
                return false;
            }
            return false;
        }
    }
    return false;
}

void ArchipelagoContext::SetLocationChecked(int locId, bool collected) {
    // func was ran before location scouts could be sent out, cache result until scouts return.
    if (!IsReceivedLocationScouts()) {
        instance().m_initLocationCollectState[locId] = collected;
        return;
    }

    auto& world = instance().m_archiWorld;
    std::lock_guard<std::mutex> lock(instance().m_locationInfoMutex);

    for (auto& [locName, locInfo] : instance().m_locationItemInfo) {
        if (locInfo.apLocationId == locId) {
            locInfo.collected = collected;

            // update location flags if possible
            auto location = world ? world->GetLocation(locInfo.locationName, true) : nullptr;
            if (!location || !location->IsProgression())
                return;

            setLocationCollected(location, collected);
            return;
        }
    }

    DuskLog.warn("No location found for locId {}.", locId);
}

void ArchipelagoContext::UpdateLocationState(int locId, bool collected) {
    auto& world = instance().m_archiWorld;
    if (world == nullptr)
        return;
    std::lock_guard<std::mutex> lock(instance().m_locationInfoMutex);

    for (const auto& [locName, locInfo] : instance().m_locationItemInfo) {
        if (locInfo.apLocationId == locId) {
            auto location = world->GetLocation(locInfo.locationName, true);
            if (!location || !location->IsProgression())
                continue;

            setLocationCollected(location, collected);
            return;
        }
    }

    DuskLog.warn("No location found for locId {}.", locId);
}

void ArchipelagoContext::UpdateAllLocationState() {
    auto& world = instance().m_archiWorld;
    if (world == nullptr)
        return;
    std::lock_guard<std::mutex> lock(instance().m_locationInfoMutex);
    // TODO: find out why some locations seem to keep their collection state upon reset (bugs)

    for (const auto& [locName, locInfo] : instance().m_locationItemInfo) {
        auto location = world->GetLocation(locInfo.locationName, true);
        if (!location || !location->IsProgression())
            continue;

        setLocationCollected(location, locInfo.collected);
    }
}

bool ArchipelagoContext::IsReceivedLocationScouts() {
    std::lock_guard<std::mutex> lock(instance().m_locationInfoMutex);
    return !instance().m_locationItemInfo.empty();
}

void ArchipelagoContext::TryHandleDeathLink() {
    if (instance().m_isEnableDeathLink && !instance().m_isFromDeathLink) {
        // TODO: come up with better death messages
        AP_DeathLinkSend("%YOU% was unable to become the Hero of Twilight.");
    }
}

bool ArchipelagoContext::TryHandleGameComplete() {
    // TODO: maybe add support for other game completion types?
    AP_StoryComplete();
    return true;
}

void ArchipelagoContext::RequestAllLocationScout(bool isHint) {
    std::set<int64_t> locations;
    // TEMP: apworld has 475 locations with ids in sequential order, so add them all individually to location set
    // (eventually we will iterate through locations.yaml for a better data-driven solution)
    for (int i = 0; i < 475; i++) {
        locations.insert(ARCHI_ITEM_OFFSET + i);
    }

    DuskLog.info("Requesting location scouts for {} locations.", locations.size());
    AP_SendLocationScouts(locations, isHint);
}

void ArchipelagoContext::RequestPlayerDeath(bool isDeathLink) {
    instance().m_isNeedPlayerDeath = true;
    instance().m_isFromDeathLink = isDeathLink;
}

bool ArchipelagoContext::GenerateConfigFromAP(randomizer::seedgen::config::Config& config, const std::string& settingsStr) {
    YAML::Node apConfigYaml;
    try {
        apConfigYaml = YAML::Load(settingsStr);
    }catch (const YAML::Exception& e) {
        DuskLog.warn("Failed to load AP Config YAML file! Reason: {}", e.what());
        return false;
    }

    config.SetSeed("Archipelago");
    randomizer::seedgen::settings::Settings& settings = config.GetSettings();

    // update settings using ap config
    for (const auto& apSettingEntry : apConfigYaml) {
        auto apSettingName = apSettingEntry.first.as<std::string>();
        auto apSettingValue = apSettingEntry.second.as<std::string>();

        const auto& settingConvert = GetAPSettingNameConvert(apSettingName);

        //try catch is neccesary here. If it fails on converting the setting, it will crash and not continue reading the settings.
        //this results in it using a default settiings file instead of the settings for the player
        try {
            if (!settingConvert.apName.empty()) {
                auto& setting = settings.GetMap().at(settingConvert.dusklightName);
                setting.SetCurrentOption(settingConvert.tryGetOptionConvert(apSettingValue));
            } else if (apSettingName == "Castle Requirements") {
                auto& setting = settings.GetMap().at("Hyrule Barrier Requirements");

                // ap assumes max mirror shards/fused shadows/dungeons, so update those settings as well

                if(apSettingValue == "Open")
                    setting.SetCurrentOption("Open");
                else if(apSettingValue == "Vanilla")
                    setting.SetCurrentOption("Vanilla");
                else if(apSettingValue == "Fused Shadows") {
                    setting.SetCurrentOption("Fused Shadows");
                    settings.GetMap().at("Hyrule Barrier Fused Shadows").SetCurrentOption("3");
                }else if(apSettingValue == "Mirror Shards") {
                    setting.SetCurrentOption("Mirror Shards");
                    settings.GetMap().at("Hyrule Barrier Mirror Shards").SetCurrentOption("4");
                }else if(apSettingValue == "All Dungeons") {
                    setting.SetCurrentOption("Dungeons");
                    settings.GetMap().at("Hyrule Barrier Dungeons").SetCurrentOption("8");
                }
            }else if (apSettingName == "Temple of Time Entrance Requirements") {
                auto& setting = settings.GetMap().at("Sacred Grove Does Not Require Skull Kid");
                auto& setting2 = settings.GetMap().at("Temple of Time Sword Requirement");

                if(apSettingValue == "Closed") {
                    setting.SetCurrentOption("Off");
                    setting2.SetCurrentOption("Master Sword");
                }else if (apSettingValue == "Open Grove") {
                    setting.SetCurrentOption("On");
                    setting2.SetCurrentOption("Master Sword");
                }else if (apSettingValue == "Open") {
                    setting.SetCurrentOption("On");
                    setting2.SetCurrentOption("None");
                }
            }else {
                DuskLog.debug("Missing Setting: {} Value: {}", apSettingName, apSettingValue);
            }
        }catch (const std::exception& e) {
            DuskLog.warn("Failed to apply AP setting \"{}\" (value \"{}\"): {}", apSettingName, apSettingValue, e.what());
        }
    }

    return true;
}

int ArchipelagoContext::GetItemAtLocation(const std::string& locName) {
    std::lock_guard<std::mutex> lock(instance().m_locationInfoMutex);
    if (!instance().m_locationItemInfo.contains(locName)) {
        DuskLog.warn("No item found for ({}).", locName);
        return 0;
    }
    return instance().m_locationItemInfo[locName].itemId;
}

int ArchipelagoContext::GetItemAtLocation(int locId) {
    std::lock_guard<std::mutex> lock(instance().m_locationInfoMutex);
    for (const auto& [locName, locInfo] : instance().m_locationItemInfo) {
        if (locInfo.apLocationId == locId) {
            return locInfo.itemId;
        }
    }
    return 0;
}

void ArchipelagoContext::CreateArchipelagoWorld() {
    std::filesystem::path workingDir;
    GetSeedDirectoryPath(workingDir);

    auto trackerRando = randomizer::Randomizer(workingDir);
    trackerRando.GenerateTrackerWorld(false);

    auto& worlds = trackerRando.GetWorlds();
    if (worlds.empty()) {
        DuskLog.error("CreateArchipelagoWorld: GenerateTrackerWorld produced no worlds - archipelago world stays null");
        return;
    }
    instance().m_archiWorld = std::move(worlds.front());
}

void ArchipelagoContext::FillArchipelagoWorld() {
    auto& world = instance().m_archiWorld;

    if (world == nullptr) {
        DuskLog.error("Archipelago world was not created!");
        return;
    }

    auto& locationInfo = instance().m_locationItemInfo;
    std::lock_guard<std::mutex> lock(instance().m_locationInfoMutex);

    // fill all locations with data pulled from archi session
    for (auto location : world->GetAllLocations()) {
        // skip locations that aren't progression, which are locations that just aren't randomized
        if (!location->IsProgression()) {
            location->SetCurrentItem(location->GetOriginalItem());
            continue;
        }

        auto locName = location->GetName();
        if (!locationInfo.contains(locName)) {
            if (!location->HasCategories("Warp Portal") &&
                !location->HasCategories("Placeholder") &&
                !location->HasCategories("Hint Sign"))
                DuskLog.warn("Missing archipelago location data for: {}", locName);
            auto origItem = location->GetOriginalItem();

            // set location to original item

            if (origItem->GetID() != -1) // ensure item is not nothing
                location->SetCurrentItem(origItem);
            else
                DuskLog.info("Location ({}) does not have an original item!", locName);

            continue;
        }

        auto& locInfo = locationInfo[locName];
        if (locInfo.itemId != -1) {
            location->SetCurrentItem(world->GetItem(locInfo.itemId));
        }else {
            DuskLog.info("Skipping location ({}) as item is -1.", locName);
        }
    }
}

void ArchipelagoContext::CreateRandomizerContext() {
    auto& world = instance().m_archiWorld;
    if (world == nullptr) {
        DuskLog.error("CreateRandomizerContext: archipelago world was not created - aborting");
        return;
    }

    // Set hint texts before writing context
    randomizer::logic::hints::GenerateAllHints(world);

    // TODO: generate archipelago item get text replacements

    auto randoData = WriteSeedData(world.get());
    randoData.mHash = GetArchipelagoSeedName();

    randomizer_GetContext() = randoData;

    std::filesystem::path workingDir;
    GetSeedDirectoryPath(workingDir);

    auto writeToFileResult = randoData.WriteToFile(workingDir / "seed.dat");

    if (writeToFileResult.has_value()) {
        DuskLog.error("Failed to create Rando Data. Reason: {}", writeToFileResult.value());
        return;
    }
}

void ArchipelagoContext::LoadRandomizerContext() {
    randomizer_GetContext() = RandomizerContext();

    std::filesystem::path workingDir;
    GetSeedDirectoryPath(workingDir);

    randomizer_GetContext().LoadFromPath(workingDir / "seed.dat");
    randomizer_GetContext().mHash = GetArchipelagoSeedName();
}

void ArchipelagoContext::GenerateLocalWorldData() {
    bool createContext = false;
    std::filesystem::path workingDir;

    GetSeedDirectoryPath(workingDir);

    if (workingDir.empty()) {
        DuskLog.fatal("GenerateLocalWorldData: seed directory path is empty (Room Info not "
            "received yet) - aborting world data generation instead of using a bogus directory.");
        return;
    }

    if (std::filesystem::exists(workingDir)) {
        instance().m_config.LoadFromFile(workingDir / "settings.yaml", workingDir / "preferences.yaml");
    }else {
        std::filesystem::create_directories(workingDir);
        // creates base yamls at directory if they dont exist yet
        instance().m_config.LoadFromFile(workingDir / "settings.yaml", workingDir / "preferences.yaml");

        if (instance().m_SettingsFile.empty()) {
            DuskLog.fatal("Settings Data was not sent to client! Unable to generate world data.");
            return;
        }

        GenerateConfigFromAP(instance().m_config, instance().m_SettingsFile);

        instance().m_config.WriteToFile(workingDir / "settings.yaml", workingDir / "preferences.yaml");

        createContext = true;
    }

    CreateArchipelagoWorld();

    FillArchipelagoWorld();

    if (createContext) {
        CreateRandomizerContext();
    }else {
        LoadRandomizerContext();
    }
}
} // dusk::archi