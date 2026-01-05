#include <revolution/os.h>
#include <revolution/vi.h>
#include "assert.h"
#include "directPrint.h"

#include <string>

#define ROUND_UP(x, align) (((x) + (align)-1) & (-(align)))
#define ROUND_DOWN(x, align) ((x) & (~((align) - 1)))

typedef struct FrameBufferInfo {
    /* 0x00 */ u8* frameMemory;
    /* 0x04 */ u32 frameSize;
    /* 0x08 */ u16 frameWidth;
    /* 0x0A */ u16 frameHeight;
    /* 0x0C */ u16 frameRow;
    /* 0x0E */ u16 reserved;
} FrameBufferInfo;

typedef struct YUVColorInfo {
    /* 0x00 */ GXColor colorRGBA;
    /* 0x04 */ u16 colorY256;
    /* 0x06 */ u16 colorU;
    /* 0x08 */ u16 colorU2;
    /* 0x0A */ u16 colorU4;
    /* 0x0C */ u16 colorV;
    /* 0x0E */ u16 colorV2;
    /* 0x10 */ u16 colorV4;
    /* 0x12 */ u16 reserved;
} YUVColorInfo;

namespace nw4hbm {
    namespace db {

        static void DrawStringToXfb_(int posh, int posv, char const* str, bool turnOver,
                                     bool backErase);
        static char const* DrawStringLineToXfb_(int posh, int posv, char const* str, int width);
        static void DrawCharToXfb_(int posh, int posv, int code);

        namespace detail {
            static void WaitVIRetrace_();
            static void* CreateFB_(GXRenderModeObj const* rmode);
        }  // namespace detail

        static inline int StrLineWidth_(char const* str) {
            int len = 0;
            char c;

            NW4HBM_ASSERT_CHECK_NULL(306, str);

            while (true) {
                c = *str++;

                if (c == '\0' || c == '\n') {
                    return len;
                }

                if (c == '\t') {
                    len = (len + 4) & -4;

                    continue;
                }

                len++;
            }

            return len;
        }

        static const u8 sAsciiTable[128] = {
            0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0xFD, 0xFE, 0x7A, 0x7A,
            0x7A, 0x7A, 0x7A, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x29, 0x64, 0x65, 0x66, 0x2B, 0x67,
            0x68, 0x25, 0x26, 0x69, 0x2A, 0x6A, 0x27, 0x2C, 0x6B, 0x00, 0x01, 0x02, 0x03,
            0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x24, 0x6C, 0x6D, 0x6E, 0x6F, 0x28, 0x70,
            0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
            0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23,
            0x71, 0x72, 0x73, 0x74, 0x75, 0xFF, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83,
            0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90,
            0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x76, 0x77, 0x78, 0x79, 0x7A,
        };

        static const u32 sFontData[64] = {
            0x70871C30, 0x8988A250, 0x88808290, 0x88830C90, 0x888402F8, 0x88882210, 0x71CF9C10,
            0xF9CF9C70, 0x8208A288, 0xF200A288, 0x0BC11C78, 0x0A222208, 0x8A222208, 0x71C21C70,
            0x23C738F8, 0x5228A480, 0x8A282280, 0x8BC822F0, 0xFA282280, 0x8A28A480, 0x8BC738F8,
            0xF9C89C08, 0x82288808, 0x82088808, 0xF2EF8808, 0x82288888, 0x82288888, 0x81C89C70,
            0x8A08A270, 0x920DA288, 0xA20AB288, 0xC20AAA88, 0xA208A688, 0x9208A288, 0x8BE8A270,
            0xF1CF1CF8, 0x8A28A220, 0x8A28A020, 0xF22F1C20, 0x82AA0220, 0x82492220, 0x81A89C20,
            0x8A28A288, 0x8A28A288, 0x8A289488, 0x8A2A8850, 0x894A9420, 0x894AA220, 0x70852220,
            0xF8011000, 0x08020800, 0x10840400, 0x20040470, 0x40840400, 0x80020800, 0xF8011000,
            0x70800000, 0x88822200, 0x08820400, 0x108F8800, 0x20821000, 0x00022200, 0x20800020,
            0x00000000,
        };

