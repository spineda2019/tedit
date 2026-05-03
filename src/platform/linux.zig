//! Linux specific terminal code
//!
//! Linux is based and has a stable system API (unlike MacOS, even if they are
//! posix compliant). Thus, we don't need to use libc, and can _only_ use zig's
//! OS APIs

const linux = @import("std").os.linux;

var starting_state: linux.termios = undefined;
const stdin: linux.fd_t = linux.STDIN_FILENO;
const stdout: linux.fd_t = linux.STDOUT_FILENO;

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

pub fn read() Error!u8 {
    var char = [1]u8{0};
    if (linux.read(stdin, &char, 1) != 0) {
        return char[0];
    } else {
        return Error.ReadFailure;
    }
}

pub fn print(char: u8) Error!void {
    if (linux.write(stdout, &.{char}, 1) != 1) {
        return Error.WriteFailure;
    }
}

pub fn clear_screen() Error!void {
    const clear_ctrl_code: []const u8 = comptime "\x1b[2J";
    if (linux.write(stdout, clear_ctrl_code.ptr, clear_ctrl_code.len) != clear_ctrl_code.len) {
        return Error.WriteFailure;
    }
}
