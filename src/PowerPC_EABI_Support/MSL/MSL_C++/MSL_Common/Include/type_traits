#ifndef MSL_TYPE_TRAITS_H_
#define MSL_TYPE_TRAITS_H_

#include <stddef>

namespace std {
    // helper class
    template<class T, T v> struct integral_constant { static const T value = v;};

    typedef integral_constant<bool, true> true_type;
    typedef integral_constant<bool, false> false_type;

    template <bool B, typename T = void>
    struct enable_if {};

    template <typename T>
    struct enable_if<true, T> { typedef T type; };

    template<class T> struct remove_reference { typedef T type; };
    template<class T> struct remove_reference<T&> { typedef T type; };

    template<bool B, class T, class F>
    struct conditional { typedef T type; };
    
    template<class T, class F>
    struct conditional<false, T, F> { typedef F type; };

    template<class T>
    struct is_array : false_type {};
    
    template<class T>
    struct is_array<T[]> : std::true_type {};
    
    template<class T, size_t N>
    struct is_array<T[N]> : std::true_type {};

    template <typename T>
    struct add_pointer
    {
        typedef T* type;
    };
    template <typename T>
    struct add_pointer<T&>
    {
        typedef T* type;
    };
    template <typename T>
    struct add_pointer<T const&>
    {
        typedef T* type;
    };
    template <typename T>
    struct add_pointer<T volatile&>
    {
        typedef T* type;
    };
    template <typename T>
    struct add_pointer<T const volatile&>
    {
        typedef T* type;
    };

    template<class T>
    struct remove_extent { typedef T type; };
    
    template<class T>
    struct remove_extent<T[]> { typedef T type; };
    
    template<class T, size_t N>
    struct remove_extent<T[N]> { typedef T type; };

    template<class T> struct remove_cv { typedef T type; };
    template<class T> struct remove_cv<const T> { typedef T type; };
    template<class T> struct remove_cv<volatile T> { typedef T type; };
    template<class T> struct remove_cv<const volatile T> { typedef T type; };
    
    template<class T> struct remove_const { typedef T type; };
    template<class T> struct remove_const<const T> { typedef T type; };
    
    template<class T> struct remove_volatile { typedef T type; };
    template<class T> struct remove_volatile<volatile T> { typedef T type; };

    template<class T, class U>
    struct is_same : std::false_type {};
    
    template<class T>
    struct is_same<T, T> : std::true_type {};
}

#endif
