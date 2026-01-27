//#include <dolphin/dolphin.h>
#include <dolphin/card.h>
#include <dolphin/mtx.h>
#include <dolphin/gx/GXVert.h>
#include <dolphin/gx/GXManage.h>
#include <dolphin/ai.h>
#include <dolphin/ar.h>
#include <dolphin/card.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include <dolphin/os/OSCache.h>
#include <dolphin/dsp.h>
#include <dolphin/dvd.h>
#include <dolphin/gd/GDBase.h>
#include <dolphin/gx/GXGeometry.h>
#include <dolphin/gx/GXFifo.h>
#include <dolphin/gx/GXFrameBuffer.h>
#include <dolphin/gx/GXGet.h>
#include <dolphin/gx/GXTexture.h>
#include <dolphin/gx/GXCpu2Efb.h>
#include <dolphin/gx/GXPerf.h>
#include <dolphin/gx/GXCull.h>
#include <dolphin/gx/GXPixel.h>
#include <dolphin/os/OSAlarm.h>
#include <dolphin/os/OSRtc.h>
#include <dolphin/os/OSContext.h>
#include <dolphin/os/OSReset.h>
#include <dolphin/os/OSResetSW.h>
#include <dolphin/os/OSAlloc.h>
#include <dolphin/os/OSMutex.h>
#include <dolphin/os/OSModule.h>
#include <dolphin/os/OSMemory.h>
#include <dolphin/os/OSMessage.h>
#include <dolphin/os/OSReboot.h>
#include <stdarg.h>
#include <stdio.h>

// Credits: Super Monkey Ball

static VIRetraceCallback sVIRetraceCallback = NULL;

void OSReport(const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  vprintf(msg, args);
  va_end(args);
}

u32 OSGetConsoleType() { return OS_CONSOLE_RETAIL1; }

u32 OSGetSoundMode() { return 2; }

s32 CARDCancel(CARDFileInfo *fileInfo) {
  puts("CARDCancel is a stub");
  return 0;
}

s32 CARDCheck(s32 chan) {
  puts("CARDCheck is a stub");
  return 0;
}

s32 CARDCheckAsync(s32 chan, CARDCallback callback) {
  puts("CARDCheckAsync is a stub");
  return 0;
}

s32 CARDClose(CARDFileInfo *fileInfo) {
  puts("CARDClose is a stub");
  return 0;
}

s32 CARDCreate(s32 chan, const char *fileName, u32 size,
               CARDFileInfo *fileInfo) {
  puts("CARDCreate is a stub");
  return 0;
}

s32 CARDCreateAsync(s32 chan, const char *fileName, u32 size,
                    CARDFileInfo *fileInfo, CARDCallback callback) {
  puts("CARDCreateAsync is a stub");
  return 0;
}

s32 CARDDelete(s32 chan, const char *fileName) {
  puts("CARDDelete is a stub");
  return 0;
}

s32 CARDDeleteAsync(s32 chan, const char *fileName, CARDCallback callback) {
  puts("CARDDeleteAsync is a stub");
  return 0;
}

s32 CARDFastDeleteAsync(s32 chan, s32 fileNo, CARDCallback callback) {
  puts("CARDFastDeleteAsync is a stub");
  return 0;
}

s32 CARDFastOpen(s32 chan, s32 fileNo, CARDFileInfo *fileInfo) {
  puts("CARDFastOpen is a stub");
  return 0;
}

s32 CARDFormat(s32 chan) {
  puts("CARDFormat is a stub");
  return 0;
}

s32 CARDFreeBlocks(s32 chan, s32 *byteNotUsed, s32 *filesNotUsed) {
  puts("CARDFreeBlocks is a stub");
  return 0;
}

s32 CARDGetResultCode(s32 chan) {
  puts("CARDGetResultCode is a stub");
  return 0;
}

s32 CARDGetStatus(s32 chan, s32 fileNo, CARDStat *stat) {
  puts("CARDGetStatus is a stub");
  return 0;
}

s32 CARDGetSectorSize(s32 chan, u32 *size) {
  puts("CARDGetSectorSize is a stub");
  return 0;
}

void CARDInit() { puts("CARDInit is a stub"); }

