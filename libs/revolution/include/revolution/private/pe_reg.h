#ifndef PE_REG_H
#define PE_REG_H

#include <revolution/private/GXFDLShortcut.h>

#define PE_REFRESH_RID_SIZE     8
#define PE_REFRESH_RID_SHIFT    24
#define PE_REFRESH_RID_MASK     0xff000000
#define PE_REFRESH_GET_RID(pe_refresh) \
        ((((unsigned long)(pe_refresh)) & PE_REFRESH_RID_MASK) >> PE_REFRESH_RID_SHIFT)

#define PE_REFRESH_ENABLE_SIZE  1
#define PE_REFRESH_ENABLE_SHIFT 9
#define PE_REFRESH_ENABLE_MASK  0x00000200
#define PE_REFRESH_GET_ENABLE(pe_refresh) \
        ((((unsigned long)(pe_refresh)) & PE_REFRESH_ENABLE_MASK) >> PE_REFRESH_ENABLE_SHIFT)

#define PE_REFRESH_INTERVAL_SIZE        9
#define PE_REFRESH_INTERVAL_SHIFT       0
#define PE_REFRESH_INTERVAL_MASK        0x000001ff
#define PE_REFRESH_GET_INTERVAL(pe_refresh) \
        ((((unsigned long)(pe_refresh)) & PE_REFRESH_INTERVAL_MASK) >> PE_REFRESH_INTERVAL_SHIFT)

#define PE_REFRESH_TOTAL_SIZE   32
#define PE_REFRESH(interval, enable, rid) \
        ((((unsigned long)(interval)) << PE_REFRESH_INTERVAL_SHIFT) | \
         (((unsigned long)(enable)) << PE_REFRESH_ENABLE_SHIFT) | \
         (((unsigned long)(rid)) << PE_REFRESH_RID_SHIFT))

#define PE_CHICKEN_PIWR_SIZE	1
#define PE_CHICKEN_PIWR_SHIFT	0
#define PE_CHICKEN_PIWR_MASK	0x00000001
#define PE_CHICKEN_GET_PIWR(pe_chicken) \
	((((unsigned long)(pe_chicken)) & PE_CHICKEN_PIWR_MASK) >> PE_CHICKEN_PIWR_SHIFT)

#define PE_CHICKEN_TXCPY_FMT_SIZE	1
#define PE_CHICKEN_TXCPY_FMT_SHIFT	1
#define PE_CHICKEN_TXCPY_FMT_MASK	0x00000002
#define PE_CHICKEN_GET_TXCPY_FMT(pe_chicken) \
    ((((unsigned long)(pe_chicken)) & PE_CHICKEN_TXCPY_FMT_MASK) >> PE_CHICKEN_TXCPY_FMT_SHIFT)

#define PE_CHICKEN_TXCPY_CCV_SIZE	1
#define PE_CHICKEN_TXCPY_CCV_SHIFT	2
#define PE_CHICKEN_TXCPY_CCV_MASK	0x00000004
#define PE_CHICKEN_GET_TXCPY_CCV(pe_chicken) \
    ((((unsigned long)(pe_chicken)) & PE_CHICKEN_TXCPY_CCV_MASK) >> PE_CHICKEN_TXCPY_CCV_SHIFT)

#define PE_CHICKEN_BLENDOP_SIZE	1
#define PE_CHICKEN_BLENDOP_SHIFT	3
#define PE_CHICKEN_BLENDOP_MASK	0x00000008
#define PE_CHICKEN_GET_BLENDOP(pe_chicken) \
	((((unsigned long)(pe_chicken)) & PE_CHICKEN_BLENDOP_MASK) >> PE_CHICKEN_BLENDOP_SHIFT)

#define PE_CHICKEN_RID_SIZE	8
#define PE_CHICKEN_RID_SHIFT	24
#define PE_CHICKEN_RID_MASK	0xff000000
#define PE_CHICKEN_GET_RID(pe_chicken) \
    ((((unsigned long)(pe_chicken)) & PE_CHICKEN_RID_MASK) >> PE_CHICKEN_RID_SHIFT)

#define PE_CMODE0_RID_SIZE      8
#define PE_CMODE0_RID_SHIFT     24
#define PE_CMODE0_RID_MASK      0xff000000
#define PE_CMODE0_GET_RID(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_RID_MASK) >> PE_CMODE0_RID_SHIFT)

#define PE_CMODE1_RID_SIZE      8
#define PE_CMODE1_RID_SHIFT     24
#define PE_CMODE1_RID_MASK      0xff000000
#define PE_CMODE1_GET_RID(pe_cmode1) \
        ((((unsigned long)(pe_cmode1)) & PE_CMODE1_RID_MASK) >> PE_CMODE1_RID_SHIFT)

#define PE_ZMODE_RID_SIZE       8
#define PE_ZMODE_RID_SHIFT      24
#define PE_ZMODE_RID_MASK       0xff000000
#define PE_ZMODE_GET_RID(pe_zmode) \
        ((((unsigned long)(pe_zmode)) & PE_ZMODE_RID_MASK) >> PE_ZMODE_RID_SHIFT)

#define PE_CONTROL_RID_SIZE     8
#define PE_CONTROL_RID_SHIFT    24
#define PE_CONTROL_RID_MASK     0xff000000
#define PE_CONTROL_GET_RID(pe_control) \
        ((((unsigned long)(pe_control)) & PE_CONTROL_RID_MASK) >> PE_CONTROL_RID_SHIFT)

#define PE_COPY_CMD_GAMMA_SIZE  2
#define PE_COPY_CMD_GAMMA_SHIFT 7
#define PE_COPY_CMD_GAMMA_MASK  0x00000180
#define PE_COPY_CMD_GET_GAMMA(pe_copy_cmd) \
        ((((unsigned long)(pe_copy_cmd)) & PE_COPY_CMD_GAMMA_MASK) >> PE_COPY_CMD_GAMMA_SHIFT)

#define PE_TOKEN_TOKEN_SIZE     16
#define PE_TOKEN_TOKEN_SHIFT    0
#define PE_TOKEN_TOKEN_MASK     0x0000ffff
#define PE_TOKEN_GET_TOKEN(pe_token) \
        ((((unsigned long)(pe_token)) & PE_TOKEN_TOKEN_MASK) >> PE_TOKEN_TOKEN_SHIFT)
#define PE_TOKEN_SET_TOKEN(pe_token, token) { \
        FDL_ASSERT(!((token) & ~((1 << PE_TOKEN_TOKEN_SIZE)-1))); \
        pe_token = (((unsigned long)(pe_token)) & ~PE_TOKEN_TOKEN_MASK) | (((unsigned long)(token)) << PE_TOKEN_TOKEN_SHIFT);\
}

#define PE_TOKEN_RID_SIZE       8
#define PE_TOKEN_RID_SHIFT      24
#define PE_TOKEN_RID_MASK       0xff000000
#define PE_TOKEN_GET_RID(pe_token) \
        ((((unsigned long)(pe_token)) & PE_TOKEN_RID_MASK) >> PE_TOKEN_RID_SHIFT)
#define PE_TOKEN_SET_RID(pe_token, rid) { \
        FDL_ASSERT(!((rid) & ~((1 << PE_TOKEN_RID_SIZE)-1))); \
        pe_token = (((unsigned long)(pe_token)) & ~PE_TOKEN_RID_MASK) | (((unsigned long)(rid)) << PE_TOKEN_RID_SHIFT);\
}

#define PE_TOKEN_INT_TOKEN_SIZE 16
#define PE_TOKEN_INT_TOKEN_SHIFT        0
#define PE_TOKEN_INT_TOKEN_MASK 0x0000ffff
#define PE_TOKEN_INT_GET_TOKEN(pe_token_int) \
        ((((unsigned long)(pe_token_int)) & PE_TOKEN_INT_TOKEN_MASK) >> PE_TOKEN_INT_TOKEN_SHIFT)
#define PE_TOKEN_INT_SET_TOKEN(pe_token_int, token) { \
        FDL_ASSERT(!((token) & ~((1 << PE_TOKEN_INT_TOKEN_SIZE)-1))); \
        pe_token_int = (((unsigned long)(pe_token_int)) & ~PE_TOKEN_INT_TOKEN_MASK) | (((unsigned long)(token)) << PE_TOKEN_INT_TOKEN_SHIFT);\
}

#define PE_TOKEN_INT_RID_SIZE   8
#define PE_TOKEN_INT_RID_SHIFT  24
#define PE_TOKEN_INT_RID_MASK   0xff000000
#define PE_TOKEN_INT_GET_RID(pe_token_int) \
        ((((unsigned long)(pe_token_int)) & PE_TOKEN_INT_RID_MASK) >> PE_TOKEN_INT_RID_SHIFT)
#define PE_TOKEN_INT_SET_RID(pe_token_int, rid) { \
        FDL_ASSERT(!((rid) & ~((1 << PE_TOKEN_INT_RID_SIZE)-1))); \
        pe_token_int = (((unsigned long)(pe_token_int)) & ~PE_TOKEN_INT_RID_MASK) | (((unsigned long)(rid)) << PE_TOKEN_INT_RID_SHIFT);\
}
#define PE_TOKEN_INT_TOTAL_SIZE 32
#define PE_TOKEN_INT(token, rid) \
        ((((unsigned long)(token)) << PE_TOKEN_INT_TOKEN_SHIFT) | \
         (((unsigned long)(rid)) << PE_TOKEN_INT_RID_SHIFT))

#define PE_FINISH_DST_SIZE      2
#define PE_FINISH_DST_SHIFT     0
#define PE_FINISH_DST_MASK      0x00000003
#define PE_FINISH_GET_DST(pe_finish) \
        ((((unsigned long)(pe_finish)) & PE_FINISH_DST_MASK) >> PE_FINISH_DST_SHIFT)
#define PE_FINISH_SET_DST(pe_finish, dst) { \
        FDL_ASSERT(!((dst) & ~((1 << PE_FINISH_DST_SIZE)-1))); \
        pe_finish = (((unsigned long)(pe_finish)) & ~PE_FINISH_DST_MASK) | (((unsigned long)(dst)) << PE_FINISH_DST_SHIFT);\
}
#define PE_FINISH_PAD0_SIZE     22
#define PE_FINISH_PAD0_SHIFT    2
#define PE_FINISH_PAD0_MASK     0x00fffffc
#define PE_FINISH_GET_PAD0(pe_finish) \
        ((((unsigned long)(pe_finish)) & PE_FINISH_PAD0_MASK) >> PE_FINISH_PAD0_SHIFT)
#define PE_FINISH_SET_PAD0(pe_finish, pad0) { \
        FDL_ASSERT(!((pad0) & ~((1 << PE_FINISH_PAD0_SIZE)-1))); \
        pe_finish = (((unsigned long)(pe_finish)) & ~PE_FINISH_PAD0_MASK) | (((unsigned long)(pad0)) << PE_FINISH_PAD0_SHIFT);\
}
#define PE_FINISH_RID_SIZE      8
#define PE_FINISH_RID_SHIFT     24
#define PE_FINISH_RID_MASK      0xff000000
#define PE_FINISH_GET_RID(pe_finish) \
        ((((unsigned long)(pe_finish)) & PE_FINISH_RID_MASK) >> PE_FINISH_RID_SHIFT)
