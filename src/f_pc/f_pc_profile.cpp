/**
 * f_pc_profile.cpp
 * Framework - Process Profile
 */

#include "f_pc/f_pc_profile.h"


#ifndef __MWERKS__
// Forward declare the static list from f_pc_profile_lst.cpp
extern process_profile_definition DUSK_CONST* DUSK_CONST g_fpcPfLst_ProfileList[];
// On PC: Direct pointer to static array
process_profile_definition DUSK_CONST* DUSK_CONST* DUSK_CONST g_fpcPf_ProfileList_p = g_fpcPfLst_ProfileList;
#else
// On Console: Pointer initialized by REL module prolog
process_profile_definition** g_fpcPf_ProfileList_p;
#endif

process_profile_definition DUSK_CONST* fpcPf_Get(s16 i_profname) {
    int index = i_profname;
    return g_fpcPf_ProfileList_p[index];
}
