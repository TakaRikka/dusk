#ifndef C_C_DYLINK_H
#define C_C_DYLINK_H

#include "f_op/f_op_scene_mng.h"

struct cDylPhs {
    static int phase_01(void*);
    static int phase_02(s16*);
    static int phase_03(void*);
    static int Link(request_of_phase_process_class*, s16);
    static int Unlink(request_of_phase_process_class*, s16);
};

struct DynamicNameTableEntry {
    s16 mKey;
    const char* name;
};

int cDyl_InitAsyncIsDone();
void cDyl_InitAsync();

BOOL cDyl_IsLinked(s16 i_ProfName);
BOOL cDyl_Unlink(s16 i_ProfName);
int cDyl_LinkASync(s16 i_ProfName);

BOOL cCc_Check();

#endif /* C_C_DYLINK_H */
