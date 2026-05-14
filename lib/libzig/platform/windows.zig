const std = @import("std");

pub const file_handle_t = std.os.windows.HANDLE;
var starting_state: std.os.windows.DWORD = undefined;

const Error = error{
    ReadFailure,
    WriteFailure,
    GetConsoleModeError,
    SetConsoleModeError,
};

const LPDWORD = *std.os.windows.DWORD;

const ScreenBufferConsoleMode = packed struct(std.os.windows.DWORD) {
    ENABLE_PROCESSED_OUTPUT: bool, //         //  0x00_01
    ENABLE_WRAP_AT_EOL_OUTPUT: bool, //       //  0x00_02
    ENABLE_VIRTUAL_TERMINAL_PROCESSING: bool, //  0x00_04
    DISABLE_NEWLINE_AUTO_RETURN: bool, //     //  0x00_08
    ENABLE_LVB_GRID_WORLDWIDE: bool, //       //  0x00_10
    __: u31 = 0,

    pub fn toDword(self: @This()) std.os.windows.DWORD {
        return @bitCast(self);
    }
};

const InputConsoleMode = packed struct(std.os.windows.DWORD) {
    ENABLE_PROCESSED_INPUT: bool, //     // 0x00_01
    ENABLE_LINE_INPUT: bool, //          // 0x00_02
    ENABLE_ECHO_INPUT: bool, //          // 0x00_04
    ENABLE_WINDOW_INPUT: bool, //        // 0x00_08
    ENABLE_MOUSE_INPUT: bool, //         // 0x00_10
    ENABLE_INSERT_MODE: bool, //         // 0x00_20
    ENABLE_QUICK_EDIT_MODE: bool, //     // 0x00_40
    _: u2 = 0, //                        // 0x00_80 and 0x01_00
    ENABLE_VIRTUAL_TERMINAL_INPUT: bool, // 0x02_00
    __: u22 = 0,

    pub fn toDword(self: @This()) std.os.windows.DWORD {
        return @bitCast(self);
    }
};

var original_input_mode: InputConsoleMode = undefined;

/// Parameters
///
/// hConsoleHandle [in]
/// A handle to the console input buffer or the console screen buffer.
/// The handle must have the GENERIC_READ access right. For more information,
/// see Console Buffer Security and Access Rights.
///
/// lpMode [out]
/// A pointer to a variable that receives the current mode of the specified
/// buffer.
///
/// Return value
/// If the function succeeds, the return value is nonzero.
/// If the function fails, the return value is zero. To get extended error
/// information, call GetLastError.
///
/// See MSFT docs for further details
extern fn GetConsoleMode(
    h_console_handle: std.os.windows.HANDLE, // in
    lp_mode: LPDWORD, //                     // out
) callconv(.winapi) std.os.windows.BOOL;

/// Parameters
///
/// hConsoleHandle [in]
/// A handle to the console input buffer or a console screen buffer. The handle
/// must have the GENERIC_READ access right. For more information, see Console
/// Buffer Security and Access Rights.
///
/// dwMode [in]
/// The input or output mode to be set.
///
/// Return value
/// If the function succeeds, the return value is nonzero.
/// If the function fails, the return value is zero. To get extended error
/// information, call GetLastError.
///
/// See MSFT docs for further details
extern fn SetConsoleMode(
    h_console_handle: std.os.windows.HANDLE, // in
    dw_mode: std.os.windows.DWORD, //        // in
) callconv(.winapi) std.os.windows.BOOL;

pub fn enterCookedMode() Error!void {
    const stdin: std.os.windows.HANDLE = std.Io.File.stdin().handle;
    const success = SetConsoleMode(stdin, original_input_mode.toDword());
    if (!success.toBool()) {
        return Error.SetConsoleModeError;
    }
}

pub fn enterRawMode() Error!void {
    const stdin: std.os.windows.HANDLE = std.Io.File.stdin().handle;
    var mode: std.os.windows.DWORD = undefined;
    const success = GetConsoleMode(stdin, &mode);
    if (success.toBool()) {
        original_input_mode = @bitCast(mode);
        var current_mode = original_input_mode;
        current_mode.ENABLE_ECHO_INPUT = false;
        current_mode.ENABLE_LINE_INPUT = false;
        current_mode.ENABLE_PROCESSED_INPUT = false;
        const set_success = SetConsoleMode(stdin, current_mode.toDword());
        if (!set_success.toBool()) {
            return Error.SetConsoleModeError;
        }
    } else {
        return Error.GetConsoleModeError;
    }
}

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
