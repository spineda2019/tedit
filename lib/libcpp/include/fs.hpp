#ifndef SRC_INCLUDE_TYPES_HPP_
#define SRC_INCLUDE_TYPES_HPP_

#include <libzig/libzig.hpp>

namespace libzig::types {

using FileHandleTag = libzig::meta::tags::fs::FileHandleTag;
using SpecialFile = libzig::meta::tags::fs::SpecialFile;

/// TODO(SEP): Docs
inline file_t GetSpecialFileHandle(SpecialFile sf) noexcept {
    switch (sf) {
        case SpecialFile::StdIn:
            return libzig::open_stdin();
            break;
        case SpecialFile::StdOut:
            return libzig::open_stdout();
            break;
        case SpecialFile::StdErr:
            return libzig::open_stderr();
            break;
    }
}

}  // namespace libzig::types

#endif  // SRC_INCLUDE_TYPES_HPP_
