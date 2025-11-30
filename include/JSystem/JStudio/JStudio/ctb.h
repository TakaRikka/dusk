#ifndef CTB_H
#define CTB_H

#include "JSystem/JGadget/binary.h"
#include "JSystem/JGadget/linklist.h"
#include "JSystem/JStudio/JStudio/object-id.h"
#include "JSystem/JUtility/JUTAssert.h"

namespace JStudio {
namespace ctb {
struct TObject : public object::TObject_ID {
    TObject(const void* id, u32 size, const void* param_2) : object::TObject_ID(id, size) {
        pData_ = param_2;
        JUT_ASSERT(82, pData_!=NULL);
    }
    virtual ~TObject() = 0;
    virtual int getScheme() const = 0;
    const void* getData() const { return pData_; }

    /* 0x08 vtable */
    /* 0x0C */ JGadget::TLinkListNode ocObject_;
    /* 0x14 */ const void* pData_;
};

struct data {
    struct THeaderData {
        u32 signature;
        u16 byteOrder;
        u16 version;
        u8 field_0x0[4];
        u32 blockNumber;
        u8 content[0];
    };

    struct TParse_THeader : public JGadget::binary::TParseData_aligned<4> {
        TParse_THeader(const void* pContent) : JGadget::binary::TParseData_aligned<4>(pContent) {}
        const THeaderData* get() const {
            return (THeaderData*) getRaw();
        }

        const void* get_signature() const {
            return &get()->signature;
        }

        u32 get_blockNumber() const {
            return get()->blockNumber;
        }

        u16 get_byteOrder() const {
            return get()->byteOrder;
        }

        u16 get_version() const {
            return get()->version;
        }

        const void* getContent() const {
            const THeaderData* header = (THeaderData*) getRaw();
            return header->content;
        }
    };

    struct TBlockData {
        u32 size;
        u16 scheme;
        u16 IDSize;
        u32 field_0x8[0];
    };

    struct TParse_TBlock : public JGadget::binary::TParseData_aligned<4> {
        TParse_TBlock(const void* pContent) : JGadget::binary::TParseData_aligned<4>(pContent) {}
        const TBlockData* get() const {
            return (const TBlockData*)getRaw();
        }

        u32 get_size() const {
            return get()->size;
        }

        u16 get_scheme() const {
            return get()->scheme;
        }

        u16 get_IDSize() const {
            return get()->IDSize;
        }

        const void* getBlockEnd_() const {
            return get()->field_0x8;
        }

        const void* get_ID() const {
            const void* rv = NULL;
            if (get_IDSize() != 0) {
                rv = getBlockEnd_();
            }
            return rv;
        }

        const void* getContent() const {
            return (const void*)((char*)getBlockEnd_() + JGadget::binary::align_roundUp((u16)get_IDSize(), 4));
        }

        const TParse_TBlock* getNext() {
            return (TParse_TBlock*)((char*)getRaw() + get_size());
        }
    };

    static const u32 ga4cSignature;
};

struct TObject_TxyzRy : public TObject {
    TObject_TxyzRy(JStudio::ctb::data::TParse_TBlock const&);
    
    virtual ~TObject_TxyzRy() {}
    virtual int getScheme() const;
};

struct TFactory {
    TFactory() {}

    virtual ~TFactory();
    virtual TObject* create(JStudio::ctb::data::TParse_TBlock const&);
    virtual void destroy(JStudio::ctb::TObject*);
};

struct TControl {
    TControl();
    virtual ~TControl();
    void appendObject(JStudio::ctb::TObject*);
    void removeObject(JStudio::ctb::TObject*);
    void destroyObject(JStudio::ctb::TObject*);
    void destroyObject_all();
    JStudio::ctb::TObject* getObject(void const*, u32);
    JStudio::ctb::TObject* getObject_index(u32);

    TFactory* getFactory() { return pFactory_; }
    void setFactory(TFactory* factory) { pFactory_ = factory; }

    /* 0x4 */ TFactory* pFactory_;
    /* 0x8 */ JGadget::TLinkList<TObject, -12> mList;
};

struct TParse : public JGadget::binary::TParse_header_block {
    TParse(JStudio::ctb::TControl*);
    virtual ~TParse();
    virtual bool parseHeader_next(void const**, u32*, u32);
    virtual bool parseBlock_next(void const**, u32*, u32);

    TControl* getControl() { return pControl_; }

    /* 0x4 */ TControl* pControl_;
};

};  // namespace ctb
};  // namespace JStudio

#endif /* CTB_H */
