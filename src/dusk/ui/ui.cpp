#include "ui.hpp"

#include <RmlUi/Core.h>
#include <SDL3/SDL_filesystem.h>
#include <absl/container/flat_hash_set.h>
#include <aurora/rmlui.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <filesystem>
#include <ranges>

#include "aurora/lib/window.hpp"
#include "dusk/io.hpp"
#include "input.hpp"
#include "prelaunch.hpp"
#include "window.hpp"

namespace dusk::ui {
namespace {

void load_font(const char* filename, bool fallback = false) {
    Rml::LoadFontFace(io::fs_path_to_string(resource_path(filename)), fallback);
}

bool sInitialized = false;
std::vector<std::unique_ptr<Document> > sDocumentStack;
// Documents that don't participate in the focus stack
std::vector<std::unique_ptr<Document> > sPassiveDocuments;
std::deque<Toast> sToasts;
bool sMenuNotificationRequested = false;

// Sometimes gamepads can connect and disconnect quickly, especially during
// connection negotiation. In this case, we'll receive an _ADDED event for a
// disconnected gamepad. Storing IDs here lets use only show disconnected
// notifications for gamepads that we sent a connected notification for.
absl::flat_hash_set<SDL_JoystickID> sConnectedGamepads;

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
    load_font("NotoMono-Regular.ttf");

    sInitialized = true;
    return true;
}

void shutdown() noexcept {
    sDocumentStack.clear();
    sPassiveDocuments.clear();
    sConnectedGamepads.clear();
    input::reset_input_state();
    input::release_input_block();
    sInitialized = false;
}

const char* battery_icon(SDL_PowerState state, int level) noexcept {
    if (state == SDL_POWERSTATE_UNKNOWN || state == SDL_POWERSTATE_NO_BATTERY) {
        return "e1a6";  // Battery Unknown
    }
    if (state == SDL_POWERSTATE_ERROR) {
        return "f7ea";  // Battery Error
    }
    if (state == SDL_POWERSTATE_CHARGED || level == 100) {
        return "e1a4";  // Battery Full
    }
    if (state == SDL_POWERSTATE_CHARGING) {
        if (level >= 90)
            return "f0a7";  // Battery Charging 90
        if (level >= 80)
            return "f0a6";  // Battery Charging 80
        if (level >= 60)
            return "f0a5";  // Battery Charging 60
        if (level >= 50)
            return "f0a4";  // Battery Charging 50
        if (level >= 30)
            return "f0a3";  // Battery Charging 30
        if (level >= 20)
            return "f0a2";  // Battery Charging 20
        return "e1a3";      // Battery Charging Full (we use it as empty)
    }
    if (level >= 90)
        return "ebd2";  // Battery 6 Bar
    if (level >= 80)
        return "ebd4";  // Battery 5 Bar
    if (level >= 60)
        return "ebe2";  // Battery 4 Bar
    if (level >= 50)
        return "ebdd";  // Battery 3 Bar
    if (level >= 30)
        return "ebe0";  // Battery 2 Bar
    if (level >= 20)
        return "ebd9";  // Battery 1 Bar
    return "e19c";      // Battery Alert
}

const char* connection_state_icon(SDL_JoystickConnectionState state) noexcept {
    switch (state) {
    case SDL_JOYSTICK_CONNECTION_WIRELESS:
        return "e1a7";
    case SDL_JOYSTICK_CONNECTION_WIRED:
        return "e1e0";
    default:
        return nullptr;
    }
}

void handle_event(const SDL_Event& event) noexcept {
    if (!aurora::rmlui::is_initialized()) {
        return;
    }

    if (event.type == SDL_EVENT_GAMEPAD_ADDED) {
        auto* gamepad = SDL_GetGamepadFromID(event.gdevice.which);
        if (SDL_GamepadConnected(gamepad)) {
            if (getSettings().game.enableControllerToasts) {
                const char* name = SDL_GetGamepadName(gamepad);
                Rml::String content = fmt::format("<span>{}</span>", name ? name : "[Unknown]");
                Rml::String title = "Device Connected";
                if (const char* icon = connection_state_icon(SDL_GetGamepadConnectionState(gamepad))) {
                    title = fmt::format(
                        "<row><span>{}</span> <icon class=\"connection\">&#x{};</icon></row>", title,
                        icon);
                }
                int batteryLevel = -1;
                const auto powerState = SDL_GetGamepadPowerInfo(gamepad, &batteryLevel);
                if (powerState != SDL_POWERSTATE_UNKNOWN) {
                    content = fmt::format(
                        "<row>{}</row><row class=\"muted\"><icon class=\"battery\">&#x{};</icon>",
                        content, battery_icon(powerState, batteryLevel));
                    if (batteryLevel > -1) {
                        content = fmt::format("{}&nbsp;<span>{}%</span>", content, batteryLevel);
                    }
                    content += "</row>";
                }
                push_toast({
                    .type = "controller",
                    .title = title,
                    .content = content,
                    .duration = std::chrono::seconds(4),
                });
            }
            sConnectedGamepads.insert(event.gdevice.which);
        }
    } else if (event.type == SDL_EVENT_GAMEPAD_REMOVED &&
               sConnectedGamepads.contains(event.gdevice.which))
    {
        if (getSettings().game.enableControllerToasts) {
            const char* name = SDL_GetGamepadNameForID(event.gdevice.which);
            push_toast({
                .type = "controller",
                .title = "Device Disconnected",
                .content = name ? name : "[Unknown]",
                .duration = std::chrono::seconds(4),
            });
        }
        sConnectedGamepads.erase(event.gdevice.which);
    }
    input::handle_event(event);
}

Document& push_document(std::unique_ptr<Document> doc, bool show, bool passive) noexcept {
    Document& ret = *doc;
    if (passive) {
        sPassiveDocuments.push_back(std::move(doc));
    } else {
        sDocumentStack.push_back(std::move(doc));
    }
    if (show) {
        ret.show();
    }
    input::sync_input_block();
    return ret;
}

void show_top_document() noexcept {
    if (auto* doc = top_document()) {
        doc->show();
    }
    input::sync_input_block();
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
    if (!aurora::rmlui::is_initialized()) {
        return;
    }

    input::update_input();
    const auto update_documents = [](auto& documents) {
        const std::size_t count = documents.size();
        for (std::size_t i = 0; i < count && i < documents.size(); ++i) {
            Document* doc = documents[i].get();
            if (doc != nullptr && !doc->closed()) {
                doc->update();
            }
        }
    };
    update_documents(sDocumentStack);
    update_documents(sPassiveDocuments);

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

    input::sync_input_block();
}

std::filesystem::path resource_path(const std::filesystem::path& filename) noexcept {
    return std::filesystem::path("res") / filename;
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

std::vector<std::unique_ptr<Document> >& get_document_stack() noexcept {
    return sDocumentStack;
}

std::deque<Toast>& get_toasts() noexcept {
    return sToasts;
}

void show_menu_notification() noexcept {
    sMenuNotificationRequested = true;
}

bool consume_menu_notification_request() noexcept {
    const bool requested = sMenuNotificationRequested;
    sMenuNotificationRequested = false;
    return requested;
}

}  // namespace dusk::ui
