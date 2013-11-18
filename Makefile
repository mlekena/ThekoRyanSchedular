CC = gcc
FILES = main.c processes.c FCFS.c
OUT = schedule

build: $(FILES)
	$(CC) -Wall -g -o $(OUT) $(FILES)

clean:
	rm $(OUT) *.o
