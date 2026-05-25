#include <cmath>
#include <SSystem/SComponent/c_xyz.h>
#include <d/d_com_inf_game.h>
#include <d/actor/d_a_player.h>
#include <d/actor/d_a_alink.h>
#include <dusk/gamepad_color.h>

#include "ui/controller_config.hpp"

#include <pad.h>

namespace dusk::input {

namespace {
    cXyz currentColor = {0, 0, 0};
    float lerpSpeed = 0.0f;

    enum ColorVariations : u8 {
        NO_COLOR = 0,
        DUSK_COLOR = 1,
        ZHINT = 2,
        LINK_WOLF = 3,
        LINK_CASUAL = 4,
        LINK_KOKIRI = 5,
        LINK_ZORA = 6,
        LINK_MAGIC = 7,
        LINK_MAGIC_HEAVY = 8,
    };

    struct ColorSetting {
        cXyz color;
        float speed;
    };

    const ColorSetting kColorTable[] = {
        /* 0: NO_COLOR         */ { {0, 0, 0},       2.0f },
        /* 1: DUSK_COLOR       */ { {50, 50, -50},   2.0f },
        /* 2: ZHINT            */ { {50, 50, 175},   2.0f },
        /* 3: LINK_WOLF        */ { {115, 115, 75},  5.0f },
        /* 4: LINK_CASUAL      */ { {235, 230, 115}, 5.0f },
        /* 5: LINK_KOKIRI      */ { {0, 100, 0},     5.0f },
        /* 6: LINK_ZORA        */ { {0, 0, 100},     5.0f },
        /* 7: LINK_MAGIC       */ { {100, 0, 5},     5.0f },
        /* 8: LINK_MAGIC_HEAVY */ { {5, 100, 100},   5.0f },
    };

    cXyz LerpColor(const cXyz a, const cXyz b, const float t) {
        return {std::lerp(a.x, b.x, t), std::lerp(a.y, b.y, t), std::lerp(a.z, b.z, t)};
    }

    void clamp(cXyz& color) {
        color.x = std::clamp(color.x, 0.0f, 255.0f);
        color.y = std::clamp(color.y, 0.0f, 255.0f);
        color.z = std::clamp(color.z, 0.0f, 255.0f);
    }

    ColorSetting getColorSetting() {
        const fopAc_ac_c* zHint = dComIfGp_att_getZHint();
        if (zHint != nullptr) {
            return kColorTable[ZHINT];
        }

        const daAlink_c* link = daAlink_getAlinkActorClass();

        if (link == nullptr)
            return kColorTable[DUSK_COLOR];

        if (link->checkWolf()) {
            return kColorTable[LINK_WOLF];
        }

        switch (dComIfGs_getSelectEquipClothes()) {
        case dItemNo_WEAR_KOKIRI_e:
            return kColorTable[LINK_KOKIRI];
        case dItemNo_WEAR_ZORA_e:
            return kColorTable[LINK_ZORA];
        case dItemNo_ARMOR_e:
            if (link->checkMagicArmorHeavy()) {
                return kColorTable[LINK_MAGIC_HEAVY];
            }

            return kColorTable[LINK_MAGIC];
        case dItemNo_WEAR_CASUAL_e:
            return kColorTable[LINK_CASUAL];
        default:
            return kColorTable[LINK_KOKIRI];
        }
    }

    cXyz getAdditionalColor() {
        if (dKy_darkworld_check()) {
            return kColorTable[DUSK_COLOR].color;
        }

        return kColorTable[NO_COLOR].color;
    }

}  // namespace

bool pad_has_led(const int port) noexcept {
    if (port > PAD_MAX_CONTROLLERS || port < 0)
        return false;

    return PADHasLED(port);
}

void handleGamepadColor() {
    auto [color, speed] = getColorSetting();
    const cXyz additionalColor = getAdditionalColor();
    cXyz finalColor = color + additionalColor;
    lerpSpeed = speed / 30.0f;

    clamp(finalColor);
    currentColor = LerpColor(currentColor, finalColor, lerpSpeed);

    for (int i = 0; i < 4; i++) {
        if (pad_has_led(i) && getSettings().game.enableLED[i]) {
            PADSetColor(
                i,
                static_cast<u8>(currentColor.x),
                static_cast<u8>(currentColor.y),
                static_cast<u8>(currentColor.z)
            );
        }
    }
}

}  // namespace dusk::input