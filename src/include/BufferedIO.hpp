#ifndef SRC_INCLUDE_BUFFEREDIO_HPP_
#define SRC_INCLUDE_BUFFEREDIO_HPP_

#include "./meta.hpp"

namespace tedit::io {
template <class T>
concept Buffer = requires(T t) {
    t.append();
    t.clear();
};

template <Buffer buffer_t, meta::fs::FileHandleType file_type>
class BufferedWriter {
 public:
    constexpr explicit BufferedWriter(buffer_t&& buf,
                                      meta::FileHandle<file_type> file)
        : buf_{meta::reference::forward(buf)}, file_{file} {}

 private:
    buffer_t buf_;
    meta::FileHandle<file_type> file_;
};

template <Buffer buffer_t>
class BufferedReader {
 public:
 private:
};

}  // namespace tedit::io

#endif  // SRC_INCLUDE_BUFFEREDIO_HPP_
