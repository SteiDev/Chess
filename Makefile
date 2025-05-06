CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -pedantic-errors -Waddress -Warray-bounds -Warray-parameter -Wformat -Wimplicit -Winfinite-recursion -Wmaybe-uninitialized -Wshift-negative-value
OBJ = obj/main.o obj/chessboardlogic.o obj/pieceMoves.o obj/gentools.o
EXENAME = chessboard

all: $(OBJ)
	@$(CC) $(OBJ) -o bin/$(EXENAME)
	@@echo Files compilled successfully!

exe: $(OBJ)
	@$(CC) $(OBJ) -o bin/$(EXENAME)
	@rm $(OBJ)
	@@echo Executable compilled successfully!

obj/gentools.o: src/gentools.c
	@$(CC) $(CFLAGS) -c $^ -o $@

obj/pieceMoves.o: src/pieceMoves.c
	@$(CC) $(CFLAGS) -c $^ -o $@

obj/chessboardlogic.o: src/chessboardlogic.c
	@$(CC) $(CFLAGS) -c $^ -o $@
obj/main.o: src/main.c
	@$(CC) $(CFLAGS) -c $^ -o $@

clear:
	@rm $(OBJ) bin/$(EXENAME)
	@@echo Files cleared successfully!