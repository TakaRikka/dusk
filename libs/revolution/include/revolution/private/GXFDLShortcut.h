#ifndef GXFDLSHORTCUT_H
#define GXFDLSHORTCUT_H

#if __MWERKS__
#define FAST_GPFLAGSET(line, regOrg, newFlag, regName) \
do {                                                                                \
    ASSERTMSGLINE(line, ((u32)(newFlag) & ~((1 << (regName ## _SIZE)) - 1)) == 0, "GX Internal: Register field out of range"); \
    (regOrg) = (u32) __rlwimi((int) (regOrg),                                       \
                              (int) (newFlag),                                      \
                              (regName ## _SHIFT),                                  \
                              (32-(regName ## _SHIFT)-(regName ## _SIZE)),          \
                              (31-(regName ## _SHIFT)));                            \
} while (0)
#else
#define FAST_GPFLAGSET(line, regOrg, newFlag, regName)
#endif

#define CP_VCD_REG_HI_TEXALL_MASK   ((1UL<<CP_VCD_REG_HI_TOTAL_SIZE)-1)
#define CP_VCD_REG_HI_TEXALL_SHIFT  CP_VCD_REG_HI_TEX0_SHIFT
#define CP_VCD_REG_LO_COLALL_MASK   (CP_VCD_REG_LO_COL0_MASK|CP_VCD_REG_LO_COL1_MASK)
#define CP_VCD_REG_LO_COLALL_SHIFT  CP_VCD_REG_LO_COL0_SHIFT

#define TEV_REGISTERL_R8_SIZE   TEV_KREGISTERL_R_SIZE
#define TEV_REGISTERL_R8_SHIFT  TEV_KREGISTERL_R_SHIFT
#define TEV_REGISTERL_R8_MASK   TEV_KREGISTERL_R_MASK
#define SC_TEV_REGISTERL_SET_R8(line, tev_registerl, r8) \
    FAST_GPFLAGSET(line, tev_registerl, r8, TEV_REGISTERL_R8)

#define TEV_REGISTERL_A8_SIZE   TEV_KREGISTERL_A_SIZE
#define TEV_REGISTERL_A8_SHIFT  TEV_KREGISTERL_A_SHIFT
#define TEV_REGISTERL_A8_MASK   TEV_KREGISTERL_A_MASK
#define SC_TEV_REGISTERL_SET_A8(line, tev_registerl, a8) \
    FAST_GPFLAGSET(line, tev_registerl, a8, TEV_REGISTERL_A8)

#define TEV_REGISTERH_B8_SIZE   TEV_KREGISTERH_B_SIZE
#define TEV_REGISTERH_B8_SHIFT  TEV_KREGISTERH_B_SHIFT
#define TEV_REGISTERH_B8_MASK   TEV_KREGISTERH_B_MASK
#define SC_TEV_REGISTERH_SET_B8(line, tev_registerh, b8) \
    FAST_GPFLAGSET(line, tev_registerh, b8, TEV_REGISTERH_B8)

#define TEV_REGISTERH_G8_SIZE   TEV_KREGISTERH_G_SIZE
#define TEV_REGISTERH_G8_SHIFT  TEV_KREGISTERH_G_SHIFT
#define TEV_REGISTERH_G8_MASK   TEV_KREGISTERH_G_MASK
#define SC_TEV_REGISTERH_SET_G8(line, tev_registerh, g8) \
    FAST_GPFLAGSET(line, tev_registerh, g8, TEV_REGISTERH_G8)

#define TEV_REGISTERL_R8_SIZE   TEV_KREGISTERL_R_SIZE
#define TEV_REGISTERL_R8_SHIFT  TEV_KREGISTERL_R_SHIFT
#define TEV_REGISTERL_R8_MASK   TEV_KREGISTERL_R_MASK
#define SC_TEV_REGISTERL_SET_R8(line, tev_registerl, r8) \
    FAST_GPFLAGSET(line, tev_registerl, r8, TEV_REGISTERL_R8)

#define TEV_REGISTERL_A8_SIZE   TEV_KREGISTERL_A_SIZE
#define TEV_REGISTERL_A8_SHIFT  TEV_KREGISTERL_A_SHIFT
#define TEV_REGISTERL_A8_MASK   TEV_KREGISTERL_A_MASK
#define SC_TEV_REGISTERL_SET_A8(line, tev_registerl, a8) \
    FAST_GPFLAGSET(line, tev_registerl, a8, TEV_REGISTERL_A8)

#define TEV_REGISTERH_B8_SIZE   TEV_KREGISTERH_B_SIZE
#define TEV_REGISTERH_B8_SHIFT  TEV_KREGISTERH_B_SHIFT
#define TEV_REGISTERH_B8_MASK   TEV_KREGISTERH_B_MASK
#define SC_TEV_REGISTERH_SET_B8(line, tev_registerh, b8) \
    FAST_GPFLAGSET(line, tev_registerh, b8, TEV_REGISTERH_B8)

#define TEV_REGISTERH_G8_SIZE   TEV_KREGISTERH_G_SIZE
#define TEV_REGISTERH_G8_SHIFT  TEV_KREGISTERH_G_SHIFT
#define TEV_REGISTERH_G8_MASK   TEV_KREGISTERH_G_MASK
#define SC_TEV_REGISTERH_SET_G8(line, tev_registerh, g8) \
    FAST_GPFLAGSET(line, tev_registerh, g8, TEV_REGISTERH_G8)

#define XF_AMBIENT0_F_RGBA_SIZE \
    ((XF_AMBIENT0_F_ALPHA_SIZE)+(XF_AMBIENT0_F_BLUE_SIZE)+ \
     (XF_AMBIENT0_F_GREEN_SIZE)+(XF_AMBIENT0_F_RED_SIZE))
#define XF_AMBIENT0_F_RGBA_SHIFT \
    XF_AMBIENT0_F_ALPHA_SHIFT
#define XF_AMBIENT0_F_RGBA_MASK \
    ((XF_AMBIENT0_F_ALPHA_MASK)|(XF_AMBIENT0_F_BLUE_MASK)| \
     (XF_AMBIENT0_F_GREEN_MASK)|(XF_AMBIENT0_F_RED_MASK))
#define SC_XF_AMBIENT0_F_SET_RGBA(xf_ambient0_f, rgba) { \
    xf_ambient0_f = (((unsigned long)(xf_ambient0_f)) & ~XF_AMBIENT0_F_RGBA_MASK) | (((unsigned long)(rgba)) << XF_AMBIENT0_F_RGBA_SHIFT);\
}

#define XF_AMBIENT0_F_ALPHA_SIZE	8
#define XF_AMBIENT0_F_ALPHA_SHIFT	0
#define XF_AMBIENT0_F_ALPHA_MASK	0x000000ff
#define XF_AMBIENT0_F_GET_ALPHA(xf_ambient0_f) \
	((((unsigned long)(xf_ambient0_f)) & XF_AMBIENT0_F_ALPHA_MASK) >> XF_AMBIENT0_F_ALPHA_SHIFT)
#define XF_AMBIENT0_F_SET_ALPHA(xf_ambient0_f, alpha) { \
	xf_ambient0_f = (((unsigned long)(xf_ambient0_f)) & ~XF_AMBIENT0_F_ALPHA_MASK) | (((unsigned long)(alpha)) << XF_AMBIENT0_F_ALPHA_SHIFT);\
}
#define XF_AMBIENT0_F_BLUE_SIZE	8
#define XF_AMBIENT0_F_BLUE_SHIFT	8
#define XF_AMBIENT0_F_BLUE_MASK	0x0000ff00
#define XF_AMBIENT0_F_GET_BLUE(xf_ambient0_f) \
	((((unsigned long)(xf_ambient0_f)) & XF_AMBIENT0_F_BLUE_MASK) >> XF_AMBIENT0_F_BLUE_SHIFT)
#define XF_AMBIENT0_F_SET_BLUE(xf_ambient0_f, blue) { \
	xf_ambient0_f = (((unsigned long)(xf_ambient0_f)) & ~XF_AMBIENT0_F_BLUE_MASK) | (((unsigned long)(blue)) << XF_AMBIENT0_F_BLUE_SHIFT);\
}
#define XF_AMBIENT0_F_GREEN_SIZE	8
#define XF_AMBIENT0_F_GREEN_SHIFT	16
#define XF_AMBIENT0_F_GREEN_MASK	0x00ff0000
#define XF_AMBIENT0_F_GET_GREEN(xf_ambient0_f) \
	((((unsigned long)(xf_ambient0_f)) & XF_AMBIENT0_F_GREEN_MASK) >> XF_AMBIENT0_F_GREEN_SHIFT)
#define XF_AMBIENT0_F_SET_GREEN(xf_ambient0_f, green) { \
	xf_ambient0_f = (((unsigned long)(xf_ambient0_f)) & ~XF_AMBIENT0_F_GREEN_MASK) | (((unsigned long)(green)) << XF_AMBIENT0_F_GREEN_SHIFT);\
}
#define XF_AMBIENT0_F_RED_SIZE	8
#define XF_AMBIENT0_F_RED_SHIFT	24
#define XF_AMBIENT0_F_RED_MASK	0xff000000
#define XF_AMBIENT0_F_GET_RED(xf_ambient0_f) \
	((((unsigned long)(xf_ambient0_f)) & XF_AMBIENT0_F_RED_MASK) >> XF_AMBIENT0_F_RED_SHIFT)
#define XF_AMBIENT0_F_SET_RED(xf_ambient0_f, red) { \
	xf_ambient0_f = (((unsigned long)(xf_ambient0_f)) & ~XF_AMBIENT0_F_RED_MASK) | (((unsigned long)(red)) << XF_AMBIENT0_F_RED_SHIFT);\
}

#define XF_AMBIENT0_F_RGB_SIZE \
    ((XF_AMBIENT0_F_BLUE_SIZE)+(XF_AMBIENT0_F_GREEN_SIZE)+(XF_AMBIENT0_F_RED_SIZE))
#define XF_AMBIENT0_F_RGB_SHIFT \
    XF_AMBIENT0_F_BLUE_SHIFT
#define XF_AMBIENT0_F_RGB_MASK \
    ((XF_AMBIENT0_F_BLUE_MASK)|(XF_AMBIENT0_F_GREEN_MASK)|(XF_AMBIENT0_F_RED_MASK))

#define SC_XF_AMBIENT0_F_SET_RGB(line, xf_ambient0_f, rgb) \
    FAST_GPFLAGSET(line, xf_ambient0_f, rgb, XF_AMBIENT0_F_RGB)

#define XF_AMBIENT1_F_RGBA_SIZE \
    ((XF_AMBIENT1_F_ALPHA_SIZE)+(XF_AMBIENT1_F_BLUE_SIZE)+ \
     (XF_AMBIENT1_F_GREEN_SIZE)+(XF_AMBIENT1_F_RED_SIZE))
#define XF_AMBIENT1_F_RGBA_SHIFT \
    XF_AMBIENT1_F_ALPHA_SHIFT
#define XF_AMBIENT1_F_RGBA_MASK \
    ((XF_AMBIENT1_F_ALPHA_MASK)|(XF_AMBIENT1_F_BLUE_MASK)| \
     (XF_AMBIENT1_F_GREEN_MASK)|(XF_AMBIENT1_F_RED_MASK))
#define SC_XF_AMBIENT1_F_SET_RGBA(xf_ambient1_f, rgba) { \
    xf_ambient1_f = (((unsigned long)(xf_ambient1_f)) & ~XF_AMBIENT1_F_RGBA_MASK) | (((unsigned long)(rgba)) << XF_AMBIENT1_F_RGBA_SHIFT);\
}

#define XF_AMBIENT1_F_ALPHA_SIZE	8
#define XF_AMBIENT1_F_ALPHA_SHIFT	0
#define XF_AMBIENT1_F_ALPHA_MASK	0x000000ff
#define XF_AMBIENT1_F_GET_ALPHA(xf_ambient1_f) \
	((((unsigned long)(xf_ambient1_f)) & XF_AMBIENT1_F_ALPHA_MASK) >> XF_AMBIENT1_F_ALPHA_SHIFT)
#define XF_AMBIENT1_F_SET_ALPHA(xf_ambient1_f, alpha) { \
	xf_ambient1_f = (((unsigned long)(xf_ambient1_f)) & ~XF_AMBIENT1_F_ALPHA_MASK) | (((unsigned long)(alpha)) << XF_AMBIENT1_F_ALPHA_SHIFT);\
}
#define XF_AMBIENT1_F_BLUE_SIZE	8
#define XF_AMBIENT1_F_BLUE_SHIFT	8
#define XF_AMBIENT1_F_BLUE_MASK	0x0000ff00
#define XF_AMBIENT1_F_GET_BLUE(xf_ambient1_f) \
	((((unsigned long)(xf_ambient1_f)) & XF_AMBIENT1_F_BLUE_MASK) >> XF_AMBIENT1_F_BLUE_SHIFT)
#define XF_AMBIENT1_F_SET_BLUE(xf_ambient1_f, blue) { \
	xf_ambient1_f = (((unsigned long)(xf_ambient1_f)) & ~XF_AMBIENT1_F_BLUE_MASK) | (((unsigned long)(blue)) << XF_AMBIENT1_F_BLUE_SHIFT);\
}
#define XF_AMBIENT1_F_GREEN_SIZE	8
#define XF_AMBIENT1_F_GREEN_SHIFT	16
#define XF_AMBIENT1_F_GREEN_MASK	0x00ff0000
#define XF_AMBIENT1_F_GET_GREEN(xf_ambient1_f) \
	((((unsigned long)(xf_ambient1_f)) & XF_AMBIENT1_F_GREEN_MASK) >> XF_AMBIENT1_F_GREEN_SHIFT)
#define XF_AMBIENT1_F_SET_GREEN(xf_ambient1_f, green) { \
	xf_ambient1_f = (((unsigned long)(xf_ambient1_f)) & ~XF_AMBIENT1_F_GREEN_MASK) | (((unsigned long)(green)) << XF_AMBIENT1_F_GREEN_SHIFT);\
}
#define XF_AMBIENT1_F_RED_SIZE	8
#define XF_AMBIENT1_F_RED_SHIFT	24
#define XF_AMBIENT1_F_RED_MASK	0xff000000
#define XF_AMBIENT1_F_GET_RED(xf_ambient1_f) \
	((((unsigned long)(xf_ambient1_f)) & XF_AMBIENT1_F_RED_MASK) >> XF_AMBIENT1_F_RED_SHIFT)
#define XF_AMBIENT1_F_SET_RED(xf_ambient1_f, red) { \
	xf_ambient1_f = (((unsigned long)(xf_ambient1_f)) & ~XF_AMBIENT1_F_RED_MASK) | (((unsigned long)(red)) << XF_AMBIENT1_F_RED_SHIFT);\
}

#define XF_AMBIENT1_F_RGB_SIZE \
    ((XF_AMBIENT1_F_BLUE_SIZE)+(XF_AMBIENT1_F_GREEN_SIZE)+(XF_AMBIENT1_F_RED_SIZE))
#define XF_AMBIENT1_F_RGB_SHIFT \
    XF_AMBIENT1_F_BLUE_SHIFT
#define XF_AMBIENT1_F_RGB_MASK \
    ((XF_AMBIENT1_F_BLUE_MASK)|(XF_AMBIENT1_F_GREEN_MASK)|(XF_AMBIENT1_F_RED_MASK))
#define SC_XF_AMBIENT1_F_SET_RGB(line, xf_ambient1_f, rgb) \
    FAST_GPFLAGSET(line, xf_ambient1_f, rgb, XF_AMBIENT1_F_RGB)

#define XF_MATERIAL0_F_RGBA_SIZE \
    ((XF_MATERIAL0_F_ALPHA_SIZE)+(XF_MATERIAL0_F_BLUE_SIZE)+ \
     (XF_MATERIAL0_F_GREEN_SIZE)+(XF_MATERIAL0_F_RED_SIZE))
#define XF_MATERIAL0_F_RGBA_SHIFT \
    XF_MATERIAL0_F_ALPHA_SHIFT
#define XF_MATERIAL0_F_RGBA_MASK \
    ((XF_MATERIAL0_F_ALPHA_MASK)|(XF_MATERIAL0_F_BLUE_MASK)| \
     (XF_MATERIAL0_F_GREEN_MASK)|(XF_MATERIAL0_F_RED_MASK))
#define SC_XF_MATERIAL0_F_SET_RGBA(xf_material0_f, rgba) { \
    xf_material0_f = (((unsigned long)(xf_material0_f)) & ~XF_MATERIAL0_F_RGBA_MASK) | (((unsigned long)(rgba)) << XF_MATERIAL0_F_RGBA_SHIFT);\
}

