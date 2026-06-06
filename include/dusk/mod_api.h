#pragma once

#include <cstddef>
#include <cstdint>

#if defined(_WIN32)
#define DUSK_MOD_EXPORT __declspec(dllexport)
#else
#define DUSK_MOD_EXPORT __attribute__((visibility("default")))
#endif

#define DUSK_MOD_API_VERSION 1

typedef void* DuskPanelHandle;
typedef void* DuskElemHandle;

// A user-configurable setting a mod declares once (in mod_init) via
// define_settings. The host persists values per mod and renders a control for
// each in the game's Mods tab.
typedef enum DuskSettingType {
    DUSK_SETTING_BOOL = 0,
    DUSK_SETTING_INT = 1,
    DUSK_SETTING_FLOAT = 2,
} DuskSettingType;

typedef struct DuskSetting {
    const char* key;   // stable id, unique within the mod
    const char* label; // display name
    const char* help;  // short description (may be NULL)
    DuskSettingType type;
    double default_value;
    double min_value; // INT/FLOAT
    double max_value; // INT/FLOAT
    double step;      // INT/FLOAT UI increment (0 -> sensible default)
} DuskSetting;

// Place this once at file scope in your mod to declare the minimum API version required.
// The loader will refuse to initialize the mod if the engine's API version is older.
#define DUSK_REQUIRE_API_VERSION                                                                   \
    extern "C" DUSK_MOD_EXPORT uint32_t mod_api_version = DUSK_MOD_API_VERSION;

struct DuskModAPIv1 {
    uint32_t api_version;
    const char* mod_dir;

    void (*log_info)(const char* fmt, ...);
    void (*log_warn)(const char* fmt, ...);
    void (*log_error)(const char* fmt, ...);

    void* (*load_resource)(const char* relative_path, size_t* out_size);
    void (*free_resource)(void* data);

    void (*register_tab_content)(
        void (*build_fn)(DuskPanelHandle panel, void* userdata), void* userdata);
    void (*register_tab_update)(void (*update_fn)(void* userdata), void* userdata);

    void (*panel_add_section)(DuskPanelHandle panel, const char* text);
    void (*panel_add_button)(
        DuskPanelHandle panel, const char* label, void (*cb)(void* userdata), void* userdata);
    DuskElemHandle (*panel_add_badge_row)(DuskPanelHandle panel, const char* label, int ok);
    DuskElemHandle (*panel_add_dyn_text)(DuskPanelHandle panel, const char* text);
    DuskElemHandle (*panel_add_progress)(DuskPanelHandle panel, float value);

    void (*elem_set_badge)(DuskElemHandle elem, int ok);
    void (*elem_set_text)(DuskElemHandle elem, const char* text);
    void (*elem_set_progress)(DuskElemHandle elem, float value);

    void (*hook_install)(void* fn_addr, void* tramp_fn, void** orig_store);
    void (*hook_pre)(void* fn_addr, int32_t (*fn)(void* args));
    void (*hook_post)(void* fn_addr, void (*fn)(void* args, void* retval));
    void (*hook_replace)(void* fn_addr, void (*fn)(void* args, void* retval));

    bool (*hook_dispatch_pre)(void* fn_addr, void* args, void* retval);
    void (*hook_dispatch_post)(void* fn_addr, void* args, void* retval);

    void (*service_publish)(const char* name, void* ptr);
    void* (*service_get)(const char* name);

    // Settings. Declare the calling mod's settings once from mod_init; the host
    // stores them, overlays any saved values, and shows a control per setting in
    // the Mods tab. setting_get/_set read/write the calling mod's live value by
    // key. These resolve the "current mod" the same way the UI/log callbacks do,
    // so call them from mod_init / mod_tick (cache values for use in hooks).
    void (*define_settings)(const DuskSetting* settings, uint32_t count);
    double (*setting_get)(const char* key);
    void (*setting_set)(const char* key, double value);
};

using DuskModAPI = DuskModAPIv1;

extern "C" {
void mod_init(DuskModAPI* api);
void mod_tick(DuskModAPI* api);
}
