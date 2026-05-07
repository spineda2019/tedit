#ifndef SRC_INCLUDE_BUFFEREDIO_HPP_
#define SRC_INCLUDE_BUFFEREDIO_HPP_

#include "./File.hpp"
#include "./meta.hpp"
#include "types.hpp"

namespace tedit::io {
template <types::io::FileStackBuffer buffer_t, tedit::file::OwningType OT>
class BufferedWriter {
 public:
    constexpr explicit BufferedWriter(buffer_t&& buf, tedit::File<OT> file)
        : buf_{meta::reference::forward(buf)},
          file_{meta::reference::move(file)},
          sentinel_{0} {}

    BufferedWriter& operator<<(unsigned char letter) {
        [[unlikely]]
        if (sentinel_ >= buf_.capacity()) {
            file_ << buf_;
            sentinel_ = 0;
        }

        buf_[sentinel_] = letter;
    }

 private:
    buffer_t buf_;
    tedit::File<OT> file_;
    types::size_t sentinel_;
};

template <types::io::FileStackBuffer buffer_t, tedit::file::OwningType OT>
class BufferedReader {
 public:
    constexpr explicit BufferedReader(buffer_t&& buf, tedit::File<OT> file)
        : buf_{meta::reference::forward(buf)},
          file_{meta::reference::move(file)},
          sentinel_{0} {}

 private:
    buffer_t buf_;
    tedit::File<OT> file_;
    types::size_t sentinel_;
};

}  // namespace tedit::io

#endif  // SRC_INCLUDE_BUFFEREDIO_HPP_
