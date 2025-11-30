#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASLfo.h"
#include "SSystem/SComponent/c_math.h"


JASLfo::JASLfo() {
    field_0x0 = 0;
    field_0x4 = 1;
    field_0x8 = 0.0f;
    mDepth = 0.0f;
    mPitch = 1.0f;
    mDelay = 0;
    field_0x16 = 0;
}


f32 JASLfo::getValue() const {
    if (field_0x16) {
        return 0.0f;
    }

    if (mDepth == 0.0f) {
        return 0.0f;
    }

    return mDepth * JMASinLap(field_0x8);
}


void JASLfo::incCounter(f32 param_0) {
    if (field_0x16) {
        field_0x16--;
        return;
    }

    field_0x8 += param_0 * ((1.0f / 72.0f) * mPitch);
    if (field_0x8 >= 1.0f) {
        field_0x8 -= 1.0f; 
    }
}

JASLfo JASLfo::sFreeRunLfo;

void JASLfo::resetCounter() {
    field_0x16 = mDelay;
    switch (field_0x4) {
    case 0:
        field_0x8 = sFreeRunLfo.field_0x8;
        break;
    case 1:
        field_0x8 = 0.0f;
        break;
    }
}
