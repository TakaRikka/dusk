#include <dolphin/dolphin.h>
#include <dolphin/gx.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <memory>
#include <dusk/logging.h>


#ifndef _WIN32
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#if __APPLE__
#include <mach/mach_time.h>
#endif
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#undef IN
#undef OUT
#endif

#if __APPLE__
static u64 MachToDolphinNum;
static u64 MachToDolphinDenom;
#elif _WIN32
static LARGE_INTEGER PerfFrequency;
static bool PerfInitialized = false;
#endif


// ==========================================================================
// General OS
// ==========================================================================

u32 OSGetConsoleType() {
    return OS_CONSOLE_RETAIL1;
}

u32 OSGetSoundMode() {
    return 2;
}

// ==========================================================================
// Message Queue (thread-safe implementation)
// ==========================================================================

// Malloc-based allocator to bypass JKRHeap operator new/delete
template<typename T>
struct MallocAllocator {
    using value_type = T;
    MallocAllocator() = default;
    template<typename U> MallocAllocator(const MallocAllocator<U>&) noexcept {}
    T* allocate(std::size_t n) {
        void* p = std::malloc(n * sizeof(T));
        if (!p) throw std::bad_alloc();
        return static_cast<T*>(p);
    }
    void deallocate(T* p, std::size_t) noexcept { std::free(p); }
    template<typename U> bool operator==(const MallocAllocator<U>&) const noexcept { return true; }
    template<typename U> bool operator!=(const MallocAllocator<U>&) const noexcept { return false; }
};

template<typename T>
struct MallocDeleter {
    void operator()(T* p) const {
        p->~T();
        std::free(p);
    }
};

template<typename T, typename... Args>
std::unique_ptr<T, MallocDeleter<T>> make_malloc_unique(Args&&... args) {
    void* mem = std::malloc(sizeof(T));
    if (!mem) throw std::bad_alloc();
    T* obj = new (mem) T(std::forward<Args>(args)...);
    return std::unique_ptr<T, MallocDeleter<T>>(obj);
}

template<typename K, typename V>
using MallocMap = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>,
    MallocAllocator<std::pair<const K, V>>>;

// Side-table for native synchronization per OSMessageQueue
struct PCMessageQueueData {
    std::mutex mtx;
    std::condition_variable cvSend;     // Notified when space becomes available
    std::condition_variable cvReceive;  // Notified when a message arrives
};

// Lazy-initialized to avoid DLL static init crashes
static std::mutex& GetMsgQueueMapMutex() {
    static std::mutex mtx;
    return mtx;
}
static MallocMap<OSMessageQueue*, std::unique_ptr<PCMessageQueueData, MallocDeleter<PCMessageQueueData>>>& GetMsgQueueMap() {
    static MallocMap<OSMessageQueue*, std::unique_ptr<PCMessageQueueData, MallocDeleter<PCMessageQueueData>>> map;
    return map;
}

static PCMessageQueueData& GetMsgQueueData(OSMessageQueue* mq) {
    std::lock_guard<std::mutex> lock(GetMsgQueueMapMutex());
    auto& map = GetMsgQueueMap();
    auto it = map.find(mq);
    if (it == map.end()) {
        auto result = map.emplace(mq, make_malloc_unique<PCMessageQueueData>());
        return *result.first->second;
    }
    return *it->second;
}

void OSInitMessageQueue(OSMessageQueue* mq, void* msgArray, s32 msgCount) {
    if (!mq) return;
    mq->queueSend.head = mq->queueSend.tail = nullptr;
    mq->queueReceive.head = mq->queueReceive.tail = nullptr;
    mq->msgArray   = msgArray;
    mq->msgCount   = msgCount;
    mq->firstIndex = 0;
    mq->usedCount  = 0;
    GetMsgQueueData(mq);  // Ensure side-table entry exists
}

int OSSendMessage(OSMessageQueue* mq, void* msg, s32 flags) {
    if (!mq) return 0;

    PCMessageQueueData& data = GetMsgQueueData(mq);
    std::unique_lock<std::mutex> lock(data.mtx);

    if (mq->usedCount >= mq->msgCount) {
        if (flags == OS_MESSAGE_NOBLOCK) return 0;
        // BLOCK: wait until space is available
        data.cvSend.wait(lock, [mq]() { return mq->usedCount < mq->msgCount; });
    }

    s32 idx = (mq->firstIndex + mq->usedCount) % mq->msgCount;
    ((OSMessage*)mq->msgArray)[idx] = msg;
    mq->usedCount++;

    data.cvReceive.notify_one();
    return 1;
}

