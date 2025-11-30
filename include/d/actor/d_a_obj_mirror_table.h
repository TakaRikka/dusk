#ifndef D_A_OBJ_MIRROR_TABLE_H
#define D_A_OBJ_MIRROR_TABLE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_w.h"

/**
 * @ingroup actors-objects
 * @class daObjMirrorTable_c
 * @brief Twilight Mirror Table
 *
 * @details
 *
 */
class daObjMirrorTable_c : public fopAc_ac_c {
public:
    inline int createHeap();
    inline bool isSwitch();
    inline void checkOnPanel();
    int draw();
    inline void setBaseMtx();
    int execute();
    inline ~daObjMirrorTable_c();
    cPhs__Step create();
    void create_init();
    void initBaseMtx();

    u8 getSwitchNo() { return (fopAcM_GetParam(this) >> 8) & 0xff; }
    void setAnmSpeed(f32 speed) { mpTableUpBckAnm->setPlaySpeed(speed); }
    void setEffect() { field_0x874 = true; }

    /* 0x568 */ J3DModel* mpTableModel;
    /* 0x56C */ J3DModel* mpMirrorModel;
    /* 0x570 */ J3DModel* mpStairModel;
    /* 0x574 */ J3DModel* mpPanelModel;
    /* 0x578 */ J3DModel* mpLightModel;
    /* 0x57C */ J3DModel* mpMSquareModel;
    /* 0x580 */ mDoExt_bckAnm* mpTableUpBckAnm;
    /* 0x584 */ mDoExt_bckAnm* mpMSquareBckAnm;
    /* 0x588 */ mDoExt_brkAnm* mpStairBrkAnm;
    /* 0x58C */ mDoExt_brkAnm* mpMSquareBrkAnm;
    /* 0x590 */ mDoExt_btkAnm* mpLightBtkAnm;
    /* 0x594 */ request_of_phase_process_class mPhaseReq;
    /* 0x59C */ dBgW mBgW[3];
    /* 0x7DC */ Mtx mMtx[3];
    /* 0x86C */ JPABaseEmitter* mpEmitter1;
    /* 0x870 */ JPABaseEmitter* mpEmitter2;
    /* 0x874 */ bool field_0x874;
    /* 0x875 */ s8 field_0x875;
};

STATIC_ASSERT(sizeof(daObjMirrorTable_c) == 0x878);

#endif /* D_A_OBJ_MIRROR_TABLE_H */
