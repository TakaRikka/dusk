#ifndef JMESSAGE_CONTROL_H
#define JMESSAGE_CONTROL_H

#include "JSystem/JMessage/processor.h"

namespace JMessage {

/**
 * @ingroup jsystem-jmessage
 * 
 */
struct TControl {
    TControl();
    virtual ~TControl();

    void reset();
    int update();
    void render();
    int setMessageCode(u16 u16GroupID, u16 u16Index);
    int setMessageID(u32 uMsgID, u32 param_1, bool* pbValid);
    bool setMessageCode_inSequence_(const TProcessor* pProcessor, u16 u16GroupID, u16 u16Index);

    bool isReady_update_() const { return pMessageText_begin_ != 0 && pSequenceProcessor_ != NULL; }
    bool isReady_render_() const { return pMessageText_current_ != 0 && pRenderingProcessor_ != NULL; }

    TProcessor* getProcessor() const {
        if (pSequenceProcessor_ != NULL) {
            return (TProcessor*)pSequenceProcessor_;
        } else {
            return (TProcessor*)pRenderingProcessor_;
        }                                   
    }

    int setMessageCode(u32 uCode) {
        return setMessageCode(uCode >> 16, uCode);
    }

    int setMessageCode_inReset_(const TProcessor* pProcessor, u16 u16GroupID, u16 u16Index) {
        JUT_ASSERT(138, pEntry_==NULL);
        JUT_ASSERT(139, pszText_update_current_==NULL);
        JUT_ASSERT(140, oStack_renderingProcessor_.empty());
        
        if (!setMessageCode_inSequence_(pProcessor, u16GroupID, u16Index)) {
            return 0;
        }

        if (isReady_update_()) {
            pSequenceProcessor_->setBegin_messageEntryText(pResourceCache_, pEntry_, pMessageText_begin_);
        }

        return 1;
    }

    const char* getMessageText_begin() const { return pMessageText_begin_; }
    void* getMessageEntry() const { return pEntry_; }
    u32 getMessageCode() const { return (uMessageGroupID_ << 16) | uMessageID_; }
    void setSequenceProcessor(TSequenceProcessor* pProcessor) { pSequenceProcessor_ = pProcessor; }
    void setRenderingProcessor(TRenderingProcessor* pProcessor) { pRenderingProcessor_ = pProcessor; }

    void resetResourceCache() { 
        if (pSequenceProcessor_ != NULL) {
            pSequenceProcessor_->resetResourceCache();
        }

        if (pRenderingProcessor_ != NULL) {
            pRenderingProcessor_->resetResourceCache();
        }

        pResourceCache_ = NULL;
    }

    void render_synchronize() {
        if (isReady_render_()) {
            pMessageText_current_ = pszText_update_current_;
            oStack_renderingProcessor_ = pRenderingProcessor_->oStack_;
        }
    }

    /* 0x04 */ TSequenceProcessor* pSequenceProcessor_;
    /* 0x08 */ TRenderingProcessor* pRenderingProcessor_;
    /* 0x0C */ u16 uMessageGroupID_;
    /* 0x0E */ u16 uMessageID_;
    /* 0x10 */ const TResource* pResourceCache_;
    /* 0x14 */ void* pEntry_;
    /* 0x18 */ const char* pMessageText_begin_;
    /* 0x1C */ const char* pszText_update_current_;
    /* 0x20 */ const char* pMessageText_current_;
    /* 0x24 */ TProcessor::TStack_ oStack_renderingProcessor_;
};
};  // namespace JMessage

#endif /* JMESSAGE_CONTROL_H */
