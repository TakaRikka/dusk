#ifndef STB_DATA_H
#define STB_DATA_H

#include "dolphin/types.h"

namespace JStudio {
namespace stb {
namespace data {

const int guBit_TSequence_type = 24;

const int BLOCK_SOUND = 'JSND';
const int BLOCK_ACTOR = 'JACT';
const int BLOCK_AMBIENTLIGHT = 'JABL';
const int BLOCK_CAMERA = 'JCMR';
const int BLOCK_FOG = 'JFOG';
const int BLOCK_LIGHT = 'JLIT';
const int BLOCK_MESSAGE = 'JMSG';
const int BLOCK_PARTICLE = 'JPTC';
const int BLOCK_NONE = -1;

// Used to expand a signed 24 int to a signed 32 int
const u32 gu32Mask_TSequence_value_signExpansion = 0xFF000000;
extern const u32 ga4cSignature;  // 'STB/0'
extern const s32 gauDataSize_TEParagraph_data[8];

inline void toString_block(char* a5c, u32 arg1) {
    // from debug, todo
}

struct THeader {
    struct Target {
        /* 0x00 */ char name[8];  // "jstudio"
        /* 0x08 */ u16 _8[3];
        /* 0x0E */ u16 target_version;
    };

    /* 0x00 */ char signature[4];
    /* 0x04 */ u16 byte_order;  // must be 0xFEFF
    /* 0x06 */ u16 version;     // 0-1 = obselete, 2-7 = OK
    /* 0x08 */ u32 _8;
    /* 0x0C */ u32 block_number;
    /* 0x10 */ Target target;
    /* 0x20 */ u8 content[0];
};

struct TBlock {
    /* 0x0 */ u32 size;
    /* 0x4 */ u32 type;  // char[4] JMSG, JSND, JACT, ...
};

struct TBlock_object : TBlock {
    /* 0x8 */ u16 flag;
    /* 0xA */ u16 id_size;
    /* 0xC */ u8 id[0];  // unique identifier
    ///* ??? */ u8 content[0];
};

struct TParagraph {
    /* 0x0 */ u16 _0;
    /* 0x2 */ u16 id_size;
    /* 0x4 */ u8 id[0];  // unique identifier
};

}  // namespace data
}  // namespace stb
}  // namespace JStudio

#endif /* STB_DATA_H */
