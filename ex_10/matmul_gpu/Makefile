.PHONY: all clean matmul_gpu

ROOT_PATH  := .
SRC_PATH   := $(ROOT_PATH)/src
BUILD_PATH := $(ROOT_PATH)/build
BIN_PATH   := $(ROOT_PATH)/bin

INC_PATH  := $(SRC_PATH)/include
INC_DIRS  := $(sort $(shell find $(INC_PATH) -type d))
INC_FLAGS := $(addprefix -iquote ,$(INC_DIRS))

CC      := nvcc 
CFLAGS  := -arch=sm_86

LDFLAGS := -lcublas -lcurand

all: matmul_gpu

clean:
	rm -rf $(BUILD_PATH) $(BIN_PATH)

matmul_gpu: $(BIN_PATH)/matmul_gpu

$(BIN_PATH)/matmul_gpu: $(BUILD_PATH)/matmul_gpu.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cu
	mkdir -p $(dir $@) 
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
