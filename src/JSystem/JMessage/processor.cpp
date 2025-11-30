#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JMessage/processor.h"
#include "JSystem/JMessage/control.h"
#include "JSystem/JUtility/JUTAssert.h"
#include <cstdint.h>

JMessage::TReference::~TReference() {}

const char* JMessage::TReference::do_word(u32 param_0) const {
    return NULL;
}

JMessage::TProcessor::~TProcessor() {}

void JMessage::TProcessor::reset() {
    on_resetStatus_(NULL);
    do_reset();
}

void JMessage::TProcessor::stack_pushCurrent(char const* pszText) {
    if (pszText != NULL) {
        if (!oStack_.IsPushable()) {
            JGADGET_WARNMSG(113, "stack full");
            return;
        }

        stack_pushCurrent_(pszText);
    }
}

void JMessage::TProcessor::stack_popCurrent() {
    JGADGET_ASSERTWARN(122, !oStack_.empty());
    stack_popCurrent_();
}

const JMessage::TResource* JMessage::TProcessor::getResource_groupID(u16 u16GroupID) const {
    if (isResourceCache_groupID(u16GroupID)) {
        return pResourceCache_;
    }

    pResourceCache_ = getResource_groupID_uncached(u16GroupID);
    return pResourceCache_;
}

u32 JMessage::TProcessor::toMessageCode_messageID(u32 uMsgID, u32 param_1, bool* pbValid) const {
    const TResource* pResourceCache = (const TResource*)getResourceCache();
    if (pResourceCache != NULL) {
        u16 u16Index = pResourceCache->toMessageIndex_messageID(uMsgID, param_1, pbValid);
        if (u16Index != 0xFFFF) {
            return u16Index | (pResourceCache->getGroupID() << 16);
        }
    }

    TResourceContainer* pResourceContainer = getResourceContainer();
    if (pResourceContainer == NULL) {
        return 0xFFFFFFFF;
    }

    JGadget::TContainerEnumerator_const<JMessage::TResource, 0> enumerator(pResourceContainer->getResourceContainer());
    const TResource* pResource;
    while (enumerator) {
        pResource = (const TResource*)&(*enumerator);
        if (pResource != pResourceCache) {
            u16 u16Index = pResource->toMessageIndex_messageID(uMsgID, param_1, pbValid);
            if (u16Index != 0xFFFF) {
                ((JMessage::TProcessor*)this)->pResourceCache_ = pResource;
                return u16Index | (pResource->getGroupID() << 16);
            }
        }
    }

    return 0xFFFFFFFF;
}

void JMessage::TProcessor::on_select_begin(char const* (*pfn)(JMessage::TProcessor*),
                                           void const* pOffset, char const* pcBase, u32 uNumber) {
    JUT_ASSERT(191, uNumber>0);

    if (!stack_isPushable()) {
        JGADGET_WARNMSG(194, "stack full");
    } else {
        oProcess_.reset_select();

        oProcess_.rData.pfn = pfn;
        oProcess_.rData.pcBase = pcBase;
        oProcess_.rData.pOffset = pOffset;
        oProcess_.rData.uRest = uNumber;
        
        JUT_ASSERT(205, pfn!=NULL);
        const char* psz = pfn(this);
        JUT_ASSERT(207, psz!=NULL);
    
        stack_pushCurrent_(psz);
        do_select_begin(uNumber);
    }
}

void JMessage::TProcessor::on_select_end() {
    TProcess_::rdata& rData = oProcess_.rData;
    JUT_ASSERT(219, rData.uRest==0);
    oProcess_.reset_normal();

    JUT_ASSERT(223, stack_isPushable());
    stack_popCurrent_();
    do_select_end();
}

void JMessage::TProcessor::on_select_separate() {
    stack_popCurrent_();
    JUT_ASSERT(234, stack_isPushable());

    TProcess_::rdata& rData = oProcess_.rData;
    JUT_ASSERT(237, rData.uRest>0);

    const char* (*pfn)(TProcessor*) = rData.pfn;
    JUT_ASSERT(240, pfn!=NULL);

    const char* psz = pfn(this);
    JUT_ASSERT(242, psz!=NULL);

    stack_pushCurrent_(psz);
    do_select_separate();
}

