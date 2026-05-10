// Copyright 2026 Sebastian Pineda

#include <include/File.hpp>
#include <meta/types.hpp>

extern "C" void cppmain(const unsigned char* const path, libzig::size_t len) {
    // const tedit::CStringView full_path{path, len};
    // const tedit::File<tedit::file::OwningType::Owning> fod{full_path};
    (void)path;
    (void)len;

    using OwningType = libcpp::file::OwningType;
    using TerminalIO = libcpp::File<OwningType::NonOwning>;
    using SpecialFile = libzig::meta::tags::fs::SpecialFile;

    const TerminalIO output{SpecialFile::StdOut};
    const TerminalIO input{SpecialFile::StdIn};

    libzig::enter_raw_mode();
    unsigned char ch{};
    while (ch != 'q') {
        input >> ch;
        switch (ch) {
            case 'Q':
                libzig::clear_screen();
                break;
            default:
                output << ch;
                break;
        }
    }
    libzig::enter_cooked_mode();
}
