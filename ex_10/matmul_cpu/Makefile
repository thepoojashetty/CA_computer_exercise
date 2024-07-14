.PHONY: all clean dgemm_cpu

ROOT_PATH  := .
SRC_PATH   := $(ROOT_PATH)/src
BUILD_PATH := $(ROOT_PATH)/build
BIN_PATH   := $(ROOT_PATH)/bin

INC_PATH  := $(SRC_PATH)/include
INC_DIRS  := $(sort $(shell find $(INC_PATH) -type d))
INC_FLAGS := $(addprefix -iquote ,$(INC_DIRS))

CC      := icc 
CFLAGS  := -qmkl -Wall -pedantic -Werror -std=c99 -O3 #-I$(MKLROOT)/include #-xHost

LDFLAGS := #-L$(MKLROOT)/lib/intel64 -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm

all: dgemm_cpu # program is named jacobi

clean:
	rm -rf $(BUILD_PATH) $(BIN_PATH)

dgemm_cpu: $(BIN_PATH)/dgemm_cpu

$(BIN_PATH)/dgemm_cpu: $(BUILD_PATH)/dgemm_cpu.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INC_FLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

