#include "runtime.hpp"

#include <string>
#include <vector>

namespace luau_runtime {
namespace {

constexpr char kConfigVarMetatable[] = "dusklight.config_var";
constexpr char kConfigSubscriptionMetatable[] = "dusklight.config_subscription";

void config_changed(ModContext*, ConfigVarHandle, const ConfigVarValue* value,
    const ConfigVarValue* previous, void* userData) {
    auto& callback = *static_cast<Callback*>(userData);
    if (value == nullptr || previous == nullptr) {
        return;
    }
    push_config_value(callback.vm->state, *value);
    push_config_value(callback.vm->state, *previous);
    std::string error;
    if (!call_ref(*callback.vm, callback.refs[0], 2, 0, kCallbackBudget, error)) {
        fail_callback(*callback.vm, "config change callback", error);
    }
}

int config_get(lua_State* state) {
    auto& handle = check_handle(state, 1, kConfigVarMetatable, HandleKind::ConfigVar);
    if (svc_config == nullptr) {
        service_unavailable(state, "ConfigService");
    }
    switch (handle.configType) {
    case CONFIG_VAR_BOOL: {
        bool value = false;
        check_result(
            state, svc_config->get_bool(handle.vm->subject, handle.value, &value), "config get");
        lua_pushboolean(state, value);
        break;
    }
    case CONFIG_VAR_INT: {
        int64_t value = 0;
        check_result(
            state, svc_config->get_int(handle.vm->subject, handle.value, &value), "config get");
        lua_pushinteger64(state, value);
        break;
    }
    case CONFIG_VAR_FLOAT: {
        double value = 0;
        check_result(
            state, svc_config->get_float(handle.vm->subject, handle.value, &value), "config get");
        lua_pushnumber(state, value);
        break;
    }
    case CONFIG_VAR_STRING: {
        size_t size = 0;
        check_result(state,
            svc_config->get_string(handle.vm->subject, handle.value, nullptr, 0, &size),
            "config get");
        std::vector<char> value(size + 1);
        check_result(state,
            svc_config->get_string(
                handle.vm->subject, handle.value, value.data(), value.size(), nullptr),
            "config get");
        lua_pushlstring(state, value.data(), size);
        break;
    }
    default:
        luaL_error(state, "unknown config value type");
    }
    return 1;
}

int config_set(lua_State* state) {
    auto& handle = check_handle(state, 1, kConfigVarMetatable, HandleKind::ConfigVar);
    if (svc_config == nullptr) {
        service_unavailable(state, "ConfigService");
    }
    ModResult result = MOD_INVALID_ARGUMENT;
    switch (handle.configType) {
    case CONFIG_VAR_BOOL:
        result = svc_config->set_bool(
            handle.vm->subject, handle.value, luaL_checkboolean(state, 2) != 0);
        break;
    case CONFIG_VAR_INT:
        result = svc_config->set_int(handle.vm->subject, handle.value, check_int64(state, 2));
        break;
    case CONFIG_VAR_FLOAT:
        result =
            svc_config->set_float(handle.vm->subject, handle.value, luaL_checknumber(state, 2));
        break;
    case CONFIG_VAR_STRING:
        result =
            svc_config->set_string(handle.vm->subject, handle.value, luaL_checkstring(state, 2));
        break;
    default:
        break;
    }
    check_result(state, result, "config set");
    return 0;
}

int config_unregister(lua_State* state) {
    auto& handle = check_handle(state, 1, kConfigVarMetatable, HandleKind::ConfigVar);
    if (svc_config == nullptr) {
        service_unavailable(state, "ConfigService");
    }
    check_result(
        state, svc_config->unregister_var(handle.vm->subject, handle.value), "config unregister");
    handle.value = 0;
    return 0;
}

int config_unsubscribe(lua_State* state) {
    auto& handle =
        check_handle(state, 1, kConfigSubscriptionMetatable, HandleKind::ConfigSubscription);
    if (svc_config == nullptr) {
        service_unavailable(state, "ConfigService");
    }
    check_result(
        state, svc_config->unsubscribe(handle.vm->subject, handle.value), "config unsubscribe");
    handle.value = 0;
    return 0;
}

int subscribe(lua_State* state, ScriptHandle& variable, int functionIndex) {
    luaL_checktype(state, functionIndex, LUA_TFUNCTION);
    Callback& callback = retain_callback(*variable.vm);
    callback.refs[0] = lua_ref(state, functionIndex);

    ConfigSubscriptionHandle subscription = 0;
    check_result(state,
        svc_config->subscribe(
            variable.vm->subject, variable.value, config_changed, &callback, &subscription),
        "config.subscribe");
    push_handle(state, *variable.vm, subscription, HandleKind::ConfigSubscription,
        kConfigSubscriptionMetatable);
    return 1;
}

int config_subscribe(lua_State* state) {
    if (svc_config == nullptr) {
        service_unavailable(state, "ConfigService");
    }
    auto& variable = check_handle(state, 1, kConfigVarMetatable, HandleKind::ConfigVar);
    return subscribe(state, variable, 2);
}

int config_var_subscribe(lua_State* state) {
    if (svc_config == nullptr) {
        service_unavailable(state, "ConfigService");
    }
    auto& variable = check_handle(state, 1, kConfigVarMetatable, HandleKind::ConfigVar);
    return subscribe(state, variable, 2);
}

int config_register(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_config == nullptr) {
        service_unavailable(state, "ConfigService");
    }
    luaL_checktype(state, 1, LUA_TTABLE);

