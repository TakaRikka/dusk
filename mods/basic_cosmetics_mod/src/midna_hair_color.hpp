#pragma once

#include <gx.h>

struct MidnaHairColors {
    GXColorS10 normalColor;
    GXColor normalKColor;
    GXColor normalKColor2;
    GXColorS10 bigColor;
    GXColor bigKColor;
    GXColor lNormalKColor;
    GXColor lNormalKColor2;
    GXColorS10 lBigColor;
    GXColor lBigKColor2;
};

extern MidnaHairColors g_currentMidnaHairColors;

void set_all_midna_hair_colors();
