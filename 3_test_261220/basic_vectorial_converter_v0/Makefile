MAIN_FILENAME = mem_grayscale_converter.c
MAIN_OBJ_NAME = mem_grayscale_converter.o

OUTPUT_NAME = mem_grayscale_converter

CC = gcc
STD = c11

DEBUG_OPT = -g

OPTIMIZATION_OPT = -O0

all:compile
	$(CC) -march=native $(OPTIMIZATION_OPT ) -std=$(STD) $(MAIN_OBJ_NAME) -o $(OUTPUT_NAME) -rdynamic -ldl -lpthread

compile: $(MAIN_FILENAME)
	$(CC) -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGE_FILES -march=native  $(OPTIMIZATION_OPT ) -std=$(STD) -o $(MAIN_OBJ_NAME) -c $(MAIN_FILENAME)

clean:
	rm $(MAIN_OBJ_NAME) $(OUTPUT_NAME)