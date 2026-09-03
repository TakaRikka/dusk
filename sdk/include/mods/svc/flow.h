#pragma once

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#define FLOW_SERVICE_ID "dev.twilitrealm.dusklight.flow"
#define FLOW_SERVICE_MAJOR 1u
#define FLOW_SERVICE_MINOR 0u

typedef uint64_t FlowGraphHandle;
typedef uint16_t FlowQueryId;
typedef uint8_t FlowEventId;

/* FLW1 node data */
typedef struct FlowNodeData {
    uint8_t bytes[8];
} FlowNodeData;
static_assert(sizeof(FlowNodeData) == 8);

/* Built-in branch queries. The result selects the edge slot next_node_index + result;
 * [param] is the branch node's 16-bit parameter. */
typedef enum FlowGameQuery {
    FLOW_QUERY_SELECT_2 = 0,            /* two-way selection result: 0 first, 1 second */
    FLOW_QUERY_EVENT_FLAG = 1,          /* 0 if event flag [param] is set */
    FLOW_QUERY_PLAYER_FORM = 2,         /* 0 human, 1 wolf, 2 riding */
    FLOW_QUERY_RANDOM = 3,              /* random result in [0, param) */
    FLOW_QUERY_SELECT_3 = 4,            /* three-way selection result: 0/1/2 */
    FLOW_QUERY_TALK_DISTANCE = 5,       /* player within talk range; param overrides max distance */
    FLOW_QUERY_RUPEES = 6,              /* 0 if rupees >= param; param 0 checks wallet max */
    FLOW_QUERY_SWORD_TUTORIAL_STEP = 7, /* 0 if the scarecrow tutorial step matches param */
    FLOW_QUERY_SWORD_TUTORIAL_RESULT = 8, /* 0 on tutorial success */
    FLOW_QUERY_SWORD_TUTORIAL_COUNT = 9,  /* 0 if first success */
    FLOW_QUERY_TEMP_FLAG = 10,            /* 0 if temporary event flag [param] is set */
    FLOW_QUERY_CHEST_FLAG = 11,           /* 0 if treasure chest flag [param] is set */
    FLOW_QUERY_SAVE_SWITCH = 12,          /* 0 if save switch [param] is set */
    FLOW_QUERY_SAVE_ITEM_FLAG = 13,
    FLOW_QUERY_DUNGEON_SWITCH = 14,
    FLOW_QUERY_DUNGEON_ITEM_FLAG = 15,
    FLOW_QUERY_ZONE_SWITCH = 16,
    FLOW_QUERY_ZONE_ITEM_FLAG = 17,
    FLOW_QUERY_ONE_ZONE_SWITCH = 18,
    FLOW_QUERY_ONE_ZONE_ITEM_FLAG = 19,
    FLOW_QUERY_EQUIPPED = 20,       /* 1 if item [param] is equipped or on an item slot */
    FLOW_QUERY_ITEM_OWNED = 21,     /* 0 if item [param] is owned */
    FLOW_QUERY_BOMB_BAG_COUNT = 22, /* number of bomb bags owned: 0-3 */
    FLOW_QUERY_ARROWS = 23,         /* 0 if arrows >= param */
    FLOW_QUERY_EMPTY_BOTTLES = 24,  /* 0 if empty bottles >= param */
    FLOW_QUERY_SHOP_CLERK = 25,     /* shop system conversation flag */
    FLOW_QUERY_TEARS_OF_LIGHT = 26, /* 0 if tears >= param; param 0 uses the required count */
    /* 0 if goat-herding time <= param seconds; publishes the time for display */
    FLOW_QUERY_HERDING_TIME = 27,
    FLOW_QUERY_LANTERN_OIL = 28,       /* 0 full, 1 partial, 2 empty */
    FLOW_QUERY_REGISTER = 29,          /* flow scratch register value */
    FLOW_QUERY_GOATS_CAUGHT = 30,      /* 0 if caught runaway goats >= param */
    FLOW_QUERY_HEARTS = 31,            /* 0 if life >= param */
    FLOW_QUERY_HOLDING_LANTERN = 32,   /* 0 if the player has the lantern out */
    FLOW_QUERY_TIME_OF_DAY = 33,       /* current hour of game time (0-23) */
    FLOW_QUERY_MAGIC = 34,             /* 0 if magic >= param */
    FLOW_QUERY_SELECT_2_CANCEL = 35,   /* 0/1 choice, 2 on B cancel */
    FLOW_QUERY_SELECT_3_CANCEL = 36,   /* 0/1/2 choice, 3 on B cancel */
    FLOW_QUERY_BOMB_BAG_CONTENTS = 37, /* 0 empty, 1 bombs, 2 water bombs, 3 bomblings */
    FLOW_QUERY_BOMBS_FIT = 38,         /* 1 if param more bombs fit in the bag, 0 if over max */
    FLOW_QUERY_BOMB_BAG_FILL = 39,     /* 0 empty, 1 partial, 2 full */
    FLOW_QUERY_WATER_BOMBS_FIT = 40,
    /* 0 clear, 1 NPC near, 2 NPC far, 3 environment, 4 Sacred Grove */
    FLOW_QUERY_TRANSFORM_BLOCKED = 41,
    FLOW_QUERY_BOMBLINGS_FIT = 42,
    FLOW_QUERY_WARP_ALLOWED = 43,    /* 0 if a dungeon warp is accepted here */
    FLOW_QUERY_GOLDEN_BUGS = 44,     /* 0 none, 1 1-11, 2 12-22, 3 23, 4 all 24 */
    FLOW_QUERY_UNDELIVERED_BUG = 45, /* 1 if carrying a golden bug not yet delivered to Agitha */
    FLOW_QUERY_UNUSED_46 = 46,       /* asserts; do not use */
    FLOW_QUERY_NEW_LETTERS = 47,     /* 0 none, 1 one (& stores its name for the tag), 2 more */
    FLOW_QUERY_POE_SOULS = 48,       /* 0 none, 1 <20, 2 <40, 3 <60, 4 60+ */
    FLOW_QUERY_DONATION_TOTAL = 49,  /* 0 if donations >= param */
    FLOW_QUERY_BALLOON_SCORE = 50,   /* 0 zero, 1 <1000, 2 <10000, 3 <61454, 4 max */
    FLOW_QUERY_IN_WATER = 51,        /* 1 if the player is swimming */
    FLOW_QUERY_IRON_BOOTS = 52,      /* 1 if iron boots are equipped */
    FLOW_QUERY_BUILTIN_COUNT,
} FlowGameQuery;

