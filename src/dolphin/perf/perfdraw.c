#include <dolphin.h>
#include <dolphin/perf.h>
#include "fake_tgmath.h"

#include "__perf.h"

DECL_WEAK f32 HEIGHT(u32 a, f32 f);
DECL_WEAK f32 COORD(u32 a);

// internal macro for Perfdraw.
#define DRAW_RECT(x1, x2, y1, y2, color)       \
    do {                                       \
        GXSetChanMatColor(GX_COLOR0A0, color); \
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);      \
        GXPosition3f32((x1), (y1), -1.0f);     \
        GXPosition3f32((x1), (y2), -1.0f);     \
        GXPosition3f32((x2), (y2), -1.0f);     \
        GXPosition3f32((x2), (y1), -1.0f);     \
        GXEnd();                               \
    } while(0)

static u32 DrawFrameMax;
static f32 DrawFrameH;
static u32 MaxBusTransactions;

static u32 DrawNumFrames = 3;
static f32 DrawFrameW = 205.33333f;
static GXColor DrawFrameBGColor = { 0xC8, 0xC8, 0xC8, 0xC8 };
static GXColor DrawFrameColor = { 0x19, 0x19, 0x19, 0xC8 };
static GXColor DrawCPUColor = { 0xFF, 0x19, 0x00, 0xC8 };
static GXColor DrawFullColor = { 0xFF, 0x00, 0xFF, 0xC8 };
static GXColor DrawGPColor = { 0x00, 0x64, 0xFF, 0xC8 };
static GXColor DrawCPUCacheColor = { 0x00, 0x96, 0x00, 0xC8 };
static GXColor DrawConnectColor = { 0x00, 0x00, 0x00, 0xC8 };
static GXColor DrawBWBarColor = { 0x32, 0x32, 0x32, 0xC8 };
static GXColor DrawIPCBarColor = { 0x00, 0x00, 0x5A, 0xAA };
static GXColor DrawGPUBarColor = { 0x5A, 0x00, 0x00, 0xAA };
static GXColor DrawIPCColor = { 0xC8, 0x64, 0x00, 0xAA };
static GXColor DrawCPColor = { 0xC8, 0x00, 0xC8, 0xC8 };
static GXColor DrawTCColor = { 0x00, 0xC8, 0x00, 0xC8 };
static GXColor DrawCPURDColor = { 0xFF, 0xFF, 0x00, 0xC8 };
static GXColor DrawCPUWRColor = { 0x00, 0x64, 0x64, 0xC8 };
static GXColor DrawDSPColor = { 0xC8, 0x00, 0x00, 0xC8 };
static GXColor DrawIOColor = { 0x96, 0x96, 0x32, 0xC8 };
static GXColor DrawVIColor = { 0xFF, 0xFF, 0xFF, 0xC8 };
static GXColor DrawPEColor = { 0x00, 0x00, 0xC8, 0xC8 };
static GXColor DrawRFColor = { 0x00, 0xFF, 0xFF, 0xC8 };
static GXColor DrawFIColor = { 0xC8, 0x64, 0x64, 0xC8 };
static GXColor DrawGPXFIColor = { 0x00, 0xC8, 0x00, 0xAA };
static GXColor DrawGPXFOColor = { 0x00, 0x00, 0xC8, 0xAA };
static GXColor DrawGPRASIDLEColor = { 0xC8, 0xC8, 0x00, 0xAA };

static BOOL bDrawBWBar = TRUE;
static BOOL bDrawCPUBar = TRUE;
static BOOL bDrawXFBars = TRUE;
static BOOL bDrawRASBar = TRUE;
static BOOL bAutoScale = TRUE;

static BOOL bDrawBWBarKey;
static f32 lastx;

static f32 FramePts[28] = {
    0.0f,
    0.0f,
    0.0f,
    10.0f,
    0.0f,
    0.0f,
    616.0f,
    0.0f,
    616.0f,
    0.0f,
    616.0f,
    10.0f,
    0.0f,
    10.0f,
    616.0f,
    10.0f,
    205.33333f,
    0.0f,
    205.33333f,
    10.0f,
    410.66666f,
    0.0f,
    410.66666f,
    10.0f,
    616.0f,
    0.0f,
    616.0f,
    10.0f,
};

