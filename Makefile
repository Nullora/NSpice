CXX = g++
CXXFLAGS = -std=c++20 -Iinclude
TARGET_DIR = build

ifeq ($(OS), Windows_NT)
    CXXFLAGS += -mwindows
    LDFLAGS += -mwindows -static -static-libgcc -static-libstdc++
    SRC = src/spice.cpp src/spice_win.cpp
    TARGET = $(TARGET_DIR)/spice.exe
    MKDIR = mkdir $(TARGET_DIR)
    RM = del /f /q $(TARGET_DIR)\spice.exe
    RUN = $(TARGET)
else
    SRC = src/spice.cpp src/spice_lin.cpp
    TARGET = $(TARGET_DIR)/spice
    MKDIR = mkdir -p $(TARGET_DIR)
    RM = rm -f $(TARGET_DIR)/spice
    RUN = $(TARGET)&
    CXXFLAGS += $(shell pkg-config --cflags ayatana-appindicator3-0.1 gtk+-3.0)
    LDFLAGS += $(shell pkg-config --libs ayatana-appindicator3-0.1 gtk+-3.0)
endif

OBJ = $(patsubst src/%.cpp, $(TARGET_DIR)/%.o, $(SRC))

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

windep:
	copy build\spice.exe prebuilt_binaries_win
	copy build\libwinpthread-1.dll prebuilt_binaries_win

usb:
	copy build\spice.exe D:\NSpice

.PHONY: all clean r install-deps windep usb