#ifndef JMESSAGE_PROCESSOR_H
#define JMESSAGE_PROCESSOR_H

#include "JSystem/JMessage/resource.h"
#include <algorithm.h>
#include "global.h"

namespace JMessage {
struct TResource;
struct TResourceContainer;

/**
 * @ingroup jsystem-jmessage
 * 
 */
struct TReference {
    TReference() { pcResource_ = NULL; }

    virtual ~TReference();
    virtual const char* do_word(u32) const;

    int on_parseCharacter(const char** ppszText) const {
        JUT_ASSERT(97, pcResource_!=NULL);
        return pcResource_->parseCharacter(ppszText);
    }

    const char* on_word(u32 param_0) const { return do_word(param_0); }

    TResource* getResource_groupID(u16 u16GroupID) const {
        return pcResource_ == NULL ? NULL : pcResource_->getResource_groupID(u16GroupID);
    }

    TResourceContainer* getResourceContainer() const { return pcResource_; }
    void setResourceContainer(TResourceContainer* container) { pcResource_ = container; }

    /* 0x4 */ TResourceContainer* pcResource_;
};

/**
 * @ingroup jsystem-jmessage
 * 
 */
struct TProcessor {
    TProcessor(const TReference* pReference)
        : pReference_(pReference),
          pResourceCache_(NULL),
          pszCurrent_(NULL) 
        {
            JUT_ASSERT(584, pReference_!=NULL);
        }

    typedef bool (*pfnProcess_func)(TProcessor*);

    struct TStack_ {
        TStack_() { clear(); }

        bool empty() const { return upsz_ == 0; }
        bool IsPushable() const { return size() < max_size(); }

        void clear() { upsz_ = 0; }
        u32 max_size() const { return 4; }
        u32 size() const { return upsz_; }

        const char* top() const {
            JUT_ASSERT(181, upsz_>0);
            return stack[upsz_ - 1];
        }

        void push(const char* pszText) {
            JUT_ASSERT(186, IsPushable());
            stack[upsz_] = pszText;
            upsz_++;
        }

        void pop() {
            JUT_ASSERT(192, upsz_>0);
            upsz_--;

            #if PLATFORM_SHIELD
            stack[upsz_] = NULL;
            #endif
        }

        TStack_& operator=(const TStack_& other) {
            upsz_ = other.upsz_;
            char** start = (char**)other.stack;
            char** end = (char**)(other.stack + other.upsz_);
            std::copy(start, end, stack);
            return *this;
        }

        /* 0x0 */ u32 upsz_;  // stack size
        /* 0x4 */ const char* stack[4];
    };  // Size: 0x14

    struct TProcess_ {
        TProcess_() { reset_normal(); }

        void reset_normal() { pfnProcess_onCharacterEnd = process_onCharacterEnd_normal_; }
        void reset_select() { pfnProcess_onCharacterEnd = process_onCharacterEnd_select_; }

        /* 0x0 */ pfnProcess_func pfnProcess_onCharacterEnd;
        struct rdata {
            /* 0x0 */ const char* (*pfn)(TProcessor*);
            /* 0x4 */ const char* pcBase;
            /* 0x8 */ const void* pOffset;
            /* 0xC */ u32 uRest;
        }
        /* 0x4 */ rData;
    };  // Size: 0x14

    void reset();
    void stack_pushCurrent(char const* pszText);
    void stack_popCurrent();
    const TResource* getResource_groupID(u16 u16GroupID) const;
    u32 toMessageCode_messageID(u32 uMsgID, u32, bool* pbValid) const;
    void on_select_begin(char const* (*pfn)(JMessage::TProcessor*), void const* pOffset,
                                        char const* pcBase, u32 uNumber);
    void on_select_end();
    void on_select_separate();
    void on_tag_();
    bool process_character_();
    static bool process_onCharacterEnd_normal_(JMessage::TProcessor* pThis);
    static bool process_onCharacterEnd_select_(JMessage::TProcessor* pThis);
    static const char* process_onSelect_limited_(JMessage::TProcessor* pThis);
    static const char* process_onSelect_(JMessage::TProcessor* pThis);
    const char* on_message(u32 uCode) const { return getMessageText_messageCode(uCode); }
    const char* getMessageText_messageCode(u32 uCode) const {
        return getMessageText_messageCode(uCode >> 16, uCode & 0xFFFF);
    }