static f32 CPUPts[4] = {
    0.0f,  
    0.0f,
    616.0f,
    0.0f,
};

static f32 GPPts[4] = {
    0.0f,  
    0.0f,
    616.0f,
    0.0f,
};

void (*GameDrawInit)();
Mtx mID;

#if !DEBUG
inline f32 HEIGHT(u32 a, f32 f) {
    return 140.0f * ((f32) a / ((f32) MaxBusTransactions * f));
}

inline f32 COORD(u32 a) {
    return 616.0f * ((f32) a / (f32) DrawFrameMax);
}
#endif

void __PERFDrawInit(void (*id)()) {
    MTXIdentity(mID);
    GameDrawInit = id;
    DrawFrameMax = (OS_CORE_CLOCK / 60) * 3;
    DrawFrameH = (PERFNumEvents + 1) * 7;
    MaxBusTransactions = (OS_BUS_CLOCK / 120);
    FramePts[3] = FramePts[11] = FramePts[13] = FramePts[15] = FramePts[19] = FramePts[23] = DrawFrameH;
    FramePts[6] = FramePts[8] = FramePts[10] = FramePts[14] = 616.0f;
    GPPts[1] = (PERFNumEvents + 2) * 19;
    GPPts[3] = GPPts[1];
}

static Mtx44 mProj;
f32 pSave[7];

void PERFPreDraw(void) {
    u32 i;
    u32 j;

    GXGetProjectionv(pSave);

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mProj[i][j] = 0.0f;
        }
    }

    mProj[0][0] = 0.003125f;
    mProj[1][1] = 0.004166667f; 
    mProj[2][2] = 1.0f;
    mProj[3][3] = 1.0f;
    mProj[0][3] = -0.95f;
    mProj[1][3] = -0.87500005f;

    GXSetProjection(mProj, GX_ORTHOGRAPHIC);
    GXClearVtxDesc();
    GXInvalidateVtxCache();
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetZMode(GX_DISABLE, GX_ALWAYS, GX_DISABLE);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
}

static void DrawBWBar(PerfSample* s) {
    u32 delta;
    u32 interval;
    f32 bwscale;
    f32 lastY;
    f32 x1;
    f32 x2;
    f32 height;
    u32 rasclocks;
    u32 rasBusy;
    u32 xfI;
    u32 xfO;
    u32 instructions;
    f32 ipc;
    f32 ipcscale;
    u32 misses;

    interval = s->gpTimeStampEnd - s->gpTimeStampStart;
    bwscale = (f32)interval / (OS_CORE_CLOCK / 60);
    lastY = 7.0f + DrawFrameH;
    x1 = COORD(s->gpTimeStampStart);
    x2 = COORD(s->gpTimeStampEnd);
    if (fabs(lastx - x1) < 1.0f) {
        x1 = lastx;
    }
    lastx = x2;

    // Draw BW Bars if toggled
    if (bDrawBWBar) {
        delta = s->cpReq[1] - s->cpReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawCPColor);
            lastY += height;
        }
        delta = s->tcReq[1] - s->tcReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawTCColor);
            lastY += height;
        }
        delta = s->cpuRdReq[1] - s->cpuRdReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawCPURDColor);
            lastY += height;
        }
        delta = s->cpuWrReq[1] - s->cpuWrReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawCPUWRColor);
            lastY += height;
        }
        delta = s->dspReq[1] - s->dspReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawDSPColor);
            lastY += height;
        }
        delta = s->ioReq[1] - s->ioReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawIOColor);
            lastY += height;
        }
        delta = s->viReq[1] - s->viReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawVIColor);
            lastY += height;
        }
        delta = s->peReq[1] - s->peReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawPEColor);
            lastY += height;
        }
        delta = s->rfReq[1] - s->rfReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawRFColor);
            lastY += height;
        }
        s->fiReq[0] /= 2;
        s->fiReq[1] /= 2;
        delta = s->fiReq[1] - s->fiReq[0];
        height = HEIGHT(delta, bwscale);
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawFIColor);
        }
    }

    if (bDrawCPUBar) {
        instructions = s->instructions[1] - s->instructions[0];
        ipc = (f32)instructions / (f32) interval;
        ipcscale = ipc / 2.0f;
        misses = s->cacheMisses[1] - s->cacheMisses[0];
        lastY = 7.0f + (140.0f + (7.0f + DrawFrameH));
        height = ipcscale * 50.0f;
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawIPCColor);
        }
        height = (50.0f * (f32) misses) / (f32) interval;
        DRAW_RECT(x1, x2, (50.0f + lastY) - height, (50.0f + lastY), DrawCPUCacheColor);
    }

    rasclocks = s->rasClocks[1] - s->rasClocks[0];

    if (bDrawXFBars) {
        lastY = 14.0f + (50.0f + (140.0f + (7.0f + DrawFrameH)));
        xfI = s->xfWaitIn[1] - s->xfWaitIn[0];
        xfO = s->xfWaitOut[1] - s->xfWaitOut[0];
        if (rasclocks >= (u32) (xfO + xfI)) {
            xfI = rasclocks - (xfO + xfI);
            height = (50.0f * xfI) / rasclocks;
            if (height > 1.0f) {
                DRAW_RECT(x1, x2, lastY, lastY + height, DrawGPXFIColor);
            }
        }
    }

    if (bDrawRASBar) {
        lastY = 50.0f + (21.0f + (50.0f + (140.0f + (7.0f + DrawFrameH))));
        rasBusy = s->rasBusy[1] - s->rasBusy[0];
        height = (50.0f * (f32)rasBusy) / rasclocks;
        if (height > 1.0f) {
            DRAW_RECT(x1, x2, lastY, lastY + height, DrawGPRASIDLEColor);
        }
    }
}

