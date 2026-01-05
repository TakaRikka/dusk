#include "group.h"

#include "common.h"
#include "layout.h"

#include <string>
#include <new>


namespace nw4hbm {
    namespace lyt {

        Group::Group() {}

        Group::Group(const res::Group* pResGroup, Pane* pRootPane) {
            Init();
            std::memcpy(mName, pResGroup->name, sizeof(mName));

            const char* paneName = detail::ConvertOffsToPtr<char>(pResGroup, sizeof(*pResGroup));

            for (int i = 0; i < pResGroup->paneNum; i++) {
                Pane* pFindPane =
                    pRootPane->FindPaneByName(paneName + (int)sizeof(pResGroup)->name * i, true);

                if (pFindPane) {
                    AppendPane(pFindPane);
                }
            }
        }

        void Group::Init() {
            mbUserAllocated = false;
        }

        Group::~Group() {
            for (PaneLinkList::Iterator it = mPaneLinkList.GetBeginIter();
                 it != mPaneLinkList.GetEndIter();)
            {
                PaneLinkList::Iterator currIt = it++;

                mPaneLinkList.Erase(currIt);
                Layout::FreeMemory(&*currIt);
            }
        }

        void Group::AppendPane(Pane* pPane) {
            if (void* pMem = Layout::AllocMemory(sizeof(detail::PaneLink))) {
                detail::PaneLink* pPaneLink = new (pMem) detail::PaneLink();

                pPaneLink->mTarget = pPane;
                mPaneLinkList.PushBack(pPaneLink);
            }
        }

        GroupContainer::~GroupContainer() {
            for (GroupList::Iterator it = mGroupList.GetBeginIter(); it != mGroupList.GetEndIter();)
            {
                GroupList::Iterator currIt = it++;

                mGroupList.Erase(currIt);

                if (!currIt->IsUserAllocated()) {
                    currIt->~Group();
                    Layout::FreeMemory(&*currIt);
                }
            }
        }

        void GroupContainer::AppendGroup(Group* pGroup) {
            mGroupList.PushBack(pGroup);
        }

        Group* GroupContainer::FindGroupByName(const char* findName) {
            for (GroupList::Iterator it = mGroupList.GetBeginIter(); it != mGroupList.GetEndIter();
                 it++)
            {
                if (detail::EqualsPaneName(it->GetName(), findName)) {
                    return &(*it);
                }
            }

            return NULL;
        }

    }  // namespace lyt
}  // namespace nw4hbm
