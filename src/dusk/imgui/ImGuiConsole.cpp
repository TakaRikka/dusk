#include <algorithm>
#include <array>
#include <numeric>
#include <string_view>
#include <chrono>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include <imgui_internal.h>

#include "fmt/format.h"
#include "ImGuiConsole.hpp"
#include "ImGuiEngine.hpp"
#include "JSystem/JUtility/JUTGamePad.h"
#include "SDL3/SDL_mouse.h"
#include "dusk/action_bindings.h"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/config.hpp"
#include "dusk/data.hpp"
#include "dusk/dusk.h"
#include "dusk/frame_interpolation.h"
#include "dusk/livesplit.h"
#include "dusk/main.h"
#include "dusk/settings.h"
#include "dusk/ui/ui.hpp"
#include "f_pc/f_pc_manager.h"
#include "f_pc/f_pc_name.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_main.h"
#include "tracy/Tracy.hpp"

#if _WIN32
#define NOMINMAX
#include "Windows.h"
#endif

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace {
ImGuiWindow* FindDragScrollWindow(ImGuiWindow* window) {
    while (window != nullptr) {
        const bool canScrollX = window->ScrollMax.x > 0.0f;
        const bool canScrollY = window->ScrollMax.y > 0.0f;
        const bool canScrollWithMouse = (window->Flags & (ImGuiWindowFlags_NoScrollWithMouse |
                                                          ImGuiWindowFlags_NoMouseInputs)) == 0;
        if ((canScrollX || canScrollY) && canScrollWithMouse) {
            return window;
        }
        window = window->ParentWindow;
    }
    return nullptr;
}
}  // namespace

namespace dusk {
    float ImGuiScale() { return 1.0f; }

    void ImGuiStringViewText(std::string_view text) {
        // begin()/end() do not work on MSVC
        ImGui::TextUnformatted(text.data(), text.data() + text.size());
    }

    void DuskToast(std::string_view message, float duration) {
        g_imguiConsole.AddToast(message, duration);
    }

    void ImGuiTextCenter(std::string_view text) {
        ImGui::NewLine();
        float fontSize = ImGui::CalcTextSize(
            text.data(),
            text.data() + text.size(),
            false,
            ImGui::GetWindowSize().x).x;
        ImGui::SameLine(ImGui::GetWindowSize().x / 2 - fontSize + fontSize / 2);
        ImGui::PushTextWrapPos(ImGui::GetWindowSize().x);
        ImGuiStringViewText(text);
        ImGui::PopTextWrapPos();
    }

    bool ImGuiButtonCenter(std::string_view text) {
        ImGui::NewLine();
        float fontSize = ImGui::CalcTextSize(text.data(), text.data() + text.size()).x;
        fontSize += ImGui::GetStyle().FramePadding.x;
        ImGui::SameLine(ImGui::GetWindowSize().x / 2 - fontSize + fontSize / 2);
        return ImGui::Button(text.data());
    }

    std::string BytesToString(size_t bytes) {
        constexpr std::array suffixes{ "B"sv, "KB"sv, "MB"sv, "GB"sv, "TB"sv, "PB"sv, "EB"sv };
        uint32_t s = 0;
        auto count = static_cast<double>(bytes);
        while (count >= 1024.0 && s < 7) {
            s++;
            count /= 1024.0;
        }
        if (count - floor(count) == 0.0)
        {
            return fmt::format(FMT_STRING("{}{}"), static_cast<size_t>(count), suffixes[s]);
        }
        return fmt::format(FMT_STRING("{:.1f}{}"), count, suffixes[s]);
    }

