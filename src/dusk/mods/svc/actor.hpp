#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "f_op/f_op_actor_mng.h"

namespace dusk::mods::svc::actor_impl {

struct ActorSlot {
    actor_method_class methodTable;
    int (*deleteFunction)(void*);
    int (*isDeleteFunction)(void*);
    bool forceDelete;
    dStage_objectNameInf objNameInf;
    actor_process_profile_definition profile;
};

process_profile_definition* get_profile_from_proc_name(s16 name);

dStage_objectNameInf* get_stageinfo_from_full_name(const std::string& name);

const char* get_full_name_from_proc_name(s16 name);  // Returns "" when not found

};  // namespace dusk::mods::svc::actor_impl
