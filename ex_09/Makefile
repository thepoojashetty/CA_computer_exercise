.PHONY: all clean stream

ROOT_PATH  := .
SRC_PATH   := $(ROOT_PATH)/src
BUILD_PATH := $(ROOT_PATH)/build
BIN_PATH   := $(ROOT_PATH)/bin

# INC_PATH  := $(SRC_PATH)/include
INC_DIRS  := $(sort $(shell find $(INC_PATH) -type d))
INC_FLAGS := $(addprefix -iquote ,$(INC_DIRS))

CFLAGS_LIKWID := -I/apps/likwid/5.3.0/include -DLIKWID_PERFMON
LDFLAGS_LIKWID := -pthread -L/apps/likwid/5.3.0/lib/ -llikwid

CC      := nvcc #icc 
CFLAGS  := #-O3 -qopt-streaming-stores always -qopenmp $(CFLAGS_LIKWID)


LDFLAGS := #$(LDFLAGS_LIKWID)

all: jacobi

clean:
	rm -rf $(BUILD_PATH) $(BIN_PATH)

 jacobi: $(BIN_PATH)/jacobi

 $(BIN_PATH)/jacobi: $(BUILD_PATH)/jacobi.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

 $(BUILD_PATH)/%.o: $(SRC_PATH)/%.cu
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

stream: $(BIN_PATH)/stream

$(BIN_PATH)/stream: $(BUILD_PATH)/stream.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.c
	mkdir -p $(dir $@)
	$(CC) $(INC_FLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

