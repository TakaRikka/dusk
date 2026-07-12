#include "config.hpp"

#include "registry.hpp"
#include "slot_map.hpp"

#include "aurora/lib/logging.hpp"
#include "dusk/config.hpp"
#include "dusk/mods/loader/loader.hpp"
#include "mods/svc/config.h"

#include <fmt/format.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace dusk::mods::svc {
namespace {

aurora::Module Log("dusk::mods::config");

enum class ConfigSlotKind : uint8_t {
    Var,
    Subscription,
};

struct ConfigSlot {
    ConfigSlotKind kind = ConfigSlotKind::Var;
    // Var payload
    ConfigVarType type = CONFIG_VAR_BOOL;
    std::unique_ptr<config::ConfigVarBase> var;
    // Subscription payload
    uint64_t varHandle = 0;
    config::Subscription coreSubscription = 0;
};

SlotMap<ConfigSlot> s_slots;
bool s_dirty = false;
std::chrono::steady_clock::time_point s_lastSave{};
constexpr std::chrono::seconds kSaveDebounce{2};

void config_flush_if_dirty(const bool force) {
    if (!s_dirty) {
        return;
    }
    const auto now = std::chrono::steady_clock::now();
    if (!force && now - s_lastSave < kSaveDebounce) {
        return;
    }
    s_dirty = false;
    s_lastSave = now;
    config::save();
}

// Translate the type-erased previous-value pointer into the C ABI snapshot struct. The string
// pointer aliases the previous std::string, which outlives the notification.
ConfigVarValue translate_previous(const uint32_t type, const void* previous) {
    ConfigVarValue value{};
    value.struct_size = sizeof(ConfigVarValue);
    value.type = static_cast<ConfigVarType>(type);
    switch (type) {
    case CONFIG_VAR_BOOL:
        value.bool_value = *static_cast<const bool*>(previous);
        break;
    case CONFIG_VAR_INT:
        value.int_value = *static_cast<const s64*>(previous);
        break;
    case CONFIG_VAR_FLOAT:
        value.float_value = *static_cast<const f64*>(previous);
        break;
    case CONFIG_VAR_STRING: {
        const auto* str = static_cast<const std::string*>(previous);
        value.string_value = str->c_str();
        value.string_length = str->size();
        break;
    }
    default:
        break;
    }
    return value;
}

// Snapshot the var's current (new) value for the notification. Strings are copied into
// stringStorage so the snapshot stays valid even if the callback writes the var again.
ConfigVarValue translate_current(
    const uint32_t type, config::ConfigVarBase& varBase, std::string& stringStorage) {
    ConfigVarValue value{};
    value.struct_size = sizeof(ConfigVarValue);
    value.type = static_cast<ConfigVarType>(type);
    switch (type) {
    case CONFIG_VAR_BOOL:
        value.bool_value = static_cast<ConfigVar<bool>&>(varBase).getValue();
        break;
    case CONFIG_VAR_INT:
        value.int_value = static_cast<ConfigVar<s64>&>(varBase).getValue();
        break;
    case CONFIG_VAR_FLOAT:
        value.float_value = static_cast<ConfigVar<f64>&>(varBase).getValue();
        break;
    case CONFIG_VAR_STRING:
        stringStorage = static_cast<ConfigVar<std::string>&>(varBase).getValue();
        value.string_value = stringStorage.c_str();
        value.string_length = stringStorage.size();
        break;
    default:
        break;
    }
    return value;
}

bool valid_var_fragment(const char* name) {
    if (name == nullptr) {
        return false;
    }
    const std::string_view fragment{name};
    if (fragment.empty() || fragment.size() > 64) {
        return false;
    }
    return std::ranges::all_of(fragment, [](char ch) {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') ||
               ch == '_' || ch == '-';
    });
}

config::ConfigVarBase* find_var(LoadedMod& mod, const uint64_t handle, uint32_t expectedType) {
    const auto* entry = s_slots.find_owned(handle, mod);
    if (entry == nullptr || entry->value.kind != ConfigSlotKind::Var ||
        entry->value.type != expectedType)
    {
        return nullptr;
    }
    return entry->value.var.get();
}

template <typename T>
ConfigVar<T>* find_typed_var(ModContext* context, ConfigVarHandle handle, uint32_t type) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return nullptr;
    }
    // The type tag was checked, so the downcast is safe.
    return static_cast<ConfigVar<T>*>(find_var(*mod, handle, type));
}

void config_remove_mod(LoadedMod& mod) {
    const auto entries = s_slots.take_all(mod);
    for (const auto& entry : entries) {
        if (entry.value.kind == ConfigSlotKind::Subscription) {
            config::unsubscribe(entry.value.coreSubscription);
        }
    }
    for (const auto& entry : entries) {
        if (entry.value.kind == ConfigSlotKind::Var) {
            config::unregister(*entry.value.var);
        }
    }
}

