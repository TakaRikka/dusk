#include <dolphin/dolphin.h>
#include <dolphin/gx/GXVert.h>
#include <stdarg.h>
#include <stdio.h>

// Credits: Super Monkey Ball

<<<<<<< HEAD
# pragma mark OS
=======
>>>>>>> wip/linkfix2
void OSReport(const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  vprintf(msg, args);
  va_end(args);
}

u32 OSGetConsoleType() { return OS_CONSOLE_RETAIL1; }

u32 OSGetSoundMode() { return 2; }

// Consolidated OS functions (moved from other sections)
void OSClearContext(OSContext *context) { puts("OSClearContext is a stub"); }

void OSInit() { puts("OSInit is a stub"); }

void OSInitMutex(OSMutex *mutex) { puts("OSInitMutex is a stub"); }

void OSUnlockMutex(OSMutex *mutex) { puts("OSUnlockMutex is a stub"); }

BOOL OSTryLockMutex(OSMutex *mutex) {
  puts("OSTryLockMutex is a stub");
  return FALSE;
}

void *OSAllocFromArenaLo(u32 size, u32 align) {
  puts("OSAllocFromArenaLo is a stub");
  return NULL;
}

BOOL OSDisableInterrupts() {
  puts("OSDisableInterrupts is a stub");
  return FALSE;
}

void OSSleepThread(OSThreadQueue *queue) { puts("OSSleepThread is a stub"); }

void OSDumpContext(OSContext *context) { puts("OSDumpContext is a stub"); }

void OSSignalCond(OSCond *cond) { puts("OSSignalCond is a stub"); }

void OSCreateAlarm(OSAlarm *alarm) { puts("OSCreateAlarm is a stub"); }

void OSCancelAlarm(OSAlarm *alarm) { puts("OSCancelAlarm is a stub"); }

s32 OSCheckActiveThreads(void) { puts("OSCheckActiveThreads is a stub"); }

int OSCreateThread(OSThread* thread, void* (*func)(void*), void* param, void* stack, u32 stackSize, OSPriority priority, u16 attr) {
  puts("OSCreateThread is a stub");
  return 0;
}

s32 OSDisableScheduler() {
  puts("OSDisableScheduler is a stub");
  return 0;
}

void OSDetachThread(OSThread* thread) {
  puts("OSDetachThread is a stub");
}

OSThread *OSGetCurrentThread() {
  puts("OSGetCurrentThread is a stub");
  return 0;
}

u16 OSGetFontEncode() {
  puts("OSGetFontEncode is a stub");
  return 0;
}

char *OSGetFontTexture(char *string, void **image, s32 *x, s32 *y, s32 *width) {
  puts("OSGetFontTexture is a stub");
  return 0;
}

char *OSGetFontWidth(char *string, s32 *width) {
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

OSSwitchThreadCallback OSSetSwitchThreadCallback(OSSwitchThreadCallback callback) {
  puts("OSSetSwitchThreadCallback is a stub");
  return NULL;
}

int OSSetThreadPriority(OSThread* thread, s32 priority) {
  puts("OSSetThreadPriority is a stub");
  return 0;
}

void OSWaitCond(OSCond* cond, OSMutex* mutex) {
  puts("OSWaitCond is a stub");
}

void OSYieldThread(void) {
  puts("OSYieldThread is a stub");
}

s32 OSSuspendThread(OSThread *thread) {
  puts("OSSuspendThread is a stub");
  return 0;
}

void OSCancelThread(OSThread *thread) {
  puts("OSCancelThread is a stub");
}

void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime *td) {
  puts("OSTicksToCalendarTime is a stub");
}

BOOL OSUnlink(OSModuleInfo *oldModule) {
  puts("OSUnlink is a stub");
  return FALSE;
}

void OSSwitchFiberEx(__REGISTER u32 param_0, __REGISTER u32 param_1, __REGISTER u32 param_2, __REGISTER u32 param_3, __REGISTER u32 code, __REGISTER u32 stack) {
  puts("OSSwitchFiberEx is a stub");
}

void OSWakeupThread(OSThreadQueue *queue) { puts("OSWakeupThread is a stub"); }

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

BOOL OSEnableInterrupts() {
  puts("OSEnableInterrupts is a stub");
  return FALSE;
}

s32 OSEnableScheduler() {
  puts("OSEnableScheduler is a stub");
  return 0;
}

void OSExitThread(void *val) { puts("OSExitThread is a stub"); }

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

u32 OSGetResetCode(void) {
  puts("OSGetResetCode is a stub");
  return 0;
}

