#ifndef DUSK_CONFIG_HPP
#define DUSK_CONFIG_HPP

#include <functional>
#include <stdexcept>
#include "nlohmann/json.hpp"
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

template<ConfigValue T>
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
 * \brief Indicate that all registrations have happened and everything should lock in.
 */
void FinishRegistration();

/**
 * \brief Load config from the standard user preferences location.
 */
void LoadFromUserPreferences();
void LoadFromFileName(const char* path);

/**
 * \brief Save the config to file.
 */
void Save();

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

template <ConfigValue T>
const ConfigImplBase* GetConfigImpl() {
    static ConfigImpl<T> config;
    return &config;
}

}  // namespace dusk::config

#endif
