#ifndef SRC_INCLUDE_FILEHANDLE_HPP_
#define SRC_INCLUDE_FILEHANDLE_HPP_

#include "./CStringView.hpp"
#include "./types.hpp"

namespace tedit {
namespace file {
enum class OwningType : unsigned char {
    Owning,
    NonOwning,
};

}  // namespace file

template <file::OwningType T>
class FileHandle final {
 public:
    explicit FileHandle(CStringView) noexcept
        requires(T == file::OwningType::Owning)
    {}

    explicit FileHandle(tedit::meta::fs::SpecialFile sf) noexcept
        requires(T == file::OwningType::NonOwning)
        : file_handle_{types::GetSpecialFileHandle(sf)} {}

    void write() {
        //
        (void)file_handle_;
    }

 private:
    tedit::types::file_handle_t file_handle_;
};

}  // namespace tedit

#endif  // SRC_INCLUDE_FILEHANDLE_HPP_
