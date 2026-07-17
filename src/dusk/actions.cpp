#include "dusk/actions.hpp"

#include "binding_label.hpp"

#include "dusk/config.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "dusk/mods/svc/config.hpp"
#include "dusk/mods/svc/player_controller.hpp"
#include "dusk/mods/svc/slot_map.hpp"
#include "dusk/ui/ui.hpp"

#include "aurora/lib/logging.hpp"

#include <SDL3/SDL_keyboard.h>

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <charconv>
#include <cstring>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace dusk::actions {
namespace {

aurora::Module Log("dusk::actions");

using mods::svc::SlotMap;

struct PressState {
    bool down = false;
    bool prevDown = false;
};

struct ActionSlot {
    std::string name;
    std::string displayName;
    Delivery delivery = Delivery::Gameplay;
    Binding defaultBinding{};
    Binding binding{};
    std::unique_ptr<ConfigVar<std::string>> configVar;
    PressState press{};
    CallbackFn callback = nullptr;
    void* userData = nullptr;
    void (*userDataCleanup)(void*) = nullptr;
};

struct CaptureSlot {
    Handle actionHandle = kInvalidHandle;
    CaptureFilter filter = CaptureFilter::Either;
    CaptureFn callback = nullptr;
    void* userData = nullptr;
    CaptureUserDataCleanupFn userDataCleanup = nullptr;
    // Snapshot of keys/buttons/triggers already down when capture started, so holding the
    // activate control does not immediately complete capture.
    std::array<bool, SDL_SCANCODE_COUNT> keysDownAtStart{};
    std::array<bool, SDL_GAMEPAD_BUTTON_COUNT> buttonsDownAtStart{};
    bool leftTriggerDownAtStart = false;
    bool rightTriggerDownAtStart = false;
    bool hasGamepadSnapshot = false;
};

void cleanup_capture_user_data(CaptureSlot& capture) {
    if (capture.userDataCleanup != nullptr) {
        capture.userDataCleanup(capture.userData);
        capture.userDataCleanup = nullptr;
        capture.userData = nullptr;
    }
}

bool is_action_trigger_axis(const SDL_GamepadAxis axis) {
    return axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER || axis == SDL_GAMEPAD_AXIS_RIGHT_TRIGGER;
}

bool trigger_axis_physically_down(
    const aurora::input::GameController& controller, const SDL_GamepadAxis axis) {
    if (controller.m_controller == nullptr || !is_action_trigger_axis(axis)) {
        return false;
    }
    const Sint16 value = SDL_GetGamepadAxis(controller.m_controller, axis);
    const u16 zone = axis == SDL_GAMEPAD_AXIS_LEFT_TRIGGER
                         ? controller.m_deadZones.leftTriggerActivationZone
                         : controller.m_deadZones.rightTriggerActivationZone;
    return value > static_cast<Sint16>(zone);
}

SlotMap<ActionSlot> s_actions;
SlotMap<CaptureSlot> s_captures;
SlotMap<std::unique_ptr<RebindSession>> s_rebindSessions;
Handle s_openDusklightMenuHandle = kInvalidHandle;
Handle s_turboSpeedButtonHandle = kInvalidHandle;

bool valid_action_name(const std::string_view name) {
    if (name.empty() || name.size() > 64) {
        return false;
    }
    return std::ranges::all_of(name, [](const char ch) {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') ||
               ch == '_' || ch == '-';
    });
}

bool valid_binding(const Binding& binding) {
    switch (binding.kind) {
    case BindingKind::Unbound:
        return true;
    case BindingKind::Keyboard:
        return binding.scancode != SDL_SCANCODE_UNKNOWN && binding.scancode < SDL_SCANCODE_COUNT;
    case BindingKind::GamepadButton:
        return binding.gamepad_button != SDL_GAMEPAD_BUTTON_INVALID && binding.gamepad_button >= 0 &&
               binding.gamepad_button < SDL_GAMEPAD_BUTTON_COUNT;
    case BindingKind::GamepadAxis:
        return is_action_trigger_axis(binding.gamepad_axis);
    default:
        return false;
    }
}

Binding normalize_binding(const Binding& in) {
    Binding out = make_unbound();
    out.kind = in.kind;
    switch (in.kind) {
    case BindingKind::Keyboard:
        out.scancode = in.scancode;
        break;
    case BindingKind::GamepadButton:
        out.gamepad_button = in.gamepad_button;
        break;
    case BindingKind::GamepadAxis:
        out.gamepad_axis = in.gamepad_axis;
        break;
    case BindingKind::Unbound:
    default:
        out.kind = BindingKind::Unbound;
        break;
    }
    return out;
}

std::string encode_binding(const Binding& binding) {
    switch (binding.kind) {
    case BindingKind::Keyboard:
        return fmt::format("k:{}", static_cast<int>(binding.scancode));
    case BindingKind::GamepadButton:
        return fmt::format("g:{}", static_cast<int>(binding.gamepad_button));
    case BindingKind::GamepadAxis:
        return fmt::format("a:{}", static_cast<int>(binding.gamepad_axis));
    case BindingKind::Unbound:
    default:
        return "u";
    }
}

std::optional<Binding> decode_binding(std::string_view text) {
    Binding binding = make_unbound();
    if (text == "u" || text.empty()) {
        return binding;
    }
    if (text.size() < 3 || text[1] != ':') {
        return std::nullopt;
    }
    int value = 0;
    const auto* begin = text.data() + 2;
    const auto* end = text.data() + text.size();
    if (std::from_chars(begin, end, value).ec != std::errc{}) {
        return std::nullopt;
    }
    if (text[0] == 'k') {
        binding.kind = BindingKind::Keyboard;
        binding.scancode = static_cast<SDL_Scancode>(value);
    } else if (text[0] == 'g') {
        binding.kind = BindingKind::GamepadButton;
        binding.gamepad_button = static_cast<SDL_GamepadButton>(value);
    } else if (text[0] == 'a') {
        binding.kind = BindingKind::GamepadAxis;
        binding.gamepad_axis = static_cast<SDL_GamepadAxis>(value);
    } else {
        return std::nullopt;
    }
    if (!valid_binding(binding)) {
        return std::nullopt;
    }
    return binding;
}

std::string binding_config_key(std::string_view providerId, std::string_view actionName) {
    if (providerId == kHostActionConfigId) {
        return fmt::format("actions.host.{}", actionName);
    }
    return fmt::format("actions.{}.{}", mods::escape_mod_id_for_config(providerId), actionName);
}

bool binding_physically_down(const Binding& binding) {
    switch (binding.kind) {
    case BindingKind::Keyboard: {
        int numKeys = 0;
        const bool* state = SDL_GetKeyboardState(&numKeys);
        if (state == nullptr || static_cast<int>(binding.scancode) < 0 ||
            static_cast<int>(binding.scancode) >= numKeys)
        {
            return false;
        }
        return state[binding.scancode];
    }
    case BindingKind::GamepadButton: {
        const auto* controller = mods::svc::primary_player_controller();
        if (controller == nullptr) {
            return false;
        }
        return SDL_GetGamepadButton(controller->m_controller, binding.gamepad_button);
    }
    case BindingKind::GamepadAxis: {
        const auto* controller = mods::svc::primary_player_controller();
        if (controller == nullptr) {
            return false;
        }
        return trigger_axis_physically_down(*controller, binding.gamepad_axis);
    }
    case BindingKind::Unbound:
    default:
        return false;
    }
}

bool delivery_allows(const Delivery delivery) {
    if (delivery == Delivery::Always) {
        return true;
    }
    return !ui::any_document_visible();
}

ActionSlot* find_owned_action(mods::LoadedMod& mod, const Handle handle) {
    auto* entry = s_actions.find_owned(handle, mod);
    return entry != nullptr ? &entry->value : nullptr;
}

ActionSlot* find_host_action_slot(const Handle handle) {
    auto* entry = s_actions.find_host(handle);
    return entry != nullptr ? &entry->value : nullptr;
}

void persist_binding(ActionSlot& slot) {
    if (!slot.configVar) {
        return;
    }
    slot.configVar->setValue(encode_binding(slot.binding));
    mods::svc::config_mark_dirty();
}

void apply_binding_from_config_or_default(ActionSlot& slot) {
    slot.binding = slot.defaultBinding;
    if (!slot.configVar) {
        return;
    }
    if (const auto decoded = decode_binding(slot.configVar->getValue())) {
        slot.binding = *decoded;
    }
}

void snapshot_capture_baseline(CaptureSlot& capture) {
    capture.keysDownAtStart.fill(false);
    capture.buttonsDownAtStart.fill(false);
    capture.leftTriggerDownAtStart = false;
    capture.rightTriggerDownAtStart = false;
    capture.hasGamepadSnapshot = false;

    int numKeys = 0;
    const bool* state = SDL_GetKeyboardState(&numKeys);
    if (state != nullptr) {
        const int limit = std::min(numKeys, static_cast<int>(SDL_SCANCODE_COUNT));
        for (int i = 0; i < limit; ++i) {
            capture.keysDownAtStart[static_cast<size_t>(i)] = state[i];
        }
    }

    const auto* controller = mods::svc::primary_player_controller();
    if (controller != nullptr) {
        capture.hasGamepadSnapshot = true;
        for (int button = 0; button < SDL_GAMEPAD_BUTTON_COUNT; ++button) {
            capture.buttonsDownAtStart[static_cast<size_t>(button)] =
                SDL_GetGamepadButton(controller->m_controller, static_cast<SDL_GamepadButton>(button));
        }
        capture.leftTriggerDownAtStart =
            trigger_axis_physically_down(*controller, SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
        capture.rightTriggerDownAtStart =
            trigger_axis_physically_down(*controller, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);
    }
}

enum class CapturePollKind {
    None,
    Binding,
    Cancel,
};

struct CapturePollResult {
    CapturePollKind kind = CapturePollKind::None;
    Binding binding{};
};

CapturePollResult poll_capture_input(CaptureSlot& capture) {
    {
        int numKeys = 0;
        const bool* state = SDL_GetKeyboardState(&numKeys);
        if (state != nullptr && SDL_SCANCODE_ESCAPE < numKeys) {
            const auto idx = static_cast<size_t>(SDL_SCANCODE_ESCAPE);
            if (!state[SDL_SCANCODE_ESCAPE]) {
                capture.keysDownAtStart[idx] = false;
            } else if (!capture.keysDownAtStart[idx]) {
                return {.kind = CapturePollKind::Cancel};
            }
        }
    }

    if (capture.filter == CaptureFilter::Either || capture.filter == CaptureFilter::Keyboard) {
        int numKeys = 0;
        const bool* state = SDL_GetKeyboardState(&numKeys);
        if (state != nullptr) {
            const int limit = std::min(numKeys, static_cast<int>(SDL_SCANCODE_COUNT));
            for (int i = SDL_SCANCODE_A; i < limit; ++i) {
                const auto idx = static_cast<size_t>(i);
                if (!state[i]) {
                    capture.keysDownAtStart[idx] = false;
                    continue;
                }
                if (capture.keysDownAtStart[idx]) {
                    continue;
                }
                if (i == SDL_SCANCODE_ESCAPE || i == SDL_SCANCODE_LCTRL || i == SDL_SCANCODE_RCTRL ||
                    i == SDL_SCANCODE_LSHIFT || i == SDL_SCANCODE_RSHIFT || i == SDL_SCANCODE_LALT ||
                    i == SDL_SCANCODE_RALT || i == SDL_SCANCODE_LGUI || i == SDL_SCANCODE_RGUI ||
                    i == SDL_SCANCODE_CAPSLOCK || i == SDL_SCANCODE_NUMLOCKCLEAR ||
                    i == SDL_SCANCODE_SCROLLLOCK)
                {
                    continue;
                }
                return {.kind = CapturePollKind::Binding,
                    .binding = make_keyboard_binding(static_cast<SDL_Scancode>(i))};
            }
        }
    }

    if (capture.filter == CaptureFilter::Either || capture.filter == CaptureFilter::Gamepad) {
        const auto* controller = mods::svc::primary_player_controller();
        if (controller != nullptr) {
            for (int button = 0; button < SDL_GAMEPAD_BUTTON_COUNT; ++button) {
                const auto idx = static_cast<size_t>(button);
                const bool down = SDL_GetGamepadButton(
                    controller->m_controller, static_cast<SDL_GamepadButton>(button));
                if (!down) {
                    if (capture.hasGamepadSnapshot) {
                        capture.buttonsDownAtStart[idx] = false;
                    }
                    continue;
                }
                if (capture.hasGamepadSnapshot && capture.buttonsDownAtStart[idx]) {
                    continue;
                }
                Binding binding = make_unbound();
                binding.kind = BindingKind::GamepadButton;
                binding.gamepad_button = static_cast<SDL_GamepadButton>(button);
                return {.kind = CapturePollKind::Binding, .binding = binding};
            }

            const auto poll_trigger = [&](const SDL_GamepadAxis axis, bool& downAtStart)
                -> std::optional<Binding> {
                const bool down = trigger_axis_physically_down(*controller, axis);
                if (!down) {
                    if (capture.hasGamepadSnapshot) {
                        downAtStart = false;
                    }
                    return std::nullopt;
                }
                if (capture.hasGamepadSnapshot && downAtStart) {
                    return std::nullopt;
                }
                Binding binding = make_unbound();
                binding.kind = BindingKind::GamepadAxis;
                binding.gamepad_axis = axis;
                return binding;
            };
            if (auto binding =
                    poll_trigger(SDL_GAMEPAD_AXIS_LEFT_TRIGGER, capture.leftTriggerDownAtStart))
            {
                return {.kind = CapturePollKind::Binding, .binding = *binding};
            }
            if (auto binding =
                    poll_trigger(SDL_GAMEPAD_AXIS_RIGHT_TRIGGER, capture.rightTriggerDownAtStart))
            {
                return {.kind = CapturePollKind::Binding, .binding = *binding};
            }
        }
    }

    return {};
}

void fire_action_callback(
    mods::LoadedMod* mod, const Handle handle, ActionSlot& slot, const Event event) {
    if (slot.callback == nullptr) {
        return;
    }
    void* ctx = mod != nullptr ? mod->context.get() : nullptr;
    try {
        slot.callback(ctx, handle, event, slot.userData);
    } catch (const std::exception& e) {
        if (mod != nullptr) {
            mods::fail_mod(*mod, MOD_ERROR, fmt::format("Exception in Action callback: {}", e.what()));
        } else {
            Log.error("Exception in host Action callback: {}", e.what());
        }
    } catch (...) {
        if (mod != nullptr) {
            mods::fail_mod(*mod, MOD_ERROR, "Unknown exception in Action callback");
        } else {
            Log.error("Unknown exception in host Action callback");
        }
    }
}

void actions_evaluate_captures() {
    struct CompletedCapture {
        uint64_t handle = 0;
        Binding binding{};
    };
    std::vector<uint64_t> liveCaptures;
    s_captures.for_each([&](const uint64_t handle, const auto&) { liveCaptures.push_back(handle); });

    std::vector<uint64_t> cancelledCaptures;
    std::vector<CompletedCapture> completedCaptures;
    for (const auto handle : liveCaptures) {
        auto* entry = s_captures.find(handle);
        if (entry == nullptr) {
            continue;
        }
        const auto polled = poll_capture_input(entry->value);
        if (polled.kind == CapturePollKind::Cancel) {
            cancelledCaptures.push_back(handle);
        } else if (polled.kind == CapturePollKind::Binding) {
            completedCaptures.push_back({handle, polled.binding});
        }
    }
    for (const auto handle : cancelledCaptures) {
        if (auto taken = s_captures.take(handle)) {
            cleanup_capture_user_data(taken->value);
        }
    }
    for (const auto& completed : completedCaptures) {
        auto taken = s_captures.take(completed.handle);
        if (!taken) {
            continue;
        }
        auto& capture = taken->value;
        auto* actionEntry = s_actions.find(capture.actionHandle);
        if (actionEntry == nullptr || actionEntry->owner != taken->owner ||
            capture.callback == nullptr)
        {
            cleanup_capture_user_data(capture);
            continue;
        }
        void* ctx = taken->owner != nullptr ? taken->owner->context.get() : nullptr;
        try {
            capture.callback(ctx, capture.actionHandle, completed.binding, capture.userData);
        } catch (const std::exception& e) {
            if (taken->owner != nullptr) {
                mods::fail_mod(*taken->owner, MOD_ERROR,
                    fmt::format("Exception in Action capture callback: {}", e.what()));
            } else {
                Log.error("Exception in host Action capture callback: {}", e.what());
            }
        } catch (...) {
            if (taken->owner != nullptr) {
                mods::fail_mod(
                    *taken->owner, MOD_ERROR, "Unknown exception in Action capture callback");
            } else {
                Log.error("Unknown exception in host Action capture callback");
            }
        }
        cleanup_capture_user_data(capture);
    }
}

void actions_evaluate_presses() {
    s_actions.for_each([&](const uint64_t handle, const auto& entry) {
        auto& slot = const_cast<ActionSlot&>(entry.value);
        const bool allow = delivery_allows(slot.delivery);
        auto& press = slot.press;
        press.prevDown = press.down;
        const bool physical = binding_physically_down(slot.binding);
        press.down = allow && physical;
        if (press.down && !press.prevDown) {
            fire_action_callback(entry.owner, handle, slot, Event::Pressed);
        } else if (!press.down && press.prevDown) {
            fire_action_callback(entry.owner, handle, slot, Event::Released);
        }
    });
}

uint64_t s_actionEvalEpoch = 0;
uint64_t s_actionEvaluatedEpoch = ~uint64_t{0};

void actions_commit_eval_epoch() {
    if (s_actionEvaluatedEpoch == s_actionEvalEpoch) {
        return;
    }
    s_actionEvaluatedEpoch = s_actionEvalEpoch;
    actions_evaluate_captures();
    actions_evaluate_presses();
}

void drop_action_slot(ActionSlot& slot) {
    if (slot.userDataCleanup != nullptr) {
        slot.userDataCleanup(slot.userData);
        slot.userDataCleanup = nullptr;
        slot.userData = nullptr;
    }
    if (slot.configVar) {
        config::unregister(*slot.configVar);
        slot.configVar.reset();
    }
}

bool name_already_live(mods::LoadedMod* mod, std::string_view name) {
    bool found = false;
    s_actions.for_each([&](uint64_t, const auto& entry) {
        if (entry.owner == mod && entry.value.name == name) {
            found = true;
        }
    });
    return found;
}

Result fill_action_slot(ActionSlot& slot, std::string_view providerId, const Desc& desc) {
    slot.name = desc.name;
    slot.displayName = desc.display_name;
    slot.delivery = desc.delivery;
    slot.callback = desc.callback;
    slot.userData = desc.user_data;
    slot.userDataCleanup = desc.user_data_cleanup;
    slot.defaultBinding = normalize_binding(desc.default_binding);
    const auto key = binding_config_key(providerId, slot.name);
    if (config::GetConfigVar(key) != nullptr) {
        Log.error("[{}] Action '{}' conflicts with config key '{}'", providerId, slot.name, key);
        return Result::Conflict;
    }
    slot.configVar =
        std::make_unique<ConfigVar<std::string>>(key, encode_binding(slot.defaultBinding));
    config::Register(*slot.configVar);
    apply_binding_from_config_or_default(slot);
    return Result::Ok;
}

Result register_host_action(const Desc& desc, Handle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = kInvalidHandle;
    }
    if (!valid_action_name(desc.name) || desc.display_name.empty() ||
        (desc.delivery != Delivery::Gameplay && desc.delivery != Delivery::Always) ||
        !valid_binding(desc.default_binding))
    {
        return Result::InvalidArgument;
    }
    if (name_already_live(nullptr, desc.name)) {
        Log.error("[host] Action '{}' conflicts with an existing live host registration", desc.name);
        return Result::Conflict;
    }

    ActionSlot slot{};
    if (const auto result = fill_action_slot(slot, kHostActionConfigId, desc); result != Result::Ok) {
        return result;
    }
    if (slot.binding.kind == BindingKind::Unbound &&
        slot.defaultBinding.kind != BindingKind::Unbound)
    {
        slot.binding = slot.defaultBinding;
        persist_binding(slot);
    }
    const auto handle = s_actions.emplace_host(std::move(slot));
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return Result::Ok;
}

bool host_registered_was_pressed(const Handle action) {
    const auto* slot = find_host_action_slot(action);
    if (slot == nullptr) {
        return false;
    }
    return slot->press.down && !slot->press.prevDown;
}

void actions_register_builtin_host_actions() {
    if (s_openDusklightMenuHandle == kInvalidHandle) {
        Desc desc{};
        desc.name = host::kOpenDusklightMenu;
        desc.display_name = "Open Dusklight Menu";
        desc.delivery = Delivery::Always;
        desc.default_binding = make_keyboard_binding(SDL_SCANCODE_F1);

        if (register_host_action(desc, &s_openDusklightMenuHandle) != Result::Ok) {
            Log.error("Failed to register host Action '{}'", host::kOpenDusklightMenu);
        }
    }

    if (s_turboSpeedButtonHandle == kInvalidHandle) {
        Desc desc{};
        desc.name = host::kTurboSpeedButton;
        desc.display_name = "Turbo Speed";
        desc.delivery = Delivery::Gameplay;
        desc.default_binding = make_keyboard_binding(SDL_SCANCODE_TAB);

        if (register_host_action(desc, &s_turboSpeedButtonHandle) != Result::Ok) {
            Log.error("Failed to register host Action '{}'", host::kTurboSpeedButton);
        }
    }
}

void erase_captures_for_owner(mods::LoadedMod* owner) {
    if (owner != nullptr) {
        auto entries = s_captures.take_all(*owner);
        for (auto& entry : entries) {
            cleanup_capture_user_data(entry.value);
        }
    } else {
        auto entries = s_captures.take_all_host();
        for (auto& entry : entries) {
            cleanup_capture_user_data(entry.value);
        }
    }
}

Result begin_capture_for_owner(mods::LoadedMod* owner, const Handle action, const CaptureFilter filter,
    CaptureFn callback, void* userData, CaptureHandle* outHandle,
    CaptureUserDataCleanupFn userDataCleanup) {
    if (outHandle != nullptr) {
        *outHandle = kInvalidCaptureHandle;
    }
    if (callback == nullptr ||
        (filter != CaptureFilter::Either && filter != CaptureFilter::Keyboard &&
            filter != CaptureFilter::Gamepad))
    {
        return Result::InvalidArgument;
    }
    if (owner != nullptr) {
        if (find_owned_action(*owner, action) == nullptr) {
            return Result::InvalidArgument;
        }
    } else if (find_host_action_slot(action) == nullptr) {
        return Result::InvalidArgument;
    }
    erase_captures_for_owner(owner);

    CaptureSlot capture{};
    capture.actionHandle = action;
    capture.filter = filter;
    capture.callback = callback;
    capture.userData = userData;
    capture.userDataCleanup = userDataCleanup;
    snapshot_capture_baseline(capture);

    const auto handle =
        owner != nullptr ? s_captures.emplace(*owner, std::move(capture)) : s_captures.emplace_host(std::move(capture));
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return Result::Ok;
}

bool erase_capture_handle(mods::LoadedMod* owner, const CaptureHandle handle) {
    std::optional<SlotMap<CaptureSlot>::Entry> taken;
    if (owner != nullptr) {
        taken = s_captures.take_owned(handle, *owner);
    } else {
        taken = s_captures.take_host(handle);
    }
    if (!taken) {
        return false;
    }
    cleanup_capture_user_data(taken->value);
    return true;
}

Result set_binding_for_owner(mods::LoadedMod* owner, const Handle action, const Binding& binding) {
    if (!valid_binding(binding)) {
        return Result::InvalidArgument;
    }
    ActionSlot* slot =
        owner != nullptr ? find_owned_action(*owner, action) : find_host_action_slot(action);
    if (slot == nullptr) {
        return Result::InvalidArgument;
    }
    slot->binding = normalize_binding(binding);
    persist_binding(*slot);
    return Result::Ok;
}

}  // namespace

