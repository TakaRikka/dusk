#ifndef DUSK_TIME_H
#define DUSK_TIME_H

#include <array>
#include <numeric>

#include "SDL3/SDL_timer.h"

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <shellapi.h>
#include <intrin.h>
#endif

class Limiter {
public:
  using duration_t = Uint64;

  void Reset() { m_oldTime = SDL_GetTicksNS(); }

  void Sleep(duration_t targetFrameTime) {
    if (targetFrameTime == 0) {
      return;
    }

    const Uint64 start = SDL_GetTicksNS();
    duration_t adjustedSleepTime = SleepTime(targetFrameTime);
    if (adjustedSleepTime > 0) {
      NanoSleep(adjustedSleepTime);
      const duration_t elapsed = TimeSince(start);
      const duration_t overslept = elapsed > adjustedSleepTime ? elapsed - adjustedSleepTime : 0;
      if (overslept < targetFrameTime) {
        m_overheadTimes[m_overheadTimeIdx] = overslept;
        m_overheadTimeIdx = (m_overheadTimeIdx + 1) % m_overheadTimes.size();
      }
    }
    Reset();
  }

  duration_t SleepTime(duration_t targetFrameTime) {
    const duration_t elapsed = TimeSince(m_oldTime);
    const duration_t sleepTime = elapsed < targetFrameTime ? targetFrameTime - elapsed : 0;
    m_overhead = std::accumulate(m_overheadTimes.begin(), m_overheadTimes.end(), duration_t{0}) /
                 m_overheadTimes.size();
    if (sleepTime > m_overhead) {
      return sleepTime - m_overhead;
    }
    return 0;
  }

private:
  Uint64 m_oldTime = 0;
  std::array<duration_t, 4> m_overheadTimes{};
  size_t m_overheadTimeIdx = 0;
  duration_t m_overhead = 0;

  duration_t TimeSince(Uint64 start) const { return SDL_GetTicksNS() - start; }

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
    const LONGLONG ticksToWait = static_cast<LONGLONG>(duration * countPerNs);
    const Uint64 ms = duration / 1'000'000ULL;
    if (ms > 1) {
      ::Sleep(static_cast<DWORD>(ms - 1));
    }
    do {
      QueryPerformanceCounter(&current);
#if defined(_M_ARM64) || defined(_M_ARM)
      __yield();
#else
      _mm_pause();
#endif
    } while (current.QuadPart - start.QuadPart < ticksToWait);
  }
#else
  void NanoSleep(const duration_t duration) { SDL_DelayPrecise(duration); }
#endif
};

#endif
