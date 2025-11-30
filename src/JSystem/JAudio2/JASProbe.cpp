#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASProbe.h"
#include "dolphin/os.h"

void JASProbe::start(char const* name) {
	volatile BOOL interrupts = OSDisableInterrupts();
	mName          = name;
	mStartTime     = OSGetTime();
	OSRestoreInterrupts(interrupts);
}


void JASProbe::stop() {
	volatile int interrupts = OSDisableInterrupts();
	_08            = (u32)OSGetTime() - mStartTime;
	_08 /= OS_TIMER_CLOCK / 59.94f;
	if (_10 < _08 && 100 < _1A8) {
		_10 = _08;
	}
	_0C             = _0C * 0.96f + _08 * 0.04f;
	u32 resultIndex = _1A8 % 100;
	_14 -= _18[resultIndex];
	_18[resultIndex] = _08;
	_14 += _08;
	_1A8++;
	OSRestoreInterrupts(interrupts);
}

JASProbe* JASProbe::sProbeTable[16];

void JASProbe::start(s32 index, char const* name) {
    if (index < 16) {
        if (sProbeTable[index] != NULL) {
            sProbeTable[index]->start(name);
        }
    }
}

void JASProbe::stop(s32 index) {
    if (index < 16) {
        if (sProbeTable[index] != NULL) {
            sProbeTable[index]->stop();
        }
    }
}
