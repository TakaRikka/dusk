#include "dusk/audio/DuskAudioSystem.h"

#include <SDL3/SDL_init.h>
#include <array>

#include "JSystem/JAudio2/JASAiCtrl.h"
#include "JSystem/JAudio2/JASChannel.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "JSystem/JAudio2/JASDSPChannel.h"
#include "JSystem/JAudio2/JASHeapCtrl.h"

#include "DuskDsp.hpp"

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
        SDL_AUDIO_S16,
        1,
        32000,
    };
    PlaybackStream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &spec,
        &GetNewAudio,
        nullptr);
}

void DuskAudioInitialize() {
    InitSDL3Output();

    JASDsp::initBuffer();
    JASDSPChannel::initAll();

    JASPoolAllocObject_MultiThreaded<JASChannel>::newMemPool(0x48);

    SDL_ResumeAudioStreamDevice(PlaybackStream);
}

void SDLCALL GetNewAudio(
    void*,
    SDL_AudioStream*,
    int,
    int total_amount) {
    while (total_amount > 0) {
        const int rendered = RenderNewAudioFrame();
        total_amount -= rendered;
    }
}

int RenderNewAudioFrame() {
    JASCriticalSection section;
    const u32 countSubframes = JASDriver::getSubFrames();

    for (u32 i = 0; i < countSubframes; i++) {
        RenderAudioSubframe();
    }

    return static_cast<u16>(countSubframes) * DSP_SUBFRAME_SIZE;
}

void RenderAudioSubframe() {
    DspSubframe& subFrame = AllSubframeBuffers[0];

    JASDriver::updateDSP();
    DuskDspRender(subFrame);

    SDL_PutAudioStreamData(PlaybackStream, &subFrame, sizeof(subFrame));
}
