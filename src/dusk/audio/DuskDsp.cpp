#include <ar.h>
#include <dolphin/os.h>

#include "DuskDsp.hpp"

#include <algorithm>
#include <cassert>
#include <iosfwd>

#include "Adpcm.hpp"
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
        CRASH("Indivisible sample count: %d\n", samples);
    }

    return (samples / channel.mSamplesPerBlock) * channel.mBytesPerBlock;
}

static void RenderChannel(
    JASDsp::TChannel& channel,
    ChannelAuxData& channelAux,
    DspSubframe& subframe);

static void ResetChannel(JASDsp::TChannel& channel) {
    channel.mSamplesLeft = channel.mEndSample - channel.mSamplePosition;

    channel.mResetFlag = false;
}

static void MixSubframe(DspSubframe& dst, const DspSubframe& src) {
    for (int i = 0; i < dst.size(); i++) {
        dst[i] = static_cast<s16>(dst[i] + src[i]);
    }
}

void dusk::audio::DspRender(DspSubframe& subframe) {
    subframe.fill(0);

    // This cast half exists because my debugger sucks and this is an easy way to look at the data.
    auto& channels = *reinterpret_cast<std::array<JASDsp::TChannel, DSP_CHANNELS>*>(JASDsp::CH_BUF);

    for (int i = 0; i < channels.size(); i++) {
        auto& channel = channels[i];
        auto& channelAux = ChannelAux[i];

        if (!channel.mIsActive) {
            continue;
        }

        ValidateChannel(channel);

        DspSubframe channelSubframe = {};
        RenderChannel(channel, channelAux, channelSubframe);
        MixSubframe(subframe, channelSubframe);
    }
}

static void RenderChannel(
    JASDsp::TChannel& channel,
    ChannelAuxData& channelAux,
    DspSubframe& subframe) {
    if (channel.mResetFlag) {
        ResetChannel(channel);
    }

    auto aramBase = static_cast<u8*>(ARGetStorageAddress()) + channel.mWaveAramAddress;

    // Streaming logic directly modifies mSamplesLeft.
    // So we use that as our tracking of where we are.
    auto curSamplePosition = channel.mEndSample - channel.mSamplesLeft;
    auto dataPosition = ConvertSamplesToDataLength(channel, curSamplePosition);

    u32 renderSamples = std::min(channel.mSamplesLeft, static_cast<u32>(DSP_SUBFRAME_SIZE));

    Adpcm4ToPcm16(
        aramBase + dataPosition,
        ConvertSamplesToDataLength(channel, renderSamples),
        subframe.data(),
        renderSamples,
        channelAux.hist1,
        channelAux.hist0);

    channel.mSamplesLeft -= renderSamples;
    channel.mSamplePosition += renderSamples;

    if (channel.mSamplesLeft == 0) {
        // Reached end of buffer.
        if (!channel.mLoopFlag) {
            // Finish.
            channel.mIsFinished = true;
            return;
        }

        channel.mSamplesLeft = channel.mEndSample - channel.mLoopStartSample;
        channel.mSamplePosition = channel.mLoopStartSample;
        curSamplePosition = channel.mEndSample - channel.mSamplesLeft;
        dataPosition = ConvertSamplesToDataLength(channel, curSamplePosition);

        Adpcm4ToPcm16(
            aramBase + dataPosition,
            ConvertSamplesToDataLength(channel, DSP_SUBFRAME_SIZE - renderSamples),
            subframe.data() + renderSamples,
            subframe.size() - renderSamples,
            channelAux.hist1,
            channelAux.hist0);

        channel.mSamplesLeft -= (DSP_SUBFRAME_SIZE - renderSamples);
        channel.mSamplePosition += (DSP_SUBFRAME_SIZE - renderSamples);
    }

    channel.mAramStreamPosition = channel.mWaveAramAddress
        + ConvertSamplesToDataLength(channel, channel.mSamplePosition);
}
