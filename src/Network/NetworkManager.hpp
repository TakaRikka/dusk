#pragma once
#include <enet/enet.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include "NetworkPackets.hpp"

// Forward declaration of Dusklight engine types (representing the player entity)
class daAlink_c; 

struct RemotePeer {
    uint32_t clientID;
    TransformPacket lastTransform;
    daAlink_c* puppetActorInstance = nullptr;
    InterpolatedTransform smoother; // Add this instance here
    bool needsSpawn = true;
    bool isActive = false;
};

class NetworkManager {
public:
    static NetworkManager& GetInstance() {
        static NetworkManager instance;
        return instance;
    }

    bool StartClient(const std::string& hostAddress, uint16_t port);
    void Shutdown();
    void Update(); // Called every engine logic tick (30Hz/Fixed Update)
    
    void SendLocalTransform(float x, float y, float z, float rotY, uint16_t animId, uint32_t flags, int8_t stage, int8_t room);
    void SendWorldEvent(uint32_t regIdx, uint8_t mask);

private:
    NetworkManager() = default;
    ~NetworkManager() { Shutdown(); }

    void ProcessIncomingPacket(ENetPacket* packet);
    void HandlePeerTransform(const TransformPacket& packet);
    void HandlePeerDisconnect(uint32_t clientID);
    
    void SynchronizePuppetActors();
    void PurgeStalePuppets();

    ENetHost* m_clientContext = nullptr;
    ENetPeer* m_serverPeer = nullptr;
    bool m_isContextRunning = false;
    uint32_t m_localClientID = 0;

    std::mutex m_peerMutex;
    std::unordered_map<uint32_t, RemotePeer> m_remotePeers;
};