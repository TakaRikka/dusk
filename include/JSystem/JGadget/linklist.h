#ifndef LINKLIST_H
#define LINKLIST_H

#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JGadget/define.h"
#include "JSystem/JGadget/search.h"
#include <iterator>


namespace JGadget {
struct TLinkListNode {
    TLinkListNode() {
        mNext = NULL;
        mPrev = NULL;
    }

    ~TLinkListNode() {}

    TLinkListNode* getNext() const { return mNext; }
    TLinkListNode* getPrev() const { return mPrev; }

public:
    /* 0x0 */ TLinkListNode* mNext;
    /* 0x4 */ TLinkListNode* mPrev;
};  // Size: 0x8

struct TNodeLinkList {
    struct iterator {
        iterator() { node = NULL; }
        explicit iterator(TLinkListNode* pNode) { node = pNode; }
        iterator& operator=(const iterator& other) { node = other.node; return *this; }

        iterator& operator++() { node = node->getNext(); return *this; }
        iterator& operator--() { node = node->getPrev(); return *this; }
        iterator operator++(int) { const iterator old(*this); (void)++*this; return old; }
        iterator operator--(int) { const iterator old(*this); (void)--*this; return old; }
        friend bool operator==(iterator a, iterator b) { return a.node == b.node; }
        friend bool operator!=(iterator a, iterator b) { return !(a == b); }

        TLinkListNode* operator->() const { return node; }
        TLinkListNode& operator*() const { return *node; }

    public:
        /* 0x00 */ TLinkListNode* node;
    };

    struct const_iterator {
        explicit const_iterator(TLinkListNode* pNode) { node = pNode; }
        explicit const_iterator(iterator it) { node = it.node; }

        const_iterator& operator++() { node = node->getNext(); return *this; }
        const_iterator& operator--() { node = node->getPrev(); return *this; }
        const_iterator operator++(int) { const const_iterator old(*this); (void)++*this; return old; }
        const_iterator operator--(int) { const const_iterator old(*this); (void)--*this; return old; }
        friend bool operator==(const_iterator a, const_iterator b) { return a.node == b.node; }
        friend bool operator!=(const_iterator a, const_iterator b) { return !(a == b); }

        friend bool operator==(const_iterator a, iterator b) { return a.node == b.node; }
        friend bool operator!=(const_iterator a, iterator b) { return !(a == b); }

        const TLinkListNode* operator->() const { return node; }
        const TLinkListNode& operator*() const { return *node; }

    public:
        /* 0x00 */ TLinkListNode* node;
    };

    TNodeLinkList() : ocObject_() { Initialize_(); }
    ~TNodeLinkList();

    void Initialize_() {
        count = 0;
        ocObject_.mNext = &ocObject_;
        ocObject_.mPrev = &ocObject_;
    }

    iterator begin() { return iterator(ocObject_.getNext()); }
    const_iterator begin() const { return const_iterator(ocObject_.getNext()); }
    iterator end() { return iterator(&ocObject_); }
    const_iterator end() const { return const_iterator((TLinkListNode*)(&ocObject_)); }
    u32 size() const { return count; }
    bool empty() const { return size() == 0; }
    iterator pop_front() { return erase(begin()); }

    iterator erase(JGadget::TNodeLinkList::iterator, JGadget::TNodeLinkList::iterator);
    iterator erase(JGadget::TNodeLinkList::iterator);
    void splice(JGadget::TNodeLinkList::iterator, JGadget::TNodeLinkList&,
                JGadget::TNodeLinkList::iterator);
    iterator Find(const JGadget::TLinkListNode*);
    iterator Insert(JGadget::TNodeLinkList::iterator, JGadget::TLinkListNode*);
    iterator Erase(JGadget::TLinkListNode*);
    void Remove(JGadget::TLinkListNode*);

    bool Iterator_isEnd_(const_iterator it) const { return it.node == &ocObject_; }
    template <typename Predicate>
    void Remove_if(Predicate predicate, TNodeLinkList& tList) {
        iterator it = begin();

        while (!Iterator_isEnd_(const_iterator(it))) {
            if (predicate(*it)) {
                iterator itPrev = it;
                ++it;
                tList.splice(tList.end(), *this, itPrev);
            } else {
                ++it;
            }
        }
    }

