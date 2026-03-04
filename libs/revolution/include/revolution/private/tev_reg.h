#ifndef TEV_REG_H
#define TEV_REG_H

#define TEV_COLOR_ENV_RID_SIZE	8
#define TEV_COLOR_ENV_RID_SHIFT	24
#define TEV_COLOR_ENV_RID_MASK	0xff000000
#define TEV_COLOR_ENV_GET_RID(tev_color_env) \
	((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_RID_MASK) >> TEV_COLOR_ENV_RID_SHIFT)

#define TEV_ALPHA_ENV_RID_SIZE	8
#define TEV_ALPHA_ENV_RID_SHIFT	24
#define TEV_ALPHA_ENV_RID_MASK	0xff000000
#define TEV_ALPHA_ENV_GET_RID(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_RID_MASK) >> TEV_ALPHA_ENV_RID_SHIFT)

#define TEV_KSEL_RID_SIZE	8
#define TEV_KSEL_RID_SHIFT	24
#define TEV_KSEL_RID_MASK	0xff000000
#define TEV_KSEL_GET_RID(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_RID_MASK) >> TEV_KSEL_RID_SHIFT)

#define TEV_ALPHA_ENV_SELD_SIZE	3
#define TEV_ALPHA_ENV_SELD_SHIFT	4
#define TEV_ALPHA_ENV_SELD_MASK	0x00000070
#define TEV_ALPHA_ENV_GET_SELD(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELD_MASK) >> TEV_ALPHA_ENV_SELD_SHIFT)

#define TEV_ALPHA_ENV_SELC_SIZE	3
#define TEV_ALPHA_ENV_SELC_SHIFT	7
#define TEV_ALPHA_ENV_SELC_MASK	0x00000380
#define TEV_ALPHA_ENV_GET_SELC(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELC_MASK) >> TEV_ALPHA_ENV_SELC_SHIFT)

#define TEV_ALPHA_ENV_SELB_SIZE	3
#define TEV_ALPHA_ENV_SELB_SHIFT	10
#define TEV_ALPHA_ENV_SELB_MASK	0x00001c00
#define TEV_ALPHA_ENV_GET_SELB(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELB_MASK) >> TEV_ALPHA_ENV_SELB_SHIFT)

#define TEV_ALPHA_ENV_SELA_SIZE	3
#define TEV_ALPHA_ENV_SELA_SHIFT	13
#define TEV_ALPHA_ENV_SELA_MASK	0x0000e000
#define TEV_ALPHA_ENV_GET_SELA(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELA_MASK) >> TEV_ALPHA_ENV_SELA_SHIFT)

#define TEV_COLOR_ENV_SELD_SIZE	4
#define TEV_COLOR_ENV_SELD_SHIFT	0
#define TEV_COLOR_ENV_SELD_MASK	0x0000000f
#define TEV_COLOR_ENV_GET_SELD(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_SELD_MASK) >> TEV_COLOR_ENV_SELD_SHIFT)

#define TEV_COLOR_ENV_SELC_SIZE	4
#define TEV_COLOR_ENV_SELC_SHIFT	4
#define TEV_COLOR_ENV_SELC_MASK	0x000000f0
#define TEV_COLOR_ENV_GET_SELC(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_SELC_MASK) >> TEV_COLOR_ENV_SELC_SHIFT)

#define TEV_COLOR_ENV_SELB_SIZE	4
#define TEV_COLOR_ENV_SELB_SHIFT	8
#define TEV_COLOR_ENV_SELB_MASK	0x00000f00
#define TEV_COLOR_ENV_GET_SELB(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_SELB_MASK) >> TEV_COLOR_ENV_SELB_SHIFT)

#define TEV_COLOR_ENV_SELA_SIZE	4
#define TEV_COLOR_ENV_SELA_SHIFT	12
#define TEV_COLOR_ENV_SELA_MASK	0x0000f000
#define TEV_COLOR_ENV_GET_SELA(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_SELA_MASK) >> TEV_COLOR_ENV_SELA_SHIFT)

#define TEV_COLOR_ENV_BIAS_SIZE	2
#define TEV_COLOR_ENV_BIAS_SHIFT	16
#define TEV_COLOR_ENV_BIAS_MASK	0x00030000
#define TEV_COLOR_ENV_GET_BIAS(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_BIAS_MASK) >> TEV_COLOR_ENV_BIAS_SHIFT)

#define TEV_COLOR_ENV_SUB_SIZE	1
#define TEV_COLOR_ENV_SUB_SHIFT	18
#define TEV_COLOR_ENV_SUB_MASK	0x00040000
#define TEV_COLOR_ENV_GET_SUB(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_SUB_MASK) >> TEV_COLOR_ENV_SUB_SHIFT)

#define TEV_COLOR_ENV_CLAMP_SIZE	1
#define TEV_COLOR_ENV_CLAMP_SHIFT	19
#define TEV_COLOR_ENV_CLAMP_MASK	0x00080000
#define TEV_COLOR_ENV_GET_CLAMP(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_CLAMP_MASK) >> TEV_COLOR_ENV_CLAMP_SHIFT)

#define TEV_COLOR_ENV_SHIFT_SIZE	2
#define TEV_COLOR_ENV_SHIFT_SHIFT	20
#define TEV_COLOR_ENV_SHIFT_MASK	0x00300000
#define TEV_COLOR_ENV_GET_SHIFT(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_SHIFT_MASK) >> TEV_COLOR_ENV_SHIFT_SHIFT)

#define TEV_COLOR_ENV_DEST_SIZE	2
#define TEV_COLOR_ENV_DEST_SHIFT	22
#define TEV_COLOR_ENV_DEST_MASK	0x00c00000
#define TEV_COLOR_ENV_GET_DEST(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_DEST_MASK) >> TEV_COLOR_ENV_DEST_SHIFT)

#define TEV_COLOR_ENV_RID_SIZE	8
#define TEV_COLOR_ENV_RID_SHIFT	24
#define TEV_COLOR_ENV_RID_MASK	0xff000000
#define TEV_COLOR_ENV_GET_RID(tev_color_env) \
    ((((unsigned long)(tev_color_env)) & TEV_COLOR_ENV_RID_MASK) >> TEV_COLOR_ENV_RID_SHIFT)

