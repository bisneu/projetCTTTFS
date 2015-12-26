CC=cc
CFLAGS=-Wall

all: tfs_create tfs_partition tfs_analyze

tfs_create: logical_layer.o physical_layer.o tfs_create.o
	$(CC) -o $@ $^

tfs_partition: logical_layer.o physical_layer.o tfs_partition.o
	$(CC) -o $@ $^

tfs_analyze: logical_layer.o physical_layer.o tfs_analyze.o
	$(CC) -o $@ $^

tfs_format: ttfs_volume.o logical_layer.o physical_layer.o tfs_format.o
	$(CC) -o $@ $^

tfs_create.o: tfs_create.c logical_layer.h
	$(CC) -o $@ -c $< $(CFLAGS)

tfs_partition.o: tfs_partition.c logical_layer.h
	$(CC) -o $@ -c $< $(CFLAGS)

tfs_analyze.o: tfs_analyze.c logical_layer.h
	$(CC) -o $@ -c $< $(CFLAGS)

tfs_format.o: tfs_format.c ttfs_volume.h
	$(CC) -o $@ -c $< $(CFLAGS)

ttfs_volume.o: ttfs_volume.c ttfs_volume.h
	$(CC) -o $@ -c $< $(CFLAGS)

logical_layer.o: logical_layer.c logical_layer.h
	$(CC) -o $@ -c $< $(CFLAGS)

physical_layer.o: physical_layer.c physical_layer.h
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper tfs

clean:
	rm -rf *.o

mrproper: clean
	rm -rf tfs_create tfs_partition tfs_analyze

tfs:
	rm -rf *.tfs

