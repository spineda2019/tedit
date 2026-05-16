//! Linux specific terminal code
//!
//! Linux is based and has a stable system API (unlike MacOS, even if they are
//! posix compliant). Thus, we don't need to use libc, and can _only_ use zig's
//! OS APIs

const linux = @import("std").os.linux;

var starting_state: linux.termios = undefined;
const stdin: linux.fd_t = linux.STDIN_FILENO;
const stdout: linux.fd_t = linux.STDOUT_FILENO;

pub const file_handle_t = linux.fd_t;

const Error = error{
    GetAttrFailed,
    SetAttrFailed,
    UnknownStartingState,
    ReadFailure,
    WriteFailure,
};

pub fn enterCookedMode() Error!void {
    if (linux.tcsetattr(stdin, .FLUSH, &starting_state) != 0) {
        return Error.SetAttrFailed;
    }
}

pub fn enterRawMode() Error!void {
    if (linux.tcgetattr(stdin, &starting_state) != 0) {
        return Error.GetAttrFailed;
    }

    var term_info = starting_state;
    term_info.lflag.ECHO = false;
    term_info.lflag.ICANON = false;
    if (linux.tcsetattr(stdin, .FLUSH, &term_info) != 0) {
        return Error.SetAttrFailed;
    }
}

pub fn read(file: file_handle_t) Error!u8 {
    var char = [1]u8{0};
    if (linux.read(file, &char, 1) != 0) {
        return char[0];
    } else {
        return Error.ReadFailure;
    }
}

pub fn read_buf(file: file_handle_t, buf: []u8) Error!void {
    if (linux.read(file, buf.ptr, buf.len) != buf.len) {
        return Error.ReadFailure;
    }
}

pub fn write(file: file_handle_t, char: u8) Error!void {
    if (linux.write(file, &.{char}, 1) != 1) {
        return Error.WriteFailure;
    }
}

pub fn write_buf(file: file_handle_t, buf: []const u8) Error!void {
    if (linux.write(file, buf.ptr, buf.len) != buf.len) {
        return Error.WriteFailure;
    }
}

pub fn clear_screen() Error!void {
    const clear_ctrl_code: []const u8 = comptime "\x1b[2J";
    if (linux.write(stdout, clear_ctrl_code.ptr, clear_ctrl_code.len) != clear_ctrl_code.len) {
        return Error.WriteFailure;
    }
}
