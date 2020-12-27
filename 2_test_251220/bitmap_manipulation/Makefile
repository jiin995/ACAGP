MAIN_FILENAME_1 = mem_grayscale_converter.c
MAIN_FILENAME_2 = plain_grayscale_converter.c

OUTPUT_NAME_1 = mem_grayscale_converter
OUTPUT_NAME_2 = plain_grayscale_converter

CC = gcc
STD = c11

DEBUG_OPT = -g

OPTIMIZATION_OPT = -O0

all:
	$(CC) $(DEBUG_OPT) -o  $(OUTPUT_NAME_1) $(MAIN_FILENAME_1)
	$(CC) $(DEBUG_OPT) -o $(OUTPUT_NAME_2) $(MAIN_FILENAME_2)

clean:
	rm $(OUTPUT_NAME_1)
	rm $(OUTPUT_NAME_2)