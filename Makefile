



test : test.o
	g++ test.o -o test -Llib -lgtest -lpthread

test.o : test.cxx scalable.h
	g++ -c test.cxx -o test.o -std=c++11 -I. -Iinclude 

.PHONY : clean

clean :
	rm -rf test test.o
