#ifndef D_A_OBJ_MASTER_SWORD_H
#define D_A_OBJ_MASTER_SWORD_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"

class daObjMasterSword_c;
typedef void (daObjMasterSword_c::*actionFunc)();

struct daObjMasterSword_Attr_c {
    f32 field_0x0;
};

/**
 * @ingroup actors-objects
 * @class daObjMasterSword_c
 * @brief Master Sword
 *
 * @details
 *
*/

class daObjMasterSword_c : public fopAc_ac_c {
public:
    enum Mode_e {
        MODE_0_e,
    };

    void initWait();
    void executeWait();
    static int createHeapCallBack(fopAc_ac_c* i_this);
    int CreateHeap();
    int create();
    inline void create_init();
    ~daObjMasterSword_c();
    int draw();

    inline void initCollision();
    inline void setCollision();
    inline void initBaseMtx();
    inline void setAction(Mode_e i_mode);
    inline void callInit();
    inline void callExecute();
    inline int execute();

    f32 attr() const { return mAttr.field_0x0; }

    u8 getEventID() { return (fopAcM_GetParam(this) >> 0x10) & 0xFF; }
    u16 getFlagNo() { return fopAcM_GetParam(this) & 0xFFFF; }

    static daObjMasterSword_Attr_c const mAttr;
    static actionFunc ActionTable[];

private:
    /* 0x568 */ J3DModel* mpModel;
    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ mDoExt_btkAnm mBtk;
    /* 0x58C */ mDoExt_brkAnm mBrk;
    /* 0x5A4 */ dCcD_Stts mCcStts;
    /* 0x5E0 */ dCcD_Cyl mCyl;
    /* 0x71C */ actionFunc* mActionFunc;
    /* 0x720 */ Mode_e mMode;
    /* 0x724 */ u32 mShadowKey;
    /* 0x728 */ cBgS_PolyInfo field_0x728;
    /* 0x738 */ f32 field_0x738;
};

STATIC_ASSERT(sizeof(daObjMasterSword_c) == 0x73C);

#endif /* D_A_OBJ_MASTER_SWORD_H */
