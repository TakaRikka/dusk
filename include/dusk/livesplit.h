#pragma once

#include <cstdint>

namespace dusk::speedrun {
void onGameFrame();
uint64_t getFrameCount();
void start();
void reset();
void connectLiveSplit(const char* host = "127.0.0.1", int port = 16834);
void disconnectLiveSplit();
bool consumeConnectedEvent();
bool consumeDisconnectedEvent();
void updateLiveSplit();
void shutdown();
}
