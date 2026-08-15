#include "mod.hpp"
#include "color_utils.hpp"
#include "hooks.hpp"
#include "midna_hair_color.hpp"
#include "option_descriptions.hpp"
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
    if (handle == 0 || svc_config->get_string(mod_ctx, handle, value.data(), value.size() + 1, &outLength) != MOD_OK) {
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
std::optional<GXColor> get_config_var_color(ConfigVarHandle handle, bool allowRainbow /*= false*/) {
    auto colorStr = get_str_option(handle, "");

    // Convert to lowercase
    for (auto& c : colorStr) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    if (is_valid_hex_color_str(colorStr) || (colorStr == "rainbow" && allowRainbow)) {
        if (is_valid_hex_color_str(colorStr)) {
            return hex_color_str_to_gx_color(colorStr);
        }

        if (allowRainbow) {
            // Assume rainbow if not a valid hex str
            auto color = get_rainbow_rgb(127.5f);
            color.r /= 2;
            color.g /= 2;
            color.b /= 2;
            return color;
        }
    }

    return std::nullopt;
}

namespace {
UiWindowHandle g_cosmeticsWindow = 0;
bool g_loadedAllBaseTextures = false;

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

void add_cosmetic_option(UiElementHandle left, ConfigVarHandle cvar, const char* name, const char* helpRml) {
    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_STRING;
    control.label = name;
    control.help_rml = helpRml;
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = cvar;
    control.max_length = 6;
    add_control(left, control);
}

void add_cosmetic_option_with_rainbow(UiElementHandle left, ConfigVarHandle cvar, const char* name, const char* helpRml) {
    UiControlDesc control = UI_CONTROL_DESC_INIT;
    control.kind = UI_CONTROL_STRING;
    control.label = name;
    control.help_rml = helpRml;
    control.binding = UI_BINDING_CONFIG_VAR;
    control.config_var = cvar;
    control.max_length = 7;
    add_control(left, control);
}

void add_midna_hair_option(UiElementHandle left, ConfigVarHandle cvar, const std::string& name) {
    static const char* kMidnaHairOptions[] = {
        "Default", "Pink", "Red", "Yellow", "Green", "Blue", "Purple", "Brown", "White", "Black"
    };
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

ModResult build_equipment_colors_tab(
    ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle right, void*, ModError*) {
    (void)right;

    add_cosmetic_option(left, g_cvars.herosTunicCapColor, "Hero's Tunic Cap Color", kLinksCapColorsExplanation);
    add_cosmetic_option(left, g_cvars.herosTunicTorsoColor, "Hero's Tunic Body Color", kLinksShirtColorsExplanation);
    add_cosmetic_option(left, g_cvars.herosTunicSkirtColor, "Hero's Tunic Skirt Color", kLinksSkirtColorsExplanation);
    add_cosmetic_option(left, g_cvars.zoraArmorCapColor, "Zora Armor Cap Color", kZoraCapColorsExplanation);
    add_cosmetic_option(left, g_cvars.zoraArmorHelmetColor, "Zora Armor Helmet Color", kZoraHelmetColorsExplanation);
    add_cosmetic_option(left, g_cvars.zoraArmorTorsoColor, "Zora Armor Torso Color", kZoraTorsoColorsExplanation);
    add_cosmetic_option(left, g_cvars.zoraArmorScalesColor, "Zora Armor Scales Color", kZoraScalesColorsExplanation);
    add_cosmetic_option(left, g_cvars.zoraArmorFlippersColor, "Zora Armor Flippers Color", kZoraFlippersColorsExplanation);
    add_cosmetic_option_with_rainbow(left, g_cvars.lanternGlowColor, "Lantern Glow Color", kLanternGlowColorsExplanation);
    add_cosmetic_option(left, g_cvars.woodenSwordColor, "Wooden Sword Color", kWoodenSwordColorsExplanation);
    add_cosmetic_option(left, g_cvars.ordonSwordBladeColor, "Ordon Sword Blade Color", kOrdonBladeColorsExplanation);
    add_cosmetic_option(left, g_cvars.ordonSwordHandleColor, "Ordon Sword Handle Color", kOrdonHandleColorsExplanation);
    add_cosmetic_option(left, g_cvars.msBladeColor, "Master Sword Blade Color", kMSBladeColorsExplanation);
    add_cosmetic_option(left, g_cvars.msHandleColor, "Master Sword Handle Color", kMSHandleColorsExplanation);
    add_cosmetic_option_with_rainbow(left, g_cvars.lightSwordGlowColor, "Light Sword Glow Color", kSwordGlowColorsExplanation);
    add_cosmetic_option(left, g_cvars.boomerangColor, "Boomerang Color", kBoomerangColorsExplanation);
    add_cosmetic_option(left, g_cvars.ironBootsColor, "Iron Boots Color", kIronBootsColorsExplanation);
    add_cosmetic_option(left, g_cvars.spinnerColor, "Spinner Color", kSpinnerColorsExplanation);

    return MOD_OK;
}

ModResult build_ui_colors_tab(
    ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle right, void*, ModError*) {
    (void)right;

    add_cosmetic_option(left, g_cvars.aButtonColor, "A Button Color", kAButtonColorsExplanation);
    add_cosmetic_option(left, g_cvars.bButtonColor, "B Button Color", kBButtonColorsExplanation);
    add_cosmetic_option(left, g_cvars.xButtonColor, "X Button Color", kXButtonColorsExplanation);
    add_cosmetic_option(left, g_cvars.yButtonColor, "Y Button Color", kYButtonColorsExplanation);
    add_cosmetic_option(left, g_cvars.zButtonColor, "Z Button Color", kZButtonColorsExplanation);
    add_cosmetic_option(left, g_cvars.heartColor, "Heart Color", kHeartColorsExplanation);

    return MOD_OK;
}

ModResult build_misc_colors_tab(
    ModContext*, UiWindowHandle, UiElementHandle left, UiElementHandle right, void*, ModError*) {
    (void)right;

    add_midna_hair_option(left, g_cvars.midnaHairBaseColor, "Midna's Hair Base Color");
    add_midna_hair_option(left, g_cvars.midnaHairTipsColor, "Midna's Hair Tips Color");
    add_cosmetic_option(left, g_cvars.midnaChargeRingColor, "Midna Charge Ring Color", kChargeRingColorsExplanation);
    add_cosmetic_option(left, g_cvars.linkHairColor, "Link's Hair Color", kLinksHairColorsExplanation);
    add_cosmetic_option(left, g_cvars.wolfLinkColor, "Wolf Link Color", kWolfLinkColorsExplanation);
    add_cosmetic_option(left, g_cvars.eponaColor, "Epona Color", kEponaColorsExplanation);

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
    tabs[0].title = "Equipment Colors";
    tabs[0].build = build_equipment_colors_tab;
    tabs[1].title = "UI Colors";
    tabs[1].build = build_ui_colors_tab;
    tabs[2].title = "Misc Colors";
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
    control.kind = UI_CONTROL_BUTTON;
    control.label = "Open Cosmetics Menu";
    control.on_pressed = on_open_cosmetics_menu;
    add_control(panel, control);
    return MOD_OK;
}
}

void load_base_texture_data() {

    // Go through each texture we can recolor and attempt to load and store
    // the texture data for future recoloring
    for (auto& replacements : get_texture_replacements() | std::views::values) {
        for (auto& replacement : replacements) {
            // If we've already loaded the texture data, don't load it again
            if (replacement.loadedTextureData) {
                continue;
            }

            // Try to get the model this texture is part of. If we can't get it, try again later
            auto model = static_cast<J3DModelData*>(dComIfG_getObjectRes(replacement.arc, replacement.modelFileName));
            if (model == nullptr) {
                continue;
            }

            J3DTexture* tex = model->getTexture();
            JUTNameTab* nametable = model->getTextureName();
            if (tex != nullptr && nametable != nullptr) {
                for (u16 i = 0; i < tex->getNum(); i++) {
                    const char* texName = nametable->getName(i);
                    if (texName != nullptr && std::strcmp(texName, replacement.textureName) == 0) {

                        // Once we've found the texture, set all our TextureKey and TextureData fields
                        // that we can set right now.
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
                        auto size = get_image_data_size(imageHeader->format, imageHeader->width,
                            imageHeader->height, imageHeader->mipmapEnabled ? imageHeader->mipmapCount : 1);
                        replacement.baseTextureData.resize(size);
                        std::memcpy(replacement.baseTextureData.data(), tex->getImgDataPtr(i), size);

                        // Calculate the texture hash
                        auto textureHash = XXH64(replacement.baseTextureData.data(), size, 0);
                        replacement.key.texture_hash = textureHash;

                        mods::log::debug("Loaded base texture data for {}. size: {:X} hash: {:X}", replacement.textureName, size, textureHash);
                        replacement.loadedTextureData = true;

                        data.width = imageHeader->width;
                        data.height = imageHeader->height;
                        data.mip_count = imageHeader->mipmapCount;
                        data.size = size;
                        data.gx_format = imageHeader->format;

                        break;
                    }
                }
            }
        }
    }

    // If we've loaded all base textures for recoloring, then we don't need to call this function again
    g_loadedAllBaseTextures = std::ranges::all_of(get_texture_replacements() | std::views::values, [](auto& replacementList) {
        return std::ranges::all_of(replacementList, [](const TextureReplacementData& replacement) {
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

        // If we don't have a valid color, don't continue either
        auto maybeColor = get_config_var_color(configVar);
        if (!maybeColor.has_value()) {
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
            if (curColor == std::nullopt ||
                curColor.value().r != color.r || curColor.value().g != color.g || curColor.value().b != color.b)
            {
                // Make a copy of the base texture data to recolor
                auto newTexture = replacement.baseTextureData;
                recolor_texture(replacement, color, newTexture);

                replacement.data.data = newTexture.data();
                replacement.data.size = newTexture.size();

                // Register the new data
                auto result = svc_texture->register_data(mod_ctx, &replacement.key, &replacement.data, &replacement.handle);
                if (result != MOD_OK) {
                    mods::log::debug("Could not register_data for {}. Result: {}", replacement.textureName, static_cast<int>(result));
                } else {
                    mods::log::debug("Registered replacement for {}.", replacement.textureName, static_cast<int>(result));
                }

                curColor = color;
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
            }
        }
    }
}

#define REGISTER_COSMETIC_OPTION(option) \
    result = register_str_option(#option, NULL, g_cvars.option, error); \
    if (result != MOD_OK) { \
        return result; \
    } \

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
    return MOD_OK;
}

MOD_EXPORT ModResult mod_update(ModError*) {
    update_rainbow_rgb(1.0f);
    set_all_midna_hair_colors();

    if (!g_loadedAllBaseTextures) {
        load_base_texture_data();
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
    return MOD_OK;
}

}
