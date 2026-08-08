#include "window.hpp"

#include "registry.hpp"
#include "slot_map.hpp"

#include <borealis/log.hpp>
#include "dusk/mods/loader/loader.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_video.h>
#include <fmt/format.h>

#include <cstdint>
#include <string>
#include <unordered_map>

namespace dusk::mods::svc {
namespace {

constexpr borealis::Log Log{"dusk::mods::window"};

struct WindowSlot {
    SDL_Window* window = nullptr;
    SDL_WindowID windowId = 0;
    WindowEventFn onEvent = nullptr;
    void* userData = nullptr;
    uint32_t graphicsRefs = 0;
};

SlotMap<WindowSlot> s_windows;
std::unordered_map<SDL_WindowID, WindowHandle> s_windowsById;

WindowSlot* resolve_window(LoadedMod& mod, WindowHandle handle) {
    auto* entry = s_windows.find_owned(handle, mod);
    return entry != nullptr ? &entry->value : nullptr;
}

bool populate_info(SDL_Window* window, WindowInfo& info) {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    int pixelWidth = 0;
    int pixelHeight = 0;
    if (!SDL_GetWindowPosition(window, &x, &y) || !SDL_GetWindowSize(window, &width, &height) ||
        !SDL_GetWindowSizeInPixels(window, &pixelWidth, &pixelHeight))
    {
        return false;
    }
    const auto flags = SDL_GetWindowFlags(window);
    info.x = x;
    info.y = y;
    info.width = static_cast<uint32_t>(width);
    info.height = static_cast<uint32_t>(height);
    info.pixel_width = static_cast<uint32_t>(pixelWidth);
    info.pixel_height = static_cast<uint32_t>(pixelHeight);
    info.display_scale = SDL_GetWindowDisplayScale(window);
    info.visible = (flags & SDL_WINDOW_HIDDEN) == 0u;
    info.focused = (flags & SDL_WINDOW_INPUT_FOCUS) != 0u;
    return true;
}

ModResult create_window_impl(ModContext* context, const WindowDesc* desc, WindowHandle* outWindow) {
    if (outWindow != nullptr) {
        *outWindow = 0;
    }
    auto* mod = mod_from_context(context);
    if (mod == nullptr || desc == nullptr || desc->struct_size < sizeof(WindowDesc) ||
        outWindow == nullptr || desc->width == 0 || desc->height == 0)
    {
        return MOD_INVALID_ARGUMENT;
    }

    SDL_WindowFlags flags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
    if ((desc->flags & WINDOW_FLAG_RESIZABLE) != 0u) {
        flags |= SDL_WINDOW_RESIZABLE;
    }
    if ((desc->flags & WINDOW_FLAG_HIDDEN) != 0u) {
        flags |= SDL_WINDOW_HIDDEN;
    }
    if ((desc->flags & WINDOW_FLAG_BORDERLESS) != 0u) {
        flags |= SDL_WINDOW_BORDERLESS;
    }
    if ((desc->flags & WINDOW_FLAG_ALWAYS_ON_TOP) != 0u) {
        flags |= SDL_WINDOW_ALWAYS_ON_TOP;
    }
    if ((desc->flags & WINDOW_FLAG_TRANSPARENT) != 0u) {
        flags |= SDL_WINDOW_TRANSPARENT;
    }

    const auto properties = SDL_CreateProperties();
    if (properties == 0) {
        Log.error("[{}] create_window: {}", mod->metadata.id, SDL_GetError());
        return MOD_ERROR;
    }

    const char* title = desc->title != nullptr ? desc->title : mod->metadata.name.c_str();
    const auto x = desc->x == WINDOW_POSITION_UNDEFINED ? SDL_WINDOWPOS_UNDEFINED : desc->x;
    const auto y = desc->y == WINDOW_POSITION_UNDEFINED ? SDL_WINDOWPOS_UNDEFINED : desc->y;
    const bool propertiesSet =
        SDL_SetStringProperty(properties, SDL_PROP_WINDOW_CREATE_TITLE_STRING, title) &&
        SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_X_NUMBER, x) &&
        SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_Y_NUMBER, y) &&
        SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, desc->width) &&
        SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, desc->height) &&
        SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER, flags) &&
        SDL_SetBooleanProperty(
            properties, SDL_PROP_WINDOW_CREATE_EXTERNAL_GRAPHICS_CONTEXT_BOOLEAN, true);
    if (!propertiesSet) {
        Log.error("[{}] create_window: {}", mod->metadata.id, SDL_GetError());
        SDL_DestroyProperties(properties);
        return MOD_ERROR;
    }

    SDL_Window* window = SDL_CreateWindowWithProperties(properties);
    SDL_DestroyProperties(properties);
    if (window == nullptr) {
        Log.error("[{}] create_window: {}", mod->metadata.id, SDL_GetError());
        return MOD_ERROR;
    }

    const auto windowId = SDL_GetWindowID(window);
    const auto handle = s_windows.emplace(*mod, WindowSlot{
                                                    .window = window,
                                                    .windowId = windowId,
                                                    .onEvent = desc->on_event,
                                                    .userData = desc->user_data,
                                                });
    s_windowsById.emplace(windowId, handle);
    *outWindow = handle;
    return MOD_OK;
}

