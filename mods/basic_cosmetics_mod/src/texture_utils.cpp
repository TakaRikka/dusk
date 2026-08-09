#include "texture_utils.hpp"
#include "color_utils.hpp"
#include "mod.hpp"

#include "mods/svc/config.h"
#include "mods/svc/log.hpp"

// Forward declaration needed for J3DModelLoader to be happy
class J3DVertexData;
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "JSystem/JSupport/JSupport.h"
#include "JSystem/JUtility/JUTNameTab.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "d/actor/d_a_alink.h"
#include "global.h"
#include "gx/GXEnum.h"
#include "m_Do/m_Do_dvd_thread.h"

#include <list>

ResTIMG* find_tex_header_in_tex_1_section(J3DTextureBlock* tex1Ptr, const char* textureName) {
    if (tex1Ptr == nullptr) {
        return nullptr;
    }

    auto strTable = JSUConvertOffsetToPtr<ResNTAB>(tex1Ptr, tex1Ptr->mpNameTable);
    for (size_t i = 0; i < strTable->mEntryNum && i < tex1Ptr->mTextureNum; i++) {
        const char* str = strTable->getName(i);

        if (strcmp(str, textureName) == 0) {
            return &JSUConvertOffsetToPtr<ResTIMG>(tex1Ptr, tex1Ptr->mpTextureRes)[i];
        }
    }

    return nullptr;
}

void recolor_rgb5a3_texture(ResTIMG* texHeaderPtr, GXColor color)
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

    constexpr int32_t blockWidth = 4;
    constexpr int32_t blockHeight = 4;

    const int32_t roundedWidth = texHeaderPtr->width + ((blockWidth - (texHeaderPtr->width % blockWidth)) % blockWidth);
    const int32_t roundedHeight = texHeaderPtr->height + ((blockHeight - (texHeaderPtr->height % blockHeight)) % blockHeight);

    const int32_t totalPixels = roundedWidth * roundedHeight;

    auto* pixelPtr = reinterpret_cast<BE<uint16_t>*>(JSUConvertOffsetToPtr<u8>(texHeaderPtr, texHeaderPtr->imageOffset));

    for (int32_t i = 0; i < totalPixels; i++) {
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

void recolor_cmpr_texture(ResTIMG* texHeaderPtr, GXColor color)
{
    uint16_t recolors[0x100];
    for (int32_t i = 0; i < 0x100; i++) {
        recolors[i] = blend_overlay_rgb_565(i, color);
    }

    constexpr int32_t blockWidth = 8;
    constexpr int32_t blockHeight = 8;

    const int32_t roundedWidth = texHeaderPtr->width + ((blockWidth - (texHeaderPtr->width % blockWidth)) % blockWidth);
    const int32_t roundedHeight = texHeaderPtr->height + ((blockHeight - (texHeaderPtr->height % blockHeight)) % blockHeight);

    const int32_t numBlocks = roundedWidth / blockWidth * roundedHeight / blockHeight;

    const int32_t iterations = numBlocks * 4;

    uint8_t* currentAddr = JSUConvertOffsetToPtr<u8>(texHeaderPtr, texHeaderPtr->imageOffset);
    for (int32_t i = 0; i < iterations; i++) {
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
                if ((leftNewRgb565 & 0x1f) == 0)
                {
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

bool convert_i8_to_cmpr(ResTIMG* texHeaderPtr) {

    const uint16_t width = texHeaderPtr->width;
    const uint16_t height = texHeaderPtr->height;

    if (width % 8 != 0 || height % 8 != 0) {
        return false;
    }

    const uint32_t tilesX = width / 8;
    const uint32_t blocksY_CMPR = height / 8;

    auto* imageData = JSUConvertOffsetToPtr<uint8_t>(texHeaderPtr, texHeaderPtr->imageOffset);
    uint8_t* writePtr = imageData;

    // Process image in 8x8 blocks
    for (uint32_t by = 0; by < blocksY_CMPR; ++by) {
        for (uint32_t bx = 0; bx < tilesX; ++bx) {
            // Locate the two stacked 8x4 I8 tiles (32 bytes each) making up the 8x8 block
            const uint32_t topTileIdx = (2 * by) * tilesX + bx;
            const uint32_t bottomTileIdx = (2 * by + 1) * tilesX + bx;

            const uint8_t* topTileData = imageData + (topTileIdx * 32);
            const uint8_t* bottomTileData = imageData + (bottomTileIdx * 32);

            uint8_t subBlockPixels[4][16];

            // Extract Sub-block 0 (Top-Left) and Sub-block 1 (Top-Right)
            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 4; ++col) {
                    subBlockPixels[0][row * 4 + col] = topTileData[row * 8 + col];
                    subBlockPixels[1][row * 4 + col] = topTileData[row * 8 + col + 4];
                }
            }

            // Extract Sub-block 2 (Bottom-Left) and Sub-block 3 (Bottom-Right)
            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 4; ++col) {
                    subBlockPixels[2][row * 4 + col] = bottomTileData[row * 8 + col];
                    subBlockPixels[3][row * 4 + col] = bottomTileData[row * 8 + col + 4];
                }
            }

            // Encode the 4 sub-blocks in CMPR order (32 bytes per 8x8 block)
            for (const auto& subBlockPixel : subBlockPixels) {
                encode_cmpr_sub_block(writePtr, subBlockPixel);
                writePtr += 8;
            }
        }
    }

    // Update ResTIMG header metadata
    texHeaderPtr->format = GX_TF_CMPR; // 0x0E
    texHeaderPtr->colorFormat = 0;
    texHeaderPtr->numColors = 0;
    texHeaderPtr->paletteOffset = 0;

    return true;
}

