
CC = g++
CCFLAGS = -Wall -Werror -pthread -std=c++17
BINNAME = wordcount
OBJ = obj/
SRC = src/

.PHONY: clean compile
$(BINNAME):	compile
	@$(CC) $(CCFLAGS) $(SRC)distwc.cc $(OBJ)threadpool.o $(OBJ)mapreduce.o -o $@

compile:
	@mkdir -p $(OBJ)
	@$(CC) $(CCFLAGS) -c $(SRC)threadpool.cc -o $(OBJ)threadpool.o
	@$(CC) $(CCFLAGS) -c $(SRC)mapreduce.cc -o $(OBJ)mapreduce.o

run: $(BINNAME)
	@./wordcount

clean:
	@rm -rf $(OBJ)

compress:
	tar -caf $(BINNAME).tar.gz $(SOURCES) Makefile README.md
