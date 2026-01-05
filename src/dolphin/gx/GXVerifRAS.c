#if DEBUG

#include <cstdio>

#include <dolphin/gx.h>

#include "__gx.h"

static char __data_0[] = "RGB multisample";
static char _305[] = "GX_TEVPREV(color)";
static char _306[] = "GX_TEVPREV(alpha)";
static char _307[] = "GX_TEVREG0(color)";
static char _308[] = "GX_TEVREG0(alpha)";
static char _309[] = "GX_TEVREG1(color)";
static char _310[] = "GX_TEVREG1(alpha)";
static char _311[] = "GX_TEVREG2(color)";
static char _312[] = "GX_TEVREG2(alpha)";

static char* TevRegNames[8] = {0};

#define SOME_GET_REG_MACRO(reg, size, shift) ((u32)((reg) << (shift)) & ((1 << (size)) - 2))
#define SOME_GET_REG_MACRO2(reg, size, shift) ((u32)((reg) >> (shift)) & ((1 << (size)) - 2))

void __GXVerifySU(void) {
    s32 scis_l;
    s32 scis_r;
    s32 scis_t;
    s32 scis_b;

    scis_l = (u32)GET_REG_FIELD(__gxVerif->rasRegs[32], 11, 12);
    scis_t = (u32)GET_REG_FIELD(__gxVerif->rasRegs[32], 11, 0);
    scis_r = (u32)GET_REG_FIELD(__gxVerif->rasRegs[33], 11, 12);
    scis_b = (u32)GET_REG_FIELD(__gxVerif->rasRegs[33], 11, 0);

    scis_l = scis_l - (u32)SOME_GET_REG_MACRO(__gxVerif->rasRegs[89], 11, 1);
    scis_r = scis_r - (u32)SOME_GET_REG_MACRO(__gxVerif->rasRegs[89], 11, 1);
    scis_t = scis_t - (u32)SOME_GET_REG_MACRO2(__gxVerif->rasRegs[89], 11, 9);
    scis_b = scis_b - (u32)SOME_GET_REG_MACRO2(__gxVerif->rasRegs[89], 11, 9);
    
    if (scis_l < 0 && __gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_SCISSOR_RECT_LEFT]) {
        __GX_WARNF(GXWARN_SCISSOR_RECT_LEFT, 0);
    }

    if (scis_t < 0 && __gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_SCISSOR_RECT_TOP]) {
        __GX_WARNF(GXWARN_SCISSOR_RECT_TOP, 0);
    }

    switch (__gxVerif->rasRegs[67] & 7) {
    case 4:
    case 5:
        if (scis_r > 719 && __gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_SCISSOR_RECT_RIGHT]) {
            __GX_WARNF(GXWARN_SCISSOR_RECT_RIGHT, 719, "YUV");
        }

        if (scis_b > 575 && __gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_SCISSOR_RECT_BOT]) {
            __GX_WARNF(GXWARN_SCISSOR_RECT_BOT, 575, "YUV");
        }
        break;
    case 0:
    case 1:
    case 3:
        if (scis_r > 639 && __gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_SCISSOR_RECT_RIGHT]) {
            __GX_WARNF(GXWARN_SCISSOR_RECT_RIGHT, 639, "RGB");
        }

        if (scis_b > 527 && __gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_SCISSOR_RECT_BOT]) {
            __GX_WARNF(GXWARN_SCISSOR_RECT_BOT, 527, "RGB");
        }
        break;
    case 2:
        if (scis_r > 639 && __gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_SCISSOR_RECT_RIGHT]) {
            __GX_WARNF(GXWARN_SCISSOR_RECT_RIGHT, 639, __data_0);
        }

        if (scis_b > 263 && __gxVerif->verifyLevel >= __gxvWarnLev[GXWARN_SCISSOR_RECT_BOT]) {
            __GX_WARNF(GXWARN_SCISSOR_RECT_BOT, 263, __data_0);
        }
        break;
    }
}

