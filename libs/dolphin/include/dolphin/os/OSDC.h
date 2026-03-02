#ifndef _DOLPHIN_OSDC_H_
#define _DOLPHIN_OSDC_H_

#ifdef __cplusplus
extern "C" {
#endif

void DCFlashInvalidate(void);
void DCEnable(void);
void DCDisable(void);
void DCFreeze(void);
void DCUnfreeze(void);
void DCTouchLoad(void* addr);
void DCBlockZero(void* addr);
void DCBlockStore(void* addr);
void DCBlockFlush(void* addr);
void DCBlockInvalidate(void* addr);

#ifdef __cplusplus
}
#endif

#endif
