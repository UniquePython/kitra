CC      = gcc
CFLAGS  = -Wall -Wextra -Wshadow -Wdouble-promotion -Wformat=2 -Wundef -std=c11 -I include $(shell pkg-config --cflags sdl2) $(shell pkg-config --cflags SDL2_image)
LIBS    = $(shell pkg-config --libs sdl2) $(shell pkg-config --libs SDL2_image) -Wl,--allow-shlib-undefined

SRC_DIR   = src
BUILD_DIR = build

SRCS   = $(wildcard $(SRC_DIR)/*.c)
OBJS   = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET = $(BUILD_DIR)/cinder

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)
