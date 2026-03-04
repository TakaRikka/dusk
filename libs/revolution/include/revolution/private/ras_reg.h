#ifndef RAS_REG
#define RAS_REG

#define RAS1_TREF_RID_SIZE	8
#define RAS1_TREF_RID_SHIFT	24
#define RAS1_TREF_RID_MASK	0xff000000
#define RAS1_TREF_GET_RID(ras1_tref) \
	((((unsigned long)(ras1_tref)) & RAS1_TREF_RID_MASK) >> RAS1_TREF_RID_SHIFT)

#define RAS1_IREF_RID_SIZE	8
#define RAS1_IREF_RID_SHIFT	24
#define RAS1_IREF_RID_MASK	0xff000000
#define RAS1_IREF_GET_RID(ras1_iref) \
    ((((unsigned long)(ras1_iref)) & RAS1_IREF_RID_MASK) >> RAS1_IREF_RID_SHIFT)

#define RAS_PERF_SELA_SIZE	5
#define RAS_PERF_SELA_SHIFT	0
#define RAS_PERF_SELA_MASK	0x0000001f
#define RAS_PERF_GET_SELA(ras_perf) \
    ((((unsigned long)(ras_perf)) & RAS_PERF_SELA_MASK) >> RAS_PERF_SELA_SHIFT)

#define RAS_PERF_SELB_SIZE	5
#define RAS_PERF_SELB_SHIFT	5
#define RAS_PERF_SELB_MASK	0x000003e0
#define RAS_PERF_GET_SELB(ras_perf) \
    ((((unsigned long)(ras_perf)) & RAS_PERF_SELB_MASK) >> RAS_PERF_SELB_SHIFT)

#define RAS_PERF_NTEV_SIZE	5
#define RAS_PERF_NTEV_SHIFT	10
#define RAS_PERF_NTEV_MASK	0x00007c00
#define RAS_PERF_GET_NTEV(ras_perf) \
    ((((unsigned long)(ras_perf)) & RAS_PERF_NTEV_MASK) >> RAS_PERF_NTEV_SHIFT)

#define RAS_PERF_NBMP_SIZE	3
#define RAS_PERF_NBMP_SHIFT	15
#define RAS_PERF_NBMP_MASK	0x00038000
#define RAS_PERF_GET_NBMP(ras_perf) \
    ((((unsigned long)(ras_perf)) & RAS_PERF_NBMP_MASK) >> RAS_PERF_NBMP_SHIFT)

#define RAS_PERF_NBR_SIZE	3
#define RAS_PERF_NBR_SHIFT	18
#define RAS_PERF_NBR_MASK	0x001c0000
#define RAS_PERF_GET_NBR(ras_perf) \
    ((((unsigned long)(ras_perf)) & RAS_PERF_NBR_MASK) >> RAS_PERF_NBR_SHIFT)

#define RAS_PERF_PAD0_SIZE	3
#define RAS_PERF_PAD0_SHIFT	21
#define RAS_PERF_PAD0_MASK	0x00e00000
#define RAS_PERF_GET_PAD0(ras_perf) \
    ((((unsigned long)(ras_perf)) & RAS_PERF_PAD0_MASK) >> RAS_PERF_PAD0_SHIFT)
#define RAS_PERF_RID_SIZE	8
#define RAS_PERF_RID_SHIFT	24
#define RAS_PERF_RID_MASK	0xff000000
#define RAS_PERF_GET_RID(ras_perf) \
    ((((unsigned long)(ras_perf)) & RAS_PERF_RID_MASK) >> RAS_PERF_RID_SHIFT)
    
#define RAS_PERF_TOTAL_SIZE	32
#define RAS_PERF(selA, selB, ntev, nbmp, nbr, rid) \
    ((((unsigned long)(selA)) << RAS_PERF_SELA_SHIFT) | \
        (((unsigned long)(selB)) << RAS_PERF_SELB_SHIFT) | \
        (((unsigned long)(ntev)) << RAS_PERF_NTEV_SHIFT) | \
        (((unsigned long)(nbmp)) << RAS_PERF_NBMP_SHIFT) | \
        (((unsigned long)(nbr)) << RAS_PERF_NBR_SHIFT) | \
        (((unsigned long)(rid)) << RAS_PERF_RID_SHIFT))

#define RAS1_TREF_TI0_SIZE	3
#define RAS1_TREF_TI0_SHIFT	0
#define RAS1_TREF_TI0_MASK	0x00000007
#define RAS1_TREF_GET_TI0(ras1_tref) \
    ((((unsigned long)(ras1_tref)) & RAS1_TREF_TI0_MASK) >> RAS1_TREF_TI0_SHIFT)

#define RAS1_TREF_TC0_SIZE	3
#define RAS1_TREF_TC0_SHIFT	3
#define RAS1_TREF_TC0_MASK	0x00000038
#define RAS1_TREF_GET_TC0(ras1_tref) \
    ((((unsigned long)(ras1_tref)) & RAS1_TREF_TC0_MASK) >> RAS1_TREF_TC0_SHIFT)

