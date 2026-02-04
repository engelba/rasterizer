CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC_DIR = src
BUILD_DIR = build

$(BUILD_DIR)/main: $(SRC_DIR)/main.c $(SRC_DIR)/point.h $(SRC_DIR)/image.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c -o $(BUILD_DIR)/main

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
