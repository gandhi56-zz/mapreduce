#ifndef THREADPOOL_H
#define THREADPOOL_H

#define FNAME_SIZE 64
#include <pthread.h>
#include <queue>    // priority_queue
#include <vector>
#include <string.h>
#include <stdio.h>

typedef void (*thread_func_t)(void *arg);

/*
 * file descriptor struct
 */
struct FileObj{
    char filename[FNAME_SIZE];
    int size;
    FileObj(){}
    void info(){
        printf("filename:%s size:%d\n", filename, size);
    }
};

/*
 * input data structure for the threadpool to work on,
 * pushed into a queue prioritizing jobs of larger size
 */
struct ThreadPool_work_t {
    thread_func_t func;
    FileObj arg;
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

/*
 * implementation of a work queue for the threadpool,
 * stores longest jobs first
 */
struct ThreadPool_work_queue_t{
    std::priority_queue<ThreadPool_work_t> pq;

    ThreadPool_work_queue_t(){}

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

/*
 * implementation of threadpool
 */
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
*/
void ThreadPool_create(ThreadPool_t& tp);
void* kill_job();
/**
* Add a task to the ThreadPool's task queue
* Parameters:
*     tp   - The ThreadPool object to add the task to
*/
void ThreadPool_add_work(ThreadPool_work_t work);

/**
* Run the next task from the task queue
* Parameters:
*     tp - The ThreadPool Object this thread belongs to
*/
void *Thread_run(ThreadPool_t* tp);
void *Thread_run_cv(ThreadPool_t* tp);
#endif
