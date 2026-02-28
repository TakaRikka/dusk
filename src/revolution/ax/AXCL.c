#include <revolution/ax.h>
#include <revolution/types.h>

static u16 __AXAuxAVolume;
static u16 __AXAuxBVolume;
static u16 __AXAuxCVolume;

u16 AXGetAuxAReturnVolume(void) {
    return __AXAuxAVolume;
}

u16 AXGetAuxBReturnVolume(void) {
    return __AXAuxBVolume;
}

u16 AXGetAuxCReturnVolume(void) {
    return __AXAuxCVolume;
}

void AXSetAuxAReturnVolume(u16 volume) {
    __AXAuxAVolume = volume;
}

void AXSetAuxBReturnVolume(u16 volume) {
    __AXAuxBVolume = volume;
}

void AXSetAuxCReturnVolume(u16 volume) {
    __AXAuxCVolume = volume;
}
