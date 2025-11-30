#ifndef D_BG_D_BG_S_MOVEBG_ACTOR_H
#define D_BG_D_BG_S_MOVEBG_ACTOR_H

#include "f_op/f_op_actor.h"

class dBgW;
typedef void (*MoveBGActor_SetFunc)(dBgW*, void*, cBgS_PolyInfo const&, bool, cXyz*, csXyz*,
                                    csXyz*);

class dBgS_MoveBgActor : public fopAc_ac_c {
public:
    /* 0x568 */ dBgW* mpBgW;
    /* 0x56C */ Mtx mBgMtx;

    dBgS_MoveBgActor();
    int MoveBGCreateHeap();
    int MoveBGCreate(char const* i_arcName, int i_dzb_id,
                                    MoveBGActor_SetFunc i_setFunc, u32 i_heapSize, Mtx* i_bgMtx);
    int MoveBGDelete();
    int MoveBGExecute();

    int MoveBGDraw() { return Draw(); }
    int MoveBGIsDelete() { return IsDelete(); }

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();
    virtual int IsDelete();
    virtual int ToFore();
    virtual int ToBack();

    static const char* m_name;
    static int m_dzb_id;
    static MoveBGActor_SetFunc m_set_func;
};

STATIC_ASSERT(sizeof(dBgS_MoveBgActor) == 0x5a0);

#endif /* D_BG_D_BG_S_MOVEBG_ACTOR_H */
