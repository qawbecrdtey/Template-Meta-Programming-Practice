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

template<char c, char ...chars>
struct char_list {
    enum : char {
        now = c,
        then = char_list<chars...>::now
    };

    enum : std::size_t {
        size = sizeof...(chars) + 1
    };

    using next = char_list<chars...>;

    template<char ...extra_chars>
    using concatenate = char_list<chars..., extra_chars...>;
};

template<char c>
struct char_list<c> {
    enum : char {
        now = c,
        then = char_list<'\0'>::now
    };

    enum : std::size_t { size = 1 };

    using next = char_list<'\0'>;

    template<char ...extra_chars>
    using concatenate = char_list<c, extra_chars...>;
};

template<>
struct char_list<'\0'> {
    enum : char { now = '\0' };
    enum : char { size = 0 };

    template<char ...extra_chars>
    using concatenate = char_list<extra_chars...>;
};

int main() {
    using one = Int32<1>;
    using two = Int32<2>;
    std::cout << Add<one, two>::value << std::endl;
    std::cout << (is_same<one, two>::value ? "true" : "false") << std::endl;
    std::cout << (is_same<one, one>::value ? "true" : "false") << std::endl;
    std::cout << (is_pointer<const volatile int *>::value ? "true" : "false") << std::endl;
    using hello = char_list<'H', 'e', 'l', 'l', 'o'>;
    using space = char_list<' '>;
    using world = char_list<'w', 'o', 'r', 'l', 'd'>;
    std::cout << static_cast<char>(hello::now) << static_cast<char>(hello::next::now) << std::endl;
    std::cout << static_cast<char>(hello::concatenate<' ', 'w', 'o', 'r', 'l', 'd'>::next::next::next::next::next::next::next::next::next::now) << std::endl;
}