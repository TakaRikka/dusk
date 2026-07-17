#include "dusk/actions.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "dusk/mods/svc/player_controller.hpp"
#include "dusk/mods/svc/registry.hpp"

#include "mods/svc/actions.h"

#include <cstdio>
#include <cstring>
#include <string>

namespace dusk::mods::svc {
namespace {

ModResult to_mod_result(const actions::Result result) {
    switch (result) {
    case actions::Result::Ok:
        return MOD_OK;
    case actions::Result::InvalidArgument:
        return MOD_INVALID_ARGUMENT;
    case actions::Result::Conflict:
        return MOD_CONFLICT;
    case actions::Result::Error:
    default:
        return MOD_ERROR;
    }
}

actions::Binding from_abi_binding(const ActionBinding& in) {
    actions::Binding out{};
    out.kind = static_cast<actions::BindingKind>(in.kind);
    out.scancode = in.scancode;
    out.gamepad_button = in.gamepad_button;
    out.gamepad_axis = in.gamepad_axis;
    return out;
}

void to_abi_binding(const actions::Binding& in, ActionBinding& out) {
    const uint32_t structSize = out.struct_size;
    out = ACTION_BINDING_INIT;
    out.struct_size = structSize;
    out.kind = static_cast<ActionBindingKind>(in.kind);
    out.scancode = in.scancode;
    out.gamepad_button = in.gamepad_button;
    out.gamepad_axis = in.gamepad_axis;
}

struct CallbackThunk {
    ActionCallbackFn abi = nullptr;
    void* userData = nullptr;
};

void callback_thunk(void* ctx, actions::Handle action, actions::Event event, void* userData) {
    auto* thunk = static_cast<CallbackThunk*>(userData);
    if (thunk == nullptr || thunk->abi == nullptr) {
        return;
    }
    thunk->abi(static_cast<ModContext*>(ctx), action, static_cast<ActionEvent>(event), thunk->userData);
}

void callback_thunk_cleanup(void* userData) {
    delete static_cast<CallbackThunk*>(userData);
}

actions::Desc from_abi_desc(const ActionDesc& desc, void* callbackUserData) {
    actions::Desc out{};
    out.name = desc.name != nullptr ? desc.name : "";
    out.display_name = desc.display_name != nullptr ? desc.display_name : "";
    out.delivery = static_cast<actions::Delivery>(desc.delivery);
    out.default_binding = from_abi_binding(desc.default_binding);
    if (desc.callback != nullptr) {
        out.callback = callback_thunk;
        out.user_data = callbackUserData;
        out.user_data_cleanup = callback_thunk_cleanup;
    }
    return out;
}

struct CaptureThunk {
    ActionCaptureFn abi = nullptr;
    void* userData = nullptr;
};

void capture_thunk(void* ctx, actions::Handle action, const actions::Binding& binding, void* userData) {
    auto* thunk = static_cast<CaptureThunk*>(userData);
    if (thunk == nullptr || thunk->abi == nullptr) {
        return;
    }
    ActionBinding abiBinding = ACTION_BINDING_INIT;
    to_abi_binding(binding, abiBinding);
    thunk->abi(static_cast<ModContext*>(ctx), action, &abiBinding, thunk->userData);
}

void capture_thunk_cleanup(void* userData) {
    delete static_cast<CaptureThunk*>(userData);
}

ModResult actions_register_action(
    ModContext* context, const ActionDesc* desc, ActionHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(ActionDesc)) {
        return MOD_INVALID_ARGUMENT;
    }
    CallbackThunk* callbackThunk = nullptr;
    if (desc->callback != nullptr) {
        callbackThunk = new CallbackThunk{.abi = desc->callback, .userData = desc->user_data};
    }
    const auto result =
        actions::register_action(*mod, from_abi_desc(*desc, callbackThunk), outHandle);
    if (result != actions::Result::Ok) {
        delete callbackThunk;
    }
    return to_mod_result(result);
}

ModResult actions_unregister_action(ModContext* context, const ActionHandle action) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::unregister_action(*mod, action));
}

ModResult actions_get_binding(
    ModContext* context, const ActionHandle action, ActionBinding* outBinding) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outBinding == nullptr || outBinding->struct_size < sizeof(ActionBinding)) {
        return MOD_INVALID_ARGUMENT;
    }
    actions::Binding binding{};
    const auto result = actions::get_binding(*mod, action, binding);
    if (result != actions::Result::Ok) {
        return to_mod_result(result);
    }
    to_abi_binding(binding, *outBinding);
    return MOD_OK;
}

ModResult actions_get_default_binding(
    ModContext* context, const ActionHandle action, ActionBinding* outBinding) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outBinding == nullptr || outBinding->struct_size < sizeof(ActionBinding)) {
        return MOD_INVALID_ARGUMENT;
    }
    actions::Binding binding{};
    const auto result = actions::get_default_binding(*mod, action, binding);
    if (result != actions::Result::Ok) {
        return to_mod_result(result);
    }
    to_abi_binding(binding, *outBinding);
    return MOD_OK;
}

ModResult actions_set_binding(
    ModContext* context, const ActionHandle action, const ActionBinding* binding) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || binding == nullptr || binding->struct_size < sizeof(ActionBinding)) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::set_binding(*mod, action, from_abi_binding(*binding)));
}

ModResult actions_reset_binding(ModContext* context, const ActionHandle action) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::reset_binding(*mod, action));
}

