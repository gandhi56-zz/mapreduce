# mapreduce

## Modifications
* changed threadpool function types for speedup
* removed ThreadPool_destroy function, each thread is destroyed as soon as there is no job left for it to work on
* ThreadPool_add_work is changed from being a bool function to a void function
* ThreadPool_work_t *ThreadPool_get_work(ThreadPool_t *tp) is removed, instead writing a method within the ThreadPool_work_queue_t struct
