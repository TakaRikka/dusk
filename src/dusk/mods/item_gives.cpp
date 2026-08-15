#include "item.hpp"

#include "dusk/mod_loader.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "dusk/mods/svc/item.hpp"

#include "aurora/lib/logging.hpp"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_item_data.h"
#include "f_op/f_op_actor_mng.h"

#include <fmt/format.h>

#include <deque>
#include <exception>
#include <string>
#include <unordered_map>
#include <vector>

namespace dusk::mods {
namespace {

aurora::Module Log{"dusk::mods::item_gives"};

// deque keeps previously returned c_str pointers valid if a callback interns another name.
std::deque<std::string> s_giveNames;
std::unordered_map<std::string, uint32_t> s_giveNameIds;

const char* item_give_name(uint32_t tag) {
    if (tag == 0 || tag > s_giveNames.size()) {
        return nullptr;
    }
    return s_giveNames[tag - 1].c_str();
}

struct GiveObserver {
    ItemGiveHandle handle = 0;
    ItemGiveObserveFn fn = nullptr;
    void* userData = nullptr;
};

struct PendingObserver {
    LoadedMod* mod = nullptr;
    ItemGiveObserveFn fn = nullptr;
    void* userData = nullptr;
};

std::unordered_map<LoadedMod*, std::vector<GiveObserver>> s_modObservers;
ItemGiveHandle s_nextGiveHandle = 1;
size_t s_observerCount = 0;

void notify_gives(const char* checkName, uint8_t itemNo, fopAc_ac_c* giver, ItemGiveOrigin origin) {
    if (s_observerCount == 0) {
        return;
    }

    // Callbacks may change registrations, so copy them before invoking one.
    std::vector<PendingObserver> observers;
    for (auto& mod : ModLoader::instance().mods()) {
        if (!mod.active) {
            continue;
        }
        const auto modIt = s_modObservers.find(&mod);
        if (modIt == s_modObservers.end()) {
            continue;
        }
        for (const auto& observer : modIt->second) {
            observers.push_back({.mod = &mod, .fn = observer.fn, .userData = observer.userData});
        }
    }

    const ItemGiveInfo info{
        .check_name = checkName,
        .giver_actor = giver,
        .item = itemNo,
        .origin = static_cast<uint8_t>(origin),
    };
    for (const auto& observer : observers) {
        if (!observer.mod->active) {
            continue;
        }
        try {
            observer.fn(observer.mod->context.get(), &info, observer.userData);
        } catch (const std::exception& e) {
            fail_mod(*observer.mod, MOD_ERROR,
                fmt::format("Exception in item give observer: {}", e.what()));
        } catch (...) {
            fail_mod(*observer.mod, MOD_ERROR, "Unknown exception in item give observer");
        }
    }
}

constexpr size_t kGiveQueueLimit = 64;
constexpr int kGiveMaxRetries = 5;

struct QueuedGive {
    LoadedMod* owner = nullptr;
    uint32_t tag = 0;
    uint8_t itemNo = 0;
    bool silent = false;
    bool resolveAtDispatch = false;
};

std::deque<QueuedGive> s_giveQueue;
QueuedGive s_inFlightGive{};
uint8_t s_inFlightItem = 0;
int s_inFlightRetries = 0;
bool s_inFlight = false;
bool s_inFlightSpawned = false;
bool s_dispatchingSilent = false;

bool safe_to_dispatch() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (link == nullptr) {
        return false;
    }

    // make sure player is in a safe action and not already in an event before dispatching
    switch (link->mProcID) {
    case daAlink_c::PROC_WAIT:
    case daAlink_c::PROC_TIRED_WAIT:
    case daAlink_c::PROC_MOVE:
    case daAlink_c::PROC_WOLF_WAIT:
    case daAlink_c::PROC_WOLF_TIRED_WAIT:
    case daAlink_c::PROC_WOLF_MOVE:
    case daAlink_c::PROC_ATN_MOVE:
    case daAlink_c::PROC_WOLF_ATN_AC_MOVE:
        break;
    default:
        return false;
    }
    if (link->checkEventRun()) {
        return false;
    }

