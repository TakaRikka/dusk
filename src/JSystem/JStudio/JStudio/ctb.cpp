#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStudio/JStudio/ctb.h"
#include <iterator>
#include <string>
#include <algorithm>

JStudio::ctb::TObject::~TObject() {}

JStudio::ctb::TObject_TxyzRy::TObject_TxyzRy(JStudio::ctb::data::TParse_TBlock const& param_0)
    : TObject(param_0.get_ID(), param_0.get_IDSize(), param_0.getContent()) {}

int JStudio::ctb::TObject_TxyzRy::getScheme() const {
    return 1;
}

JStudio::ctb::TControl::TControl() : pFactory_(NULL) {}

JStudio::ctb::TControl::~TControl() {
    JGADGET_ASSERTWARN(94, ocObject_.empty());
}

void JStudio::ctb::TControl::appendObject(JStudio::ctb::TObject* p) {
    JUT_ASSERT(106, p!=NULL)
    ocObject_.Push_back(p);
}

void JStudio::ctb::TControl::removeObject(JStudio::ctb::TObject* p) {
    JUT_ASSERT(113, p!=NULL)
    ocObject_.Erase(p);
}

void JStudio::ctb::TControl::destroyObject(JStudio::ctb::TObject* param_0) {
    removeObject(param_0);
    TFactory *pFactory = getFactory();
    JUT_ASSERT(129, pFactory!=NULL);
    pFactory->destroy(param_0);
}

void JStudio::ctb::TControl::destroyObject_all() {
    while (!ocObject_.empty()) {
        destroyObject(&ocObject_.back());
    }
}

// NONMATCHING - TPRObject_ID_equal issues
JStudio::ctb::TObject* JStudio::ctb::TControl::getObject(void const* param_0, u32 param_1) {
    JGadget::TLinkList<TObject, -12>::iterator begin = ocObject_.begin();
    JGadget::TLinkList<TObject, -12>::iterator end = ocObject_.end();
    JGadget::TLinkList<TObject, -12>::iterator local_50 = std::find_if(begin, end, object::TPRObject_ID_equal(param_0, param_1));
    if ((local_50 != end) != false) {
        return &*local_50;
    }
    return NULL;
}

JStudio::ctb::TObject* JStudio::ctb::TControl::getObject_index(u32 param_0) {
    if (param_0 >= ocObject_.size()) {
        return 0;
    }
    JGadget::TLinkList<TObject, -12>::iterator aiStack_14 = ocObject_.begin();
    std::advance(aiStack_14, param_0);
    return &*aiStack_14;
}

JStudio::ctb::TFactory::~TFactory() {}

JStudio::ctb::TObject* JStudio::ctb::TFactory::create(JStudio::ctb::data::TParse_TBlock const& param_0) {
    switch(param_0.get_scheme()) {
    case 1:
        return new TObject_TxyzRy(param_0);
    default:
        return NULL;
    }
}

void JStudio::ctb::TFactory::destroy(JStudio::ctb::TObject* param_0) {
    delete param_0;
}

// NONMATCHING TParse_header_block vtable location
JStudio::ctb::TParse::TParse(JStudio::ctb::TControl* param_0) : pControl_(param_0) {}

JStudio::ctb::TParse::~TParse() {}

bool JStudio::ctb::TParse::parseHeader_next(void const** ppData_inout, u32* puBlock_out, u32 param_3) {
    JUT_ASSERT(221, ppData_inout!=NULL);
    JUT_ASSERT(222, puBlock_out!=NULL);
    void const* pData = *ppData_inout;
    JUT_ASSERT(224, pData!=NULL);
    data::TParse_THeader aTStack_478(pData);
    *ppData_inout = aTStack_478.getContent();
    *puBlock_out = aTStack_478.get_blockNumber();
    if (memcmp(aTStack_478.get_signature(), &JStudio::ctb::data::ga4cSignature, sizeof(JStudio::ctb::data::ga4cSignature)) != 0) {
        return false;
    }
    if (aTStack_478.get_byteOrder() != 0xfeff) {
        return false;
    }
    u16 version = aTStack_478.get_version();
    if (version < 1) {
        return false;
    }
    return version <= 1;
}

bool JStudio::ctb::TParse::parseBlock_next(void const** ppData_inout, u32* puData_out, u32 param_3) {
    JUT_ASSERT(260, ppData_inout!=NULL);
    JUT_ASSERT(261, puData_out!=NULL);
    void const* pData = *ppData_inout;
    JUT_ASSERT(263, pData!=NULL);
    data::TParse_TBlock aTStack_260(pData);
    *ppData_inout = aTStack_260.getNext();
    *puData_out = aTStack_260.get_size();
    JStudio::ctb::TControl* pControl = getControl();
    JUT_ASSERT(269, pControl!=NULL);
    if ((param_3 & 0x10) != 0) {
        if (pControl->getObject(aTStack_260.get_ID(), aTStack_260.get_IDSize()) != NULL) {
            return true;
        }
    }
    if ((param_3 & 0x20) != 0) {
        return true;
    }
    JStudio::ctb::TFactory* pFactory = pControl->getFactory();
    if (pFactory == NULL) {
       return false;
    } 

    JStudio::ctb::TObject* pTVar6 = pFactory->create(aTStack_260);
    if (pTVar6 == NULL) {
        if ((param_3 & 0x40) == 0) {
            return false;
        } else {
            return true;
        }
    } 
    pControl->appendObject(pTVar6);
    return true;
}
