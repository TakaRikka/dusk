#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JUtility/JUTDirectFile.h"
#include <dolphin/os.h>
#include "global.h"
#include <stdint.h>

int JUTDirectFile::fetch32byte() {
	mToRead = mLength - ALIGN_PREV(mPos, DVD_MIN_TRANSFER_SIZE);

	if (mToRead > JUTDF_BUFSIZE) {
		mToRead = JUTDF_BUFSIZE;
	}
	int interrupts = OSEnableInterrupts();
	int readRes    = DVDReadAsyncPrio(&mFileInfo, mSectorStart, ALIGN_NEXT(mToRead, DVD_MIN_TRANSFER_SIZE),
                               ALIGN_PREV(mPos, DVD_MIN_TRANSFER_SIZE), NULL, 2);
	OSRestoreInterrupts(interrupts);
	if (!readRes) {
		return -1;
	} else {
		interrupts = OSEnableInterrupts();
		while (DVDGetCommandBlockStatus(&mFileInfo.cb)) {
			;
		}
		OSRestoreInterrupts(interrupts);
		return mToRead;
	}
}

JUTDirectFile::JUTDirectFile() {
	mLength      = 0;
	mPos         = 0;
	mToRead      = 0;
	mSectorStart = (u8*)ALIGN_NEXT((uintptr_t)mBuffer, DVD_MIN_TRANSFER_SIZE);
	mIsOpen      = false;
}


JUTDirectFile::~JUTDirectFile() {
    mIsOpen = false;
}

bool JUTDirectFile::fopen(const char* filename) {
	if (!filename) {
		return false;
	}

	int interrupts = OSEnableInterrupts();
	int dvdRes     = DVDOpen(const_cast<char*>(filename), &mFileInfo);
	OSRestoreInterrupts(interrupts);

	if (!dvdRes) {
		mIsOpen = false;
		return false;
	}

	interrupts = OSEnableInterrupts();
	mLength         = mFileInfo.length;
	OSRestoreInterrupts(interrupts);

	mPos    = 0;
	mIsOpen = true;
	return true;
}

static void dummy(JUTDirectFile* directFile) {
	directFile->isOpened();
}

void JUTDirectFile::fclose() {
	if (!isOpened()) {
		return;
	}

	int interrupts = OSEnableInterrupts();
	DVDClose(&mFileInfo);
	OSRestoreInterrupts(interrupts);
	mIsOpen = false;
}

int JUTDirectFile::fgets(void* buf, int len) {
	// if file isn't open, return error (-1).
	if (!isOpened()) {
		return -1;
	}

	// if desired length to get is 0, get... 0 bytes.
	if (len == 0) {
		return 0;
	}

	// if desired length to get is 1, return 1.
	// (final byte gotten is always 0, so len 1 is pointless).
	if (len == 1) {
		buf = NULL;
		return 1;
	}

	// if buffer to read into doesn't exist, return error.
	if (!buf) {
		return -1;
	}

	// if we're already beyond the file length, return error.
	if (mPos >= mLength) {
		return -1;
	}

	u8* byteBuf   = (u8*)buf;
	int readCount = 0;

	while (mPos < mLength) {
		// if there's nothing left to read, return error.
		if (mToRead == 0 && fetch32byte() < 0) {
			return -1;
		}

		// read in each chunk.
		u32 currPos   = mPos & (JUTDF_BUFSIZE - 1);
		u32 chunkSize = (mToRead - currPos);
		if (readCount + chunkSize > len - 1) {
			chunkSize = len - readCount - 1;
		}

		BOOL isAtEnd = FALSE;
		for (int i = 0; i < chunkSize; i++) {
			u8 byte = mSectorStart[currPos++];
			*byteBuf++ = byte;

			// if we hit the end of a line, stop reading.
			if (byte == '\n') {
				isAtEnd   = TRUE;
				chunkSize = i + 1;
				break;
			}
		}

		// if we exceed the buffer size, stop reading.
		if (currPos >= JUTDF_BUFSIZE) {
			mToRead = 0;
		}

		// if we hit the end of a line, set final byte to 0 and stop reading.
		if (isAtEnd == TRUE) {
			*byteBuf = 0;
			mPos += chunkSize;
			readCount += chunkSize;
			break;
		}

		// we should have read the full chunkSize, so update count/pos.
		mPos += chunkSize;
		readCount += chunkSize;

		// if we're at (or beyond) our desired length, set final byte to 0 and stop reading.
		if (readCount >= len - 1) {
			*byteBuf = 0;
			break;
		}
	}

	// if got to the end of the data, set final byte to 0.
	if (mPos >= mLength) {
		*byteBuf = 0;
	}

	return readCount;
}
