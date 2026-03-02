#include <revolution.h>
#include <revolution/os.h>

#define ROUND(n, a) (((u32)(n) + (a)-1) & ~((a)-1))
#define TRUNC(n, a) (((u32)(n)) & ~((a)-1))

static void* s_mem2ArenaHi;
static void* __OSArenaHi;

static void* __OSArenaLo = (void*)-1;
static void* s_mem2ArenaLo = (void*)-1;

void* OSGetMEM1ArenaHi(void) {
    ASSERTMSGLINE(71, (u32)__OSArenaLo != -1, "OSGetArenaHi(): OSInit() must be called in advance.");
    ASSERTMSGLINE(73, (u32)__OSArenaLo <= (u32)__OSArenaHi, "OSGetArenaHi(): invalid arena (hi < lo).");
    return __OSArenaHi;
}

void* OSGetMEM2ArenaHi(void) {
    return s_mem2ArenaHi;
}

void* OSGetArenaHi(void) {
    return OSGetMEM1ArenaHi();
}

void* OSGetMEM1ArenaLo(void) {
    ASSERTMSGLINE(99, (u32)__OSArenaLo != -1, "OSGetArenaLo(): OSInit() must be called in advance.");
    ASSERTMSGLINE(101, (u32)__OSArenaLo <= (u32)__OSArenaHi, "OSGetArenaLo(): invalid arena (hi < lo).");
    return __OSArenaLo;
}

void* OSGetMEM2ArenaLo(void) {
    return s_mem2ArenaLo;
}

void* OSGetArenaLo(void) {
    return OSGetMEM1ArenaLo();
}

void OSSetMEM1ArenaHi(void* newHi) {
    __OSArenaHi = newHi;
}

void OSSetMEM2ArenaHi(void* newHi) {
    s_mem2ArenaHi = newHi;
}

void OSSetArenaHi(void* newHi) {
    OSSetMEM1ArenaHi(newHi);
}

void OSSetMEM1ArenaLo(void* newLo) {
    __OSArenaLo = newLo;
}

void OSSetMEM2ArenaLo(void* newLo) {
    s_mem2ArenaLo = newLo;
}

void OSSetArenaLo(void* newLo) {
    OSSetMEM1ArenaLo(newLo);
}

void* OSAllocFromMEM1ArenaLo(u32 size, u32 align) {
    void* ptr;
    u8* arenaLo;

    ptr = OSGetMEM1ArenaLo();
    arenaLo = ptr = (void*)ROUND(ptr, align);
    arenaLo += size;
    arenaLo = (u8*)ROUND(arenaLo, align);
    OSSetMEM1ArenaLo(arenaLo);
    return ptr;
}

void* OSAllocFromMEM1ArenaHi(u32 size, u32 align) {
    void* ptr;
    u8* arenaHi;

    arenaHi = OSGetMEM1ArenaHi();
    arenaHi = (u8*)TRUNC(arenaHi, align);
    arenaHi -= size;
    arenaHi = ptr = (void*)TRUNC(arenaHi, align);
    OSSetMEM1ArenaHi(arenaHi);
    return ptr;
}