void ensure_host_actions_registered() {
    actions_register_builtin_host_actions();
}

void ensure_actions_evaluated() {
    ++s_actionEvalEpoch;
    actions_commit_eval_epoch();
}

void evaluate_frame() {
    if (s_actionEvaluatedEpoch == s_actionEvalEpoch) {
        return;
    }
    ++s_actionEvalEpoch;
    actions_commit_eval_epoch();
}

void shutdown_host() {
    s_rebindSessions.erase_all_host();
    erase_captures_for_owner(nullptr);
    auto entries = s_actions.take_all_host();
    for (auto& entry : entries) {
        drop_action_slot(entry.value);
    }
    s_openDusklightMenuHandle = kInvalidHandle;
    s_turboSpeedButtonHandle = kInvalidHandle;
}

void remove_mod(mods::LoadedMod& mod) {
    s_rebindSessions.erase_all(mod);
    erase_captures_for_owner(&mod);
    auto entries = s_actions.take_all(mod);
    for (auto& entry : entries) {
        drop_action_slot(entry.value);
    }
}

std::vector<ActionInfo> list_host_actions() {
    std::vector<ActionInfo> out;
    s_actions.for_each([&](const uint64_t handle, const auto& entry) {
        if (entry.owner != nullptr) {
            return;
        }
        out.push_back(ActionInfo{
            .handle = handle,
            .name = entry.value.name,
            .displayName = entry.value.displayName,
        });
    });
    return out;
}

