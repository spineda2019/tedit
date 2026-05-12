const std = @import("std");

pub const file_handle_t = std.os.windows.HANDLE;
var starting_state: std.os.windows.DWORD = undefined;

const Error = error{
    ReadFailure,
    WriteFailure,
};

pub fn enterCookedMode() Error!void {}

pub fn enterRawMode() Error!void {}

pub fn read(file: file_handle_t) Error!u8 {
    var char = [1]u8{0};
    var iosb: std.os.windows.IO_STATUS_BLOCK = undefined;
    const result = std.os.windows.ntdll.NtReadFile(
        file,
        null,
        null,
        null,
        &iosb,
        &char,
        char.len,
        null,
        null,
    );

    if (result == .SUCCESS) {
        return char[0];
    } else {
        return Error.ReadFailure;
    }
}

pub fn write(file: file_handle_t, char: u8) Error!void {
    var iosb: std.os.windows.IO_STATUS_BLOCK = undefined;
    const result = std.os.windows.ntdll.NtWriteFile(
        file,
        null,
        null,
        null,
        &iosb,
        &char,
        1,
        null,
        null,
    );

    if (result != .SUCCESS) {
        return Error.WriteFailure;
    }
}

pub fn clear_screen() Error!void {}
