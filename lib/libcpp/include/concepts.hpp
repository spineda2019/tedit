#ifndef LIB_LIBCPP_INCLUDE_CONCEPTS_HPP_
#define LIB_LIBCPP_INCLUDE_CONCEPTS_HPP_

template <class T>
concept Primitive =
    meta::cmp::OneOf<typename meta::reference::remove_reference_t<T>,
                     bool,           //
                     signed char,    //
                     unsigned char,  //
                     signed short,   //
                     signed short,   //
                     signed int,     //
                     unsigned int,   //
                     signed long,    //
                     unsigned long>::value;

namespace cmp {
template <class T>
concept PartialEq = Primitive<T> || requires(T a, T b) {
    { a == b } -> meta::cmp::same_type<bool>;
    { a != b } -> meta::cmp::same_type<bool>;
};

template <class T>
concept Eq = Primitive<T> || (PartialEq<T> && requires(T a, T b, T c) {
                 // The following laws can't be checked by the compiler. The
                 // type constraints don't provide much that PartialEq doesn't
                 // already
                 { a == b } -> meta::cmp::same_type<decltype(b == a)>;
                 { a != b } -> meta::cmp::same_type<decltype(b != a)>;
             });

enum class Ordering {
    Less,
    Equal,
    Greater,
};

template <class T>
concept PartialOrd =
    Primitive<T> || (PartialEq<T> && requires(T a, T b) {
        a.PartialCmp(b);  // TODO(SEP): returns an optional Ordering

        { a < b } -> meta::cmp::same_type<bool>;
        { a <= b } -> meta::cmp::same_type<bool>;
        { a > b } -> meta::cmp::same_type<bool>;
        { a >= b } -> meta::cmp::same_type<bool>;
    });

template <class T>
concept Ord =
    Primitive<T> || (PartialOrd<T> && Eq<T> && requires(T a, T b, T c) {
        { a.Cmp(b) } -> meta::cmp::same_type<Ordering>;
        { a.Max(b) } -> meta::cmp::same_type<T>;
        { a.Min(b) } -> meta::cmp::same_type<T>;
        { a.Clamp(b, c) } -> meta::cmp::same_type<T>;
    });

static_assert(Ord<int>);

}  // namespace cmp

template <class T>
concept Semigroup = requires(T a, T b) {
    { a + b } -> meta::cmp::same_type<T>;
};

template <class T>
concept Monoid = Semigroup<T> && requires() {
    { (T::mempty()) } -> meta::cmp::same_type<T>;
};

namespace values {
inline consteval size_t GetPageSize() {
    switch (meta::target_platform) {
        case meta::platform::Os::Linux:
        case meta::platform::Os::Windows:
            return 4096;
        case meta::platform::Os::Mac:
            return 16384;
    }
}

}  // namespace values
namespace io {
template <class T>
concept Iterable = requires(T iter) {
    typename T::value_type;
    typename T::difference_type;
    typename T::reference;
    typename T::pointer;

    { iter.Start() } -> meta::cmp::same_type<typename T::value_type*>;
};

template <class T>
concept Buffer = requires(T buf, types::size_t i) {
    { buf[i] } -> meta::cmp::same_type<typename T::value_type&>;
    { buf.len() } -> meta::cmp::same_type<types::size_t>;
    { buf.c_ptr() } -> meta::cmp::same_type<typename T::value_type*>;
};

template <class T>
concept StackBuffer = Buffer<T> && (T::Size() == types::values::GetPageSize());

template <class SB>
concept FileStackBuffer =
    StackBuffer<SB> &&
    meta::cmp::same_type<typename SB::value_type, unsigned char>;

}  // namespace io

#endif  // LIB_LIBCPP_INCLUDE_CONCEPTS_HPP_