#define TEV_ALPHA_ENV_MODE_SIZE	2
#define TEV_ALPHA_ENV_MODE_SHIFT	0
#define TEV_ALPHA_ENV_MODE_MASK	0x00000003
#define TEV_ALPHA_ENV_GET_MODE(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_MODE_MASK) >> TEV_ALPHA_ENV_MODE_SHIFT)

#define TEV_ALPHA_ENV_SWAP_SIZE	2
#define TEV_ALPHA_ENV_SWAP_SHIFT	2
#define TEV_ALPHA_ENV_SWAP_MASK	0x0000000c
#define TEV_ALPHA_ENV_GET_SWAP(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SWAP_MASK) >> TEV_ALPHA_ENV_SWAP_SHIFT)

#define TEV_ALPHA_ENV_SELD_SIZE	3
#define TEV_ALPHA_ENV_SELD_SHIFT	4
#define TEV_ALPHA_ENV_SELD_MASK	0x00000070
#define TEV_ALPHA_ENV_GET_SELD(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELD_MASK) >> TEV_ALPHA_ENV_SELD_SHIFT)

#define TEV_ALPHA_ENV_SELC_SIZE	3
#define TEV_ALPHA_ENV_SELC_SHIFT	7
#define TEV_ALPHA_ENV_SELC_MASK	0x00000380
#define TEV_ALPHA_ENV_GET_SELC(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELC_MASK) >> TEV_ALPHA_ENV_SELC_SHIFT)

#define TEV_ALPHA_ENV_SELB_SIZE	3
#define TEV_ALPHA_ENV_SELB_SHIFT	10
#define TEV_ALPHA_ENV_SELB_MASK	0x00001c00
#define TEV_ALPHA_ENV_GET_SELB(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELB_MASK) >> TEV_ALPHA_ENV_SELB_SHIFT)

#define TEV_ALPHA_ENV_SELA_SIZE	3
#define TEV_ALPHA_ENV_SELA_SHIFT	13
#define TEV_ALPHA_ENV_SELA_MASK	0x0000e000
#define TEV_ALPHA_ENV_GET_SELA(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SELA_MASK) >> TEV_ALPHA_ENV_SELA_SHIFT)

#define TEV_ALPHA_ENV_BIAS_SIZE	2
#define TEV_ALPHA_ENV_BIAS_SHIFT	16
#define TEV_ALPHA_ENV_BIAS_MASK	0x00030000
#define TEV_ALPHA_ENV_GET_BIAS(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_BIAS_MASK) >> TEV_ALPHA_ENV_BIAS_SHIFT)

#define TEV_ALPHA_ENV_SUB_SIZE	1
#define TEV_ALPHA_ENV_SUB_SHIFT	18
#define TEV_ALPHA_ENV_SUB_MASK	0x00040000
#define TEV_ALPHA_ENV_GET_SUB(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SUB_MASK) >> TEV_ALPHA_ENV_SUB_SHIFT)

#define TEV_ALPHA_ENV_CLAMP_SIZE	1
#define TEV_ALPHA_ENV_CLAMP_SHIFT	19
#define TEV_ALPHA_ENV_CLAMP_MASK	0x00080000
#define TEV_ALPHA_ENV_GET_CLAMP(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_CLAMP_MASK) >> TEV_ALPHA_ENV_CLAMP_SHIFT)

#define TEV_ALPHA_ENV_SHIFT_SIZE	2
#define TEV_ALPHA_ENV_SHIFT_SHIFT	20
#define TEV_ALPHA_ENV_SHIFT_MASK	0x00300000
#define TEV_ALPHA_ENV_GET_SHIFT(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_SHIFT_MASK) >> TEV_ALPHA_ENV_SHIFT_SHIFT)

#define TEV_ALPHA_ENV_DEST_SIZE	2
#define TEV_ALPHA_ENV_DEST_SHIFT	22
#define TEV_ALPHA_ENV_DEST_MASK	0x00c00000
#define TEV_ALPHA_ENV_GET_DEST(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_DEST_MASK) >> TEV_ALPHA_ENV_DEST_SHIFT)

#define TEV_ALPHA_ENV_RID_SIZE	8
#define TEV_ALPHA_ENV_RID_SHIFT	24
#define TEV_ALPHA_ENV_RID_MASK	0xff000000
#define TEV_ALPHA_ENV_GET_RID(tev_alpha_env) \
    ((((unsigned long)(tev_alpha_env)) & TEV_ALPHA_ENV_RID_MASK) >> TEV_ALPHA_ENV_RID_SHIFT)

#define TEV_REGISTERL_R_SIZE	11
#define TEV_REGISTERL_R_SHIFT	0
#define TEV_REGISTERL_R_MASK	0x000007ff
#define TEV_REGISTERL_GET_R(tev_registerl) \
    ((((unsigned long)(tev_registerl)) & TEV_REGISTERL_R_MASK) >> TEV_REGISTERL_R_SHIFT)

#define TEV_REGISTERL_PAD0_SIZE	1
#define TEV_REGISTERL_PAD0_SHIFT	11
#define TEV_REGISTERL_PAD0_MASK	0x00000800
#define TEV_REGISTERL_GET_PAD0(tev_registerl) \
    ((((unsigned long)(tev_registerl)) & TEV_REGISTERL_PAD0_MASK) >> TEV_REGISTERL_PAD0_SHIFT)

#define TEV_REGISTERL_A_SIZE	11
#define TEV_REGISTERL_A_SHIFT	12
#define TEV_REGISTERL_A_MASK	0x007ff000
#define TEV_REGISTERL_GET_A(tev_registerl) \
    ((((unsigned long)(tev_registerl)) & TEV_REGISTERL_A_MASK) >> TEV_REGISTERL_A_SHIFT)