    template <typename Predicate>
    void remove_if(Predicate predicate) {
        TNodeLinkList list;
        Remove_if(predicate, list);
    }

public:
    /* 0x00 */ u32 count;
    /* 0x04 */ TLinkListNode ocObject_;
};  // Size: 0xC

template <typename T, int I>
struct TLinkList : TNodeLinkList {
    TLinkList() : TNodeLinkList() {}

    struct iterator : TNodeLinkList::iterator {
        iterator() {}
        explicit iterator(TNodeLinkList::iterator iter) : TNodeLinkList::iterator(iter) {}

        iterator& operator=(const iterator& rhs) {
            //TODO: Probably fakematch? Not sure what's going on here exactly
            (TIterator<std::bidirectional_iterator_tag, T, s32, T*, T&>&)*this =
                (const TIterator<std::bidirectional_iterator_tag, T, s32, T*, T&>&)rhs;
            this->node = rhs.node;
            return *this;
        }

        iterator& operator++() {
            TNodeLinkList::iterator::operator++();
            return *this;
        }
        iterator& operator--() {
            TNodeLinkList::iterator::operator--();
            return *this;
        }
        iterator operator++(int) {
            const iterator old(*this);
            ++*this;
            return old;
        }
        iterator operator--(int) {
            const iterator old(*this);
            --*this;
            return old;
        }
        friend bool operator==(iterator a, iterator b) {
            return (TNodeLinkList::iterator&)a == (TNodeLinkList::iterator&)b;
        }
        friend bool operator!=(iterator a, iterator b) { return !(a == b); }

        T* operator->() const { return Element_toValue(TNodeLinkList::iterator::operator->()); }
        T& operator*() const {
            T* p = operator->();
            JUT_ASSERT(541, p!=NULL);
            return *p;
        }

        typedef s32 difference_type;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef std::bidirectional_iterator_tag iterator_category;
    };

    struct const_iterator : TNodeLinkList::const_iterator {
        explicit const_iterator(TNodeLinkList::const_iterator iter) :
            TNodeLinkList::const_iterator(iter) {}
        explicit const_iterator(iterator iter) :
            TNodeLinkList::const_iterator((TNodeLinkList::iterator&)iter) {}

        const_iterator& operator++() {
            TNodeLinkList::const_iterator::operator++();
            return *this;
        }
        const_iterator& operator--() {
            TNodeLinkList::const_iterator::operator++();
            return *this;
        }
        const_iterator operator++(int) {
            const const_iterator old(*this);
            ++*this;
            return old;
        }
        const_iterator operator--(int) {
            const const_iterator old(*this);
            --*this;
            return old;
        }
        friend bool operator==(const_iterator a, const_iterator b) {
            return (TNodeLinkList::const_iterator&)a == (TNodeLinkList::const_iterator&)b;
        }
        friend bool operator!=(const_iterator a, const_iterator b) { return !(a == b); }

        const T* operator->() const { return Element_toValue(TNodeLinkList::const_iterator::operator->()); }
        const T& operator*() const {
            const T* p = &*operator->();
            JUT_ASSERT(0x24a, p!=NULL);
            return *p;
        }
    };

    static TLinkListNode* Element_toNode(T* p) {
        JUT_ASSERT(0x2F1, p!=NULL);
        return reinterpret_cast<TLinkListNode*>(reinterpret_cast<char*>(p) - I);
    }
    static const TLinkListNode* Element_toNode(const T* p) {
        JUT_ASSERT(0x2F6, p!=NULL);
        return reinterpret_cast<const TLinkListNode*>(reinterpret_cast<const char*>(p) - I);
    }
    static T* Element_toValue(TLinkListNode* p) {
        JUT_ASSERT(0x2FB, p!=NULL);
        return reinterpret_cast<T*>(reinterpret_cast<char*>(p) + I);
    }
    static const T* Element_toValue(const TLinkListNode* p) {
        JUT_ASSERT(0x300, p!=NULL);
        return reinterpret_cast<const T*>(reinterpret_cast<const char*>(p) + I);
    }