s32 CARDMount(s32 chan, void *workArea, CARDCallback detachCallback) {
  puts("CARDMount is a stub");
  return 0;
}

s32 CARDMountAsync(s32 chan, void *workArea, CARDCallback detachCallback,
                   CARDCallback attachCallback) {
  puts("CARDMountAsync is a stub");
  return 0;
}

s32 CARDOpen(s32 chan, const char *fileName, CARDFileInfo *fileInfo) {
  puts("CARDOpen is a stub");
  return 0;
}

s32 CARDProbeEx(s32 chan, s32 *memSize, s32 *sectorSize) {
  puts("CARDProbeEx is a stub");
  return 0;
}

s32 CARDRead(CARDFileInfo *fileInfo, void *addr, s32 length, s32 offset) {
  puts("CARDRead is a stub");
  return 0;
}

s32 CARDReadAsync(CARDFileInfo *fileInfo, void *addr, s32 length, s32 offset,
                  CARDCallback callback) {
  puts("CARDReadAsync is a stub");
  return 0;
}

s32 CARDRename(s32 chan, const char *oldName, const char *newName) {
  puts("CARDRename is a stub");
  return 0;
}

s32 CARDRenameAsync(s32 chan, const char *oldName, const char *newName,
                    CARDCallback callback) {
  puts("CARDRenameAsync is a stub");
  return 0;
}

s32 CARDSetStatusAsync(s32 chan, s32 fileNo, CARDStat *stat,
                       CARDCallback callback) {
  puts("CARDSetStatusAsync is a stub");
  return 0;
}

s32 CARDUnmount(s32 chan) {
  puts("CARDUnmount is a stub");
  return 0;
}

s32 CARDWrite(CARDFileInfo *fileInfo, void *addr, s32 length,
              s32 offset) {
  puts("CARDWrite is a stub");
  return 0;
}

s32 CARDWriteAsync(CARDFileInfo *fileInfo, void *addr, s32 length,
                   s32 offset, CARDCallback callback) {
  puts("CARDWriteAsync is a stub");
  return 0;
}

s32 CARDGetSerialNo(s32 chan, u64 *serialNo) { return 0; }

s32 CARDSetStatus(s32 chan, s32 fileNo, CARDStat *stat) { return 0; }

void DCFlushRange(void *addr, u32 nBytes) {
  // puts("DCFlushRange is a stub");
}

void DCFlushRangeNoSync(void *addr, u32 nBytes) {
  // puts("DCFlushRangeNoSync is a stub");
}

void DCInvalidateRange(void *addr, u32 nBytes) {
  // puts("DCInvalidateRange is a stub");
}

void DCStoreRange(void *addr, u32 nBytes) {
  // puts("DCStoreRange is a stub");
}

void DCStoreRangeNoSync(void *addr, u32 nBytes) {
  // puts("DCStoreRangeNoSync is a stub");
}

s32 DVDCancel(volatile DVDCommandBlock *block) {
  puts("DVDCancel is a stub");
  return 0;
}

int DVDReadAbsAsyncForBS(DVDCommandBlock *block, void *addr, s32 length, s32 offset,
                          DVDCBCallback callback) {
  puts("DVDReadAbsAsyncForBS is a stub");
  return 0;
}

int DVDReadDiskID(DVDCommandBlock* block, DVDDiskID* diskID, DVDCBCallback callback) {
  puts("DVDReadDiskID is a stub");
  return 0;
}

void DVDReset() { puts("DVDReset is a stub"); }

BOOL EXIDeselect(int chan) {
  puts("EXIDeselect is a stub");
  return FALSE;
}

BOOL EXIDma(int chan, void *buffer, s32 size, int d, int e) {
  puts("EXIDma is a stub");
  return FALSE;
}

BOOL EXIImm(int chan, u32 *b, int c, int d, int e) {
  puts("EXIImm is a stub");
  return FALSE;
}

BOOL EXILock(int chan, int b, int c) {
  puts("EXILock is a stub");
  return FALSE;
}

BOOL EXISelect(int chan, int b, int c) {
  puts("EXISelect is a stub");
  return FALSE;
}

BOOL EXISync(int chan) {
  puts("EXISync is a stub");
  return FALSE;
}

