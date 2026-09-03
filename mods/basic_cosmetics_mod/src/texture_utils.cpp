#include "texture_utils.hpp"
#include "color_utils.hpp"
#include "mod.hpp"

#include "mods/svc/config.h"
#include "mods/svc/log.hpp"

#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/JSupport/JSupport.h"
#include "JSystem/JUtility/JUTNameTab.h"
#include "d/actor/d_a_alink.h"

static void get_gx_tile_info(uint8_t format, uint32_t& tileWidth, uint32_t& tileHeight, uint32_t& tileSize) {
    switch (format) {
    case GX_TF_I8:
    case GX_TF_IA4:
    case GX_TF_C8:
        tileWidth = 8; tileHeight = 4; tileSize = 32;
        break;
    case GX_TF_IA8:
    case GX_TF_RGB565:
    case GX_TF_RGB5A3:
    case GX_TF_C14X2:
        tileWidth = 4; tileHeight = 4; tileSize = 32;
        break;
    case GX_TF_RGBA8:
        tileWidth = 4; tileHeight = 4; tileSize = 64;
        break;
    case GX_TF_I4:
    case GX_TF_C4:
    case GX_TF_CMPR:
    default:
        tileWidth = 8; tileHeight = 8; tileSize = 32;
        break;
    }
}

uint32_t get_image_data_size(uint32_t format, uint32_t width, uint32_t height, uint32_t mipmapCount) {

    uint32_t tileWidth, tileHeight, tileSize;
    get_gx_tile_info(format, tileWidth, tileHeight, tileSize);

    uint32_t totalSize = 0;

    for (uint8_t i = 0; i < mipmapCount; ++i) {
        // Round dimensions up to nearest tile boundary
        uint32_t paddedWidth = (width + tileWidth - 1) & ~(tileWidth - 1);
        uint32_t paddedHeight = (height + tileHeight - 1) & ~(tileHeight - 1);

        uint32_t tilesX = paddedWidth / tileWidth;
        uint32_t tilesY = paddedHeight / tileHeight;

        totalSize += tilesX * tilesY * tileSize;

        // Downscale dimensions for next mipmap level
        width = std::max(1u, width >> 1);
        height = std::max(1u, height >> 1);
    }

    return totalSize;
}

// When left is greater than right
// 0b00 points to the left color
// 0b01 points to the right color
// 0b10 is closer to left color
// 0b11 is closer to right color

// When left is not greater than right
// 0b00 points to the left color
// 0b01 points to the right color
// 0b10 is midway between the colors
// 0b11 is transparent

// That means when maintaining the relative order, if we have to swap the colors:

// in the case of left being greater than right:
// 0b00 will swap to 0b01
// 0b01 will swap to 0b00
// 0b10 will swap to 0b11
// 0b11 will swap to 0b10
// So the left bit stays the same, and the right bit changes
// Can do xor (^) like 0b01010101 or 0x55 for each u16

// in the case of left not being greater than right:
// 0b00 will swap to 0b01
// 0b01 will swap to 0b00
// 0b10 will stay the same
// 0b11 will stay the same
// so if the left bit is a 0, the right bit will change
uint32_t swap_index_bits(bool leftIsGreater, uint32_t bits) {
    if (leftIsGreater) {
        return bits ^ 0x55555555;
    }

    const uint32_t mask = ((bits >> 1) & 0x55555555) ^ 0x55555555;
    return bits ^ mask;
}

