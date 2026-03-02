#ifndef MSL_FUNCTIONAL_H_
#define MSL_FUNCTIONAL_H_

namespace std {

namespace detail {

template <class T1, class T2>
struct less {
	bool operator()(const T1& lhs, const T2& rhs) const { return lhs < rhs; }
};

}  // namespace detail

template <class T>
struct less : public std::detail::less<T, T> {
    bool operator()(const T& lhs, const T& rhs) const { return lhs < rhs; }
};

}  // namespace std

#endif