#define TEV_REGISTERL_PAD1_SIZE	1
#define TEV_REGISTERL_PAD1_SHIFT	23
#define TEV_REGISTERL_PAD1_MASK	0x00800000
#define TEV_REGISTERL_GET_PAD1(tev_registerl) \
    ((((unsigned long)(tev_registerl)) & TEV_REGISTERL_PAD1_MASK) >> TEV_REGISTERL_PAD1_SHIFT)

#define TEV_REGISTERL_RID_SIZE	8
#define TEV_REGISTERL_RID_SHIFT	24
#define TEV_REGISTERL_RID_MASK	0xff000000
#define TEV_REGISTERL_GET_RID(tev_registerl) \
    ((((unsigned long)(tev_registerl)) & TEV_REGISTERL_RID_MASK) >> TEV_REGISTERL_RID_SHIFT)

#define TEV_REGISTERL_TOTAL_SIZE	32
#define TEV_REGISTERL(r, a, rid) \
    ((((unsigned long)(r)) << TEV_REGISTERL_R_SHIFT) | \
        (((unsigned long)(a)) << TEV_REGISTERL_A_SHIFT) | \
        (((unsigned long)(rid)) << TEV_REGISTERL_RID_SHIFT))

#define TEV_KREGISTERL_R_SIZE	8
#define TEV_KREGISTERL_R_SHIFT	0
#define TEV_KREGISTERL_R_MASK	0x000000ff
#define TEV_KREGISTERL_GET_R(tev_kregisterl) \
    ((((unsigned long)(tev_kregisterl)) & TEV_KREGISTERL_R_MASK) >> TEV_KREGISTERL_R_SHIFT)

#define TEV_KREGISTERL_PAD0_SIZE	4
#define TEV_KREGISTERL_PAD0_SHIFT	8
#define TEV_KREGISTERL_PAD0_MASK	0x00000f00
#define TEV_KREGISTERL_GET_PAD0(tev_kregisterl) \
    ((((unsigned long)(tev_kregisterl)) & TEV_KREGISTERL_PAD0_MASK) >> TEV_KREGISTERL_PAD0_SHIFT)

#define TEV_KREGISTERL_A_SIZE	8
#define TEV_KREGISTERL_A_SHIFT	12
#define TEV_KREGISTERL_A_MASK	0x000ff000
#define TEV_KREGISTERL_GET_A(tev_kregisterl) \
    ((((unsigned long)(tev_kregisterl)) & TEV_KREGISTERL_A_MASK) >> TEV_KREGISTERL_A_SHIFT)

#define TEV_KREGISTERL_PAD1_SIZE	4
#define TEV_KREGISTERL_PAD1_SHIFT	20
#define TEV_KREGISTERL_PAD1_MASK	0x00f00000
#define TEV_KREGISTERL_GET_PAD1(tev_kregisterl) \
    ((((unsigned long)(tev_kregisterl)) & TEV_KREGISTERL_PAD1_MASK) >> TEV_KREGISTERL_PAD1_SHIFT)

#define TEV_KREGISTERL_RID_SIZE	8
#define TEV_KREGISTERL_RID_SHIFT	24
#define TEV_KREGISTERL_RID_MASK	0xff000000
#define TEV_KREGISTERL_GET_RID(tev_kregisterl) \
    ((((unsigned long)(tev_kregisterl)) & TEV_KREGISTERL_RID_MASK) >> TEV_KREGISTERL_RID_SHIFT)

#define TEV_KREGISTERL_TOTAL_SIZE	32
#define TEV_KREGISTERL(r, a, rid) \
    ((((unsigned long)(r)) << TEV_KREGISTERL_R_SHIFT) | \
        (((unsigned long)(a)) << TEV_KREGISTERL_A_SHIFT) | \
        (((unsigned long)(rid)) << TEV_KREGISTERL_RID_SHIFT))

#define TEV_REGISTERH_B_SIZE	11
#define TEV_REGISTERH_B_SHIFT	0
#define TEV_REGISTERH_B_MASK	0x000007ff
#define TEV_REGISTERH_GET_B(tev_registerh) \
    ((((unsigned long)(tev_registerh)) & TEV_REGISTERH_B_MASK) >> TEV_REGISTERH_B_SHIFT)

#define TEV_REGISTERH_PAD0_SIZE	1
#define TEV_REGISTERH_PAD0_SHIFT	11
#define TEV_REGISTERH_PAD0_MASK	0x00000800
#define TEV_REGISTERH_GET_PAD0(tev_registerh) \
    ((((unsigned long)(tev_registerh)) & TEV_REGISTERH_PAD0_MASK) >> TEV_REGISTERH_PAD0_SHIFT)

#define TEV_REGISTERH_G_SIZE	11
#define TEV_REGISTERH_G_SHIFT	12
#define TEV_REGISTERH_G_MASK	0x007ff000
#define TEV_REGISTERH_GET_G(tev_registerh) \
    ((((unsigned long)(tev_registerh)) & TEV_REGISTERH_G_MASK) >> TEV_REGISTERH_G_SHIFT)

#define TEV_REGISTERH_PAD1_SIZE	1
#define TEV_REGISTERH_PAD1_SHIFT	23
#define TEV_REGISTERH_PAD1_MASK	0x00800000
#define TEV_REGISTERH_GET_PAD1(tev_registerh) \
    ((((unsigned long)(tev_registerh)) & TEV_REGISTERH_PAD1_MASK) >> TEV_REGISTERH_PAD1_SHIFT)

#define TEV_REGISTERH_RID_SIZE	8
#define TEV_REGISTERH_RID_SHIFT	24
#define TEV_REGISTERH_RID_MASK	0xff000000
#define TEV_REGISTERH_GET_RID(tev_registerh) \
    ((((unsigned long)(tev_registerh)) & TEV_REGISTERH_RID_MASK) >> TEV_REGISTERH_RID_SHIFT)

#define TEV_REGISTERH_TOTAL_SIZE	32
#define TEV_REGISTERH(b, g, rid) \
    ((((unsigned long)(b)) << TEV_REGISTERH_B_SHIFT) | \
        (((unsigned long)(g)) << TEV_REGISTERH_G_SHIFT) | \
        (((unsigned long)(rid)) << TEV_REGISTERH_RID_SHIFT))

