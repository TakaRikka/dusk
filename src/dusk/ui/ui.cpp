#include "ui.hpp"

#include <RmlUi/Core.h>
#include <SDL3/SDL_filesystem.h>
#include <aurora/rmlui.hpp>

#include <algorithm>
#include <filesystem>
#include <ranges>

#include "aurora/lib/window.hpp"
#include "input.hpp"
#include "prelaunch.hpp"
#include "window.hpp"

namespace dusk::ui {
namespace {

void load_font(const char* filename, bool fallback = false) {
    Rml::LoadFontFace(resource_path(filename).string(), fallback);
}

bool sInitialized = false;
std::vector<std::unique_ptr<Document> > sDocumentStack;
// Documents that don't participate in the focus stack
std::vector<std::unique_ptr<Document> > sPassiveDocuments;
std::deque<Toast> sToasts;

}  // namespace

bool initialize() noexcept {
    if (sInitialized) {
        return true;
    }
    if (!aurora::rmlui::is_initialized()) {
        return false;
    }

    load_font("FiraSans-Regular.ttf", true);
    load_font("FiraSans-Bold.ttf");
    load_font("FiraSansCondensed-Regular.ttf");
    load_font("FiraSansCondensed-Bold.ttf");
    load_font("AlegreyaSC-Regular.ttf");
    load_font("AlegreyaSC-Bold.ttf");
    load_font("MaterialSymbolsRounded-Regular.ttf");

    sInitialized = true;
    return true;
}

void shutdown() noexcept {
    sDocumentStack.clear();
    sPassiveDocuments.clear();
    reset_input_state();
    release_input_block();
    sInitialized = false;
}

Document& push_document(std::unique_ptr<Document> doc, bool show, bool passive) noexcept {
    Document& ret = *doc;
    if (passive) {
        sPassiveDocuments.push_back(std::move(doc));
    } else {
        sDocumentStack.push_back({std::move(doc)});
    }
    if (show) {
        ret.show();
    }
    sync_input_block();
    return ret;
}

void show_top_document() noexcept {
    if (auto* doc = top_document()) {
        doc->show();
    }
    sync_input_block();
}

bool any_document_visible() noexcept {
    return std::any_of(sDocumentStack.begin(), sDocumentStack.end(),
        [](const auto& doc) { return doc && doc->visible(); });
}

bool is_prelaunch_open() noexcept {
    return std::any_of(sDocumentStack.begin(), sDocumentStack.end(), [](const auto& doc) {
        const auto* prelaunch = dynamic_cast<const Prelaunch*>(doc.get());
        return prelaunch != nullptr && !prelaunch->pending_close() && !prelaunch->closed();
    });
}

Document* top_document() noexcept {
    for (auto& doc : std::views::reverse(sDocumentStack)) {
        if (!doc->closed() && !doc->pending_close()) {
            return doc.get();
        }
    }
    return nullptr;
}

void update() noexcept {
    update_input();
    for (const auto& doc : sDocumentStack) {
        doc->update();
    }
    for (const auto& doc : sPassiveDocuments) {
        doc->update();
    }

    // Remove closed documents
    {
        const auto [first, last] =
            std::ranges::remove_if(sDocumentStack, [](const auto& doc) { return doc->closed(); });
        sDocumentStack.erase(first, last);
    }
    {
        const auto [first, last] = std::ranges::remove_if(
            sPassiveDocuments, [](const auto& doc) { return doc->closed(); });
        sPassiveDocuments.erase(first, last);
    }

    // If no documents have focus, explicitly focus the top one
    if (auto* context = aurora::rmlui::get_context();
        context != nullptr && (context->GetFocusElement() == nullptr ||
                                  context->GetFocusElement() == context->GetRootElement()))
    {
        for (auto& doc : std::views::reverse(sDocumentStack)) {
            if (!doc->closed() && !doc->pending_close() && doc->focus()) {
                break;
            }
        }
    }

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

Rml::Element* append(Rml::Element* parent, const Rml::String& tag) noexcept {
    if (parent == nullptr) {
        return nullptr;
    }
    auto* doc = parent->GetOwnerDocument();
    if (doc == nullptr) {
        return nullptr;
    }
    return parent->AppendChild(doc->CreateElement(tag));
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
    case Rml::Input::KeyIdentifier::KI_F1:
        return event.GetParameter<int>("shift_key", 0) ? NavCommand::None : NavCommand::Menu;
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

void push_toast(Toast toast) noexcept {
    sToasts.push_back(std::move(toast));
}

std::deque<Toast>& get_toasts() noexcept {
    return sToasts;
}

}  // namespace dusk::ui