void JMessage::TProcessor::do_reset() {}

void JMessage::TProcessor::do_begin(void const* pEntry, char const* pszText) {}

void JMessage::TProcessor::do_end() {}

void JMessage::TProcessor::do_character(int iCharacter) {}

bool JMessage::TProcessor::do_tag(u32 uTag, void const* pData, u32 uSize) {
    return false;
}

void JMessage::TProcessor::do_select_begin(u32 uNumber) {}

void JMessage::TProcessor::do_select_end() {}

void JMessage::TProcessor::do_select_separate() {}

void JMessage::TProcessor::on_tag_() {
    u32 uSize;
    char* psz = (char*)getCurrent();
    JUT_ASSERT(292, psz!=NULL);
    JUT_ASSERT(293, psz[-1]==data::gcTagBegin);

    uSize = *(u8*)psz;
    pszCurrent_ = uSize + psz - 1;

    psz++;

    u32 uTag = *(u8*)psz & 0xFF;
    psz++;

    uTag <<= 8;
    uTag |= *(u8*)psz & 0xFF;
    psz++;

    uTag <<= 8;
    uTag |= *(u8*)psz & 0xFF;
    psz++;

    on_tag(uTag, psz, uSize - 5);
}

void JMessage::TProcessor::do_resetStatus_(char const* pszText) {}

bool JMessage::TProcessor::do_setBegin_isReady_() const {
    return true;
}

void JMessage::TProcessor::do_tag_(u32 uTag, void const* pData, u32 uSize) {
    u8 uGroupID = data::getTagGroup(uTag);
    unsigned int uCode = data::getTagCode(uTag);

    switch (uGroupID) {
    case 0xFF:
        switch (uCode & 0xFFFF) {
        case 4:
            JUT_ASSERT(331, uSize==4);
            stack_pushCurrent(on_word(JGadget::binary::TParseValue<JGadget::binary::TParseValue_endian_big_<u32> >::parse(pData)));
            break;
        case 5:
            JUT_ASSERT(341, uSize==4);
            stack_pushCurrent(on_message(JGadget::binary::TParseValue<JGadget::binary::TParseValue_endian_big_<u32> >::parse(pData)));
            break;
        default:
            JGADGET_WARNMSG1(351, "unknown system-tag-code : ", uCode & 0xFFFF);
        }
        break;
    case 0xFE:
        JUT_ASSERT(356, uSize==0);
        stack_pushCurrent(on_word(uCode & 0xFFFF));
        break;
    case 0xFD:
        JUT_ASSERT(361, uSize==0);
        stack_pushCurrent(on_message_limited(uCode));
        break;
    case 0xF6: {
        u32 uNumber = uCode & 0xFFFF;
        JUT_ASSERT(368, uSize==4*uNumber);
        on_select_begin(process_onSelect_limited_, pData, (char*)pData + uSize, uCode);
        break;
    }
    case 0xF5:
        u32 uNumber = uCode & 0xFFFF;
        JUT_ASSERT(377, uSize==4*uNumber);
        on_select_begin(process_onSelect_, pData, (char*)pData + uSize, uCode);
        break;
    }

    JGADGET_WARNMSG4(387, "unknown tag-value : [", (unsigned int)uGroupID, "-", uCode & 0xFFFF, ']');
}

bool JMessage::TProcessor::process_character_() {
    JUT_ASSERT(397, pszCurrent_!=NULL);
    int iCharacter = on_parseCharacter(&pszCurrent_);

    switch (iCharacter) {
    case 0:
        if (!oProcess_.pfnProcess_onCharacterEnd(this)) {
            return false;
        }
        break;
    case data::gcTagBegin:
        on_tag_();
        break;
    default:
        on_character(iCharacter);
    }

    return true;
}

