#include "imgui.h"

#include "ImGuiConfig.hpp"
#include "ImGuiEngine.hpp"
#include "ImGuiPreLaunchWindow.hpp"

#include "ImGuiConsole.hpp"
#include "dusk/main.h"
#include "dusk/settings.h"

#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_filesystem.h>

#include "aurora/lib/internal.hpp"
#include "aurora/lib/window.hpp"

namespace dusk {

typedef void (ImGuiPreLaunchWindow::*drawFunc)();

drawFunc drawTable[2] = {
    &ImGuiPreLaunchWindow::drawMainMenu,
    &ImGuiPreLaunchWindow::drawOptions
};

static constexpr std::array<SDL_DialogFileFilter, 2> skGameDiscFileFilters{{
    {"Game Disc Images", "iso;gcm;ciso;gcz;nfs;rvz;wbfs;wia;tgc"},
    {"All Files", "*"},
}};

void fileDialogCallback(void* userdata, const char* const* filelist, [[maybe_unused]] int filter) {
    auto* self = static_cast<ImGuiPreLaunchWindow*>(userdata);
    if (filelist != nullptr) {
        if (filelist[0] == nullptr) {
            // Cancelled
            self->m_selectedIsoPath.clear();
        } else {
            self->m_selectedIsoPath = filelist[0];
            getSettings().backend.isoPath.setValue(self->m_selectedIsoPath);
            config::Save();
        }
    } else {
        // Error occurred
        self->m_selectedIsoPath.clear();
        self->m_errorString = fmt::format("File dialog error: {}", SDL_GetError());
    }
}


ImGuiPreLaunchWindow::ImGuiPreLaunchWindow() {

}

bool ImGuiPreLaunchWindow::isSelectedPathValid() const {
    return !m_selectedIsoPath.empty() && SDL_GetPathInfo(m_selectedIsoPath.c_str(), nullptr);
}

void ImGuiPreLaunchWindow::draw() {
    if (m_IsFirstDraw) {
        m_selectedIsoPath = getSettings().backend.isoPath;
        m_IsFirstDraw = false;
    }

    if (isSelectedPathValid() && getSettings().backend.skipPreLaunchUI) {
        dusk::IsGameLaunched = true;
        return;
    }

    auto& io = ImGui::GetIO();

    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("Pre Launch Window", nullptr, ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

    const auto& windowSize = ImGui::GetWindowSize();

    for (int i = 0; i < 5; i++)
        ImGui::NewLine();

    float iconSize = 128.f;
    ImGui::SameLine(windowSize.x / 2 - iconSize + (iconSize / 2));
    if (ImGuiEngine::duskIcon != 0)
        ImGui::Image(ImGuiEngine::duskIcon, ImVec2{iconSize, iconSize});
    ImGui::PushFont(ImGuiEngine::fontExtraLarge);
    ImGuiTextCenter("Dusk");
    ImGui::PopFont();

    (this->*drawTable[m_CurMenu])();

    ImGui::End();
}

void ImGuiPreLaunchWindow::drawMainMenu() {
    const auto& windowSize = ImGui::GetWindowSize();
    static const char* popopModalLabel = "Skip Pre-Launch UI?";
    ImGui::SetCursorPosY(windowSize.y - 200);

    ImGui::PushFont(ImGuiEngine::fontLarge);

    bool disabled = false;
    if (m_selectedIsoPath.empty() || !SDL_GetPathInfo(m_selectedIsoPath.c_str(), nullptr)) {
        ImGui::BeginDisabled();
        disabled = true;
    }

    if (ImGuiButtonCenter("Start Game")) {
        if (!getSettings().backend.skipPreLaunchUI) {
            ImGui::OpenPopup(popopModalLabel);
        }else {
            dusk::IsGameLaunched = true;
        }
    }

    if (disabled) {
        ImGui::EndDisabled();
    }

    if (ImGuiButtonCenter("Options")) {
        m_CurMenu = 1;
    }

    ImGui::PopFont();

    if (ImGui::BeginPopupModal(popopModalLabel, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Would you like to skip this menu on future launches?");
        ImGui::Separator();

        if (ImGui::Button("Yes", ImVec2(120, 0))) {
            getSettings().backend.skipPreLaunchUI.setValue(true);
            config::Save();
            ImGui::CloseCurrentPopup();
            dusk::IsGameLaunched = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("No", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            dusk::IsGameLaunched = true;
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
    if (ImGui::BeginChild("OptionsChild", ImVec2(childWidth, endCursorY - cursorY), ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground)) {
        ImGui::InputText("Game ISO Path", &m_selectedIsoPath, ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        if (ImGui::Button("Set")) {
            SDL_ShowOpenFileDialog(&fileDialogCallback, this, aurora::window::get_sdl_window(), skGameDiscFileFilters.data(),
                                   int(skGameDiscFileFilters.size()), nullptr, false);
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

}
