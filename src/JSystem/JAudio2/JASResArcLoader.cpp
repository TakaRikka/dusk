/**
 * JASResArcLoader.cpp
 * JAS - Resource Archive Loader
 */

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASResArcLoader.h"
#include "JSystem/JAudio2/JASDvdThread.h"
#include "JSystem/JAudio2/JASTaskThread.h"


size_t JASResArcLoader::getResSize(JKRArchive const* i_archiveP, u16 i_resourceID) {    
    JKRArchive::SDIFileEntry* file = i_archiveP->findIdResource(i_resourceID);

	if (file == 0) {
        return 0;
	}

    return file->data_size;
}

size_t JASResArcLoader::getResMaxSize(JKRArchive const* i_archiveP) {
    u32 maxSize, index;
    u32 fileEntries = i_archiveP->countFile();

    for (maxSize = 0, index = 0; index < fileEntries; index++) {
        JKRArchive::SDIFileEntry* file = i_archiveP->findIdxResource(index);
        if (file) {
            if (maxSize < file->data_size) {
                maxSize = file->data_size;
            }
        }
    }

    return maxSize;
}

static void JASResArcLoader::loadResourceCallback(void* args) {
    CallbackArgs* castedArgs = static_cast<CallbackArgs*>(args);
	u32 readResult           = castedArgs->mArchive->readResource(castedArgs->mBuffer, castedArgs->mBufferSize, castedArgs->mID);
	if (castedArgs->mCallback) {
		castedArgs->mCallback(readResult, castedArgs->mCallbackArg);
	}

	if (readResult == 0) {
		if (castedArgs->mQueue) {
			OSSendMessage(castedArgs->mQueue, (void*)RESARCMSG_Error, OS_MESSAGE_BLOCK);
		}
	} else {
		if (castedArgs->mQueue) {
			OSSendMessage(castedArgs->mQueue, (void*)RESARCMSG_Success, OS_MESSAGE_BLOCK);
		}
	}
}


int JASResArcLoader::loadResourceAsync(JKRArchive* archive, u16 id, u8* buffer, u32 size, LoadCallback callback, u32 cbArg)
{
	CallbackArgs args(id, buffer, size, archive);
	args.mCallback    = callback;
	args.mCallbackArg = cbArg;

	return JASDvd::getThreadPointer()->sendCmdMsg(&loadResourceCallback, (void*)&args, sizeof(CallbackArgs));
}
