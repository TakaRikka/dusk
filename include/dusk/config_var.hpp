#ifndef DUSK_CONFIG_VAR_HPP
#define DUSK_CONFIG_VAR_HPP

#include "dolphin/types.h"
#include <type_traits>
#include <cstdlib>
#include <string>

/**
 * The configuration system.
 *
 * Configuration works via "configuration variables" aka "CVars". Each stores a single value that
 * may be individually written to/from a configuration file.
 *
 * CVars, like ogres, have layers. Higher layers (e.g. a set value) override lower layers
 * (e.g. the default value).
 *
 * To define a CVar, simply make a global variable of type ConfigVar<T>,
 * and make sure Register() is called on it during program startup.
 *
 * config_var.hpp contains the simplest "just the configuration vars themselves".
 * This should be safe to include for files that need to *access* configuration,
 * without blowing up compile times on implementation details.
 *
 * config.hpp on the other hand contains far more calls for mutating, loading, and defining CVars.
 */
namespace dusk::config {

/**
 * \brief Layers that a configuration variable can currently be at.
 *
 * A configuration variable can be on one of multiple *layers*, which determines where
 * the current value is coming from.
 */
enum class ConfigVarLayer : u8 {
    /**
     * The CVar is at the default value defined by the application code.
     */
    Default,

    /**
     * The CVar has been modified by the user and may be saved to config.
     */
    Value,

    /**
     * The CVar is modified by launch argument, overruling the normal config value.
     * Will not get saved to config.
     */
    Override,
};

class ConfigImplBase;

/**
 * Base class that all CVars inherit from.
 * You want the templated ConfigVar instead for actual usage.
 */
class ConfigVarBase {
protected:
    /**
     * The name of this CVar, used in the configuration file.
     */
    const char* name;

    /**
     * Whether this CVar has been registered with the global managing logic.
     * If this is not done, it is not functional.
     */
    bool registered;

    /**
     * The layer this CVar is at.
     */
    ConfigVarLayer layer;

    /**
     * Pointer to an implementation struct for various load/save calls.
     */
    const ConfigImplBase* impl;

    ConfigVarBase(const char* name, const ConfigImplBase* impl);
    virtual ~ConfigVarBase() = default;

    /**
     * Check that the CVar is registered, aborting if this is not the case.
     */
    void checkRegistered() const {
        if (!registered)
            abort();
    }

public:
    /**
     * Get the name of this CVar, used in the configuration file.
     */
    [[nodiscard]] const char* getName() const noexcept;

    /**
     * Get the pointer to the implementation struct.
     */
    [[nodiscard]] const ConfigImplBase* getImpl() const noexcept;

    /**
     * Get the layer this CVar is currently at.
     */
    [[nodiscard]] constexpr ConfigVarLayer getLayer() const noexcept {
        return layer;
    }

    /**
     * Mark this CVar as being registered with the central save/load logic.
     * This is necessary to make it legal to access.
     */
    void markRegistered();
};

template <typename T>
concept ConfigValueInteger =
    std::is_same_v<T, s8>
    || std::is_same_v<T, u8>
    || std::is_same_v<T, s16>
    || std::is_same_v<T, u16>
    || std::is_same_v<T, s32>
    || std::is_same_v<T, u32>
    || std::is_same_v<T, s64>
    || std::is_same_v<T, u64>;

/**
 * \brief Concept that defines the legal set of types that can be used for CVar values.
 *
 * Valid types cannot be cv-qualified and must be basic primitive types (int, float, bool),
 * strings, or enums of the basic primitives.
 */
template <typename T>
concept ConfigValue =
    !std::is_const_v<T>
    && !std::is_volatile_v<T>
    && (std::is_same_v<T, bool>
        || ConfigValueInteger<T>
        || std::is_same_v<T, f32>
        || std::is_same_v<T, f64>
        || std::is_same_v<T, std::string>
        || (std::is_enum_v<T> && ConfigValueInteger<std::underlying_type_t<T>>));

template <ConfigValue T>
const ConfigImplBase* GetConfigImpl();

/**
 * \brief A CVar storing values.
 *
 * @tparam T The type of value stored in the CVar.
 */
template <ConfigValue T>
class ConfigVar : public ConfigVarBase {
    T defaultValue;
    T value;
    T overrideValue;

public:
    /**
     * \brief Construct a CVar.
     *
     * @param name The name of this CVar. Must be unique.
     * @param arg Arguments to forward to construct the default value.
     */
    template <typename... Args>
    explicit ConfigVar(const char* name, Args&&... arg)
        : ConfigVarBase(name, GetConfigImpl<T>()), defaultValue(std::forward<Args>(arg)...),
        value(), overrideValue() {}

    /**
     * \brief Get the current value of the CVar.
     *
     * This reference is not guaranteed to remain up-to-date after modification of the CVar.
     * It will, however, remain sound to access.
     */
    [[nodiscard]] constexpr const T& getValue() const noexcept {
        checkRegistered();
        switch (layer) {
        case ConfigVarLayer::Default:
            return defaultValue;
        case ConfigVarLayer::Value:
            return value;
        case ConfigVarLayer::Override:
            return overrideValue;
        default:
            abort();
        }
    }

    /**
     * \brief Change the value of a CVar.
     *
     * The new value is always stored at the Value layer.
     *
     * @param newValue The new value the CVar will get.
     * @param replaceOverride If true, clear an existing override layer if there is one.
     *     If this is false and there is an override layer,
     *     the result of getValue() will not change immediately.
     */
    void setValue(T newValue, bool replaceOverride = true) {
        checkRegistered();
        value = std::move(newValue);

        if (replaceOverride) {
            overrideValue = {};
            layer = ConfigVarLayer::Value;
        } else if (layer != ConfigVarLayer::Override) {
            layer = ConfigVarLayer::Value;
        }
    }

    operator const T&() {
        return getValue();
    }

    /**
     * \brief Give a CVar an override value.
     *
     * This overrides (but does not replace) the apparent set value of this CVar.
     * The overriden value will not get saved to config.
     *
     * @param newValue The new value the CVar will get.
     */
    void setOverrideValue(T newValue) {
        checkRegistered();
        overrideValue = std::move(newValue);
        layer = ConfigVarLayer::Override;
    }
};

}

#endif  // DUSK_CONFIG_VAR_HPP
