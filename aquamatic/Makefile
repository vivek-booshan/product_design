CC = zig cc

NCURSES_INCLUDE = "/nix/store/3lbb9lx1gmis6qm8clq9g9k37i5l8l52-ncurses-6.4.20221231-dev/include"
NCURSES_LIB = "${NCURSES_PATH}/lib"
NCURSES_LIB_ARM = "${NCURSES_PATH_ARM}/lib"

CFLAGS = -I $(NCURSES_INCLUDE) -Os
LDFLAGS = -L $(NCURSES_LIB) -l ncurses
LDFLAGS_ARM = -L $(NCURSES_LIB_ARM) -l ncurses
ARM_TARGET = --target=aarch64-linux-gnu
WARNING_FLAGS = -Wall -Wextra -Wpedantic -Wconversion
SRC = main.c tui.c daemon.c preset.c help.c temperature.c init_serial_port.c

BUILD_DIR = build/native
BUILD_DIR_ARM = build/arm

OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)
OBJ_ARM = $(SRC:%.c=$(BUILD_DIR_ARM)/%.o)

OUT = $(BUILD_DIR)/aquamatic
OUT_ARM = $(BUILD_DIR_ARM)/aquamatic

# Default target
all: $(OUT_ARM) $(OUT)
native: $(OUT)
pi: $(OUT_ARM)
# Create directories if they don't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR_ARM):
	mkdir -p $(BUILD_DIR_ARM)

# Compile native
$(BUILD_DIR)/%.o: %.c  $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $@ $(WARNING_FLAGS)

## header only
# $(OUT):
# 	$(CC) $(SRC) -o $(OUT) $(LDFLAGS) $(WARNING_FLAGS) 

## dll 
$(OUT): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o $(OUT) $(WARNING_FLAGS)

# Compile ARM
$(BUILD_DIR_ARM)/%.o: %.c $(BUILD_DIR_ARM)
	$(CC) -c $< $(ARM_TARGET) $(CFLAGS) -o $@

## header only
# $(OUT_ARM):
# 	$(CC) $(SRC) $(LDFLAGS_ARM) -o $(OUT_ARM) $(ARM_TARGET) $(WARNING_FLAGS)

## dll
$(OUT_ARM): $(OBJ_ARM)
	$(CC) $(OBJ_ARM) $(CFLAGS) $(LDFLAGS_ARM) -o $(OUT_ARM) $(ARM_TARGET)

# Clean target (removes compiled output)
clean:
	rm -rf $(BUILD_DIR) $(BUILD_DIR_ARM)

