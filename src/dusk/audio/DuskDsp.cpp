#include <ar.h>
#include <dolphin/os.h>

#include "DuskDsp.hpp"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <span>

#include "Adpcm.hpp"
#include "freeverb/revmodel.hpp"
#include "JSystem/JAudio2/JASDriverIF.h"
#include "dusk/audio/DuskAudioSystem.h"
#include "dusk/endian.h"
#include "global.h"
#include "tracy/Tracy.hpp"

using namespace dusk::audio;

ChannelAuxData dusk::audio::ChannelAux[DSP_CHANNELS] = {};

static revmodel SharedReverb;
static bool ReverbHasTail = false;

static bool sDumpWasActive = false;
static FILE* sChannelDumpFiles[DSP_CHANNELS] = {};

static void OpenChannelDumpFiles() {
    char name[32];
    for (int i = 0; i < DSP_CHANNELS; i++) {
        snprintf(name, sizeof(name), "channel_%02d.raw", i);
        sChannelDumpFiles[i] = fopen(name, "wb");
    }
}

static void CloseChannelDumpFiles() {
    for (int i = 0; i < DSP_CHANNELS; i++) {
        if (sChannelDumpFiles[i]) {
            fclose(sChannelDumpFiles[i]);
            sChannelDumpFiles[i] = nullptr;
        }
    }
}

f32 dusk::audio::MasterVolume = 1.0f;
f32 dusk::audio::PrevMasterVolume = 1.0f;
bool dusk::audio::EnableReverb = true;
bool dusk::audio::DumpAudio = false;

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

/**
 * Reset state for a DSP channel between independent playbacks.
 */
