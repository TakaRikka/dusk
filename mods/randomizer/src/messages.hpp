#pragma once

#include <dolphin/types.h>

// Forward declaration
namespace JMessage {
struct TProcessor;
struct TControl;
}

void HandleTextOverrides(JMessage::TControl* control, JMessage::TProcessor const* pProcessor, int groupID, int index);

bool HandleCustomText(JMessage::TControl* control, u16 msgId);

char* GetTextOverride(s16 groupID, u32 messageId);