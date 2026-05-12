//! Apparently std.c doesn't need linking against libc?

const Error = error{
    GetAttrFailed,
    SetAttrFailed,
    ReadFailure,
    WriteFailure,
};
const std = @import("std");

var starting_state: std.c.termios = undefined;
pub const file_handle_t = std.c.fd_t;
const stdin: file_handle_t = std.c.STDIN_FILENO;
const stdout: file_handle_t = std.c.STDOUT_FILENO;

pub fn enterCookedMode() Error!void {
    if (std.c.tcsetattr(stdin, .FLUSH, &starting_state) != 0) {
        return Error.SetAttrFailed;
    }
}

pub fn enterRawMode() Error!void {
    if (std.c.tcgetattr(stdin, &starting_state) != 0) {
        return Error.GetAttrFailed;
    }

    var term_info = starting_state;
    term_info.lflag.ECHO = false;
    term_info.lflag.ICANON = false;
    if (std.c.tcsetattr(stdin, .FLUSH, &term_info) != 0) {
        return Error.SetAttrFailed;
    }
}

pub fn read(file: file_handle_t) Error!u8 {
    var char = [1]u8{0};
    if (std.c.read(file, &char, 1) != 0) {
        return char[0];
    } else {
        return Error.ReadFailure;
    }
}

pub fn write(file: file_handle_t, char: u8) Error!void {
    if (std.c.write(file, &.{char}, 1) != 1) {
        return Error.WriteFailure;
    }
}

pub fn clear_screen() Error!void {
    const clear_ctrl_code: []const u8 = comptime "\x1b[2J";
    if (std.c.write(stdout, clear_ctrl_code.ptr, clear_ctrl_code.len) != clear_ctrl_code.len) {
        return Error.WriteFailure;
    }
}
