#ifndef MSL_ALGORITHM_H_
#define MSL_ALGORITHM_H_

#include <iterator.h>
#include <string.h>
#include <functional.h>

namespace std {

template <class ForwardIterator, class T, typename Predicate>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& val, Predicate p) {
	typedef typename iterator_traits<ForwardIterator>::difference_type difference_type;
	difference_type len = std::distance(first, last);

	while (len > 0) {
		ForwardIterator i = first;
		difference_type step = len / 2;
		std::advance(i, step);

		if (p(*i, val)) {
			first = ++i;
			len -= step + 1;
		} else {
			len = step;
		}
	}

	return first;
}

template <class ForwardIterator, class T>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& val) {
	// For some reason, calling the other lower_bound matches for debug, but not for retail:
	// return lower_bound(first, last, val, std::detail::less<T, T>());

	typedef typename iterator_traits<ForwardIterator>::difference_type difference_type;
	difference_type len = std::distance(first, last);

	while (len > 0) {
		ForwardIterator i = first;
		difference_type step = len / 2;
		std::advance(i, step);

		if (*i < val) {
			first = ++i;
			len -= step + 1;
		} else {
			len = step;
		}
	}

	return first;
}

template <class ForwardIterator, class T, class Predicate>
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& val, Predicate p) {
	typedef typename iterator_traits<ForwardIterator>::difference_type difference_type;
	difference_type len = std::distance(first, last);

	while (len > 0) {
		ForwardIterator i = first;
		difference_type step = len / 2;
		std::advance(i, step);

		if (!p(val, *i)) {
			first = ++i;
			len -= step + 1;
		} else {
			len = step;
		}
	}

	return first;
}

// should be inline, but breaks JStudio/JStudio/ctb weak function order
template<class InputIt, class UnaryPredicate>
InputIt find_if(InputIt first, InputIt last, UnaryPredicate p) {
	while (first != last && !p(*first)) {
		++first;
	}
	return first;
}

// fakematch: val should be a const reference, but that breaks JMessage::TResource::toMessageIndex_messageID
template<class ForwardIterator, class T>
inline ForwardIterator find(ForwardIterator first, ForwardIterator last, T& val) {
    while (first != last && !(*first == val)) {
		++first;
	}
    return first;
}

/*
template<class OutputIt, class Size, int A2>
struct __fill_n {
    OutputIt fill_n(OutputIt first, Size count, const unsigned long& value);
};

template<>
unsigned long* __fill_n<unsigned long, long, 0>::fill_n(unsigned long* first, long count, const unsigned long& value) {
    for (; count > 0; count--) {
        *first++ = value;
    }
    return first;
}

template<class OutputIt, class Size, class T>
OutputIt fill_n(OutputIt first, Size count, const T& value) {
    return __fill_n::fill_n(first, count, value);
}


template<class ForwardIt, class T>
void __fill(ForwardIt first, ForwardIt last, const T& value, std::random_access_iterator_tag param_3) {
    fill_n(first, last - first, value);
}
*/

template<class ForwardIt, class T>
inline void fill(ForwardIt first, ForwardIt last, const T& value) {
    for (; first != last; ++first){
        *first = value;
    }
}

template<class InputIt, class OutputIt>
inline OutputIt copy(InputIt first, InputIt last,
              OutputIt d_first) {
    for (; first < last; ++first, ++d_first) {
        *d_first = *first;
    }
    return d_first;
}

template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
	while (last != first)
		*--result = *--last;
	return result;
}

template <class T, bool A>
struct __copy_backward
{
	static T* copy_backward(T* first, T* last, T* result)
	{
		while (last > first)
			*--result = *--last;
		return result;
	}
};

template <class T>
struct __copy_backward<T, true>
{
	static T* copy_backward(T* first, T* last, T* result)
	{
#if DEBUG
		size_t n = static_cast<size_t>(last - first);
		result -= n;
		memmove(result, first, n*sizeof(T));
		return result;
#else
        while (last > first)
			*--result = *--last;
		return result;
#endif
	}
};

template <class T>
inline T* copy_backward(T* first, T* last, T* result) {
	return __copy_backward<T, true>::copy_backward(first, last, result);
}

}  // namespace std

#endif
