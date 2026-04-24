#pragma once

#include "JSystem/JAudio2/JASDSPInterface.h"

#include <array>
#include <cassert>

#include "SDL3/SDL_audio.h"
#include <span>

// ReSharper disable once CppUnusedIncludeDirective
#include "global.h"

namespace dusk::audio {
    constexpr int SampleRate = 32000;

    enum class OutputChannel : u8 {
        LEFT,
        RIGHT,
        OutputChannel_MAX
    };

    /**
     * Data stored by DSP implementation for each DSP channel.
     */
    struct ChannelAuxData {
        s16 hist1;
        s16 hist0;

        // Used for debugging tools.
        u32 resetCount;

        /**
         * Previous volume values, per output channel.
         * Used to avoid clicking when volumes change.
         * Set to NaN after channel reset, indicating that initial volume value is previous.
         */
        f32 prevVolume[static_cast<int>(OutputChannel::OutputChannel_MAX)];

        f32& PrevVolume(OutputChannel channel) {
            assert(channel < OutputChannel::OutputChannel_MAX);
            return prevVolume[static_cast<int>(channel)];
        }

        // buffer for decoding before resampling, size is chosen based on how many input samples we would need to fetch for the highest possible pitch
        // to fill one subframe of output samples after resampling
        static constexpr int DECODE_BUF_SIZE = 2048;
        s16 decodeBuf[DECODE_BUF_SIZE];
        int decodeBufCount;

        // basically stores our position between resamplePrev and decodeBuf[0] so we don't lose that fractional resampler position next subframe
        f32 resamplePos;
        // last consumed sample from decodeBuf
        s16 resamplePrev;

        // phase of oscillator channels
        u16 oscPhase;

        // low pass previous state
        f32 prev_lp_out;  // out[n-1]
        f32 prev_lp_in;   // in[n-1]

        // biquad state
        f32 biq_in1; // in[n-1]
        f32 biq_in2; // in[n-2]
        f32 biq_out1; // out[n-1]
        f32 biq_out2; // out[n-2]
    };

    extern ChannelAuxData ChannelAux[DSP_CHANNELS];

    /**
     * Data storage for a single subframe and output channel's worth of samples.
     */
    using DspSubframe = std::array<f32, DSP_SUBFRAME_SIZE>;

    /**
     * Data storage for a single subframe's worth of samples, across all output channels.
     */
    struct OutputSubframe {
        static constexpr int NUM_CHANNELS = static_cast<int>(OutputChannel::OutputChannel_MAX);

        std::array<DspSubframe, NUM_CHANNELS> channels;

        DspSubframe& operator[](OutputChannel channel) {
            assert(channel < OutputChannel::OutputChannel_MAX);
            return channels[static_cast<int>(channel)];
        }
    };

    /**
     * Initialize the DSP system, creating data storage needed for channels and such.
     */
    void DspInit();

    /**
     * Render a subframe of audio with the current DSP state.
     */
    void DspRender(OutputSubframe& subframe);

    /**
     * Get the amount of samples a single "block" of this channel's data has.
     */
    constexpr u32 BlockSamples(const JASDsp::TChannel& channel) {
        return channel.mSamplesPerBlock;
    }

    /**
     * Get the amount of bytes a single "block" of this channel's data has.
     */
    constexpr u32 BlockBytes(const JASDsp::TChannel& channel) {
        if (channel.mSamplesPerBlock == 1) {
            if (channel.mBytesPerBlock == 16) {
                return 2;
            }
            if (channel.mBytesPerBlock == 8) {
                return 1;
            }

            CRASH("Unknown format");
        }

        return channel.mBytesPerBlock;
    }

    /**
     * Apply a volume level to audio data.
     * Interpolates across the two provided volume levels to avoid clicking.
     */
    void ApplyVolume(std::span<f32> dst, std::span<f32> src, f32 startVolume, f32 endVolume);

    extern f32 MasterVolume;
    extern f32 PrevMasterVolume;
    extern bool EnableReverb;
    extern bool DumpAudio;
}