#define PE_FINISH_SET_RID(pe_finish, rid) { \
        FDL_ASSERT(!((rid) & ~((1 << PE_FINISH_RID_SIZE)-1))); \
        pe_finish = (((unsigned long)(pe_finish)) & ~PE_FINISH_RID_MASK) | (((unsigned long)(rid)) << PE_FINISH_RID_SHIFT);\
}
#define PE_FINISH_TOTAL_SIZE    32
#define PE_FINISH(dst, rid) \
        ((((unsigned long)(dst)) << PE_FINISH_DST_SHIFT) | \
         (((unsigned long)(rid)) << PE_FINISH_RID_SHIFT))

#define PE_PI_CTL_AFMT_SIZE	2
#define PE_PI_CTL_AFMT_SHIFT	0
#define PE_PI_CTL_AFMT_MASK	0x00000003
#define PE_PI_CTL_GET_AFMT(pe_pi_ctl) \
	((((unsigned long)(pe_pi_ctl)) & PE_PI_CTL_AFMT_MASK) >> PE_PI_CTL_AFMT_SHIFT)
#define PE_PI_CTL_SET_AFMT(pe_pi_ctl, afmt) { \
	FDL_ASSERT(!((afmt) & ~((1 << PE_PI_CTL_AFMT_SIZE)-1))); \
	pe_pi_ctl = (((unsigned long)(pe_pi_ctl)) & ~PE_PI_CTL_AFMT_MASK) | (((unsigned long)(afmt)) << PE_PI_CTL_AFMT_SHIFT);\
}
#define PE_PI_CTL_ZFMT_SIZE	1
#define PE_PI_CTL_ZFMT_SHIFT	2
#define PE_PI_CTL_ZFMT_MASK	0x00000004
#define PE_PI_CTL_GET_ZFMT(pe_pi_ctl) \
	((((unsigned long)(pe_pi_ctl)) & PE_PI_CTL_ZFMT_MASK) >> PE_PI_CTL_ZFMT_SHIFT)
#define PE_PI_CTL_SET_ZFMT(pe_pi_ctl, zfmt) { \
	FDL_ASSERT(!((zfmt) & ~((1 << PE_PI_CTL_ZFMT_SIZE)-1))); \
	pe_pi_ctl = (((unsigned long)(pe_pi_ctl)) & ~PE_PI_CTL_ZFMT_MASK) | (((unsigned long)(zfmt)) << PE_PI_CTL_ZFMT_SHIFT);\
}
#define PE_PI_CTL_PAD0_SIZE	21
#define PE_PI_CTL_PAD0_SHIFT	3
#define PE_PI_CTL_PAD0_MASK	0x00fffff8
#define PE_PI_CTL_GET_PAD0(pe_pi_ctl) \
	((((unsigned long)(pe_pi_ctl)) & PE_PI_CTL_PAD0_MASK) >> PE_PI_CTL_PAD0_SHIFT)
#define PE_PI_CTL_SET_PAD0(pe_pi_ctl, pad0) { \
	FDL_ASSERT(!((pad0) & ~((1 << PE_PI_CTL_PAD0_SIZE)-1))); \
	pe_pi_ctl = (((unsigned long)(pe_pi_ctl)) & ~PE_PI_CTL_PAD0_MASK) | (((unsigned long)(pad0)) << PE_PI_CTL_PAD0_SHIFT);\
}
#define PE_PI_CTL_RID_SIZE	8
#define PE_PI_CTL_RID_SHIFT	24
#define PE_PI_CTL_RID_MASK	0xff000000
#define PE_PI_CTL_GET_RID(pe_pi_ctl) \
	((((unsigned long)(pe_pi_ctl)) & PE_PI_CTL_RID_MASK) >> PE_PI_CTL_RID_SHIFT)
#define PE_PI_CTL_SET_RID(pe_pi_ctl, rid) { \
	FDL_ASSERT(!((rid) & ~((1 << PE_PI_CTL_RID_SIZE)-1))); \
	pe_pi_ctl = (((unsigned long)(pe_pi_ctl)) & ~PE_PI_CTL_RID_MASK) | (((unsigned long)(rid)) << PE_PI_CTL_RID_SHIFT);\
}
#define PE_PI_CTL_TOTAL_SIZE	32
#define PE_PI_CTL(afmt, zfmt, rid) \
	((((unsigned long)(afmt)) << PE_PI_CTL_AFMT_SHIFT) | \
	 (((unsigned long)(zfmt)) << PE_PI_CTL_ZFMT_SHIFT) | \
	 (((unsigned long)(rid)) << PE_PI_CTL_RID_SHIFT))

#define PE_CMODE0_BLEND_ENABLE_SIZE     1
#define PE_CMODE0_BLEND_ENABLE_SHIFT    0
#define PE_CMODE0_BLEND_ENABLE_MASK     0x00000001
#define PE_CMODE0_GET_BLEND_ENABLE(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_BLEND_ENABLE_MASK) >> PE_CMODE0_BLEND_ENABLE_SHIFT)
#define PE_CMODE0_SET_BLEND_ENABLE(pe_cmode0, blend_enable) { \
        FDL_ASSERT(!((blend_enable) & ~((1 << PE_CMODE0_BLEND_ENABLE_SIZE)-1))); \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_BLEND_ENABLE_MASK) | (((unsigned long)(blend_enable)) << PE_CMODE0_BLEND_ENABLE_SHIFT);\
}
#define PE_CMODE0_LOGICOP_ENABLE_SIZE   1
#define PE_CMODE0_LOGICOP_ENABLE_SHIFT  1
#define PE_CMODE0_LOGICOP_ENABLE_MASK   0x00000002
#define PE_CMODE0_GET_LOGICOP_ENABLE(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_LOGICOP_ENABLE_MASK) >> PE_CMODE0_LOGICOP_ENABLE_SHIFT)
#define PE_CMODE0_SET_LOGICOP_ENABLE(pe_cmode0, logicop_enable) { \
        FDL_ASSERT(!((logicop_enable) & ~((1 << PE_CMODE0_LOGICOP_ENABLE_SIZE)-1))); \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_LOGICOP_ENABLE_MASK) | (((unsigned long)(logicop_enable)) << PE_CMODE0_LOGICOP_ENABLE_SHIFT);\
}
#define PE_CMODE0_DITHER_ENABLE_SIZE    1
#define PE_CMODE0_DITHER_ENABLE_SHIFT   2
#define PE_CMODE0_DITHER_ENABLE_MASK    0x00000004
#define PE_CMODE0_GET_DITHER_ENABLE(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_DITHER_ENABLE_MASK) >> PE_CMODE0_DITHER_ENABLE_SHIFT)
#define PE_CMODE0_SET_DITHER_ENABLE(pe_cmode0, dither_enable) { \
        FDL_ASSERT(!((dither_enable) & ~((1 << PE_CMODE0_DITHER_ENABLE_SIZE)-1))); \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_DITHER_ENABLE_MASK) | (((unsigned long)(dither_enable)) << PE_CMODE0_DITHER_ENABLE_SHIFT);\
}
#define PE_CMODE0_COLOR_MASK_SIZE       1
#define PE_CMODE0_COLOR_MASK_SHIFT      3
#define PE_CMODE0_COLOR_MASK_MASK       0x00000008
#define PE_CMODE0_GET_COLOR_MASK(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_COLOR_MASK_MASK) >> PE_CMODE0_COLOR_MASK_SHIFT)
#define PE_CMODE0_SET_COLOR_MASK(pe_cmode0, color_mask) { \
        FDL_ASSERT(!((color_mask) & ~((1 << PE_CMODE0_COLOR_MASK_SIZE)-1))); \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_COLOR_MASK_MASK) | (((unsigned long)(color_mask)) << PE_CMODE0_COLOR_MASK_SHIFT);\
}
#define PE_CMODE0_ALPHA_MASK_SIZE       1
#define PE_CMODE0_ALPHA_MASK_SHIFT      4
#define PE_CMODE0_ALPHA_MASK_MASK       0x00000010
#define PE_CMODE0_GET_ALPHA_MASK(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_ALPHA_MASK_MASK) >> PE_CMODE0_ALPHA_MASK_SHIFT)
#define PE_CMODE0_SET_ALPHA_MASK(pe_cmode0, alpha_mask) { \
        FDL_ASSERT(!((alpha_mask) & ~((1 << PE_CMODE0_ALPHA_MASK_SIZE)-1))); \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_ALPHA_MASK_MASK) | (((unsigned long)(alpha_mask)) << PE_CMODE0_ALPHA_MASK_SHIFT);\
}
#define PE_CMODE0_DFACTOR_SIZE  3
#define PE_CMODE0_DFACTOR_SHIFT 5
#define PE_CMODE0_DFACTOR_MASK  0x000000e0
#define PE_CMODE0_GET_DFACTOR(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_DFACTOR_MASK) >> PE_CMODE0_DFACTOR_SHIFT)
#define PE_CMODE0_SET_DFACTOR(pe_cmode0, dfactor) { \
        FDL_ASSERT(!((dfactor) & ~((1 << PE_CMODE0_DFACTOR_SIZE)-1))); \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_DFACTOR_MASK) | (((unsigned long)(dfactor)) << PE_CMODE0_DFACTOR_SHIFT);\
}
#define PE_CMODE0_SFACTOR_SIZE  3
#define PE_CMODE0_SFACTOR_SHIFT 8
#define PE_CMODE0_SFACTOR_MASK  0x00000700
#define PE_CMODE0_GET_SFACTOR(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_SFACTOR_MASK) >> PE_CMODE0_SFACTOR_SHIFT)
#define PE_CMODE0_SET_SFACTOR(pe_cmode0, sfactor) { \
        FDL_ASSERT(!((sfactor) & ~((1 << PE_CMODE0_SFACTOR_SIZE)-1))); \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_SFACTOR_MASK) | (((unsigned long)(sfactor)) << PE_CMODE0_SFACTOR_SHIFT);\
}
#define PE_CMODE0_BLENDOP_SIZE  1
#define PE_CMODE0_BLENDOP_SHIFT 11
#define PE_CMODE0_BLENDOP_MASK  0x00000800
#define PE_CMODE0_GET_BLENDOP(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_BLENDOP_MASK) >> PE_CMODE0_BLENDOP_SHIFT)
#define PE_CMODE0_SET_BLENDOP(pe_cmode0, blendop) { \
        FDL_ASSERT(!((blendop) & ~((1 << PE_CMODE0_BLENDOP_SIZE)-1))); \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_BLENDOP_MASK) | (((unsigned long)(blendop)) << PE_CMODE0_BLENDOP_SHIFT);\
}
#define PE_CMODE0_LOGICOP_SIZE  4
#define PE_CMODE0_LOGICOP_SHIFT 12
#define PE_CMODE0_LOGICOP_MASK  0x0000f000
#define PE_CMODE0_GET_LOGICOP(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_LOGICOP_MASK) >> PE_CMODE0_LOGICOP_SHIFT)
#define PE_CMODE0_SET_LOGICOP(pe_cmode0, logicop) { \
        FDL_ASSERT(!((logicop) & ~((1 << PE_CMODE0_LOGICOP_SIZE)-1))); \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_LOGICOP_MASK) | (((unsigned long)(logicop)) << PE_CMODE0_LOGICOP_SHIFT);\
}
#define PE_CMODE0_PAD0_SIZE     8
#define PE_CMODE0_PAD0_SHIFT    16
#define PE_CMODE0_PAD0_MASK     0x00ff0000
#define PE_CMODE0_GET_PAD0(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_PAD0_MASK) >> PE_CMODE0_PAD0_SHIFT)
#define PE_CMODE0_SET_PAD0(pe_cmode0, pad0) { \
        FDL_ASSERT(!((pad0) & ~((1 << PE_CMODE0_PAD0_SIZE)-1))); \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_PAD0_MASK) | (((unsigned long)(pad0)) << PE_CMODE0_PAD0_SHIFT);\
}
#define PE_CMODE0_RID_SIZE      8
#define PE_CMODE0_RID_SHIFT     24
#define PE_CMODE0_RID_MASK      0xff000000
#define PE_CMODE0_GET_RID(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_RID_MASK) >> PE_CMODE0_RID_SHIFT)
#define PE_CMODE0_SET_RID(pe_cmode0, rid) { \
        FDL_ASSERT(!((rid) & ~((1 << PE_CMODE0_RID_SIZE)-1))); \
        pe_cmode0 = (((unsigned long)(pe_cmode0)) & ~PE_CMODE0_RID_MASK) | (((unsigned long)(rid)) << PE_CMODE0_RID_SHIFT);\
}
#define PE_CMODE0_TOTAL_SIZE    32
#define PE_CMODE0(blend_enable, logicop_enable, dither_enable, color_mask, alpha_mask, dfactor, sfactor, blendop, logicop, rid) \
        ((((unsigned long)(blend_enable)) << PE_CMODE0_BLEND_ENABLE_SHIFT) | \
         (((unsigned long)(logicop_enable)) << PE_CMODE0_LOGICOP_ENABLE_SHIFT) | \
         (((unsigned long)(dither_enable)) << PE_CMODE0_DITHER_ENABLE_SHIFT) | \
         (((unsigned long)(color_mask)) << PE_CMODE0_COLOR_MASK_SHIFT) | \
         (((unsigned long)(alpha_mask)) << PE_CMODE0_ALPHA_MASK_SHIFT) | \
         (((unsigned long)(dfactor)) << PE_CMODE0_DFACTOR_SHIFT) | \
         (((unsigned long)(sfactor)) << PE_CMODE0_SFACTOR_SHIFT) | \
         (((unsigned long)(blendop)) << PE_CMODE0_BLENDOP_SHIFT) | \
         (((unsigned long)(logicop)) << PE_CMODE0_LOGICOP_SHIFT) | \
         (((unsigned long)(rid)) << PE_CMODE0_RID_SHIFT))

