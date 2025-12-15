CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
TARGET = echo_client
SRCS = echo_client.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o core

run: $(TARGET)
	./$(TARGET) 172.16.40.1 "Тестовое сообщение"

.PHONY: all clean run
