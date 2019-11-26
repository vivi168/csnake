CC=cc
SRC=main.c draw.c
DEP=draw.h
LFLAGS=-lm -lSDL2 -lSDL2_image -g
EXEC=snake

$(EXEC): $(SRC) $(DEP)
	$(CC) -o $@ $(SRC) $(CFLAGS) $(LFLAGS)

.PHONY: clean

clean:
	rm $(EXEC)
