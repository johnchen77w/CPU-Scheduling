all: scheduling 

scheduling: scheduling.c
	gcc -o scheduling.out scheduling.c
  
clean:
	-rm *.out
