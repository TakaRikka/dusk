#include "dusk/imgui.h"
#include "imgui/imgui.hpp"

#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <fmt/format.h>
#include <imgui.h>
#include <numeric>
#include <thread>

#if _WIN32
#include "Windows.h"
#endif

static bool m_frameRate = true;
static bool m_pipelineInfo = true;
static bool m_graphicsBackend = true;
static int m_debugOverlayCorner = 0; // top-left

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace aurora::gfx
{
extern std::atomic_uint32_t queuedPipelines;
extern std::atomic_uint32_t createdPipelines;

extern size_t g_drawCallCount;
extern size_t g_mergedDrawCallCount;
extern size_t g_lastVertSize;
extern size_t g_lastUniformSize;
extern size_t g_lastIndexSize;
extern size_t g_lastStorageSize;
} // namespace aurora::gfx

static void SetOverlayWindowLocation(int corner)
{
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
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

static void ImGuiStringViewText(std::string_view text)
{
    // begin()/end() do not work on MSVC
    ImGui::TextUnformatted(text.data(), text.data() + text.size());
}

static std::string BytesToString(size_t bytes)
{
    constexpr std::array suffixes{"B"sv, "KB"sv, "MB"sv, "GB"sv, "TB"sv, "PB"sv, "EB"sv};
    uint32_t s = 0;
    auto count = static_cast<double>(bytes);
    while (count >= 1024.0 && s < 7)
    {
        s++;
        count /= 1024.0;
    }
    if (count - floor(count) == 0.0)
    {
        return fmt::format(FMT_STRING("{}{}"), static_cast<size_t>(count), suffixes[s]);
    }
    return fmt::format(FMT_STRING("{:.1f}{}"), count, suffixes[s]);
}

static void ShowProcesses();

void imgui_main(const AuroraInfo *info)
{

    ImGuiIO &io = ImGui::GetIO();
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration |
                                   ImGuiWindowFlags_AlwaysAutoResize |
                                   ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (m_debugOverlayCorner != -1)
    {
        SetOverlayWindowLocation(m_debugOverlayCorner);
        windowFlags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.65f);
    if (ImGui::Begin("Debug Overlay", nullptr, windowFlags))
    {
        bool hasPrevious = false;
        if (m_frameRate)
        {
            if (hasPrevious)
            {
                ImGui::Separator();
            }
            hasPrevious = true;

            ImGuiStringViewText(fmt::format(FMT_STRING("FPS: {:.1f}\n"), io.Framerate));
        }
        if (m_graphicsBackend)
        {
            if (hasPrevious)
            {
                ImGui::Separator();
            }
            hasPrevious = true;

            std::string_view backendString = "Unknown"sv;
            switch (info->backend)
            {
            case BACKEND_D3D12:
                backendString = "D3D12"sv;
                break;
            case BACKEND_METAL:
                backendString = "Metal"sv;
                break;
            case BACKEND_VULKAN:
                backendString = "Vulkan"sv;
                break;
            case BACKEND_OPENGL:
                backendString = "OpenGL"sv;
                break;
            case BACKEND_OPENGLES:
                backendString = "OpenGL ES"sv;
                break;
            case BACKEND_WEBGPU:
                backendString = "WebGPU"sv;
                break;
            case BACKEND_NULL:
                backendString = "Null"sv;
                break;
            }
            ImGuiStringViewText(fmt::format(FMT_STRING("Backend: {}\n"), backendString));
        }
        if (m_pipelineInfo)
        {
            if (hasPrevious)
            {
                ImGui::Separator();
            }
            hasPrevious = true;

            ImGuiStringViewText(
                fmt::format(FMT_STRING("Queued pipelines:  {}\n"), aurora::gfx::queuedPipelines.load()));
            ImGuiStringViewText(
                fmt::format(FMT_STRING("Done pipelines:    {}\n"), aurora::gfx::createdPipelines.load()));
            ImGuiStringViewText(
                fmt::format(FMT_STRING("Draw call count:   {}\n"), aurora::gfx::g_drawCallCount));
            ImGuiStringViewText(fmt::format(FMT_STRING("Merged draw calls: {}\n"),
                                            aurora::gfx::g_mergedDrawCallCount));
            ImGuiStringViewText(fmt::format(FMT_STRING("Vertex size:       {}\n"),
                                            BytesToString(aurora::gfx::g_lastVertSize)));
            ImGuiStringViewText(fmt::format(FMT_STRING("Uniform size:      {}\n"),
                                            BytesToString(aurora::gfx::g_lastUniformSize)));
            ImGuiStringViewText(fmt::format(FMT_STRING("Index size:        {}\n"),
                                            BytesToString(aurora::gfx::g_lastIndexSize)));
            ImGuiStringViewText(fmt::format(FMT_STRING("Storage size:      {}\n"),
                                            BytesToString(aurora::gfx::g_lastStorageSize)));
            ImGuiStringViewText(fmt::format(
                FMT_STRING("Total:             {}\n"),
                BytesToString(aurora::gfx::g_lastVertSize + aurora::gfx::g_lastUniformSize +
                              aurora::gfx::g_lastIndexSize + aurora::gfx::g_lastStorageSize)));
        }
    }
    ImGui::End();

    DuskImguiProcesses();
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
            if (overslept < duration_t{targetFrameTime})
            {
                m_overheadTimes[m_overheadTimeIdx] = overslept;
                m_overheadTimeIdx = (m_overheadTimeIdx + 1) % m_overheadTimes.size();
            }
        }
        Reset();
    }

    duration_t SleepTime(duration_t targetFrameTime)
    {
        const auto sleepTime = duration_t{targetFrameTime} - TimeSince(m_oldTime);
        m_overhead = std::accumulate(m_overheadTimes.begin(), m_overheadTimes.end(), duration_t{}) /
                     m_overheadTimes.size();
        if (sleepTime > m_overhead)
        {
            return sleepTime - m_overhead;
        }
        return duration_t{0};
    }

  private:
    delta_clock::time_point m_oldTime;
    std::array<duration_t, 4> m_overheadTimes{};
    size_t m_overheadTimeIdx = 0;
    duration_t m_overhead = duration_t{0};

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
        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds{1}) / 60);
}