BOOL EXIUnlock(int chan) {
  puts("EXIUnlock is a stub");
  return FALSE;
}

void LCEnable() { puts("LCEnable is a stub"); }

void OSClearContext(OSContext *context) { puts("OSClearContext is a stub"); }

BOOL OSDisableInterrupts() {
  puts("OSDisableInterrupts is a stub");
  return FALSE;
}

void OSDumpContext(OSContext *context) { puts("OSDumpContext is a stub"); }

OSThread *OSGetCurrentThread() {
  puts("OSGetCurrentThread is a stub");
  return 0;
}

u16 OSGetFontEncode() {
  puts("OSGetFontEncode is a stub");
  return 0;
}

char *OSGetFontTexture(const char* string, void** image, s32* x, s32* y, s32* width) {
  puts("OSGetFontTexture is a stub");
  return 0;
}

char *OSGetFontWidth(const char* string, s32* width) {
  puts("OSGetFontWidth is a stub");
  return 0;
}

BOOL OSGetResetButtonState() {
  puts("OSGetResetButtonState is a stub");
  return FALSE;
}

u32 OSGetStackPointer() {
  puts("OSGetStackPointer is a stub");
  return 0;
}

BOOL OSInitFont(OSFontHeader *fontData) {
  puts("OSInitFont is a stub");
  return FALSE;
}

BOOL OSLink(OSModuleInfo *newModule, void *bss) {
  puts("OSLink is a stub");
  return TRUE;
}

void OSLoadContext(OSContext *context) { puts("OSLoadContext is a stub"); }

void OSResetSystem(int reset, u32 resetCode, BOOL forceMenu) {
  puts("OSResetSystem is a stub");
}

BOOL OSRestoreInterrupts(BOOL level) {
  puts("OSRestoreInterrupts is a stub");
  return FALSE;
}

s32 OSResumeThread(OSThread *thread) {
  puts("OSResumeThread is a stub");
  return 0;
}

void OSSetCurrentContext(OSContext *context) {
  puts("OSSetCurrentContext is a stub");
}

void OSSetStringTable(void* stringTable) {
  puts("OSSetStringTable is a stub");
}

s32 OSSuspendThread(OSThread *thread) {
  puts("OSSuspendThread is a stub");
  return 0;
}

void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime *td) {
  puts("OSTicksToCalendarTime is a stub");
}

BOOL OSUnlink(OSModuleInfo *oldModule) {
  puts("OSUnlink is a stub");
  return FALSE;
}

void OSWakeupThread(OSThreadQueue *queue) { puts("OSWakeupThread is a stub"); }

void PPCHalt() { puts("PPCHalt is a stub"); }

void SoundChoID(int a, int b) { puts("SoundChoID is a stub"); }

void SoundPan(int a, int b, int c) { puts("SoundPan is a stub"); }

void SoundPitch(u16 a, int b) { puts("SoundPitch is a stub"); }

void SoundRevID(int a, int b) { puts("SoundRevID is a stub"); }

void VIConfigure(const GXRenderModeObj *rm) { puts("VIConfigure is a stub"); }

void VIConfigurePan(u16 xOrg, u16 yOrg, u16 width, u16 height) {
  puts("VIConfigurePan is a stub");
}

u32 VIGetRetraceCount() {
  // puts("VIGetRetraceCount is a stub");
  return 0; // TODO this might be important
}

u32 VIGetNextField() {
  puts("VIGetNextField is a stub");
  return 0;
}

void VISetBlack(BOOL black) { puts("VISetBlack is a stub"); }

void VISetNextFrameBuffer(void *fb) {
  // puts("VISetNextFrameBuffer is a stub");
}

void VIWaitForRetrace() {
  if (sVIRetraceCallback) {
    sVIRetraceCallback(0);
  }
}

s32 __CARDFormatRegionAsync(int a, int b) {
  puts("__CARDFormatRegionAsync is a stub");
  return 0;
}

void __GXSetSUTexSize() { puts("__GXSetSUTexSize is a stub"); }

void __GXSetVAT() { puts("__GXSetVAT is a stub"); }

void __GXSetVCD() { puts("__GXSetVCD is a stub"); }

