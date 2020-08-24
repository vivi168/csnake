CC=x86_64-w64-mingw32-gcc
SRC=main.c draw.c
DEP=draw.h
LFLAGS=-g -O2 -Wall -Wextra -lm -lmingw32 -lSDL2main -lSDL2
SDLFLAGS=-I SDL2-2.0.12/x86_64-w64-mingw32/include/SDL2 -L SDL2-2.0.12/x86_64-w64-mingw32/lib
EXEC=snake

$(EXEC): $(SRC) $(DEP)
	$(CC) -o $@ $(SRC) $(CFLAGS) $(LFLAGS) $(SDLFLAGS)

.PHONY: clean

clean:
	rm $(EXEC)
