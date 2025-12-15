CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
TARGET = daytime_client
SRCS = daytime_client.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o core

run: $(TARGET)
	./$(TARGET) 172.16.40.1

.PHONY: all clean run