void recolor_texture(J3DTextureBlock* tex1Ptr, const char* textureName, GXColor color) {
    ResTIMG* texHeaderPtr = find_tex_header_in_tex_1_section(tex1Ptr, textureName);
    if (texHeaderPtr == nullptr) {
        return;
    }

    switch (texHeaderPtr->format) {
    case GX_TF_CMPR:
        recolor_cmpr_texture(texHeaderPtr, color);
        break;
    case GX_TF_RGB5A3:
        recolor_rgb5a3_texture(texHeaderPtr, color);
        break;
    case GX_TF_I8:
        if (convert_i8_to_cmpr(texHeaderPtr)) {
            recolor_cmpr_texture(texHeaderPtr, color);
        } else {
            mods::log::debug("Could not convert {} from i8 to cmpr", textureName);
        }
        break;
    default:
        break;
    }
}

J3DTextureBlock* find_tex_1_in_bmd(J3DModelFileData* bmdPtr)
{
    if (bmdPtr == nullptr) {
        return nullptr;
    }

    if (bmdPtr->mMagic1 != MULTI_CHAR('J3D2')) {
        // Model was not a BMD or BDL!
        return nullptr;
    }

    if (bmdPtr->mMagic2 != MULTI_CHAR('bmd3') && bmdPtr->mMagic2 != MULTI_CHAR('bdl4')) {
        // Model was not a BMD or BDL!
        return nullptr;
    }

    J3DModelBlock* curBlock = bmdPtr->mBlocks;
    for (int32_t i = 0; i < bmdPtr->mBlockNum; i++) {
        if (curBlock->mBlockType == MULTI_CHAR('TEX1')) {
            return static_cast<J3DTextureBlock*>(curBlock);
        }

        // Line taken from J3DModelLoader.cpp
        curBlock = (J3DModelBlock*)((uintptr_t)curBlock + curBlock->mBlockSize);
    }

    return nullptr;
}

struct CosmeticOverride {
    std::list<std::string_view> textures{};
    ConfigVarHandle hexColor{0};
};

