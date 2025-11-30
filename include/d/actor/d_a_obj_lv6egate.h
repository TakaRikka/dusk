#ifndef D_A_OBJ_LV6EGATE_H
#define D_A_OBJ_LV6EGATE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjLv6EGate_c
 * @brief Temple of Time Electric Gate
 *
 * @details
 *
 */
class daObjLv6EGate_c : public dBgS_MoveBgActor, public request_of_phase_process_class {
public:
    int create1st();
    void setMtx();
    void offDisp();
    void onDisp();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();
    virtual ~daObjLv6EGate_c() {}

    int getSwNo() { return fopAcM_GetParamBit(this, 0, 8); }
    int getType() { return fopAcM_GetParamBit(this, 8, 1); }

    /* 0x05A8 */ Mtx field_0x5a8;
    /* 0x05D8 */ Mtx field_0x5d8;
    /* 0x0608 */ dCcD_Stts mCcStts;
    /* 0x0644 */ dCcD_Cyl mAtCyl[8];
    /* 0x1024 */ u8 mElecOff;
    /* 0x1025 */ u8 field_0x1025;
    /* 0x1028 */ u32 mEmtId[4];
};

STATIC_ASSERT(sizeof(daObjLv6EGate_c) == 0x1038);

#endif /* D_A_OBJ_LV6EGATE_H */
