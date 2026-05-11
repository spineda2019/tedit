#ifndef LIB_LIBCPP_INCLUDE_DETAIL_FS_HPP_
#define LIB_LIBCPP_INCLUDE_DETAIL_FS_HPP_

#include <libzig/libzig.hpp>
#include <libzig/meta/comptime_values.hpp>
#include <libzig/meta/types.hpp>

namespace libzig::types::detail {
using FileHandleTag = libzig::meta::tags::fs::FileHandleTag;
using SpecialFile = libzig::meta::tags::fs::SpecialFile;

template <FileHandleTag T>
inline auto GetSpecialFileHandleHelper(SpecialFile) noexcept {
    static_assert(false);
}

using FHT = meta::tags::fs::FileHandleTag;

/// TODO(SEP): Docs
template <>
inline auto GetSpecialFileHandleHelper<FHT::FileDescriptor>(
    SpecialFile sf) noexcept {
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

template <>
inline auto GetSpecialFileHandleHelper<FHT::Handle>(SpecialFile) noexcept {
    return nullptr;
}
}  // namespace libzig::types::detail

#endif  // LIB_LIBCPP_INCLUDE_DETAIL_FS_HPP_
