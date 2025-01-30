const std = @import("std");

pub fn build(b: *std.Build) void {
    // const target_64bit = std.Target{
    //     .cpu = .{ .arch = .aarch64 },
    //     .os = .linux,
    //     .abi = .gnu,
    // };

    // const target_32bit = std.Target {
    //     .cpu_arch = .arm,
    //     .os_tag = .linux,
    //     .abi = .gnueabihf,
    // };
    const target = b.standardTargetOptions(.{});
    const exe = b.addExecutable(.{
        .name = "led",
        .root_source_file = b.path("led.zig"),
        .target = target,
        .optimize = .ReleaseSafe,
    });

    exe.linkLibC();
    // needs to be known at compile time so can't do path construction
    exe.addLibraryPath(.{ .cwd_relative = "/nix/store/kxc482rd7qgyhlg128g0f5bkqfmhrh47-wiringpi-wiringPi-aarch64-unknown-linux-gnu-3.10/lib" });
    exe.addIncludePath(.{ .cwd_relative = "/nix/store/kxc482rd7qgyhlg128g0f5bkqfmhrh47-wiringpi-wiringPi-aarch64-unknown-linux-gnu-3.10/include" });
    exe.linkSystemLibrary("wiringPi");
    b.installArtifact(exe);
}