#define TEV_KREGISTERH_B_SIZE	8
#define TEV_KREGISTERH_B_SHIFT	0
#define TEV_KREGISTERH_B_MASK	0x000000ff
#define TEV_KREGISTERH_GET_B(tev_kregisterh) \
    ((((unsigned long)(tev_kregisterh)) & TEV_KREGISTERH_B_MASK) >> TEV_KREGISTERH_B_SHIFT)

#define TEV_KREGISTERH_PAD0_SIZE	4
#define TEV_KREGISTERH_PAD0_SHIFT	8
#define TEV_KREGISTERH_PAD0_MASK	0x00000f00
#define TEV_KREGISTERH_GET_PAD0(tev_kregisterh) \
    ((((unsigned long)(tev_kregisterh)) & TEV_KREGISTERH_PAD0_MASK) >> TEV_KREGISTERH_PAD0_SHIFT)

#define TEV_KREGISTERH_G_SIZE	8
#define TEV_KREGISTERH_G_SHIFT	12
#define TEV_KREGISTERH_G_MASK	0x000ff000
#define TEV_KREGISTERH_GET_G(tev_kregisterh) \
    ((((unsigned long)(tev_kregisterh)) & TEV_KREGISTERH_G_MASK) >> TEV_KREGISTERH_G_SHIFT)

#define TEV_KREGISTERH_PAD1_SIZE	4
#define TEV_KREGISTERH_PAD1_SHIFT	20
#define TEV_KREGISTERH_PAD1_MASK	0x00f00000
#define TEV_KREGISTERH_GET_PAD1(tev_kregisterh) \
    ((((unsigned long)(tev_kregisterh)) & TEV_KREGISTERH_PAD1_MASK) >> TEV_KREGISTERH_PAD1_SHIFT)

#define TEV_KREGISTERH_RID_SIZE	8
#define TEV_KREGISTERH_RID_SHIFT	24
#define TEV_KREGISTERH_RID_MASK	0xff000000
#define TEV_KREGISTERH_GET_RID(tev_kregisterh) \
    ((((unsigned long)(tev_kregisterh)) & TEV_KREGISTERH_RID_MASK) >> TEV_KREGISTERH_RID_SHIFT)

#define TEV_KREGISTERH_TOTAL_SIZE	32
#define TEV_KREGISTERH(b, g, rid) \
    ((((unsigned long)(b)) << TEV_KREGISTERH_B_SHIFT) | \
        (((unsigned long)(g)) << TEV_KREGISTERH_G_SHIFT) | \
        (((unsigned long)(rid)) << TEV_KREGISTERH_RID_SHIFT))

#define TEV_KSEL_XRB_SIZE	2
#define TEV_KSEL_XRB_SHIFT	0
#define TEV_KSEL_XRB_MASK	0x00000003
#define TEV_KSEL_GET_XRB(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_XRB_MASK) >> TEV_KSEL_XRB_SHIFT)

#define TEV_KSEL_XGA_SIZE	2
#define TEV_KSEL_XGA_SHIFT	2
#define TEV_KSEL_XGA_MASK	0x0000000c
#define TEV_KSEL_GET_XGA(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_XGA_MASK) >> TEV_KSEL_XGA_SHIFT)

#define TEV_KSEL_KCSEL0_SIZE	5
#define TEV_KSEL_KCSEL0_SHIFT	4
#define TEV_KSEL_KCSEL0_MASK	0x000001f0
#define TEV_KSEL_GET_KCSEL0(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_KCSEL0_MASK) >> TEV_KSEL_KCSEL0_SHIFT)

#define TEV_KSEL_KASEL0_SIZE	5
#define TEV_KSEL_KASEL0_SHIFT	9
#define TEV_KSEL_KASEL0_MASK	0x00003e00
#define TEV_KSEL_GET_KASEL0(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_KASEL0_MASK) >> TEV_KSEL_KASEL0_SHIFT)

#define TEV_KSEL_KCSEL1_SIZE	5
#define TEV_KSEL_KCSEL1_SHIFT	14
#define TEV_KSEL_KCSEL1_MASK	0x0007c000
#define TEV_KSEL_GET_KCSEL1(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_KCSEL1_MASK) >> TEV_KSEL_KCSEL1_SHIFT)

#define TEV_KSEL_KASEL1_SIZE	5
#define TEV_KSEL_KASEL1_SHIFT	19
#define TEV_KSEL_KASEL1_MASK	0x00f80000
#define TEV_KSEL_GET_KASEL1(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_KASEL1_MASK) >> TEV_KSEL_KASEL1_SHIFT)

#define TEV_KSEL_RID_SIZE	8
#define TEV_KSEL_RID_SHIFT	24
#define TEV_KSEL_RID_MASK	0xff000000
#define TEV_KSEL_GET_RID(tev_ksel) \
    ((((unsigned long)(tev_ksel)) & TEV_KSEL_RID_MASK) >> TEV_KSEL_RID_SHIFT)

#define TEV_ALPHAFUNC_A0_SIZE	8
#define TEV_ALPHAFUNC_A0_SHIFT	0
#define TEV_ALPHAFUNC_A0_MASK	0x000000ff
#define TEV_ALPHAFUNC_GET_A0(tev_alphafunc) \
    ((((unsigned long)(tev_alphafunc)) & TEV_ALPHAFUNC_A0_MASK) >> TEV_ALPHAFUNC_A0_SHIFT)

#define TEV_ALPHAFUNC_A1_SIZE	8
#define TEV_ALPHAFUNC_A1_SHIFT	8
#define TEV_ALPHAFUNC_A1_MASK	0x0000ff00
#define TEV_ALPHAFUNC_GET_A1(tev_alphafunc) \
    ((((unsigned long)(tev_alphafunc)) & TEV_ALPHAFUNC_A1_MASK) >> TEV_ALPHAFUNC_A1_SHIFT)

#define TEV_ALPHAFUNC_OP0_SIZE	3
#define TEV_ALPHAFUNC_OP0_SHIFT	16
#define TEV_ALPHAFUNC_OP0_MASK	0x00070000
#define TEV_ALPHAFUNC_GET_OP0(tev_alphafunc) \
    ((((unsigned long)(tev_alphafunc)) & TEV_ALPHAFUNC_OP0_MASK) >> TEV_ALPHAFUNC_OP0_SHIFT)

