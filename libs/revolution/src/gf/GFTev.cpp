#include <revolution/gf.h>
#include <revolution/gd.h>

void GFSetTevColorS10(GXTevRegID reg, GXColorS10 color) {
    u32 regRA;
    u32 regBG;

    regRA = BP_TEV_COLOR_REG_RA(color.r & 0x7FF, color.a & 0x7FF, 0, 0xE0 + reg * 2);
    regBG = BP_TEV_COLOR_REG_BG(color.b & 0x7FF, color.g & 0x7FF, 0, 0xE1 + reg * 2);

    GFWriteBPCmd(regRA);
    GFWriteBPCmd(regBG);
    GFWriteBPCmd(regBG);
    GFWriteBPCmd(regBG);
}
