#ifndef D_D_K_WPILLAR_H
#define D_D_K_WPILLAR_H

#include "f_op/f_op_kankyo_mng.h"
#include "d/d_kankyo.h"

class dkWpillar_c : public kankyo_class {
public:
    int create();
    ~dkWpillar_c();
    int execute();
    int draw();

    /* 0x0F8 */ J3DModel* mpModel;
    /* 0x0FC */ mDoExt_bckAnm mBck;
    /* 0x118 */ mDoExt_btkAnm mBtk;
    /* 0x130 */ mDoExt_brkAnm mBrk;
    /* 0x148 */ JKRSolidHeap* mpHeap;
    /* 0x14C */ dKy_tevstr_c mTevstr;
};

#endif /* D_D_K_WPILLAR_H */
