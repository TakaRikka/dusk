#ifndef D_D_KANTERA_ICON_METER_H
#define D_D_KANTERA_ICON_METER_H

#include "d/d_drawlist.h"

class CPaneMgr;

class dDlst_KanteraIcon_c : public dDlst_base_c {
public:
    virtual void draw();
    virtual ~dDlst_KanteraIcon_c();

    void setScreen(J2DScreen* screen) { mp_scrn = screen; }
    J2DScreen* getScreen() { return mp_scrn; }

private:
    /* 0x04 */ J2DScreen* mp_scrn;
};

class dKantera_icon_c {
public:
    dKantera_icon_c();
    void initiate();
    void setAlphaRate(f32);
    void setPos(f32, f32);
    void setScale(f32, f32);
    void setNowGauge(u16, u16);

    virtual ~dKantera_icon_c();

    void drawSelf() { mpKanteraIcon->draw(); }

private:
    /* 0x04 */ dDlst_KanteraIcon_c* mpKanteraIcon;
    /* 0x08 */ CPaneMgr* mpParent;
    /* 0x0C */ CPaneMgr* mpGauge;
};

#endif /* D_D_KANTERA_ICON_METER_H */
