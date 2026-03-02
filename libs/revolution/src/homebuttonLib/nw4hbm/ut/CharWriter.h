#ifndef NW4HBM_UT_CHAR_WRITER_H
#define NW4HBM_UT_CHAR_WRITER_H

#include <revolution/types.h>

#include "Color.h"
#include "fontResources.h"

#include "../math/types.h"

#include <revolution/gx.h>

namespace nw4hbm {
    namespace ut {

        class Font;

        class CharWriter {
        public:
            typedef enum GradationMode {
                /* 0 */ GRADMODE_NONE = 0,
                /* 1 */ GRADMODE_H,
                /* 2 */ GRADMODE_V,
                /* 3 */ NUM_OF_GRADMODE
            } GradationMode;

        private:
            typedef struct ColorMapping {
                /* 0x00 */ Color min;
                /* 0x04 */ Color max;
            } ColorMapping;

            typedef struct VertexColor {
                /* 0x00 */ Color lu, ru;
                /* 0x08 */ Color ld, rd;
            } VertexColor;

            typedef struct TextureFilter {
            public:
                bool operator!=(const TextureFilter& rhs) const {
                    return atSmall != rhs.atSmall || atLarge != rhs.atLarge;
                }

                /* 0x00 */ GXTexFilter atSmall;
                /* 0x04 */ GXTexFilter atLarge;
            } TextureFilter;

            typedef struct TextColor {
                /* 0x00 */ Color start;
                /* 0x04 */ Color end;
                /* 0x08 */ GradationMode gradationMode;
            } TextColor;

            typedef struct LoadingTexture {
            public:
                bool operator!=(const LoadingTexture& rhs) const {
                    return slot != rhs.slot || texture != rhs.texture || filter != rhs.filter;
                }

                void Reset() {
                    slot = GX_TEXMAP_NULL;
                    texture = NULL;
                }

                /* 0x00 */ GXTexMapID slot;
                /* 0x04 */ void* texture;
                /* 0x08 */ TextureFilter filter;
            } LoadingTexture;

        public:
            CharWriter();
            ~CharWriter();

            const Font* GetFont() const;

            f32 GetScaleH() const;
            f32 GetScaleV() const;

            f32 GetCursorX() const;
            f32 GetCursorY() const;

            void SetFont(const Font& font);
            void SetColorMapping(Color min, Color max);

            void SetScale(f32 hScale, f32 vScale);
            void SetScale(f32 scale);

            void SetCursor(f32 x, f32 y);
            void SetCursor(f32 x, f32 y, f32 z);
            void SetCursorX(f32 x);
            void SetCursorY(f32 y);

            f32 GetFontWidth() const;
            f32 GetFontHeight() const;
            f32 GetFontAscent() const;

            bool IsWidthFixed() const;
            void EnableFixedWidth(bool flag);

            f32 GetFixedWidth() const;
            void SetFixedWidth(f32 width);

            void SetGradationMode(GradationMode mode);

            void SetTextColor(Color color);
            void SetTextColor(Color start, Color end);
            Color GetTextColor() const;

            void SetFontSize(f32 width, f32 height);
            void SetFontSize(f32 height);

            void SetupGX();

            void ResetColorMapping();
            void ResetTextureCache();

            void EnableLinearFilter(bool atSmall, bool atLarge);

            f32 Print(u16 code);

            void MoveCursorX(f32 dx);
            void MoveCursorY(f32 dy);

            void PrintGlyph(f32 x, f32 y, f32 z, const Glyph& glyph);

            void LoadTexture(const Glyph& glyph, GXTexMapID slot);

            void UpdateVertexColor();

        private:
            static void SetupVertexFormat();

            static void SetupGXDefault();
            static void SetupGXWithColorMapping(Color min, Color max);
            static void SetupGXForI();
            static void SetupGXForRGBA();

            /* 0x00 */ ColorMapping mColorMapping;
            /* 0x08 */ VertexColor mVertexColor;
            /* 0x18 */ TextColor mTextColor;
            /* 0x24 */ math::VEC2 mScale;
            /* 0x2C */ math::VEC3 mCursorPos;
            /* 0x38 */ TextureFilter mFilter;
            /* 0x40 */ u8 padding_[2];
            /* 0x42 */ u8 mAlpha;
            /* 0x43 */ bool mIsWidthFixed;
            /* 0x44 */ f32 mFixedWidth;
            /* 0x48 */ const Font* mFont;

            static const u32 DEFAULT_COLOR_MAPPING_MIN = 0x00000000;
            static const u32 DEFAULT_COLOR_MAPPING_MAX = 0xFFFFFFFF;

        private:
            static LoadingTexture mLoadingTexture;
        };  // size = 0x4C

    }  // namespace ut
}  // namespace nw4hbm

#endif
