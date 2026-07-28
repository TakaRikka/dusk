#include "messages.hpp"

#include "JSystem/JMessage/control.h"
#include "d/d_msg_class.h"
#include "d/d_com_inf_game.h"
#include "randomizer_context.hpp"
#include "custom_flow_ids.hpp"
#include "utilities.h"

#include <fmt/format.h>

static JMSMesgEntry_c defaultJMSMesgEntry{
    .string_offset = 0,
    .message_id = 0,
    .event_label_id = 0,
    .se_speaker = 0x24,
    .fuki_kind = 0x00,
    .output_type = 0x00,
    .fuki_pos_type = 0x00,
    .unk_0xc = 0xFF,
    .unk_0xd = 0x00,
    .se_mood = 0x00,
    .camera_id = 0x00,
    .base_anm_id = 0x02,
    .face_anm_id = 0x03,
    .unk_0x12 = 0x0400,
};

// Format certain messages that need to have dynamic info in them
char* GetFormatedTextOverride(u32 key, std::string& text) {
    // Store formatted message in static buffer so it never goes away.
    // This is fine as long as we only ever need to format messages
    // for textboxes, but will cause issues if we need to use it for 
    // other UI elements
    static std::array<char, 256> buf;
    u32 value{};
    char* outIt;
    // For item counts, execItemGet hasn't run yet, so add one to the count
    switch (key) {
    case (0 << 16) | 325: // Group 0, id 325
        // Poe Soul get item text
        value = dComIfGs_getPohSpiritNum() + 1;
        outIt = fmt::vformat_to(buf.data(), text, fmt::make_format_args(value));
        break;
    case (0 << 16) | 335: // Group 0, id 335
        // Sky book characters get item text
        value = getAncientDocumentNum() + 1;
        outIt = fmt::vformat_to(buf.data(), text, fmt::make_format_args(value));
        break;
    default:
        // No override, return original text
        return text.data();
    }

    // Null-terminate
    size_t len = std::distance(buf.data(), outIt);
    buf[len] = '\0';

    // Return overriden text
    return buf.data();
}

u8 getLanguageForOverride() {
    u8 language = randomizer::Text::ENGLISH;

    // TODO: add service or something to check game language
    /*if (dusk::version::isRegionPal()) {
        language = dComIfGs_getPalLanguage();
    }*//* else if (dusk::version::isRegionJpn()) {
        language = randomizer::Text::JAPANESE;
    }*/

    return language;
}

void HandleTextOverrides(JMessage::TControl* control, JMessage::TProcessor const* pProcessor, int groupID, int index) {
    if (randomizer_IsActive()) {
        // Get the entry for this message
        auto entry = static_cast<JMSMesgEntry_c*>(pProcessor->getMessageEntry_messageCode(groupID, index));
        if (!entry) {
            return;
        }

        // If the message id is >= 5000 then it's part of the stage file's message group
        // Otherwise it's part of group 0
        auto msgId = entry->message_id.host();
        u16 group = 0;
        if (msgId >= 5000) {
            group = dComIfGp_getStageStagInfo()->mMsgGroup;
        }

        u32 key = (group << 16) | msgId;
        auto& textOverrides = randomizer_GetContext().mTextOverrides;
        u8 language = getLanguageForOverride();
        if (textOverrides.at(language).contains(key)) {
            control->pMessageText_begin_ = GetFormatedTextOverride(key, textOverrides[language][key]);
        }
    }
}

bool HandleCustomText(JMessage::TControl* control, u16 msgId) {
    if (randomizer_IsActive()) {
        u32 key = (CUSTOM_BMG_GROUP << 16) | msgId;
        auto& textOverrides = randomizer_GetContext().mTextOverrides;
        u8 language = getLanguageForOverride();
        if (textOverrides.at(language).contains(key)) {
            control->pMessageText_begin_ = GetFormatedTextOverride(key, textOverrides[language][key]);

            // Get the attributes for this text-box if they were specified
            auto& attributeOverrides = randomizer_GetContext().mAttributeOverrides;
            if (attributeOverrides.contains(key)) {
                control->pEntry_ = reinterpret_cast<void*>(&attributeOverrides[key]);
            // Otherwise, use the default entry
            } else {
                defaultJMSMesgEntry.message_id = msgId;
                control->pEntry_ = &defaultJMSMesgEntry;
            }
            return true;
        }
    }
    return false;
}

// Used in special cases
char* GetTextOverride(s16 groupID, u32 messageId) {
    u32 key = (groupID << 16) | messageId;
    auto& textOverrides = randomizer_GetContext().mTextOverrides;
    u8 language = getLanguageForOverride();
    if (textOverrides.at(language).contains(key)) {
        return GetFormatedTextOverride(key, textOverrides[language][key]);
    }
    return NULL;
}
