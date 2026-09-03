#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define MESSAGE_SERVICE_ID "dev.twilitrealm.dusklight.message"
#define MESSAGE_SERVICE_MAJOR 1u
#define MESSAGE_SERVICE_MINOR 1u

typedef uint16_t MessageId;
typedef uint64_t MessageHandle;
typedef uint64_t MessageOverrideHandle;

/* INF1 entry data */
typedef struct MessageEntryData {
    uint8_t bytes[20];
} MessageEntryData;
static_assert(sizeof(MessageEntryData) == 20);

/* INF1 box kind (offset 0x09): message screen class */
typedef enum MessageBoxKind {
    MESSAGE_BOX_TALK = 0,         /* ordinary dialogue box */
    MESSAGE_BOX_DEMO_CAPTION = 1, /* boxless cutscene caption */
    MESSAGE_BOX_SIGN = 2,         /* signs and posted notices */
    MESSAGE_BOX_PLAIN = 5,        /* boxless system text */
    MESSAGE_BOX_KANBAN = 6,       /* signboard screen class; unused */
    MESSAGE_BOX_STAFF_ROLL = 7,
    MESSAGE_BOX_LIGHT_SPIRIT = 8, /* spirit text window and glow */
    MESSAGE_BOX_ITEM_GET = 9,     /* centered item-get box */
    MESSAGE_BOX_ITEM_NAME = 11,   /* UI string fetch, no box */
    MESSAGE_BOX_PLACE_NAME = 12,  /* area intro banner */
    MESSAGE_BOX_MIDNA = 13,       /* Midna dialogue colors and glow */
    MESSAGE_BOX_ANIMAL = 14,      /* wolf-form animal speech glow */
    MESSAGE_BOX_NOTICE = 15,      /* floating gameplay notice; can't be used during dialogue */
    MESSAGE_BOX_SAVE = 16,        /* save and memory card prompts */
    MESSAGE_BOX_HOWL = 17,        /* howling stone UI */
    MESSAGE_BOX_BOSS_NAME = 19,   /* boss intro banner */
} MessageBoxKind;

/* INF1 draw type (offset 0x0A): text pacing */
typedef enum MessageDrawType {
    MESSAGE_DRAW_TYPED = 0,         /* types per-character; A skips typing */
    MESSAGE_DRAW_INSTANT = 1,       /* whole page at once (menus, prompts) */
    MESSAGE_DRAW_TYPED_NO_SKIP = 2, /* types per-character; A does not skip */
    MESSAGE_DRAW_FADE = 3,          /* page fades in */
    MESSAGE_DRAW_UI_NAME = 4,       /* UI string fetch (item names), no box pacing */
    MESSAGE_DRAW_TYPED_SLOW = 5,    /* weighted slow typing (light spirit speech) */
    MESSAGE_DRAW_UI_ACTION = 7,     /* UI string fetch (action button labels) */
    MESSAGE_DRAW_FADE_SLOW = 9,     /* slow page fade (staff credits) */
} MessageDrawType;

/* INF1 box position (offset 0x0B) */
typedef enum MessageBoxPosition {
    MESSAGE_POSITION_BOTTOM = 0,
    MESSAGE_POSITION_TOP = 1,
    MESSAGE_POSITION_MIDDLE = 2,
    MESSAGE_POSITION_AUTO = 3, /* top or bottom, avoiding the speaker on screen */
} MessageBoxPosition;

/* Applies to both the text and gradient colors. */
typedef enum MessageTextColor {
    MESSAGE_COLOR_DEFAULT = 0, /* box default */
    MESSAGE_COLOR_RED = 1,     /* 0xF07878 */
    MESSAGE_COLOR_GREEN = 2,   /* 0xAADC8C */
    MESSAGE_COLOR_BLUE = 3,    /* 0xA0B4DC */
    MESSAGE_COLOR_YELLOW = 4,  /* 0xDCDC82 */
    MESSAGE_COLOR_SKY = 5,     /* 0xB4C8E6 */
    MESSAGE_COLOR_PURPLE = 6,  /* 0xC8A0DC */
    MESSAGE_COLOR_WHITE = 7,   /* 0xFFFFFF */
    MESSAGE_COLOR_ORANGE = 8,  /* 0xDCAA78 */
} MessageTextColor;

/* Message text language. 5 (Dutch) is unused. */
typedef enum MessageLanguage {
    MESSAGE_LANGUAGE_ENGLISH = 0,
    MESSAGE_LANGUAGE_GERMAN = 1,
    MESSAGE_LANGUAGE_FRENCH = 2,
    MESSAGE_LANGUAGE_SPANISH = 3,
    MESSAGE_LANGUAGE_ITALIAN = 4,
    MESSAGE_LANGUAGE_JAPANESE = 6,
} MessageLanguage;

typedef struct MessageVariantData {
    uint8_t language; /* MessageLanguage */
    MessageEntryData entry;
    const uint8_t* text;
    size_t text_size;
} MessageVariantData;

typedef struct MessageTextData {
    const uint8_t* text;
    size_t text_size;
} MessageTextData;

typedef struct MessageOverrideContext {
    uint16_t group;
    uint16_t message_id;
    uint8_t language; /* MessageLanguage */
    const uint8_t* original_text;
    size_t original_text_size;
} MessageOverrideContext;

/* Return true with a valid out_text to override, or false to try the next registration. */
typedef bool (*MessageOverrideFn)(ModContext* ctx, const MessageOverrideContext* message,
    MessageTextData* out_text, void* user_data);

typedef struct MessageService {
    ServiceHeader header;

    ModResult (*override_message)(ModContext* ctx, uint16_t group, uint16_t message_id,
        uint8_t language, const uint8_t* text, size_t text_size, MessageOverrideHandle* out_handle);
    ModResult (*override_message_fn)(ModContext* ctx, uint16_t group, uint16_t message_id,
        uint8_t language, MessageOverrideFn fn, void* user_data, MessageOverrideHandle* out_handle);
    ModResult (*remove_override)(ModContext* ctx, MessageOverrideHandle handle);

    ModResult (*register_message)(ModContext* ctx, uint16_t group,
        const MessageVariantData* variants, size_t variant_count, MessageId* out_id,
        MessageHandle* out_handle);
    ModResult (*remove_message)(ModContext* ctx, MessageHandle handle);
} MessageService;

MOD_DECLARE_SERVICE(
    MessageService, svc_message, MESSAGE_SERVICE_ID, MESSAGE_SERVICE_MAJOR, MESSAGE_SERVICE_MINOR);
