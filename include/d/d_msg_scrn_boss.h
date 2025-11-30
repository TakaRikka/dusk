#ifndef MSG_SCRN_D_MSG_SCRN_BOSS_H
#define MSG_SCRN_D_MSG_SCRN_BOSS_H

#include "d/d_msg_scrn_base.h"

class dMsgScrnBoss_c : public dMsgScrnBase_c {
public:
    dMsgScrnBoss_c();

    virtual ~dMsgScrnBoss_c();
    virtual void exec();
    virtual void drawSelf();
    virtual void fukiScale(f32);
    virtual void fukiTrans(f32, f32);
    virtual void fontAlpha(f32);
    virtual void fukiAlpha(f32);

private:
    /* 0xC4 */ CPaneMgr* mpBaseParent;
    /* 0xC8 */ CPaneMgr* mpFontParent;
};

#endif /* MSG_SCRN_D_MSG_SCRN_BOSS_H */
