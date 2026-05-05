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
class Popup;

// Button clicked/pressed
constexpr u32 kSoundClick = Z2SE_SY_CURSOR_OK;
// "Play" button clicked/pressed
constexpr u32 kSoundPlay = Z2SE_SY_ITEM_COMBINE_ON;

// Menu button pressed (open/close menu bar or hide/show the active window)
constexpr u32 kSoundMenuOpen = Z2SE_SY_MENU_SUB_IN;
constexpr u32 kSoundMenuClose = Z2SE_SY_MENU_SUB_OUT;

// Window opened/closed
constexpr u32 kSoundWindowOpen = Z2SE_SY_MENU_NEXT;
constexpr u32 kSoundWindowClose = Z2SE_SY_MENU_BACK;

// Window tab changed
constexpr u32 kSoundTabChanged = Z2SE_SY_MENU_CURSOR_COMMON;

// Item within menu focused
constexpr u32 kSoundItemFocus = Z2SE_SY_CURSOR_ITEM;
// Item changed (e.g. number input left/right)
constexpr u32 kSoundItemChange = Z2SE_SY_NAME_CURSOR;
// Item enabled ("On")
constexpr u32 kSoundItemEnable = Z2SE_SUBJ_VIEW_IN;
// Item disabled ("Off")
constexpr u32 kSoundItemDisable = Z2SE_SUBJ_VIEW_OUT;

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

Document& push_document(std::unique_ptr<Document> doc, bool show = true) noexcept;
void show_top_document() noexcept;
bool any_document_visible() noexcept;
bool is_prelaunch_open() noexcept;
Document* top_document() noexcept;

std::filesystem::path resource_path(const std::filesystem::path& filename) noexcept;
std::string escape(std::string_view str) noexcept;
Rml::Element* append(Rml::Element* parent, const Rml::String& tag) noexcept;

NavCommand map_nav_event(const Rml::Event& event) noexcept;
Insets safe_area_insets(Rml::Context* context) noexcept;

}  // namespace dusk::ui
