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

void recolor_cmpr_texture(J3DTextureBlock* tex1Ptr, const char* textureName, GXColor color)
{
    ResTIMG* texHeaderPtr = find_tex_header_in_tex_1_section(tex1Ptr, textureName);
    if (texHeaderPtr == nullptr) {
        return;
    }

    if (texHeaderPtr->format != GX_VA_TEX1) {
        // Texture is not CMPR
        return;
    }

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
            {.textures = {"hs_body"}, .hexColor = g_cvars.eponaColor},
        };
        // Wolf Link Color
        cosmeticOverrides[DVDConvertPathToEntrynum("/res/Object/Wmdl.arc")]["bmwr/wl.bmd"] = {
            {.textures = {"wl_body"}, .hexColor = g_cvars.wolfLinkColor},
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
                    recolor_cmpr_texture(tex1Addr, textureName.data(), color);
                }
            }
        }
    }
}