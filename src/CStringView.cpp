#include "include/CStringView.hpp"

#include "include/meta.hpp"

namespace tedit {
CStringView::CStringView() noexcept : data_{}, len_{} {};

CStringView::CStringView(const unsigned char* data,
                         tedit::types::size_t len) noexcept
    : data_{data}, len_{len} {
    // TODO(SEP): Remove
    (void)data_;
    (void)len_;
};
}  // namespace tedit
