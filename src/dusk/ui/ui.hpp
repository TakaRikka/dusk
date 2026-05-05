#pragma once

#include <RmlUi/Core.h>
#include <SDL3/SDL_events.h>

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

#include "nav_types.hpp"

namespace dusk::ui {
class Document;

using clock = std::chrono::steady_clock;

struct Toast {
    Rml::String type;
    Rml::String title;
    Rml::String content;
    clock::duration duration;
    clock::time_point startTime = clock::now();
};

struct Insets {
    float top = 0.0f;
    float right = 0.0f;
    float bottom = 0.0f;
    float left = 0.0f;

    bool operator==(const Insets& other) const noexcept {
        return top == other.top && right == other.right && bottom == other.bottom &&
               left == other.left;
    }
};

bool initialize() noexcept;
void shutdown() noexcept;

void handle_event(const SDL_Event& event) noexcept;
void update() noexcept;

Document& push_document(
    std::unique_ptr<Document> doc, bool show = true, bool passive = false) noexcept;
void show_top_document() noexcept;
bool any_document_visible() noexcept;
Document* top_document() noexcept;

std::filesystem::path resource_path(const std::filesystem::path& filename) noexcept;
std::string escape(std::string_view str) noexcept;

NavCommand map_nav_event(const Rml::Event& event) noexcept;
Insets safe_area_insets(Rml::Context* context) noexcept;

void push_toast(Toast toast) noexcept;
std::vector<Toast>& get_toasts() noexcept;

}  // namespace dusk::ui
