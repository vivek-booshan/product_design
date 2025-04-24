CC = zig cc

SRC_DIR = src
BUILD_DIR = build/native
BUILD_DIR_ARM = build/arm

NCURSES_LIB = ${NCURSES_PATH}/lib
NCURSES_LIB_ARM = ${NCURSES_PATH_ARM}/lib
WIRINGPI_LIB = ${WIRINGPI_PATH}/lib
WIRINGPI_INCLUDE = ${WIRINGPI_PATH}/include

CFLAGS = -I$(NCURSES_INCLUDE) -Iinclude -I$(WIRINGPI_INCLUDE) -Os
WARNING_FLAGS = -Wall -Wextra -Wpedantic -Wconversion
ARM_TARGET = --target=aarch64-linux-gnu

LDFLAGS = -L$(NCURSES_LIB) -lncurses
LDFLAGS_ARM = -L$(NCURSES_LIB_ARM) -lncurses -L$(WIRINGPI_LIB) -lwiringPi

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
SRC_NAMES = $(notdir $(SRC_FILES))              # e.g., daemon.c
OBJ = $(addprefix $(BUILD_DIR)/, $(SRC_NAMES:.c=.o))
OBJ_ARM = $(addprefix $(BUILD_DIR_ARM)/, $(SRC_NAMES:.c=.o))

OUT = $(BUILD_DIR)/aquamatic
OUT_ARM = $(BUILD_DIR_ARM)/aquamatic

all: $(OUT) $(OUT_ARM)
native: $(OUT)
pi: $(OUT_ARM)

.PHONY: all native pi clean

# Create build dirs if needed
$(BUILD_DIR):
	@mkdir -p $@
$(BUILD_DIR_ARM):
	@mkdir -p $@

# Native object compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) $(WARNING_FLAGS) -o $@

# ARM object compilation
$(BUILD_DIR_ARM)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR_ARM)
	$(CC) -c $< $(CFLAGS) $(WARNING_FLAGS) $(ARM_TARGET) -o $@

# Link native
$(OUT): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) $(WARNING_FLAGS) -o $@

# Link ARM
$(OUT_ARM): $(OBJ_ARM)
	$(CC) $(OBJ_ARM) $(CFLAGS) $(LDFLAGS_ARM) $(WARNING_FLAGS) $(ARM_TARGET) -o $@

clean:
	rm -rf build
