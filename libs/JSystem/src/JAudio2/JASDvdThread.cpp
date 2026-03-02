#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASDvdThread.h"
#include "JSystem/JAudio2/JASTaskThread.h"
#include "JSystem/JKernel/JKRSolidHeap.h"

JASTaskThread* JASDvd::sThread;

JASTaskThread* JASDvd::getThreadPointer() {
    return JASDvd::sThread;
}

bool JASDvd::createThread(s32 priority, int msgCount, u32 stackSize) {
    if (sThread != NULL) {
        JUT_WARN_DEVICE(32, 2, "%s", "JASDvd::createThread が２回呼び出されました");
        return false;
    }

    sThread = new (JASDram, 0) JASTaskThread(priority, msgCount, stackSize);
    JUT_ASSERT(36, sThread);

    JKRHeap* pCurrentHeap = JKRGetSystemHeap();
    JUT_ASSERT(38, pCurrentHeap);

    sThread->setCurrentHeap(pCurrentHeap);
    sThread->resume();
	return true;
}
