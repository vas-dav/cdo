CC = gcc
CFLAGS = -Wall -ggdb
SRCS = $(shell find ./src -type f -name "*.c")
BUILD_DIR = $(shell git rev-parse --show-toplevel)/build
OBJS = $(patsubst ./src/%, $(BUILD_DIR)/%, $(SRCS:.c=.o))

all: $(BUILD_DIR)/cdo

$(BUILD_DIR)/cdo: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: ./src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all
