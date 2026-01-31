#ifndef NW4HBM_LYT_COMMON_H
#define NW4HBM_LYT_COMMON_H

#include <revolution/tpl.h>
#include <revolution/types.h>

#include "../ut/Color.h"

#include "animation.h"
#include "resources.h"

#include "../db/assert.h"

namespace nw4hbm {
    namespace lyt {
        namespace detail {

            typedef math::VEC2 TexCoords[4];

            class TexCoordAry {
            public:
                TexCoordAry();

                u8 GetSize() const { return mNum; }
                const TexCoords* GetArray() const { return mpData; }

                void SetSize(u8 num);

                bool IsEmpty() const { return mCap == 0; }

                void Reserve(u8 num);

                void Free();
                void Copy(const void* pResTexCoord, u8 texCoordNum);

                void SetCoord(u32 idx, const math::VEC2* vec);
                void GetCoord(u32 idx, math::VEC2* vec) const;

            private:
                /* 0x00 */ u8 mCap;
                /* 0x01 */ u8 mNum;
                /* 0x04 */ TexCoords* mpData;
            };

            bool EqualsPaneName(const char* name1, const char* name2);
            bool EqualsMaterialName(const char* name1, const char* name2);

            bool TestFileHeader(const res::BinaryFileHeader& fileHeader);
            bool TestFileHeader(const res::BinaryFileHeader& fileHeader, u32 testSig);

            bool IsModulateVertexColor(ut::Color* vtxColors, u8 glbAlpha);

            ut::Color MultipleAlpha(const ut::Color col, u8 alpha);
            void MultipleAlpha(ut::Color* dst, const ut::Color* src, u8 alpha);

            void SetVertexFormat(bool bModulate, u8 texCoordNum);

            void DrawQuad(const math::VEC2& basePt, const Size& size, u8 texCoordNum,
                          const TexCoords* texCoords, const ut::Color* vtxColors);
            void DrawQuad(const math::VEC2& basePt, const Size& size, u8 texCoordNum,
                          const TexCoords* texCoords, const ut::Color* vtxColors, u8 alpha);

            void DrawLine(const math::VEC2& pos, const Size& size, ut::Color color);

            void InitGXTexObjFromTPL(GXTexObj* to, TPLPalette* pal, u32 id);

            inline s32 GetSignatureInt(const char* sig) {
                return *reinterpret_cast<const s32*>(sig);
            }

            inline const char* GetStrTableStr(const void* pStrTable, int index) {
                const u32* offsets = static_cast<const u32*>(pStrTable);
                const char* stringPool = static_cast<const char*>(pStrTable);

                return &stringPool[offsets[index]];
            }

            inline u8 GetVtxColorElement(const ut::Color* cols, u32 idx) {
                NW4HBM_ASSERT(199, idx < ANIMTARGET_VERTEXCOLOR_MAX);
                return reinterpret_cast<const u8*>(cols + idx / 4)[idx % 4];
            }

            inline void SetVtxColorElement(ut::Color* cols, u32 idx, u8 value) {
                NW4HBM_ASSERT(212, idx < ANIMTARGET_VERTEXCOLOR_MAX);
                reinterpret_cast<u8*>(cols + idx / 4)[idx % 4] = value;
            }

            inline u8 GetHorizontalPosition(u8 var) {
                return var % HORIZONTALPOSITION_MAX;
            }

            inline u8 GetVerticalPosition(u8 var) {
                return var / VERTICALPOSITION_MAX;
            }

            inline void SetHorizontalPosition(u8* pVar, u8 newVal) {
                *pVar = GetVerticalPosition(*pVar) * HORIZONTALPOSITION_MAX + newVal;
            }

            inline void SetVerticalPosition(u8* pVar, u8 newVal) {
                *pVar = newVal * VERTICALPOSITION_MAX + GetHorizontalPosition(*pVar);
            }

        }  // namespace detail
    }  // namespace lyt
}  // namespace nw4hbm

#endif