#define PE_CMODE1_CONSTANT_ALPHA_SIZE   8
#define PE_CMODE1_CONSTANT_ALPHA_SHIFT  0
#define PE_CMODE1_CONSTANT_ALPHA_MASK   0x000000ff
#define PE_CMODE1_GET_CONSTANT_ALPHA(pe_cmode1) \
        ((((unsigned long)(pe_cmode1)) & PE_CMODE1_CONSTANT_ALPHA_MASK) >> PE_CMODE1_CONSTANT_ALPHA_SHIFT)
#define PE_CMODE1_SET_CONSTANT_ALPHA(pe_cmode1, constant_alpha) { \
        FDL_ASSERT(!((constant_alpha) & ~((1 << PE_CMODE1_CONSTANT_ALPHA_SIZE)-1))); \
        pe_cmode1 = (((unsigned long)(pe_cmode1)) & ~PE_CMODE1_CONSTANT_ALPHA_MASK) | (((unsigned long)(constant_alpha)) << PE_CMODE1_CONSTANT_ALPHA_SHIFT);\
}
#define PE_CMODE1_CONSTANT_ALPHA_ENABLE_SIZE    1
#define PE_CMODE1_CONSTANT_ALPHA_ENABLE_SHIFT   8
#define PE_CMODE1_CONSTANT_ALPHA_ENABLE_MASK    0x00000100
#define PE_CMODE1_GET_CONSTANT_ALPHA_ENABLE(pe_cmode1) \
        ((((unsigned long)(pe_cmode1)) & PE_CMODE1_CONSTANT_ALPHA_ENABLE_MASK) >> PE_CMODE1_CONSTANT_ALPHA_ENABLE_SHIFT)
#define PE_CMODE1_SET_CONSTANT_ALPHA_ENABLE(pe_cmode1, constant_alpha_enable) { \
        FDL_ASSERT(!((constant_alpha_enable) & ~((1 << PE_CMODE1_CONSTANT_ALPHA_ENABLE_SIZE)-1))); \
        pe_cmode1 = (((unsigned long)(pe_cmode1)) & ~PE_CMODE1_CONSTANT_ALPHA_ENABLE_MASK) | (((unsigned long)(constant_alpha_enable)) << PE_CMODE1_CONSTANT_ALPHA_ENABLE_SHIFT);\
}
#define PE_CMODE1_YUV_SIZE      2
#define PE_CMODE1_YUV_SHIFT     9
#define PE_CMODE1_YUV_MASK      0x00000600
#define PE_CMODE1_GET_YUV(pe_cmode1) \
        ((((unsigned long)(pe_cmode1)) & PE_CMODE1_YUV_MASK) >> PE_CMODE1_YUV_SHIFT)
#define PE_CMODE1_SET_YUV(pe_cmode1, yuv) { \
        FDL_ASSERT(!((yuv) & ~((1 << PE_CMODE1_YUV_SIZE)-1))); \
        pe_cmode1 = (((unsigned long)(pe_cmode1)) & ~PE_CMODE1_YUV_MASK) | (((unsigned long)(yuv)) << PE_CMODE1_YUV_SHIFT);\
}
#define PE_CMODE1_PAD0_SIZE     13
#define PE_CMODE1_PAD0_SHIFT    11
#define PE_CMODE1_PAD0_MASK     0x00fff800
#define PE_CMODE1_GET_PAD0(pe_cmode1) \
        ((((unsigned long)(pe_cmode1)) & PE_CMODE1_PAD0_MASK) >> PE_CMODE1_PAD0_SHIFT)
#define PE_CMODE1_SET_PAD0(pe_cmode1, pad0) { \
        FDL_ASSERT(!((pad0) & ~((1 << PE_CMODE1_PAD0_SIZE)-1))); \
        pe_cmode1 = (((unsigned long)(pe_cmode1)) & ~PE_CMODE1_PAD0_MASK) | (((unsigned long)(pad0)) << PE_CMODE1_PAD0_SHIFT);\
}
#define PE_CMODE1_RID_SIZE      8
#define PE_CMODE1_RID_SHIFT     24
#define PE_CMODE1_RID_MASK      0xff000000
#define PE_CMODE1_GET_RID(pe_cmode1) \
        ((((unsigned long)(pe_cmode1)) & PE_CMODE1_RID_MASK) >> PE_CMODE1_RID_SHIFT)
#define PE_CMODE1_SET_RID(pe_cmode1, rid) { \
        FDL_ASSERT(!((rid) & ~((1 << PE_CMODE1_RID_SIZE)-1))); \
        pe_cmode1 = (((unsigned long)(pe_cmode1)) & ~PE_CMODE1_RID_MASK) | (((unsigned long)(rid)) << PE_CMODE1_RID_SHIFT);\
}
#define PE_CMODE1_TOTAL_SIZE    32
#define PE_CMODE1(constant_alpha, constant_alpha_enable, yuv, rid) \
        ((((unsigned long)(constant_alpha)) << PE_CMODE1_CONSTANT_ALPHA_SHIFT) | \
         (((unsigned long)(constant_alpha_enable)) << PE_CMODE1_CONSTANT_ALPHA_ENABLE_SHIFT) | \
         (((unsigned long)(yuv)) << PE_CMODE1_YUV_SHIFT) | \
         (((unsigned long)(rid)) << PE_CMODE1_RID_SHIFT))

#define PE_ZMODE_ENABLE_SIZE    1
#define PE_ZMODE_ENABLE_SHIFT   0
#define PE_ZMODE_ENABLE_MASK    0x00000001
#define PE_ZMODE_GET_ENABLE(pe_zmode) \
        ((((unsigned long)(pe_zmode)) & PE_ZMODE_ENABLE_MASK) >> PE_ZMODE_ENABLE_SHIFT)
#define PE_ZMODE_SET_ENABLE(pe_zmode, enable) { \
        FDL_ASSERT(!((enable) & ~((1 << PE_ZMODE_ENABLE_SIZE)-1))); \
        pe_zmode = (((unsigned long)(pe_zmode)) & ~PE_ZMODE_ENABLE_MASK) | (((unsigned long)(enable)) << PE_ZMODE_ENABLE_SHIFT);\
}
#define PE_ZMODE_FUNC_SIZE      3
#define PE_ZMODE_FUNC_SHIFT     1
#define PE_ZMODE_FUNC_MASK      0x0000000e
#define PE_ZMODE_GET_FUNC(pe_zmode) \
        ((((unsigned long)(pe_zmode)) & PE_ZMODE_FUNC_MASK) >> PE_ZMODE_FUNC_SHIFT)
#define PE_ZMODE_SET_FUNC(pe_zmode, func) { \
        FDL_ASSERT(!((func) & ~((1 << PE_ZMODE_FUNC_SIZE)-1))); \
        pe_zmode = (((unsigned long)(pe_zmode)) & ~PE_ZMODE_FUNC_MASK) | (((unsigned long)(func)) << PE_ZMODE_FUNC_SHIFT);\
}
#define PE_ZMODE_MASK_SIZE      1
#define PE_ZMODE_MASK_SHIFT     4
#define PE_ZMODE_MASK_MASK      0x00000010
#define PE_ZMODE_GET_MASK(pe_zmode) \
        ((((unsigned long)(pe_zmode)) & PE_ZMODE_MASK_MASK) >> PE_ZMODE_MASK_SHIFT)
#define PE_ZMODE_SET_MASK(pe_zmode, mask) { \
        FDL_ASSERT(!((mask) & ~((1 << PE_ZMODE_MASK_SIZE)-1))); \
        pe_zmode = (((unsigned long)(pe_zmode)) & ~PE_ZMODE_MASK_MASK) | (((unsigned long)(mask)) << PE_ZMODE_MASK_SHIFT);\
}
#define PE_ZMODE_PAD0_SIZE      19
#define PE_ZMODE_PAD0_SHIFT     5
#define PE_ZMODE_PAD0_MASK      0x00ffffe0
#define PE_ZMODE_GET_PAD0(pe_zmode) \
        ((((unsigned long)(pe_zmode)) & PE_ZMODE_PAD0_MASK) >> PE_ZMODE_PAD0_SHIFT)
#define PE_ZMODE_SET_PAD0(pe_zmode, pad0) { \
        FDL_ASSERT(!((pad0) & ~((1 << PE_ZMODE_PAD0_SIZE)-1))); \
        pe_zmode = (((unsigned long)(pe_zmode)) & ~PE_ZMODE_PAD0_MASK) | (((unsigned long)(pad0)) << PE_ZMODE_PAD0_SHIFT);\
}
#define PE_ZMODE_RID_SIZE       8
#define PE_ZMODE_RID_SHIFT      24
#define PE_ZMODE_RID_MASK       0xff000000
#define PE_ZMODE_GET_RID(pe_zmode) \
        ((((unsigned long)(pe_zmode)) & PE_ZMODE_RID_MASK) >> PE_ZMODE_RID_SHIFT)
