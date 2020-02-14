#ifndef THREADPOOL_H
#define THREADPOOL_H

#define FNAME_SIZE 64

//#include "mapreduce.h"
#include <pthread.h>
#include <queue>    // priority_queue
#include <vector>
#include <string.h>
#include <stdio.h>

typedef void (*thread_func_t)(void *arg);

struct FileObj{
    char filename[FNAME_SIZE];
    int size;
    FileObj(){}
    void info(){
        printf("filename:%s size:%d\n", filename, size);
    }
};

struct ThreadPool_work_t {
    thread_func_t func;              // The function pointer
    FileObj arg;                       // The arguments for the function

    ThreadPool_work_t(){};
    ThreadPool_work_t(thread_func_t fun){
        func = fun;
    };

    ThreadPool_work_t(char* fname, int size, thread_func_t fun){
        strcpy(arg.filename, fname);
        arg.size = size;
        func = fun;
    }

    void info(){
        arg.info();
    }
    bool operator<(const ThreadPool_work_t& work) const {
        return arg.size < work.arg.size;
    }
};


struct ThreadPool_work_queue_t{
    std::priority_queue<ThreadPool_work_t> pq;

    ThreadPool_work_queue_t(){

    }

    void push_job(ThreadPool_work_t job){
        pq.push(job);
    }

    ThreadPool_work_t get_job(){
        ThreadPool_work_t job = pq.top();
        pq.pop();
        return job;
    }

    int size(){
        return pq.size();
    }

    void pop(){
        pq.pop();
    }

    bool empty(){
        return pq.empty();
    }

};

struct ThreadPool_t{
    std::vector<pthread_t> threads;
    ThreadPool_t(){}

    ThreadPool_t(int numThreads){
        threads.resize(numThreads);
    }
};


/**
* A C style constructor for creating a new ThreadPool object,
* Parameters:
*	  tp  - thread pool object passed by reference
*     num - The number of threads to create
*/
void ThreadPool_create(ThreadPool_t& tp);

/**
* A C style destructor to destroy a ThreadPool object
* Parameters:
*     tp - The pointer to the ThreadPool object to be destroyed
*/
void ThreadPool_destroy(ThreadPool_t& tp);

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
bool ThreadPool_add_work(ThreadPool_work_t work);

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
void *Thread_run(ThreadPool_t* tp);

void clear_mem();
#endif
