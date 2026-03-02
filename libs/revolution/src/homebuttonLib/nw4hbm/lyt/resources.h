#ifndef NW4HBM_LYT_RESOURCES_H
#define NW4HBM_LYT_RESOURCES_H

#include <revolution/gx.h>
#include <revolution/types.h>

#include "../ut/binaryFileFormat.h"

#include "lyt_types.h"
#include "resourceAccessor.h"


namespace nw4hbm {
    namespace lyt {

        typedef struct InflationLRTB {
            /* 0x00 */ f32 l;
            /* 0x04 */ f32 r;
            /* 0x08 */ f32 t;
            /* 0x0C */ f32 b;
        } InflationLRTB;

        typedef struct WindowFrameSize {
            /* 0x00 */ f32 l;
            /* 0x04 */ f32 r;
            /* 0x08 */ f32 t;
            /* 0x0C */ f32 b;
        } WindowFrameSize;

        class MaterialResourceNum {
        public:
            u8 GetTexMapNum() const { return detail::GetBits<>(bits, 0, 4); }
            u8 GetTexSRTNum() const { return detail::GetBits<>(bits, 4, 4); }
            u8 GetTexCoordGenNum() const { return detail::GetBits<>(bits, 8, 4); }
            bool HasTevSwapTable() const { return detail::TestBit<>(bits, 12); }
            u8 GetIndTexSRTNum() const { return detail::GetBits<>(bits, 13, 2); }
            u8 GetIndTexStageNum() const { return detail::GetBits<>(bits, 15, 3); }
            u8 GetTevStageNum() const { return detail::GetBits<>(bits, 18, 5); }
            bool HasAlphaCompare() const { return detail::TestBit<>(bits, 23); }
            bool HasBlendMode() const { return detail::TestBit<>(bits, 24); }
            u8 GetChanCtrlNum() const { return detail::GetBits<>(bits, 25, 1); }
            u8 GetMatColNum() const { return detail::GetBits<>(bits, 27, 1); }

        private:
            /* 0x00 */ u32 bits;
        };  // size = 0x04

        namespace res {

            /*** COMMON ***/

            typedef struct BinaryFileHeader {
                /* 0x00 */ char signature[4];
                /* 0x04 */ u16 byteOrder;
                /* 0x06 */ u16 version;
                /* 0x08 */ u32 fileSize;
                /* 0x0C */ u16 headerSize;
                /* 0x0E */ u16 dataBlocks;
            } BinaryFileHeader;  // size = 0x10

            typedef struct DataBlockHeader {
                /* 0x00 */ char kind[4];
                /* 0x04 */ u32 size;
            } DataBlockHeader;  // size = 0x08

            /*** ANIMATION ***/

            typedef struct StepKey {
                /* 0x00 */ f32 frame;
                /* 0x04 */ u16 value;
                /* 0x06 */ u16 padding;
            } StepKey;  // size = 0x08

            typedef struct HermiteKey {
                /* 0x00 */ f32 frame;
                /* 0x04 */ f32 value;
                /* 0x08 */ f32 slope;
            } HermiteKey;  // size = 0x0C

            typedef struct AnimationInfo {
            public:
                /* 0x00 */ u32 kind;
                /* 0x04 */ u8 num;
                /* 0x05 */ u8 padding[3];

            public:
                static const u32 ANIM_INFO_PANE_PAIN_SRT = 'RLPA';
                static const u32 ANIM_INFO_PANE_VERTEX_COLOR = 'RLVC';
                static const u32 ANIM_INFO_PANE_VISIBILITY = 'RLVI';

                static const u32 ANIM_INFO_MATERIAL_COLOR = 'RLMC';
                static const u32 ANIM_INFO_MATERIAL_TEXTURE_PATTERN = 'RLTP';
                static const u32 ANIM_INFO_MATERIAL_TEXTURE_SRT = 'RLTS';
                static const u32 ANIM_INFO_MATERIAL_IND_TEX_SRT = 'RLIM';
            } AnimationInfo;

