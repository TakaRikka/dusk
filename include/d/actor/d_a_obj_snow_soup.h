#ifndef D_A_OBJ_SNOW_SOUP_H
#define D_A_OBJ_SNOW_SOUP_H

#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-objects
 * @class daObjSnowSoup_c
 * @brief Snowpeak Ruins Soup Pot?
 *
 * @details
 *
 */
class daObjSnowSoup_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    daObjSnowSoup_c();
    virtual ~daObjSnowSoup_c();
    int createHeap();
    cPhs__Step create();
    int Delete();
    int draw();
    int execute();
    void init();
    void setModelMtx();
    void createSmkEmtCommon();
    void deleteSmkEmtCommon();
    void createSmkEmtChange(int);
    void deleteSmkEmtChange(int);

private:
    /* 0x574 */ cXyz mFirewoodPos;
    /* 0x580 */ cXyz mBubblePos;
    /* 0x58C */ JPABaseEmitter* mpSmkEmtCommon[4];
    /* 0x59C */ JPABaseEmitter* mpSmkEmtChange[3][2];
    /* 0x5B4 */ J3DModel* mpModel[3];
    /* 0x5C0 */ u8 mState;
};

STATIC_ASSERT(sizeof(daObjSnowSoup_c) == 0x5C4);

#endif /* D_A_OBJ_SNOW_SOUP_H */
