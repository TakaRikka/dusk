#ifndef D_A_OBJ_FCHAIN_H
#define D_A_OBJ_FCHAIN_H

#include "f_op/f_op_actor_mng.h"

class daObjFchain_shape_c : public J3DPacket {
public:
    virtual void draw();
    virtual ~daObjFchain_shape_c() {}
};

/**
 * @ingroup actors-objects
 * @class daObjFchain_c
 * @brief Wolf Chain Shackle
 *
 * @details
 *
 */
class daObjFchain_c : public fopAc_ac_c {
public:
    int createHeap();
    int create();
    ~daObjFchain_c();
    void checkPlayerFoot(cXyz const*, cXyz const*, cXyz*);
    void setGroundVec(cXyz*, f32);
    int execute();
    int draw();

    cXyz* getPos() { return field_0x694; }
    csXyz* getAngle() { return field_0x8a4; }
    J3DModelData* getModelData() { return mModelData; }

private:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModelData* mModelData;
    /* 0x574 */ daObjFchain_shape_c mShape;
    /* 0x584 */ s16 field_0x584;
    /* 0x586 */ u16 field_0x586;
    /* 0x588 */ s16 field_0x588;
    /* 0x58C */ cXyz field_0x58c[22];
    /* 0x694 */ cXyz field_0x694[22];
    /* 0x79C */ cXyz field_0x79c[22];
    /* 0x8A4 */ csXyz field_0x8a4[22];
};

STATIC_ASSERT(sizeof(daObjFchain_c) == 0x928);


#endif /* D_A_OBJ_FCHAIN_H */
