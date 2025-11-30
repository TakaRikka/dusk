#ifndef D_A_OBJ_SMOKE_H
#define D_A_OBJ_SMOKE_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-objects
 * @class daObjSmoke_c
 * @brief Elde Inn Stove Smoke
 *
 * @details
 *
 */
class daObjSmoke_c : public fopAc_ac_c {
public:
    u8 getSwbit() {
        return fopAcM_GetParamBit(this,0,8);
    }

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int create();
    int execute();
    int _delete();

private:
    /* 0x568 */ JPABaseEmitter* mParticle;
};

STATIC_ASSERT(sizeof(daObjSmoke_c) == 0x56C);

#endif /* D_A_OBJ_SMOKE_H */