#define PE_ZMODE_SET_RID(pe_zmode, rid) { \
        FDL_ASSERT(!((rid) & ~((1 << PE_ZMODE_RID_SIZE)-1))); \
        pe_zmode = (((unsigned long)(pe_zmode)) & ~PE_ZMODE_RID_MASK) | (((unsigned long)(rid)) << PE_ZMODE_RID_SHIFT);\
}
#define PE_ZMODE_TOTAL_SIZE     32
#define PE_ZMODE(enable, func, mask, rid) \
        ((((unsigned long)(enable)) << PE_ZMODE_ENABLE_SHIFT) | \
         (((unsigned long)(func)) << PE_ZMODE_FUNC_SHIFT) | \
         (((unsigned long)(mask)) << PE_ZMODE_MASK_SHIFT) | \
         (((unsigned long)(rid)) << PE_ZMODE_RID_SHIFT))

#define PE_PI_EFB_ADDR_PAD0_SIZE	2
#define PE_PI_EFB_ADDR_PAD0_SHIFT	0
#define PE_PI_EFB_ADDR_PAD0_MASK	0x00000003
#define PE_PI_EFB_ADDR_GET_PAD0(pe_pi_efb_addr) \
	((((unsigned long)(pe_pi_efb_addr)) & PE_PI_EFB_ADDR_PAD0_MASK) >> PE_PI_EFB_ADDR_PAD0_SHIFT)
#define PE_PI_EFB_ADDR_SET_PAD0(pe_pi_efb_addr, pad0) { \
	FDL_ASSERT(!((pad0) & ~((1 << PE_PI_EFB_ADDR_PAD0_SIZE)-1))); \
	pe_pi_efb_addr = (((unsigned long)(pe_pi_efb_addr)) & ~PE_PI_EFB_ADDR_PAD0_MASK) | (((unsigned long)(pad0)) << PE_PI_EFB_ADDR_PAD0_SHIFT);\
}
#define PE_PI_EFB_ADDR_X_SIZE	10
#define PE_PI_EFB_ADDR_X_SHIFT	2
#define PE_PI_EFB_ADDR_X_MASK	0x00000ffc
#define PE_PI_EFB_ADDR_GET_X(pe_pi_efb_addr) \
	((((unsigned long)(pe_pi_efb_addr)) & PE_PI_EFB_ADDR_X_MASK) >> PE_PI_EFB_ADDR_X_SHIFT)
#define PE_PI_EFB_ADDR_SET_X(pe_pi_efb_addr, x) { \
	FDL_ASSERT(!((x) & ~((1 << PE_PI_EFB_ADDR_X_SIZE)-1))); \
	pe_pi_efb_addr = (((unsigned long)(pe_pi_efb_addr)) & ~PE_PI_EFB_ADDR_X_MASK) | (((unsigned long)(x)) << PE_PI_EFB_ADDR_X_SHIFT);\
}
#define PE_PI_EFB_ADDR_Y_SIZE	10
#define PE_PI_EFB_ADDR_Y_SHIFT	12
#define PE_PI_EFB_ADDR_Y_MASK	0x003ff000
#define PE_PI_EFB_ADDR_GET_Y(pe_pi_efb_addr) \
	((((unsigned long)(pe_pi_efb_addr)) & PE_PI_EFB_ADDR_Y_MASK) >> PE_PI_EFB_ADDR_Y_SHIFT)
#define PE_PI_EFB_ADDR_SET_Y(pe_pi_efb_addr, y) { \
	FDL_ASSERT(!((y) & ~((1 << PE_PI_EFB_ADDR_Y_SIZE)-1))); \
	pe_pi_efb_addr = (((unsigned long)(pe_pi_efb_addr)) & ~PE_PI_EFB_ADDR_Y_MASK) | (((unsigned long)(y)) << PE_PI_EFB_ADDR_Y_SHIFT);\
}
#define PE_PI_EFB_ADDR_TYPE_SIZE	2
#define PE_PI_EFB_ADDR_TYPE_SHIFT	22
#define PE_PI_EFB_ADDR_TYPE_MASK	0x00c00000
#define PE_PI_EFB_ADDR_GET_TYPE(pe_pi_efb_addr) \
	((((unsigned long)(pe_pi_efb_addr)) & PE_PI_EFB_ADDR_TYPE_MASK) >> PE_PI_EFB_ADDR_TYPE_SHIFT)
#define PE_PI_EFB_ADDR_SET_TYPE(pe_pi_efb_addr, type) { \
	FDL_ASSERT(!((type) & ~((1 << PE_PI_EFB_ADDR_TYPE_SIZE)-1))); \
	pe_pi_efb_addr = (((unsigned long)(pe_pi_efb_addr)) & ~PE_PI_EFB_ADDR_TYPE_MASK) | (((unsigned long)(type)) << PE_PI_EFB_ADDR_TYPE_SHIFT);\
}
#define PE_PI_EFB_ADDR_TOTAL_SIZE	24
#define PE_PI_EFB_ADDR(x, y, type) \
	((((unsigned long)(x)) << PE_PI_EFB_ADDR_X_SHIFT) | \
	 (((unsigned long)(y)) << PE_PI_EFB_ADDR_Y_SHIFT) | \
	 (((unsigned long)(type)) << PE_PI_EFB_ADDR_TYPE_SHIFT))

#define PE_INTRCTL_INT0EN_SIZE	1
#define PE_INTRCTL_INT0EN_SHIFT	0
#define PE_INTRCTL_INT0EN_MASK	0x00000001
#define PE_INTRCTL_GET_INT0EN(pe_intrctl) \
	((((unsigned long)(pe_intrctl)) & PE_INTRCTL_INT0EN_MASK) >> PE_INTRCTL_INT0EN_SHIFT)
#define PE_INTRCTL_SET_INT0EN(pe_intrctl, int0en) { \
	FDL_ASSERT(!((int0en) & ~((1 << PE_INTRCTL_INT0EN_SIZE)-1))); \
	pe_intrctl = (((unsigned long)(pe_intrctl)) & ~PE_INTRCTL_INT0EN_MASK) | (((unsigned long)(int0en)) << PE_INTRCTL_INT0EN_SHIFT);\
}
#define PE_INTRCTL_INT1EN_SIZE	1
#define PE_INTRCTL_INT1EN_SHIFT	1
#define PE_INTRCTL_INT1EN_MASK	0x00000002
#define PE_INTRCTL_GET_INT1EN(pe_intrctl) \
	((((unsigned long)(pe_intrctl)) & PE_INTRCTL_INT1EN_MASK) >> PE_INTRCTL_INT1EN_SHIFT)
#define PE_INTRCTL_SET_INT1EN(pe_intrctl, int1en) { \
	FDL_ASSERT(!((int1en) & ~((1 << PE_INTRCTL_INT1EN_SIZE)-1))); \
	pe_intrctl = (((unsigned long)(pe_intrctl)) & ~PE_INTRCTL_INT1EN_MASK) | (((unsigned long)(int1en)) << PE_INTRCTL_INT1EN_SHIFT);\
}
#define PE_INTRCTL_INT0CLR_SIZE	1
#define PE_INTRCTL_INT0CLR_SHIFT	2
#define PE_INTRCTL_INT0CLR_MASK	0x00000004
#define PE_INTRCTL_GET_INT0CLR(pe_intrctl) \
	((((unsigned long)(pe_intrctl)) & PE_INTRCTL_INT0CLR_MASK) >> PE_INTRCTL_INT0CLR_SHIFT)
#define PE_INTRCTL_SET_INT0CLR(pe_intrctl, int0clr) { \
	FDL_ASSERT(!((int0clr) & ~((1 << PE_INTRCTL_INT0CLR_SIZE)-1))); \
	pe_intrctl = (((unsigned long)(pe_intrctl)) & ~PE_INTRCTL_INT0CLR_MASK) | (((unsigned long)(int0clr)) << PE_INTRCTL_INT0CLR_SHIFT);\
}
#define PE_INTRCTL_INT1CLR_SIZE	1
#define PE_INTRCTL_INT1CLR_SHIFT	3
#define PE_INTRCTL_INT1CLR_MASK	0x00000008
#define PE_INTRCTL_GET_INT1CLR(pe_intrctl) \
	((((unsigned long)(pe_intrctl)) & PE_INTRCTL_INT1CLR_MASK) >> PE_INTRCTL_INT1CLR_SHIFT)
#define PE_INTRCTL_SET_INT1CLR(pe_intrctl, int1clr) { \
	FDL_ASSERT(!((int1clr) & ~((1 << PE_INTRCTL_INT1CLR_SIZE)-1))); \
	pe_intrctl = (((unsigned long)(pe_intrctl)) & ~PE_INTRCTL_INT1CLR_MASK) | (((unsigned long)(int1clr)) << PE_INTRCTL_INT1CLR_SHIFT);\
}
#define PE_INTRCTL_TOTAL_SIZE	4
#define PE_INTRCTL(int0en, int1en, int0clr, int1clr) \
	((((unsigned long)(int0en)) << PE_INTRCTL_INT0EN_SHIFT) | \
	 (((unsigned long)(int1en)) << PE_INTRCTL_INT1EN_SHIFT) | \
	 (((unsigned long)(int0clr)) << PE_INTRCTL_INT0CLR_SHIFT) | \
	 (((unsigned long)(int1clr)) << PE_INTRCTL_INT1CLR_SHIFT))

#define SC_PE_ZMODE_SET_ENABLE(line, pe_zmode,enable) \
	FAST_GPFLAGSET(line, pe_zmode,enable,PE_ZMODE_ENABLE)

#define SC_PE_ZMODE_SET_FUNC(line, pe_zmode,func) \
	FAST_GPFLAGSET(line, pe_zmode,func,PE_ZMODE_FUNC)

#define SC_PE_ZMODE_SET_MASK(line, pe_zmode,mask) \
	FAST_GPFLAGSET(line, pe_zmode,mask,PE_ZMODE_MASK)

#define SC_PE_ZMODE_SET_PAD0(line, pe_zmode,pad0) \
	FAST_GPFLAGSET(line, pe_zmode,pad0,PE_ZMODE_PAD0)

#define SC_PE_ZMODE_SET_RID(line, pe_zmode,rid) \
	FAST_GPFLAGSET(line, pe_zmode,rid,PE_ZMODE_RID)

#define SC_PE_CMODE0_SET_BLEND_ENABLE(line, pe_cmode0,blend_enable) \
	FAST_GPFLAGSET(line, pe_cmode0,blend_enable,PE_CMODE0_BLEND_ENABLE)

#define SC_PE_CMODE0_SET_LOGICOP_ENABLE(line, pe_cmode0,logicop_enable) \
	FAST_GPFLAGSET(line, pe_cmode0,logicop_enable,PE_CMODE0_LOGICOP_ENABLE)

#define SC_PE_CMODE0_SET_DITHER_ENABLE(line, pe_cmode0,dither_enable) \
	FAST_GPFLAGSET(line, pe_cmode0,dither_enable,PE_CMODE0_DITHER_ENABLE)

#define SC_PE_CMODE0_SET_COLOR_MASK(line, pe_cmode0,color_mask) \
	FAST_GPFLAGSET(line, pe_cmode0,color_mask,PE_CMODE0_COLOR_MASK)

