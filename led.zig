const std = @import("std");
const wpi = @cImport({
    @cInclude("wiringPi.h");
});

const LEDPIN = @as(c_int, 0);
const DELAY = @as(c_uint, 500);

pub fn main() !void {
    try wpi.wiringPiSetup();
    // if (wpi.wiringPiSetup() == -1) {
    //     std.debug.print("Setup wiringPi failed !", .{});
    // }
    try wpi.pinMode(LEDPIN, wpi.OUTPUT);
    while (true) {
        try wpi.digitalWrite(LEDPIN, wpi.LOW);
        std.debug.print("...LED on\n", .{});
        try wpi.delay(DELAY);

        try wpi.digitalWrite(LEDPIN, wpi.HIGH);
        std.debug.print("LED off...\n", .{});
        try wpi.delay(DELAY);
    }
}
