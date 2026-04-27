#include "prelaunch_screen.hpp"

#include "button.hpp"
#include "disc_state.hpp"
#include "game_option.hpp"
#include "prelaunch_layout.hpp"
#include "ui.hpp"

#include "../file_select.hpp"
#include "../iso_validate.hpp"
#include "dusk/config.hpp"
#include "dusk/main.h"
#include "dusk/settings.h"

#include <RmlUi/Core.h>
#include <RmlUi/Core/ElementDocument.h>
#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_misc.h>
#include <aurora/aurora.h>
#include <dolphin/card.h>
#include <fmt/format.h>

#include "aurora/lib/window.hpp"

#include <aurora/rmlui.hpp>

#include <array>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace dusk::ui::prelaunch {
namespace {

enum class View {
    Main,
    Options,
    LanguageSelect,
    GraphicsBackendSelect,
    SaveFileTypeSelect,
};

struct BackendChoice {
    AuroraBackend backend = BACKEND_AUTO;
    std::string id;
    std::string name;
};

constexpr std::array<const char*, 5> kLanguageNames = {
    "English", "German", "French", "Spanish", "Italian",
};

constexpr std::array<SDL_DialogFileFilter, 2> kGameDiscFileFilters{{
    {"Game Disc Images", "iso;gcm;ciso;gcz;nfs;rvz;wbfs;wia;tgc"},
    {"All Files", "*"},
}};

std::string iso_validation_error_message(iso::ValidationError code) {
    switch (code) {
    case iso::ValidationError::IOError:
        return "Unable to read selected disc image";
    case iso::ValidationError::InvalidImage:
        return "Unable to interpret selected file as a disc image";
    case iso::ValidationError::WrongGame:
        return "Disc is for a different game";
    case iso::ValidationError::WrongVersion:
        return "Disc is for an unsupported version. Only NTSC & PAL GameCube are "
               "supported at this time";
    case iso::ValidationError::ExecutableMismatch:
        return "Disc contains modified executable files";
    case iso::ValidationError::Success:
        return {};
    case iso::ValidationError::Unknown:
    default:
        return "Unknown disc validation error";
    }
}

std::string_view backend_name(AuroraBackend backend) {
    switch (backend) {
    default:
        return "Auto";
    case BACKEND_D3D12:
        return "D3D12";
    case BACKEND_D3D11:
        return "D3D11";
    case BACKEND_METAL:
        return "Metal";
    case BACKEND_VULKAN:
        return "Vulkan";
    case BACKEND_OPENGL:
        return "OpenGL";
    case BACKEND_OPENGLES:
        return "OpenGL ES";
    case BACKEND_WEBGPU:
        return "WebGPU";
    case BACKEND_NULL:
        return "Null";
    }
}

std::string_view backend_id(AuroraBackend backend) {
    switch (backend) {
    default:
        return "auto";
    case BACKEND_D3D12:
        return "d3d12";
    case BACKEND_D3D11:
        return "d3d11";
    case BACKEND_METAL:
        return "metal";
    case BACKEND_VULKAN:
        return "vulkan";
    case BACKEND_OPENGL:
        return "opengl";
    case BACKEND_OPENGLES:
        return "opengles";
    case BACKEND_WEBGPU:
        return "webgpu";
    case BACKEND_NULL:
        return "null";
    }
}

bool try_parse_backend(std::string_view backend, AuroraBackend& outBackend) {
    if (backend == "auto") {
        outBackend = BACKEND_AUTO;
        return true;
    }
    if (backend == "d3d11") {
        outBackend = BACKEND_D3D11;
        return true;
    }
    if (backend == "d3d12") {
        outBackend = BACKEND_D3D12;
        return true;
    }
    if (backend == "metal") {
        outBackend = BACKEND_METAL;
        return true;
    }
    if (backend == "vulkan") {
        outBackend = BACKEND_VULKAN;
        return true;
    }
    if (backend == "opengl") {
        outBackend = BACKEND_OPENGL;
        return true;
    }
    if (backend == "opengles") {
        outBackend = BACKEND_OPENGLES;
        return true;
    }
    if (backend == "webgpu") {
        outBackend = BACKEND_WEBGPU;
        return true;
    }
    if (backend == "null") {
        outBackend = BACKEND_NULL;
        return true;
    }
    return false;
}

std::string_view card_type_name(CARDFileType type) {
    switch (type) {
    case CARD_GCIFOLDER:
        return "GCI Folder";
    case CARD_RAWIMAGE:
        return "Card Image";
    default:
        return "";
    }
}

std::filesystem::path resource_path(const char* filename) {
    const char* basePath = SDL_GetBasePath();
    if (basePath == nullptr) {
        return std::filesystem::path("res") / filename;
    }
    return std::filesystem::path(basePath) / "res" / filename;
}

std::string display_path(std::string_view path) {
    const char* home = SDL_GetUserFolder(SDL_FOLDER_HOME);
    if (home == nullptr || home[0] == '\0') {
        home = std::getenv("HOME");
    }
    if (home == nullptr || home[0] == '\0') {
        return std::string(path);
    }

    std::string homePath(home);
    while (homePath.size() > 1 && homePath.back() == '/') {
        homePath.pop_back();
    }

    if (path == homePath) {
        return "~";
    }

    if (path.size() > homePath.size() && path.substr(0, homePath.size()) == homePath &&
        path[homePath.size()] == '/')
    {
        return "~" + std::string(path.substr(homePath.size()));
    }

    return std::string(path);
}

std::vector<BackendChoice> backend_choices() {
    std::vector<BackendChoice> choices;
    choices.push_back({BACKEND_AUTO, std::string(backend_id(BACKEND_AUTO)),
                       std::string(backend_name(BACKEND_AUTO))});

    size_t backendCount = 0;
    const AuroraBackend* availableBackends = aurora_get_available_backends(&backendCount);
    for (size_t i = 0; i < backendCount; ++i) {
        const AuroraBackend backend = availableBackends[i];
        choices.push_back(
            {backend, std::string(backend_id(backend)), std::string(backend_name(backend))});
    }

    return choices;
}

class Screen : public Rml::EventListener {
public:
    bool initialize() {
        if (m_initialized) {
            return true;
        }
        if (!ui::initialize()) {
            return false;
        }

        m_selectedIsoPath = getSettings().backend.isoPath.getValue();
        validate_selected_iso(false);
        m_initialGraphicsBackend = getSettings().backend.graphicsBackend.getValue();
        m_initialized = true;

        if (is_selected_path_valid() && getSettings().backend.skipPreLaunchUI.getValue()) {
            IsGameLaunched = true;
            return true;
        }

        set_active(true);
        rebuild();
        if (m_document == nullptr) {
            shutdown();
            return false;
        }
        return true;
    }

