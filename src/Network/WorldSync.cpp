#include "NetworkManager.hpp"
#include <iostream>

// Theoretical layout tracking Nintendo's global save/state manager
struct StageEventReg {
    uint8_t memoryBitmask[32]; // Keeps tabs on open chests, active doors, etc.
};

extern "C" StageEventReg* dComIfGs_getStageEventReg(); 

// Monitored Hook: Call this function whenever the game writes to a stage register
void OnLocalWorldFlagChanged(uint32_t registerIndex, uint8_t updatedBitmaskValue) {
    // 1. Commit the flag locally to our own game instance
    StageEventReg* regs = dComIfGs_getStageEventReg();
    if (regs != nullptr && registerIndex < 32) {
        regs->memoryBitmask[registerIndex] |= updatedBitmaskValue;
    }

    // 2. Dispatch a packet so other players see the chest open or door unlock
    NetworkManager& net = NetworkManager::GetInstance();
    if (net.StartClient /* Verify context connectivity states */) {
        WorldStatePacket packet;
        packet.type = PACKET_WORLD_STATE_SYNC;
        packet.eventRegIndex = registerIndex;
        packet.bitmaskValue = updatedBitmaskValue;

        // (We encapsulate this via the network client instance context)
        // net.SendPacketToServer(&packet, sizeof(WorldStatePacket), ENET_PACKET_FLAG_RELIABLE);
    }
}

// Inbound Execution: Processes world alterations from other network sessions
void HandleInboundWorldSync(const WorldStatePacket& incomingPacket) {
    StageEventReg* regs = dComIfGs_getStageEventReg();
    if (regs == nullptr || incomingPacket.eventRegIndex >= 32) return;

    std::cout << "[Network Sync] Remote player triggered event bitmask index: " 
              << (int)incomingPacket.eventRegIndex << " with value: " 
              << (int)incomingPacket.bitmaskValue << std::endl;

    // Apply the bitwise OR update to the client's memory map
    regs->memoryBitmask[incomingPacket.eventRegIndex] |= incomingPacket.bitmaskValue;

    // CRITICAL ENGINE TRIGGER:
    // Inform the engine layout renderer to reload current room models (e.g., swapping a closed chest mesh for an open one)
    // fopAcM_searchActorByProcessName(PROC_OBJ_CHEST) -> trigger state updates;
}