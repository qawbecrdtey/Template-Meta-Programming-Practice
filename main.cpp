#include <iostream>

template<bool b>
struct boolean_type { enum __value : bool { value = b }; };
struct true_type : boolean_type<true> {};
struct false_type : boolean_type<false> {};

template<int n>
struct Int32 { enum __value__ : int { value = n }; };

template<typename T, typename U>
struct Add { enum __value__ : int { value = T::value + U::value }; };

template<typename T, typename U>
struct is_same : false_type {};

template<typename T>
struct is_same<T, T> : true_type {};

template<typename T>
struct remove_const { using type = T; };

template<typename T>
struct remove_const<T const> { using type = T; };

template<typename T>
struct remove_volatile { using type = T; };

template<typename T>
struct remove_volatile<volatile T> { using type = T; };

template<typename T>
struct remove_cv { using type = typename remove_volatile<typename remove_const<T>::type>::type; };

template<typename T>
struct is_pointer_base : false_type {};

template<typename T>
struct is_pointer_base<T *> : true_type {};

template<typename T>
struct is_pointer : is_pointer_base<typename remove_cv<T>::type> {};

int main() {
    using one = Int32<1>;
    using two = Int32<2>;
    std::cout << Add<one, two>::value << std::endl;
    std::cout << (is_same<one, two>::value ? "true" : "false") << std::endl;
    std::cout << (is_same<one, one>::value ? "true" : "false") << std::endl;
    std::cout << (is_pointer<const volatile int *>::value ? "true" : "false") << std::endl;
}