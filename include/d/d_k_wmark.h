#ifndef D_D_K_WMARK_H
#define D_D_K_WMARK_H

#include "f_op/f_op_kankyo_mng.h"
#include "d/d_kankyo.h"

class dkWmark_c : public kankyo_class {
public:
    ~dkWmark_c();
    BOOL setMatrix();

    inline int create();
    inline int execute();
    inline int draw();

    static int m_nowID;

    static void setFootMark(cXyz* i_pos, s16 param_1, int param_2) {
        fopKyM_create(PROC_WMARK, param_2 | (param_1 << 0x10), i_pos, NULL, NULL);
    }

    /* 0x0F8 */ JKRSolidHeap* mpHeap;
    /* 0x0FC */ J3DModel* mpModel;
    /* 0x100 */ dKy_tevstr_c mTevstr;
    /* 0x488 */ u8 field_0x488;
    /* 0x48A */ s16 field_0x48a;
    /* 0x48C */ s16 field_0x48c;
    /* 0x48E */ s16 mColorType;
    /* 0x490 */ f32 mAlphaFactor;
    /* 0x494 */ int field_0x494;
};

#endif /* D_D_K_WMARK_H */
