main: main.o int_tree.o
	gcc -o main main.o int_tree.o
main.o: main.c int_tree.c int_tree.h
	gcc -c main.c -o main.o
int_tree.o: int_tree.c int_tree.h
	gcc -c int_tree.c -o int_tree.o
clean:
	rm *.o