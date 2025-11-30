#ifndef D_A_OBJ_KAZENEKO_H
#define D_A_OBJ_KAZENEKO_H

#include "f_op/f_op_actor.h"
#include "d/d_cc_d.h"

struct KazoNekoAttr {
    f32 mCylRadius;
    f32 mCylHeight;
    f32 field_0x08;
    f32 field_0x0c;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    u8 field_0x24;
    u8 field_0x25;
    s16 field_0x26;
    int field_0x28;
};

/**
 * @ingroup actors-objects
 * @class daObjKazeNeko_c
 * @brief Weather Vane
 *
 * @details
 *
 */
class daObjKazeNeko_c : public fopAc_ac_c {
public:
    void create_init();
    void initBaseMtx();
    void setBaseMtx();
    void initCcCylinder();
    void setCcCylinder();
    void swingHead();
    void getFirstVec(cXyz*, int);
    inline int createHeap();
    inline int draw();
    inline int execute();
    inline int create();
    inline ~daObjKazeNeko_c();

    const KazoNekoAttr& attr() { return M_attr; }

    static KazoNekoAttr const M_attr;
private:
    /* 0x568 */ J3DModel* mModel;
    /* 0x56C */ J3DModel* mArmModels[4];
    /* 0x57C */ request_of_phase_process_class mPhase;
    /* 0x584 */ Mtx mMtx;
    /* 0x5B4 */ dCcD_Stts mStts;
    /* 0x5F0 */ dCcD_Cyl mCyl;
    /* 0x72c */ cXyz field_0x72c;
    /* 0x738 */ int field_0x738;
    /* 0x73C */ f32 field_0x73c;
    /* 0x740 */ f32 field_0x740;
    /* 0x744 */ f32 field_0x744;
    /* 0x748 */ s16 field_0x748;
    /* 0x74A */ s16 field_0x74a;
    /* 0x74C */ s16 field_0x74c;
    /* 0x74E */ s16 field_0x74e;
    /* 0x750 */ s16 field_0x750;
    /* 0x752 */ u8 field_0x752;
    /* 0x753 */ u8 field_0x753;
};

STATIC_ASSERT(sizeof(daObjKazeNeko_c) == 0x754);

#endif /* D_A_OBJ_KAZENEKO_H */
