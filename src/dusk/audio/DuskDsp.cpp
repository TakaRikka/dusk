#include <ar.h>
#include <dolphin/os.h>

#include "DuskDsp.hpp"

#include <algorithm>
#include <cassert>
#include <span>

#include "Adpcm.hpp"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/endian.h"
#include "global.h"

using namespace dusk::audio;

ChannelAuxData dusk::audio::ChannelAux[DSP_CHANNELS] = {};

f32 dusk::audio::MasterVolume = 1.0f;
f32 dusk::audio::PrevMasterVolume = 1.0f;

/**
 * Validate that a DSP channel's format is actually something we know how to play.
 */
static bool ValidateChannelWaveFormat(const JASDsp::TChannel& channel) {
    if (channel.mSamplesPerBlock == AdpcmSampleCount && channel.mBytesPerBlock == Adpcm4FrameSize)
        return true;
    if (channel.mSamplesPerBlock == 1 && channel.mBytesPerBlock == 16)
        return true;
    /*
    if (channel.mSamplesPerBlock == AdpcmSampleCount && channel.mBytesPerBlock == Adpcm2FrameSize)
        return true;
    if (channel.mSamplesPerBlock == 1 && channel.mBytesPerBlock == 8)
        return true;
    */
    return false;
}

/**
 * Validate that a DSP channel is actually something we know how to play.
 */
static void ValidateChannel(const JASDsp::TChannel& channel) {
    if (!ValidateChannelWaveFormat(channel)) {
        CRASH(
            "Unable to handle channel format: %02x, %02x\n",
            channel.mSamplesPerBlock,
            channel.mBytesPerBlock);
    }
}

static u32 ConvertSamplesToDataLength(const JASDsp::TChannel& channel, u32 samples) {
    if (samples % channel.mSamplesPerBlock != 0) {
        // Ensure we round up.
        samples += channel.mSamplesPerBlock;
        //CRASH("Indivisible sample count: %d\n", samples);
    }

    return (samples / channel.mSamplesPerBlock) * BlockBytes(channel);
}

/**
 * Render the audio data contributed by a single DSP channel. Reads & decodes new input samples.
 */
static void RenderChannel(
    JASDsp::TChannel& channel,
    ChannelAuxData& channelAux,
    OutputSubframe& subframe);

/**
 * Converts a pitch value on a DSP channel to a sample rate.
 */
constexpr static int PitchToSampleRate(u16 value) {
    return static_cast<int>(static_cast<u64>(SampleRate) * value / 4096);
}

static void UpdateSampleRate(const JASDsp::TChannel& channel, ChannelAuxData& aux) {
    auto sampleRate = PitchToSampleRate(channel.mPitch);

    const SDL_AudioSpec spec = {
        SDL_AUDIO_S16,
        1,
        sampleRate
    };

    SDL_SetAudioStreamFormat(aux.resampleStream, &spec, nullptr);
    aux.prevPitch = channel.mPitch;
}

/**
 * Reset state for a DSP channel between independent playbacks.
 */
static void ResetChannel(JASDsp::TChannel& channel, ChannelAuxData& aux) {
    aux.resetCount += 1;

    channel.mSamplesLeft = channel.mEndSample - channel.mSamplePosition;

    aux.hist0 = 0;
    aux.hist1 = 0;

    SDL_ClearAudioStream(aux.resampleStream);
    UpdateSampleRate(channel, aux);

    for (auto& volume : aux.prevVolume) {
        volume = NAN;
    }

    channel.mResetFlag = false;
}

/**
 * Mix subframe data from src into dst.
 */
static void MixSubframe(DspSubframe& dst, const DspSubframe& src) {
    for (int i = 0; i < dst.size(); i++) {
        dst[i] += src[i];
    }
}

