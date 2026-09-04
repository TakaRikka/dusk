#pragma once

#include <limits>
#include <mutex>
#include <string>

#include "Archipelago.h"

namespace dusk::archi
{
    class ArchipelagoContext {
    private:
        struct TEMP_GameItemInfo {
            int itemId = -1;
            randomizer::logic::item::Importance importance = randomizer::logic::item::Importance::INVALID;
            std::string itemName;
        };

        struct TEMP_GameLocationInfo {
            int apId = -1;
            std::string locName;
        };

        struct GameLocationInfo {
            int itemId = -1;
            std::string itemName;
            std::string locationName;
            int64_t apLocationId = -1;
            bool collected = false;
        };

        struct ReceivedItemEntry {
            int relativeId;
            bool notify;
            int player;
            int64_t location;
            bool wasResetPending;
        };

        // Lock ordering: if a caller needs both, always acquire m_queueMutex before
        // m_locationInfoMutex (Execute()'s drain does this via IsReceivedLocationScouts()). No path
        // currently acquires them in the reverse order - keep it that way, or this becomes a
        // lock-order-inversion deadlock risk.
        std::vector<ReceivedItemEntry> m_receivedItemsQueue;
        std::mutex m_queueMutex;

        // Guards m_locationItemInfo: HandleReceiveLocationScout() mutates it from the AP network
        // thread (via AP_SetLocationInfoCallback), while Execute() and everything it calls
        // (hasAtomicLocalGrant, IsLocationChecked, UpdateCheckedLocations, etc.) reads/mutates it
        // from the main thread every frame. Plain mutex is sufficient - traced every locking
        // function below and none call another while already holding this lock, and AP_SendItem()
        // (called from UpdateCheckedLocations while holding it) only queues a websocket send with
        // no synchronous callback re-entry into this code.
        std::mutex m_locationInfoMutex;

        // Rando Data
        randomizer::seedgen::config::Config m_config;
        std::unique_ptr<randomizer::logic::world::World> m_archiWorld = nullptr;
        bool m_isUpdateLocations = false;
        bool m_isNeedResetInv = false;
        bool m_isAllowUpdateLocations = false;
        bool m_isEnableDeathLink = false;

        // Frame counter for the periodic UpdateCheckedLocations() backstop in Execute(), for checks
        // whose flag is set outside execItemGet() (AG poe soul pulls, Agitha bug rewards).
        int m_locationRescanTimer = 0;

        // AP Data
        std::unordered_map<std::string, GameLocationInfo> m_locationItemInfo;
        std::map<int, bool> m_initLocationCollectState;
        AP_RoomInfo m_roomInfo;
        std::string m_SettingsFile;
        bool m_isNeedPlayerDeath = false;
        bool m_isFromDeathLink = false;

        // TEMP
        std::map<int, TEMP_GameItemInfo> m_apItemToGameItem;
        std::vector<TEMP_GameLocationInfo> m_apLocToGameLoc;

        void LoadTempItemInfo();

        void LoadTempLocationInfo();

        void itemRecvImpl(int id, bool notify);

        bool shouldSkipDuplicateItem(const ReceivedItemEntry& entry);

        void persistProcessedItems();

        bool hasAtomicLocalGrant(int64_t apLocationId);

        int getItemIdFromApId(int apId);

        std::string getLocationNameFromApId(int apId) const;

        bool tryKillPlayer();

        // Push the "TP_{team}_{slot}_Current Stage/Room/Floor/Layer" datastorage keys that
        // poptracker's autotracking reads for map tabs. Only sends keys whose value changed.
        static void UpdateMapTrackingData();

        // Push the scent/quest-item/howling-stone/memory-reward and 8 boss-defeated datastorage
        // keys that poptracker's autotracking reads. Only sends keys whose value changed.
        static void UpdateFlagTrackingData();

        // Last values sent by UpdateMapTrackingData(); sentinel-initialized so the first call
        // after connecting sends a full set.
        std::string m_lastSentStageName;
        int m_lastSentRoom = std::numeric_limits<int>::min();
        int m_lastSentFloor = std::numeric_limits<int>::min();
        int m_lastSentLayer = std::numeric_limits<int>::min();

        // Last values sent by UpdateFlagTrackingData(), keyed by AP key name.
        std::unordered_map<std::string, bool> m_lastSentFlags;
    public:
        ArchipelagoContext();

        // Config Getters/Setters

        static void SetServerIp(const std::string_view& ip, int file);
        static void SetSlotName(const std::string_view& name, int file);
        static void SetPassword(const std::string_view& pass, int file);

        static const std::string& GetServerIp(int file);
        static const std::string& GetSlotName(int file);
        static const std::string& GetPassword(int file);

        static std::string GetArchipelagoSeedName();

        static void GetSeedDirectoryPath(std::filesystem::path& outPath);

        static bool IsSeedHashArchipelago(const std::string& seedStr);

        static bool IsCurrentSeedHash(const std::string& seedStr);

        // Connection Handlers

        static bool ConnectToServer(int file, bool isBlocking = false);

        static void DisconnectFromServer();

        static bool IsConnected();

        // True once seed_name has arrived (filled asynchronously after connect). Callers that need
        // the seed directory must wait for this, not just IsConnected().
        static bool IsRoomInfoReady();

        // State Handlers

        static void MessageThreadFunc();

        static void Execute();

        static void HandleItemReceived(AP_NetworkItem& id, bool notify);

        static void HandleResetInventory();

        static void HandleReceiveLocationScout(const std::vector<AP_NetworkItem>& items);

        static void UpdateCheckedLocations(bool warnIfNoChange = false);

        static void SetNeedUpdateLocations(bool update);

        static bool IsLocationChecked(int locId);

        static void SetLocationChecked(int locId, bool collected);

        static void UpdateLocationState(int locId, bool collected);

        static void UpdateAllLocationState();

        static bool IsReceivedLocationScouts();

        static void TryHandleDeathLink();

        static bool TryHandleGameComplete();

        // State Requesters

        static void RequestAllLocationScout(bool isHint = false);

        static void RequestPlayerDeath(bool isDeathLink = false);

        // AP -> Internal Rando Converters

        static bool GenerateConfigFromAP(randomizer::seedgen::config::Config& config, const std::string& settingsStr);

        static int GetItemAtLocation(const std::string& locName);

        static int GetItemAtLocation(int locId);

        static void CreateArchipelagoWorld();

        static void FillArchipelagoWorld();

        static void CreateRandomizerContext();

        static void LoadRandomizerContext();

        static void GenerateLocalWorldData();

    };
} // dusk::archi