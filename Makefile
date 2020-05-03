CC = gcc
CFLAGS = -I. -Wall -pedantic -std=gnu99
_OBJ = error.o helper.o player.o path.o game.o
DEPS = error.h helper.h player.h path.h game.h

.PHONY: all
.DEFAULT_GOAL := all

OBJ_DIR = obj

OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))
 
$(OBJ_DIR)/%.o: %.c $(DEPS)
	if ! [ -d obj ]; then mkdir -p obj; fi
	$(CC) -c $< -o $@ $(CFLAGS)

2310a: $(OBJ)
	$(CC) 2310a.c $^ -o $@ $(CFLAGS)

2310b: $(OBJ)
	$(CC) 2310b.c $^ -o $@ $(CFLAGS)

2310dealer: $(OBJ)
	$(CC) 2310dealer.c $^ -o $@ $(CFLAGS)

all: 2310a 2310b 2310dealer

clean:
	rm -rf obj
	rm 2310a
	rm 2310b
	rm 2310dealer
