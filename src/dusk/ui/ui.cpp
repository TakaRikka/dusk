#include "ui.hpp"

#include <RmlUi/Core.h>
#include <SDL3/SDL_filesystem.h>
#include <aurora/rmlui.hpp>

#include <algorithm>
#include <filesystem>

#include "aurora/lib/window.hpp"
#include "input.hpp"
#include "window.hpp"

namespace dusk::ui {
namespace {

void load_font(const char* filename, bool fallback = false) {
    Rml::LoadFontFace(resource_path(filename).string(), fallback);
}

bool sInitialized = false;

struct OpenDocument {
    std::unique_ptr<Document> doc;
    bool pendingDestroy = false;
};
std::vector<OpenDocument> sDocuments;

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
    sDocuments.clear();
    reset_input_state();
    release_input_block();
    sInitialized = false;
}

Document& push_document(std::unique_ptr<Document> doc, bool show) noexcept {
    if (auto* doc = top_document()) {
        doc->hide();
    }
    Document& ret = *doc;
    sDocuments.push_back({std::move(doc)});
    if (show) {
        ret.show();
    }
    sync_input_block();
    return ret;
}

void pop_document() noexcept {
    for (auto it = sDocuments.rbegin(); it != sDocuments.rend(); ++it) {
        if (!it->pendingDestroy) {
            it->doc->hide();
            it->pendingDestroy = true;
            break;
        }
    }
    if (auto* doc = top_document()) {
        doc->show();
    }
    sync_input_block();
}

void toggle_top_document() noexcept {
    auto* doc = top_document();
    if (doc == nullptr) {
        return;
    }

    if (doc->visible()) {
        doc->hide();
    } else {
        doc->show();
    }
    sync_input_block();
}

bool any_document_visible() noexcept {
    return std::any_of(sDocuments.begin(), sDocuments.end(),
        [](const OpenDocument& doc) { return doc.doc != nullptr && doc.doc->visible(); });
}

Document* top_document() noexcept {
    for (auto it = sDocuments.rbegin(); it != sDocuments.rend(); ++it) {
        if (!it->pendingDestroy) {
            return it->doc.get();
        }
    }
    return nullptr;
}

void update() noexcept {
    update_input();
    for (const auto& doc : sDocuments) {
        doc.doc->update();
    }
    sDocuments.erase(
        std::remove_if(sDocuments.begin(), sDocuments.end(),
            [](const OpenDocument& doc) { return doc.pendingDestroy && doc.doc->can_destroy(); }),
        sDocuments.end());
    sync_input_block();
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
    case Rml::Input::KeyIdentifier::KI_PAUSE:
        return NavCommand::Menu;
    case Rml::Input::KeyIdentifier::KI_NEXT:
        return NavCommand::Next;
    case Rml::Input::KeyIdentifier::KI_PRIOR:
        return NavCommand::Previous;
    default:
        return NavCommand::None;
    }
}

Insets safe_area_insets(Rml::Context* context) noexcept {
    if (context == nullptr) {
        return {};
    }

    auto* window = aurora::window::get_sdl_window();
    if (window == nullptr) {
        return {};
    }

    const AuroraWindowSize windowSize = aurora::window::get_window_size();
    if (windowSize.width == 0 || windowSize.height == 0) {
        return {};
    }

    SDL_Rect safeRect{};
    if (!SDL_GetWindowSafeArea(window, &safeRect)) {
        return {};
    }

    const Rml::Vector2i contextSize = context->GetDimensions();
    const float scaleX = static_cast<float>(contextSize.x) / static_cast<float>(windowSize.width);
    const float scaleY = static_cast<float>(contextSize.y) / static_cast<float>(windowSize.height);

    const float safeRight = static_cast<float>(safeRect.x + safeRect.w);
    const float safeBottom = static_cast<float>(safeRect.y + safeRect.h);
    return {
        .top = std::max(0.0f, static_cast<float>(safeRect.y)) * scaleY,
        .right = std::max(0.0f, static_cast<float>(windowSize.width) - safeRight) * scaleX,
        .bottom = std::max(0.0f, static_cast<float>(windowSize.height) - safeBottom) * scaleY,
        .left = std::max(0.0f, static_cast<float>(safeRect.x)) * scaleX,
    };
}

}  // namespace dusk::ui