    virtual ~TProcessor();
    virtual void do_reset();
    virtual void do_begin(void const* pEntry, char const* pszText);
    virtual void do_end();
    virtual void do_character(int iCharacter);
    virtual bool do_tag(u32 uTag, void const* pData, u32 uSize);
    virtual void do_select_begin(u32 uNumber);
    virtual void do_select_end();
    virtual void do_select_separate();
    virtual void do_resetStatus_(char const* pszText);
    virtual bool do_setBegin_isReady_() const;
    virtual void do_begin_(void const* pEntry, char const* pszText) = 0;
    virtual void do_end_() = 0;
    virtual void do_tag_(u32 uTag, void const* pData, u32 uSize) = 0;

    void on_resetStatus_(const char* pszText) {
        pszCurrent_ = pszText;
        oStack_.clear();
        oProcess_.reset_normal();
        do_resetStatus_(pszText);
    }

    void on_begin(const void* pEntry, const char* pszText) {
        do_begin_(pEntry, pszText);
        do_begin(pEntry, pszText);
    }

    void on_end() {
        do_end_();
        do_end();
    }

    void on_tag(u32 uTag, const void* pData, u32 uSize) {
        if (!do_tag(uTag, pData, uSize)) {
            do_tag_(uTag, pData, uSize);
        }
    }

    const char* on_word(u32 param_0) const { return pReference_->on_word(param_0); }

    int on_parseCharacter(const char** ppszText) const {
        return pReference_->on_parseCharacter(ppszText);
    }
    void on_character(int iCharacter) { do_character(iCharacter); }

    const char* on_message_limited(u16 u16Index) const {
        JUT_ASSERT(482, pResourceCache_!=NULL);
        return pResourceCache_->getMessageText_messageIndex(u16Index);
    }

    bool on_setBegin_isReady_() const { return do_setBegin_isReady_(); }

    int setBegin_messageEntryText(const TResource* pResource, const void* pEntry, const char* pszText) {
        JUT_ASSERT(297, on_setBegin_isReady_());
        pResourceCache_ = pResource;
        on_resetStatus_(pszText);
        on_begin(pEntry, pszText);
        return 1;
    }

    int setBegin_messageEntryText(const TProcessor* pProcessor, const void* pEntry, const char* pszText) {
        JUT_ASSERT(306, pProcessor!=NULL);
        setBegin_messageEntryText(pProcessor->getResourceCache(), pEntry, pszText);
        return 1;
    }

    void* getMessageEntry_messageCode(u16 u16Code, u16 u16Index) const {
        const TResource* pResource = getResource_groupID(u16Code);

        if (pResource == NULL) {
            return NULL;
        }

        return pResource->getMessageEntry_messageIndex(u16Index);
    }

    const char* getMessageText_messageCode(u16 u16Code, u16 u16Index) const {
        void* pEntry = getMessageEntry_messageCode(u16Code, u16Index);

        if (pEntry == NULL) {
            return NULL;
        }

        return pResourceCache_->getMessageText_messageEntry(pEntry);
    }

    void stack_pushCurrent_(const char* pszText) {
        oStack_.push(getCurrent());
        pszCurrent_ = pszText;
    }

    void stack_popCurrent_() {
        pszCurrent_ = oStack_.top();
        oStack_.pop();
    }

    bool stack_isPushable() const { return oStack_.IsPushable(); }

    const TResource* getResourceCache() const { return pResourceCache_; }
    const char* getCurrent() const { return pszCurrent_; }
    const TReference* getReference() const { return pReference_; }

    const TResource* getResource_groupID_uncached(u16 u16GroupID) const {
        return pReference_->getResource_groupID(u16GroupID);
    }

    bool isResourceCache_groupID(u16 u16GroupID) const {
        return pResourceCache_ != NULL && u16GroupID == pResourceCache_->getGroupID();
    }

    TResourceContainer* getResourceContainer() const {
        return pReference_ == NULL ? NULL : pReference_->getResourceContainer();
    }

    void setResourceCache(TResource* pResource) { pResourceCache_ = pResource; }
    void resetResourceCache() { setResourceCache(NULL); }

