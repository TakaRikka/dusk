#include "dusk/imgui.h"
#include "imgui/imgui.hpp"

#include <array>
#include <chrono>
#include <imgui.h>
#include <numeric>
#include <thread>

#if _WIN32
#include "Windows.h"
#endif

static bool ImguiHidden = false;

void imgui_main(const AuroraInfo *info)
{
    if (ImGui::IsKeyPressed(ImGuiKey_F1)) {
        ImguiHidden = !ImguiHidden;
    }

    if (ImguiHidden) {
        return;
    }

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(MenuView)) {
            ImGui::MenuItem("Hide UI", "F1", &ImguiHidden);
            ImGui::EndMenu();
        }

        DuskImguiDebugOverlay(info);
        DuskImguiProcesses();

        ImGui::EndMainMenuBar();
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
