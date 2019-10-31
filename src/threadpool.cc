
#define FOO

#ifndef THREADPOOL_H
	#include "../include/threadpool.h"	// threadpool.h includes mapreduce.h
#endif
#include <stdio.h>

// Global variables
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVar = PTHREAD_COND_INITIALIZER;
ThreadPool_work_queue_t waitQueue;


/**
* A C style constructor for creating a new ThreadPool object
* Parameters:
*	  tp  - thread pool object passed by reference
*     num - The number of threads to create
*/
void ThreadPool_create(ThreadPool_t& tp){
	#ifdef FOO
		printf("creating %d threads...\n", (int)tp.threads.size());
	#endif
	for (uint16_t i = 0; i < tp.threads.size(); ++i){
		pthread_create(&tp.threads[i], NULL, [](void* pool) -> void*{
				Thread_run((ThreadPool_t*)pool);
				return NULL;
			}, &tp);
	}
}

void* kill_thread(){
	printf("[%d] kill \n", (int)pthread_self());
	pthread_exit(0);
}

/**
* A C style destructor to destroy a ThreadPool object
* Parameters:
*     tp - The pointer to the ThreadPool object to be destroyed
*/
void ThreadPool_destroy(ThreadPool_t& tp){
	for (uint16_t i = 0; i < tp.threads.size(); ++i){
		ThreadPool_add_work(ThreadPool_work_t((thread_func_t)kill_thread));
	}
}

/**
* Add a task to the ThreadPool's task queue
* Parameters:
*     tp   - The ThreadPool object to add the task to
*     func - The function pointer that will be called in the thread
*     arg  - The arguments for the function
* Return:
*     true  - If successful
*     false - Otherwise
*/
bool ThreadPool_add_work(ThreadPool_work_t work){
	printf("pushing job %s...\n", work.arg.filename);
	pthread_mutex_lock(&mtx);
	waitQueue.push_job(work);
	pthread_mutex_unlock(&mtx);
	return true;
}

/**
* Get a task from the given ThreadPool object
* Parameters:
*     tp - The ThreadPool object being passed
* Return:
*     ThreadPool_work_t* - The next task to run
*/
ThreadPool_work_t *ThreadPool_get_work(ThreadPool_t *tp);

/**
* Run the next task from the task queue
* Parameters:
*     tp - The ThreadPool Object this thread belongs to
*/
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
		
		// execute map
		work.func(work.arg.filename);
	}
	pthread_mutex_unlock(&mtx);
	#ifdef FOO
		printf("[%d] kill \n", (int)pthread_self());
	#endif
	pthread_exit(0);
	return NULL;
}

void clear_mem(){
	pthread_mutex_lock(&mtx);
	while (!waitQueue.empty()){
		waitQueue.pop();
	}
	pthread_mutex_unlock(&mtx);
}
