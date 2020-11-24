MAIN_FILENAME_1 = aligned.c
MAIN_FILENAME_2 = misaligned.c

OBJ_NAME_1 = aligned.o
OBJ_NAME_2 = misaligned.o

OUTPUT_NAME_1 = aligned
OUTPUT_NAME_2 = misaligned

CC = gcc
STD = c11

DEBUG_OPT = -g

OPTIMIZATION_OPT = -O0

all: compile
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) $(OBJ_NAME_1) -o $(OUTPUT_NAME_1) -rdynamic -ldl -lpthread
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) $(OBJ_NAME_2) -o $(OUTPUT_NAME_2) -rdynamic -ldl -lpthread

compile: $(MAIN_FILENAME)
	$(CC) -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGE_FILES -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) -o $(OBJ_NAME_1) -c $(MAIN_FILENAME_1)
	$(CC) -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGE_FILES -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) -o $(OBJ_NAME_2) -c $(MAIN_FILENAME_2)

clean:
	rm $(OBJ_NAME_1) $(OUTPUT_NAME_1) $(OBJ_NAME_2) $(OUTPUT_NAME_2)