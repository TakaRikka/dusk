#ifndef D_A_OBJ_KZNKARM_H
#define D_A_OBJ_KZNKARM_H

#include "d/d_bg_s_acch.h"
#include "f_op/f_op_actor_mng.h"

struct daObjKznkarm_Attr_c {
    /* 0x00 */ f32 gravity;
    /* 0x04 */ f32 maximum_falling_speed;
    /* 0x08 */ f32 bounce_coeff;
    /* 0x0C */ f32 water_drag;
    /* 0x10 */ f32 initial_throwing_force;
    /* 0x14 */ f32 initial_throwing_upward_force;
    /* 0x18 */ Vec field_0x18;
    /* 0x24 */ Vec position;
    /* 0x30 */ S16Vec angular_speed;
    /* 0x36 */ s16 throw_angular_speed;
};

/**
 * @ingroup actors-objects
 * @class daObjKznkarm_c
 * @brief ???
 *
 * @details
 *
 */
class daObjKznkarm_c : public fopAc_ac_c {
public:
    enum Mode_e {
        MODE_BROKEN_e,
        MODE_CARRY_e,
        MODE_THROW_e,
        MODE_STAY_e,
        MODE_MAX_e,
    };

    typedef void (daObjKznkarm_c::*actionFunc)();
    
    void setAction(daObjKznkarm_c::Mode_e);
    void callInit();
    void callExecute();
    void initBroken();
    void executeBroken();
    void initCarry();
    void executeCarry();
    void initThrow();
    void executeThrow();
    void initStay();
    void executeStay();
    void create_init();
    void initBaseMtx();
    void setBaseMtx();
    s16 getGroundSlope(s16);

    inline int createHeap();
    inline int draw();
    inline int execute();
    inline int create();
    inline ~daObjKznkarm_c();
    inline daObjKznkarm_Attr_c* attr() const;

    static daObjKznkarm_Attr_c const M_attr;
    static actionFunc ActionTable[4][2];

private:
    /* 0x568 */ J3DModel* mpModel;
    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ dBgS_AcchCir mAcchCir;
    /* 0x5B4 */ dBgS_ObjAcch mAcch;
    /* 0x78C */ actionFunc* mAction;
    /* 0x790 */ Mode_e mMode;
    /* 0x794 */ u32 mShadowId;
    /* 0x798 */ u32 field_0x798[4];
    /* 0x7A8 */ s16 field_0x7a8;
    /* 0x7AA */ s16 field_0x7aa;
    /* 0x7AC */ s16 field_0x7ac;
    /* 0x7AE */ u8 field_0x7ae;
    /* 0x7AF */ u8 field_0x7af;
    /* 0x7B0 */ u8 field_0x7b0;
    /* 0x7B1 */ u8 field_0x7b1[0x7b4 - 0x7b1];
};

STATIC_ASSERT(sizeof(daObjKznkarm_c) == 0x7b4);

#endif /* D_A_OBJ_KZNKARM_H */
