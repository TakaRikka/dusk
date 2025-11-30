#ifndef A_OBJ_D_A_OBJ_SS_BASE_H
#define A_OBJ_D_A_OBJ_SS_BASE_H

#include "f_op/f_op_actor.h"

class daObj_SSBase_c : public fopAc_ac_c {
public:
    daObj_SSBase_c();

    virtual ~daObj_SSBase_c();
    virtual void setSoldOut();
    virtual u32 getProcessID();

    void* getParentPtr() { return mpParentPtr; }
    void setParentPtr(void* parentPtr) { mpParentPtr = parentPtr; }
    u16 getFlowNodeNumber() { return mFlowNodeNum; }
    void setFlowNodeNumber(u16 flowNodeNum) { mFlowNodeNum = flowNodeNum; }
    u16 getValueNumber() { return mValueNum; }
    void setValueNumber(u16 valueNum) { mValueNum = valueNum; }

private:
    /* 0x56C */ void* mpParentPtr;
    /* 0x570 */ u16 field_0x570;
    /* 0x572 */ u16 mFlowNodeNum;
    /* 0x574 */ u16 mValueNum;
};

STATIC_ASSERT(sizeof(daObj_SSBase_c) == 0x578);

#endif /* A_OBJ_D_A_OBJ_SS_BASE_H */
