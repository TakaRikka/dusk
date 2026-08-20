#include "mod.hpp"
#include "color_utils.hpp"
#include "hooks.hpp"
#include "midna_hair_color.hpp"
#include "texture_utils.hpp"

#include "mods/service.hpp"
#include "mods/svc/config.h"
#include "mods/svc/hook.hpp"
#include "mods/svc/log.h"
#include "mods/svc/log.hpp"
#include "mods/svc/ui.h"

#include "d/d_com_inf_game.h"

#include <xxhash.h>

#include <optional>
#include <ranges>
#include <string>

DEFINE_MOD();
IMPORT_SERVICE(LogService, svc_log);
IMPORT_SERVICE(ConfigService, svc_config);
IMPORT_SERVICE(HookService, svc_hook);
IMPORT_SERVICE(TextureService, svc_texture);
IMPORT_SERVICE(UiService, svc_ui);

static cvars g_cvars;

cvars& get_cvars() {
    return g_cvars;
}

std::string get_str_option(ConfigVarHandle handle, const std::string& fallback) {
    std::string value{};
    size_t outLength{};
    svc_config->get_string(mod_ctx, handle, NULL, 0, &outLength);
    value.resize(outLength);
    if (handle == 0 || svc_config->get_string(
                           mod_ctx, handle, value.data(), value.size() + 1, &outLength) != MOD_OK)
    {
        return fallback;
    }
    return value;
}

int64_t get_int_option(ConfigVarHandle handle, int64_t fallback) {
    int64_t value = fallback;
    if (handle == 0 || svc_config->get_int(mod_ctx, handle, &value) != MOD_OK) {
        return fallback;
    }
    return value;
}

// Helper for getting configVar color
std::optional<GXColor> get_config_var_color(ConfigVarHandle handle, bool allowRainbow) {
    auto colorStr = get_str_option(handle, "");
    // Convert to lowercase
    for (auto& c : colorStr) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    if (colorStr == "rainbow" && allowRainbow) {
        auto color = get_rainbow_rgb(127.5f);
        color.r /= 2;
        color.g /= 2;
        color.b /= 2;
        return color;
    }
    if (is_valid_hex_color_str(colorStr)) {
        return hex_color_str_to_gx_color(colorStr);
    }
    return std::nullopt;
}