void dusk::audio::DspRender(OutputSubframe& subframe) {
    std::span channels(JASDsp::CH_BUF, DSP_CHANNELS);

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

        if (channel.mForcedStop) {
            channel.mIsFinished = true;
            continue;
        }

        if (channel.mWaveAramAddress == 0) {
            // I think these are oscillator channels? Not backed by audio.
            // No idea how to implement these yet, so skip them.
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

    for (auto& channel : subframe.channels) {
        ApplyVolume(channel, channel, PrevMasterVolume, MasterVolume);
    }
    PrevMasterVolume = MasterVolume;
}

/**
 * Actually decode samples from memory for the given audio channel.
 */
static void ReadSampleData(
    const JASDsp::TChannel& channel,
    ChannelAuxData& aux,
    const u8* data,
    size_t dataLength,
    s16* pcm,
    size_t pcmLength) {
    if (channel.mSamplesPerBlock == 1) {
        if (channel.mBytesPerBlock == 0x10) {
            // PCM16
            assert(reinterpret_cast<uintptr_t>(data) % 2 == 0 && "PCM data must be aligned");
            assert(dataLength % 2 == 0 && "Data length must be multiple of 2");
            assert(dataLength * 2 >= pcmLength && "Input too small!");

            auto srcPcm = reinterpret_cast<const BE(s16)*>(data);
            for (size_t i = 0; i < pcmLength; i++) {
                pcm[i] = srcPcm[i];
            }
        } else {
            CRASH("Unsupported format: PCM8");
        }
    } else {
        if (channel.mBytesPerBlock == 9) {
            Adpcm4ToPcm16(data, dataLength, pcm, pcmLength, aux.hist1, aux.hist0);
        } else {
            CRASH("Unsupported format: ADPCM2");
        }
    }
}

/**
 * Read a single *contiguous* chunk of sample data from a channel,
 * writes the samples to the channel's resampler stream.
 *
 * @returns Amount of samples actually read. Can be greater than the amount requested.
 */
static int ReadChannelSamplesChunk(
    JASDsp::TChannel& channel,
    ChannelAuxData& aux,
    int desiredSamples) {

    assert(desiredSamples >= 0);

    auto aramBase = static_cast<u8*>(ARGetStorageAddress()) + channel.mWaveAramAddress;

    // Streaming logic directly modifies mSamplesLeft.
    // So we use that as our tracking of where we are.
    auto curSamplePosition = channel.mEndSample - channel.mSamplesLeft;

    u32 skipSamples = curSamplePosition % channel.mSamplesPerBlock;
    if (skipSamples != 0) {
        // We need to start reading in the middle of a block. This can happen thanks to loops.
        // So we move back to the start of the block and keep track that those samples should
        // *not* be emitted.
        desiredSamples += static_cast<int>(skipSamples);
        curSamplePosition -= skipSamples;

        channel.mSamplesLeft += skipSamples;
        channel.mSamplePosition -= skipSamples;
    }

    // Pad desiredSamples so that we always leave the channel block-aligned.
    desiredSamples = ALIGN_NEXT(desiredSamples, channel.mSamplesPerBlock);

    assert(curSamplePosition % channel.mSamplesPerBlock == 0);
    auto dataPosition = ConvertSamplesToDataLength(channel, curSamplePosition);

    u32 renderSamples = std::min(channel.mSamplesLeft, static_cast<u32>(desiredSamples));

    int renderSize = static_cast<int>(sizeof(s16) * renderSamples);
    auto renderData = static_cast<s16*>(alloca(renderSize));
    memset(renderData, 0, renderSize);

    ReadSampleData(
        channel,
        aux,
        aramBase + dataPosition,
        ConvertSamplesToDataLength(channel, renderSamples),
        renderData,
        renderSamples);

    channel.mSamplesLeft -= renderSamples;
    channel.mSamplePosition += renderSamples;

    SDL_PutAudioStreamData(
        aux.resampleStream,
        renderData + skipSamples,
        static_cast<int>(renderSize - skipSamples * sizeof(u16)));

    assert(channel.mSamplePosition % channel.mSamplesPerBlock == 0 || channel.mSamplesLeft == 0);

    return static_cast<int>(renderSamples - skipSamples);
}

/**
 * Reads new audio channels from a DSP channel and writes them to the resampler stream.
 */
static void SDLCALL ReadChannelSamples(
    void *userdata,
    SDL_AudioStream*,
    int additional_amount,
    int) {

    if (additional_amount == 0) {
        return;
    }

    const auto index = static_cast<u32>(reinterpret_cast<uintptr_t>(userdata));
    auto& channel = JASDsp::CH_BUF[index];
    auto& aux = ChannelAux[index];

    if (channel.mSamplesLeft == 0 && !channel.mLoopFlag) {
        // May get called when we're out of data to read.
        // This is expected, as we need to drain the resampler channel before we mark the channel as finished.
        return;
    }

    auto samplesRead = ReadChannelSamplesChunk(channel, aux, additional_amount);
    additional_amount -= samplesRead;

    if (channel.mSamplesLeft == 0) {
        // Reached end of buffer.
        if (!channel.mLoopFlag) {
            return;
        }

        channel.mSamplesLeft = channel.mEndSample - channel.mLoopStartSample;
        channel.mSamplePosition = channel.mLoopStartSample;

        aux.hist1 = channel.mpPenult;
        aux.hist0 = channel.mpLast;
    }

    if (additional_amount >= 0) {
        ReadChannelSamplesChunk(channel, aux, additional_amount);
    }

    channel.mAramStreamPosition = channel.mWaveAramAddress
        + ConvertSamplesToDataLength(channel, channel.mSamplePosition);
}

/**
 * Get the expected BusConnect value needed to define the given output channel in a DSP channel.
 */
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

/**
 * For a DSP channel the JASDsp::OutputChannelConfig value targeting the given output channel.
 * Returns null if the DSP channel does not output to this output channel.
 */
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

struct VolumeValue {
    f32 Target;
    f32 Init;
};

/**
 * Get the volume that the given DSP channel should render to the given output channel at.
 */
static VolumeValue GetVolumeForOutputChannel(
    const JASDsp::TChannel& sourceChannel,
    OutputChannel outputChannel) {

    u16 volume;
    u16 initVolume;
    f32 panValue = 1;
    if (sourceChannel.mAutoMixerBeenSet) {
        volume = sourceChannel.mAutoMixerVolume;
        initVolume = sourceChannel.mAutoMixerInitVolume;

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
            return {0, 0};
        }

        volume = config->mTargetVolume;
        initVolume = config->mCurrentVolume;
    }

    // TODO: interpolate to avoid popping.
    f32 targetRatio = VolumeFromU16(volume);
    targetRatio *= panValue;

    f32 initRatio = VolumeFromU16(initVolume);
    initRatio *= panValue;

    return {targetRatio, initRatio};
}