Result get_host_binding(const Handle action, Binding& outBinding) {
    const auto* slot = find_host_action_slot(action);
    if (slot == nullptr) {
        return Result::InvalidArgument;
    }
    outBinding = slot->binding;
    return Result::Ok;
}

Result get_host_default_binding(const Handle action, Binding& outBinding) {
    const auto* slot = find_host_action_slot(action);
    if (slot == nullptr) {
        return Result::InvalidArgument;
    }
    outBinding = slot->defaultBinding;
    return Result::Ok;
}

Result set_host_binding(const Handle action, const Binding& binding) {
    return set_binding_for_owner(nullptr, action, binding);
}

Result reset_host_binding(const Handle action) {
    auto* slot = find_host_action_slot(action);
    if (slot == nullptr) {
        return Result::InvalidArgument;
    }
    slot->binding = slot->defaultBinding;
    persist_binding(*slot);
    return Result::Ok;
}

Result begin_host_capture(const Handle action, const CaptureFilter filter, CaptureFn callback,
    void* userData, CaptureHandle* outHandle, CaptureUserDataCleanupFn userDataCleanup) {
    return begin_capture_for_owner(
        nullptr, action, filter, callback, userData, outHandle, userDataCleanup);
}

Result cancel_host_capture(const CaptureHandle handle) {
    if (handle == kInvalidCaptureHandle) {
        return Result::InvalidArgument;
    }
    if (!erase_capture_handle(nullptr, handle)) {
        return Result::InvalidArgument;
    }
    return Result::Ok;
}

