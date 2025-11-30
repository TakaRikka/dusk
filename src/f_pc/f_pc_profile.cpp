/**
 * f_pc_profile.cpp
 * Framework - Process Profile
 */

#include "f_pc/f_pc_profile.h"

process_profile_definition** g_fpcPf_ProfileList_p;

process_profile_definition* fpcPf_Get(s16 i_profname) {
    int index = i_profname;
    return g_fpcPf_ProfileList_p[index];
}
