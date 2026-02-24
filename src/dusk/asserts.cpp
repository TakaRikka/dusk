#include <cstdint>
#include <dolphin/types.h>

static_assert(sizeof(u8) == sizeof(uint8_t));
static_assert(sizeof(s8) == sizeof(int8_t));
static_assert(sizeof(u16) == sizeof(uint16_t));
static_assert(sizeof(s16) == sizeof(int16_t));
static_assert(sizeof(u32) == sizeof(uint32_t));
static_assert(sizeof(s32) == sizeof(int32_t));
static_assert(sizeof(u64) == sizeof(uint64_t));
static_assert(sizeof(s64) == sizeof(int64_t));