bool host_capture_is_active(const CaptureHandle handle) {
    return handle != kInvalidCaptureHandle && s_captures.find_host(handle) != nullptr;
}

bool any_capture_active() {
    bool active = false;
    s_captures.for_each([&](const uint64_t, const auto&) { active = true; });
    return active;
}

bool open_dusklight_menu_was_pressed() {
    return host_registered_was_pressed(s_openDusklightMenuHandle);
}

bool open_dusklight_menu_should_suppress_default_back() {
    const auto* slot = find_host_action_slot(s_openDusklightMenuHandle);
    return slot != nullptr && (slot->binding.kind == BindingKind::GamepadButton ||
                               slot->binding.kind == BindingKind::GamepadAxis);
}

bool open_dusklight_menu_should_suppress_default_f1() {
    const auto* slot = find_host_action_slot(s_openDusklightMenuHandle);
    return slot != nullptr && slot->binding.kind == BindingKind::Keyboard;
}

std::string open_dusklight_menu_keyboard_label() {
    const auto* slot = find_host_action_slot(s_openDusklightMenuHandle);
    if (slot == nullptr || slot->binding.kind != BindingKind::Keyboard) {
        return "F1";
    }
    return format_binding_label(slot->binding, nullptr);
}

std::string open_dusklight_menu_gamepad_label(
    SDL_Gamepad* gamepad, const std::string_view unbound_fallback) {
    const auto* slot = find_host_action_slot(s_openDusklightMenuHandle);
    if (slot == nullptr || (slot->binding.kind != BindingKind::GamepadButton &&
                            slot->binding.kind != BindingKind::GamepadAxis))
    {
        return std::string{unbound_fallback};
    }
    return format_binding_label(slot->binding, gamepad);
}

