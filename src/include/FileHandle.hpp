#ifndef SRC_INCLUDE_FILEHANDLE_HPP_
#define SRC_INCLUDE_FILEHANDLE_HPP_

#include "./CStringView.hpp"

namespace tedit {
namespace file {
enum class OwningType : unsigned char {
    Owning,
    NonOwning,
};

enum class SpecialFile {
    StdOut,
    StdIn,
    StdErr,
};
}  // namespace file

template <file::OwningType T>
class FileHandle final {
 public:
    explicit FileHandle(CStringView) noexcept
        requires(T == file::OwningType::Owning)
    {}

    explicit FileHandle(file::SpecialFile) noexcept
        requires(T == file::OwningType::NonOwning)
    {}

    void write() {
        //
        (void)fd_;
    }

 private:
    int fd_;
};

}  // namespace tedit

#endif  // SRC_INCLUDE_FILEHANDLE_HPP_
