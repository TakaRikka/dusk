#ifndef NW4HBM_SND_NAND_SOUND_ARCHIVE_H
#define NW4HBM_SND_NAND_SOUND_ARCHIVE_H

#include <revolution/nand.h>
#include "../ut/FileStream.h"
#include "SoundArchive.h"
#include "SoundArchiveFile.h"

namespace nw4hbm {
    namespace snd {

        class NandSoundArchive : public SoundArchive {
        private:
            class NandFileStream;

        public:
            NandSoundArchive();

            /* 0x08 */ virtual ~NandSoundArchive();
            /* 0x0C */ virtual const void* detail_GetFileAddress(u32 id) const { return NULL; }
            /* 0x10 */ virtual const void* detail_GetWaveDataFileAddress(u32 id) const {
                return NULL;
            }
            /* 0x14 */ virtual int detail_GetRequiredStreamBufferSize() const;
            /* 0x18 */ virtual ut::FileStream* OpenStream(void* buffer, int size, u32 offset,
                                                          u32 length) const;
            /* 0x1C */ virtual ut::FileStream* OpenExtStream(void* buffer, int size,
                                                             const char* extPath, u32 offset,
                                                             u32 length) const;

            bool Open(const char* path);
            void Close();

            bool LoadHeader(void* buffer, u32 size);
            bool LoadLabelStringData(void* buffer, u32 size);

            u32 GetHeaderSize() const { return mFileReader.GetInfoChunkSize(); }
            u32 GetLabelStringDataSize() const { return mFileReader.GetLabelStringChunkSize(); }

        private:
            bool LoadFileHeader() NO_INLINE;

        private:
            /* 0x108 */ detail::SoundArchiveFileReader mFileReader;
            /* 0x14C */ NANDFileInfo mFileInfo;
            /* 0x1D8 */ bool mOpen;
        };

    }  // namespace snd
}  // namespace nw4hbm

#endif