int OSReceiveMessage(OSMessageQueue* mq, void* msg, s32 flags) {
    if (!mq) return 0;

    PCMessageQueueData& data = GetMsgQueueData(mq);
    std::unique_lock<std::mutex> lock(data.mtx);

    if (mq->usedCount == 0) {
        if (flags == OS_MESSAGE_NOBLOCK) return 0;
        // BLOCK: wait until a message arrives
        data.cvReceive.wait(lock, [mq]() { return mq->usedCount > 0; });
    }

    if (msg) {
        *(OSMessage*)msg = ((OSMessage*)mq->msgArray)[mq->firstIndex];
    }
    mq->firstIndex = (mq->firstIndex + 1) % mq->msgCount;
    mq->usedCount--;

    data.cvSend.notify_one();
    return 1;
}

int OSJamMessage(OSMessageQueue* mq, void* msg, s32 flags) {
    if (!mq) return 0;

    PCMessageQueueData& data = GetMsgQueueData(mq);
    std::unique_lock<std::mutex> lock(data.mtx);

    if (mq->usedCount >= mq->msgCount) {
        if (flags == OS_MESSAGE_NOBLOCK) return 0;
        // BLOCK: wait until space is available
        data.cvSend.wait(lock, [mq]() { return mq->usedCount < mq->msgCount; });
    }

    // Jam inserts at the front of the queue
    mq->firstIndex = (mq->firstIndex - 1 + mq->msgCount) % mq->msgCount;
    ((OSMessage*)mq->msgArray)[mq->firstIndex] = msg;
    mq->usedCount++;

    data.cvReceive.notify_one();
    return 1;
}

// ==========================================================================
// Arena Functions
// ==========================================================================

void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps) {
    return arenaStart;
}

// ==========================================================================
// Remaining OS Stubs
// ==========================================================================

void OSSetSoundMode(u32 mode) {}

void OSCreateAlarm(OSAlarm* alarm) {}

void OSCancelAlarm(OSAlarm* alarm) {}

u16 OSGetFontEncode() { return 0; }

char* OSGetFontTexture(char* string, void** image, s32* x, s32* y, s32* width) { return 0; }
char* OSGetFontWidth(char* string, s32* width) { return 0; }

BOOL OSGetResetButtonState() { return FALSE; }
BOOL OSInitFont(OSFontHeader* fontData) { return FALSE; }
BOOL OSLink(OSModuleInfo* newModule, void* bss) { return TRUE; }

void OSResetSystem(int reset, u32 resetCode, BOOL forceMenu) {
    OSReport("[PC] OSResetSystem called (reset=%d, code=%u)\n", reset, resetCode);
}

void OSSetStringTable(void* stringTable) {}
BOOL OSUnlink(OSModuleInfo* oldModule) { return FALSE; }

void OSSwitchFiberEx(__REGISTER u32 param_0, __REGISTER u32 param_1, __REGISTER u32 param_2,
                     __REGISTER u32 param_3, __REGISTER u32 code, __REGISTER u32 stack) {
    // On PC, call the function directly instead of switching stacks.
    // The PPC version switches to 'stack' and calls code(param_0, param_1).
    // Only caller is mDoPrintf_vprintf_Interrupt: OSSwitchFiberEx(fmt, args, 0, 0, vprintf, sp)
    typedef void (*Func2)(u32, u32);
    ((Func2)(uintptr_t)code)(param_0, param_1);
}

u32 __OSGetDIConfig() { return 0; }
u32 OSGetProgressiveMode(void) { return 0; }
u32 OSGetResetCode(void) { return 0; }
BOOL OSGetResetSwitchState() { return FALSE; }
BOOL OSLinkFixed(OSModuleInfo* newModule, void* bss) { return TRUE; }
void OSProtectRange(u32 chan, void* addr, u32 nBytes, u32 control) {}
void OSSetPeriodicAlarm(OSAlarm* alarm, OSTime start, OSTime period, OSAlarmHandler handler) {}
void OSSetProgressiveMode(u32 on) {}
void OSSetSaveRegion(void* start, void* end) {}
OSErrorHandler OSSetErrorHandler(OSError error, OSErrorHandler handler) { return NULL; }
void OSSetAlarm(OSAlarm* alarm, OSTime tick, OSAlarmHandler handler) {}

#pragma mark SOUND
void SoundChoID(int a, int b) {
    STUB_LOG();
}
void SoundPan(int a, int b, int c) {
    STUB_LOG();
}
void SoundPitch(u16 a, int b) {
    STUB_LOG();
}
void SoundRevID(int a, int b) {
    STUB_LOG();
}