auto& get_cosmetic_overrides() {
    static std::unordered_map<s32, std::unordered_map<std::string_view, std::list<CosmeticOverride>>> cosmeticOverrides{};
    if (cosmeticOverrides.empty()) {
        auto& g_cvars = get_cvars();
        // Ordon Clothes Link Model
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Bmdl.arc")]["bmwr/al_swb.bmd"] = {
            {.textures = {"al_SWB"},  .hexColor = g_cvars.woodenSwordColor},
        };
        // Main Link Model
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Kmdl.arc")]["bmwr/al_head.bmd"] = {
            {.textures = {"al_cap"},  .hexColor = g_cvars.herosTunicCapColor},
            {.textures = {"al_hair"}, .hexColor = g_cvars.linkHairColor},
        };
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Kmdl.arc")]["bmwr/al.bmd"] = {
            {.textures = {"al_upbody"},  .hexColor = g_cvars.herosTunicTorsoColor},
            {.textures = {"al_lowbody"}, .hexColor = g_cvars.herosTunicSkirtColor},
        };
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Kmdl.arc")]["bmwr/al_bootsh.bmd"] = {
            {.textures = {"al_bootsH"},  .hexColor = g_cvars.ironBootsColor},
        };
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Kmdl.arc")]["bmwr/al_swb.bmd"] = {
            {.textures = {"al_SWB"},  .hexColor = g_cvars.woodenSwordColor},
        };
        // Zora Armor Link Model
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Zmdl.arc")]["bmwr/zl_head.bmd"] = {
            {.textures = {"zl_cap"},    .hexColor = g_cvars.zoraArmorCapColor},
            {.textures = {"zl_helmet"}, .hexColor = g_cvars.zoraArmorHelmetColor},
        };
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Zmdl.arc")]["bmwr/zl.bmd"] = {
            {.textures = {"zl_armor", "zl_armL"}, .hexColor = g_cvars.zoraArmorTorsoColor},
            {.textures = {"zl_body"},             .hexColor = g_cvars.zoraArmorScalesColor},
            {.textures = {"zl_boots"},            .hexColor = g_cvars.zoraArmorFlippersColor},
        };
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Zmdl.arc")]["bmwr/al_bootsh.bmd"] = {
            {.textures = {"al_bootsH"},  .hexColor = g_cvars.ironBootsColor},
        };
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Zmdl.arc")]["bmwr/al_swb.bmd"] = {
            {.textures = {"al_SWB"},  .hexColor = g_cvars.woodenSwordColor},
        };
        // Zora Armor field model
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/O_gD_zora.arc")]["bmdr/o_gd_al_zora.bmd"] = {
            {.textures = {"zl_armor"}, .hexColor = g_cvars.zoraArmorTorsoColor},
            {.textures = {"zl_body"},  .hexColor = g_cvars.zoraArmorScalesColor},
            {.textures = {"zl_helmet"}, .hexColor = g_cvars.zoraArmorHelmetColor},
            {.textures = {"zl_cap"},    .hexColor = g_cvars.zoraArmorCapColor},
        };
        // Magic Armor Model
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Mmdl.arc")]["bmwr/al_bootsh.bmd"] = {
            {.textures = {"al_bootsH"},  .hexColor = g_cvars.ironBootsColor},
        };
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Mmdl.arc")]["bmwr/al_swb.bmd"] = {
            {.textures = {"al_SWB"},  .hexColor = g_cvars.woodenSwordColor},
        };
        // Ordon Sword Colors
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Alink.arc")]["bmwr/al_swa.bmd"] = {
            {.textures = {"al_SWA", "hilight01"}, .hexColor = g_cvars.ordonSwordBladeColor},
            {.textures = {"al_SWgripA"}, .hexColor = g_cvars.ordonSwordHandleColor},
        };
        // Master Sword Colors
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Alink.arc")]["bmwe/al_swm.bmd"] = {
            {.textures = {"al_SWM"}, .hexColor = g_cvars.msBladeColor},
            {.textures = {"al_SWgripM"}, .hexColor = g_cvars.msHandleColor},
        };
        // Boomerang Color
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Alink.arc")]["bmdr/al_boom.bmd"] = {
            {.textures = {"L_al_boom00"}, .hexColor = g_cvars.boomerangColor},
        };
        // Spinner Color
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Alink.arc")]["bmdr/al_sp.bmd"] = {
            {.textures = {"al_SP"}, .hexColor = g_cvars.spinnerColor},
        };
        // Epona Color
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Horse.arc")]["bmdr/hs.bmd"] = {
            {.textures = {"hs_body", "hs_eye.1", "hs_eye.2", "hs_eye.3"}, .hexColor = g_cvars.eponaColor},
        };
        // Wolf Link Color
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Wmdl.arc")]["bmwr/wl.bmd"] = {
            {.textures = {"wl_body", "wl_eye.1", "wl_eye.2", "wl_eye.3", "wl_eye.4", "wl_eye.5"}, .hexColor = g_cvars.wolfLinkColor},
        };
        // Wooden Sword Item Model
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/O_gD_SWB.arc")]["bmdr/o_gd_al_swb.bmd"] = {
            {.textures = {"al_SWB"},  .hexColor = g_cvars.woodenSwordColor},
        };
        // Boomerang Item Model
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/O_gD_boom.arc")]["bmdr/o_gd_boom.bmd"] = {
            {.textures = {"L_al_boom00"}, .hexColor = g_cvars.boomerangColor},
        };
        // Iron Boots Item Model
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/O_gD_boot.arc")]["bmwr/o_gd_al_bootsh.bmd"] = {
            {.textures = {"al_bootsH"},  .hexColor = g_cvars.ironBootsColor},
        };
        // Spinner Item Model
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/O_gD_SP.arc")]["bmdr/o_gd_al_sp.bmd"] = {
            {.textures = {"al_SP"}, .hexColor = g_cvars.spinnerColor},
        };
        // Gale Boomerang for Ook
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/E_mk.arc")]["bmdr/bm.bmd"] = {
            {.textures = {"L_al_boom00", "bm_boom"}, .hexColor = g_cvars.boomerangColor},
        };
    }
    return cosmeticOverrides;
}

s32 get_entry_number(mDoDvdThd_mountArchive_c* mountArchive) {
    return mountArchive->mEntryNumber;
}

void handle_texture_overrides_on_load(mDoDvdThd_mountArchive_c* mountArchive) {

    auto entryNum = get_entry_number(mountArchive);
    auto& cosmeticOverrides = get_cosmetic_overrides();
    if (!cosmeticOverrides.contains(entryNum)) {
        return;
    }

    for (const auto& [resName, overrides] : cosmeticOverrides[entryNum]) {

        auto* archive = mountArchive->getArchive();
        auto* entry = archive->findFsResource(resName.data(), 0);
        if (!entry) {
            continue;
        }

        auto* tex1Addr = find_tex_1_in_bmd(static_cast<J3DModelFileData*>(archive->fetchResource(entry, NULL)));
        if (!tex1Addr) {
            continue;
        }

        for (const auto& cosmeticOverride : overrides) {
            const auto& [textures, hexColorVar] = cosmeticOverride;
            const auto& hexColorStr = get_str_option(hexColorVar, "");
            if (!is_valid_hex_color_str(hexColorStr)) {
                mods::log::debug("Invalid Hex Str {}", hexColorStr);
                continue;
            }

            auto color = hex_color_str_to_gx_color(hexColorStr);
            if (tex1Addr) {
                for (const auto& textureName : textures) {
                    recolor_texture(tex1Addr, textureName.data(), color);
                }
            }
        }
    }
}