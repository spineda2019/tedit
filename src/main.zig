const std = @import("std");
const Io = std.Io;
const arg = @import("arg.zig");

const tedit = @import("tedit");

extern fn cppmain([*:0]const u8) void;

const Error = error{
    BadArgCount,
};

const ProgramError = Error || std.mem.Allocator.Error || std.process.Args.ToSliceError || arg.ParseError;

const Options = struct {
    version: bool,
    help: bool,
    path: ?[:0]const u8,

    pub fn convert_bool(input: []const u8) bool {
        return std.mem.eql(u8, "true", input);
    }
};

pub fn main(init: std.process.Init) ProgramError!void {
    const arena = init.arena.allocator();
    const arg_slice = try init.minimal.args.toSlice(arena);
    const args: arg.ArgumentParser(Options) = .{ .cmdline = arg_slice[1..] };
    const options: Options = try args.parse();

    std.debug.print("Opts: {}\n", .{options});

    const path: [:0]const u8 = blk: {
        if (options.path) |p| {
            break :blk p;
        } else {
            break :blk &.{0};
        }
    };

    cppmain(path);
}