        static const u32 sFontData2[77] = {
            0x51421820, 0x53E7A420, 0x014A2C40, 0x01471000, 0x0142AA00, 0x03EAA400, 0x01471A78,
            0x00000000, 0x50008010, 0x20010820, 0xF8020040, 0x20420820, 0x50441010, 0x00880000,
            0x00070E00, 0x01088840, 0x78898820, 0x004A8810, 0x788A8810, 0x01098808, 0x00040E04,
            0x70800620, 0x11400820, 0x12200820, 0x10001020, 0x10000820, 0x100F8820, 0x70000620,
            0x60070000, 0x110F82A0, 0x12AA8AE0, 0x084F92A0, 0x100FBE1C, 0x10089008, 0x60070808,
            0x00000000, 0x02000200, 0x7A078270, 0x8BC81E88, 0x8A2822F8, 0x9A282280, 0x6BC79E78,
            0x30000000, 0x48080810, 0x41E80000, 0x422F1830, 0xFBE88810, 0x40288890, 0x43C89C60,
            0x81000000, 0x81000000, 0x990F3C70, 0xA10AA288, 0xE10AA288, 0xA10AA288, 0x98CAA270,
            0x00000000, 0x00000020, 0xF1EF1E20, 0x8A28A0F8, 0x8A281C20, 0xF1E80220, 0x80283C38,
            0x00000000, 0x00000000, 0x8A28B688, 0x8A2A8888, 0x8A2A8878, 0x894A8808, 0x788536F0,
            0x00000000, 0x00000000, 0xF8000000, 0x10000000, 0x20000000, 0x40000000, 0xF8000000,
        };

        static FrameBufferInfo sFrameBufferInfo;
        static YUVColorInfo sFrameBufferColor;
        static int sInitialized = false;

        static inline int GetDotWidth_() {
            return sFrameBufferInfo.frameWidth < 400 ? 1 : 2;
        }
        static inline int GetDotHeight_() {
            return sFrameBufferInfo.frameHeight < 300 ? 1 : 2;
        }

        void DirectPrint_Init() {
            if (!sInitialized) {
                DirectPrint_ChangeXfb(NULL, 640, 480);
                DirectPrint_SetColor(0xff, 0xff, 0xff);

                sInitialized = true;
            }
        }

        bool DirectPrint_IsActive() {
            return sInitialized && sFrameBufferInfo.frameMemory;
        }

        void DirectPrint_EraseXfb(int posh, int posv, int sizeh, int sizev) {
            int posEndH, posEndV;

            if (sFrameBufferInfo.frameMemory == NULL) {
                return;
            }

            if (GetDotWidth_() == 2) {
                posh *= 2;
                sizeh *= 2;
            }

            posEndH = posh + sizeh;
            posh = posh >= 0 ? posh : 0;

            posEndH = posEndH <= sFrameBufferInfo.frameWidth ? posEndH : sFrameBufferInfo.frameWidth;
            sizeh = posEndH - posh;

            if (GetDotHeight_() == 2) {
                posv *= 2;
                sizev *= 2;
            }

            posEndV = posv + sizev;
            posv = posv >= 0 ? posv : 0;

            posEndV = posEndV <= sFrameBufferInfo.frameHeight ? posEndV : sFrameBufferInfo.frameHeight;
            sizev = posEndV - posv;

            u16* pixel = reinterpret_cast<u16*>(sFrameBufferInfo.frameMemory) +
                         sFrameBufferInfo.frameRow * posv + posh;

            for (int cntv = 0; cntv < sizev; cntv++) {
                for (int cnth = 0; cnth < sizeh; cnth++) {
                    *pixel++ = 0x1080;  // some sort of white or black?
                }

                pixel += sFrameBufferInfo.frameRow - sizeh;
            }
        }

        void DirectPrint_ChangeXfb(void* framebuf, u16 width, u16 height) {
            sFrameBufferInfo.frameMemory = static_cast<u8*>(framebuf);
            sFrameBufferInfo.frameWidth = width;
            sFrameBufferInfo.frameHeight = height;

            sFrameBufferInfo.frameRow = ROUND_UP(static_cast<u16>(width), 16);
            sFrameBufferInfo.frameSize =
                sFrameBufferInfo.frameRow * sFrameBufferInfo.frameHeight * 2;
        }

        void DirectPrint_ChangeXfb(void* framebuf) {
            sFrameBufferInfo.frameMemory = static_cast<u8*>(framebuf);
        }

        void DirectPrint_StoreCache(void) {
            DCStoreRange(sFrameBufferInfo.frameMemory, sFrameBufferInfo.frameSize);
        }

