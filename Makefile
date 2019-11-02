
CC = g++
CCFLAGS = -Wall -Werror -pthread -std=c++17
BINNAME = wordcount

.PHONY: clean compile
$(BINNAME):	compile
	@$(CC) $(CCFLAGS) distwc.cc threadpool.o mapreduce.o -o $@

compile:
	@$(CC) $(CCFLAGS) -c threadpool.cc -o threadpool.o
	@$(CC) $(CCFLAGS) -c mapreduce.cc -o mapreduce.o

run: $(BINNAME)
	@./wordcount

clean:
	@rm -rf wordcount threadpool.o mapreduce.o

compress:
	tar -caf $(BINNAME).tar.gz threadpool.cc mapreduce.cc threadpool.h mapreduce.h Makefile README.md
