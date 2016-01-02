gcc -c ttfs_volume.c
gcc tfs_format.o ttfs_volume.o ttfs_volume.h physical_layer.o logical_layer.o -o tfs_format

rm disk.tfs
echo "**************************************** création du disque **********************************************************"
./tfs_create -s 50 
echo "**************************************** partitionage du disque **********************************************************"
./tfs_partition -p 20 -p 10 -p 10  -p 9 disk.tfs
echo "**************************************** analyse du disque **********************************************************"
./tfs_analyze disk.tfs
echo "**************************************** formatage 1 du disque **********************************************************"
./tfs_format -p 1 -mf 4
echo "**************************************** formatage 2 du disque **********************************************************"
./tfs_format -p 2 -mf 7  
echo "**************************************** formatage 3 du disque **********************************************************"
./tfs_format -p 3 -mf 9 
echo "**************************************** formatage 4 du disque **********************************************************"
./tfs_format -p 4 -mf 5