    int itemId = 0;
    return link->mMsgFlow.getEventId(&itemId) == 0;
}

bool resolve_queued_give(const QueuedGive& give, ItemGiveOrigin origin, uint8_t& outItem) {
    outItem = give.itemNo;
    if (give.resolveAtDispatch) {
        outItem = item_check(item_give_name(give.tag), give.itemNo, nullptr);
    }
    if (outItem != dItemNo_NONE_e) {
        return true;
    }

    notify_gives(item_give_name(give.tag), dItemNo_NONE_e, nullptr, origin);
    return false;
}

void dispatch_silent_give(const QueuedGive& give) {
    uint8_t itemNo = 0;
    if (!resolve_queued_give(give, ITEM_GIVE_ORIGIN_QUEUE_SILENT, itemNo)) {
        return;
    }

    Log.debug("dispatching silent item {:#x} for '{}'", itemNo,
        item_give_name(give.tag) != nullptr ? item_give_name(give.tag) : "");
    s_dispatchingSilent = true;
    execItemGet(itemNo, give.tag, nullptr);
    s_dispatchingSilent = false;
}

void dispatch_demo_give() {
    Log.debug("dispatching item {:#x} for '{}'", s_inFlightItem,
        item_give_name(s_inFlightGive.tag) != nullptr ? item_give_name(s_inFlightGive.tag) : "");

    daAlink_c* link = daAlink_getAlinkActorClass();
    dComIfGp_getEvent()->setGtItm(s_inFlightItem);
    link->procCoGetItemInit();
    const s16 eventIndex = dComIfGp_getEventManager().getEventIdx(link, "DEFAULT_GETITEM", 0xFF);
    fopAcM_orderChangeEventId(link, eventIndex, 1, 0xFFFF);
}

}  // namespace

uint32_t item_give_tag(const char* name) {
    if (name == nullptr || *name == '\0') {
        return 0;
    }
    if (const auto it = s_giveNameIds.find(name); it != s_giveNameIds.end()) {
        return it->second;
    }

    s_giveNames.emplace_back(name);
    const auto tag = static_cast<uint32_t>(s_giveNames.size());
    s_giveNameIds.emplace(s_giveNames.back(), tag);
    return tag;
}

uint8_t item_check_tagged(uint32_t giveTag, uint8_t itemNo, fopAc_ac_c* giver) {
    const char* name = item_give_name(giveTag);
    return name != nullptr ? item_check(name, itemNo, giver) : itemNo;
}

void item_check_enqueue(const char* name, uint8_t itemNo) {
    if (s_giveQueue.size() >= kGiveQueueLimit) {
        Log.warn("item give queue is full; dropping check '{}'", name != nullptr ? name : "");
        return;
    }
    s_giveQueue.push_back({
        .tag = item_give_tag(name),
        .itemNo = itemNo,
        .resolveAtDispatch = true,
    });
}

void item_granted(uint8_t itemNo, uint32_t giveTag, fopAc_ac_c* giver) {
    ItemGiveOrigin origin = ITEM_GIVE_ORIGIN_GAME;
    if (s_dispatchingSilent) {
        origin = ITEM_GIVE_ORIGIN_QUEUE_SILENT;
    } else if (s_inFlight && itemNo == s_inFlightItem && giveTag == s_inFlightGive.tag) {
        origin = ITEM_GIVE_ORIGIN_QUEUE;
        s_inFlight = false;
        s_inFlightSpawned = false;
    }
    notify_gives(item_give_name(giveTag), itemNo, giver, origin);
}

bool item_give_queue_dispatching() {
    return s_inFlight && !s_inFlightSpawned;
}

uint32_t item_give_queue_take_tag() {
    if (!item_give_queue_dispatching()) {
        return 0;
    }
    s_inFlightSpawned = true;
    return s_inFlightGive.tag;
}

