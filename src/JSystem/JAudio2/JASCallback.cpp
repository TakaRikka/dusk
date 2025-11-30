#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASCallback.h"
#include "JSystem/JAudio2/JASCriticalSection.h"

bool JASCallbackMgr::regist(JASCallback* callback, void* argument) {
	JASCriticalSection criticalSection;
	for (int i = 0; i < NUM_CALLBACKS; i++) {
		if (callbacks_[i].callback_ == NULL) {
			callbacks_[i].callback_ = callback;
			callbacks_[i].arg_ = argument;
			return true;
		}
	}

	return false;
}

int JASCallbackMgr::reject(JASCallback* callback, void* argument) {
	int rejectNum = 0;
	JASCriticalSection criticalSection;
	for (int i = 0; i < NUM_CALLBACKS; i++) {
		if (callbacks_[i].callback_ == callback && callbacks_[i].arg_ == argument) {
			callbacks_[i].callback_ = NULL;
			callbacks_[i].arg_ = NULL;
			rejectNum++;
		}
	}
	return rejectNum;
}

void JASCallbackMgr::callback() {
	for (int i = 0; i < NUM_CALLBACKS; i++) {
		if (callbacks_[i].callback_ && callbacks_[i].callback_(callbacks_[i].arg_) < 0) {
			callbacks_[i].callback_ = NULL;
			callbacks_[i].arg_ = NULL;
		}
	}
}
