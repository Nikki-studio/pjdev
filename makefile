BINARY := bin/pjdev
SOURCES := $(shell find src -name "*.cpp")
OBJECTS := $(SOURCES:src/%.cpp=obj/%.o)
DEPS := $(OBJECTS:.o=.d)

CXX := g++
CXXFLAGS  := -g -Iinclude -O3 -MMD -MP -Werror
LDFLAGS := -lncursesw

MAKEFLAGS := --no-builtin-rules --no-builtin--variables
NPROC := $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

.PHONY:all
all: $(BINARY)

$(BINARY): $(OBJECTS) | bin
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

obj/%.o:src/%.cpp include/%.hpp | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: install run clean

obj bin:
	mkdir -p $@

install:
	cp $(BINARY) /usr/local/$(BINARY)

run: $(BINARY)
	./$<

clean:
	rm -rf obj bin
	mkdir -p obj bin

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all     - Build the binary (default)"
	@echo "  clean   - Remove build artifacts"
	@echo "  run     - Build and run"
	@echo "  install - Install to /usr/local/bin"
	@echo "  help    - Show this help"
	@echo ""
	@echo "Build with: make -j$(NPROC)"