#ifndef JGADGET_SEARCH_H
#define JGADGET_SEARCH_H

#include <dolphin/types.h>
#include <iterator>
#include <functional>
#include <algorithm>

namespace JGadget {
namespace search {
template <typename T>
struct TExpandStride_ {};

template <>
struct TExpandStride_<s32> {
    static s32 get(s32 n) { return n << 3; }
};

struct TPR1IsEqual_string_ {
    TPR1IsEqual_string_(const char* sz) {
        string_ = sz;
    }

    bool operator()(const char* sz) const {
        bool ret;
        if (string_ == NULL) {
            ret = sz == NULL;
        } else {
            ret = sz != NULL && strcmp(string_, sz) == 0;
        }
        return ret;
    }

    const char* string_;
};

}  // namespace search

const char* toStringFromIndex(int index, const char* const* pValue, u32 count, const char* fallback);
int toIndexFromString_linear(const char*, const char* const*, u32, int);

template <typename T>
inline const T& toValueFromIndex(int idx, const T* pValue, u32 count, const T& fallback) {
    JUT_ASSERT(200, pValue!=NULL);
    u32 index = idx;
    if (index >= count) {
        return fallback;
    } else {
        return pValue[index];
    }
}

template <typename T, typename Predicate>
inline int toIndexFromValue_linear_if(Predicate p, const T* pValue, u32 count, int fallback) {
    JUT_ASSERT(212, pValue!=NULL);
    
    const T* first = pValue;
    const T* last = pValue + count;
    const T* found = std::find_if(first, last, p);

    if (found == last) {
        return fallback;
    }

    return std::distance(first, found);
}

template <typename Category, typename T, typename Distance, typename Pointer, typename Reference>
struct TIterator : public std::iterator<Category, T, Distance, Pointer, Reference> {
};

template <typename Iterator>
struct TIterator_reverse : public std::reverse_iterator<Iterator> {
    TIterator_reverse() {}
    TIterator_reverse(Iterator it) : std::reverse_iterator<Iterator>(it) {}
};

template <typename Iterator, typename T, typename Predicate>
inline Iterator findUpperBound_binary_all(Iterator first, Iterator last, const T& val, Predicate p) {
    return std::upper_bound(first, last, val, p);
}

template <typename Iterator, typename T, typename Predicate>
inline Iterator findUpperBound_binary_begin(Iterator first, Iterator last, const T& val, Predicate p) {
    if (first == last) {
        return last;
    }

    typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
    difference_type dist = std::distance(first, last);
    difference_type stride = 1;
    search::TExpandStride_<difference_type> expand;
    Iterator i = first;

    while (true) {
        if (p(val, *i)) {
            if (stride == 1) {
                return i;
            } else {
                break;
            }
        }
        first = i;
        dist -= stride;
        if (dist <= 0) {
            i = last;
            break;
        }
        i += stride;
        stride = expand.get(stride);
    }

    return findUpperBound_binary_all(first, i, val, p);
}

template <typename Iterator, typename T, typename Predicate>
inline Iterator findUpperBound_binary_end(Iterator first, Iterator last, const T& val, Predicate p) {
    if (first == last) {
        return last;
    }

    typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
    --last;
    difference_type dist = std::distance(first, last);
    difference_type stride = 1;
    search::TExpandStride_<difference_type> expand;
    Iterator i = last;

    while (true) {
        if (!p(val, *i)) {
            if (stride == 1) {
                return ++i;
            } else {
                break;
            }
        }
        last = i;
        dist -= stride;
        if (dist <= 0) {
            i = first;
            break;
        }
        i -= stride;
        stride = expand.get(stride);
    }

    return findUpperBound_binary_all(i, ++last, val, p);
}

template <typename Iterator, typename T, typename Predicate>
inline Iterator findUpperBound_binary_current(Iterator first, Iterator last, Iterator current, const T& val, Predicate p) {
    return current == last || p(val, *current) ?
        findUpperBound_binary_end(first, current, val, p) :
        findUpperBound_binary_begin(current, last, val, p);
}

template <typename Iterator, typename T>
inline Iterator findUpperBound_binary_current(Iterator first, Iterator last, Iterator current, const T& val) {
    return findUpperBound_binary_current(first, last, current, val, std::less<T>());
}

}  // namespace JGadget

#endif /* JGADGET_SEARCH_H */