            typedef struct AnimationTarget {
                /* 0x00 */ u8 id;
                /* 0x01 */ u8 target;
                /* 0x02 */ u8 curveType;
                /* 0x03 */ u8 padding1;
                /* 0x04 */ u16 keyNum;
                /* 0x06 */ u8 padding2[2];
                /* 0x08 */ u32 keysOffset;
            } AnimationTarget;  // size = 0x10

            typedef struct AnimationBlock {
                /* 0x00 */ DataBlockHeader blockHeader;
                /* 0x08 */ u16 frameSize;
                /* 0x0A */ u8 loop;
                /* 0x0B */ u8 padding1;
                /* 0x0C */ u16 fileNum;
                /* 0x0E */ u16 animContNum;
                /* 0x10 */ u32 animContOffsetsOffset;
            } AnimationBlock;  // size = 0x14

            typedef struct AnimationContent {
            public:
                enum {
                    /* 0 */ ACType_Pane = 0,
                    /* 1 */ ACType_Material,
                    /* 2 */ ACType_Max
                };

            public:
                /* 0x00 */ char name[20];
                /* 0x14 */ u8 num;
                /* 0x15 */ u8 type;
                /* 0x16 */ u8 padding[2];
            } AnimationContent;  // size = 0x18

            /*** MATERIAL ***/

            typedef struct Texture {
                /* 0x00 */ u32 nameStrOffset;
                /* 0x04 */ u8 type;
                /* 0x05 */ u8 padding[3];
            } Texture;  // size = 0x08

            typedef struct Material {
                /* 0x00 */ char name[20];
                /* 0x14 */ GXColorS10 tevCols[TEVCOLOR_MAX];
                /* 0x2C */ GXColor tevKCols[GX_MAX_KCOLOR];
                /* 0x3C */ MaterialResourceNum resNum;
            } Material;  // size = 0x40

            typedef struct TexMap {
                /* 0x00 */ u16 texIdx;
                /* 0x02 */ u8 wrapS;
                /* 0x03 */ u8 wrapT;
            } TexMap;  // size = 0x04

            /*** PANES ***/

            static const u32 FILE_HEADER_SIGNATURE_ANIMATION = 'RLAN';
            static const u32 FILE_HEADER_SIGNATURE_LAYOUT = 'RLYT';

            static const u32 OBJECT_SIGNATURE_LAYOUT = 'lyt1';
            static const u32 OBJECT_SIGNATURE_FONT_LIST = 'fnl1';
            static const u32 OBJECT_SIGNATURE_MATERIAL_LIST = 'mat1';
            static const u32 OBJECT_SIGNATURE_TEXTURE_LIST = 'txl1';
            static const u32 OBJECT_SIGNATURE_PANE = 'pan1';
            static const u32 OBJECT_SIGNATURE_PANE_CHILD_START = 'pas1';
            static const u32 OBJECT_SIGNATURE_PANE_CHILD_END = 'pae1';
            static const u32 OBJECT_SIGNATURE_PICTURE = 'pic1';
            static const u32 OBJECT_SIGNATURE_BOUNDING = 'bnd1';
            static const u32 OBJECT_SIGNATURE_WINDOW = 'wnd1';
            static const u32 OBJECT_SIGNATURE_TEXT_BOX = 'txt1';
            static const u32 OBJECT_SIGNATURE_GROUP = 'grp1';
            static const u32 OBJECT_SIGNATURE_GROUP_CHILD_START = 'grs1';
            static const u32 OBJECT_SIGNATURE_GROUP_CHILD_END = 'gre1';
            static const u32 OBJECT_SIGNATURE_PANE_ANIM = 'pai1';