#define SC_PE_CMODE0_SET_ALPHA_MASK(line, pe_cmode0,alpha_mask) \
	FAST_GPFLAGSET(line, pe_cmode0,alpha_mask,PE_CMODE0_ALPHA_MASK)

#define SC_PE_CMODE0_SET_DFACTOR(line, pe_cmode0,dfactor) \
	FAST_GPFLAGSET(line, pe_cmode0,dfactor,PE_CMODE0_DFACTOR)

#define SC_PE_CMODE0_SET_SFACTOR(line, pe_cmode0,sfactor) \
	FAST_GPFLAGSET(line, pe_cmode0,sfactor,PE_CMODE0_SFACTOR)

#define SC_PE_CMODE0_SET_BLENDOP(line, pe_cmode0,blendop) \
	FAST_GPFLAGSET(line, pe_cmode0,blendop,PE_CMODE0_BLENDOP)

#define SC_PE_CMODE0_SET_LOGICOP(line, pe_cmode0,logicop) \
	FAST_GPFLAGSET(line, pe_cmode0,logicop,PE_CMODE0_LOGICOP)

#define SC_PE_CMODE0_SET_PAD0(line, pe_cmode0,pad0) \
	FAST_GPFLAGSET(line, pe_cmode0,pad0,PE_CMODE0_PAD0)

#define SC_PE_CMODE0_SET_RID(line, pe_cmode0,rid) \
	FAST_GPFLAGSET(line, pe_cmode0,rid,PE_CMODE0_RID)

#define SC_PE_CMODE1_SET_CONSTANT_ALPHA(line, pe_cmode1,constant_alpha) \
	FAST_GPFLAGSET(line, pe_cmode1,constant_alpha,PE_CMODE1_CONSTANT_ALPHA)

#define SC_PE_CMODE1_SET_CONSTANT_ALPHA_ENABLE(line, pe_cmode1,constant_alpha_enable) \
	FAST_GPFLAGSET(line, pe_cmode1,constant_alpha_enable,PE_CMODE1_CONSTANT_ALPHA_ENABLE)

#define SC_PE_CMODE1_SET_YUV(line, pe_cmode1,yuv) \
	FAST_GPFLAGSET(line, pe_cmode1,yuv,PE_CMODE1_YUV)

#define SC_PE_CMODE1_SET_PAD0(line, pe_cmode1,pad0) \
	FAST_GPFLAGSET(line, pe_cmode1,pad0,PE_CMODE1_PAD0)

#define SC_PE_CMODE1_SET_RID(line, pe_cmode1,rid) \
	FAST_GPFLAGSET(line, pe_cmode1,rid,PE_CMODE1_RID)

#define SC_PE_CONTROL_SET_PIXTYPE(line, pe_control,pixtype) \
	FAST_GPFLAGSET(line, pe_control,pixtype,PE_CONTROL_PIXTYPE)

#define SC_PE_CONTROL_SET_ZCMODE(line, pe_control,zcmode) \
	FAST_GPFLAGSET(line, pe_control,zcmode,PE_CONTROL_ZCMODE)

#define SC_PE_CONTROL_SET_ZTOP(line, pe_control,ztop) \
	FAST_GPFLAGSET(line, pe_control,ztop,PE_CONTROL_ZTOP)

#define SC_PE_CONTROL_SET_PAD0(line, pe_control,pad0) \
	FAST_GPFLAGSET(line, pe_control,pad0,PE_CONTROL_PAD0)

#define SC_PE_CONTROL_SET_RID(line, pe_control,rid) \
	FAST_GPFLAGSET(line, pe_control,rid,PE_CONTROL_RID)

#define SC_PE_FIELD_MASK_SET_EVEN(line, pe_field_mask,even) \
	FAST_GPFLAGSET(line, pe_field_mask,even,PE_FIELD_MASK_EVEN)

#define SC_PE_FIELD_MASK_SET_ODD(line, pe_field_mask,odd) \
	FAST_GPFLAGSET(line, pe_field_mask,odd,PE_FIELD_MASK_ODD)

#define SC_PE_FIELD_MASK_SET_PAD0(line, pe_field_mask,pad0) \
	FAST_GPFLAGSET(line, pe_field_mask,pad0,PE_FIELD_MASK_PAD0)

#define SC_PE_FIELD_MASK_SET_RID(line, pe_field_mask,rid) \
	FAST_GPFLAGSET(line, pe_field_mask,rid,PE_FIELD_MASK_RID)

#define SC_PE_FINISH_SET_DST(line, pe_finish,dst) \
	FAST_GPFLAGSET(line, pe_finish,dst,PE_FINISH_DST)

#define SC_PE_FINISH_SET_PAD0(line, pe_finish,pad0) \
	FAST_GPFLAGSET(line, pe_finish,pad0,PE_FINISH_PAD0)

#define SC_PE_FINISH_SET_RID(line, pe_finish,rid) \
	FAST_GPFLAGSET(line, pe_finish,rid,PE_FINISH_RID)

#define SC_PE_REFRESH_SET_INTERVAL(line, pe_refresh,interval) \
	FAST_GPFLAGSET(line, pe_refresh,interval,PE_REFRESH_INTERVAL)

#define SC_PE_REFRESH_SET_ENABLE(line, pe_refresh,enable) \
	FAST_GPFLAGSET(line, pe_refresh,enable,PE_REFRESH_ENABLE)

#define SC_PE_REFRESH_SET_PAD0(line, pe_refresh,pad0) \
	FAST_GPFLAGSET(line, pe_refresh,pad0,PE_REFRESH_PAD0)

#define SC_PE_REFRESH_SET_RID(line, pe_refresh,rid) \
	FAST_GPFLAGSET(line, pe_refresh,rid,PE_REFRESH_RID)

#define SC_PE_TOKEN_SET_TOKEN(line, pe_token,token) \
	FAST_GPFLAGSET(line, pe_token,token,PE_TOKEN_TOKEN)

#define SC_PE_TOKEN_SET_PAD0(line, pe_token,pad0) \
	FAST_GPFLAGSET(line, pe_token,pad0,PE_TOKEN_PAD0)

#define SC_PE_TOKEN_SET_RID(line, pe_token,rid) \
	FAST_GPFLAGSET(line, pe_token,rid,PE_TOKEN_RID)

#define SC_PE_TOKEN_INT_SET_TOKEN(line, pe_token_int,token) \
	FAST_GPFLAGSET(line, pe_token_int,token,PE_TOKEN_INT_TOKEN)

#define SC_PE_TOKEN_INT_SET_PAD0(line, pe_token_int,pad0) \
	FAST_GPFLAGSET(line, pe_token_int,pad0,PE_TOKEN_INT_PAD0)

#define SC_PE_TOKEN_INT_SET_RID(line, pe_token_int,rid) \
	FAST_GPFLAGSET(line, pe_token_int,rid,PE_TOKEN_INT_RID)

#define SC_PE_COPY_SRC_ADDR_SET_X(line, pe_copy_src_addr,x) \
	FAST_GPFLAGSET(line, pe_copy_src_addr,x,PE_COPY_SRC_ADDR_X)

#define SC_PE_COPY_SRC_ADDR_SET_Y(line, pe_copy_src_addr,y) \
	FAST_GPFLAGSET(line, pe_copy_src_addr,y,PE_COPY_SRC_ADDR_Y)

#define SC_PE_COPY_SRC_ADDR_SET_PAD0(line, pe_copy_src_addr,pad0) \
	FAST_GPFLAGSET(line, pe_copy_src_addr,pad0,PE_COPY_SRC_ADDR_PAD0)

#define SC_PE_COPY_SRC_ADDR_SET_RID(line, pe_copy_src_addr,rid) \
	FAST_GPFLAGSET(line, pe_copy_src_addr,rid,PE_COPY_SRC_ADDR_RID)

#define SC_PE_COPY_SRC_SIZE_SET_X(line, pe_copy_src_size,x) \
	FAST_GPFLAGSET(line, pe_copy_src_size,x,PE_COPY_SRC_SIZE_X)

#define SC_PE_COPY_SRC_SIZE_SET_Y(line, pe_copy_src_size,y) \
	FAST_GPFLAGSET(line, pe_copy_src_size,y,PE_COPY_SRC_SIZE_Y)

#define SC_PE_COPY_SRC_SIZE_SET_PAD0(line, pe_copy_src_size,pad0) \
	FAST_GPFLAGSET(line, pe_copy_src_size,pad0,PE_COPY_SRC_SIZE_PAD0)

#define SC_PE_COPY_SRC_SIZE_SET_RID(line, pe_copy_src_size,rid) \
	FAST_GPFLAGSET(line, pe_copy_src_size,rid,PE_COPY_SRC_SIZE_RID)

#define SC_PE_COPY_DST_BASE_SET_BASE(line, pe_copy_dst_base,base) \
	FAST_GPFLAGSET(line, pe_copy_dst_base,base,PE_COPY_DST_BASE_BASE)

#define SC_PE_COPY_DST_BASE_SET_PAD0(line, pe_copy_dst_base,pad0) \
	FAST_GPFLAGSET(line, pe_copy_dst_base,pad0,PE_COPY_DST_BASE_PAD0)

#define SC_PE_COPY_DST_BASE_SET_RID(line, pe_copy_dst_base,rid) \
	FAST_GPFLAGSET(line, pe_copy_dst_base,rid,PE_COPY_DST_BASE_RID)

#define SC_PE_COPY_DST_STRIDE_SET_STRIDE(line, pe_copy_dst_stride,stride) \
	FAST_GPFLAGSET(line, pe_copy_dst_stride,stride,PE_COPY_DST_STRIDE_STRIDE)

#define SC_PE_COPY_DST_STRIDE_SET_PAD0(line, pe_copy_dst_stride,pad0) \
	FAST_GPFLAGSET(line, pe_copy_dst_stride,pad0,PE_COPY_DST_STRIDE_PAD0)

#define SC_PE_COPY_DST_STRIDE_SET_RID(line, pe_copy_dst_stride,rid) \
	FAST_GPFLAGSET(line, pe_copy_dst_stride,rid,PE_COPY_DST_STRIDE_RID)

#define SC_PE_COPY_SCALE_SET_SCALE(line, pe_copy_scale,scale) \
	FAST_GPFLAGSET(line, pe_copy_scale,scale,PE_COPY_SCALE_SCALE)

#define SC_PE_COPY_SCALE_SET_PAD0(line, pe_copy_scale,pad0) \
	FAST_GPFLAGSET(line, pe_copy_scale,pad0,PE_COPY_SCALE_PAD0)

#define SC_PE_COPY_SCALE_SET_RID(line, pe_copy_scale,rid) \
	FAST_GPFLAGSET(line, pe_copy_scale,rid,PE_COPY_SCALE_RID)

#define SC_PE_COPY_CLEAR_COLOR_AR_SET_RED(line, pe_copy_clear_color_ar,red) \
	FAST_GPFLAGSET(line, pe_copy_clear_color_ar,red,PE_COPY_CLEAR_COLOR_AR_RED)

#define SC_PE_COPY_CLEAR_COLOR_AR_SET_ALPHA(line, pe_copy_clear_color_ar,alpha) \
	FAST_GPFLAGSET(line, pe_copy_clear_color_ar,alpha,PE_COPY_CLEAR_COLOR_AR_ALPHA)

