#ifndef JMESSAGE_DATA_H
#define JMESSAGE_DATA_H

#include "JSystem/JGadget/binary.h"

namespace JMessage {

/**
 * @ingroup jsystem-jmessage
 * 
 */
struct data {
    // TParse_THeader handles parsing the BMG file header data
    struct TParse_THeader : public JGadget::binary::TParseData_aligned<4> {
        TParse_THeader(const void* pData) : TParseData_aligned(pData) {}

        const void* getContent() const { return (char*)getRaw() + 0x20; }
        char* get() const { return (char*)getRaw(); }

        u32* get_signature() const { return (u32*)(get() + 0x0); }
        u32 get_type() const { return *(u32*)(get() + 0x4); }
        u32 get_blockNumber() const { return *(u32*)(get() + 0xC); }
        u32 get_encoding() const { return *(u8*)(get() + 0x10); }
    };

    // TParse_TBlock handles parsing the generic section header data
    struct TParse_TBlock : public JGadget::binary::TParseData_aligned<4> {
        TParse_TBlock(const void* pData) : TParseData_aligned(pData) {}

        const char* get() const { return (char*)getRaw(); }
        u32 get_size() const { return *(u32*)(get() + 0x4); }
        const void* getNext() const { return (char*)getRaw() + get_size(); }
        u32 get_type() const { return *(u32*)(get() + 0x0); }
    };

    // TParse_TBlock_info handles parsing INF1 section data
    struct TParse_TBlock_info : public TParse_TBlock {
        TParse_TBlock_info(const void* pData) : TParse_TBlock(pData) {}

        char* get() const { return (char*)getRaw(); }

        char* getContent() const { return (char*)getRaw() + 0x10; }

        u32 get_messageEntrySize() const { return *(u16*)(get() + 0xA); }

        u32 get_messageEntryNumber() const { return *(u16*)(get() + 0x8); }

        u16 get_groupID() const { return *(u16*)(get() + 0xC); }
    };

    // TParse_TBlock_messageID handles parsing MID1 section data
    struct TParse_TBlock_messageID : public TParse_TBlock {
        TParse_TBlock_messageID(const void* pData) : TParse_TBlock(pData) {}

        char* get() const { return (char*)getRaw(); }
        u8 get_formSupplement() const { return *(u8*)(get() + 0xB); }
        int get_number() const { return *(u16*)(get() + 0x8); }
        u32* getContent() const { return (u32*)((uintptr_t)getRaw() + 0x10); }
        u32 get_form() const { return *(u8*)(get() + 0xA) & 0xF; }
        bool get_isOrdered() const { return *(u8*)(get() + 0xA) & 0xF0; }
    };

    struct TParse_TBlock_color : public TParse_TBlock {
        TParse_TBlock_color(const void* pData) : TParse_TBlock(pData) {}
    };

    // TParse_TBlock_messageText handles parsing DAT1 section data
    struct TParse_TBlock_messageText : public TParse_TBlock {
        TParse_TBlock_messageText(const void* pData) : TParse_TBlock(pData) {}

        char* getContent() const { return (char*)getRaw() + 0x8; }
    };

    // TParse_TBlock_stringAttribute handles parsing STR1 section data
    struct TParse_TBlock_stringAttribute : public TParse_TBlock {
        TParse_TBlock_stringAttribute(const void* pData) : TParse_TBlock(pData) {}

        char* getContent() const { return (char*)getRaw() + 0x8; }
    };

    static unsigned int getTagCode(u32 tag) { return tag & 0xFFFF; }
    static u8 getTagGroup(u32 tag) { return tag >> 0x10; }

    static const u32 ga4cSignature;
    static const u32 ga4cSignature_color;

    static const int gcTagBegin = '\x1A';  // All text Control Tags will begin with this character

    enum {
        MESSAGEINDEX_USER_ENUM_MIN = 0x0000,
        MESSAGEINDEX_USER_ENUM_MAX = 0xFEFF,

        MESSAGEINDEX_RESERVED_ENUM_MIN = 0xFF00,
        MESSAGEINDEX_RESERVED_ENUM_MAX = 0xFFFF,

        MESSAGEGROUPID_USER_ENUM_MIN = 0x0000,
        MESSAGEGROUPID_USER_ENUM_MAX = 0xFFFF,
    };
};
};  // namespace JMessage

#endif /* JMESSAGE_DATA_H */