#define RAS1_TREF_TE0_SIZE	1
#define RAS1_TREF_TE0_SHIFT	6
#define RAS1_TREF_TE0_MASK	0x00000040
#define RAS1_TREF_GET_TE0(ras1_tref) \
    ((((unsigned long)(ras1_tref)) & RAS1_TREF_TE0_MASK) >> RAS1_TREF_TE0_SHIFT)

#define RAS1_TREF_CC0_SIZE	3
#define RAS1_TREF_CC0_SHIFT	7
#define RAS1_TREF_CC0_MASK	0x00000380
#define RAS1_TREF_GET_CC0(ras1_tref) \
    ((((unsigned long)(ras1_tref)) & RAS1_TREF_CC0_MASK) >> RAS1_TREF_CC0_SHIFT)

#define RAS1_TREF_PAD0_SIZE	2
#define RAS1_TREF_PAD0_SHIFT	10
#define RAS1_TREF_PAD0_MASK	0x00000c00
#define RAS1_TREF_GET_PAD0(ras1_tref) \
    ((((unsigned long)(ras1_tref)) & RAS1_TREF_PAD0_MASK) >> RAS1_TREF_PAD0_SHIFT)

#define RAS1_TREF_TI1_SIZE	3
#define RAS1_TREF_TI1_SHIFT	12
#define RAS1_TREF_TI1_MASK	0x00007000
#define RAS1_TREF_GET_TI1(ras1_tref) \
    ((((unsigned long)(ras1_tref)) & RAS1_TREF_TI1_MASK) >> RAS1_TREF_TI1_SHIFT)

#define RAS1_TREF_TC1_SIZE	3
#define RAS1_TREF_TC1_SHIFT	15
#define RAS1_TREF_TC1_MASK	0x00038000
#define RAS1_TREF_GET_TC1(ras1_tref) \
    ((((unsigned long)(ras1_tref)) & RAS1_TREF_TC1_MASK) >> RAS1_TREF_TC1_SHIFT)

#define RAS1_TREF_TE1_SIZE	1
#define RAS1_TREF_TE1_SHIFT	18
#define RAS1_TREF_TE1_MASK	0x00040000
#define RAS1_TREF_GET_TE1(ras1_tref) \
    ((((unsigned long)(ras1_tref)) & RAS1_TREF_TE1_MASK) >> RAS1_TREF_TE1_SHIFT)

#define RAS1_TREF_CC1_SIZE	3
#define RAS1_TREF_CC1_SHIFT	19
#define RAS1_TREF_CC1_MASK	0x00380000
#define RAS1_TREF_GET_CC1(ras1_tref) \
    ((((unsigned long)(ras1_tref)) & RAS1_TREF_CC1_MASK) >> RAS1_TREF_CC1_SHIFT)

#define RAS1_TREF_PAD1_SIZE	2
#define RAS1_TREF_PAD1_SHIFT	22
#define RAS1_TREF_PAD1_MASK	0x00c00000
#define RAS1_TREF_GET_PAD1(ras1_tref) \
    ((((unsigned long)(ras1_tref)) & RAS1_TREF_PAD1_MASK) >> RAS1_TREF_PAD1_SHIFT)

#define RAS1_TREF_RID_SIZE	8
#define RAS1_TREF_RID_SHIFT	24
#define RAS1_TREF_RID_MASK	0xff000000
#define RAS1_TREF_GET_RID(ras1_tref) \
    ((((unsigned long)(ras1_tref)) & RAS1_TREF_RID_MASK) >> RAS1_TREF_RID_SHIFT)

#define SC_RAS1_SS_SET_SS0(line, ras1_ss,ss0) \
	FAST_GPFLAGSET(line, ras1_ss,ss0,RAS1_SS_SS0)

#define SC_RAS1_SS_SET_TS0(line, ras1_ss,ts0) \
	FAST_GPFLAGSET(line, ras1_ss,ts0,RAS1_SS_TS0)

#define SC_RAS1_SS_SET_SS1(line, ras1_ss,ss1) \
	FAST_GPFLAGSET(line, ras1_ss,ss1,RAS1_SS_SS1)

#define SC_RAS1_SS_SET_TS1(line, ras1_ss,ts1) \
	FAST_GPFLAGSET(line, ras1_ss,ts1,RAS1_SS_TS1)

#define SC_RAS1_SS_SET_PAD0(line, ras1_ss,pad0) \
	FAST_GPFLAGSET(line, ras1_ss,pad0,RAS1_SS_PAD0)

#define SC_RAS1_SS_SET_RID(line, ras1_ss,rid) \
	FAST_GPFLAGSET(line, ras1_ss,rid,RAS1_SS_RID)

#define SC_RAS1_IREF_SET_BI0(line, ras1_iref,bi0) \
	FAST_GPFLAGSET(line, ras1_iref,bi0,RAS1_IREF_BI0)

