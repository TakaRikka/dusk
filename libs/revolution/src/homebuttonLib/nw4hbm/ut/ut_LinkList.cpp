#include "LinkList.h"

#include "../db/assert.h"

namespace nw4hbm {
    namespace ut {
        namespace detail {

            LinkListImpl::~LinkListImpl() {
                Clear();
            }

            LinkListImpl::Iterator LinkListImpl::Erase(Iterator it) {
                // clang-format off
                NW4HBM_ASSERT(31, it.mPointer!=&mNode);
                // clang-format on

                Iterator itNext(it);
                ++itNext;

                return Erase(it, itNext);
            }

            LinkListImpl::Iterator LinkListImpl::Erase(LinkListImpl::Iterator itFirst,
                                                       LinkListImpl::Iterator itLast) {
                LinkListNode *p = itFirst.mPointer, *pItLast = itLast.mPointer, *pNext;

                while (p != pItLast) {
                    pNext = p->mNext;
                    Erase(p);
                    p = pNext;
                }

                return itLast;
            }

            void LinkListImpl::Clear() {
                Erase(GetBeginIter(), GetEndIter());
            }

            LinkListImpl::Iterator LinkListImpl::Insert(Iterator it, LinkListNode* p) {
                NW4HBM_ASSERT_CHECK_NULL(74, p);

                LinkListNode* pIt = it.mPointer;
                NW4HBM_ASSERT_CHECK_NULL(76, pIt);

                LinkListNode* pItPrev = pIt->mPrev;
                NW4HBM_ASSERT_CHECK_NULL(79, pItPrev);

                NW4HBM_ASSERT(81, p->mNext == NULL);
                NW4HBM_ASSERT(82, p->mPrev == NULL);
                p->mNext = pIt;
                p->mPrev = pItPrev;

                pIt->mPrev = p;
                pItPrev->mNext = p;

                mSize++;

                return (Iterator)p;
            }

            LinkListImpl::Iterator LinkListImpl::Erase(LinkListNode* p) {
                // clang-format off
                NW4HBM_ASSERT(96, !IsEmpty());
                NW4HBM_ASSERT_CHECK_NULL(97, p);
                NW4HBM_ASSERT(98, p!=&mNode);
                // clang-format on

                LinkListNode* pNext = p->mNext;
                LinkListNode* pPrev = p->mPrev;

                SetPrev(pPrev, pNext);
                SetNext(pPrev, pNext);

                mSize--;

                p->mNext = NULL;
                p->mPrev = NULL;

                return (Iterator)pNext;
            }

            void LinkListImpl::SetPrev(LinkListNode* pPrev, LinkListNode* pNext) {
                NW4HBM_ASSERT_CHECK_NULL(101, pNext);
                pNext->mPrev = pPrev;
            }

            void LinkListImpl::SetNext(LinkListNode* pPrev, LinkListNode* pNext) {
                NW4HBM_ASSERT_CHECK_NULL(103, pPrev);
                pPrev->mNext = pNext;
            }

        }  // namespace detail
    }  // namespace ut
}  // namespace nw4hbm