    void shutdown() {
        close_document();
        set_active(false);
        m_initialized = false;
        m_focusIds.clear();
    }

    bool is_active() const { return m_initialized && ui::is_active(); }

    void handle_event(const SDL_Event& event) { ui::handle_event(event); }

    void update() {
        if (m_requestedBack) {
            m_requestedBack = false;
            back();
            if (!is_active()) {
                return;
            }
        }

        if (!m_requestedActivation.empty()) {
            const std::string requestedActivation = m_requestedActivation;
            m_requestedActivation.clear();
            activate(requestedActivation);
            if (!is_active()) {
                return;
            }
        }

        if (m_requestedCycleDirection != 0) {
            const std::string requestedCycleId = m_requestedCycleId;
            const int requestedCycleDirection = m_requestedCycleDirection;
            m_requestedCycleId.clear();
            m_requestedCycleDirection = 0;
            cycle_option(requestedCycleId, requestedCycleDirection);
            if (!is_active()) {
                return;
            }
        }

        if (is_selected_path_valid() && getSettings().backend.skipPreLaunchUI.getValue()) {
            IsGameLaunched = true;
            shutdown();
            return;
        }

        ui::update();
        rebuild_if_layout_changed();
    }

    void ProcessEvent(Rml::Event& event) override {
        if (event.GetId() == Rml::EventId::Keydown) {
            const auto key = static_cast<Rml::Input::KeyIdentifier>(
                event.GetParameter<int>("key_identifier", Rml::Input::KI_UNKNOWN));
            if (handle_key(key)) {
                event.StopImmediatePropagation();
            }
        }
    }

private:
    bool m_initialized = false;
    View m_view = View::Main;
    Rml::ElementDocument* m_document = nullptr;
    std::vector<std::string> m_focusIds;
    std::vector<std::unique_ptr<Button> > m_buttons;
    std::unique_ptr<DiscState> m_discState;
    std::vector<std::unique_ptr<GameOption> > m_options;
    std::string m_pendingFocusId;
    std::string m_requestedActivation;
    std::string m_requestedCycleId;
    int m_requestedCycleDirection = 0;
    bool m_requestedBack = false;
    std::string m_selectedIsoPath;
    std::string m_errorString;
    std::string m_initialGraphicsBackend;
    bool m_compactLayout = false;
    bool m_selectedIsoValid = false;
    bool m_isPal = false;

