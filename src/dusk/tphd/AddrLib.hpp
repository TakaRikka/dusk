// Ported from decaf-emu/addrlib (https://github.com/decaf-emu/addrlib),
// which is itself derived from AMD's address library.
// Copyright (c) 2014 Advanced Micro Devices, Inc. All Rights Reserved.
// Licensed under the AMD MIT-style license; see the AMD copyright header in
// AddrLib.cpp.
//
// Minimal R600/R700 surface-address port sufficient for deswizzling Wii-U
// GTX textures at load time. Hardcoded for Wii-U HW configuration:
//   pipes = 2, banks = 4, pipe interleave = 256B,
//   row size = 2KB, sample split = 2KB, swap size = 256B.

#ifndef DUSK_TPHD_ADDRLIB_HPP
#define DUSK_TPHD_ADDRLIB_HPP

#include <span>
#include <vector>

#include <dolphin/types.h>

namespace dusk::tphd::addrlib {

enum class TileMode : u32 {
    LinearGeneral  = 0,
    LinearAligned  = 1,
    Tiled1DThin1   = 2,
    Tiled1DThick   = 3,
    Tiled2DThin1   = 4,
    Tiled2DThin2   = 5,
    Tiled2DThin4   = 6,
    Tiled2DThick   = 7,
    Tiled2BThin1   = 8,
    Tiled2BThin2   = 9,
    Tiled2BThin4   = 10,
    Tiled2BThick   = 11,
    Tiled3DThin1   = 12,
    Tiled3DThick   = 13,
    Tiled3BThin1   = 14,
    Tiled3BThick   = 15,
};

struct SurfaceDesc {
    u32 width;          // pixels (or BCN blocks)
    u32 height;         // pixels (or BCN blocks)
    u32 pitch;          // pixels (or BCN blocks)
    u32 bpp;            // bits per pixel (or per 4x4 BCN block, e.g. 64 for BC1)
    TileMode tileMode;
    u32 swizzle;        // GTX swizzle field; pipe = (>>8)&1, bank = (>>9)&3
    bool isBcn;
    bool isDepth;
};

// Deswizzle a single surface mip level into a row-major linear buffer.
std::vector<u8> deswizzle(const SurfaceDesc& desc, std::span<const u8> tiledBytes);

struct SurfaceInfoIn {
    u32 width;        // pixels at mip 0 (caller supplies surface base dims)
    u32 height;       // pixels at mip 0
    u32 bpp;          // bits per pixel (e.g. 32 for RGBA8). For BCN, bits per
                      // 4x4 block (64 for BC1, 128 for BC3/5).
    u32 mipLevel;     // 0 = base, 1..N = mip
    TileMode tileMode;
    bool isBcn;
};

struct SurfaceInfoOut {
    u32 width;        // possibly NextPow2'd / block-converted dim used for
                      // the address computation (BCN blocks if isBcn)
    u32 height;       // similar
    u32 pitch;        // aligned pitch used by deswizzle (block units if BCN)
    u32 heightAligned;
    TileMode tileMode; // possibly demoted (2D→1D for small mips)
};

void computeSurfaceInfo(const SurfaceInfoIn& in, SurfaceInfoOut& out);

}  // namespace dusk::tphd::addrlib

#endif
