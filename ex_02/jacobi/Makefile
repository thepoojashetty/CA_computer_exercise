.PHONY: all clean jacobiunroll

ROOT_PATH  := .
SRC_PATH   := $(ROOT_PATH)/src
BUILD_PATH := $(ROOT_PATH)/build
BIN_PATH   := $(ROOT_PATH)/bin

INC_PATH  := $(SRC_PATH)/include
INC_DIRS  := $(sort $(shell find $(INC_PATH) -type d))
INC_FLAGS := $(addprefix -iquote ,$(INC_DIRS))

CC      := icc 
CFLAGS  := -Wall -pedantic -Werror -std=c99 -O3 #-xHost
CFLAGS  += -DJACOBIUNROLLTYPE=4 #1,2,4

LDFLAGS :=

all: jacobiunroll # program is named jacobiunroll

clean:
	rm -rf $(BUILD_PATH) $(BIN_PATH)

jacobiunroll: $(BIN_PATH)/jacobiunroll

$(BIN_PATH)/jacobiunroll: $(BUILD_PATH)/main.o $(BUILD_PATH)/jacobi.o $(BUILD_PATH)/get_time.o $(BUILD_PATH)/draw.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INC_FLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

