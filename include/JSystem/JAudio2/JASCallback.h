#ifndef JASCALLBACK_H
#define JASCALLBACK_H

#include <dolphin/types.h>

typedef s32 JASCallback(void*);

/**
 * @ingroup jsystem-jaudio
 * 
 */
struct JASCallbackMgr {
	struct TCallback {
		TCallback()
		    : callback_(NULL)
		    , arg_(NULL) {
		}

		/* 0x00 */ JASCallback* callback_;
		/* 0x04 */ void* arg_;
	};

	bool regist(JASCallback* callback, void* argument);
	int reject(JASCallback* callback, void* argument);
	void callback();

	static const int NUM_CALLBACKS = 32;

	/* 0x00 */ TCallback callbacks_[NUM_CALLBACKS];
};

#endif /* JASCALLBACK_H */
