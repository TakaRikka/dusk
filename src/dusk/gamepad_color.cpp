#include <cmath>
#include <SSystem/SComponent/c_xyz.h>
#include <d/d_com_inf_game.h>
#include <d/actor/d_a_player.h>
#include <d/actor/d_a_alink.h>
#include <dolphin/pad.h>
#include <dusk/settings.h>
#include <dusk/gamepad_color.h>

cXyz currentGamepadColor = {0, 0, 0};
cXyz finalGamepadColor = {0, 0, 0};
cXyz additionalGamepadColor = {0, 0, 0};

float lerpSpeed = 0.0f;

const cXyz duskColor = {50, 50, -50};
const cXyz noColor = {0, 0, 0};

static u8 s_lastLeftMode = 0xFF;
static u8 s_lastLeftParam0 = 0xFF;
static u8 s_lastLeftParam1 = 0xFF;
static u8 s_lastLeftParam2 = 0xFF;

static u8 s_lastRightMode = 0xFF;
static u8 s_lastRightParam0 = 0xFF;
static u8 s_lastRightParam1 = 0xFF;
static u8 s_lastRightParam2 = 0xFF;

void updateLeftTrigger(u8 mode, u8 p0, u8 p1, u8 p2) {
    if (mode != s_lastLeftMode || p0 != s_lastLeftParam0 || p1 != s_lastLeftParam1 || p2 != s_lastLeftParam2) {
        PADSetTriggerEffect(PAD_CHAN0, 0, mode, p0, p1, p2);
        s_lastLeftMode = mode;
        s_lastLeftParam0 = p0;
        s_lastLeftParam1 = p1;
        s_lastLeftParam2 = p2;
    }
}

void updateRightTrigger(u8 mode, u8 p0, u8 p1, u8 p2) {
    if (mode != s_lastRightMode || p0 != s_lastRightParam0 || p1 != s_lastRightParam1 || p2 != s_lastRightParam2) {
        PADSetTriggerEffect(PAD_CHAN0, 1, mode, p0, p1, p2);
        s_lastRightMode = mode;
        s_lastRightParam0 = p0;
        s_lastRightParam1 = p1;
        s_lastRightParam2 = p2;
    }
}

cXyz LerpColor(cXyz a, cXyz b, float t) {
    return {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t};
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
    static u32 s_pulseFrame = 0;
    s_pulseFrame++;

    bool setColor = false;

    // Pulse red when health is 2 hearts or less (each heart is 4 quarter-hearts)
    if (dComIfGs_getLife() > 0 && dComIfGs_getLife() <= 8) {
        float wave = (std::sin(s_pulseFrame * 0.12f) + 1.0f) * 0.5f; // 0.0 to 1.0
        float redVal = 60.0f + wave * 195.0f;
        FadeLED({redVal, 0.0f, 0.0f}, 10.0f);
        setColor = true;
    }

    if (!setColor) {
        fopAc_ac_c* zhint = dComIfGp_att_getZHint();
        if (zhint != nullptr) {
            FadeLED({50, 50, 175}, 2.0f);
            setColor = true;
        }
    }

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

    // --- PS5 DualSense Adaptive Triggers Integration ---
    if (dusk::getSettings().game.enableDualSenseTriggers.getValue() && PADGetControllerType(PAD_CHAN0) == PAD_TYPE_PS5) {
        if (link != nullptr) {
            // 1. Check for Heavy/Iron Boots first (overrides targeting/archery triggers)
            if (link->checkEquipHeavyBoots()) {
                updateLeftTrigger(0x01, 0, 220, 0);  // Heavy resistance L2
                updateRightTrigger(0x01, 0, 220, 0); // Heavy resistance R2
            } else {
                // 2. Left Trigger (L2) - Z-Targeting
                fopAc_ac_c* zhint = dComIfGp_att_getZHint();
                if (zhint != nullptr) {
                    updateLeftTrigger(0x01, 10, 150, 0); // Tactile targeting stiffness
                } else {
                    updateLeftTrigger(0x00, 0, 0, 0); // Normal trigger
                }

                // 3. Right Trigger (R2) - Archery (Bow & Slingshot)
                if (link->checkBowAnime()) {
                    if (link->checkBowChargeWaitAnime()) {
                        updateRightTrigger(0x01, 5, 210, 0); // Heavy draw tension
                    } else if (link->checkBowReadyAnime()) {
                        updateRightTrigger(0x01, 20, 120, 0); // Standard draw tension
                    } else {
                        updateRightTrigger(0x00, 0, 0, 0); // Arrow shot (immediate release)
                    }
                } else {
                    updateRightTrigger(0x00, 0, 0, 0); // Normal trigger
                }
            }
        } else {
            // Menus / loading screen safety fallback
            updateLeftTrigger(0x00, 0, 0, 0);
            updateRightTrigger(0x00, 0, 0, 0);
        }
    } else {
        // Disabled in settings or active controller is not a DualSense: clear triggers to normal
        updateLeftTrigger(0x00, 0, 0, 0);
        updateRightTrigger(0x00, 0, 0, 0);
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