bool JMessage::TProcessor::process_onCharacterEnd_normal_(JMessage::TProcessor* pThis) {
    JUT_ASSERT(420, pThis->oProcess_.pfnProcess_onCharacterEnd== &process_onCharacterEnd_normal_);

    if (!pThis->oStack_.empty()) {
        pThis->stack_popCurrent();
        return true;
    }

    pThis->on_end();
    return false;
}

bool JMessage::TProcessor::process_onCharacterEnd_select_(JMessage::TProcessor* pThis) {
    JUT_ASSERT(435, pThis->oProcess_.pfnProcess_onCharacterEnd== &process_onCharacterEnd_select_);
    JUT_ASSERT(436, !pThis->oStack_.empty());

    TProcess_::rdata& rData = pThis->oProcess_.rData;
    rData.uRest--;

    u32 uRest = rData.uRest;
    if (uRest != 0) {
        pThis->on_select_separate();
        return true;
    }

    pThis->on_select_end();
    return true;
}

const char* JMessage::TProcessor::process_onSelect_limited_(JMessage::TProcessor* pThis) {
    TProcess_::rdata& rData = pThis->oProcess_.rData;
    JUT_ASSERT(454, rData.pfn==&process_onSelect_limited_);
    JUT_ASSERT(455, rData.pcBase!=NULL);
    JUT_ASSERT(456, rData.pOffset!=NULL);
    JUT_ASSERT(457, rData.uRest>0);

    u16* puOffset = (u16*)rData.pOffset;
    u16 uData = JGadget::binary::TParseValue<JGadget::binary::TParseValue_endian_big_<u16> >::parse(puOffset);

    rData.pOffset = (void*)(puOffset + 1);
    return &rData.pcBase[uData];
}

const char* JMessage::TProcessor::process_onSelect_(JMessage::TProcessor* pThis) {
    TProcess_::rdata& rData = pThis->oProcess_.rData;
    JUT_ASSERT(471, rData.pfn==&process_onSelect_);
    JUT_ASSERT(472, rData.pcBase!=NULL);
    JUT_ASSERT(473, rData.pOffset!=NULL);
    JUT_ASSERT(474, rData.uRest>0);

    u32* puOffset = (u32*)rData.pOffset;
    u32 uData = JGadget::binary::TParseValue<JGadget::binary::TParseValue_endian_big_<u32> >::parse(puOffset);

    rData.pOffset = (void*)(puOffset + 1);
    return &rData.pcBase[uData];
}

JMessage::TSequenceProcessor::TSequenceProcessor(JMessage::TReference const* pReference,
                                                 JMessage::TControl* pControl)
    : TProcessor(pReference),
      pControl_(pControl),
      eStatus_(STATUS_READY)
      {}

JMessage::TSequenceProcessor::~TSequenceProcessor() {}

const char* JMessage::TSequenceProcessor::process(char const* pszText) {
    do {
        switch (eStatus_) {
        case STATUS_NORMAL:
            break;
        case STATUS_JUMP: {
            JUT_ASSERT(549, pControl_!=NULL);

            if (!on_jump_isReady()) {
                return pszCurrent_;
            }

            eStatus_ = STATUS_NORMAL;

            TProcess_& process = oProcess2_;
            TSequenceProcessor::jumpPfn pfn = process.process.jump_process.jumpFn;
            JUT_ASSERT(556, pfn!=NULL);

            const void* pEntry = pfn(this);
            if (pEntry != NULL) {
                on_jump(pEntry, pControl_->getMessageText_begin());
            }
            break;
        }
        case STATUS_BRANCH: {
            JUT_ASSERT(567, pControl_!=NULL);

            u32 queryResult = on_branch_queryResult();
            if (queryResult > 0x7FFFFFFF) {
                switch (queryResult) {
                case -1:
                    return pszCurrent_;
                case -2:
                    eStatus_ = STATUS_NORMAL;
                    break;
                default:
                    JUT_ASSERT(579, false);
                }
            } else {
                eStatus_ = STATUS_NORMAL;

                TProcess_& process = oProcess2_;
                if (queryResult >= process.process.branch_process.uTarget) {
                    JGADGET_WARNMSG1(588, "unknown result : ", queryResult);
                } else {
                    TSequenceProcessor::branchPfn pfn = process.process.branch_process.branchFn;
                    JUT_ASSERT(592, pfn!=NULL);

                    const void* pEntry = pfn(this, queryResult);
                    if (pEntry != NULL) {
                        on_branch(pEntry, pControl_->getMessageText_begin());
                    }
                }
            }
            break;
        }
        default:
            JUT_ASSERT(602, false);
        }

        if (pszCurrent_ == pszText) {
            on_end();
            return NULL;
        }

        if (!on_isReady()) {
            return pszCurrent_;
        }
    } while (process_character_());

    return NULL;
}

