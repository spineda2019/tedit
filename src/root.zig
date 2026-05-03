const std = @import("std");
const builtin = @import("builtin");
const platform = switch (builtin.target.os.tag) {
    .linux => @import("platform/linux.zig"),
    .windows => @import("platform/windows.zig"),
    .macos => @import("platform/macos.zig"),
    else => |os| @compileError("Unsupported OS: " ++ @tagName(os)),
};

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

export fn read_char() u8 {
    return platform.read() catch |err| {
        @panic(@errorName(err));
    };
}

export fn print_char(char: u8) void {
    platform.print(char) catch |err| {
        @panic(@errorName(err));
    };
}

export fn clear_screen() void {
    platform.clear_screen() catch |err| {
        @panic(@errorName(err));
    };
}
