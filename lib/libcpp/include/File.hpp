#ifndef SRC_INCLUDE_FILEHANDLE_HPP_
#define SRC_INCLUDE_FILEHANDLE_HPP_

#include <libcpp/include/StringView.hpp>
#include <libcpp/include/fs.hpp>
#include <libcpp/meta/types.hpp>
#include <libzig/libzig.hpp>

namespace libcpp {
namespace file {
enum class OwningType : unsigned char {
    Owning,
    NonOwning,
};

}  // namespace file

template <file::OwningType T>
class File final {
 public:
    explicit File(StringView path) noexcept
        requires(T == file::OwningType::Owning)
        : file_handle_{libzig::open(path.CStr(), path.Len())} {}

    explicit File(libzig::meta::tags::fs::SpecialFile sf) noexcept
        requires(T == file::OwningType::NonOwning)
        : file_handle_{libzig::types::GetSpecialFileHandle(sf)} {}

    /// `auto&&` here is a forwarding reference, NOT an rvalue
    template <class Self>
    auto&& operator<<(this Self&& self, unsigned char letter) {
        // DEDUCING THIS LFGGGGGGG
        libzig::write_char(self.file_handle_, letter);
        return libcpp::meta::types::reference::forward<Self>(self);
    }

    template <class Self>
    auto&& operator<<(this Self&& self, char const* str) {
        for (unsigned char letter{static_cast<unsigned char>(*str)}; letter;
             letter = static_cast<unsigned char>(*++str)) {
            libzig::write_char(self.file_handle_, letter);
        }
        return libcpp::meta::types::reference::forward<Self>(self);
    }

    template <class Self>
    auto&& operator<<(this Self&& self, StringView view) {
        for (libzig::size_t idx{0}; idx < view.Len(); ++idx) {
            meta::types::reference::forward<Self>(self) << view[idx];
        }
        return libcpp::meta::types::reference::forward<Self>(self);
    }

    /// `auto&&` here is a forwarding reference, NOT an rvalue
    template <class Self>
    auto&& operator>>(this Self&& self, unsigned char& letter) {
        // DEDUCING THIS LFGGGGGGG
        letter = libzig::read_char(self.file_handle_);
        return libcpp::meta::types::reference::forward<Self>(self);
    }

 public:  // rule of 5
 private:
    libzig::file_t file_handle_;
};

}  // namespace libcpp

#endif  // SRC_INCLUDE_FILEHANDLE_HPP_
