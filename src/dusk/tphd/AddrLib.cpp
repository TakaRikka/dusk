/*
 * Ported from decaf-emu/addrlib (https://github.com/decaf-emu/addrlib),
 * src/r600/r600addrlib.{cpp,h} and src/core/addrlib.cpp.
 *
 * Original AMD copyright header:
 *
 * Copyright (C) 2014 Advanced Micro Devices, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS, AUTHORS
 * AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * --------------------------------------------------------------------------
 *
 * This is a minimal extraction of decaf-emu's R600AddrLib hardcoded for
 * the Wii-U R700-class GPU (mPipes=2, mBanks=4, group/swap/split sizes).
 * The R600 class hierarchy is collapsed to free functions; only the surface
 * address paths needed for GTX texture deswizzling are kept.
 */

#include "AddrLib.hpp"

#include <algorithm>
#include <cstring>
#include <bit>

namespace dusk::tphd::addrlib {

// ---- Wii-U R700 hardware constants ----------------------------------------
// Match decaf's R600AddrLib state after DecodeGbRegs for the Wii-U register
// configuration: pipes=2, banks=4, group=256B, row=2KB, swap=256B, split=2KB.
static constexpr u32 kPipes               = 2;
static constexpr u32 kBanks               = 4;
static constexpr u32 kPipeInterleaveBytes = 256;
static constexpr u32 kRowSize             = 2048;
static constexpr u32 kSwapSize            = 256;
static constexpr u32 kSplitSize           = 2048;
// Wii-U does not enable the optimal bank-swap heuristic.
static constexpr bool kOptimalBankSwap    = false;

// ---- Decaf addrcommon.h constants -----------------------------------------
static constexpr u32 kMicroTileWidth     = 8;
static constexpr u32 kMicroTileHeight    = 8;
static constexpr u32 kMicroTilePixels    = kMicroTileWidth * kMicroTileHeight;
static constexpr u32 kThickTileThickness = 4;

static constexpr u32 BITS_TO_BYTES(u32 v) { return (v + 7) / 8; }
static constexpr u32 _BIT(u32 v, u32 b) { return (v >> b) & 1; }

static u32 Log2(u32 v) {
    u32 r = 0;
    while (v > 1) { v >>= 1; ++r; }
    return r;
}

static constexpr bool IsPow2(u32 v) { return v != 0 && (v & (v - 1)) == 0; }


static constexpr u32 NextPow2(u32 v) {
    return v <= 1 ? 1u : std::bit_ceil(v);
}
static constexpr u32 PowTwoAlign(u32 v, u32 align) {
    return (v + align - 1) & ~(align - 1);
}

// ---- Tile-mode classification ---------------------------------------------

static u32 ComputeSurfaceThickness(TileMode tm) {
    switch (tm) {
    case TileMode::Tiled1DThick:
    case TileMode::Tiled2DThick:
    case TileMode::Tiled2BThick:
    case TileMode::Tiled3DThick:
    case TileMode::Tiled3BThick:
        return 4u;
    default:
        return 1u;
    }
}

static bool IsThickMacroTiled(TileMode tm) {
    switch (tm) {
    case TileMode::Tiled2DThick:
    case TileMode::Tiled2BThick:
    case TileMode::Tiled3DThick:
    case TileMode::Tiled3BThick:
        return true;
    default:
        return false;
    }
}

static bool IsBankSwappedTileMode(TileMode tm) {
    switch (tm) {
    case TileMode::Tiled2BThin1:
    case TileMode::Tiled2BThin2:
    case TileMode::Tiled2BThin4:
    case TileMode::Tiled2BThick:
    case TileMode::Tiled3BThin1:
    case TileMode::Tiled3BThick:
        return true;
    default:
        return false;
    }
}

// AddrTileType: 0=Displayable, 1=NonDisplayable, 2=DepthSampleOrder, 3=Thick.
// Wii-U GTX color textures empirically use the Displayable (bpp-switched)
// microtile layout, depth surfaces use the simple x/y-interleave pattern
// that AMD calls NonDisplayable. (Same convention as Cemu's port.)
static u32 GetTileType(bool isDepth) {
    return isDepth ? 1u /* NonDisplayable */ : 0u /* Displayable */;
}

static u32 ComputeSurfaceRotationFromTileMode(TileMode tm) {
    switch (tm) {
    case TileMode::Tiled2DThin1:
    case TileMode::Tiled2DThin2:
    case TileMode::Tiled2DThin4:
    case TileMode::Tiled2DThick:
    case TileMode::Tiled2BThin1:
    case TileMode::Tiled2BThin2:
    case TileMode::Tiled2BThin4:
    case TileMode::Tiled2BThick:
        return kPipes * ((kBanks >> 1) - 1);
    case TileMode::Tiled3DThin1:
    case TileMode::Tiled3DThick:
    case TileMode::Tiled3BThin1:
    case TileMode::Tiled3BThick:
        return (kPipes >= 4) ? ((kPipes >> 1) - 1) : 1;
    default:
        return 0;
    }
}

static u32 ComputeMacroTileAspectRatio(TileMode tm) {
    switch (tm) {
    case TileMode::Tiled2BThin1:
    case TileMode::Tiled3DThin1:
    case TileMode::Tiled3BThin1:
        return 1;
    case TileMode::Tiled2DThin2:
    case TileMode::Tiled2BThin2:
        return 2;
    case TileMode::Tiled2DThin4:
    case TileMode::Tiled2BThin4:
        return 4;
    default:
        return 1;
    }
}

// ---- Pixel-index-within-microtile -----------------------------------------

static u32 ComputePixelIndexWithinMicroTile(u32 x, u32 y, u32 z, u32 bpp,
                                            TileMode tm, u32 tileType) {
    u32 b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0, b7 = 0, b8 = 0;
    const u32 x0 = _BIT(x, 0), x1 = _BIT(x, 1), x2 = _BIT(x, 2);
    const u32 y0 = _BIT(y, 0), y1 = _BIT(y, 1), y2 = _BIT(y, 2);
    const u32 z0 = _BIT(z, 0), z1 = _BIT(z, 1), z2 = _BIT(z, 2);
    const u32 thickness = ComputeSurfaceThickness(tm);

    if (tileType == 3 /* Thick */) {
        b0 = x0; b1 = y0; b2 = z0; b3 = x1;
        b4 = y1; b5 = z1; b6 = x2; b7 = y2;
    } else if (tileType == 1 /* NonDisplayable */) {
        b0 = x0; b1 = y0; b2 = x1;
        b3 = y1; b4 = x2; b5 = y2;
    } else {
        switch (bpp) {
        case 8:
            b0 = x0; b1 = x1; b2 = x2; b3 = y1; b4 = y0; b5 = y2; break;
        case 16:
            b0 = x0; b1 = x1; b2 = x2; b3 = y0; b4 = y1; b5 = y2; break;
        case 64:
            b0 = x0; b1 = y0; b2 = x1; b3 = x2; b4 = y1; b5 = y2; break;
        case 128:
            b0 = y0; b1 = x0; b2 = x1; b3 = x2; b4 = y1; b5 = y2; break;
        case 32:
        case 96:
        default:
            b0 = x0; b1 = x1; b2 = y0; b3 = x2; b4 = y1; b5 = y2; break;
        }
    }
    if (tileType != 3 && thickness > 1) {
        b6 = z0; b7 = z1;
    }
    if (thickness == 8) {
        b8 = z2;
    }
    return (b0) | (b1 << 1) | (b2 << 2) | (b3 << 3) | (b4 << 4) |
           (b5 << 5) | (b6 << 6) | (b7 << 7) | (b8 << 8);
}

// ---- Pipe / Bank from coord (no rotation) ---------------------------------
// Hardcoded for Wii-U: pipes=2, banks=4.

static u32 ComputePipeFromCoordWoRotation(u32 x, u32 y) {
    return (_BIT(y, 3) ^ _BIT(x, 3)) & 1;
}

static u32 ComputeBankFromCoordWoRotation(u32 x, u32 y) {
    const u32 ty = y / kPipes;
    const u32 ty4 = _BIT(ty, 4);
    const u32 ty3 = _BIT(ty, 3);
    const u32 x3  = _BIT(x, 3);
    const u32 x4  = _BIT(x, 4);
    u32 b0 = (ty4 ^ x3);
    if (kOptimalBankSwap && kPipes == 8) {
        b0 ^= _BIT(x, 5);
    }
    const u32 b1 = (ty3 ^ x4);
    return b0 | (b1 << 1);
}

// ---- Bank-swapped width ---------------------------------------------------

static u32 ComputeSurfaceBankSwappedWidth(TileMode tm, u32 bpp, u32 numSamples,
                                          u32 pitch) {
    if (!IsBankSwappedTileMode(tm)) return 0;

    u32 slicesPerTile = 1;
    const u32 bytesPerSample = 8 * bpp;
    const u32 samplesPerTile = bytesPerSample ? (kSplitSize / bytesPerSample) : 0;
    if (samplesPerTile != 0) {
        slicesPerTile = std::max<u32>(1u, numSamples / samplesPerTile);
    }
    if (IsThickMacroTiled(tm)) {
        numSamples = 4;
    }
    const u32 bytesPerTileSlice = numSamples * bytesPerSample / slicesPerTile;
    const u32 factor = ComputeMacroTileAspectRatio(tm);
    const u32 swapTiles = std::max<u32>(1u, (kSwapSize >> 1) / bpp);
    const u32 swapWidth = swapTiles * 8 * kBanks;
    const u32 heightBytes = numSamples * factor * kPipes * bpp / slicesPerTile;
    const u32 swapMax = kPipes * kBanks * kRowSize / heightBytes;
    const u32 swapMin = kPipeInterleaveBytes * 8 * kBanks / bytesPerTileSlice;

    u32 bankSwapWidth = std::min(swapMax, std::max(swapMin, swapWidth));
    while (bankSwapWidth >= 2 * pitch) {
        bankSwapWidth >>= 1;
    }
    return bankSwapWidth;
}

// ---- Surface-address from coord -------------------------------------------

static u64 ComputeSurfaceAddrFromCoordMicroTiled(u32 x, u32 y, u32 slice,
                                                 u32 bpp, u32 pitch, u32 height,
                                                 TileMode tm, bool isDepth) {
    const u64 microTileThickness = (tm == TileMode::Tiled1DThick) ? 4u : 1u;
    const u64 microTileBytes =
        BITS_TO_BYTES(static_cast<u32>(kMicroTilePixels * microTileThickness * bpp));
    const u64 microTilesPerRow = pitch / kMicroTileWidth;
    const u64 microTileIndexX = x / kMicroTileWidth;
    const u64 microTileIndexY = y / kMicroTileHeight;
    const u64 microTileIndexZ = slice / microTileThickness;

    const u64 microTileOffset = microTileBytes *
        (microTileIndexX + microTileIndexY * microTilesPerRow);
    const u64 sliceBytes =
        BITS_TO_BYTES(static_cast<u32>(pitch * height * microTileThickness * bpp));
    const u64 sliceOffset = microTileIndexZ * sliceBytes;
    const u64 pixelIndex =
        ComputePixelIndexWithinMicroTile(x, y, slice, bpp, tm, GetTileType(isDepth));
    const u64 pixelOffset = (bpp * pixelIndex) / 8;

    return pixelOffset + microTileOffset + sliceOffset;
}

static u64 ComputeSurfaceAddrFromCoordMacroTiled(u32 x, u32 y, u32 slice, u32 sample,
                                                 u32 bpp, u32 pitch, u32 height,
                                                 u32 numSamples, TileMode tm,
                                                 bool isDepth, u32 pipeSwizzle,
                                                 u32 bankSwizzle) {
    const u64 numPipes = kPipes;
    const u64 numBanks = kBanks;
    const u64 numGroupBits = Log2(kPipeInterleaveBytes);
    const u64 numPipeBits  = Log2(kPipes);
    const u64 numBankBits  = Log2(kBanks);

    const u64 microTileThickness = ComputeSurfaceThickness(tm);
    const u64 microTileBits = kMicroTilePixels * microTileThickness * bpp * numSamples;
    const u64 microTileBytes = microTileBits / 8;

    const u64 pixelIndex =
        ComputePixelIndexWithinMicroTile(x, y, slice, bpp, tm, GetTileType(isDepth));

    u64 sampleOffset, pixelOffset;
    if (isDepth) {
        sampleOffset = bpp * sample;
        pixelOffset  = numSamples * bpp * pixelIndex;
    } else {
        sampleOffset = sample * (microTileBits / numSamples);
        pixelOffset  = bpp * pixelIndex;
    }
    u64 elemOffset = pixelOffset + sampleOffset;

    const u64 bytesPerSample = microTileBytes / numSamples;
    u64 sampleSlice = 0;
    u64 numSampleSplits = 1;
    if (numSamples > 1 && microTileBytes > kSplitSize) {
        const u64 samplesPerSlice = kSplitSize / bytesPerSample;
        numSampleSplits = numSamples / samplesPerSlice;
        numSamples = static_cast<u32>(samplesPerSlice);
        const u64 tileSliceBits = microTileBits / numSampleSplits;
        sampleSlice = elemOffset / tileSliceBits;
        elemOffset %= tileSliceBits;
    }
    elemOffset /= 8;

    u64 pipe = ComputePipeFromCoordWoRotation(x, y);
    u64 bank = ComputeBankFromCoordWoRotation(x, y);
    u64 bankPipe = pipe + numPipes * bank;
    const u64 rotation = ComputeSurfaceRotationFromTileMode(tm);
    const u64 swizzle = pipeSwizzle + numPipes * bankSwizzle;
    u64 sliceIn = slice;
    if (IsThickMacroTiled(tm)) {
        sliceIn /= kThickTileThickness;
    }
    bankPipe ^= numPipes * sampleSlice * ((numBanks >> 1) + 1) ^ (swizzle + sliceIn * rotation);
    bankPipe %= numPipes * numBanks;
    pipe = bankPipe % numPipes;
    bank = bankPipe / numPipes;

    const u64 sliceBytes =
        BITS_TO_BYTES(static_cast<u32>(pitch * height * microTileThickness * bpp * numSamples));
    const u64 sliceOffset = sliceBytes *
        ((sampleSlice + numSampleSplits * slice) / microTileThickness);

    u64 macroTilePitch  = 8 * numBanks;
    u64 macroTileHeight = 8 * numPipes;
    switch (tm) {
    case TileMode::Tiled2DThin2:
    case TileMode::Tiled2BThin2:
        macroTilePitch /= 2;
        macroTileHeight *= 2;
        break;
    case TileMode::Tiled2DThin4:
    case TileMode::Tiled2BThin4:
        macroTilePitch /= 4;
        macroTileHeight *= 4;
        break;
    default:
        break;
    }
    const u64 macroTilesPerRow = pitch / macroTilePitch;
    const u64 macroTileBytes =
        BITS_TO_BYTES(static_cast<u32>(numSamples * microTileThickness * bpp *
                                       macroTileHeight * macroTilePitch));
    const u64 macroTileIndexX = x / macroTilePitch;
    const u64 macroTileIndexY = y / macroTileHeight;
    const u64 macroTileOffset = macroTileBytes *
        (macroTileIndexX + macroTilesPerRow * macroTileIndexY);

    if (IsBankSwappedTileMode(tm)) {
        static constexpr u32 bankSwapOrder[] = { 0, 1, 3, 2, 6, 7, 5, 4, 0, 0 };
        const u32 bankSwapWidth =
            ComputeSurfaceBankSwappedWidth(tm, bpp, numSamples, pitch);
        const u64 swapIndex = (bankSwapWidth != 0)
            ? (macroTilePitch * macroTileIndexX / bankSwapWidth) : 0;
        bank ^= bankSwapOrder[swapIndex & (kBanks - 1)];
    }

    const u64 groupMask = (1u << numGroupBits) - 1;
    const u64 totalOffset = elemOffset +
        ((macroTileOffset + sliceOffset) >> (numBankBits + numPipeBits));
    const u64 offsetHigh = (totalOffset & ~groupMask) << (numBankBits + numPipeBits);
    const u64 offsetLow  = totalOffset & groupMask;
    const u64 bankBits   = bank << (numPipeBits + numGroupBits);
    const u64 pipeBits   = pipe << numGroupBits;
    return bankBits | pipeBits | offsetLow | offsetHigh;
}

// ---- High-level deswizzle -------------------------------------------------

std::vector<u8> deswizzle(const SurfaceDesc& desc, std::span<const u8> tiledBytes) {
    // For BCN formats addrlib operates on block coordinates; bpp is bits per
    // 4x4 block (e.g. 64 for BC1). Reduce width/height to block extents.
    const u32 blockWidth  = desc.isBcn ? (desc.width  + 3) / 4 : desc.width;
    const u32 blockHeight = desc.isBcn ? (desc.height + 3) / 4 : desc.height;

    const u32 bytesPerElement = desc.bpp / 8;
    const u32 linearStride = blockWidth * bytesPerElement;
    std::vector<u8> linear(static_cast<size_t>(linearStride) * blockHeight, 0);

    const u32 pipeSwizzle = (desc.swizzle >> 8) & 1;
    const u32 bankSwizzle = (desc.swizzle >> 9) & 3;

    // Linear tile modes: trivial copy honoring pitch.
    if (desc.tileMode == TileMode::LinearGeneral ||
        desc.tileMode == TileMode::LinearAligned) {
        for (u32 y = 0; y < blockHeight; ++y) {
            const u32 srcOff = y * desc.pitch * bytesPerElement;
            if (srcOff + linearStride > tiledBytes.size()) break;
            std::memcpy(linear.data() + y * linearStride,
                        tiledBytes.data() + srcOff, linearStride);
        }
        return linear;
    }

    const bool microTiled =
        (desc.tileMode == TileMode::Tiled1DThin1 ||
         desc.tileMode == TileMode::Tiled1DThick);

    for (u32 y = 0; y < blockHeight; ++y) {
        for (u32 x = 0; x < blockWidth; ++x) {
            u64 srcOff;
            if (microTiled) {
                srcOff = ComputeSurfaceAddrFromCoordMicroTiled(
                    x, y, /*slice*/ 0, desc.bpp, desc.pitch, blockHeight,
                    desc.tileMode, desc.isDepth);
            } else {
                srcOff = ComputeSurfaceAddrFromCoordMacroTiled(
                    x, y, /*slice*/ 0, /*sample*/ 0, desc.bpp, desc.pitch,
                    blockHeight, /*numSamples*/ 1, desc.tileMode, desc.isDepth,
                    pipeSwizzle, bankSwizzle);
            }
            if (srcOff + bytesPerElement > tiledBytes.size()) continue;
            const u32 dstOff = (y * blockWidth + x) * bytesPerElement;
            std::memcpy(linear.data() + dstOff,
                        tiledBytes.data() + srcOff, bytesPerElement);
        }
    }
    return linear;
}

// R600AddrLib::ConvertToNonBankSwappedMode (r600addrlib.cpp:355)
static TileMode ConvertToNonBankSwappedMode(TileMode tm) {
    switch (tm) {
    case TileMode::Tiled2BThin1: return TileMode::Tiled2DThin1;
    case TileMode::Tiled2BThin2: return TileMode::Tiled2DThin2;
    case TileMode::Tiled2BThin4: return TileMode::Tiled2DThin4;
    case TileMode::Tiled2BThick: return TileMode::Tiled2DThick;
    case TileMode::Tiled3BThin1: return TileMode::Tiled3DThin1;
    case TileMode::Tiled3BThick: return TileMode::Tiled3DThick;
    default:                     return tm;
    }
}

// R600AddrLib::ComputeSurfaceMipLevelTileMode (r600addrlib.cpp:544). 
static TileMode ComputeSurfaceMipLevelTileMode(TileMode baseTileMode,
                                               u32 bpp,
                                               u32 level,
                                               u32 width,
                                               u32 height,
                                               u32 numSamples,
                                               bool noRecursive) {
    // ComputeSurfaceTileSlices == 1 for our case (numSamples=1, thin).
    // HwlDegradeThickTileMode is identity for thin tiles.
    TileMode tileMode = baseTileMode;

    const u32 rotation = ComputeSurfaceRotationFromTileMode(tileMode);
    if ((rotation % kPipes) == 0) {
        switch (tileMode) {
        case TileMode::Tiled3DThin1: tileMode = TileMode::Tiled2DThin1; break;
        case TileMode::Tiled3DThick: tileMode = TileMode::Tiled2DThick; break;
        case TileMode::Tiled3BThin1: tileMode = TileMode::Tiled2BThin1; break;
        case TileMode::Tiled3BThick: tileMode = TileMode::Tiled2BThick; break;
        default: break;
        }
    }

    if (noRecursive || level == 0) {
        return tileMode;
    }

    if (bpp == 96 || bpp == 48 || bpp == 24) bpp /= 3;

    width  = NextPow2(width);
    height = NextPow2(height);

    tileMode = ConvertToNonBankSwappedMode(tileMode);

    const u32 thickness        = ComputeSurfaceThickness(tileMode);
    const u32 microTileBytes   = BITS_TO_BYTES(thickness * bpp * 64);
    const u32 widthAlignFactor = (microTileBytes <= kPipeInterleaveBytes)
                                 ? (kPipeInterleaveBytes / microTileBytes) : 1u;

    u32 macroTileWidth  = 8 * kBanks;
    u32 macroTileHeight = 8 * kPipes;

    switch (tileMode) {
    case TileMode::Tiled2DThin1:
    case TileMode::Tiled3DThin1:
        if (width < widthAlignFactor * macroTileWidth || height < macroTileHeight) {
            tileMode = TileMode::Tiled1DThin1;
        }
        break;
    case TileMode::Tiled2DThin2:
        macroTileWidth >>= 1; macroTileHeight *= 2;
        if (width < widthAlignFactor * macroTileWidth || height < macroTileHeight) {
            tileMode = TileMode::Tiled1DThin1;
        }
        break;
    case TileMode::Tiled2DThin4:
        macroTileWidth >>= 2; macroTileHeight *= 4;
        if (width < widthAlignFactor * macroTileWidth || height < macroTileHeight) {
            tileMode = TileMode::Tiled1DThin1;
        }
        break;
    case TileMode::Tiled2DThick:
    case TileMode::Tiled3DThick:
        if (width < widthAlignFactor * macroTileWidth || height < macroTileHeight) {
            tileMode = TileMode::Tiled1DThick;
        }
        break;
    default: break;
    }

    // numSlices < 4 collapse — our textures are all 2D (slices=1), so the
    // Thick→Thin demote always fires when we hit a thick mode. 
    if (tileMode == TileMode::Tiled1DThick)      tileMode = TileMode::Tiled1DThin1;
    else if (tileMode == TileMode::Tiled2DThick) tileMode = TileMode::Tiled2DThin1;
    else if (tileMode == TileMode::Tiled3DThick) tileMode = TileMode::Tiled3DThin1;

    return ComputeSurfaceMipLevelTileMode(tileMode, bpp, level, width, height,
                                          numSamples, /*noRecursive*/ true);
}

// R600AddrLib::ComputeSurfaceAlignmentsMicrotiled (r600addrlib.cpp:714).
static void ComputeAlignmentsMicroTiled(TileMode tileMode, u32 bpp, u32 numSamples,
                                        u32& pitchAlign, u32& heightAlign) {
    if (bpp == 96 || bpp == 48 || bpp == 24) bpp /= 3;
    const u32 thickness     = ComputeSurfaceThickness(tileMode);
    const u32 pitchAlignment = kPipeInterleaveBytes / bpp / numSamples / thickness;
    pitchAlign  = std::max<u32>(8u, pitchAlignment);
    heightAlign = 8;
    // AdjustPitchAlignment is no-op without flags.display.
}

// R600AddrLib::ComputeSurfaceAlignmentsMacrotiled (r600addrlib.cpp:805).
static void ComputeAlignmentsMacroTiled(TileMode tileMode, u32 bpp, u32 numSamples,
                                        u32& pitchAlign, u32& heightAlign,
                                        u32& macroTileWidth, u32& macroTileHeight) {
    const u32 aspectRatio = (tileMode == TileMode::Tiled2DThin2 ||
                             tileMode == TileMode::Tiled2BThin2) ? 2u
                          : (tileMode == TileMode::Tiled2DThin4 ||
                             tileMode == TileMode::Tiled2BThin4) ? 4u : 1u;
    const u32 thickness = ComputeSurfaceThickness(tileMode);
    if (bpp == 96 || bpp == 48 || bpp == 24) bpp /= 3;
    if (bpp == 3) bpp = 1;

    macroTileWidth  = 8 * kBanks / aspectRatio;
    macroTileHeight = aspectRatio * 8 * kPipes;
    pitchAlign  = std::max<u32>(macroTileWidth,
                                macroTileWidth * (kPipeInterleaveBytes / bpp / (8 * thickness) / numSamples));
    heightAlign = macroTileHeight;
    // IsDualBaseAlignNeeded is R6XX-only -> false here; baseAlign branch skipped.
}

// AddrLib::PadDimensions (addrlib.cpp:433), simplified: no cube, no slice padding.
static void PadDimensions(TileMode /*tm*/, u32& pitch, u32 pitchAlign,
                          u32& height, u32 heightAlign, u32 padDims) {
    if (padDims == 0) padDims = 3;
    if (IsPow2(pitchAlign)) {
        pitch = PowTwoAlign(pitch, pitchAlign);
    } else {
        pitch = ((pitch + pitchAlign - 1) / pitchAlign) * pitchAlign;
    }
    if (padDims > 1) {
        height = PowTwoAlign(height, heightAlign);
    }
}

// R600AddrLib::ComputeSurfaceInfoMicroTiled (r600addrlib.cpp:969).
static void ComputeSurfaceInfoMicroTiled(u32 width, u32 height, u32 numSamples, u32 bpp,
                                         TileMode tileMode, u32 mipLevel, SurfaceInfoOut& out) {
    u32 pitch  = width;
    u32 h      = height;
    if (mipLevel) {
        pitch = NextPow2(pitch);
        h     = NextPow2(h);
        // numSlices < 4 / thick collapse: no thick at this point for our flow.
    }
    u32 pitchAlign = 0, heightAlign = 0;
    ComputeAlignmentsMicroTiled(tileMode, bpp, numSamples, pitchAlign, heightAlign);
    PadDimensions(tileMode, pitch, pitchAlign, h, heightAlign, /*padDims*/ 0);
    out.pitch         = pitch;
    out.height        = height;
    out.heightAligned = h;
    out.tileMode      = tileMode;
}

// R600AddrLib::ComputeSurfaceInfoMacroTiled (r600addrlib.cpp:1198).
static void ComputeSurfaceInfoMacroTiled(u32 width, u32 height, u32 numSamples, u32 bpp,
                                         TileMode tileMode, TileMode baseTileMode,
                                         u32 mipLevel, SurfaceInfoOut& out) {
    u32 pitch = width;
    u32 h     = height;
    if (mipLevel) {
        pitch = NextPow2(pitch);
        h     = NextPow2(h);
    }
    u32 pitchAlignBase = 0, heightAlignBase = 0, mwBase = 0, mhBase = 0;
    if (tileMode != baseTileMode && mipLevel != 0 &&
        IsThickMacroTiled(baseTileMode) && !IsThickMacroTiled(tileMode)) {
        ComputeAlignmentsMacroTiled(baseTileMode, bpp, numSamples,
                                    pitchAlignBase, heightAlignBase, mwBase, mhBase);
        const u32 pitchAlignFactor = std::max<u32>(1u, (kPipeInterleaveBytes >> 3) / bpp);
        if (pitch < (pitchAlignBase * pitchAlignFactor) || h < heightAlignBase) {
            ComputeSurfaceInfoMicroTiled(width, height, numSamples, bpp,
                                         TileMode::Tiled1DThin1, mipLevel, out);
            return;
        }
    }

    u32 pitchAlign = 0, heightAlign = 0, macroWidth = 0, macroHeight = 0;
    ComputeAlignmentsMacroTiled(tileMode, bpp, numSamples,
                                pitchAlign, heightAlign, macroWidth, macroHeight);
    const u32 bankSwappedWidth = ComputeSurfaceBankSwappedWidth(tileMode, bpp, numSamples, pitch);
    pitchAlign = std::max(pitchAlign, bankSwappedWidth);
    // IsDualPitchAlignNeeded is R6XX-only -> false here.
    PadDimensions(tileMode, pitch, pitchAlign, h, heightAlign, /*padDims*/ 0);
    out.pitch         = pitch;
    out.height        = height;
    out.heightAligned = h;
    out.tileMode      = tileMode;
}

void computeSurfaceInfo(const SurfaceInfoIn& in, SurfaceInfoOut& out) {
    // AddrLib::ComputeMipLevel + R600AddrLib::HwlComputeMipLevel: align BCN
    // base dims to 4 pixels; for mipLevel>0, reduce dims and NextPow2 them.
    u32 width  = in.width;
    u32 height = in.height;
    if (in.isBcn && in.mipLevel == 0) {
        width  = PowTwoAlign(width, 4u);
        height = PowTwoAlign(height, 4u);
    }
    if (in.mipLevel > 0) {
        width  = std::max(1u, width  >> in.mipLevel);
        height = std::max(1u, height >> in.mipLevel);
        width  = NextPow2(width);
        height = NextPow2(height);
    }

    if (in.isBcn) {
        width  = (width  + 3) / 4;
        height = (height + 3) / 4;
    }

    const u32 numSamples = 1;
    const TileMode demoted = ComputeSurfaceMipLevelTileMode(in.tileMode, in.bpp,
                                                            in.mipLevel, width, height,
                                                            numSamples, /*noRecursive*/ false);
    out.width  = width;
    out.height = height;
    out.tileMode = demoted;

    switch (demoted) {
    case TileMode::LinearGeneral:
    case TileMode::LinearAligned: {
        // ComputeSurfaceInfoLinear
        const u32 pa = std::max<u32>(64u, kPipeInterleaveBytes / in.bpp / numSamples);
        u32 pitch = width, h = height;
        if (in.mipLevel) { pitch = NextPow2(pitch); h = NextPow2(h); }
        PadDimensions(demoted, pitch, pa, h, 1u, /*padDims*/ 0);
        out.pitch = pitch; out.heightAligned = h;
        break;
    }
    case TileMode::Tiled1DThin1:
    case TileMode::Tiled1DThick:
        ComputeSurfaceInfoMicroTiled(width, height, numSamples, in.bpp,
                                     demoted, in.mipLevel, out);
        break;
    default:
        ComputeSurfaceInfoMacroTiled(width, height, numSamples, in.bpp,
                                     demoted, in.tileMode, in.mipLevel, out);
        break;
    }
}

}  // namespace dusk::tphd::addrlib