void recolor_cmpr_texture(const TextureReplacementData& replacementData, const GXColor color, std::vector<u8>& newTextureDataOut)
{
    uint16_t recolors[0x100];
    for (int32_t i = 0; i < 0x100; i++) {
        recolors[i] = blend_overlay_rgb_565(i, color);
    }

    const uint8_t mipCount = (replacementData.data.mip_count > 0) ? replacementData.data.mip_count : 1;
    uint32_t mipWidth = replacementData.key.width;
    uint32_t mipHeight = replacementData.key.height;

    uint8_t* currentAddr = newTextureDataOut.data();

    for (uint8_t mip = 0; mip < mipCount; ++mip) {
        // Round dimensions up to the nearest 8x8 tile boundary
        const uint32_t roundedWidth = (mipWidth + 7) & ~7;
        const uint32_t roundedHeight = (mipHeight + 7) & ~7;

        const uint32_t numBlocks = (roundedWidth / 8) * (roundedHeight / 8);
        const uint32_t iterations = numBlocks * 4; // 4 CMPR sub-blocks per 8x8 tile

        for (uint32_t i = 0; i < iterations; i++) {
            auto* rgb565Ptr = reinterpret_cast<BE<uint16_t>*>(currentAddr);

            auto leftRgb565 = rgb565Ptr[0];
            auto rightRgb565 = rgb565Ptr[1];
            const bool leftIsGreater = leftRgb565 > rightRgb565;

            const uint32_t leftGrayVal = desaturate_rgb_565(leftRgb565);
            const uint32_t rightGrayVal = desaturate_rgb_565(rightRgb565);

            uint16_t leftNewRgb565 = recolors[leftGrayVal];
            uint16_t rightNewRgb565 = recolors[rightGrayVal];

            bool needsBitSwap = false;

            if (leftIsGreater) {
                if (leftNewRgb565 == rightNewRgb565) {
                    // Need to make sure that subtracting 1 does not mess
                    // everything up. For example, 0x1000 - 1 => 0x0fff which is
                    // a completely different color.
                    if ((leftNewRgb565 & 0x1f) == 0) {
                        // If left value has 0 blue, we change its blue to 1.
                        leftNewRgb565 += 1;
                    }
                    rightNewRgb565 = leftNewRgb565 - 1;
                }
                else if (leftNewRgb565 < rightNewRgb565) {
                    needsBitSwap = true;
                }
            }
            else if (leftNewRgb565 > rightNewRgb565) {
                needsBitSwap = true;
            }

            if (needsBitSwap) {
                // The left and right colors are swapping so that their values
                // are relative in the same way. We need to update the bits
                // referencing the palette entries to handle the swap.
                const uint16_t temp = leftNewRgb565;
                leftNewRgb565 = rightNewRgb565;
                rightNewRgb565 = temp;

                auto wordPtr = reinterpret_cast<BE<uint32_t>*>(currentAddr);
                const uint32_t bits = wordPtr[1];

                const uint32_t newBits = swap_index_bits(leftIsGreater, bits);
                wordPtr[1] = newBits;
            }

            rgb565Ptr[0] = leftNewRgb565;
            rgb565Ptr[1] = rightNewRgb565;

            currentAddr += 8;
        }

        // Halve dimensions for the next mipmap level
        mipWidth = std::max(1u, mipWidth >> 1);
        mipHeight = std::max(1u, mipHeight >> 1);
    }
}

