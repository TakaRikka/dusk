#pragma once

#include <concepts>
#include <functional>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "config_var.hpp"

namespace dusk::config {

/*
 * config.hpp is a heavier "full" header for the configuration system.
 * For a basic overview and the basic types (sufficient for accessing settings),
 * look at config_var.hpp.
 *
 * Avoid including this header in the entire game, it's heavier than I'd like!
 */

/**
 * \brief Base class containing virtual functions used for save/load of CVars.
 */
class ConfigImplBase {
protected:
    virtual ~ConfigImplBase() = default;

public:
    /**
     * \brief Load a JSON value into a CVar at the Value layer.
     */
    virtual void loadFromJson(ConfigVarBase& cVar, const nlohmann::json& jsonValue) const = 0;

    /**
     * \brief Load a simple launch argument into the CVar at the Override layer.
     */
    virtual void loadFromArg(ConfigVarBase& cVar, std::string_view stringValue) const = 0;

    /**
     * \brief Dump the value contained in the CVar to JSON.
     */
    [[nodiscard]] virtual nlohmann::json dumpToJson(const ConfigVarBase& cVar) const = 0;
};

template <ConfigValue T>
class ConfigImpl : public ConfigImplBase {
    // Just downcasting the references...
    void loadFromJson(ConfigVarBase& cVar, const nlohmann::json& jsonValue) const final {
        assert(typeid(cVar) == typeid(ConfigVar<T>));
        loadFromJson(dynamic_cast<ConfigVar<T>&>(cVar), jsonValue);
    }

    void loadFromArg(ConfigVarBase& cVar, std::string_view stringValue) const final {
        assert(typeid(cVar) == typeid(ConfigVar<T>));
        loadFromArg(dynamic_cast<ConfigVar<T>&>(cVar), stringValue);
    }

    [[nodiscard]] nlohmann::json dumpToJson(const ConfigVarBase& cVar) const final {
        assert(typeid(cVar) == typeid(ConfigVar<T>));
        return dumpToJson(dynamic_cast<const ConfigVar<T>&>(cVar));
    }

    /**
     * \brief Load a JSON value into a CVar at the Value layer.
     */
    static void loadFromJson(ConfigVar<T>& cVar, const nlohmann::json& jsonValue);

    /**
     * \brief Load a simple launch argument into the CVar at the Override layer.
     */
    static void loadFromArg(ConfigVar<T>& cVar, std::string_view stringValue);

    /**
     * \brief Dump the value contained in the CVar to JSON.
     */
    [[nodiscard]] static nlohmann::json dumpToJson(const ConfigVar<T>& cVar);
};

/**
 * \brief Thrown by config loading functions if the value provided is invalid for the CVar.
 */
class InvalidConfigError : public std::runtime_error {
public:
    explicit InvalidConfigError(const char* what) : runtime_error(what) {}
};

/**
 * \brief Register a CVar to make the config system aware of it.
 *
 * This must be done on startup *before* config has been loaded.
 */
void Register(ConfigVarBase& configVar);

/**
 * \brief Unregister a CVar, detaching it from the config system.
 *
 * If the CVar carries a user-set value (Value or Speedrun layer), it is stashed as an
 * unregistered key: Save() keeps writing it, and a later Register() of the same name restores
 * it through the normal back-fill path. The CVar may be destroyed after this returns.
 */
void unregister(ConfigVarBase& configVar);

/**
 * \brief Load config from the standard user preferences location.
 */
void load_from_user_preferences();
void load_from_file_name(const char* path);

void load_arg_override(std::string_view name, std::string_view value);

void shutdown();

/**
 * \brief Save the config to file.
 */
void save();

/**
 * \brief Get a registered CVar by name.
 *
 * @return null if the CVar does not exist.
 */
ConfigVarBase* GetConfigVar(std::string_view name);

/**
 * \brief Resets all custom action bindings for a specific port to nothing
 *
 * @param port The port to be cleared of action bindings
 */
void ClearAllActionBindings(int port);

/**
 * \brief Call a function on every registered CVar.
 */
void EnumerateRegistered(std::function<void(ConfigVarBase&)> callback);

/**
 * \brief Type-erased change callback. previousValue points at the value before the mutation
 * (a `const T*` for a `ConfigVar<T>`) and is valid only for the duration of the call.
 */
using ChangeCallback = std::function<void(ConfigVarBase& cVar, const void* previousValue)>;

/**
 * \brief Token identifying a change subscription. 0 is never a valid token.
 */
using Subscription = u64;

/**
 * \brief Subscribe to changes of the named CVar (registered or not yet registered).
 *
 * Fired synchronously on the mutating thread (in practice the game thread) whenever the CVar's
 * effective value changes at runtime: setValue, override/speedrun setters and clears. Values
 * applied by config load or launch arguments do *not* notify: loads happen during startup
 * before the subsystems callbacks push values into are initialized, and each subsystem reads
 * its initial value itself at its own init. Callbacks may mutate other CVars; a nested
 * mutation of the same CVar applies but does not re-notify.
 */
Subscription subscribe(std::string_view name, ChangeCallback callback);

/**
 * \brief Typed convenience overload: the callback receives the current and previous values.
 */
template <ConfigValue T, typename Callback>
requires std::invocable<Callback, const T&, const T&> Subscription subscribe(
    ConfigVar<T>& cVar, Callback&& callback) {
    return subscribe(cVar.getName(),
        [&cVar, cb = std::forward<Callback>(callback)](ConfigVarBase&, const void* previousValue) {
            cb(cVar.getValue(), *static_cast<const T*>(previousValue));
        });
}

void unsubscribe(Subscription token);

/**
 * \brief Register a CVar and attach a change callback in one step.
*
 * Useful for pushing settings into external systems (e.g. aurora) from one place instead of
 * every UI setter. The callback fires only for runtime changes (see subscribe); not when
 * loaded from config or launch arguments.
 */
template <ConfigValue T, typename Callback>
requires std::invocable<Callback, const T&, const T&> Subscription Register(
    ConfigVar<T>& cVar, Callback&& onChange) {
    auto subscription = subscribe(cVar, std::forward<Callback>(onChange));
    Register(static_cast<ConfigVarBase&>(cVar));
    return subscription;
}

template <ConfigValue T>
const ConfigImplBase* GetConfigImpl() {
    static ConfigImpl<T> config;
    return &config;
}

}  // namespace dusk::config
