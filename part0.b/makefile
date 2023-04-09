all: looper

looper: looper.o
	gcc -g -m32 -Wall -o looper looper.o

looper.o: looper.c
	gcc -g -m32 -Wall -c looper.c

.PHONY: clean

clean:
	rm -f *.o out