#define XF_MATERIAL0_F_RGBA_SIZE \
    ((XF_MATERIAL0_F_ALPHA_SIZE)+(XF_MATERIAL0_F_BLUE_SIZE)+ \
     (XF_MATERIAL0_F_GREEN_SIZE)+(XF_MATERIAL0_F_RED_SIZE))
#define XF_MATERIAL0_F_RGBA_SHIFT \
    XF_MATERIAL0_F_ALPHA_SHIFT
#define XF_MATERIAL0_F_RGBA_MASK \
    ((XF_MATERIAL0_F_ALPHA_MASK)|(XF_MATERIAL0_F_BLUE_MASK)| \
     (XF_MATERIAL0_F_GREEN_MASK)|(XF_MATERIAL0_F_RED_MASK))

#define XF_MATERIAL0_F_RGB_SIZE \
    ((XF_MATERIAL0_F_BLUE_SIZE)+(XF_MATERIAL0_F_GREEN_SIZE)+(XF_MATERIAL0_F_RED_SIZE))
#define XF_MATERIAL0_F_RGB_SHIFT \
    XF_MATERIAL0_F_BLUE_SHIFT
#define XF_MATERIAL0_F_RGB_MASK \
    ((XF_MATERIAL0_F_BLUE_MASK)|(XF_MATERIAL0_F_GREEN_MASK)|(XF_MATERIAL0_F_RED_MASK))