#define SC_PE_COPY_CLEAR_COLOR_AR_SET_PAD0(line, pe_copy_clear_color_ar,pad0) \
	FAST_GPFLAGSET(line, pe_copy_clear_color_ar,pad0,PE_COPY_CLEAR_COLOR_AR_PAD0)

#define SC_PE_COPY_CLEAR_COLOR_AR_SET_RID(line, pe_copy_clear_color_ar,rid) \
	FAST_GPFLAGSET(line, pe_copy_clear_color_ar,rid,PE_COPY_CLEAR_COLOR_AR_RID)

#define SC_PE_COPY_CLEAR_COLOR_GB_SET_BLUE(line, pe_copy_clear_color_gb,blue) \
	FAST_GPFLAGSET(line, pe_copy_clear_color_gb,blue,PE_COPY_CLEAR_COLOR_GB_BLUE)

#define SC_PE_COPY_CLEAR_COLOR_GB_SET_GREEN(line, pe_copy_clear_color_gb,green) \
	FAST_GPFLAGSET(line, pe_copy_clear_color_gb,green,PE_COPY_CLEAR_COLOR_GB_GREEN)

#define SC_PE_COPY_CLEAR_COLOR_GB_SET_PAD0(line, pe_copy_clear_color_gb,pad0) \
	FAST_GPFLAGSET(line, pe_copy_clear_color_gb,pad0,PE_COPY_CLEAR_COLOR_GB_PAD0)

#define SC_PE_COPY_CLEAR_COLOR_GB_SET_RID(line, pe_copy_clear_color_gb,rid) \
	FAST_GPFLAGSET(line, pe_copy_clear_color_gb,rid,PE_COPY_CLEAR_COLOR_GB_RID)

#define SC_PE_COPY_CLEAR_Z_SET_DATA(line, pe_copy_clear_z,data) \
	FAST_GPFLAGSET(line, pe_copy_clear_z,data,PE_COPY_CLEAR_Z_DATA)

#define SC_PE_COPY_CLEAR_Z_SET_RID(line, pe_copy_clear_z,rid) \
	FAST_GPFLAGSET(line, pe_copy_clear_z,rid,PE_COPY_CLEAR_Z_RID)

#define SC_PE_COPY_CMD_SET_CLAMP_TOP(line, pe_copy_cmd,clamp_top) \
	FAST_GPFLAGSET(line, pe_copy_cmd,clamp_top,PE_COPY_CMD_CLAMP_TOP)

#define SC_PE_COPY_CMD_SET_CLAMP_BOTTOM(line, pe_copy_cmd,clamp_bottom) \
	FAST_GPFLAGSET(line, pe_copy_cmd,clamp_bottom,PE_COPY_CMD_CLAMP_BOTTOM)

#define SC_PE_COPY_CMD_SET_PAD0(line, pe_copy_cmd,pad0) \
	FAST_GPFLAGSET(line, pe_copy_cmd,pad0,PE_COPY_CMD_PAD0)

#define SC_PE_COPY_CMD_SET_TEX_FORMATH(line, pe_copy_cmd,tex_formatH) \
	FAST_GPFLAGSET(line, pe_copy_cmd,tex_formatH,PE_COPY_CMD_TEX_FORMATH)

#define SC_PE_COPY_CMD_SET_TEX_FORMAT(line, pe_copy_cmd,tex_format) \
	FAST_GPFLAGSET(line, pe_copy_cmd,tex_format,PE_COPY_CMD_TEX_FORMAT)

#define SC_PE_COPY_CMD_SET_GAMMA(line, pe_copy_cmd,gamma) \
	FAST_GPFLAGSET(line, pe_copy_cmd,gamma,PE_COPY_CMD_GAMMA)

#define SC_PE_COPY_CMD_SET_MIP_MAP_FILTER(line, pe_copy_cmd,mip_map_filter) \
	FAST_GPFLAGSET(line, pe_copy_cmd,mip_map_filter,PE_COPY_CMD_MIP_MAP_FILTER)

#define SC_PE_COPY_CMD_SET_VERTICAL_SCALE(line, pe_copy_cmd,vertical_scale) \
	FAST_GPFLAGSET(line, pe_copy_cmd,vertical_scale,PE_COPY_CMD_VERTICAL_SCALE)

#define SC_PE_COPY_CMD_SET_CLEAR(line, pe_copy_cmd,clear) \
	FAST_GPFLAGSET(line, pe_copy_cmd,clear,PE_COPY_CMD_CLEAR)

#define SC_PE_COPY_CMD_SET_INTERLACED(line, pe_copy_cmd,interlaced) \
	FAST_GPFLAGSET(line, pe_copy_cmd,interlaced,PE_COPY_CMD_INTERLACED)

#define SC_PE_COPY_CMD_SET_OPCODE(line, pe_copy_cmd,opcode) \
	FAST_GPFLAGSET(line, pe_copy_cmd,opcode,PE_COPY_CMD_OPCODE)

#define SC_PE_COPY_CMD_SET_CCV(line, pe_copy_cmd,ccv) \
	FAST_GPFLAGSET(line, pe_copy_cmd,ccv,PE_COPY_CMD_CCV)

#define SC_PE_COPY_CMD_SET_PAD1(line, pe_copy_cmd,pad1) \
	FAST_GPFLAGSET(line, pe_copy_cmd,pad1,PE_COPY_CMD_PAD1)

#define SC_PE_COPY_CMD_SET_RID(line, pe_copy_cmd,rid) \
	FAST_GPFLAGSET(line, pe_copy_cmd,rid,PE_COPY_CMD_RID)

#define SC_PE_COPY_VFILTER0_SET_COEFF0(line, pe_copy_vfilter0,coeff0) \
	FAST_GPFLAGSET(line, pe_copy_vfilter0,coeff0,PE_COPY_VFILTER0_COEFF0)

#define SC_PE_COPY_VFILTER0_SET_COEFF1(line, pe_copy_vfilter0,coeff1) \
	FAST_GPFLAGSET(line, pe_copy_vfilter0,coeff1,PE_COPY_VFILTER0_COEFF1)

#define SC_PE_COPY_VFILTER0_SET_COEFF2(line, pe_copy_vfilter0,coeff2) \
	FAST_GPFLAGSET(line, pe_copy_vfilter0,coeff2,PE_COPY_VFILTER0_COEFF2)

#define SC_PE_COPY_VFILTER0_SET_COEFF3(line, pe_copy_vfilter0,coeff3) \
	FAST_GPFLAGSET(line, pe_copy_vfilter0,coeff3,PE_COPY_VFILTER0_COEFF3)

#define SC_PE_COPY_VFILTER0_SET_RID(line, pe_copy_vfilter0,rid) \
	FAST_GPFLAGSET(line, pe_copy_vfilter0,rid,PE_COPY_VFILTER0_RID)

#define SC_PE_COPY_VFILTER1_SET_COEFF4(line, pe_copy_vfilter1,coeff4) \
	FAST_GPFLAGSET(line, pe_copy_vfilter1,coeff4,PE_COPY_VFILTER1_COEFF4)

#define SC_PE_COPY_VFILTER1_SET_COEFF5(line, pe_copy_vfilter1,coeff5) \
	FAST_GPFLAGSET(line, pe_copy_vfilter1,coeff5,PE_COPY_VFILTER1_COEFF5)

#define SC_PE_COPY_VFILTER1_SET_COEFF6(line, pe_copy_vfilter1,coeff6) \
	FAST_GPFLAGSET(line, pe_copy_vfilter1,coeff6,PE_COPY_VFILTER1_COEFF6)

#define SC_PE_COPY_VFILTER1_SET_PAD0(line, pe_copy_vfilter1,pad0) \
	FAST_GPFLAGSET(line, pe_copy_vfilter1,pad0,PE_COPY_VFILTER1_PAD0)

#define SC_PE_COPY_VFILTER1_SET_RID(line, pe_copy_vfilter1,rid) \
	FAST_GPFLAGSET(line, pe_copy_vfilter1,rid,PE_COPY_VFILTER1_RID)

#define SC_PE_XBOUND_SET_LEFT(line, pe_xbound,left) \
	FAST_GPFLAGSET(line, pe_xbound,left,PE_XBOUND_LEFT)

#define SC_PE_XBOUND_SET_RIGHT(line, pe_xbound,right) \
	FAST_GPFLAGSET(line, pe_xbound,right,PE_XBOUND_RIGHT)

#define SC_PE_XBOUND_SET_PAD0(line, pe_xbound,pad0) \
	FAST_GPFLAGSET(line, pe_xbound,pad0,PE_XBOUND_PAD0)

#define SC_PE_XBOUND_SET_RID(line, pe_xbound,rid) \
	FAST_GPFLAGSET(line, pe_xbound,rid,PE_XBOUND_RID)

#define SC_PE_YBOUND_SET_TOP(line, pe_ybound,top) \
	FAST_GPFLAGSET(line, pe_ybound,top,PE_YBOUND_TOP)

#define SC_PE_YBOUND_SET_BOTTOM(line, pe_ybound,bottom) \
	FAST_GPFLAGSET(line, pe_ybound,bottom,PE_YBOUND_BOTTOM)

#define SC_PE_YBOUND_SET_PAD0(line, pe_ybound,pad0) \
	FAST_GPFLAGSET(line, pe_ybound,pad0,PE_YBOUND_PAD0)

#define SC_PE_YBOUND_SET_RID(line, pe_ybound,rid) \
	FAST_GPFLAGSET(line, pe_ybound,rid,PE_YBOUND_RID)

#define SC_PE_PERFMODE_SET_CNTR0(line, pe_perfmode,cntr0) \
	FAST_GPFLAGSET(line, pe_perfmode,cntr0,PE_PERFMODE_CNTR0)

#define SC_PE_PERFMODE_SET_CNTR1(line, pe_perfmode,cntr1) \
	FAST_GPFLAGSET(line, pe_perfmode,cntr1,PE_PERFMODE_CNTR1)

#define SC_PE_PERFMODE_SET_CNTR2(line, pe_perfmode,cntr2) \
	FAST_GPFLAGSET(line, pe_perfmode,cntr2,PE_PERFMODE_CNTR2)

#define SC_PE_PERFMODE_SET_CNTR3(line, pe_perfmode,cntr3) \
	FAST_GPFLAGSET(line, pe_perfmode,cntr3,PE_PERFMODE_CNTR3)

#define SC_PE_PERFMODE_SET_CNTR4(line, pe_perfmode,cntr4) \
	FAST_GPFLAGSET(line, pe_perfmode,cntr4,PE_PERFMODE_CNTR4)

#define SC_PE_PERFMODE_SET_CNTR5(line, pe_perfmode,cntr5) \
	FAST_GPFLAGSET(line, pe_perfmode,cntr5,PE_PERFMODE_CNTR5)

#define SC_PE_PERFMODE_SET_PAD0(line, pe_perfmode,pad0) \
	FAST_GPFLAGSET(line, pe_perfmode,pad0,PE_PERFMODE_PAD0)

#define SC_PE_PERFMODE_SET_RID(line, pe_perfmode,rid) \
	FAST_GPFLAGSET(line, pe_perfmode,rid,PE_PERFMODE_RID)

