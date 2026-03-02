#include <dolphin/axart.h>
#include <dolphin/os.h>
#include <dolphin/mix.h>

void AXARTPitchEnv(AXART_PITCH_ENV* articulator) {
    if (articulator->cents != articulator->target) {
        articulator->cents += articulator->delta;
        if (articulator->delta > 0) {
            if (articulator->cents > articulator->target) {
                articulator->cents = articulator->target;
            }
        } else if (articulator->delta < 0) {
            if (articulator->cents < articulator->target) {
                articulator->cents = articulator->target;
            }
        }
    }
}

void AXARTVolumeEnv(AXART_VOLUME_ENV* articulator) {
    if (articulator->attenuation != articulator->target) {
        articulator->attenuation += articulator->delta;
        if (articulator->delta > 0) {
            if (articulator->attenuation > articulator->target) {
                articulator->attenuation = articulator->target;
            }
        } else if (articulator->delta < 0) {
            if (articulator->attenuation < articulator->target) {
                articulator->attenuation = articulator->target;
            }
        }
    }
}
