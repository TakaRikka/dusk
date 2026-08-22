/**
 * JMessage/control.cpp
 * JMessage Controller
 */

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JMessage/control.h"

#if TARGET_PC
#include "dusk/mods/svc/flow.hpp"
#endif

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

JMessage::TControl::~TControl() {
#if TARGET_PC
    dusk::flow::release_message_control(this);
#endif
}

void JMessage::TControl::reset() {
#if TARGET_PC
    dusk::flow::release_message_control(this);
#endif
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
#if TARGET_PC
    const TResource* resource = NULL;
    const void* resolvedEntry = NULL;
    const char* resolvedText = NULL;
    if (u16Index >= dusk::flow::kCustomMessageMin) {
        if (!dusk::flow::custom_message_for_processor(
                this, pProcessor, u16Index, resource, resolvedEntry, resolvedText))
        {
            return false;
        }
        const_cast<TProcessor*>(pProcessor)->setResourceCache(const_cast<TResource*>(resource));
    } else {
        resource = pProcessor->getResource_groupID(u16GroupID);
        if (resource == NULL) {
            return false;
        }
        const void* nativeEntry = resource->getMessageEntry_messageIndex(u16Index);
        const char* nativeText =
            nativeEntry != NULL ? resource->getMessageText_messageEntry(nativeEntry) : NULL;
        resolvedEntry = nativeEntry;
        resolvedText = nativeText;
        dusk::flow::resolve_message_for_control(this, resource->oParse_THeader_.getRaw(), u16Index,
            nativeEntry, nativeText, resolvedEntry, resolvedText);
    }
    pEntry_ = const_cast<void*>(resolvedEntry);
    if (pEntry_ == NULL || resolvedText == NULL) {
        return false;
    }
#else
    pEntry_ = pProcessor->getMessageEntry_messageCode(u16GroupID, u16Index);
    if (pEntry_ == NULL) {
        return false;
    }
#endif

#if TARGET_PC
    uMessageGroupID_ = resource->getGroupID();
    pResourceCache_ = resource;
#else
    uMessageGroupID_ = u16GroupID;
    pResourceCache_ = pProcessor->getResourceCache();
#endif
    uMessageID_ = u16Index;

    JUT_ASSERT(155, pResourceCache_!=NULL);

#if TARGET_PC
    pMessageText_begin_ = resolvedText;
#else
    pMessageText_begin_ = pResourceCache_->getMessageText_messageEntry(pEntry_);
#endif
    pMessageText_current_ = pMessageText_begin_;
    oStack_renderingProcessor_.clear();
    return true;
}
