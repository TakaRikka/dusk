#ifndef M_DO_M_DO_GRAPHIC_H
#define M_DO_M_DO_GRAPHIC_H

#include "JSystem/JFramework/JFWDisplay.h"
#include "m_Do/m_Do_mtx.h"
#include "global.h"

#if WIDESCREEN_SUPPORT
#define FB_WIDTH  (640)
#define FB_HEIGHT (456)
#else
#define FB_WIDTH  (608)
#define FB_HEIGHT (448)
#endif

int mDoGph_Create();
void mDoGph_drawFilterQuad(s8 param_0, s8 param_1);

struct ResTIMG;
class JKRSolidHeap;
class mDoGph_gInf_c {
public:
    class bloom_c {
    public:
        bloom_c() { m_buffer = NULL; }
        void create();
        void remove();
        void draw();

        u8 getEnable() { return mEnable; }
        void setEnable(u8 i_enable) { mEnable = i_enable; }
        GXColor* getMonoColor() { return &mMonoColor; }
        GXColor* getBlendColor() { return &mBlendColor; }
        void setPoint(u8 i_point) { mPoint = i_point; }
        void setBlureSize(u8 i_size) { mBlureSize = i_size; }
        void setBlureRatio(u8 i_ratio) { mBlureRatio = i_ratio; }
        void setBlendColor(GXColor i_color) { mBlendColor = i_color; }
        void setMonoColor(GXColor i_color) { mMonoColor = i_color; }
        void setMode(u8 i_mode) { mMode = i_mode; }
        void* getBuffer() { return m_buffer; }
        u8 getPoint() { return mPoint; }
        u8 getBlureSize() { return mBlureSize; }
        u8 getBlureRatio() { return mBlureRatio; }

        /* 0x00 */ GXColor mBlendColor;
        /* 0x04 */ GXColor mMonoColor;
        /* 0x08 */ u8 mEnable;
        /* 0x09 */ u8 mMode;
        /* 0x0A */ u8 mPoint;
        /* 0x0B */ u8 mBlureSize;
        /* 0x0C */ u8 mBlureRatio;
        /* 0x10 */ void* m_buffer;
    };

    #if PLATFORM_WII || PLATFORM_SHIELD
    class csr_c {
    public:
        virtual ~csr_c();
        virtual bool isPointer();

        void particleExecute();

        static u32 m_blurID;
    };

    static void entryBaseCsr(csr_c*);
    #endif

    static void create();
    static void beginRender();
    static void fadeOut(f32);
    static void fadeOut(f32, _GXColor&);
    static void fadeIn(f32 fadeSpeed, _GXColor& fadeColor) {
        fadeOut(-fadeSpeed, fadeColor);
    }
    static void fadeOut_f(f32, _GXColor&);
    static void onBlure(const Mtx);
    static void onBlure();
    static void calcFade();

    static void fadeIn(f32 fadeSpeed) {
        fadeOut(-fadeSpeed);
    }

    static JUTFader* getFader() { return mFader; }
    static void setFader(JUTFader* fader) {
        JFWDisplay::getManager()->setFader(fader);
        mFader = fader;
    }

    static int startFadeOut(int param_0) { return JFWDisplay::getManager()->startFadeOut(param_0); }
    static int startFadeIn(int param_0) { return JFWDisplay::getManager()->startFadeIn(param_0); }
    static void setFadeColor(JUtility::TColor& color) { mFader->setColor(color); }
    static void setClearColor(JUtility::TColor color) { JFWDisplay::getManager()->setClearColor(color); }
    static void setBackColor(GXColor& color) { mBackColor = color; }
    static void endFrame() { JFWDisplay::getManager()->endFrame(); }
    static void offFade() { mFade = 0; }
    static u8 isFade() { return mFade; }
    static void fadeIn_f(f32 i_fadeSpeed, _GXColor& i_fadeColor) { fadeOut_f(-i_fadeSpeed, i_fadeColor); }
    static void offBlure() { mBlureFlag = false; }
    static u8 isBlure() { return mBlureFlag; }
    static void setBlureRate(u8 i_rate) { mBlureRate = i_rate; }
    static u8 getBlureRate() { return mBlureRate; }
    static MtxP getBlureMtx() { return mBlureMtx; }
    static void offAutoForcus() { mAutoForcus = 0; }
    static BOOL isAutoForcus() { return mAutoForcus; }
    static void setTickRate(u32 rate) { JFWDisplay::getManager()->setTickRate(rate); }
    static void waitBlanking(int wait) { JFWDisplay::getManager()->waitBlanking(wait); }

    static void setBlureMtx(const Mtx m) {
        cMtx_copy(m, mBlureMtx);
    }

    static f32 getWidthF() {
        #if PLATFORM_WII || PLATFORM_SHIELD
        return m_widthF;
        #else
        return FB_WIDTH;
        #endif
    }

    static f32 getHeightF() {
        #if PLATFORM_WII || PLATFORM_SHIELD
        return m_heightF;
        #else
        return FB_HEIGHT;
        #endif
    }

    static f32 getWidth() { return FB_WIDTH; }
    static f32 getHeight() { return FB_HEIGHT; }

    static f32 getMinYF() {
        #if PLATFORM_WII || PLATFORM_SHIELD
        return m_minYF;
        #else
        return 0.0f;
        #endif
    }

