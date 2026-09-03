#include "item.hpp"

namespace dusk::mods {

static BossItemActorSpeeds bossItemActorSpeeds{};
void set_boss_item_actor_speeds(f32 f, f32 y) {
    bossItemActorSpeeds.f = f;
    bossItemActorSpeeds.y = y;
}

BossItemActorSpeeds get_boss_item_actor_speeds() {
    return bossItemActorSpeeds;
}

}
