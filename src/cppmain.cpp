// Copyright 2026 Sebastian Pineda

#include <libcpp/include/File.hpp>
#include <libcpp/include/io.hpp>
#include <libzig/meta/types.hpp>

extern "C" void cppmain(const unsigned char* const path,
                        libzig::size_t len) noexcept {
    const libcpp::StringView full_path{path, len};
    // const tedit::File<tedit::file::OwningType::Owning> fod{full_path};

    using OwningType = libcpp::file::OwningType;
    using TerminalIO = libcpp::File<OwningType::NonOwning>;
    using SpecialFile = libzig::meta::tags::fs::SpecialFile;

    const TerminalIO output{SpecialFile::StdOut};
    const TerminalIO input{SpecialFile::StdIn};

    if (full_path.Len()) {
        output << "Got a file: " << full_path << "\n";
    } else {
        output << "Empty Path\n";
    }

    libcpp::io::terminal::EnterRawMode();
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
    libcpp::io::terminal::EnterCookedMode();
}