void __GXVerifyBUMP(void) {
    u32 i;
    u32 nBmp;
    u32 nTev;
    u32 nTex;
    u32 matrix;

    nBmp = GET_REG_FIELD(__gxVerif->rasRegs[0], 3, 16);
    nTex = GET_REG_FIELD(__gxVerif->rasRegs[0], 4, 0);
    nTev = GET_REG_FIELD(__gxVerif->rasRegs[0], 4, 10) + 1;

    for (i = 0; i < nTev; i++) {
        matrix = GET_REG_FIELD(__gxVerif->rasRegs[16 + i], 4, 9);
        if (__gxVerif->verifyLevel >= GX_WARN_SEVERE) {
            if ((u32)(__gxVerif->rasRegs[16 + i] & 0xFF000000) + 0x01000000 == 0U && __gxVerif->verifyLevel >= __gxvWarnLev[7]) {
                sprintf(__gxvDummyStr, __gxvWarnings[7], i);
                __gxVerif->cb(__gxvWarnLev[7], 7U, __gxvDummyStr);
            }

            if ((GET_REG_FIELD(__gxVerif->rasRegs[16 + i], 2, 7) != 0 || matrix != 0)
             && GET_REG_FIELD(__gxVerif->rasRegs[16 + i], 2, 0) >= nBmp && __gxVerif->verifyLevel >= __gxvWarnLev[8]) {
                sprintf(__gxvDummyStr, __gxvWarnings[8], i);
                __gxVerif->cb(__gxvWarnLev[8], 8U, __gxvDummyStr);
            }

            if (matrix != 0) {
                matrix = (matrix & 3) - 1;
                if (((u32)(__gxVerif->rasRegs[(matrix * 3) + 6] & 0xFF000000) + 0x01000000 == 0
                 || (u32)(__gxVerif->rasRegs[(matrix * 3) + 7] & 0xFF000000) + 0x01000000 == 0U
                 || (u32)(__gxVerif->rasRegs[(matrix * 3) + 8] & 0xFF000000) + 0x01000000 == 0U) && __gxVerif->verifyLevel >= __gxvWarnLev[9]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[9], matrix, i);
                    __gxVerif->cb(__gxvWarnLev[9], 9U, __gxvDummyStr);
                }
            }
        }
    }

    if (__gxVerif->verifyLevel >= GX_WARN_SEVERE) {
        if (nBmp != 0 && (u32)(__gxVerif->rasRegs[0x27] & 0xFF000000) + 0x01000000 == 0 && __gxVerif->verifyLevel >= __gxvWarnLev[10]) {
            __gxVerif->cb(__gxvWarnLev[10], 0xAU, __gxvWarnings[10]);
        }

        if (nBmp != 0 && (u32)(__gxVerif->rasRegs[0x25] & 0xFF000000) + 0x01000000 == 0 && __gxVerif->verifyLevel >= __gxvWarnLev[11]) {
            sprintf(__gxvDummyStr, __gxvWarnings[11], 0U, 1);
            __gxVerif->cb(__gxvWarnLev[11], 0xBU, __gxvDummyStr);
        }

        if (nBmp > 2U && (u32)(__gxVerif->rasRegs[0x26] & 0xFF000000) + 0x01000000 == 0 && __gxVerif->verifyLevel >= __gxvWarnLev[11]) {
            sprintf(__gxvDummyStr, __gxvWarnings[11], 2U, 3);
            __gxVerif->cb(__gxvWarnLev[11], 0xBU, __gxvDummyStr);
        }

        if (nBmp != 0 && GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 3) >= nTex && __gxVerif->verifyLevel >= __gxvWarnLev[12]) {
            sprintf(__gxvDummyStr, __gxvWarnings[12], 0U);
            __gxVerif->cb(__gxvWarnLev[12], 0xCU, __gxvDummyStr);
        }

        if (nBmp > 1U && GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 9) >= nTex && __gxVerif->verifyLevel >= __gxvWarnLev[12]) {
            sprintf(__gxvDummyStr, __gxvWarnings[12], 1U);
            __gxVerif->cb(__gxvWarnLev[12], 0xCU, __gxvDummyStr);
        }

        if (nBmp > 2U && GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 15) >= nTex && __gxVerif->verifyLevel >= __gxvWarnLev[12]) {
            sprintf(__gxvDummyStr, __gxvWarnings[12], 2U);
            __gxVerif->cb(__gxvWarnLev[12], 0xCU, __gxvDummyStr);
        }

        if (nBmp > 3U && GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 21) >= nTex && __gxVerif->verifyLevel >= __gxvWarnLev[12]) {
            sprintf(__gxvDummyStr, __gxvWarnings[12], 3U);
            __gxVerif->cb(__gxvWarnLev[12], 0xCU, __gxvDummyStr);
        }

        if (nBmp != 0 && GET_REG_FIELD(__gxVerif->rasRegs[0x10], 1, 20) && __gxVerif->verifyLevel >= __gxvWarnLev[13]) {
            __gxVerif->cb(__gxvWarnLev[13], 0xDU, __gxvWarnings[13]);
        }

        if (nBmp != 0 && GET_REG_FIELD(__gxVerif->rasRegs[0x10], 2, 7) != 0 && __gxVerif->verifyLevel >= __gxvWarnLev[14]) {
            __gxVerif->cb(__gxvWarnLev[14], 0xEU, __gxvWarnings[14]);
        }

        if ((u32)(__gxVerif->rasRegs[0xF] & 0xFF000000) + 0x01000000 == 0 && (nTex != 0 || nBmp != 0) && __gxVerif->verifyLevel >= __gxvWarnLev[15]) {
            __gxVerif->cb(__gxvWarnLev[15], 0xFU, __gxvWarnings[15]);
        }
    }
}

