#include <ar.h>
#include <dolphin/os.h>

#include "DuskDsp.hpp"

#include <algorithm>
#include <cassert>
#include <span>

#include "Adpcm.hpp"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "global.h"

using namespace dusk::audio;

ChannelAuxData dusk::audio::ChannelAux[DSP_CHANNELS] = {};

static bool ValidateChannelWaveFormat(const JASDsp::TChannel& channel) {
    if (channel.mSamplesPerBlock == AdpcmSampleCount && channel.mBytesPerBlock == Adpcm4FrameSize)
        return true;
    /*
    if (channel.mSamplesPerBlock == AdpcmSampleCount && channel.mBytesPerBlock == Adpcm2FrameSize)
        return true;
    if (channel.mSamplesPerBlock == 1 && channel.mBytesPerBlock == 8)
        return true;
    if (channel.mSamplesPerBlock == 1 && channel.mBytesPerBlock == 16)
        return true;
    */
    return false;
}

static void ValidateChannel(const JASDsp::TChannel& channel) {
    if (!ValidateChannelWaveFormat(channel)) {
        CRASH(
            "Unable to handle channel format: %02x, %02x\n",
            channel.mSamplesPerBlock,
            channel.mBytesPerBlock);
    }
}

static u32 ConvertDataLengthToSamples(const JASDsp::TChannel& channel, u32 dataLen) {
    if (dataLen % channel.mBytesPerBlock != 0) {
        CRASH("Indivisible data length: %d\n", dataLen);
    }

    return (dataLen / channel.mBytesPerBlock) * channel.mSamplesPerBlock;
}

static u32 ConvertSamplesToDataLength(const JASDsp::TChannel& channel, u32 samples) {
    if (samples % channel.mSamplesPerBlock != 0) {
        // Ensure we round up.
        samples += channel.mSamplesPerBlock;
        //CRASH("Indivisible sample count: %d\n", samples);
    }

    return (samples / channel.mSamplesPerBlock) * channel.mBytesPerBlock;
}

static void RenderChannel(
    JASDsp::TChannel& channel,
    ChannelAuxData& channelAux,
    OutputSubframe& subframe);

static void ResetChannel(JASDsp::TChannel& channel, ChannelAuxData& aux) {
    channel.mSamplesLeft = channel.mEndSample - channel.mSamplePosition;

    const SDL_AudioSpec spec = {
        SDL_AUDIO_S16,
        1,
        static_cast<int>(static_cast<u64>(SampleRate) * channel.mPitch / 4096)
    };

    aux.hist0 = 0;
    aux.hist1 = 0;

    SDL_ClearAudioStream(aux.resampleStream);
    SDL_SetAudioStreamFormat(aux.resampleStream, &spec, nullptr);

    channel.mResetFlag = false;
}

static void MixSubframe(DspSubframe& dst, const DspSubframe& src) {
    for (int i = 0; i < dst.size(); i++) {
        dst[i] += src[i];
    }
}

void dusk::audio::DspRender(OutputSubframe& subframe) {
    // This cast half exists because my debugger sucks and this is an easy way to look at the data.
    auto& channels = *reinterpret_cast<std::array<JASDsp::TChannel, DSP_CHANNELS>*>(JASDsp::CH_BUF);

    for (int i = 0; i < channels.size(); i++) {
        auto& channel = channels[i];
        auto& channelAux = ChannelAux[i];

        if (!channel.mIsActive) {
            continue;
        }

        if (channel.mPauseFlag) {
            // Not really sure what the practical difference between pause and
            // deactivation is. Either avoids clearing state or allows the DSP to avoid popping?
            continue;
        }

        if (channel.mBytesPerBlock == 0) {
            // I think these are oscillator channels? Not backed by audio.
            channel.mIsFinished = true;
            continue;
        }

        ValidateChannel(channel);

        OutputSubframe channelSubframe = {};
        RenderChannel(channel, channelAux, channelSubframe);

        for (int o = 0; o < subframe.channels.size(); o++) {
            MixSubframe(subframe.channels[o], channelSubframe.channels[o]);
        }
    }
}

static void SDLCALL ReadChannelSamples(
    void *userdata,
    SDL_AudioStream *stream,
    int additional_amount,
    int) {

    const auto index = static_cast<u32>(reinterpret_cast<uintptr_t>(userdata));
    auto& channel = JASDsp::CH_BUF[index];
    auto& aux = ChannelAux[index];

    if (channel.mSamplesLeft == 0 && !channel.mLoopFlag) {
        // May get called when we're out of data to read.
        // This is expected, as we need to drain the resampler channel before we mark the channel as finished.
        return;
    }

    additional_amount = ALIGN_NEXT(additional_amount, channel.mSamplesPerBlock);

    int requestedSize = static_cast<int>(sizeof(s16) * additional_amount);
    auto requested = static_cast<s16*>(alloca(requestedSize));
    memset(requested, 0, requestedSize);

    auto aramBase = static_cast<u8*>(ARGetStorageAddress()) + channel.mWaveAramAddress;

    // Streaming logic directly modifies mSamplesLeft.
    // So we use that as our tracking of where we are.
    auto curSamplePosition = channel.mEndSample - channel.mSamplesLeft;
    auto dataPosition = ConvertSamplesToDataLength(channel, curSamplePosition);

    u32 renderSamples = std::min(channel.mSamplesLeft, static_cast<u32>(additional_amount));

    Adpcm4ToPcm16(
        aramBase + dataPosition,
        ConvertSamplesToDataLength(channel, renderSamples),
        requested,
        renderSamples,
        aux.hist1,
        aux.hist0);

    channel.mSamplesLeft -= renderSamples;
    channel.mSamplePosition += renderSamples;

    if (channel.mSamplesLeft == 0) {
        // Reached end of buffer.
        if (!channel.mLoopFlag) {
            return;
        }

        channel.mSamplesLeft = channel.mEndSample - channel.mLoopStartSample;
        channel.mSamplePosition = channel.mLoopStartSample;
        curSamplePosition = channel.mEndSample - channel.mSamplesLeft;
        dataPosition = ConvertSamplesToDataLength(channel, curSamplePosition);

        Adpcm4ToPcm16(
            aramBase + dataPosition,
            ConvertSamplesToDataLength(channel, additional_amount - renderSamples),
            requested + renderSamples,
            additional_amount - renderSamples,
            aux.hist1,
            aux.hist0);

        channel.mSamplesLeft -= (additional_amount - renderSamples);
        channel.mSamplePosition += (additional_amount - renderSamples);
    }

    channel.mAramStreamPosition = channel.mWaveAramAddress
        + ConvertSamplesToDataLength(channel, channel.mSamplePosition);

    SDL_PutAudioStreamData(stream, requested, requestedSize);
}

