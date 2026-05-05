// Copyright 2026 Sebastian Pineda

#include "include/CStringView.hpp"
#include "include/FileHandle.hpp"
#include "include/libtedit.hpp"

extern "C" void cppmain(const unsigned char* const path,
                        tedit::types::size_t len) {
    const tedit::CStringView full_path{path, len};

    using TerminalIO = tedit::FileHandle<tedit::file::OwningType::NonOwning>;
    const TerminalIO output{tedit::meta::fs::SpecialFile::StdOut};
    const TerminalIO input{tedit::meta::fs::SpecialFile::StdIn};

    const tedit::FileHandle<tedit::file::OwningType::Owning> fod{full_path};

    tedit::enter_raw_mode();
    unsigned char ch{};
    while (ch != 'q') {
        ch = tedit::read_char();
        switch (ch) {
            case 'Q':
                tedit::clear_screen();
                break;
            default:
                output << ch;
                break;
        }
    }
    tedit::enter_cooked_mode();
}