#define SOMEINDEX(index) (index & 3) + ((index * 8) & ~0x1F)

#define MAX(a, b) ((a) > (b) ? (a) : (b))

void __GXVerifyTEX(void) {
    u32 i;
    u32 nBmp;
    u32 nTev;
    u32 nTex;
    u32 enabled;
    u32 texId;
    u32 direct[8];
    u32 indirect[8];
    u32 h2;
    u32 w2;
    u32 nlevels;

    nBmp = GET_REG_FIELD(__gxVerif->rasRegs[0], 3, 16);
    nTex = GET_REG_FIELD(__gxVerif->rasRegs[0], 4, 0);
    nTev = GET_REG_FIELD(__gxVerif->rasRegs[0], 4, 10) + 1;

    for (i = 0; i < 8; i++) {
        direct[i] = 0;
        indirect[i] = 0;
    }

    for (i = 0; i < nTev + nBmp; i++) {
        if (i < nTev) {
            if (__gxVerif->verifyLevel >= 1) {
                if ((__gxVerif->rasRegs[(i >> 1U) + 0x28] & 0xFF000000) + 0x01000000 == 0U && __gxVerif->verifyLevel >= __gxvWarnLev[16]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[16], i);
                    __gxVerif->cb(__gxvWarnLev[16], 16, __gxvDummyStr);
                }

                if (i & 1) {
                    enabled = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 1, 18);
                    if (enabled && (GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 3, 15) >= nTex) && __gxVerif->verifyLevel >= __gxvWarnLev[17]) {
                        sprintf(__gxvDummyStr, __gxvWarnings[17], i);
                        __gxVerif->cb(__gxvWarnLev[17], 17, __gxvDummyStr);
                    }
                    texId = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 3, 12);
                } else {
                    enabled = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 1, 6);
                    if (enabled && (GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 3, 3) >= nTex) && __gxVerif->verifyLevel >= __gxvWarnLev[17]) {
                        sprintf(__gxvDummyStr, __gxvWarnings[17], i);
                        __gxVerif->cb(__gxvWarnLev[17], 17, __gxvDummyStr);
                    }
                    texId = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 3, 0);
                }

                if (enabled) {
                    direct[texId] = 1;
                }
            }
        } else {
            enabled = 1;
            if ((i - nTev) == 0) {
                texId = GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 0);
            } else if ((i - nTev) == 1U) {
                texId = GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 6);
            } else if ((i - nTev) == 2U) {
                texId = GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 12);
            } else {
                texId = GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 18);
            }

            if (!indirect[texId] && direct[texId] && __gxVerif->verifyLevel >= __gxvWarnLev[18]) {
                sprintf(__gxvDummyStr, __gxvWarnings[18], texId);
                __gxVerif->cb(__gxvWarnLev[18], 18, __gxvDummyStr);
            }
            indirect[texId] = 1;
        }
    
        if (enabled) {
            if (__gxVerif->verifyLevel >= GX_WARN_SEVERE) {
                if (((u32)(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0
                 || (u32)(__gxVerif->rasRegs[0x84 + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0
                 || (u32)(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0
                 || (u32)(__gxVerif->rasRegs[0x8C + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0
                 || (u32)(__gxVerif->rasRegs[0x90 + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0) && __gxVerif->verifyLevel >= __gxvWarnLev[19]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[19], texId);
                    __gxVerif->cb(__gxvWarnLev[19], 19, __gxvDummyStr);
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x8C + SOMEINDEX(texId)], 1, 21) == 0
                 && (u32)(__gxVerif->rasRegs[0x94 + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0 && __gxVerif->verifyLevel >= __gxvWarnLev[20]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[20], texId);
                    __gxVerif->cb(__gxvWarnLev[20], 20, __gxvDummyStr);
                }

                if (((u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 8
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 9
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 10)
                 && (__gxVerif->rasRegs[0x98 + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0U && __gxVerif->verifyLevel >= __gxvWarnLev[21]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[21], texId);
                    __gxVerif->cb(__gxvWarnLev[21], 21, __gxvDummyStr);
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 0) + 1 == 0) {
                    w2 = 1;
                } else {
                    w2 = 1;
                    while (!(w2 & (GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 0) + 1))) {
                        w2 *= 2;
                    }
                    w2 = (GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 0) + 1) == w2;
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 10) + 1 == 0) {
                    h2 = 1;
                } else {
                    h2 = 1;
                    while (!(h2 & (GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 10) + 1))) {
                        h2 *= 2;
                    }
                    h2 = (GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 10) + 1) == h2;
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)] & 0xFFFF, 2, 5) && !w2 && __gxVerif->verifyLevel >= __gxvWarnLev[22]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[22], "Width", texId);
                    __gxVerif->cb(__gxvWarnLev[22], 22, __gxvDummyStr);
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)] & 0xFFFF, 2, 5) && !h2 && __gxVerif->verifyLevel >= __gxvWarnLev[22]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[22], "Height", texId);
                    __gxVerif->cb(__gxvWarnLev[22], 22, __gxvDummyStr);
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 2, 0) && !w2 && __gxVerif->verifyLevel >= __gxvWarnLev[23]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[23], "S", texId);
                    __gxVerif->cb(__gxvWarnLev[23], 23, __gxvDummyStr);
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 2, 2) && !h2 && __gxVerif->verifyLevel >= __gxvWarnLev[23]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[23], "T", texId);
                    __gxVerif->cb(__gxvWarnLev[23], 23, __gxvDummyStr);
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)] & 0xFFFF, 2, 5) != 0
                 && ((u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 8
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 9
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 10)
                 && (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 3, 5) != 1
                 && (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 3, 5) != 5 && __gxVerif->verifyLevel >= __gxvWarnLev[24]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[24], texId);
                    __gxVerif->cb(__gxvWarnLev[24], 24, __gxvDummyStr);
                }

                if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[0x84 + SOMEINDEX(texId)], 8, 0) > (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x84 + SOMEINDEX(texId)], 8, 8) && __gxVerif->verifyLevel >= __gxvWarnLev[25]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[25], texId);
                    __gxVerif->cb(__gxvWarnLev[25], 25, __gxvDummyStr);
                }

                for (
                  nlevels = 0;
                  (
                    MAX((u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 0) + 1,
                        (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 10) + 1) >> nlevels
                  ) != 0;
                  nlevels++) {
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x84 + SOMEINDEX(texId)], 8, 8) > (nlevels - 1) * 16 && __gxVerif->verifyLevel >= __gxvWarnLev[26]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[26], texId);
                    __gxVerif->cb(__gxvWarnLev[26], 26, __gxvDummyStr);
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 21) && GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 8) && __gxVerif->verifyLevel >= __gxvWarnLev[27]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[27], texId);
                    __gxVerif->cb(__gxvWarnLev[27], 27, __gxvDummyStr);
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 2, 19)
                 && (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)] & 0xFFFF, 2, 5) == 0
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 3, 5) != 6
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 4) != 1
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 8
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 9
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 10
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 8) != 0
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 21)) && __gxVerif->verifyLevel >= __gxvWarnLev[28]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[28], texId);
                    __gxVerif->cb(__gxvWarnLev[28], 28, __gxvDummyStr);
                }
            }

            if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 18) != 0) {
                if (((u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 3, 5) != 4 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 4) != 1) && __gxVerif->verifyLevel >= __gxvWarnLev[29]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[29], texId);
                    __gxVerif->cb(__gxvWarnLev[29], 29, __gxvDummyStr);
                }

                if ((!GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 17) || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) != 1 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 2, 19) != 0) && __gxVerif->verifyLevel >= __gxvWarnLev[30]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[30], texId);
                    __gxVerif->cb(__gxvWarnLev[30], 30, __gxvDummyStr);
                }
            }

            if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 17) != 0) {
                if (((u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 8 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 9 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 10) && __gxVerif->verifyLevel >= __gxvWarnLev[31]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[31], texId);
                    __gxVerif->cb(__gxvWarnLev[31], 31, __gxvDummyStr);
                }

                if ((!GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 18) || 0) && __gxVerif->verifyLevel >= __gxvWarnLev[30]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[30], texId);
                    __gxVerif->cb(__gxvWarnLev[30], 30, __gxvDummyStr);
                }
            }
        }
    }
}

