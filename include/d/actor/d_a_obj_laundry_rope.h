#ifndef D_A_OBJ_LAUNDRY_ROPE_H
#define D_A_OBJ_LAUNDRY_ROPE_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor.h"

struct daObjLndRope_Attr_c {
    f32 mGravity;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
};

struct daObjLndRope_Hio_c {
    f32 mGravity;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
};

/**
 * @ingroup actors-objects
 * @class daObjLndRope_c
 * @brief Laundry Rope
 *
 * @details
 *
 */
class daObjLndRope_c : public fopAc_ac_c {
public:
    void create_init();
    void initBaseMtx();
    void setBaseMtx();
    void setNormalRopePos();

    inline ~daObjLndRope_c();
    inline int create();
    inline int createHeap();
    inline int draw();
    inline int execute();
    inline u8 getPathId();
    inline cXyz* getRopeStartPos();
    inline float getStartRate(cXyz* param_1);


    #if DEBUG
    const daObjLndRope_Hio_c* attr() const {
        return &M_Hio;
    };
    #else
    const daObjLndRope_Attr_c* attr() const {
        return &mAttr;
    };
    #endif

    static const daObjLndRope_Attr_c mAttr;
    static const daObjLndRope_Hio_c M_Hio;

private:
    /* 0x0568 */ mDoExt_3DlineMat1_c mRopeMat;
    /* 0x05a4 */ request_of_phase_process_class mPhase;
    /* 0x05ac */ Mtx mMtx;
    /* 0x05dc */ dCcD_Stts mStts;
    /* 0x0618 */ dCcD_Sph mSphs[13];
    /* 0x15f0 */ cXyz mPositions1[13];
    /* 0x168c */ cXyz mPositions2[13];
    /* 0x1728 */ fpc_ProcID mProcIds[13];
    /* 0x175c */ cXyz mPos;
    /* 0x1768 */ f32 mScale;
    /* 0x176c */ f32 mSegmentLength;
    /* 0x1770 */ u8 mFlag;
};

STATIC_ASSERT(sizeof(daObjLndRope_c) == 0x1774);


#endif /* D_A_OBJ_LAUNDRY_ROPE_H */