constexpr u16 GetBusConnect(const OutputChannel channel) {
    switch (channel) {
    // TODO: This is a guess for now.
    case OutputChannel::LEFT:
        return 0x0D00;
    case OutputChannel::RIGHT:
        return 0x0D60;
    default:
        CRASH("Invalid output channel!");
    }
}

static const JASDsp::OutputChannelConfig* GetOutputConfig(
    const JASDsp::TChannel& sourceChannel,
    OutputChannel channel) {

    auto busConnect = GetBusConnect(channel);
    for (const auto& mOutputChannel : sourceChannel.mOutputChannels) {
        auto config = &mOutputChannel;
        if (config->mBusConnect == busConnect) {
            return config;
        }
    }

    return nullptr;
}

static f32 GetVolumeForOutputChannel(
    const JASDsp::TChannel& sourceChannel,
    OutputChannel outputChannel) {

    u16 volume;
    f32 panValue = 1;
    if (sourceChannel.mAutoMixerBeenSet) {
        volume = sourceChannel.mAutoMixerVolume;

        auto autoMixerPan = static_cast<f32>(sourceChannel.mAutoMixerPanDolby >> 8) / 127;

        switch (outputChannel) {
            case OutputChannel::LEFT:
                panValue = 1 - autoMixerPan;
                break;
            case OutputChannel::RIGHT:
                panValue = autoMixerPan;
                break;
            default:
                CRASH("Unhandled output channel: OutputChannel");
        }

    } else {
        auto config = GetOutputConfig(sourceChannel, outputChannel);
        if (config == nullptr) {
            return 0;
        }

        volume = config->mTargetVolume;
    }

    // TODO: interpolate to avoid popping.
    f32 ratio = static_cast<f32>(volume) / static_cast<f32>(JASDriver::getChannelLevel_dsp());
    ratio *= panValue;

    return ratio;
}

static void RenderOutputChannel(
    const JASDsp::TChannel& sourceChannel,
    OutputChannel outputChannel,
    const std::span<f32> inputSamples,
    OutputSubframe& fullOutputSubframe) {

    auto& outputSubframe = fullOutputSubframe[outputChannel];
    assert(inputSamples.size() <= outputSubframe.size());

    auto volume = GetVolumeForOutputChannel(sourceChannel, outputChannel);
    if (volume == 0) {
        return;
    }

    for (int i = 0; i < inputSamples.size(); i++) {
        outputSubframe[i] = inputSamples[i] * volume;
    }
}

static void RenderChannel(
    JASDsp::TChannel& channel,
    ChannelAuxData& channelAux,
    OutputSubframe& subframe) {
    if (channel.mResetFlag) {
        ResetChannel(channel, channelAux);
    }

    DspSubframe audioLoadBuffer = {};

    int wantRead = sizeof(audioLoadBuffer);
    auto read = SDL_GetAudioStreamData(
        channelAux.resampleStream,
        &audioLoadBuffer,
        wantRead);

    if (read < wantRead) {
        channel.mIsFinished = true;
    }

    auto hasReadSamples = std::span(audioLoadBuffer).subspan(0, wantRead / sizeof(f32));

    static_assert(OutputSubframe::NUM_CHANNELS == 2, "Keep RenderChannel in sync!");

    RenderOutputChannel(channel, OutputChannel::LEFT, hasReadSamples, subframe);
    RenderOutputChannel(channel, OutputChannel::RIGHT, hasReadSamples, subframe);
}

void dusk::audio::DspInit() {
    constexpr SDL_AudioSpec srcSpec = {
        SDL_AUDIO_S16,
        1,
        SampleRate
    };
    constexpr SDL_AudioSpec dstSpec = {
        SDL_AUDIO_F32,
        1,
        SampleRate
    };

    for (u32 i = 0; i < DSP_CHANNELS; i++) {
        auto& aux = ChannelAux[i];
        aux.resampleStream = SDL_CreateAudioStream(&srcSpec, &dstSpec);

        SDL_SetAudioStreamGetCallback(
            aux.resampleStream,
            ReadChannelSamples,
            reinterpret_cast<void*>(static_cast<uintptr_t>(i)));
    }
}
