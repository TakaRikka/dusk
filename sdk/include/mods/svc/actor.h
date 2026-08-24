#pragma once

#include <mods/api.h>
#include <mods/svc/config.h>

#define ACTOR_SERVICE_ID "dev.twilitrealm.dusklight.actor"
#define ACTOR_SERVICE_MAJOR 1u
#define ACTOR_SERVICE_MINOR 0u

typedef int16_t ProfileName;
typedef uint32_t ActorId;
typedef uint64_t ActorHandle;

typedef struct {
    const char name[8];  // Canonical stage name. Must be unique among active mod actors. Matching a
                         // game actor name intentionally overrides stage lookup for that actor.

    uint16_t priority_group; /* priorityGroup is the priority for when execute will be called on the
    actor. Here are the main groups:
    0: The room manager actor
    1: Game scenes
    2: Various room change actors
    3: Most objects to be executed before Link
    4: Some bosses, canoe, epona, spinner, chests
    5: Link's actor
    6: Boomerang, midna
    7: Most objects, actors, bosses, triggers to be executed after link (most actors go here)
    8: Various objects and enemies
    9: Various actors
    10: Timer, Scene Exit actor
    11: Grass, Suspend Actors
    */

    size_t process_size;    // Size of the actor class (use sizeof(my_actor_class))
    int16_t draw_priority;  // an enum value that is prefixed with fpcDwPi. Select an existing value
                            // from the fpcDwPi to pick a draw priority matching the actor you wish
                            // to match priorities with.
    uint32_t status;    // Flags from fopAc_Status_e enum (all have fopAcStts_UNK_0x40000_e, a lot
                        // have fopAcStts_UNK_0x4000_e, add fopAcStts_CULL_e to enable culling)
    uint8_t group;      // The actor type. An enum value from fopAc_Group_e (fopAc_ACTOR_e,
                        // fopAc_ENEMY_e, fopAc_NPC_e)
    uint8_t cull_type;  // Enum value from fopAc_Cull_e
    int (*create_function)(
        void*);  // Called after the actor is spawned, return type is a enum value of cPhs_Step
    int (*delete_function)(void*);     // Releases resources; returns 1 when deletion is complete
    int (*execute_function)(void*);    // Called once per game tick, the actor's priorityGroup
                                       // determines when it will run relative to other actors
    int (*is_delete_function)(void*);  // Returns 1 when normal deletion may begin
    int (*draw_function)(void*);       // Called to draw the actor
} ActorProfileDesc;

typedef struct {
    uint32_t parameters;  // The parameters to be passed to the actor
    int8_t argument;  // The argument to be passed to the actor (acts as an extra byte for a parameter)
    int8_t room_num;  // The room to spawn the actor in
    struct {
        float x;
        float y;
        float z;
    } position;
    struct {
        int16_t x;
        int16_t y;
        int16_t z;
    } angle;
    struct {
        float x;
        float y;
        float z;
    } scale;
    int (*create_function)(void*);  // Optional: A custom function to run when the actor is created.
} ActorSpawnParams;

typedef struct ActorService {
    ServiceHeader header;
    ModResult (*register_actor)(ModContext* ctx, const ActorProfileDesc* desc,
        ProfileName* outProfileName, ActorHandle* outActorHandle);
    ModResult (*unregister_actor)(ModContext* ctx, ActorHandle handle);
    ModResult (*create_actor_from_name)(
        ModContext* ctx, const char* name, const ActorSpawnParams* params, ActorId* outId);
    ModResult (*create_actor)(
        ModContext* ctx, ProfileName name, const ActorSpawnParams* params, ActorId* outId);
    ModResult (*create_child_actor_from_name)(ModContext* ctx, const char* name, ActorId parentID,
        const ActorSpawnParams* params, ActorId* outId);
    ModResult (*create_child_actor)(ModContext* ctx, ProfileName name, ActorId parentID,
        const ActorSpawnParams* params, ActorId* outId);
    ModResult (*get_actor_id)(ModContext* ctx, ProfileName name, ActorId* outId);
    ModResult (*get_actor_room_num)(ModContext* ctx, ActorId actorId, int8_t* outRoomNum);
    /* Returns MOD_UNAVAILABLE if the actor cannot be queued for deletion immediately. */
    ModResult (*delete_actor)(ModContext* ctx, ActorId actorId);
} ActorService;

#ifdef __cplusplus
#include "mods/service.hpp"

template <>
struct mods::ServiceTraits<ActorService> {
    static constexpr const char* id = ACTOR_SERVICE_ID;
    static constexpr uint16_t major_version = ACTOR_SERVICE_MAJOR;
    static constexpr uint16_t minor_version = ACTOR_SERVICE_MINOR;
};
#endif