namespace {
UiWindowHandle g_cosmeticsWindow = 0;
bool g_loadedAllBaseTextures = false;
constexpr uint32_t kTextureLoadRetryFrames = 30;
uint32_t g_textureLoadRetryCountdown = 0;

constexpr const char* kOverlayPresets[] = {
    "ab706e",
    "6382a0",
    "94749a",
    "ec8644",
    "b9ab00",
    "ec9fc8",
    "505154",
    "f8f7f4",
    "91723e",
};

constexpr const char* kLightPresets[] = {
    "ff0000",
    "f68821",
    "f6f321",
    "00ff00",
    "0000ff",
    "8000ff",
    "a0a0a0",
    "30d0d0",
};

constexpr const char* kRainbowLightPresets[] = {
    "rainbow",
    "ff0000",
    "f68821",
    "f6f321",
    "00ff00",
    "0000ff",
    "8000ff",
    "a0a0a0",
};

constexpr const char* kAButtonPresets[] = {
    "ff0000",
    "ff5000",
    "ffaf00",
    "0080ff",
    "0000ff",
    "8000ff",
    "5555ff",
    "ff20ff",
};

constexpr const char* kBButtonPresets[] = {
    "ffff40",
    "ffa0ff",
    "00e87b",
    "00aaff",
    "6078ff",
    "000000",
    "00f3ff",
};

constexpr const char* kXyButtonPresets[] = {
    "ff0000",
    "ff8200",
    "f7df00",
    "70ff00",
    "00bd11",
    "0000ff",
    "800088",
    "000000",
    "ff00aa",
    "00ffff",
};

constexpr const char* kZButtonPresets[] = {
    "ff0000",
    "ff8200",
    "f7df00",
    "70ff00",
    "00bd11",
    "800088",
    "000000",
    "00ffff",
};

constexpr const char* kChargeRingPresets[] = {
    "ff9f9f",
    "ff0000",
    "ffff00",
    "00ff00",
    "0000ff",
    "ff00ff",
    "331900",
    "feffff",
    "000000",
};

ModResult register_str_option(
    const char* name, const char* defaultValue, ConfigVarHandle& outHandle, ModError* error) {
    ConfigVarDesc cvarDesc = CONFIG_VAR_DESC_INIT;
    cvarDesc.name = name;
    cvarDesc.type = CONFIG_VAR_STRING;
    cvarDesc.default_string = defaultValue;
    if (svc_config->register_var(mod_ctx, &cvarDesc, &outHandle) != MOD_OK) {
        return mods::set_error(error, MOD_ERROR, "failed to register cosmetics option");
    }
    return MOD_OK;
}

ModResult register_int_option(
    const char* name, int64_t defaultValue, ConfigVarHandle& outHandle, ModError* error) {
    ConfigVarDesc cvarDesc = CONFIG_VAR_DESC_INIT;
    cvarDesc.name = name;
    cvarDesc.type = CONFIG_VAR_INT;
    cvarDesc.default_int = defaultValue;
    if (svc_config->register_var(mod_ctx, &cvarDesc, &outHandle) != MOD_OK) {
        return mods::set_error(error, MOD_ERROR, "failed to register cosmetics option");
    }
    return MOD_OK;
}

void add_control(UiElementHandle pane, const UiControlDesc& desc) {
    auto result = svc_ui->pane_add_control(mod_ctx, pane, &desc, nullptr);
    if (result != MOD_OK) {
        mods::log::debug("pane_add_control failed {}", static_cast<int>(result));
    }
}

template <size_t N>
void add_cosmetic_option(
    UiElementHandle pane, ConfigVarHandle cvar, const char* name, const char* const (&presets)[N]) {
    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_COLOR;
    control.label = name;
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = cvar;
    control.color_presets = presets;
    control.color_preset_count = N;
    add_control(pane, control);
}

void add_midna_hair_option(UiElementHandle left, ConfigVarHandle cvar, const std::string& name) {
    static const char* kMidnaHairOptions[] = {
        "Default", "Pink", "Red", "Yellow", "Green", "Blue", "Purple", "Brown", "White", "Black"};
    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_SELECT;
    control.label = name.c_str();
    control.help_rml = "Choose Midna's hair color.";
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = cvar;
    control.options = kMidnaHairOptions;
    control.option_count = 10;
    add_control(left, control);
}

void add_group(
    UiElementHandle groups, UiElementHandle colors, const char* label, UiGroupBuildFn build) {
    UiGroupDesc group = UI_GROUP_DESC_INIT;
    group.label = label;
    group.build = build;
    const auto result = svc_ui->pane_add_group(mod_ctx, groups, colors, &group, nullptr);
    if (result != MOD_OK) {
        mods::log::debug("pane_add_group failed {}", static_cast<int>(result));
    }
}

ModResult build_hero_tunic_colors(ModContext*, UiElementHandle pane, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, pane, "Hero's Tunic");
    add_cosmetic_option(pane, g_cvars.herosTunicCapColor, "Cap", kOverlayPresets);
    add_cosmetic_option(pane, g_cvars.herosTunicTorsoColor, "Body", kOverlayPresets);
    add_cosmetic_option(pane, g_cvars.herosTunicSkirtColor, "Skirt", kOverlayPresets);
    return MOD_OK;
}

ModResult build_zora_armor_colors(ModContext*, UiElementHandle pane, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, pane, "Zora Armor");
    add_cosmetic_option(pane, g_cvars.zoraArmorCapColor, "Cap", kOverlayPresets);
    add_cosmetic_option(pane, g_cvars.zoraArmorHelmetColor, "Helmet", kOverlayPresets);
    add_cosmetic_option(pane, g_cvars.zoraArmorTorsoColor, "Torso", kOverlayPresets);
    add_cosmetic_option(pane, g_cvars.zoraArmorScalesColor, "Scales", kOverlayPresets);
    add_cosmetic_option(pane, g_cvars.zoraArmorFlippersColor, "Flippers", kOverlayPresets);
    return MOD_OK;
}