#define SC_RAS1_IREF_SET_BC0(line, ras1_iref,bc0) \
	FAST_GPFLAGSET(line, ras1_iref,bc0,RAS1_IREF_BC0)

#define SC_RAS1_IREF_SET_BI1(line, ras1_iref,bi1) \
	FAST_GPFLAGSET(line, ras1_iref,bi1,RAS1_IREF_BI1)

#define SC_RAS1_IREF_SET_BC1(line, ras1_iref,bc1) \
	FAST_GPFLAGSET(line, ras1_iref,bc1,RAS1_IREF_BC1)

#define SC_RAS1_IREF_SET_BI2(line, ras1_iref,bi2) \
	FAST_GPFLAGSET(line, ras1_iref,bi2,RAS1_IREF_BI2)

#define SC_RAS1_IREF_SET_BC2(line, ras1_iref,bc2) \
	FAST_GPFLAGSET(line, ras1_iref,bc2,RAS1_IREF_BC2)

#define SC_RAS1_IREF_SET_BI3(line, ras1_iref,bi3) \
	FAST_GPFLAGSET(line, ras1_iref,bi3,RAS1_IREF_BI3)

#define SC_RAS1_IREF_SET_BC3(line, ras1_iref,bc3) \
	FAST_GPFLAGSET(line, ras1_iref,bc3,RAS1_IREF_BC3)

#define SC_RAS1_IREF_SET_RID(line, ras1_iref,rid) \
	FAST_GPFLAGSET(line, ras1_iref,rid,RAS1_IREF_RID)

#define SC_RAS1_TREF_SET_TI0(line, ras1_tref,ti0) \
	FAST_GPFLAGSET(line, ras1_tref,ti0,RAS1_TREF_TI0)

#define SC_RAS1_TREF_SET_TC0(line, ras1_tref,tc0) \
	FAST_GPFLAGSET(line, ras1_tref,tc0,RAS1_TREF_TC0)

#define SC_RAS1_TREF_SET_TE0(line, ras1_tref,te0) \
	FAST_GPFLAGSET(line, ras1_tref,te0,RAS1_TREF_TE0)

#define SC_RAS1_TREF_SET_CC0(line, ras1_tref,cc0) \
	FAST_GPFLAGSET(line, ras1_tref,cc0,RAS1_TREF_CC0)

#define SC_RAS1_TREF_SET_PAD0(line, ras1_tref,pad0) \
	FAST_GPFLAGSET(line, ras1_tref,pad0,RAS1_TREF_PAD0)

#define SC_RAS1_TREF_SET_TI1(line, ras1_tref,ti1) \
	FAST_GPFLAGSET(line, ras1_tref,ti1,RAS1_TREF_TI1)

#define SC_RAS1_TREF_SET_TC1(line, ras1_tref,tc1) \
	FAST_GPFLAGSET(line, ras1_tref,tc1,RAS1_TREF_TC1)

#define SC_RAS1_TREF_SET_TE1(line, ras1_tref,te1) \
	FAST_GPFLAGSET(line, ras1_tref,te1,RAS1_TREF_TE1)

#define SC_RAS1_TREF_SET_CC1(line, ras1_tref,cc1) \
	FAST_GPFLAGSET(line, ras1_tref,cc1,RAS1_TREF_CC1)

#define SC_RAS1_TREF_SET_PAD1(line, ras1_tref,pad1) \
	FAST_GPFLAGSET(line, ras1_tref,pad1,RAS1_TREF_PAD1)

#define SC_RAS1_TREF_SET_RID(line, ras1_tref,rid) \
	FAST_GPFLAGSET(line, ras1_tref,rid,RAS1_TREF_RID)

#define SC_RAS_PERF_SET_SELA(line, ras_perf,selA) \
	FAST_GPFLAGSET(line, ras_perf,selA,RAS_PERF_SELA)

#define SC_RAS_PERF_SET_SELB(line, ras_perf,selB) \
	FAST_GPFLAGSET(line, ras_perf,selB,RAS_PERF_SELB)

#define SC_RAS_PERF_SET_NTEV(line, ras_perf,ntev) \
	FAST_GPFLAGSET(line, ras_perf,ntev,RAS_PERF_NTEV)

#define SC_RAS_PERF_SET_NBMP(line, ras_perf,nbmp) \
	FAST_GPFLAGSET(line, ras_perf,nbmp,RAS_PERF_NBMP)

#define SC_RAS_PERF_SET_NBR(line, ras_perf,nbr) \
	FAST_GPFLAGSET(line, ras_perf,nbr,RAS_PERF_NBR)

#define SC_RAS_PERF_SET_PAD0(line, ras_perf,pad0) \
	FAST_GPFLAGSET(line, ras_perf,pad0,RAS_PERF_PAD0)

#define SC_RAS_PERF_SET_RID(line, ras_perf,rid) \
	FAST_GPFLAGSET(line, ras_perf,rid,RAS_PERF_RID)


#endif // RAS_REG
