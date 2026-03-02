#include <revolution/gx.h>
#include <revolution/os.h>

#include "__gx.h"

GXRenderModeObj GXNtsc240Ds = {
    1,
    640, 240, 240,
    40, 0,
    640, 480,
    0,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXNtsc240DsAa = {
    1, 640, 240, 240, 40, 0, 640, 480, 0, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXNtsc240Int = {
    0, 640, 240, 240, 40, 0, 640, 480, 0, 1, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXNtsc240IntAa = {
    0, 640, 240, 240, 40, 0, 640, 480, 0, 1, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXNtsc480IntDf = {
    0, 640, 480, 480, 40, 0, 640, 480, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 8, 8, 10, 12, 10, 8, 8 }
};

GXRenderModeObj GXNtsc480Int = {
    0, 640, 480, 480, 40, 0, 640, 480, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXNtsc480IntAa = {
    0, 640, 242, 480, 40, 0, 640, 480, 1, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 4, 8, 12, 16, 12, 8, 4 }
};

GXRenderModeObj GXNtsc480Prog = {
    2, 640, 480, 480, 40, 0, 640, 480, 0, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXNtsc480ProgSoft = {
    2, 640, 480, 480, 40, 0, 640, 480, 0, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 8, 8, 10, 12, 10, 8, 8 }
};

GXRenderModeObj GXNtsc480ProgAa = {
    2, 640, 242, 480, 40, 0, 640, 480, 0, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 4, 8, 12, 16, 12, 8, 4 }
};

GXRenderModeObj GXMpal240Ds = {9, 640, 240, 240, 40, 0, 640, 480, 0, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXMpal240DsAa = {9, 640, 240, 240, 40, 0, 640, 480, 0, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXMpal240Int = {8, 640, 240, 240, 40, 0, 640, 480, 0, 1, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXMpal240IntAa = {8, 640, 240, 240, 40, 0, 640, 480, 0, 1, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXMpal480IntDf = {8, 640, 480, 480, 40, 0, 640, 480, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 8, 8, 10, 12, 10, 8, 8 } };
GXRenderModeObj GXMpal480Int = {8, 640, 480, 480, 40, 0, 640, 480, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXMpal480IntAa = {8, 640, 242, 480, 40, 0, 640, 480, 1, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 4, 8, 12, 16, 12, 8, 4 } };

GXRenderModeObj GXMpal480Prog = {
    10,
    640, 480, 480,
    40, 0,
    640, 480,
    0,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXMpal480ProgSoft = {
    10,
    640, 480, 480,
    40, 0,
    640, 480,
    0,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 8, 8, 10, 12, 10, 8, 8 }
};

GXRenderModeObj GXMpal480ProgAa = {
    10,
    640, 242, 480,
    40, 0,
    640, 480,
    0,
    0,
    1,
    { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 },
    { 4, 8, 12, 16, 12, 8, 4 }
};

GXRenderModeObj GXPal264Ds = {5, 640, 264, 264, 40, 11, 640, 528, 0, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXPal264DsAa = {5, 640, 264, 264, 40, 11, 640, 528, 0, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXPal264Int = {4, 640, 264, 264, 40, 23, 640, 528, 0, 1, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXPal264IntAa = {4, 640, 264, 264, 40, 23, 640, 528, 0, 1, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXPal528IntDf = {4, 640, 528, 528, 40, 23, 640, 528, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 8, 8, 10, 12, 10, 8, 8 } };
GXRenderModeObj GXPal528Int = {4, 640, 528, 528, 40, 23, 640, 528, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXPal524IntAa = {4, 640, 264, 524, 40, 23, 640, 524, 1, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 4, 8, 12, 16, 12, 8, 4 } };
GXRenderModeObj GXEurgb60Hz240Ds = {21, 640, 240, 240, 40, 0, 640, 480, 0, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXEurgb60Hz240DsAa = {21, 640, 240, 240, 40, 0, 640, 480, 0, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXEurgb60Hz240Int = {20, 640, 240, 240, 40, 0, 640, 480, 0, 1, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXEurgb60Hz240IntAa = {20, 640, 240, 240, 40, 0, 640, 480, 0, 1, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXEurgb60Hz480IntDf = {20, 640, 480, 480, 40, 0, 640, 480, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 8, 8, 10, 12, 10, 8, 8 } };
GXRenderModeObj GXEurgb60Hz480Int = {20, 640, 480, 480, 40, 0, 640, 480, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };
GXRenderModeObj GXEurgb60Hz480IntAa = {20, 640, 242, 480, 40, 0, 640, 480, 1, 0, 1, { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 }, { 4, 8, 12, 16, 12, 8, 4 } };

GXRenderModeObj GXEurgb60Hz480Prog = {
    22,
    640, 480, 480,
    40, 0,
    640, 480,
    0,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 0, 0, 21, 22, 21, 0, 0 }
};

GXRenderModeObj GXEurgb60Hz480ProgSoft = {
    22,
    640, 480, 480,
    40, 0,
    640, 480,
    0,
    0,
    0,
    { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 },
    { 8, 8, 10, 12, 10, 8, 8 }
};

GXRenderModeObj GXEurgb60Hz480ProgAa = {
    22,
    640, 242, 480,
    40, 0,
    640, 480,
    0,
    0,
    1,
    { 3, 2, 9, 6, 3, 10, 3, 2, 9, 6, 3, 10, 9, 2, 3, 6, 9, 10, 9, 2, 3, 6, 9, 10 },
    { 4, 8, 12, 16, 12, 8, 4 }
};

GXRenderModeObj GXRmHW = {1, 320, 240, 240, 40, 0, 640, 480, 0, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 } };

void GXAdjustForOverscan(const GXRenderModeObj* rmin, GXRenderModeObj* rmout, u16 hor, u16 ver) {
    u16 hor2 = hor * 2;
    u16 ver2 = ver * 2;
    u32 verf;
    u32 mode;

    if (rmin != rmout) {
        *rmout = *rmin;
    }

    mode = rmin->viTVmode & 3;
    rmout->fbWidth = rmin->fbWidth - hor2;
    verf = (ver2 * rmin->efbHeight) / (u32)rmin->xfbHeight;
    rmout->efbHeight = rmin->efbHeight - verf;
    if (rmin->xFBmode == VI_XFBMODE_SF && mode == 0) {
        rmout->xfbHeight = rmin->xfbHeight - ver2 / 2;
    } else {
        rmout->xfbHeight = rmin->xfbHeight - ver2;
    }

    rmout->viWidth = rmin->viWidth - hor2;

    if (mode == 1) {
        rmout->viHeight = rmin->viHeight - (ver2 * 2);
    } else {
        rmout->viHeight = rmin->viHeight - ver2;
    }
 
    rmout->viXOrigin = rmin->viXOrigin + hor;
    rmout->viYOrigin = rmin->viYOrigin + ver;
}

void GXSetDispCopySrc(u16 left, u16 top, u16 wd, u16 ht) {
    CHECK_GXBEGIN(1510, "GXSetDispCopySrc");

    __GXData->cpDispSrc = 0;
    
    SET_REG_FIELD(1513, __GXData->cpDispSrc, 10,  0, left);
    SET_REG_FIELD(1514, __GXData->cpDispSrc, 10, 10, top);
    SET_REG_FIELD(1514, __GXData->cpDispSrc,  8, 24, 0x49);

    __GXData->cpDispSize = 0;
    SET_REG_FIELD(1518, __GXData->cpDispSize, 10,  0, wd - 1);
    SET_REG_FIELD(1519, __GXData->cpDispSize, 10, 10, ht - 1);
    SET_REG_FIELD(1519, __GXData->cpDispSize,  8, 24, 0x4A);
}


void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht) {
    CHECK_GXBEGIN(1538, "GXSetTexCopySrc");

    __GXData->cpTexSrc = 0;
    SET_REG_FIELD(1541, __GXData->cpTexSrc, 10,  0, left);
    SET_REG_FIELD(1542, __GXData->cpTexSrc, 10, 10, top);
    SET_REG_FIELD(1542, __GXData->cpTexSrc,  8, 24, 0x49);

    __GXData->cpTexSize = 0;
    SET_REG_FIELD(1546, __GXData->cpTexSize, 10,  0, wd - 1);
    SET_REG_FIELD(1547, __GXData->cpTexSize, 10, 10, ht - 1);
    SET_REG_FIELD(1547, __GXData->cpTexSize,  8, 24, 0x4A);
}

void GXSetDispCopyDst(u16 wd, u16 ht) {
    u16 stride;

    ASSERTMSGLINE(1568, (wd & 15) == 0, "GXSetDispCopyDst: Width must be a multiple of 16");
    CHECK_GXBEGIN(1569, "GXSetDispCopyDst");

    stride = (int)wd * 2;
    __GXData->cpDispStride = 0;
    SET_REG_FIELD(1575, __GXData->cpDispStride, 10,  0, (stride >> 5) );
    SET_REG_FIELD(1575, __GXData->cpDispStride,  8, 24, 0x4D);
}

void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap) {
    u32 rowTiles;
    u32 colTiles;
    u32 cmpTiles;
    u32 peTexFmt;
    u32 peTexFmtH;

    CHECK_GXBEGIN(1602, "GXSetTexCopyDst");

    __GXData->cpTexZ = 0;
    peTexFmt = fmt & 0xF;
    ASSERTMSGLINEV(1633, peTexFmt < 13, "%s: invalid texture format", "GXSetTexCopyDst");

    if (fmt == GX_TF_Z16) {
        peTexFmt = 0xB;
    }

    switch (fmt) {
    case GX_TF_I4:
    case GX_TF_I8:
    case GX_TF_IA4:
    case GX_TF_IA8:
    case GX_CTF_YUVA8:
        SET_REG_FIELD(0, __GXData->cpTex, 2, 15, 3);
        break;
    default:
        SET_REG_FIELD(0, __GXData->cpTex, 2, 15, 2);
        break;
    }

    __GXData->cpTexZ = (fmt & _GX_TF_ZTF) == _GX_TF_ZTF;
    peTexFmtH = (peTexFmt & 8) >> 3;
    SET_REG_FIELD(1656, __GXData->cpTex, 1, 3, peTexFmtH);
    peTexFmt = peTexFmt & 7;
    __GetImageTileCount(fmt, wd, ht, &rowTiles, &colTiles, &cmpTiles);

    __GXData->cpTexStride = 0;
    SET_REG_FIELD(1665, __GXData->cpTexStride, 10,  0, rowTiles * cmpTiles);
    SET_REG_FIELD(1667, __GXData->cpTexStride,  8, 24, 0x4D);
    SET_REG_FIELD(1667, __GXData->cpTex, 1, 9, mipmap);
    SET_REG_FIELD(1668, __GXData->cpTex, 3, 4, peTexFmt);
}

void GXSetDispCopyFrame2Field(GXCopyMode mode) {
    CHECK_GXBEGIN(1685, "GXSetDispCopyFrame2Field");
    SET_REG_FIELD(1686, __GXData->cpDisp, 2, 12, mode);
    SET_REG_FIELD(1686, __GXData->cpTex, 2, 12, 0);
}

void GXSetCopyClamp(GXFBClamp clamp) {
    u8 clmpB;
    u8 clmpT;

    CHECK_GXBEGIN(1706, "GXSetCopyClamp");

    clmpT = (clamp & GX_CLAMP_TOP) == 1;
    clmpB = (clamp & GX_CLAMP_BOTTOM) == 2;

    SET_REG_FIELD(1710, __GXData->cpDisp, 1, 0, clmpT);
    SET_REG_FIELD(1711, __GXData->cpDisp, 1, 1, clmpB);

    SET_REG_FIELD(1713, __GXData->cpTex, 1, 0, clmpT);
    SET_REG_FIELD(1714, __GXData->cpTex, 1, 1, clmpB);
}

static u32 __GXGetNumXfbLines(u32 efbHt, u32 iScale) {
    u32 count;
    u32 realHt;
    u32 iScaleD;

    count = (efbHt - 1) * 0x100;
    realHt = (count / iScale) + 1;

    iScaleD = iScale;

    if (iScaleD > 0x80 && iScaleD < 0x100) {
        while (iScaleD % 2 == 0) {
            iScaleD /= 2;
        }

        if (efbHt % iScaleD == 0) {
            realHt++;
        }
    }

    if (realHt > 0x400) {
        realHt = 0x400;
    }

    return realHt;
}

u16 GXGetNumXfbLines(u16 efbHeight, f32 yScale) {
    u32 iScale;
    ASSERTMSGLINE(1761, yScale >= 1.0f, "GXGetNumXfbLines: Vertical scale must be >= 1.0");

    iScale = (u32)(256.0f / yScale) & 0x1FF;
    return __GXGetNumXfbLines(efbHeight, iScale);
}

f32 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight) {
    f32 fScale;
    f32 yScale;
    u32 iScale;
    u32 tgtHt;
    u32 realHt;

    ASSERTMSGLINE(1785, xfbHeight <= 1024, "GXGetYScaleFactor: Display copy only supports up to 1024 lines.\n");
    ASSERTMSGLINE(1787, efbHeight <= xfbHeight, "GXGetYScaleFactor: EFB height should not be greater than XFB height.\n");

    tgtHt = xfbHeight;
    yScale = (f32)xfbHeight / (f32)efbHeight;
    iScale = (u32)(256.0f / yScale) & 0x1FF;
    realHt = __GXGetNumXfbLines(efbHeight, iScale);

    while (realHt > xfbHeight) {
        tgtHt--;
        yScale = (f32)tgtHt / (f32)efbHeight;
        iScale = (u32)(256.0f / yScale) & 0x1FF;
        realHt = __GXGetNumXfbLines(efbHeight, iScale);
    }

    fScale = yScale;
    while (realHt < xfbHeight) {
        fScale = yScale;
        tgtHt++;
        yScale = (f32)tgtHt / (f32)efbHeight;
        iScale = (u32)(256.0f / yScale) & 0x1FF;
        realHt = __GXGetNumXfbLines(efbHeight, iScale);
    }

    return fScale;
}

u32 GXSetDispCopyYScale(f32 vscale) {
    u8 enable;
    u32 iScale;
    u32 ht;
    u32 reg;

    CHECK_GXBEGIN(1832, "GXSetDispCopyYScale");

    ASSERTMSGLINE(1834, vscale >= 1.0f, "GXSetDispCopyYScale: Vertical scale must be >= 1.0");

    iScale = (u32) (256.0f / vscale) & 0x1FF;
    enable = (iScale != 256);

    reg = 0;
    SET_REG_FIELD(1841, reg, 9, 0, iScale);
    SET_REG_FIELD(1841, reg, 8, 24, 0x4E);
    GX_WRITE_RAS_REG(reg);
    __GXData->bpSentNot = 0;
    SET_REG_FIELD(1846, __GXData->cpDisp, 1, 10, enable);
    ht = (u32)GET_REG_FIELD(__GXData->cpDispSize, 10, 10) + 1;
    return __GXGetNumXfbLines(ht, iScale);
}

void GXSetCopyClear(GXColor clear_clr, u32 clear_z) {
    u32 reg;

    CHECK_GXBEGIN(1871, "GXSetCopyClear");
    ASSERTMSGLINE(1873, clear_z <= 0xFFFFFF, "GXSetCopyClear: Z clear value is out of range");

    reg = 0;
    SET_REG_FIELD(1876, reg, 8, 0, clear_clr.r);
    SET_REG_FIELD(1877, reg, 8, 8, clear_clr.a);
    SET_REG_FIELD(1877, reg, 8, 24, 0x4F);
    GX_WRITE_RAS_REG(reg);

    reg = 0;
    SET_REG_FIELD(1882, reg, 8, 0, clear_clr.b);
    SET_REG_FIELD(1883, reg, 8, 8, clear_clr.g);
    SET_REG_FIELD(1883, reg, 8, 24, 0x50);
    GX_WRITE_RAS_REG(reg);

    reg = 0;
    SET_REG_FIELD(1888, reg, 24, 0, clear_z);
    SET_REG_FIELD(1888, reg, 8, 24, 0x51);
    GX_WRITE_RAS_REG(reg);
    __GXData->bpSentNot = 0;
}

void GXSetCopyFilter(GXBool aa, const u8 sample_pattern[12][2], GXBool vf, const u8 vfilter[7]) {
    u32 msLoc[4];
    u32 coeff0;
    u32 coeff1;

    CHECK_GXBEGIN(1916, "GXSetCopyFilter");

    if (aa != 0) {
        msLoc[0] = 0;
        SET_REG_FIELD(1920, msLoc[0], 4,  0, sample_pattern[0][0]);
        SET_REG_FIELD(1921, msLoc[0], 4,  4, sample_pattern[0][1]);
        SET_REG_FIELD(1922, msLoc[0], 4,  8, sample_pattern[1][0]);
        SET_REG_FIELD(1923, msLoc[0], 4, 12, sample_pattern[1][1]);
        SET_REG_FIELD(1924, msLoc[0], 4, 16, sample_pattern[2][0]);
        SET_REG_FIELD(1925, msLoc[0], 4, 20, sample_pattern[2][1]);
        SET_REG_FIELD(1926, msLoc[0], 8, 24, 1);

        msLoc[1] = 0;
        SET_REG_FIELD(1929, msLoc[1], 4,  0, sample_pattern[3][0]);
        SET_REG_FIELD(1930, msLoc[1], 4,  4, sample_pattern[3][1]);
        SET_REG_FIELD(1931, msLoc[1], 4,  8, sample_pattern[4][0]);
        SET_REG_FIELD(1932, msLoc[1], 4, 12, sample_pattern[4][1]);
        SET_REG_FIELD(1933, msLoc[1], 4, 16, sample_pattern[5][0]);
        SET_REG_FIELD(1934, msLoc[1], 4, 20, sample_pattern[5][1]);
        SET_REG_FIELD(1935, msLoc[1], 8, 24, 2);

        msLoc[2] = 0;
        SET_REG_FIELD(1938, msLoc[2], 4,  0, sample_pattern[6][0]);
        SET_REG_FIELD(1939, msLoc[2], 4,  4, sample_pattern[6][1]);
        SET_REG_FIELD(1940, msLoc[2], 4,  8, sample_pattern[7][0]);
        SET_REG_FIELD(1941, msLoc[2], 4, 12, sample_pattern[7][1]);
        SET_REG_FIELD(1942, msLoc[2], 4, 16, sample_pattern[8][0]);
        SET_REG_FIELD(1943, msLoc[2], 4, 20, sample_pattern[8][1]);
        SET_REG_FIELD(1944, msLoc[2], 8, 24, 3);

        msLoc[3] = 0;
        SET_REG_FIELD(1947, msLoc[3], 4,  0, sample_pattern[9][0]);
        SET_REG_FIELD(1948, msLoc[3], 4,  4, sample_pattern[9][1]);
        SET_REG_FIELD(1949, msLoc[3], 4,  8, sample_pattern[10][0]);
        SET_REG_FIELD(1950, msLoc[3], 4, 12, sample_pattern[10][1]);
        SET_REG_FIELD(1951, msLoc[3], 4, 16, sample_pattern[11][0]);
        SET_REG_FIELD(1952, msLoc[3], 4, 20, sample_pattern[11][1]);
        SET_REG_FIELD(1953, msLoc[3], 8, 24, 4);
    } else {
        msLoc[0] = 0x01666666;
        msLoc[1] = 0x02666666;
        msLoc[2] = 0x03666666;
        msLoc[3] = 0x04666666;
    }

    GX_WRITE_RAS_REG(msLoc[0]);
    GX_WRITE_RAS_REG(msLoc[1]);
    GX_WRITE_RAS_REG(msLoc[2]);
    GX_WRITE_RAS_REG(msLoc[3]);

    coeff0 = 0;
    SET_REG_FIELD(0, coeff0, 8, 24, 0x53);
    coeff1 = 0;
    SET_REG_FIELD(0, coeff1, 8, 24, 0x54);

    if (vf != 0) {
        SET_REG_FIELD(1977, coeff0, 6,  0, vfilter[0]);
        SET_REG_FIELD(1978, coeff0, 6,  6, vfilter[1]);
        SET_REG_FIELD(1979, coeff0, 6, 12, vfilter[2]);
        SET_REG_FIELD(1980, coeff0, 6, 18, vfilter[3]);
        SET_REG_FIELD(1981, coeff1, 6,  0, vfilter[4]);
        SET_REG_FIELD(1982, coeff1, 6,  6, vfilter[5]);
        SET_REG_FIELD(1983, coeff1, 6, 12, vfilter[6]);
    } else {
        SET_REG_FIELD(0, coeff0, 6,  0, 0);
        SET_REG_FIELD(0, coeff0, 6,  6, 0);
        SET_REG_FIELD(0, coeff0, 6, 12, 21);
        SET_REG_FIELD(0, coeff0, 6, 18, 22);
        SET_REG_FIELD(0, coeff1, 6,  0, 21);
        SET_REG_FIELD(0, coeff1, 6,  6,  0);
        SET_REG_FIELD(0, coeff1, 6, 12,  0);
    }

    GX_WRITE_RAS_REG(coeff0);
    GX_WRITE_RAS_REG(coeff1);
    __GXData->bpSentNot = 0;
}

void GXSetDispCopyGamma(GXGamma gamma) {
    CHECK_GXBEGIN(2016, "GXSetDispCopyGamma");
    SET_REG_FIELD(2017, __GXData->cpDisp, 2, 7, gamma);
}

#if DEBUG
static void __GXVerifCopy(void* dest, u8 clear) {
    u8 clmpT;
    u8 clmpB;
    u32 x0;
    u32 y0;
    u32 dx;
    u32 dy;

    CHECK_GXBEGIN(2037, "GXCopyDisp");

    clmpT = GET_REG_FIELD(__GXData->cpDisp, 1, 0);
    clmpB = (u32)GET_REG_FIELD(__GXData->cpDisp, 1, 1);
    x0 = GET_REG_FIELD(__GXData->cpDispSrc, 10, 0);
    dx = GET_REG_FIELD(__GXData->cpDispSize, 10, 0) + 1;
    y0 = GET_REG_FIELD(__GXData->cpDispSrc, 10, 10);
    dy = GET_REG_FIELD(__GXData->cpDispSize, 10, 10) + 1;

    ASSERTMSGLINE(2047, clmpT || y0 != 0, "GXCopy: Have to set GX_CLAMP_TOP if source top == 0");
    ASSERTMSGLINE(2049, clmpB || y0 + dy <= 528, "GXCopy: Have to set GX_CLAMP_BOTTOM if source bottom > 528");
    ASSERTMSGLINE(2054, (__GXData->peCtrl & 7) != 3 || clear == 0, "GXCopy: Can not do clear while pixel type is Z");
    
    if ((u32) (__GXData->peCtrl & 7) == 5) {
        ASSERTMSGLINE(2060, clear == 0, "GXCopy: Can not clear YUV framebuffer");
        ASSERTMSGLINE(2062, (x0 & 3) == 0, "GXCopy: Source x is not multiple of 4 for YUV copy");
        ASSERTMSGLINE(2064, (y0 & 3) == 0, "GXCopy: Source y is not multiple of 4 for YUV copy");
        ASSERTMSGLINE(2066, (dx & 3) == 0, "GXCopy: Source width is not multiple of 4 for YUV copy");
        ASSERTMSGLINE(2068, (dy & 3) == 0, "GXCopy: Source height is not multiple of 4 for YUV copy");
    } else {
        ASSERTMSGLINE(2072, (x0 & 1) == 0, "GXCopy: Source x is not multiple of 2 for RGB copy");
        ASSERTMSGLINE(2074, (y0 & 1) == 0, "GXCopy: Source y is not multiple of 2 for RGB copy");
        ASSERTMSGLINE(2076, (dx & 1) == 0, "GXCopy: Source width is not multiple of 2 for RGB copy");
        ASSERTMSGLINE(2078, (dy & 1) == 0, "GXCopy: Source height is not multiple of 2 for RGB copy");
    }

    ASSERTMSGLINE(2082, ((u32)dest & 0x1F) == 0, "GXCopy: Display destination address not 32B aligned");
}
#endif

void GXCopyDisp(void* dest, GXBool clear) {
    u32 reg;
    u32 tempPeCtrl;
    u32 phyAddr;
    u8 changePeCtrl;

    CHECK_GXBEGIN(2108, "GXCopyDisp");

#if DEBUG
    __GXVerifCopy(dest, clear);
#endif

    if (clear) {
        reg = __GXData->zmode;
        SET_REG_FIELD(0, reg, 1, 0, 1);
        SET_REG_FIELD(0, reg, 3, 1, 7);
        GX_WRITE_RAS_REG(reg);

        reg = __GXData->cmode0;
        SET_REG_FIELD(0, reg, 1, 0, 0);
        SET_REG_FIELD(0, reg, 1, 1, 0);
        GX_WRITE_RAS_REG(reg);
    }

    changePeCtrl = FALSE;

    if ((clear || (u32)GET_REG_FIELD(__GXData->peCtrl, 3, 0) == 3) && (u32)GET_REG_FIELD(__GXData->peCtrl, 1, 6) == 1) {
        changePeCtrl = TRUE;
        tempPeCtrl = __GXData->peCtrl;
        SET_REG_FIELD(0, tempPeCtrl, 1, 6, 0);
        GX_WRITE_RAS_REG(tempPeCtrl);
    }

    GX_WRITE_RAS_REG(__GXData->cpDispSrc);
    GX_WRITE_RAS_REG(__GXData->cpDispSize);
    GX_WRITE_RAS_REG(__GXData->cpDispStride);

    phyAddr = (u32)dest & 0x3FFFFFFF;
    reg = 0;
    SET_REG_FIELD(2147, reg, 24, 0, phyAddr >> 5);
    SET_REG_FIELD(2147, reg, 8, 24, 0x4B);
    GX_WRITE_RAS_REG(reg);

    SET_REG_FIELD(2151, __GXData->cpDisp, 1, 11, clear);
    SET_REG_FIELD(2151, __GXData->cpDisp, 1, 14, 1);
    SET_REG_FIELD(2151, __GXData->cpDisp, 8, 24, 0x52);
    GX_WRITE_RAS_REG(__GXData->cpDisp);

    if (clear) {
        GX_WRITE_RAS_REG(__GXData->zmode);
        GX_WRITE_RAS_REG(__GXData->cmode0);
    }

    if (changePeCtrl) {
        GX_WRITE_RAS_REG(__GXData->peCtrl);
    }

    __GXData->bpSentNot = 0;
}

void GXCopyTex(void* dest, GXBool clear) {
    u32 reg;
    u32 tempPeCtrl;
    u32 phyAddr;
    u8 changePeCtrl;

    CHECK_GXBEGIN(2191, "GXCopyTex");

#if DEBUG
    __GXVerifCopy(dest, clear);
#endif
    if (clear) {
        reg = __GXData->zmode;
        SET_REG_FIELD(0, reg, 1, 0, 1);
        SET_REG_FIELD(0, reg, 3, 1, 7);
        GX_WRITE_RAS_REG(reg);

        reg = __GXData->cmode0;
        SET_REG_FIELD(0, reg, 1, 0, 0);
        SET_REG_FIELD(0, reg, 1, 1, 0);
        GX_WRITE_RAS_REG(reg);
    }

    changePeCtrl = 0;
    tempPeCtrl = __GXData->peCtrl;

    if (__GXData->cpTexZ && ((tempPeCtrl & 7) != 3)) {
        changePeCtrl = 1;
        SET_REG_FIELD(0, tempPeCtrl, 3, 0, 3);
    }

    if ((clear || ((u32) (tempPeCtrl & 7) == 3)) && ((u32) ((tempPeCtrl & 0x40) >> 6) == 1)) {
        changePeCtrl = 1;
        SET_REG_FIELD(0, tempPeCtrl, 1, 6, 0);
    }

    if (changePeCtrl) {
        GX_WRITE_RAS_REG(tempPeCtrl);
    }

    GX_WRITE_RAS_REG(__GXData->cpTexSrc);
    GX_WRITE_RAS_REG(__GXData->cpTexSize);
    GX_WRITE_RAS_REG(__GXData->cpTexStride);

    phyAddr = (u32)dest & 0x3FFFFFFF;
    reg = 0;
    SET_REG_FIELD(2240, reg, 24, 0, phyAddr >> 5);
    SET_REG_FIELD(2240, reg, 8, 24, 0x4B);
    GX_WRITE_RAS_REG(reg);

    SET_REG_FIELD(2244, __GXData->cpTex, 1, 11, clear);
    SET_REG_FIELD(2244, __GXData->cpTex, 1, 14, 0);
    SET_REG_FIELD(2244, __GXData->cpTex, 8, 24, 0x52);
    GX_WRITE_RAS_REG(__GXData->cpTex);

    if (clear) {
        GX_WRITE_RAS_REG(__GXData->zmode);
        GX_WRITE_RAS_REG(__GXData->cmode0);
    }

    if (changePeCtrl) {
        GX_WRITE_RAS_REG(__GXData->peCtrl);
    }

    __GXData->bpSentNot = 0;
}

void GXClearBoundingBox(void) {
    u32 reg;

    CHECK_GXBEGIN(2278, "GXClearBoundingBox");
    reg = 0x550003FF;
    GX_WRITE_RAS_REG(reg);
    reg = 0x560003FF;
    GX_WRITE_RAS_REG(reg);
    __GXData->bpSentNot = 0;
}

void GXReadBoundingBox(u16* left, u16* top, u16* right, u16* bottom) {
    *left = GX_GET_PE_REG(8);
    *top = GX_GET_PE_REG(10);
    *right = GX_GET_PE_REG(9);
    *bottom = GX_GET_PE_REG(11);
}
