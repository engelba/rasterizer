CC = gcc
# -I$(INCLUDE_DIR) tells the compiler to look in include/ for header files
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

CFLAGS = -Wall -Wextra -std=c11 -I $(INCLUDE_DIR)
# Automatically find all .c files in src/
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Define the final executable path
TARGET = $(BUILD_DIR)/main

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
