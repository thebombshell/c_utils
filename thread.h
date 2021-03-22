
/**
 * thread.h
 */

#ifndef C_UTIL_THREAD_H
#define C_UTIL_THREAD_H

#include "c89atomic.h"
#include "data_structures.h"
#include <stdlib.h>

#if defined(_WIN32)

#include <windows.h>

#elif defined(__linux__)

#include <pthread.h>
#include <semaphore.h>

#endif

typedef void (*thread_func)(void* t_arg);

typedef struct {
	
#if defined(_WIN32)

	HANDLE thread;
	
#elif defined(__linux__)

	pthread_t thread;
	
#endif

	thread_func func;
	void* argument;
} thread, *p_thread;

int thread_init(thread* t_thread, thread_func t_func, void* t_arg);

void thread_final(thread* t_thread);

void thread_join(p_thread t_thread);

unsigned int thread_join_timeout(p_thread t_thread, unsigned long long int t_ms);

typedef struct {
	
	c89atomic_uint64 value;
	
} atomic, *p_atomic;

int atomic_init(atomic* t_atmoic, unsigned long long int t_value);

void atomic_final(atomic* t_atomic);

unsigned long long int atomic_increment(p_atomic t_atomic);

unsigned long long int atomic_decrement(p_atomic t_atomic);

void atomic_set(p_atomic t_atomic, unsigned long long int t_value);

unsigned long long int atomic_get(p_atomic t_atomic);

typedef struct {
	
#if defined(_WIN32)

	HANDLE mutex;
	
#elif defined(__linux__)
	
	pthread_mutex_t mutex;
	
#endif
	
} mutex, *p_mutex;

int mutex_init(mutex* t_mutex, unsigned int t_lock_now);

void mutex_final(mutex* t_mutex);

void mutex_wait(p_mutex t_mutex);

unsigned int mutex_wait_timeout(p_mutex t_mutex, unsigned long long int t_ms);

unsigned int mutex_try(p_mutex t_mutex);

void mutex_release(p_mutex t_mutex);

typedef struct {
	
#if defined(_WIN32)
	
	HANDLE semaphore;
	
#elif defined(__linux__)
	
	sem_t semaphore;
	
#endif
	
} semaphore, *p_semaphore;

int semaphore_init(semaphore* t_semaphore, unsigned long long int t_value, unsigned long long int t_limit);

void semaphore_final(semaphore* t_semaphore);

void semahpore_wait(p_semaphore t_semaphore);

unsigned int semahpore_wait_timeout(p_semaphore t_semaphore, unsigned long long int t_ms);

unsigned int semahpore_try(p_semaphore t_semaphore);

void semaphore_signal(p_semaphore t_semaphore, unsigned long long int t_value);

typedef struct {
	
	vector vector;
	semaphore semahpore;
	
} queue, *p_queue;

int queue_init(queue* t_queue, unsigned long long int t_);

#endif