    void SetOverlayWindowLocation(int corner) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 workPos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 workSize = viewport->WorkSize;
        ImVec2 windowPos;
        ImVec2 windowPosPivot;
        const float padding = 10.0f * ImGuiScale();
        windowPos.x = (corner & 1) != 0 ? (workPos.x + workSize.x - padding) : (workPos.x + padding);
        windowPos.y = (corner & 2) != 0 ? (workPos.y + workSize.y - padding) : (workPos.y + padding);
        windowPosPivot.x = (corner & 1) != 0 ? 1.0f : 0.0f;
        windowPosPivot.y = (corner & 2) != 0 ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPosPivot);
    }

    bool ShowCornerContextMenu(int& corner, int avoidCorner) {
        bool result = false;
        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Custom", nullptr, corner == -1)) {
                corner = -1;
                result = true;
            }
            if (ImGui::MenuItem("Top-left", nullptr, corner == 0, avoidCorner != 0)) {
                corner = 0;
                result = true;
            }
            if (ImGui::MenuItem("Top-right", nullptr, corner == 1, avoidCorner != 1)) {
                corner = 1;
                result = true;
            }
            if (ImGui::MenuItem("Bottom-left", nullptr, corner == 2, avoidCorner != 2)) {
                corner = 2;
                result = true;
            }
            if (ImGui::MenuItem("Bottom-right", nullptr, corner == 3, avoidCorner != 3)) {
                corner = 3;
                result = true;
            }
            ImGui::EndPopup();
        }
        return result;
    }

    // from https://github.com/ocornut/imgui/issues/1496#issuecomment-569892444
    void ImGuiBeginGroupPanel(const char* name, const ImVec2& size) {
        ImGui::BeginGroup();

        auto cursorPos = ImGui::GetCursorScreenPos();
        auto itemSpacing = ImGui::GetStyle().ItemSpacing;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

        auto frameHeight = ImGui::GetFrameHeight();
        ImGui::BeginGroup();

        ImVec2 effectiveSize = size;
        if (size.x < 0.0f)
            effectiveSize.x = ImGui::GetContentRegionAvail().x;
        else
            effectiveSize.x = size.x;
        ImGui::Dummy(ImVec2(effectiveSize.x, 0.0f));

        ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::BeginGroup();
        ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::TextUnformatted(name);
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));
        ImGui::BeginGroup();

        ImGui::PopStyleVar(2);

        ImGui::GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->WorkRect.Max.x -= frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->Size.x -= frameHeight;

        ImGui::PushItemWidth(effectiveSize.x - frameHeight);
    }

    // from https://github.com/ocornut/imgui/issues/1496#issuecomment-569892444
    void ImGuiEndGroupPanel() {
        ImGui::PopItemWidth();

        auto itemSpacing = ImGui::GetStyle().ItemSpacing;

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

        auto frameHeight = ImGui::GetFrameHeight();

        // workaround for incorrect capture of columns/table width by placing
        // zero-sized dummy element in the same group, this ensure
        // max X cursor position is updated correctly
        ImGui::SameLine(0.0f, 0.0f);
        ImGui::Dummy(ImVec2(0.0f, 0.0f));

        ImGui::EndGroup();
        ImGui::EndGroup();

        ImGui::SameLine(0.0f, 0.0f);
        ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
        ImGui::Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));

        ImGui::EndGroup();

        auto itemMin = ImGui::GetItemRectMin();
        auto itemMax = ImGui::GetItemRectMax();

        float frameSpacingY = 8.0f;
        float frameBottomPadding = 10.0f;

        ImVec2 halfFrame = ImVec2((frameHeight * 0.25f) * 0.5f, frameHeight * 0.5f);
        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(itemMin.x + halfFrame.x, itemMin.y + halfFrame.y + frameSpacingY),
            ImVec2(itemMax.x - halfFrame.x, itemMax.y + frameBottomPadding),
            ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)),
            halfFrame.x);

        ImGui::PopStyleVar(2);

        ImGui::GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->WorkRect.Max.x += frameHeight * 0.5f;
        ImGui::GetCurrentWindow()->Size.x += frameHeight;

        ImGui::Dummy(ImVec2(0.0f, 0.0f));

        ImGui::EndGroup();
    }

    ImGuiConsole g_imguiConsole;

    ImGuiConsole::ImGuiConsole() {}

    void ImGuiConsole::HandleSDLEvent(const SDL_Event& event) {
        (void)event;
    }

    void ImGuiConsole::UpdateSettings() {
        getTransientSettings().skipFrameRateLimit = getSettings().game.enableTurboKeybind &&
            (ImGui::IsKeyDown(ImGuiKey_Tab) || getActionBindHoldAnyPort(ActionBinds::TURBO_SPEED_BUTTON));

        if (dusk::frame_interp::get_ui_tick_pending() && mDoMain::developmentMode == 1 && (mDoCPd_c::getHold(PAD_1) & (PAD_TRIGGER_R | PAD_TRIGGER_L)) == (PAD_TRIGGER_R | PAD_TRIGGER_L) && mDoCPd_c::getTrigY(PAD_1)) {
            getTransientSettings().moveLinkActive = !getTransientSettings().moveLinkActive;
        }
        if (mDoMain::developmentMode != 1) {
            getTransientSettings().moveLinkActive = false;
        }
    }

    void ImGuiConsole::PreDraw() {
        ZoneScoped;

        UpdateSettings();

        if (ImGui::IsKeyPressed(ImGuiKey_F11)) {
            getSettings().video.enableFullscreen.setValue(!getSettings().video.enableFullscreen);
            VISetWindowFullscreen(getSettings().video.enableFullscreen);
            config::Save();
        }

        if (getSettings().game.enableResetKeybind && ImGui::GetIO().KeyCtrl &&
            ImGui::IsKeyReleased(ImGuiKey_R) && !fpcM_SearchByName(fpcNm_LOGO_SCENE_e))
        {
            JUTGamePad::C3ButtonReset::sResetSwitchPushing = true;
        }

        if (ImGui::GetIO().KeyShift && ImGui::IsKeyPressed(ImGuiKey_F1)) {
            if (getSettings().backend.enableAdvancedSettings) {
                m_isHidden = !m_isHidden;
            } else {
                m_isHidden = true;
            }
        }
        
        bool showMenu = !m_isHidden;

        // The menu bar renders with ImGuiCol_WindowBg behind it. We just want ImGuiCol_MenuBarBg,
        // so make the window bg fully transparent temporarily
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        if (showMenu && ImGui::BeginMainMenuBar()) {
            m_menuTools.draw();

            ImGui::EndMainMenuBar();
        }
        ImGui::PopStyleColor();

        if (dusk::IsGameLaunched && !m_isLaunchInitialized) {
            m_isLaunchInitialized = true;
            if (getSettings().game.speedrunMode && getSettings().game.liveSplitEnabled) {
                dusk::speedrun::connectLiveSplit();
            }
        }

        UpdateDragScroll();

        // Show message when Aurora backend is Null
        if (aurora_get_backend() == BACKEND_NULL) {
            auto& io = ImGui::GetIO();
            ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowBgAlpha(0.65f);
            ImGui::Begin("Pre Launch Window", nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                    ImGuiWindowFlags_NoBringToFrontOnFocus);
            ImGui::NewLine();
            if (ImGuiEngine::duskLogo) {
                const auto& windowSize = ImGui::GetWindowSize();
                ImGui::NewLine();
                float iconSize = 150.f;
                float width = iconSize * 2.5f;
                ImGui::SameLine(windowSize.x / 2 - width + (width / 2));
                ImGui::Image(ImGuiEngine::duskLogo, ImVec2{width, iconSize});
            } else {
                ImGui::PushFont(ImGuiEngine::fontExtraLarge);
                ImGuiTextCenter("Dusklight");
                ImGui::PopFont();
            }
            ImGui::PushFont(ImGuiEngine::fontLarge);
            ImGuiTextCenter("Failed to initialize any graphics backend.");
            ImGuiTextCenter("\nDusklight requires Vulkan 1.1+, or Direct X 12.0.");
            ImGuiTextCenter("\nTry updating your Operating System and GPU drivers.");
            const auto& style = ImGui::GetStyle();
            const auto retrySize = ImGui::CalcTextSize("Retry (Auto backend)");
            const auto quitSize = ImGui::CalcTextSize("Quit");
            float buttonsWidth = quitSize.x + style.FramePadding.x * 2.0f;
            if constexpr (SupportsProcessRestart) {
                buttonsWidth += retrySize.x + style.FramePadding.x * 2.0f + style.ItemSpacing.x;
            }
#if DUSK_CAN_OPEN_DATA_FOLDER
            const auto openSize = ImGui::CalcTextSize("Open Data Folder");
            buttonsWidth += openSize.x + style.FramePadding.x * 2.0f + style.ItemSpacing.x;
#endif
            ImGui::NewLine();
            ImGui::SetCursorPosX(
                ImMax(style.WindowPadding.x, (ImGui::GetWindowSize().x - buttonsWidth) * 0.5f));
            if constexpr (SupportsProcessRestart) {
                if (ImGui::Button("Retry (Auto backend)")) {
                    getSettings().backend.graphicsBackend.setValue("auto");
                    config::Save();
                    RestartRequested = true;
                    IsRunning = false;
                }
                ImGui::SameLine();
            }
#if DUSK_CAN_OPEN_DATA_FOLDER
            if (ImGui::Button("Open Data Folder")) {
                data::open_data_path();
            }
            ImGui::SameLine();
#endif
            if (ImGui::Button("Quit")) {
                IsRunning = false;
            }
            ImGui::PopFont();
            ImGui::End();
        }

        m_menuTools.ShowInputViewer();

        if (dusk::IsGameLaunched && !dusk::getSettings().game.speedrunMode) {
            m_menuTools.ShowDebugOverlay();
            m_menuTools.ShowCameraOverlay();
            m_menuTools.ShowProcessManager();
            m_menuTools.ShowHeapOverlay();
            m_menuTools.ShowStubLog();
            m_menuTools.ShowBloomWindow();
            m_menuTools.ShowPlayerInfo();
            m_menuTools.ShowAudioDebug();
            m_menuTools.ShowSaveEditor();
            m_menuTools.ShowStateShare();
            m_menuTools.ShowActorSpawner();
        }

        // Hide mouse cursor if the F1 menu is not open and the cursor is idle for 3 seconds.
        if (dusk::getSettings().game.gyroMode.getValue() != GyroMode::Mouse)
        {
            ImGuiIO& io = ImGui::GetIO();
            if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f) {
                mouseHideTimer = 0.0f;
                ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;  // Imgui will re-show cursor.
            } else if (mouseHideTimer <= 3.0f) {
                mouseHideTimer += ImGui::GetIO().DeltaTime;
            } else {
                ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
                SDL_HideCursor();
            }
        }

        ShowToasts();
    }

    void ImGuiConsole::PostDraw() {
        m_menuTools.afterDraw();
        ShowPipelineProgress();
    }

    void ImGuiConsole::UpdateDragScroll() {
        ImGuiContext& g = *ImGui::GetCurrentContext();
        ImGuiIO& io = ImGui::GetIO();

        if (io.MouseSource != ImGuiMouseSource_TouchScreen) {
            m_dragScrollWindow = nullptr;
            return;
        }

        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            m_dragScrollWindow = nullptr;
            return;
        }

        if (io.WantTextInput || (g.ActiveId != 0 && g.InputTextState.ID == g.ActiveId)) {
            m_dragScrollWindow = nullptr;
            return;
        }

        if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left, io.MouseDragThreshold)) {
            return;
        }

        if (m_dragScrollWindow == nullptr) {
            ImGuiWindow* hoveredWindow = nullptr;
            ImGuiWindow* hoveredWindowUnderMovingWindow = nullptr;
            ImGui::FindHoveredWindowEx(io.MousePos, false, &hoveredWindow,
                                       &hoveredWindowUnderMovingWindow);
            m_dragScrollWindow = FindDragScrollWindow(hoveredWindow);
            m_dragScrollLastMousePos = io.MousePos;
        }

        if (m_dragScrollWindow == nullptr) {
            return;
        }

        const auto mouseDelta = io.MousePos - m_dragScrollLastMousePos;
        m_dragScrollLastMousePos = io.MousePos;

        if (mouseDelta.x != 0.0f && m_dragScrollWindow->ScrollMax.x > 0.0f) {
            ImGui::SetScrollX(m_dragScrollWindow,
                              ImClamp(m_dragScrollWindow->Scroll.x - mouseDelta.x, 0.0f,
                                      m_dragScrollWindow->ScrollMax.x));
        }
        if (mouseDelta.y != 0.0f && m_dragScrollWindow->ScrollMax.y > 0.0f) {
            ImGui::SetScrollY(m_dragScrollWindow,
                              ImClamp(m_dragScrollWindow->Scroll.y - mouseDelta.y, 0.0f,
                                      m_dragScrollWindow->ScrollMax.y));
        }
    }

    bool ImGuiConsole::CheckMenuViewToggle(ImGuiKey key, bool& active) {
        if (ImGui::IsKeyPressed(key)) {
            active = !active;
        }

        return active;
    }

    std::string_view backend_name(AuroraBackend backend) {
        switch (backend) {
        default:
            return "Auto"sv;
        case BACKEND_D3D12:
            return "D3D12"sv;
        case BACKEND_D3D11:
            return "D3D11"sv;
        case BACKEND_METAL:
            return "Metal"sv;
        case BACKEND_VULKAN:
            return "Vulkan"sv;
        case BACKEND_OPENGL:
            return "OpenGL"sv;
        case BACKEND_OPENGLES:
            return "OpenGL ES"sv;
        case BACKEND_WEBGPU:
            return "WebGPU"sv;
        case BACKEND_NULL:
            return "Null"sv;
        }
    }

    std::string_view backend_id(AuroraBackend backend) {
        switch (backend) {
        default:
            return "auto"sv;
        case BACKEND_D3D12:
            return "d3d12"sv;
        case BACKEND_D3D11:
            return "d3d11"sv;
        case BACKEND_METAL:
            return "metal"sv;
        case BACKEND_VULKAN:
            return "vulkan"sv;
        case BACKEND_OPENGL:
            return "opengl"sv;
        case BACKEND_OPENGLES:
            return "opengles"sv;
        case BACKEND_WEBGPU:
            return "webgpu"sv;
        case BACKEND_NULL:
            return "null"sv;
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

    void ImGuiConsole::AddToast(std::string_view message, float duration) {
        m_toasts.emplace_back(std::string(message), duration);
    }

    void ImGuiConsole::ShowToasts() {
        if (m_toasts.empty()) {
            return;
        }
        auto& toast = m_toasts.front();
        const float dt = ImGui::GetIO().DeltaTime;
        toast.remain -= dt;
        toast.current += dt;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        const ImVec2 workPos = viewport->WorkPos;
        const ImVec2 workSize = viewport->WorkSize;
        constexpr float padding = 10.0f;
        const ImVec2 windowPos{workPos.x + workSize.x / 2, workPos.y + workSize.y - padding};
        ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, ImVec2{0.5f, 1.f});

        const float alpha = std::min({toast.remain, toast.current, 1.f});
        ImGui::SetNextWindowBgAlpha(alpha * 0.65f);
        ImVec4 textColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);
        textColor.w *= alpha;
        ImVec4 borderColor = ImGui::GetStyleColorVec4(ImGuiCol_Border);
        borderColor.w *= alpha;
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
        if (ImGui::Begin("Toast", nullptr,
                         ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
                             ImGuiWindowFlags_NoMove))
        {
            ImGuiStringViewText(toast.message);
        }
        ImGui::End();
        ImGui::PopStyleColor(2);

        if (toast.remain <= 0.f) {
            m_toasts.pop_front();
        }
    }

    void ImGuiConsole::ShowPipelineProgress() {
        const auto* stats = aurora_get_stats();
        const u32 queuedPipelines = stats->queuedPipelines;
        if (queuedPipelines == 0 || !getSettings().backend.showPipelineCompilation) {
            return;
        }
        const u32 createdPipelines = stats->createdPipelines;
        const u32 totalPipelines = queuedPipelines + createdPipelines;

        const auto* viewport = ImGui::GetMainViewport();
        const auto padding = viewport->WorkPos.y + 10.f;
        const auto halfWidth = viewport->GetWorkCenter().x;
        ImGui::SetNextWindowPos(ImVec2{halfWidth, padding}, ImGuiCond_Always, ImVec2{0.5f, 0.f});
        ImGui::SetNextWindowSize(ImVec2{halfWidth, 0.f}, ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.65f);
        ImGui::Begin("Pipelines", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing);
        const auto percent = static_cast<float>(createdPipelines) / static_cast<float>(totalPipelines);
        const auto progressStr = fmt::format("Processing pipelines: {} / {}", createdPipelines, totalPipelines);
        const auto textSize = ImGui::CalcTextSize(progressStr.data(), progressStr.data() + progressStr.size());
        ImGui::NewLine();
        ImGui::SameLine(ImGui::GetWindowWidth() / 2.f - textSize.x + textSize.x / 2.f);
        ImGuiStringViewText(progressStr);
        ImGui::ProgressBar(percent);
        ImGui::End();
    }
}
