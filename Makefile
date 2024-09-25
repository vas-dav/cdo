CC = gcc
CFLAGS = -Wall -ggdb
SRCS = $(shell find ./src -type f -name "*.c")
BUILD_DIR = $(shell git rev-parse --show-toplevel)/build
OBJS = $(SRCS:.c=.o)

all: $(BUILD_DIR)/cdo

$(BUILD_DIR)/cdo: $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(OBJS)

.PHONY: all