#if DEBUG
static char _521[] = "A";
static char _522[] = "B";
static char _523[] = "C";
static char _524[] = "D";
asm void __GXVerifyTEV(void) {
    nofralloc
#include "../../nonmatchings/__GXVerifyTEV.s"
}
#pragma peephole on
#else
void __GXVerifyTEV(void) {
    u32 i; // r31
    u32 nTev; // r29
    u32 nCol; // r28
    u32 enabled; // r30
    u32 color; // r27
    u32 Clh[4]; // r1+0x38
    u32 Alh[4]; // r1+0x28
    u32 Cwritten[4]; // r1+0x18
    u32 Awritten[4]; // r1+0x8

    nTev = GET_REG_FIELD(__gxVerif->rasRegs[0], 4, 10) + 1;
    nCol = GET_REG_FIELD(__gxVerif->rasRegs[0], 3, 4);
    nCol;

    for (i = 0; i < 4; i++) {
        Clh[i] = 0;
        Alh[i] = 0;
        Cwritten[i] = 0;
        Awritten[i] = 0;
    }

    for (i = 0; i < nTev; i++) {
        if (__gxVerif->verifyLevel >= GX_WARN_SEVERE
         && (((u32) ((__gxVerif->rasRegs[(i * 2) + 0xC0] & 0xFF000000) + 0x01000000) == 0U) || ((u32) ((__gxVerif->rasRegs[(i * 2) + 0xC1] & 0xFF000000) + 0x01000000) == 0U))) {
            sprintf(__gxvDummyStr, __gxvWarnings[32], i);
            __gxVerif->cb(1, 0x20U, __gxvDummyStr);
        }

        if (i & 1) {
            color = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 3, 19);
        } else {
            color = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 3, 7);
        }

        if (__gxVerif->verifyLevel >= GX_WARN_MEDIUM && ((color == 0 && nCol < 1) || (color == 1 && nCol < 2))) {
            sprintf(__gxvDummyStr, __gxvWarnings[33], i);
            __gxVerif->cb(1, 0x21U, __gxvDummyStr);
        }

        if (i & 1) {
            enabled = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 1, 18);
        } else {
            enabled = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 1, 6);
        }

        if (__gxVerif->verifyLevel >= GX_WARN_SEVERE) {
            if (!enabled && ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 12) == 8 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 12) == 9)) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x22], "A", i);
                __gxVerif->cb(1, 0x22U, __gxvDummyStr);
            }

            if (!enabled && ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 8) == 8 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 8) == 9)) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x22], "B", i);
                __gxVerif->cb(1, 0x22U, __gxvDummyStr);
            }

            if (!enabled && ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 4) == 8 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 4) == 9)) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x22], "C", i);
                __gxVerif->cb(1, 0x22U, __gxvDummyStr);
            }

            if (!enabled && ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 0) == 8 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 0) == 9)) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x22], "D", i);
                __gxVerif->cb(1, 0x22U, __gxvDummyStr);
            }
        
            if (!enabled && (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 13) == 4) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x23], "A", i);
                __gxVerif->cb(1, 0x23U, __gxvDummyStr);
            }

            if (!enabled && (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 10) == 4) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x23], "B", i);
                __gxVerif->cb(1, 0x23U, __gxvDummyStr);
            }

            if (!enabled && (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7) == 4) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x23], "C", i);
                __gxVerif->cb(1, 0x23U, __gxvDummyStr);
            }

            if (!enabled && (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 4) == 4) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x23], "D", i);
                __gxVerif->cb(1, 0x23U, __gxvDummyStr);
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 12) <= 7 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 12) + 0xE1] & 0xFF000000) + 0x01000000) == 0U) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 12) ? !Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 13)] : !Cwritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 13)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x24], "A", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 0xCU) ? "alpha" : "color", (__gxVerif->rasRegs[(i * 2) + 0xC0] >> 0xDU) & 7);
                    __gxVerif->cb(1, 0x24U, __gxvDummyStr);
                }
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 8) <= 7 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 8) + 0xE1] & 0xFF000000) + 0x01000000) == 0U) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 8) ? !Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 9)] : !Cwritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 9)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x24], "B", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 8U) ? "alpha" : "color", (__gxVerif->rasRegs[(i * 2) + 0xC0] >> 9U) & 7);
                    __gxVerif->cb(1, 0x24U, __gxvDummyStr);
                }
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 4) <= 7 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 4) + 0xE1] & 0xFF000000) + 0x01000000) == 0U) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 4) ? !Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 5)] : !Cwritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 5)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x24], "C", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 4U) ? "alpha" : "color", (__gxVerif->rasRegs[(i * 2) + 0xC0] >> 5U) & 7);
                    __gxVerif->cb(1, 0x24U, __gxvDummyStr);
                }
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 0) <= 7 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 0) + 0xE1] & 0xFF000000) + 0x01000000) == 0U) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 0) ? !Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 1)] : !Cwritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 1)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x24], "D", i, (__gxVerif->rasRegs[(i * 2) + 0xC0] & 1) ? "alpha" : "color", (__gxVerif->rasRegs[(i * 2) + 0xC0] >> 1U) & 7);
                    __gxVerif->cb(1, 0x24U, __gxvDummyStr);
                }
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 13) <= 3 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 4, 14) + 0xE0] & 0xFF000000) + 0x01000000) == 0U && Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 13)] == 0U) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x25], "A", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 13));
                __gxVerif->cb(1, 0x25U, __gxvDummyStr);
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 10) <= 3 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 11) + 0xE0] & 0xFF000000) + 0x01000000) == 0U && Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 10)] == 0U) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x25], "B", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 10));
                __gxVerif->cb(1, 0x25U, __gxvDummyStr);
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7) <= 3 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 8) + 0xE0] & 0xFF000000) + 0x01000000) == 0U && Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7)] == 0U) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x25], "C", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7));
                __gxVerif->cb(1, 0x25U, __gxvDummyStr);
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 4) <= 3 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 3) + 0xE0] & 0xFF000000) + 0x01000000) == 0U && Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 4)] == 0U) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x25], "D", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 4));
                __gxVerif->cb(1, 0x25U, __gxvDummyStr);
            }
        }

        if (__gxVerif->verifyLevel >= GX_WARN_ALL) {
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 12) <= 7) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 12) ? Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 13)] : Clh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 13)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x26], "A", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 12) ? "alpha" : "color", GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 13));
                    __gxVerif->cb(3, 0x26U, __gxvDummyStr);
                }
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 8) <= 7) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 8) ? Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 9)] : Clh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 9)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x26], "B", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 8) ? "alpha" : "color", GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 9));
                    __gxVerif->cb(3, 0x26U, __gxvDummyStr);
                }
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 4) <= 7) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 4) ? Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 5)] : Clh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 5)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x26], "C", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 4) ? "alpha" : "color", GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 5));
                    __gxVerif->cb(3, 0x26U, __gxvDummyStr);
                }
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 13) <= 3 && (u32)Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 13)] != 0) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x27], "A", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 0xDU));
                __gxVerif->cb(3, 0x27U, __gxvDummyStr);
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 10) <= 3 && (u32)Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 10)] != 0) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x27], "B", i, ((__gxVerif->rasRegs[(i * 2) + 0xC1] >> 0xAU) & 7));
                __gxVerif->cb(3, 0x27U, __gxvDummyStr);
            }

            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7) <= 3 && (u32)Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7)] != 0) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x27], "C", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7U));
                __gxVerif->cb(3, 0x27U, __gxvDummyStr);
            }
        }
        Cwritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 2, 22)] = 1;
        Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 2, 22)] = 1;
        Clh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 2, 22)] = (!GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 2, 0) && !GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 19));
        Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 2, 22)] = (!GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 2, 0) && !GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 1, 19));
    }

    for (i = 0; i < 4; i++) {
        if (Cwritten[i] != 0U) {
            __gxVerif->rasRegs[(i * 2) + 0xE1] = (__gxVerif->rasRegs[(i * 2) + 0xE1] & 0xFFFFFF) | 0xFF000000;
        }
        if (Awritten[i] != 0U) {
            __gxVerif->rasRegs[(i * 2) + 0xE0] = (__gxVerif->rasRegs[(i * 2) + 0xE0] & 0xFFFFFF) | 0xFF000000;
        }
    }

    if (GET_REG_FIELD(__gxVerif->rasRegs[0xF5], 2, 2) && __gxVerif->verifyLevel >= 1) {
        if ((u32) ((__gxVerif->rasRegs[0xF4] & 0xFF000000) + 0x01000000) == 0U) {
            __gxVerif->cb(1, 0x28U, __gxvWarnings[0x28]);
        }
        if (!enabled) {
            __gxVerif->cb(1, 0x29U, __gxvWarnings[0x29]);
        }
    }

    if (__gxVerif->verifyLevel >= GX_WARN_MEDIUM) {
        if (GET_REG_FIELD(__gxVerif->rasRegs[((nTev - 1) * 2) + 0xC0], 2, 22)) {
            __gxVerif->cb(2, 0x2AU, __gxvWarnings[0x2A]);
        }
        if (GET_REG_FIELD(__gxVerif->rasRegs[((nTev - 1) * 2) + 0xC1], 2, 22)) {
            __gxVerif->cb(2, 0x2BU, __gxvWarnings[0x2B]);
        }
    }

    if (__gxVerif->verifyLevel >= GX_WARN_ALL) {
        if (!GET_REG_FIELD(__gxVerif->rasRegs[((nTev - 1) * 2) + 0xC1], 2, 0) && !GET_REG_FIELD(__gxVerif->rasRegs[((nTev - 1) * 2) + 0xC0], 1, 19)) {
            __gxVerif->cb(3, 0x2CU, __gxvWarnings[0x2C]);
        }
        if (!GET_REG_FIELD(__gxVerif->rasRegs[((nTev - 1) * 2) + 0xC1], 2, 0) && !GET_REG_FIELD(__gxVerif->rasRegs[((nTev - 1) * 2) + 0xC1], 1, 19)) {
            __gxVerif->cb(3, 0x2DU, __gxvWarnings[0x2D]);
        }
    }

    if (__gxVerif->verifyLevel >= GX_WARN_MEDIUM && GET_REG_FIELD(__gxVerif->rasRegs[0x43], 1, 6) && (GET_REG_FIELD(__gxVerif->rasRegs[0xF3], 2, 22) || ((u32) GET_REG_FIELD(__gxVerif->rasRegs[0xF3], 3, 16) != 7) || ((u32) GET_REG_FIELD(__gxVerif->rasRegs[0xF3], 3, 19) != 7))) {
        __gxVerif->cb(2, 0x2EU, __gxvWarnings[0x2E]);
    }
}
#endif

