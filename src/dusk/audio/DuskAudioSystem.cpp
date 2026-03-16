#include "dusk/audio/DuskAudioSystem.h"

#include <SDL3/SDL_init.h>
#include <array>
#include <fstream>
#include <ios>

#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASChannel.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDSPChannel.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"

#include "DuskDsp.hpp"
#include "JSystem/JAudio2/JASAudioThread.h"

using namespace dusk::audio;

static DspSubframe AllSubframeBuffers[DSP_OUTPUT_CHANNELS];

static SDL_AudioStream* PlaybackStream;

static void SDLCALL GetNewAudio(
    void *userdata,
    SDL_AudioStream *stream,
    int additional_amount,
    int total_amount);

static int RenderNewAudioFrame();
static void RenderAudioSubframe();

static void InitSDL3Output() {
    SDL_Init(SDL_INIT_AUDIO);

    constexpr SDL_AudioSpec spec = {
        SDL_AUDIO_F32,
        1,
        SampleRate,
    };
    PlaybackStream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &spec,
        &GetNewAudio,
        nullptr);
}

void dusk::audio::Initialize() {
    InitSDL3Output();
    DspInit();

    JASDsp::initBuffer();
    JASDSPChannel::initAll();

    JASPoolAllocObject_MultiThreaded<JASChannel>::newMemPool(0x48);

    SDL_ResumeAudioStreamDevice(PlaybackStream);
}

void SDLCALL GetNewAudio(
    void*,
    SDL_AudioStream*,
    int needed,
    int) {
    while (needed > 0) {
        const int rendered = RenderNewAudioFrame();
        needed -= rendered;
    }
}

static std::ofstream outRaw("guh.raw", std::ios_base::out | std::ios_base::binary);
static std::ofstream outRawF32("guh.f32.raw", std::ios_base::out | std::ios_base::binary);

int RenderNewAudioFrame() {
    JASCriticalSection section;
    const u32 countSubframes = JASDriver::getSubFrames();

    JASAudioThread::setDSPSyncCount(countSubframes);

    for (u32 i = 0; i < countSubframes; i++) {
        RenderAudioSubframe();

        JASAudioThread::snIntCount -= 1;
    }

    outRaw.flush();

    return static_cast<u16>(countSubframes) * DSP_SUBFRAME_SIZE;
}

void RenderAudioSubframe() {
    DspSubframe& subFrame = AllSubframeBuffers[0];

    JASDriver::updateDSP();
    DspRender(subFrame);

    std::array<f32, DSP_SUBFRAME_SIZE> guh = {};
    for (int i = 0; i < DSP_SUBFRAME_SIZE; i++) {
        guh[i] = (f32)subFrame[i] / (f32)0x7FFF;
    }

    outRaw.write((const char*)subFrame.data(), sizeof(subFrame));
    outRawF32.write((const char*)guh.data(), sizeof(guh));

    SDL_PutAudioStreamData(PlaybackStream, guh.data(), sizeof(guh));
    // SDL_PutAudioStreamData(PlaybackStream, subFrame.data(), sizeof(subFrame));
}
