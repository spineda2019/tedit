const std = @import("std");

pub const file_handle_t = std.os.windows.HANDLE;

const Error = error{};

pub fn enterCookedMode() Error!void {}

pub fn enterRawMode() Error!void {}

pub fn read(file: file_handle_t) Error!u8 {
    _ = file;
    return 0;
}

pub fn write(file: file_handle_t, char: u8) Error!void {
    _ = file;
    _ = char;
}

pub fn clear_screen() Error!void {}
