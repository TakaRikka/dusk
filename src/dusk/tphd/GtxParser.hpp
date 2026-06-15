#ifndef DUSK_TPHD_GTX_PARSER_HPP
#define DUSK_TPHD_GTX_PARSER_HPP

#include <array>
#include <optional>
#include <span>
#include <vector>

#include <dolphin/types.h>

#include "dusk/endian.h"

namespace dusk::tphd {

// On-disk GX2 file header. Followed by a stream of BLK{ blocks.
struct Gfx2Header {
    /* 0x00 */ char magic[4];     // "Gfx2"
    /* 0x04 */ BE(u32) headerSize;
};

// Common 0x20-byte header on every BLK{ block.
struct Gfx2BlockHeader {
    /* 0x00 */ char magic[4];     // "BLK{"
    /* 0x04 */ BE(u32) headerSize;
    /* 0x08 */ BE(u32) versionMajor;
    /* 0x0C */ BE(u32) versionMinor;
    /* 0x10 */ BE(u32) blockType;
    /* 0x14 */ BE(u32) blockDataSize;
    /* 0x18 */ BE(u32) ident;
    /* 0x1C */ BE(u32) flags;
};
static_assert(sizeof(Gfx2BlockHeader) == 0x20);

// On-disk surface-info block body (the GX2 surface descriptor layout).
struct Gx2SurfaceBody {
    /* 0x00 */ BE(u32) dim;
    /* 0x04 */ BE(u32) width;
    /* 0x08 */ BE(u32) height;
    /* 0x0C */ BE(u32) depth;
    /* 0x10 */ BE(u32) mipCount;
    /* 0x14 */ BE(u32) format;
    /* 0x18 */ BE(u32) aa;
    /* 0x1C */ BE(u32) use;
    /* 0x20 */ BE(u32) imgSize;
    /* 0x24 */ BE(u32) imgPtr;
    /* 0x28 */ BE(u32) mipSize;
    /* 0x2C */ BE(u32) mipPtr;
    /* 0x30 */ BE(u32) tileMode;
    /* 0x34 */ BE(u32) swizzle;
    /* 0x38 */ BE(u32) alignment;
    /* 0x3C */ BE(u32) pitch;
    /* 0x40 */ BE(u32) mipOffsets[13];
};
static_assert(sizeof(Gx2SurfaceBody) == 0x74);

struct GtxSurface {
    u32 width;
    u32 height;
    u32 depth;
    u32 mipCount;
    u32 format;          // GX2 surface format code (0x31 BC1, 0x1A RGBA8, ...)
    u32 aa;
    u32 use;
    u32 tileMode;
    u32 swizzle;
    u32 pitch;
    u32 imgSize;         // base level size (bytes)
    u32 mipSize;         // mip chain size (bytes, levels 1..N-1)
    std::array<u32, 13> mipOffsets;

    std::span<const u8> baseData;   // into the owning GTX buffer
    std::span<const u8> mipData;    // into the owning GTX buffer
};

std::vector<GtxSurface> parseGtx(std::span<const u8> gtxBytes);

}

#endif
