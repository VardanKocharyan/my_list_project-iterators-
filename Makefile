# Compiler and flags
CXX      := clang++
CXXFLAGS := -Wall -Wextra -Werror -std=c++20 -Iinclude
# Target executable name
TARGET   := app
# Directories
SRC_DIR  := src
OBJ_DIR  := obj
# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
# Default target
all: $(TARGET)
# Link all object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@
# Compile each .cpp file into obj/*.o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
# Create obj/ directory if it does not exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
# Remove build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
.PHONY: all clean
