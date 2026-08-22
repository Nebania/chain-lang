CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I./include
LDFLAGS  := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TARGET   := chainlang
BINPATH  := $(abspath $(TARGET))
SRCDIR   := src
OBJDIR   := obj

ifeq (,$(findstring CHAIN_DISABLE_WEBVIEW,$(CXXFLAGS)))
    CXXFLAGS += $(shell pkg-config --cflags gtk+-3.0 webkit2gtk-4.1 javascriptcoregtk-4.1)
    LDFLAGS  += $(shell pkg-config --libs gtk+-3.0 webkit2gtk-4.1 javascriptcoregtk-4.1)
endif

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))

.PHONY: all build clean run debug release install

all: build

build: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	@$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@ 2>> error.txt && \
		echo "Build successful!" || \
		(echo "Build failed — see error.txt" && exit 1)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	@echo "  Compiling $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@ 2>> error.txt

$(OBJDIR):
	@mkdir -p $(OBJDIR)

debug: CXXFLAGS += -g -O0 -DDEBUG
debug: build

release: CXXFLAGS += -O2 -DNDEBUG
release: build

run: build
	@./$(TARGET)

install: release
	@ln -sf "$(BINPATH)" /usr/local/bin/chainlang
	@ln -sf "$(BINPATH)" /usr/local/bin/chain
	@echo "Linked /usr/local/bin/chainlang -> $(BINPATH)"
	@echo "Linked /usr/local/bin/chain -> $(BINPATH)"
	@echo "The binary stays in this folder — don't move or delete it, or the symlinks will break."

clean:
	@rm -rf $(OBJDIR) $(TARGET) error.txt
	@echo "Cleaned."
