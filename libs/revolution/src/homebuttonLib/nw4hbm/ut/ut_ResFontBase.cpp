#include "ResFont.h"

#include "../db/assert.h"

struct CMapScanEntry {
    /* 0x00 */ u16 ccode;
    /* 0x02 */ u16 index;
};  // size = 0x04

struct CMapInfoScan {
    /* 0x00 */ u16 num;
    CMapScanEntry entries[];  // flexible,  offset 0x02 (unit size 0x04)
};  // size = 0x02

namespace nw4hbm {
    namespace ut {
        namespace detail {

            ResFontBase::ResFontBase() : mResource(NULL), mFontInfo(NULL) {}

            ResFontBase::~ResFontBase() {}

            void ResFontBase::SetResourceBuffer(void* pUserBuffer, FontInformation* pFontInfo) {
                NW4HBM_ASSERT_VALID_PTR(79, pUserBuffer);
                NW4HBM_ASSERT_VALID_PTR(80, pFontInfo);
                NW4HBM_ASSERT(81, mResource == NULL);
                NW4HBM_ASSERT(82, mFontInfo == NULL);
                mResource = pUserBuffer;
                mFontInfo = pFontInfo;
            }

            int ResFontBase::GetWidth() const {
                NW4HBM_ASSERT_VALID_PTR(128, this);
                NW4HBM_ASSERT_VALID_PTR(129, mFontInfo);
                return mFontInfo->width;
            }

            int ResFontBase::GetHeight() const {
                NW4HBM_ASSERT_VALID_PTR(145, this);
                NW4HBM_ASSERT_VALID_PTR(146, mFontInfo);
                return mFontInfo->height;
            }

            int ResFontBase::GetAscent() const {
                NW4HBM_ASSERT_VALID_PTR(162, this);
                NW4HBM_ASSERT_VALID_PTR(163, mFontInfo);
                return mFontInfo->ascent;
            }

            int ResFontBase::GetDescent() const {
                NW4HBM_ASSERT_VALID_PTR(179, this);
                NW4HBM_ASSERT_VALID_PTR(180, mFontInfo);
                return mFontInfo->height - mFontInfo->ascent;
            }

            int ResFontBase::GetBaselinePos() const {
                NW4HBM_ASSERT_VALID_PTR(196, this);
                NW4HBM_ASSERT_VALID_PTR(197, mFontInfo);
                return mFontInfo->pGlyph->baselinePos;
            }

            int ResFontBase::GetCellHeight() const {
                NW4HBM_ASSERT_VALID_PTR(213, this);
                NW4HBM_ASSERT_VALID_PTR(214, mFontInfo);
                return mFontInfo->pGlyph->cellHeight;
            }

            int ResFontBase::GetCellWidth() const {
                NW4HBM_ASSERT_VALID_PTR(230, this);
                NW4HBM_ASSERT_VALID_PTR(231, mFontInfo);
                return mFontInfo->pGlyph->cellWidth;
            }

            int ResFontBase::GetMaxCharWidth() const {
                NW4HBM_ASSERT_VALID_PTR(247, this);
                NW4HBM_ASSERT_VALID_PTR(248, mFontInfo);
                return mFontInfo->pGlyph->maxCharWidth;
            }

            Font::Type ResFontBase::GetType() const {
                return TYPE_RESOURCE;
            }

            GXTexFmt ResFontBase::GetTextureFormat() const {
                NW4HBM_ASSERT_VALID_PTR(279, this);
                NW4HBM_ASSERT_VALID_PTR(280, mFontInfo);
                return static_cast<GXTexFmt>(mFontInfo->pGlyph->sheetFormat);
            }

            int ResFontBase::GetLineFeed() const {
                NW4HBM_ASSERT_VALID_PTR(296, this);
                NW4HBM_ASSERT_VALID_PTR(297, mFontInfo);
                return mFontInfo->linefeed;
            }

            CharWidths ResFontBase::GetDefaultCharWidths() const {
                NW4HBM_ASSERT_VALID_PTR(313, this);
                NW4HBM_ASSERT_VALID_PTR(314, mFontInfo);
                return mFontInfo->defaultWidth;
            }

            void ResFontBase::SetDefaultCharWidths(const CharWidths& widths) {
                // clang-format off
                NW4HBM_ASSERT_VALID_PTR(330, this);
                NW4HBM_ASSERT_VALID_PTR(331, mFontInfo);
                NW4HBM_ASSERT_VALID_PTR(332, & widths);
                mFontInfo->defaultWidth = widths;
                // clang-format on
            }

            bool ResFontBase::SetAlternateChar(u16 c) {
                NW4HBM_ASSERT_VALID_PTR(350, this);
                NW4HBM_ASSERT_VALID_PTR(351, mFontInfo);
                u16 index = FindGlyphIndex(c);

                if (index != GLYPH_INDEX_NOT_FOUND) {
                    mFontInfo->alterCharIndex = index;
                    return true;
                } else {
                    return false;
                }
            }

            void ResFontBase::SetLineFeed(int linefeed) {
                NW4HBM_ASSERT_VALID_PTR(375, this);
                NW4HBM_ASSERT_VALID_PTR(376, mFontInfo);
                NW4R_ASSERT_MINMAXLT(377, linefeed, -128, 127);
                mFontInfo->linefeed = linefeed;
            }

            int ResFontBase::GetCharWidth(u16 c) const {
                return GetCharWidths(c).charWidth;
            }

