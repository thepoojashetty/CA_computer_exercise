.PHONY: all clean jacobi

ROOT_PATH  := .
SRC_PATH   := $(ROOT_PATH)/src
BUILD_PATH := $(ROOT_PATH)/build
BIN_PATH   := $(ROOT_PATH)/bin

INC_PATH  := $(SRC_PATH)/include
INC_DIRS  := $(sort $(shell find $(INC_PATH) -type d))
INC_FLAGS := $(addprefix -iquote ,$(INC_DIRS))

CFLAGS_LIKWID := -I/apps/likwid/5.3.0/include -DLIKWID_PERFMON
LDFLAGS_LIKWID := -pthread -L/apps/likwid/5.3.0/lib/ -llikwid

CC      := nvcc 
#CFLAGS  := -Wall -pedantic -Werror -std=c99 -O3 #-xHost
#CFLAGS  := -qopt-streaming-stores always -03 $(CFLAGS_LIKWID)


LDFLAGS := #$(LDFLAGS_LIKWID)

all: strbenchmark 

clean:
	rm -rf $(BUILD_PATH) $(BIN_PATH)

#stream: $(BIN_PATH)/stream
strbenchmark:$(BIN_PATH)/strbenchmark

$(BIN_PATH)/strbenchmark: $(BUILD_PATH)/main.o
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.cu
	mkdir -p $(dir $@)
	$(CC) $(INC_FLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

