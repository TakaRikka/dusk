#ifndef D_A_BULLET_H
#define D_A_BULLET_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-unsorted
 * @class daBullet_c
 * @brief Bullet (Unused?)
 *
 * @details
 *
 */
class daBullet_c : public fopAc_ac_c {
public:
    typedef int (daBullet_c::*processFn)(void*);

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ dBgS_ObjAcch mAcch;
    /* 0x74C */ dCcD_Stts mCcStts;
    /* 0x788 */ dBgS_AcchCir mAcchCir;
    /* 0x7C8 */ dCcD_Sph mCcSph;
    /* 0x900 */ cBgS_GndChk mGndChk;
    /* 0x93C */ u8 field_0x93C[0x940 - 0x93C];
    /* 0x940 */ f32 mGroundY;
    /* 0x944 */ processFn mProcess;
    /* 0x950 */ int mLifetime;
    /* 0x954 */ u16 mMode;
    /* 0x956 */ u8 mType;
    /* 0x957 */ u8 field_0x957;

    virtual ~daBullet_c();
    int create();
    BOOL CreateHeap();
    int Delete();
    int Execute();
    int Draw();
    static int createHeapCallBack(fopAc_ac_c*);
    const char* getResName();
    const char* getBmdName();
    int getTypeFromParam();
    void initialize();
    int setProcess(processFn);
    int wait(void*);
    int move(void*);

    static dCcD_SrcGObjInf const mCcDObjInfo;
    static dCcD_SrcSph mCcDSph;
};

STATIC_ASSERT(sizeof(daBullet_c) == 0x95c);

struct daBullet_HIOParam {
    /* 0x00 */ f32 gravity;
    /* 0x04 */ f32 weight;
    /* 0x08 */ f32 height;
    /* 0x0C */ f32 knee_height;
    /* 0x10 */ f32 width;
    /* 0x14 */ s16 lifetime;
};

class daBullet_Param_c {
public:
    virtual ~daBullet_Param_c() {}

    static daBullet_HIOParam const m;
};


#endif /* D_A_BULLET_H */
