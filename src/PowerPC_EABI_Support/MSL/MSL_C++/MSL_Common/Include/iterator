#ifndef MSL_ITERATOR_H_
#define MSL_ITERATOR_H_

#include <stddef>

namespace std {
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class Iterator>
struct iterator_traits {
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::iterator_category iterator_category;
};

template <class T>
struct iterator_traits<T*> {
    typedef ptrdiff_t difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef random_access_iterator_tag iterator_category;
};

template<
    class Category,
    class T,
    class Distance,
    class Pointer,
    class Reference
> struct iterator {
    typedef Distance difference_type;
    typedef T value_type;
    typedef Pointer pointer;
    typedef Reference reference;
    typedef Category iterator_category;
};

template <class Iterator>
struct reverse_iterator : public iterator<typename iterator_traits<Iterator>::iterator_category,
                                          typename iterator_traits<Iterator>::value_type,
                                          typename iterator_traits<Iterator>::difference_type,
                                          typename iterator_traits<Iterator>::pointer,
                                          typename iterator_traits<Iterator>::reference> {
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef typename iterator_traits<Iterator>::value_type value_type;
    typedef typename iterator_traits<Iterator>::pointer pointer;
    typedef typename iterator_traits<Iterator>::reference reference;
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;

    reverse_iterator() {}
    reverse_iterator(Iterator it) : current(it) {}
    reverse_iterator operator++(int) { const reverse_iterator old(*this); --current; return old; }
    reference operator*() {
        tmp = current;
        return *--tmp;
    }

    Iterator base() const { return current; }

    Iterator tmp;
    Iterator current;
};

template <class Iterator1, class Iterator2>
bool operator!= (const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
    return (lhs.base() != rhs.base()) != false;
}

template <class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
    for (; n > 0; --n)
        ++i;
}

template <class BidirectionalIterator, class Distance>
inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
    if (n >= 0)
        for (; n > 0; --n)
            ++i;
    else
        for (; n < 0; ++n)
            --i;
}

template <class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
    i += n;
}

template <class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) {
    __advance(i, n, typename iterator_traits<InputIterator>::iterator_category());
}

// TODO: combine this with above later
template <class InputIt, class Distance>
inline void advance_fake(InputIt& it, Distance n) {
    while (n > 0) {
        --n;
        ++it;
    }
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type result = 0;
    for (; first != last; ++first)
        ++result;
    return result;
}

template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
    return last - first;
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator first,
                                                                         InputIterator last) {
    return __distance(first, last, typename iterator_traits<InputIterator>::iterator_category());
}

// This needs to be defined with gcc concepts or something similar. Workaround.
template <class InputIt, class Distance>
inline void advance_pointer(InputIt& it, Distance n) {
    it += n;
}

}  // namespace std

#endif
