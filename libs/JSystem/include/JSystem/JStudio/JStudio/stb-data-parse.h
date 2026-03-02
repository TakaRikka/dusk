#ifndef STB_DATA_PARSE_H
#define STB_DATA_PARSE_H

#include "JSystem/JGadget/binary.h"
#include "JSystem/JStudio/JStudio/stb-data.h"

using namespace JGadget::binary;

namespace JStudio {
namespace stb {
namespace data {

// Parses a THeader
class TParse_THeader : public TParseData_aligned<4> {
public:
    TParse_THeader(const void* p) : TParseData_aligned<4>(p) {}

    const THeader* get() const { return (THeader*)getRaw(); }
    const void* getContent() const { return ((THeader*)getRaw())->content; }
    const char* get_signature() const { return get()->signature; }
    u16 get_byteOrder() const { return get()->byte_order; }
    u16 get_version() const { return get()->version; }
    u32 get_blockNumber() const { return get()->block_number; }
    const THeader::Target& get_target() const { return get()->target; }
};

class TParse_TBlock : public TParseData_aligned<4> {
public:
    TParse_TBlock(const void* content) : TParseData_aligned<4>(content) {}

    const TBlock* get() const { return (TBlock*)getRaw(); }
    const TBlock* getNext() const { return (TBlock*)((u8*)getRaw() + get_size()); }

    u32 get_size() const { return get()->size; }
    u32 get_type() const { return get()->type; }
    const void* getContent() const { return ((char*)getRaw()) + 8;}
};

class TParse_TSequence : public TParseData_aligned<4> {
public:
    struct TData {
        /* 0x00 */ u8 type;
        /* 0x04 */ u32 param;
        /* 0x08 */ const void* content;
        /* 0x0C */ const void* next;
    };

    TParse_TSequence(const void* content) : TParseData_aligned<4>(content) {}
    void getData(TData*) const;

    const void* get() const { return getRaw(); }
    u32 get_head() const { return *(u32*)get(); }
};

class TParse_TParagraph : public TParseData_aligned<4> {
public:
    struct TData {
        /* 0x00 */ u32 type;
        /* 0x04 */ u32 param;
        /* 0x08 */ const void* content;
        /* 0x0C */ const void* next;
    };

    TParse_TParagraph(const void* content) : TParseData_aligned<4>(content) {}
    void getData(TData*) const;
};

struct TParse_TParagraph_data : public TParseData {
    struct TData {
        /* 0x00 */ u8 status;
        /* 0x04 */ u32 entrySize;
        /* 0x08 */ u32 entryCount;
        /* 0x0C */ const void* content;
        /* 0x10 */ const void* next;
    };

    TParse_TParagraph_data(const void* content) : TParseData(content) {}
    void getData(TData* pData) const;
};

// Parses a TObject ("demo object")
class TParse_TBlock_object : public TParse_TBlock {
public:
    TParse_TBlock_object(const void* content) : TParse_TBlock(content) {}

    const TBlock_object* get() const { return (TBlock_object*)getRaw(); }
    const void* getContent() const {
        return ((TBlock_object*)getRaw())->id + align_roundUp(get_IDSize(), 4);
    }

    u16 get_flag() const { return get()->flag; }
    u16 get_IDSize() const { return get()->id_size; }
    const void* get_ID() const { return get()->id; }
};

class TParse_TParagraph_dataID : public TParseData_aligned<4> {
public:
    TParse_TParagraph_dataID(const void* pContent) : TParseData_aligned<4>(pContent) {}

    const TParagraph* get() const { return (TParagraph*)getRaw(); }
    u16 get_IDSize() const { return get()->id_size; }
    const void* get_ID() const { return get()->id; }
    const void* getContent() const {
        return ((TParagraph*)getRaw())->id + align_roundUp(get_IDSize(), 4);
    }
};

}  // namespace data
}  // namespace stb
}  // namespace JStudio

#endif /* STB_DATA_PARSE_H */
