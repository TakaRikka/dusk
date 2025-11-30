#ifndef D_A_BG_H
#define D_A_BG_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_w_kcol.h"

class daBg_btkAnm_c {
public:
    int create(J3DModelData*, J3DAnmTextureSRTKey*, int);
    void entry(J3DModelData*);
    void play();

    void entryFrame() { mpBtk->entryFrame(); }
    void playspeed(f32 speed) { mpBtk->setPlaySpeed(speed); }

    /* 0x0 */ mDoExt_btkAnm* mpBtk;
    /* 0x4 */ u8 field_0x4;
};

class daBg_brkAnm_c {
public:
    int create(J3DModelData*, J3DAnmTevRegKey*, int);
    void entry(J3DModelData*);
    void play();

    void entryFrame() { mpBrk->entryFrame(); }
    void entryFrame(f32 frame) { mpBrk->entryFrame(frame); }
    f32 getEndFrame() { return mpBrk->getEndFrame(); }

    /* 0x0 */ mDoExt_brkAnm* mpBrk;
    /* 0x4 */ u8 field_0x4;
};

struct daBg_Part {
    /* 0x00 */ J3DModel* model;
    /* 0x04 */ daBg_btkAnm_c* btk;
    /* 0x08 */ daBg_brkAnm_c* brk;
    /* 0x0C */ dKy_tevstr_c* tevstr;
    /* 0x10 */ f32 btk_speed;
};  // Size: 0x14

/**
 * @ingroup actors-unsorted
 * @class daBg_c
 * @brief Background
 *
 * @details Stage collision.
 *
 */
class daBg_c : public fopAc_ac_c {
public:
    const char* setArcName();
    inline int createHeap();
    ~daBg_c();
    inline int draw();
    inline int create();
    inline int execute();
    inline int isDelete();

private:
    /* 0x568 */ u8 field_0x568[0x570 - 0x568];
    /* 0x570 */ daBg_Part mBgParts[6];
    /* 0x5E8 */ dBgW* mpBgW;
    /* 0x5EC */ dBgWKCol* mpKCol;
    /* 0x5F0 */ u8 field_0x5f0;
    /* 0x5F0 */ u8 field_0x5f1;
};

STATIC_ASSERT(sizeof(daBg_c) == 0x5F4);


#endif /* D_A_BG_H */
