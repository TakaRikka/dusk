#include "NetworkManager.hpp"
#include <iostream>

// External engine hooks inside Dusklight's codebase
extern "C" daAlink_c* fopAcM_FastCreate(unsigned int procName, unsigned int parameter, const float* pos, int roomNo, const float* rot);
extern "C" void fopAcM_Delete(daAlink_c* actor);

bool NetworkManager::StartClient(const std::string& hostAddress, uint16_t port) {
    if (enet_initialize() != 0) {
        std::cerr << "[Network] Failed to initialize ENet framework." << std::endl;
        return false;
    }

    m_clientContext = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!m_clientContext) {
        std::cerr << "[Network] Failed to instantiate ENet client instance host." << std::endl;
        return false;
    }

    ENetAddress address;
    enet_address_set_host(&address, hostAddress.c_str());
    address.port = port;

    m_serverPeer = enet_host_connect(m_clientContext, &address, 2, 0);
    if (!m_serverPeer) {
        std::cerr << "[Network] Destination server peer target deployment failed." << std::endl;
        enet_host_destroy(m_clientContext);
        return false;
    }

    // Monitor for connection validation acknowledgment
    ENetEvent netEvent;
    if (enet_host_service(m_clientContext, &netEvent, 5000) > 0 && netEvent.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "[Network] Successfully connected to infrastructure host." << std::endl;
        m_isContextRunning = true;

        // Dispatch introductory handshake identifier
        HandshakePacket hello;
        hello.requestedClientID = 0; // Server dictates assigning real execution ID
        ENetPacket* packet = enet_packet_create(&hello, sizeof(HandshakePacket), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(m_serverPeer, 0, packet);
        return true;
    }

    enet_peer_reset(m_serverPeer);
    enet_host_destroy(m_clientContext);
    return false;
}

