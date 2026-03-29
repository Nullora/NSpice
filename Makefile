CXX = g++
CXXFLAGS = -std=c++20 -Iinclude
TARGET_DIR = build

# Detect OS
ifeq ($(OS), Windows_NT)
    CXXFLAGS += -mwindows
    LDFLAGS += -mwindows
	SRC = src/spice.cpp src/spice_win.cpp
    TARGET = $(TARGET_DIR)/spice.exe
    MKDIR = mkdir $(TARGET_DIR)
    RM = rmdir /s /q $(TARGET_DIR)
    RUN = $(TARGET)
else
	SRC = src/spice.cpp src/spice_lin.cpp
    TARGET = $(TARGET_DIR)/spice
    MKDIR = mkdir -p $(TARGET_DIR)
    RM = rm -rf $(TARGET_DIR)
    RUN = $(TARGET)&
    CXXFLAGS += $(shell pkg-config --cflags ayatana-appindicator3-0.1 gtk+-3.0) 
    LDFLAGS += $(shell pkg-config --libs ayatana-appindicator3-0.1 gtk+-3.0)
endif

OBJ = $(SRC:src/%.cpp=$(TARGET_DIR)/%.o)

all: $(TARGET)

$(TARGET_DIR)/%.o: src/%.cpp | $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ) | $(TARGET_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(TARGET_DIR):
	$(MKDIR)

clean:
	$(RM)

install-deps:
	@if command -v pacman > /dev/null; then \
		sudo pacman -S libayatana-appindicator gtk3; \
	elif command -v apt > /dev/null; then \
		sudo apt install -y libayatana-appindicator3-dev libgtk-3-dev; \
	else \
		echo "Unsupported distro, install g++ and gtk3 manually"; \
	fi


r: all
	$(RUN)

compdb:
	bear -- make

.PHONY: all clean r compdb install-deps