#define SC_XF_MATERIAL0_F_SET_RGB(line, xf_material0_f, rgb) \
    FAST_GPFLAGSET(line, xf_material0_f, rgb, XF_MATERIAL0_F_RGB)

#define XF_MATERIAL0_F_RGB_SIZE \
    ((XF_MATERIAL0_F_BLUE_SIZE)+(XF_MATERIAL0_F_GREEN_SIZE)+(XF_MATERIAL0_F_RED_SIZE))
#define XF_MATERIAL0_F_RGB_SHIFT \
    XF_MATERIAL0_F_BLUE_SHIFT
#define XF_MATERIAL0_F_RGB_MASK \
    ((XF_MATERIAL0_F_BLUE_MASK)|(XF_MATERIAL0_F_GREEN_MASK)|(XF_MATERIAL0_F_RED_MASK))
#define SC_XF_MATERIAL0_F_SET_RGB(line, xf_material0_f, rgb) \
    FAST_GPFLAGSET(line, xf_material0_f, rgb, XF_MATERIAL0_F_RGB)

#define XF_MATERIAL1_F_RGBA_SIZE \
    ((XF_MATERIAL1_F_ALPHA_SIZE)+(XF_MATERIAL1_F_BLUE_SIZE)+ \
     (XF_MATERIAL1_F_GREEN_SIZE)+(XF_MATERIAL1_F_RED_SIZE))
