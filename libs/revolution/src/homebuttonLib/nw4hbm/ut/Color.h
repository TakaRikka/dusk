#ifndef NW4HBM_UT_COLOR_H
#define NW4HBM_UT_COLOR_H

#include <revolution/types.h>
#include <revolution/gx.h>


namespace nw4hbm {
    namespace ut {
        typedef struct Color : public GXColor {
            static const u32 RED = 0xFF0000FF;
            static const u32 GREEN = 0x00FF00FF;
            static const u32 BLUE = 0x0000FFFF;

            static const u32 CYAN = 0x00FFFFFF;
            static const u32 MAGENTA = 0xFF00FFFF;
            static const u32 YELLOW = 0xFFFF00FF;

            static const u32 BLACK = 0x000000FF;
            static const u32 GRAY = 0x808080FF;
            static const u32 WHITE = 0xFFFFFFFF;

            static const u32 NOCOLOR = 0x00000000;
            static const u32 MAXCOLOR = 0xFFFFFFFF;

            // Constructor

            Color() { *this = WHITE; }
            Color(u32 color) { *this = color; }
            Color(const GXColor& color) { *this = color; }
            Color(int red, int green, int blue, int alpha) { Set(red, green, blue, alpha); }

            // Left out destructor

            ~Color() {}

            // Operators

            Color& operator=(u32 color) {
                ToU32ref() = color;
                return *this;
            }

            Color& operator=(const GXColor& color) {
                return *this = *reinterpret_cast<const u32*>(&color);
            }

            Color operator|(u32 color) const { return Color(ToU32() | color); }
            Color operator&(u32 color) const { return Color(ToU32() & color); }

            u32& ToU32ref() { return *reinterpret_cast<u32*>(this); }
            const u32& ToU32ref() const { return *reinterpret_cast<const u32*>(this); }
            u32 ToU32() const { return ToU32ref(); }

            operator u32() const { return ToU32ref(); }

            // Functions

            void Set(int red, int green, int blue, int alpha) {
                r = red;
                g = green;
                b = blue;
                a = alpha;
            }
        } Color;
    }  // namespace ut
}  // namespace nw4hbm

#endif