ModResult config_register_var(
    ModContext* context, const ConfigVarDesc* desc, ConfigVarHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(ConfigVarDesc) ||
        !valid_var_fragment(desc->name))
    {
        return MOD_INVALID_ARGUMENT;
    }

    const auto fullName =
        fmt::format("mod.{}.{}", escape_mod_id_for_config(mod->metadata.id), desc->name);
    if (config::GetConfigVar(fullName) != nullptr) {
        Log.error("[{}] config var '{}' conflicts with an existing config key", mod->metadata.id,
            fullName);
        return MOD_CONFLICT;
    }

    std::unique_ptr<config::ConfigVarBase> var;
    switch (desc->type) {
    case CONFIG_VAR_BOOL:
        var = std::make_unique<ConfigVar<bool>>(fullName, desc->default_bool);
        break;
    case CONFIG_VAR_INT:
        var = std::make_unique<ConfigVar<s64>>(fullName, desc->default_int);
        break;
    case CONFIG_VAR_FLOAT:
        var = std::make_unique<ConfigVar<f64>>(fullName, desc->default_float);
        break;
    case CONFIG_VAR_STRING:
        var = std::make_unique<ConfigVar<std::string>>(
            fullName, desc->default_string != nullptr ? desc->default_string : "");
        break;
    default:
        return MOD_INVALID_ARGUMENT;
    }

    // Back-fills a stashed/saved value (or a --cvar override) if one exists for this key.
    // Loads apply silently: the registering mod cannot have subscribed to this var yet, and it
    // reads the value right after registration anyway.
    config::Register(*var);

    const auto handle = s_slots.emplace(
        *mod, ConfigSlot{.kind = ConfigSlotKind::Var, .type = desc->type, .var = std::move(var)});
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return MOD_OK;
}

ModResult config_unregister_var(ModContext* context, ConfigVarHandle var) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || var == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto* entry = s_slots.find_owned(var, *mod);
    if (entry == nullptr || entry->value.kind != ConfigSlotKind::Var) {
        Log.error("[{}] config unregister failed: unknown handle {}", mod->metadata.id, var);
        return MOD_INVALID_ARGUMENT;
    }

    // Only the owning mod can (currently) subscribe to a var
    std::vector<uint64_t> bound;
    s_slots.for_each([&](const uint64_t handle, const auto& e) {
        if (e.value.kind == ConfigSlotKind::Subscription && e.value.varHandle == var) {
            bound.push_back(handle);
        }
    });
    for (const auto handle : bound) {
        config::unsubscribe(s_slots.take(handle)->value.coreSubscription);
    }

    // The persisted value is stashed and restored by a future registration of the same name.
    config::unregister(*s_slots.take(var)->value.var);
    return MOD_OK;
}

