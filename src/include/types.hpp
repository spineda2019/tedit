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
}  // namespace tedit::types

#endif  // SRC_INCLUDE_TYPES_HPP_
