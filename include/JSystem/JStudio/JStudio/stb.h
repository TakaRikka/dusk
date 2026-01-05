#ifndef STB_H
#define STB_H

#include "JSystem/JGadget/linklist.h"
#include "JSystem/JStudio/JStudio/object-id.h"
#include "JSystem/JStudio/JStudio/stb-data-parse.h"
#include "dolphin/os.h"

namespace JStudio {
class TObject;
namespace stb {

class TControl;

class TParse : public TParse_header_block {
public:
    TParse(TControl*);
    virtual ~TParse();
    virtual bool parseHeader_next(void const**, u32*, u32);
    virtual bool parseBlock_next(void const**, u32*, u32);
    virtual bool parseHeader(data::TParse_THeader const&, u32);
    virtual bool parseBlock_block(data::TParse_TBlock const&, u32);
    virtual bool parseBlock_object(data::TParse_TBlock_object const&, u32);

    TControl* getControl() const { return pControl; }

private:
    TControl* pControl;
};

class TObject : public object::TObject_ID {
public:
    enum TEStatus {
        /* 0x0 */ STATUS_STILL = 0,
        /* 0x1 */ STATUS_END = 1 << 0,
        /* 0x2 */ STATUS_WAIT = 1 << 1,
        /* 0x4 */ STATUS_SUSPEND = 1 << 2,
        /* 0x8 */ STATUS_INACTIVE = 1 << 3,
    };

    TObject(data::TParse_TBlock_object const&);
    explicit TObject(u32, void const*, u32);
    virtual ~TObject();

    void setFlag_operation(u8, int);
#if PLATFORM_SHIELD && !DEBUG
    void reset(void const* arg1) {
        bSequence_ = 0;
        mStatus = STATUS_STILL;
        pSequence_next = arg1;
        u32Wait_ = 0;
    }
#else
    void reset(void const*);
#endif
#if !DEBUG
    void reset() { reset(NULL); }
#else
    void reset();
#endif
    bool forward(u32);
    virtual void do_begin();
    virtual void do_end();
    virtual void do_paragraph(u32, void const*, u32);
    virtual void do_wait(u32);
    virtual void do_data(void const*, u32, void const*, u32);
    void process_sequence_();
    void process_paragraph_reserved_(u32, void const*, u32);

    const char* toString_status(int status);

    void on_begin() { do_begin(); }
    void on_end() { do_end(); }
    void on_paragraph(u32 arg1, const void* arg2, u32 arg3) { do_paragraph(arg1, arg2, arg3); }
    void on_wait(u32 arg1) { do_wait(arg1); }
    void on_data(const void* arg1, u32 arg2, const void* arg3, u32 arg4) {
        do_data(arg1, arg2, arg3, arg4);
    }

    TControl* getControl() const { return pControl; }
    void setControl_(TControl* control) { pControl = control; }
    int getSuspend() const { return _20; }
    void setSuspend(s32 val) { _20 = val; }
    bool isSuspended() const { return getSuspend() > 0; }
    void suspend(s32 val) { _20 += val; }
    const void* getSequence() const { return pSequence; }
    void setSequence_(const void* arg1) { pSequence = arg1; }
    const void* getSequence_offset(s32 i_no) const {
        int s32Val = (s32)getSequence();
        return (const void*)(s32Val + i_no);
    }
    const void* getSequence_next() const { return pSequence_next; }
    void setSequence_next(const void* seq) { pSequence_next = seq; }
    u32 getWait() const { return u32Wait_; }
    void setWait(u32 wait) { u32Wait_ = wait; }
    TEStatus getStatus() const { return mStatus; }
    void setStatus_(TEStatus status) { mStatus = status; }
    u32 toInt32FromUInt24_(u32 val) {
        if (val & 0x800000) {
            val |= data::gu32Mask_TSequence_value_signExpansion;
        }
        return val;
    }
    void setFlag_operation_(u32 u32Data) {
        ASSERT((u32Data >> data::guBit_TSequence_type) == 0);
        setFlag_operation(u32Data >> 16, u32Data & 0xFFFF);
    }