ModResult build_sword_colors(ModContext*, UiElementHandle pane, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, pane, "Swords");
    add_cosmetic_option(pane, g_cvars.woodenSwordColor, "Wooden Sword", kOverlayPresets);
    add_cosmetic_option(pane, g_cvars.ordonSwordBladeColor, "Ordon Blade", kLightPresets);
    add_cosmetic_option(pane, g_cvars.ordonSwordHandleColor, "Ordon Handle", kLightPresets);
    add_cosmetic_option(pane, g_cvars.msBladeColor, "Master Sword Blade", kLightPresets);
    add_cosmetic_option(pane, g_cvars.msHandleColor, "Master Sword Handle", kLightPresets);
    add_cosmetic_option(
        pane, g_cvars.lightSwordGlowColor, "Light Sword Glow", kRainbowLightPresets);
    return MOD_OK;
}

ModResult build_equipment_colors(ModContext*, UiElementHandle pane, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, pane, "Equipment");
    add_cosmetic_option(pane, g_cvars.lanternGlowColor, "Lantern Glow", kRainbowLightPresets);
    add_cosmetic_option(pane, g_cvars.boomerangColor, "Gale Boomerang", kOverlayPresets);
    add_cosmetic_option(pane, g_cvars.ironBootsColor, "Iron Boots", kOverlayPresets);
    add_cosmetic_option(pane, g_cvars.spinnerColor, "Spinner", kOverlayPresets);
    return MOD_OK;
}

ModResult build_button_colors(ModContext*, UiElementHandle pane, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, pane, "Buttons");
    add_cosmetic_option(pane, g_cvars.aButtonColor, "A Button", kAButtonPresets);
    add_cosmetic_option(pane, g_cvars.bButtonColor, "B Button", kBButtonPresets);
    add_cosmetic_option(pane, g_cvars.xButtonColor, "X Button", kXyButtonPresets);
    add_cosmetic_option(pane, g_cvars.yButtonColor, "Y Button", kXyButtonPresets);
    add_cosmetic_option(pane, g_cvars.zButtonColor, "Z Button", kZButtonPresets);
    return MOD_OK;
}

ModResult build_hud_colors(ModContext*, UiElementHandle pane, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, pane, "HUD");
    add_cosmetic_option(pane, g_cvars.heartColor, "Hearts", kBButtonPresets);
    return MOD_OK;
}

ModResult build_link_colors(ModContext*, UiElementHandle pane, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, pane, "Link");
    add_cosmetic_option(pane, g_cvars.linkHairColor, "Hair", kOverlayPresets);
    return MOD_OK;
}

ModResult build_midna_colors(ModContext*, UiElementHandle pane, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, pane, "Midna");
    add_cosmetic_option(pane, g_cvars.midnaChargeRingColor, "Charge Ring", kChargeRingPresets);
    return MOD_OK;
}

ModResult build_companion_colors(ModContext*, UiElementHandle pane, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, pane, "Companions");
    add_cosmetic_option(pane, g_cvars.wolfLinkColor, "Wolf Link", kOverlayPresets);
    add_cosmetic_option(pane, g_cvars.eponaColor, "Epona", kOverlayPresets);
    return MOD_OK;
}

ModResult build_equipment_colors_tab(
    ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle right, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, left, "Color Groups");
    add_group(left, right, "Hero's Tunic", build_hero_tunic_colors);
    add_group(left, right, "Zora Armor", build_zora_armor_colors);
    add_group(left, right, "Swords", build_sword_colors);
    add_group(left, right, "Equipment", build_equipment_colors);

    return MOD_OK;
}

ModResult build_ui_colors_tab(
    ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle right, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, left, "Color Groups");
    add_group(left, right, "Buttons", build_button_colors);
    add_group(left, right, "HUD", build_hud_colors);

    return MOD_OK;
}