    iterator Insert(iterator iter, T* element) {
        return iterator(TNodeLinkList::Insert((TNodeLinkList::iterator&)iter, Element_toNode(element)));
    }
    iterator Erase(T* element) { return iterator(TNodeLinkList::Erase(Element_toNode(element))); }

    iterator begin() { return iterator(TNodeLinkList::begin()); }
    const_iterator begin() const { return const_iterator(const_cast<TLinkList*>(this)->begin()); }
    iterator end() { return iterator(TNodeLinkList::end()); }
    const_iterator end() const { return const_iterator(const_cast<TLinkList*>(this)->end()); }
    T& front() {
        JUT_ASSERT(642, !empty());
        return *begin();
    }
    T& back() { JUT_ASSERT(652, !empty()); return *--end(); }
    void pop_front() { erase(TNodeLinkList::begin()); }
    void Push_front(T* element) { Insert(begin(), element); }
    void Push_back(T* element) { Insert(end(), element); }
    iterator Find(const T* element) {
        return iterator(TNodeLinkList::Find(Element_toNode(element)));
    }
    void Remove(T* element) { TNodeLinkList::Remove(Element_toNode(element)); }

    typedef T value_type;
};

template <typename T, int I>
struct TLinkList_factory : public TLinkList<T, I> {
    inline virtual ~TLinkList_factory() = 0;
    virtual T* Do_create() = 0;
    virtual void Do_destroy(T*) = 0;

    void Clear_destroy() {
        while (!this->empty()) {
            T* item = &this->front();
            this->pop_front();
            Do_destroy(item);
        }
    }

    typename TLinkList<T, I>::iterator Erase_destroy(T* param_0) {
        typename TLinkList<T, I>::iterator spC(this->Erase(param_0));
        Do_destroy(param_0);
        return spC;
    }
};

template <typename T, int I>
TLinkList_factory<T, I>::~TLinkList_factory() {
#ifdef __MWERKS__
    JGADGET_ASSERTWARN(934, empty());
#else
    JGADGET_ASSERTWARN(934, this->empty());
#endif
}

template <typename T>
struct TEnumerator {
    inline TEnumerator(T _current, T _end)
        : current(_current), end(_end) {}

    bool isEnd() const { return current != end; }
    operator bool() const { return isEnd(); }
    T operator*() {
        T rv = current;
        ++current;
        return rv;
    }

    T current;
    T end;
};

// TEnumerator2 should be the same but there are two issues:
// 1. How to derive the iterator return type for operator* (the debug makes it seem like operator* is called
// so the return value should be what the iterator points to)
// 2. Calling the * operator seems to make functions using TEnumerator<T*> not work. See
// JStudio::TAdaptor::adaptor_setVariableValue_n
// Perhaps template specialization?
template <typename Iterator, typename T>
struct TEnumerator2 {
    inline TEnumerator2(Iterator _current, Iterator _end)
        : current(_current), end(_end) {}

    bool isEnd() const { return current != end; }
    operator bool() const { return isEnd(); }
    T& operator*() {
        T& rv = *current;
        ++current;
        return rv;
    }

    Iterator current;
    Iterator end;
};

template <typename T>
struct TContainerEnumerator : public TEnumerator2<typename T::iterator, typename T::value_type> {
    inline TContainerEnumerator(T& param_0)
        : TEnumerator2<typename T::iterator, typename T::value_type>(param_0.begin(), param_0.end()) {}
};


template <typename T, int I>
struct TContainerEnumerator_const : public TEnumerator2<typename TLinkList<T, I>::const_iterator, const T> {
    inline TContainerEnumerator_const(const TLinkList<T, I>* param_0)
        : TEnumerator2<typename TLinkList<T, I>::const_iterator, const T>(param_0->begin(), param_0->end()) {}
};

};  // namespace JGadget

#endif /* LINKLIST_H */
