.PHONY: all clean matmul

ROOT_PATH  := .
SRC_PATH   := $(ROOT_PATH)/src
BUILD_PATH := $(ROOT_PATH)/build
BIN_PATH   := $(ROOT_PATH)/bin

INC_PATH  := $(SRC_PATH)/include
INC_DIRS  := $(sort $(shell find $(INC_PATH) -type d))
INC_FLAGS := $(addprefix -iquote ,$(INC_DIRS))

CC      := nvcc 
CFLAGS  := -arch sm_86 src/matmul.cu -lcublas -lcurand

LDFLAGS :=

all: matmul

clean:
	rm -rf $(BUILD_PATH) $(BIN_PATH)

matmul: $(BIN_PATH)/matmul

$(BIN_PATH)/matmul: $(BUILD_PATH)/matmul.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cu
	mkdir -p $(dir $@) 
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
