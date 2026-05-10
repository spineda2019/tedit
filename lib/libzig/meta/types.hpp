#ifndef LIB_LIBZIG_META_TYPES_HPP_
#define LIB_LIBZIG_META_TYPES_HPP_

namespace libzig::meta::tags {
namespace platform {
enum class Os : unsigned char {
    Linux,
    Mac,
    Windows,
};
}  // namespace platform
namespace fs {
enum class FileHandleTag : unsigned char {
    FileDescriptor,  // All of posix
    Handle,          // Just Windows TBH
};

enum class SpecialFile : unsigned char {
    StdOut,
    StdIn,
    StdErr,
};
}  // namespace fs
}  // namespace libzig::meta::tags

namespace libzig::meta::types {

/// \brief core types that should be exposed in libzig.hpp
namespace core {
struct SizeT final {
    using type = decltype(sizeof(unsigned char));
    static_assert(sizeof(type) >= 2,
                  "C++11 and above mandates size_t is not less than 16 bits");
};
}  // namespace core

namespace fs {
template <tags::fs::FileHandleTag T>
struct FileHandle final {
    static_assert(false, "Could not determine platform file handle type");
};

template <>
struct FileHandle<tags::fs::FileHandleTag::FileDescriptor> final {
    using type = int;
};

template <>
struct FileHandle<tags::fs::FileHandleTag::Handle> final {
    /// Technically not "guaranteed" by MSFT, but HANDLE has literally always
    /// been void* after 16 bit DOS.
    using type = void*;
};
}  // namespace fs

namespace reference {
template <class T>
struct RemoveReference final {
    using type = T;
};

template <class T>
struct RemoveReference<T&> final {
    using type = T;
};

template <class T>
struct RemoveReference<T&&> final {
    using type = T;
};

template <class T>
using remove_reference_t = typename RemoveReference<T>::type;

template <class T>
constexpr T&& forward(remove_reference_t<T>& any) noexcept {
    return static_cast<T&&>(any);
}

template <class T>
constexpr T&& forward(remove_reference_t<T>&& any) noexcept {
    return static_cast<T&&>(any);
}

template <class T>
constexpr remove_reference_t<T>&& move(T&& t) noexcept {
    return static_cast<remove_reference_t<T>&&>(t);
}
}  // namespace reference

namespace cmp {

template <bool Condition, class T, class F>
struct If final {
    static_assert(false);
};

template <class T, class F>
struct If<true, T, F> final {
    using type = T;
};

template <class T, class F>
struct If<false, T, F> final {
    using type = F;
};

}  // namespace cmp

namespace signedness {
// TODO(SEP): IsSigned meta functions
}  // namespace signedness

}  // namespace libzig::meta::types

#endif  // LIB_LIBZIG_META_TYPES_HPP_
