#pragma once

#include "mods/svc/config.h"

#include <string>

struct cvars {
    ConfigVarHandle herosTunicCapColor = 0;
    ConfigVarHandle herosTunicTorsoColor = 0;
    ConfigVarHandle herosTunicSkirtColor = 0;
    ConfigVarHandle zoraArmorCapColor = 0;
    ConfigVarHandle zoraArmorHelmetColor = 0;
    ConfigVarHandle zoraArmorTorsoColor = 0;
    ConfigVarHandle zoraArmorScalesColor = 0;
    ConfigVarHandle zoraArmorFlippersColor = 0;
    ConfigVarHandle lanternGlowColor = 0;
    ConfigVarHandle woodenSwordColor = 0;
    ConfigVarHandle msBladeColor = 0;
    ConfigVarHandle msHandleColor = 0;
    ConfigVarHandle lightSwordGlowColor = 0;
    ConfigVarHandle boomerangColor = 0;
    ConfigVarHandle ironBootsColor = 0;
    ConfigVarHandle spinnerColor = 0;
    ConfigVarHandle heartColor = 0;
    ConfigVarHandle midnaHairBaseColor = 0;
    ConfigVarHandle midnaHairTipsColor = 0;
    ConfigVarHandle midnaChargeRingColor = 0;
    ConfigVarHandle linkHairColor = 0;
    ConfigVarHandle wolfLinkColor = 0;
    ConfigVarHandle eponaColor = 0;
};

cvars& get_cvars();

std::string get_str_option(ConfigVarHandle handle, const std::string& fallback);

int64_t get_int_option(ConfigVarHandle handle, int64_t fallback);