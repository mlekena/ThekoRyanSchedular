CC = gcc
FILES = main.c processes.c FCFS.c RunningProcesses.c RR.c SRT.c MLQ.c
OUT = schedule

build: $(FILES)
	$(CC) -Wall -g -o $(OUT) $(FILES)

clean:
	rm $(OUT) *.o
