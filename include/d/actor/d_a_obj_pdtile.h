#ifndef D_A_OBJ_PDTILE_H
#define D_A_OBJ_PDTILE_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjPDtile_c
 * @brief P - Drop Tile
 *
 * @details
 *
 */
class daObjPDtile_c : public dBgS_MoveBgActor, request_of_phase_process_class {
public:
    enum Type {
        TYPE_0,
        TYPE_1,
        TYPE_2,
        TYPE_3,
        TYPE_4,
    };

    int create1st();
    void setMtx();
    void rideActor(fopAc_ac_c*);
    int CreateHeap();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();
    virtual ~daObjPDtile_c() {}

    u8 getType() { return fopAcM_GetParamBit(this, 0, 4); }

private:
    /* 0x5A8 */ Mtx mBgMtx;
    /* 0x5D8 */ Mtx field_0x5d8;
    /* 0x608 */ J3DModel* mModel;
    /* 0x60C */ Type mType;
    /* 0x610 */ int field_0x610;
    /* 0x614 */ u8 field_0x614;
    /* 0x618 */ int field_0x618;
    /* 0x61C */ f32 field_0x61c;
    /* 0x620 */ cXyz field_0x620;
    /* 0x62C */ f32 field_0x62c;
    /* 0x630 */ u8 field_0x630;
    /* 0x634 */ dCcD_Stts mStts;
    /* 0x670 */ dCcD_Cyl mCyl;
    /* 0x7AC */ int field_0x7ac;
    /* 0x7B0 */ s16 field_0x7b0;
    /* 0x7B2 */ s16 field_0x7b2;
    /* 0x7B4 */ u8 field_0x7b4;
    /* 0x7B5 */ u8 field_0x7b5;
    /* 0x7B8 */ int field_0x7b8;
    /* 0x7BC */ char* mArcName;
    /* 0x7C0 */ u32 mDzbId;
    /* 0x7C4 */ u32 mBmdId;
    /* 0x7C8 */ Vec* mCullBox;
    /* 0x7CC */ f32 field_0x7cc;
    /* 0x7D0 */ u8 field_0x7d0;
    /* 0x7D4 */ int field_0x7d4;
    /* 0x7D8 */ int field_0x7d8;
    /* 0x7DC */ Z2SoundObjSimple mSoundObj;
};

STATIC_ASSERT(sizeof(daObjPDtile_c) == 0x7fc);


#endif /* D_A_OBJ_PDTILE_H */
