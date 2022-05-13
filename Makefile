# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
TARGET_DLL := capl.dll
TARGET_TEST	:= test.exe
CC = i686-w64-mingw32-gcc
CXX = i686-w64-mingw32-g++


SRCS_DIR := Sources
BUILD_DIR := Build
TEST_DIR := Test
COMMON_SRCS := $(SRCS_DIR)/crc.c $(SRCS_DIR)/filepraser.c $(SRCS_DIR)/minilogger.c

.PHONY: capl
capl: $(BUILD_DIR)
	$(CXX) $(COMMON_SRCS) $(SRCS_DIR)/capldll.cpp -IIncludes -o $(BUILD_DIR)/$(TARGET_DLL) -shared -static 

.PHONY: test
test: $(BUILD_DIR)
	$(CXX) $(COMMON_SRCS) $(SRCS_DIR)/test.c -IIncludes -o $(BUILD_DIR)/$(TARGET_TEST) -Lbuild -lcapl
	cd $(TEST_DIR)  && ../$(BUILD_DIR)/$(TARGET_TEST) 

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -f  $(wildcard $(TEST_DIR)/hex*) $(TEST_DIR)/capldlllog $(TEST_DIR)/testlog