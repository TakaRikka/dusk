#ifndef JMESSAGE_RESOURCE_H
#define JMESSAGE_RESOURCE_H

#include "JSystem/JGadget/linklist.h"
#include "JSystem/JMessage/data.h"
#include "JSystem/JMessage/locale.h"

namespace JMessage {

/**
 * @ingroup jsystem-jmessage
 * 
 */
struct TResource {
    TResource()
        : oParse_THeader_(NULL),
          oParse_TBlock_info_(NULL),
          pMessageText_(NULL),
          pStringAttribute_(NULL),
          oParse_TBlock_messageID_(NULL)
          {}

    u16 toMessageIndex_messageID(u32 uMsgID, u32, bool* pbValid) const;

    bool isContained_messageIndex(u16 uMessageIndex) const {
        return uMessageIndex < getMessageEntryNumber();
    }

    u32 getMessageEntrySize() const { return oParse_TBlock_info_.get_messageEntrySize(); }
    u32 getMessageEntryNumber() const { return oParse_TBlock_info_.get_messageEntryNumber(); }

    u16 getGroupID() const { return oParse_TBlock_info_.get_groupID(); }

    void* getMessageEntry_messageIndex(u16 uMessageIndex) const {
        if (!isContained_messageIndex(uMessageIndex)) {
            return NULL;
        }

        void* pEntry = oParse_TBlock_info_.getContent() + (uMessageIndex * getMessageEntrySize());
        return pEntry;
    }

    char* getMessageText_messageEntry(const void* pEntry) const {
        JUT_ASSERT(141, pEntry!=NULL);
        return pMessageText_ + *(int*)pEntry;
    }

    const char* getMessageText_messageIndex(u16 uMessageIndex) const {
        void* pEntry = getMessageEntry_messageIndex(uMessageIndex);
        if (pEntry == NULL) {
            return NULL;
        }

        return getMessageText_messageEntry(pEntry);
    }

    void setData_header(const void* pData) {
        oParse_THeader_.setRaw(pData);
    }

    void setData_block_info(const void* pData) {
        oParse_TBlock_info_.setRaw(pData);

        JGADGET_ASSERTWARN(75, oParse_TBlock_info_.get_groupID()<= data::MESSAGEGROUPID_USER_ENUM_MAX);

        JGADGET_ASSERTWARN(77, oParse_TBlock_info_.get_messageEntryNumber()<= data::MESSAGEINDEX_USER_ENUM_MAX);
    }

    void setData_block_messageText(const void* pData) {
        pMessageText_ = data::TParse_TBlock_messageText(pData).getContent();
    }

    void setData_block_stringAttribute(const void* pData) {
        pStringAttribute_ = data::TParse_TBlock_stringAttribute(pData).getContent();
    }

    void setData_block_messageID(const void* pData) {
        oParse_TBlock_messageID_.setRaw(pData);
    }

    JGadget::TLinkListNode ocObject_;
    /* 0x08 */ data::TParse_THeader oParse_THeader_;
    /* 0x0C */ data::TParse_TBlock_info oParse_TBlock_info_;
    /* 0x10 */ char* pMessageText_;
    /* 0x14 */ char* pStringAttribute_;
    /* 0x18 */ data::TParse_TBlock_messageID oParse_TBlock_messageID_;
};

/**
 * @ingroup jsystem-jmessage
 * 
 */
struct TResource_color {
    TResource_color()
        : oParse_THeader_(NULL),
          oParse_TBlock_color_(NULL)
          {}

    void reset() { 
        oParse_THeader_.setRaw(NULL);
        oParse_TBlock_color_.setRaw(NULL);
    }

    /* 0x0 */ data::TParse_THeader oParse_THeader_;
    /* 0x4 */ data::TParse_TBlock_color oParse_TBlock_color_;
};  // Size: 0x8

/**
 * @ingroup jsystem-jmessage
 * 
 */
struct TResourceContainer {
    struct TCResource : public JGadget::TLinkList_factory<TResource, 0> {
        TCResource();
        TResource* Get_groupID(u16 u16GroupID);

        virtual ~TCResource();
        virtual JMessage::TResource* Do_create();
        virtual void Do_destroy(JMessage::TResource* pResource);
    };

    TResourceContainer();
    void setEncoding(u8 e);
    void setEncoding_(u8 e);

    int parseCharacter(const char** ppszText) const {
        JUT_ASSERT(330, pfnParseCharacter_!=NULL);
        return pfnParseCharacter_(ppszText);
    }

    TResource* getResource_groupID(u16 u16GroupID) { return resContainer_.Get_groupID(u16GroupID); }
    TResource* getResource_groupID(u16 u16GroupID) const { return getResource_groupID(u16GroupID); }

    bool isEncodingSettable(u8 e) const { return encodingType_ == e || encodingType_ == 0; }
    const TCResource* getResourceContainer() const { return &resContainer_; }
    void destroyResource() {
        resContainer_.Clear_destroy();
    }
    void destroyResource_color() {
        resColor_.reset();
    }
    void destroyResource_all() {
        destroyResource();
        destroyResource_color();
    }

    static JMessage::locale::parseCharacter_function sapfnParseCharacter_[5];

    /* 0x00 */ u8 encodingType_;
    /* 0x04 */ JMessage::locale::parseCharacter_function pfnParseCharacter_;
    /* 0x08 */ TCResource resContainer_;
    /* 0x18 */ TResource_color resColor_;
};

/**
 * @ingroup jsystem-jmessage
 * 
 */
struct TParse : public JGadget::binary::TParse_header_block {
    TParse(JMessage::TResourceContainer* pContainer);

    virtual ~TParse();
    virtual bool parseHeader_next(const void** ppData_inout, u32* puBlock_out, u32);
    virtual bool parseBlock_next(const void** ppData_inout, u32* puData_out, u32);

    /* 0x4 */ TResourceContainer* pContainer_;
    /* 0x8 */ TResource* pResource_;
};
};  // namespace JMessage

#endif /* JMESSAGE_RESOURCE_H */
