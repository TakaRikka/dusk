#ifndef NW4HBM_LYT_GROUP_H
#define NW4HBM_LYT_GROUP_H

#include <revolution/types.h>

#include "pane.h"

#include "../ut/LinkList.h"

namespace nw4hbm {
    namespace lyt {

        namespace detail {
            typedef struct PaneLink {
                ut::LinkListNode mLink;

                /* 0x08 */ Pane* mTarget;
            } PaneLink;
        }  // namespace detail
        typedef ut::LinkList<detail::PaneLink, offsetof(detail::PaneLink, mLink)> PaneLinkList;

        class Group {
        public:
            Group();
            Group(const res::Group* pResGroup, Pane* pRootPane);

            /* 0x08 */ virtual ~Group();

            const char* GetName() const { return mName; }
            bool IsUserAllocated() const { return mbUserAllocated; }

            PaneLinkList& GetPaneList() { return mPaneLinkList; };

            void Init();
            void AppendPane(Pane* pane);

            /* 0x00 (vtable) */
            /* 0x04 */ ut::LinkListNode mLink;

        protected:
            /* 0x0C */ PaneLinkList mPaneLinkList;
            /* 0x18 */ char mName[16];
            /* 0x29 */ bool mbUserAllocated;
            /* 0x2A */ u8 mPadding[2];
        };
        typedef ut::LinkList<Group, offsetof(Group, mLink)> GroupList;

        class GroupContainer {
        public:
            GroupContainer() {}
            ~GroupContainer();

            GroupList& GetGroupList() { return mGroupList; }

            void AppendGroup(Group* pGroup);
            Group* FindGroupByName(const char* findName);

        protected:
            /* 0x00 */ GroupList mGroupList;
        };

    }  // namespace lyt
}  // namespace nw4hbm

#endif
