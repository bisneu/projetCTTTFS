CC=cc
CFLAGS=-Wall
LDFLAGS=
EXEC=tfs_create
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

physical_layer.o: physical_layer.h
logical_layer.o: logical_layer.h
tfs_create.o: tfs_create.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