bool JMessage::TSequenceProcessor::on_isReady() {
    JUT_ASSERT(629, eStatus_==STATUS_NORMAL);
    return do_isReady();
}

void JMessage::TSequenceProcessor::on_jump_register(JMessage::TSequenceProcessor::jumpPfn pfn, u32 u32Target) {
    JUT_ASSERT(638, eStatus_==STATUS_NORMAL);
    eStatus_ = STATUS_JUMP;

    TProcess_& process = oProcess2_;
    process.process.jump_process.jumpFn = pfn;
    process.process.jump_process.u32Target = u32Target;
}

bool JMessage::TSequenceProcessor::on_jump_isReady() {
    JUT_ASSERT(649, eStatus_==STATUS_JUMP);
    return do_jump_isReady();
}

void JMessage::TSequenceProcessor::on_jump(void const* pEntry, char const* pszText) {
    on_resetStatus_(pszText);

    JUT_ASSERT(658, eStatus_==STATUS_NORMAL);
    JUT_ASSERT(659, pEntry!=NULL);
    JUT_ASSERT(660, pszText!=NULL);
    do_jump(pEntry, pszText);
}

/* 802A8690-802A86A0 2A2FD0 0010+00 1/1 0/0 0/0 .text
 * on_branch_register__Q28JMessage18TSequenceProcessorFPFPCQ28JMessage18TSequenceProcessorUl_PCvPCvUl
 */
void JMessage::TSequenceProcessor::on_branch_register(JMessage::TSequenceProcessor::branchPfn pfn, void const* pTarget, u32 uTarget) {
    JUT_ASSERT(669, eStatus_==STATUS_NORMAL);

    TProcess_& process = oProcess2_;
    process.process.branch_process.branchFn = pfn;
    process.process.branch_process.pTarget = pTarget;
    process.process.branch_process.uTarget = uTarget;
}

void JMessage::TSequenceProcessor::on_branch_query(u32 uCode) {
    JUT_ASSERT(680, eStatus_==STATUS_NORMAL);
    eStatus_ = STATUS_BRANCH;
    do_branch_query(uCode);
}

u32 JMessage::TSequenceProcessor::on_branch_queryResult() {
    JUT_ASSERT(691, eStatus_==STATUS_BRANCH);
    u32 result = do_branch_queryResult();
    return result;
}

void JMessage::TSequenceProcessor::on_branch(void const* pEntry, char const* pszText) {
    on_resetStatus_(pszText);

    JUT_ASSERT(717, eStatus_==STATUS_NORMAL);
    JUT_ASSERT(718, pEntry!=NULL);
    JUT_ASSERT(719, pszText!=NULL);
    do_branch(pEntry, pszText);
}

bool JMessage::TSequenceProcessor::do_isReady() {
    return true;
}

bool JMessage::TSequenceProcessor::do_jump_isReady() {
    return true;
}

void JMessage::TSequenceProcessor::do_jump(void const* pEntry, char const* pszText) {}

void JMessage::TSequenceProcessor::do_branch_query(u32 uCode) {}

s32 JMessage::TSequenceProcessor::do_branch_queryResult() {
    return -2;
}

void JMessage::TSequenceProcessor::do_branch(void const* pEntry, char const* pszText) {}