    bool selected_path_exists() const {
#if TARGET_ANDROID
        return !m_selectedIsoPath.empty();
#else
        return !m_selectedIsoPath.empty() && SDL_GetPathInfo(m_selectedIsoPath.c_str(), nullptr);
#endif
    }

    bool is_selected_path_valid() const { return m_selectedIsoValid; }

    void validate_selected_iso(bool save_valid_path) {
        m_errorString.clear();
        m_selectedIsoValid = false;
        m_isPal = false;

        if (m_selectedIsoPath.empty()) {
            return;
        }

        if (!selected_path_exists()) {
            m_errorString = "Selected disc image does not exist";
            return;
        }

        const iso::ValidationError validationResult = iso::validate(m_selectedIsoPath.c_str());
        if (validationResult != iso::ValidationError::Success) {
            m_errorString = iso_validation_error_message(validationResult);
            return;
        }

        m_selectedIsoValid = true;
        m_isPal = iso::isPal(m_selectedIsoPath.c_str());
        if (save_valid_path) {
            getSettings().backend.isoPath.setValue(m_selectedIsoPath);
            Save();
        }
    }

    void close_document() {
        if (m_document == nullptr) {
            return;
        }

        m_document->RemoveEventListener(Rml::EventId::Keydown, this);
        m_buttons.clear();
        m_discState.reset();
        m_options.clear();
        m_focusIds.clear();
        m_document->Close();
        m_document = nullptr;
    }

    std::string logo_path() const {
        const auto logo_path = resource_path("logo-mascot.png");
        if (std::filesystem::exists(logo_path)) {
            return logo_path.string();
        }

        return {};
    }

    std::string selected_disc_text() const {
        if (!m_selectedIsoPath.empty()) {
            return display_path(m_selectedIsoPath);
        }

        return "Select a disc...";
    }

    std::string disc_status_text() const {
        if (!m_errorString.empty()) {
            return m_errorString;
        }
        if (is_selected_path_valid()) {
            return fmt::format("Disc region: {}", m_isPal ? "PAL" : "NTSC");
        }
        return {};
    }

    bool should_use_compact_layout() const {
        Rml::Context* context = aurora::rmlui::get_context();
        if (context == nullptr) {
            return false;
        }

        const Rml::Vector2i dimensions = context->GetDimensions();
        float dp_ratio = context->GetDensityIndependentPixelRatio();
        if (dp_ratio <= 0.0f) {
            dp_ratio = 1.0f;
        }

        const float width = static_cast<float>(dimensions.x) / dp_ratio;
        const float height = static_cast<float>(dimensions.y) / dp_ratio;
        return height < 680.0f && width >= 720.0f;
    }

    void rebuild_if_layout_changed() {
        const bool compactLayout = should_use_compact_layout();
        if (compactLayout == m_compactLayout) {
            return;
        }

        if (Rml::Context* context = aurora::rmlui::get_context()) {
            if (Rml::Element* focused = context->GetFocusElement()) {
                m_pendingFocusId = focused->GetId();
            }
        }
        rebuild();
    }

    void queue_activation(std::string id) { m_requestedActivation = std::move(id); }

    void queue_back() { m_requestedBack = true; }

    void queue_cycle(std::string id, int direction) {
        m_requestedCycleId = std::move(id);
        m_requestedCycleDirection = direction;
    }

    void add_button_control(Rml::Element* parent, std::string_view id, std::string_view text,
                            ButtonVariant variant) {
        const std::string idString(id);
        m_focusIds.push_back(idString);
        m_buttons.push_back(std::make_unique<Button>(
            parent, idString, text, variant, [this, idString] { queue_activation(idString); }));
    }

    void add_option_control(Rml::Element* parent, std::string_view id, std::string_view title,
                            std::string_view value, std::string_view detail) {
        const std::string idString(id);
        m_focusIds.push_back(idString);
        m_options.push_back(
            std::make_unique<GameOption>(parent, idString, title, value, detail,
                                         [this, idString] { queue_activation(idString); }));
    }

    void add_disc_control(Rml::Element* parent) {
        const std::string idString("select-disc");
        m_focusIds.push_back(idString);
        m_discState = std::make_unique<DiscState>(
            parent, idString, selected_disc_text(), disc_status_text(), !m_errorString.empty(),
            [this, idString] { queue_activation(idString); });
    }

