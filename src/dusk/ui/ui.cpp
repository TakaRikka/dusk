#include "ui.hpp"

#include <RmlUi/Core.h>
#include <SDL3/SDL_filesystem.h>
#include <aurora/rmlui.hpp>

#include <filesystem>

#include "window.hpp"

namespace dusk::ui {
namespace {

void load_font(const char* filename, bool fallback = false) {
    Rml::LoadFontFace(resource_path(filename).string(), fallback);
}

bool sInitialized = false;
std::vector<std::unique_ptr<Window> > sWindows;

}  // namespace

bool initialize() noexcept {
    if (sInitialized) {
        return true;
    }
    if (!aurora::rmlui::is_initialized()) {
        return false;
    }

    load_font("FiraSans-Regular.ttf", true);
    load_font("FiraSansCondensed-Regular.ttf");
    load_font("FiraSansCondensed-Bold.ttf");

    sInitialized = true;
    return true;
}

void shutdown() noexcept {
    sWindows.clear();
    sInitialized = false;
}

void handle_event(const SDL_Event& event) noexcept {
    // TODO
}

Window& add_window(std::unique_ptr<Window> window) noexcept {
    Window& ret = *window;
    sWindows.push_back(std::move(window));
    return ret;
}

void remove_window(Window& window) noexcept {
    // TODO
}

void update() noexcept {
    for (const auto& window : sWindows) {
        window->update();
    }
}

std::filesystem::path resource_path(const std::filesystem::path& filename) noexcept {
    const char* basePath = SDL_GetBasePath();
    if (basePath == nullptr) {
        return std::filesystem::path("res") / filename;
    }
    return std::filesystem::path(basePath) / "res" / filename;
}

std::string escape(std::string_view str) noexcept {
    std::string result;
    result.reserve(str.size());
    for (const char c : str) {
        switch (c) {
        case '&':
            result += "&amp;";
            break;
        case '<':
            result += "&lt;";
            break;
        case '>':
            result += "&gt;";
            break;
        case '"':
            result += "&quot;";
            break;
        case '\'':
            result += "&apos;";
            break;
        default:
            result += c;
            break;
        }
    }
    return result;
}

}  // namespace dusk::ui