bool turbo_speed_button_is_held() {
    const auto* slot = find_host_action_slot(s_turboSpeedButtonHandle);
    return slot != nullptr && slot->press.down;
}

void RebindSession::clear() const {
    mCaptureHandle = kInvalidCaptureHandle;
    mCapturingAction = kInvalidHandle;
}

void RebindSession::sync_capture_state() const {
    if (mCaptureHandle == kInvalidCaptureHandle) {
        return;
    }
    bool active = false;
    if (mOwner != nullptr) {
        if (is_capture_active(*mOwner, mCaptureHandle, active) != Result::Ok) {
            active = false;
        }
    } else {
        active = host_capture_is_active(mCaptureHandle);
    }
    if (!active) {
        clear();
    }
}

bool RebindSession::is_capturing() const {
    sync_capture_state();
    return mCaptureHandle != kInvalidCaptureHandle;
}

bool RebindSession::is_capturing(const Handle action) const {
    return is_capturing() && mCapturingAction == action;
}

Handle RebindSession::capturing_action() const {
    sync_capture_state();
    return mCapturingAction;
}

const char* RebindSession::capturing_prompt(const Handle action) const {
    return is_capturing(action) ? kRebindCapturingPrompt : nullptr;
}

void RebindSession::on_complete(
    void*, const Handle action, const Binding& binding, void* userData) {
    auto* self = static_cast<RebindSession*>(userData);
    if (self == nullptr) {
        return;
    }
    self->clear();
    if (self->mOwner != nullptr) {
        set_binding(*self->mOwner, action, binding);
    } else {
        set_host_binding(action, binding);
    }
}