    void build_main(Rml::Element* screen) {
        m_focusIds.clear();
        m_buttons.clear();
        m_discState.reset();
        m_options.clear();

        layout::add_brand(screen, logo_path(), m_compactLayout);
        Rml::Element* panel = layout::add_panel(screen, false, m_compactLayout);
        add_disc_control(panel);
        if (is_selected_path_valid()) {
            add_button_control(panel, "start", "Start Game", ButtonVariant::Primary);
        }

        add_button_control(panel, "options", "Options", ButtonVariant::Quiet);
    }

    AuroraBackend configured_backend() const {
        AuroraBackend backend = BACKEND_AUTO;
        if (!try_parse_backend(getSettings().backend.graphicsBackend.getValue(), backend)) {
            backend = BACKEND_AUTO;
        }
        return backend;
    }

    std::string configured_backend_id() const {
        return std::string(backend_id(configured_backend()));
    }

    std::string first_options_focus_id() const { return m_isPal ? "language" : "graphics-backend"; }

    void build_options(Rml::Element* screen) {
        m_focusIds.clear();
        m_buttons.clear();
        m_discState.reset();
        m_options.clear();

        layout::add_heading(screen, "Options");
        Rml::Element* panel = layout::add_panel(screen, true);

        if (m_isPal) {
            const auto selectedLanguage = getSettings().game.language.getValue();
            add_option_control(panel, "language", "Language",
                               kLanguageNames[static_cast<u8>(selectedLanguage)], "");
        }

        const AuroraBackend backend = configured_backend();
        const std::string restartDetail =
            getSettings().backend.graphicsBackend.getValue() != m_initialGraphicsBackend ?
                "Restart required" :
                "";
        add_option_control(panel, "graphics-backend", "Graphics Backend", backend_name(backend),
                           restartDetail);

        const auto fileType =
            static_cast<CARDFileType>(getSettings().backend.cardFileType.getValue());
        add_option_control(panel, "save-file-type", "Save File Type", card_type_name(fileType), "");

        add_button_control(panel, "back", "Back", ButtonVariant::Quiet);
    }

    void build_language_select(Rml::Element* screen) {
        m_focusIds.clear();
        m_buttons.clear();
        m_discState.reset();
        m_options.clear();

        layout::add_heading(screen, "Language");
        Rml::Element* panel = layout::add_panel(screen, true);

        const auto selectedLanguage = getSettings().game.language.getValue();
        for (size_t i = 0; i < kLanguageNames.size(); ++i) {
            const std::string id = fmt::format("language-{}", i);
            add_option_control(panel, id, kLanguageNames[i],
                               i == static_cast<size_t>(selectedLanguage) ? "Current" : "", "");
        }

        add_button_control(panel, "back", "Back", ButtonVariant::Quiet);
    }

    void build_graphics_backend_select(Rml::Element* screen) {
        m_focusIds.clear();
        m_buttons.clear();
        m_discState.reset();
        m_options.clear();

        layout::add_heading(screen, "Graphics Backend");
        Rml::Element* panel = layout::add_panel(screen, true);

        const std::string currentBackendId = configured_backend_id();
        for (const BackendChoice& choice : backend_choices()) {
            const std::string id = "backend-" + choice.id;
            add_option_control(panel, id, choice.name,
                               choice.id == currentBackendId ? "Current" : "", "");
        }

        add_button_control(panel, "back", "Back", ButtonVariant::Quiet);
    }

    void build_save_fileType_select(Rml::Element* screen) {
        m_focusIds.clear();
        m_buttons.clear();
        m_discState.reset();
        m_options.clear();

        layout::add_heading(screen, "Save File Type");
        Rml::Element* panel = layout::add_panel(screen, true);

        const auto fileType =
            static_cast<CARDFileType>(getSettings().backend.cardFileType.getValue());
        add_option_control(panel, "save-gci-folder", "GCI Folder",
                           fileType == CARD_GCIFOLDER ? "Current" : "", "");
        add_option_control(panel, "save-card-image", "Card Image",
                           fileType == CARD_RAWIMAGE ? "Current" : "", "");

        add_button_control(panel, "back", "Back", ButtonVariant::Quiet);
    }

