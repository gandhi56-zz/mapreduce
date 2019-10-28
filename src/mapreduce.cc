
#define debug

#include "../include/threadpool.h"

#include <sys/types.h>
#include <sys/stat.h>

void MR_Run(int num_files, char *filenames[],
            Mapper map, int num_mappers,
            Reducer concate, int num_reducers){

    ThreadPool_work_queue_t waitQueue;

    // push all jobs
    struct stat st;
    for (int i = 0; i < num_files; ++i){
        #ifdef debug
            printf("parsing %s\n", filenames[i]);
        #endif
        stat(filenames[i], &st);

        // create a job
        ThreadPool_work_t work(filenames[i], st.st_size, (thread_func_t)map);
        waitQueue.push_job(work);
    }

    // create the pools
    ThreadPool_t mapThreads(num_mappers);
    ThreadPool_create(mapThreads);
}

void MR_Emit(char *key, char *value){}

unsigned long MR_Partition(char *key, int num_partitions){
    return 0;
}

void MR_ProcessPartition(int partition_number){}

char *MR_GetNext(char *key, int partition_number){
    return nullptr;
}