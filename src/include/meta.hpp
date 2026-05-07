/// Copyright 2026 Sebastian Pineda
/// \file meta.hpp
/// Template metaprogramming
///
/// This file should be in charge of defining types computed at compile-time

#ifndef SRC_INCLUDE_META_HPP_
#define SRC_INCLUDE_META_HPP_

namespace tedit::meta {

struct size_type final {
    using type = decltype(sizeof(unsigned char));
    static_assert(sizeof(type) >= 2,
                  "C++11 and above mandates size_t is not less than 16 bits");
};

namespace fs {
enum class FileHandleType : unsigned char {
    FileDescriptor,  // All of posix
    Handle,          // Just Windows TBH
};

enum class SpecialFile : unsigned char {
    StdOut,
    StdIn,
    StdErr,
};
}  // namespace fs

namespace platform {
enum class Os : unsigned char {
    Linux,
    Mac,
    Windows,
};
}

template <fs::FileHandleType T>
struct FileHandle final {
    static_assert(false, "Could not determine platform file handle type");
};

template <>
struct FileHandle<fs::FileHandleType::FileDescriptor> final {
    using type = int;
};

template <>
struct FileHandle<fs::FileHandleType::Handle> final {
    /// Technically not "guaranteed" by MSFT, but HANDLE has literally always
    /// been void* after 16 bit DOS.
    using type = void*;
};

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

/// Respects cv qualifiers, so `const int` and `int` would not be considered
/// the same type
template <class T, class V>
struct IsSame final {
    static inline constexpr bool value{false};
};

template <class TV>
struct IsSame<TV, TV> final {
    static inline constexpr bool value{true};
};

template <class T, class V>
static inline constexpr bool is_same_v{IsSame<T, V>::value};

template <class T, class V>
concept same_type = is_same_v<T, V>;

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

template <class Compared, class Head, class... Rest>
struct OneOf final {
 private:
    static inline consteval bool Detail() {
        if constexpr (is_same_v<Compared, Head>) {
            return true;
        } else {
            return OneOf<Compared, Rest...>::value;
        }
    }

 public:
    static inline constexpr bool value{Detail()};
};

template <class Compared, class Last>
struct OneOf<Compared, Last> final {
    static inline constexpr bool value{is_same_v<Compared, Last>};
};

static_assert(OneOf<bool, bool>::value);
static_assert(!OneOf<bool, void>::value);
static_assert(OneOf<bool, int, char, unsigned long, bool>::value);

}  // namespace cmp

namespace signedness {
// TODO(SEP): IsSigned meta functions
}  // namespace signedness

}  // namespace tedit::meta

#endif  // SRC_INCLUDE_META_HPP_
