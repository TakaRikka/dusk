#ifndef JPALIST_H
#define JPALIST_H

#include <dolphin/types.h>

/**
 * @ingroup jsystem-jparticle
 * 
 */
template <class T>
struct JPANode {
    JPANode() : mpPrev(NULL), mpNext(NULL) {
    }
    ~JPANode() {}
    JPANode<T>* getPrev() const { return mpPrev; }
    JPANode<T>* getNext() const { return mpNext; }
    T* getObject() { return &mData; }

    /* 0x00 */ JPANode<T>* mpPrev;
    /* 0x04 */ JPANode<T>* mpNext;
    /* 0x08 */ T mData;
};

/**
 * @ingroup jsystem-jparticle
 * 
 */
template <class T>
struct JPAList {
    /* 0x00 */ JPANode<T>* mpFirst;
    /* 0x04 */ JPANode<T>* mpLast;
    /* 0x08 */ u32 mNum;

    JPAList() : mpFirst(NULL), mpLast(NULL), mNum() {}
    ~JPAList() {}

    JPANode<T>* getEnd() { return NULL; }
    JPANode<T>* getFirst() const { return mpFirst; }
    JPANode<T>* getLast() const { return mpLast; }
    u32 getNum() const { return mNum; }

    void push_front(JPANode<T>* node) {
        if (mpFirst != NULL) {
            node->mpPrev = NULL;
            node->mpNext = mpFirst;
            mpFirst->mpPrev = node;
            mpFirst = node;
        } else {
            mpLast = node;
            mpFirst = node;
            node->mpNext = node->mpPrev = NULL;
        }

        mNum++;
    }

    void push_back(JPANode<T>* node) {
        if (mpLast != NULL) {
            node->mpPrev = mpLast;
            node->mpNext = NULL;
            mpLast->mpNext = node;
            mpLast = node;
        } else {
            mpFirst = node;
            mpLast = node;
            node->mpNext = node->mpPrev = NULL;
        }

        mNum++;
    }

    JPANode<T>* pop_front() {
        JPANode<T>* ret = NULL;

        if (mNum == 1) {
            ret = mpFirst;
            mpFirst = mpLast = NULL;
            mNum--;
        } else if (mNum) {
            ret = mpFirst;
            ret->mpNext->mpPrev = NULL;
            mpFirst = ret->mpNext;
            mNum--;
        }

        return ret;
    }

    JPANode<T>* pop_back() {
        JPANode<T>* ret = NULL;

        if (mNum == 1) {
            ret = mpLast;
            mpFirst = mpLast = NULL;
            mNum--;
        } else if (mNum) {
            ret = mpLast;
            ret->mpPrev->mpNext = NULL;
            mpLast = ret->mpPrev;
            mNum--;
        }

        return ret;
    }

    JPANode<T>* erase(JPANode<T>* node) {
        if (node->mpNext != NULL && node->mpPrev != NULL) {
            node->mpPrev->mpNext = node->mpNext;
            node->mpNext->mpPrev = node->mpPrev;
            mNum--;
        } else if (node->mpNext != NULL) {
            node->mpNext->mpPrev = NULL;
            mpFirst = node->mpNext;
            mNum--;
        } else if (node->mpPrev != NULL) {
            node->mpPrev->mpNext = NULL;
            mpLast = node->mpPrev;
            mNum--;
        } else {
            mpFirst = mpLast = NULL;
            mNum--;
        }
        return node;
    }
};

#endif
