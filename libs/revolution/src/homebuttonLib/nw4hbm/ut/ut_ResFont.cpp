#include "ResFont.h"

#include <revolution/hbm.h>
#include "../db/assert.h"


#define MAGIC_FONT 'RFNT'           // Revolution Font
#define MAGIC_FONT_UNPACKED 'RFNU'  // Revolution Font, unpacked

#define MAGIC_FONT_INFO 'FINF'        // FontInformation
#define MAGIC_FONT_TEX_GLYPH 'TGLP'   // FontTextureGlyph
#define MAGIC_FONT_CHAR_WIDTH 'CWDH'  // Font[Char]Width
#define MAGIC_FONT_CODE_MAP 'CMAP'    // FontCodeMap
#define MAGIC_FONT_GLGR 'GLGR'        // Glyph Group?

#define CONVERT_OFFSET_TO_PTR(type_, ptr_, offset_)                                                \
    reinterpret_cast<type_*>(reinterpret_cast<u32>(ptr_) + offset_)

namespace nw4hbm {
    namespace ut {

        namespace {
            template <typename T>
            void ResolveOffset(T*& ptr, void* base) {
                *reinterpret_cast<u32*>(&ptr) =
                    reinterpret_cast<u32>(base) + reinterpret_cast<u32>(ptr);
            }
        }  // namespace

        ResFont::ResFont() {}

        ResFont::~ResFont() {}

        bool ResFont::SetResource(void* brfnt) {
            NW4HBM_ASSERT_VALID_PTR(97, this);
            NW4HBM_ASSERT_VALID_PTR(98, brfnt);
            NW4HBM_ASSERT_ALIGN32(99, brfnt);

            FontInformation* pFontInfo = NULL;
            BinaryFileHeader* fileHeader = static_cast<BinaryFileHeader*>(brfnt);

            if (!IsManaging(NULL)) {
                NW4R_DB_WARNING(107, false, "Font resource already atached.");
                return false;
            }

            if (fileHeader->signature == MAGIC_FONT_UNPACKED) {
                BinaryBlockHeader* blockHeader;
                int nBlocks = 0;

                blockHeader =
                    CONVERT_OFFSET_TO_PTR(BinaryBlockHeader, fileHeader, fileHeader->headerSize);

                while (nBlocks < fileHeader->dataBlocks) {
                    NW4HBM_ASSERT_VALID_PTR(124, blockHeader);
                    if (blockHeader->kind == MAGIC_FONT_INFO) {
                        pFontInfo = CONVERT_OFFSET_TO_PTR(FontInformation, blockHeader,
                                                          sizeof(*blockHeader));
                        break;
                    }

                    blockHeader =
                        CONVERT_OFFSET_TO_PTR(BinaryBlockHeader, blockHeader, blockHeader->size);
                    nBlocks++;
                }
            } else {
                if (fileHeader->version == NW4HBM_VERSION(1, 4)) {
                    if (!IsValidBinaryFile(fileHeader, MAGIC_FONT, NW4HBM_VERSION(1, 4), 2)) {
                        NW4R_DB_WARNING(150, false, "Invalid font resource.");
                        return false;
                    }
                } else {
                    if (!IsValidBinaryFile(fileHeader, MAGIC_FONT, NW4HBM_VERSION(1, 2), 2)) {
                        NW4R_DB_WARNING(160, false, "Invalid font resource.");
                        return false;
                    }
                }

                pFontInfo = Rebuild(fileHeader);
            }

            if (!pFontInfo) {
                return false;
            }

            SetResourceBuffer(brfnt, pFontInfo);
            InitReaderFunc(GetEncoding());

            return true;
        }

        void dummyString() {
            OSReport("ResFont::RemoveResource(): Res font is not loaded.\n");
        }

