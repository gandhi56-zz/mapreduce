
CC = g++
CCFLAGS = -Wall -Werror -pthread -std=c++17
SOURCES = $(wildcard ./src/*.cpp)
OBJECTS = $(SOURCES:./src/%.cpp=./obj/%.o)
BINNAME = mapreduce

.PHONY: clean compile

$(BINNAME): compile
	$(CC) $(CCFLAGS) $(OBJECTS) -o $@

compile: $(OBJECTS)

$(OBJECTS): $(SOURCES)
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) $< -c -o $@

clean:
	@rm -rf $(OBJECTS) $(BINNAME)

compress:
	tar -caf $(BINNAME).tar.gz $(SOURCES) Makefile README.md
