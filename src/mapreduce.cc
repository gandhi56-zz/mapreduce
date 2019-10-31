#define FOO

#include "../include/threadpool.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <map>

// global variables
pthread_mutex_t partitionsMtx;
extern pthread_mutex_t mtx;
extern pthread_cond_t condVar;
std::vector< std::map<char*, std::vector<char*> > > partitions;

void MR_Run(int num_files, char *filenames[], Mapper map, int num_mappers, 
    Reducer concate, int num_reducers){
    
	// create partitions
	partitions.resize(num_reducers);
    
	// create and push all map jobs
    struct stat st;
    for (int i = 0; i < num_files; ++i){
        stat(filenames[i], &st);
        ThreadPool_work_t work(filenames[i], st.st_size, (thread_func_t)map);

        ThreadPool_add_work(work);
		//pthread_cond_signal(&condVar);
    }
	
	// create the mappers pool
    ThreadPool_t mapThreads(num_mappers);
    ThreadPool_create(mapThreads);

	// create and push self-destroy jobs
	//ThreadPool_destroy(mapThreads);
	clear_mem();
}

void MR_Emit(char *key, char *value){

	#ifdef FOO
		printf("[%d] recv %s\n", (int)pthread_self(), key);
	#endif

    pthread_mutex_lock(&mtx);
    int num_partitions = partitions.size();
    pthread_mutex_unlock(&mtx);

    unsigned long idx = MR_Partition(key, num_partitions);
    pthread_mutex_lock(&mtx);
    partitions[idx][key].push_back(value);
    pthread_mutex_unlock(&mtx);
}

unsigned long MR_Partition(char *key, int num_partitions){
    unsigned long hash = 5381;
    int c;
    while((c =*key++) != '\0')
        hash = hash*33 + c;
    return hash % num_partitions;
}

void MR_ProcessPartition(int partition_number){

}

char *MR_GetNext(char *key, int partition_number){
    return nullptr;
}