#define TEV_ALPHAFUNC_OP1_SIZE	3
#define TEV_ALPHAFUNC_OP1_SHIFT	19
#define TEV_ALPHAFUNC_OP1_MASK	0x00380000
#define TEV_ALPHAFUNC_GET_OP1(tev_alphafunc) \
    ((((unsigned long)(tev_alphafunc)) & TEV_ALPHAFUNC_OP1_MASK) >> TEV_ALPHAFUNC_OP1_SHIFT)

#define TEV_ALPHAFUNC_LOGIC_SIZE	2
#define TEV_ALPHAFUNC_LOGIC_SHIFT	22
#define TEV_ALPHAFUNC_LOGIC_MASK	0x00c00000
#define TEV_ALPHAFUNC_GET_LOGIC(tev_alphafunc) \
    ((((unsigned long)(tev_alphafunc)) & TEV_ALPHAFUNC_LOGIC_MASK) >> TEV_ALPHAFUNC_LOGIC_SHIFT)

#define TEV_ALPHAFUNC_RID_SIZE	8
#define TEV_ALPHAFUNC_RID_SHIFT	24
#define TEV_ALPHAFUNC_RID_MASK	0xff000000
#define TEV_ALPHAFUNC_GET_RID(tev_alphafunc) \
    ((((unsigned long)(tev_alphafunc)) & TEV_ALPHAFUNC_RID_MASK) >> TEV_ALPHAFUNC_RID_SHIFT)

#define TEV_Z_ENV_0_ZOFF_SIZE	24
#define TEV_Z_ENV_0_ZOFF_SHIFT	0
#define TEV_Z_ENV_0_ZOFF_MASK	0x00ffffff
#define TEV_Z_ENV_0_GET_ZOFF(tev_z_env_0) \
    ((((unsigned long)(tev_z_env_0)) & TEV_Z_ENV_0_ZOFF_MASK) >> TEV_Z_ENV_0_ZOFF_SHIFT)

#define TEV_Z_ENV_0_RID_SIZE	8
#define TEV_Z_ENV_0_RID_SHIFT	24
#define TEV_Z_ENV_0_RID_MASK	0xff000000
#define TEV_Z_ENV_0_GET_RID(tev_z_env_0) \
    ((((unsigned long)(tev_z_env_0)) & TEV_Z_ENV_0_RID_MASK) >> TEV_Z_ENV_0_RID_SHIFT)

#define TEV_Z_ENV_1_TYPE_SIZE	2
#define TEV_Z_ENV_1_TYPE_SHIFT	0
#define TEV_Z_ENV_1_TYPE_MASK	0x00000003
#define TEV_Z_ENV_1_GET_TYPE(tev_z_env_1) \
    ((((unsigned long)(tev_z_env_1)) & TEV_Z_ENV_1_TYPE_MASK) >> TEV_Z_ENV_1_TYPE_SHIFT)

#define TEV_Z_ENV_1_OP_SIZE	2
#define TEV_Z_ENV_1_OP_SHIFT	2
#define TEV_Z_ENV_1_OP_MASK	0x0000000c
#define TEV_Z_ENV_1_GET_OP(tev_z_env_1) \
    ((((unsigned long)(tev_z_env_1)) & TEV_Z_ENV_1_OP_MASK) >> TEV_Z_ENV_1_OP_SHIFT)

#define TEV_Z_ENV_1_PAD0_SIZE	20
#define TEV_Z_ENV_1_PAD0_SHIFT	4
#define TEV_Z_ENV_1_PAD0_MASK	0x00fffff0
#define TEV_Z_ENV_1_GET_PAD0(tev_z_env_1) \
    ((((unsigned long)(tev_z_env_1)) & TEV_Z_ENV_1_PAD0_MASK) >> TEV_Z_ENV_1_PAD0_SHIFT)

#define TEV_Z_ENV_1_RID_SIZE	8
#define TEV_Z_ENV_1_RID_SHIFT	24
#define TEV_Z_ENV_1_RID_MASK	0xff000000
#define TEV_Z_ENV_1_GET_RID(tev_z_env_1) \
    ((((unsigned long)(tev_z_env_1)) & TEV_Z_ENV_1_RID_MASK) >> TEV_Z_ENV_1_RID_SHIFT)

typedef struct {
    unsigned long rid : 8;
    unsigned long dest : 2;
    unsigned long shift : 2;
    unsigned long clamp : 1;
    unsigned long sub : 1;
    unsigned long bias : 2;
    unsigned long sela : 4;
    unsigned long selb : 4;
    unsigned long selc : 4;
    unsigned long seld : 4;
} tev_color_env_t;

typedef struct {
    unsigned long rid : 8;
    unsigned long dest : 2;
    unsigned long shift : 2;
    unsigned long clamp : 1;
    unsigned long sub : 1;
    unsigned long bias : 2;
    unsigned long sela : 3;
    unsigned long selb : 3;
    unsigned long selc : 3;
    unsigned long seld : 3;
    unsigned long swap : 2;
    unsigned long mode : 2;
} tev_alpha_env_t;

#define SC_TEV_COLOR_ENV_SET_SELD(line, tev_color_env,seld) \
	FAST_GPFLAGSET(line, tev_color_env,seld,TEV_COLOR_ENV_SELD)

#define SC_TEV_COLOR_ENV_SET_SELC(line, tev_color_env,selc) \
	FAST_GPFLAGSET(line, tev_color_env,selc,TEV_COLOR_ENV_SELC)

#define SC_TEV_COLOR_ENV_SET_SELB(line, tev_color_env,selb) \
	FAST_GPFLAGSET(line, tev_color_env,selb,TEV_COLOR_ENV_SELB)

#define SC_TEV_COLOR_ENV_SET_SELA(line, tev_color_env,sela) \
	FAST_GPFLAGSET(line, tev_color_env,sela,TEV_COLOR_ENV_SELA)

