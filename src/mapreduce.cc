
#include "../include/threadpool.h"
#include "../include/mapreduce.h"

#include <sys/types.h>
#include <sys/stat.h>

void MR_Run(int num_files, char *filenames[],
            Mapper map, int num_mappers,
            Reducer concate, int num_reducers){

    ThreadPool_t mapThreads;
    ThreadPool_t reduceThreads;
    ThreadPool_create(mapThreads, num_mappers);
    ThreadPool_create(reduceThreads, num_reducers);

    struct stat sb;
    for (int i = 0; i < num_files; ++i){
        lstat(filenames[i], &sb);
        FileObj fo(filenames[i], sb.st_size);
    }

}

void MR_Emit(char *key, char *value){}

unsigned long MR_Partition(char *key, int num_partitions){
    return 0;
}

void MR_ProcessPartition(int partition_number){}

char *MR_GetNext(char *key, int partition_number){
    return nullptr;
}