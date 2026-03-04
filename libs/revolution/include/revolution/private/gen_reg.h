#ifndef GEN_REG_H
#define GEN_REG_H

#include <revolution/private/GXFDLShortcut.h>

#define GEN_MODE_RID_SIZE	8
#define GEN_MODE_RID_SHIFT	24
#define GEN_MODE_RID_MASK	0xff000000
#define GEN_MODE_GET_RID(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_RID_MASK) >> GEN_MODE_RID_SHIFT)

#define GEN_MODE_NTEX_SIZE	4
#define GEN_MODE_NTEX_SHIFT	0
#define GEN_MODE_NTEX_MASK	0x0000000f
#define GEN_MODE_GET_NTEX(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_NTEX_MASK) >> GEN_MODE_NTEX_SHIFT)

#define GEN_MODE_NTEX_SIZE	4
#define GEN_MODE_NTEX_SHIFT	0
#define GEN_MODE_NTEX_MASK	0x0000000f
#define GEN_MODE_GET_NTEX(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_NTEX_MASK) >> GEN_MODE_NTEX_SHIFT)

#define GEN_MODE_NCOL_SIZE	3
#define GEN_MODE_NCOL_SHIFT	4
#define GEN_MODE_NCOL_MASK	0x00000070
#define GEN_MODE_GET_NCOL(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_NCOL_MASK) >> GEN_MODE_NCOL_SHIFT)

#define GEN_MODE_PAD0_SIZE	1
#define GEN_MODE_PAD0_SHIFT	7
#define GEN_MODE_PAD0_MASK	0x00000080
#define GEN_MODE_GET_PAD0(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_PAD0_MASK) >> GEN_MODE_PAD0_SHIFT)

#define GEN_MODE_FLAT_EN_SIZE	1
#define GEN_MODE_FLAT_EN_SHIFT	8
#define GEN_MODE_FLAT_EN_MASK	0x00000100
#define GEN_MODE_GET_FLAT_EN(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_FLAT_EN_MASK) >> GEN_MODE_FLAT_EN_SHIFT)

#define GEN_MODE_MS_EN_SIZE	1
#define GEN_MODE_MS_EN_SHIFT	9
#define GEN_MODE_MS_EN_MASK	0x00000200
#define GEN_MODE_GET_MS_EN(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_MS_EN_MASK) >> GEN_MODE_MS_EN_SHIFT)

#define GEN_MODE_NTEV_SIZE	4
#define GEN_MODE_NTEV_SHIFT	10
#define GEN_MODE_NTEV_MASK	0x00003c00
#define GEN_MODE_GET_NTEV(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_NTEV_MASK) >> GEN_MODE_NTEV_SHIFT)

#define GEN_MODE_REJECT_EN_SIZE	2
#define GEN_MODE_REJECT_EN_SHIFT	14
#define GEN_MODE_REJECT_EN_MASK	0x0000c000
#define GEN_MODE_GET_REJECT_EN(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_REJECT_EN_MASK) >> GEN_MODE_REJECT_EN_SHIFT)

#define GEN_MODE_NBMP_SIZE	3
#define GEN_MODE_NBMP_SHIFT	16
#define GEN_MODE_NBMP_MASK	0x00070000
#define GEN_MODE_GET_NBMP(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_NBMP_MASK) >> GEN_MODE_NBMP_SHIFT)

#define GEN_MODE_ZFREEZE_SIZE	1
#define GEN_MODE_ZFREEZE_SHIFT	19
#define GEN_MODE_ZFREEZE_MASK	0x00080000
#define GEN_MODE_GET_ZFREEZE(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_ZFREEZE_MASK) >> GEN_MODE_ZFREEZE_SHIFT)

#define GEN_MODE_PAD1_SIZE	4
#define GEN_MODE_PAD1_SHIFT	20
#define GEN_MODE_PAD1_MASK	0x00f00000
#define GEN_MODE_GET_PAD1(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_PAD1_MASK) >> GEN_MODE_PAD1_SHIFT)

#define GEN_MODE_RID_SIZE	8
#define GEN_MODE_RID_SHIFT	24
#define GEN_MODE_RID_MASK	0xff000000
#define GEN_MODE_GET_RID(gen_mode) \
	((((unsigned long)(gen_mode)) & GEN_MODE_RID_MASK) >> GEN_MODE_RID_SHIFT)


#define SC_GEN_MODE_SET_NTEX(line, gen_mode,ntex) \
	FAST_GPFLAGSET(line, gen_mode,ntex,GEN_MODE_NTEX)

#define SC_GEN_MODE_SET_NCOL(line, gen_mode,ncol) \
	FAST_GPFLAGSET(line, gen_mode,ncol,GEN_MODE_NCOL)

