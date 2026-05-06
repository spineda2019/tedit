#ifndef SRC_INCLUDE_FILEHANDLE_HPP_
#define SRC_INCLUDE_FILEHANDLE_HPP_

#include "./CStringView.hpp"
#include "./libtedit.hpp"
#include "./types.hpp"
#include "meta.hpp"

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

    /// `auto&&` here is a forwarding reference, NOT an rvalue
    template <class Self>
    auto&& operator<<(this Self&& self, unsigned char letter) {
        // DEDUCING THIS LFGGGGGGG
        tedit::write_char(self.file_handle_, letter);
        return tedit::meta::reference::forward<Self>(self);
    }

    /// `auto&&` here is a forwarding reference, NOT an rvalue
    template <class Self>
    auto&& operator>>(this Self&& self, unsigned char& letter) {
        // DEDUCING THIS LFGGGGGGG
        letter = tedit::read_char(self.file_handle_);
        return tedit::meta::reference::forward<Self>(self);
    }

 private:
    tedit::types::file_handle_t file_handle_;
};

}  // namespace tedit

#endif  // SRC_INCLUDE_FILEHANDLE_HPP_