            typedef struct Pane {
                /* 0x00 */ DataBlockHeader blockHeader;
                /* 0x08 */ u8 flag;
                /* 0x09 */ u8 basePosition;
                /* 0x0A */ u8 alpha;
                /* 0x0B */ u8 padding;
                /* 0x0C */ char name[16];
                /* 0x1C */ char userData[8];
                /* 0x24 */ math::VEC3 translate;
                /* 0x30 */ math::VEC3 rotate;
                /* 0x3C */ math::VEC2 scale;
                /* 0x44 */ Size size;
            } Pane;  // size = 0x4C

            typedef struct Bounding : Pane {
                // (empty)
            } Bounding;

            typedef struct Picture : public Pane {
                /* 0x4C */ u32 vtxCols[4];
                /* 0x5C */ u16 materialIdx;
                /* 0x5E */ u8 texCoordNum;
                /* 0x5F */ u8 padding[1];
            } Picture;

            typedef struct Font {
                /* 0x00 */ u32 nameStrOffset;
                /* 0x04 */ u8 type;
                u8 padding[3];
            } Font;

            typedef struct TextBox : public Pane {
                /* 0x4C */ u16 textBufBytes;
                /* 0x4E */ u16 textStrBytes;
                /* 0x50 */ u16 materialIdx;
                /* 0x52 */ u16 fontIdx;
                /* 0x54 */ u8 textPosition;
                u8 padding[3];
                /* 0x58 */ u32 textStrOffset;
                /* 0x5C */ u32 textCols[TEXTCOLOR_MAX];
                /* 0x64 */ Size fontSize;
                /* 0x6C */ f32 charSpace;
                /* 0x70 */ f32 lineSpace;
            } TextBox;

            typedef struct WindowFrame {
                /* 0x00 */ u16 materialIdx;
                /* 0x02 */ u8 textureFlip;
                /* 0x03 */ u8 padding1;
            } WindowFrame;

            typedef struct WindowContent {
                /* 0x00 */ u32 vtxCols[VERTEXCOLOR_MAX];
                /* 0x10 */ u16 materialIdx;
                /* 0x12 */ u8 texCoordNum;
                /* 0x13 */ u8 padding[1];
            } WindowContent;

            typedef struct Window : public Pane {
                /* 0x4C */ InflationLRTB inflation;
                /* 0x5C */ u8 frameNum;
                /* 0x5D */ u8 padding1;
                /* 0x5E */ u8 padding2;
                /* 0x5F */ u8 padding3;
                /* 0x60 */ u32 contentOffset;
                /* 0x64 */ u32 frameOffsetTableOffset;
            } Window;

            /*** GROUP ***/

            typedef struct Group {
                /* 0x00 */ DataBlockHeader blockHeader;
                /* 0x08 */ char name[16];
                /* 0x18 */ u16 paneNum;
                /* 0x19 */ u8 padding[2];
            } Group;

            /*** LAYOUT ***/

            typedef struct Layout {
                /* 0x00 */ DataBlockHeader blockHeader;
                /* 0x08 */ u8 originType;
                /* 0x09 */ u8 padding[3];
                /* 0x0C */ Size layoutSize;
            } Layout;

        }  // namespace res

        namespace res {
            typedef struct TextureList {
                /* 0x00 */ DataBlockHeader blockHeader;
                /* 0x08 */ u16 texNum;
                u8 padding[2];
            } TextureList;

            typedef struct FontList {
                /* 0x00 */ DataBlockHeader blockHeader;
                /* 0x08 */ u16 fontNum;
                u8 padding[2];
            } FontList;

            typedef struct MaterialList {
                /* 0x00 */ DataBlockHeader blockHeader;
                /* 0x08 */ u16 materialNum;
                u8 padding[2];
            } MaterialList;
        }  // namespace res

        typedef struct ResBlockSet {
            /* 0x00 */ const res::TextureList* pTextureList;
            /* 0x04 */ const res::FontList* pFontList;
            /* 0x08 */ const res::MaterialList* pMaterialList;
            /* 0x0C */ ResourceAccessor* pResAccessor;
        } ResBlockSet;

    }  // namespace lyt
}  // namespace nw4hbm

#endif
