#ifndef D_D_GAMEOVER_H
#define D_D_GAMEOVER_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_menu_save.h"
#include "f_op/f_op_msg.h"
#include "f_op/f_op_msg_mng.h"
#include "f_pc/f_pc_manager.h"
#include "m_Do/m_Do_hostIO.h"

class dDlst_Gameover_CAPTURE_c : public dDlst_base_c {
public:
    virtual void draw();
    virtual ~dDlst_Gameover_CAPTURE_c() {}
};

class dMsgScrnLight_c;
class dDlst_GameOverScrnDraw_c : public dDlst_base_c {
public:
    dDlst_GameOverScrnDraw_c(JKRArchive*);
    void setBackAlpha(f32);

    virtual void draw();
    virtual ~dDlst_GameOverScrnDraw_c();

    /* 0x04 */ J2DScreen* mpScreen;
    /* 0x08 */ J2DPicture* mpBackImg;
    /* 0x0C */ dMsgScrnLight_c* mpLight;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ JUtility::TColor mFadeColor;
};  // Size: 0x18

enum dGameover_Proc {
    /* 0 */ PROC_PLAYER_ANM_WAIT,
    /* 1 */ PROC_DISP_FADE_OUT,
    /* 2 */ PROC_DISP_WAIT,
    /* 3 */ PROC_DEMO_FADE_IN,
    /* 4 */ PROC_DEMO_FADE_OUT,
    /* 5 */ PROC_SAVE_OPEN,
    /* 6 */ PROC_SAVE_MOVE,
    /* 7 */ PROC_SAVE_CLOSE,
    /* 8 */ PROC_DELETE_WAIT,
};

class dGameover_c : public msg_class {
public:
    int _create();
    int _execute();
    void playerAnmWait_init();
    void playerAnmWait_proc();
    void dispFadeOut_init();
    void dispFadeOut_proc();
    void dispWait_init();
    void dispWait_proc();
    void demoFadeIn_init();
    void demoFadeIn_proc();
    void demoFadeOut_init();
    void demoFadeOut_proc();
    void saveOpen_init();
    void saveOpen_proc();
    void saveMove_init();
    void saveMove_proc();
    void saveClose_init();
    void saveClose_proc();
    void deleteWait_init();
    void deleteWait_proc();
    int _draw();
    int _delete();

    s32 deleteCheck() { return mProc == 8; }
    void animeStart() { mIsDemoSave = true; }

    /* 0x0FC */ dMenu_save_c* dMs_c;
    /* 0x100 */ dDlst_GameOverScrnDraw_c* dgo_screen_c;
    /* 0x104 */ dDlst_Gameover_CAPTURE_c* dgo_capture_c;
    /* 0x108 */ request_of_phase_process_class mPhase;
    /* 0x110 */ JKRHeap* mpHeap;
    /* 0x114 */ s16 mTimer;
    /* 0x116 */ s16 field_0x116;
    /* 0x118 */ u8 mProc;
    /* 0x119 */ bool mIsDemoSave;
};  // Size: 0x11C

s32 d_GameOver_Create(u8 param_0);
bool d_GameOver_Delete(fpc_ProcID& i_id);

inline s32 d_GameOver_CheckDelete(fpc_ProcID& id) {
    if (id != fpcM_ERROR_PROCESS_ID_e) {
        dGameover_c* gameover = (dGameover_c*)fopMsgM_SearchByID(id);

        if (gameover != NULL) {
            return gameover->deleteCheck();
        } else {
            return 0;
        }
    }

    return 0;
}

inline s32 d_GameOver_animeStart(fpc_ProcID& id) {
    if (id != fpcM_ERROR_PROCESS_ID_e) {
        dGameover_c* gameover = (dGameover_c*)fopMsgM_SearchByID(id);

        if (gameover != NULL) {
            gameover->animeStart();
            return 1;
        } else {
            return 0;
        }
    }

    return 0;
}

#endif /* D_D_GAMEOVER_H */
