//#define FOO

#include "../include/threadpool.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <map>
#include <unistd.h>	// sleep

// global variables
pthread_mutex_t partitionsMtx;
extern pthread_mutex_t mtx;
extern pthread_cond_t condVar;
std::vector<std::multimap<std::string, std::string> > partitions;

// wrapper function declaration
void* MR_reduce_partition(int* partition_number);

void show_partitions(){
	for (uint16_t idx = 0; idx < partitions.size(); ++idx){
		printf("[%d]", idx);
		for (auto it = partitions[idx].begin(); it != partitions[idx].end(); ++it){
			printf("%s ", it->first.c_str());
		}
		printf("\n");
	}
}

void MR_Run(int num_files, char *filenames[], Mapper map, int num_mappers, 
    Reducer concate, int num_reducers){
    
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

	// detach all mapper threads
	for (auto& thread : mapThreads.threads){
		pthread_join(thread, NULL);
	}

	// create and push self-destroy jobs
	//ThreadPool_destroy(mapThreads);
	clear_mem();
	show_partitions();

	// create reducer threads
	std::vector<pthread_t> reduceThreads(num_reducers);
/*
	for (int idx = 0; idx < num_reducers; ++idx){
		pthread_create(&reduceThreads[idx], NULL, [](int* i) -> void*{
				MR_reduce_partition(i);
				return NULL;
			}, &idx);
	
	}
*/
}

void MR_Emit(char *key, char *value){
    pthread_mutex_lock(&mtx);
    int num_partitions = partitions.size();

    unsigned long idx = MR_Partition(key, num_partitions);
    partitions[idx].insert(std::make_pair(std::string(key), std::string(value)));
	#ifdef FOO
		printf("[%lu] recv %s\n", idx, key);
	#endif
    pthread_mutex_unlock(&mtx);
}

unsigned long MR_Partition(char *key, int num_partitions){
    unsigned long hash = 5381;
    int c;
    while((c =*key++) != '\0')
        hash = hash*33 + c;
    return hash % num_partitions;
}

void* MR_reduce_partition(int* partition_number){
	MR_ProcessPartition(*partition_number);
	return NULL;
}

void MR_ProcessPartition(int partition_number){
	for (auto it = partitions[partition_number].begin(); it != partitions[partition_number].end(); ++it){
		// call reduce here
	}
}

char *MR_GetNext(char *key, int partition_number){
	std::string strkey = std::string(key);
	std::string value = partitions[partition_number].find(strkey)->second;
    return nullptr;
}
