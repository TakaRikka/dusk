#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JGadget/linklist.h"
#include "JSystem/JGadget/define.h"

namespace JGadget {
namespace {
template <typename T>
class TPRIsEqual_pointer_ {
public:
    TPRIsEqual_pointer_<T>(const T* p) { this->p_ = p; }

    bool operator()(const T& rSrc) const { return &rSrc == this->p_; }

private:
    const T* p_;
};
}
}

JGadget::TNodeLinkList::~TNodeLinkList() {
#if DEBUG
    Confirm();
    clear();
    JGADGET_ASSERTWARN(84, empty())
    oNode_.clear_();
#endif
}

JGadget::TNodeLinkList::iterator
JGadget::TNodeLinkList::erase(JGadget::TNodeLinkList::iterator it) {
    JUT_ASSERT(102, it.p_!=&oNode_);
    iterator next = it;
    ++next;
    return erase(it, next);
}

JGadget::TNodeLinkList::iterator JGadget::TNodeLinkList::erase(iterator a, iterator b) {
    TLinkListNode* cur = a.p_;
    TLinkListNode* end = b.p_;
    TLinkListNode* next;

    for (; cur != end; cur = next) {
        next = cur->pNext_;
        Erase(cur);
    }

    return b;
}

void JGadget::TNodeLinkList::splice(iterator it, TNodeLinkList& rSrc) {
    JUT_ASSERT(146, this!=&rSrc);
    splice(it, rSrc, rSrc.begin(), rSrc.end());
    JUT_ASSERT(148, rSrc.empty());
}

void JGadget::TNodeLinkList::splice(iterator it, TNodeLinkList& rSrc, iterator itSrc) {
    iterator itSrcNext = itSrc;
    ++itSrcNext;

    if ((it == itSrc) || (it == itSrcNext)) {
        return;
    } else {
        TLinkListNode* const node = &(*itSrc);
        rSrc.Erase(node);
        Insert(it, node);
    }
}

void JGadget::TNodeLinkList::splice(iterator it, TNodeLinkList& rSrc, iterator itBegin, iterator itEnd) {
    s32 dist = 0;
    if (this == &rSrc) {
        if (itBegin == itEnd) {
            return;
        }
    } else {
        dist = std::distance(itBegin, itEnd);
        if (dist == 0) {
            return;
        }
    }
    TLinkListNode* r31 = it.p_;
    TLinkListNode* r30 = itBegin.p_;
    TLinkListNode* r29 = itEnd.p_;
    TLinkListNode* r25 = r29->pPrev_;
    TLinkListNode* r24 = r30->pPrev_;
    r24->pNext_ = r29;
    r29->pPrev_ = r24;
    rSrc.count -= dist;
    TLinkListNode* r23 = r31->pPrev_;
    r23->pNext_ = r30;
    r30->pPrev_ = r23;
    r25->pNext_ = r31;
    r31->pPrev_ = r25;
    count += dist;
}

JGadget::TNodeLinkList::iterator JGadget::TNodeLinkList::Find(const JGadget::TLinkListNode* p) {
    return std::find_if(begin(), end(), TPRIsEqual_pointer_<TLinkListNode>(p));
}

JGadget::TNodeLinkList::iterator JGadget::TNodeLinkList::Insert(iterator it, TLinkListNode* p) {
    JUT_ASSERT(300, p!=NULL);
    TLinkListNode* pIt = it.p_;
    JUT_ASSERT(302, pIt!=NULL);
    TLinkListNode* pItPrev = pIt->pPrev_;
    JUT_ASSERT(305, pItPrev!=0);
    JGADGET_ASSERTWARN(307, p->pNext_==NULL);
    JGADGET_ASSERTWARN(308, p->pPrev_==NULL);

    p->pNext_ = pIt;
    p->pPrev_ = pItPrev;
    pIt->pPrev_ = p;
    pItPrev->pNext_ = p;
    count++;
    return iterator(p);
}

JGadget::TNodeLinkList::iterator JGadget::TNodeLinkList::Erase(TLinkListNode* p) {
    JUT_ASSERT(325, !empty());
    JUT_ASSERT(326, p!=0);
    JUT_ASSERT(327, p!=&oNode_);
    TLinkListNode* pNext = p->pNext_;
    TLinkListNode* pPrev = p->pPrev_;
    JUT_ASSERT(330, pNext!=NULL);
    pNext->pPrev_ = pPrev;
    JUT_ASSERT(332, pPrev!=NULL);
    pPrev->pNext_ = pNext;
    count--;
#if DEBUG
    p->clear_();
#endif
    return iterator(pNext);
}

// NONMATCHING - missing stack
void JGadget::TNodeLinkList::Remove(TLinkListNode* p) {
    remove_if(TPRIsEqual_pointer_<TLinkListNode>(p));
}

bool JGadget::TNodeLinkList::Confirm() const {
    int u = 0;
    const_iterator itEnd = end();
    JGADGET_EXITWARN(357, itEnd.p_==&oNode_);
    const_iterator it = begin();
    JGADGET_EXITWARN(359, it.p_==oNode_.pNext_);
    for (; it != itEnd; ++it, ++u) {
        JGADGET_EXITWARN(362, u<size());
        const TLinkListNode* pIt = it.p_;
        JUT_ASSERT(364, pIt!=NULL);
        JGADGET_EXITWARN(365, pIt->pNext_->pPrev_==pIt);
        JGADGET_EXITWARN(366, pIt->pPrev_->pNext_==pIt);
    }
    JGADGET_EXITWARN(368, it.p_==&oNode_);
    JGADGET_EXITWARN(369, u==size());
    return true;
}

bool JGadget::TNodeLinkList::Confirm_iterator(const_iterator it) const {
    const_iterator itBegin = begin();
    const_iterator itEnd = end();
    while (itBegin != itEnd) {
        if (itBegin == it) {
            return true;
        }
        ++itBegin;
    }
    JGADGET_EXITWARN(383, it==itEnd);
    return true;
}
