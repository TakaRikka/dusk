/**
 * GCN_mem_alloc.c
 * Description:
 */

#include "dolphin/os.h"

inline static void InitDefaultHeap(void) {
	void* arenaLo;
	void* arenaHi;

	OSReport("GCN_Mem_Alloc.c : InitDefaultHeap. No Heap Available\n");
	OSReport("Metrowerks CW runtime library initializing default heap\n");

	arenaLo = OSGetArenaLo();
	arenaHi = OSGetArenaHi();

	arenaLo = OSInitAlloc(arenaLo, arenaHi, 1);
	OSSetArenaLo(arenaLo);

	arenaLo = (void*)OSRoundUp32B(arenaLo);
	arenaHi = (void*)OSRoundDown32B(arenaHi);

	OSSetCurrentHeap(OSCreateHeap(arenaLo, arenaHi));
	OSSetArenaLo(arenaLo = arenaHi);
}

void __sys_free(void* p) {
    if (__OSCurrHeap == -1) {
        InitDefaultHeap();
    }

    OSFreeToHeap(__OSCurrHeap, p);
}