#pragma mark DC

void DCFlushRange(void* addr, u32 nBytes) {
    // Not needed on PC.
}

void DCFlushRangeNoSync(void* addr, u32 nBytes) {
    // Not needed on PC.
}

void DCInvalidateRange(void* addr, u32 nBytes) {
    // Not needed on PC.
}

void DCStoreRange(void* addr, u32 nBytes) {
    // Not needed on PC.
}

void DCStoreRangeNoSync(void* addr, u32 nBytes) {
    // Not needed on PC.
}

#pragma mark EXI

BOOL EXIDeselect(int chan) {
    STUB_LOG();
    return FALSE;
}

BOOL EXIDma(int chan, void* buffer, s32 size, int d, int e) {
    STUB_LOG();
    return FALSE;
}

BOOL EXIImm(int chan, u32* b, int c, int d, int e) {
    STUB_LOG();
    return FALSE;
}

BOOL EXILock(int chan, int b, int c) {
    STUB_LOG();
    return FALSE;
}

BOOL EXISelect(int chan, int b, int c) {
    STUB_LOG();
    return FALSE;
}

BOOL EXISync(int chan) {
    STUB_LOG();
    return FALSE;
}

BOOL EXIUnlock(int chan) {
    STUB_LOG();
    return FALSE;
}

#pragma mark LC

void LCEnable() {
    STUB_LOG();
}

// OS-related functions consolidated under "# pragma mark OS" further up

#pragma mark VI

// VI retrace emulation: on GameCube, the VI chip fires a hardware interrupt at
// every VSync (~60Hz). This triggers pre/post retrace callbacks, which in turn
// send messages to JUTVideo's message queue. waitForTick() blocks on that queue.
// On PC, we simulate this by calling VIWaitForRetrace() once per frame in the
// main loop, which increments the retrace counter and fires the callbacks.
static u32 sRetraceCount = 0;
static VIRetraceCallback sVIPreRetraceCallback = NULL;
static VIRetraceCallback sVIPostRetraceCallback = NULL;

extern "C" {

void VIConfigure(const GXRenderModeObj* rm) {
    STUB_LOG();
}

void VIConfigurePan(u16 xOrg, u16 yOrg, u16 width, u16 height) {
    STUB_LOG();
}

u32 VIGetRetraceCount() {
    return sRetraceCount;
}

u32 VIGetNextField() {
    return 0;
}

void VISetBlack(BOOL black) {
    STUB_LOG();
}

void VISetNextFrameBuffer(void* fb) {
    STUB_LOG();
}

void VIWaitForRetrace() {
    sRetraceCount++;
    if (sVIPreRetraceCallback) {
        sVIPreRetraceCallback(sRetraceCount);
    }
    if (sVIPostRetraceCallback) {
        sVIPostRetraceCallback(sRetraceCount);
    }
}

void* VIGetCurrentFrameBuffer(void) {
    return NULL;
}

u32 VIGetDTVStatus(void) {
    return 0;
}

void* VIGetNextFrameBuffer(void) {
    return NULL;
}

VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback) {
    VIRetraceCallback old = sVIPostRetraceCallback;
    sVIPostRetraceCallback = callback;
    return old;
}

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback cb) {
    VIRetraceCallback old = sVIPreRetraceCallback;
    sVIPreRetraceCallback = cb;
    return old;
}

}  // extern "C"

#pragma mark Z2Audio
class Z2AudioCS {
public:
    void extensionProcess(s32, s32);
};
void Z2AudioCS::extensionProcess(s32, s32) {
    STUB_LOG();
}

#pragma mark JORServer
#include <JSystem/JHostIO/JORServer.h>

int JOREventCallbackListNode::JORAct(u32, const char*) {
    return 0;
}

#pragma mark JSUMemoryOutputStream
#include <JSystem/JSupport/JSUMemoryStream.h>
s32 JSUMemoryOutputStream::getAvailable() const {
    return mLength - mPosition;
}

s32 JSUMemoryOutputStream::getPosition() const {
    return mPosition;
}

#pragma mark JSURandomOutputStream
#include <JSystem/JSupport/JSURandomOutputStream.h>
s32 JSUMemoryOutputStream::seek(s32 offset, JSUStreamSeekFrom origin) {
    // XXX I think this is correct? could be broken.
    return this->seekPos(offset, origin);
}

#pragma mark JKRHeap
#include <JSystem/JKernel/JKRHeap.h>
JKRHeap* JKRHeap::sRootHeap2;  // XXX this is defined for WII/SHIELD, should we just define it for
                               // dusk builds?

