/**
 * f_pc_profile.cpp
 * Framework - Process Profile
 */

#include "f_pc/f_pc_profile.h"

#if TARGET_PC
#include "dusk/mods/svc/actor.hpp"
#include "f_pc/f_pc_name.h"
#endif

#ifndef __MWERKS__
// Forward declare the static list from f_pc_profile_lst.cpp
DUSK_GAME_EXTERN process_profile_definition DUSK_CONST* DUSK_CONST g_fpcPfLst_ProfileList[];
// On PC: Direct pointer to static array
DUSK_GAME_DATA process_profile_definition DUSK_CONST* DUSK_CONST* DUSK_CONST g_fpcPf_ProfileList_p =
    g_fpcPfLst_ProfileList;
#else
// On Console: Pointer initialized by REL module prolog
process_profile_definition** g_fpcPf_ProfileList_p;
#endif

process_profile_definition DUSK_CONST* fpcPf_Get(s16 i_profname) {
#if TARGET_PC
    // Check if a mod has registered an actor with i_profname. Fallback to the profile list if it
    // doesn't exist.
    process_profile_definition* profile =
        dusk::mods::svc::actor_impl::get_profile_from_proc_name(i_profname);
    if (profile != nullptr) {
        return profile;
    }
    if (i_profname < 0 || i_profname >= fpcNm_MAX_NUM) {
        return nullptr;
    }
#endif

    int index = i_profname;
    return g_fpcPf_ProfileList_p[index];
}
