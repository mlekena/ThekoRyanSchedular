CC = gcc
FILES = main.c processes.c
OUT = schedule

build: $(FILES)
	$(CC) -Wall -g -o $(OUT) $(FILES)

clean:
	rm $(OUT) *.o
