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
	maxSize = 0;
    for (index = 0; index < fileEntries; index++) {
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
    TLoadResInfo* castedArgs = static_cast<TLoadResInfo*>(args);
	u32 readResult           = castedArgs->mArchive->readResource(castedArgs->mBuffer, castedArgs->mBufferSize, castedArgs->mID);
	if (castedArgs->mCallback) {
		castedArgs->mCallback(readResult, castedArgs->mCallbackArg);
	}

	if (readResult == 0) {
		if (castedArgs->mQueue) {
			OSSendMessage(castedArgs->mQueue, (void*)RESARCMSG_Error, OS_MESSAGE_BLOCK);
		}
		return;
	}
	if (castedArgs->mQueue) {
		OSSendMessage(castedArgs->mQueue, (void*)RESARCMSG_Success, OS_MESSAGE_BLOCK);
	}
}


int JASResArcLoader::loadResourceAsync(JKRArchive* archive, u16 id, u8* buffer, u32 size, LoadCallback callback, u32 cbArg)
{
	TLoadResInfo args(archive, id, buffer, size);
	args.mCallback    = callback;
	args.mCallbackArg = cbArg;

	int r31 = JASDvd::getThreadPointer()->sendCmdMsg(&loadResourceCallback, (void*)&args, sizeof(TLoadResInfo));
	if (!r31) {
		JUT_WARN(160, "%s", "sendCmdMsg loadResourceCallback Failed");
	}
	return r31;
}
