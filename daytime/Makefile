CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET = daytime_client

all: $(TARGET)

$(TARGET): daytime_client.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $<

clean:
	rm -f $(TARGET) *.o