namespace svc {

void item_gives_tick() {
    if ((!s_inFlight && s_giveQueue.empty()) || !safe_to_dispatch()) {
        return;
    }

    if (s_inFlight) {
        if (++s_inFlightRetries > kGiveMaxRetries) {
            Log.error("item {:#x} for '{}' did not complete after {} attempts; dropping it",
                s_inFlightItem,
                item_give_name(s_inFlightGive.tag) != nullptr ? item_give_name(s_inFlightGive.tag) :
                                                                "",
                kGiveMaxRetries);
            s_inFlight = false;
            s_inFlightSpawned = false;
            return;
        }
        s_inFlightSpawned = false;
        dispatch_demo_give();
        return;
    }

    while (!s_giveQueue.empty() && s_giveQueue.front().silent) {
        const QueuedGive give = s_giveQueue.front();
        s_giveQueue.pop_front();
        dispatch_silent_give(give);
    }
    if (s_giveQueue.empty()) {
        return;
    }

    const QueuedGive give = s_giveQueue.front();
    uint8_t itemNo = 0;
    if (!resolve_queued_give(give, ITEM_GIVE_ORIGIN_QUEUE, itemNo)) {
        s_giveQueue.pop_front();
        return;
    }

    s_giveQueue.pop_front();
    s_inFlightGive = give;
    s_inFlightItem = itemNo;
    s_inFlightRetries = 0;
    s_inFlight = true;
    s_inFlightSpawned = false;
    dispatch_demo_give();
}

void item_gives_clear() {
    if (!s_giveQueue.empty() || s_inFlight) {
        Log.info("dropping {} pending item give(s)",
            s_giveQueue.size() + static_cast<size_t>(s_inFlight));
    }
    s_giveQueue.clear();
    s_inFlight = false;
    s_inFlightSpawned = false;
}

ModResult item_give_enqueue(LoadedMod& mod, const char* checkName, uint8_t itemNo, uint32_t flags) {
    if (s_giveQueue.size() >= kGiveQueueLimit) {
        return MOD_UNAVAILABLE;
    }
    s_giveQueue.push_back({
        .owner = &mod,
        .tag = item_give_tag(checkName),
        .itemNo = itemNo,
        .silent = (flags & ITEM_GIVE_SILENT) != 0,
        .resolveAtDispatch = (flags & ITEM_GIVE_RESOLVE) != 0,
    });
    return MOD_OK;
}

ModResult item_give_add_observer(
    LoadedMod& mod, ItemGiveObserveFn fn, void* userData, ItemGiveHandle& outHandle) {
    auto& observer = s_modObservers[&mod].emplace_back();
    observer.handle = s_nextGiveHandle++;
    observer.fn = fn;
    observer.userData = userData;
    outHandle = observer.handle;
    ++s_observerCount;
    return MOD_OK;
}

ModResult item_give_remove_observer(LoadedMod& mod, ItemGiveHandle handle) {
    const auto modIt = s_modObservers.find(&mod);
    if (modIt == s_modObservers.end()) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto removed = std::erase_if(
        modIt->second, [&](const auto& observer) { return observer.handle == handle; });
    s_observerCount -= removed;
    return removed != 0 ? MOD_OK : MOD_INVALID_ARGUMENT;
}

void item_gives_remove_mod(LoadedMod& mod) {
    if (const auto modIt = s_modObservers.find(&mod); modIt != s_modObservers.end()) {
        s_observerCount -= modIt->second.size();
        s_modObservers.erase(modIt);
    }
    std::erase_if(s_giveQueue, [&](const QueuedGive& give) { return give.owner == &mod; });
    if (s_inFlight && s_inFlightGive.owner == &mod) {
        // The event system already owns this grant, so it can no longer be canceled safely.
        s_inFlightGive.owner = nullptr;
    }
}

}  // namespace svc
}  // namespace dusk::mods
