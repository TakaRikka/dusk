#ifndef JUTDIRECTFILE_H
#define JUTDIRECTFILE_H

#include "dolphin/dvd.h"

#define JUTDF_BUFSIZE (0x800)

/**
* @ingroup jsystem-jutility
* 
*/
struct JUTDirectFile {
    int fetch32byte();
    JUTDirectFile();
    ~JUTDirectFile();
    bool fopen(char const*);
    void fclose();
    int fgets(void*, int);

    /* 0x000 */ u8 mBuffer[0x820];
	/* 0x820 */ u8* mSectorStart;
	/* 0x824 */ u32 mToRead;
	/* 0x828 */ u32 mLength;
	/* 0x82C */ u32 mPos;
	/* 0x830 */ bool mIsOpen;
	/* 0x834 */ DVDFileInfo mFileInfo;
};

#endif /* JUTDIRECTFILE_H */
