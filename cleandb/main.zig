const std = @import("std");

const CleanupError = error{
    bad_args,
};

const ParsedArgs = struct {
    help_requested: bool,
    path_to_clean: []const u8,
};

const usage = "Usage: compiledb <PATH>";

pub fn main(init: std.process.Init) !void {
    const allocator = init.arena.allocator();
    const args = try init.minimal.args.toSlice(allocator);
    const io = init.io;

    const parsed_args: ParsedArgs = try parseArgs(args);
    if (parsed_args.help_requested) {
        printHelp();
        return;
    }

    std.debug.print("Cleaning up: {s}\n", .{parsed_args.path_to_clean});

    var dir: std.Io.Dir = try std.Io.Dir.openDirAbsolute(
        io,
        parsed_args.path_to_clean,
        .{
            .iterate = true,
        },
    );
    defer dir.close(io);

    var iterator: std.Io.Dir.Iterator = dir.iterate();

    var fragments: std.array_list.Aligned([]const u8, null) = .empty;

    while (try iterator.next(io)) |entry| {
        if (entry.kind == .file and std.mem.endsWith(u8, entry.name, ".json.tmp")) {
            try fragments.append(allocator, entry.name);
        }
    }

    if (fragments.items.len == 0) {
        std.debug.print("No Json fragments found, exiting...\n", .{});
        return;
    } else {
        var compiledb_file: std.Io.File = try dir.createFile(
            io,
            "compile_commands.json",
            .{ .truncate = true },
        );
        defer compiledb_file.close(io);

        var write_buffer: [4096]u8 = undefined;
        var read_buffer: [4096]u8 = undefined;

        var file_writer: std.Io.File.Writer = compiledb_file.writerStreaming(
            io,
            &write_buffer,
        );
        const writer = &file_writer.interface;
        defer file_writer.flush() catch unreachable;

        _ = try writer.write("[\n");
        defer _ = writer.write("]") catch unreachable;

        for (fragments.items) |fragment| {
            std.debug.print("Found: {s}\n", .{fragment});
            var fragment_file = try dir.openFile(io, fragment, .{});
            defer dir.deleteFile(io, fragment) catch unreachable;
            defer fragment_file.close(io);
            std.debug.print("Size: {}\n", .{try fragment_file.length(io)});

            var file_reader = fragment_file.readerStreaming(
                io,
                &read_buffer,
            );
            const reader = &file_reader.interface;
            const read = try reader.streamRemaining(writer);

            std.debug.print("Processed {} bytes...\n\n", .{read});
        }
    }
}

fn printHelp() void {
    std.debug.print("{s}\n", .{usage});
}

fn parseArgs(args: []const [:0]const u8) CleanupError!ParsedArgs {
    if (args.len != 2) {
        std.debug.print("{s}\n", .{usage});
        return CleanupError.bad_args;
    } else {
        var parsed_args: ParsedArgs = .{
            .path_to_clean = args[1],
            .help_requested = false,
        };

        for (args) |arg| {
            if (std.mem.eql(u8, "-h", arg)) {
                parsed_args.help_requested = true;
            }
        }

        return parsed_args;
    }
}
