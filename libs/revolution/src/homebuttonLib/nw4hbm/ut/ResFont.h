#ifndef NW4HBM_UT_RES_FONT_H
#define NW4HBM_UT_RES_FONT_H

#include <revolution/types.h>

#include "Font.h"
#include "binaryFileFormat.h"

namespace nw4hbm {
    namespace ut {
        namespace detail {

            class ResFontBase : public Font {
            public:
                ResFontBase();

                /* 0x08 */ virtual ~ResFontBase();
                /* 0x0C */ virtual int GetWidth() const;
                /* 0x10 */ virtual int GetHeight() const;
                /* 0x14 */ virtual int GetAscent() const;
                /* 0x18 */ virtual int GetDescent() const;
                /* 0x1C */ virtual int GetBaselinePos() const;
                /* 0x20 */ virtual int GetCellHeight() const;
                /* 0x24 */ virtual int GetCellWidth() const;
                /* 0x28 */ virtual int GetMaxCharWidth() const;
                /* 0x2C */ virtual Type GetType() const;
                /* 0x30 */ virtual GXTexFmt GetTextureFormat() const;
                /* 0x34 */ virtual int GetLineFeed() const;
                /* 0x38 */ virtual CharWidths GetDefaultCharWidths() const;
                /* 0x3C */ virtual void SetDefaultCharWidths(const CharWidths& widths);
                /* 0x40 */ virtual bool SetAlternateChar(u16 c);
                /* 0x44 */ virtual void SetLineFeed(int linefeed);
                /* 0x48 */ virtual int GetCharWidth(u16 c) const;
                /* 0x4C */ virtual CharWidths GetCharWidths(u16 c) const;
                /* 0x50 */ virtual void GetGlyph(Glyph* glyph, u16 c) const;
                /* 0x54 */ virtual FontEncoding GetEncoding() const;

                void SetResourceBuffer(void* pUserBuffer, FontInformation* pFontInfo);

                u16 GetGlyphIndex(u16 c) const;
                const CharWidths& GetCharWidthsFromIndex(u16 index) const;
                const CharWidths& GetCharWidthsFromIndex(const FontWidth* pWidth, u16 index) const;
                void GetGlyphFromIndex(Glyph* glyph, u16 index) const;
                u16 FindGlyphIndex(u16 c) const;
                u16 FindGlyphIndex(const FontCodeMap* pMap, u16 c) const;

                bool IsManaging(const void* ptr) const { return mResource == ptr; }

            private:
                /* 0x00 (base) */
                /* 0x10 */ void* mResource;
                /* 0x14 */ FontInformation* mFontInfo;
            };  // size = 0x18

        }  // namespace detail

        class ResFont : public detail::ResFontBase {
        public:
            ResFont();
            virtual ~ResFont();

            bool SetResource(void* brfnt);
            static FontInformation* Rebuild(BinaryFileHeader* fileHeader);

        private:
            static const u32 SIGNATURE_FONT = 'RFNT';          /* Revolution FoNT */
            static const u32 SIGNATURE_FONT_UNPACKED = 'RFNU'; /* Revolution FoNt Unpacked */
            static const u32 SIGNATURE_FONT_INFO = 'FINF';     /* Font INFormation */
            static const u32 SIGNATURE_TEX_GLYPH = 'TGLP';     /* Texture GLyPh */
            static const u32 SIGNATURE_CHAR_WIDTH = 'CWDH';    /* Character WiDtH */
            static const u32 SIGNATURE_CODE_MAP = 'CMAP';      /* Code MAP */
            static const u32 SIGNATURE_GLGR = 'GLGR';          /* Something related to GLyPh? */
        };

    }  // namespace ut
}  // namespace nw4hbm

#endif
