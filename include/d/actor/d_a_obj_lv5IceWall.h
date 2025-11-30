#ifndef D_A_OBJ_LV5ICEWALL_H
#define D_A_OBJ_LV5ICEWALL_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daIceWall_c
 * @brief Snowpeak Ruins Ice Wall
 *
 * @details
 *
 */
class daIceWall_c : public dBgS_MoveBgActor {
public:
    enum daIceWall_MODE {
        MODE_WAIT,
        MODE_BREAK,
    };

    void setBaseMtx();
    int create();
    void procMain();
    void init_modeWait();
    void modeWait();
    void init_modeBreak();
    void modeBreak();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    int getSwBit1() { return fopAcM_GetParamBit(this, 0, 8); }

    int getScaleX() { return fopAcM_GetParamBit(this, 0x10, 5); }
    int getScaleY() { return fopAcM_GetParamBit(this, 0x15, 5); }
    int getScaleZ() { return fopAcM_GetParamBit(this, 0x1A, 5); }

    static const dCcD_SrcGObjInf mCcDObjInfo;
    static dCcD_SrcCyl mCcDCyl;

private:
    /* 0x5A0 */ dCcD_Stts mCcStts;
    /* 0x5DC */ dCcD_Cyl mCcCyl[2];
    /* 0x854 */ request_of_phase_process_class mPhase;
    /* 0x85C */ J3DModel* mpModel[2];
    /* 0x864 */ u8 mMode;
    /* 0x865 */ u8 mIsBreaking;
    /* 0x866 */ u8 mHitcount;
    /* 0x867 */ u8 mIsBreakSwBit;
    /* 0x868 */ u8 mIsBreakingSwBit;
    /* 0x86C */ cXyz mScale;
    /* 0x878 */ u32 mEmitter0ID;
    /* 0x87C */ u32 mEmitter1ID;
    /* 0x880 */ u32 mEmitter2ID;
};

STATIC_ASSERT(sizeof(daIceWall_c) == 0x884);

class daIceWall_HIO_c : public fOpAcm_HIO_entry_c {
public:
    daIceWall_HIO_c();
    virtual ~daIceWall_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ cXyz xyz;
    /* 0x10 */ u8 hitcount;
    /* 0x11 */ u8 hitcount2;
};


#endif /* D_A_OBJ_LV5ICEWALL_H */
