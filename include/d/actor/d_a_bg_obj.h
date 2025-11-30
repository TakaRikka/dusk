#ifndef D_A_BG_OBJ_H
#define D_A_BG_OBJ_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_bg_w.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

class daBgObj_c;
typedef int (daBgObj_c::*createHeapFunc)();
typedef int (daBgObj_c::*createInitFunc)();
typedef int (daBgObj_c::*executeFunc)();
typedef void (daBgObj_c::*tgSetFunc)();
typedef int (daBgObj_c::*actionFunc)();

/**
 * @ingroup actors-unsorted
 * @class daBgObj_c
 * @brief Moving Background Obj?
 *
 * @details
 *
 */
class daBgObj_c : public dBgS_MoveBgActor {
public:
    class spec_data_c {
    public:
        u8* initParticleBlock(u8*);
        u8* initSoundBlock(u8*);
        u8* initTexShareBlock(u8*);
        u8* initFarInfoBlock(u8*);
        bool Set(void*);

        /* 0x00 */ u16 mSpecType;
        /* 0x02 */ u8 field_0x02;
        /* 0x03 */ u8 field_0x03;
        /* 0x04 */ u32 mParticleNum;
        /* 0x08 */ u8* mpParticleBlock;
        /* 0x0C */ u32 mSoundNum;
        /* 0x10 */ u8* mpSoundBlock;
        /* 0x14 */ u8 field_0x14;
        /* 0x15 */ u8 field_0x15;
        /* 0x16 */ u8 field_0x16;
        /* 0x18 */ u8* mpTexShareBlock;
        /* 0x1C */ u8 mTexShareNum;
        /* 0x20 */ f32 mpFarInfoBlock;
    };

    void release(dBgW*);
    void regist(dBgW*);
    void initAtt();
    void setAttentionInfo(fopAc_ac_c*);
    void initBaseMtx();
    void setBaseMtx();
    void settingCullSizeBoxForCo(int);
    void settingCullSizeBoxForCull(int);
    int CreateInitType0();
    int CreateInitType1();
    int CreateHeapType0();
    int CreateHeapType1();
    void doShareTexture();
    int create1st();
    void setColCommon();
    void set_tri_0();
    void set_tri_1();
    void set_cyl_0();
    void set_tri_2();
    void set_tri_3();
    void setParticle();
    void setSe();
    BOOL checkDestroy();
    BOOL checkHitAt(cCcD_Obj*);
    void orderWait_tri();
    void orderWait_cyl();
    void orderWait_spec();
    int actionOrderWait();
    int actionOrder();
    int actionEvent();
    int actionWait();
    int ExecuteType0();
    int ExecuteType1();
    void indirectProc(J3DModel*);

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    void setAction(u8 i_action) { mAction = i_action; }

    static createHeapFunc mCreateHeapFunc[];
    static createInitFunc mCreateInitFunc[];
    static executeFunc mExecuteFunc[];
    static tgSetFunc mTgSetFunc[];

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* field_0x5a8[2][2];
    /* 0x5B8 */ mDoExt_btkAnm* field_0x5b8[2][2];
    /* 0x5C8 */ mDoExt_brkAnm* field_0x5c8[2][2];
    /* 0x5D8 */ dBgW* mpBgW2;
    /* 0x5DC */ dCcD_Stts mStts;
    /* 0x618 */ dCcD_Tri mTris[4];
    /* 0xB88 */ dCcD_Cyl mCyl;
    /* 0xCC4 */ u8 field_0xcc4;
    /* 0xCC5 */ u8 mAction;
    /* 0xCC6 */ s16 mEventIdx;
    /* 0xCC8 */ u8 field_0xcc8;
    /* 0xCC9 */ u8 field_0xcc9;
    /* 0xCCA */ u16 field_0xcca;
    /* 0xCCC */ u16 field_0xccc;
    /* 0xCD0 */ spec_data_c mSpecData;
    /* 0xCF4 */ csXyz mRotation;
    /* 0xCFC */ u32 mAttnActorID;
    /* 0xD00 */ u8 field_0xd00;
    /* 0xD01 */ u8 field_0xd01;
    /* 0xD02 */ u8 field_0xd02;
};

namespace daBgObj_prm {
inline u8 getEvId(daBgObj_c* i_this) {
    return (fopAcM_GetParam(i_this) >> 0x15) & 0xFF;
}

inline u8 getSwBit(daBgObj_c* i_this) {
    return i_this->field_0xccc & 0xFF;
}

inline u8 getSwBit2(daBgObj_c* i_this) {
    return (i_this->field_0xccc >> 8) & 0xFF;
}

inline u8 getObjArg0(daBgObj_c* i_this) {
    return i_this->field_0xcca & 0xFF;
}
};     // namespace daBgObj_prm

struct spec_dat {
public:
    u16 field_0x0;
    u16 field_0x2;
    u8 field_0x4;
};


#endif /* D_A_BG_OBJ_H */