        FontInformation* ResFont::Rebuild(BinaryFileHeader* fileHeader) {
            BinaryBlockHeader* blockHeader;
            FontInformation* info;
            int nBlocks;

            NW4HBM_ASSERT_VALID_PTR(218, fileHeader);
            NW4HBM_ASSERT_ALIGN32(219, fileHeader);

            info = NULL;
            nBlocks = 0;
            blockHeader =
                CONVERT_OFFSET_TO_PTR(BinaryBlockHeader, fileHeader, fileHeader->headerSize);

            while (nBlocks < fileHeader->dataBlocks) {
                NW4HBM_ASSERT_VALID_PTR(230, blockHeader);

                switch (blockHeader->kind) {
                case MAGIC_FONT_INFO: {
                    NW4HBM_ASSERT(237, info == NULL);
                    info =
                        CONVERT_OFFSET_TO_PTR(FontInformation, blockHeader, sizeof(*blockHeader));

                    NW4HBM_ASSERT(243, info->fontType == FONT_TYPE_NNGCTEXTURE);
                    NW4HBM_ASSERT(244, info->alterCharIndex != GLYPH_INDEX_NOT_FOUND);

                    // no check
                    NW4HBM_ASSERT_CHECK_NULL(247, info->pGlyph);
                    ResolveOffset(info->pGlyph, fileHeader);
                    NW4HBM_ASSERT_VALID_PTR(249, info->pGlyph);

                    if (info->pWidth) {
                        ResolveOffset(info->pWidth, fileHeader);
                        NW4HBM_ASSERT_VALID_PTR(255, info->pWidth);
                    }

                    if (info->pMap) {
                        ResolveOffset(info->pMap, fileHeader);
                        NW4HBM_ASSERT_VALID_PTR(260, info->pMap);
                    }
                } break;

                case MAGIC_FONT_TEX_GLYPH: {
                    FontTextureGlyph* glyph =
                        CONVERT_OFFSET_TO_PTR(FontTextureGlyph, blockHeader, sizeof(*blockHeader));

                    NW4HBM_ASSERT_CHECK_NULL(274, glyph->sheetImage);
                    // no check
                    ResolveOffset(glyph->sheetImage, fileHeader);
                    NW4HBM_ASSERT_VALID_PTR(276, glyph->sheetImage);
                    NW4R_ASSERT_MIN(279, glyph->cellWidth, 1);
                    NW4R_ASSERT_MIN(280, glyph->cellHeight, 1);
                    NW4R_ASSERT_MINMAXLT(281, glyph->sheetSize, 0x200, 0x400000);
                    NW4R_ASSERT_MIN(282, glyph->sheetNum, 1);
                    NW4R_ASSERT_MIN(283, glyph->sheetRow, 1);
                    NW4R_ASSERT_MIN(284, glyph->sheetLine, 1);
                    NW4R_ASSERT_MINMAXLT(285, glyph->sheetWidth, 0x20, 0x400);
                    NW4R_ASSERT_MINMAXLT(286, glyph->sheetHeight, 0x20, 0x400);
                } break;

                case MAGIC_FONT_CHAR_WIDTH: {
                    FontWidth* width =
                        CONVERT_OFFSET_TO_PTR(FontWidth, blockHeader, sizeof(*blockHeader));

                    NW4HBM_ASSERT(298, width->indexBegin <= width->indexEnd);

                    if (width->pNext) {
                        ResolveOffset(width->pNext, fileHeader);
                        NW4HBM_ASSERT_VALID_PTR(303, width->pNext);
                    }
                } break;

                case MAGIC_FONT_CODE_MAP: {
                    FontCodeMap* map =
                        CONVERT_OFFSET_TO_PTR(FontCodeMap, blockHeader, sizeof(*blockHeader));

                    NW4HBM_ASSERT(316, map->ccodeBegin <= map->ccodeEnd);
                    NW4HBM_ASSERT(319, (map->mappingMethod == FONT_MAPMETHOD_DIRECT) ||
                                           (map->mappingMethod == FONT_MAPMETHOD_TABLE) ||
                                           (map->mappingMethod == FONT_MAPMETHOD_SCAN));

                    if (map->pNext) {
                        ResolveOffset(map->pNext, fileHeader);
                        NW4HBM_ASSERT_VALID_PTR(324, map->pNext);
                    }
                } break;

                case MAGIC_FONT_GLGR:
                    break;

                default:
                    nw4hbm::db::detail::Panic(
                        __FILE__, 345, "The font has unknown block('%c%c%c%c').",
                        blockHeader->kind >> 24, (blockHeader->kind >> 16) & 0xFF,
                        (blockHeader->kind >> 8) & 0xFF, blockHeader->kind & 0xFF);
                    return NULL;
                }

                blockHeader =
                    CONVERT_OFFSET_TO_PTR(BinaryBlockHeader, blockHeader, blockHeader->size);
                nBlocks++;
            }

            fileHeader->signature = MAGIC_FONT_UNPACKED;

            return info;
        }

    }  // namespace ut
}  // namespace nw4hbm