void recolor_rgb5a3_texture(const TextureReplacementData& replacementData, const GXColor color, std::vector<u8>& newTextureDataOut)
{
    // Precompute lookup tables for both RGB555 (opaque) and RGB444 (translucent) modes
    uint16_t recolors_rgb555[0x100];
    uint16_t recolors_rgb444[0x100];

    for (int32_t i = 0; i < 0x100; i++) {
        const uint8_t r = blend_overlay_channel(i, color.r);
        const uint8_t g = blend_overlay_channel(i, color.g);
        const uint8_t b = blend_overlay_channel(i, color.b);

        // Pack as RGB555: Bit 15 set to 1 + 5 bits R, G, B
        recolors_rgb555[i] = 0x8000 | ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3);

        // Pack as RGB444: 4 bits R, G, B (Bit 15 remains 0)
        recolors_rgb444[i] = ((r >> 4) << 8) | ((g >> 4) << 4) | (b >> 4);
    }

    const uint8_t mipCount =
        replacementData.data.mip_count > 0 ? replacementData.data.mip_count : 1;
    uint32_t mipWidth = replacementData.key.width;
    uint32_t mipHeight = replacementData.key.height;
    auto* pixelPtr = reinterpret_cast<BE<uint16_t>*>(newTextureDataOut.data());

    for (uint8_t mip = 0; mip < mipCount; ++mip) {
        const uint32_t roundedWidth = (mipWidth + 3) & ~3;
        const uint32_t roundedHeight = (mipHeight + 3) & ~3;
        const uint32_t totalPixels = roundedWidth * roundedHeight;

        for (uint32_t i = 0; i < totalPixels; i++) {
            const uint16_t rawPixel = pixelPtr[i];

            // MSB determines if pixel is opaque or translucent
            if (rawPixel & 0x8000) {
                // Pixel is opaque
                const uint8_t r5 = (rawPixel >> 10) & 0x1F;
                const uint8_t g5 = (rawPixel >> 5) & 0x1F;
                const uint8_t b5 = rawPixel & 0x1F;

                // Expand 5-bit to 8-bit
                const uint8_t r8 = (r5 << 3) | (r5 >> 2);
                const uint8_t g8 = (g5 << 3) | (g5 >> 2);
                const uint8_t b8 = (b5 << 3) | (b5 >> 2);

                const uint8_t grayVal = static_cast<uint8_t>((r8 * 77 + g8 * 150 + b8 * 29) >> 8);

                pixelPtr[i] = recolors_rgb555[grayVal];
            } else {
                // Pixel is translucent
                const uint16_t alpha3 = rawPixel & 0x7000;

                const uint8_t r4 = (rawPixel >> 8) & 0x0F;
                const uint8_t g4 = (rawPixel >> 4) & 0x0F;
                const uint8_t b4 = rawPixel & 0x0F;

                // Expand 4-bit to 8-bit
                const uint8_t r8 = (r4 << 4) | r4;
                const uint8_t g8 = (g4 << 4) | g4;
                const uint8_t b8 = (b4 << 4) | b4;

                const uint8_t grayVal = static_cast<uint8_t>((r8 * 77 + g8 * 150 + b8 * 29) >> 8);

                // Combine original alpha with recolored RGB444
                pixelPtr[i] = alpha3 | recolors_rgb444[grayVal];
            }
        }

        pixelPtr += totalPixels;
        mipWidth = std::max(1u, mipWidth >> 1);
        mipHeight = std::max(1u, mipHeight >> 1);
    }
}

// Function to encode a single 4x4 sub-block (16 pixels) into an 8-byte CMPR block
static void encode_cmpr_sub_block(uint8_t* dst, const uint8_t pixels[16]) {
    uint8_t min_val = 255;
    uint8_t max_val = 0;

    for (int i = 0; i < 16; ++i) {
        if (pixels[i] < min_val) min_val = pixels[i];
        if (pixels[i] > max_val) max_val = pixels[i];
    }

    auto intensity_to_rgb565 = [](uint8_t val) -> uint16_t {
        uint16_t r5 = val >> 3;
        uint16_t g6 = val >> 2;
        uint16_t b5 = val >> 3;
        return static_cast<uint16_t>((r5 << 11) | (g6 << 5) | b5);
    };

    uint16_t c0_565 = intensity_to_rgb565(max_val);
    uint16_t c1_565 = intensity_to_rgb565(min_val);
    uint32_t indices = 0;

    if (max_val > min_val) {
        // Enforce c0_565 > c1_565 in unsigned 16-bit representation to use 4-color mode
        if (c0_565 == c1_565) {
            if ((c0_565 & 0x001F) < 0x001F) {
                c0_565 += 1;
            } else {
                c1_565 -= 1;
            }
        }

        // Interpolated 8-bit intensity values for quantization
        const int c0 = max_val;
        const int c1 = min_val;
        const int c2 = (2 * max_val + min_val) / 3;
        const int c3 = (max_val + 2 * min_val) / 3;

        // Map each pixel to the nearest palette entry
        for (int i = 0; i < 16; ++i) {
            const int p = pixels[i];
            const int d0 = std::abs(p - c0);
            const int d1 = std::abs(p - c1);
            const int d2 = std::abs(p - c2);
            const int d3 = std::abs(p - c3);

            uint32_t best_idx = 0;
            int min_d = d0;

            if (d1 < min_d) { min_d = d1; best_idx = 1; }
            if (d2 < min_d) { min_d = d2; best_idx = 2; }
            if (d3 < min_d) { min_d = d3; best_idx = 3; }

            indices |= (best_idx << (30 - (2 * i)));
        }
    }

    // Account for big endian data expectation
    dst[0] = static_cast<uint8_t>(c0_565 >> 8);
    dst[1] = static_cast<uint8_t>(c0_565 & 0xFF);
    dst[2] = static_cast<uint8_t>(c1_565 >> 8);
    dst[3] = static_cast<uint8_t>(c1_565 & 0xFF);
    dst[4] = static_cast<uint8_t>(indices >> 24);
    dst[5] = static_cast<uint8_t>((indices >> 16) & 0xFF);
    dst[6] = static_cast<uint8_t>((indices >> 8) & 0xFF);
    dst[7] = static_cast<uint8_t>(indices & 0xFF);
}

