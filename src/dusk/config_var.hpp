#pragma once

#include "dolphin/types.h"
#include <concepts>
#include <cstdlib>
#include <limits>
#include <optional>
#include <string>
#include <type_traits>

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

    /**
     * The CVar is temporarily overridden by speedrun mode.
     * Will not get saved to config. Cleared when speedrun mode is disabled.
     * Lower priority than Override, so launch args still win.
     */
    Speedrun,
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
    std::string name;

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

    ConfigVarBase(const ConfigVarBase&) = delete;
    ConfigVarBase(std::string name, const ConfigImplBase* impl);

    /**
     * Check that the CVar is registered, aborting if this is not the case.
     */
    void checkRegistered() const {
        if (!registered)
            abort();
    }

    /**
     * Whether any change subscriber (see config::subscribe) is attached to this CVar's name.
     */
    [[nodiscard]] bool has_subscribers() const;

    /**
     * Notify change subscribers (see config::subscribe) that the effective value of this CVar
     * changed. Called by mutators after the change has been applied; previousValue points at
     * the old value (a `const T*` for a `ConfigVar<T>`), valid only for the duration of the
     * call.
     */
    void notify_changed(const void* previousValue);

public:
    virtual ~ConfigVarBase();

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
    void unmarkRegistered();

    /**
     * Clear a speedrun-mode override if one is active on this CVar.
     * Safe to call on any CVar, no-op if not at the Speedrun layer.
     */
    virtual void clearSpeedrunOverride() {}
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

template <typename T>
struct ConfigValueTraits {
    static constexpr bool enabled = false;
};

/**
 * \brief Concept that defines the legal set of types that can be used for CVar values.
 *
 * Valid types cannot be cv-qualified and must be basic primitive types (int, float, bool),
 * strings, enums of the basic primitives, or explicitly-enabled structured settings.
 */
template <typename T>
concept ConfigValue =
    !std::is_const_v<T>
    && !std::is_volatile_v<T>
    && std::equality_comparable<T>
    && (std::is_same_v<T, bool>
        || ConfigValueInteger<T>
        || std::is_same_v<T, f32>
        || std::is_same_v<T, f64>
        || std::is_same_v<T, std::string>
        || (std::is_enum_v<T> && ConfigValueInteger<std::underlying_type_t<T>>)
        || ConfigValueTraits<T>::enabled);

template <ConfigValue T>
const ConfigImplBase* GetConfigImpl();

template <ConfigValue T>
class ConfigImpl;

template <typename T>
struct ConfigEnumRange {
    static constexpr auto min = std::numeric_limits<std::underlying_type_t<T>>::min();
    static constexpr auto max = std::numeric_limits<std::underlying_type_t<T>>::max();
};

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
    ConfigVarLayer priorLayer = ConfigVarLayer::Default;

public:
    /**
     * \brief Construct a CVar.
     *
     * @param name The name of this CVar. Must be unique.
     * @param arg Arguments to forward to construct the default value.
     */
    template <typename... Args>
    ConfigVar(std::string name, Args&&... arg)
        : ConfigVarBase(std::move(name), GetConfigImpl<T>()), defaultValue(std::forward<Args>(arg)...),
        value(), overrideValue() {}

    ConfigVar(ConfigVar const&) = delete;

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
        case ConfigVarLayer::Speedrun:
            return overrideValue;
        default:
            abort();
        }
    }

    [[nodiscard]] constexpr const T& getDefaultValue() const noexcept {
        checkRegistered();
        return defaultValue;
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
        const auto previous = previous_for_notify();
        value = std::move(newValue);

        if (replaceOverride) {
            overrideValue = {};
            layer = ConfigVarLayer::Value;
        } else if (layer != ConfigVarLayer::Override) {
            layer = ConfigVarLayer::Value;
        }
        notify_if_changed(previous);
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
        const auto previous = previous_for_notify();
        overrideValue = std::move(newValue);
        layer = ConfigVarLayer::Override;
        notify_if_changed(previous);
    }

    /**
     * \brief Give a CVar a speedrun-mode override value.
     *
     * Lower priority than a launch-arg override. Cleared when speedrun mode is disabled.
     * The overridden value will not get saved to config.
     *
     * @param newValue The new value the CVar will get.
     */
    void setSpeedrunValue(T newValue) {
        checkRegistered();
        if (layer != ConfigVarLayer::Override) {
            const auto previous = previous_for_notify();
            priorLayer = layer;
            overrideValue = std::move(newValue);
            layer = ConfigVarLayer::Speedrun;
            notify_if_changed(previous);
        }
    }

    void clearOverride() {
        checkRegistered();
        if (layer == ConfigVarLayer::Override) {
            const auto previous = previous_for_notify();
            overrideValue = {};
            layer = ConfigVarLayer::Value;
            notify_if_changed(previous);
        }
    }

    void clearSpeedrunOverride() override {
        checkRegistered();
        if (layer == ConfigVarLayer::Speedrun) {
            const auto previous = previous_for_notify();
            overrideValue = {};
            layer = priorLayer;
            notify_if_changed(previous);
        }
    }

    /**
     * \brief Get the user-persisted value, ignoring any temporary overrides.
     *
     * Used by Save() to write the correct value even when a speedrun override is active.
     */
    [[nodiscard]] constexpr const T& getValueForSave() const noexcept {
        checkRegistered();
        const ConfigVarLayer effectiveLayer = (layer == ConfigVarLayer::Speedrun) ? priorLayer : layer;
        return effectiveLayer == ConfigVarLayer::Default ? defaultValue : value;
    }

private:
    // The config loader applies values through the silent load_* methods below.
    friend class ConfigImpl<T>;

    /**
     * Copy of the effective value before a mutation, taken only when someone is subscribed.
     */
    [[nodiscard]] std::optional<T> previous_for_notify() const {
        return has_subscribers() ? std::optional<T>{getValue()} : std::nullopt;
    }

    /**
     * Notify subscribers if the effective value actually changed across a mutation.
     */
    void notify_if_changed(const std::optional<T>& previous) {
        if (previous.has_value() && !(getValue() == *previous)) {
            notify_changed(&*previous);
        }
    }

    /**
     * setValue(newValue, false) without notifying change subscribers. Used when loading config:
     * loads happen during startup before the subsystems change callbacks push values into are
     * initialized, and each subsystem applies the loaded value itself at its own init.
     */
    void load_value(T newValue) {
        checkRegistered();
        value = std::move(newValue);
        if (layer != ConfigVarLayer::Override) {
            layer = ConfigVarLayer::Value;
        }
    }

    /**
     * setOverrideValue without notifying change subscribers (see load_value).
     */
    void load_override_value(T newValue) {
        checkRegistered();
        overrideValue = std::move(newValue);
        layer = ConfigVarLayer::Override;
    }
};

using ActionBindConfigVar = ConfigVar<int>;

}
