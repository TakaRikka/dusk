#include <iostream>
#include <unordered_map>
#include <enet/enet.h>
#include "NetworkPackets.hpp"

struct ConnectedClient {
    uint32_t clientID;
    ENetPeer* peerReference;
    int8_t currentStageId = -1;
    int8_t currentRoomId = -1;
};

// Global server context state
std::unordered_map<ENetPeer*, ConnectedClient> g_ActiveClients;
uint32_t g_NextClientID = 1000;

void BroadcastToRoom(ENetPeer* excludePeer, int8_t stageId, int8_t roomId, const void* data, size_t size, uint32_t flags) {
    ENetPacket* packet = enet_packet_create(data, size, flags);
    
    for (auto& [peer, client] : g_ActiveClients) {
        if (peer == excludePeer) continue; // Skip the sender
        
        // Context-driven optimization: Only route movement details to players in the immediate vicinity
        if (client.currentStageId == stageId && client.currentRoomId == roomId) {
            enet_peer_send(peer, 0, packet);
        }
    }
    // ENet increments packet reference tracking internally, so we flush/destroy cleanly here
    enet_host_flush(enet_peer_get_host(excludePeer));
}

void HandleServerHandshake(ENetPeer* peer, ENetPacket* incomingPacket) {
    if (incomingPacket->dataLength < sizeof(HandshakePacket)) return;

    HandshakePacket* clientHandshake = reinterpret_cast<HandshakePacket*>(incomingPacket->data);
    if (clientHandshake->magicValue != 0x54504F4E) {
        std::cerr << "[Server] Rejected client connection: Invalid Protocol Magic String." << std::endl;
        enet_peer_disconnect_now(peer, 0);
        return;
    }

    uint32_t assignedId = g_NextClientID++;
    ConnectedClient newClient;
    newClient.clientID = assignedId;
    newClient.peerReference = peer;
    
    g_ActiveClients[peer] = newClient;
    std::cout << "[Server] Authenticated Client Session. Assigned ID: " << assignedId << std::endl;

    // Send assignment confirmation packet back to the individual client
    HandshakePacket response;
    response.requestedClientID = assignedId;
    ENetPacket* outPacket = enet_packet_create(&response, sizeof(HandshakePacket), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, outPacket);
}

void HandleServerTransform(ENetPeer* peer, ENetPacket* incomingPacket) {
    if (incomingPacket->dataLength < sizeof(TransformPacket)) return;

    TransformPacket* transform = reinterpret_cast<TransformPacket*>(incomingPacket->data);
    
    // Track where this user is on the server to maintain network layer visibility filtering
    auto it = g_ActiveClients.find(peer);
    if (it != g_ActiveClients.end()) {
        it->second.currentStageId = transform->stageId;
        it->second.currentRoomId = transform->roomId;
    }

    // Forward the transformation details out to every other player sharing this visual space
    BroadcastToRoom(peer, transform->stageId, transform->roomId, transform, sizeof(TransformPacket), ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
}

void HandleServerDisconnect(ENetPeer* peer) {
    auto it = g_ActiveClients.find(peer);
    if (it != g_ActiveClients.end()) {
        uint32_t deadId = it->second.clientID;
        int8_t lastStage = it->second.currentStageId;
        int8_t lastRoom = it->second.currentRoomId;
        
        std::cout << "[Server] Client session severed: " << deadId << std::endl;
        
        // Notify localized room users to immediately strip down this puppet instance
        DisconnectPacket disconnectNotice;
        disconnectNotice.clientID = deadId;
        BroadcastToRoom(peer, lastStage, lastRoom, &disconnectNotice, sizeof(DisconnectPacket), ENET_PACKET_FLAG_RELIABLE);
        
        g_ActiveClients.erase(it);
    }
}

int main(int argc, char** argv) {
    if (enet_initialize() != 0) {
        std::cerr << "Initialization of network socket matrix failed." << std::endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 27015; // Standard port deployment mapping target

    ENetHost* serverContext = enet_host_create(&address, 32, 2, 0, 0);
    if (!serverContext) {
        std::cerr << "Critical failure: Could not instantiate structural server host backend context." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "[Server] Dedicated Twilight Princess Network layer bound to port 27015. Processing ticks..." << std::endl;

    ENetEvent netEvent;
    bool serverRunning = true;
    
    while (serverRunning) {
        // Poll for standard connection activity loops blocking every 10ms
        while (enet_host_service(serverContext, &netEvent, 10) > 0) {
            switch (netEvent.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "[Server] Unauthenticated connection initialization request received from network endpoint." << std::endl;
                    break;
                    
                case ENET_EVENT_TYPE_RECEIVE: {
                    if (netEvent.packet->dataLength > 0) {
                        uint8_t packetType = netEvent.packet->data[0];
                        if (packetType == PACKET_HANDSHAKE) {
                            HandleServerHandshake(netEvent.peer, netEvent.packet);
                        } else if (packetType == PACKET_TRANSFORM_UPDATE) {
                            HandleServerTransform(netEvent.peer, netEvent.packet);
                        }
                    }
                    enet_packet_destroy(netEvent.packet);
                    break;
                }
                
                case ENET_EVENT_TYPE_DISCONNECT:
                    HandleServerDisconnect(netEvent.peer);
                    break;
                    
                default:
                    break;
            }
        }
    }

    enet_host_destroy(serverContext);
    return EXIT_SUCCESS;
}