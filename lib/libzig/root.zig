const std = @import("std");
const builtin = @import("builtin");
const platform = switch (builtin.target.os.tag) {
    .linux => @import("platform/linux.zig"),
    .windows => @import("platform/windows.zig"),
    .macos, .freebsd, .netbsd, .dragonfly, .openbsd, .serenity => @import("platform/posix.zig"),
    else => |os| @compileError("Unsupported OS: " ++ @tagName(os)),
};

const file_handle_t = platform.file_handle_t;

export fn enter_cooked_mode() void {
    platform.enterCookedMode() catch |err| {
        @panic(@errorName(err));
    };
}

export fn enter_raw_mode() void {
    platform.enterRawMode() catch |err| {
        @panic(@errorName(err));
    };
}

export fn read_char(file: file_handle_t) u8 {
    return platform.read(file) catch |err| {
        @panic(@errorName(err));
    };
}

export fn write_char(file: file_handle_t, char: u8) void {
    platform.write(file, char) catch |err| {
        @panic(@errorName(err));
    };
}

export fn clear_screen() void {
    platform.clear_screen() catch |err| {
        @panic(@errorName(err));
    };
}

export fn open_stdin() file_handle_t {
    return std.Io.File.stdin().handle;
}

export fn open_stdout() file_handle_t {
    return std.Io.File.stdout().handle;
}

export fn open_stderr() file_handle_t {
    return std.Io.File.stderr().handle;
}
