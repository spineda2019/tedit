// Copyright 2026 Sebastian Pineda

#include "./libtedit.hpp"

extern "C" void cppmain(const unsigned char* const path) {
    //
    (void)path;
    tedit::enter_raw_mode();
    unsigned char ch{};
    while (ch != 'q') {
        ch = tedit::read_char();
        switch (ch) {
            case 'Q':
                tedit::clear_screen();
                break;
            default:
                tedit::print_char(ch);
                break;
        }
    }
    tedit::enter_cooked_mode();
}
