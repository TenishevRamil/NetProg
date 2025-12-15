CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET = echo_client

all: $(TARGET)

$(TARGET): echo_client.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $<

clean:
	rm -f $(TARGET) *.o
