#include <array>
#include <numeric>
#include <string_view>
#include <chrono>
#include <thread>

#include "fmt/format.h"
#include "imgui.h"
#include "aurora/gfx.h"

#include "ImGuiConsole.hpp"

#include "JSystem/JUtility/JUTGamePad.h"

#if _WIN32
#include "Windows.h"
#endif

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace dusk {
    void ImGuiStringViewText(std::string_view text) {
        // begin()/end() do not work on MSVC
        ImGui::TextUnformatted(text.data(), text.data() + text.size());
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
        constexpr float padding = 10.0f;
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

    ImGuiConsole g_imguiConsole;

    ImGuiConsole::ImGuiConsole() {}

    void ImGuiConsole::draw() {
        if (CheckMenuViewToggle(ImGuiKey_F1, m_isHidden)) {
            return;
        }

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Hide UI", "F1", &m_isHidden);
                ImGui::MenuItem("Process Management", "F2", &m_showProcessManagement);
                ImGui::MenuItem("Debug Overlay", "F3", &m_showDebugOverlay);
                ImGui::MenuItem("Heaps", "F4", &m_showHeapOverlay);
                ImGui::MenuItem("Stub Log", "F5", &m_showStubLog);
                ImGui::MenuItem("Camera", "F6", &m_showCameraOverlay);
                ImGui::MenuItem("Input Viewer", nullptr, &m_showInputViewer);
                ImGui::MenuItem("Map Loader", nullptr, &m_showMapLoader);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Game")) {
                if (ImGui::Selectable("Reset")) {
                    JUTGamePad::C3ButtonReset::sResetSwitchPushing = true;
                }
                ImGui::EndMenu();
            }

            ShowDebugOverlay();
            ShowCameraOverlay();
            ShowProcessManager();
            ShowHeapOverlay();
            ShowInputViewer();
            ShowStubLog();
            ShowMapLoader();

            DuskDebugPad(); // temporary, remove later

            ImGui::EndMainMenuBar();
        }
    }

    void ImGuiConsole::ShowDebugOverlay() {
        if (!CheckMenuViewToggle(ImGuiKey_F3, m_showDebugOverlay)) {
            return;
        }

        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration       |
                                       ImGuiWindowFlags_AlwaysAutoResize   |
                                       ImGuiWindowFlags_NoFocusOnAppearing |
                                       ImGuiWindowFlags_NoNav;
        if (m_debugOverlayCorner != -1) {
            SetOverlayWindowLocation(m_debugOverlayCorner);
            windowFlags |= ImGuiWindowFlags_NoMove;
        }

        ImGui::SetNextWindowBgAlpha(0.65f);
        if (ImGui::Begin("Debug Overlay", nullptr, windowFlags)) {
            bool hasPrevious = false;
            if (hasPrevious) {
                ImGui::Separator();
            }
            hasPrevious = true;

            ImGuiStringViewText(fmt::format(FMT_STRING("FPS: {:.2f}\n"), io.Framerate));

            if (hasPrevious) {
                ImGui::Separator();
            }
            hasPrevious = true;

            ImGuiStringViewText(fmt::format(FMT_STRING("Backend: {}\n"), backend_name(aurora_get_backend())));

            if (hasPrevious) {
                ImGui::Separator();
            }
            hasPrevious = true;

            AuroraStats const* stats = aurora_get_stats();

            ImGuiStringViewText(
                fmt::format(FMT_STRING("Queued pipelines:  {}\n"), stats->queuedPipelines));
            ImGuiStringViewText(
                fmt::format(FMT_STRING("Done pipelines:    {}\n"), stats->createdPipelines));
            ImGuiStringViewText(
                fmt::format(FMT_STRING("Draw call count:   {}\n"), stats->drawCallCount));
            ImGuiStringViewText(fmt::format(FMT_STRING("Merged draw calls: {}\n"),
                stats->mergedDrawCallCount));
            ImGuiStringViewText(fmt::format(FMT_STRING("Vertex size:       {}\n"),
                BytesToString(stats->lastVertSize)));
            ImGuiStringViewText(fmt::format(FMT_STRING("Uniform size:      {}\n"),
                BytesToString(stats->lastUniformSize)));
            ImGuiStringViewText(fmt::format(FMT_STRING("Index size:        {}\n"),
                BytesToString(stats->lastIndexSize)));
            ImGuiStringViewText(fmt::format(FMT_STRING("Storage size:      {}\n"),
                BytesToString(stats->lastStorageSize)));
            ImGuiStringViewText(fmt::format(
                FMT_STRING("Total:             {}\n"),
                BytesToString(stats->lastVertSize + stats->lastUniformSize +
                    stats->lastIndexSize + stats->lastStorageSize)));
        }
        ImGui::End();
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
}

class Limiter
{
    using delta_clock = std::chrono::high_resolution_clock;
    using duration_t = std::chrono::nanoseconds;

public:
    void Reset()
    {
        m_oldTime = delta_clock::now();
    }

    void Sleep(duration_t targetFrameTime)
    {
        if (targetFrameTime.count() == 0)
        {
            return;
        }

        auto start = delta_clock::now();
        duration_t adjustedSleepTime = SleepTime(targetFrameTime);
        if (adjustedSleepTime.count() > 0)
        {
            NanoSleep(adjustedSleepTime);
            duration_t overslept = TimeSince(start) - adjustedSleepTime;
            if (overslept < duration_t{ targetFrameTime })
            {
                m_overheadTimes[m_overheadTimeIdx] = overslept;
                m_overheadTimeIdx = (m_overheadTimeIdx + 1) % m_overheadTimes.size();
            }
        }
        Reset();
    }

    duration_t SleepTime(duration_t targetFrameTime)
    {
        const auto sleepTime = duration_t{ targetFrameTime } - TimeSince(m_oldTime);
        m_overhead = std::accumulate(m_overheadTimes.begin(), m_overheadTimes.end(), duration_t{}) /
            m_overheadTimes.size();
        if (sleepTime > m_overhead)
        {
            return sleepTime - m_overhead;
        }
        return duration_t{ 0 };
    }

private:
    delta_clock::time_point m_oldTime;
    std::array<duration_t, 4> m_overheadTimes{};
    size_t m_overheadTimeIdx = 0;
    duration_t m_overhead = duration_t{ 0 };

    duration_t TimeSince(delta_clock::time_point start)
    {
        return std::chrono::duration_cast<duration_t>(delta_clock::now() - start);
    }

#if _WIN32
    bool m_initialized;
    double m_countPerNs;

    void NanoSleep(const duration_t duration)
    {
        if (!m_initialized)
        {
            LARGE_INTEGER freq;
            QueryPerformanceFrequency(&freq);
            m_countPerNs = static_cast<double>(freq.QuadPart) / 1000000000.0;
            m_initialized = true;
        }

        DWORD ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        auto tickCount =
            static_cast<LONGLONG>(static_cast<double>(duration.count()) * m_countPerNs);
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        if (ms > 10)
        {
            // Adjust for Sleep overhead
            ::Sleep(ms - 10);
        }
        auto end = count.QuadPart + tickCount;
        do
        {
            QueryPerformanceCounter(&count);
        } while (count.QuadPart < end);
    }
#else
    void NanoSleep(const duration_t duration)
    {
        std::this_thread::sleep_for(duration);
    }
#endif
};

static Limiter g_frameLimiter;
void frame_limiter()
{
    g_frameLimiter.Sleep(
        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds{ 1 }) / 60);
}
