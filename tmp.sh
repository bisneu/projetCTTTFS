gcc -c ttfs_volume.c
gcc tfs_format.o ttfs_volume.o ttfs_volume.h physical_layer.o logical_layer.o -o tfs_format

rm disk.tfs
./tfs_create -s 20 
./tfs_partition -p 10 disk.tfs
./tfs_analyze disk.tfs

