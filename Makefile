TARGET := capl
TARGET_TEST	:= test.exe
CC = i686-w64-mingw32-gcc
CXX = i686-w64-mingw32-g++
COMPILER := $(CXX)

SRCS_DIR := src
BUILD_DIR := build
DATA_DIR := data
INC_DIR := inc
RELEASE_DIR := release
DOC_DIR := doc

SHARED := 1
ifeq ($(SHARED),1)
SHARED_FLAG := -shared
LINK_FLAG := -L$(BUILD_DIR) -l$(TARGET)
TARGET := $(addsuffix .dll,$(TARGET))
else
TARGET := $(addsuffix .exe,$(TARGET))
endif

STATIC := 1
ifeq ($(STATIC),1)
STATIC_FLAG := -static
endif

All_DIR := $(SRCS_DIR) $(DATA_DIR) $(INC_DIR) $(RELEASE_DIR) $(DOC_DIR) $(BUILD_DIR)

INC_DIRS := $(patsubst %,-I%,$(shell find $(SRCS_DIR) -type d))
$(info All inc dirs: $(INC_DIRS))


SRCS := $(shell find $(SRCS_DIR) -name '*.c' -or -name '*.cpp' -or -name '*.S')
$(info All srcs: $(SRCS))

HEADERS := $(patsubst $(SRCS_DIR)/%.h,$(INC_DIR)/%.h,$(shell find $(SRCS_DIR) -name '*.h')) 

SRCS_O := $(patsubst $(SRCS_DIR)/%,$(BUILD_DIR)/%.o,$(SRCS))
$(info All objects: $(SRCS_O))

# DEPS := $(SRCS_O:.o=.d)

TARGET_O := $(filter-out %/test.c.o,$(SRCS_O))
$(info All target objects: $(TARGET_O))

TARGET_TEST_O := $(filter-out %/main.c.o,$(SRCS_O))
$(info All target_test objects: $(TARGET_TEST_O))

.default: $(BUILD_DIR)/$(TARGET)
$(BUILD_DIR)/$(TARGET): $(BUILD_DIR) $(TARGET_O) 
	$(COMPILER) $(TARGET_O)  -o$@ $(STATIC_FLAG) $(SHARED_FLAG)

$(BUILD_DIR)/$(TARGET_TEST): $(BUILD_DIR) $(TARGET_TEST_O) 
	$(COMPILER) $(TARGET_TEST_O) -o$@ $(LINK_FLAG)

$(BUILD_DIR)/%.o: $(SRCS_DIR)/%
	mkdir -p $(dir $@)
	$(COMPILER) $^ -o $@ $(INC_DIRS) -c


.PHONY: test
test: $(BUILD_DIR)/$(TARGET_TEST)
	cd $(DATA_DIR)  && ../$(BUILD_DIR)/$(TARGET_TEST) 

.PHONY: dir
dir: $(All_DIR)

$(All_DIR):
	mkdir $@

.PHONY: cpheaders
cpheaders: $(HEADERS)

$(INC_DIR)/%.h: $(SRCS_DIR)/%.h
	mkdir -p $(dir $@)
	cp $^ $@

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(wildcard $(DATA_DIR)/hex*) $(DATA_DIR)/testlog $(DATA_DIR)/capldlllog
	
# -include $(DEPS)