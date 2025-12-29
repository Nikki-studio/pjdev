BINARY := bin/pjdev
SOURCES := $(shell find src -name "*.cpp")
OBJECTS := $(patsubst src/%.cpp , obj/%.o, $(SOURCES))
FLAGS  := -g -Iinclude -O3 -MMD -MP -Werror
LDFLAGS := -lncursesw

$(BINARY): $(OBJECTS) | obj
	g++ $(FLAGS) $^ -o $@ $(LDFLAGS)

obj/%.o:src/%.cpp include/%.hpp
	g++ $(FLAGS) -c $< -o $@ $(LDFLAGS)

obj bin:
	mkdir -p $@

install:
	cp $(BINARY) /usr/$(BINARY)

run: $(BINARY)
	./$<

clean: 
	rm -rf obj bin
	mkdir -p obj bin
