#ifndef MSL_MEMORY_H_
#define MSL_MEMORY_H_

namespace std {

template<class ForwardIt, class Size, class T>
inline ForwardIt uninitialized_fill_n(ForwardIt first, Size count, const T& value) {
    for (; count--; ++first) {
        if (first != NULL) {
            *first = value;
        }
    }
    return first;
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
    ForwardIterator __save = result;

    for (; first != last; ++first, ++result) {
        *result = *first;
    }
    return result;
}

template <class T, bool A, bool B>
struct __uninitialized_copy_helper {
	static T* uninitialized_copy(T* first, T* last, T* result) {
		return __uninitialized_copy(first, last, result);
	}
};

template <class T>
struct __uninitialized_copy_helper<T, true, false>
{
	static T* uninitialized_copy(T* first, T* last, T* result)
	{
		for (; first < last; ++result, ++first)
			*result = *first;
		return result;
	}
};

template <class T>
inline T* uninitialized_copy(T* first, T* last, T* result) {
	return __uninitialized_copy_helper<T, true, false>::uninitialized_copy(first, last, result);
}

}

#endif
