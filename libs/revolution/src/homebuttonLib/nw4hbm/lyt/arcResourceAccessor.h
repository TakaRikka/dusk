#ifndef NW4HBM_LYT_ARC_RESOURCE_ACCESSOR_H
#define NW4HBM_LYT_ARC_RESOURCE_ACCESSOR_H

#include <revolution/types.h>

#include <revolution/arc.h>

#include "../ut/Font.h"
#include "../ut/LinkList.h"

#include "resourceAccessor.h"

namespace nw4hbm {
    namespace lyt {

        static const int RESOURCE_NAME_MAX = 128;

        class FontRefLink {
        public:
            FontRefLink();

            void Set(const char* name, ut::Font* pFont);

            const char* GetFontName() const { return mFontName; }
            ut::Font* GetFont() const { return mpFont; }

            /* 0x00 */ ut::LinkListNode mLink;

        protected:
            /* 0x08 */ char mFontName[RESOURCE_NAME_MAX];
            /* 0x88 */ ut::Font* mpFont;
        };
        typedef ut::LinkList<FontRefLink, offsetof(FontRefLink, mLink)> FontRefLinkList;

        class ArcResourceLink {
        public:
            ArcResourceLink() {}

            bool Set(void* archiveStart, const char* resRootDirectory);

            char* GetResRootDir() { return mResRootDir; }
            ARCHandle* GetArcHandle() { return &mArcHandle; }

            /* 0x00 */ ut::LinkListNode mLink;

        protected:
            /* 0x08 */ ARCHandle mArcHandle;

            /* 0x24 */ char mResRootDir[RESOURCE_NAME_MAX];
        };
        typedef ut::LinkList<ArcResourceLink, offsetof(ArcResourceLink, mLink)> ArcResourceLinkList;

        class ArcResourceAccessor : public ResourceAccessor {
        public:
            ArcResourceAccessor();

            /* 0x08 */ virtual ~ArcResourceAccessor() {}
            /* 0x0C */ virtual void* GetResource(u32 resType, const char* name, u32* pSize = NULL);
            /* 0x10 */ virtual ut::Font* GetFont(const char* name);

            bool Attach(void* archiveStart, const char* resourceRootDirectory);

            bool IsAttached(void) { return this->mArcBuf != NULL; }

        private:
            /* 0x00 (base) */
            /* 0x04 */ ARCHandle mArcHandle;
            /* 0x20 */ void* mArcBuf;
            /* 0x24 */ FontRefLinkList mFontList;
            /* 0x30 */ char mResRootDir[RESOURCE_NAME_MAX];
        };

        class MultiArcResourceAccessor : public ResourceAccessor {
        public:
            MultiArcResourceAccessor();

            /* 0x08 */ virtual ~MultiArcResourceAccessor();
            /* 0x0C */ virtual void* GetResource(u32 resType, const char* name, u32* pSize = NULL);
            /* 0x10 */ virtual ut::Font* GetFont(const char* name);

            void Attach(ArcResourceLink* pLink);
            void DetachAll() { reinterpret_cast<ut::detail::LinkListImpl*>(&mArcList)->Clear(); }
            void RegistFont(FontRefLink* pLink);

        protected:
            /* 0x00 (base) */
            /* 0x04 */ ArcResourceLinkList mArcList;
            /* 0x10 */ FontRefLinkList mFontList;
        };

        namespace detail {
            ut::Font* FindFont(FontRefLinkList* pFontRefList, const char* name);
        }

    }  // namespace lyt
}  // namespace nw4hbm

#endif