        void DirectPrint_DrawString(int posh, int posv, bool turnOver, char const* format, ...) {
            if (sFrameBufferInfo.frameMemory) {
                std::va_list vargs;

                va_start(vargs, format);
                detail::DirectPrint_DrawStringToXfb(posh, posv, format, vargs, turnOver, false);
                va_end(vargs);
            }
        }

        // Intel IPP RGBToYCbCr algorithm, same as OSFatal.c::RGB2YUV
        void DirectPrint_SetColor(u8 r, u8 g, u8 b) {
            int y = (int)(0.257f * (int)r + 0.504f * (int)g + 0.098f * (int)b + 16.0f);
            int u = (int)(-0.148f * (int)r - 0.291f * (int)g + 0.439f * (int)b + 128.0f);
            int v = (int)(0.439f * (int)r - 0.368f * (int)g - 0.071f * (int)b + 128.0f);

            sFrameBufferColor.colorRGBA.r = r;
            sFrameBufferColor.colorRGBA.g = g;
            sFrameBufferColor.colorRGBA.b = b;
            sFrameBufferColor.colorRGBA.a = 0xff;

            sFrameBufferColor.colorY256 = static_cast<u16>(y << 8);

            sFrameBufferColor.colorU = static_cast<u16>(u);
            sFrameBufferColor.colorU2 = static_cast<u16>(u / 2);
            sFrameBufferColor.colorU4 = static_cast<u16>(u / 4);

            sFrameBufferColor.colorV = static_cast<u16>(v);
            sFrameBufferColor.colorV2 = static_cast<u16>(v / 2);
            sFrameBufferColor.colorV4 = static_cast<u16>(v / 4);
        }

        void detail::DirectPrint_DrawStringToXfb(int posh, int posv, char const* format,
                                                 std::va_list vargs, bool turnOver,
                                                 bool backErase) {
            char string[256];

            NW4HBM_ASSERT(647, sFrameBufferInfo.frameMemory != NULL);

            int length = std::vsnprintf(string, sizeof(string), format, vargs);
            int posLeftStart = posh;

            if (length > 0) {
                DrawStringToXfb_(posh, posv, string, turnOver, backErase);
            }
        }

        static void DrawStringToXfb_(int posh, int posv, char const* str, bool turnOver,
                                     bool backErase) {
            int basePosH = posh;
            int width;
            int frameWidth = sFrameBufferInfo.frameWidth / GetDotWidth_();

            while (*str != '\0') {
                int len;

                if (backErase) {
                    len = StrLineWidth_(str);

                    DirectPrint_EraseXfb(posh - 6, posv - 3, (len + 2) * 6, 13);
                }

                width = (frameWidth - posh) / 6;
                str = DrawStringLineToXfb_(posh, posv, str, width);
                posv += 10;

                if (*str == '\n') {
                    str++;
                    posh = basePosH;
                    continue;
                }

                if (*str == '\0') {
                    continue;
                }

                str++;

                if (!turnOver) {
                    str = strchr(str, '\n');

                    if (str) {
                        str++;
                        posh = basePosH;
                        continue;
                    } else {
                        break;
                    }
                }

                posh = 0;
            }
        }

        static char const* DrawStringLineToXfb_(int posh, int posv, char const* str, int width) {
            char c;
            int code;
            int cnt = 0;

            NW4HBM_ASSERT_CHECK_NULL(745, str);
            NW4HBM_ASSERT(746, width > 0);

            for (; (c = *str) != '\0'; str++) {
                if (c == '\n' || c == '\0') {  // another check against null character?
                    return str;
                }

                code = sAsciiTable[c % sizeof(sAsciiTable)];

                if (code == 0xfd) {
                    int tab_size = 4 - (cnt & 3);

                    posh += tab_size * 6;
                    cnt += tab_size;
                } else {
                    if (code != 0xff) {
                        DrawCharToXfb_(posh, posv, code);
                    }

                    posh += 6;
                    cnt++;
                }

                if (cnt >= width) {
                    if (str[1] == '\n') {
                        str++;
                    }

                    return str;
                }
            }

            return str;
        }

