// Copyright 2026 Sebastian Pineda

#include "libtedit.hpp"

extern "C" void cppmain(const unsigned char* const path) {
    //
    (void)path;
    enter_raw_mode();
    enter_cooked_mode();
}
