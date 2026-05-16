const std = @import("std");

pub const ParseError = error{
    UnexpectedArgument,
    ExpectedArgName,
    ExpectedArgVal,
    FoundNameBeforeVal,
    ConversionError,
    MissingArgVal,
    UnknownArgName,
};

pub fn ArgumentParser(comptime T: type) type {
    comptime {
        const type_info = @typeInfo(T);
        switch (type_info) {
            .@"struct" => |s| {
                if (s.is_tuple) {
                    @compileError("TODO: Handle tuples");
                }
            },
            else => {
                @compileError("Expected a struct, got: " ++ @tagName(type_info));
            },
        }
    }

    return struct {
        const Self = @This();
        const State = enum {
            SeekArgVal,
            SeekArgName,
        };
        const field_names = std.meta.fieldNames(T);

        cmdline: []const []const u8,

        pub fn parse(self: Self) ParseError!T {
            var opts: T = defaultInitialize(T);

            var seek_state = State.SeekArgName;
            var current_name: ?[]const u8 = null;

            for (self.cmdline) |arg| {
                if (arg.len == 0) {
                    return ParseError.UnexpectedArgument;
                }

                switch (seek_state) {
                    .SeekArgName => {
                        // names must look like --this
                        if (arg.len <= 2) {
                            return ParseError.ExpectedArgName;
                        }
                        if (arg[0] != '-' or arg[1] != '-') {
                            return ParseError.ExpectedArgName;
                        }

                        const was_flag: bool = mutateIfFlag(&opts, arg[2..]);
                        if (!was_flag) {
                            current_name = arg[2..];
                            seek_state = .SeekArgVal;
                        }
                    },
                    .SeekArgVal => {
                        if (current_name) |name| {
                            try mutateOption(&opts, name, arg);

                            seek_state = .SeekArgName;
                        } else {
                            return ParseError.FoundNameBeforeVal;
                        }
                    },
                }
            }

            if (seek_state == .SeekArgVal) {
                return ParseError.MissingArgVal;
            } else {
                return opts;
            }
        }

        fn defaultInitialize(comptime Ty: type) Ty {
            var opts: T = undefined;
            inline for (comptime std.meta.fieldNames(Ty)) |field_name| {
                const FieldType = @FieldType(T, field_name);
                const field_info = @typeInfo(FieldType);
                @field(opts, field_name) = switch (field_info) {
                    .optional => null,
                    .bool => false,
                    .@"struct" => defaultInitialize(@TypeOf(@field(opts, field_name))),
                    .array => .{},
                    .float => 0.0,
                    .int => 0,
                    else => undefined,
                };
            }

            return opts;
        }

        fn mutateIfFlag(opts: *T, arg_name: []const u8) bool {
            inline for (field_names) |target_field_name| {
                if (std.mem.eql(u8, target_field_name, arg_name)) {
                    if (@FieldType(T, target_field_name) == bool) {
                        @field(opts, target_field_name) = true;
                        return true;
                    }
                }
            }

            return false;
        }

        fn mutateOption(
            opts: *T,
            arg_name: []const u8,
            arg_val: []const u8,
        ) ParseError!void {
            inline for (field_names) |target_field_name| {
                if (std.mem.eql(u8, target_field_name, arg_name)) {
                    const RetType = comptime blk: {
                        const raw = @FieldType(T, target_field_name);
                        break :blk switch (@typeInfo(raw)) {
                            .optional => |opt| opt.child,
                            else => raw,
                        };
                    };
                    const convert: fn ([]const u8) RetType = comptime blk: {
                        switch (@typeInfo(RetType)) {
                            .pointer => |ptr| {
                                if (ptr.size == .slice and ptr.child == u8) {
                                    break :blk struct {
                                        fn id(in: []const u8) RetType {
                                            return @ptrCast(in);
                                        }
                                    }.id;
                                } else {
                                    @compileError("TODO: Complex ptr types");
                                }
                            },
                            else => {
                                //
                                // @compileLog(info);
                            },
                        }
                        //
                        const func_name: []const u8 = "convert_" ++ @typeName(RetType);
                        for (@typeInfo(T).@"struct".decls) |decl| {
                            if (std.mem.eql(u8, func_name, decl.name)) {
                                const func = @field(T, func_name);
                                const decl_type = @TypeOf(func);
                                const decl_info = @typeInfo(decl_type);
                                switch (decl_info) {
                                    .@"fn" => |f| {
                                        if (f.return_type != RetType) {
                                            @compileError(decl.name ++ " has wrong conversion return type");
                                        }

                                        if (f.params.len != 1) {
                                            @compileError(decl.name ++ " should only take in a single arg");
                                        }

                                        break :blk func;
                                    },
                                    else => |other| {
                                        var err = "Decl ";
                                        err = err ++ decl.name;
                                        err = err ++ " should be a function returning a ";
                                        err = err ++ @typeName(RetType);
                                        err = err ++ "but was instead a ";
                                        err = err ++ @tagName(other);
                                        @compileError(err);
                                    },
                                }
                            }
                        }

                        @compileError("Could not find viable conversion function for type: " ++ @typeName(RetType));
                    };
                    @field(opts, target_field_name) = convert(arg_val);
                    return;
                }
            }

            return ParseError.UnexpectedArgument;
        }
    };
}
