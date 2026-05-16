#ifndef SRC_INCLUDE_BUFFEREDIO_HPP_
#define SRC_INCLUDE_BUFFEREDIO_HPP_

#include <libcpp/include/concepts.hpp>

#include "libcpp/include/File.hpp"
#include "libcpp/meta/types.hpp"
#include "libzig/libzig.hpp"

namespace libzig::io {
template <libcpp::meta::concepts::io::FileStackBuffer buffer_t,
          libcpp::file::OwningType OT>
class BufferedWriter {
 public:
    constexpr explicit BufferedWriter(buffer_t&& buf, libcpp::File<OT>& file)
        : buf_{libcpp::meta::types::reference::forward<buffer_t>(buf)},
          file_{file},
          sentinel_{0} {}

    BufferedWriter& operator<<(unsigned char letter) {
        [[unlikely]]
        if (sentinel_ >= buf_.capacity()) {
            file_.Write(buf_.begin(), sentinel_);
            sentinel_ = 0;
        }

        buf_[sentinel_++] = letter;
        return *this;
    }

 public:  // rule of 5
    ~BufferedWriter() noexcept { file_.Write(buf_.begin(), sentinel_); }
    auto operator=(const BufferedWriter&) = default;
    BufferedWriter(const BufferedWriter&) = default;
    auto operator=(BufferedWriter&&) = default;
    BufferedWriter(BufferedWriter&&) = default;

 private:
    buffer_t buf_;
    libcpp::File<OT>& file_;
    size_t sentinel_;
};

template <libcpp::meta::concepts::io::FileStackBuffer buffer_t,
          libcpp::file::OwningType OT>
class BufferedReader {
 public:
    constexpr explicit BufferedReader(buffer_t&& buf, libcpp::File<OT>& file)
        : buf_{libcpp::meta::types::reference::forward<buffer_t>(buf)},
          file_{file},
          sentinel_{0} {}

    BufferedReader& operator>>(unsigned char& letter) {
        [[unlikely]]
        if (sentinel_ >= buf_.capacity()) {
            file_.Read(buf_.begin(), sentinel_);
            sentinel_ = 0;
        }

        letter = buf_[sentinel_++];
        return *this;
    }

 public:  // rule of 0
 private:
    buffer_t buf_;
    libcpp::File<OT>& file_;
    size_t sentinel_;
};

}  // namespace libzig::io

#endif  // SRC_INCLUDE_BUFFEREDIO_HPP_