void NetworkManager::Update() {
    if (!m_isContextRunning || !m_clientContext) return;

    ENetEvent netEvent;
    // Process all pending packets without blocking the active frame cycle (timeout set to 0)
    while (enet_host_service(m_clientContext, &netEvent, 0) > 0) {
        switch (netEvent.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                ProcessIncomingPacket(netEvent.packet);
                enet_packet_destroy(netEvent.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "[Network] Connection dropped by remote host controller." << std::endl;
                m_isContextRunning = false;
                break;
            default:
                break;
        }
    }

    SynchronizePuppetActors();
}

void NetworkManager::ProcessIncomingPacket(ENetPacket* packet) {
    if (packet->dataLength < sizeof(uint8_t)) return;

    uint8_t type = packet->data[0];
    std::lock_guard<std::mutex> lock(m_peerMutex);

    switch (type) {
        case PACKET_HANDSHAKE: {
            if (packet->dataLength == sizeof(HandshakePacket)) {
                HandshakePacket* data = reinterpret_cast<HandshakePacket*>(packet->data);
                m_localClientID = data->requestedClientID;
                std::cout << "[Network] Assigned Client Network Matrix ID: " << m_localClientID << std::endl;
            }
            break;
        }
        case PACKET_TRANSFORM_UPDATE: {
            if (packet->dataLength == sizeof(TransformPacket)) {
                TransformPacket* data = reinterpret_cast<TransformPacket*>(packet->data);
                if (data->clientID != m_localClientID) {
                    HandlePeerTransform(*data);
                }
            }
            break;
        }
        case PACKET_PLAYER_DISCONNECT: {
            if (packet->dataLength == sizeof(DisconnectPacket)) {
                DisconnectPacket* data = reinterpret_cast<DisconnectPacket*>(packet->data);
                HandlePeerDisconnect(data->clientID);
            }
            break;
        }
    }
}

void NetworkManager::HandlePeerTransform(const TransformPacket& packet) {
    auto& peer = m_remotePeers[packet.clientID];
    peer.clientID = packet.clientID;
    peer.lastTransform = packet;
    peer.isActive = true;

// If peer.needsSpawn is true, it instantly teleports them on their first frame rather than sliding across the map.
    peer.smoother.SetTarget(packet.posX, packet.posY, packet.posZ, packet.rotY, peer.needsSpawn);
}

void NetworkManager::HandlePeerDisconnect(uint32_t clientID) {
    auto it = m_remotePeers.find(clientID);
    if (it != m_remotePeers.end()) {
        if (it->second.puppetActorInstance != nullptr) {
            fopAcM_Delete(it->second.puppetActorInstance);
        }
        m_remotePeers.erase(it);
        std::cout << "[Network] Client context reference removed: " << clientID << std::endl;
    }
}

void NetworkManager::SynchronizePuppetActors() {
    std::lock_guard<std::mutex> lock(m_peerMutex);
    
    // Grab the local client environment context to assess matching location rules
    // Dummy accessors tracking internal engine variables
    int8_t localStageId = 1; // e.g., dStage_roomControl_c::getStageID()
    int8_t localRoomId = 0;  // e.g., dStage_roomControl_c::getRoomID()

    for (auto& [id, peer] : m_remotePeers) {
        if (!peer.isActive) continue;

        // Context Separation: Delete or skip puppets not residing within our immediate cell space
        if (peer.lastTransform.stageId != localStageId || peer.lastTransform.roomId != localRoomId) {
            if (peer.puppetActorInstance != nullptr) {
                fopAcM_Delete(peer.puppetActorInstance);
                peer.puppetActorInstance = nullptr;
                peer.needsSpawn = true;
            }
            continue;
        }

        // Factory Initialization: Instantiate a puppet entity representation inside memory
        if (peer.needsSpawn && peer.puppetActorInstance == nullptr) {
            float spawnPos[3] = { peer.lastTransform.posX, peer.lastTransform.posY, peer.lastTransform.posZ };
            float spawnRot[3] = { 0.0f, peer.lastTransform.rotY, 0.0f };
            
            // PROC_ALINK = Player entity profile type inside twilight princess
            peer.puppetActorInstance = fopAcM_FastCreate(7, 0xFFFFFFFF, spawnPos, localRoomId, spawnRot);
            
            if (peer.puppetActorInstance != nullptr) {
                peer.needsSpawn = false;
                // CRITICAL INJECTION: Set an internal structural override flag on the puppet's instance 
                // data to skip controller hardware processing loops during native engine ticks.
                // *((uint32_t*)peer.puppetActorInstance + IS_PUPPET_OFFSET) = 1;
            }
            continue;
        }

        // Transform Frame Interpolation
        if (peer.puppetActorInstance != nullptr) {
            // Unpack underlying native spatial vector arrays from Dusklight memory
            float* posArray = reinterpret_cast<float*>(reinterpret_cast<char*>(peer.puppetActorInstance) + 0xFC); 
            float* rotArray = reinterpret_cast<float*>(reinterpret_cast<char*>(peer.puppetActorInstance) + 0x11C);

            // -------------------------------------------------------------
            // REMOVED (Old Snapping Logic):
            // posArray[0] = peer.lastTransform.posX;
            // posArray[1] = peer.lastTransform.posY;
            // posArray[2] = peer.lastTransform.posZ;
            // rotArray[1] = peer.lastTransform.rotY;
            // -------------------------------------------------------------

            // This increments the current position 25% closer to the target packet 
            // every single game tick, creating smooth fluid movement animations.
            peer.smoother.Step(posArray, &rotArray[1]); 

            // Enforce explicit remote animation bank configuration overrides
            // peer.puppetActorInstance->getAnmInterface()->setAnimationById(peer.lastTransform.animationId);
        }
    }
}

void NetworkManager::SendLocalTransform(float x, float y, float z, float rotY, uint16_t animId, uint32_t flags, int8_t stage, int8_t room) {
    if (!m_isContextRunning || !m_serverPeer) return;

    TransformPacket packet;
    packet.clientID = m_localClientID;
    packet.posX = x;
    packet.posY = y;
    packet.posZ = z;
    packet.rotY = rotY;
    packet.animationId = animId;
    packet.actionFlags = flags;
    packet.stageId = stage;
    packet.roomId = room;

    ENetPacket* eNetPacket = enet_packet_create(&packet, sizeof(TransformPacket), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
    enet_peer_send(m_serverPeer, 0, eNetPacket);
    enet_host_flush(m_clientContext); // Immediate delivery optimization
}

void NetworkManager::Shutdown() {
    if (m_clientContext) {
        std::lock_guard<std::mutex> lock(m_peerMutex);
        for (auto& [id, peer] : m_remotePeers) {
            if (peer.puppetActorInstance != nullptr) {
                fopAcM_Delete(peer.puppetActorInstance);
            }
        }
        m_remotePeers.clear();

        if (m_serverPeer) {
            enet_peer_disconnect(m_serverPeer, 0);
            enet_host_flush(m_clientContext);
        }
        enet_host_destroy(m_clientContext);
        enet_deinitialize();
        m_clientContext = nullptr;
        m_serverPeer = nullptr;
        m_isContextRunning = false;
        std::cout << "[Network] Hardware layer interface brought down safely." << std::endl;
    }
}