/**
 * Given decoded & resampled input samples, render a DSP channel to a given output channel.
 */
static void RenderOutputChannel(
    const JASDsp::TChannel& sourceChannel,
    ChannelAuxData& aux,
    OutputChannel outputChannel,
    const std::span<f32> inputSamples,
    OutputSubframe& fullOutputSubframe) {

    auto& outputSubframe = fullOutputSubframe[outputChannel];
    assert(inputSamples.size() <= outputSubframe.size());

    auto volume = GetVolumeForOutputChannel(sourceChannel, outputChannel);

    f32 targetVolume = volume.Target;
    auto& prevVolume = aux.PrevVolume(outputChannel);
    if (std::isnan(prevVolume)) {
        // Initialize previous volume to new volume on first render.
        prevVolume = volume.Init;
    }

    if (prevVolume == 0 && targetVolume == 0) {
        return;
    }

    ApplyVolume(outputSubframe, inputSamples, prevVolume, targetVolume);
    prevVolume = targetVolume;
}

static void RenderChannel(
    JASDsp::TChannel& channel,
    ChannelAuxData& channelAux,
    OutputSubframe& subframe) {
    if (channel.mResetFlag) {
        ResetChannel(channel, channelAux);
    } else if (channelAux.prevPitch != channel.mPitch) {
        UpdateSampleRate(channel, channelAux);
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

    RenderOutputChannel(channel, channelAux, OutputChannel::LEFT, hasReadSamples, subframe);
    RenderOutputChannel(channel, channelAux,OutputChannel::RIGHT, hasReadSamples, subframe);
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

void dusk::audio::ApplyVolume(
    std::span<f32> dst,
    const std::span<f32> src,
    const f32 startVolume,
    const f32 endVolume) {
    assert(dst.size() >= src.size());

    if (startVolume == endVolume) {
        for (int i = 0; i < src.size(); i++) {
            dst[i] = src[i] * startVolume;
        }
    } else {
        const f32 step = (endVolume - startVolume) / static_cast<f32>(src.size());
        auto curVolume = startVolume;
        for (int i = 0; i < src.size(); i++) {
            dst[i] = src[i] * curVolume;
            curVolume += step;
        }
    }
}
