CC          := armv7hl-redhat-linux-gnueabi-gcc
CFLAGS      := -c -Iinc
DBG_DIR     := debug
TEST_DIR    := test
BUILD_DIR   := build
BIN_DIR     := bin

EXE         := $(BIN_DIR)/alaw.x
DBG_EXE     := $(BIN_DIR)/alaw_debug.x
TEST_EXE    := $(BIN_DIR)/alaw_test.x

SRC_DIR     := src
INC_DIR     := inc

SOURCES     := $(wildcard $(SRC_DIR)/*.c)
HEADERS     := $(wildcard $(INC_DIR)/*.h)

# Separate main.c and test.c
MAIN_SRC    := $(filter-out $(SRC_DIR)/test.c, $(SOURCES))
TEST_SRC    := $(filter-out $(SRC_DIR)/main.c, $(SOURCES))

OBJECTS     := $(addprefix $(BUILD_DIR)/$(BIN_DIR)/, $(MAIN_SRC:$(SRC_DIR)/%.c=%.o))
DBG_OBJECTS := $(addprefix $(BUILD_DIR)/$(DBG_DIR)/$(BIN_DIR)/, $(MAIN_SRC:$(SRC_DIR)/%.c=%.o))
TEST_OBJECTS:= $(addprefix $(BUILD_DIR)/$(TEST_DIR)/$(BIN_DIR)/, $(TEST_SRC:$(SRC_DIR)/%.c=%.o))

# Separate the debug CFLAGS for test and other sources
CFLAGS_TEST := -c -I$(INC_DIR)
CFLAGS_DBG  := -c -g -I$(INC_DIR)
.PHONY: all debug test clean

all: $(EXE)

debug: $(DBG_EXE)

test: $(TEST_EXE)

$(EXE): $(OBJECTS) Makefile | $(BIN_DIR)
	$(CC) -o $@ $(OBJECTS) 2>&1

$(DBG_EXE): $(DBG_OBJECTS) Makefile | $(BIN_DIR)
	$(CC) -o $@ $(DBG_OBJECTS) 2>&1

$(TEST_EXE): $(TEST_OBJECTS) $(HEADERS) Makefile | $(BIN_DIR)
	$(CC) -o $@ $(TEST_OBJECTS) 2>&1

$(BUILD_DIR)/$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) Makefile | $(BUILD_DIR)/$(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(DBG_DIR)/$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) Makefile | $(BUILD_DIR)/$(DBG_DIR)/$(BIN_DIR)
	$(CC) $(CFLAGS_DBG) $< -o $@

$(BUILD_DIR)/$(TEST_DIR)/$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) Makefile | $(BUILD_DIR)/$(TEST_DIR)/$(BIN_DIR)
	$(CC) $(CFLAGS_TEST) $< -o $@

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(BUILD_DIR)/$(BIN_DIR):
	@mkdir -p $(BUILD_DIR)/$(BIN_DIR)

$(BUILD_DIR)/$(DBG_DIR)/$(BIN_DIR):
	@mkdir -p $(BUILD_DIR)/$(DBG_DIR)/$(BIN_DIR)

$(BUILD_DIR)/$(TEST_DIR)/$(BIN_DIR):
	@mkdir -p $(BUILD_DIR)/$(TEST_DIR)/$(BIN_DIR)

clean:
	rm -f -v *~ $(EXE) $(DBG_EXE) $(TEST_EXE) core
	@rm -rf $(BUILD_DIR)