        static void DrawCharToXfb_(int posh, int posv, int code) {
            static u32 twiceBit[4] = {0, 3, 12, 15};

            int ncode = code >= 100 ? code - 100 : code;
            int fonth = ncode % 5 * 6;
            int fontv = ncode / 5 * 7;
            const u32* fontLine = code < 100 ? &sFontData[fontv] : &sFontData2[fontv];

            int wH = GetDotWidth_();
            int wV = GetDotHeight_();

            u16* pixel = reinterpret_cast<u16*>(sFrameBufferInfo.frameMemory) +
                         sFrameBufferInfo.frameRow * posv * wV + posh * wH;

            if (posv < 0 || posh < 0) {
                return;
            }

            if (sFrameBufferInfo.frameWidth <= wH * (posh + 6) ||
                sFrameBufferInfo.frameHeight <= wV * (posv + 7))
            {
                return;
            }

            for (int cntv = 0; cntv < 7; cntv++) {
                u32 fontBits = *fontLine++ << fonth;

                if (wH == 1) {
                    fontBits = (fontBits & 0xfc000000) >> 1;
                } else {
                    fontBits =
                        (twiceBit[(fontBits >> 26) & 3] | twiceBit[(fontBits >> 28) & 3] << 4 |
                         twiceBit[(fontBits >> 30) & 3] << 8)
                        << 19;
                }

                for (int cnth = 0; cnth < wH * 6;) {
                    u16 pixColor = (fontBits & (1u << 30) ? sFrameBufferColor.colorY256 : 0x00) |
                                   ((fontBits & (1u << 31) ? sFrameBufferColor.colorU4 : 0x20) +
                                    (fontBits & (1u << 30) ? sFrameBufferColor.colorU2 : 0x40) +
                                    (fontBits & (1u << 29) ? sFrameBufferColor.colorU4 : 0x20));

                    *pixel = pixColor;

                    if (wV > 1) {
                        pixel[sFrameBufferInfo.frameRow] = pixColor;
                    }

                    pixel++;

                    pixColor = (fontBits & (1u << 29) ? sFrameBufferColor.colorY256 : 0x00) |
                               ((fontBits & (1u << 30) ? sFrameBufferColor.colorV4 : 0x20) +
                                (fontBits & (1u << 29) ? sFrameBufferColor.colorV2 : 0x40) +
                                (fontBits & (1u << 28) ? sFrameBufferColor.colorV4 : 0x20));

                    *pixel = pixColor;

                    if (wV > 1) {
                        pixel[sFrameBufferInfo.frameRow] = pixColor;
                    }

                    pixel++;

                    fontBits <<= 2;
                    cnth += 2;
                }

                pixel += sFrameBufferInfo.frameRow * wV - 6 * wH;
            }
        }

        static void detail::WaitVIRetrace_(void) {
            int intrStatus = OSEnableInterrupts(); /* int enabled; */
            u32 preCnt = VIGetRetraceCount();

            while (preCnt == VIGetRetraceCount()) { /* ... */
            }

            OSRestoreInterrupts(intrStatus);
        }

        static void* detail::CreateFB_(GXRenderModeObj const* rmode) {
            u32 arenaHi = (u32)OSGetArenaHi();
            u32 memSize = (u16)ROUND_UP((u16)rmode->fbWidth, 16) * rmode->xfbHeight * 2;
            u32 frameBuf = ROUND_DOWN(arenaHi - memSize, 32);

            VIConfigure(rmode);
            VISetNextFrameBuffer(reinterpret_cast<void*>(frameBuf));

            return reinterpret_cast<void*>(frameBuf);
        }

        void* detail::DirectPrint_SetupFB(GXRenderModeObj const* rmode) {
            void* frameMemory;

            DirectPrint_Init();

            frameMemory = VIGetCurrentFrameBuffer();
            if (!frameMemory) {
                if (!rmode) {
                    switch (VIGetTvFormat()) {
                    case VI_TVMODE_NTSC_INT:
                        rmode = &GXNtsc480IntDf;
                        break;

                    case VI_TVMODE_NTSC_DS:
                        rmode = &GXPal528IntDf;
                        break;

                    case VI_TVMODE_PAL_DS:
                        rmode = &GXEurgb60Hz480IntDf;
                        break;

                    case VI_TVMODE_NTSC_PROG:
                        rmode = &GXMpal480IntDf;
                        break;
                    }
                }

                frameMemory = CreateFB_(rmode);
            }

            VISetBlack(false);
            VIFlush();
            WaitVIRetrace_();

            if (rmode) {
                DirectPrint_ChangeXfb(frameMemory, rmode->fbWidth, rmode->xfbHeight);
            } else {
                DirectPrint_ChangeXfb(frameMemory);
            }

            return frameMemory;
        }

    }  // namespace db
}  // namespace nw4hbm
