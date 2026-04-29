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

NavCommand map_nav_event(const Rml::Event& event) noexcept {
    const auto key = static_cast<Rml::Input::KeyIdentifier>(
        event.GetParameter<int>("key_identifier", Rml::Input::KI_UNKNOWN));
    switch (key) {
    case Rml::Input::KeyIdentifier::KI_UP:
        return NavCommand::Up;
    case Rml::Input::KeyIdentifier::KI_DOWN:
        return NavCommand::Down;
    case Rml::Input::KeyIdentifier::KI_LEFT:
        return NavCommand::Left;
    case Rml::Input::KeyIdentifier::KI_RIGHT:
        return NavCommand::Right;
    case Rml::Input::KeyIdentifier::KI_ESCAPE:
        return NavCommand::Cancel;
    case Rml::Input::KeyIdentifier::KI_RETURN:
        return NavCommand::Confirm;
    case Rml::Input::KeyIdentifier::KI_NEXT:
        return NavCommand::Next;
    case Rml::Input::KeyIdentifier::KI_PRIOR:
        return NavCommand::Previous;
    default:
        return NavCommand::None;
    }
}

}  // namespace dusk::ui
