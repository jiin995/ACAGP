MAIN_FILENAME_1 = scalar.c
MAIN_FILENAME_2 = vectorial_1.c
MAIN_FILENAME_3 = vectorial_2.c
MAIN_FILENAME_4 = vectorial_3.c

OUTPUT_NAME_1 = scalar
OUTPUT_NAME_2 = vectorial_1
OUTPUT_NAME_3 = vectorial_2
OUTPUT_NAME_4 = vectorial_3

CC = gcc
STD = c11

DEBUG_OPT = 

OPTIMIZATION_OPT = -O3

all:
	$(CC) $(DEBUG_OPT) -march=native -o $(OUTPUT_NAME_1) $(MAIN_FILENAME_1)
	#$(CC) $(DEBUG_OPT) -march=native -o $(OUTPUT_NAME_2) $(MAIN_FILENAME_2)
	$(CC) $(DEBUG_OPT) -march=native -o $(OUTPUT_NAME_3) $(MAIN_FILENAME_3)
	$(CC) $(DEBUG_OPT) -march=native -o $(OUTPUT_NAME_4) $(MAIN_FILENAME_4)

clean:
	rm $(OUTPUT_NAME_1)
	rm $(OUTPUT_NAME_2)