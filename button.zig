const std = @import("std");

const wpi = @cImport({
    @cInclude("wiringPi.h");
});

const LEDPIN = @as(c_int, 0);
const BUTTON = @as(c_int, 1);

pub fn main() !void {
    if (wpi.wiringPiSetup() == -1) {
        std.debug.print("setup wiringPi failed!");
    }

    wpi.pinMode(LEDPIN, wpi.OUTPUT);
    wpi.pinMode(BUTTON, wpi.INPUT);

    wpi.digitalWrite(LEDPIN, wpi.HIGH);

    while (true) {
        if (wpi.digitalRead(BUTTON) == 0) {
            wpi.digitalWrite(LEDPIN, wpi.LOW);
        } else {
            wpi.digitalWrite(LEDPIN, wpi.HIGH);
        }
    }
}