    void rebuild() {
        Rml::Context* context = aurora::rmlui::get_context();
        if (context == nullptr) {
            return;
        }

        const std::string preferredFocus = choose_focus_after_rebuild();
        close_document();

        m_document = context->CreateDocument();
        if (m_document == nullptr) {
            return;
        }

        m_compactLayout = should_use_compact_layout();
        const bool compactMainLayout = m_view == View::Main && m_compactLayout;

        layout::style_document(m_document);
        Rml::Element* screen =
            layout::add_screen(m_document, compactMainLayout ? layout::ScreenLayout::CompactSplit :
                                                               layout::ScreenLayout::Standard);
        if (m_view == View::Main) {
            build_main(screen);
        } else if (m_view == View::Options) {
            build_options(screen);
        } else if (m_view == View::LanguageSelect) {
            build_language_select(screen);
        } else if (m_view == View::GraphicsBackendSelect) {
            build_graphics_backend_select(screen);
        } else if (m_view == View::SaveFileTypeSelect) {
            build_save_fileType_select(screen);
        }

        m_document->AddEventListener(Rml::EventId::Keydown, this);

        m_document->Show();
        focus_id(preferredFocus.empty() ? first_focus_id() : preferredFocus);
    }

    std::string choose_focus_after_rebuild() const {
        if (!m_pendingFocusId.empty()) {
            return m_pendingFocusId;
        }
        if (Rml::Context* context = aurora::rmlui::get_context()) {
            if (Rml::Element* focused = context->GetFocusElement()) {
                return focused->GetId();
            }
        }
        return {};
    }

    std::string first_focus_id() const {
        return m_focusIds.empty() ? std::string{} : m_focusIds.front();
    }

    int focus_index() const {
        Rml::Context* context = aurora::rmlui::get_context();
        if (context == nullptr) {
            return -1;
        }
        const Rml::Element* focused = context->GetFocusElement();
        if (focused == nullptr) {
            return -1;
        }

        const std::string& id = focused->GetId();
        for (int i = 0; i < static_cast<int>(m_focusIds.size()); ++i) {
            if (m_focusIds[i] == id) {
                return i;
            }
        }
        return -1;
    }

    void focus_id(std::string_view id) {
        if (m_document == nullptr || id.empty()) {
            return;
        }
        if (Rml::Element* element = m_document->GetElementById(std::string(id))) {
            element->Focus(true);
            m_pendingFocusId.clear();
        }
    }

    void move_focus(int direction) {
        if (m_focusIds.empty()) {
            return;
        }

        int index = focus_index();
        if (index < 0) {
            focus_id(m_focusIds.front());
            return;
        }

        const int nextIndex = index + direction;
        if (nextIndex < 0 || nextIndex >= static_cast<int>(m_focusIds.size())) {
            return;
        }

        focus_id(m_focusIds[static_cast<size_t>(nextIndex)]);
    }

    bool handle_key(Rml::Input::KeyIdentifier key) {
        switch (key) {
        case Rml::Input::KI_UP:
            move_focus(-1);
            return true;
        case Rml::Input::KI_DOWN:
            move_focus(1);
            return true;
        case Rml::Input::KI_LEFT:
            queue_cycle_focused(-1);
            return true;
        case Rml::Input::KI_RIGHT:
            queue_cycle_focused(1);
            return true;
        case Rml::Input::KI_RETURN:
            queue_focused_activation();
            return true;
        case Rml::Input::KI_ESCAPE:
        case Rml::Input::KI_F15:
            queue_back();
            return true;
        default:
            return false;
        }
    }

    void queue_focused_activation() {
        Rml::Context* context = aurora::rmlui::get_context();
        if (context == nullptr) {
            return;
        }
        if (Rml::Element* focused = context->GetFocusElement()) {
            queue_activation(focused->GetId());
        }
    }

    void queue_cycle_focused(int direction) {
        Rml::Context* context = aurora::rmlui::get_context();
        if (context == nullptr) {
            return;
        }
        if (Rml::Element* focused = context->GetFocusElement()) {
            queue_cycle(focused->GetId(), direction);
        }
    }

    static void file_dialog_callback(void* userdata, const char* path, const char* error) {
        auto* self = static_cast<Screen*>(userdata);
        if (self == nullptr) {
            return;
        }

        if (error != nullptr) {
            self->m_selectedIsoPath.clear();
            self->m_selectedIsoValid = false;
            self->m_isPal = false;
            self->m_errorString = fmt::format("File dialog error: {}", error);
            self->m_pendingFocusId = "select-disc";
            self->rebuild();
            return;
        }

        if (path == nullptr) {
            self->m_pendingFocusId =
                self->m_view == View::Options ? self->first_options_focus_id() : "select-disc";
            self->rebuild();
            return;
        }

        self->m_selectedIsoPath = path;
        self->validate_selected_iso(true);
        self->m_pendingFocusId =
            self->m_selectedIsoValid ?
                (self->m_view == View::Options ? self->first_options_focus_id() : "start") :
                (self->m_view == View::Options ? self->first_options_focus_id() : "select-disc");
        self->rebuild();
    }