void __GXUpdateBPMask() { puts("__GXUpdateBPMask is a stub"); }

u32 __OSGetDIConfig() {
  puts("__OSGetDIConfig is a stub");
  return 0;
}

__OSInterruptHandler __OSSetInterruptHandler(__OSInterrupt interrupt,
                                             __OSInterruptHandler handler) {
  puts("__OSSetInterruptHandler is a stub");
  return 0;
}

OSInterruptMask __OSUnmaskInterrupts(OSInterruptMask mask) {
  puts("__OSUnmaskInterrupts is a stub");
  return 0;
}

void SISetSamplingRate(u32 msec) {
  // Maybe we could include SI later
  puts("SISetSamplingRate is a stub");
}

VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback) {
  sVIRetraceCallback = callback;
  return callback;
}

void GXSetGPMetric(GXPerf0 perf0, GXPerf1 perf1) {
  // puts("GXSetGPMetric is a stub");
}

void GXReadGPMetric(u32 *cnt0, u32 *cnt1) {
  // puts("GXReadGPMetric is a stub");
}

void GXClearGPMetric(void) {
  // puts("GXClearGPMetric is a stub");
}

void GXReadMemMetric(u32 *cp_req, u32 *tc_req, u32 *cpu_rd_req, u32 *cpu_wr_req,
                     u32 *dsp_req, u32 *io_req, u32 *vi_req, u32 *pe_req,
                     u32 *rf_req, u32 *fi_req) {
  // puts("GXReadMemMetric is a stub");
}

void GXClearMemMetric(void) {
  // puts("GXClearMemMetric is a stub");
}

void GXClearVCacheMetric(void) {
  // puts("GXClearVCacheMetric is a stub");
}

void GXReadPixMetric(u32 *top_pixels_in, u32 *top_pixels_out,
                     u32 *bot_pixels_in, u32 *bot_pixels_out,
                     u32 *clr_pixels_in, u32 *copy_clks) {
  // puts("GXReadPixMetric is a stub");
}

void GXClearPixMetric(void) {
  // puts("GXClearPixMetric is a stub");
}

void GXSetVCacheMetric(GXVCachePerf attr) {
  // puts("GXSetVCacheMetric is a stub");
}

void GXReadVCacheMetric(u32 *check, u32 *miss, u32 *stall) {
  // puts("GXReadVCacheMetric is a stub");
}

void GXSetDrawSync(u16 token) {
  // puts("GXSetDrawSync is a stub");
}

GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback cb) {
  puts("GXSetDrawSyncCallback is a stub");
  // TODO
  return cb;
}

void PPCSync(void) {
  // puts("PPCSync is a stub");
}

void GXWaitDrawDone(void) {
  // puts("GXWaitDrawDone is a stub");
}

void GXSetTevIndTile(GXTevStageID tev_stage, GXIndTexStageID ind_stage,
                     u16 tilesize_s, u16 tilesize_t, u16 tilespacing_s,
                     u16 tilespacing_t, GXIndTexFormat format,
                     GXIndTexMtxID matrix_sel, GXIndTexBiasSel bias_sel,
                     GXIndTexAlphaSel alpha_sel) {
  // TODO
  // puts("GXSetTevIndTile is a stub");
}

void GXResetWriteGatherPipe(void) {
  // puts("GXResetWriteGatherPipe is a stub");
}

void ARQInit(void) { puts("ARQInit is a stub"); }

void GXProject(f32 x, f32 y, f32 z, const f32 mtx[3][4], const f32 *pm,
               const f32 *vp, f32 *sx, f32 *sy, f32 *sz) {
  Vec peye;
  f32 xc;
  f32 yc;
  f32 zc;
  f32 wc;

  peye.x = mtx[0][3] + ((mtx[0][2] * z) + ((mtx[0][0] * x) + (mtx[0][1] * y)));
  peye.y = mtx[1][3] + ((mtx[1][2] * z) + ((mtx[1][0] * x) + (mtx[1][1] * y)));
  peye.z = mtx[2][3] + ((mtx[2][2] * z) + ((mtx[2][0] * x) + (mtx[2][1] * y)));
  if (pm[0] == 0.0f) {
    xc = (peye.x * pm[1]) + (peye.z * pm[2]);
    yc = (peye.y * pm[3]) + (peye.z * pm[4]);
    zc = pm[6] + (peye.z * pm[5]);
    wc = 1.0f / -peye.z;
  } else {
    xc = pm[2] + (peye.x * pm[1]);
    yc = pm[4] + (peye.y * pm[3]);
    zc = pm[6] + (peye.z * pm[5]);
    wc = 1.0f;
  }
  *sx = (vp[2] / 2.0f) + (vp[0] + (wc * (xc * vp[2] / 2.0f)));
  *sy = (vp[3] / 2.0f) + (vp[1] + (wc * (-yc * vp[3] / 2.0f)));
  *sz = vp[5] + (wc * (zc * (vp[5] - vp[4])));
}

