#include "imgui.h"

#include "ImGuiConfig.hpp"
#include "ImGuiEngine.hpp"
#include "ImGuiPreLaunchWindow.hpp"

#include "../file_select.hpp"
#include "../iso_validate.hpp"
#include "ImGuiConsole.hpp"
#include "dusk/main.h"
#include "dusk/settings.h"

#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL_filesystem.h>
#include <thread>

#include "aurora/lib/internal.hpp"
#include "aurora/lib/window.hpp"

namespace dusk {

typedef void (ImGuiPreLaunchWindow::*drawFunc)();

drawFunc drawTable[2] = {&ImGuiPreLaunchWindow::drawMainMenu, &ImGuiPreLaunchWindow::drawOptions};

static constexpr std::array<const char*, 5> skLanguageNames = {
    "English", "German", "French", "Spanish", "Italian"
};

static constexpr std::array<SDL_DialogFileFilter, 2> skGameDiscFileFilters{{
    {"Game Disc Images", "iso;gcm;ciso;gcz;nfs;rvz;wbfs;wia;tgc"},
    {"All Files", "*"},
}};

static std::string ShowIsoInvalidError(const iso::ValidationError code) {
    using namespace std::literals::string_literals;

    switch (code) {
    case iso::ValidationError::Success:
        return "Disc image successfully verified."s;
    case iso::ValidationError::IOError:
        return "An unknown I/O error occurred."s;
    case iso::ValidationError::InvalidImage:
        return "Unable to interpret the selected file as a disc image."s;
    case iso::ValidationError::WrongGame:
        return "The selected disc image is for a different game."s;
    case iso::ValidationError::WrongVersion:
        return "The selected disc image is for an unsupported version of the game."s;
    case iso::ValidationError::DiscHashMismatch:
        return "The selected disc image doesn't match known fingerprint."s;
    case iso::ValidationError::Cancelled:
        return "Disc verification cancelled."s;
    default:
        return "An unknown error occurred."s;
    }
}

static std::string_view card_type_name(CARDFileType type) {
    switch (type) {
    case CARD_GCIFOLDER:
        return "GCI Folder"sv;
    case CARD_RAWIMAGE:
        return "Card Image"sv;
    default:
        return ""sv;
    }
}

void fileDialogCallback(void* userdata, const char* path, const char* error) {
    auto* self = static_cast<ImGuiPreLaunchWindow*>(userdata);
    if (error != nullptr) {
        self->m_selectedIsoPath.clear();
        self->m_errorString = fmt::format("File dialog error: {}", error);
        return;
    }

    if (path == nullptr) {
        self->m_selectedIsoPath.clear();
        return;
    }

    self->m_selectedIsoPath = path;
    self->m_isPal = iso::isPal(path);
    getSettings().backend.isoPath.setValue(self->m_selectedIsoPath);
    getSettings().backend.discVerificationResult.setValue(DiscVerificationResult::NotChecked);
    config::Save();

    auto& dv = self->m_discVerification;
    dv.status = {};
    dv.task = std::thread([self, &dv]() {
        dv.isRunning = true;
        dv.error = iso::validate(self->m_selectedIsoPath.c_str(), dv.status);
        dv.isRunning = false;

        if (dv.error == iso::ValidationError::Success) {
            getSettings().backend.discVerificationResult.setValue(DiscVerificationResult::Passed);
        } else { 
            dv.shouldOpenReport = true;
            if (dv.error != iso::ValidationError::Cancelled) {
                getSettings().backend.discVerificationResult.setValue(DiscVerificationResult::Failed);
            }
        }
        config::Save();
    });
    dv.task.detach();
}

ImGuiPreLaunchWindow::ImGuiPreLaunchWindow() = default;

bool ImGuiPreLaunchWindow::isSelectedPathValid() const {
#if TARGET_ANDROID
    return !m_selectedIsoPath.empty();  // unsure why SDL_GetPathInfo doesnt work here
#else
    return !m_selectedIsoPath.empty() && SDL_GetPathInfo(m_selectedIsoPath.c_str(), nullptr);
#endif
}

void ImGuiPreLaunchWindow::draw() {
    if (m_IsFirstDraw) {
        m_selectedIsoPath = getSettings().backend.isoPath;
        m_isPal = !m_selectedIsoPath.empty() && iso::isPal(m_selectedIsoPath.c_str());
        m_initialGraphicsBackend = getSettings().backend.graphicsBackend;
        m_IsFirstDraw = false;
    }

    if (isSelectedPathValid() && getSettings().backend.skipPreLaunchUI) {
        dusk::IsGameLaunched = true;
        return;
    }

    auto& io = ImGui::GetIO();

    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowBgAlpha(0.65f);

    ImGui::Begin("Pre Launch Window", nullptr,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoBringToFrontOnFocus);

    const auto& windowSize = ImGui::GetWindowSize();

    for (int i = 0; i < 5; i++)
        ImGui::NewLine();

    float iconSize = 150.f;
    ImGui::SameLine(windowSize.x / 2 - iconSize + (iconSize / 2));
    if (ImGuiEngine::orgIcon != 0) {
        ImGui::Image(ImGuiEngine::orgIcon, ImVec2{iconSize, iconSize});
    }
    ImGuiTextCenter("Twilit Realm presents");
    if (ImGuiEngine::duskLogo) {
        ImGui::NewLine();
        float width = iconSize * 2.5f;
        ImGui::SameLine(windowSize.x / 2 - width + (width / 2));
        ImGui::Image(ImGuiEngine::duskLogo, ImVec2{width, iconSize});
    } else {
        ImGui::PushFont(ImGuiEngine::fontExtraLarge);
        ImGuiTextCenter("Dusk");
        ImGui::PopFont();
    }

    (this->*drawTable[m_CurMenu])();

    ImGui::End();

    drawDiscVerification();
}

void ImGuiPreLaunchWindow::drawMainMenu() {
    const auto& windowSize = ImGui::GetWindowSize();
    ImGui::SetCursorPosY(windowSize.y - 200);

    ImGui::PushFont(ImGuiEngine::fontLarge);

    if (!isSelectedPathValid()) {
        if (!m_errorString.empty()) {
            ImGuiTextCenter(m_errorString);
        }

        if (ImGuiButtonCenter("Select disc image...")) {
            ShowFileSelect(&fileDialogCallback, this, aurora::window::get_sdl_window(),
                           skGameDiscFileFilters.data(), int(skGameDiscFileFilters.size()), nullptr,
                           false);
        }
    } else {
        if (ImGuiButtonCenter("Start game")) {
            dusk::IsGameLaunched = true;
        }
    }

    if (ImGuiButtonCenter("Options")) {
        m_CurMenu = 1;
    }

    ImGui::PopFont();
}

void ImGuiPreLaunchWindow::drawDiscVerification() {
    if (m_discVerification.isRunning && !ImGui::IsPopupOpen("DiscVerifyProgress")) {
        ImGui::OpenPopup("DiscVerifyProgress");
    }

    if (ImGui::BeginPopupModal("DiscVerifyProgress", nullptr,
                               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Verifying disc image...");
        const auto bytesTotal = m_discVerification.status.bytesTotal;
        if (bytesTotal != 0) {
            const auto percent = static_cast<float>(m_discVerification.status.bytesRead) /
                                 static_cast<float>(bytesTotal);
            ImGui::ProgressBar(percent);
        }

        if (ImGui::Button("Cancel")) {
            m_discVerification.status.shouldCancel = true;
        }

        if (!m_discVerification.isRunning) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if (m_discVerification.shouldOpenReport && !ImGui::IsPopupOpen("DiscVerifyReport")) {
        ImGui::OpenPopup("DiscVerifyReport");
    }

    if (ImGui::BeginPopupModal("DiscVerifyReport", nullptr,
                               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                   ImGuiWindowFlags_NoSavedSettings |
                                   ImGuiWindowFlags_AlwaysAutoResize))
    {
        auto error = m_discVerification.error;
        ImGui::Text("%s", ShowIsoInvalidError(error).c_str());
        if (error == iso::ValidationError::Cancelled ||
            error == iso::ValidationError::DiscHashMismatch ||
            error == iso::ValidationError::WrongVersion)
        {
            ImGui::Text("You may experience issues during gameplay.");
        }

        if (ImGuiButtonCenter("OK")) {
            m_discVerification.shouldOpenReport = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void ImGuiPreLaunchWindow::drawOptions() {
    const auto& windowSize = ImGui::GetWindowSize();

    ImGui::NewLine();

    ImGui::PushFont(ImGuiEngine::fontLarge);
    ImGuiTextCenter("Options");
    ImGui::Separator();
    ImGui::PopFont();

    auto cursorY = ImGui::GetCursorPosY();
    float endCursorY = windowSize.y - 100;

    float childWidth = windowSize.x - 400;

    ImGui::SetCursorPosX(windowSize.x / 2 - (childWidth / 2));
    if (ImGui::BeginChild("OptionsChild", ImVec2(childWidth, endCursorY - cursorY),
                          ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground))
    {
        if (!m_errorString.empty()) {
            ImGuiTextCenter(m_errorString);
        }

        ImGui::InputText("Game ISO Path", &m_selectedIsoPath, ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        if (ImGui::Button(m_selectedIsoPath == "" ? "Set" : "Change")) {
            ShowFileSelect(&fileDialogCallback, this, aurora::window::get_sdl_window(),
                           skGameDiscFileFilters.data(), int(skGameDiscFileFilters.size()), nullptr,
                           false);
        }

        ImGui::Text("Disc verification:");
        ImGui::SameLine();
        switch (getSettings().backend.discVerificationResult.getValue()) {
            case dusk::DiscVerificationResult::NotChecked:
                ImGui::TextColored(ImVec4(0.322f, 0.322f, 0.322f, 1.0f), "not checked");
                break;
            case dusk::DiscVerificationResult::Failed:
                ImGui::TextColored(ImVec4(0.906f, 0.f, 0.0435f, 1.0f), "failed");
                break;
            case dusk::DiscVerificationResult::Passed:
                ImGui::TextColored(ImVec4(0.f, 0.651f, 0.239f, 1.0f), "passed");
                break;
        }

        if (m_isPal) {
            auto selectedLanguage = getSettings().game.language.getValue();
            if (ImGui::BeginCombo("Language", skLanguageNames[static_cast<u8>(selectedLanguage)])) {
                for (u8 i = 0; i < skLanguageNames.size(); ++i) {
                    if (ImGui::Selectable(skLanguageNames[i])) {
                        getSettings().game.language.setValue(static_cast<GameLanguage>(i));
                        config::Save();
                    }
                }

                ImGui::EndCombo();
            }
        }

        AuroraBackend configuredBackend = BACKEND_AUTO;
        const std::string& configuredBackendId = getSettings().backend.graphicsBackend;
        if (!try_parse_backend(configuredBackendId, configuredBackend)) {
            configuredBackend = BACKEND_AUTO;
        }

        if (ImGui::BeginCombo("Graphics Backend", backend_name(configuredBackend).data())) {
            if (ImGui::Selectable("Auto", configuredBackend == BACKEND_AUTO)) {
                getSettings().backend.graphicsBackend.setValue("auto");
                config::Save();
            }

            size_t backendCount = 0;
            const AuroraBackend* availableBackends = aurora_get_available_backends(&backendCount);
            for (size_t i = 0; i < backendCount; ++i) {
                const AuroraBackend backend = availableBackends[i];
                const bool isSelected = configuredBackend == backend;
                if (ImGui::Selectable(backend_name(backend).data(), isSelected)) {
                    getSettings().backend.graphicsBackend.setValue(
                        std::string(backend_id(backend)));
                    config::Save();
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
        if (configuredBackendId != m_initialGraphicsBackend) {
            ImGui::TextDisabled("Restart Required");
        }
        auto curFileType = (CARDFileType)getSettings().backend.cardFileType.getValue();

        if (ImGui::BeginCombo("Save File Type", card_type_name(curFileType).data())) {

            if (ImGui::Selectable("GCI Folder", curFileType == CARD_GCIFOLDER)) {
                getSettings().backend.cardFileType.setValue(CARD_GCIFOLDER);
                config::Save();
            }

            if (ImGui::Selectable("Card Image", curFileType == CARD_RAWIMAGE)) {
                getSettings().backend.cardFileType.setValue(CARD_RAWIMAGE);
                config::Save();
            }

            ImGui::EndCombo();
        }

        ImGui::EndChild();
    }

    ImGui::SetCursorPosY(endCursorY);
    ImGui::NewLine();

    ImGui::Separator();

    ImGui::PushFont(ImGuiEngine::fontLarge);
    if (ImGuiButtonCenter("Back")) {
        m_CurMenu = 0;
    }
    ImGui::PopFont();
}

}  // namespace dusk