static void ResetChannel(JASDsp::TChannel& channel, ChannelAuxData& aux) {
    aux.resetCount += 1;

    channel.mSamplesLeft = channel.mEndSample - channel.mSamplePosition;

    aux.hist0 = 0;
    aux.hist1 = 0;

    aux.decodeBufCount = 0;
    aux.resamplePos = 0.0;
    aux.resamplePrev = 0;

    aux.prev_lp_out = 0.0f;
    aux.prev_lp_in = 0.0f;

    aux.biq_in1 = 0.0f;
    aux.biq_in2 = 0.0f;
    aux.biq_out1 = 0.0f;
    aux.biq_out2 = 0.0f;

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
    ZoneScoped;
    if (DumpAudio != sDumpWasActive) {
        sDumpWasActive = DumpAudio;
        if (DumpAudio) {
            OpenChannelDumpFiles();
        } else {
            CloseChannelDumpFiles();
        }
    }

    std::span channels(JASDsp::CH_BUF, DSP_CHANNELS);

    DspSubframe reverbInputL = {};
    DspSubframe reverbInputR = {};
    bool anyReverbInput = false;

    for (int i = 0; i < channels.size(); i++) {
        auto& channel = channels[i];
        auto& channelAux = ChannelAux[i];

        if (!channel.mIsActive) {
            continue;
        }
        else if (channel.mPauseFlag) {
            // Not really sure what the practical difference between pause and
            // deactivation is. Either avoids clearing state or allows the DSP to avoid popping?
            continue;
        }
        else if (channel.mForcedStop) {
            channel.mIsFinished = true;
            continue;
        }
        else if (channel.mWaveAramAddress == 0) {
            // I think these are oscillator channels? Not backed by audio.
            // No idea how to implement these yet, so skip them.
            channel.mIsFinished = true;
            continue;
        }

        ValidateChannel(channel);

        OutputSubframe channelSubframe = {};
        RenderChannel(channel, channelAux, channelSubframe);

        if (EnableReverb) {
            // scale the input to the reverb rather than using wet/dry on the output.
            // this way the reverb's internal buffers accumulate energy proportional to mAutoMixerFxMix,
            // so any tail always decays at the correct level regardless of mAutoMixerFxMix changes
            // prevents transients when the next sound starts playing with a different reverb level
            // 600.0f was pulled out of my ass and just sounds good enough for console
            f32 inputGain = (channel.mAutoMixerFxMix >> 8) / 600.0f;
            if (inputGain > 0) {
                anyReverbInput = true;
                for (int j = 0; j < DSP_SUBFRAME_SIZE; j++) {
                    reverbInputL[j] += channelSubframe.channels[0][j] * inputGain;
                    reverbInputR[j] += channelSubframe.channels[1][j] * inputGain;
                }
            }
        }

        if (DumpAudio && sChannelDumpFiles[i]) {
            f32 interleaved[DSP_SUBFRAME_SIZE * 2];
            for (int j = 0; j < DSP_SUBFRAME_SIZE; j++) {
                interleaved[j * 2 + 0] = channelSubframe.channels[0][j];
                interleaved[j * 2 + 1] = channelSubframe.channels[1][j];
            }
            fwrite(interleaved, sizeof(f32), DSP_SUBFRAME_SIZE * 2, sChannelDumpFiles[i]);
        }

        for (int o = 0; o < subframe.channels.size(); o++) {
            MixSubframe(subframe.channels[o], channelSubframe.channels[o]);
        }
    }

    if (EnableReverb && (anyReverbInput || ReverbHasTail)) {
        // Equivalent to -80 dBFS: rms = 1e-4, rms^2 = 1e-8, sumSq = 2 * N * 1e-8
        constexpr f32 REVERB_ENERGY_EPSILON = 2.0f * DSP_SUBFRAME_SIZE * 1e-8f;
        f32 wetEnergy = SharedReverb.processmix(
            reverbInputL.data(), reverbInputR.data(),
            subframe.channels[0].data(), subframe.channels[1].data(),
            DSP_SUBFRAME_SIZE, 1, 1.0f
        );
        ReverbHasTail = wetEnergy >= REVERB_ENERGY_EPSILON;
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
 * Read a single *contiguous* chunk of sample data from a channel into outBuf
 *
 * @returns Amount of samples written to outBuf. May be less than desiredSamples
 */
static int ReadChannelSamplesChunk(
    JASDsp::TChannel& channel,
    ChannelAuxData& aux,
    int desiredSamples,
    s16* outBuf,
    int outBufSize) {

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

    int outputCount = static_cast<int>(renderSamples - skipSamples);

    // this should never be hit with the limits on pitch shift (i think) but just in case!!
    outputCount = std::min(outputCount, outBufSize);
    if (outputCount > 0) {
        memcpy(outBuf, renderData + skipSamples, outputCount * sizeof(s16));
    }

    assert(curSamplePosition % channel.mSamplesPerBlock == 0 || channel.mSamplesLeft == 0);

    return outputCount;
}

/**
 * Fill decodeBuf with at least `needed` samples, fewer may be written if the channel has no loop and its data ends
 */
static void FillDecodeBuf(JASDsp::TChannel& channel, ChannelAuxData& aux, int needed) {
    while (aux.decodeBufCount < needed) {
        if (channel.mSamplesLeft == 0) {
            if (!channel.mLoopFlag) {
                // we aren't a looping channel and there's no samples left, we out of this fuckin loop
                break;
            } else {
                // we are looping, handle loop logic
                channel.mSamplesLeft = channel.mEndSample - channel.mLoopStartSample;
                channel.mSamplePosition = channel.mLoopStartSample;
                aux.hist1 = channel.mpPenult;
                aux.hist0 = channel.mpLast;
            }
        }

        int remainingDecodeSpace = ChannelAuxData::DECODE_BUF_SIZE - aux.decodeBufCount;
        if (remainingDecodeSpace == 0) {
            break;
        }

        aux.decodeBufCount += ReadChannelSamplesChunk(
            channel, aux, std::min(remainingDecodeSpace, needed - aux.decodeBufCount),
            aux.decodeBuf + aux.decodeBufCount, remainingDecodeSpace
        );
    }

    channel.mAramStreamPosition = channel.mWaveAramAddress + ConvertSamplesToDataLength(channel, channel.mSamplePosition);
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

/**
 * Fetch, decode, resample, output
 */
static void RenderChannel(
    JASDsp::TChannel& channel,
    ChannelAuxData& channelAux,
    OutputSubframe& subframe) {

    if (channel.mResetFlag) {
        ResetChannel(channel, channelAux);
    }

    // how many input samples we step per output sample, aka the resampling ratio
    f32 step = (f32)PitchToSampleRate(channel.mPitch) / SampleRate;

    // how many input samples to resample to DSP_SUBFRAME_SIZE output samples
    int needed = static_cast<int>(channelAux.resamplePos + DSP_SUBFRAME_SIZE * step) + 2;

    FillDecodeBuf(channel, channelAux, needed);

    // source ran dry, channel is finished
    if(channelAux.decodeBufCount < needed) {
        channel.mIsFinished = true;
    }

    DspSubframe audioLoadBuffer = {};
    f32 pos = channelAux.resamplePos;
    s16 prev = channelAux.resamplePrev;
    s16 next = channelAux.decodeBufCount > 0 ? channelAux.decodeBuf[0] : prev;
    int srcIdx = 0;

    // linear resampling and f32 conversion
    for (int i = 0; i < DSP_SUBFRAME_SIZE; i++) {
        audioLoadBuffer[i] = (prev + pos * (next - prev)) / 32768.0f;
        pos += step;
        while (pos >= 1.0f) {
            pos -= 1.0f;
            prev = next;
            srcIdx++;
            next = srcIdx < channelAux.decodeBufCount ? channelAux.decodeBuf[srcIdx] : prev;
        }
    }

    // save resampler state for the next subframe, prevents popping on pitch change
    channelAux.resamplePos = pos;
    channelAux.resamplePrev = prev;

    // IIR FILTER

    // IIR part 1, low-pass: out[n] = (in[n] - in[n-1]) * (coeff/128) + out[n-1]
    if (s16 coeff = channel.iir_filter_params[4]; coeff != 0) {
        for (f32& sample : audioLoadBuffer) {
            f32 out = std::clamp(
                (sample - channelAux.prev_lp_in) * ((f32)coeff / 128.0f) + channelAux.prev_lp_out, -1.0f, 1.0f
            );
            
            channelAux.prev_lp_in = sample;        // in[n-1]  = in[n]
            sample = channelAux.prev_lp_out = out; // out[n-1] = out[n]
        }
    }

    // IIR part 2, biquad: out[n] = (b1*in[n-1] + b2*in[n-2] + a1*out[n-1] + a2*out[n-2]) / 32768
    if ((channel.mFilterMode & 0x20) != 0) {
        for (f32& sample : audioLoadBuffer) {
            f32 out = std::clamp((
                channel.iir_filter_params[0] * channelAux.biq_in1  + // b1
                channel.iir_filter_params[1] * channelAux.biq_in2  + // b2
                channel.iir_filter_params[2] * channelAux.biq_out1 + // a1
                channel.iir_filter_params[3] * channelAux.biq_out2   // a2
            ) / 32768.0f, -1.0f, 1.0f);

            // shift history, then store new input and output
            channelAux.biq_in2 = channelAux.biq_in1;   // in[n-2]  = in[n-1]
            channelAux.biq_in1 = sample;               // in[n-1]  = in[n]
            channelAux.biq_out2 = channelAux.biq_out1; // out[n-2] = out[n-1]
            sample = channelAux.biq_out1 = out;        // out[n-1] = out[n]
        }
    }

    // move any remaining samples in the decode buf to the beginning
    int remainingDecodeBuf = channelAux.decodeBufCount - srcIdx;
    if (remainingDecodeBuf > 0) {
        memmove(channelAux.decodeBuf, channelAux.decodeBuf + srcIdx, remainingDecodeBuf * sizeof(s16));
    }

    channelAux.decodeBufCount = std::max(0, remainingDecodeBuf);

    auto hasReadSamples = std::span(audioLoadBuffer).subspan(0, DSP_SUBFRAME_SIZE);

    static_assert(OutputSubframe::NUM_CHANNELS == 2, "Keep RenderChannel in sync!");

    RenderOutputChannel(channel, channelAux, OutputChannel::LEFT, hasReadSamples, subframe);
    RenderOutputChannel(channel, channelAux, OutputChannel::RIGHT, hasReadSamples, subframe);
}

void dusk::audio::DspInit() {
    SharedReverb.setwet(1.0f);
    SharedReverb.setdry(0.0f);
    SharedReverb.setroomsize(0.5f);
    SharedReverb.setdamp(0.7f);
    SharedReverb.setwidth(1.0f);
    SharedReverb.setmode(0.0f);
    SharedReverb.mute();
}

void dusk::audio::ApplyVolume(
    std::span<f32> dst,
    const std::span<f32> src,
    const f32 startVolume,
    const f32 endVolume) {
    assert(dst.size() >= src.size());

    if (startVolume == endVolume) {
        for (int i = 0; i < (int)src.size(); i++) {
            dst[i] = src[i] * startVolume;
        }
    } else {
        const f32 step = (endVolume - startVolume) / static_cast<f32>(src.size());
        for (int i = 0; i < (int)src.size(); i++) {
            dst[i] = src[i] * (startVolume + i * step);
        }
    }
}
