#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
    union {
        volatile int  word;
        volatile char byte;
    };     
    pthread_mutex_t mutex;
    pthread_cond_t cond;  
      
} rwlock_t;

rwlock_t * rwlock_create()
{
    rwlock_t * lock = malloc(sizeof(rwlock_t));
    lock->word = 0;
    pthread_cond_init(&lock->cond, NULL);
    pthread_mutex_init(&lock->mutex, NULL);
    return lock;
}

void thread_sleep(rwlock_t * lock)
{
    pthread_mutex_lock(&lock->mutex);
    pthread_cond_wait(&lock->cond, &lock->mutex);
    pthread_mutex_unlock(&lock->mutex);
}

void thread_wakeup(rwlock_t * lock)
{
    pthread_cond_broadcast(&lock->cond);
}

void write_lock(rwlock_t * lock)
{
    while (__sync_fetch_and_or(&lock->byte, 1)) {
        thread_sleep(lock);
    }

    while (lock->word != 1) {
        thread_sleep(lock);
    }
}

void write_unlock(rwlock_t * lock)
{
    lock->byte = 0;
    thread_wakeup(lock);
}

void read_lock(rwlock_t * lock, int tid)
{
    while (__sync_fetch_and_or(&lock->byte, 1)) {
        thread_sleep(lock);
    }

    lock->word = (lock->word | 1 << (tid + 8)) & ~1;
}

void read_unlock(rwlock_t * lock, int tid)
{
    __sync_fetch_and_and(&lock->word, ~(1 << (tid + 8)));
    thread_wakeup(lock);
}

#define NREADERS 5
#define NWRITERS 2
#define NTOTAL   (NREADERS + NWRITERS)

struct info {
    int tid;
    rwlock_t * lock;
};

struct info * create_info(int tid, rwlock_t * lock) {
    struct info * info  = malloc(sizeof(struct info));
    info->tid = tid;
    info->lock = lock;
    return info;
}

void * reader_thread(void * arg) {
    struct info * info = (struct info *)arg;
    int i;
    
    for (i = 0; i < 10; i++) {
        printf("thread %d: read lock begin\n", info->tid);
        read_lock(info->lock, info->tid);
       printf("thread %d: read lock inside (%04x)\n", info->tid,
            info->lock->word);
        usleep(1);
        read_unlock(info->lock, info->tid);
        printf("thread %d: read lock done\n", info->tid);
        usleep(1);
    }

    free(info);
    return NULL;
}

void * writer_thread(void * arg) {
    struct info * info = (struct info *)arg;
    int i;
    
    for (i = 0; i < 10; i++) {
        printf("thread %d: write lock begin\n", info->tid);
        write_lock(info->lock);
        printf("thread %d: write lock inside (%04x)\n", info->tid,
            info->lock->word);
        usleep(1);
        write_unlock(info->lock);
        printf("thread %d: write lock done\n", info->tid);
        usleep(1);
    }
    
    free(info);
    return NULL;
}

int main() {
    rwlock_t * lock = rwlock_create();
    pthread_t tid[NTOTAL];
    int i = 0;
    
    for ( ; i < NREADERS; i++) {       
        pthread_create(&tid[i], NULL, reader_thread, create_info(i, lock));
    }
    
    for ( ; i < NTOTAL; i++) {
        pthread_create(&tid[i], NULL, writer_thread, create_info(i, lock));
    }
    
    for (i = 0; i < NTOTAL; i++) {
        pthread_join(tid[i], NULL);
    }
    
    return 0;
}