BOOL OSGetResetSwitchState() {
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

void OSInitCond(OSCond* cond) {
  puts("OSInitCond is a stub");
}

void OSInitMessageQueue(OSMessageQueue* mq, void* msgArray, s32 msgCount) {
  puts("OSInitMessageQueue is a stub");
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
  return TRUE;
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

void OSSetArenaHi(void* newHi) {
  puts("OSSetArenaHi is a stub");
}

void OSSetArenaLo(void* newLo) {
  puts("OSSetArenaLo is a stub");
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

OSErrorHandler OSSetErrorHandler(OSError error, OSErrorHandler handler) {
  puts("OSSetErrorHandler is a stub");
  return NULL;
}

void OSSetAlarm(OSAlarm* alarm, OSTime tick, OSAlarmHandler handler) {
  puts("OSSetAlarm is a stub");
}

void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps) {
  puts("OSInitAlloc is a stub");
  return NULL;
}

void OSFillFPUContext(__REGISTER OSContext* context) {
  puts("OSFillFPUContext is a stub");
}

void OSSetSoundMode(u32 mode) {}

# pragma mark SOUND
void SoundChoID(int a, int b) { puts("SoundChoID is a stub"); }
void SoundPan(int a, int b, int c) { puts("SoundPan is a stub"); }
void SoundPitch(u16 a, int b) { puts("SoundPitch is a stub"); }
void SoundRevID(int a, int b) { puts("SoundRevID is a stub"); }

# pragma mark CARD

#include <dolphin/card.h>

extern "C" int CARDProbe(s32 chan) {
  puts("CARDProbe is a stub");
  return 0;
}

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

extern "C" s32 CARDWrite(CARDFileInfo *fileInfo, void *addr, s32 length,
              s32 offset) {
  puts("CARDWrite is a stub");
  return 0;
}

s32 CARDWriteAsync(CARDFileInfo *fileInfo, const void *addr, s32 length,
                   s32 offset, CARDCallback callback) {
  puts("CARDWriteAsync is a stub");
  return 0;
}

s32 CARDGetSerialNo(s32 chan, u64 *serialNo) { return 0; }

s32 CARDSetStatus(s32 chan, s32 fileNo, CARDStat *stat) { return 0; }

s32 __CARDFormatRegionAsync(int a, int b) {
  puts("__CARDFormatRegionAsync is a stub");
  return 0;
}

# pragma mark DC

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

# pragma mark EXI

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

# pragma mark LC

void LCEnable() { puts("LCEnable is a stub"); }

// OS-related functions consolidated under "# pragma mark OS" further up

# pragma mark VI

static VIRetraceCallback sVIRetraceCallback = NULL;

extern "C" {

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

void* VIGetCurrentFrameBuffer(void) {
  puts("VIGetCurrentFrameBuffer is a stub");
  return NULL;
}

u32 VIGetDTVStatus(void) {
  puts("VIGetDTVStatus is a stub");
  return 0;
}

void* VIGetNextFrameBuffer(void) {
  puts("VIGetNextFrameBuffer is a stub");
  return NULL;
}

VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback) {
  sVIRetraceCallback = callback;
  return callback;
}

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb) {
  puts("VISetPreRetraceCallback is a stub");
  return cb;
}

} // extern "C"

# pragma mark DSP
#include <dolphin/dsp.h>
extern "C" void __DSP_insert_task(DSPTaskInfo* task) {
  puts("__DSP_insert_task is a stub");
}

extern "C" void __DSP_boot_task(DSPTaskInfo*) {
  puts("__DSP_boot_task is a stub");
}

extern "C" void __DSP_exec_task(DSPTaskInfo*, DSPTaskInfo*) {
  puts("__DSP_exec_task is a stub");
}