/* Built-in event actions. Params (big-endian): one u32, two u16 (p0, p1), or four u8. */
typedef enum FlowGameEvent {
    FLOW_EVENT_SET_EVENT_FLAG = 0, /* sets event flags [p0] and [p1]; 0 = none */
    FLOW_EVENT_CLEAR_EVENT_FLAG = 1,
    FLOW_EVENT_ADD_RUPEES = 2,
    FLOW_EVENT_REMOVE_RUPEES = 3,
    FLOW_EVENT_ADD_HEARTS = 4,
    FLOW_EVENT_REMOVE_HEARTS = 5,
    FLOW_EVENT_ADD_MAGIC = 6,
    FLOW_EVENT_REMOVE_MAGIC = 7,
    FLOW_EVENT_START_EVENT = 8, /* publish (p0 event id, p1 item id) for the speaker to poll */
    FLOW_EVENT_JUMP_FLOW = 9,   /* continue at flow [p]; 0 jumps to the stage/Midna flow */
    FLOW_EVENT_SET_TEMP_FLAG = 10,
    FLOW_EVENT_CLEAR_TEMP_FLAG = 11,
    FLOW_EVENT_OPEN_DOOR = 12,       /* marks the flow as a door unlock path (probe only) */
    FLOW_EVENT_SELECT_VERTICAL = 13, /* vertical selection; p = result index chosen on B cancel */
    FLOW_EVENT_SET_SWITCH = 14,      /* p0 scope: 0 save, 1 dungeon, 2 zone, 3 one-zone; p1 bit */
    FLOW_EVENT_CLEAR_SWITCH = 15,
    FLOW_EVENT_SHOP_SELECT = 16, /* start shop item selection; four u8 shop params */
    FLOW_EVENT_GIVE_ITEM = 17,   /* p0 item number, p1 count */
    /* four u8 direction values for the speaker; p3 plays a sound */
    FLOW_EVENT_STAGE_DIRECTION = 18,
    FLOW_EVENT_SET_SPEAKER = 19,  /* point the box at talk partner [p1] */
    FLOW_EVENT_WARP_PLAYER = 20,  /* move the player to the room spawn tagged [p] */
    FLOW_EVENT_WAIT = 21,         /* close the box and wait [p] frames */
    FLOW_EVENT_FILL_LANTERN = 22, /* refill oil to [p] percent; 0 = full */
    /* p: 1-3 red/green/blue potion, 4 milk, 5 half milk, 6 oil, 7 hot spring water */
    FLOW_EVENT_FILL_BOTTLE = 23,
    FLOW_EVENT_SHOP_SOLD_OUT = 24,
    FLOW_EVENT_SET_REGISTER = 25,  /* set the flow scratch register (see FLOW_QUERY_REGISTER) */
    FLOW_EVENT_TENT_PURCHASE = 26, /* unattended stand purchase; marks the item sold out */
    FLOW_EVENT_FILL_BOMBS = 27,    /* u8 p0 bag select, u8 p1 operation; u16 p1 count */
    FLOW_EVENT_SELL_BOMBS = 28,    /* empty the selected bag and pay out */
    /* horizontal selection; p = result index chosen on B cancel */
    FLOW_EVENT_SELECT_HORIZONTAL = 29,
    FLOW_EVENT_FILL_ARROWS = 30, /* p1 count, 0 = max; p0 nonzero defers the refill */
    FLOW_EVENT_RETURN_RENTAL_BOMB_BAG = 31,
    FLOW_EVENT_FADE_IN = 32, /* p0: 0 black, 1 white; p1 frames */
    FLOW_EVENT_FADE_OUT = 33,
    FLOW_EVENT_SET_TRADE_ITEM = 34, /* set the trade-quest item */
    FLOW_EVENT_REMOVE_ITEM = 35,
    FLOW_EVENT_SET_SAVE_SWITCH = 36, /* set save switch (p0 area, p1 bit) */
    FLOW_EVENT_CLEAR_SAVE_SWITCH = 37,
    FLOW_EVENT_RECEIVE_LETTER = 38,
    FLOW_EVENT_UNLOCK_MAP_REGION = 39,
    FLOW_EVENT_EMPTY_BOTTLE = 40, /* p as in FLOW_EVENT_FILL_BOTTLE */
    FLOW_EVENT_ADD_DONATION = 41,
    FLOW_EVENT_UNUSED_42 = 42, /* no-op */
    FLOW_EVENT_BUILTIN_COUNT,
} FlowGameEvent;