#pragma mark mDoExt_onCupOnAupPacket
#include <m_Do/m_Do_ext.h>
mDoExt_offCupOnAupPacket::~mDoExt_offCupOnAupPacket() {
    STUB_LOG();
}
#pragma mark mDoExt_onCupOffAupPacket
mDoExt_onCupOffAupPacket::~mDoExt_onCupOffAupPacket() {
    STUB_LOG();
}

#pragma mark dKankyo_vrboxHIO_c
#include <d/d_kankyo.h>
void dKankyo_vrboxHIO_c::dKankyo_vrboxHIOInfoUpDateF() {
    STUB_LOG();
}

void dKankyo_lightHIO_c::dKankyo_lightHIOInfoUpDateF() {
    STUB_LOG();
}

#pragma mark dKankyo_HIO_c
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

#pragma mark AI
#include <dolphin/ai.h>
u32 AIGetDSPSampleRate(void) {
    STUB_LOG();
    return 48000;  // Default sample rate?
}

void AIInit(u8* stack) {
    STUB_LOG();
    // This function initializes the AI system, but we don't have any specific implementation here.
    // In a real scenario, it would set up the audio interface and prepare it for use.
}

void AIInitDMA(u32 start_addr, u32 length) {
    STUB_LOG();
}

AIDCallback AIRegisterDMACallback(AIDCallback callback) {
    STUB_LOG();
    return callback;
}

void AISetDSPSampleRate(u32 rate) {
    // Should this link with the getsamplerate? this is very TODO
    STUB_LOG();
}

void AIStartDMA(void) {
    STUB_LOG();
}

void AIStopDMA(void) {
    STUB_LOG();
}

#pragma mark GX
#include <dolphin/gx.h>

void GXSetGPMetric(GXPerf0 perf0, GXPerf1 perf1) {
    STUB_LOG();
}
void GXReadGPMetric(u32* cnt0, u32* cnt1) {
    STUB_LOG();
}
void GXClearGPMetric(void) {
    STUB_LOG();
}
void GXReadMemMetric(u32* cp_req, u32* tc_req, u32* cpu_rd_req, u32* cpu_wr_req, u32* dsp_req,
                     u32* io_req, u32* vi_req, u32* pe_req, u32* rf_req, u32* fi_req) {
    STUB_LOG();
}
void GXClearMemMetric(void) {
    STUB_LOG();
}
void GXClearVCacheMetric(void) {
    STUB_LOG();
}
void GXReadPixMetric(u32* top_pixels_in, u32* top_pixels_out, u32* bot_pixels_in,
                     u32* bot_pixels_out, u32* clr_pixels_in, u32* copy_clks) {
    STUB_LOG();
}
void GXClearPixMetric(void) {
    STUB_LOG();
}
void GXSetVCacheMetric(GXVCachePerf attr) {
    STUB_LOG();
}
void GXReadVCacheMetric(u32* check, u32* miss, u32* stall) {
    STUB_LOG();
}
void GXSetDrawSync(u16 token) {
    STUB_LOG();
}
GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback cb) {
    STUB_LOG();
    return cb;
}
void GXWaitDrawDone(void) {
    STUB_LOG();
}
void GXResetWriteGatherPipe(void) {
    STUB_LOG();
}

void GXAbortFrame(void) {
    STUB_LOG();
}
// GXEnableTexOffsets: now provided by Aurora's GXGeometry.cpp (fifo branch)
OSThread* GXGetCurrentGXThread(void) {
    STUB_LOG();
    return NULL;
}
void* GXGetFifoBase(const GXFifoObj* fifo) {
    STUB_LOG();
    return NULL;
}
u32 GXGetFifoSize(const GXFifoObj* fifo) {
    STUB_LOG();
    return 0;
}
u16 GXGetNumXfbLines(u16 efbHeight, f32 yScale) {
    STUB_LOG();
    return 0;
}

f32 GXGetYScaleFactor(u16 efbHeight, u16 xfbHeight) {
    STUB_LOG();
    return 0.0f;
}

void GXInitTexCacheRegion(GXTexRegion* region, GXBool is_32b_mipmap, u32 tmem_even,
                          GXTexCacheSize size_even, u32 tmem_odd, GXTexCacheSize size_odd) {
    STUB_LOG();
} 
// XXX, this should be some struct?
// GXRenderModeObj GXNtsc480IntDf;
//GXRenderModeObj GXNtsc480Int;
void GXPeekZ(u16 x, u16 y, u32* z) {
    STUB_LOG();
    *z = 0;
}
void GXReadXfRasMetric(u32* xf_wait_in, u32* xf_wait_out, u32* ras_busy, u32* clocks) {
    STUB_LOG();
    *xf_wait_in = 0;
    *xf_wait_out = 0;
    *ras_busy = 0;
    *clocks = 0;
}