void GXGetViewportv(f32 *vp) {
  // TODO
}

void OSSetSoundMode(u32 mode) {}

u32 AIGetDSPSampleRate(void) {
  puts("AIGetDSPSampleRate is a stub");
  return 0;
}

void AIInit(u8* stack) {
  puts("AIInit is a stub");
}

void AIInitDMA(u32 start_addr, u32 length) {
  puts("AIInitDMA is a stub");
}

AIDCallback AIRegisterDMACallback(AIDCallback callback) {
  puts("AIRegisterDMACallback is a stub");
  return NULL;
}

void AISetDSPSampleRate(u32 rate) {
  puts("AISetDSPSampleRate is a stub");
}

void AIStartDMA(void) {
  puts("AIStartDMA is a stub");
}

void AIStopDMA(void) {
  puts("AIStopDMA is a stub");
}

u32 ARAlloc(u32 length) {
  puts("ARAlloc is a stub");
  return 0;
}

u32 ARGetSize(void) {
  puts("ARGetSize is a stub");
  return 0;
}

u32 ARInit(u32* stack_index_addr, u32 num_entries) {
  puts("ARInit is a stub");
  return 0;
}

void ARQPostRequest(ARQRequest* request, u32 owner, u32 type, u32 priority, u32 source, u32 dest, u32 length, ARQCallback callback) {
  puts("ARQPostRequest is a stub");
}

int CARDProbe(s32 chan) {
  puts("CARDProbe is a stub");
  return 0;
}

void C_MTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS, f32 scaleT, f32 transS,
                     f32 transT) {
  puts("C_MTXLightOrtho is a stub");
}

void C_MTXLightPerspective(Mtx m, f32 fovY, f32 aspect, f32 scaleS, f32 scaleT, f32 transS,
                           f32 transT) {
  puts("C_MTXLightPerspective is a stub");
}

void C_MTXLookAt(Mtx m, const Point3d* camPos, const Vec* camUp, const Point3d* target) {
  puts("C_MTXLookAt is a stub");
}

void C_MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f) {
  puts("C_MTXOrtho is a stub");
}

void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f) {
  puts("C_MTXPerspective is a stub");
}

void C_MTXRotAxisRad(Mtx m, const Vec* axis, f32 rad) {
  puts("C_MTXRotAxisRad is a stub");
}

void C_QUATRotAxisRad(Quaternion* r, const Vec* axis, f32 rad) {
  puts("C_QUATRotAxisRad is a stub");
}

void C_QUATSlerp(const Quaternion* p, const Quaternion* q, Quaternion* r, f32 t) {
  puts("C_QUATSlerp is a stub");
}

void C_VECHalfAngle(const Vec* a, const Vec* b, Vec* half) {
  puts("C_VECHalfAngle is a stub");
}

void C_VECReflect(const Vec* src, const Vec* normal, Vec* dst) {
  puts("C_VECReflect is a stub");
}

void DCZeroRange(void* addr, u32 nBytes) {
  puts("C_VECReflect is a stub");
}

void DSPAssertInt(void) {
  puts("DSPAssertInt is a stub");
}

u32 DSPCheckMailFromDSP(void) {
  puts("DSPCheckMailFromDSP is a stub");
  return 0;
}

u32 DSPCheckMailToDSP(void) {
  puts("DSPCheckMailToDSP is a stub");
  return 0;
}

void DSPInit(void) {
  puts("DSPInit is a stub");
}

