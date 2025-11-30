#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JGadget/linklist.h"

template <typename T>
class TPRIsEqual_pointer_ {
public:
    TPRIsEqual_pointer_<T>(const T* p) { this->p_ = p; }

    bool operator()(const T& rSrc) const { return &rSrc == this->p_; }

private:
    const T* p_;
};

JGadget::TNodeLinkList::~TNodeLinkList() {}

JGadget::TNodeLinkList::iterator
JGadget::TNodeLinkList::erase(JGadget::TNodeLinkList::iterator it) {
    iterator next = it;
    ++next;
    return erase(it, next);
}

/* 802DCAA0-802DCB08 2D73E0 0068+00 1/1 0/0 0/0 .text
 * erase__Q27JGadget13TNodeLinkListFQ37JGadget13TNodeLinkList8iteratorQ37JGadget13TNodeLinkList8iterator
 */
JGadget::TNodeLinkList::iterator JGadget::TNodeLinkList::erase(iterator a, iterator b) {
    TLinkListNode* cur = a.node;
    TLinkListNode* end = b.node;
    TLinkListNode* next;

    for (; cur != end; cur = next) {
        next = cur->mNext;
        Erase(cur);
    }

    return b;
}

/* 802DCB08-802DCBA8 2D7448 00A0+00 1/1 0/0 0/0 .text
 * splice__Q27JGadget13TNodeLinkListFQ37JGadget13TNodeLinkList8iteratorRQ27JGadget13TNodeLinkListQ37JGadget13TNodeLinkList8iterator
 */
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

JGadget::TNodeLinkList::iterator JGadget::TNodeLinkList::Insert(iterator it, TLinkListNode* p) {
    TLinkListNode* pIt = it.node;
    TLinkListNode* pItPrev = pIt->mPrev;

    p->mNext = pIt;
    p->mPrev = pItPrev;
    pIt->mPrev = p;
    pItPrev->mNext = p;
    count++;
    return iterator(p);
}

JGadget::TNodeLinkList::iterator JGadget::TNodeLinkList::Erase(TLinkListNode* p) {
    TLinkListNode* pNext = p->mNext;
    TLinkListNode* pPrev = p->mPrev;
    pNext->mPrev = pPrev;
    pPrev->mNext = pNext;
    count--;
    return iterator(pNext);
}

// NONMATCHING - missing stack
void JGadget::TNodeLinkList::Remove(TLinkListNode* p) {
    remove_if(TPRIsEqual_pointer_<TLinkListNode>(p));
}