extern "C" void __DSP_remove_task(DSPTaskInfo* task) {
  puts("__DSP_remove_task is a stub");
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

# pragma mark Z2Audio
#include <Z2AudioLib/Z2AudioCS.h>
void Z2AudioCS::extensionProcess(s32, s32) {
  puts("Z2AudioMgr::play is a stub");
}

# pragma mark JORServer
#include <JSystem/JHostIO/JORServer.h>
void JORServer::releaseMCTX(JORMContext*) {
  puts("releaseMCTX is a stub");
}

JORMContext* JORServer::attachMCTX(u32) {
  puts("attachMCTX is a stub");
  return NULL;
}

JORServer* JORServer::instance;

void JORMContext::genCheckBoxSub(u32 kind, const char* label, u32 id, u32 style, u16 initValue, u16 mask,
                                 JOREventListener* pListener, u16 posX, u16 posY, u16 width,
                                 u16 height) {
  puts("JORServer::genCheckBoxSub is a stub");
}
void JORMContext::updateCheckBoxSub(u32 mode, u32 id, u16 value, u16 mask, u32 param_4) {
  puts("JORServer::updateCheckBoxSub is a stub");
}

<<<<<<< HEAD
=======
int JOREventCallbackListNode::JORAct(u32, const char*) {
    return 0;
}

>>>>>>> wip/linkfix2
# pragma mark JSUMemoryOutputStream
#include <JSystem/JSupport/JSUMemoryStream.h>
s32 JSUMemoryOutputStream::getAvailable() const {
  return mLength - mPosition;
}

s32 JSUMemoryOutputStream::getPosition() const {
  return mPosition;
}

# pragma mark JSURandomOutputStream
#include <JSystem/JSupport/JSURandomOutputStream.h>
s32 JSUMemoryOutputStream::seek(s32 offset, JSUStreamSeekFrom origin) {
  // XXX I think this is correct? could be broken.
  return this->seekPos(offset, origin);
}

# pragma mark JKRHeap
#include <JSystem/JKernel/JKRHeap.h>
JKRHeap* JKRHeap::sRootHeap2; // XXX this is defined for WII/SHIELD, should we just define it for dusk builds?

# pragma mark mDoExt_onCupOnAupPacket
#include <m_Do/m_Do_ext.h>
mDoExt_offCupOnAupPacket::~mDoExt_offCupOnAupPacket() {
  puts("mDoExt_onCupOffAupPacket_c destructor is a stub");
}
# pragma mark mDoExt_onCupOffAupPacket
mDoExt_onCupOffAupPacket::~mDoExt_onCupOffAupPacket() {
  puts("mDoExt_onCupOffAupPacket_c destructor is a stub");
}

# pragma mark mDoExt
namespace mDoExt {
    u8 CurrentHeapAdjustVerbose;
    u8 HeapAdjustVerbose;
    u8 HeapAdjustQuiet;
};

# pragma mark dKankyo_vrboxHIO_c
#include <d/d_kankyo.h>
void dKankyo_vrboxHIO_c::dKankyo_vrboxHIOInfoUpDateF() {
  puts("dKankyo_vrboxHIO_c::dKankyo_vrboxHIOInfoUpDateF is a stub");
}

void dKankyo_lightHIO_c::dKankyo_lightHIOInfoUpDateF() {
  puts("dKankyo_lightHIO_c::dKankyo_lightHIOInfoUpDateF is a stub");
}

# pragma mark dKankyo_HIO_c
#include <d/d_kankyo.h>
dKankyo_HIO_c::dKankyo_HIO_c() {
    light.m_displayTVColorSettings = FALSE;
    vrbox.m_displayVrboxTVColorSettings = FALSE;
}

dKankyo_ParticlelightHIO_c::dKankyo_ParticlelightHIO_c() {
    field_0x5 = 0;
    prim_col.r = 255;
    prim_col.g = 255;
    prim_col.b = 255;
    prim_col.a = 255;
    env_col.r = 255;
    env_col.g = 255;
    env_col.b = 255;
    env_col.a = 255;
    blend_ratio = 0.5f;
    field_0x14 = 0;
    type = 0;
    field_0x19 = 1;
    field_0x1a = 0;
}

dKankyo_dungeonlightHIO_c::dKankyo_dungeonlightHIO_c() {
    field_0x5 = 0;
    usedLights = 0;
    displayDebugSphere = 0;
    field_0x8 = 0;
    field_0x9 = 0;
}

dKankyo_demolightHIO_c::dKankyo_demolightHIO_c() {
    adjust_ON = 0;
    light.mPosition.x = 0.0f;
    light.mPosition.y = 0.0f;
    light.mPosition.z = 0.0f;
    light.mColor.r = 255;
    light.mColor.g = 255;
    light.mColor.b = 255;
    light.mPow = 1000.0f;
    light.mFluctuation = 0.0f;
}

dKankyo_efflightHIO_c::dKankyo_efflightHIO_c() {
    adjust_ON = 0;
    power = 80.0f;
    fluctuation = 100.0f;

    step1.start_frame = 1;
    step1.r = 191;
    step1.g = 150;
    step1.b = 45;

    step2.start_frame = 4;
    step2.r = 180;
    step2.g = 60;
    step2.b = 0;

    step3.start_frame = 8;
    step3.r = 75;
    step3.g = 15;
    step3.b = 0;

    step4.start_frame = 15;
    step4.r = 0;
    step4.g = 0;
    step4.b = 0;
}

dKankyo_vrboxHIO_c::dKankyo_vrboxHIO_c() {
    m_VrboxSetting = 0;
    field_0x5 = 0;
    field_0x7 = 0;
    field_0x8 = 0;
    field_0x9 = 0;
    field_0xa = 0;
    field_0xb = 0;
    field_0xc = 0;
    field_0xd = 0;
    field_0xe = -1;
    field_0xf = -1;
    field_0x10 = -1;
    field_0x11 = -1;
    field_0x12 = -1;
    field_0x13 = -1;
    field_0x14 = 0;
    m_horizonHeight = 0.0f;
}

dKankyo_lightHIO_c::dKankyo_lightHIO_c() {
    m_HOSTIO_setting = FALSE;
    field_0x52 = 0;
    m_forcedPalette = FALSE;
    m_displayColorPaletteCheckInfo = TRUE;

    field_0x58 = 0.0f;
    field_0x60 = 0;
    field_0x61 = 0;
    field_0x62 = 0;
    field_0x63 = 0;
    field_0x64 = 0;
    field_0x65 = 0;
    field_0x66 = 0;
    field_0x67 = 0;
    field_0x68 = 0;
    field_0x69 = 0;
    field_0x6a = 0;
    field_0x6b = 0;

    m_BG_fakelight_R = 0;
    m_BG_fakelight_G = 0;
    m_BG_fakelight_B = 0;
    m_BG_fakelight_power = 0.0f;

    field_0x80 = 0;
}

dKankyo_bloomHIO_c::dKankyo_bloomHIO_c() {
    field_0x4 = 0;
    m_saturationPattern = 0;
    field_0x5 = 0;

    for (int i = 0; i < 64; i++) {
        dKydata_BloomInfo_c* bloominf = dKyd_BloomInf_tbl_getp(i);
        bloom_info[i] = bloominf->info;
    }
}

dKankyo_windHIO_c::dKankyo_windHIO_c() {
    display_wind_dir = 0;
    use_HOSTIO_adjustment = 0;
    field_0x8 = -1;
    global_x_angle = 0;
    global_y_angle = 0;
    global_wind_power = 0.3f;
    field_0x14 = 0.0;
    field_0x18 = 35.0f;
    field_0x1c = 6.0f;
    display_wind_trajectory = 0;
    lightsword_x_angle = 1800;
    lightsword_init_scale = 500.0f;
    lightsword_end_scale = 300.0f;
    influence = 1.0f;
    lightsword_move_speed = 150.0f;
    influence_attenuation = 0.3f;
    wind_change_speed = 0.05f;
    minigame_no_wind_duration = 90;
    minigame_low_wind_duration = 60;
    minigame_high_wind_duration = 90;
}
dKankyo_navyHIO_c::dKankyo_navyHIO_c() {
    field_0x5 = 0;
    field_0x6 = 0;
    field_0x8 = 12;
    cloud_sunny_wind_influence_rate = 10.0f;
    cloud_sunny_bottom_height = 2500.0f;
    cloud_sunny_top_height = 2500.0f;
    cloud_sunny_size = 0.6f;
    cloud_sunny_height_shrink_rate = 0.9999f;
    cloud_sunny_alpha = 1.0f;
    cloud_cloudy_wind_influence_rate = 25.0f;
    cloud_cloudy_bottom_height = 1200.0f;
    cloud_cloudy_top_height = 1200.0f;
    cloud_cloudy_size = 0.84f;
    cloud_cloudy_height_shrink_rate = 0.96f;
    cloud_cloudy_alpha = 1.0f;
    field_0x3c = 4000.0f;
    field_0x40 = 2000.0f;
    field_0x44 = 2500.0f;
    field_0x48 = 80.0f;
    field_0x4c = 0.18f;
    field_0x68 = 1;
    field_0x69 = 3;
    field_0x50 = 255.0f;
    field_0x58 = 800.0f;
    field_0x5c = 250.0f;
    field_0x54 = 1.0f;
    field_0x60 = 1000.0f;
    field_0x64 = 0.2f;
    housi_max_number = 300;
    housi_max_alpha = 120.0f;
    housi_max_scale = 9.0f;
    field_0x74 = 45;
    field_0x75 = 136;
    field_0x76 = 170;
    field_0x78 = 109;
    field_0x79 = 60;
    field_0x7a = 205;
    field_0x7c = 120.0f;
    field_0x80 = 100.0f;
    field_0x84 = 0.2f;
    field_0x8a = 0;
    field_0x88 = 0;
    field_0x80 = 0.0f;
    moon_col.r = 0;
    moon_col.g = 0;
    moon_col.b = 0;
    moon_col.a = 255;
    moon_scale = 8000.0f;
    field_0xb0.x = 16.5f;
    field_0xb0.y = -2.0f;
    field_0xb0.z = 30.0f;
    field_0xbc = 160.0f;
    field_0xc0 = 0.06f;
    field_0xc4 = 200;
    field_0xc8 = 3.0f;
    field_0xcc = 60.0f;
    field_0xd0 = 69;
    field_0xd1 = 60;
    field_0xd2 = 39;
    field_0xd4 = 124;
    field_0xd5 = 124;
    field_0xd6 = 104;
    field_0xd3 = 255;
    field_0xd8 = 255;
    field_0xd9 = 0;
    field_0xda = 0;
    field_0xdc = 255;
    field_0xdd = 255;
    field_0xde = 0;
    field_0xe0 = 500;
    field_0xe4 = 0.4f;
    sun_col.r = 255;
    sun_col.g = 255;
    sun_col.b = 241;
    sun_col2.r = 255;
    sun_col2.g = 145;
    sun_col2.b = 73;
    sun_adjust_ON = 0;
    smell_adjust_ON = 0;
    smell_col.r = 255;
    smell_col.g = 255;
    smell_col.b = 115;
    smell_col2.r = 80;
    smell_col2.g = 50;
    smell_col2.b = 0;
    smell_alpha = 1.0f;
    field_0xf0 = 190;
    field_0xf1 = 120;
    field_0xf2 = 120;
    field_0x108 = 60;
    field_0x109 = 0;
    field_0x10a = 0;
    field_0xf4 = 60;
    field_0xf5 = 150;
    field_0xf6 = 230;
    field_0x10c = 50;
    field_0x10d = 65;
    field_0x10e = 80;
    field_0xf8 = 80;
    field_0xf9 = 80;
    field_0xfa = 20;
    field_0x110 = 30;
    field_0x111 = 30;
    field_0x112 = 10;
    field_0xfc = 33;
    field_0xfd = 255;
    field_0xfe = 125;
    field_0x114 = 33;
    field_0x115 = 255;
    field_0x116 = 125;
    field_0x120 = 0.1f;
    field_0x124 = 1.0f;
    constellation_maker_ON = 0;
    constellation_maker_pos[0].x = 5900.0f;
    constellation_maker_pos[0].y = 14000.0f;
    constellation_maker_pos[0].z = -16000.0f;
    constellation_maker_pos[1].x = 7500.0f;
    constellation_maker_pos[1].y = 14000.0f;
    constellation_maker_pos[1].z = -14700.0f;
    constellation_maker_pos[2].x = 8700.0f;
    constellation_maker_pos[2].y = 13920.0f;
    constellation_maker_pos[2].z = -14700.0f;
    constellation_maker_pos[3].x = 10200.0f;
    constellation_maker_pos[3].y = 14320.0f;
    constellation_maker_pos[3].z = -15000.0f;
    constellation_maker_pos[4].x = 12300.0f;
    constellation_maker_pos[4].y = 15400.0f;
    constellation_maker_pos[4].z = -18400.0f;
    constellation_maker_pos[5].x = 13000.0f;
    constellation_maker_pos[5].y = 13500.0f;
    constellation_maker_pos[5].z = -15000.0f;
    constellation_maker_pos[6].x = 13000.0f;
    constellation_maker_pos[6].y = 15400.0f;
    constellation_maker_pos[6].z = -14500.0f;
    constellation_maker_pos[7].x = 13000.0f;
    constellation_maker_pos[7].y = 15400.0f;
    constellation_maker_pos[7].z = -14500.0f;
    constellation_maker_pos[8].x = 13000.0f;
    constellation_maker_pos[8].y = 15400.0f;
    constellation_maker_pos[8].z = -14500.0f;
    constellation_maker_pos[9].x = 13000.0f;
    constellation_maker_pos[9].y = 15400.0f;
    constellation_maker_pos[9].z = -14500.0f;
    lightning_scale_x_min = 14.0f;
    lightning_scale_x_max = 20.0f;
    lightning_scale_y_min = 14.0f;
    lightning_scale_y_max = 20.0f;
    lightning_tilt_angle = 2000;
    field_0x1b6 = 3;
    lightning_debug_mode = 0;
    collect_light_reflect_pos.x = 60000.0f;
    collect_light_reflect_pos.y = -5000.0f;
    collect_light_reflect_pos.z = 0.0f;
    moya_alpha = 0.12f;
    field_0x1c5 = 0;
    thunder_col.r = 75;
    thunder_col.g = 130;
    thunder_col.b = 150;
    thunder_height = 2000.0f;
    thunder_blacken_rate = 0.75f;
    water_in_col_ratio_R = 0.0f;
    water_in_col_ratio_G = 0.4f;
    water_in_col_ratio_B = 0.5f;
    field_0x1e8 = -10.0f;
    field_0x1ec = 40.0f;
    field_0x1f0 = 50.0f;
    field_0x1f4 = 200.0f;
    field_0x1f8 = 0.0f;
    field_0x1e4 = 80;
    field_0x1e5 = 80;
    field_0x1e6 = 80;
    field_0x1fd = 2;
    field_0x1fe = 3;
    field_0x1ff = 0;
    field_0x200 = 0;
    mist_tag_fog_near = -2000.0f;
    mist_tag_fog_far = 200.0f;
    wipe_test_ON = 0xff;
    field_0x210 = 0.0f;
    fade_test_speed = 0;
    field_0x215 = 1;
    smell_railtag_space = 0.0f;
    field_0x22a = 0;
    field_0x22c = 0;
    field_0x22d = 0;
    light_adjust_ON = 0;
    adjust_light_ambcol.r = 24;
    adjust_light_ambcol.g = 24;
    adjust_light_ambcol.b = 24;
    adjust_light_dif0_col_R = 126;
    adjust_light_dif0_col_G = 110;
    adjust_light_dif0_col_B = 89;
    adjust_light_dif1_col.r = 24;
    adjust_light_dif1_col.g = 41;
    adjust_light_dif1_col.b = 50;
    adjust_light_main_pos.x = 500.0f;
    adjust_light_main_pos.y = 500.0f;
    adjust_light_main_pos.z = 500.0f;
    mist_twilight_c1_col.r = 182;
    mist_twilight_c1_col.g = 88;
    mist_twilight_c1_col.b = 50;
    mist_twilight_c1_col.a = 150;
    mist_twilight_c2_col.r = 117;
    mist_twilight_c2_col.g = 69;
    mist_twilight_c2_col.b = 50;
    mist_twilight_c2_col.a = 255;
    field_0x264.r = 124;
    field_0x264.g = 60;
    field_0x264.b = 50;
    field_0x267 = 255;
    field_0x268 = 150;
    adjust_custom_R = 70;
    adjust_custom_G = 70;
    adjust_custom_B = 70;
    adjust_light_mode = 1;
    adjust_height = 0.0f;
    field_0x278 = 120.0f;
    shadow_adjust_ON = 0;
    shadow_normal_alpha = 0.4f;
    shadow_max_alpha = 0.65f;
    field_0x29c = 0;
    field_0x27c = 70.0f;
    field_0x280 = 0.05f;
    field_0x284 = 1.5f;
    field_0x288 = 0.00025f;
    field_0x28c = 0.001f;
    unk_color_1.r = 255;
    unk_color_1.g = 255;
    unk_color_1.b = 255;
    unk_alpha_1 = 255;
    unk_color_2.r = 0;
    unk_color_2.g = 0;
    unk_color_2.b = 0;
    unk_alpha_2 = 255;
    unk_color_3.r = 60;
    unk_color_3.g = 30;
    unk_color_3.b = 0;
    unk_alpha_3 = 255;
    field_0x29d = 1;
    camera_light_col.r = 25;
    camera_light_col.g = 90;
    camera_light_col.b = 183;
    camera_light_alpha = 255;
    camera_light_y_shift = 1500.0f;
    camera_light_power = 1.25f;
    camera_light_cutoff = 90.0f;
    camera_light_sp = 2;
    camera_light_da = 3;
    demo_adjust_SW = 0;
    demo_focus_pos = 30;
    demo_focus_offset_x = 0.0025f;
    demo_focus_offset_y = 0.0025f;
    grass_ambcol.r = 0;
    grass_ambcol.g = 0;
    grass_ambcol.b = 0;
    grass_adjust_ON = 0;
    grass_shine_value = 0.0f;
    stars_max_number = 0xffff;
    display_save_location = 0;
    unk_light_influence_ratio = 100;
    door_light_influence_ratio = 255;
    fish_pond_colreg_adjust_ON = 0;
    fish_pond_colreg_c0.r = 0;
    fish_pond_colreg_c0.g = 0;
    fish_pond_colreg_c0.b = 0;
    water_mud_adjust_ON = 0;
    field_0x2ea = 0;
    field_0x2ec = 0;
    fish_pond_tree_adjust_ON = 0;
    fish_pond_tree_ambcol.r = 0;
    fish_pond_tree_ambcol.g = 0;
    fish_pond_tree_ambcol.b = 0;
    fish_pond_tree_dif0_col.r = 0;
    fish_pond_tree_dif0_col.g = 0;
    fish_pond_tree_dif0_col.b = 0;
    fish_pond_tree_dif1_col.r = 0;
    fish_pond_tree_dif1_col.g = 0;
    fish_pond_tree_dif1_col.b = 0;
    rainbow_adjust_ON = 0;
    rainbow_separation_dist = 4500;
    rainbow_max_alpha = 175;
    field_0x2ff = 0;
    grass_light_influence_ratio = 100;
    grass_light_debug = 0;
    field_0x302 = 2000;
    field_0x304 = 0.6f;
    field_0x308 = 0;
    moya_col.r = 255;
    moya_col.g = 255;
    moya_col.b = 255;
    field_0x30d = 0;
    twilight_sense_saturation_mode = 0;
    twilight_sense_pat = 0;
    twilight_sense_pat_tv_display_ON = 0;
    camera_light_adjust_ON = 0;
    possessed_zelda_light_col.r = 30;
    possessed_zelda_light_col.g = 55;
    possessed_zelda_light_col.b = 110;
    possessed_zelda_light_alpha = 255;
    possessed_zelda_light_height = -800.0f;
    possessed_zelda_light_power = 250.0f;
    beast_ganon_light_col.r = 60;
    beast_ganon_light_col.g = 95;
    beast_ganon_light_col.b = 100;
    beast_ganon_light_alpha = 255;
    beast_ganon_light_height = -800.0f;
    beast_ganon_light_power = 150.0f;
    water_in_light_col.r = 138;
    water_in_light_col.g = 192;
    water_in_light_col.b = 188;
}

# pragma mark AI
#include <dolphin/ai.h>
u32 AIGetDSPSampleRate(void) {
  puts("AIGetDSPSampleRate is a stub");
  return 48000; // Default sample rate?
}

void AIInit(u8* stack) {
  puts("AIInit is a stub");
  // This function initializes the AI system, but we don't have any specific implementation here.
  // In a real scenario, it would set up the audio interface and prepare it for use.
}

void AIInitDMA(u32 start_addr, u32 length) {
  puts("AIInitDMA is a stub");
}

AIDCallback AIRegisterDMACallback(AIDCallback callback) {
  puts("AIRegisterDMACallback is a stub");
  return callback;
}

void AISetDSPSampleRate(u32 rate) {
  // Should this link with the getsamplerate? this is very TODO
  puts("AISetDSPSampleRate is a stub");
}

void AIStartDMA(void) {
  puts("AIStartDMA is a stub");
}

void AIStopDMA(void) {
  puts("AIStopDMA is a stub");
}

# pragma mark AR
#include <dolphin/ar.h>
// Auxilary RAM doesn't exist on PC platforms, do we need to call malloc/free for these instead?
// For now, we will just stub these functions.
u32 ARAlloc(u32 length) {
  puts("ARAlloc is a stub");
  return 0;
}

u32 ARGetSize(void) {
    return 0x10000; // 64KB, this is the size of the AR memory region
}

u32 ARInit(u32* stack_index_addr, u32 num_entries) {
  puts("ARInit is a stub");
  return 0;
}

# pragma mark ARQ
void ARQPostRequest(ARQRequest* request, u32 owner, u32 type, u32 priority, u32 source, u32 dest, u32 length, ARQCallback callback) {
  puts("ARQPostRequest is a stub");
}

void ARQInit() {
  puts("ARQInit is a stub");
}

# pragma mark DVD
#include <dolphin/dvd.h>
s32 DVDCancel(volatile DVDCommandBlock* block) {
  puts("DVDCancel is a stub");
  return 0;
}
s32 DVDCancel(DVDCommandBlock* block) {
  puts("DVDCancel is a stub");
  return 0;
}
BOOL DVDChangeDir(const char* dirName) {
  puts("DVDChangeDir is a stub");
  return TRUE;
}
BOOL DVDCheckDisk(void) {
  puts("DVDCheckDisk is a stub");
  return TRUE;
}
BOOL DVDClose(DVDFileInfo* fileInfo) {
  puts("DVDClose is a stub");
  return TRUE;
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
  return TRUE;
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
  return TRUE;
}
int DVDOpenDir(const char* dirName, DVDDir* dir) {
  puts("DVDOpenDir is a stub");
  return 0;
}
BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                      DVDCallback callback, s32 prio) {
  puts("DVDReadAsyncPrio is a stub");
  return TRUE;
}
int DVDReadDir(DVDDir* dir, DVDDirEntry* dirent) {
  puts("DVDReadDir is a stub");
  return 0;
}
s32 DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, s32 prio) {
  puts("DVDReadPrio is a stub");
  return 0;
}

