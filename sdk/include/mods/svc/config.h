#pragma once

#include <mods/api.h>

#define CONFIG_SERVICE_ID "dev.twilitrealm.dusklight.config"
#define CONFIG_SERVICE_MAJOR 1u
#define CONFIG_SERVICE_MINOR 0u

/* Handle for a config var registered by the calling mod. 0 is never a valid handle. */
typedef uint64_t ConfigVarHandle;
/* Handle for a change subscription. 0 is never a valid handle. */
typedef uint64_t ConfigSubscriptionHandle;

typedef enum ConfigVarType {
    CONFIG_VAR_BOOL = 0,   /* bool */
    CONFIG_VAR_INT = 1,    /* int64_t */
    CONFIG_VAR_FLOAT = 2,  /* double */
    CONFIG_VAR_STRING = 3, /* UTF-8 */
} ConfigVarType;

typedef struct ConfigVarDesc {
    uint32_t struct_size;
    /* Name fragment: 1-64 characters from [A-Za-z0-9_-]. The full config key is
     * "mod.<escaped mod id>.<name>", persisted in config.json alongside host settings.
     * "enabled" is reserved by the loader. */
    const char* name;
    ConfigVarType type;
    /* Default value; only the field matching `type` is read. */
    bool default_bool;
    int64_t default_int;
    double default_float;
    const char* default_string; /* NULL means "" */
} ConfigVarDesc;

#define CONFIG_VAR_DESC_INIT {sizeof(ConfigVarDesc), NULL, CONFIG_VAR_BOOL, false, 0, 0.0, NULL}

/* Snapshot of a var's value; only the field matching `type` is meaningful. */
typedef struct ConfigVarValue {
    uint32_t struct_size;
    ConfigVarType type;
    bool bool_value;
    int64_t int_value;
    double float_value;
    const char* string_value; /* NUL-terminated; NULL for non-string vars */
    size_t string_length;     /* excludes the NUL */
} ConfigVarValue;

/*
 * Fired on the game thread whenever the var's effective value changes at runtime: the calling mod's
 * own set_* calls and any other runtime writer. Writes that leave the value unchanged do not fire,
 * and neither do values applied from config.json or --cvar during registration. `value` holds the
 * new (current) value and `previous` the one it replaced; both snapshots are valid only for the
 * duration of the call (copy string_value if you need to keep it). Setting the same var from inside
 * its own callback applies the write but is not re-notified.
 */
typedef void (*ConfigChangedFn)(ModContext* ctx, ConfigVarHandle var, const ConfigVarValue* value,
    const ConfigVarValue* previous, void* user_data);

/*
 * Scoped configuration variables.
 *
 * Registrations are owned by the calling mod and removed automatically (subscriptions included)
 * when it is disabled, reloaded, or fails. Values are saved to config.json. Writes are debounced,
 * not flushed per set.
 */
typedef struct ConfigService {
    ServiceHeader header;

    /* Register a config var. If a value for the full key was saved earlier (or set via --cvar),
     * it takes effect immediately; otherwise the var starts at the default. Registering a name
     * that is already live is MOD_CONFLICT. */
    ModResult (*register_var)(
        ModContext* ctx, const ConfigVarDesc* desc, ConfigVarHandle* out_handle);
    /* Unregister a var previously registered by the calling mod. Its persisted value is kept. */
    ModResult (*unregister_var)(ModContext* ctx, ConfigVarHandle var);

    /* Typed accessors; the type must match the registration (MOD_INVALID_ARGUMENT otherwise). */
    ModResult (*get_bool)(ModContext* ctx, ConfigVarHandle var, bool* out_value);
    ModResult (*set_bool)(ModContext* ctx, ConfigVarHandle var, bool value);
    ModResult (*get_int)(ModContext* ctx, ConfigVarHandle var, int64_t* out_value);
    ModResult (*set_int)(ModContext* ctx, ConfigVarHandle var, int64_t value);
    ModResult (*get_float)(ModContext* ctx, ConfigVarHandle var, double* out_value);
    ModResult (*set_float)(ModContext* ctx, ConfigVarHandle var, double value);
    /* Copies the NUL-terminated value into buffer. out_length (optional) receives the full
     * length excluding the NUL regardless of buffer size; call with buffer == NULL and
     * buffer_size == 0 to query the length. A non-NULL buffer that is too small fails with
     * MOD_INVALID_ARGUMENT and writes nothing. */
    ModResult (*get_string)(
        ModContext* ctx, ConfigVarHandle var, char* buffer, size_t buffer_size, size_t* out_length);
    ModResult (*set_string)(ModContext* ctx, ConfigVarHandle var, const char* value);

    /* Subscribe to changes of a var registered by the calling mod. out_handle may be NULL if
     * the subscription is never removed manually (cleanup on mod teardown is automatic). */
    ModResult (*subscribe)(ModContext* ctx, ConfigVarHandle var, ConfigChangedFn callback,
        void* user_data, ConfigSubscriptionHandle* out_handle);
    ModResult (*unsubscribe)(ModContext* ctx, ConfigSubscriptionHandle handle);
} ConfigService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct dusk::mods::ServiceTraits<ConfigService> {
    static constexpr const char* id = CONFIG_SERVICE_ID;
    static constexpr uint16_t major_version = CONFIG_SERVICE_MAJOR;
    static constexpr uint16_t minor_version = CONFIG_SERVICE_MINOR;
};
#endif