#define SC_TEV_COLOR_ENV_SET_BIAS(line, tev_color_env,bias) \
	FAST_GPFLAGSET(line, tev_color_env,bias,TEV_COLOR_ENV_BIAS)

#define SC_TEV_COLOR_ENV_SET_SUB(line, tev_color_env,sub) \
	FAST_GPFLAGSET(line, tev_color_env,sub,TEV_COLOR_ENV_SUB)

#define SC_TEV_COLOR_ENV_SET_CLAMP(line, tev_color_env,clamp) \
	FAST_GPFLAGSET(line, tev_color_env,clamp,TEV_COLOR_ENV_CLAMP)

#define SC_TEV_COLOR_ENV_SET_SHIFT(line, tev_color_env,shift) \
	FAST_GPFLAGSET(line, tev_color_env,shift,TEV_COLOR_ENV_SHIFT)

#define SC_TEV_COLOR_ENV_SET_DEST(line, tev_color_env,dest) \
	FAST_GPFLAGSET(line, tev_color_env,dest,TEV_COLOR_ENV_DEST)

#define SC_TEV_COLOR_ENV_SET_RID(line, tev_color_env,rid) \
	FAST_GPFLAGSET(line, tev_color_env,rid,TEV_COLOR_ENV_RID)

#define SC_TEV_ALPHA_ENV_SET_MODE(line, tev_alpha_env,mode) \
	FAST_GPFLAGSET(line, tev_alpha_env,mode,TEV_ALPHA_ENV_MODE)

#define SC_TEV_ALPHA_ENV_SET_SWAP(line, tev_alpha_env,swap) \
	FAST_GPFLAGSET(line, tev_alpha_env,swap,TEV_ALPHA_ENV_SWAP)

#define SC_TEV_ALPHA_ENV_SET_SELD(line, tev_alpha_env,seld) \
	FAST_GPFLAGSET(line, tev_alpha_env,seld,TEV_ALPHA_ENV_SELD)

#define SC_TEV_ALPHA_ENV_SET_SELC(line, tev_alpha_env,selc) \
	FAST_GPFLAGSET(line, tev_alpha_env,selc,TEV_ALPHA_ENV_SELC)

#define SC_TEV_ALPHA_ENV_SET_SELB(line, tev_alpha_env,selb) \
	FAST_GPFLAGSET(line, tev_alpha_env,selb,TEV_ALPHA_ENV_SELB)

#define SC_TEV_ALPHA_ENV_SET_SELA(line, tev_alpha_env,sela) \
	FAST_GPFLAGSET(line, tev_alpha_env,sela,TEV_ALPHA_ENV_SELA)

#define SC_TEV_ALPHA_ENV_SET_BIAS(line, tev_alpha_env,bias) \
	FAST_GPFLAGSET(line, tev_alpha_env,bias,TEV_ALPHA_ENV_BIAS)

#define SC_TEV_ALPHA_ENV_SET_SUB(line, tev_alpha_env,sub) \
	FAST_GPFLAGSET(line, tev_alpha_env,sub,TEV_ALPHA_ENV_SUB)

#define SC_TEV_ALPHA_ENV_SET_CLAMP(line, tev_alpha_env,clamp) \
	FAST_GPFLAGSET(line, tev_alpha_env,clamp,TEV_ALPHA_ENV_CLAMP)

#define SC_TEV_ALPHA_ENV_SET_SHIFT(line, tev_alpha_env,shift) \
	FAST_GPFLAGSET(line, tev_alpha_env,shift,TEV_ALPHA_ENV_SHIFT)

#define SC_TEV_ALPHA_ENV_SET_DEST(line, tev_alpha_env,dest) \
	FAST_GPFLAGSET(line, tev_alpha_env,dest,TEV_ALPHA_ENV_DEST)

#define SC_TEV_ALPHA_ENV_SET_RID(line, tev_alpha_env,rid) \
	FAST_GPFLAGSET(line, tev_alpha_env,rid,TEV_ALPHA_ENV_RID)

#define SC_TEV_REGISTERL_SET_R(line, tev_registerl,r) \
	FAST_GPFLAGSET(line, tev_registerl,r,TEV_REGISTERL_R)

#define SC_TEV_REGISTERL_SET_PAD0(line, tev_registerl,pad0) \
	FAST_GPFLAGSET(line, tev_registerl,pad0,TEV_REGISTERL_PAD0)

#define SC_TEV_REGISTERL_SET_A(line, tev_registerl,a) \
	FAST_GPFLAGSET(line, tev_registerl,a,TEV_REGISTERL_A)

#define SC_TEV_REGISTERL_SET_PAD1(line, tev_registerl,pad1) \
	FAST_GPFLAGSET(line, tev_registerl,pad1,TEV_REGISTERL_PAD1)

#define SC_TEV_REGISTERL_SET_RID(line, tev_registerl,rid) \
	FAST_GPFLAGSET(line, tev_registerl,rid,TEV_REGISTERL_RID)

#define SC_TEV_REGISTERH_SET_B(line, tev_registerh,b) \
	FAST_GPFLAGSET(line, tev_registerh,b,TEV_REGISTERH_B)

#define SC_TEV_REGISTERH_SET_PAD0(line, tev_registerh,pad0) \
	FAST_GPFLAGSET(line, tev_registerh,pad0,TEV_REGISTERH_PAD0)

#define SC_TEV_REGISTERH_SET_G(line, tev_registerh,g) \
	FAST_GPFLAGSET(line, tev_registerh,g,TEV_REGISTERH_G)

#define SC_TEV_REGISTERH_SET_PAD1(line, tev_registerh,pad1) \
	FAST_GPFLAGSET(line, tev_registerh,pad1,TEV_REGISTERH_PAD1)

#define SC_TEV_REGISTERH_SET_RID(line, tev_registerh,rid) \
	FAST_GPFLAGSET(line, tev_registerh,rid,TEV_REGISTERH_RID)

#define SC_TEV_KREGISTERL_SET_R(line, tev_kregisterl,r) \
	FAST_GPFLAGSET(line, tev_kregisterl,r,TEV_KREGISTERL_R)

