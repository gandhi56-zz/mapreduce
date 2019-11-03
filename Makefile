
CC = g++
CCFLAGS = -Wall -Werror -pthread -std=c++17
BINNAME = wc

.PHONY: clean compile
wc:	compile
	@$(CC) $(CCFLAGS) distwc.cc threadpool.o mapreduce.o -o wordcount

compile:
	@$(CC) $(CCFLAGS) -c threadpool.cc -o threadpool.o
	@$(CC) $(CCFLAGS) -c mapreduce.cc -o mapreduce.o

run: $(BINNAME)
	@./wc

clean:
	@rm -rf wordcount threadpool.o mapreduce.o

compress:
	tar -caf mapreduce.tar.gz threadpool.cc mapreduce.cc threadpool.h mapreduce.h Makefile README.md
