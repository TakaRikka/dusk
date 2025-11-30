#ifndef FVB_H
#define FVB_H

#include "JSystem/JGadget/linklist.h"
#include "JSystem/JStudio/JStudio/fvb-data-parse.h"
#include "JSystem/JStudio/JStudio/object-id.h"

namespace JStudio {
namespace fvb {

class TControl;

class TParse : public TParse_header_block {
public:
    TParse(JStudio::fvb::TControl*);
    virtual ~TParse();
    virtual bool parseHeader_next(void const**, u32*, u32);
    virtual bool parseBlock_next(void const**, u32*, u32);

    TControl* getControl() const { return pControl_; }

private:
    TControl* pControl_;
};

class TObject : public object::TObject_ID {
public:
    TObject(const data::TParse_TBlock& block);
    TObject(void const* id, u32 id_size, TFunctionValue* value);

    explicit TObject(const data::TParse_TBlock& block, TFunctionValue* value)
        : TObject_ID(block.get_ID(), block.get_IDSize()), pfv_(value) {
        ASSERT(pfv_ != NULL);
    }

    virtual ~TObject() = 0;

    virtual void prepare_data_(const data::TParse_TParagraph::TData& data, TControl* control) = 0;

    void prepare(const data::TParse_TBlock& block, TControl* control);

    TFunctionValue* const referFunctionValue() { return pfv_; }

protected:
    /* 0x0C */ JGadget::TLinkListNode mNode;
    /* 0x14 */ TFunctionValue* pfv_;
};

class TFactory {
public:
    TFactory() {}

    virtual ~TFactory();
    virtual TObject* create(JStudio::fvb::data::TParse_TBlock const&);
    virtual void destroy(JStudio::fvb::TObject*);
};

class TControl {
public:
    TControl();
    virtual ~TControl();

    void appendObject(JStudio::fvb::TObject*);
    void removeObject(JStudio::fvb::TObject*);
    void destroyObject(JStudio::fvb::TObject*);
    void destroyObject_all();
    TObject* getObject(void const*, u32);
    TObject* getObject_index(u32);

    TFactory* getFactory() const { return pFactory; }
    void setFactory(TFactory* factory) { pFactory = factory; }

private:
    /* 0x4 */ TFactory* pFactory;
    /* 0x8 */ JGadget::TLinkList<TObject, -12> ocObject_;
};  // Size: 0x14

class TObject_composite : public TObject {
public:
    TObject_composite(JStudio::fvb::data::TParse_TBlock const&);
    virtual void prepare_data_(JStudio::fvb::data::TParse_TParagraph::TData const&,
                                              JStudio::fvb::TControl*);
    virtual ~TObject_composite() {}

private:
    TFunctionValue_composite fnValue;
};

class TObject_constant : public TObject {
public:
    TObject_constant(data::TParse_TBlock const&);
    virtual ~TObject_constant() {}

    virtual void prepare_data_(data::TParse_TParagraph::TData const&, TControl*);

private:
    TFunctionValue_constant fnValue;
};

class TObject_transition : public TObject {
public:
    TObject_transition(data::TParse_TBlock const&);
    virtual ~TObject_transition() {}

    virtual void prepare_data_(data::TParse_TParagraph::TData const&, TControl*);

private:
    TFunctionValue_transition fnValue;
};

class TObject_list : public TObject {
public:
    struct ListData {
        /* 0x0 */ f32 _0;
        /* 0x4 */ u32 _4;
        /* 0x8 */ f32 _8[0];
    };
    TObject_list(data::TParse_TBlock const&);
    virtual ~TObject_list() {}

    virtual void prepare_data_(data::TParse_TParagraph::TData const&, TControl*);

private:
    TFunctionValue_list fnValue;
};

class TObject_list_parameter : public TObject {
public:
    struct ListData {
        u32 _0;
        f32 _4[0];
    };
    TObject_list_parameter(data::TParse_TBlock const&);
    virtual ~TObject_list_parameter() {}

    virtual void prepare_data_(data::TParse_TParagraph::TData const&, TControl*);

private:
    TFunctionValue_list_parameter fnValue;
};

struct TObject_hermite : public TObject {
public:
    struct ListData {
        u32 _0;  // u : 28, uSize : 4
        f32 _4[0];
    };
    TObject_hermite(data::TParse_TBlock const&);
    virtual ~TObject_hermite() {}

    virtual void prepare_data_(data::TParse_TParagraph::TData const&, TControl*);

private:
    TFunctionValue_hermite fnValue;
};

}  // namespace fvb
}  // namespace JStudio

#endif /* FVB_H */