#define SC_TEV_KREGISTERL_SET_PAD0(line, tev_kregisterl,pad0) \
	FAST_GPFLAGSET(line, tev_kregisterl,pad0,TEV_KREGISTERL_PAD0)

#define SC_TEV_KREGISTERL_SET_A(line, tev_kregisterl,a) \
	FAST_GPFLAGSET(line, tev_kregisterl,a,TEV_KREGISTERL_A)

#define SC_TEV_KREGISTERL_SET_PAD1(line, tev_kregisterl,pad1) \
	FAST_GPFLAGSET(line, tev_kregisterl,pad1,TEV_KREGISTERL_PAD1)

#define SC_TEV_KREGISTERL_SET_RID(line, tev_kregisterl,rid) \
	FAST_GPFLAGSET(line, tev_kregisterl,rid,TEV_KREGISTERL_RID)

#define SC_TEV_KREGISTERH_SET_B(line, tev_kregisterh,b) \
	FAST_GPFLAGSET(line, tev_kregisterh,b,TEV_KREGISTERH_B)

#define SC_TEV_KREGISTERH_SET_PAD0(line, tev_kregisterh,pad0) \
	FAST_GPFLAGSET(line, tev_kregisterh,pad0,TEV_KREGISTERH_PAD0)

#define SC_TEV_KREGISTERH_SET_G(line, tev_kregisterh,g) \
	FAST_GPFLAGSET(line, tev_kregisterh,g,TEV_KREGISTERH_G)

#define SC_TEV_KREGISTERH_SET_PAD1(line, tev_kregisterh,pad1) \
	FAST_GPFLAGSET(line, tev_kregisterh,pad1,TEV_KREGISTERH_PAD1)

#define SC_TEV_KREGISTERH_SET_RID(line, tev_kregisterh,rid) \
	FAST_GPFLAGSET(line, tev_kregisterh,rid,TEV_KREGISTERH_RID)

#define SC_TEV_FOG_PARAM_0_SET_A_MANT(line, tev_fog_param_0,a_mant) \
	FAST_GPFLAGSET(line, tev_fog_param_0,a_mant,TEV_FOG_PARAM_0_A_MANT)

#define SC_TEV_FOG_PARAM_0_SET_A_EXPN(line, tev_fog_param_0,a_expn) \
	FAST_GPFLAGSET(line, tev_fog_param_0,a_expn,TEV_FOG_PARAM_0_A_EXPN)

#define SC_TEV_FOG_PARAM_0_SET_A_SIGN(line, tev_fog_param_0,a_sign) \
	FAST_GPFLAGSET(line, tev_fog_param_0,a_sign,TEV_FOG_PARAM_0_A_SIGN)

#define SC_TEV_FOG_PARAM_0_SET_PAD0(line, tev_fog_param_0,pad0) \
	FAST_GPFLAGSET(line, tev_fog_param_0,pad0,TEV_FOG_PARAM_0_PAD0)

#define SC_TEV_FOG_PARAM_0_SET_RID(line, tev_fog_param_0,rid) \
	FAST_GPFLAGSET(line, tev_fog_param_0,rid,TEV_FOG_PARAM_0_RID)

#define SC_TEV_FOG_PARAM_1_SET_B_MAG(line, tev_fog_param_1,b_mag) \
	FAST_GPFLAGSET(line, tev_fog_param_1,b_mag,TEV_FOG_PARAM_1_B_MAG)

#define SC_TEV_FOG_PARAM_1_SET_RID(line, tev_fog_param_1,rid) \
	FAST_GPFLAGSET(line, tev_fog_param_1,rid,TEV_FOG_PARAM_1_RID)

#define SC_TEV_FOG_PARAM_2_SET_B_SHF(line, tev_fog_param_2,b_shf) \
	FAST_GPFLAGSET(line, tev_fog_param_2,b_shf,TEV_FOG_PARAM_2_B_SHF)

#define SC_TEV_FOG_PARAM_2_SET_PAD0(line, tev_fog_param_2,pad0) \
	FAST_GPFLAGSET(line, tev_fog_param_2,pad0,TEV_FOG_PARAM_2_PAD0)

#define SC_TEV_FOG_PARAM_2_SET_RID(line, tev_fog_param_2,rid) \
	FAST_GPFLAGSET(line, tev_fog_param_2,rid,TEV_FOG_PARAM_2_RID)

#define SC_TEV_FOG_PARAM_3_SET_C_MANT(line, tev_fog_param_3,c_mant) \
	FAST_GPFLAGSET(line, tev_fog_param_3,c_mant,TEV_FOG_PARAM_3_C_MANT)

#define SC_TEV_FOG_PARAM_3_SET_C_EXPN(line, tev_fog_param_3,c_expn) \
	FAST_GPFLAGSET(line, tev_fog_param_3,c_expn,TEV_FOG_PARAM_3_C_EXPN)

#define SC_TEV_FOG_PARAM_3_SET_C_SIGN(line, tev_fog_param_3,c_sign) \
	FAST_GPFLAGSET(line, tev_fog_param_3,c_sign,TEV_FOG_PARAM_3_C_SIGN)

#define SC_TEV_FOG_PARAM_3_SET_PROJ(line, tev_fog_param_3,proj) \
	FAST_GPFLAGSET(line, tev_fog_param_3,proj,TEV_FOG_PARAM_3_PROJ)

#define SC_TEV_FOG_PARAM_3_SET_FSEL(line, tev_fog_param_3,fsel) \
	FAST_GPFLAGSET(line, tev_fog_param_3,fsel,TEV_FOG_PARAM_3_FSEL)

#define SC_TEV_FOG_PARAM_3_SET_RID(line, tev_fog_param_3,rid) \
	FAST_GPFLAGSET(line, tev_fog_param_3,rid,TEV_FOG_PARAM_3_RID)

#define SC_TEV_RANGE_ADJ_C_SET_CENTER(line, tev_range_adj_c,center) \
	FAST_GPFLAGSET(line, tev_range_adj_c,center,TEV_RANGE_ADJ_C_CENTER)

#define SC_TEV_RANGE_ADJ_C_SET_ENB(line, tev_range_adj_c,enb) \
	FAST_GPFLAGSET(line, tev_range_adj_c,enb,TEV_RANGE_ADJ_C_ENB)