u32 DSPReadMailFromDSP(void) {
  puts("DSPReadMailFromDSP is a stub");
  return 0;
}

void DSPSendMailToDSP(u32 mail) {
  puts("DSPSendMailToDSP is a stub");
}

BOOL DVDChangeDir(const char* dirName) {
  puts("DVDChangeDir is a stub");
  return FALSE;
}

BOOL DVDCheckDisk(void) {
  puts("DVDCheckDisk is a stub");
  return FALSE;
}

BOOL DVDClose(DVDFileInfo* fileInfo) {
  puts("DVDClose is a stub");
  return FALSE;
}

int DVDCloseDir(DVDDir* dir) {
  puts("DVDCloseDir is a stub");
  return 0;
}

s32 DVDConvertPathToEntrynum(const char* pathPtr) {
  puts("DVDConvertPathToEntrynum is a stub");
  return 0;
}

BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo) {
  puts("DVDFastOpen is a stub");
  return FALSE;
}

s32 DVDGetCommandBlockStatus(const DVDCommandBlock* block) {
  puts("DVDGetCommandBlockStatus is a stub");
  return 0;
}

DVDDiskID* DVDGetCurrentDiskID(void) {
  puts("DVDGetCurrentDiskID is a stub");
  return NULL;
}

s32 DVDGetDriveStatus(void) {
  puts("DVDGetDriveStatus is a stub");
  return 0;
}

void DVDInit(void) {
  puts("DVDInit is a stub");
}

BOOL DVDOpen(const char* fileName, DVDFileInfo* fileInfo) {
  puts("DVDOpen is a stub");
  return FALSE;
}

int DVDOpenDir(const char* dirName, DVDDir* dir) {
  puts("DVDOpenDir is a stub");
  return 0;
}

BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                      DVDCallback callback, s32 prio) {
  puts("DVDReadAsyncPrio is a stub");
  return FALSE;
}

int DVDReadDir(DVDDir* dir, DVDDirEntry* dirent) {
  puts("DVDReadDir is a stub");
  return 0;
}

s32 DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, s32 prio) {
  puts("DVDReadPrio is a stub");
  return 0;
}

void GDFlushCurrToMem(void) {
  puts("GDFlushCurrToMem is a stub");
}

void GDInitGDLObj(GDLObj* dl, void* start, u32 length) {
  puts("GDInitGDLObj is a stub");
}

void GDOverflowed(void) {
  puts("GDOverflowed is a stub");
}

void GDPadCurr32(void) {
  puts("GDPadCurr32 is a stub");
}

void GDSetArray(GXAttr attr, void* base_ptr, u8 stride) {
  puts("GDSetArray is a stub");
}

void GDSetArrayRaw(GXAttr attr, u32 base_ptr_raw, u8 stride) {
  puts("GDSetArrayRaw is a stub");
}

void GDSetVtxDescv(const GXVtxDescList* attrPtr) {
  puts("GDSetVtxDescv is a stub");
}

void GXAbortFrame(void) {
  puts("GXAbortFrame is a stub");
}

void GXEnableTexOffsets(GXTexCoordID coord, GXBool line_enable, GXBool point_enable) {
  puts("GXEnableTexOffsets is a stub");
}

OSThread* GXGetCurrentGXThread(void) {
  puts("GXGetCurrentGXThread is a stub");
  return NULL;
}

void* GXGetFifoBase(const GXFifoObj* fifo) {
  puts("GXGetFifoBase is a stub");
  return NULL;
}

u32 GXGetFifoSize(const GXFifoObj* fifo) {
  puts("GXGetFifoSize is a stub");
  return 0;
}

u16 GXGetNumXfbLines(u16 efbHeight, f32 yScale) {
  puts("GXGetNumXfbLines is a stub");
  return 0;
}

void GXGetScissor(u32* left, u32* top, u32* wd, u32* ht) {
  puts("GXGetScissor is a stub");
}

u32 GXGetTexObjTlut(const GXTexObj* tex_obj) {
  puts("GXGetTexObjTlut is a stub");
  return 0;
}

f32 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight) {
  puts("GXGetYScaleFactor is a stub");
  return 0.f;
}

