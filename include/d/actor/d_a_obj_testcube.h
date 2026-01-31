#ifndef D_A_OBJ_TESTCUBE_H
#define D_A_OBJ_TESTCUBE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

class daObjCube_c : public dBgS_MoveBgActor {
public:
#if DEBUG
    daObjCube_c() {}

    void getDzbName(char*);
    void getBmdName(char*);
    void initBaseMtx();
    void setBaseMtx();
    int create();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    /* 0x5A8 */ request_of_phase_process_class mPhase;
    /* 0x5B0 */ J3DModel* mpModel;
    /* 0x5B4 */ u8 mShape;
    /* 0x5B5 */ u8 mNameArg;
    /* 0x5B6 */ u8 field_0x5b6;
#endif
};

namespace daObjCube_prm {
inline u8 getNameArg(daObjCube_c* i_this) {
    return fopAcM_GetParam(i_this) & 0xFF;
}

inline u8 getShape(daObjCube_c* i_this) {
    return (fopAcM_GetParam(i_this) >> 0x1C) & 7;
}

inline u8 getSwitch(daObjCube_c* i_this) {
    return (fopAcM_GetParam(i_this) >> 8) & 0xFF;
}

inline u8 getArg0(daObjCube_c* i_this) {
    return fopAcM_GetParam(i_this) >> 0x1F;
}
}

#endif /* D_A_OBJ_TESTCUBE_H */