ModResult build_misc_colors_tab(
    ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle right, void*, ModError*) {
    svc_ui->pane_add_section(mod_ctx, left, "Hair Presets");
    add_midna_hair_option(left, g_cvars.midnaHairBaseColor, "Midna's Hair Base Color");
    add_midna_hair_option(left, g_cvars.midnaHairTipsColor, "Midna's Hair Tips Color");
    svc_ui->pane_add_section(mod_ctx, left, "Color Groups");
    add_group(left, right, "Link", build_link_colors);
    add_group(left, right, "Midna", build_midna_colors);
    add_group(left, right, "Companions", build_companion_colors);

    return MOD_OK;
}

void on_cosmetics_menu_window_closed(ModContext*, UiWindowHandle, void*) {
    g_cosmeticsWindow = 0;
}

void on_open_cosmetics_menu(ModContext*, void*) {
    if (g_cosmeticsWindow != 0) {
        return;
    }
    UiTabDesc tabs[] = {UI_TAB_DESC_INIT, UI_TAB_DESC_INIT, UI_TAB_DESC_INIT};
    tabs[0].title = "Equipment";
    tabs[0].build = build_equipment_colors_tab;
    tabs[1].title = "Interface";
    tabs[1].build = build_ui_colors_tab;
    tabs[2].title = "Characters";
    tabs[2].build = build_misc_colors_tab;
    UiWindowDesc desc = UI_WINDOW_DESC_INIT;
    desc.tabs = tabs;
    desc.tab_count = ARRAY_SIZE(tabs);
    desc.on_closed = on_cosmetics_menu_window_closed;
    if (svc_ui->window_push(mod_ctx, &desc, &g_cosmeticsWindow) != MOD_OK) {
        svc_log->error(mod_ctx, "failed to open basic cosmetics window");
    }
}

ModResult build_panel(ModContext*, UiElementHandle panel, void*, ModError*) {
    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_GROUP;
    control.label = "Open Cosmetics Menu";
    control.on_pressed = on_open_cosmetics_menu;
    add_control(panel, control);
    return MOD_OK;
}
}  // namespace

void load_base_texture_data() {
    // Go through each texture we can recolor and attempt to load and store
    // the texture data for future recoloring
    for (auto& replacements : get_texture_replacements() | std::views::values) {
        for (auto& replacement : replacements) {
            // If we've already loaded the texture data, don't load it again
            if (replacement.loadedTextureData) {
                continue;
            }

            // Avoid noisy resource lookups until the archive has finished loading.
            auto* resInfo = dComIfG_getObjectResInfo(replacement.arc);
            if (resInfo == nullptr || resInfo->getArchive() == nullptr) {
                continue;
            }

            // Try to get the model this texture is part of. If we can't get it, try again later
            auto model = static_cast<J3DModelData*>(
                dComIfG_getObjectRes(replacement.arc, replacement.modelFileName));
            if (model == nullptr) {
                continue;
            }

            J3DTexture* tex = model->getTexture();
            JUTNameTab* nametable = model->getTextureName();
            if (tex != nullptr && nametable != nullptr) {
                for (u16 i = 0; i < tex->getNum(); i++) {
                    const char* texName = nametable->getName(i);
                    if (texName != nullptr && std::strcmp(texName, replacement.textureName) == 0) {
                        // Once we've found the texture, set all our TextureKey and TextureData
                        // fields that we can set right now.
                        auto imageHeader = tex->getResTIMG(i);
                        auto& key = replacement.key;
                        auto& data = replacement.data;
                        key.kind = TEXTURE_KEY_SOURCE;
                        key.has_tlut = imageHeader->numColors > 0;
                        key.width = imageHeader->width;
                        key.height = imageHeader->height;
                        key.gx_format = imageHeader->format;

                        // Currently, no replaced textures have a tlut
                        key.tlut_hash = replacement.tlutHash;

                        // Calculate the size of the image data
                        const uint32_t mipCount =
                            imageHeader->mipmapEnabled ?
                                std::max<uint32_t>(imageHeader->mipmapCount, 1) :
                                1;
                        auto size = get_image_data_size(
                            imageHeader->format, imageHeader->width, imageHeader->height, mipCount);
                        replacement.baseTextureData.resize(size);
                        std::memcpy(
                            replacement.baseTextureData.data(), tex->getImgDataPtr(i), size);

                        // Source texture keys hash only the base mip level.
                        const auto baseMipSize = get_image_data_size(
                            imageHeader->format, imageHeader->width, imageHeader->height, 1);
                        auto textureHash =
                            XXH64(replacement.baseTextureData.data(), baseMipSize, 0);
                        replacement.key.texture_hash = textureHash;

                        mods::log::debug("Loaded base texture data for {}. size: {:X} hash: {:X}",
                            replacement.textureName, size, textureHash);
                        replacement.loadedTextureData = true;

                        data.width = imageHeader->width;
                        data.height = imageHeader->height;
                        data.mip_count = mipCount;
                        data.size = size;
                        data.gx_format = imageHeader->format;

                        break;
                    }
                }
            }
        }
    }

    // If we've loaded all base textures for recoloring, then we don't need to call this function
    // again
    g_loadedAllBaseTextures = std::ranges::all_of(
        get_texture_replacements() | std::views::values, [](auto& replacementList) {
            return std::ranges::all_of(
                replacementList, [](const TextureReplacementData& replacement) {
                    return replacement.loadedTextureData;
                });
        });
}

