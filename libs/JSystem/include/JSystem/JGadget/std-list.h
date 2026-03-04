#ifndef STD_LIST_H
#define STD_LIST_H

#include "JSystem/JGadget/define.h"
#include "JSystem/JGadget/search.h"
#include "JSystem/JGadget/std-memory.h"

namespace JGadget {
    template <typename T, typename Allocator = JGadget::TAllocator<T> >
    struct TList {
        struct TNode_ {
            /* 0x00 */ TNode_* pNext_;
            /* 0x04 */ TNode_* pPrev_;
        };

        struct iterator : public TIterator<std::bidirectional_iterator_tag, T, s32, T*, T&> {
            iterator() {p_ = NULL; }
            iterator(TNode_* pNode) { p_ = pNode; }

            iterator& operator++() { p_ = p_->pNext_; return *this; }
            iterator& operator--() { p_ = p_->pPrev_; return *this; }
            iterator operator++(int) { const iterator old(*this); (void)++*this; return old; }
            iterator operator--(int) { const iterator old(*this); (void)--*this; return old; }
            friend bool operator==(iterator a, iterator b) { return a.p_ == b.p_; }
            friend bool operator!=(iterator a, iterator b) { return !(a == b); }
            T& operator*() const {
                JUT_ASSERT(125, p_!=NULL);
                return *(T*)&p_[1];
            }

            /* 0x00 */ TNode_* p_;
        };

        struct const_iterator {
            const_iterator(TNode_* pNode) { p_ = pNode; }

            const_iterator& operator++() { p_ = p_->pNext_; return *this; }
            const_iterator& operator--() { p_ = p_->pPrev_; return *this; }
            friend bool operator==(const_iterator a, const_iterator b) { return a.p_ == b.p_; }
            friend bool operator!=(const_iterator a, const_iterator b) { return !(a == b); }

            /* 0x00 */ TNode_* p_;
        };

        TList(const Allocator& allocator = Allocator()) {
            field_0x4 = 0;
            Initialize_();
        }
        ~TList() {
            int r30;
            int r29;
            JGadget_outMessage* r28;
            int r27;
            Confirm();
            clear();
            r30 = 0;
            r29 = 1;
            if (empty()) {
                return;
            }

            JGadget_outMessage out(JGadget_outMessage::warning, __FILE__, 229);
            r30 = 1;
            r28 = &out;
            *r28 << "empty()";
            r27 = 0;
            if (!r27) {
                r29 = 0;
            }
        }

        iterator push_front(const T& element) { return insert(begin(), element); }
        iterator push_back(const T& element) { return insert(end(), element); }
        iterator insert(iterator it, const T& element) {
            TNode_* p = it.p_;
            JUT_ASSERT(286, p!=NULL);
            TNode_* prev = p->pPrev_;
            TNode_* node = CreateNode_(p, prev, element);
            if (!node) {
                return end();
            }
            p->pPrev_ = node;
            prev->pNext_ = node;
            field_0x4++;
            return iterator(node);
        }

        iterator erase(iterator start, iterator end) {
            while (start != end) {
                start = erase(start);
            }
            return start;
        }

        iterator erase(iterator it) {
            JUT_ASSERT(314, !empty());
            TNode_* p = it.p_;
            JUT_ASSERT(316, p!=NULL);
            TNode_* next = p->pNext_;
            p->pPrev_->pNext_ = next;
            next->pPrev_ = p->pPrev_;
            DestroyNode_(p);
            field_0x4--;
            return iterator(next);
        }

        void clear() { erase(begin(), end()); }
        BOOL empty() const { return size() == 0; }
        int size() const { return field_0x4; }
        iterator begin() { return iterator(oEnd_.pNext_); }
        iterator end() { return iterator(&oEnd_); }
        const_iterator begin() const { return const_iterator(oEnd_.pNext_); }
        const_iterator end() const { return const_iterator((TNode_*)&oEnd_); }
        TIterator_reverse<iterator> rbegin() { return TIterator_reverse<iterator>(begin()); }
        TIterator_reverse<iterator> rend() { return TIterator_reverse<iterator>(end()); }

        void Initialize_() {
            oEnd_.pNext_ = &oEnd_;
            oEnd_.pPrev_ = &oEnd_;
        }

        BOOL Confirm() const {
            u32 r28 = 0;
            const_iterator local_24 = end();
            if (local_24.p_ != &oEnd_) {
                return FALSE;
            }
            const_iterator local_28 = begin();
            if (local_28.p_ != oEnd_.pNext_) {
                return FALSE;
            }
            for (; local_28 != local_24; ++local_28) {
                TNode_* node = local_28.p_;
                if (node->pNext_->pPrev_ != node) {
                    return FALSE;
                }
                if (node->pPrev_->pNext_ != node) {
                    return FALSE;
                }
                r28++;
            }
            if (local_28.p_ != &oEnd_) {
                return FALSE;
            }
            if (r28 != size()) {
                return FALSE;
            }
            return TRUE;
        }

        TNode_* CreateNode_(TNode_ *pNext, TNode_ *pPrev, const T& pObject) {
            JUT_ASSERT(483, pNext!=NULL);
            JUT_ASSERT(484, pPrev!=NULL);
            TNode_* node = (TNode_*)mAllocator.AllocateRaw(sizeof(TNode_) + sizeof(T));
            if (!node) {
                JGADGET_WARNMSG(489, "can\'t allocate memory");
                return NULL;
            }
            node->pNext_ = pNext;
            node->pPrev_ = pPrev;
            mAllocator.construct((T*)&node[1], pObject);
            return node;
        }

        void DestroyNode_(TNode_* p) {
            JUT_ASSERT(501, p!=NULL);
            JUT_ASSERT(502, p!=&oEnd_);
            JUT_ASSERT(503, p->pNext_->pPrev_!=p);
            JUT_ASSERT(504, p->pPrev_->pNext_!=p);
            mAllocator.destroy((T*)&p[1]);
            mAllocator.DeallocateRaw((T*)p);
        }

        /* 0x00 */ Allocator mAllocator;
        /* 0x04 */ int field_0x4;
        /* 0x08 */ TNode_ oEnd_;
    };
}

#endif /* STD_LIST_H */
