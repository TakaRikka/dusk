#include <revolution.h>
#include <revolution/si.h>

#include "__os.h"

#define LATENCY 8

static u32 SamplingRate = 0;

typedef struct XY {
    u16 line;
    u8 count;
} XY;

static XY XYNTSC[12] = {
    {0x00F6, 0x02},
    {0x000E, 0x13},
    {0x001E, 0x09},
    {0x002C, 0x06},
    {0x0034, 0x05},
    {0x0041, 0x04},
    {0x0057, 0x03},
    {0x0057, 0x03},
    {0x0057, 0x03},
    {0x0083, 0x02},
    {0x0083, 0x02},
    {0x0083, 0x02},
};

static XY XYPAL[12] = {
    {0x0128, 0x02},
    {0x000F, 0x15},
    {0x001D, 0x0B},
    {0x002D, 0x07},
    {0x0034, 0x06},
    {0x003F, 0x05},
    {0x004E, 0x04},
    {0x0068, 0x03},
    {0x0068, 0x03},
    {0x0068, 0x03},
    {0x0068, 0x03},
    {0x009C, 0x02},
};

void SISetSamplingRate(u32 msec) {
    XY* xy;
    BOOL progressive;
    BOOL enabled;

    ASSERTMSGLINE(387, msec <= 11, "SISetSamplingRate(): out of rage (0 <= msec <= 11)");
    if (msec > 11) {
        msec = 11;
    }
    enabled = OSDisableInterrupts();
    SamplingRate = msec;

    switch (VIGetTvFormat()) {
    case VI_NTSC:
    case VI_MPAL:
    case VI_EURGB60:
        xy = XYNTSC;
        break;
    case VI_PAL:
        xy = XYPAL;
        break;
    default:
        OSReport("SISetSamplingRate: unknown TV format. Use default.");
        msec = 0;
        xy = XYNTSC;
        break;
    }

    progressive = __VIRegs[VI_CLOCK_SEL] & 1;
    SISetXY((progressive ? 2 : 1) * xy[msec].line, xy[msec].count);
    OSRestoreInterrupts(enabled);
}

void SIRefreshSamplingRate(void) {
    SISetSamplingRate(SamplingRate);
}

#if DEBUG
void __SITestSamplingRate(u32 tvmode) {
    u32 msec;
    u32 line;
    u32 count;
    XY* xy;

    switch (tvmode) {
    case VI_NTSC:
    case VI_MPAL:
        xy = XYNTSC;
        for (msec = 0; msec <= 11; msec++) {
            line = xy[msec].line;
            count = xy[msec].count;
            OSReport("%2d[msec]: count %3d, line %3d, last %3d, diff0 %2d.%03d, diff1 %2d.%03d\n",
                msec, count, line, line * (count - 1) + LATENCY, (line * 636) / 10000, (line * 636) % 10000,
                ((263 - line * (count - 1)) * 636) / 10000, ((263 - line * (count - 1)) * 636) % 10000);
            ASSERTLINE(446, line * (count - 1) + LATENCY < 263);
            
            if (msec != 0) {
                ASSERTLINE(449, 636 * line < msec * 10000);
                ASSERTLINE(450, 636 * (263 - line * (count - 1)) < msec * 10000);
            }
        }
    break;
    case VI_PAL:
        xy = XYPAL;
        for (msec = 0; msec <= 11; msec++) {
            line = xy[msec].line;
            count = xy[msec].count;
            OSReport("%2d[msec]: count %3d, line %3d, last %3d, diff0 %2d.%03d, diff1 %2d.%03d\n",
                msec, count, line, line * (count - 1) + LATENCY, (line * 640) / 10000, (line * 640) % 10000,
                ((313 - line * (count - 1)) * 640) / 10000, ((313 - line * (count - 1)) * 640) % 10000);
            ASSERTLINE(470, line * (count - 1) + LATENCY < 313);
            
            if (msec != 0) {
                ASSERTLINE(473, 640 * line < msec * 10000);
                ASSERTLINE(474, 640 * (313 - line * (count - 1)) < msec * 10000);
            }
        }
        break;
    }
}
#endif
