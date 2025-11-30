#ifndef D_A_COACH_2D_H
#define D_A_COACH_2D_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_pane_class.h"
#include "d/d_msg_scrn_light.h"
#include "d/d_drawlist.h"

/**
 * @ingroup actors-unsorted
 * @class daCoach2D_c
 * @brief Coach 2D
 *
 * @details
 *
 */
class daCoach2D_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    class c_list : public dDlst_base_c {
    public:
        c_list() { mpCoach2D = NULL; }

        virtual void draw();
        virtual ~c_list() {}

        void set(daCoach2D_c* i_coach2D) {
            mpCoach2D = i_coach2D;
        }

        /* 0x4 */ daCoach2D_c* mpCoach2D;
    };

    int createHeap();
    int create();
    int destroy();
    int draw();
    int execute();
    void drawMeter();
    void initiate();
    void update();
    void setBrkAnime(bool);

    daCoach2D_c() { initiate(); }
    virtual ~daCoach2D_c() {}

    void show() { mIsVisible = true; }
    void hide() { mIsVisible = false; }
    bool isVisible() { return mIsVisible ? true : false; }
    void setHitCount(u8 i_hitCount) { mHitCount = i_hitCount; }
    void setMaxHitCount(u8 i_maxHitCount) { mMaxHitCount = i_maxHitCount; }

    /* 0x574 */ J2DScreen* mpScrn;
    /* 0x578 */ J2DAnmTevRegKey* mpFireIconBrk;
    /* 0x57C */ dMsgScrnLight_c* mMsgLight;
    /* 0x580 */ CPaneMgr* mpPaneAll;
    /* 0x584 */ CPaneMgr* mpPaneBasha;
    /* 0x588 */ CPaneMgr* mpPaneFire;
    /* 0x58C */ CPaneMgr* mpPaneFireB;
    struct {
        /* 0x00 */ f32 pos_x;
        /* 0x04 */ f32 pos_y;
        /* 0x08 */ f32 pikari_pos_x;
        /* 0x0C */ f32 pikari_pos_y;
        /* 0x10 */ f32 pikari_anm_frame;
        /* 0x14 */ u8 field_0x14;
        /* 0x15 */ u8 field_0x15;
    }
    /* 0x590 */ mIconInfo[20];
    /* 0x770 */ c_list mList;
    /* 0x778 */ f32 mBrkFrame;
    /* 0x77C */ f32 mBrkSpeed;
    /* 0x780 */ u8 mHitCount;
    /* 0x781 */ u8 mMaxHitCount;
    /* 0x782 */ u8 mIsVisible;
};

STATIC_ASSERT(sizeof(daCoach2D_c) == 0x784);

#endif /* D_A_COACH_2D_H */
