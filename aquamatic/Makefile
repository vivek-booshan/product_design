CC = zig cc

NCURSES_LIB = "${NCURSES_PATH}/lib"
NCURSES_LIB_ARM = "${NCURSES_PATH_ARM}/lib"

CFLAGS = -I $(NCURSES_INCLUDE) -Os
LDFLAGS = -L $(NCURSES_LIB) -l ncurses
LDFLAGS_ARM = -L $(NCURSES_LIB_ARM) -l ncurses
ARM_TARGET = --target=aarch64-linux-gnu
WARNING_FLAGS = -Wall -Wextra -Wpedantic -Wconversion
SRC = main.c tui.c daemon.c preset.c help.c temperature.c init_serial_port.c plots.c ph.c

BUILD_DIR = build/native
BUILD_DIR_ARM = build/arm

OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)
OBJ_ARM = $(SRC:%.c=$(BUILD_DIR_ARM)/%.o)

OUT = $(BUILD_DIR)/aquamatic
OUT_ARM = $(BUILD_DIR_ARM)/aquamatic

all: $(OUT_ARM) $(OUT)
native: $(OUT)
pi: $(OUT_ARM)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR_ARM):
	mkdir -p $(BUILD_DIR_ARM)

### COMPILE NATIVE ###
$(BUILD_DIR)/%.o: %.c  $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $@ $(WARNING_FLAGS)

$(OUT): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o $(OUT) $(WARNING_FLAGS)

### COMPILE ARM ###

$(BUILD_DIR_ARM)/%.o: %.c $(BUILD_DIR_ARM)
	$(CC) -c $< $(ARM_TARGET) $(CFLAGS) -o $@

$(OUT_ARM): $(OBJ_ARM)
	$(CC) $(OBJ_ARM) $(CFLAGS) $(LDFLAGS_ARM) -o $(OUT_ARM) $(ARM_TARGET) $(WARNING_FLAGS)

# Clean target (removes compiled output)
clean:
	rm -rf $(BUILD_DIR) $(BUILD_DIR_ARM)

