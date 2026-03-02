#include <revolution/gf.h>
#include <revolution/gd.h>

void GFSetChanAmbColor(GXChannelID chan, GXColor color) {
    GFWriteXFCmd((chan & 1) + XF_REG_AMBIENT0_ID, color.r << 0x18 | color.g << 0x10 | color.b << 8 | color.a);
}
