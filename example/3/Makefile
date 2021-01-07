MAIN_FILENAME_1 = vectorial.c
MAIN_FILENAME_2 = scalar.c

LIB = utils.c
OBJ_LIB = utils.o

OUTPUT_NAME_1 = vectorial
OUTPUT_NAME_2 = scalar

CC = gcc
STD = c11

DEBUG_OPT = 

OPTIMIZATION_OPT = -O0
all: lib
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) $(OBJ_LIB) $(MAIN_FILENAME_1) -o $(OUTPUT_NAME_1) 
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) $(OBJ_LIB) $(MAIN_FILENAME_2) -o $(OUTPUT_NAME_2) 

lib: $(LIB)
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) -o $(OBJ_LIB) -c $(LIB)

clean:
	rm $(OBJ_LIB) $(OUTPUT_NAME_1) $(OUTPUT_NAME_2)
