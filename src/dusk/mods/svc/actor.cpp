#include "mods/svc/actor.h"
#include "dusk/mods/svc/actor.hpp"

#include "config.hpp"
#include "registry.hpp"
#include "slot_map.hpp"

#include "aurora/lib/logging.hpp"
#include "dusk/mod_loader.hpp"

#include <fmt/format.h>

#include "d/d_stage.h"
#include "f_op/f_op_actor_tag.h"
#include "f_pc/f_pc_deletor.h"

namespace dusk::mods::svc::actor_impl {
namespace {

aurora::Module Log("dusk::mods::actor");

SlotMap<std::unique_ptr<ActorSlot>> s_slots;
std::unordered_map<s16, ActorHandle> procNameToHandle;
std::unordered_map<std::string, ActorHandle> fullNameToHandle;

ModResult register_actor(ModContext* ctx, const ActorProfileDesc* desc, ProfileName* outProfileName,
    ActorHandle* outActorHandle) {
    const auto handle = s_slots.emplace(*ctx->mod,
        std::make_unique<ActorSlot>(
            ActorSlot{{desc->create_function, desc->delete_function, desc->execute_function,
                          desc->is_delete_function, desc->draw_function},
                {
                    {},  // Set after registered
                    0,   // Set after registered to a slot
                    -1   // Use the default argument
                },
                {
                    /* Layer ID     */ fpcLy_CURRENT_e,
                    /* List ID      */ desc->priority_group,
                    /* List Prio    */ fpcPi_CURRENT_e,  // Always fpcPi_CURRENT_e
                    /* Proc Name    */ 0,
                    /* Proc SubMtd  */ &g_fpcLf_Method.base,  // usually: &g_fpcLf_Method.base
                    /* Size         */ (u32)desc->process_size,
                    /* Size Other   */ 0,                     // Always 0
                    /* Parameters   */ 0,                     // Always 0
                    /* Leaf SubMtd  */ &g_fopAc_Method.base,  // usually &g_fopAc_Method.base
                    /* Draw Prio    */ desc->draw_priority,
                    /* Actor SubMtd */ nullptr,  // set this later
                    /* Status       */ desc->status,
                    /* Group        */ desc->group,
                    /* Cull Type    */ desc->cull_type,
                }}));

    s32 procNameFull = fpcNm_MAX_NUM + s_slots.index_of(handle);
    if (procNameFull >= 0x7FFF) {
        s_slots.erase(handle);
        Log.error("Hit registered actor limit (0x7FFF) while registering actor {}", desc->name);
        return MOD_ERROR;
    }

    ActorSlot& slot = *s_slots.find(handle)->value.get();
    strncpy(slot.objNameInf.name, desc->name, sizeof(slot.objNameInf.name) - 1);
    slot.profile.sub_method = &slot.methodTable;
    s16 procName = (s16)procNameFull;
    procNameToHandle[procName] = handle;
    fullNameToHandle[std::string(slot.objNameInf.name)] = handle;
    slot.objNameInf.procname = (s16)procName;
    slot.profile.base.base.name = procName;
    *outProfileName = procName;
    *outActorHandle = handle;

    return MOD_OK;
}

// Must be called before the slot associated with the handle is erased
static void remove_handle_from_maps(ActorHandle handle) {
    auto entry = s_slots.find(handle);
    if (entry == nullptr) {
        return;
    }

    std::string fullName = entry->value->objNameInf.name;

    const auto& it = fullNameToHandle.find(fullName);
    if (it == fullNameToHandle.end()) {
        return;
    }
    fullNameToHandle.erase(it);

    const auto& it2 = procNameToHandle.find(entry->value->profile.base.base.name);
    if (it2 == procNameToHandle.end()) {
        return;
    }
    procNameToHandle.erase(it2);
}

ModResult unregister_actor(ModContext* ctx, ActorHandle handle) {
    auto slot = s_slots.find(handle);
    if (slot == nullptr) {
        return MOD_ERROR;
    }

    // Search through the current actor list and request a delete to all actors of the type that we
    // are unregistering. This may be unsafe under some circumstances, but this is the cleanest way
    // to implement unregistering actors at runtime without the game crashing.

    node_list_class* actorList = &g_fopAcTg_Queue;
    bool isDelete = false;
    if (actorList->mSize > 0) {
        node_class* node = actorList->mpHead;
        node_class* pNext = NODE_GET_NEXT(node);

        while (node) {
            fopAc_ac_c* actor = (fopAc_ac_c*)((create_tag_class*)node)->mpTagData;
            if (actor->name == slot->value->profile.base.base.name) {
                isDelete = true;
                fopAcM_delete(actor);  // Request a delete
            }
            node = pNext;
            pNext = NODE_GET_NEXT(pNext);
        }
    }

    if (isDelete) {
        node_list_class* delete_actor_list = &g_fpcDtTg_Queue;
        if (delete_actor_list->mSize > 0) {
            node_class* node = delete_actor_list->mpHead;
            node_class* pNext = NODE_GET_NEXT(node);

            while (node) {
                ((delete_tag_class*)node)->timer =
                    0;  // Set the timer to 0, force a delete when fpcDt_Handler is called

                node = pNext;
                pNext = NODE_GET_NEXT(pNext);
            }
        }
        fpcDt_Handler();  // Delete all actors currently in the delete queue
    }

    remove_handle_from_maps(handle);
    s_slots.erase(handle);

    return MOD_OK;
}

ModResult create_actor(
    ModContext* ctx, ProfileName name, const ActorSpawnParams* params, ActorId* outId) {
    cXyz pos = {params->position.x, params->position.y, params->position.z};
    csXyz angle = {params->angle.x, params->angle.y, params->angle.z};
    cXyz scale = {params->scale.x, params->scale.y, params->scale.z};
    fpc_ProcID id = fopAcM_create(name, 0xFFFF, params->parameters, &pos, params->room_num, &angle,
        &scale, params->argument, params->create_function);

    if (id == fpcM_ERROR_PROCESS_ID_e) {
        Log.error("Error Creating Actor with profile name {}", name);
        return MOD_ERROR;
    }

    if (outId) {
        *outId = id;
    }
    return MOD_OK;
}

ModResult create_actor_from_name(
    ModContext* ctx, const char* name, const ActorSpawnParams* params, ActorId* outId) {
    dStage_objectNameInf* objectName = dStage_searchName(name);
    if (objectName == nullptr) {
        Log.error("Attempted to create actor ({}) but it can't be found!", name);
        return MOD_ERROR;
    }

    ActorSpawnParams copy = *params;
    copy.argument = objectName->argument;

    return create_actor(ctx, objectName->procname, &copy, outId);
}

ModResult create_child_actor(ModContext* ctx, ProfileName name, ActorId parentID,
    const ActorSpawnParams* params, ActorId* outId) {
    cXyz pos = {params->position.x, params->position.y, params->position.z};
    csXyz angle = {params->angle.x, params->angle.y, params->angle.z};
    cXyz scale = {params->scale.x, params->scale.y, params->scale.z};
    fpc_ProcID id = fopAcM_createChild(name, parentID, params->parameters, &pos, params->room_num,
        &angle, &scale, params->argument, params->create_function);

    if (id == fpcM_ERROR_PROCESS_ID_e) {
        Log.error("Error Creating Actor with profile name {} as child of actor with id {}", name,
            parentID);
        return MOD_ERROR;
    }

    if (outId) {
        *outId = id;
    }
    return MOD_OK;
}

ModResult create_child_actor_from_name(ModContext* ctx, const char* name, ActorId parentID,
    const ActorSpawnParams* params, ActorId* outId) {
    dStage_objectNameInf* objectName = dStage_searchName(name);
    if (objectName == nullptr) {
        Log.error("Attempted to create actor ({}) but it can't be found!", name);
        return MOD_ERROR;
    }

    ActorSpawnParams copy = *params;
    copy.argument = objectName->argument;

    return create_child_actor(ctx, objectName->procname, parentID, &copy, outId);
}

ModResult get_actor_id(ModContext* ctx, ProfileName name, ActorId* outId) {
    fopAc_ac_c* actor = fopAcM_SearchByName(name);
    if (actor == nullptr) {
        Log.error("Attempting to get actor by profile name ({}) but it doesn't exist!", name);
        return MOD_ERROR;
    }

    *outId = fopAcM_GetID(actor);
    return MOD_OK;
}

ModResult get_actor_room_num(ModContext* ctx, ActorId actorId, int8_t* outRoomNum) {
    fopAc_ac_c* actor = fopAcM_SearchByID(actorId);
    if (actor == nullptr) {
        Log.error("Attempted to get room number of actor Id ({}) but it doesn't exist!", actorId);
        return MOD_ERROR;
    }

    *outRoomNum = fopAcM_GetRoomNo(actor);
    return MOD_OK;
}

ModResult delete_actor(ModContext* ctx, ActorId actorId) {
    fopAc_ac_c* actor = fopAcM_SearchByID(actorId);
    if (actor == nullptr) {
        Log.warn("Attempted to delete actor with ID ({}) but it doesn't exist!", actorId);
        return MOD_OK;
    }
    fopAcM_delete(actor);
    return MOD_OK;
}

}  // namespace

process_profile_definition* get_profile_from_proc_name(s16 name) {
    const auto& it = procNameToHandle.find(name);
    if (it == procNameToHandle.end()) {
        return nullptr;
    }
    auto entry = s_slots.find(it->second);
    if (entry) {
        return &entry->value->profile.base.base;
    }
    return nullptr;
}

const char* get_full_name_from_proc_name(s16 name) {
    const auto& it = procNameToHandle.find(name);
    if (it == procNameToHandle.end()) {
        return "";
    }
    auto entry = s_slots.find(it->second);
    if (entry) {
        return entry->value->objNameInf.name;
    }
    return "";
}

dStage_objectNameInf* get_stageinfo_from_full_name(const std::string& name) {
    const auto& it = fullNameToHandle.find(name);
    if (it == fullNameToHandle.end()) {
        return nullptr;
    }
    auto entry = s_slots.find(it->second);
    if (entry) {
        return &entry->value->objNameInf;
    }
    return nullptr;
}

void actor_remove_mod(LoadedMod& mod) {
    s_slots.for_each([&](const ActorHandle handle, const auto& slot) {
        if (slot.owner == &mod) {
            unregister_actor(mod.context.get(), handle);
        }
    });

    // Erase to make sure all slots are cleared
    s_slots.erase_all(mod);
}

}  // namespace dusk::mods::svc::actor_impl

namespace dusk::mods::svc {
namespace {

constexpr ActorService s_actorService{
    .header = SERVICE_HEADER(ActorService, ACTOR_SERVICE_MAJOR, ACTOR_SERVICE_MINOR),
    .register_actor = actor_impl::register_actor,
    .unregister_actor = actor_impl::unregister_actor,
    .create_actor_from_name = actor_impl::create_actor_from_name,
    .create_actor = actor_impl::create_actor,
    .create_child_actor_from_name = actor_impl::create_child_actor_from_name,
    .create_child_actor = actor_impl::create_child_actor,
    .get_actor_id = actor_impl::get_actor_id,
    .get_actor_room_num = actor_impl::get_actor_room_num,
    .delete_actor = actor_impl::delete_actor,
};

}

constinit const ServiceModule g_actorModule{
    .id = ACTOR_SERVICE_ID,
    .majorVersion = ACTOR_SERVICE_MAJOR,
    .minorVersion = ACTOR_SERVICE_MINOR,
    .service = &s_actorService,
    .modDeactivating = actor_impl::actor_remove_mod,
};

}  // namespace dusk::mods::svc