ModResult check_and_set_recolored_textures() {
    for (auto& [configVar, replacements] : get_texture_replacements()) {
        // If the configvar hasn't been set, don't continue
        if (configVar == 0) {
            continue;
        }

        auto maybeColor = get_config_var_color(configVar);
        if (!maybeColor.has_value()) {
            // An empty or invalid option means the original texture should be restored.
            for (auto& replacement : replacements) {
                if (replacement.handle != 0) {
                    auto result = svc_texture->unregister(mod_ctx, replacement.handle);
                    if (result != MOD_OK) {
                        mods::log::debug("Could not unregister replacement for {}. Result: {}",
                            replacement.textureName, static_cast<int>(result));
                    }
                    replacement.handle = 0;
                }
                replacement.curColor.reset();
            }
            continue;
        }

        auto color = maybeColor.value();
        for (auto& replacement : replacements) {
            // If we haven't loaded the base texture yet, don't try to recolor it
            if (!replacement.loadedTextureData) {
                continue;
            }

            // If our color hasn't changed, don't try to recolor
            auto& curColor = replacement.curColor;
            if (curColor == std::nullopt || curColor.value().r != color.r ||
                curColor.value().g != color.g || curColor.value().b != color.b)
            {
                // Make a copy of the base texture data to recolor
                auto newTexture = replacement.baseTextureData;
                recolor_texture(replacement, color, newTexture);

                TextureData newTextureData = replacement.data;
                newTextureData.data = newTexture.data();
                newTextureData.size = newTexture.size();

                // Keep the current replacement active if registering the new one fails.
                TextureReplacementHandle newHandle{};
                auto result = svc_texture->register_data(
                    mod_ctx, &replacement.key, &newTextureData, &newHandle);
                if (result != MOD_OK) {
                    mods::log::debug("Could not register_data for {}. Result: {}",
                        replacement.textureName, static_cast<int>(result));
                } else {
                    mods::log::debug("Registered replacement for {}.", replacement.textureName,
                        static_cast<int>(result));
                    auto oldHandle = replacement.handle;
                    replacement.handle = newHandle;
                    curColor = color;

                    if (oldHandle != 0) {
                        result = svc_texture->unregister(mod_ctx, oldHandle);
                        if (result != MOD_OK) {
                            mods::log::debug(
                                "Could not unregister previous replacement for {}. Result: {}",
                                replacement.textureName, static_cast<int>(result));
                        }
                    }
                }
            }
        }
    }

    return MOD_OK;
}

void unregister_all_texture_handles() {
    for (auto& replacements : get_texture_replacements() | std::views::values) {
        for (auto& replacement : replacements) {
            if (replacement.handle != 0) {
                svc_texture->unregister(mod_ctx, replacement.handle);
                replacement.handle = 0;
            }
            replacement.curColor.reset();
        }
    }
}

