#pragma once

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

        // State Handlers

        static void MessageThreadFunc();

        static void Execute();

        static void HandleItemReceived(AP_NetworkItem& id, bool notify);

        static void HandleResetInventory();

        static void HandleReceiveLocationScout(const std::vector<AP_NetworkItem>& items);

        static void UpdateCheckedLocations();

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