#ifndef D_A_OBJ_LV4PRWALL_H
#define D_A_OBJ_LV4PRWALL_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daObjLv4PRwall_c
 * @brief Arbiter's Grounds Rail Wall (Not Stallord?)
 *
 * @details
 *
 */
class daObjLv4PRwall_c : public dBgS_MoveBgActor, request_of_phase_process_class {
public:
    int create1st();
    void setMtx();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();
    virtual ~daObjLv4PRwall_c() {}
    
    int getType() { return fopAcM_GetParamBit(this, 0, 3); }
    int getSwNo() { return fopAcM_GetParamBit(this, 3, 8); }
    int getSw2No() { return fopAcM_GetParamBit(this, 11, 8); }
    int getSw3No() { return fopAcM_GetParamBit(this, 19, 8); }

    /* 0x5A8 */ Mtx mBgMtx2;
    /* 0x5D8 */ Mtx mModelMtx;
    /* 0x608 */ J3DModel* mpModel;
    /* 0x60C */ s16 field_0x60c;
    /* 0x60E */ s16 field_0x60e;
    /* 0x610 */ s16 field_0x610;
    /* 0x614 */ int mType;
    /* 0x618 */ u8 field_0x618;
    /* 0x619 */ u8 field_0x619;
    /* 0x61C */ f32 field_0x61c;
    /* 0x620 */ f32 field_0x620;
    /* 0x624 */ int field_0x624;
    /* 0x628 */ u8 field_0x628;
    /* 0x629 */ u8 field_0x629;
};

STATIC_ASSERT(sizeof(daObjLv4PRwall_c) == 0x62c);


#endif /* D_A_OBJ_LV4PRWALL_H */
