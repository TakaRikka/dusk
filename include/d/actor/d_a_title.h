#ifndef D_A_TITLE_H
#define D_A_TITLE_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_drawlist.h"

class dDlst_daTitle_c : public dDlst_base_c {
public:
    virtual void draw();
    virtual ~dDlst_daTitle_c() {}

    /* 0x4 */ J2DScreen* Scr;
};

class mDoDvdThd_mountArchive_c;
class CPaneMgrAlpha;

/**
 * @ingroup actors-unsorted
 * @class daTitle_c
 * @brief Title Logo
 *
 * @details
 *
 */
class daTitle_c : public fopAc_ac_c {
public:
    //daTitle_c() {}
    int CreateHeap();
    int create();
    static int createHeapCallBack(fopAc_ac_c*);
    int Execute();
    void KeyWaitAnm();
    void loadWait_init();
    void loadWait_proc();
    void logoDispWaitInit();
    void logoDispWait();
    void logoDispAnmInit();
    void logoDispAnm();
    void keyWaitInit();
    void keyWait();
    void nextScene_init();
    void nextScene_proc();
    void fastLogoDispInit();
    void fastLogoDisp();
    int getDemoPrm();
    int Draw();
    int Delete();

private:
    /* 0x568 */ request_of_phase_process_class mPhaseReq;
    /* 0x570 */ JKRHeap* mpHeap;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ mDoExt_bckAnm mBck;
    /* 0x594 */ mDoExt_bpkAnm mBpk;
    /* 0x5AC */ mDoExt_brkAnm mBrk;
    /* 0x5C4 */ mDoExt_btkAnm mBtk;
    /* 0x5DC */ JKRExpHeap* m2DHeap;
    /* 0x5E0 */ mDoDvdThd_mountArchive_c* mpMount;
    /* 0x5E4 */ dDlst_daTitle_c mTitle;
    /* 0x5EC */ JUTFont* mpFont;
    /* 0x5F0 */ u8 field_0x5f0[8];
    /* 0x5F8 */ u8 field_0x5f8;
    /* 0x5F9 */ u8 field_0x5f9;
    /* 0x5FA */ u8 field_0x5fa;
    /* 0x5FB */ u8 mProcID;
    /* 0x5FC */ u8 field_0x5fc;
    /* 0x600 */ CPaneMgrAlpha* field_0x600;
    /* 0x604 */ u8 field_0x604;
};  // Size: 0x608

typedef void (daTitle_c::*procFunc)();

STATIC_ASSERT(sizeof(daTitle_c) == 0x608);

#endif /* D_A_TITLE_H */
