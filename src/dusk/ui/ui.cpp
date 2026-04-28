#include "ui.hpp"

#include <RmlUi/Core.h>
#include <SDL3/SDL_filesystem.h>
#include <aurora/rmlui.hpp>

#include <filesystem>

namespace dusk::ui {
namespace {

void load_font(const char* filename, bool fallback = false) {
    Rml::LoadFontFace(resource_path(filename).string(), fallback);
}

}  // namespace

static bool s_initialized = false;

bool initialize() noexcept {
    if (s_initialized) {
        return true;
    }
    if (!aurora::rmlui::is_initialized()) {
        return false;
    }

    load_font("FiraSans-Regular.ttf", true);
    load_font("FiraSansCondensed-Regular.ttf");
    load_font("FiraSansCondensed-Bold.ttf");

    s_initialized = true;
    return true;
}

void shutdown() noexcept {
    s_initialized = false;
}

void handle_event(const SDL_Event& event) noexcept {
    // TODO
}

void update() noexcept {
    // TODO
}

std::filesystem::path resource_path(const std::filesystem::path& filename) noexcept {
    const char* basePath = SDL_GetBasePath();
    if (basePath == nullptr) {
        return std::filesystem::path("res") / filename;
    }
    return std::filesystem::path(basePath) / "res" / filename;
}

}  // namespace dusk::ui
