
#ifndef THREADPOOL_H
	#include "../include/threadpool.h"
#endif
#include <stdio.h>


/**
* A C style constructor for creating a new ThreadPool object,
* assumes tp is empty and needs to be initialized
* Parameters:
*	  tp  - thread pool object passed by reference
*     num - The number of threads to create
*/
void ThreadPool_create(ThreadPool_t& tp, int num){
	tp.threads.resize(num);
	tp.isActive.reset();
}

/**
* A C style destructor to destroy a ThreadPool object
* Parameters:
*     tp - The pointer to the ThreadPool object to be destroyed
*/
void ThreadPool_destroy(ThreadPool_t *tp){
	// TODO
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
bool ThreadPool_add_work(ThreadPool_t *tp, thread_func_t func, void *arg);

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
void *Thread_run(ThreadPool_t *tp);
