#include "NandFileStream.h"

#include "../db/assert.h"

namespace nw4hbm {
    namespace ut {

        NW4HBM_UT_GET_DERIVED_RUNTIME_TYPEINFO(NandFileStream, FileStream);

        void NandFileStream::NandAsyncCallback_(s32 result, NANDCommandBlock* commandBlock) {
            NW4HBM_ASSERT_CHECK_NULL(44, commandBlock);
            NandFileStream* p = reinterpret_cast<NandFileStreamInfo*>(commandBlock)->stream;

            p->mIsBusy = false;
            p->mAsyncResult = result;

            if (p->mCallback != NULL) {
                p->mCallback(result, p, p->mArg);
            }
        }

        void NandFileStream::Initialize_() {
            mCanRead = false;
            mCanWrite = false;

            mCloseOnDestroyFlg = false;
            mCloseEnableFlg = false;

            mAvailable = false;
            mIsBusy = false;

            mCallback = NULL;
            mArg = NULL;
            mAsyncResult = NAND_RESULT_OK;

            mFileInfo.stream = this;
        }

        NandFileStream::NandFileStream(const char* path, u32 mode) {
            NW4HBM_ASSERT_CHECK_NULL(113, path);
            Initialize_();
            Open(path, mode);
        }

        NandFileStream::NandFileStream(const NANDFileInfo* pInfo, u32 mode, bool enableClose) {
            Initialize_();
            Open(pInfo, mode, enableClose);
        }

        NandFileStream::~NandFileStream() {
            if (mCloseOnDestroyFlg) {
                Close();
            }
        }

        bool NandFileStream::Open(const char* path, u32 mode) {
            NW4HBM_ASSERT_CHECK_NULL(173, path);

            if (mCloseOnDestroyFlg) {
                Close();
            }

            mCanRead = mode & NAND_ACCESS_READ;
            mCanWrite = mode & NAND_ACCESS_WRITE;

            if (NANDOpen(path, &mFileInfo.nandInfo, mode) != NAND_RESULT_OK) {
                return false;
            }

            if (mCanRead) {
                u32 fileSize;

                if (NANDGetLength(&mFileInfo.nandInfo, &fileSize) != NAND_RESULT_OK) {
                    NANDClose(&mFileInfo.nandInfo);
                    return false;
                }

                mFilePosition.SetFileSize(fileSize);
            }

            mFilePosition.Seek(0, SEEK_BEG);

            mCloseOnDestroyFlg = true;
            mCloseEnableFlg = true;
            mAvailable = true;

            return true;
        }

        bool NandFileStream::Open(const NANDFileInfo* pInfo, u32 mode, bool enableClose) {
            if (mCloseOnDestroyFlg) {
                Close();
            }

            mCanRead = mode & NAND_ACCESS_READ;
            mCanWrite = mode & NAND_ACCESS_WRITE;

            mFileInfo.nandInfo = *pInfo;

            u32 fileSize;
            if (NANDGetLength(&mFileInfo.nandInfo, &fileSize) != NAND_RESULT_OK) {
                if (enableClose) {
                    NANDClose(&mFileInfo.nandInfo);
                }

                return false;
            }

            mFilePosition.SetFileSize(fileSize);
            mFilePosition.Seek(0, SEEK_BEG);

            mCloseOnDestroyFlg = false;
            mCloseEnableFlg = enableClose;
            mAvailable = true;

            return true;
        }

        void NandFileStream::Close() {
            if (mCloseEnableFlg && mAvailable) {
                NW4R_DB_ASSERTMSG(264, NANDClose(&mFileInfo.nandInfo) == NAND_RESULT_OK,
                                         "Can't Close NAND File. It still has been used\n");
                mAvailable = false;
            }
        }

