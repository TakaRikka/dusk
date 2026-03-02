#ifndef JORFILE_H
#define JORFILE_H

#include "JSystem/JSupport/JSUMemoryStream.h"

class JORDir {
public:
    void setStatus(u32 status) { m_status = status; }
    void setFindHandle(u32 handle) { m_findHandle = handle; }
    void setFileAttribute(u32 attribute) { m_fileAttribute = attribute; }
    void setLowDateTime(u32 time) { m_lowDateTime = time; }
    void setHighDateTime(u32 time) { m_highDateTime = time; }

    char* getFilename() { return m_filename; }

    /* 0x00 */ u32 m_status;
    /* 0x04 */ u32 m_findHandle;
    /* 0x08 */ u32 m_fileAttribute;
    /* 0x0C */ u32 m_lowDateTime;
    /* 0x10 */ u32 m_highDateTime;
    /* 0x14 */ char* m_filename;
};

class JORFile {
public:
    enum ECommand {
        ECommand_OPEN,
        ECommand_CLOSE,
        ECommand_READ,
        ECommand_WRITE,
    };

    enum EStatus {
        EStatus_WAIT,
        EStatus_READ_BEGIN,
        EStatus_READ_DATA,
        EStatus_READ_END,
        EStatus_WRITE_BEGIN,
        EStatus_WRITE_DATA,
        EStatus_WRITE_END,
    };

    enum EFlags {
        EFlags_READ        = (1 << 0),
        EFlags_WRITE       = (1 << 1),
        EFlags_UNK_0x4     = (1 << 2),
        EFlags_DEFAULT_EXT = (1 << 4),
        EFlags_UNK_0x20    = (1 << 5),
        EFlags_HAS_SUFFIX  = (1 << 6),
    };

    JORFile();

    int countMaskSize(const char* mask);
    void setBuffer(void* buffer, s32 length);

    char* getFilename() { return mFilename; }
    u32 getHandle() const { return mHandle; }

    JSUMemoryOutputStream& getDataStream() { return mDataStream; }

    void setHandle(u32 handle) { mHandle = handle; }
    void setFileLength(s32 length) { mFileLength = length; }
    void setNFileName(u16 length) { mNFileName = length; }
    void setNBaseName(u16 length) { mNBaseName = length; }
    void setNExtensionName(u16 length) { mNExtensionName = length; }
    void setStatus(s32 status) { mStatus = status; }

    virtual ~JORFile() {
        close();
    }
    virtual int open(const char* path, u32 flags, const char* extMask, const char* defaultExt, const char* param_4, const char* fileSuffix);
    virtual int open(u32 flags, const char* extMask, const char* defaultExt, const char* param_4, const char* fileSuffix) {
        return open(NULL, flags, extMask, defaultExt, param_4, fileSuffix);
    }
    virtual void close();
    virtual s32 readData(void* buffer, s32 length);
    virtual s32 writeData(const void* buffer, s32 length);
    virtual void readBegin_(s32 len);
    virtual void readLoop_();
    virtual void writeBegin_(s32 len);
    virtual void writeLoop_(const void* pBuffer, s32 size, u32 pos);
    virtual void writeDone_(s32 len);
    virtual void waitMessage_();
    virtual s32 getFileSize() const { return mFileLength; }

    /* 0x04 */ u32 mHandle;
    /* 0x08 */ s32 mFileLength;
    /* 0x0C */ s32 mStatus;
    /* 0x10 */ u16 mNFileName;
    /* 0x12 */ u16 mNBaseName;
    /* 0x14 */ u16 mNExtensionName;
    /* 0x16 */ u16 mFlags;
    /* 0x18 */ int field_0x18;
    /* 0x1C */ JSUMemoryOutputStream mDataStream;
    /* 0x30 */ char mFilename[256];
};

#endif /* JORFILE_H */