ModResult actions_is_down(ModContext* context, const ActionHandle action, bool* outDown) {
    if (outDown != nullptr) {
        *outDown = false;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outDown == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::is_down(*mod, action, *outDown));
}

ModResult actions_was_pressed(ModContext* context, const ActionHandle action, bool* outPressed) {
    if (outPressed != nullptr) {
        *outPressed = false;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outPressed == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::was_pressed(*mod, action, *outPressed));
}

ModResult actions_was_released(ModContext* context, const ActionHandle action, bool* outReleased) {
    if (outReleased != nullptr) {
        *outReleased = false;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outReleased == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::was_released(*mod, action, *outReleased));
}

ModResult actions_is_held(ModContext* context, const ActionHandle action, bool* outHeld) {
    if (outHeld != nullptr) {
        *outHeld = false;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outHeld == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::is_held(*mod, action, *outHeld));
}

ModResult actions_begin_capture(ModContext* context, const ActionHandle action,
    const ActionCaptureFilter filter, ActionCaptureFn callback, void* userData,
    ActionCaptureHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || callback == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    auto* thunk = new CaptureThunk{.abi = callback, .userData = userData};
    const auto result = actions::begin_capture(*mod, action, static_cast<actions::CaptureFilter>(filter),
        capture_thunk, thunk, outHandle, capture_thunk_cleanup);
    if (result != actions::Result::Ok) {
        delete thunk;
    }
    return to_mod_result(result);
}

ModResult actions_cancel_capture(ModContext* context, const ActionCaptureHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::cancel_capture(*mod, handle));
}

ModResult actions_is_capture_active(
    ModContext* context, const ActionCaptureHandle handle, bool* outActive) {
    if (outActive != nullptr) {
        *outActive = false;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outActive == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::is_capture_active(*mod, handle, *outActive));
}

ModResult actions_format_binding_label(
    ModContext* context, const ActionBinding* binding, char* outBuf, const uint32_t outBufSize) {
    if (mod_from_context(context) == nullptr || binding == nullptr || outBuf == nullptr ||
        outBufSize == 0 || binding->struct_size < sizeof(ActionBinding))
    {
        return MOD_INVALID_ARGUMENT;
    }

    SDL_Gamepad* gamepad = nullptr;
    if (const auto* controller = primary_player_controller(); controller != nullptr) {
        gamepad = controller->m_controller;
    }
    const std::string label = actions::format_binding_label(from_abi_binding(*binding), gamepad);
    std::snprintf(outBuf, outBufSize, "%s", label.c_str());
    return MOD_OK;
}

ModResult actions_create_rebind_session(
    ModContext* context, ActionRebindSessionHandle* outSession) {
    if (outSession != nullptr) {
        *outSession = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outSession == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::create_rebind_session(*mod, outSession));
}

ModResult actions_destroy_rebind_session(
    ModContext* context, const ActionRebindSessionHandle session) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::destroy_rebind_session(*mod, session));
}

ModResult actions_begin_rebind_session(ModContext* context, const ActionRebindSessionHandle session,
    const ActionHandle action, const ActionCaptureFilter filter) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::begin_rebind_session(
        *mod, session, action, static_cast<actions::CaptureFilter>(filter)));
}

ModResult actions_cancel_rebind_session(
    ModContext* context, const ActionRebindSessionHandle session) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return to_mod_result(actions::cancel_rebind_session(*mod, session));
}

ModResult actions_query_rebind_session(ModContext* context, const ActionRebindSessionHandle session,
    bool* outCapturing, ActionHandle* outAction) {
    if (outCapturing != nullptr) {
        *outCapturing = false;
    }
    if (outAction != nullptr) {
        *outAction = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || outCapturing == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    actions::Handle action = 0;
    const auto result = actions::query_rebind_session(*mod, session, *outCapturing, &action);
    if (result == actions::Result::Ok && outAction != nullptr) {
        *outAction = action;
    }
    return to_mod_result(result);
}

constexpr ActionService s_actionService{
    .header = SERVICE_HEADER(ActionService, ACTIONS_SERVICE_MAJOR, ACTIONS_SERVICE_MINOR),
    .register_action = actions_register_action,
    .unregister_action = actions_unregister_action,
    .get_binding = actions_get_binding,
    .get_default_binding = actions_get_default_binding,
    .set_binding = actions_set_binding,
    .reset_binding = actions_reset_binding,
    .is_down = actions_is_down,
    .was_pressed = actions_was_pressed,
    .was_released = actions_was_released,
    .is_held = actions_is_held,
    .begin_capture = actions_begin_capture,
    .cancel_capture = actions_cancel_capture,
    .is_capture_active = actions_is_capture_active,
    .format_binding_label = actions_format_binding_label,
    .create_rebind_session = actions_create_rebind_session,
    .destroy_rebind_session = actions_destroy_rebind_session,
    .begin_rebind_session = actions_begin_rebind_session,
    .cancel_rebind_session = actions_cancel_rebind_session,
    .query_rebind_session = actions_query_rebind_session,
};

}  // namespace

constinit const ServiceModule g_actionsModule{
    .id = ACTIONS_SERVICE_ID,
    .majorVersion = ACTIONS_SERVICE_MAJOR,
    .minorVersion = ACTIONS_SERVICE_MINOR,
    .service = &s_actionService,
    .initialize = actions::ensure_host_actions_registered,
    .modDetached = actions::remove_mod,
    .frameBegin = actions::evaluate_frame,
    .shutdown = actions::shutdown_host,
};

}  // namespace dusk::mods::svc