Result RebindSession::begin(const Handle action, const CaptureFilter filter) {
    cancel();
    CaptureHandle handle = kInvalidCaptureHandle;
    const Result result = mOwner != nullptr
                              ? begin_capture(
                                    *mOwner, action, filter, &RebindSession::on_complete, this, &handle)
                              : begin_host_capture(
                                    action, filter, &RebindSession::on_complete, this, &handle);
    if (result != Result::Ok) {
        return result;
    }
    mCaptureHandle = handle;
    mCapturingAction = action;
    return Result::Ok;
}

void RebindSession::cancel() {
    if (mCaptureHandle == kInvalidCaptureHandle) {
        return;
    }
    if (mOwner != nullptr) {
        cancel_capture(*mOwner, mCaptureHandle);
    } else {
        cancel_host_capture(mCaptureHandle);
    }
    clear();
}

Result register_action(mods::LoadedMod& mod, const Desc& desc, Handle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = kInvalidHandle;
    }
    if (!valid_action_name(desc.name) || desc.display_name.empty() ||
        (desc.delivery != Delivery::Gameplay && desc.delivery != Delivery::Always) ||
        !valid_binding(desc.default_binding))
    {
        return Result::InvalidArgument;
    }

    if (name_already_live(&mod, desc.name)) {
        Log.error("[{}] Action '{}' conflicts with an existing live registration", mod.metadata.id,
            desc.name);
        return Result::Conflict;
    }

    ActionSlot slot{};
    if (const auto result = fill_action_slot(slot, mod.metadata.id, desc); result != Result::Ok) {
        return result;
    }

    const auto handle = s_actions.emplace(mod, std::move(slot));
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return Result::Ok;
}

