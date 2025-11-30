/**
 * f_pc_load.cpp
 * Framework - Process Loader
 */

#include "f_pc/f_pc_load.h"
#include "c/c_dylink.h"
#include "SSystem/SComponent/c_phase.h"

BOOL fpcLd_Use(s16 i_procName) {
    if (fpcLd_IsLoaded(i_procName) == TRUE && fpcLd_Load(i_procName) == cPhs_COMPLEATE_e)
        return TRUE;
    return FALSE;
}

BOOL fpcLd_IsLoaded(s16 i_procName) {
    return cDyl_IsLinked((s16)i_procName);
}

void fpcLd_Free(s16 i_procName) {
    cDyl_Unlink((s16)i_procName);
}

s32 fpcLd_Load(s16 i_procName) {
    switch (cDyl_LinkASync((s16)i_procName)) {
    case cPhs_COMPLEATE_e:
        return cPhs_COMPLEATE_e;
    case cPhs_INIT_e:
        return cPhs_INIT_e;
    default:
        return cPhs_ERROR_e;
    }
}