    /* 0x04 */ const TReference* pReference_;
    /* 0x08 */ const mutable TResource* pResourceCache_;
    /* 0x0C */ const char* pszCurrent_;
    /* 0x10 */ TStack_ oStack_;
    /* 0x24 */ TProcess_ oProcess_;
};

struct TControl;

/**
 * @ingroup jsystem-jmessage
 * 
 */
struct TSequenceProcessor : public TProcessor {
    typedef const void* (*branchPfn)(const TSequenceProcessor*, u32);
    typedef const void* (*jumpPfn)(const TSequenceProcessor*);

    struct TProcess_ {
        TProcess_() { reset(); }

        void reset() {}

        union processData {
            struct branchData {
                /* 0x0 */ branchPfn branchFn;
                /* 0x4 */ const void* pTarget;
                /* 0x8 */ u32 uTarget;
            } branch_process;
            struct jumpData {
                /* 0x0 */ jumpPfn jumpFn;
                /* 0x4 */ u32 u32Target;
            } jump_process;
        }
        /* 0x0 */ process;
    };

    enum SeqStatus {
        STATUS_READY,
        STATUS_ONE,
        STATUS_NORMAL,
        STATUS_JUMP,
        STATUS_BRANCH,
    };

    TSequenceProcessor(JMessage::TReference const* pReference, JMessage::TControl* pControl);
    const char* process(char const* pszText);
    bool on_isReady();
    void on_jump_register(jumpPfn pfn, u32 u32Target);
    bool on_jump_isReady();
    void on_jump(void const* pEntry, char const* pszText);
    void on_branch_register(branchPfn pfn, void const* pTarget, u32 uTarget);
    void on_branch_query(u32 uCode);
    u32 on_branch_queryResult();
    void on_branch(void const* pEntry, char const* pszText);
    static void process_setMessageIndex_reserved_(u16 u16Index);
    static void* process_setMessageCode_(TSequenceProcessor const* pProcessor, u16 u16GroupID, u16 u16Index);
    static const void* process_onJump_limited_(TSequenceProcessor const* pProcessor);
    static const void* process_onJump_(TSequenceProcessor const* pProcessor);
    static const void* process_onBranch_limited_(TSequenceProcessor const* pProcessor, u32 uTargetIndex);
    static const void* process_onBranch_(TSequenceProcessor const* pProcessor, u32 uTargetIndex);

    virtual ~TSequenceProcessor();
    virtual void do_resetStatus_(char const* pszText);
    virtual bool do_setBegin_isReady_() const;
    virtual void do_begin_(void const* pEntry, char const* pszText);
    virtual void do_end_();
    virtual void do_tag_(u32 uTag, void const* pData, u32 uSize);
    virtual bool do_isReady();
    virtual bool do_jump_isReady();
    virtual void do_jump(void const* pEntry, char const* pszText);
    virtual void do_branch_query(u32 uCode);
    virtual s32 do_branch_queryResult();
    virtual void do_branch(void const* pEntry, char const* pszText);

    TControl* getControl() const { return pControl_; }

    static const void* process_setMessageIndex_(const TSequenceProcessor* pProcessor, u16 u16Index) {
        return process_setMessageCode_(pProcessor, pProcessor->getResourceCache()->getGroupID(),
                                       u16Index);
    }

    static const void* process_setMessageCode_(const TSequenceProcessor* pProcessor, u32 uCode) {
        return process_setMessageCode_(pProcessor, uCode >> 16, uCode & 0xFFFF);
    }

    /* 0x38 */ TControl* pControl_;
    /* 0x3C */ SeqStatus eStatus_;
    /* 0x40 */ TProcess_ oProcess2_;
};

/**
 * @ingroup jsystem-jmessage
 * 
 */
struct TRenderingProcessor : public TProcessor {
    TRenderingProcessor(JMessage::TReference const* pReference);
    int process(char const* pszText);

    virtual ~TRenderingProcessor();
    virtual void do_begin_(void const* pEntry, char const* pszText);
    virtual void do_end_();
    virtual void do_tag_(u32 uTag, void const* pData, u32 uSize);

    bool process_messageEntryText(const TProcessor* pProcessor, void const* pEntry, const char* pszText) {
        if (!TProcessor::setBegin_messageEntryText(pProcessor, pEntry, pszText)) {
            return false;
        }

        process(NULL);
        return true;
    }
};
};  // namespace JMessage

#endif /* JMESSAGE_PROCESSOR_H */