bool convert_i8_to_cmpr(TextureReplacementData& replacementData, std::vector<u8>& cmprOut) {
    if (cmprOut.empty()) {
        return false;
    }

    const uint8_t mipCount = (replacementData.data.mip_count > 0) ? replacementData.data.mip_count : 1;
    const auto expectedInputSize = get_image_data_size(
        GX_TF_I8, replacementData.key.width, replacementData.key.height, mipCount);
    if (cmprOut.size() < expectedInputSize) {
        return false;
    }

    const auto sourceData = cmprOut;
    std::vector<u8> convertedData(get_image_data_size(
        GX_TF_CMPR, replacementData.key.width, replacementData.key.height, mipCount));
    uint32_t mipWidth = replacementData.key.width;
    uint32_t mipHeight = replacementData.key.height;

    const uint8_t* readPtr = sourceData.data();
    uint8_t* writePtr = convertedData.data();

    for (uint8_t mip = 0; mip < mipCount; ++mip) {
        const uint32_t paddedWidthI8 = (mipWidth + 7) & ~7;
        const uint32_t paddedHeightI8 = (mipHeight + 3) & ~3;
        const uint32_t tilesX_I8 = paddedWidthI8 / 8;

        const uint32_t paddedWidthCMPR = (mipWidth + 7) & ~7;
        const uint32_t paddedHeightCMPR = (mipHeight + 7) & ~7;
        const uint32_t blocksX_CMPR = paddedWidthCMPR / 8;
        const uint32_t blocksY_CMPR = paddedHeightCMPR / 8;

        for (uint32_t by = 0; by < blocksY_CMPR; ++by) {
            for (uint32_t bx = 0; bx < blocksX_CMPR; ++bx) {
                uint8_t subBlockPixels[4][16]{};

                for (uint32_t subBlockY = 0; subBlockY < 2; ++subBlockY) {
                    for (uint32_t subBlockX = 0; subBlockX < 2; ++subBlockX) {
                        const uint32_t subBlock = subBlockY * 2 + subBlockX;
                        for (uint32_t row = 0; row < 4; ++row) {
                            for (uint32_t col = 0; col < 4; ++col) {
                                const uint32_t x = bx * 8 + subBlockX * 4 + col;
                                const uint32_t y = by * 8 + subBlockY * 4 + row;
                                if (x >= mipWidth || y >= mipHeight) {
                                    continue;
                                }

                                const uint32_t tile = (y / 4) * tilesX_I8 + (x / 8);
                                const uint32_t tileOffset = (y % 4) * 8 + (x % 8);
                                subBlockPixels[subBlock][row * 4 + col] =
                                    readPtr[tile * 32 + tileOffset];
                            }
                        }
                    }
                }

                for (const auto& subBlockPixel : subBlockPixels) {
                    encode_cmpr_sub_block(writePtr, subBlockPixel);
                    writePtr += 8;
                }
            }
        }

        const uint32_t tilesY_I8 = paddedHeightI8 / 4;
        readPtr += tilesX_I8 * tilesY_I8 * 32;

        mipWidth = std::max(1u, mipWidth >> 1);
        mipHeight = std::max(1u, mipHeight >> 1);
    }

    cmprOut.swap(convertedData);

    // Update format
    replacementData.data.gx_format = GX_TF_CMPR;

    return true;
}

