const std = @import("std");
const Io = std.Io;

const tedit = @import("tedit");

extern fn cppmain([*:0]const u8) void;

const Error = error{
    BadArgCount,
};

const ProgramError = Error || std.mem.Allocator.Error || std.process.Args.ToSliceError;

pub fn main(init: std.process.Init) ProgramError!void {
    const arena = init.arena.allocator();
    const args = try init.minimal.args.toSlice(arena);
    var arg_list: std.ArrayList([*:0]const u8) = try .initCapacity(arena, 2);

    for (args, 0..) |arg, idx| {
        if (idx > 0) {
            try arg_list.append(arena, arg.ptr);
        }
    }

    const path: [*:0]const u8 = switch (arg_list.items.len) {
        1 => arg_list.items[0],
        else => return Error.BadArgCount,
    };

    std.debug.print("Using file: {s}\n", .{path});

    cppmain(path);
}
