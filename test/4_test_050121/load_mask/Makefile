MAIN_FILENAME_1 = vectorial.c
MAIN_FILENAME_2 = scalar.c

OBJ_NAME_1 = vectorial.o
OBJ_NAME_2 = scalar.o

OUTPUT_NAME_1 = vectorial
OUTPUT_NAME_2 = scalar

CC = gcc
STD = c11

DEBUG_OPT = 

OPTIMIZATION_OPT = -O0

#all: compile
#	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) $(OBJ_NAME_1) -o $(OUTPUT_NAME_1) 
#	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) $(OBJ_NAME_2) -o $(OUTPUT_NAME_2) 
all:
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) -o $(OUTPUT_NAME_1) $(MAIN_FILENAME_1)
	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) -o $(OUTPUT_NAME_2) $(MAIN_FILENAME_2)

#compile: $(MAIN_FILENAME)
#	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) -o $(OBJ_NAME_1) -c $(MAIN_FILENAME_1)
#	$(CC) -march=native $(DEBUG_OPT) $(OPTIMIZATION_OPT ) -std=$(STD) -o $(OBJ_NAME_2) -c $(MAIN_FILENAME_2)

clean:
	rm $(OBJ_NAME_1) $(OUTPUT_NAME_1) $(OBJ_NAME_2) $(OUTPUT_NAME_2)