void recolor_texture(TextureReplacementData& replacementData, GXColor color, std::vector<u8>& newTextureDataOut) {

    switch (replacementData.key.gx_format) {
    case GX_TF_CMPR:
        recolor_cmpr_texture(replacementData, color, newTextureDataOut);
        break;
    case GX_TF_RGB5A3:
        recolor_rgb5a3_texture(replacementData, color, newTextureDataOut);
        break;
    case GX_TF_I8:
        if (convert_i8_to_cmpr(replacementData, newTextureDataOut)) {
            recolor_cmpr_texture(replacementData, color, newTextureDataOut);
        } else {
            mods::log::debug("Could not convert {} from i8 to cmpr", replacementData.textureName);
        }
        break;
    default:
        break;
    }
}

std::unordered_map<ConfigVarHandle, std::list<TextureReplacementData>>& get_texture_replacements() {
    static std::unordered_map<ConfigVarHandle, std::list<TextureReplacementData>> replacements{};

    if (replacements.empty()) {
        replacements = {
            {get_cvars().herosTunicCapColor, {
                {
                    .arc = "Kmdl",
                    .modelFileName = "al_head.bmd",
                    .textureName = "al_cap",
                }
            }},
            {get_cvars().herosTunicTorsoColor, {
                {
                    .arc = "Kmdl",
                    .modelFileName = "al.bmd",
                    .textureName = "al_upbody",
                }
            }},
            {get_cvars().herosTunicSkirtColor, {
                {
                    .arc = "Kmdl",
                    .modelFileName = "al.bmd",
                    .textureName = "al_lowbody",
                }
            }},
            {get_cvars().zoraArmorCapColor, {
                {
                    .arc = "Zmdl",
                    .modelFileName = "zl_head.bmd",
                    .textureName = "zl_cap",
                }
            }},
            {get_cvars().zoraArmorHelmetColor, {
                {
                    .arc = "Zmdl",
                    .modelFileName = "zl_head.bmd",
                    .textureName = "zl_helmet",
                }
            }},
            {get_cvars().zoraArmorTorsoColor, {
                {
                    .arc = "Zmdl",
                    .modelFileName = "zl.bmd",
                    .textureName = "zl_armor",
                },
                {
                    .arc = "Zmdl",
                    .modelFileName = "zl.bmd",
                    .textureName = "zl_armL",
                }
            }},
            {get_cvars().zoraArmorScalesColor, {
                {
                    .arc = "Zmdl",
                    .modelFileName = "zl.bmd",
                    .textureName = "zl_body",
                }
            }},
            {get_cvars().zoraArmorFlippersColor, {
                {
                    .arc = "Zmdl",
                    .modelFileName = "zl.bmd",
                    .textureName = "zl_boots",
                }
            }},
            {get_cvars().woodenSwordColor, {
                {
                    .arc = "Bmdl", // Ordon Clothes Model
                    .modelFileName = "al_swb.bmd",
                    .textureName = "al_SWB",
                },
                {
                    .arc = "Kmdl", // Hero's Tunic Model
                    .modelFileName = "al_swb.bmd",
                    .textureName = "al_SWB",
                },
                {
                    .arc = "Zmdl", // Zora Armor Model
                    .modelFileName = "al_swb.bmd",
                    .textureName = "al_SWB",
                },
                {
                    .arc = "Mmdl", // Magic Armor Model
                    .modelFileName = "al_swb.bmd",
                    .textureName = "al_SWB",
                },
                {
                    .arc = "O_gD_SWB", // Get Item Model
                    .modelFileName = "o_gd_al_swb.bmd",
                    .textureName = "al_SWB",
                }
            }},
            {get_cvars().ordonSwordHandleColor, {
                {
                    .arc = "Alink",
                    .modelFileName = "al_swa.bmd",
                    .textureName = "al_SWgripA",
                },
                {
                    .arc = "O_gD_SWA", // Get Item Model
                    .modelFileName = "o_gd_al_swa.bmd",
                    .textureName = "al_SWgripA",
                }
            }},
            {get_cvars().ordonSwordBladeColor, {
                {
                    .arc = "Alink",
                    .modelFileName = "al_swa.bmd",
                    .textureName = "al_SWA",
                }
            }},
            {get_cvars().msHandleColor, {
                {
                    .arc = "Alink",
                    .modelFileName = "al_swm.bmd",
                    .textureName = "al_SWgripM",
                }
            }},
            {get_cvars().msBladeColor, {
                {
                    .arc = "Alink",
                    .modelFileName = "al_swm.bmd",
                    .textureName = "al_SWM",
                }
            }},
            {get_cvars().boomerangColor, {
                {
                    .arc = "Alink", // Boomerang in Link's hand
                    .modelFileName = "al_boom.bmd",
                    .textureName = "L_al_boom00",
                },
                {
                    .arc = "E_mk", // Boomerang in Ook's hand
                    .modelFileName = "bm.bmd",
                    .textureName = "L_al_boom00",
                },
                {
                    .arc = "E_mk", // Boomerang in Ook's hand
                    .modelFileName = "bm.bmd",
                    .textureName = "bm_boom",
                },
                {
                    .arc = "O_gD_boom", // Get Item Model
                    .modelFileName = "o_gd_boom.bmd",
                    .textureName = "L_al_boom00",
                }
            }},
            {get_cvars().ironBootsColor, {
                {
                    .arc = "Bmdl", // Ordon Clothes Model
                    .modelFileName = "al_bootsh.bmd",
                    .textureName = "al_bootsH",
                },
                {
                    .arc = "Kmdl", // Hero's Tunic Model
                    .modelFileName = "al_bootsh.bmd",
                    .textureName = "al_bootsH",
                },
                {
                    .arc = "Zmdl", // Zora Armor Model
                    .modelFileName = "al_bootsh.bmd",
                    .textureName = "al_bootsH",
                },
                {
                    .arc = "Mmdl", // Magic Armor Model
                    .modelFileName = "al_bootsh.bmd",
                    .textureName = "al_bootsH",
                },
                {
                    .arc = "O_gD_boot", // Get Item Model
                    .modelFileName = "o_gd_al_bootsh.bmd",
                    .textureName = "al_bootsH",
                }
            }},
            {get_cvars().spinnerColor, {
                {
                    .arc = "Alink", // Spinner used by Link
                    .modelFileName = "al_sp.bmd",
                    .textureName = "al_SP",
                },
                {
                    .arc = "O_gD_SP", // Get Item Model
                    .modelFileName = "o_gd_al_sp.bmd",
                    .textureName = "al_SP",
                }
            }},
            {get_cvars().linkHairColor, {
                {
                    .arc = "Bmdl", // Ordon Clothes Model
                    .modelFileName = "bl_head.bmd",
                    .textureName = "bl_hair",
                },
                {
                    .arc = "Kmdl", // Hero's Tunic Model
                    .modelFileName = "al_head.bmd",
                    .textureName = "al_hair",
                },
                {
                    .arc = "Mmdl", // Magic Armor Model
                    .modelFileName = "ml_head.bmd",
                    .textureName = "al_hair",
                }
            }},
            {get_cvars().wolfLinkColor, {
                {
                    .arc = "Wmdl",
                    .modelFileName = "wl.bmd",
                    .textureName = "wl_body",
                },
                {
                    .arc = "Wmdl",
                    .modelFileName = "wl.bmd",
                    .textureName = "wl_eye.1",
                },
                {
                    .arc = "Wmdl",
                    .modelFileName = "wl.bmd",
                    .textureName = "wl_eye.2",
                },
                {
                    .arc = "Wmdl",
                    .modelFileName = "wl.bmd",
                    .textureName = "wl_eye.3",
                },
                {
                    .arc = "Wmdl",
                    .modelFileName = "wl.bmd",
                    .textureName = "wl_eye.4",
                },
                {
                    .arc = "Wmdl",
                    .modelFileName = "wl.bmd",
                    .textureName = "wl_eye.5",
                }
            }},
            {get_cvars().eponaColor, {
                {
                    .arc = "Horse",
                    .modelFileName = "hs.bmd",
                    .textureName = "hs_body",
                },
                {
                    .arc = "Horse",
                    .modelFileName = "hs.bmd",
                    .textureName = "hs_eye.1",
                },
                {
                    .arc = "Horse",
                    .modelFileName = "hs.bmd",
                    .textureName = "hs_eye.2",
                },
                {
                    .arc = "Horse",
                    .modelFileName = "hs.bmd",
                    .textureName = "hs_eye.3",
                },
            }},
        };
    }

    return replacements;
}
