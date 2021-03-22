
#include "thread.h"

#include <assert.h>
#include <stdlib.h>

int atomic_init(atomic* t_atomic, unsigned long long int t_value) {
	
	assert(t_atomic);
	
	c89atomic_store_64(&t_atomic->value, t_value);
	
	return 1;
}

void atomic_final(atomic* t_atomic) {
	
	assert(t_atomic);
}

unsigned long long int atomic_increment(p_atomic t_atomic) {
	
	assert(t_atomic);
	
	return (unsigned long long int)c89atomic_fetch_add_64(&t_atomic->value, 1);
}

unsigned long long int atomic_decrement(p_atomic t_atomic) {
	
	assert(t_atomic);
	
	return (unsigned long long int)c89atomic_fetch_sub_64(&t_atomic->value, 1);
}

void atomic_set(p_atomic t_atomic, unsigned long long int t_value) {
	
	assert(t_atomic);
	
	c89atomic_store_64(&t_atomic->value, t_value);
}

unsigned long long int atomic_get(p_atomic t_atomic) {
	
	assert(t_atomic);
	
	return (unsigned long long int)c89atomic_load_64(&t_atomic->value);
}

#ifdef _WIN32

DWORD WINAPI thread_proc(LPVOID t_param) {
	
	thread* thread = (thread*)t_param;
	thread->func(thread->argument);
	
	return 0;
}

int thread_init(thread* t_thread, thread_func t_func, void* t_arg) {
	
	assert(t_thread && t_func);
	
	t_thread->func = t_func;
	t_thread->argument = t_arg;
	t_thread->thread = CreateThread(0, 0, thread_proc, t_thread, 0, 0);
	
	return t_thread->thread ? 1 : 0;
}

void thread_final(thread* t_thread) {
	
	assert(t_thread);
	
	CloseHandle(t_thread->thread);
}

void thread_join(p_thread t_thread) {
	
	assert(t_thread);
	
	WaitForSingleObject(t_thread->thread, INFINITE);
}

unsigned int thread_join_timeout(p_thread t_thread, unsigned long long int t_ms) {
	
	assert(t_thread);
	
	DWORD result = WaitForSingleObject(t_thread->thread, (DWORD)t_ms);
	return result == 0 ? 1 : 0;
}

void mutex_init(mutex* t_mutex, unsigned int t_lock_now) {
	
	assert(t_mutex);
	
	t_mutex->mutex = CreateMutex(0, t_lock_now, 0);
	return t_mutex->mutex ? 1 : 0;
}

void mutex_final(mutex* t_mutex) {
	
	assert(t_mutex);
	
	CloseHandle(t_mutex->mutex);
}

void mutex_wait(p_mutex t_mutex) {
	
	assert(t_mutex);
	
	WaitForSingleObject(t_mutex->mutex, INFINITE);
}

unsigned int mutex_wait_timeout(p_mutex t_mutex, unsigned long long int t_ms) {
	
	assert(t_mutex);
	
	DWORD result = WaitForSingleObject(t_mutex->mutex, (DWORD)t_ms);
	return result == 0 ? 1 : 0;
}

unsigned int mutex_try(p_mutex t_mutex) {
	
	assert(t_mutex);
	
	DWORD result = WaitForSingleObject(t_mutex->mutex, 0);
	return result == 0 ? 1 : 0;
}

void mutex_release(p_mutex t_mutex) {
	
	assert(t_mutex);
	
	ReleaseMutex(t_mutex->mutex);
}

int semaphore_init(semaphore* t_semaphore, unsigned long long int t_value, unsigned long long int t_limit) {
	
	assert(t_semaphore && t_limit);
	
	t_semaphore->semaphore = CreateSemaphore(0, (LONG)t_value, (LONG)t_limit, 0);
	return t_semaphore->semaphore ? 1 : 0;
}

void semaphore_final(semaphore* t_semaphore) {
	
	assert(t_semaphore);
	
	CloseHandle(t_semaphore->semaphore);
}

void semahpore_wait(p_semaphore t_semaphore) {
	
	assert(t_semaphore);
	
	WaitForSingleObject(t_semaphore->semaphore, INFINITE);
}


unsigned int semahpore_wait_timeout(p_semaphore t_semaphore, unsigned long long int t_ms) {
	
	assert(t_semaphore);
	
	DWORD result = WaitForSingleObject(t_semaphore->semaphore, (DWORD)t_ms);
	return result == 0 ? 1 : 0;
}

unsigned int semahpore_try(p_semaphore t_semaphore) {
	
	assert(t_semaphore);
	
	DWORD result = WaitForSingleObject(t_semaphore->semaphore, 0);
	return result == 0 ? 1 : 0;
}

void semaphore_signal(p_semaphore t_semaphore, unsigned long long int t_value) {
	
	assert(t_semaphore);
	
	ReleaseSemaphore(t_semaphore->semaphore, t_value, 0);
}