void JMessage::TSequenceProcessor::do_resetStatus_(char const* pszText) {
    eStatus_ = STATUS_READY;

    if (pszText != NULL) {
        eStatus_ = STATUS_NORMAL;
    }
}

bool JMessage::TSequenceProcessor::do_setBegin_isReady_() const {
    return eStatus_ == STATUS_READY;
}

void JMessage::TSequenceProcessor::do_begin_(void const* pEntry, char const* pszText) {
    JUT_ASSERT(782, eStatus_==STATUS_NORMAL);
}

void JMessage::TSequenceProcessor::do_end_() {
    JUT_ASSERT(789, eStatus_==STATUS_NORMAL);
    eStatus_ = STATUS_ONE;
}

void JMessage::TSequenceProcessor::do_tag_(u32 uTag, void const* pData, u32 uSize) {
    u8 uGroupID = data::getTagGroup(uTag);
    unsigned int uCode = data::getTagCode(uTag);

    switch (uGroupID) {
    case 0xFF:
        switch (uCode & 0xFFFF) {
        case 0:
        case 1:
        case 2:
        case 3:
            break;
        case 6: {
            JUT_ASSERT(807, uSize==4);
            u32 u32Target = JGadget::binary::TParseValue<JGadget::binary::TParseValue_endian_big_<u32> >::parse(pData);
            on_jump_register(process_onJump_, u32Target);
            break;
        }
        case 7:
            break;
        default: {
            TProcessor::do_tag_(uTag, pData, uSize);
            break;
        }
        }
        break;
    case 0xFC: {
        JUT_ASSERT(828, uSize==0);
        on_jump_register(process_onJump_limited_, uCode & 0xFFFF);
        break;
    }
    case 0xFB: {
        u32 uNumber = uCode & 0xFFFF;
        JUT_ASSERT(835, uSize==2*uNumber);
        on_branch_register(process_onBranch_limited_, pData, uNumber);
        break;
    }
    case 0xFA: {
        u32 uNumber = uCode & 0xFFFF;
        JUT_ASSERT(843, uSize==4*uNumber);
        on_branch_register(process_onBranch_, pData, uNumber);
        break;
    }
    case 0xF9: {
        JUT_ASSERT(849, uSize==0);
        on_branch_query(uCode & 0xFFFF);
        break;
    }
    case 0xF8: {
        u16* pu16Data = (u16*)pData;
        u32 uNumber = JGadget::binary::TParseValue<JGadget::binary::TParseValue_endian_big_<u16> >::parse(pu16Data);
        JUT_ASSERT(859, uSize==2+2*uNumber);
        on_branch_register(process_onBranch_limited_, pu16Data + 1, uNumber);
        on_branch_query(uCode & 0xFFFF);
        break;
    }
    case 0xF7: {
        u16* pu16Data = (u16*)pData;
        u32 uNumber = JGadget::binary::TParseValue<JGadget::binary::TParseValue_endian_big_<u16> >::parse(pu16Data);
        JUT_ASSERT(871, uSize==2+4*uNumber);
        on_branch_register(process_onBranch_, pu16Data + 1, uNumber);
        on_branch_query(uCode & 0xFFFF);
        break;
    }
    default:
        TProcessor::do_tag_(uTag, pData, uSize);
    }
}

void JMessage::TSequenceProcessor::process_setMessageIndex_reserved_(u16 u16Index) {
    JUT_ASSERT(890, (data::MESSAGEINDEX_RESERVED_ENUM_MIN<=u16Index)&& (u16Index<=data::MESSAGEINDEX_RESERVED_ENUM_MAX));

    switch (u16Index) {
    case 0xFFFF:
        JGADGET_WARNMSG1(896, "message-index reserved : ", u16Index);
        return;
    }
}

