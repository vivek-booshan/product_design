CC = zig cc
WIRINGPI = ${WIRINGPI_PATH}
CFLAGS = -I$(WIRINGPI)/include
LDFLAGS = -L$(WIRINGPI)/lib -lwiringPi
# CFLAGS = -I/nix/store/kxc482rd7qgyhlg128g0f5bkqfmhrh47-wiringpi-wiringPi-aarch64-unknown-linux-gnu-3.10/include
# LDFLAGS = -L/nix/store/kxc482rd7qgyhlg128g0f5bkqfmhrh47-wiringpi-wiringPi-aarch64-unknown-linux-gnu-3.10/lib -lwiringPi
SRC = led.c
OUT = led
TARGET = aarch64-linux-gnu


# Default target
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(OUT) -target $(TARGET)

# Clean target (removes compiled output)
clean:
	rm -f $(OUT)

# Run the compiled program
# run: $(OUT)
# 	./$(OUT)
