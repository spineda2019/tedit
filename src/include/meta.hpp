#ifndef SRC_INCLUDE_META_HPP_
#define SRC_INCLUDE_META_HPP_

namespace tedit::meta {
namespace fs {
enum class FileHandleType {
    FileDescriptor,  // All of posix
    Handle,          // Just Windows TBH
};
}
struct size_type final {
    using type = decltype(sizeof(unsigned char));
    static_assert(sizeof(type) >= 2,
                  "C++11 and above mandates size_t is not less than 16 bits");
};

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

}  // namespace tedit::meta

#endif  // SRC_INCLUDE_META_HPP_
