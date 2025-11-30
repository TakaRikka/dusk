#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JSupport/JSUList.h"

JSUPtrLink::JSUPtrLink(void* object) {
    mList = NULL;
    mObject = object;
    mPrev = NULL;
    mNext = NULL;
}

JSUPtrLink::~JSUPtrLink() {
    if (mList != NULL) {
        mList->remove(this);
    }
}

JSUPtrList::JSUPtrList(bool init) {
    if (init) {
        initiate();
    }
}

JSUPtrList::~JSUPtrList() {
    JSUPtrLink* node = mHead;
    s32 removed = 0;
    while (mLength > removed) {
        node->mList = NULL;
        node = node->getNext();
        removed += 1;
    }
}

void JSUPtrList::initiate() {
    mHead = NULL;
    mTail = NULL;
    mLength = 0;
}

void JSUPtrList::setFirst(JSUPtrLink* first) {
    first->mList = this;
    first->mPrev = NULL;
    first->mNext = NULL;
    mTail = first;
    mHead = first;
    mLength = 1;
}

bool JSUPtrList::append(JSUPtrLink* ptr) {
    JSUPtrList* list = ptr->mList;
    bool result = (NULL == list);
    if (!result) {
        result = list->remove(ptr);
    }

    if (result) {
        if (mLength == 0) {
            setFirst(ptr);
        } else {
            ptr->mList = this;
            ptr->mPrev = mTail;
            ptr->mNext = NULL;
            mTail->mNext = ptr;
            mTail = ptr;
            mLength++;
        }
    }

    return result;
}

bool JSUPtrList::prepend(JSUPtrLink* ptr) {
    JSUPtrList* list = ptr->mList;
    bool result = (NULL == list);
    if (!result) {
        result = list->remove(ptr);
    }

    if (result) {
        if (mLength == 0) {
            setFirst(ptr);
        } else {
            ptr->mList = this;
            ptr->mPrev = NULL;
            ptr->mNext = mHead;
            mHead->mPrev = ptr;
            mHead = ptr;
            mLength++;
        }
    }

    return result;
}

bool JSUPtrList::insert(JSUPtrLink* before, JSUPtrLink* ptr) {
    if (before == mHead) {
        return prepend(ptr);
    } else if (before == NULL) {
        return append(ptr);
    }

    if (before->mList != this) {
        return false;
    }

    bool result = (NULL == ptr->mList);
    if (!result) {
        result = ptr->mList->remove(ptr);
    }

    if (result) {
        JSUPtrLink* prev = before->mPrev;
        ptr->mList = this;
        ptr->mPrev = prev;
        ptr->mNext = before;
        prev->mNext = ptr;
        before->mPrev = ptr;
        mLength++;
    }

    return result;
}

bool JSUPtrList::remove(JSUPtrLink* ptr) {
    bool is_parent = (ptr->mList == this);
    if (is_parent) {
        if (mLength == 1) {
            mHead = NULL;
            mTail = NULL;
        } else if (ptr == mHead) {
            ptr->mNext->mPrev = NULL;
            mHead = ptr->mNext;
        } else if (ptr == mTail) {
            ptr->mPrev->mNext = NULL;
            mTail = ptr->mPrev;
        } else {
            ptr->mPrev->mNext = ptr->mNext;
            ptr->mNext->mPrev = ptr->mPrev;
        }

        ptr->mList = NULL;
        mLength--;
    }

    return is_parent;
}

JSUPtrLink* JSUPtrList::getNthLink(u32 index) const {
    if (index >= mLength) {
        return NULL;
    }

    JSUPtrLink* node = mHead;
    for (u32 i = 0; i < index; i++) {
        node = node->getNext();
    }

    return node;
}
