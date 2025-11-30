#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio/stb.h"
#include "JSystem/JStudio/JStudio/jstudio-object.h"
#include "JSystem/JUtility/JUTException.h"
#include <algorithm.h>
#include <string.h>

namespace JStudio {
namespace stb {

// unchecked, in debug dol
const char* TObject::toString_status(int status) {
    const char* result;

    switch (status) {
    default:
        result = "(undefined)";
        break;
    case 0:
        result = "still";
        break;
    case 1:
        result = "end";
        break;
    case 2:
        result = "wait";
        break;
    case 4:
        result = "suspend";
        break;
    case 8:
        result = "inactive";
        break;
    }
    return result;
}

TObject::TObject(u32 arg1, const void* id, u32 id_size)
    : TObject_ID(id, id_size), pControl(NULL), signature(arg1), mFlag(0), bSequence_(0), _20(0),
      pSequence(NULL), pSequence_next(NULL), u32Wait_(0), mStatus(STATUS_STILL) {}

TObject::TObject(const data::TParse_TBlock_object& object)
    : TObject_ID(object.get_ID(), object.get_IDSize()), pControl(NULL),
      signature(object.get_type()), mFlag(object.get_flag()), bSequence_(0), _20(0), pSequence(0),
      pSequence_next(object.getContent()), u32Wait_(0), mStatus(STATUS_STILL) {}

TObject::~TObject() {
    JUT_EXPECT(getControl() == NULL);
}

void TObject::setFlag_operation(u8 op, int val) {
    switch (op) {
    default:
        JUTWarn w;
        w << "unknown flag-operation : " << op;
        break;
    case 1:
        mFlag |= val;
        break;
    case 2:
        mFlag &= val;
        break;
    case 3:
        mFlag ^= val;
        break;
    }
}

void TObject::reset(const void* arg1) {
    bSequence_ = 0;
    mStatus = STATUS_STILL;
    pSequence_next = arg1;
    u32Wait_ = 0;
}

bool TObject::forward(u32 arg1) {
    bool temp = false;

    while (true) {
        if (mFlag & 0x8000) {
            switch (getStatus()) {
            case STATUS_STILL:
            case STATUS_END:
                break;
            case STATUS_WAIT:
            case STATUS_SUSPEND:
                setStatus_(STATUS_INACTIVE);
                if (bSequence_) {
                    on_end();
                }
                break;
            case STATUS_INACTIVE:
                break;
            default:
                ASSERT(false);
                break;
            }
            return true;
        }

        if (getStatus() == STATUS_INACTIVE) {
            ASSERT(bSequence_);
            on_begin();
            setStatus_(STATUS_WAIT);
        }
        ASSERT(getStatus() != STATUS_INACTIVE);

        TControl* control = getControl();
        if ((control != NULL && control->isSuspended()) || isSuspended()) {
            if (bSequence_) {
                ASSERT((getStatus() == STATUS_WAIT) || (getStatus() == STATUS_SUSPEND));
                setStatus_(STATUS_SUSPEND);
                on_wait(arg1);
            }
            return true;
        }

        while (true) {
            void* nextseq = (void*)getSequence_next();
            setSequence_(nextseq);

            if (nextseq == NULL) {
                if (bSequence_) {
                    ASSERT(getStatus() != STATUS_STILL);
                    if (!temp) {
                        on_wait(0);
                    }
                    bSequence_ = false;
                    setStatus_(STATUS_END);
                    on_end();
                }
                return false;
            }

            if (!bSequence_) {
                ASSERT(getStatus() == STATUS_STILL);
                bSequence_ = true;
                on_begin();
            }
            setStatus_(STATUS_WAIT);
            if (u32Wait_ == 0) {
                process_sequence_();
                if (u32Wait_ == 0) {
                    break;
                }
            }
            ASSERT(u32Wait_ > 0);

            temp = true;
            if (arg1 >= u32Wait_) {
                u32 wait = u32Wait_;
                arg1 -= u32Wait_;
                u32Wait_ = 0;
                on_wait(wait);
            } else {
                u32Wait_ -= arg1;
                on_wait(arg1);
                return true;
            }
        }
    }
}

void TObject::do_begin() {
    /* empty function */
}

void TObject::do_end() {
    /* empty function */
}

void TObject::do_paragraph(u32 param_0, void const* param_1, u32 param_2) {
    /* empty function */
}

void TObject::do_wait(u32 param_0) {
    /* empty function */
}

void TObject::do_data(void const* param_0, u32 param_1, void const* param_2, u32 param_3) {
    /* empty function */
}

void TObject::process_sequence_() {
    ASSERT(getWait() == 0);
    data::TParse_TSequence seq(getSequence());

    data::TParse_TSequence::TData dat;

    seq.getData(&dat);
    u8 type = dat.type;
    u32 u32Value = dat.param;
    const void* pContent = dat.content;
    const void* pNext = dat.next;
    setSequence_next(dat.next);

    switch (type) {
    case 0:
        JUT_EXPECT(u32Value == 0);
        JUT_EXPECT(pContent == NULL);
        break;
    case 1:
        JUT_EXPECT(pContent == NULL);
        setFlag_operation_(u32Value);
        break;
    case 2:
        JUT_EXPECT(pContent == NULL);
        setWait(u32Value);
        break;
    case 3: {
        JUT_EXPECT(pContent == NULL);
        s32 off = toInt32FromUInt24_(u32Value);
        void* nextseq = (void*)getSequence_offset(off);
        setSequence_next(nextseq);
        break;
    }
    case 4: {
        JUT_EXPECT(pContent == NULL);
        u32 val = toInt32FromUInt24_(u32Value);
        suspend(val);
        break;
    }
    case 0x80: {
        ASSERT(pContent != NULL);
        void* p = (void*)pContent;
        data::TParse_TParagraph para(NULL);
        while (p < pNext) {
            para.setRaw(p);

            data::TParse_TParagraph::TData para_dat;
            para.getData(&para_dat);
            if (para_dat.type <= 0xff) {
                process_paragraph_reserved_(para_dat.type, para_dat.content, para_dat.param);
            } else {
                on_paragraph(para_dat.type, para_dat.content, para_dat.param);
            }
            p = (void*)para_dat.next;
            ASSERT(p != NULL);
        }
        JUT_EXPECT(p == pNext);
        break;
    }
    default:
        JUTWarn w;
        w << "unknown sequence : " << dat.type;
        break;
    }
}

void TObject::process_paragraph_reserved_(u32 arg1, const void* pContent, u32 uSize) {
    switch (arg1) {
    case 0x1:
        ASSERT(pContent != NULL);
        ASSERT(uSize == 4);
        setFlag_operation_(*(u32*)pContent);
        break;
    case 0x2:
        ASSERT(pContent != NULL);
        ASSERT(uSize == 4);
        setWait(*(u32*)pContent);
        break;
    case 0x3: {
        ASSERT(pContent != NULL);
        ASSERT(uSize == 4);
        const void* seq = getSequence_offset(*(s32*)pContent);
        setSequence_next(seq);
        break;
    }
    case 0x80:
        on_data(NULL, 0, pContent, uSize);
        break;
    case 0x81: {
        data::TParse_TParagraph_dataID dataID(pContent);
        const void* temp = dataID.getContent();
        on_data(dataID.get_ID(), dataID.get_IDSize(), temp,
                uSize - ((uintptr_t)temp - (uintptr_t)dataID.getRaw()));
        break;
    }
    case 0x82:
        ASSERT(pContent != NULL);
        break;
    }
}

TObject_control::TObject_control(const void* arg1, u32 arg2) : TObject(-1, arg1, arg2) {}

TControl::TControl() : _4(0), _8(0), pFactory(NULL), mObject_control(NULL, 0), _54(0) {
    resetStatus_();
    mObject_control.setControl_(this);
}

// TObject_control::TObject_control() definition gets inlined here

TControl::~TControl() {
    mObject_control.setControl_(NULL);
    JUT_EXPECT(ocObject_.empty());
}

void TControl::appendObject(TObject* p) {
    p->setControl_(this);
    mObjectContainer.Push_back(p);
}

void TControl::removeObject(TObject* p) {
    ASSERT(p != NULL);
    ASSERT(p->getControl() == this);
    p->setControl_(NULL);
    mObjectContainer.Erase(p);
}

void TControl::destroyObject(TObject* p) {
    removeObject(p);
    ASSERT(pFactory != NULL);
    pFactory->destroy(p);
}

void TControl::destroyObject_all() {
    while (!mObjectContainer.empty()) {
        destroyObject(&mObjectContainer.back());
    }
}

// NONMATCHING - TPRObject_ID_equal copy issue
TObject* TControl::getObject(void const* param_0, u32 param_1) {
    JGadget::TLinkList<TObject, -12>::iterator begin = mObjectContainer.begin();
    JGadget::TLinkList<TObject, -12>::iterator end = mObjectContainer.end();
    JGadget::TLinkList<TObject, -12>::iterator local_50 = std::find_if(begin, end, object::TPRObject_ID_equal(param_0, param_1));
    if ((local_50 != end) != false) {
        return &*local_50;
    }
    return NULL;
}

void TControl::reset() {
    resetStatus_();
    mObject_control.reset(NULL);
    JGadget::TContainerEnumerator<JStudio::stb::TObject, -12> aTStack_18(&mObjectContainer);
    while (aTStack_18) {
        (*aTStack_18).reset(NULL);
    }
}

bool TControl::forward(u32 param_0) {
    _54 = mObject_control.getSuspend();
    bool rv = mObject_control.forward(param_0);
    int uVar7 = 0xf;
    int uVar6 = 0;
    JGadget::TContainerEnumerator<JStudio::stb::TObject, -12> aTStack_38(&mObjectContainer);
    while (aTStack_38) {
        JStudio::stb::TObject& this_00 = *aTStack_38;
        rv = this_00.forward(param_0) || rv;
        int uVar3 = this_00.getStatus();
        uVar7 &= uVar3;
        uVar6 |= uVar3;
    }
    setStatus_(uVar7 | (uVar6 << 0x10));
    return rv;
}

TFactory::~TFactory() {}

JStudio::TObject* TFactory::create(data::TParse_TBlock_object const& param_0) {
    return NULL;
}

void TFactory::destroy(TObject* p) {
    delete p;
}

TParse::TParse(TControl* pControl) : pControl(pControl) {}

TParse::~TParse() {}

bool TParse::parseHeader_next(const void** ppData_inout, u32* puBlock_out, u32 flags) {
    ASSERT(ppData_inout != NULL);
    ASSERT(puBlock_out != NULL);

    const void* pData = *ppData_inout;
    ASSERT(pData != NULL);

    const data::TParse_THeader header(pData);
    *ppData_inout = header.getContent();
    *puBlock_out = header.get_blockNumber();

    if (memcmp(header.get_signature(), &data::ga4cSignature, 4) != 0) {
        JUTWarn w;
        w << "unknown signature";
        return false;
    }

    if (header.get_byteOrder() != 0xFEFF) {
        JUTWarn w;
        w << "illegal byte-order";
        return false;
    }
    u16 version = header.get_version();
    if (version < 1) {
        JUTWarn w;
        w << "obselete version : " << (long)0;
        return false;
    } else if (version > 3) {
        JUTWarn w;
        w << "unknown version : " << version;
        return false;
    }
    return parseHeader(header, flags);
}

bool TParse::parseBlock_next(void const** ppData_inout, u32* puData_out, u32 flags) {
    ASSERT(ppData_inout != NULL);
    ASSERT(puData_out != NULL);

    const void* pData = *ppData_inout;
    ASSERT(pData != NULL);

    data::TParse_TBlock blk(pData);
    *ppData_inout = blk.getNext();
    *puData_out = blk.get_size();
    return parseBlock_block(blk, flags);
}

bool TParse::parseHeader(data::TParse_THeader const& param_0, u32 flags) {
    return true;
}

bool TParse::parseBlock_block(const data::TParse_TBlock& ppBlock, u32 flags) {
    return parseBlock_object(ppBlock.get(), flags);
}

bool TParse::parseBlock_object(const data::TParse_TBlock_object& ppObject, u32 flags) {
    TControl* pControl = getControl();
    ASSERT(pControl != NULL);

    if (ppObject.get_type() == data::BLOCK_NONE) {
        TObject_control& ref = pControl->referObject_control();
        ref.reset(ppObject.getContent());
        return true;
    }

    if (flags & 0x10) {
        TObject* p = pControl->getObject(ppObject.get_ID(), ppObject.get_IDSize());
        if (p != NULL) {
            p->reset(ppObject.getContent());
            return true;
        }
    }

    if (flags & 0x20)
        return true;

    TFactory* pFactory = pControl->getFactory();
    if (pFactory == NULL) {
        JUTWarn w;
        w << "factory not specified";
        return false;
    }

    JStudio::TObject* p = pFactory->create(ppObject);
    if (p == NULL) {
        if (flags & 0x40)
            return true;

        char a5c[8];
        char t[16];
        int type = ppObject.get_type();
        data::toString_block(a5c, type);

        JUTWarn w;
        w << "can't create object : " << a5c;
        w << "(0x" << type << ")";
        return false;
    }
    pControl->appendObject(p);
    return true;
}

}  // namespace stb
}  // namespace JStudio
