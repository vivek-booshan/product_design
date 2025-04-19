const std = @import("std");

pub fn build(b: *std.Build) void {
    const target_native = b.standardTargetOptions(.{});
    const target_arm = b.standardTargetOptions(.{ .default_target = .{ .arch = .aarch64, .os = .linux } });
    const optimize = b.standardOptimizeOption(.{});

    const src_files = &[_][]const u8{
        "main.c",
        "tui.c",
        "daemon.c",
        "preset.c",
        "help.c",
        "display_temperature.c",
        "get_temperature.c",
    };

    const ncurses_include = std.process.getEnvVarOwned(b.allocator, "NCURSES_INCLUDE") catch {
        std.debug.print("⚠️  NCURSES_INCLUDE not set in environment\n", .{});
        return;
    };

    const ncurses_lib_native = std.process.getEnvVarOwned(b.allocator, "NCURSES_LIB") catch {
        std.debug.print("⚠️  NCURSES_LIB not set in environment\n", .{});
        return;
    };

    const ncurses_lib_arm = std.process.getEnvVarOwned(b.allocator, "NCURSES_LIB_ARM") catch {
        std.debug.print("⚠️  NCURSES_LIB_ARM not set in environment\n", .{});
        return;
    };

    // Native build
    const exe = b.addExecutable(.{
        .name = "aquamatic",
        .target = target_native,
        .optimize = optimize,
    });

    for (src_files) |src| {
        exe.addCSourceFile(.{ .file = .{ .path = src }, .flags = &[_][]const u8{"-Os", "-g", "-Wall", "-Wextra", "-Wpedantic", "-Wconversion"} });
    }

    exe.addIncludePath(.{ .path = ncurses_include });
    exe.addLibraryPath(.{ .path = ncurses_lib_native });
    exe.linkSystemLibrary("ncurses");
    exe.install();

    // Cross-compiled ARM
    const arm_exe = b.addExecutable(.{
        .name = "aquamatic-arm",
        .target = target_arm,
        .optimize = optimize,
    });

    for (src_files) |src| {
        arm_exe.addCSourceFile(.{ .file = .{ .path = src }, .flags = &[_][]const u8{"-Os", "-g", "-Wall", "-Wextra", "-Wpedantic", "-Wconversion"} });
    }

    arm_exe.addIncludePath(.{ .path = ncurses_include });
    arm_exe.addLibraryPath(.{ .path = ncurses_lib_arm });
    arm_exe.linkSystemLibrary("ncurses");
    arm_exe.install();

    // Build steps
    b.step("native", "Build for host").dependOn(&exe.step);
    b.step("pi", "Build for ARM Pi").dependOn(&arm_exe.step);
}
