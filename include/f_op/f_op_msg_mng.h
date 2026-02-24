#ifndef F_F_OP_MSG_MNG_H_
#define F_F_OP_MSG_MNG_H_

#include "SSystem/SComponent/c_xyz.h"
#include "f_op/f_op_msg.h"
#include "f_pc/f_pc_leaf.h"

class JKRExpHeap;
class JKRHeap;
class fopAc_ac_c;
struct msg_class;

struct msg_process_profile_definition {
    /* 0x00 */ leaf_process_profile_definition base;
    /* 0x24 */ leafdraw_method_class* sub_method;  // Subclass methods
};

struct fopMsg_prm_class {
    /* 0x00 */ fopAc_ac_c* talk_actor;
    /* 0x04 */ cXyz pos;
    /* 0x10 */ u32 msg_idx;
    /* 0x14 */ u32 field_0x14;
    /* 0x18 */ fpc_ProcID field_0x18;
};  // Size: 0x1C

struct fopMsg_prm_timer : public fopMsg_prm_class {
    /* 0x1C */ int timer_mode;
    /* 0x20 */ u32 limit_ms;
    /* 0x24 */ u8 type;
    /* 0x25 */ u8 field_0x25;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
};  // Size: 0x38

typedef int (*fopMsgCreateFunc)(void*);

JKRExpHeap* fopMsgM_createExpHeap(u32 i_heapSize, JKRHeap* i_heap);
fpc_ProcID fopMsgM_Create(s16 i_procName, fopMsgCreateFunc i_createFunc, void* i_append);
fpc_ProcID fopMsgM_create(s16 i_procName, fopAc_ac_c* i_talkActor, cXyz* i_pos, u32* i_msgIdx,
                          u32* param_4, fopMsgCreateFunc createFunc);
void fopMsgM_Delete(void* i_this);
fopMsg_prm_class* fopMsgM_GetAppend(void* i_msg);
void fopMsgM_setMessageID(fpc_ProcID msg_id);
void fopMsgM_destroyExpHeap(JKRExpHeap* i_heap);
f32 fopMsgM_valueIncrease(int param_0, int param_1, u8 i_type);
s32 fopMsgM_setStageLayer(void* i_process);
fpc_ProcID fopMsgM_messageSet(u32 i_msgIdx, fopAc_ac_c* i_talkActor, u32 param_2);
fpc_ProcID fopMsgM_messageSet(u32 i_msgIdx, u32 param_1);
fpc_ProcID fopMsgM_messageSetDemo(u32 i_msgidx);
msg_class* fopMsgM_SearchByID(fpc_ProcID i_id);
char* fopMsgM_messageGet(char* i_stringBuf, u32 i_msgId);
fpc_ProcID fop_Timer_create(s16 i_procName, u8 i_mode, u32 i_limitMs, u8 i_type, u8 param_4,
                            f32 param_5, f32 param_6, f32 param_7, f32 param_8,
                            fopMsgCreateFunc i_createFunc);

inline fpc_ProcID fopMsgM_Timer_create(s16 i_procName, u8 i_mode, u32 i_limitMs, u8 i_type,
                                       u8 param_4, f32 param_5, f32 param_6, f32 param_7,
                                       f32 param_8, fopMsgCreateFunc i_createFunc) {
    return fop_Timer_create(i_procName, i_mode, i_limitMs, i_type, param_4, param_5, param_6,
                            param_7, param_8, i_createFunc);
}
u8 fopMsgM_itemNumIdx(u8 i_no);

#endif
