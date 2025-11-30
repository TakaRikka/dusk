#ifndef D_A_BALLOON_2D_H
#define D_A_BALLOON_2D_H

#include "dolphin/types.h"
#include "f_op/f_op_actor.h"
#include "SSystem/SComponent/c_phase.h"
#include "d/d_drawlist.h"
#include "d/d_s_play.h"

class J2DScreen;
class CPaneMgr;
class J2DPicture;
class J2DPane;
class J2DTextBox;

/**
 * @ingroup actors-unsorted
 * @class daBalloon2D_c
 * @brief ???
 *
 * @details
 *
 */
class daBalloon2D_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    class c_list : public dDlst_base_c {
    public:
        c_list() { mBalloon = NULL; }
        void set(daBalloon2D_c* i_this) { mBalloon = i_this; }

        virtual void draw();
        virtual ~c_list() {}

        daBalloon2D_c* mBalloon;
    };

    daBalloon2D_c() {
        initiate();
    }

    int createHeap();
    int create();
    int destroy();
    int draw();
    int execute();
    void drawMeter();
    void setComboCount(u8, u8);
    void setScoreCount(u32);
    void addScoreCount(cXyz*, u32, u8);
    void initiate();
    void update();
    void setComboNum(u8);
    void setBalloonSize(u8);
    void setScoreNum(int);
    void setAllAlpha();
    void setComboAlpha();
    void drawAddScore();
    void setHIO(bool);
    
    virtual
    ~daBalloon2D_c() {}

    void show() { mIsVisible = 1; }
    void hide() { mIsVisible = 0; }
    u8 isVisible() { return mIsVisible; }

    static daBalloon2D_c* myclass;

    struct ScoreCount {
        cXyz field_0x0;
        u16 field_0xc;
        u8 field_0xe;
        u8 field_0xf;
    };

    class CHeadScore : public ScoreCount {
    public:
        ~CHeadScore();
        CHeadScore();
    };

    /* 0x574 */ J2DScreen* mScreen;
    /* 0x578 */ CPaneMgr* field_0x578;
    /* 0x57C */ CPaneMgr* field_0x57c;
    /* 0x580 */ CPaneMgr* field_0x580;
    /* 0x584 */ CPaneMgr* field_0x584;
    /* 0x588 */ CPaneMgr* field_0x588;
    /* 0x58C */ CPaneMgr* field_0x58c;
    /* 0x590 */ CPaneMgr* field_0x590;
    /* 0x594 */ CPaneMgr* field_0x594;
    /* 0x598 */ CPaneMgr* field_0x598;
    /* 0x59C */ CPaneMgr* field_0x59c;
    /* 0x5A0 */ CPaneMgr* field_0x5a0;
    /* 0x5A4 */ CPaneMgr* field_0x5a4;
    /* 0x5A8 */ J2DPicture* field_0x5a8[7];
    /* 0x5A4 */ J2DPicture* field_0x5c4[10];
    /* 0x5EC */ J2DPicture* field_0x5ec;
    /* 0x5F0 */ c_list field_0x5f0;
    /* 0x5F8 */ CHeadScore field_0x5f8[20];
    /* 0x738 */ f32 mAllAlpha;
    /* 0x73C */ f32 mComboAlpha;
    /* 0x740 */ u32 mScoreCount;
    /* 0x744 */ u8 mComboNum;
    /* 0x745 */ u8 mBalloonSize;
    /* 0x746 */ u8 mIsVisible;
};

#include "f_op/f_op_actor_mng.h"

#endif /* D_A_BALLOON_2D_H */