#define XF_MATERIAL1_F_RGBA_SHIFT \
    XF_MATERIAL1_F_ALPHA_SHIFT
#define XF_MATERIAL1_F_RGBA_MASK \
    ((XF_MATERIAL1_F_ALPHA_MASK)|(XF_MATERIAL1_F_BLUE_MASK)| \
     (XF_MATERIAL1_F_GREEN_MASK)|(XF_MATERIAL1_F_RED_MASK))
#define SC_XF_MATERIAL1_F_SET_RGBA(xf_material1_f, rgba) { \
    xf_material1_f = (((unsigned long)(xf_material1_f)) & ~XF_MATERIAL1_F_RGBA_MASK) | (((unsigned long)(rgba)) << XF_MATERIAL1_F_RGBA_SHIFT);\
}


#define XF_MATERIAL1_F_RGB_SIZE \
    ((XF_MATERIAL1_F_BLUE_SIZE)+(XF_MATERIAL1_F_GREEN_SIZE)+(XF_MATERIAL1_F_RED_SIZE))
#define XF_MATERIAL1_F_RGB_SHIFT \
    XF_MATERIAL1_F_BLUE_SHIFT
#define XF_MATERIAL1_F_RGB_MASK \
    ((XF_MATERIAL1_F_BLUE_MASK)|(XF_MATERIAL1_F_GREEN_MASK)|(XF_MATERIAL1_F_RED_MASK))
