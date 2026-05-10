#ifndef LIB_LIBZIG_META_TYPE_TRAITS_HPP_
#define LIB_LIBZIG_META_TYPE_TRAITS_HPP_

namespace libzig::meta {
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
}  // namespace libzig::meta

#endif  // LIB_LIBZIG_META_TYPE_TRAITS_HPP_