void DVDReadAbsAsyncForBS(void *a, struct bb2struct *b, int c, int d,
                          void (*e)()) {
  puts("DVDReadAbsAsyncForBS is a stub");
}

void DVDReadDiskID(void *a, DVDDiskID *b, void (*c)()) {
  puts("DVDReadDiskID is a stub");
}

void DVDReset() {
  puts("DVDReset is a stub");
}

# pragma mark GD
#include <dolphin/gd/GDBase.h>
#include <dolphin/gd/GDGeometry.h>
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

# pragma mark GX
#include <dolphin/gx.h>

// Moved-in GX helpers and helpers for metrics/project
void __GXSetSUTexSize() { puts("__GXSetSUTexSize is a stub"); }
void __GXSetVAT() { puts("__GXSetVAT is a stub"); }
void __GXSetVCD() { puts("__GXSetVCD is a stub"); }
void __GXUpdateBPMask() { puts("__GXUpdateBPMask is a stub"); }

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
  return cb;
}
void GXDrawCylinder(u8 numEdges) {
  puts("GXDrawCylinder is a stub");
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
}
void GXResetWriteGatherPipe(void) {
  // puts("GXResetWriteGatherPipe is a stub");
}

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
void GXAbortFrame(void) {
  puts("GXAbortFrame is a stub");
}
void GXEnableTexOffsets(GXTexCoordID coord, u8 line_enable, u8 point_enable) {
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
void GXGetViewportv(f32* vp) {
  puts("GXGetViewportv is a stub");
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
  return 0.0f;
}
void GXInitTexCacheRegion(GXTexRegion* region, u8 is_32b_mipmap, u32 tmem_even, GXTexCacheSize size_even, u32 tmem_odd, GXTexCacheSize size_odd) {
  puts("GXInitTexCacheRegion is a stub");
}
// XXX, this should be some struct?
GXRenderModeObj GXNtsc480IntDf;
GXRenderModeObj GXNtsc480Int;
void GXPeekZ(u16 x, u16 y, u32* z) {
  puts("GXPeekZ is a stub");
  *z = 0;
}
void GXReadXfRasMetric(u32* xf_wait_in, u32* xf_wait_out, u32* ras_busy, u32* clocks) {
  puts("GXReadXfRasMetric is a stub");
  *xf_wait_in = 0;
  *xf_wait_out = 0;
  *ras_busy = 0;
  *clocks = 0;
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
void GXSetFogRangeAdj(GXBool enable, u16 center, const GXFogAdjTable *table) {
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

# pragma mark KPAD
// is this actually used?
extern "C" void KPADDisableDPD(s32) {
  puts("KPADDisableDPD is a stub");

}
extern "C" void KPADEnableDPD(s32) {
  puts("KPADEnableDPD is a stub");
}

// LC (consolidated above)
void LCDisable(void) {
  puts("LCDisable is a stub");
}
void LCQueueWait(__REGISTER u32 len) {
  puts("LCQueueWait is a stub");
}
u32 LCStoreData(void* destAddr, void* srcAddr, u32 nBytes) {
  puts("LCStoreData is a stub");
  return 0;
}

# pragma mark PPC Arch
// MSR stuff?
void PPCHalt() { puts("PPCHalt is a stub"); }

extern "C" void PPCSync(void) {
  // puts("PPCSync is a stub");
}

u32 PPCMfhid2() {
  puts("PPCMfhid2 is a stub");
  return 0;
}

u32 PPCMfmsr() {
  puts("PPCMfmsr is a stub");
  return 0;
}

void PPCMtmsr(u32 newMSR) {
  puts("PPCMtmsr is a stub");
}

# pragma mark WPAD
// uh.. this is revolution include not dolphin?
typedef void (*WPADExtensionCallback)(s32 chan, s32 devType);
extern "C" WPADExtensionCallback WPADSetExtensionCallback(s32 chan, WPADExtensionCallback cb) {
  puts("WPADSetExtensionCallback is a stub");
  return cb;
}

# pragma mark GF
#include <dolphin/gf/GFPixel.h>
void GFSetZMode(u8 compare_enable, GXCompare func, u8 update_enable) {
  puts("GFSetZMode is a stub");
}
void GFSetGenMode2(u8 nTexGens, u8 nChans, u8 nTevs, u8 nInds, GXCullMode cm) {
  puts("GFSetGenMode2 is a stub");
}
void GFSetTevColorS10(GXTevRegID reg, GXColorS10 color) {
  puts("GFSetTevColorS10 is a stub");
}
void GFSetBlendModeEtc(GXBlendMode type, GXBlendFactor src_factor,
                       GXBlendFactor dst_factor, GXLogicOp logic_op,
                       u8 color_update_enable, u8 alpha_update_enable,
                       u8 dither_enable) {
  puts("GFSetBlendModeEtc is a stub");
}
void GFSetChanAmbColor(GXChannelID chan, GXColor color) {
  puts("GFSetChanAmbColor is a stub");
}
void GFSetFog(GXFogType type, f32 startz, f32 endz, f32 nearz, f32 farz, GXColor color) {
  puts("GFSetFog is a stub");
}

# pragma mark DEBUGPAD
#include <d/d_debug_pad.h>
dDebugPad_c::dDebugPad_c() {
  puts("constructing debug pad, stubbed?");
}

# pragma mark f_ap
#include <f_ap/f_ap_game.h>
u8 fapGm_HIO_c::mCaptureScreenDivH = 1;

# pragma mark dMsgObject
#include <d/d_msg_object.h>
void dMsgObject_c::setSelectWordFlag(u8 flag) {
  puts("dMsgObject_c::setSelectWordFlag is a stub");
}
void dMsgObject_c::setWord(const char* i_word) {
  puts("dMsgObject_c::setWord is a stub");
}
void dMsgObject_c::setSelectWord(int i_no, const char* i_word) {
  puts("dMsgObject_c::setSelectWord is a stub");
}
<<<<<<< HEAD
=======

#pragma mark HIO
#include <revolution/hio2.h>
#include <dolphin/hio.h>
BOOL HIO2Close(s32 handle) {
  puts("HIO2Close is a stub");
  return FALSE;
}

BOOL HIO2EnumDevices(HIO2EnumCallback callback) {
  puts("HIO2EnumDevices is a stub");
  return FALSE;
}

BOOL HIO2Init(void) {
  puts("HIO2Init is a stub");
  return FALSE;
}

s32 HIO2Open(HIO2DeviceType type, HIO2UnkCallback exiCb, HIO2DisconnectCallback disconnectCb) {
  puts("HIO2Open is a stub");
  return 0;
}

BOOL HIO2Read(s32 handle, u32 addr, void* buffer, s32 size) {
  puts("HIO2Read is a stub");
  return FALSE;
}

BOOL HIO2Write(s32 handle, u32 addr, void* buffer, s32 size) {
  puts("HIO2Write is a stub");
  return FALSE;
}

BOOL HIORead(u32 addr, void* buffer, s32 size) {
  puts("HIORead is a stub");
  return FALSE;
}

BOOL HIOWrite(u32 addr, void* buffer, s32 size) {
  puts("HIOWrite is a stub");
  return FALSE;
}

#pragma mark JHICommBuf
#include <JSystem/JHostIO/JHIComm.h>
void JHICommBufHeader::init() {
  puts("JHICommBufHeader::init is a stub");
}

int JHICommBufHeader::load() {
  puts("JHICommBufHeader::load is a stub");
}

int JHICommBufReader::read(void*, int) {
  puts("JHICommBufReader::read is a stub");
  return 0;
}
void JHICommBufReader::readEnd() {
  puts("JHICommBufReader::readEnd is a stub");
}

int JHICommBufReader::readBegin() {
  puts("JHICommBufReader::readBegin is a stub");
}

int JHICommBufWriter::writeBegin() {
  puts("JHICommBufWriter::writeBegin is a stub");
  return 0;
}

int JHICommBufWriter::write(void*, int) {
  puts("JHICommBufWriter::write is a stub");
  return 0;
}

void JHICommBufWriter::writeEnd() {
  puts("JHICommBufWriter::writeEnd is a stub");
}

u32 JHICommBufReader::Header::getReadableSize() const {
  puts("JHICommBufReader::Header::getReadableSize is a stub");
  return 0;
}
>>>>>>> wip/linkfix2
