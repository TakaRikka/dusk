#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JAISoundChild.h"
#include "JSystem/JAudio2/JASTrack.h"

void JAISoundChild::init() {
    mMove.init();
    mParams.init();
}

void JAISoundChild::mixOut(JASTrack* track) {
    mParams = mMove.params_;
    track->assignExtBuffer(0, &mParams);
}

void JAISoundChild::calc() {
    mMove.calc();
}