            CharWidths ResFontBase::GetCharWidths(u16 c) const {
                u16 index = GetGlyphIndex(c);

                return GetCharWidthsFromIndex(index);
            }

            void ResFontBase::GetGlyph(Glyph* glyph, u16 c) const {
                u16 index = GetGlyphIndex(c);

                GetGlyphFromIndex(glyph, index);
            }

            FontEncoding ResFontBase::GetEncoding() const {
                NW4HBM_ASSERT_VALID_PTR(456, this);
                NW4HBM_ASSERT_VALID_PTR(457, mFontInfo);
                return static_cast<FontEncoding>(mFontInfo->encoding);
            }

            u16 ResFontBase::GetGlyphIndex(u16 c) const {
                NW4HBM_ASSERT_VALID_PTR(482, this);
                NW4HBM_ASSERT_VALID_PTR(483, mFontInfo);
                u16 index = FindGlyphIndex(c);

                return index != GLYPH_INDEX_NOT_FOUND ? index : mFontInfo->alterCharIndex;
            }

            u16 ResFontBase::FindGlyphIndex(u16 c) const {
                NW4HBM_ASSERT_VALID_PTR(502, this);
                NW4HBM_ASSERT_VALID_PTR(503, mFontInfo);
                FontCodeMap* pMap;
                for (pMap = mFontInfo->pMap; pMap; pMap = pMap->pNext) {
                    if (pMap->ccodeBegin <= c && c <= pMap->ccodeEnd) {
                        return FindGlyphIndex(pMap, c);
                    }
                }

                return GLYPH_INDEX_NOT_FOUND;
            }

            u16 ResFontBase::FindGlyphIndex(const FontCodeMap* pMap, u16 c) const {
                NW4HBM_ASSERT_VALID_PTR(539, this);
                NW4HBM_ASSERT_VALID_PTR(540, pMap);
                u16 index = GLYPH_INDEX_NOT_FOUND;

                switch (pMap->mappingMethod) {
                case FONT_MAPMETHOD_DIRECT: {
                    u16 offset = *pMap->mapInfo;

                    index = c - pMap->ccodeBegin + offset;
                } break;

                case FONT_MAPMETHOD_TABLE: {
                    int table_index = c - pMap->ccodeBegin;

                    index = pMap->mapInfo[table_index];
                } break;

                case FONT_MAPMETHOD_SCAN: {
                    const CMapInfoScan* scanInfo =
                        reinterpret_cast<const CMapInfoScan*>(pMap->mapInfo);

                    const CMapScanEntry* first = scanInfo->entries;
                    const CMapScanEntry* last = scanInfo->entries + (scanInfo->num - 1);

                    while (first <= last) {
                        const CMapScanEntry* mid = first + (last - first) / 2;

                        if (mid->ccode < c) {
                            first = mid + 1;
                        } else if (c < mid->ccode) {
                            last = mid - 1;
                        } else {
                            index = mid->index;
                            break;
                        }
                    }
                } break;

                default:
                    nw4hbm::db::detail::Panic(__FILE__, 597, "unknwon MAPMETHOD");
                    break;
                }

                return index;
            }

            const CharWidths& ResFontBase::GetCharWidthsFromIndex(u16 index) const {
                const FontWidth* pWidth;

                NW4HBM_ASSERT_VALID_PTR(615, this);
                NW4HBM_ASSERT_VALID_PTR(616, mFontInfo);
                for (pWidth = mFontInfo->pWidth; pWidth; pWidth = pWidth->pNext) {
                    if (pWidth->indexBegin <= index && index <= pWidth->indexEnd) {
                        return GetCharWidthsFromIndex(pWidth, index);
                    }
                }

                return mFontInfo->defaultWidth;
            }

            const CharWidths& ResFontBase::GetCharWidthsFromIndex(const FontWidth* pWidth,
                                                                  u16 index) const {
                NW4HBM_ASSERT_VALID_PTR(651, pWidth);
                return pWidth->widthTable[index - pWidth->indexBegin];
            }

            void ResFontBase::GetGlyphFromIndex(Glyph* glyph, u16 index) const {
                NW4HBM_ASSERT_VALID_PTR(671, this);
                NW4HBM_ASSERT_VALID_PTR(672, mFontInfo);
                FontTextureGlyph& tg = *mFontInfo->pGlyph;

                u32 cellsInASheet = tg.sheetRow * tg.sheetLine;
                u32 sheetNo = index / cellsInASheet;
                u32 cellNo = index % cellsInASheet;
                u32 cellUnitX = cellNo % tg.sheetRow;
                u32 cellUnitY = cellNo / tg.sheetRow;
                u32 cellPixelX = cellUnitX * (tg.cellWidth + 1);
                u32 cellPixelY = cellUnitY * (tg.cellHeight + 1);
                u32 offsetBytes = sheetNo * tg.sheetSize;
                void* pSheet = tg.sheetImage + offsetBytes;

                glyph->pTexture = pSheet;
                glyph->widths = GetCharWidthsFromIndex(index);
                glyph->height = static_cast<u8>(tg.cellHeight);
                glyph->texFormat = static_cast<GXTexFmt>(tg.sheetFormat);
                glyph->texWidth = static_cast<u16>(tg.sheetWidth);
                glyph->texHeight = static_cast<u16>(tg.sheetHeight);
                glyph->cellX = cellPixelX + 1;
                glyph->cellY = cellPixelY + 1;
            }

        }  // namespace detail
    }  // namespace ut
}  // namespace nw4hbm
