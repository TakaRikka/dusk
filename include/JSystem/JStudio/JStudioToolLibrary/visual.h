#ifndef JSTUDIOTOOLLIBRARY_VISUAL_H
#define JSTUDIOTOOLLIBRARY_VISUAL_H

#include "JSystem/JUtility/TColor.h"
#include "JSystem/JUtility/JUTFont.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"

namespace JStudioToolLibrary {
    struct TRectangle {
        TRectangle();
        TRectangle(int, int, int, int);

        void set(int, int, int, int);
        int getBottom() const;
        int getTop() const;
        int getRight() const;
        int getLeft() const;
        int getWidth() const;
        int getHeight() const;

        /* 0x00 */ int iLeft_;
        /* 0x04 */ int iTop_;
        /* 0x08 */ int iRight_;
        /* 0x0C */ int iBottom_;
    };

    struct TDrawPrimitive2D {
        TDrawPrimitive2D();

        void setRender(J2DOrthoGraph* pOrthoGraph) {
            pOrthoGraph_ = pOrthoGraph;
        }

        void fillRectangle(int, int, int, int);
        void setColor(const JUtility::TColor&);
        void prepare();
        bool isEnabled() const;
        TRectangle getRectangle() const;
        void setColor(const JUtility::TColor&, const JUtility::TColor&, const JUtility::TColor&, const JUtility::TColor&);
        void frameRectangle(int, int, int, int);
        void setLineWidth(f32);

        /* 0x0 */ J2DOrthoGraph* pOrthoGraph_;
    };

    struct TDrawPrimitive3D {
        TDrawPrimitive3D();

        void setColor(GXColor);
        void setLineWidth(f32);
        void setGXState_position3f32();
        void endGX();
        void beginGX(GXPrimitive, u32);
        void setGXColor(GXColor);
        void setGXLineWidth(f32);
        void prepare();
        void drawAxis();
        void setGXColor();
        void setGXLineWidth();
        void setGXState_position1x8();
        void drawAxis_arrow();
        void drawAxis_color();
        void setGXState_position1x8_color1x8();
        void drawAxis_color_arrow();
        void drawGrid_xyz(uint);
        void setGXState_position3s16();
        void drawGrid_xy(uint);
        void drawGrid_xz(uint);
        void drawGrid_yz(uint);

        /* 0x0 */ f32 fLineWidth_;
        /* 0x4 */ GXColor oColor_;
    };

    struct TPrint {
        TPrint();

        int getY() const;
        int getX() const;
    
        void setFont(JUTFont* pFont) {
            pFont_ = pFont;
        }
    
        void setColor(const JUtility::TColor&);
        void locate(int, int);
        void prepare();
        int getFontHeight() const;
        int getFontWidth() const;
        bool isEnabled() const;
        void processControlCharacter(int);
        void print(int);
        void drawCharacter(int);
        void print(const char*);
        void print_f(const char*, ...);
        void print_f_va(const char*, va_list);

        /* 0x00 */ JUTFont* pFont_;
        /* 0x04 */ int field_0x4;
        /* 0x08 */ int field_0x8;
        /* 0x0C */ int iX_;
        /* 0x10 */ int iY_;
    };

    struct TColor_variable {
        struct TVelocity {
            TVelocity(const TVelocity&);
            TVelocity(int, int, int, int);

            /* 0x00 */ int field_0x0;
            /* 0x04 */ int field_0x4;
            /* 0x08 */ int field_0x8;
            /* 0x0C */ int field_0xc;
        };

        TColor_variable(const JUtility::TColor&, const TVelocity&);
        void update();
        void updateValue_(u8*, int*);

        /* 0x00 */ JUtility::TColor oColor_;
        /* 0x04 */ TVelocity oVelocity_;
    };
}

#endif
