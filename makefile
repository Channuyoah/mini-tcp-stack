CXX = g++
CXXFLAGS = -Wall -std=c++17 -g

SRC_DIR = tcp
BUILD_DIR = build

TARGET = $(BUILD_DIR)/tcp_sim

SRCS = main.cpp \
       $(SRC_DIR)/state.cpp \
       $(SRC_DIR)/packet.cpp \
       $(SRC_DIR)/tcp.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)

.PHONY: all clean
