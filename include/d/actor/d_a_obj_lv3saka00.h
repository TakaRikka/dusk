#ifndef D_A_OBJ_LV3SAKA00_H
#define D_A_OBJ_LV3SAKA00_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjLv3saka_c
 * @brief Lakebed Temple Spiral 00
 *
 * @details
 *
 */
class daObjLv3saka_c : public dBgS_MoveBgActor {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    void setNrmDzb();
    void setWtrDzb();
    int Draw();
    int Delete();

    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8) & 0xFF; }
    u8 getType() { return fopAcM_GetParamBit(this, 28, 4) & 0xFF; }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ dBgW* mpBgW2;
};

#endif /* D_A_OBJ_LV3SAKA00_H */
