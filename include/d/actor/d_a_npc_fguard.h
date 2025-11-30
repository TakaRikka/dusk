#ifndef D_A_NPC_FGUARD_H
#define D_A_NPC_FGUARD_H

#include "d/actor/d_a_npc_cd2.h"
#include "SSystem/SComponent/c_math.h"

/**
 * @ingroup actors-npcs
 * @class daNpcFgd_c
 * @brief ???
 *
 * @details
 *
*/

class daNpcFgd_c : public daNpcCd2_c {
public:
    void initPosAngle(Vec&, s16);
    cPhs__Step create();
    void create_init();
    inline virtual ~daNpcFgd_c();

    int getType() { return fopAcM_GetParam(this) & 0xFF; }
    int getObjNum() { return (fopAcM_GetParam(this) >> 8) & 0xFF; }
    inline int createHeap();
    inline void initCollision();
    int getFirstFlag() { return ((fopAcM_GetParam(this) >> 16) & 0xFF) != 0; }
    void setAnime(int param_1) { setAnm((J3DAnmTransformKey*)getAnmP(param_1, mObjNum), 1.0f, 12.0f, 2, cM_rndF(5.0f), -1); }
    inline void setBaseMtx();
    inline void checkGroundHeight();
    inline void setCollision();
    inline int execute();
    inline int draw();
    void offDrawFlag() { mFlag = 0; }
    void onDrawFlag() { mFlag = 1; }

    /* 0xAC8 */ J3DModel* field_0xac8;
    /* 0xACC */ int mObjNum;
    /* 0xAD0 */ u32 field_0xad0;
    /* 0xAD4 */ f32 field_0xad4;
    /* 0xAD8 */ u8 mFlag;
};

#endif /* D_A_NPC_FGUARD_H */
