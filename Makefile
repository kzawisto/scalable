



test : test.o
	g++ test.o -o test -Llib -lgtest -lpthread

test.o : test.cxx $(find include/scalable/* -type f)
	g++ -c test.cxx -o test.o -std=c++11 -Iinclude 

.PHONY : clean, purge, purge_gtest

clean :
	rm -rf test test.o

purge_gtest: 
	rm -rf include/gtest include/gmock lib
purge: clean purge_gtest