Result unregister_action(mods::LoadedMod& mod, const Handle action) {
    if (action == kInvalidHandle) {
        return Result::InvalidArgument;
    }
    auto taken = s_actions.take_owned(action, mod);
    if (!taken) {
        return Result::InvalidArgument;
    }

    std::vector<uint64_t> bound;
    s_captures.for_each([&](const uint64_t handle, const auto& entry) {
        if (entry.owner == &mod && entry.value.actionHandle == action) {
            bound.push_back(handle);
        }
    });
    for (const auto handle : bound) {
        erase_capture_handle(&mod, handle);
    }

    drop_action_slot(taken->value);
    return Result::Ok;
}

Result get_binding(mods::LoadedMod& mod, const Handle action, Binding& outBinding) {
    const auto* slot = find_owned_action(mod, action);
    if (slot == nullptr) {
        return Result::InvalidArgument;
    }
    outBinding = slot->binding;
    return Result::Ok;
}

Result get_default_binding(mods::LoadedMod& mod, const Handle action, Binding& outBinding) {
    const auto* slot = find_owned_action(mod, action);
    if (slot == nullptr) {
        return Result::InvalidArgument;
    }
    outBinding = slot->defaultBinding;
    return Result::Ok;
}

Result set_binding(mods::LoadedMod& mod, const Handle action, const Binding& binding) {
    return set_binding_for_owner(&mod, action, binding);
}

