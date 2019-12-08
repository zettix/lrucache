default_target: all

all: LRUCache_test

LRUCache_test: LRUCache.h LRUCache_test.cpp
	g++ -g LRUCache_test.cpp -o LRUCache_test

clean:
	rm -f *.gch *.o LRUCache_test
