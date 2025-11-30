#ifndef D_OVLP_D_OVLP_FADE2_H
#define D_OVLP_D_OVLP_FADE2_H

#include "d/d_com_inf_game.h"
#include "f_op/f_op_overlap_mng.h"

class dOvlpFd2_dlst_c : public dDlst_base_c {
public:
    virtual void draw();
    virtual ~dOvlpFd2_dlst_c() {}

    MtxP getMtx() { return mMtx; }
    void entry() { dComIfGd_set2DXlu(this); }

    /* 0x4 */ Mtx mMtx;
};

class dOvlpFd2_c : public overlap_task_class {
public:
    dOvlpFd2_c();
    void execFirstSnap();
    void execFadeOut();
    void execNextSnap();
    void execFadeIn();

    inline void draw();
    inline void execute();

    typedef void (dOvlpFd2_c::*executeFn)();
    void setExecute(executeFn i_func) { mExecuteFn = i_func; }

    /* 0x0CC */ executeFn mExecuteFn;
    /* 0x0D8 */ dDlst_snapShot_c mSnapshot_c;
    /* 0x0DC */ dOvlpFd2_dlst_c mDlst_c;
    /* 0x110 */ s16 field_0x110;
    /* 0x112 */ s16 field_0x112;
    /* 0x114 */ s16 field_0x114;
    /* 0x118 */ f32 field_0x118;
    /* 0x11C */ u8 field_0x11c;
    /* 0x11D */ s8 mTimer;
    /* 0x11E */ u8 field_0x11e;
};

#endif /* D_OVLP_D_OVLP_FADE2_H */
