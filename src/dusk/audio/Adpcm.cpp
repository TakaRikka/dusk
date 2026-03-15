#include "Adpcm.hpp"

#include <cassert>
#include <fstream>

#include "JSystem/JAudio2/JASAramStream.h"

// https://github.com/magcius/vgmtrans/blob/8e34ddc2fb43948dc1e1a8759c739a0c1c7b62d7/src/main/formats/JaiSeqScanner.cpp#L489-L531
// https://github.com/XAYRGA/JaiSeqX/blob/f29c024ec3663503f506aa02bcd503ada6e7d8aa/JaiSeqXLJA/DSP/JAIDSPADPCM4.cs#L86-L87

static constexpr u16 Coefficient0[] = {
    0,0x0800,0,0x0400,
    0x1000,0x0e00,0x0c00,0x1200,
    0x1068,0x12c0,0x1400,0x0800,
    0x0400,0xfc00,0xfc00,0xf800
};

static constexpr u16 Coefficient1[] = {
    0,0,0x0800,0x0400,0xf800,
    0xfa00,0xfc00,0xf600,0xf738,
    0xf704,0xf400,0xf800,0xfc00,
    0x0400,0,0,
};

constexpr int AdpcmFrameSize = 9;

static s16 Clamp16(s32 value) {
    if (value > 0x7FFF) return 0x7FFF;
    if (value < -0x8000) return -0x8000;
    return value;
}

void dusk::audio::Adpcm4ToPcm16(const u8* adpcm, size_t adpcmLength, s16* pcm, size_t pcmLength, s16& hist2, s16& hist1) {
    assert (adpcmLength % AdpcmFrameSize == 0 && "ADPCM must be divisible by frame size");

    auto endPtr = pcm + pcmLength;

    for (int i = 0; i < adpcmLength; i += AdpcmFrameSize) {
        u8 header = adpcm[i];

        s32 scale = 1 << (header >> 4);
        u8 coefIndex = header & 0xF;

        s16 coef0 = (s16)Coefficient0[coefIndex];
        s16 coef1 = (s16)Coefficient1[coefIndex];

        for (int sampleIdx = 0; sampleIdx < 16; sampleIdx++) {
            u8 adpcmValue = adpcm[i + 1 + sampleIdx / 2];
            u8 unsignedNibble = sampleIdx % 2 == 0 ? adpcmValue >> 4 : adpcmValue & 0xF;
            s8 signedNibble = ((s8)(unsignedNibble << 4)) >> 4;
            s16 sample = Clamp16((((signedNibble * scale) << 11) + (coef0 * hist1 + coef1 * hist2)) >> 11);

            hist2 = hist1;
            hist1 = sample;

            *pcm++ = sample;
            if (endPtr == pcm)
                return;
        }
    }
}