#if DEBUG
DECL_WEAK f32 HEIGHT(u32 a, f32 f) {
    return 140.0f * ((f32) a / ((f32) MaxBusTransactions * f));
}

DECL_WEAK f32 COORD(u32 a) {
    return 616.0f * ((f32) a / (f32) DrawFrameMax);
}
#endif

static void DrawKey(void) {
    u32 delta;
    u32 foo[2];
    f32 bwscale;
    f32 lastY;
    f32 x1;
    f32 x2;
    f32 height;

    x1 = 595.4667f;
    x2 = 616.0f;
    lastY = 7.0f + DrawFrameH;
    bwscale = 1.0f;
    foo[0] = 0;
    foo[1] = MaxBusTransactions / 10;

    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) {
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawCPColor);
        lastY += height; 
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) {
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawTCColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawCPURDColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawCPUWRColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawDSPColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawIOColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawVIColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawPEColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawRFColor);
        lastY += height;
    }
    
    delta = (foo[1] - foo[0]);
    height = HEIGHT(delta, bwscale);
    if (height > 1.0f) { 
        DRAW_RECT(x1, x2, lastY, lastY + height, DrawFIColor);
        lastY += height;
    }
}

void PERFDumpScreen(void) {
    PerfSample* samples;
    u32 s;
    u32 id;
    u32 i;
    u32 delta;
    u32 e;
    f32 c;
    f32 lastY;
    f32 allX;
    f32 xcoord;

    if (GameDrawInit) {
        samples = PERFFrames[PERFCurrFrame].samples;
        
        if (bAutoScale) {
            DrawNumFrames = PERFFrames[PERFCurrFrame].end / (OS_CORE_CLOCK / 60) + 1;
        }
        
        DrawFrameMax = (OS_CORE_CLOCK / 60) * DrawNumFrames;
        DrawFrameW = 616.0f / DrawNumFrames;
        allX = COORD(PERFFrames[PERFCurrFrame].end);
        GXLoadPosMtxImm(mID, 0);
        DRAW_RECT(0.0f, 616.0f, 0.0f, DrawFrameH, DrawFrameBGColor);
        GXSetChanMatColor(GX_COLOR0A0, DrawFrameColor);
        GXSetLineWidth(0xCU, GX_TO_ZERO);

        // different draw shape? Consider other forms of this draw macro that may work. If anything will work.
        GXBegin(GX_LINES, GX_VTXFMT0, 8);
        GXPosition3f32(FramePts[0], FramePts[1], -1.0f);
        GXPosition3f32(FramePts[2], FramePts[3], -1.0f);
        GXPosition3f32(FramePts[4], FramePts[5], -1.0f);
        GXPosition3f32(FramePts[6], FramePts[7], -1.0f);
        GXPosition3f32(FramePts[8], FramePts[9], -1.0f);
        GXPosition3f32(FramePts[10], FramePts[11], -1.0f);
        GXPosition3f32(FramePts[12], FramePts[13], -1.0f);
        GXPosition3f32(FramePts[14], FramePts[15], -1.0f);
        GXEnd();

        if (DrawNumFrames > 1) {
            GXBegin(GX_LINES, GX_VTXFMT0, (DrawNumFrames - 1) * 2);
            for(i = 1; i < DrawNumFrames; i++) {
                xcoord = COORD(i * (OS_CORE_CLOCK / 60));
                GXPosition3f32(xcoord, FramePts[17], -1.0f);
                GXPosition3f32(xcoord, FramePts[19], -1.0f);
            }
            GXEnd();
        }

        GXSetChanMatColor(GX_COLOR0A0, DrawFullColor);
        GXSetLineWidth(0x20U, GX_TO_ZERO);

        GXBegin(GX_LINES, GX_VTXFMT0, 2);
        GXPosition3f32(0.0f, 0.0f, -1.0f);
        GXPosition3f32(allX, 0.0f, -1.0f);
        GXEnd();

        GXSetChanMatColor(GX_COLOR0A0, DrawCPUCacheColor);

        GXBegin(GX_LINES, GX_VTXFMT0, 2);
        GXPosition3f32(0.0f, 0.0f, -1.0f);
        GXPosition3f32(COORD(PERFFrames[PERFCurrFrame].cachemisscycles), 0.0f, -1.0f); 
        GXEnd();

        if (bDrawBWBar) {
            lastY = 7.0f + DrawFrameH;
            GXSetChanMatColor(GX_COLOR0A0, DrawBWBarColor);

            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(0.0f, lastY, -1.0f);
            GXPosition3f32(0.0f, 140.0f + lastY, -1.0f);
            GXPosition3f32(allX, 140.0f + lastY, -1.0f); 
            GXPosition3f32(allX, lastY, -1.0f);
            GXEnd();
        }

        if (bDrawCPUBar) {
            lastY = 7.0f + (140.0f + (7.0f + DrawFrameH));
            GXSetChanMatColor(GX_COLOR0A0, DrawIPCBarColor);

            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(0.0f, lastY, -1.0f);
            GXPosition3f32(0.0f, 50.0f + lastY, -1.0f);
            GXPosition3f32(allX, 50.0f + lastY, -1.0f);
            GXPosition3f32(allX, lastY, -1.0f);
            GXEnd();
            GXSetChanMatColor(GX_COLOR0A0, DrawConnectColor);
            GXSetLineWidth(6, GX_TO_ZERO);
            GXBegin(GX_LINES, GX_VTXFMT0, 2);
            GXPosition3f32(0.0f, 25.0f + lastY, -1.0f);
            GXPosition3f32(allX, 25.0f + lastY, -1.0f);
            GXEnd();
        }

        if (bDrawXFBars) {
            lastY = 14.0f + (50.0f + (140.0f + (7.0f + DrawFrameH)));
            GXSetChanMatColor(GX_COLOR0A0, DrawGPUBarColor);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(0.0f, lastY, -1.0f);
            GXPosition3f32(0.0f, 50.0f + lastY, -1.0f);
            GXPosition3f32(allX, 50.0f + lastY, -1.0f);
            GXPosition3f32(allX, lastY, -1.0f);
            GXEnd();
        }

        if (bDrawRASBar) {
            lastY = 50.0f + (21.0f + (50.0f + (140.0f + (7.0f + DrawFrameH))));
            GXSetChanMatColor(GX_COLOR0A0, DrawGPUBarColor);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4); 
            GXPosition3f32(0.0f, lastY, -1.0f);
            GXPosition3f32(0.0f, 50.0f + lastY, -1.0f);
            GXPosition3f32(allX, 50.0f + lastY, -1.0f);
            GXPosition3f32(allX, lastY, -1.0f);
            GXEnd();
        }

        for (s = 0; s < PERFFrames[PERFCurrFrame].lastSample; s++) {
            id = samples[s].id;
            if (id == 0xFF) {
                DrawBWBar(&samples[s]);
            } else {
                switch(PERFEvents[id].type) {
                    case PERF_CPU_GP_EVENT:
                        GXSetChanMatColor(GX_COLOR0A0, PERFEvents[id].color);
                        GXSetLineWidth(32, GX_TO_ZERO);
                        GXBegin(GX_LINES, GX_VTXFMT0, 2);
                        GXPosition3f32(COORD(samples[s].cpuTimeStampStart), (f32) ((id + 1) * 7), -1.0f);
                        GXPosition3f32(COORD(samples[s].cpuTimeStampEnd), (f32) ((id + 1) * 7), -1.0f); 
                        GXEnd();
                        delta = samples[s].cacheMisses[3] - samples[s].cacheMisses[2];
                        if (delta) {
                            e = delta + samples[s].cpuTimeStampStart;
                            c = COORD(e);
                            GXSetChanMatColor(GX_COLOR0A0, DrawCPUCacheColor);
                            GXBegin(GX_LINES, GX_VTXFMT0, 2);
                            GXPosition3f32(COORD(samples[s].cpuTimeStampStart), (f32) ((id + 1) * 7), -1.0f);
                            GXPosition3f32(c, (f32) ((id + 1) * 7), -1.0f);
                            GXEnd();
                        }

                        if (samples[s].gpTimeStampEnd != 0) {
                            if (!samples[s].interrupted) {
                                GXSetChanMatColor(GX_COLOR0A0, DrawGPColor);
                                GXSetLineWidth(32, GX_TO_ZERO);
                                GXBegin(GX_LINES, GX_VTXFMT0, 2);
                                GXPosition3f32(COORD(samples[s].gpTimeStampStart), DrawFrameH, -1.0f);
                                GXPosition3f32(COORD(samples[s].gpTimeStampEnd), DrawFrameH, -1.0f);
                                GXEnd();

                                DrawBWBar(&samples[s]);
                                
                                GXSetChanMatColor(GX_COLOR0A0, DrawConnectColor);
                                GXSetLineWidth(6, GX_TO_ZERO);
                                GXBegin(GX_LINES, GX_VTXFMT0, 4);
                                GXPosition3f32(COORD(samples[s].gpTimeStampStart), DrawFrameH, -1.0f);
                                GXPosition3f32(COORD(samples[s].cpuTimeStampStart), (f32) ((id + 1) * 7), -1.0f);
                                GXPosition3f32(COORD(samples[s].gpTimeStampEnd), DrawFrameH, -1.0f);
                                GXPosition3f32(COORD(samples[s].cpuTimeStampEnd), (f32) ((id + 1) * 7), -1.0f);
                                GXEnd();
                            } else {
                                GXSetChanMatColor(GX_COLOR0A0, DrawGPColor);
                                GXSetLineWidth(32, GX_TO_ZERO);
                                GXBegin(GX_LINES, GX_VTXFMT0, 2);
                                GXPosition3f32(COORD(samples[s].origgpStart), DrawFrameH, -1.0f);
                                GXPosition3f32(COORD(samples[s].gpTimeStampEnd), DrawFrameH, -1.0f);
                                GXEnd();

                                DrawBWBar(&samples[s]);
                                
                                GXSetChanMatColor(GX_COLOR0A0, DrawConnectColor);
                                GXSetLineWidth(6, GX_TO_ZERO);
                                GXBegin(GX_LINES, GX_VTXFMT0, 4);
                                GXPosition3f32(COORD(samples[s].origgpStart), DrawFrameH, -1.0f);
                                GXPosition3f32(COORD(samples[s].origcpuStart), (f32) ((id + 1) * 7), -1.0f);
                                GXPosition3f32(COORD(samples[s].gpTimeStampEnd), DrawFrameH, -1.0f);
                                GXPosition3f32(COORD(samples[s].cpuTimeStampEnd), (f32) ((id + 1) * 7), -1.0f);
                                GXEnd();
                            }
                        }
                        break;
                    case PERF_CPU_EVENT:
                        GXSetChanMatColor(GX_COLOR0A0, PERFEvents[id].color);
                        GXSetLineWidth(32, GX_TO_ZERO);
                        GXBegin(GX_LINES, GX_VTXFMT0, 2);
                        !samples[s].cpuTimeStampStart; // needed to match
                        GXPosition3f32(COORD(samples[s].cpuTimeStampStart), (f32) ((id + 1) * 7), -1.0f);
                        GXPosition3f32(COORD(samples[s].cpuTimeStampEnd), (f32) ((id + 1) * 7), -1.0f);
                        GXEnd();

                        delta = samples[s].cacheMisses[3] - samples[s].cacheMisses[2];
                        if (delta != 0) {
                            e = delta + samples[s].cpuTimeStampStart;
                            c = COORD(e);
                            GXSetChanMatColor(GX_COLOR0A0, DrawCPUCacheColor);
                            GXBegin(GX_LINES, GX_VTXFMT0, 2);
                            GXPosition3f32(COORD(samples[s].cpuTimeStampStart), (f32) ((id + 1) * 7), -1.0f);
                            GXPosition3f32(c, (f32) ((id + 1) * 7), -1.0f);
                            GXEnd();
                        }
                        break;
                    case PERF_GP_EVENT:
                        if (samples[s].interrupted == 0) {
                            GXSetChanMatColor(GX_COLOR0A0, DrawGPColor);
                            GXSetLineWidth(32, GX_TO_ZERO);
                            GXBegin(GX_LINES, GX_VTXFMT0, 2);
                            GXPosition3f32(COORD(samples[s].gpTimeStampStart), DrawFrameH, -1.0f);
                            GXPosition3f32(COORD(samples[s].gpTimeStampEnd), DrawFrameH, -1.0f);
                            GXEnd();
                            DrawBWBar(&samples[s]);
                        } else {
                            GXSetChanMatColor(GX_COLOR0A0, DrawGPColor);
                            GXSetLineWidth(32, GX_TO_ZERO);
                            GXBegin(GX_LINES, GX_VTXFMT0, 2);
                            GXPosition3f32(COORD(samples[s].origgpStart), DrawFrameH, -1.0f);
                            GXPosition3f32(COORD(samples[s].gpTimeStampEnd), DrawFrameH, -1.0f);
                            GXEnd();
                            DrawBWBar(&samples[s]);
                        }
                        break;
                }
            }
        }

        if (bDrawBWBarKey) {
            DrawKey();
        }
    }
}

