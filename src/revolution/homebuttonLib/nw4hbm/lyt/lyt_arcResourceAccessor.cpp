#include "arcResourceAccessor.h"

#include <revolution/arc.h>

#include <string>

namespace {

    s32 FindNameResource(ARCHandle* pArcHandle, const char* resName) NO_INLINE {
        s32 entryNum = -1;

        ARCDir dir;
        BOOL bSuccess = ARCOpenDir(pArcHandle, ".", &dir);
        NW4HBM_ASSERT(48, bSuccess);

        ARCDirEntry dirEntry;

        while (ARCReadDir(&dir, &dirEntry)) {
            if (dirEntry.isDir != 0) {
                bSuccess = ARCChangeDir(pArcHandle, dirEntry.name);
                NW4HBM_ASSERT(57, bSuccess);

                entryNum = FindNameResource(pArcHandle, resName);
                bSuccess = ARCChangeDir(pArcHandle, "..");
                NW4HBM_ASSERT(60, bSuccess);

                if (entryNum != -1) {
                    break;
                }
            } else if (stricmp(resName, dirEntry.name) == 0) {
                entryNum = dirEntry.entryNum;
                break;
            }
        }

        bSuccess = ARCCloseDir(&dir);
        NW4HBM_ASSERT(77, bSuccess);
        return entryNum;
    }

    void* GetResourceSub(ARCHandle* pArcHandle, const char* resRootDir, u32 resType,
                         const char* name, u32* pSize) {
        s32 entryNum = -1;

        if (ARCConvertPathToEntrynum(pArcHandle, resRootDir) != -1 &&
            ARCChangeDir(pArcHandle, resRootDir))
        {
            if (!resType) {
                entryNum = FindNameResource(pArcHandle, name);
            } else {
                char resTypeStr[5];
                resTypeStr[0] = resType >> 24;
                resTypeStr[1] = resType >> 16;
                resTypeStr[2] = resType >> 8;
                resTypeStr[3] = resType;
                resTypeStr[4] = '\0';

                if (ARCConvertPathToEntrynum(pArcHandle, resTypeStr) != -1 &&
                    ARCChangeDir(pArcHandle, resTypeStr))
                {
                    entryNum = ARCConvertPathToEntrynum(pArcHandle, name);
                    BOOL bSuccess = ARCChangeDir(pArcHandle, "..");
                    NW4HBM_ASSERT(117, bSuccess);
                }
            }

            BOOL bSuccess = ARCChangeDir(pArcHandle, "..");
            NW4HBM_ASSERT(123, bSuccess);
        }

        if (entryNum != -1) {
            ARCFileInfo arcFileInfo;
            BOOL bSuccess = ARCFastOpen(pArcHandle, entryNum, &arcFileInfo);
            NW4HBM_ASSERT(131, bSuccess);

            void* resPtr = ARCGetStartAddrInMem(&arcFileInfo);

            if (pSize) {
                *pSize = ARCGetLength(&arcFileInfo);
            }

            ARCClose(&arcFileInfo);

            return resPtr;
        }

        return NULL;
    }
}  // namespace

namespace nw4hbm {
    namespace lyt {
        ut::Font* detail::FindFont(FontRefLinkList* pFontRefList, const char* name) {
            for (FontRefLinkList::Iterator it = pFontRefList->GetBeginIter();
                 it != pFontRefList->GetEndIter(); it++)
            {
                if (strcmp(name, it->GetFontName()) == 0) {
                    return it->GetFont();
                }
            }
            return NULL;
        }

        FontRefLink::FontRefLink() : mpFont(NULL) {}

        void FontRefLink::Set(const char* name, ut::Font* pFont) {
            strcpy(mFontName, name);
            mpFont = pFont;
        }

        ArcResourceAccessor::ArcResourceAccessor() : mArcBuf(NULL) {}

        void dummyString() {
            OSReport("NW4HBM:Failed assertion std::strlen(name) < FONTNAMEBUF_MAX");
        }

        bool ArcResourceAccessor::Attach(void* archiveStart, const char* resourceRootDirectory) {
            // clang-format off
    NW4HBM_ASSERT(220, ! IsAttached());
    NW4HBM_ASSERT_CHECK_NULL(221, archiveStart);
    NW4HBM_ASSERT_CHECK_NULL(222, resourceRootDirectory);
            // clang-format on

            BOOL bSuccess = ARCInitHandle(archiveStart, &mArcHandle);

            if (!bSuccess) {
                return false;
            }

            mArcBuf = archiveStart;

            strncpy(mResRootDir, resourceRootDirectory, ARRAY_SIZE(mResRootDir) - 1);
            mResRootDir[ARRAY_SIZE(mResRootDir) - 1] = '\0';

            return true;
        }

        void dummyString2() {
            OSReport("NW4HBM:Failed assertion IsAttached()");
            OSReport("NW4HBM:Pointer must not be NULL (pLink)");
        }

        void* ArcResourceAccessor::GetResource(u32 resType, const char* name, u32* pSize) {
            return GetResourceSub(&mArcHandle, mResRootDir, resType, name, pSize);
        }

        bool ArcResourceLink::Set(void* archiveStart, const char* resRootDirectory) {
            BOOL bSuccess = ARCInitHandle(archiveStart, &mArcHandle);

            if (!bSuccess) {
                return false;
            }

            strncpy(mResRootDir, resRootDirectory, ARRAY_SIZE(mResRootDir) - 1);
            mResRootDir[ARRAY_SIZE(mResRootDir) - 1] = '\0';

            return true;
        }

        ut::Font* ArcResourceAccessor::GetFont(const char* name) {
            return detail::FindFont(&mFontList, name);
        }

        MultiArcResourceAccessor::MultiArcResourceAccessor() {}

        MultiArcResourceAccessor::~MultiArcResourceAccessor() {
            DetachAll();
        }

        void MultiArcResourceAccessor::Attach(ArcResourceLink* pLink) {
            mArcList.PushBack(pLink);
        }

        void* MultiArcResourceAccessor::GetResource(u32 resType, const char* name, u32* pSize) {
            for (ArcResourceLinkList::Iterator it = mArcList.GetBeginIter();
                 it != mArcList.GetEndIter(); it++)
            {
                ARCHandle* pArcHandle = it->GetArcHandle();
                if (void* resPtr =
                        GetResourceSub(pArcHandle, it->GetResRootDir(), resType, name, pSize))
                {
                    return resPtr;
                }
            }
            return NULL;
        }

        void MultiArcResourceAccessor::RegistFont(FontRefLink* pLink) {
            mFontList.PushBack(pLink);
        }

        ut::Font* MultiArcResourceAccessor::GetFont(const char* name) {
            return detail::FindFont(&mFontList, name);
        }

    }  // namespace lyt
}  // namespace nw4hbm
