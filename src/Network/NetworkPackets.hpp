#pragma once
#include <cstdint>

#pragma pack(push, 1)

enum PacketType : uint8_t {
    PACKET_HANDSHAKE = 0,
    PACKET_PLAYER_DISCONNECT,
    PACKET_TRANSFORM_UPDATE,
    PACKET_WORLD_STATE_SYNC
};

struct HandshakePacket {
    uint8_t type = PACKET_HANDSHAKE;
    uint32_t magicValue = 0x54504F4E; // 'TPON' - Twilight Princess Online
    uint32_t requestedClientID;
};

struct DisconnectPacket {
    uint8_t type = PACKET_PLAYER_DISCONNECT;
    uint32_t clientID;
};

struct TransformPacket {
    uint8_t type = PACKET_TRANSFORM_UPDATE;
    uint32_t clientID;
    
    // Position vectors
    float posX;
    float posY;
    float posZ;
    
    // Angular rotation (Facing angle)
    float rotY;
    
    // Engine State
    uint16_t animationId;
    uint32_t actionFlags; // Bitmask for rolling, swimming, attacking, etc.
    uint16_t activeItemId;
    
    // Instance Filtering
    int8_t stageId;
    int8_t roomId;
};

struct WorldStatePacket {
    uint8_t type = PACKET_WORLD_STATE_SYNC;
    uint32_t clientID;
    uint32_t eventRegIndex; // Index for world event registers
    uint8_t bitmaskValue;   // Flag changes (chests, triggers, doors)
};

#pragma pack(pop)