#ifndef _DOLPHIN_GX_GXPERF_H_
#define _DOLPHIN_GX_GXPERF_H_

#include <dolphin/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXSetGPMetric(GXPerf0 perf0, GXPerf1 perf1);
void GXReadGPMetric(u32* cnt0, u32* cnt1);
void GXClearGPMetric(void);
u32 GXReadGP0Metric(void);
u32 GXReadGP1Metric(void);
void GXReadMemMetric(u32* cp_req, u32* tc_req, u32* cpu_rd_req, u32* cpu_wr_req, u32* dsp_req, u32* io_req, u32* vi_req, u32* pe_req, u32* rf_req, u32* fi_req);
void GXClearMemMetric(void);
void GXReadPixMetric(u32* top_pixels_in, u32* top_pixels_out, u32* bot_pixels_in, u32* bot_pixels_out, u32* clr_pixels_in, u32* copy_clks);
void GXClearPixMetric(void);
void GXSetVCacheMetric(GXVCachePerf attr);
void GXReadVCacheMetric(u32* check, u32* miss, u32* stall);
void GXClearVCacheMetric(void);
void GXInitXfRasMetric(void);
void GXReadXfRasMetric(u32* xf_wait_in, u32* xf_wait_out, u32* ras_busy, u32* clocks);
u32 GXReadClksPerVtx(void);

#ifdef __cplusplus
}
#endif

#endif