void GXInitTexCacheRegion(GXTexRegion* region, GXBool is_32b_mipmap, u32 tmem_even, GXTexCacheSize size_even,
                          u32 tmem_odd, GXTexCacheSize size_odd) {
  puts("GXInitTexCacheRegion is a stub");
}

GXRenderModeObj GXNtsc480Int = {
    0, 640, 480, 480, 40, 0, 640, 480, 1, 0, 0, { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 }, { 0, 0, 21, 22, 21, 0, 0 }
};

void GXPeekZ(u16 x, u16 y, u32* z) {
  puts("GXPeekZ is a stub");
}

void GXReadXfRasMetric(u32* xf_wait_in, u32* xf_wait_out, u32* ras_busy, u32* clocks) {
  puts("GXReadXfRasMetric is a stub");
}

void GXSetClipMode(GXClipMode mode) {
  puts("GXSetClipMode is a stub");
}

void GXSetCoPlanar(GXBool enable) {
  puts("GXSetCoPlanar is a stub");
}

void GXSetCopyClamp(GXFBClamp clamp) {
  puts("GXSetCopyClamp is a stub");
}

OSThread* GXSetCurrentGXThread(void) {
  puts("GXSetCurrentGXThread is a stub");
  return NULL;
}

void GXSetFogRangeAdj(GXBool enable, u16 center, const GXFogAdjTable* table) {
  puts("GXSetFogRangeAdj is a stub");
}

void GXSetMisc(GXMiscToken token, u32 val) {
  puts("GXSetMisc is a stub");
}

void GXSetPointSize(u8 pointSize, GXTexOffset texOffsets) {
  puts("GXSetPointSize is a stub");
}

void GXSetProjectionv(const f32* ptr) {
  puts("GXSetProjectionv is a stub");
}

void GXSetVtxAttrFmtv(GXVtxFmt vtxfmt, const GXVtxAttrFmtList* list) {
  puts("GXSetVtxAttrFmtv is a stub");
}

void KPADDisableDPD(s32) {
  puts("KPADDisableDPD is a stub");
}

void KPADEnableDPD(s32) {
  puts("KPADEnableDPD is a stub");
}

void LCDisable(void) {
  puts("LCDisable is a stub");
}

void LCQueueWait(u32 len) {
  puts("LCQueueWait is a stub");
}

u32 LCStoreData(void* destAddr, void* srcAddr, u32 nBytes) {
  puts("LCStoreData is a stub");
  return 0;
}

void* OSAllocFromArenaLo(u32 size, u32 align) {
  puts("OSAllocFromArenaLo is a stub");
  return NULL;
}

void OSCancelAlarm(OSAlarm *alarm) {
  puts("OSCancelAlarm is a stub");
}

void OSCancelThread(OSThread* thread) {
  puts("OSCancelThread is a stub");
}

s32 OSCheckActiveThreads(void) {
  puts("OSCheckActiveThreads is a stub");
  return 0;
}

void OSCreateAlarm(OSAlarm* alarm) {
  puts("OSCreateAlarm is a stub");
}

int OSCreateThread(OSThread* thread, void* (*func)(void*), void* param, void* stack, u32 stackSize, OSPriority priority, u16 attr) {
  puts("OSCreateThread is a stub");
  return 0;
}

void OSDetachThread(OSThread* thread) {
  puts("OSDetachThread is a stub");
}

s32 OSDisableScheduler(void) {
  puts("OSDisableScheduler is a stub");
  return 0;
}

BOOL OSEnableInterrupts(void) {
  puts("OSEnableInterrupts is a stub");
  return FALSE;
}

s32 OSEnableScheduler(void) {
  puts("OSEnableScheduler is a stub");
  return 0;
}

void OSExitThread(void* val) {
  puts("OSExitThread is a stub");
}

void OSFillFPUContext(OSContext* context) {
  puts("OSFillFPUContext is a stub");
}

void* OSGetArenaHi(void) {
  puts("OSGetArenaHi is a stub");
  return NULL;
}

void* OSGetArenaLo(void) {
  puts("OSGetArenaLo is a stub");
  return NULL;
}

OSContext* OSGetCurrentContext(void) {
  puts("OSGetCurrentContext is a stub");
  return NULL;
}