    const std::string name = get_optional_string(state, 1, "name");
    const std::string type = get_optional_string(state, 1, "type");
    ConfigVarDesc desc = CONFIG_VAR_DESC_INIT;
    desc.name = name.c_str();
    if (type == "bool") {
        desc.type = CONFIG_VAR_BOOL;
        desc.default_bool = get_optional_bool(state, 1, "default", false);
    } else if (type == "int") {
        desc.type = CONFIG_VAR_INT;
        desc.default_int = get_optional_int(state, 1, "default", 0);
    } else if (type == "float") {
        desc.type = CONFIG_VAR_FLOAT;
        desc.default_float = get_optional_number(state, 1, "default", 0);
    } else if (type == "string") {
        desc.type = CONFIG_VAR_STRING;
    } else {
        luaL_error(state, "config type must be 'bool', 'int', 'float', or 'string'");
    }

    std::string defaultString;
    if (desc.type == CONFIG_VAR_STRING) {
        defaultString = get_optional_string(state, 1, "default");
        desc.default_string = defaultString.c_str();
    }

    ConfigVarHandle handle = 0;
    check_result(state, svc_config->register_var(vm.subject, &desc, &handle), "config.register");
    push_handle(state, vm, handle, HandleKind::ConfigVar, kConfigVarMetatable, desc.type);
    return 1;
}

}  // namespace

void push_config_value(lua_State* state, const ConfigVarValue& value) {
    switch (value.type) {
    case CONFIG_VAR_BOOL:
        lua_pushboolean(state, value.bool_value);
        break;
    case CONFIG_VAR_INT:
        lua_pushinteger64(state, value.int_value);
        break;
    case CONFIG_VAR_FLOAT:
        lua_pushnumber(state, value.float_value);
        break;
    case CONFIG_VAR_STRING:
        lua_pushlstring(state, value.string_value != nullptr ? value.string_value : "",
            value.string_value != nullptr ? value.string_length : 0);
        break;
    default:
        lua_pushnil(state);
        break;
    }
}

int open_config(lua_State* state) {
    Vm& vm = vm_from_upvalue(state);
    if (svc_config == nullptr) {
        service_unavailable(state, "ConfigService");
    }
    static const luaL_Reg kVarMethods[] = {
        {"get", config_get},
        {"set", config_set},
        {"subscribe", config_var_subscribe},
        {"unregister", config_unregister},
        {nullptr, nullptr},
    };
    static const luaL_Reg kSubscriptionMethods[] = {
        {"unsubscribe", config_unsubscribe},
        {nullptr, nullptr},
    };
    create_handle_metatable(state, kConfigVarMetatable, kVarMethods, "ConfigVar");
    create_handle_metatable(
        state, kConfigSubscriptionMetatable, kSubscriptionMethods, "ConfigSubscription");

    lua_newtable(state);
    set_function(state, vm, "register", config_register);
    set_function(state, vm, "subscribe", config_subscribe);
    lua_setreadonly(state, -1, true);
    return 1;
}

}  // namespace luau_runtime