#define SC_TEV_RANGE_ADJ_C_SET_PAD0(line, tev_range_adj_c,pad0) \
	FAST_GPFLAGSET(line, tev_range_adj_c,pad0,TEV_RANGE_ADJ_C_PAD0)

#define SC_TEV_RANGE_ADJ_C_SET_RID(line, tev_range_adj_c,rid) \
	FAST_GPFLAGSET(line, tev_range_adj_c,rid,TEV_RANGE_ADJ_C_RID)

#define SC_TEV_RANGE_ADJ_SET_R0(line, tev_range_adj,r0) \
	FAST_GPFLAGSET(line, tev_range_adj,r0,TEV_RANGE_ADJ_R0)

#define SC_TEV_RANGE_ADJ_SET_R1(line, tev_range_adj,r1) \
	FAST_GPFLAGSET(line, tev_range_adj,r1,TEV_RANGE_ADJ_R1)

#define SC_TEV_RANGE_ADJ_SET_RID(line, tev_range_adj,rid) \
	FAST_GPFLAGSET(line, tev_range_adj,rid,TEV_RANGE_ADJ_RID)

#define SC_TEV_FOG_COLOR_SET_B(line, tev_fog_color,b) \
	FAST_GPFLAGSET(line, tev_fog_color,b,TEV_FOG_COLOR_B)

#define SC_TEV_FOG_COLOR_SET_G(line, tev_fog_color,g) \
	FAST_GPFLAGSET(line, tev_fog_color,g,TEV_FOG_COLOR_G)

#define SC_TEV_FOG_COLOR_SET_R(line, tev_fog_color,r) \
	FAST_GPFLAGSET(line, tev_fog_color,r,TEV_FOG_COLOR_R)

#define SC_TEV_FOG_COLOR_SET_RID(line, tev_fog_color,rid) \
	FAST_GPFLAGSET(line, tev_fog_color,rid,TEV_FOG_COLOR_RID)

#define SC_TEV_ALPHAFUNC_SET_A0(line, tev_alphafunc,a0) \
	FAST_GPFLAGSET(line, tev_alphafunc,a0,TEV_ALPHAFUNC_A0)

#define SC_TEV_ALPHAFUNC_SET_A1(line, tev_alphafunc,a1) \
	FAST_GPFLAGSET(line, tev_alphafunc,a1,TEV_ALPHAFUNC_A1)

#define SC_TEV_ALPHAFUNC_SET_OP0(line, tev_alphafunc,op0) \
	FAST_GPFLAGSET(line, tev_alphafunc,op0,TEV_ALPHAFUNC_OP0)

#define SC_TEV_ALPHAFUNC_SET_OP1(line, tev_alphafunc,op1) \
	FAST_GPFLAGSET(line, tev_alphafunc,op1,TEV_ALPHAFUNC_OP1)

#define SC_TEV_ALPHAFUNC_SET_LOGIC(line, tev_alphafunc,logic) \
	FAST_GPFLAGSET(line, tev_alphafunc,logic,TEV_ALPHAFUNC_LOGIC)

#define SC_TEV_ALPHAFUNC_SET_RID(line, tev_alphafunc,rid) \
	FAST_GPFLAGSET(line, tev_alphafunc,rid,TEV_ALPHAFUNC_RID)

#define SC_TEV_Z_ENV_0_SET_ZOFF(line, tev_z_env_0,zoff) \
	FAST_GPFLAGSET(line, tev_z_env_0,zoff,TEV_Z_ENV_0_ZOFF)

#define SC_TEV_Z_ENV_0_SET_RID(line, tev_z_env_0,rid) \
	FAST_GPFLAGSET(line, tev_z_env_0,rid,TEV_Z_ENV_0_RID)

#define SC_TEV_Z_ENV_1_SET_TYPE(line, tev_z_env_1,type) \
	FAST_GPFLAGSET(line, tev_z_env_1,type,TEV_Z_ENV_1_TYPE)

#define SC_TEV_Z_ENV_1_SET_OP(line, tev_z_env_1,op) \
	FAST_GPFLAGSET(line, tev_z_env_1,op,TEV_Z_ENV_1_OP)

#define SC_TEV_Z_ENV_1_SET_PAD0(line, tev_z_env_1,pad0) \
	FAST_GPFLAGSET(line, tev_z_env_1,pad0,TEV_Z_ENV_1_PAD0)

#define SC_TEV_Z_ENV_1_SET_RID(line, tev_z_env_1,rid) \
	FAST_GPFLAGSET(line, tev_z_env_1,rid,TEV_Z_ENV_1_RID)

#define SC_TEV_KSEL_SET_XRB(line, tev_ksel,xrb) \
	FAST_GPFLAGSET(line, tev_ksel,xrb,TEV_KSEL_XRB)

#define SC_TEV_KSEL_SET_XGA(line, tev_ksel,xga) \
	FAST_GPFLAGSET(line, tev_ksel,xga,TEV_KSEL_XGA)

#define SC_TEV_KSEL_SET_KCSEL0(line, tev_ksel,kcsel0) \
	FAST_GPFLAGSET(line, tev_ksel,kcsel0,TEV_KSEL_KCSEL0)

#define SC_TEV_KSEL_SET_KASEL0(line, tev_ksel,kasel0) \
	FAST_GPFLAGSET(line, tev_ksel,kasel0,TEV_KSEL_KASEL0)

#define SC_TEV_KSEL_SET_KCSEL1(line, tev_ksel,kcsel1) \
	FAST_GPFLAGSET(line, tev_ksel,kcsel1,TEV_KSEL_KCSEL1)

#define SC_TEV_KSEL_SET_KASEL1(line, tev_ksel,kasel1) \
	FAST_GPFLAGSET(line, tev_ksel,kasel1,TEV_KSEL_KASEL1)

#define SC_TEV_KSEL_SET_RID(line, tev_ksel,rid) \
	FAST_GPFLAGSET(line, tev_ksel,rid,TEV_KSEL_RID)


#endif // TEV_REG_H