void __GXVerifyPE(void) {
    u32 i;

    if (__gxVerif->verifyLevel >= GX_WARN_SEVERE && GET_REG_FIELD(__gxVerif->rasRegs[0x41], 1, 0) && GET_REG_FIELD(__gxVerif->rasRegs[0x41], 1, 1) && __gxVerif->verifyLevel >= __gxvWarnLev[0x2F]) {
        __gxVerif->cb(__gxvWarnLev[0x2F], 0x2FU, __gxvWarnings[0x2F]);
    }

    if (__gxVerif->verifyLevel >= GX_WARN_MEDIUM) {
        if (GET_REG_FIELD(__gxVerif->rasRegs[0], 1, 9) && (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x43], 3, 0) != 2 && __gxVerif->verifyLevel >= __gxvWarnLev[0x31]) {
            __gxVerif->cb(__gxvWarnLev[0x31], 0x31U, __gxvWarnings[0x31]);
        }
        if (!GET_REG_FIELD(__gxVerif->rasRegs[0], 1, 9) && (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x43], 3, 0) == 2 && __gxVerif->verifyLevel >= __gxvWarnLev[0x32]) {
            __gxVerif->cb(__gxvWarnLev[0x32], 0x32U, __gxvWarnings[0x32]);
        }
    }

    if (__gxVerif->verifyLevel >= GX_WARN_ALL) {
        for (i = 0; i < 4; i++) {
            if (((u32)GET_REG_FIELD(__gxVerif->rasRegs[i + 1], 4, 4) > GET_REG_FIELD(__gxVerif->rasRegs[i + 1], 4, 12) || (u32)GET_REG_FIELD(__gxVerif->rasRegs[i + 1], 4, 12) > (u32)GET_REG_FIELD(__gxVerif->rasRegs[i + 1], 4, 20))
             && (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x43], 3, 0) == 2 && __gxVerif->verifyLevel >= __gxvWarnLev[0x33]) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x33], i);
                __gxVerif->cb(__gxvWarnLev[0x33], 0x33U, __gxvDummyStr);
            }
        }
    }
}

#endif
