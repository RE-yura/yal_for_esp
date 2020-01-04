CC = g++
all: compile1 compile2

compile1: main.cpp
	$(CC) -std=c++11 main.cpp -o main.o

compile2: ktl_test.cpp
	$(CC) -std=c++11 ktl_test.cpp -o ktl_test.o -lktl 


