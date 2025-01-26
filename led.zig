const std = @import("std");
const wpi = @cImport({
    @cInclude("wiringPi.h");
    @cDefine("LEDPIN", "0");
});
const DELAY = @as(c_uint, 500);

pub fn main() void {
    if (wpi.wiringPiSetup() == -1) {
        std.debug.print("Setup wiringPi failed !", .{});
    }
    wpi.pinMode(wpi.LEDPIN, wpi.OUTPUT);
    while (true) {
        wpi.digitalWrite(wpi.LEDPIN, wpi.LOW);
        std.debug.print("...LED on\n", .{});
        wpi.delay(DELAY);
        wpi.digitalWrite(wpi.LEDPIN, wpi.HIGH);
        std.debug.print("LED off...\n", .{});
        wpi.delay(DELAY);
    }
}
