#include <cmath>
#include <SSystem/SComponent/c_xyz.h>
#include <d/d_com_inf_game.h>
#include <d/actor/d_a_player.h>
#include <d/actor/d_a_alink.h>
#include <dusk/gamepad_color.h>

cXyz currentGamepadColor = {0, 0, 0};
cXyz finalGamepadColor = {0, 0, 0};
cXyz additionalGamepadColor = {0, 0, 0};

float lerpSpeed = 0.0f;

const cXyz duskColor = {50, 50, -50};
const cXyz noColor = {0, 0, 0};

cXyz LerpColor(cXyz a, cXyz b, float t) {
    return {std::lerp(a.x, b.x, t), std::lerp(a.y, b.y, t), std::lerp(a.z, b.z, t)};
}

void FadeLED(cXyz newColor, float speed) {
    finalGamepadColor = newColor;
    lerpSpeed = speed / 30.0f;
}

void SetLED(cXyz newColor) {
    currentGamepadColor = newColor;
    finalGamepadColor = newColor;
}

void SetGamepadAdditionalColor(cXyz addColor) {
    additionalGamepadColor.x = addColor.x;
    additionalGamepadColor.y = addColor.y;
    additionalGamepadColor.z = addColor.z;
}

void handleGamepadColor() {
    bool setColor = false;

    fopAc_ac_c* zhint = dComIfGp_att_getZHint();
    if (zhint != NULL) {
        FadeLED({50, 50, 175}, 2.0f);
        setColor = true;
    }

    daPy_py_c* player = daPy_getPlayerActorClass();
    daAlink_c* link = daAlink_getAlinkActorClass();

    if (link != nullptr && !setColor) {
        if (link->checkWolf()) {
            FadeLED({115, 115, 75}, 5.0f);
            setColor = true;
        } else {
            switch (dComIfGs_getSelectEquipClothes()) {
            case dItemNo_WEAR_KOKIRI_e:
                FadeLED({0, 100, 0}, 5.0f);
                setColor = true;
                break;
            case dItemNo_WEAR_ZORA_e:
                FadeLED({0, 0, 100}, 5.0f);
                setColor = true;
                break;
            case dItemNo_ARMOR_e:
                if (link->checkMagicArmorHeavy()) {
                    FadeLED({5, 100, 100}, 5.0f);
                } else {
                    FadeLED({100, 0, 5}, 5.0f);
                }
                setColor = true;
                break;
            case dItemNo_WEAR_CASUAL_e:
                FadeLED({235, 230, 115}, 5.0f);
                setColor = true;
                break;
            }
        }
    }

    if (dKy_darkworld_check()) {
        SetGamepadAdditionalColor(duskColor);
    } else {
        SetGamepadAdditionalColor(noColor);
    }

    f32 finalRed = finalGamepadColor.x + additionalGamepadColor.x;
    f32 finalGreen = finalGamepadColor.y + additionalGamepadColor.y;
    f32 finalBlue = finalGamepadColor.z + additionalGamepadColor.z;

    if (finalRed > 255)
        finalRed = 255;
    if (finalRed < 0)
        finalRed = 0;

    if (finalGreen > 255)
        finalGreen = 255;
    if (finalGreen < 0)
        finalGreen = 0;

    if (finalBlue > 255)
        finalBlue = 255;
    if (finalBlue < 0)
        finalBlue = 0;

    currentGamepadColor = LerpColor(currentGamepadColor, cXyz{finalRed, finalGreen, finalBlue}, lerpSpeed);
    PADSetColor(PAD_CHAN0, (u8)currentGamepadColor.x, (u8)currentGamepadColor.y, (u8)currentGamepadColor.z);
}