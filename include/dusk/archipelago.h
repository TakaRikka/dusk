#pragma once
// Dusklight Archipelago bridge module.
//
// Lifecycle (wired next to dusk::speedrun):
//   dusk::archipelago::onGameFrame();  // per game frame: drain item queue, grant items
//   dusk::archipelago::update();       // per frame: service socket (lazy-inits the listener)
//   dusk::archipelago::shutdown();     // optional, at exit
// See Dusklight-AP/DESIGN.md for the bridge protocol and the dSv_info_c offset map.

#include <cstdint>

namespace dusk::archipelago {

void init(int port = 17354);
void onGameFrame();
void update();
void shutdown();

bool isListening();
bool isClientConnected();
int  listenPort();

}  // namespace dusk::archipelago
