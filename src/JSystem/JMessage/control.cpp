/**
 * JMessage/control.cpp
 * JMessage Controller
 */

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JMessage/control.h"

JMessage::TControl::TControl()
    : pSequenceProcessor_(NULL),
      pRenderingProcessor_(NULL),
      uMessageGroupID_(0xFFFF),
      uMessageID_(0xFFFF),
      pResourceCache_(NULL),
      pEntry_(NULL),
      pMessageText_begin_(NULL),
      pszText_update_current_(NULL),
      pMessageText_current_(NULL)
      {}

JMessage::TControl::~TControl() {}

void JMessage::TControl::reset() {
    pEntry_ = NULL;
    pMessageText_begin_ = NULL;
    pszText_update_current_ = NULL;
    pMessageText_current_ = NULL;
    oStack_renderingProcessor_.clear();

    if (pSequenceProcessor_ != NULL) {
        pSequenceProcessor_->reset();
    }

    if (pRenderingProcessor_ != NULL) {
        pRenderingProcessor_->reset();
    }
}

int JMessage::TControl::update() {
    if (!isReady_update_()) {
        return 0;
    }

    pszText_update_current_ = pSequenceProcessor_->process(NULL);
    if (pszText_update_current_ == NULL) {
        pMessageText_begin_ = 0;
        return 0;
    }

    return 1;
}

void JMessage::TControl::render() {
    if (isReady_render_()) {
        pRenderingProcessor_->setBegin_messageEntryText(pResourceCache_, pEntry_, pMessageText_current_);
        pRenderingProcessor_->oStack_ = oStack_renderingProcessor_;
        pRenderingProcessor_->process(pszText_update_current_);
    }
}

int JMessage::TControl::setMessageCode(u16 u16GroupID, u16 u16Index) {
    TProcessor* pProcessor = getProcessor();
    JUT_ASSERT(120, pProcessor!=NULL);
    return setMessageCode_inReset_(pProcessor, u16GroupID, u16Index);
}

int JMessage::TControl::setMessageID(u32 uMsgID, u32 param_1, bool* pbValid) {
    TProcessor* pProcessor = getProcessor();
    JUT_ASSERT(132, pProcessor!=NULL);

    u32 uCode = pProcessor->toMessageCode_messageID(uMsgID, param_1, pbValid);
    if (uCode == 0xFFFFFFFF) {
        return 0;
    }

    return setMessageCode_inReset_(pProcessor, uCode >> 16, uCode & 0xFFFF);
}

bool JMessage::TControl::setMessageCode_inSequence_(JMessage::TProcessor const* pProcessor, u16 u16GroupID, u16 u16Index) {
    pEntry_ = pProcessor->getMessageEntry_messageCode(u16GroupID, u16Index);
    if (pEntry_ == NULL) {
        return false;
    }

    uMessageGroupID_ = u16GroupID;
    uMessageID_ = u16Index;
    
    pResourceCache_ = pProcessor->getResourceCache();
    JUT_ASSERT(155, pResourceCache_!=NULL);

    pMessageText_begin_ = pResourceCache_->getMessageText_messageEntry(pEntry_);
    pMessageText_current_ = pMessageText_begin_;
    oStack_renderingProcessor_.clear();
    return true;
}
