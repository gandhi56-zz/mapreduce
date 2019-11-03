
#define FOO

#include "threadpool.h"
#include <stdio.h>

// Global variables
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVar = PTHREAD_COND_INITIALIZER;
ThreadPool_work_queue_t waitQueue;

// Function definitions
void ThreadPool_create(ThreadPool_t& tp){
	#ifdef FOO
		printf("creating %d threads...\n", (int)tp.threads.size());
	#endif
	for (unsigned short i = 0; i < tp.threads.size(); ++i){
		pthread_create(&tp.threads[i], nullptr, [](void* pool) -> void*{
				Thread_run_cv((ThreadPool_t*)pool);
				return nullptr;
			}, &tp);
	}
}

void* kill_job(){
	#ifdef FOO
		printf("[%d] kill \n", (int)pthread_self());
	#endif
	
	pthread_exit(0);
	return nullptr;
}

void ThreadPool_add_work(ThreadPool_work_t work){
	pthread_mutex_lock(&mtx);
	waitQueue.push_job(work);
	pthread_mutex_unlock(&mtx);
}

void *Thread_run(ThreadPool_t* tp){
	#ifdef FOO
		printf("[%d] init \n", (int)pthread_self());
	#endif
	ThreadPool_work_t work;
	while (1){
		pthread_mutex_lock(&mtx);
		if (waitQueue.size() == 0)	break;
		work = waitQueue.get_job();
		pthread_mutex_unlock(&mtx);
		work.func(work.arg.filename);
	}
	pthread_mutex_unlock(&mtx);
	#ifdef FOO
		printf("[%d] kill \n", (int)pthread_self());
	#endif
	pthread_exit(0);
	return nullptr;
}

void *Thread_run_cv(ThreadPool_t* tp){
	#ifdef FOO
		printf("[%d] init \n", (int)pthread_self());
	#endif
	ThreadPool_work_t job;
	pthread_mutex_lock(&mtx);
	printf("$%d\n", waitQueue.size());
	while (waitQueue.size() == 0){
		pthread_cond_wait(&condVar, &mtx);
	}
	job = waitQueue.get_job();
	pthread_mutex_unlock(&mtx);
	job.func(job.arg.filename);
	
	return nullptr;
}
