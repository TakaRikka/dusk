#ifndef PI_REG_H
#define PI_REG_H

#include <revolution/private/GXFDLShortcut.h>

#define PI_REG_INTSR	0x000
#define PI_REG_INTMSK	0x004
#define PI_REG_CPBAS	0x00C
#define PI_REG_CPTOP	0x010
#define PI_REG_CPWRT	0x014
#define PI_REG_CPABT	0x018
#define PI_REG_PIESR	0x01C
#define PI_REG_PIEAR	0x020
#define PI_REG_CONFIG	0x024
#define PI_REG_DURAR	0x028
#define PI_REG_CHIPID	0x02C
#define PI_REG_STRGTH	0x030
#define PI_REG_CPUDBB	0x034

#define PI_REGSP_CP	0x0C000000
#define PI_REGSP_PE	0x0C001000
#define PI_REGSP_VI	0x0C002000
#define PI_REGSP_PI	0x0C003000
#define PI_REGSP_MEM	0x0C004000
#define PI_REGSP_DSP	0x0C005000
#define PI_REGSP_IO	0x0C006000
#define PI_GFXSP	0x0C008000

#define PI_CPWRT_REG_CPWRT_SIZE	24
#define PI_CPWRT_REG_CPWRT_SHIFT	5
#define PI_CPWRT_REG_CPWRT_MASK	0x1fffffe0
#define PI_CPWRT_REG_GET_CPWRT(pi_cpwrt_reg) \
	((((unsigned long)(pi_cpwrt_reg)) & PI_CPWRT_REG_CPWRT_MASK) >> PI_CPWRT_REG_CPWRT_SHIFT)

#define XF_PERF0_F_PERF_B_SIZE	5
#define XF_PERF0_F_PERF_B_SHIFT	5
#define XF_PERF0_F_PERF_B_MASK	0x000003e0
#define XF_PERF0_F_GET_PERF_B(xf_perf0_f) \
	((((unsigned long)(xf_perf0_f)) & XF_PERF0_F_PERF_B_MASK) >> XF_PERF0_F_PERF_B_SHIFT)

#define XF_PERF0_F_PERF_C_SIZE	5
#define XF_PERF0_F_PERF_C_SHIFT	10
#define XF_PERF0_F_PERF_C_MASK	0x00007c00
#define XF_PERF0_F_GET_PERF_C(xf_perf0_f) \
	((((unsigned long)(xf_perf0_f)) & XF_PERF0_F_PERF_C_MASK) >> XF_PERF0_F_PERF_C_SHIFT)

#define XF_PERF0_F_PERF_D_SIZE	5
#define XF_PERF0_F_PERF_D_SHIFT	15
#define XF_PERF0_F_PERF_D_MASK	0x000f8000
#define XF_PERF0_F_GET_PERF_D(xf_perf0_f) \
	((((unsigned long)(xf_perf0_f)) & XF_PERF0_F_PERF_D_MASK) >> XF_PERF0_F_PERF_D_SHIFT)

#define XF_PERF0_F_PERF_A_SIZE	5
#define XF_PERF0_F_PERF_A_SHIFT	0
#define XF_PERF0_F_PERF_A_MASK	0x0000001f
#define XF_PERF0_F_GET_PERF_A(xf_perf0_f) \
	((((unsigned long)(xf_perf0_f)) & XF_PERF0_F_PERF_A_MASK) >> XF_PERF0_F_PERF_A_SHIFT)

#define PI_CPWRT_REG_CPWRAP_SIZE	1
#define PI_CPWRT_REG_CPWRAP_SHIFT	29
#define PI_CPWRT_REG_CPWRAP_MASK	0x20000000
#define PI_CPWRT_REG_GET_CPWRAP(pi_cpwrt_reg) \
	((((unsigned long)(pi_cpwrt_reg)) & PI_CPWRT_REG_CPWRAP_MASK) >> PI_CPWRT_REG_CPWRAP_SHIFT)


#define SC_PI_CPWRT_REG_SET_CPWRT(line, pi_cpwrt_reg,cpwrt) \
	FAST_GPFLAGSET(line, pi_cpwrt_reg,cpwrt,PI_CPWRT_REG_CPWRT)

#define SC_PI_CPWRT_REG_SET_CPWRAP(line, pi_cpwrt_reg,cpwrap) \
	FAST_GPFLAGSET(line, pi_cpwrt_reg,cpwrap,PI_CPWRT_REG_CPWRAP)


#endif // PI_REG_H
