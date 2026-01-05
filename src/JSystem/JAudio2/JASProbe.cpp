#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASProbe.h"
#include "JSystem/JAudio2/JASCriticalSection.h"

void JASProbe::start(char const* name) {
	JASCriticalSection cs;
	mName = name;
	mStartTime = OSGetTime();
}


void JASProbe::stop() {
	JASCriticalSection cs;
	u32 r29 = OSGetTime();
	_08            = r29 - mStartTime;
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
}

JASProbe* JASProbe::sProbeTable[16];

void JASProbe::start(s32 index, char const* name) {
	JASProbe* probe;
    if (index < 16) {
		probe = sProbeTable[index];
        if (probe != NULL) {
            probe->start(name);
        }
    }
}

void JASProbe::stop(s32 index) {
	JASProbe* probe;
    if (index < 16) {
		probe = sProbeTable[index];
        if (probe != NULL) {
            probe->stop();
        }
    }
}
