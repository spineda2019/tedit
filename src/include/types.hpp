#ifndef SRC_INCLUDE_TYPES_HPP_
#define SRC_INCLUDE_TYPES_HPP_

#include "./macro_wrappers.hpp"
#include "./meta.hpp"

namespace tedit::types {

using size_t = tedit::meta::size_type::type;

using file_handle_t = typename tedit::meta::FileHandle<
    tedit::meta::platform_filehandle_type>::type;
static_assert(sizeof(file_handle_t) > 1, "sizeof File Handle type must be > 1");

namespace detail {
// TODO(SEP): can't put in meta due to recursive include

/// TODO(SEP): Docs
template <tedit::meta::fs::FileHandleType T>
    requires(T == tedit::meta::fs::FileHandleType::FileDescriptor)
file_handle_t GetSpecialFileHandleHelper(
    tedit::meta::fs::SpecialFile sf) noexcept {
    switch (sf) {
        case meta::fs::SpecialFile::StdIn:
            return 0;
            break;
        case meta::fs::SpecialFile::StdOut:
            return 1;
            break;
        case meta::fs::SpecialFile::StdErr:
            return 2;
            break;
    }
}

template <tedit::meta::fs::FileHandleType T>
    requires(T == tedit::meta::fs::FileHandleType::Handle)
file_handle_t GetSpecialFileHandleHelper(
    tedit::meta::fs::SpecialFile) noexcept {
    static_assert(false, "TODO: Windows not yet supported");
}
}  // namespace detail

// defined in TU
file_handle_t GetSpecialFileHandle(tedit::meta::fs::SpecialFile) noexcept;

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
}  // namespace tedit::types

#endif  // SRC_INCLUDE_TYPES_HPP_
