#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#include <limits.h>

#define WINDOW_SERVICE_ID "dev.twilitrealm.dusklight.window"
#define WINDOW_SERVICE_MAJOR 1u
#define WINDOW_SERVICE_MINOR 0u

#define WINDOW_POSITION_UNDEFINED INT32_MIN

typedef uint64_t WindowHandle;

typedef enum WindowFlags {
    WINDOW_FLAG_NONE = 0u,
    WINDOW_FLAG_RESIZABLE = 1u << 0u,
    WINDOW_FLAG_HIDDEN = 1u << 1u,
    WINDOW_FLAG_BORDERLESS = 1u << 2u,
    WINDOW_FLAG_ALWAYS_ON_TOP = 1u << 3u,
    WINDOW_FLAG_TRANSPARENT = 1u << 4u,
} WindowFlags;

typedef enum WindowEventType {
    WINDOW_EVENT_CLOSE_REQUESTED = 0,
    WINDOW_EVENT_RESIZED = 1,
    WINDOW_EVENT_MOVED = 2,
    WINDOW_EVENT_FOCUS_GAINED = 3,
    WINDOW_EVENT_FOCUS_LOST = 4,
    WINDOW_EVENT_SHOWN = 5,
    WINDOW_EVENT_HIDDEN = 6,
} WindowEventType;

typedef struct WindowEvent {
    uint32_t struct_size;
    WindowEventType type;
    int32_t x;
    int32_t y;
    uint32_t width;
    uint32_t height;
    uint32_t pixel_width;
    uint32_t pixel_height;
    float display_scale;
} WindowEvent;

typedef void (*WindowEventFn)(
    ModContext* ctx, WindowHandle window, const WindowEvent* event, void* user_data);

typedef struct WindowDesc {
    uint32_t struct_size;
    const char* title;
    uint32_t width;
    uint32_t height;
    int32_t x;
    int32_t y;
    uint32_t flags;
    WindowEventFn on_event;
    void* user_data;
} WindowDesc;

#define WINDOW_DESC_INIT                                                                           \
    {sizeof(WindowDesc), NULL, 640u, 480u, WINDOW_POSITION_UNDEFINED, WINDOW_POSITION_UNDEFINED,   \
        WINDOW_FLAG_RESIZABLE | WINDOW_FLAG_HIDDEN, NULL, NULL}

typedef struct WindowInfo {
    uint32_t struct_size;
    int32_t x;
    int32_t y;
    uint32_t width;
    uint32_t height;
    uint32_t pixel_width;
    uint32_t pixel_height;
    float display_scale;
    bool visible;
    bool focused;
} WindowInfo;

#define WINDOW_INFO_INIT {sizeof(WindowInfo), 0, 0, 0u, 0u, 0u, 0u, 1.0f, false, false}

/*
 * Auxiliary native windows. All functions and callbacks run on the game thread. Window close
 * events are requests; the window remains alive until destroy_window is called. Any graphics
 * present target attached to a window must be unregistered before the window can be destroyed;
 * at most one present target may be attached to a window at a time.
 */
typedef struct WindowService {
    ServiceHeader header;

    ModResult (*create_window)(ModContext* ctx, const WindowDesc* desc, WindowHandle* out_window);
    ModResult (*destroy_window)(ModContext* ctx, WindowHandle window);
    ModResult (*show_window)(ModContext* ctx, WindowHandle window);
    ModResult (*hide_window)(ModContext* ctx, WindowHandle window);
    ModResult (*set_title)(ModContext* ctx, WindowHandle window, const char* title);
    ModResult (*set_size)(ModContext* ctx, WindowHandle window, uint32_t width, uint32_t height);
    ModResult (*get_info)(ModContext* ctx, WindowHandle window, WindowInfo* out_info);
} WindowService;

MOD_DECLARE_SERVICE(
    WindowService, svc_window, WINDOW_SERVICE_ID, WINDOW_SERVICE_MAJOR, WINDOW_SERVICE_MINOR);
