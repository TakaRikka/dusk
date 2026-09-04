#include "dusk/audio/DuskAudioSystem.h"

#include <SDL3/SDL_init.h>
#include <array>
#include <cassert>
#include <span>

#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASChannel.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDSPChannel.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"

#include "DuskDsp.hpp"
#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "tracy/Tracy.hpp"

using namespace dusk::audio;

static OutputSubframe OutBuffer;
static std::array<f32, DSP_SUBFRAME_SIZE * OutputSubframe::NUM_CHANNELS> OutInterleaveBufferFull;

static SDL_AudioStream* PlaybackStream;

/**
 * SDL audiostream callback to trigger rendering of new audio data.
 */
static void SDLCALL GetNewAudio(
    void*,
    SDL_AudioStream*,
    int needed,
    int);

/**
 * Render an entire new frame of audio and output it to SDL3.
 * Note: "audio frames" are unrelated to video frames.
 * @return Amount of audio samples rendered in bytes.
 */
static int RenderNewAudioFrame();

/**
 * Render an audio subframe and output it to SDL3.
 */
static int RenderAudioSubframe();

static size_t GetChannelCountForOutputMode(dusk::AudioOutputMode config) {
    switch (config) {
        default:
        case dusk::AudioOutputMode::StereoSpeakers:
        case dusk::AudioOutputMode::StereoHeadphones:
            return 2;
        case dusk::AudioOutputMode::Surround6ch:
            return 6;
        case dusk::AudioOutputMode::Surround8ch:
            return 8;
    }
}

static bool InitSDL3Output() {
    const auto speakerConfig = dusk::getSettings().audio.outputMode.getValue();
    const auto desiredChannelCount = GetChannelCountForOutputMode(speakerConfig);
    const bool hrtf = speakerConfig == dusk::AudioOutputMode::StereoHeadphones;

    if (PlaybackStream && desiredChannelCount == OutChannelCount) {
        JASCriticalSection section;
        EnableHrtf = hrtf;
        return false;
    }

    if (PlaybackStream) {
        SDL_PauseAudioStreamDevice(PlaybackStream);
        SDL_DestroyAudioStream(PlaybackStream);
    } else {
        SDL_Init(SDL_INIT_AUDIO);
    }

    const SDL_AudioSpec spec = {
        SDL_AUDIO_F32,
        static_cast<int>(desiredChannelCount),
        SampleRate,
    };
    SDL_AudioStream* newStream =
        SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, &GetNewAudio, nullptr);

    {
        JASCriticalSection section;
        EnableHrtf = hrtf;
        OutChannelCount = desiredChannelCount;
        PlaybackStream = newStream;
    }

    return true;
}

void dusk::audio::Initialize() {
    InitSDL3Output();
    DspInit();

    JASDsp::initBuffer();
    JASDSPChannel::initAll();

    JASPoolAllocObject_MultiThreaded<JASChannel>::newMemPool(0x48);

    SDL_ResumeAudioStreamDevice(PlaybackStream);
}

void dusk::audio::Reinitialize() {
    if (InitSDL3Output()) {
        SDL_ResumeAudioStreamDevice(PlaybackStream);
    }
}

void dusk::audio::SetMasterVolume(const f32 value) {
    JASCriticalSection section;

    MasterVolume = value;
}

void dusk::audio::SetPaused(const bool paused) {
    if (paused) {
        SDL_PauseAudioStreamDevice(PlaybackStream);
    } else {
        SDL_ResumeAudioStreamDevice(PlaybackStream);
    }
}

void dusk::audio::SetEnableReverb(const bool value) {
    JASCriticalSection section;

    EnableReverb = value;
}

#ifdef TRACY_ENABLE
static auto FrameName = "GetNewAudio";
#endif

void SDLCALL GetNewAudio(
    void*,
    SDL_AudioStream*,
    int needed,
    int) {
    FrameMarkStart(FrameName);
    while (needed > 0) {
        const int rendered = RenderNewAudioFrame();
        needed -= rendered;
    }
    FrameMarkEnd(FrameName);
}

int RenderNewAudioFrame() {
    ZoneScoped;
    JASCriticalSection section;
    const u32 countSubframes = JASDriver::getSubFrames();
    int bytesWritten = 0;

    JASAudioThread::setDSPSyncCount(countSubframes);

    for (u32 i = 0; i < countSubframes; i++) {
        bytesWritten += RenderAudioSubframe();

        JASAudioThread::snIntCount -= 1;
    }

    return bytesWritten;
}

static void InterleaveOutputData(const OutputSubframe& data, std::span<f32> target) {
    assert(target.size() >= data.channels[0].size() * OutChannelCount);

    size_t outPos = 0;
    for (size_t inPos = 0; inPos < data.channels[0].size(); inPos++) {
        for (size_t channelIdx = 0; channelIdx < OutChannelCount; channelIdx++) {
            target[outPos++] = data.channels[channelIdx][inPos];
        }
    }
}

int RenderAudioSubframe() {
    ZoneScoped;
    OutBuffer = {};

    JASDriver::updateDSP();
    DspRender(OutBuffer);

    std::span<f32> OutInterleaveBuffer{OutInterleaveBufferFull.data(), static_cast<size_t>(DSP_SUBFRAME_SIZE * OutChannelCount)};
    InterleaveOutputData(OutBuffer, OutInterleaveBuffer);

    if (JASDriver::extMixCallback != nullptr && JASDriver::sMixMode == MIX_MODE_INTERLEAVE) {
        // NOTE: In the real game, this gets called on the entire audio frame, rather than the subframe.
        // That's probably more efficient, but I didn't wanna change the code to calculate the
        // entire audio buffers at once.
        // This is only used for the movie player, and it seems to work fine with the smaller calls.
        const auto mixData = JASDriver::extMixCallback(DSP_SUBFRAME_SIZE);
        if (mixData) {
            for (int i = 0; i < DSP_SUBFRAME_SIZE; i++) {
                const auto oi = i * OutChannelCount;
                OutInterleaveBuffer[oi] += static_cast<f32>(mixData[i * 2]) / 32767.0f;
                OutInterleaveBuffer[oi + 1] += static_cast<f32>(mixData[i * 2 + 1]) / 32767.0f;
            }
        }
    }

    auto bytesToWrite = OutInterleaveBuffer.size_bytes();
    SDL_PutAudioStreamData(PlaybackStream, OutInterleaveBuffer.data(), bytesToWrite);
    return bytesToWrite;
}

u32 dusk::audio::GetResetCount(int channelIdx) {
    return ChannelAux[channelIdx].resetCount;
}

f32 dusk::audio::VolumeFromU16(u16 value) {
    return static_cast<f32>(value) / static_cast<f32>(JASDriver::getChannelLevel_dsp());
}
