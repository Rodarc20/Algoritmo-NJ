nj.out: main.o nj.o nodo.o
	g++ -pthread -o nj.out -std=c++11 main.o nj.o nodo.o
main.o: main.cpp nj.h nodo.h
	g++ -pthread -std=c++11 -c main.cpp
nj.o: nj.cpp nj.h nodo.h
	g++ -pthread -std=c++11 -c nj.cpp
nodo.o: nodo.cpp nodo.h
	g++ -pthread -std=c++11 -c nodo.cpp
