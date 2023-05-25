# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
TARGET_EXEC := capl
TEST_EXEC :=test

CC = gcc
CXX = g++
BUILD_DIR := ./build
SRC_DIRS := ./src
CXXFLAGS := -g
CFLAGS := -g


DATA_DIR := data
INC_DIR := inc
RELEASE_DIR := release
DOC_DIR := doc
All_DIR := $(SRC_DIRS) $(DATA_DIR) $(INC_DIR) $(RELEASE_DIR) $(DOC_DIR) $(BUILD_DIR)

SHARED ?= 1
ifeq ($(SHARED),1)
SHARED_FLAG := -shared
LIB_FLAG := -L$(BUILD_DIR) -l$(TARGET_EXEC)
TARGET_EXEC := $(addsuffix .dll,$(TARGET_EXEC))
endif

STATIC ?= 1
ifeq ($(STATIC),1)
STATIC_FLAG := -static
endif

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
HEADERS := $(patsubst $(SRCS_DIR)/%.h,$(INC_DIR)/%.h,$(shell find $(SRCS_DIR) -name '*.h')) 

# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(filter-out %/test.cpp.o,$(OBJS))
	$(CXX) $(filter-out %/test.cpp.o,$(OBJS)) -o $@ $(LDFLAGS) $(STATIC_FLAG) $(SHARED_FLAG)

$(BUILD_DIR)/$(TEST_EXEC): $(filter-out %/main.cpp.o,$(OBJS))
	$(CXX) $(filter-out %/main.cpp.o,$(OBJS)) -o $@ $(LDFLAGS) $(LIB_FLAG)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: test
test: $(BUILD_DIR)/$(TEST_EXEC)
	cd $(DATA_DIR)  && ../$(BUILD_DIR)/$(TEST_EXEC)

.PHONY: dir
dir: $(All_DIR)

$(All_DIR):
	mkdir $@

.PHONY: cpheaders
cpheaders: $(HEADERS)

$(INC_DIR)/%.h: $(SRCS_DIR)/%.h
	mkdir -p $(dir $@)
	cp $^ $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(wildcard $(DATA_DIR)/hex* $(DATA_DIR)/testlog $(DATA_DIR)/capldlllog)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)