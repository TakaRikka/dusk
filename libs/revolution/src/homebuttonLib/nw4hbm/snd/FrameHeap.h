#ifndef NW4HBM_SND_FRAME_HEAP_H
#define NW4HBM_SND_FRAME_HEAP_H

#include <revolution/types.h>

#include "../ut/LinkList.h"

#include <revolution/mem.h>

namespace nw4hbm {
    namespace snd {
        namespace detail {
            class FrameHeap {
            public:
                typedef void (*FreeCallback)(void* buffer, u32 size, void* callbackArg);

                FrameHeap();
                ~FrameHeap();

                bool Create(void* base, u32 size);
                void Destroy();
                void Clear();
                void* Alloc(u32 size, FreeCallback callback, void* callbackArg);

                int SaveState();
                void LoadState(int id);

                int GetCurrentLevel() const;
                u32 GetFreeSize() const;

                bool IsValid() const { return mHandle != NULL; }

            private:
                typedef struct Block {
                    /* 0x00 */ ut::LinkListNode mLink;
                    /* 0x08 */ u32 mSize;
                    /* 0x0C */ FreeCallback mCallback;
                    /* 0x10 */ void* mCallbackArg;

                    Block(u32 size, FreeCallback callback, void* callbackArg)
                        : mSize(size), mCallback(callback), mCallbackArg(callbackArg) {}

                    ~Block() {
                        if (mCallback != NULL) {
                            mCallback(GetBufferAddr(), mSize, mCallbackArg);
                        }
                    }

                    void* GetBufferAddr() { return ut::AddOffsetToPtr(this, BLOCK_BUFFER_SIZE); }
                } Block;
                typedef ut::LinkList<Block, offsetof(Block, mLink)> BlockList;

                typedef struct Section {
                    /* 0x00 */ ut::LinkListNode mLink;
                    /* 0x08 */ BlockList mBlockList;

                    ~Section();
                    void AppendBlock(Block* block) { mBlockList.PushBack(block); }
                } Section;
                typedef ut::LinkList<Section, offsetof(Section, mLink)> SectionList;

                bool NewSection();
                void ClearSection();

                static const int BLOCK_BUFFER_SIZE = OSRoundUp32B(sizeof(Block));
                static const int HEAP_ALIGN = 32;

                /* 0x00 */ MEMHeapHandle mHandle;
                /* 0x04 */ SectionList mSectionList;
            };
        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
