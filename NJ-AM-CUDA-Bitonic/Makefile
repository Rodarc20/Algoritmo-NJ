nj.out: main.o nj.o nodo.o
	nvcc -o nj.out main.o nj.o nodo.o
main.o: main.cu nj.h nodo.h
	nvcc -c main.cu
nj.o: nj.cu nj.h nodo.h
	nvcc -c nj.cu
nodo.o: nodo.cpp nodo.h
	nvcc -c nodo.cpp
