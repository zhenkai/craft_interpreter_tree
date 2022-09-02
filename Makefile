CC = clang++
SRCS := $(shell find . -name '*.cpp')
BUILD_DIR := ./build
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
CXXFLAGS = -std=c++20
TARGET = lox

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean: 
	-rm -rf $(BUILD_DIR) $(TARGET)
