#pragma once

#include "helpers/bits.hpp"
#include "mods/svc/flow.h"
#include "mods/svc/message.h"

#include <cstdint>

namespace JMessage {
struct TControl;
struct TProcessor;
struct TResource;
}  // namespace JMessage

namespace dusk::flow {

inline constexpr uint16_t kCustomNodeMin = 0x8000;
inline constexpr uint16_t kCustomEdgeMin = 0x8000;
inline constexpr uint16_t kCustomMessageMin = 0x8000;
inline constexpr uint16_t kCustomMessageMax = 0xfeff;
inline constexpr uint16_t kCustomQueryMin = 0x8000;
inline constexpr uint8_t kCustomEventMin = 0x80;
inline constexpr uint16_t kEnd = 0xffff;

/* Associates a parsed BMG resource with its service group and validates its reserved ranges. */
bool bind_resource(const void* bmgData, uint16_t group);

/* All resolver outputs are copies so callbacks may mutate service registrations safely. */
bool resolve_node(const void* bmgData, uint16_t nodeIndex, FlowNodeData& outNode);
bool resolve_edge(const void* bmgData, uint16_t edgeIndex, uint16_t& outTarget);
bool resolve_message_entry(
    const void* bmgData, uint16_t messageIndex, MessageEntryData& outEntry);

uint16_t dispatch_query(uint16_t queryId, const void* speakerActor, uint16_t parameter,
    uint8_t resultCount, FlowQueryPhase phase, uint16_t nodeIndex);
void dispatch_event(uint8_t eventId, const void* speakerActor, const uint8_t parameters[4]);

bool custom_message_group(uint16_t messageId, uint16_t& outGroup);
bool custom_message_for_control(
    JMessage::TControl* control, uint16_t messageId, const void*& outEntry, const char*& outText);

/* JMessage bridges. Return true when the service supplied a custom or overridden value. */
bool message_code_for_id(
    const JMessage::TProcessor* processor, uint32_t messageId, uint32_t& outCode);
bool custom_message_for_processor(JMessage::TControl* control,
    const JMessage::TProcessor* processor, uint16_t messageIndex,
    const JMessage::TResource*& outResource, const void*& outEntry, const char*& outText);
bool resolve_message_for_control(JMessage::TControl* control, const void* bmgData,
    uint16_t messageIndex, const void* nativeEntry, const char* nativeText, const void*& outEntry,
    const char*& outText);
bool resolve_message(const JMessage::TProcessor* processor, const void* bmgData,
    uint16_t messageIndex, const void* nativeEntry, const char* nativeText, const void*& outEntry,
    const char*& outText);
void release_message_control(const JMessage::TControl* control);
void release_message_processor(const JMessage::TProcessor* processor);

namespace detail {
inline bool find_section(
    const uint8_t* bmg, uint32_t tag, const uint8_t*& outSection, size_t& outSize) {
    const uint32_t sectionCount = read_bits<uint32_t>(bmg + 0x0c);
    size_t offset = 0x20;
    for (uint32_t i = 0; i < sectionCount; ++i) {
        if (offset > std::numeric_limits<size_t>::max() - 8) {
            return false;
        }
        const size_t sectionSize = read_bits<uint32_t>(bmg + offset + 4);
        if (sectionSize < 8 || offset > std::numeric_limits<size_t>::max() - sectionSize) {
            return false;
        }
        if (read_bits<uint32_t>(bmg + offset) == tag) {
            outSection = bmg + offset;
            outSize = sectionSize;
            return true;
        }
        offset += sectionSize;
    }
    return false;
}
}  // namespace detail

}  // namespace dusk::flow