#define SC_PE_CHICKEN_SET_PIWR(line, pe_chicken,piwr) \
	FAST_GPFLAGSET(line, pe_chicken,piwr,PE_CHICKEN_PIWR)

#define SC_PE_CHICKEN_SET_TXCPY_FMT(line, pe_chicken,txcpy_fmt) \
	FAST_GPFLAGSET(line, pe_chicken,txcpy_fmt,PE_CHICKEN_TXCPY_FMT)

#define SC_PE_CHICKEN_SET_TXCPY_CCV(line, pe_chicken,txcpy_ccv) \
	FAST_GPFLAGSET(line, pe_chicken,txcpy_ccv,PE_CHICKEN_TXCPY_CCV)

#define SC_PE_CHICKEN_SET_BLENDOP(line, pe_chicken,blendop) \
	FAST_GPFLAGSET(line, pe_chicken,blendop,PE_CHICKEN_BLENDOP)

#define SC_PE_CHICKEN_SET_PAD0(line, pe_chicken,pad0) \
	FAST_GPFLAGSET(line, pe_chicken,pad0,PE_CHICKEN_PAD0)

#define SC_PE_CHICKEN_SET_RID(line, pe_chicken,rid) \
	FAST_GPFLAGSET(line, pe_chicken,rid,PE_CHICKEN_RID)

#define SC_PE_QUAD_OFFSET_SET_X(line, pe_quad_offset,x) \
	FAST_GPFLAGSET(line, pe_quad_offset,x,PE_QUAD_OFFSET_X)

#define SC_PE_QUAD_OFFSET_SET_Y(line, pe_quad_offset,y) \
	FAST_GPFLAGSET(line, pe_quad_offset,y,PE_QUAD_OFFSET_Y)

#define SC_PE_QUAD_OFFSET_SET_PAD0(line, pe_quad_offset,pad0) \
	FAST_GPFLAGSET(line, pe_quad_offset,pad0,PE_QUAD_OFFSET_PAD0)

#define SC_PE_QUAD_OFFSET_SET_RID(line, pe_quad_offset,rid) \
	FAST_GPFLAGSET(line, pe_quad_offset,rid,PE_QUAD_OFFSET_RID)

#define SC_PE_COLOR_RGB8_SET_BLUE(line, pe_color_rgb8,blue) \
	FAST_GPFLAGSET(line, pe_color_rgb8,blue,PE_COLOR_RGB8_BLUE)

#define SC_PE_COLOR_RGB8_SET_GREEN(line, pe_color_rgb8,green) \
	FAST_GPFLAGSET(line, pe_color_rgb8,green,PE_COLOR_RGB8_GREEN)

#define SC_PE_COLOR_RGB8_SET_RED(line, pe_color_rgb8,red) \
	FAST_GPFLAGSET(line, pe_color_rgb8,red,PE_COLOR_RGB8_RED)

#define SC_PE_COLOR_RGB8_SET_PAD0(line, pe_color_rgb8,pad0) \
	FAST_GPFLAGSET(line, pe_color_rgb8,pad0,PE_COLOR_RGB8_PAD0)

#define SC_PE_COLOR_RGBA6_SET_ALPHA(line, pe_color_rgba6,alpha) \
	FAST_GPFLAGSET(line, pe_color_rgba6,alpha,PE_COLOR_RGBA6_ALPHA)

#define SC_PE_COLOR_RGBA6_SET_BLUE(line, pe_color_rgba6,blue) \
	FAST_GPFLAGSET(line, pe_color_rgba6,blue,PE_COLOR_RGBA6_BLUE)

#define SC_PE_COLOR_RGBA6_SET_GREEN(line, pe_color_rgba6,green) \
	FAST_GPFLAGSET(line, pe_color_rgba6,green,PE_COLOR_RGBA6_GREEN)

#define SC_PE_COLOR_RGBA6_SET_RED(line, pe_color_rgba6,red) \
	FAST_GPFLAGSET(line, pe_color_rgba6,red,PE_COLOR_RGBA6_RED)

#define SC_PE_COLOR_RGBA6_SET_PAD0(line, pe_color_rgba6,pad0) \
	FAST_GPFLAGSET(line, pe_color_rgba6,pad0,PE_COLOR_RGBA6_PAD0)

#define SC_PE_COLOR_RGBAA_SET_BLUE(line, pe_color_rgbaa,blue) \
	FAST_GPFLAGSET(line, pe_color_rgbaa,blue,PE_COLOR_RGBAA_BLUE)

#define SC_PE_COLOR_RGBAA_SET_GREEN(line, pe_color_rgbaa,green) \
	FAST_GPFLAGSET(line, pe_color_rgbaa,green,PE_COLOR_RGBAA_GREEN)

#define SC_PE_COLOR_RGBAA_SET_RED(line, pe_color_rgbaa,red) \
	FAST_GPFLAGSET(line, pe_color_rgbaa,red,PE_COLOR_RGBAA_RED)

#define SC_PE_TEX_COPY_I4_SET_I1(line, pe_tex_copy_i4,i1) \
	FAST_GPFLAGSET(line, pe_tex_copy_i4,i1,PE_TEX_COPY_I4_I1)

#define SC_PE_TEX_COPY_I4_SET_I0(line, pe_tex_copy_i4,i0) \
	FAST_GPFLAGSET(line, pe_tex_copy_i4,i0,PE_TEX_COPY_I4_I0)

#define SC_PE_TEX_COPY_IA4_SET_I(line, pe_tex_copy_ia4,i) \
	FAST_GPFLAGSET(line, pe_tex_copy_ia4,i,PE_TEX_COPY_IA4_I)

#define SC_PE_TEX_COPY_IA4_SET_ALPHA(line, pe_tex_copy_ia4,alpha) \
	FAST_GPFLAGSET(line, pe_tex_copy_ia4,alpha,PE_TEX_COPY_IA4_ALPHA)

#define SC_PE_TEX_COPY_IA8_SET_I(line, pe_tex_copy_ia8,i) \
	FAST_GPFLAGSET(line, pe_tex_copy_ia8,i,PE_TEX_COPY_IA8_I)

#define SC_PE_TEX_COPY_IA8_SET_ALPHA(line, pe_tex_copy_ia8,alpha) \
	FAST_GPFLAGSET(line, pe_tex_copy_ia8,alpha,PE_TEX_COPY_IA8_ALPHA)

#define SC_PE_TEX_COPY_R5G6B5_SET_BLUE(line, pe_tex_copy_r5g6b5,blue) \
	FAST_GPFLAGSET(line, pe_tex_copy_r5g6b5,blue,PE_TEX_COPY_R5G6B5_BLUE)

#define SC_PE_TEX_COPY_R5G6B5_SET_GREEN(line, pe_tex_copy_r5g6b5,green) \
	FAST_GPFLAGSET(line, pe_tex_copy_r5g6b5,green,PE_TEX_COPY_R5G6B5_GREEN)

#define SC_PE_TEX_COPY_R5G6B5_SET_RED(line, pe_tex_copy_r5g6b5,red) \
	FAST_GPFLAGSET(line, pe_tex_copy_r5g6b5,red,PE_TEX_COPY_R5G6B5_RED)

#define SC_PE_TEX_COPY_RGB5_SET_BLUE(line, pe_tex_copy_rgb5,blue) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgb5,blue,PE_TEX_COPY_RGB5_BLUE)

#define SC_PE_TEX_COPY_RGB5_SET_GREEN(line, pe_tex_copy_rgb5,green) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgb5,green,PE_TEX_COPY_RGB5_GREEN)

#define SC_PE_TEX_COPY_RGB5_SET_RED(line, pe_tex_copy_rgb5,red) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgb5,red,PE_TEX_COPY_RGB5_RED)

#define SC_PE_TEX_COPY_RGB5_SET_FORMAT(line, pe_tex_copy_rgb5,format) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgb5,format,PE_TEX_COPY_RGB5_FORMAT)

#define SC_PE_TEX_COPY_RGB4A3_SET_BLUE(line, pe_tex_copy_rgb4a3,blue) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgb4a3,blue,PE_TEX_COPY_RGB4A3_BLUE)

#define SC_PE_TEX_COPY_RGB4A3_SET_GREEN(line, pe_tex_copy_rgb4a3,green) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgb4a3,green,PE_TEX_COPY_RGB4A3_GREEN)

#define SC_PE_TEX_COPY_RGB4A3_SET_RED(line, pe_tex_copy_rgb4a3,red) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgb4a3,red,PE_TEX_COPY_RGB4A3_RED)

#define SC_PE_TEX_COPY_RGB4A3_SET_ALPHA(line, pe_tex_copy_rgb4a3,alpha) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgb4a3,alpha,PE_TEX_COPY_RGB4A3_ALPHA)

#define SC_PE_TEX_COPY_RGB4A3_SET_FORMAT(line, pe_tex_copy_rgb4a3,format) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgb4a3,format,PE_TEX_COPY_RGB4A3_FORMAT)

#define SC_PE_TEX_COPY_RGBA8_SET_RED(line, pe_tex_copy_rgba8,red) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgba8,red,PE_TEX_COPY_RGBA8_RED)

#define SC_PE_TEX_COPY_RGBA8_SET_ALPHA(line, pe_tex_copy_rgba8,alpha) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgba8,alpha,PE_TEX_COPY_RGBA8_ALPHA)

#define SC_PE_TEX_COPY_RGBA8_SET_BLUE(line, pe_tex_copy_rgba8,blue) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgba8,blue,PE_TEX_COPY_RGBA8_BLUE)

#define SC_PE_TEX_COPY_RGBA8_SET_GREEN(line, pe_tex_copy_rgba8,green) \
	FAST_GPFLAGSET(line, pe_tex_copy_rgba8,green,PE_TEX_COPY_RGBA8_GREEN)

#define SC_PE_MASK_SET_M0(line, pe_mask,m0) \
	FAST_GPFLAGSET(line, pe_mask,m0,PE_MASK_M0)

#define SC_PE_MASK_SET_M1(line, pe_mask,m1) \
	FAST_GPFLAGSET(line, pe_mask,m1,PE_MASK_M1)

#define SC_PE_MASK_SET_M2(line, pe_mask,m2) \
	FAST_GPFLAGSET(line, pe_mask,m2,PE_MASK_M2)

#define SC_PE_MASK_SET_M3(line, pe_mask,m3) \
	FAST_GPFLAGSET(line, pe_mask,m3,PE_MASK_M3)

#define SC_PE_MISC_SET_V0(line, pe_misc,v0) \
	FAST_GPFLAGSET(line, pe_misc,v0,PE_MISC_V0)

#define SC_PE_MISC_SET_V1(line, pe_misc,v1) \
	FAST_GPFLAGSET(line, pe_misc,v1,PE_MISC_V1)

#define SC_PE_MISC_SET_V2(line, pe_misc,v2) \
	FAST_GPFLAGSET(line, pe_misc,v2,PE_MISC_V2)

#define SC_PE_MISC_SET_V3(line, pe_misc,v3) \
	FAST_GPFLAGSET(line, pe_misc,v3,PE_MISC_V3)

#define SC_PE_MISC_SET_ST(line, pe_misc,st) \
	FAST_GPFLAGSET(line, pe_misc,st,PE_MISC_ST)

#define SC_PE_MISC_SET_SB(line, pe_misc,sb) \
	FAST_GPFLAGSET(line, pe_misc,sb,PE_MISC_SB)

