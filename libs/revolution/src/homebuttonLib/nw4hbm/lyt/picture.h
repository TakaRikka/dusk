#ifndef NW4HBM_LYT_PICTURE_H
#define NW4HBM_LYT_PICTURE_H

#include "pane.h"

#include "common.h"

namespace nw4hbm {
    namespace lyt {

        class Picture : public Pane {
        public:
            Picture(u8 num);
            Picture(const res::Picture* pResPic, const ResBlockSet& resBlockSet);

            /* 0x08 */ virtual ~Picture();
            /* 0x0C */ NW4HBM_UT_RUNTIME_TYPEINFO;
            /* 0x18 */ virtual void DrawSelf(const DrawInfo& drawInfo);
            /* 0x24 */ virtual ut::Color GetVtxColor(u32 idx) const;
            /* 0x28 */ virtual void SetVtxColor(u32 idx, ut::Color value);
            /* 0x34 */ virtual u8 GetVtxColorElement(u32 idx) const;
            /* 0x38 */ virtual void SetVtxColorElement(u32 idx, u8 value);
            /* 0x64 */ virtual void Append(TPLPalette* pTplRes);
            /* 0x68 */ virtual void Append(const GXTexObj& texObj);

            void SetTexCoordNum(u8 num);
            u8 GetTexCoordNum() const;

            void GetTexCoord(u32 idx, math::VEC2* coords) const;
            void SetTexCoord(u32 idx, const math::VEC2* coords);

            void Init(u8 texNum);
            void ReserveTexCoord(u8 num);

        private:
            /* 0x00 (base) */
            /* 0xD4 */ ut::Color mVtxColors[VERTEXCOLOR_MAX] ATTRIBUTE_ALIGN(4);
            /* 0xE4 */ detail::TexCoordAry mTexCoordAry;
        };

    }  // namespace lyt
}  // namespace nw4hbm

#endif
