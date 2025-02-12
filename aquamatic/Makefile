# CC = zig cc
# # NCURSES = ${NCURSES_PATH}
# NCURSES_INCLUDE = "/nix/store/3lbb9lx1gmis6qm8clq9g9k37i5l8l52-ncurses-6.4.20221231-dev/include"
# NCURSES_LIB = "/nix/store/x9lgx9pd242kw0sdvdmwvmgj6igw8h8k-ncurses-6.4.20221231/lib"
# NCURSES_LIB_ARM = "/nix/store/9wrswq92618yx8y0kv1mnj7d8wlw6b70-ncurses-aarch64-unknown-linux-gnu-6.4.20221231/lib"
# CFLAGS = -I $(NCURSES_INCLUDE)
# LDFLAGS = -L $(NCURSES_LIB) -l ncurses
# LDFLAGS_ARM = -L $(NCURSES_LIB_ARM) -l ncurses
# ARM_TARGET = --target=aarch64-linux-gnu


# # $(WIRINGPI)/include 
# # LDFLAGS = -L$(WIRINGPI)/lib -lwiringPi
# # CFLAGS = -I/nix/store/kxc482rd7qgyhlg128g0f5bkqfmhrh47-wiringpi-wiringPi-aarch64-unknown-linux-gnu-3.10/include
# # LDFLAGS = -L/nix/store/kxc482rd7qgyhlg128g0f5bkqfmhrh47-wiringpi-wiringPi-aarch64-unknown-linux-gnu-3.10/lib -lwiringPi

# SRC = main.c tui.c daemon.c
# OBJ = main.o tui.o daemon.o
# OUT = aquamatic
# OUT_ARM = aquamatic_arm
# TARGET = aarch64-linux-gnu


# # Default target
# all: $(OUT)

# $(OUT): $(SRC)
# 	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(OUT) 

# # Clean target (removes compiled output)
# clean:
# 	rm -f $(OUT)

# # Run the compiled program
# # run: $(OUT)
# # 	./$(OUT)

# main_arm :
# 	$(CC) -c main.c $(ARM_TARGET) -Os
# tui_arm :
# 	$(CC) -c tui.c $(ARM_TARGET) $(CFLAGS) $(LDFLAGS_ARM) -Os
# daemon_arm :
# 	$(CC) -c daemon.c $(ARM_TARGET) -Os

# arm : $(OUT_ARM)

# $(OUT_ARM) : $(OBJ)
# 	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS_ARM) -o $(OUT) $(ARM_TARGET) -Os

CC = zig cc

NCURSES_INCLUDE = "/nix/store/3lbb9lx1gmis6qm8clq9g9k37i5l8l52-ncurses-6.4.20221231-dev/include"
NCURSES_LIB = "/nix/store/x9lgx9pd242kw0sdvdmwvmgj6igw8h8k-ncurses-6.4.20221231/lib"
NCURSES_LIB_ARM = "/nix/store/9wrswq92618yx8y0kv1mnj7d8wlw6b70-ncurses-aarch64-unknown-linux-gnu-6.4.20221231/lib"

CFLAGS = -I $(NCURSES_INCLUDE)
LDFLAGS = -L $(NCURSES_LIB) -l ncurses
LDFLAGS_ARM = -L $(NCURSES_LIB_ARM) -l ncurses
ARM_TARGET = --target=aarch64-linux-gnu

SRC = main.c tui.c daemon.c

BUILD_DIR = build/native
BUILD_DIR_ARM = build/arm

OBJ = $(BUILD_DIR)/main.o $(BUILD_DIR)/tui.o $(BUILD_DIR)/daemon.o
OBJ_ARM = $(BUILD_DIR_ARM)/main.o $(BUILD_DIR_ARM)/tui.o $(BUILD_DIR_ARM)/daemon.o

OUT = $(BUILD_DIR)/aquamatic
OUT_ARM = $(BUILD_DIR_ARM)/aquamatic

# Default target
all: $(OUT_ARM) $(OUT)

# Create directories if they don't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR_ARM):
	mkdir -p $(BUILD_DIR_ARM)

# Compile native
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $@

$(OUT): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o $(OUT) 

# Compile ARM
$(BUILD_DIR_ARM)/%.o: %.c $(BUILD_DIR_ARM)
	$(CC) -c $< $(ARM_TARGET) $(CFLAGS) -o $@

$(OUT_ARM): $(OBJ_ARM)
	$(CC) $(OBJ_ARM) $(CFLAGS) $(LDFLAGS_ARM) -o $(OUT_ARM) $(ARM_TARGET)

# Clean target (removes compiled output)
clean:
	rm -rf $(BUILD_DIR) $(BUILD_DIR_ARM)

