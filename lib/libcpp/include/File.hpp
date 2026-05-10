#ifndef SRC_INCLUDE_FILEHANDLE_HPP_
#define SRC_INCLUDE_FILEHANDLE_HPP_

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
    explicit File() noexcept
        requires(T == file::OwningType::Owning)
    {}

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

    /// `auto&&` here is a forwarding reference, NOT an rvalue
    template <class Self>
    auto&& operator>>(this Self&& self, unsigned char& letter) {
        // DEDUCING THIS LFGGGGGGG
        letter = libzig::read_char(self.file_handle_);
        return libcpp::meta::types::reference::forward<Self>(self);
    }

 private:
    libzig::file_t file_handle_;
};

}  // namespace libcpp

#endif  // SRC_INCLUDE_FILEHANDLE_HPP_
