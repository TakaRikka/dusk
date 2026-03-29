#include <array>
#include <numeric>
#include <string_view>
#include <chrono>
#include <thread>

#include "fmt/format.h"
#include "imgui.h"
#include "aurora/gfx.h"
#include <imgui_internal.h>

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

        ImVec2 halfFrame = ImVec2((frameHeight * 0.25f) * 0.5f, frameHeight * 0.5f);
        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(itemMin.x + halfFrame.x, itemMin.y + halfFrame.y),
            ImVec2(itemMax.x - halfFrame.x, itemMax.y),
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

    void ImGuiConsole::draw() {
        if (CheckMenuViewToggle(ImGuiKey_F1, m_isHidden)) {
            m_menuTools.afterDraw();
            return;
        }

        if (ImGui::BeginMainMenuBar()) {
            m_menuGame.draw();
            m_menuTools.draw();

            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 80.0f);
            ImGuiIO& io = ImGui::GetIO();
            ImGuiStringViewText(fmt::format(FMT_STRING("FPS: {:.2f}\n"), io.Framerate));

            ImGui::EndMainMenuBar();
        }

        m_menuTools.afterDraw();
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