#define SC_XF_MATERIAL1_F_SET_RGB(line, xf_material1_f, rgb) \
    FAST_GPFLAGSET(line, xf_material1_f, rgb, XF_MATERIAL1_F_RGB)

#define XF_COLOR0CNTRL_F_LIGHT0123_SIZE \
    ((XF_COLOR0CNTRL_F_LIGHT0_SIZE)+(XF_COLOR0CNTRL_F_LIGHT1_SIZE)+ \
     (XF_COLOR0CNTRL_F_LIGHT2_SIZE)+(XF_COLOR0CNTRL_F_LIGHT3_SIZE))
#define XF_COLOR0CNTRL_F_LIGHT0123_SHIFT \
    XF_COLOR0CNTRL_F_LIGHT0_SHIFT
#define XF_COLOR0CNTRL_F_LIGHT0123_MASK \
    ((XF_COLOR0CNTRL_F_LIGHT0_MASK)|(XF_COLOR0CNTRL_F_LIGHT1_MASK)| \
     (XF_COLOR0CNTRL_F_LIGHT2_MASK)|(XF_COLOR0CNTRL_F_LIGHT3_MASK))
#define SC_XF_COLOR0CNTRL_F_SET_LIGHT0123(line, xf_color0cntrl_f, light0123) \
    FAST_GPFLAGSET(line, xf_color0cntrl_f, light0123, XF_COLOR0CNTRL_F_LIGHT0123)

#define XF_COLOR0CNTRL_F_LIGHT4567_SIZE \
    ((XF_COLOR0CNTRL_F_LIGHT4_SIZE)+(XF_COLOR0CNTRL_F_LIGHT5_SIZE)+ \
     (XF_COLOR0CNTRL_F_LIGHT6_SIZE)+(XF_COLOR0CNTRL_F_LIGHT7_SIZE))
#define XF_COLOR0CNTRL_F_LIGHT4567_SHIFT \
    XF_COLOR0CNTRL_F_LIGHT4_SHIFT
#define XF_COLOR0CNTRL_F_LIGHT4567_MASK \
    ((XF_COLOR0CNTRL_F_LIGHT4_MASK)|(XF_COLOR0CNTRL_F_LIGHT5_MASK)| \
     (XF_COLOR0CNTRL_F_LIGHT6_MASK)|(XF_COLOR0CNTRL_F_LIGHT7_MASK))
#define SC_XF_COLOR0CNTRL_F_SET_LIGHT4567(line, xf_color0cntrl_f, light4567) \
    FAST_GPFLAGSET(line, xf_color0cntrl_f, light4567, XF_COLOR0CNTRL_F_LIGHT4567)

#endif // GXFDLSHORTCUT_H