void* JMessage::TSequenceProcessor::process_setMessageCode_(JMessage::TSequenceProcessor const* pProcessor, u16 u16GroupID, u16 u16Index) {
    JGADGET_ASSERTWARN(904, u16GroupID<=data::MESSAGEGROUPID_USER_ENUM_MIN);

    if (u16Index >= 0xFF00) {
        process_setMessageIndex_reserved_(u16Index);
        return NULL;
    }

    TControl* pControl = pProcessor->getControl();
    JUT_ASSERT(911, pControl!=NULL);

    if (!pControl->setMessageCode_inSequence_(pProcessor, u16GroupID, u16Index)) {
        JGADGET_WARNMSG1(914, "message-code undefined : ", pControl->getMessageCode());
        return NULL;
    }

    return pControl->getMessageEntry();
}

const void*
JMessage::TSequenceProcessor::process_onJump_limited_(JMessage::TSequenceProcessor const* pProcessor) {
    const TProcess_::processData::jumpData& rData = pProcessor->oProcess2_.process.jump_process;

    JUT_ASSERT(925, rData.u32Target<=std::uint16_t(-1));
    return process_setMessageIndex_(pProcessor, rData.u32Target);
}

const void*
JMessage::TSequenceProcessor::process_onJump_(JMessage::TSequenceProcessor const* pProcessor) {
    const TProcess_::processData::jumpData& rData = pProcessor->oProcess2_.process.jump_process;
    return process_setMessageCode_(pProcessor, rData.u32Target);
}

const void*
JMessage::TSequenceProcessor::process_onBranch_limited_(JMessage::TSequenceProcessor const* pProcessor, u32 uTargetIndex) {
    const TProcess_::processData::branchData& rData = pProcessor->oProcess2_.process.branch_process;

    JUT_ASSERT(942, rData.pTarget!=NULL);
    JUT_ASSERT(943, uTargetIndex<rData.uTarget);

    u16 u16Index = JGadget::binary::TParseValue<JGadget::binary::TParseValue_endian_big_<u16> >::parse(rData.pTarget, uTargetIndex);
    return process_setMessageIndex_(pProcessor, u16Index);
}

const void*
JMessage::TSequenceProcessor::process_onBranch_(JMessage::TSequenceProcessor const* pProcessor, u32 uTargetIndex) {
    const TProcess_::processData::branchData& rData = pProcessor->oProcess2_.process.branch_process;

    JUT_ASSERT(957, rData.pTarget!=NULL);
    JUT_ASSERT(958, uTargetIndex<rData.uTarget);

    u32 uCode = JGadget::binary::TParseValue<JGadget::binary::TParseValue_endian_big_<u32> >::parse(rData.pTarget, uTargetIndex);
    return process_setMessageCode_(pProcessor, uCode);
}

JMessage::TRenderingProcessor::TRenderingProcessor(JMessage::TReference const* pReference)
    : TProcessor(pReference) {}

JMessage::TRenderingProcessor::~TRenderingProcessor() {}

int JMessage::TRenderingProcessor::process(char const* pszText) {
    do {
        if (pszCurrent_ == pszText) {
            on_end();
            return 0;
        }
    } while (process_character_());

    return 0;
}

void JMessage::TRenderingProcessor::do_begin_(void const* pEntry, char const* pszText) {}

void JMessage::TRenderingProcessor::do_end_() {}

void JMessage::TRenderingProcessor::do_tag_(u32 uTag, void const* pData, u32 uSize) {
    u8 uGroupID = data::getTagGroup(uTag);
    unsigned int uCode = data::getTagCode(uTag);

    switch (uGroupID) {
    case 0xFF:
        switch (uCode & 0xFFFF) {
        case 0:
        case 1:
        case 2:
        case 3: {
            JGADGET_WARNMSG1(1047, "system-tag-code ignored : ", uCode & 0xFFFF);
            break;
        }
        case 6:
            break;
        case 7:
            break;
        default:
            TProcessor::do_tag_(uTag, pData, uSize);
            break;
        }
        break;
    case 0xFC:
    case 0xFB:
    case 0xFA:
    case 0xF9:
    case 0xF8:
    case 0xF7:
        break;
    default:
        TProcessor::do_tag_(uTag, pData, uSize);
        break;
    }
}