#define SC_PE_MISC_SET_SL(line, pe_misc,sl) \
	FAST_GPFLAGSET(line, pe_misc,sl,PE_MISC_SL)

#define SC_PE_MISC_SET_SR(line, pe_misc,sr) \
	FAST_GPFLAGSET(line, pe_misc,sr,PE_MISC_SR)

#define SC_PE_MISC_SET_TT(line, pe_misc,tt) \
	FAST_GPFLAGSET(line, pe_misc,tt,PE_MISC_TT)

#define SC_PE_MISC_SET_TB(line, pe_misc,tb) \
	FAST_GPFLAGSET(line, pe_misc,tb,PE_MISC_TB)

#define SC_PE_MISC_SET_TL(line, pe_misc,tl) \
	FAST_GPFLAGSET(line, pe_misc,tl,PE_MISC_TL)

#define SC_PE_MISC_SET_TR(line, pe_misc,tr) \
	FAST_GPFLAGSET(line, pe_misc,tr,PE_MISC_TR)

#define SC_PE_MISC_SET_TM(line, pe_misc,tm) \
	FAST_GPFLAGSET(line, pe_misc,tm,PE_MISC_TM)

#define SC_PE_MISC_SET_TP(line, pe_misc,tp) \
	FAST_GPFLAGSET(line, pe_misc,tp,PE_MISC_TP)

#define SC_PE_MISC_SET_SV(line, pe_misc,sv) \
	FAST_GPFLAGSET(line, pe_misc,sv,PE_MISC_SV)

#define SC_PE_TAG_SET_CYCLE(line, pe_tag,cycle) \
	FAST_GPFLAGSET(line, pe_tag,cycle,PE_TAG_CYCLE)

#define SC_PE_TAG_SET_YEC(line, pe_tag,yec) \
	FAST_GPFLAGSET(line, pe_tag,yec,PE_TAG_YEC)

#define SC_PE_TAG_SET_YOC(line, pe_tag,yoc) \
	FAST_GPFLAGSET(line, pe_tag,yoc,PE_TAG_YOC)

#define SC_PE_TAG_SET_XOC(line, pe_tag,xoc) \
	FAST_GPFLAGSET(line, pe_tag,xoc,PE_TAG_XOC)

#define SC_PE_TAG_SET_CLR(line, pe_tag,clr) \
	FAST_GPFLAGSET(line, pe_tag,clr,PE_TAG_CLR)

#define SC_PE_CMD_SET_VALID(line, pe_cmd,valid) \
	FAST_GPFLAGSET(line, pe_cmd,valid,PE_CMD_VALID)

#define SC_PE_CMD_SET_OP(line, pe_cmd,op) \
	FAST_GPFLAGSET(line, pe_cmd,op,PE_CMD_OP)

#define SC_PE_CMD_SET_BANKA(line, pe_cmd,bankA) \
	FAST_GPFLAGSET(line, pe_cmd,bankA,PE_CMD_BANKA)

#define SC_PE_CMD_SET_BANKB(line, pe_cmd,bankB) \
	FAST_GPFLAGSET(line, pe_cmd,bankB,PE_CMD_BANKB)

#define SC_PE_INTRCTL_SET_INT0EN(line, pe_intrctl,int0en) \
	FAST_GPFLAGSET(line, pe_intrctl,int0en,PE_INTRCTL_INT0EN)

#define SC_PE_INTRCTL_SET_INT1EN(line, pe_intrctl,int1en) \
	FAST_GPFLAGSET(line, pe_intrctl,int1en,PE_INTRCTL_INT1EN)

#define SC_PE_INTRCTL_SET_INT0CLR(line, pe_intrctl,int0clr) \
	FAST_GPFLAGSET(line, pe_intrctl,int0clr,PE_INTRCTL_INT0CLR)

#define SC_PE_INTRCTL_SET_INT1CLR(line, pe_intrctl,int1clr) \
	FAST_GPFLAGSET(line, pe_intrctl,int1clr,PE_INTRCTL_INT1CLR)

#define SC_PE_PI_EFB_ADDR_SET_PAD0(line, pe_pi_efb_addr,pad0) \
	FAST_GPFLAGSET(line, pe_pi_efb_addr,pad0,PE_PI_EFB_ADDR_PAD0)

#define SC_PE_PI_EFB_ADDR_SET_X(line, pe_pi_efb_addr,x) \
	FAST_GPFLAGSET(line, pe_pi_efb_addr,x,PE_PI_EFB_ADDR_X)

#define SC_PE_PI_EFB_ADDR_SET_Y(line, pe_pi_efb_addr,y) \
	FAST_GPFLAGSET(line, pe_pi_efb_addr,y,PE_PI_EFB_ADDR_Y)

#define SC_PE_PI_EFB_ADDR_SET_TYPE(line, pe_pi_efb_addr,type) \
	FAST_GPFLAGSET(line, pe_pi_efb_addr,type,PE_PI_EFB_ADDR_TYPE)

#define SC_PE_PI_ZMODE_SET_ZEN(line, pe_pi_zmode,zen) \
	FAST_GPFLAGSET(line, pe_pi_zmode,zen,PE_PI_ZMODE_ZEN)

#define SC_PE_PI_ZMODE_SET_ZFUNC(line, pe_pi_zmode,zfunc) \
	FAST_GPFLAGSET(line, pe_pi_zmode,zfunc,PE_PI_ZMODE_ZFUNC)

#define SC_PE_PI_ZMODE_SET_MASK(line, pe_pi_zmode,mask) \
	FAST_GPFLAGSET(line, pe_pi_zmode,mask,PE_PI_ZMODE_MASK)

#define SC_PE_PI_ZMODE_SET_PAD0(line, pe_pi_zmode,pad0) \
	FAST_GPFLAGSET(line, pe_pi_zmode,pad0,PE_PI_ZMODE_PAD0)

#define SC_PE_PI_ZMODE_SET_RID(line, pe_pi_zmode,rid) \
	FAST_GPFLAGSET(line, pe_pi_zmode,rid,PE_PI_ZMODE_RID)

#define SC_PE_PI_CMODE0_SET_BEN(line, pe_pi_cmode0,ben) \
	FAST_GPFLAGSET(line, pe_pi_cmode0,ben,PE_PI_CMODE0_BEN)

#define SC_PE_PI_CMODE0_SET_LEN(line, pe_pi_cmode0,len) \
	FAST_GPFLAGSET(line, pe_pi_cmode0,len,PE_PI_CMODE0_LEN)

#define SC_PE_PI_CMODE0_SET_DEN(line, pe_pi_cmode0,den) \
	FAST_GPFLAGSET(line, pe_pi_cmode0,den,PE_PI_CMODE0_DEN)

#define SC_PE_PI_CMODE0_SET_CMSK(line, pe_pi_cmode0,cmsk) \
	FAST_GPFLAGSET(line, pe_pi_cmode0,cmsk,PE_PI_CMODE0_CMSK)

#define SC_PE_PI_CMODE0_SET_AMSK(line, pe_pi_cmode0,amsk) \
	FAST_GPFLAGSET(line, pe_pi_cmode0,amsk,PE_PI_CMODE0_AMSK)

#define SC_PE_PI_CMODE0_SET_DFACTOR(line, pe_pi_cmode0,dfactor) \
	FAST_GPFLAGSET(line, pe_pi_cmode0,dfactor,PE_PI_CMODE0_DFACTOR)

#define SC_PE_PI_CMODE0_SET_SFACTOR(line, pe_pi_cmode0,sfactor) \
	FAST_GPFLAGSET(line, pe_pi_cmode0,sfactor,PE_PI_CMODE0_SFACTOR)

#define SC_PE_PI_CMODE0_SET_LOGICOP(line, pe_pi_cmode0,logicop) \
	FAST_GPFLAGSET(line, pe_pi_cmode0,logicop,PE_PI_CMODE0_LOGICOP)

#define SC_PE_PI_CMODE0_SET_PAD0(line, pe_pi_cmode0,pad0) \
	FAST_GPFLAGSET(line, pe_pi_cmode0,pad0,PE_PI_CMODE0_PAD0)

#define SC_PE_PI_CMODE0_SET_RID(line, pe_pi_cmode0,rid) \
	FAST_GPFLAGSET(line, pe_pi_cmode0,rid,PE_PI_CMODE0_RID)

#define SC_PE_PI_CMODE1_SET_CONSTALPHA(line, pe_pi_cmode1,constAlpha) \
	FAST_GPFLAGSET(line, pe_pi_cmode1,constAlpha,PE_PI_CMODE1_CONSTALPHA)

#define SC_PE_PI_CMODE1_SET_EN(line, pe_pi_cmode1,en) \
	FAST_GPFLAGSET(line, pe_pi_cmode1,en,PE_PI_CMODE1_EN)

#define SC_PE_PI_CMODE1_SET_PAD0(line, pe_pi_cmode1,pad0) \
	FAST_GPFLAGSET(line, pe_pi_cmode1,pad0,PE_PI_CMODE1_PAD0)

#define SC_PE_PI_CMODE1_SET_RID(line, pe_pi_cmode1,rid) \
	FAST_GPFLAGSET(line, pe_pi_cmode1,rid,PE_PI_CMODE1_RID)

#define SC_PE_PI_ALPHA_THRESHOLD_SET_ALPHA_THRESHOLD(line, pe_pi_alpha_threshold,alpha_threshold) \
	FAST_GPFLAGSET(line, pe_pi_alpha_threshold,alpha_threshold,PE_PI_ALPHA_THRESHOLD_ALPHA_THRESHOLD)

#define SC_PE_PI_ALPHA_THRESHOLD_SET_AFUNCTION(line, pe_pi_alpha_threshold,afunction) \
	FAST_GPFLAGSET(line, pe_pi_alpha_threshold,afunction,PE_PI_ALPHA_THRESHOLD_AFUNCTION)

#define SC_PE_PI_ALPHA_THRESHOLD_SET_PAD0(line, pe_pi_alpha_threshold,pad0) \
	FAST_GPFLAGSET(line, pe_pi_alpha_threshold,pad0,PE_PI_ALPHA_THRESHOLD_PAD0)

#define SC_PE_PI_ALPHA_THRESHOLD_SET_RID(line, pe_pi_alpha_threshold,rid) \
	FAST_GPFLAGSET(line, pe_pi_alpha_threshold,rid,PE_PI_ALPHA_THRESHOLD_RID)

#define SC_PE_PI_CTL_SET_AFMT(line, pe_pi_ctl,afmt) \
	FAST_GPFLAGSET(line, pe_pi_ctl,afmt,PE_PI_CTL_AFMT)

#define SC_PE_PI_CTL_SET_ZFMT(line, pe_pi_ctl,zfmt) \
	FAST_GPFLAGSET(line, pe_pi_ctl,zfmt,PE_PI_CTL_ZFMT)

#define SC_PE_PI_CTL_SET_PAD0(line, pe_pi_ctl,pad0) \
	FAST_GPFLAGSET(line, pe_pi_ctl,pad0,PE_PI_CTL_PAD0)

#define SC_PE_PI_CTL_SET_RID(line, pe_pi_ctl,rid) \
	FAST_GPFLAGSET(line, pe_pi_ctl,rid,PE_PI_CTL_RID)

#endif // PE_REG_H