    static f32 getMinXF() {
        #if PLATFORM_WII || PLATFORM_SHIELD
        return m_minXF;
        #else
        return 0.0f;
        #endif
    }

    static f32 getMaxYF() {
        #if PLATFORM_WII || PLATFORM_SHIELD
        return m_maxYF;
        #else
        return FB_HEIGHT;
        #endif
    }

    static f32 getMaxXF() {
        #if PLATFORM_WII || PLATFORM_SHIELD
        return m_maxXF;
        #else
        return FB_WIDTH;
        #endif
    }

    static f32 getAspect() {
        #if WIDESCREEN_SUPPORT
        return m_aspect;
        #else
        return 1.3571428f;
        #endif
    }

    static int getMinY() {
        #if WIDESCREEN_SUPPORT
        return m_minY;
        #else
        return 0;
        #endif
    }

    static int getMinX() {
        #if WIDESCREEN_SUPPORT
        return m_minX;
        #else
        return 0;
        #endif
    }

    static int getMaxY() {
        #if WIDESCREEN_SUPPORT
        return m_maxY;
        #else
        return FB_HEIGHT;
        #endif
    }

    static int getMaxX() {
        #if WIDESCREEN_SUPPORT
        return m_maxX;
        #else
        return FB_WIDTH;
        #endif
    }

    static ResTIMG* getFrameBufferTimg() { return mFrameBufferTimg; }
    static ResTIMG* getZbufferTimg() { return mZbufferTimg; }
    static void* getFrameBufferTex() { return mFrameBufferTex; }
    static void* getZbufferTex() { return mZbufferTex; }
    static void setFadeRate(f32 rate) { mFadeRate = rate; }
    static f32 getFadeRate() { return mFadeRate; }
    static bloom_c* getBloom() { return &m_bloom; }
    static GXColor& getFadeColor() { return mFadeColor; }
    static GXColor& getBackColor() { return mBackColor; }
    static void endRender() { JFWDisplay::getManager()->endRender(); }
    static GXTexObj* getZbufferTexObj() { return &mZbufferTexObj; }
    static GXTexObj* getFrameBufferTexObj() { return &mFrameBufferTexObj; }
    static void setFrameRate(u16 i_rate) { JFWDisplay::getManager()->setFrameRate(i_rate); }

    static int getFrameBufferSize() {
        #define RoundUp16b(x)   (u16)(((u16)(x) + 16 - 1) & ~(16 - 1))
        return RoundUp16b(JFWDisplay::getManager()->getEfbWidth()) * JFWDisplay::getManager()->getEfbHeight() * 2;
    }

    static void* getFrameBufferMemory() {
        return JFWDisplay::getManager()->getXfbManager()->getDisplayingXfb();
    }

    static f32 getInvScale() {
        #if WIDESCREEN_SUPPORT
        return m_invScale;
        #else
        return 1.0f;
        #endif
    }

    static f32 getScale() { return 1.0f; }

    #if WIDESCREEN_SUPPORT
    static void setTvSize();

    static void onWide();
    static void offWide();
    static u8 isWide();

    static void onWideZoom();
    static void offWideZoom();
    static BOOL isWideZoom();

    static void setWideZoomProjection(Mtx44& m);
    static void setWideZoomLightProjection(Mtx& m);
    #endif

    static GXTexObj mFrameBufferTexObj;
    static GXTexObj mZbufferTexObj;
    static bloom_c m_bloom;
    static Mtx mBlureMtx;
    static GXColor mBackColor;
    static GXColor mFadeColor;
    static JUTFader* mFader;
    static ResTIMG* mFrameBufferTimg;
    static void* mFrameBufferTex;
    static ResTIMG* mZbufferTimg;
    static void* mZbufferTex;
    static f32 mFadeRate;
    static f32 mFadeSpeed;
    static u8 mBlureFlag;
    static u8 mBlureRate;
    static u8 mFade;
    static bool mAutoForcus;

    #if PLATFORM_SHIELD
    static JKRHeap* getHeap() {
        return m_heap;
    }

    static void setHeap(JKRSolidHeap* i_heap) {
        m_heap = (JKRHeap*)i_heap;
    }

    static JKRHeap* m_heap;
    #endif

    #if PLATFORM_WII || PLATFORM_SHIELD
    static void resetDimming();

    static csr_c* m_baseCsr;
    static csr_c* m_csr;

    static cXyz m_nowEffPos;
    static cXyz m_oldEffPos;
    static cXyz m_oldOldEffPos;
    #endif

    #if WIDESCREEN_SUPPORT
    static u8 mWide;
    static u8 mWideZoom;
    static ResTIMG* m_fullFrameBufferTimg;
    static void* m_fullFrameBufferTex;
    static GXTexObj m_fullFrameBufferTexObj;

    static f32 m_aspect;
    static f32 m_scale;
    static f32 m_invScale;

    static f32 m_minXF;
    static f32 m_minYF;
    static int m_minX;
    static int m_minY;

    static f32 m_maxXF;
    static f32 m_maxYF;
    static int m_maxX;
    static int m_maxY;

    static int m_width;
    static int m_height;
    static f32 m_heightF;
    static f32 m_widthF;
    #endif
};

#endif /* M_DO_M_DO_GRAPHIC_H */