ModResult destroy_window_impl(ModContext* context, WindowHandle handle) {
    auto* mod = mod_from_context(context);
    if (mod == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    auto* slot = resolve_window(*mod, handle);
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    if (slot->graphicsRefs != 0) {
        return MOD_CONFLICT;
    }
    const auto windowId = slot->windowId;
    SDL_Window* window = slot->window;
    s_windowsById.erase(windowId);
    s_windows.erase_owned(handle, *mod);
    SDL_DestroyWindow(window);
    return MOD_OK;
}

ModResult show_window_impl(ModContext* context, WindowHandle handle) {
    auto* mod = mod_from_context(context);
    auto* slot = mod != nullptr ? resolve_window(*mod, handle) : nullptr;
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return SDL_ShowWindow(slot->window) ? MOD_OK : MOD_ERROR;
}

ModResult hide_window_impl(ModContext* context, WindowHandle handle) {
    auto* mod = mod_from_context(context);
    auto* slot = mod != nullptr ? resolve_window(*mod, handle) : nullptr;
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return SDL_HideWindow(slot->window) ? MOD_OK : MOD_ERROR;
}

ModResult set_title_impl(ModContext* context, WindowHandle handle, const char* title) {
    auto* mod = mod_from_context(context);
    auto* slot = mod != nullptr ? resolve_window(*mod, handle) : nullptr;
    if (slot == nullptr || title == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return SDL_SetWindowTitle(slot->window, title) ? MOD_OK : MOD_ERROR;
}

ModResult set_size_impl(ModContext* context, WindowHandle handle, uint32_t width, uint32_t height) {
    auto* mod = mod_from_context(context);
    auto* slot = mod != nullptr ? resolve_window(*mod, handle) : nullptr;
    if (slot == nullptr || width == 0 || height == 0) {
        return MOD_INVALID_ARGUMENT;
    }
    return SDL_SetWindowSize(slot->window, static_cast<int>(width), static_cast<int>(height)) ?
               MOD_OK :
               MOD_ERROR;
}

ModResult get_info_impl(ModContext* context, WindowHandle handle, WindowInfo* outInfo) {
    if (outInfo == nullptr || outInfo->struct_size < sizeof(WindowInfo)) {
        return MOD_INVALID_ARGUMENT;
    }
    const uint32_t structSize = outInfo->struct_size;
    *outInfo = WindowInfo{.struct_size = structSize};
    auto* mod = mod_from_context(context);
    auto* slot = mod != nullptr ? resolve_window(*mod, handle) : nullptr;
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    return populate_info(slot->window, *outInfo) ? MOD_OK : MOD_ERROR;
}

void remove_mod_windows(LoadedMod& mod) {
    auto entries = s_windows.take_all(mod);
    for (auto& entry : entries) {
        s_windowsById.erase(entry.value.windowId);
        SDL_DestroyWindow(entry.value.window);
    }
}

constexpr WindowService s_windowService{
    .header = SERVICE_HEADER(WindowService, WINDOW_SERVICE_MAJOR, WINDOW_SERVICE_MINOR),
    .create_window = create_window_impl,
    .destroy_window = destroy_window_impl,
    .show_window = show_window_impl,
    .hide_window = hide_window_impl,
    .set_title = set_title_impl,
    .set_size = set_size_impl,
    .get_info = get_info_impl,
};

}  // namespace

bool window_dispatch_event(const SDL_Event& event) {
    SDL_Window* eventWindow = SDL_GetWindowFromEvent(&event);
    if (eventWindow == nullptr) {
        return false;
    }
    const auto it = s_windowsById.find(SDL_GetWindowID(eventWindow));
    if (it == s_windowsById.end()) {
        return false;
    }
    const auto handle = it->second;
    const auto* entry = s_windows.find(handle);
    if (entry == nullptr) {
        return true;
    }

    WindowEventType type;
    bool exposed = true;
    switch (event.type) {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        type = WINDOW_EVENT_CLOSE_REQUESTED;
        break;
    case SDL_EVENT_WINDOW_RESIZED:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
        type = WINDOW_EVENT_RESIZED;
        break;
    case SDL_EVENT_WINDOW_MOVED:
        type = WINDOW_EVENT_MOVED;
        break;
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        type = WINDOW_EVENT_FOCUS_GAINED;
        break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        type = WINDOW_EVENT_FOCUS_LOST;
        break;
    case SDL_EVENT_WINDOW_SHOWN:
        type = WINDOW_EVENT_SHOWN;
        break;
    case SDL_EVENT_WINDOW_HIDDEN:
        type = WINDOW_EVENT_HIDDEN;
        break;
    default:
        exposed = false;
        break;
    }
    if (!exposed || entry->value.onEvent == nullptr || !entry->owner->active) {
        return true;
    }

    WindowInfo info = WINDOW_INFO_INIT;
    populate_info(entry->value.window, info);
    const WindowEvent windowEvent{
        .struct_size = sizeof(WindowEvent),
        .type = type,
        .x = info.x,
        .y = info.y,
        .width = info.width,
        .height = info.height,
        .pixel_width = info.pixel_width,
        .pixel_height = info.pixel_height,
        .display_scale = info.display_scale,
    };
    auto* owner = entry->owner;
    const auto callback = entry->value.onEvent;
    void* userData = entry->value.userData;
    try {
        callback(owner->context.get(), handle, &windowEvent, userData);
    } catch (const std::exception& e) {
        fail_mod(
            *owner, MOD_ERROR, fmt::format("Exception in window event callback: {}", e.what()));
    } catch (...) {
        fail_mod(*owner, MOD_ERROR, "Unknown exception in window event callback");
    }
    return true;
}

ModResult window_acquire_for_graphics(
    LoadedMod& mod, WindowHandle handle, SDL_Window*& outWindow) {
    outWindow = nullptr;
    auto* slot = resolve_window(mod, handle);
    if (slot == nullptr) {
        return MOD_INVALID_ARGUMENT;
    }
    if (slot->graphicsRefs != 0) {
        return MOD_CONFLICT;
    }
    ++slot->graphicsRefs;
    outWindow = slot->window;
    return MOD_OK;
}

void window_release_for_graphics(LoadedMod& mod, WindowHandle handle) {
    auto* slot = resolve_window(mod, handle);
    if (slot != nullptr && slot->graphicsRefs > 0) {
        --slot->graphicsRefs;
    }
}

bool window_get_pixel_size(LoadedMod& mod, WindowHandle handle, uint32_t& width, uint32_t& height) {
    auto* slot = resolve_window(mod, handle);
    if (slot == nullptr) {
        return false;
    }
    int pixelWidth = 0;
    int pixelHeight = 0;
    if (!SDL_GetWindowSizeInPixels(slot->window, &pixelWidth, &pixelHeight) || pixelWidth <= 0 ||
        pixelHeight <= 0)
    {
        return false;
    }
    width = static_cast<uint32_t>(pixelWidth);
    height = static_cast<uint32_t>(pixelHeight);
    return true;
}

constinit const ServiceModule g_windowModule{
    .id = WINDOW_SERVICE_ID,
    .majorVersion = WINDOW_SERVICE_MAJOR,
    .minorVersion = WINDOW_SERVICE_MINOR,
    .service = &s_windowService,
    .modDetached = remove_mod_windows,
};

}  // namespace dusk::mods::svc
