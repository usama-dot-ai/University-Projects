CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11 -Wno-deprecated-declarations
LDFLAGS = -lssl -lcrypto
TARGET = crypto_tool
SRC = crypto_tool.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
