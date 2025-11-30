#ifndef MSG_SCRN_D_MSG_SCRN_TREE_H
#define MSG_SCRN_D_MSG_SCRN_TREE_H

#include "d/d_msg_scrn_base.h"

class JUTFont;
class JKRExpHeap;

struct dMsgScrnTree_c : public dMsgScrnBase_c {
    dMsgScrnTree_c(JUTFont*, JKRExpHeap*);
    virtual ~dMsgScrnTree_c();
    void exec();
    void draw();
    void fukiAlpha(f32);
    void fukiScale(f32);
    void fukiTrans(f32, f32);

    /* 0xC4 */ CPaneMgr* field_0xc4;
    /* 0xC8 */ CPaneMgr* field_0xc8;
    /* 0xCC */ J2DAnmTransform* field_0xcc;
    /* 0xD0 */ J2DAnmTextureSRTKey* field_0xd0;
    /* 0xD4 */ J2DAnmTextureSRTKey* field_0xd4;
    /* 0xD8 */ JKRExpHeap* field_0xd8;
    /* 0xDC */ f32 field_0xdc;
    /* 0xDC */ f32 field_0xe0;

};

#endif /* MSG_SCRN_D_MSG_SCRN_TREE_H */