ModResult config_get_bool(ModContext* context, ConfigVarHandle var, bool* outValue) {
    if (outValue != nullptr) {
        *outValue = false;
    }
    auto* cvar = find_typed_var<bool>(context, var, CONFIG_VAR_BOOL);
    if (cvar == nullptr || outValue == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    *outValue = cvar->getValue();
    return MOD_OK;
}

ModResult config_set_bool(ModContext* context, ConfigVarHandle var, bool value) {
    auto* cvar = find_typed_var<bool>(context, var, CONFIG_VAR_BOOL);
    if (cvar == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    cvar->setValue(value);
    config_mark_dirty();
    return MOD_OK;
}

ModResult config_get_int(ModContext* context, ConfigVarHandle var, int64_t* outValue) {
    if (outValue != nullptr) {
        *outValue = 0;
    }
    auto* cvar = find_typed_var<s64>(context, var, CONFIG_VAR_INT);
    if (cvar == nullptr || outValue == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    *outValue = cvar->getValue();
    return MOD_OK;
}

ModResult config_set_int(ModContext* context, ConfigVarHandle var, int64_t value) {
    auto* cvar = find_typed_var<s64>(context, var, CONFIG_VAR_INT);
    if (cvar == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    cvar->setValue(value);
    config_mark_dirty();
    return MOD_OK;
}

ModResult config_get_float(ModContext* context, ConfigVarHandle var, double* outValue) {
    if (outValue != nullptr) {
        *outValue = 0.0;
    }
    auto* cvar = find_typed_var<f64>(context, var, CONFIG_VAR_FLOAT);
    if (cvar == nullptr || outValue == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    *outValue = cvar->getValue();
    return MOD_OK;
}

ModResult config_set_float(ModContext* context, ConfigVarHandle var, double value) {
    auto* cvar = find_typed_var<f64>(context, var, CONFIG_VAR_FLOAT);
    if (cvar == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    cvar->setValue(value);
    config_mark_dirty();
    return MOD_OK;
}

ModResult config_get_string(
    ModContext* context, ConfigVarHandle var, char* buffer, size_t bufferSize, size_t* outLength) {
    if (outLength != nullptr) {
        *outLength = 0;
    }
    auto* cvar = find_typed_var<std::string>(context, var, CONFIG_VAR_STRING);
    if (cvar == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto& value = cvar->getValue();
    if (outLength != nullptr) {
        *outLength = value.size();
    }
    if (buffer == nullptr) {
        // Length query; any other use of a null buffer is a caller bug.
        return bufferSize == 0 ? MOD_OK : MOD_INVALID_ARGUMENT;
    }
    if (bufferSize < value.size() + 1) {
        return MOD_INVALID_ARGUMENT;
    }
    std::memcpy(buffer, value.c_str(), value.size() + 1);
    return MOD_OK;
}

ModResult config_set_string(ModContext* context, ConfigVarHandle var, const char* value) {
    auto* cvar = find_typed_var<std::string>(context, var, CONFIG_VAR_STRING);
    if (cvar == nullptr || value == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    cvar->setValue(std::string{value});
    config_mark_dirty();
    return MOD_OK;
}

ModResult config_subscribe(ModContext* context, ConfigVarHandle var, ConfigChangedFn callback,
    void* userData, ConfigSubscriptionHandle* outHandle) {
    if (outHandle != nullptr) {
        *outHandle = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || var == 0 || callback == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto* entry = s_slots.find_owned(var, *mod);
    if (entry == nullptr || entry->value.kind != ConfigSlotKind::Var) {
        return MOD_INVALID_ARGUMENT;
    }

    const auto coreSubscription = config::subscribe(entry->value.var->getName(),
        [modPtr = mod, callback, userData, varHandle = var, type = entry->value.type](
            config::ConfigVarBase& varBase, const void* previous) {
            const ConfigVarValue previousValue = translate_previous(type, previous);
            std::string stringStorage;
            const ConfigVarValue currentValue = translate_current(type, varBase, stringStorage);
            try {
                callback(modPtr->context.get(), varHandle, &currentValue, &previousValue, userData);
            } catch (const std::exception& e) {
                fail_mod(*modPtr, MOD_ERROR,
                    fmt::format("Exception in config change callback: {}", e.what()));
            } catch (...) {
                fail_mod(*modPtr, MOD_ERROR, "Unknown exception in config change callback");
            }
        });
    const auto handle = s_slots.emplace(*mod, ConfigSlot{
                                                  .kind = ConfigSlotKind::Subscription,
                                                  .varHandle = var,
                                                  .coreSubscription = coreSubscription,
                                              });
    if (outHandle != nullptr) {
        *outHandle = handle;
    }
    return MOD_OK;
}

ModResult config_unsubscribe(ModContext* context, ConfigSubscriptionHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr || handle == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    const auto* entry = s_slots.find_owned(handle, *mod);
    if (entry == nullptr || entry->value.kind != ConfigSlotKind::Subscription) {
        Log.error("[{}] config unsubscribe failed: unknown handle {}", mod->metadata.id, handle);
        return MOD_INVALID_ARGUMENT;
    }
    config::unsubscribe(entry->value.coreSubscription);
    s_slots.erase(handle);
    return MOD_OK;
}

constexpr ConfigService s_configService{
    .header = SERVICE_HEADER(ConfigService, CONFIG_SERVICE_MAJOR, CONFIG_SERVICE_MINOR),
    .register_var = config_register_var,
    .unregister_var = config_unregister_var,
    .get_bool = config_get_bool,
    .set_bool = config_set_bool,
    .get_int = config_get_int,
    .set_int = config_set_int,
    .get_float = config_get_float,
    .set_float = config_set_float,
    .get_string = config_get_string,
    .set_string = config_set_string,
    .subscribe = config_subscribe,
    .unsubscribe = config_unsubscribe,
};

}  // namespace

void config_mark_dirty() {
    s_dirty = true;
}

config::ConfigVarBase* config_find_var(
    LoadedMod& mod, const ConfigVarHandle handle, const uint32_t expectedType) {
    return find_var(mod, handle, expectedType);
}

constinit const ServiceModule g_configModule{
    .id = CONFIG_SERVICE_ID,
    .majorVersion = CONFIG_SERVICE_MAJOR,
    .minorVersion = CONFIG_SERVICE_MINOR,
    .service = &s_configService,
    .modDetached = config_remove_mod,
    .frameEnd = [] { config_flush_if_dirty(false); },
    .shutdown = [] { config_flush_if_dirty(true); },
};

}  // namespace dusk::mods::svc
