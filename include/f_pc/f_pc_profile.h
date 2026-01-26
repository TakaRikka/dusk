
#ifndef F_PC_PROFILE_H_
#define F_PC_PROFILE_H_

#include <dolphin/types.h>

typedef struct nodedraw_method_class nodedraw_method_class;
typedef struct leafdraw_method_class leafdraw_method_class;
typedef struct process_method_class process_method_class;

typedef struct process_profile_definition {
    /* 0x00 */ unsigned int layer_id;
    /* 0x04 */ u16 list_id;
    /* 0x06 */ u16 list_priority;
    /* 0x08 */ s16 name;
    /* 0x0C */ process_method_class* methods;
    /* 0x10 */ u32 process_size;
    /* 0x14 */ u32 unk_size;
    /* 0x18 */ u32 parameters;
} process_profile_definition;

#define LAYER_DEFAULT (-2)

struct leaf_process_profile_definition;
process_profile_definition* fpcPf_Get(s16 i_profname);
extern process_profile_definition** g_fpcPf_ProfileList_p;

#endif