void GXSetCopyClamp(GXFBClamp clamp) {
    STUB_LOG();
}
OSThread* GXSetCurrentGXThread(void) {
    STUB_LOG();
    return NULL;
}

void GXSetMisc(GXMiscToken token, u32 val) {
    STUB_LOG();
}

#pragma mark KPAD
// is this actually used?
extern "C" void KPADDisableDPD(s32) {
    STUB_LOG();
}
extern "C" void KPADEnableDPD(s32) {
    STUB_LOG();
}

// LC (consolidated above)
void LCDisable(void) {
    STUB_LOG();
}
void LCQueueWait(__REGISTER u32 len) {
    STUB_LOG();
}
u32 LCStoreData(void* destAddr, void* srcAddr, u32 nBytes) {
    STUB_LOG();
    return 0;
}

#pragma mark PPC Arch
// MSR stuff?
void PPCHalt() {
    STUB_LOG();
}

extern "C" void PPCSync(void) {
    STUB_LOG();
}

u32 PPCMfhid2() {
    STUB_LOG();
    return 0;
}

u32 PPCMfmsr() {
    STUB_LOG();
    return 0;
}

void PPCMtmsr(u32 newMSR) {
    STUB_LOG();
}

#pragma mark WPAD
// uh.. this is revolution include not dolphin?
typedef void (*WPADExtensionCallback)(s32 chan, s32 devType);
extern "C" WPADExtensionCallback WPADSetExtensionCallback(s32 chan, WPADExtensionCallback cb) {
    STUB_LOG();
    return cb;
}

#pragma mark DEBUGPAD
#include <d/d_debug_pad.h>
dDebugPad_c::dDebugPad_c() {
    STUB_LOG();
}

#pragma mark f_ap
#include <f_ap/f_ap_game.h>
u8 fapGm_HIO_c::mCaptureScreenDivH = 1;

#pragma mark dMsgObject
#include <d/d_msg_object.h>
void dMsgObject_c::setWord(const char* i_word) {
    STUB_LOG();
}
void dMsgObject_c::setSelectWord(int i_no, const char* i_word) {
    STUB_LOG();
}

#pragma mark HIO
#include <dolphin/hio.h>
#include <dolphin/hio2.h>
BOOL HIO2Close(s32 handle) {
    STUB_LOG();
    return FALSE;
}

BOOL HIO2EnumDevices(HIO2EnumCallback callback) {
    STUB_LOG();
    return FALSE;
}

BOOL HIO2Init(void) {
    STUB_LOG();
    return FALSE;
}

s32 HIO2Open(HIO2DeviceType type, HIO2UnkCallback exiCb, HIO2DisconnectCallback disconnectCb) {
    STUB_LOG();
    return 0;
}

BOOL HIO2Read(s32 handle, u32 addr, void* buffer, s32 size) {
    STUB_LOG();
    return FALSE;
}

BOOL HIO2Write(s32 handle, u32 addr, void* buffer, s32 size) {
    STUB_LOG();
    return FALSE;
}

BOOL HIORead(u32 addr, void* buffer, s32 size) {
    STUB_LOG();
    return FALSE;
}

BOOL HIOWrite(u32 addr, void* buffer, s32 size) {
    STUB_LOG();
    return FALSE;
}

#pragma mark JHICommBuf
#include <JSystem/JHostIO/JHIComm.h>
void JHICommBufHeader::init() {
    STUB_LOG();
}

int JHICommBufHeader::load() {
    STUB_LOG();
    return 0;
}

int JHICommBufReader::read(void*, int) {
    STUB_LOG();
    return 0;
}
void JHICommBufReader::readEnd() {
    STUB_LOG();
}

int JHICommBufReader::readBegin() {
    STUB_LOG();
    return 0;
}

int JHICommBufWriter::writeBegin() {
    STUB_LOG();
    return 0;
}

int JHICommBufWriter::write(void*, int) {
    STUB_LOG();
    return 0;
}

void JHICommBufWriter::writeEnd() {
    STUB_LOG();
}

u32 JHICommBufReader::Header::getReadableSize() const {
    STUB_LOG();
    return 0;
}

#pragma mark Decomp artifacts
void stripFloat(f32) {}
void stripDouble(f64) {}
int getStripInt() { return 0; }
void F(f32*) {}
