#ifndef LIB_LIBCPP_INCLUDE_CONCEPTS_HPP_
#define LIB_LIBCPP_INCLUDE_CONCEPTS_HPP_

#include <libcpp/meta/type_traits.hpp>
#include <libcpp/meta/types.hpp>
#include <libzig/libzig.hpp>
#include <libzig/meta/comptime_values.hpp>
#include <libzig/meta/type_traits.hpp>

namespace libcpp::meta::concepts {

template <class T>
concept Integral = types::numeric::IsIntegral<T>::value;

template <class T>
concept Floating = types::numeric::IsFloating<T>::value;

template <class T>
concept Arithmetic = types::numeric::IsArithmetic<T>::value;

template <class T>
concept Primitive =
    types::numeric::IsSigned<T>::value ||  // a "signed" non-primitive should be
                                           // impossible
    libzig::meta::cmp::OneOf<
        typename libcpp::meta::types::reference::remove_reference_t<T>,
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
    { a == b } -> libzig::meta::cmp::same_type<bool>;
    { a != b } -> libzig::meta::cmp::same_type<bool>;
};

template <class T>
concept Eq = Primitive<T> || (PartialEq<T> && requires(T a, T b, T c) {
                 // The following laws can't be checked by the compiler. The
                 // type constraints don't provide much that PartialEq doesn't
                 // already
                 { a == b } -> libzig::meta::cmp::same_type<decltype(b == a)>;
                 { a != b } -> libzig::meta::cmp::same_type<decltype(b != a)>;
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

        { a < b } -> libzig::meta::cmp::same_type<bool>;
        { a <= b } -> libzig::meta::cmp::same_type<bool>;
        { a > b } -> libzig::meta::cmp::same_type<bool>;
        { a >= b } -> libzig::meta::cmp::same_type<bool>;
    });

template <class T>
concept Ord =
    Primitive<T> || (PartialOrd<T> && Eq<T> && requires(T a, T b, T c) {
        { a.Cmp(b) } -> libzig::meta::cmp::same_type<Ordering>;
        { a.Max(b) } -> libzig::meta::cmp::same_type<T>;
        { a.Min(b) } -> libzig::meta::cmp::same_type<T>;
        { a.Clamp(b, c) } -> libzig::meta::cmp::same_type<T>;
    });

static_assert(Ord<int>);

}  // namespace cmp

template <class T>
concept Semigroup = requires(T a, T b) {
    { a + b } -> libzig::meta::cmp::same_type<T>;
};

template <class T>
concept Monoid = Semigroup<T> && requires() {
    { (T::mempty()) } -> libzig::meta::cmp::same_type<T>;
};

namespace values {
inline consteval libzig::size_t GetPageSize() {
    switch (libzig::meta::comptime_values::target_platform) {
        case libzig::meta::tags::platform::Os::Linux:
        case libzig::meta::tags::platform::Os::Windows:
            return 4096;
        case libzig::meta::tags::platform::Os::Mac:
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

    { iter.Start() } -> libzig::meta::cmp::same_type<typename T::value_type*>;
};

template <class T>
concept Buffer = requires(T buf, libzig::size_t i) {
    { buf[i] } -> libzig::meta::cmp::same_type<typename T::value_type&>;
    { buf.len() } -> libzig::meta::cmp::same_type<libzig::size_t>;
    { buf.c_ptr() } -> libzig::meta::cmp::same_type<typename T::value_type*>;
};

template <class T>
concept StackBuffer = Buffer<T> && (T::Size() == values::GetPageSize());

template <class SB>
concept FileStackBuffer =
    StackBuffer<SB> &&
    libzig::meta::cmp::same_type<typename SB::value_type, unsigned char>;

}  // namespace io
}  // namespace libcpp::meta::concepts

#endif  // LIB_LIBCPP_INCLUDE_CONCEPTS_HPP_