    /* 0x10 */ JGadget::TLinkListNode ocObject_;

public: // private: // public for the fakematch in JStudio_JStage::TAdaptor_actor::adaptor_do_begin
    /* 0x14 */ TControl* pControl;
    /* 0x18 */ u32 signature;
    /* 0x1C */ u16 mFlag;
    /* 0x1E */ u8 bSequence_;
    /* 0x20 */ u32 _20;  // "second per frame"?
    /* 0x24 */ const void* pSequence;
    /* 0x28 */ const void* pSequence_next;
    /* 0x2C */ u32 u32Wait_;
    /* 0x30 */ TEStatus mStatus;
};

class TFactory {
public:
    TFactory() {}

    virtual ~TFactory();
    virtual JStudio::TObject* create(data::TParse_TBlock_object const&);
    virtual void destroy(TObject*);
};

class TObject_control : public TObject {
public:
    TObject_control(void const*, u32);
};

// Manages TObjects
class TControl {
public:
    TControl();
    virtual ~TControl();

    void appendObject(TObject*);
    void removeObject(TObject*);
    void destroyObject(TObject*);
    void destroyObject_all();
    TObject* getObject(void const*, u32);
    void reset();
    bool forward(u32);

    void setStatus_(u32 status) { mStatus = status; }
    void resetStatus_() { setStatus_(0); }
    bool isSuspended() const { return _54 > 0; }
    TFactory* getFactory() const { return pFactory; }
    void setFactory(TFactory* factory) { pFactory = factory; }
    TObject_control& referObject_control() { return mObject_control; }
    int getSuspend() const { return _54; }
    void setSuspend(s32 suspend) { mObject_control.setSuspend(suspend); }
    void suspend(s32 param_0) { mObject_control.suspend(param_0); }
    void unsuspend(s32 param_0) { suspend(-param_0); }

private:
    /* 0x04 */ u32 _4;
    /* 0x08 */ u32 _8;
    /* 0x0C */ TFactory* pFactory;
    /* 0x10 */ JGadget::TLinkList<TObject, -12> ocObject_;
    /* 0x1C */ u32 mStatus;
    /* 0x20 */ TObject_control mObject_control;
    /* 0x54 */ s32 _54;
};

template <int S>
struct TParseData : public data::TParse_TParagraph_data::TData {
    TParseData(const void* pContent) {
        set(data::TParse_TParagraph_data(pContent));
    }

    TParseData() {
        set(NULL);
    }

    void set(const data::TParse_TParagraph_data& data) {
        data.getData(this);
    }

    void set(const void* pContent) {
        set(data::TParse_TParagraph_data(pContent));
    }

    bool isEnd() const {
        return status == 0;
    }

    bool empty() const {
        return content == NULL;
    }

    bool isValid() const {
        return !empty() && status == S;
    }

    const void* getContent() const { return content; }

    u32 size() const { return entryCount; }
};

template <int S, class Iterator=JGadget::binary::TValueIterator_raw<u8> >
struct TParseData_fixed : public TParseData<S> {
    TParseData_fixed(const void* pContent) : TParseData<S>(pContent) {}
    TParseData_fixed() : TParseData<S>() {}

    const void* getNext() const {
        return this->next;
    }

    bool isValid() const {
        return TParseData<S>::isValid() && getNext() != NULL;
    }

    Iterator begin() const {
        return Iterator(this->content);
    }

    Iterator end() const {
        Iterator i(this->content);
        i += this->size();
        return i;
    }

    typename Iterator::ValueType front() const {
        return *begin();
    }

    typename Iterator::ValueType back() const {
        return *--end();
    }
};

struct TParseData_string : public TParseData<0x60> {
    TParseData_string(const void* pContent) : TParseData<0x60>(pContent) {}
    TParseData_string() : TParseData<0x60>() {}

    const char* getData() const { return (const char*)getContent(); }
};

}  // namespace stb
}  // namespace JStudio

#endif /* STB_H */
