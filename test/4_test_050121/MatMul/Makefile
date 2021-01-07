MAIN_FILENAME_1 = scalar.c
MAIN_FILENAME_2 = vectorial.c

OUTPUT_NAME_1 = scalar
OUTPUT_NAME_2 = vectorial

CC = gcc
STD = c11

DEBUG_OPT = -g

OPTIMIZATION_OPT = -O3 

all:
	$(CC) $(DEBUG_OPT) $(OPTIMIZATION_OPT) -march=native -o $(OUTPUT_NAME_1) $(MAIN_FILENAME_1)
	$(CC) $(DEBUG_OPT) $(OPTIMIZATION_OPT) -march=native -o $(OUTPUT_NAME_2) $(MAIN_FILENAME_2)

clean:
	rm $(OUTPUT_NAME_1)
	rm $(OUTPUT_NAME_2)