#elif defined(__linux__)

void* thread_proc(void* t_param) {
	
	assert(t_param);
	
	thread* thread = (thread*)t_param;
	thread->func(thread->argument);
	
	return 0;
}

int thread_init(thread* t_thread, thread_func t_func, void* t_arg) {
	
	assert(t_thread && t_func);
	
	t_thread->func = t_func;
	t_thread->argument = t_arg;
	pthread_create(&t_thread->thread, 0, thread_proc, t_thread); 
	return 1;
}

void thread_final(thread* t_thread) {
	
	assert(t_thread);
}

void thread_join(p_thread t_thread) {
	
	assert(t_thread);
	
	thread* thread = (thread*)t_thread;
	pthread_join(thread->thread, 0);
}

unsigned int thread_join_timeout(p_thread t_thread, unsigned long long int t_ms) {
	
	assert(t_thread);
	
	thread* thread = (thread*)t_thread;
	
	if (t_ms) {
	
		long int ms = (long int)(t_ms % 1000); 
		struct timespec timeout = { .tv_sec=(time_t)((t_ms - ms) / 1000), .tv_nsec=ms * 1000000 };
		return pthread_timedjoin_np(thread->thread, 0, &timeout) ? 0 : 1;
	}
	return pthread_tryjoin_np(thread->thread, 0) ? 0 : 1;
}

int mutex_init(mutex* t_mutex, unsigned int t_lock_now) {
	
	assert(t_mutex);
	
	pthread_mutex_init(&t_mutex->mutex, 0);
	if (t_lock_now) {
		
		pthread_mutex_lock(&t_mutex->mutex);
	}
	return t_mutex->mutex ? 1 : 0;
}

void mutex_final(mutex* t_mutex) {
	
	assert(t_mutex);
	
	pthread_mutex_destroy(&t_mutex->mutex);
}

void mutex_wait(p_mutex t_mutex) {
	
	assert(t_mutex);
	
	mutex* mutex = (mutex*)t_mutex;
	pthread_mutex_lock(&mutex->mutex);
}

unsigned int mutex_wait_timeout(p_mutex t_mutex, unsigned long long int t_ms) {
	
	assert(t_mutex);
	
	mutex* mutex = (mutex*)t_mutex;
	long int ms = (long int)(t_ms % 1000); 
	struct timespec timeout = { .tv_sec=(time_t)((t_ms - ms) / 1000), .tv_nsec=ms * 1000000 };
	return pthread_mutex_timedlock(&mutex->mutex, &timeout) ? 0 : 1;
}

unsigned int mutex_try(p_mutex t_mutex) {
	
	assert(t_mutex);
	
	mutex* mutex = (mutex*)t_mutex;
	return pthread_mutex_trylock(&mutex->mutex) ? 0 : 1;
}

void mutex_release(p_mutex t_mutex) {
	
	assert(t_mutex);
	
	mutex* mutex = (mutex*)t_mutex;
	pthread_mutex_unlock(&mutex->mutex);
}

int semaphore_init(semaphore* t_semaphore, unsigned long long int t_value, unsigned long long int t_limit) {
	
	assert(t_semaphore);
	assert(t_limit > 1 && "limit not supported by linux");
	
	if (sem_init(&t_semaphore->semaphore, 0, t_value) != 0)
	{
		return 0;
	}
	
	return 1;
}

void semaphore_final(semaphore* t_semaphore) {
	
	assert(t_semaphore);
	
	semaphore* semaphore = (semaphore*)t_semaphore;
	sem_destroy(&t_semaphore->semaphore);
}

void semahpore_wait(p_semaphore t_semaphore) {
	
	assert(t_semaphore);
	
	sem_wait(&t_semaphore->semaphore);
}

unsigned int semahpore_wait_timeout(p_semaphore t_semaphore, unsigned long long int t_ms) {
	
	assert(t_semaphore);
	
	semaphore* semaphore = (semaphore*)t_semaphore;
	long int ms = (long int)(t_ms % 1000); 
	struct timespec timeout = { .tv_sec=(time_t)((t_ms - ms) / 1000), .tv_nsec=ms * 1000000 };
	return sem_timedwait(&semaphore->semaphore, &timeout) ? 0 : 1;
}

unsigned int semahpore_try(p_semaphore t_semaphore) {
	
	assert(t_semaphore);
	
	semaphore* semaphore = (semaphore*)t_semaphore;
	return sem_trywait(&semaphore->semaphore) ? 0 : 1;
}

void semaphore_signal(p_semaphore t_semaphore, unsigned long long int t_value) {
	
	assert(t_semaphore);
	
	semaphore* semaphore = (semaphore*)t_semaphore;
	while (t_value) {
		
		--t_value;
		sem_post(&semaphore->semaphore);
	}
}

#endif