
CXX = g++
CXXFLAGS = -std=c++20 $(shell pkg-config --cflags ayatana-appindicator3-0.1 gtk+-3.0)
LDFLAGS = $(shell pkg-config --libs ayatana-appindicator3-0.1 gtk+-3.0)
TARGET = build/spice
SRC = src/spice.cpp
OBJ = $(SRC:src/%.cpp=build/%.o)
 
all: $(TARGET)
 
build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
$(TARGET): $(OBJ) | build
	$(CXX) -o $@ $^ $(LDFLAGS)
 
build:
	mkdir -p build
 
clean:
	rm -rf build
 
.PHONY: all r clean
r:
	./build/spice
 