Result reset_binding(mods::LoadedMod& mod, const Handle action) {
    auto* slot = find_owned_action(mod, action);
    if (slot == nullptr) {
        return Result::InvalidArgument;
    }
    slot->binding = slot->defaultBinding;
    persist_binding(*slot);
    return Result::Ok;
}

Result is_down(mods::LoadedMod& mod, const Handle action, bool& outDown) {
    outDown = false;
    const auto* slot = find_owned_action(mod, action);
    if (slot == nullptr) {
        return Result::InvalidArgument;
    }
    outDown = slot->press.down;
    return Result::Ok;
}

Result was_pressed(mods::LoadedMod& mod, const Handle action, bool& outPressed) {
    outPressed = false;
    const auto* slot = find_owned_action(mod, action);
    if (slot == nullptr) {
        return Result::InvalidArgument;
    }
    outPressed = slot->press.down && !slot->press.prevDown;
    return Result::Ok;
}

Result was_released(mods::LoadedMod& mod, const Handle action, bool& outReleased) {
    outReleased = false;
    const auto* slot = find_owned_action(mod, action);
    if (slot == nullptr) {
        return Result::InvalidArgument;
    }
    outReleased = !slot->press.down && slot->press.prevDown;
    return Result::Ok;
}

Result is_held(mods::LoadedMod& mod, const Handle action, bool& outHeld) {
    outHeld = false;
    const auto* slot = find_owned_action(mod, action);
    if (slot == nullptr) {
        return Result::InvalidArgument;
    }
    outHeld = slot->press.down && slot->press.prevDown;
    return Result::Ok;
}

Result begin_capture(mods::LoadedMod& mod, const Handle action, const CaptureFilter filter,
    CaptureFn callback, void* userData, CaptureHandle* outHandle,
    CaptureUserDataCleanupFn userDataCleanup) {
    return begin_capture_for_owner(
        &mod, action, filter, callback, userData, outHandle, userDataCleanup);
}

Result cancel_capture(mods::LoadedMod& mod, const CaptureHandle handle) {
    if (handle == kInvalidCaptureHandle) {
        return Result::InvalidArgument;
    }
    if (!erase_capture_handle(&mod, handle)) {
        return Result::InvalidArgument;
    }
    return Result::Ok;
}

Result is_capture_active(mods::LoadedMod& mod, const CaptureHandle handle, bool& outActive) {
    outActive = false;
    if (handle == kInvalidCaptureHandle) {
        return Result::InvalidArgument;
    }
    outActive = s_captures.find_owned(handle, mod) != nullptr;
    return Result::Ok;
}

Result create_rebind_session(mods::LoadedMod& mod, RebindSessionHandle* outSession) {
    if (outSession == nullptr) {
        return Result::InvalidArgument;
    }
    *outSession = kInvalidRebindSessionHandle;
    auto session = std::make_unique<RebindSession>(mod);
    *outSession = s_rebindSessions.emplace(mod, std::move(session));
    return Result::Ok;
}

Result destroy_rebind_session(mods::LoadedMod& mod, const RebindSessionHandle session) {
    if (session == kInvalidRebindSessionHandle) {
        return Result::InvalidArgument;
    }
    auto taken = s_rebindSessions.take_owned(session, mod);
    if (!taken || !taken->value) {
        return Result::InvalidArgument;
    }
    taken->value->cancel();
    return Result::Ok;
}

Result begin_rebind_session(mods::LoadedMod& mod, const RebindSessionHandle session,
    const Handle action, const CaptureFilter filter) {
    auto* entry = s_rebindSessions.find_owned(session, mod);
    if (entry == nullptr || !entry->value) {
        return Result::InvalidArgument;
    }
    return entry->value->begin(action, filter);
}

Result cancel_rebind_session(mods::LoadedMod& mod, const RebindSessionHandle session) {
    auto* entry = s_rebindSessions.find_owned(session, mod);
    if (entry == nullptr || !entry->value) {
        return Result::InvalidArgument;
    }
    entry->value->cancel();
    return Result::Ok;
}

Result query_rebind_session(mods::LoadedMod& mod, const RebindSessionHandle session,
    bool& outCapturing, Handle* outAction) {
    outCapturing = false;
    if (outAction != nullptr) {
        *outAction = kInvalidHandle;
    }
    auto* entry = s_rebindSessions.find_owned(session, mod);
    if (entry == nullptr || !entry->value) {
        return Result::InvalidArgument;
    }
    outCapturing = entry->value->is_capturing();
    if (outAction != nullptr) {
        *outAction = entry->value->capturing_action();
    }
    return Result::Ok;
}

}  // namespace dusk::actions