    void show_file_select() {
        ShowFileSelect(&file_dialog_callback, this, aurora::window::get_sdl_window(),
                       kGameDiscFileFilters.data(), kGameDiscFileFilters.size(), nullptr, false);
    }

    void activate(std::string_view id) {
        if (id == "start") {
            if (is_selected_path_valid()) {
                IsGameLaunched = true;
                shutdown();
            }
            return;
        }

        if (id == "select-disc") {
            show_file_select();
            return;
        }

        if (id == "options") {
            m_view = View::Options;
            m_pendingFocusId = first_options_focus_id();
            rebuild();
            return;
        }

        if (id == "language" && m_isPal) {
            m_view = View::LanguageSelect;
            const auto selectedLanguage = getSettings().game.language.getValue();
            m_pendingFocusId = fmt::format("language-{}", static_cast<int>(selectedLanguage));
            rebuild();
            return;
        }

        if (id == "graphics-backend") {
            m_view = View::GraphicsBackendSelect;
            m_pendingFocusId = "backend-" + configured_backend_id();
            rebuild();
            return;
        }

        if (id == "save-file-type") {
            const auto fileType =
                static_cast<CARDFileType>(getSettings().backend.cardFileType.getValue());
            m_view = View::SaveFileTypeSelect;
            m_pendingFocusId = fileType == CARD_GCIFOLDER ? "save-gci-folder" : "save-card-image";
            rebuild();
            return;
        }

        if (id == "back") {
            back();
            return;
        }

        select_option(id);
    }

    void back() {
        if (m_view == View::Options) {
            m_view = View::Main;
            m_pendingFocusId = is_selected_path_valid() ? "start" : "select-disc";
            rebuild();
        } else if (m_view == View::LanguageSelect) {
            m_view = View::Options;
            m_pendingFocusId = "language";
            rebuild();
        } else if (m_view == View::GraphicsBackendSelect) {
            m_view = View::Options;
            m_pendingFocusId = "graphics-backend";
            rebuild();
        } else if (m_view == View::SaveFileTypeSelect) {
            m_view = View::Options;
            m_pendingFocusId = "save-file-type";
            rebuild();
        }
    }

    void cycle_option(std::string_view id, int direction) {
        if (m_view == View::LanguageSelect || m_view == View::GraphicsBackendSelect ||
            m_view == View::SaveFileTypeSelect)
        {
            move_focus(direction);
        }
    }

    void select_option(std::string_view id) {
        const std::string idString(id);
        if (idString.rfind("language-", 0) == 0 && m_isPal) {
            const int languageIndex = std::stoi(idString.substr(std::string("language-").size()));
            getSettings().game.language.setValue(static_cast<GameLanguage>(languageIndex));
            Save();
            m_view = View::Options;
            m_pendingFocusId = "language";
            rebuild();
            return;
        }

        if (idString.rfind("backend-", 0) == 0) {
            const std::string selectedBackend = idString.substr(std::string("backend-").size());
            AuroraBackend backend = BACKEND_AUTO;
            if (try_parse_backend(selectedBackend, backend)) {
                getSettings().backend.graphicsBackend.setValue(selectedBackend);
                Save();
            }
            m_view = View::Options;
            m_pendingFocusId = "graphics-backend";
            rebuild();
            return;
        }

        if (id == "save-gci-folder" || id == "save-card-image") {
            getSettings().backend.cardFileType.setValue(id == "save-gci-folder" ? CARD_GCIFOLDER :
                                                                                  CARD_RAWIMAGE);
            Save();
            m_view = View::Options;
            m_pendingFocusId = "save-file-type";
            rebuild();
        }
    }
};

Screen s_screen;

}  // namespace

bool initialize() {
    return s_screen.initialize();
}

void shutdown() {
    s_screen.shutdown();
}

bool is_active() {
    return s_screen.is_active();
}

void handle_event(const SDL_Event& event) {
    s_screen.handle_event(event);
}

void update() {
    s_screen.update();
}

}  // namespace dusk::ui::prelaunch