u32 OSGetProgressiveMode(void) {
  puts("OSGetProgressiveMode is a stub");
  return 0;
}

u32 OSGetResetCode() {
  puts("OSGetResetCode is a stub");
  return 0;
}

BOOL OSGetResetSwitchState(void) {
  puts("OSGetResetSwitchState is a stub");
  return FALSE;
}

s32 OSGetThreadPriority(OSThread* thread) {
  puts("OSGetThreadPriority is a stub");
  return 0;
}

OSTick OSGetTick(void) {
  puts("OSGetTick is a stub");
  return 0;
}

OSTime OSGetTime(void) {
  puts("OSGetTime is a stub");
  return 0;
}

void OSInit(void) {
  puts("OSInit is a stub");
}

void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps) {
  puts("OSInitAlloc is a stub");
  return NULL;
}

void OSInitCond(OSCond* cond) {
  puts("OSInitCond is a stub");
}

void OSInitMessageQueue(OSMessageQueue* mq, void* msgArray, s32 msgCount) {
  puts("OSInitMessageQueue is a stub");
}

void OSInitMutex(OSMutex* mutex) {
  puts("OSInitMutex is a stub");
}

void OSInitThreadQueue(OSThreadQueue* queue) {
  puts("OSInitThreadQueue is a stub");
}

BOOL OSIsThreadTerminated(OSThread* thread) {
  puts("OSIsThreadTerminated is a stub");
  return FALSE;
}

int OSJamMessage(OSMessageQueue* mq, void* msg, s32 flags) {
  puts("OSJamMessage is a stub");
  return 0;
}

BOOL OSLinkFixed(OSModuleInfo* newModule, void* bss) {
  puts("OSLinkFixed is a stub");
  return FALSE;
}

void OSLockMutex(OSMutex* mutex) {
  puts("OSLockMutex is a stub");
}

void OSProtectRange(u32 chan, void* addr, u32 nBytes, u32 control) {
  puts("OSProtectRange is a stub");
}

int OSReceiveMessage(OSMessageQueue* mq, void* msg, s32 flags) {
  puts("OSReceiveMessage is a stub");
  return 0;
}

int OSSendMessage(OSMessageQueue* mq, void* msg, s32 flags) {
  puts("OSSendMessage is a stub");
  return 0;
}

void OSSetAlarm(OSAlarm* alarm, OSTime tick, OSAlarmHandler handler) {
  puts("OSSetAlarm is a stub");
}

void OSSetArenaHi(void* newHi) {
  puts("OSSetArenaHi is a stub");
}

void OSSetArenaLo(void* newLo) {
  puts("OSSetArenaLo is a stub");
}

OSErrorHandler OSSetErrorHandler(OSError error, OSErrorHandler handler) {
  puts("OSSetErrorHandler is a stub");
  return NULL;
}

void OSSetPeriodicAlarm(OSAlarm* alarm, OSTime start, OSTime period, OSAlarmHandler handler) {
  puts("OSSetPeriodicAlarm is a stub");
}

void OSSetProgressiveMode(u32 on) {
  puts("OSSetProgressiveMode is a stub");
}

void OSSetSaveRegion(void* start, void* end) {
  puts("OSSetSaveRegion is a stub");
}

OSSwitchThreadCallback OSSetSwitchThreadCallback(OSSwitchThreadCallback callback) {
  puts("OSSetSwitchThreadCallback is a stub");
  return NULL;
}

int OSSetThreadPriority(OSThread* thread, OSPriority priority) {
  puts("OSSetThreadPriority is a stub");
  return 0;
}

void OSSignalCond(OSCond* cond) {
  puts("OSSignalCond is a stub");
}

void OSSleepThread(OSThreadQueue* queue) {
  puts("OSSleepThread is a stub");
}

BOOL OSTryLockMutex(OSMutex* mutex) {
  puts("OSTryLockMutex is a stub");
  return FALSE;
}

void OSUnlockMutex(OSMutex* mutex) {
  puts("OSUnlockMutex is a stub");
}

void OSWaitCond(OSCond* cond, OSMutex* mutex) {
  puts("OSWaitCond is a stub");
}

void OSYieldThread(void) {
  puts("OSYieldThread is a stub");
}
