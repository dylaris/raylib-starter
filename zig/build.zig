const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{
        .default_target = .{ .abi = .gnu, }, // avoid to link musl in my machine
    });
    const optimize = b.standardOptimizeOption(.{});

    // zig build -Dstatic=true
    const static_link = b.option(
        bool,
        "static",
        "Static link raylib"
    ) orelse false;

    const exe = b.addExecutable(.{
        .name = "main",
        .root_module = b.createModule(.{
            .root_source_file = b.path("main.zig"),
            .target = target,
            .optimize = optimize,
        }),
    });

    const raylib_dir = "../raylib/raylib-5.5_linux_amd64";
    exe.addIncludePath(b.path(raylib_dir ++ "/include"));

    if (static_link) {
        exe.addObjectFile(b.path(raylib_dir ++ "/lib/libraylib.a"));
    } else {
        exe.addLibraryPath(b.path(raylib_dir ++ "/lib"));
        exe.linkSystemLibrary("raylib");
    }

    exe.linkSystemLibrary("m");
    exe.linkSystemLibrary("pthread");
    exe.linkSystemLibrary("dl");
    exe.linkLibC();

    b.installArtifact(exe);
}