#define SC_GEN_MODE_SET_PAD0(line, gen_mode,pad0) \
	FAST_GPFLAGSET(line, gen_mode,pad0,GEN_MODE_PAD0)

#define SC_GEN_MODE_SET_FLAT_EN(line, gen_mode,flat_en) \
	FAST_GPFLAGSET(line, gen_mode,flat_en,GEN_MODE_FLAT_EN)

#define SC_GEN_MODE_SET_MS_EN(line, gen_mode,ms_en) \
	FAST_GPFLAGSET(line, gen_mode,ms_en,GEN_MODE_MS_EN)

#define SC_GEN_MODE_SET_NTEV(line, gen_mode,ntev) \
	FAST_GPFLAGSET(line, gen_mode,ntev,GEN_MODE_NTEV)

#define SC_GEN_MODE_SET_REJECT_EN(line, gen_mode,reject_en) \
	FAST_GPFLAGSET(line, gen_mode,reject_en,GEN_MODE_REJECT_EN)

#define SC_GEN_MODE_SET_NBMP(line, gen_mode,nbmp) \
	FAST_GPFLAGSET(line, gen_mode,nbmp,GEN_MODE_NBMP)

#define SC_GEN_MODE_SET_ZFREEZE(line, gen_mode,zfreeze) \
	FAST_GPFLAGSET(line, gen_mode,zfreeze,GEN_MODE_ZFREEZE)

#define SC_GEN_MODE_SET_PAD1(line, gen_mode,pad1) \
	FAST_GPFLAGSET(line, gen_mode,pad1,GEN_MODE_PAD1)

#define SC_GEN_MODE_SET_RID(line, gen_mode,rid) \
	FAST_GPFLAGSET(line, gen_mode,rid,GEN_MODE_RID)

#define SC_GEN_MSLOC_SET_XS0(line, gen_msloc,xs0) \
	FAST_GPFLAGSET(line, gen_msloc,xs0,GEN_MSLOC_XS0)

#define SC_GEN_MSLOC_SET_YS0(line, gen_msloc,ys0) \
	FAST_GPFLAGSET(line, gen_msloc,ys0,GEN_MSLOC_YS0)

#define SC_GEN_MSLOC_SET_XS1(line, gen_msloc,xs1) \
	FAST_GPFLAGSET(line, gen_msloc,xs1,GEN_MSLOC_XS1)

#define SC_GEN_MSLOC_SET_YS1(line, gen_msloc,ys1) \
	FAST_GPFLAGSET(line, gen_msloc,ys1,GEN_MSLOC_YS1)

#define SC_GEN_MSLOC_SET_XS2(line, gen_msloc,xs2) \
	FAST_GPFLAGSET(line, gen_msloc,xs2,GEN_MSLOC_XS2)

#define SC_GEN_MSLOC_SET_YS2(line, gen_msloc,ys2) \
	FAST_GPFLAGSET(line, gen_msloc,ys2,GEN_MSLOC_YS2)

#define SC_GEN_MSLOC_SET_RID(line, gen_msloc,rid) \
	FAST_GPFLAGSET(line, gen_msloc,rid,GEN_MSLOC_RID)

#define SC_GEN_COLOR_SET_ALPHA(line, gen_color,alpha) \
	FAST_GPFLAGSET(line, gen_color,alpha,GEN_COLOR_ALPHA)

#define SC_GEN_COLOR_SET_BLUE(line, gen_color,blue) \
	FAST_GPFLAGSET(line, gen_color,blue,GEN_COLOR_BLUE)

#define SC_GEN_COLOR_SET_GREEN(line, gen_color,green) \
	FAST_GPFLAGSET(line, gen_color,green,GEN_COLOR_GREEN)

#define SC_GEN_COLOR_SET_RED(line, gen_color,red) \
	FAST_GPFLAGSET(line, gen_color,red,GEN_COLOR_RED)

#define SC_GEN_Z24_COLOR_SET_BLUE(line, gen_z24_color,blue) \
	FAST_GPFLAGSET(line, gen_z24_color,blue,GEN_Z24_COLOR_BLUE)

#define SC_GEN_Z24_COLOR_SET_GREEN(line, gen_z24_color,green) \
	FAST_GPFLAGSET(line, gen_z24_color,green,GEN_Z24_COLOR_GREEN)

#define SC_GEN_Z24_COLOR_SET_RED(line, gen_z24_color,red) \
	FAST_GPFLAGSET(line, gen_z24_color,red,GEN_Z24_COLOR_RED)

#define SC_GEN_Z24_COLOR_SET_PAD0(line, gen_z24_color,pad0) \
	FAST_GPFLAGSET(line, gen_z24_color,pad0,GEN_Z24_COLOR_PAD0)

#endif // GEN_REG_H
