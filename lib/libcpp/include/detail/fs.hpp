#ifndef LIB_LIBCPP_INCLUDE_DETAIL_FS_HPP_
#define LIB_LIBCPP_INCLUDE_DETAIL_FS_HPP_

#include <libzig/libzig.hpp>
#include <libzig/meta/comptime_values.hpp>
#include <libzig/meta/types.hpp>

namespace libzig::types::detail {
using FileHandleTag = libzig::meta::tags::fs::FileHandleTag;
using SpecialFile = libzig::meta::tags::fs::SpecialFile;

/// TODO(SEP): Docs
template <FileHandleTag T>
    requires(T == FileHandleTag::FileDescriptor)
inline file_t GetSpecialFileHandleHelper(SpecialFile sf) noexcept {
    switch (sf) {
        case SpecialFile::StdIn:
            return 0;
            break;
        case SpecialFile::StdOut:
            return 1;
            break;
        case SpecialFile::StdErr:
            return 2;
            break;
    }
}

template <FileHandleTag T>
    requires(T == FileHandleTag::Handle)
inline file_t GetSpecialFileHandleHelper(SpecialFile) noexcept {
    static_assert(false, "TODO: Windows not yet supported");
}
}  // namespace libzig::types::detail

#endif  // LIB_LIBCPP_INCLUDE_DETAIL_FS_HPP_
