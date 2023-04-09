all: shell

shell: myshell.o LineParser.o
	gcc -g -m32 -Wall -o myshell myshell.o LineParser.o

myshell.o: myshell.c
	gcc -g -m32 -Wall -c myshell.c

LineParser.o: LineParser.c
	gcc -g -m32 -Wall -c LineParser.c


.PHONY: clean

clean:
	rm -f *.o out