typedef enum FlowQueryPhase {
    FLOW_QUERY_PHASE_PROBE = 0,
    FLOW_QUERY_PHASE_EXECUTE = 1,
} FlowQueryPhase;

typedef struct FlowQueryContext {
    const void* speaker_actor;
    uint16_t parameter;
    uint8_t result_count;
    uint8_t phase; /* FlowQueryPhase */
} FlowQueryContext;

typedef uint16_t (*FlowQueryFn)(ModContext* ctx, const FlowQueryContext* query, void* user_data);

typedef struct FlowEventContext {
    const void* speaker_actor;
    uint8_t parameters[4];
} FlowEventContext;

typedef void (*FlowEventFn)(ModContext* ctx, const FlowEventContext* event, void* user_data);

typedef struct FlowService {
    ServiceHeader header;

    ModResult (*begin_graph)(ModContext* ctx, uint16_t group, FlowGraphHandle* out_handle);
    /* Allocates one node ID. Fill it with fill_node before commit_graph. */
    ModResult (*allocate_node)(ModContext* ctx, FlowGraphHandle handle, uint16_t* out_id);
    /* Adds a series of edges with the given targets. */
    ModResult (*add_edges)(ModContext* ctx, FlowGraphHandle handle, const uint16_t* targets,
        uint16_t count, uint16_t* out_first);
    /* May be called again to replace a node until commit_graph. */
    ModResult (*fill_node)(
        ModContext* ctx, FlowGraphHandle handle, uint16_t node_index, const FlowNodeData* node);
    /* Replace a native node or edge when the graph commits; reverted when it is removed. */
    ModResult (*patch_node)(
        ModContext* ctx, FlowGraphHandle handle, uint16_t node_index, const FlowNodeData* node);
    ModResult (*patch_edge)(
        ModContext* ctx, FlowGraphHandle handle, uint16_t edge_index, uint16_t target_node);
    ModResult (*commit_graph)(ModContext* ctx, FlowGraphHandle handle);
    ModResult (*remove_graph)(ModContext* ctx, FlowGraphHandle handle);

    ModResult (*register_query)(ModContext* ctx, const char* debug_name, FlowQueryFn fn,
        void* user_data, FlowQueryId* out_id);
    ModResult (*register_event)(ModContext* ctx, const char* debug_name, FlowEventFn fn,
        void* user_data, FlowEventId* out_id);
} FlowService;

MOD_DECLARE_SERVICE(FlowService, svc_flow, FLOW_SERVICE_ID, FLOW_SERVICE_MAJOR, FLOW_SERVICE_MINOR);