void PERFPostDraw(void) {
    u32 i;
    u32 j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mProj[i][j] = 0.0f;
        }
    }

    mProj[0][0] = pSave[1];
    mProj[0][2] = pSave[2];
    mProj[1][1] = pSave[3];
    mProj[1][2] = pSave[4];
    mProj[2][2] = pSave[5];
    mProj[2][3] = pSave[6];
    mProj[3][2] = -1.0f;
    GXSetProjection(mProj, GX_PERSPECTIVE);
    GameDrawInit();
}

void PERFSetDrawBWBarKey(BOOL tf) {
    bDrawBWBarKey = tf;
}

void PERFSetDrawBWBar(BOOL tf) {
    bDrawBWBar = tf;
}

void PERFSetDrawCPUBar(BOOL tf) {
    bDrawCPUBar = tf;
}

void PERFSetDrawXFBars(BOOL tf) {
    bDrawXFBars = tf;
}

void PERFSetDrawRASBar(BOOL tf) {
    bDrawRASBar = tf;
}

void PERFToggleDrawBWBarKey(void) {
    bDrawBWBarKey = (bDrawBWBarKey) ? FALSE : TRUE; 
}

void PERFToggleDrawBWBar(void) {
    bDrawBWBar = (bDrawBWBar) ? FALSE : TRUE; 
}

void PERFToggleDrawCPUBar(void) {
    bDrawCPUBar = (bDrawCPUBar) ? FALSE : TRUE; 
}

void PERFToggleDrawXFBars(void) {
    bDrawXFBars = (bDrawXFBars) ? FALSE : TRUE; 
}

void PERFToggleDrawRASBar(void) {
    bDrawRASBar = (bDrawRASBar) ? FALSE : TRUE; 
}

void PERFSetDrawFrames(u32 frames) {
    if (frames != 0) {
        DrawNumFrames = frames;
        bAutoScale = FALSE;
    } else {
        bAutoScale = TRUE;
    }
}
