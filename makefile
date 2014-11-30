all: dictionary

dictionary:  dictionary.o bst.o rbt.o
	g++ dictionary.o bst.o rbt.o -o dictionary

dictionary.o: dictionary.cpp dictionary.h
	g++ -c dictionary.cpp 

bst.o: bst.cpp dictionary.h
	g++ -c bst.cpp

rbt.o: rbt.cpp dictionary.h
	g++ -c rbt.cpp

clean:
	rm -f *.o *~  dictionary
