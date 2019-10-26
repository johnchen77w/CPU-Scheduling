all: scheduling 

part1: scheduling.c
	gcc -o scheduling.out scheduling.c
  
clean:
	-rm *.out
