#ifndef DUSK_TIME_H
#define DUSK_TIME_H

#include <chrono>
#include <numeric>
#include <array>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <shellapi.h>
#endif

#include "dusk/logging.h"

constexpr auto DUSK_FRAME_PERIOD = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(1001.0 / 30000.0));

class Limiter {
  using delta_clock = std::chrono::high_resolution_clock;
  using duration_t = std::chrono::nanoseconds;

public:
  void Reset() { m_oldTime = delta_clock::now(); }

  void Sleep(duration_t targetFrameTime) {
    if (targetFrameTime.count() == 0) {
      return;
    }

    auto start = delta_clock::now();
    duration_t adjustedSleepTime = SleepTime(targetFrameTime);
    if (adjustedSleepTime.count() > 0) {
      NanoSleep(adjustedSleepTime);
      duration_t overslept = TimeSince(start) - adjustedSleepTime;
      if (overslept < duration_t{targetFrameTime}) {
        m_overheadTimes[m_overheadTimeIdx] = overslept;
        m_overheadTimeIdx = (m_overheadTimeIdx + 1) % m_overheadTimes.size();
      }
    }
    Reset();
  }

  duration_t SleepTime(duration_t targetFrameTime) {
    const auto sleepTime = duration_t{targetFrameTime} - TimeSince(m_oldTime);
    m_overhead = std::accumulate(m_overheadTimes.begin(), m_overheadTimes.end(), duration_t{}) / m_overheadTimes.size();
    if (sleepTime > m_overhead) {
      return sleepTime - m_overhead;
    }
    return duration_t{0};
  }

private:
  delta_clock::time_point m_oldTime;
  std::array<duration_t, 4> m_overheadTimes{};
  size_t m_overheadTimeIdx = 0;
  duration_t m_overhead = duration_t{0};

  duration_t TimeSince(delta_clock::time_point start) {
    return std::chrono::duration_cast<duration_t>(delta_clock::now() - start);
  }

#if _WIN32
  void NanoSleep(const duration_t duration) {
    static bool initialized = false;
    static double countPerNs;
    static size_t numSleeps = 0;

    // QueryPerformanceFrequency's result is constant, but calling it occasionally
    // appears to stabilize QueryPerformanceCounter. Without it, the game drifts
    // from 60hz to 144hz. (Cursed, but I suspect it's NVIDIA/G-SYNC related)
    if (!initialized || numSleeps++ % 1000 == 0) {
      LARGE_INTEGER freq;
      if (QueryPerformanceFrequency(&freq) == 0) {
        DuskLog.warn("QueryPerformanceFrequency failed: {}", GetLastError());
        return;
      }
      countPerNs = static_cast<double>(freq.QuadPart) / 1e9;
      initialized = true;
      numSleeps = 0;
    }

    LARGE_INTEGER start, current;
    QueryPerformanceCounter(&start);
    LONGLONG ticksToWait = static_cast<LONGLONG>(duration.count() * countPerNs);
    if (DWORD ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count(); ms > 1) {
      ::Sleep(ms - 1);
    }
    do {
      QueryPerformanceCounter(&current);
      _mm_pause(); // Yield CPU
    } while (current.QuadPart - start.QuadPart < ticksToWait);
  }
#else
  void NanoSleep(const duration_t duration) { std::this_thread::sleep_for(duration); }
#endif
};

#endif
