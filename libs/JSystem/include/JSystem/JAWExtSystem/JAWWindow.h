#ifndef JAWWINDOW_H
#define JAWWINDOW_H

#include "JSystem/JAWExtSystem/JAWGraphContext.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/J2DGraph/J2DWindow.h"
#include "JSystem/JGeometry.h"

class JAWGraphContext;
struct JUTGamePad;

class JUTPoint {
public:
    JUTPoint(int i_x, int i_y) {
        x = i_x;
        y = i_y;
    }

    /* 0x00 */ int x;
    /* 0x04 */ int y;
};

class JAWWindow {
public:
    class TWindowText : public J2DPane {
    public:
        TWindowText(JAWWindow*);
        virtual ~TWindowText();
        virtual void drawSelf(f32, f32);
        virtual void drawSelf(f32, f32, Mtx*);

        /* 0x0FC */ JAWGraphContext m_graf;
        /* 0x118 */ JAWWindow* m_pParent;
        /* 0x11C */ JUTPoint m_point;
    };

    class TJ2DWindowDraw : public J2DWindow {
    public:
        TJ2DWindowDraw(u32 param_1, const JGeometry::TBox2<f32>& param_2, const char* param_3) : J2DWindow(param_1, param_2, param_3, TEXTUREBASE_0, NULL) {}
        virtual ~TJ2DWindowDraw() {}

        void drawPane(int x, int y, const J2DGrafContext* p_grafCtx) {
            J2DPane::draw(x, y, p_grafCtx, true, true);
            calcMtx();
        }
    };

    JAWWindow(const char*, int, int);
    virtual ~JAWWindow();
    virtual void onDraw(JAWGraphContext*);
    virtual BOOL onInit();
    virtual void frameWork() {}
    virtual void onPadProc(const JUTGamePad&) {}
    virtual void onTrigA(const JUTGamePad&) {}
    virtual void onTrigB(const JUTGamePad&) {}
    virtual void onTrigX(const JUTGamePad&) {}
    virtual void onTrigY(const JUTGamePad&) {}
    virtual void onTrigMenu(const JUTGamePad&) {}
    virtual void onTrigL(const JUTGamePad&) {}
    virtual void onTrigZ(const JUTGamePad&) {}
    virtual void onTrigUp(const JUTGamePad&) {}
    virtual void onTrigDown(const JUTGamePad&) {}
    virtual void onTrigLeft(const JUTGamePad&) {}
    virtual void onTrigRight(const JUTGamePad&) {}
    virtual void onReleaseA(const JUTGamePad&) {}
    virtual void onReleaseB(const JUTGamePad&) {}
    virtual void onReleaseX(const JUTGamePad&) {}
    virtual void onReleaseY(const JUTGamePad&) {}
    virtual void onReleaseMenu(const JUTGamePad&) {}
    virtual void onReleaseL(const JUTGamePad&) {}
    virtual void onReleaseZ(const JUTGamePad&) {}
    virtual void onReleaseUp(const JUTGamePad&) {}
    virtual void onReleaseDown(const JUTGamePad&) {}
    virtual void onReleaseLeft(const JUTGamePad&) {}
    virtual void onReleaseRight(const JUTGamePad&) {}
    virtual void onKeyA(const JUTGamePad&) {}
    virtual void onKeyB(const JUTGamePad&) {}
    virtual void onKeyX(const JUTGamePad&) {}
    virtual void onKeyY(const JUTGamePad&) {}
    virtual void onKeyMenu(const JUTGamePad&) {}
    virtual void onKeyL(const JUTGamePad&) {}
    virtual void onKeyZ(const JUTGamePad&) {}
    virtual void onKeyUp(const JUTGamePad&) {}
    virtual void onKeyDown(const JUTGamePad&) {}
    virtual void onKeyLeft(const JUTGamePad&) {}
    virtual void onKeyRight(const JUTGamePad&) {}

    BOOL initIf();
    void setTitleColor(const JUtility::TColor&, const JUtility::TColor&);
    void setWindowColor(const JUtility::TColor&, const JUtility::TColor&, const JUtility::TColor&, const JUtility::TColor&);
    void move(f32, f32);
    void addPosition(f32, f32);
    void addSize(f32, f32);
    static JUtility::TColor convJudaColor(u16);
    void padProc(const JUTGamePad&);

    void setWindowColor(const JUtility::TColor& color) {
        setWindowColor(color, color, color, color);
    }

    void setWindowColor(u8 r, u8 g, u8 b, u8 a) {
        setWindowColor(JUtility::TColor(r, g, b, a));
    }

    /* 0x004 */ Mtx mMatrix;
    /* 0x034 */ u8 field_0x34[0x38 - 0x34];
    /* 0x038 */ TJ2DWindowDraw m_drawWindow;
    /* 0x180 */ J2DTextBox m_titleText;
    /* 0x2B0 */ TWindowText m_windowText;
    /* 0x3D8 */ JUtility::TColor m_windowColor0;
    /* 0x3DC */ JUtility::TColor m_windowColor1;
    /* 0x3E0 */ JUtility::TColor m_windowColor2;
    /* 0x3E4 */ JUtility::TColor m_windowColor3;
    /* 0x3E8 */ int field_0x3e8;
    /* 0x3EC */ u8 m_isInit;

    void setMatrix(Mtx mtx) { MTXCopy(mtx, mMatrix); }
    void setAlpha(u8 alpha) { m_drawWindow.setAlpha(alpha); }
    void draw(int x, int y, const J2DGrafContext* p_grafCtx) { m_drawWindow.drawPane(x, y, p_grafCtx); }
};

#endif /* JAWWINDOW_H */
