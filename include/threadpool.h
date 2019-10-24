#ifndef THREADPOOL_H
#define THREADPOOL_H

#define MAX_THREADS 100000  // very theoretical, bitset needs initial some value

#include <pthread.h>
#include <queue>    // priority_queue
#include <vector>
#include <bitset>

typedef void (*thread_func_t)(void *arg);

struct FileObj{
    char* filename;
    int size;
    FileObj(char* fname, int sz):filename(fname), size(sz) {}
};

struct ThreadPool_work_t {
    thread_func_t func;              // The function pointer
    void *arg;                       // The arguments for the function
    // TODO: Add other members here if needed

    bool operator<(const ThreadPool_work_t& work) const {
        // TODO
        return true;
    };

};


struct ThreadPool_work_queue_t{
    // TODO: Add members here
    std::priority_queue<struct ThreadPool_work_t> pq;

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

};

struct ThreadPool_t{
    // TODO: Add members here
    std::vector<pthread_t> threads;
    std::bitset<MAX_THREADS> isActive;
    ThreadPool_t(){
        isActive.reset();
    }

    ThreadPool_t(int numThreads){
        threads.resize(numThreads);
        isActive.reset();
    }

    bool is_active(int idx){
        return isActive.test(idx);
    }
};


/**
* A C style constructor for creating a new ThreadPool object,
* assumes tp is empty and needs to be initialized
* Parameters:
*	  tp  - thread pool object passed by reference
*     num - The number of threads to create
*/
void ThreadPool_create(ThreadPool_t& tp, int num);

/**
* A C style destructor to destroy a ThreadPool object
* Parameters:
*     tp - The pointer to the ThreadPool object to be destroyed
*/
void ThreadPool_destroy(ThreadPool_t *tp);

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

#endif