#define REGISTER_COSMETIC_OPTION(option)                                                           \
    result = register_str_option(#option, NULL, g_cvars.option, error);                            \
    if (result != MOD_OK) {                                                                        \
        return result;                                                                             \
    }

extern "C" {
MOD_EXPORT ModResult mod_initialize(ModError* error) {
    svc_log->info(mod_ctx, "basic_cosmetics_mod initialized");

    ModResult result{};

    REGISTER_COSMETIC_OPTION(herosTunicCapColor)
    REGISTER_COSMETIC_OPTION(herosTunicTorsoColor)
    REGISTER_COSMETIC_OPTION(herosTunicSkirtColor)
    REGISTER_COSMETIC_OPTION(zoraArmorCapColor)
    REGISTER_COSMETIC_OPTION(zoraArmorHelmetColor)
    REGISTER_COSMETIC_OPTION(zoraArmorTorsoColor)
    REGISTER_COSMETIC_OPTION(zoraArmorScalesColor)
    REGISTER_COSMETIC_OPTION(zoraArmorFlippersColor)
    REGISTER_COSMETIC_OPTION(lanternGlowColor)
    REGISTER_COSMETIC_OPTION(woodenSwordColor)
    REGISTER_COSMETIC_OPTION(ordonSwordBladeColor)
    REGISTER_COSMETIC_OPTION(ordonSwordHandleColor)
    REGISTER_COSMETIC_OPTION(msBladeColor)
    REGISTER_COSMETIC_OPTION(msHandleColor)
    REGISTER_COSMETIC_OPTION(lightSwordGlowColor)
    REGISTER_COSMETIC_OPTION(boomerangColor)
    REGISTER_COSMETIC_OPTION(ironBootsColor)
    REGISTER_COSMETIC_OPTION(spinnerColor)
    REGISTER_COSMETIC_OPTION(aButtonColor)
    REGISTER_COSMETIC_OPTION(bButtonColor)
    REGISTER_COSMETIC_OPTION(xButtonColor)
    REGISTER_COSMETIC_OPTION(yButtonColor)
    REGISTER_COSMETIC_OPTION(zButtonColor)
    REGISTER_COSMETIC_OPTION(heartColor)

    result = register_int_option("midnaHairBaseColor", 0, g_cvars.midnaHairBaseColor, error);
    if (result != MOD_OK) {
        return result;
    }

    result = register_int_option("midnaHairTipsColor", 0, g_cvars.midnaHairTipsColor, error);
    if (result != MOD_OK) {
        return result;
    }

    REGISTER_COSMETIC_OPTION(midnaChargeRingColor)
    REGISTER_COSMETIC_OPTION(linkHairColor)
    REGISTER_COSMETIC_OPTION(wolfLinkColor)
    REGISTER_COSMETIC_OPTION(eponaColor)

    UiModsPanelDesc panelDesc = UI_MODS_PANEL_DESC_INIT;
    panelDesc.build = build_panel;
    svc_ui->register_mods_panel(mod_ctx, &panelDesc);

    // Add all our hooks
    result = add_all_hooks();
    if (result != MOD_OK) {
        return result;
    }

    g_loadedAllBaseTextures = false;
    g_textureLoadRetryCountdown = 0;
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    update_rainbow_rgb(1.0f);
    set_all_midna_hair_colors();

    if (!g_loadedAllBaseTextures) {
        if (g_textureLoadRetryCountdown == 0) {
            load_base_texture_data();
            g_textureLoadRetryCountdown = kTextureLoadRetryFrames;
        } else {
            --g_textureLoadRetryCountdown;
        }
    }

    check_and_set_recolored_textures();

    return MOD_OK;
}

MOD_EXPORT ModResult mod_shutdown(ModError*) {
    svc_log->info(mod_ctx, "basic_cosmetics_mod unloaded");
    g_cosmeticsWindow = 0;
    remove_all_hooks();
    unregister_all_texture_handles();
    get_texture_replacements().clear();
    g_loadedAllBaseTextures = false;
    g_textureLoadRetryCountdown = 0;
    return MOD_OK;
}
}
