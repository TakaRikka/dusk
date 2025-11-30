#ifndef JASRESARCLOADER_H
#define JASRESARCLOADER_H

#include "JSystem/JKernel/JKRArchive.h"
#include <dolphin/os.h>

namespace JASResArcLoader {
    size_t getResSize(JKRArchive const*, u16);
    size_t getResMaxSize(JKRArchive const*);
    static void loadResourceCallback(void*);
    int loadResourceAsync(JKRArchive*, u16, u8*, u32, void (*)(u32, u32), u32);
};

class JKRArchive;

// from pikmin2
typedef void (*LoadCallback)(u32, u32);

enum ResArcMessage {
	RESARCMSG_Error   = -1,
	RESARCMSG_Success = 0,
};

struct CallbackArgs {
	inline CallbackArgs(u16 id, u8* buf, u32 size, JKRArchive* archive)
	    : mArchive(archive)
	    , mID(id)
	    , mBuffer(buf)
	    , mBufferSize(size)
	    , mCallback(0)
	    , mCallbackArg(0)
	    , mQueue(0)
	{
	}

	JKRArchive* mArchive;   // _00
	u16 mID;                // _04
	u8* mBuffer;            // _08
	u32 mBufferSize;        // _0C
	LoadCallback mCallback; // _10
	u32 mCallbackArg;       // _14, arg to pass to mCallback along with readResource result
	OSMessageQueue* mQueue; // _18
};

#endif /* JASRESARCLOADER_H */