        s32 NandFileStream::Read(void* buf, u32 length) {
            NW4HBM_ASSERT_ALIGN32(284, buf);
            NW4HBM_ASSERT_ALIGN32(285, length);
            NW4R_DB_ASSERTMSG(286, this->IsAvailable() != 0, "NandFileStream is not opened");
            NANDSeek(&mFileInfo.nandInfo, mFilePosition.Tell(), NAND_SEEK_BEG);

            s32 result = NANDRead(&mFileInfo.nandInfo, buf, length);
            mFilePosition.Skip(result);

            return result;
        }

        bool NandFileStream::ReadAsync(void* buf, u32 length, IOStreamCallback pCallback,
                                       void* pCallbackArg) {
            NW4HBM_ASSERT_ALIGNED(313, buf, 32);
            NW4HBM_ASSERT_ALIGNED(314, length, 32);
            NW4R_DB_ASSERTMSG(315, this->IsAvailable() != 0, "NandFileStream is not opened");

            return ReadAsyncImpl(buf, length, pCallback, pCallbackArg);
        }

        // fake? ReadAsync requires inlines
        void NandFileStream::ReadAsyncSetArgs(IOStreamCallback pCallback, void* pCallbackArg) {
            mCallback = pCallback;
            mArg = pCallbackArg;
            mIsBusy = true;
        }

        // fake? ReadAsync requires inlines
        bool NandFileStream::ReadAsyncImpl(void* buf, u32 length, IOStreamCallback pCallback,
                                           void* pCallbackArg) {
            NW4HBM_ASSERT_ALIGN32(370, buf);
            NW4HBM_ASSERT_ALIGN32(371, length);
            NW4R_DB_ASSERTMSG(372, this->IsAvailable() != 0, "NandFileStream is not opened");

            ReadAsyncSetArgs(pCallback, pCallbackArg);

            NANDSeek(&mFileInfo.nandInfo, mFilePosition.Tell(), NAND_SEEK_BEG);

            bool success = NANDReadAsync(&mFileInfo.nandInfo, buf, length, NandAsyncCallback_,
                                         &mFileInfo.nandBlock) == NAND_RESULT_OK;

            if (success) {
                mFilePosition.Skip(length);
            } else {
                mIsBusy = false;
            }

            return success;
        }

        void NandFileStream::Write(const void* buf, u32 length) {
            NW4HBM_ASSERT_ALIGN32(396, buf);
            NW4HBM_ASSERT_ALIGN32(397, length);
            NW4R_DB_ASSERTMSG(398, this->IsAvailable() != 0, "NandFileStream is not opened");

            NANDSeek(&mFileInfo.nandInfo, mFilePosition.Tell(), NAND_SEEK_BEG);
            s32 result = NANDWrite(&mFileInfo.nandInfo, buf, length);

            //! @bug: Error code will be interpreted as a negative size
            mFilePosition.Append(result);
        }

        bool NandFileStream::WriteAsync(const void* buf, u32 length, IOStreamCallback pCallback,
                                        void* pCallbackArg) {
            NW4HBM_ASSERT_ALIGN32(423, buf);
            NW4HBM_ASSERT_ALIGN32(424, length);
            NW4R_DB_ASSERTMSG(425, this->IsAvailable() != 0, "NandFileStream is not opened");

            mCallback = pCallback;
            mArg = pCallbackArg;
            mIsBusy = true;

            NANDSeek(&mFileInfo.nandInfo, mFilePosition.Tell(), NAND_SEEK_BEG);

            s32 result = NANDWriteAsync(&mFileInfo.nandInfo, buf, length, NandAsyncCallback_,
                                        &mFileInfo.nandBlock);

            if (result == NAND_RESULT_OK) {
                mFilePosition.Append(length);
            } else {
                mIsBusy = false;
            }

            return result == NAND_RESULT_OK;
        }

        void NandFileStream::Seek(s32 offset, u32 origin) {
            NW4R_DB_ASSERTMSG(462, this->IsAvailable() != 0, "NandFileStream is not opened");
            mFilePosition.Seek(offset, origin);
        }

    }  // namespace ut
}  // namespace nw4hbm
